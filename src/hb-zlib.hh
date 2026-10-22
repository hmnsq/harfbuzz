#अगर_अघोषित HB_ZLIB_HH
#घोषणा HB_ZLIB_HH

#समावेश "hb-blob.hh"

अटल अंतरभूत bool
hb_blob_is_gzip (स्थिर अक्षर *तथ्य,
                 अचिन्हित    तथ्य_len)
{
  वापस तथ्य_len >= 3 &&
         (अचिन्हित अक्षर) तथ्य[0] == 0x1Fu &&
         (अचिन्हित अक्षर) तथ्य[1] == 0x8Bu &&
         (अचिन्हित अक्षर) तथ्य[2] == 0x08u;
}

अटल अंतरभूत bool
hb_gzip_get_uncompressed_आकार (स्थिर अक्षर *तथ्य,
                               अचिन्हित    तथ्य_len,
                               uint32_t   *आकार)
{
  अगर (तथ्य_len < 4)
    वापस false;

  स्थिर अचिन्हित अक्षर *trailer = (स्थिर अचिन्हित अक्षर *) तथ्य + तथ्य_len - 4;
  अगर (आकार)
    *आकार = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  वापस true;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         अचिन्हित   max_output_len);

#पूर्ण_अगर /* HB_ZLIB_HH */
