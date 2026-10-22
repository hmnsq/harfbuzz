#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#অন্তর্ভাব "hb-blob.hh"

স্থির অন্তঃস্থ তার্কিক
hb_blob_is_gzip (ধ্রুব বর্ণ *data,
                 unsigned    data_len)
{
  প্রত্যাগম data_len >= 3 &&
         (unsigned বর্ণ) data[0] == 0x1Fu &&
         (unsigned বর্ণ) data[1] == 0x8Bu &&
         (unsigned বর্ণ) data[2] == 0x08u;
}

স্থির অন্তঃস্থ তার্কিক
hb_gzip_get_uncompressed_size (ধ্রুব বর্ণ *data,
                               unsigned    data_len,
                               uint32_t   *size)
{
  যদি (data_len < 4)
    প্রত্যাগম অসত্য;

  ধ্রুব unsigned বর্ণ *trailer = (ধ্রুব unsigned বর্ণ *) data + data_len - 4;
  যদি (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  প্রত্যাগম সত্য;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
