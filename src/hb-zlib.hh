#جيڪڏهن_وصف_نه_ٿيل HB_ZLIB_HH
#وصف_ڪريو HB_ZLIB_HH

#ضمن "hb-blob.hh"

ساكن مضمن منطقي
hb_blob_is_gzip (ثابت حرف *معلومات,
                 بنانشان    معلومات_len)
{
  أرجع معلومات_len >= 3 &&
         (بنانشان حرف) معلومات[0] == 0x1Fu &&
         (بنانشان حرف) معلومات[1] == 0x8Bu &&
         (بنانشان حرف) معلومات[2] == 0x08u;
}

ساكن مضمن منطقي
hb_gzip_get_uncompressed_سائيز (ثابت حرف *معلومات,
                               بنانشان    معلومات_len,
                               uint32_t   *سائيز)
{
  إذا (معلومات_len < 4)
    أرجع خطأ;

  ثابت بنانشان حرف *trailer = (ثابت بنانشان حرف *) معلومات + معلومات_len - 4;
  إذا (سائيز)
    *سائيز = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  أرجع سچ;
}

HB_اندروني hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         بنانشان   max_output_len);

#جيڪڏهن_پورو /* HB_ZLIB_HH */
