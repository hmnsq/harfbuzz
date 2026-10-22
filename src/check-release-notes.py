#!/usr/bin/env python3
आनयन sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") इव news_file:
    पङ्क्तयः = news_file.readlines()

start = None
end = None
अत्थाय i, पङ्क्तिः अन्तः enumerate(पङ्क्तयः):
    पङ्क्तिः = पङ्क्तिः.rstrip()
    सचे पङ्क्तिः.startswith("Overview of changes leading to"):
        सचे start is न None:  # Start of next release
            end = i
            भङ्ग
        सचे पङ्क्तिः.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

प्रतिज्ञा start च end च end > start

print("".join(पङ्क्तयः[start:end]))
