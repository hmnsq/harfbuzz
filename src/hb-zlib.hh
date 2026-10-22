#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#അന്തര്ഭാവ "hb-blob.hh"

സ്ഥിതം വരിയിൽ തർക്കം
hb_blob_is_gzip (സ്ഥിരം അക്ഷരം *data,
                 unsigned    data_len)
{
  തിരികെ data_len >= 3 &&
         (unsigned അക്ഷരം) data[0] == 0x1Fu &&
         (unsigned അക്ഷരം) data[1] == 0x8Bu &&
         (unsigned അക്ഷരം) data[2] == 0x08u;
}

സ്ഥിതം വരിയിൽ തർക്കം
hb_gzip_get_uncompressed_size (സ്ഥിരം അക്ഷരം *data,
                               unsigned    data_len,
                               uint32_t   *size)
{
  എങ്കിൽ (data_len < 4)
    തിരികെ അസത്യം;

  സ്ഥിരം unsigned അക്ഷരം *trailer = (സ്ഥിരം unsigned അക്ഷരം *) data + data_len - 4;
  എങ്കിൽ (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  തിരികെ സത്യം;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
