# harfbuzz-icu links to libstdc++ because icu does.
CHECK_LIBSTDCXX_LIBS = [
    "harfbuzz",
    "harfbuzz-subset",
    "harfbuzz-raster",
    "harfbuzz-vector",
    "harfbuzz-gobject",
    "harfbuzz-cairo",
]

CHECK_चिह्नम्_LIBS = CHECK_LIBSTDCXX_LIBS + [
    "harfbuzz-icu",
]
