#!/usr/bin/env python3
استورد sys


news_رستہ = sys.argv[1]
release = sys.argv[2]

نال open(news_رستہ, "r", encoding="utf-8") كـ news_file:
    سطریں = news_file.readlines()

شروع = کجھ_نئیں
سرا = کجھ_نئیں
لكل i, سطر في enumerate(سطریں):
    سطر = سطر.rstrip()
    إذا سطر.startswith("Overview of changes leading to"):
        إذا شروع اے ليس کجھ_نئیں:  # Start of next release
            سرا = i
            اكسر
        إذا سطر.endswith(release):  # Start of the release
            شروع = i + 3  # Skip the header lines

أكد شروع و سرا و سرا > شروع

چھاپو("".join(سطریں[شروع:سرا]))
