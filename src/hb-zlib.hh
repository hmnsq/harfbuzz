#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#અન્તર્ભાવ "hb-blob.hh"

સ્થિર અંતઃરેખ તાર્કિક
hb_blob_is_gzip (અચલ અક્ષર *data,
                 unsigned    data_len)
{
  પાછા data_len >= 3 &&
         (unsigned અક્ષર) data[0] == 0x1Fu &&
         (unsigned અક્ષર) data[1] == 0x8Bu &&
         (unsigned અક્ષર) data[2] == 0x08u;
}

સ્થિર અંતઃરેખ તાર્કિક
hb_gzip_get_uncompressed_size (અચલ અક્ષર *data,
                               unsigned    data_len,
                               uint32_t   *size)
{
  જો (data_len < 4)
    પાછા ખોટું;

  અચલ unsigned અક્ષર *trailer = (અચલ unsigned અક્ષર *) data + data_len - 4;
  જો (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  પાછા સાચું;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
