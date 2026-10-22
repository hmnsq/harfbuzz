#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#अन्तर्भाव "hb-blob.hh"

स्थिर अन्तःस्थ तार्किक
hb_blob_is_gzip (ध्रुव वर्ण *data,
                 unsigned    data_len)
{
  पटिनिवत्त data_len >= 3 &&
         (unsigned वर्ण) data[0] == 0x1Fu &&
         (unsigned वर्ण) data[1] == 0x8Bu &&
         (unsigned वर्ण) data[2] == 0x08u;
}

स्थिर अन्तःस्थ तार्किक
hb_gzip_get_uncompressed_size (ध्रुव वर्ण *data,
                               unsigned    data_len,
                               uint32_t   *size)
{
  सचे (data_len < 4)
    पटिनिवत्त मिच्छा;

  ध्रुव unsigned वर्ण *trailer = (ध्रुव unsigned वर्ण *) data + data_len - 4;
  सचे (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  पटिनिवत्त सच्च;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
