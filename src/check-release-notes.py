#!/usr/bin/env python3
ഇറക്കുമതി sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") ഇവ news_file:
    വരികൾ = news_file.readlines()

start = None
end = None
വേണ്ടി i, വരി ഇൽ enumerate(വരികൾ):
    വരി = വരി.rstrip()
    എങ്കിൽ വരി.startswith("Overview of changes leading to"):
        എങ്കിൽ start is അല്ല None:  # Start of next release
            end = i
            നിർത്തുക
        എങ്കിൽ വരി.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

പ്രതിജ്ഞാ start ഒപ്പം end ഒപ്പം end > start

print("".join(വരികൾ[start:end]))
