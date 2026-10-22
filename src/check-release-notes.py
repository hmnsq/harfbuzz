#!/usr/bin/env python3
ਆਯਾਤ sys


news_ਰਸਤਾ = sys.argv[1]
release = sys.argv[2]

ਨਾਲ open(news_ਰਸਤਾ, "r", encoding="utf-8") ਇਵ news_file:
    ਸਤਰਾਂ = news_file.readlines()

ਸ਼ੁਰੂ = ਕੁਝ_ਨਹੀਂ
ਸਿਰਾ = ਕੁਝ_ਨਹੀਂ
ਲਈ i, ਸਤਰ ਵਿੱਚ enumerate(ਸਤਰਾਂ):
    ਸਤਰ = ਸਤਰ.rstrip()
    ਜੇ ਸਤਰ.startswith("Overview of changes leading to"):
        ਜੇ ਸ਼ੁਰੂ ਹੈ ਨਹੀਂ ਕੁਝ_ਨਹੀਂ:  # Start of next release
            ਸਿਰਾ = i
            ਰੋਕੋ
        ਜੇ ਸਤਰ.endswith(release):  # Start of the release
            ਸ਼ੁਰੂ = i + 3  # Skip the header lines

ਪ੍ਰਤਿਜ੍ਞਾ ਸ਼ੁਰੂ ਅਤੇ ਸਿਰਾ ਅਤੇ ਸਿਰਾ > ਸ਼ੁਰੂ

ਛਾਪੋ("".join(ਸਤਰਾਂ[ਸ਼ੁਰੂ:ਸਿਰਾ]))
