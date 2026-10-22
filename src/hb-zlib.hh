#اگر_غیر_تعریف_شدہ HB_ZLIB_HH
#تعریف_کرو HB_ZLIB_HH

#شامل "hb-blob.hh"

ساکن اندرونی منطقی
hb_blob_is_gzip (ثابت حرف *ڈیٹا,
                 غیرعلامتی    ڈیٹا_len)
{
  واپس ڈیٹا_len >= 3 &&
         (غیرعلامتی حرف) ڈیٹا[0] == 0x1Fu &&
         (غیرعلامتی حرف) ڈیٹا[1] == 0x8Bu &&
         (غیرعلامتی حرف) ڈیٹا[2] == 0x08u;
}

ساکن اندرونی منطقی
hb_gzip_get_uncompressed_سائز (ثابت حرف *ڈیٹا,
                               غیرعلامتی    ڈیٹا_len,
                               uint32_t   *سائز)
{
  اگر (ڈیٹا_len < 4)
    واپس جھوٹ;

  ثابت غیرعلامتی حرف *trailer = (ثابت غیرعلامتی حرف *) ڈیٹا + ڈیٹا_len - 4;
  اگر (سائز)
    *سائز = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  واپس سچ;
}

HB_اندرونی hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         غیرعلامتی   max_output_len);

#اگر_ختم /* HB_ZLIB_HH */
