#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#ಅನ್ತರ್ಭಾವ "hb-blob.hh"

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
hb_blob_is_gzip (ಧ್ರುವ ವರ್ಣ *data,
                 unsigned    data_len)
{
  ಪ್ರತ್ಯಾಗಮ data_len >= 3 &&
         (unsigned ವರ್ಣ) data[0] == 0x1Fu &&
         (unsigned ವರ್ಣ) data[1] == 0x8Bu &&
         (unsigned ವರ್ಣ) data[2] == 0x08u;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
hb_gzip_get_uncompressed_size (ಧ್ರುವ ವರ್ಣ *data,
                               unsigned    data_len,
                               uint32_t   *size)
{
  ಯದಿ (data_len < 4)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಧ್ರುವ unsigned ವರ್ಣ *trailer = (ಧ್ರುವ unsigned ವರ್ಣ *) data + data_len - 4;
  ಯದಿ (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
