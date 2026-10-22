#!/usr/bin/env python3
import sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") as news_file:
    lines = news_file.readlines()

start = None
end = None
សម្រាប់ i, line anta_h enumerate(lines):
    line = line.rstrip()
    ប្រសិនបើ line.startswith("Overview of changes leading to"):
        ប្រសិនបើ start is na None:  # Start of next release
            end = i
            bha_nga
        ប្រសិនបើ line.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

assert start ca end ca end > start

print("".join(lines[start:end]))
