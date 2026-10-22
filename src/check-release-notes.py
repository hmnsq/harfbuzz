#!/usr/bin/env python3
استورد sys


news_مسیر = sys.argv[1]
release = sys.argv[2]

با open(news_مسیر, "r", encoding="utf-8") كـ news_file:
    سطرها = news_file.readlines()

شروع = هیچ
انتها = هیچ
لكل i, سطر في enumerate(سطرها):
    سطر = سطر.rstrip()
    إذا سطر.startswith("Overview of changes leading to"):
        إذا شروع است ليس هیچ:  # Start of next release
            انتها = i
            اكسر
        إذا سطر.endswith(release):  # Start of the release
            شروع = i + 3  # Skip the header lines

أكد شروع و انتها و انتها > شروع

چاپ("".join(سطرها[شروع:انتها]))
