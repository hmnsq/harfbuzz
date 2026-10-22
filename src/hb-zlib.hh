#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#অন্তর্ভাৱ "hb-blob.hh"

স্থির অন্তঃস্থ তার্কিক
hb_blob_is_gzip (ধ্রুৱ ৱর্ণ *data,
                 unsigned    data_len)
{
  ঘূৰাই data_len >= 3 &&
         (unsigned ৱর্ণ) data[0] == 0x1Fu &&
         (unsigned ৱর্ণ) data[1] == 0x8Bu &&
         (unsigned ৱর্ণ) data[2] == 0x08u;
}

স্থির অন্তঃস্থ তার্কিক
hb_gzip_get_uncompressed_size (ধ্রুৱ ৱর্ণ *data,
                               unsigned    data_len,
                               uint32_t   *size)
{
  যদি (data_len < 4)
    ঘূৰাই মিছা;

  ধ্রুৱ unsigned ৱর্ণ *trailer = (ধ্রুৱ unsigned ৱর্ণ *) data + data_len - 4;
  যদি (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  ঘূৰাই সঁচা;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
