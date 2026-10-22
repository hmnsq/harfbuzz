# harfbuzz-icu links to libstdc++ because icu does.
CHECK_LIBSTDCXX_LIBS = [
    "harfbuzz",
    "harfbuzz-subset",
    "harfbuzz-raster",
    "harfbuzz-vector",
    "harfbuzz-gobject",
    "harfbuzz-cairo",
]

CHECK_చిహ్నం_LIBS = CHECK_LIBSTDCXX_LIBS + [
    "harfbuzz-icu",
]
