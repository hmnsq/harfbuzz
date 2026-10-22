#جے_بنا_تعریف HB_ZLIB_CC
#تعریف_کرو HB_ZLIB_CC
#جے_تعریف_شدہ HB_ZLIB_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"

#ضمن "hb-zlib.hh"

#جے_تعریف_شدہ HAVE_ZLIB
#ضمن <zlib.h>
#جے_انت

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         بنانشانی   max_output_len)
{
#جے_بنا_تعریف HAVE_ZLIB
  أرجع خالی_سوچک;
#وإلا
  بنانشانی compressed_len = 0;
  ثابت uint8_t *compressed = (ثابت uint8_t *) hb_blob_get_انکڑے (blob, &compressed_len);
  إذا (!compressed || !compressed_len)
    أرجع خالی_سوچک;

  z_دھارا دھارا = {};
  دھارا.اگلا_وچ = (Bytef *) compressed;
  دھارا.avail_وچ = compressed_len;

  إذا (inflateInit2 (&دھارا, 16 + MAX_WBITS) != Z_OK)
    أرجع خالی_سوچک;
  HB_SCOPE_GUARD (inflate_سرا (&دھارا));

  uint32_t expected_ناپ = 0;
  hb_gzip_get_uncompressed_ناپ ((ثابت حرف *) compressed,
                                 compressed_len,
                                 &expected_ناپ);

  size_t allocated = hb_min ((size_t) hb_max (expected_ناپ, 4096u),
                             (size_t) max_output_len);
  حرف *output = (حرف *) hb_malloc (allocated);
  إذا (!output)
    أرجع خالی_سوچک;
  خودکار output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  صحيح حالت = Z_OK;
  طالما (سچ)
  {
    size_t produced = (size_t) دھارا.کل_باہرلا;
    إذا (unlikely (produced >= (size_t) max_output_len))
      أرجع خالی_سوچک;

    إذا (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      إذا (unlikely (new_allocated <= allocated))
        أرجع خالی_سوچک;

      حرف *new_output = (حرف *) hb_realloc (output, new_allocated);
      إذا (unlikely (!new_output))
        أرجع خالی_سوچک;

      output = new_output;
      allocated = new_allocated;
    }

    دھارا.اگلا_باہرلا = (Bytef *) output + دھارا.کل_باہرلا;
    دھارا.avail_باہرلا = (uInt) (allocated - (size_t) دھارا.کل_باہرلا);

    حالت = inflate (&دھارا, Z_انت);
    إذا (حالت == Z_دھارا_سرا)
      اكسر;

    إذا ((حالت == Z_OK || حالت == Z_BUF_ERROR) && دھارا.avail_باہرلا == 0)
      استمر;

    أرجع خالی_سوچک;
  }

  إذا (unlikely ((size_t) دھارا.کل_باہرلا > (size_t) max_output_len))
    أرجع خالی_سوچک;

  output_guard.release ();
  أرجع hb_blob_create_or_fail (output,
                                 (بنانشانی) دھارا.کل_باہرلا,
                                 HB_حافظہ_منوال_WRITABLE,
                                 output,
                                 hb_free);
#جے_انت
}

#جے_انت /* HB_ZLIB_CC pacify */
#جے_انت /* HB_ZLIB_CC guard */
