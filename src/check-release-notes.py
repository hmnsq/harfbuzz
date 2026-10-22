#!/usr/bin/env python3
استورد sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") كـ news_file:
    هێڵەکان = news_file.readlines()

start = None
end = None
لكل i, هێڵ في enumerate(هێڵەکان):
    هێڵ = هێڵ.rstrip()
    إذا هێڵ.startswith("Overview of changes leading to"):
        إذا start is ليس None:  # Start of next release
            end = i
            اكسر
        إذا هێڵ.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

أكد start و end و end > start

print("".join(هێڵەکان[start:end]))
