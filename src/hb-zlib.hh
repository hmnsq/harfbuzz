#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#அந்தர்பாவ "hb-blob.hh"

நிலையான உள்வரி தருக்க
hb_blob_is_gzip (மாறிலி எழுத்து *data,
                 unsigned    data_len)
{
  திரும்பு data_len >= 3 &&
         (unsigned எழுத்து) data[0] == 0x1Fu &&
         (unsigned எழுத்து) data[1] == 0x8Bu &&
         (unsigned எழுத்து) data[2] == 0x08u;
}

நிலையான உள்வரி தருக்க
hb_gzip_get_uncompressed_size (மாறிலி எழுத்து *data,
                               unsigned    data_len,
                               uint32_t   *size)
{
  எனில் (data_len < 4)
    திரும்பு பொய்;

  மாறிலி unsigned எழுத்து *trailer = (மாறிலி unsigned எழுத்து *) data + data_len - 4;
  எனில் (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  திரும்பு உண்மை;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
