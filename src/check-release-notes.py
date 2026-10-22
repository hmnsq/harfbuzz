#!/usr/bin/env python3
ଆମଦାନୀ sys


news_path = sys.argv[1]
release = sys.argv[2]

with open(news_path, "r", encoding="utf-8") ଇଵ news_file:
    ଧାଡ଼ିଗୁଡ଼ିକ = news_file.readlines()

start = None
end = None
ପାଇଁ i, ଧାଡ଼ି ଭିତରେ enumerate(ଧାଡ଼ିଗୁଡ଼ିକ):
    ଧାଡ଼ି = ଧାଡ଼ି.rstrip()
    ଯଦି ଧାଡ଼ି.startswith("Overview of changes leading to"):
        ଯଦି start is ନୁହେଁ None:  # Start of next release
            end = i
            ଭାଙ୍ଗ
        ଯଦି ଧାଡ଼ି.endswith(release):  # Start of the release
            start = i + 3  # Skip the header lines

ପ୍ରତିଜ୍ଞା start ଏବଂ end ଏବଂ end > start

print("".join(ଧାଡ଼ିଗୁଡ଼ିକ[start:end]))
