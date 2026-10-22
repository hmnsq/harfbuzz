#!/usr/bin/env python3
आनयन sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") इव news_file:
    पङ्क्तिहरू = news_file.readlines()

start = None
end = None
लागि i, पङ्क्ति अन्तः enumerate(पङ्क्तिहरू):
    पङ्क्ति = पङ्क्ति.rstrip()
    यदि पङ्क्ति.startswith("Overview of changes leading to"):
        यदि start is होइन None:  # Start of next release
            end = i
            रोक
        यदि पङ्क्ति.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

प्रतिज्ञा start र end र end > start

print("".join(पङ्क्तिहरू[start:end]))
