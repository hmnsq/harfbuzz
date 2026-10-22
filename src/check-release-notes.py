#!/usr/bin/env python3
استورد sys


news_لار = sys.argv[1]
release = sys.argv[2]

سره open(news_لار, "r", encoding="utf-8") كـ news_file:
    کرښې = news_file.readlines()

پیل = هیڅ
پای_ټکی = هیڅ
لكل i, کرښه في enumerate(کرښې):
    کرښه = کرښه.rstrip()
    إذا کرښه.startswith("Overview of changes leading to"):
        إذا پیل دی ليس هیڅ:  # Start of next release
            پای_ټکی = i
            اكسر
        إذا کرښه.endswith(release):  # Start of the release
            پیل = i + 3  # Skip the header lines

أكد پیل و پای_ټکی و پای_ټکی > پیل

چاپ("".join(کرښې[پیل:پای_ټکی]))
