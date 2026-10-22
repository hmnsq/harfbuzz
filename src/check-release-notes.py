#!/usr/bin/env python3
استورد sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") كـ news_file:
    أسطر = news_file.readlines()

start = None
end = None
لكل i, سطر في enumerate(أسطر):
    سطر = سطر.rstrip()
    إذا سطر.startswith("Overview of changes leading to"):
        إذا start is ليس None:  # Start of next release
            end = i
            اكسر
        إذا سطر.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

أكد start و end و end > start

print("".join(أسطر[start:end]))
