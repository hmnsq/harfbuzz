#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#अन्तर्भाव "hb-blob.hh"

स्थिर ओळीत तार्किक
hb_blob_is_gzip (स्थिरांक अक्षर *data,
                 अचिन्हित    data_len)
{
  परत data_len >= 3 &&
         (अचिन्हित अक्षर) data[0] == 0x1Fu &&
         (अचिन्हित अक्षर) data[1] == 0x8Bu &&
         (अचिन्हित अक्षर) data[2] == 0x08u;
}

स्थिर ओळीत तार्किक
hb_gzip_get_uncompressed_size (स्थिरांक अक्षर *data,
                               अचिन्हित    data_len,
                               uint32_t   *size)
{
  जर (data_len < 4)
    परत असत्य;

  स्थिरांक अचिन्हित अक्षर *trailer = (स्थिरांक अचिन्हित अक्षर *) data + data_len - 4;
  जर (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  परत सत्य;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         अचिन्हित   max_output_len);

#endif /* HB_ZLIB_HH */
