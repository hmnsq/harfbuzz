#!/usr/bin/env python3
درآمد sys


news_راستہ = sys.argv[1]
release = sys.argv[2]

ساتھ open(news_راستہ, "r", encoding="utf-8") بطور news_file:
    سطریں = news_file.readlines()

آغاز = کچھ_نہیں
انتہا = کچھ_نہیں
برائے i, سطر میں enumerate(سطریں):
    سطر = سطر.rstrip()
    اگر سطر.startswith("Overview of changes leading to"):
        اگر آغاز ہے نہیں کچھ_نہیں:  # Start of next release
            انتہا = i
            توڑو
        اگر سطر.endswith(release):  # Start of the release
            آغاز = i + 3  # Skip the header lines

أكد آغاز اور انتہا اور انتہا > آغاز

چھاپو("".join(سطریں[آغاز:انتہا]))
