#!/usr/bin/env python3
ಆನಯನ sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") ಇವ news_file:
    ಸಾಲುಗಳು = news_file.readlines()

start = None
end = None
ಪ್ರತಿ i, ಸಾಲು ಅನ್ತಃ enumerate(ಸಾಲುಗಳು):
    ಸಾಲು = ಸಾಲು.rstrip()
    ಯದಿ ಸಾಲು.startswith("Overview of changes leading to"):
        ಯದಿ start is ನ None:  # Start of next release
            end = i
            ಭಙ್ಗ
        ಯದಿ ಸಾಲು.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

ಪ್ರತಿಜ್ಞಾ start ಚ end ಚ end > start

print("".join(ಸಾಲುಗಳು[start:end]))
