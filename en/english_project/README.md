### This is a group project me and my buddies did during our first semester at VU.
* We fetched comments from YouTube Shorts, the list of which is in ```video_ids```, and flagged them as "trash" according to our defined flags.
* ```glaze_comments.py``` - we handpicked these to compare and detect similar comments using the sentence-transfomers module.
* ```collect.py``` gathers all the .json files in flagged_comments/ and glues them together into a big .json file ```all_comments.json```

### Setup
* Enter your Google YouTube API key in ```config.py```, adjust other settings as necessary.
* Put the video id's in the ```video_ids``` file separated by newlines.

### Notes
* The four numbers(0 or 1) in the flags field indicate whether the comment has the many_emojis, short, repetitive and glazy flags respectively.
