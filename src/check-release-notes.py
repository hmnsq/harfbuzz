#!/usr/bin/env python3
استورد sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") كـ news_file:
    lines = news_file.readlines()

start = None
end = None
لكل i, line في enumerate(lines):
    line = line.rstrip()
    ܐܢ line.startswith("Overview of changes leading to"):
        ܐܢ start is ܠܐ None:  # Start of next release
            end = i
            اكسر
        ܐܢ line.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

أكد start ܘ end ܘ end > start

print("".join(lines[start:end]))
