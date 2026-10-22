#!/usr/bin/env python3
استورد sys


news_رستو = sys.argv[1]
release = sys.argv[2]

سان open(news_رستو, "r", encoding="utf-8") كـ news_file:
    سٽون = news_file.readlines()

شروعات = ڪجهه_نه
پڇاڙي = ڪجهه_نه
لكل i, سٽ في enumerate(سٽون):
    سٽ = سٽ.rstrip()
    إذا سٽ.startswith("Overview of changes leading to"):
        إذا شروعات آهي ليس ڪجهه_نه:  # Start of next release
            پڇاڙي = i
            اكسر
        إذا سٽ.endswith(release):  # Start of the release
            شروعات = i + 3  # Skip the header lines

أكد شروعات و پڇاڙي و پڇاڙي > شروعات

ڇاپيو("".join(سٽون[شروعات:پڇاڙي]))
