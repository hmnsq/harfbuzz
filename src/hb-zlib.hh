#جے_بنا_تعریف HB_ZLIB_HH
#تعریف_کرو HB_ZLIB_HH

#ضمن "hb-blob.hh"

ساكن مضمن منطقي
hb_blob_is_gzip (ثابت حرف *انکڑے,
                 بنانشانی    انکڑے_len)
{
  أرجع انکڑے_len >= 3 &&
         (بنانشانی حرف) انکڑے[0] == 0x1Fu &&
         (بنانشانی حرف) انکڑے[1] == 0x8Bu &&
         (بنانشانی حرف) انکڑے[2] == 0x08u;
}

ساكن مضمن منطقي
hb_gzip_get_uncompressed_ناپ (ثابت حرف *انکڑے,
                               بنانشانی    انکڑے_len,
                               uint32_t   *ناپ)
{
  إذا (انکڑے_len < 4)
    أرجع خطأ;

  ثابت بنانشانی حرف *trailer = (ثابت بنانشانی حرف *) انکڑے + انکڑے_len - 4;
  إذا (ناپ)
    *ناپ = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  أرجع سچ;
}

HB_اندرونی hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         بنانشانی   max_output_len);

#جے_انت /* HB_ZLIB_HH */
