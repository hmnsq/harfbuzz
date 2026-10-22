#!/usr/bin/env python3
واردکن sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") بهعنوان news_file:
    سطرها = news_file.readlines()

start = None
end = None
برای i, سطر در enumerate(سطرها):
    سطر = سطر.rstrip()
    اگر سطر.startswith("Overview of changes leading to"):
        اگر start is نه None:  # Start of next release
            end = i
            بشکن
        اگر سطر.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

أكد start و end و end > start

print("".join(سطرها[start:end]))
