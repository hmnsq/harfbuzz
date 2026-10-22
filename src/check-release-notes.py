#!/usr/bin/env python3
இறக்குமதி sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") இவ news_file:
    வரிகள் = news_file.readlines()

start = None
end = None
க்கு i, வரி உள் enumerate(வரிகள்):
    வரி = வரி.rstrip()
    எனில் வரி.startswith("Overview of changes leading to"):
        எனில் start is இல்லை None:  # Start of next release
            end = i
            நிறுத்து
        எனில் வரி.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

ப்ரதிஜ்ஞா start மற்றும் end மற்றும் end > start

print("".join(வரிகள்[start:end]))
