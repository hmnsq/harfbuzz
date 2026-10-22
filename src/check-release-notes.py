#!/usr/bin/env python3
આયાત sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") ઇવ news_file:
    લીટીઓ = news_file.readlines()

start = None
end = None
માટે i, લીટી માં enumerate(લીટીઓ):
    લીટી = લીટી.rstrip()
    જો લીટી.startswith("Overview of changes leading to"):
        જો start is નહીં None:  # Start of next release
            end = i
            તોડો
        જો લીટી.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

પ્રતિજ્ઞા start અને end અને end > start

print("".join(લીટીઓ[start:end]))
