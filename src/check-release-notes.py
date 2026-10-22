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
    𐤀𐤌 line.startswith("Overview of changes leading to"):
        𐤀𐤌 start is 𐤋𐤀 None:  # Start of next release
            end = i
            اكسر
        𐤀𐤌 line.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

أكد start 𐤅 end 𐤅 end > start

print("".join(lines[start:end]))
