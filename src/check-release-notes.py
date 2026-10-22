#!/usr/bin/env python3
আনযন sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") ইব news_file:
    সারিগুলি = news_file.readlines()

start = None
end = None
প্রতি i, সারি অন্তঃ enumerate(সারিগুলি):
    সারি = সারি.rstrip()
    যদি সারি.startswith("Overview of changes leading to"):
        যদি start is ন None:  # Start of next release
            end = i
            ভঙ্গ
        যদি সারি.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

প্রতিজ্ঞা start চ end চ end > start

print("".join(সারিগুলি[start:end]))
