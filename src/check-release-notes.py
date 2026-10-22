#!/usr/bin/env python3
आयात sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") as news_file:
    पंक्तियाँ = news_file.readlines()

शुरू = None
end = None
क्रम i, पंक्ति in enumerate(पंक्तियाँ):
    पंक्ति = पंक्ति.rstrip()
    अगर पंक्ति.startswith("Overview of changes leading to"):
        अगर शुरू is not None:  # Start of next release
            end = i
            अवरोध
        अगर पंक्ति.endswith(release):  # Start of the release
            शुरू = i + 3  # Skip the header lines

assert शुरू and end and end > शुरू

print("".मेल(पंक्तियाँ[शुरू:end]))
