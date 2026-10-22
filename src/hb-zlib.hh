#ifndef HB_ZLIB_HH
#define HB_ZLIB_HH

#अन्तर्भाव "hb-blob.hh"

स्थिर अन्तःस्थ तार्किक
hb_blob_is_gzip (अचर वर्ण *data,
                 अचिन्हित    data_len)
{
  फर्काऊ data_len >= 3 &&
         (अचिन्हित वर्ण) data[0] == 0x1Fu &&
         (अचिन्हित वर्ण) data[1] == 0x8Bu &&
         (अचिन्हित वर्ण) data[2] == 0x08u;
}

स्थिर अन्तःस्थ तार्किक
hb_gzip_get_uncompressed_size (अचर वर्ण *data,
                               अचिन्हित    data_len,
                               uint32_t   *size)
{
  यदि (data_len < 4)
    फर्काऊ असत्य;

  अचर अचिन्हित वर्ण *trailer = (अचर अचिन्हित वर्ण *) data + data_len - 4;
  यदि (size)
    *size = (uint32_t) trailer[0] |
            ((uint32_t) trailer[1] << 8) |
            ((uint32_t) trailer[2] << 16) |
            ((uint32_t) trailer[3] << 24);
  फर्काऊ सत्य;
}

HB_INTERNAL hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         अचिन्हित   max_output_len);

#endif /* HB_ZLIB_HH */
