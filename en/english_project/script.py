# -*- coding: utf-8 -*-
import re
import emoji
from dataclasses import dataclass, field, asdict
from pathlib import Path
import os
import sys
import json
from sentence_transformers import SentenceTransformer, util
import googleapiclient.discovery
from googleapiclient.errors import HttpError

from config import *
from glaze_comments import glaze_comments
video_ids_file = "video_ids"
flagged_comment_path = "flagged_comments"
cached_comment_path = "cached_comments"
output_ext = "json"
csv_output_path = "data.csv"

if not os.path.isfile("video_ids"):
    print(f"Error: {video_ids_file} not found.")
    sys.exit(1)

with open(video_ids_file) as f:
    video_ids = [line.strip() for line in f if line.strip()]

if len(video_ids) == 0:
    print(f"{video_ids_file} doesn't contain any video id's.")
    sys.exit(1)

os.makedirs(cached_comment_path, exist_ok=True)
os.makedirs(flagged_comment_path, exist_ok=True)

@dataclass
class Comment:
    text: str
    like_count: int
    reply_count: int
    emoji_prct: float = 0.0
    glaze_score: float = 0.0
    glaze_match: str = ""
    flags: str = "0000"


def has_repetitive_pattern(comment: str) -> bool:
    c = emoji.replace_emoji(comment.lower(), '')
    # Detect repeated letters: "aaa", "woooow"
    if re.search(r'(.)\1{3,}', c):
        return True
    # Detect repeated small substrings: "lololol", "abcabcabc"
    if re.search(r'(.{1,6})\1\1', c):
        return True
    return False


def emoji_count(comment: str) -> int:
    return emoji.emoji_count(comment)


def fetchYtComments(api_key, comment_query_count, video_id) -> list[Comment]:
    os.environ["OAUTHLIB_INSECURE_TRANSPORT"] = "1"

    api_service_name = "youtube"
    api_version = "v3"
    count = 0

    youtube = googleapiclient.discovery.build(
        api_service_name, api_version, developerKey = api_key)

    npt = ""
    nextPage = True

    fetched_comments = []
    cache_path = Path(f"{cached_comment_path}/cache_{video_id}.{output_ext}")
    skip_fetch = False

    if ENABLE_CACHE:
        data_list = []

        if cache_path.exists():
            with cache_path.open("r") as f:
                try:
                    if SKIP_CACHED_VIDEOS:
                        print("    SKIP_CACHED_VIDEOS enabled: skipping video")
                        return []

                    data_list = json.load(f)
                    skip_fetch = True
                    print("    Reading data from cache file")

                    for page in data_list:
                        items = page.get("items", [])
                        for item in items:
                            snippet = item.get("snippet", {})
                            top = snippet.get("topLevelComment", {}).get("snippet", {})
                            text = top.get("textDisplay", "")

                            text = text.replace("\n", " ").strip()
                            if text.strip():
                                like_count = item["snippet"]["topLevelComment"]["snippet"]["likeCount"]
                                reply_count = item["snippet"]["totalReplyCount"]
                                fetched_comments.append(Comment(text, like_count, reply_count))
                except json.JSONDecodeError as e:
                    print("    Cache file is empty or contains invalid JSON: starting fetch")
                    print(e)
        else:
            print("    Cache file does not exist: starting fetch")
    else:
        print("    Cache disabled: starting fetch")

    if skip_fetch:
        return fetched_comments

    with cache_path.open("w") as f:
        f.write("[\n")

        while True:
                try:
                    if nextPage and count < comment_query_count:
                        response = youtube.commentThreads().list(
                            part="snippet",
                            videoId=video_id,
                            maxResults=100,
                            pageToken=npt,
                            order="time"
                        ).execute()
                    else:
                        f.write("{}\n]\n")
                        break

                    json.dump(response, f, ensure_ascii=False, indent=2)
                    f.write(",\n")

                    if "nextPageToken" not in response or not response["nextPageToken"]:
                        nextPage = False
                    else:
                        npt = response["nextPageToken"]

                    count += len(response["items"])

                    for item in response["items"]:
                        text = item["snippet"]["topLevelComment"]["snippet"]["textDisplay"].replace("\n", " ").strip()
                        like_count = item["snippet"]["topLevelComment"]["snippet"]["likeCount"]
                        reply_count = item["snippet"]["totalReplyCount"]
                        comment = Comment(text, like_count, reply_count)
                        fetched_comments.append(comment)

                except HttpError as e:
                    if e.resp.status == 400:
                        print(f"    Invalid video ID: {video_id}")
                    elif e.resp.status == 403:
                        print("    Access denied / quota exceeded")
                    elif e.resp.status == 404:
                        print(f"    Video {video_id} could not found")
                    else:
                        print(f"    HTTP error {e.resp.status}: {e.content.decode()}")
                    break
    
    return fetched_comments


def flag_comments(comments: list[Comment], glaze_comments, comp_model, min_comp_score, min_emoji_perc, min_cmt_len):
    # Compute embeddings
    texts = [c.text for c in comments]
    embeddings1 = comp_model.encode(texts, convert_to_tensor=True)
    embeddings2 = comp_model.encode(glaze_comments, convert_to_tensor=True)

    # Compute cosine similarity matrix
    similarities = util.cos_sim(embeddings1, embeddings2)

    for idx_i, c in enumerate(comments):
        text = texts[idx_i]
        new_flags = ""

        # Check for glaze
        gs = 0.0
        gm = ""
        for idx_j, sentence2 in enumerate(glaze_comments):
            score = similarities[idx_i][idx_j].item()
            if score >= min_comp_score:
                gs = score
                gm = sentence2
                break

        cmt_emoji_count = emoji_count(text)
        c.emoji_prct = cmt_emoji_count / len(text)

        new_flags += "1" if cmt_emoji_count / len(text) >= min_emoji_perc else "0"
        new_flags += "1" if len(text) - cmt_emoji_count < min_cmt_len else "0"
        new_flags += "1" if has_repetitive_pattern(text) else "0"
        new_flags += "1" if gs >= min_comp_score else "0"

        if gs == 0.0:
            delattr(c, "glaze_score")
            delattr(c, "glaze_match")
        else:
            c.glaze_score = gs
            c.glaze_match = gm

        c.flags = new_flags


def main():
    model = SentenceTransformer("all-MiniLM-L6-v2")

    data_output = Path(csv_output_path)
    processed_count = 0
    count = 0

    # Write CSV header row
    if FORMAT_CSV and ((os.path.exists(data_output) and os.path.getsize(data_output) == 0) or not os.path.exists(data_output)):
        with data_output.open("w") as fo:
            fo.write("VideoID, TotalComments, GoodComments, TrashComments, Glaze, ManyEmojis, Repetitive, Short, GoodLikeAvg, FlaggedLikeAvg, GoodReplyAvg, FlaggedReplyAvg\n")

    with data_output.open("a") as fo:
        for VIDEO_ID in video_ids:
            count += 1
            progression = f"{count}/{len(video_ids)}"
            output_file = Path(f"{flagged_comment_path}/data_{VIDEO_ID}.{output_ext}")

            if SKIP_PROCESSED_VIDEOS and os.path.exists(output_file):
                print(f"{progression} Skipping video {VIDEO_ID}: already processed")
                continue

            print(f"{progression} Processing comments for video {VIDEO_ID}")
            comments = fetchYtComments(DEVELOPER_KEY, COMMENT_QUERY_COUNT, VIDEO_ID)

            print(f"    {len(comments)} comments were loaded")
            if len(comments) == 0:
                continue
            
            processed_count += 1
            flag_comments(comments, glaze_comments, model, MIN_GLAZE_SCORE, MIN_EMOJI_PERC, MIN_CMT_LEN)

            total = len(comments)
            flagged = 0
            repetitive = 0
            short = 0
            many_emojis = 0
            glazy = 0
            flagged_avg_likes = 0
            flagged_avg_replies = 0
            normal_like_avg = 0
            normal_reply_avg = 0

            for c in comments:
                flagged_cmt = False
                if c.flags[0] == "1":
                    many_emojis += 1
                    flagged_cmt = True
                if c.flags[1] == "1":
                    short += 1
                    flagged_cmt = True
                if c.flags[2] == "1":
                    repetitive += 1
                    flagged_cmt = True
                if c.flags[3] == "1":
                    glazy += 1
                    flagged_cmt = True

                if flagged_cmt:
                    flagged_avg_likes += c.like_count
                    flagged_avg_replies += c.reply_count
                    flagged += 1
                else:
                    normal_like_avg += c.like_count
                    normal_reply_avg += c.reply_count
            
            if flagged:
                flagged_avg_likes /= flagged
                flagged_avg_replies /= flagged
            if flagged != total:
                normal_like_avg /= (total - flagged)
                normal_reply_avg /= (total - flagged)
            
            if FORMAT_CSV:
                output_string = f"{VIDEO_ID}, {total}, {total - flagged}, {flagged}, {glazy}, {many_emojis}, {repetitive}, {short}, {normal_like_avg}, {flagged_avg_likes}, {normal_reply_avg}, {flagged_avg_replies}\n"
                fo.write(output_string)
            
            video_data = {
                "video_id": VIDEO_ID,
                "comment_count": str(len(comments)),
                "flagged": str(flagged),
                "many_emojis": str(many_emojis),
                "short": str(short),
                "repetetive": str(repetitive),
                "glazy": str(glazy),
            }

            json_object = {
                **video_data,
                "comments": [c.__dict__ for c in comments]
            }

            print(f"    {flagged}/{len(comments)} comments were flagged")
            
            print(f"    Writing flagged comments to {output_file}")
            with output_file.open("w") as f:
                json.dump(json_object, f, ensure_ascii=False, indent=2)

    print(f"Processed {processed_count} video comments")


if __name__ == "__main__":
    print("Starting Program")
    main()

