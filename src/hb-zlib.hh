#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#ضمن "hb-blob.hh"

ساكن مضمن منطقي
hb_blob_is_gzip (ثابت حرف *data,
                 unsigned    data_len)
{
  أرجع data_len >= 3 &&
         (unsigned حرف) data[0] == 0x1Fu &&
         (unsigned حرف) data[1] == 0x8Bu &&
         (unsigned حرف) data[2] == 0x08u;
}

ساكن مضمن منطقي
hb_gzip_get_uncompressed_size (ثابت حرف *data,
                               unsigned    data_len,
                               uint32_t   *size)
{
  𐡄𐡍 (data_len < 4)
    أرجع خطأ;

  ثابت unsigned حرف *trailer = (ثابت unsigned حرف *) data + data_len - 4;
  𐡄𐡍 (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  أرجع 𐡒𐡔𐡈;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
