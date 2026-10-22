#!/usr/bin/env python3
আনযন sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") ইৱ news_file:
    সাৰিগুলি = news_file.readlines()

start = None
end = None
বাবে i, সাৰি অন্তঃ enumerate(সাৰিগুলি):
    সাৰি = সাৰি.rstrip()
    যদি সাৰি.startswith("Overview of changes leading to"):
        যদি start is নহয় None:  # Start of next release
            end = i
            ভঙ্গ
        যদি সাৰি.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

প্রতিজ্ঞা start আৰু end আৰু end > start

print("".join(সাৰিগুলি[start:end]))
