#اگر_غیر_تعریف_شدہ HB_ZLIB_CC
#تعریف_کرو HB_ZLIB_CC
#اگر_تعریف_شدہ HB_ZLIB_CC /* Pacify -Wunused-macros. */

#شامل "hb.hh"

#شامل "hb-zlib.hh"

#اگر_تعریف_شدہ HAVE_ZLIB
#شامل <zlib.h>
#اگر_ختم

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         غیرعلامتی   max_output_len)
{
#اگر_غیر_تعریف_شدہ HAVE_ZLIB
  واپس خالی_اشاریہ;
#ورنہ
  غیرعلامتی compressed_len = 0;
  ثابت uint8_t *compressed = (ثابت uint8_t *) hb_blob_get_ڈیٹا (blob, &compressed_len);
  اگر (!compressed || !compressed_len)
    واپس خالی_اشاریہ;

  z_دھارا دھارا = {};
  دھارا.اگلا_اندر = (Bytef *) compressed;
  دھارا.avail_اندر = compressed_len;

  اگر (inflateInit2 (&دھارا, 16 + MAX_WBITS) != Z_OK)
    واپس خالی_اشاریہ;
  HB_SCOPE_GUARD (inflate_انتہا (&دھارا));

  uint32_t expected_سائز = 0;
  hb_gzip_get_uncompressed_سائز ((ثابت حرف *) compressed,
                                 compressed_len,
                                 &expected_سائز);

  size_t allocated = hb_min ((size_t) hb_max (expected_سائز, 4096u),
                             (size_t) max_output_len);
  حرف *output = (حرف *) hb_malloc (allocated);
  اگر (!output)
    واپس خالی_اشاریہ;
  خودکار output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  صحیح حالت_2 = Z_OK;
  جب (سچ)
  {
    size_t produced = (size_t) دھارا.کل_برآمد;
    اگر (unlikely (produced >= (size_t) max_output_len))
      واپس خالی_اشاریہ;

    اگر (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      اگر (unlikely (new_allocated <= allocated))
        واپس خالی_اشاریہ;

      حرف *new_output = (حرف *) hb_realloc (output, new_allocated);
      اگر (unlikely (!new_output))
        واپس خالی_اشاریہ;

      output = new_output;
      allocated = new_allocated;
    }

    دھارا.اگلا_برآمد = (Bytef *) output + دھارا.کل_برآمد;
    دھارا.avail_برآمد = (uInt) (allocated - (size_t) دھارا.کل_برآمد);

    حالت_2 = inflate (&دھارا, Z_اختتام);
    اگر (حالت_2 == Z_دھارا_انتہا)
      توڑو;

    اگر ((حالت_2 == Z_OK || حالت_2 == Z_BUF_ERROR) && دھارا.avail_برآمد == 0)
      جاری;

    واپس خالی_اشاریہ;
  }

  اگر (unlikely ((size_t) دھارا.کل_برآمد > (size_t) max_output_len))
    واپس خالی_اشاریہ;

  output_guard.release ();
  واپس hb_blob_create_or_fail (output,
                                 (غیرعلامتی) دھارا.کل_برآمد,
                                 HB_حافظہ_منوال_WRITABLE,
                                 output,
                                 hb_free);
#اگر_ختم
}

#اگر_ختم /* HB_ZLIB_CC pacify */
#اگر_ختم /* HB_ZLIB_CC guard */
