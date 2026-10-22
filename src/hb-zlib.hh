#که_نه_تعریف_شوی HB_ZLIB_HH
#تعریف_کړه HB_ZLIB_HH

#ضمن "hb-blob.hh"

ساكن مضمن منطقي
hb_blob_is_gzip (ثابت حرف *معلومات,
                 بینښه    معلومات_len)
{
  أرجع معلومات_len >= 3 &&
         (بینښه حرف) معلومات[0] == 0x1Fu &&
         (بینښه حرف) معلومات[1] == 0x8Bu &&
         (بینښه حرف) معلومات[2] == 0x08u;
}

ساكن مضمن منطقي
hb_gzip_get_uncompressed_اندازه (ثابت حرف *معلومات,
                               بینښه    معلومات_len,
                               uint32_t   *اندازه)
{
  إذا (معلومات_len < 4)
    أرجع خطأ;

  ثابت بینښه حرف *trailer = (ثابت بینښه حرف *) معلومات + معلومات_len - 4;
  إذا (اندازه)
    *اندازه = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  أرجع ریښتیا;
}

HB_دننی hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         بینښه   max_output_len);

#د_که_پای /* HB_ZLIB_HH */
