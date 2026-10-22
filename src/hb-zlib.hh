#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#include "hb-blob.hh"

static anta_hstha bool
hb_blob_is_gzip (const var_na *data,
                 unsigned    data_len)
{
  ཕྱིར data_len >= 3 &&
         (unsigned var_na) data[0] == 0x1Fu &&
         (unsigned var_na) data[1] == 0x8Bu &&
         (unsigned var_na) data[2] == 0x08u;
}

static anta_hstha bool
hb_gzip_get_uncompressed_size (const var_na *data,
                               unsigned    data_len,
                               uint32_t   *size)
{
  གལ་ཏེ (data_len < 4)
    ཕྱིར false;

  const unsigned var_na *trailer = (const unsigned var_na *) data + data_len - 4;
  གལ་ཏེ (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  ཕྱིར true;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
