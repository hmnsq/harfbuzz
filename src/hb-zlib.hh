#اگر_تعریف_نشده HB_ZLIB_HH
#تعریف_کن HB_ZLIB_HH

#ضمن "hb-blob.hh"

ساكن مضمن منطقي
hb_blob_is_gzip (ثابت حرف *داده,
                 بیعلامت    داده_len)
{
  أرجع داده_len >= 3 &&
         (بیعلامت حرف) داده[0] == 0x1Fu &&
         (بیعلامت حرف) داده[1] == 0x8Bu &&
         (بیعلامت حرف) داده[2] == 0x08u;
}

ساكن مضمن منطقي
hb_gzip_get_uncompressed_اندازه (ثابت حرف *داده,
                               بیعلامت    داده_len,
                               uint32_t   *اندازه)
{
  إذا (داده_len < 4)
    أرجع خطأ;

  ثابت بیعلامت حرف *trailer = (ثابت بیعلامت حرف *) داده + داده_len - 4;
  إذا (اندازه)
    *اندازه = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  أرجع درست;
}

HB_داخلی hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         بیعلامت   max_output_len);

#پایان_اگر /* HB_ZLIB_HH */
