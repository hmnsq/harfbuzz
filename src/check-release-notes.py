#!/usr/bin/env python3
आनयन sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") इव news_file:
    ओळी = news_file.readlines()

start = None
end = None
प्रति i, ओळ अन्तः enumerate(ओळी):
    ओळ = ओळ.rstrip()
    यदि ओळ.startswith("Overview of changes leading to"):
        यदि start is न None:  # Start of next release
            end = i
            भङ्ग
        यदि ओळ.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

प्रतिज्ञा start च end च end > start

print("".join(ओळी[start:end]))
