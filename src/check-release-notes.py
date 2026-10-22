#!/usr/bin/env python3
आनयन sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") इव news_file:
    पंक्तियाँ = news_file.readlines()

start = None
end = None
प्रति i, पंक्ति अन्तः enumerate(पंक्तियाँ):
    पंक्ति = पंक्ति.rstrip()
    यदि पंक्ति.startswith("Overview of changes leading to"):
        यदि start is न None:  # Start of next release
            end = i
            भङ्ग
        यदि पंक्ति.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

प्रतिज्ञा start च end च end > start

print("".join(पंक्तियाँ[start:end]))
