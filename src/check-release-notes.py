#!/usr/bin/env python3
import sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") as news_file:
    मगा_लिन्या = news_file.readlines()

start = None
end = None
for i, लिन्या in enumerate(मगा_लिन्या):
    लिन्या = लिन्या.rstrip()
    if लिन्या.startswith("Overview of changes leading to"):
        if start is not None:  # Start of next release
            end = i
            break
        if लिन्या.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

assert start and end and end > start

print("".join(मगा_लिन्या[start:end]))
