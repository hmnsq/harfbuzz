#جيڪڏهن_وصف_نه_ٿيل HB_ZLIB_CC
#وصف_ڪريو HB_ZLIB_CC
#جيڪڏهن_وصف_ٿيل HB_ZLIB_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"

#ضمن "hb-zlib.hh"

#جيڪڏهن_وصف_ٿيل HAVE_ZLIB
#ضمن <zlib.h>
#جيڪڏهن_پورو

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         بنانشان   max_output_len)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_ZLIB
  أرجع خالي_اشارو;
#وإلا
  بنانشان compressed_len = 0;
  ثابت uint8_t *compressed = (ثابت uint8_t *) hb_blob_get_معلومات (blob, &compressed_len);
  إذا (!compressed || !compressed_len)
    أرجع خالي_اشارو;

  z_وهڪرو وهڪرو = {};
  وهڪرو.ايندڙ_اندريون = (Bytef *) compressed;
  وهڪرو.avail_اندريون = compressed_len;

  إذا (inflateInit2 (&وهڪرو, 16 + MAX_WBITS) != Z_OK)
    أرجع خالي_اشارو;
  HB_SCOPE_GUARD (inflate_پڇاڙي (&وهڪرو));

  uint32_t expected_سائيز = 0;
  hb_gzip_get_uncompressed_سائيز ((ثابت حرف *) compressed,
                                 compressed_len,
                                 &expected_سائيز);

  size_t allocated = hb_min ((size_t) hb_max (expected_سائيز, 4096u),
                             (size_t) max_output_len);
  حرف *output = (حرف *) hb_malloc (allocated);
  إذا (!output)
    أرجع خالي_اشارو;
  خودڪار output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  صحيح حالت = Z_OK;
  طالما (سچ)
  {
    size_t produced = (size_t) وهڪرو.ڪل_ٻاهريون;
    إذا (unlikely (produced >= (size_t) max_output_len))
      أرجع خالي_اشارو;

    إذا (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      إذا (unlikely (new_allocated <= allocated))
        أرجع خالي_اشارو;

      حرف *new_output = (حرف *) hb_realloc (output, new_allocated);
      إذا (unlikely (!new_output))
        أرجع خالي_اشارو;

      output = new_output;
      allocated = new_allocated;
    }

    وهڪرو.ايندڙ_ٻاهريون = (Bytef *) output + وهڪرو.ڪل_ٻاهريون;
    وهڪرو.avail_ٻاهريون = (uInt) (allocated - (size_t) وهڪرو.ڪل_ٻاهريون);

    حالت = inflate (&وهڪرو, Z_پڄاڻي);
    إذا (حالت == Z_وهڪرو_پڇاڙي)
      اكسر;

    إذا ((حالت == Z_OK || حالت == Z_BUF_ERROR) && وهڪرو.avail_ٻاهريون == 0)
      استمر;

    أرجع خالي_اشارو;
  }

  إذا (unlikely ((size_t) وهڪرو.ڪل_ٻاهريون > (size_t) max_output_len))
    أرجع خالي_اشارو;

  output_guard.release ();
  أرجع hb_blob_create_or_fail (output,
                                 (بنانشان) وهڪرو.ڪل_ٻاهريون,
                                 HB_يادگيري_گهڻائي_WRITABLE,
                                 output,
                                 hb_free);
#جيڪڏهن_پورو
}

#جيڪڏهن_پورو /* HB_ZLIB_CC pacify */
#جيڪڏهن_پورو /* HB_ZLIB_CC guard */
