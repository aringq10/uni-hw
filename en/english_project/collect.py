import os
import glob
import json

def append_json_files(input_dir: str, output_file: str, filename_pattern: str):
    if not os.path.isdir(input_dir):
        raise ValueError(f"'{input_dir}' is not a valid directory.")

    pattern = os.path.join(input_dir, filename_pattern)
    json_files = glob.glob(pattern)

    if not json_files:
        raise ValueError(f"No files found matching pattern '{pattern}'.")

    appended_count = 0
    all_objects = []

    for jf in json_files:
        try:
            with open(jf, "r", encoding="utf-8") as f:
                data = json.load(f)
                all_objects.append(data)
                appended_count += 1
        except OSError as e:
            print(f"Warning: Could not read file '{jf}': {e}")

    try:
        with open(output_file, "w", encoding="utf-8") as out:
            json.dump(all_objects, out, indent=2, ensure_ascii=False)
    except OSError as e:
        raise OSError(f"Could not open or write to output file '{output_file}': {e}")

    return appended_count

count = append_json_files("./flagged_comments", "all_comments.json", "data_*.json")

print(f"collected {count} video .json files")
