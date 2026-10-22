#!/usr/bin/env python3
దిగుమతి sys


news_మార్గం = sys.argv[1]
release = sys.argv[2]

తోడుగా open(news_మార్గం, "r", encoding="utf-8") ఇవ news_file:
    పంక్తులు = news_file.readlines()

ప్రారంభం = ఏమీలేదు
చివర = ఏమీలేదు
కొరకు i, పంక్తి లో enumerate(పంక్తులు):
    పంక్తి = పంక్తి.rstrip()
    ఒకవేళ పంక్తి.startswith("Overview of changes leading to"):
        ఒకవేళ ప్రారంభం అయిన కాదు ఏమీలేదు:  # Start of next release
            చివర = i
            ఆపు
        ఒకవేళ పంక్తి.endswith(release):  # Start of the release
            ప్రారంభం = i + 3  # Skip the header lines

ప్రతిజ్ఞా ప్రారంభం మరియు చివర మరియు చివర > ప్రారంభం

ముద్రించు("".join(పంక్తులు[ప్రారంభం:చివర]))
