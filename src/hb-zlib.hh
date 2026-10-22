#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#ଅନ୍ତର୍ଭାଵ "hb-blob.hh"

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
hb_blob_is_gzip (ଅଚଳ ଅକ୍ଷର *data,
                 unsigned    data_len)
{
  ଫେରନ୍ତୁ data_len >= 3 &&
         (unsigned ଅକ୍ଷର) data[0] == 0x1Fu &&
         (unsigned ଅକ୍ଷର) data[1] == 0x8Bu &&
         (unsigned ଅକ୍ଷର) data[2] == 0x08u;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
hb_gzip_get_uncompressed_size (ଅଚଳ ଅକ୍ଷର *data,
                               unsigned    data_len,
                               uint32_t   *size)
{
  ଯଦି (data_len < 4)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଅଚଳ unsigned ଅକ୍ଷର *trailer = (ଅଚଳ unsigned ଅକ୍ଷର *) data + data_len - 4;
  ଯଦି (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
