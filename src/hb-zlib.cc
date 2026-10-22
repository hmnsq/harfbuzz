#اگر_تعریف_نشده HB_ZLIB_CC
#تعریف_کن HB_ZLIB_CC
#اگر_تعریف_شده HB_ZLIB_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"

#ضمن "hb-zlib.hh"

#اگر_تعریف_شده HAVE_ZLIB
#ضمن <zlib.h>
#پایان_اگر

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         بیعلامت   max_output_len)
{
#اگر_تعریف_نشده HAVE_ZLIB
  أرجع اشاره_تهی;
#وإلا
  بیعلامت compressed_len = 0;
  ثابت uint8_t *compressed = (ثابت uint8_t *) hb_blob_get_داده (blob, &compressed_len);
  إذا (!compressed || !compressed_len)
    أرجع اشاره_تهی;

  z_جریان جریان = {};
  جریان.بعدی_درون = (Bytef *) compressed;
  جریان.avail_درون = compressed_len;

  إذا (inflateInit2 (&جریان, 16 + MAX_WBITS) != Z_OK)
    أرجع اشاره_تهی;
  HB_SCOPE_GUARD (inflate_انتها (&جریان));

  uint32_t expected_اندازه = 0;
  hb_gzip_get_uncompressed_اندازه ((ثابت حرف *) compressed,
                                 compressed_len,
                                 &expected_اندازه);

  size_t allocated = hb_min ((size_t) hb_max (expected_اندازه, 4096u),
                             (size_t) max_output_len);
  حرف *output = (حرف *) hb_malloc (allocated);
  إذا (!output)
    أرجع اشاره_تهی;
  خودکار output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  صحيح وضعیت = Z_OK;
  طالما (درست)
  {
    size_t produced = (size_t) جریان.مجموع_بیرون;
    إذا (unlikely (produced >= (size_t) max_output_len))
      أرجع اشاره_تهی;

    إذا (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      إذا (unlikely (new_allocated <= allocated))
        أرجع اشاره_تهی;

      حرف *new_output = (حرف *) hb_realloc (output, new_allocated);
      إذا (unlikely (!new_output))
        أرجع اشاره_تهی;

      output = new_output;
      allocated = new_allocated;
    }

    جریان.بعدی_بیرون = (Bytef *) output + جریان.مجموع_بیرون;
    جریان.avail_بیرون = (uInt) (allocated - (size_t) جریان.مجموع_بیرون);

    وضعیت = inflate (&جریان, Z_پایان);
    إذا (وضعیت == Z_جریان_انتها)
      اكسر;

    إذا ((وضعیت == Z_OK || وضعیت == Z_BUF_ERROR) && جریان.avail_بیرون == 0)
      استمر;

    أرجع اشاره_تهی;
  }

  إذا (unlikely ((size_t) جریان.مجموع_بیرون > (size_t) max_output_len))
    أرجع اشاره_تهی;

  output_guard.release ();
  أرجع hb_blob_create_or_fail (output,
                                 (بیعلامت) جریان.مجموع_بیرون,
                                 HB_حافظه_نما_WRITABLE,
                                 output,
                                 hb_free);
#پایان_اگر
}

#پایان_اگر /* HB_ZLIB_CC pacify */
#پایان_اگر /* HB_ZLIB_CC guard */
