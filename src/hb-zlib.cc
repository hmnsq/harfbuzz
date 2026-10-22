#که_نه_تعریف_شوی HB_ZLIB_CC
#تعریف_کړه HB_ZLIB_CC
#که_تعریف_شوی HB_ZLIB_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"

#ضمن "hb-zlib.hh"

#که_تعریف_شوی HAVE_ZLIB
#ضمن <zlib.h>
#د_که_پای

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         بینښه   max_output_len)
{
#که_نه_تعریف_شوی HAVE_ZLIB
  أرجع تش_اشاره;
#وإلا
  بینښه compressed_len = 0;
  ثابت uint8_t *compressed = (ثابت uint8_t *) hb_blob_get_معلومات (blob, &compressed_len);
  إذا (!compressed || !compressed_len)
    أرجع تش_اشاره;

  z_بهیر بهیر = {};
  بهیر.بل_دننه = (Bytef *) compressed;
  بهیر.avail_دننه = compressed_len;

  إذا (inflateInit2 (&بهیر, 16 + MAX_WBITS) != Z_OK)
    أرجع تش_اشاره;
  HB_SCOPE_GUARD (inflate_پای_ټکی (&بهیر));

  uint32_t expected_اندازه = 0;
  hb_gzip_get_uncompressed_اندازه ((ثابت حرف *) compressed,
                                 compressed_len,
                                 &expected_اندازه);

  size_t allocated = hb_min ((size_t) hb_max (expected_اندازه, 4096u),
                             (size_t) max_output_len);
  حرف *output = (حرف *) hb_malloc (allocated);
  إذا (!output)
    أرجع تش_اشاره;
  خپلکاری output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  صحيح حالت = Z_OK;
  طالما (ریښتیا)
  {
    size_t produced = (size_t) بهیر.ټول_بهر;
    إذا (unlikely (produced >= (size_t) max_output_len))
      أرجع تش_اشاره;

    إذا (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      إذا (unlikely (new_allocated <= allocated))
        أرجع تش_اشاره;

      حرف *new_output = (حرف *) hb_realloc (output, new_allocated);
      إذا (unlikely (!new_output))
        أرجع تش_اشاره;

      output = new_output;
      allocated = new_allocated;
    }

    بهیر.بل_بهر = (Bytef *) output + بهیر.ټول_بهر;
    بهیر.avail_بهر = (uInt) (allocated - (size_t) بهیر.ټول_بهر);

    حالت = inflate (&بهیر, Z_پای);
    إذا (حالت == Z_بهیر_پای_ټکی)
      اكسر;

    إذا ((حالت == Z_OK || حالت == Z_BUF_ERROR) && بهیر.avail_بهر == 0)
      استمر;

    أرجع تش_اشاره;
  }

  إذا (unlikely ((size_t) بهیر.ټول_بهر > (size_t) max_output_len))
    أرجع تش_اشاره;

  output_guard.release ();
  أرجع hb_blob_create_or_fail (output,
                                 (بینښه) بهیر.ټول_بهر,
                                 HB_حافظه_ډېرپېښ_WRITABLE,
                                 output,
                                 hb_free);
#د_که_پای
}

#د_که_پای /* HB_ZLIB_CC pacify */
#د_که_پای /* HB_ZLIB_CC guard */
