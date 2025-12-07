# -*- coding: utf-8 -*-

from config import DEVELOPER_KEY, COMMENT_QUERY_COUNT, MIN_GLAZE_SCORE, MIN_EMOJI_PERC, MIN_CMT_LEN, ENABLE_CACHE
from glaze_comments import glaze_comments
video_ids_file = "video_ids"
flagged_comment_path = "flagged_comments"
cached_comment_path = "cached_comments"
output_ext = "json"

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
class CommentFlags:
    glaze_score: float = 0
    glaze_match: str = ""
    many_emojis: bool = False
    emoji_prct: float = 0.0
    emoji_count: int = 0
    repetitive: bool = False
    short: bool = False

@dataclass
class Comment:
    text: str
    flags: CommentFlags = field(default_factory=CommentFlags)



def has_repetitive_pattern(comment: str) -> bool:
    c = comment.lower()
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
                                fetched_comments.append(Comment(text))
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
                        comment = Comment(text)
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

    for idx_i, _ in enumerate(comments):
        text = texts[idx_i]
        # Check for other flags
        if has_repetitive_pattern(text):
            comments[idx_i].flags.repetitive = True

        cmt_emoji_count = emoji_count(text)
        comments[idx_i].flags.emoji_prct = cmt_emoji_count / len(text)
        comments[idx_i].flags.emoji_count = cmt_emoji_count
        if cmt_emoji_count / len(text) >= min_emoji_perc:
            comments[idx_i].flags.many_emojis = True

        if len(text) - cmt_emoji_count < min_cmt_len:
            comments[idx_i].flags.short = True
        
        # Check for glaze
        for idx_j, sentence2 in enumerate(glaze_comments):
            score = similarities[idx_i][idx_j].item()
            if score >= min_comp_score:
                comments[idx_i].flags.glaze_score = score
                comments[idx_i].flags.glaze_match = sentence2
                break


def main():
    model = SentenceTransformer("all-MiniLM-L6-v2")

    for VIDEO_ID in video_ids:
        print(f"Processing comments for video {VIDEO_ID}")
        comments = fetchYtComments(DEVELOPER_KEY, COMMENT_QUERY_COUNT, VIDEO_ID)

        print(f"    {len(comments)} comments were loaded")
        if len(comments) == 0:
            continue

        flag_comments(comments, glaze_comments, model, MIN_GLAZE_SCORE, MIN_EMOJI_PERC, MIN_CMT_LEN)

        flagged = 0
        repetitive = 0
        short = 0
        many_emojis = 0
        glazy = 0

        for c in comments:
            flagged_cmt = False
            if c.flags.repetitive:
                repetitive += 1
                flagged_cmt = True
            if c.flags.short:
                short += 1
                flagged_cmt = True
            if c.flags.many_emojis:
                many_emojis += 1
                flagged_cmt = True
            if c.flags.glaze_score >= MIN_GLAZE_SCORE:
                glazy += 1
                flagged_cmt = True
            if flagged_cmt:
                flagged += 1
        
        video_data = {
            "video_id": VIDEO_ID,
            "comment_count": str(len(comments)),
            "flagged": str(flagged),
            "repetetive": str(repetitive),
            "short": str(short),
            "many_emojis": str(many_emojis),
            "glazy": str(glazy),
        }

        json_object = {
            **video_data,
            "comments": [asdict(c) for c in comments]
        }

        print(f"    {flagged}/{len(comments)} comments were flagged")
        
        output_file = Path(f"{flagged_comment_path}/data_{VIDEO_ID}.{output_ext}")
        print(f"    Writing flagged comments to {output_file}")
        with output_file.open("w") as f:
            json.dump(json_object, f, ensure_ascii=False, indent=2)


if __name__ == "__main__":
    print("Starting Program")
    main()

