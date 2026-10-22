#!/usr/bin/env python3
आनयन sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") इव news_file:
    पङ्क्तयः = news_file.readlines()

start = None
end = None
प्रति i, पङ्क्तिः अन्तः enumerate(पङ्क्तयः):
    पङ्क्तिः = पङ्क्तिः.rstrip()
    जइ पङ्क्तिः.startswith("Overview of changes leading to"):
        जइ start is ण None:  # Start of next release
            end = i
            भङ्ग
        जइ पङ्क्तिः.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

प्रतिज्ञा start य end य end > start

print("".join(पङ्क्तयः[start:end]))
