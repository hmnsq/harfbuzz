#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#شامل "hb-blob.hh"

ایستا درونخطی منطقی
hb_blob_is_gzip (ثابت نویسه *data,
                 unsigned    data_len)
{
  بازگشت data_len >= 3 &&
         (unsigned نویسه) data[0] == 0x1Fu &&
         (unsigned نویسه) data[1] == 0x8Bu &&
         (unsigned نویسه) data[2] == 0x08u;
}

ایستا درونخطی منطقی
hb_gzip_get_uncompressed_size (ثابت نویسه *data,
                               unsigned    data_len,
                               uint32_t   *size)
{
  اگر (data_len < 4)
    بازگشت نادرست;

  ثابت unsigned نویسه *trailer = (ثابت unsigned نویسه *) data + data_len - 4;
  اگر (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  بازگشت درست;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
