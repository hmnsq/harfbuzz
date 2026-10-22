#!/usr/bin/env python3
आयात sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") इव news_file:
    ओळी = news_file.readlines()

start = None
end = None
साठी i, ओळ मध्ये enumerate(ओळी):
    ओळ = ओळ.rstrip()
    जर ओळ.startswith("Overview of changes leading to"):
        जर start is नाही None:  # Start of next release
            end = i
            थांबा
        जर ओळ.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

प्रतिज्ञा start आणि end आणि end > start

print("".join(ओळी[start:end]))
