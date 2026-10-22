#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#অন্তর্ভাব "hb-blob.hh"

স্থির অন্তঃরেখ যৌক্তিক
hb_blob_is_gzip (ধ্রুবক অক্ষর *তথ্য,
                 unsigned    তথ্য_len)
{
  ফেরত তথ্য_len >= 3 &&
         (unsigned অক্ষর) তথ্য[0] == 0x1Fu &&
         (unsigned অক্ষর) তথ্য[1] == 0x8Bu &&
         (unsigned অক্ষর) তথ্য[2] == 0x08u;
}

স্থির অন্তঃরেখ যৌক্তিক
hb_gzip_get_uncompressed_আকার (ধ্রুবক অক্ষর *তথ্য,
                               unsigned    তথ্য_len,
                               uint32_t   *আকার_2)
{
  যদি (তথ্য_len < 4)
    ফেরত মিথ্যা;

  ধ্রুবক unsigned অক্ষর *trailer = (ধ্রুবক unsigned অক্ষর *) তথ্য + তথ্য_len - 4;
  যদি (আকার_2)
    *আকার_2 = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  ফেরত সত্য;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len);

#endif /* HB_ZLIB_HH */
