#!/usr/bin/env python3
import sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") as news_file:
    lines = news_file.readlines()

start = None
end = None
ために i, line in enumerate(lines):
    line = line.rstrip()
    もし line.startswith("Overview of changes leading to"):
        もし start is not None:  # Start of next release
            end = i
            break
        もし line.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

assert start and end and end > start

print("".join(lines[start:end]))
