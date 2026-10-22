#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ZLIB_CC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ZLIB_CC
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_ZLIB_CC /* Pacify -Wunused-macros. */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-zlib.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_ZLIB
#ਅਨ੍ਤਰ੍ਭਾਵ <zlib.h>
#ਜੇ_ਅੰਤ

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         ਬਿਨਾਨਿਸ਼ਾਨੀ   max_output_len)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_ZLIB
  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
#ਨਹੀਂਤਾਂ
  ਬਿਨਾਨਿਸ਼ਾਨੀ compressed_len = 0;
  ਅਚਲ uint8_t *compressed = (ਅਚਲ uint8_t *) hb_blob_get_ਅੰਕੜੇ (blob, &compressed_len);
  ਜੇ (!compressed || !compressed_len)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  z_ਧਾਰਾ ਧਾਰਾ = {};
  ਧਾਰਾ.ਅਗਲਾ_ਵਿੱਚ = (Bytef *) compressed;
  ਧਾਰਾ.avail_ਵਿੱਚ = compressed_len;

  ਜੇ (inflateInit2 (&ਧਾਰਾ, 16 + MAX_WBITS) != Z_OK)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  HB_SCOPE_GUARD (inflate_ਸਿਰਾ (&ਧਾਰਾ));

  uint32_t expected_ਮਾਪ = 0;
  hb_gzip_get_uncompressed_ਮਾਪ ((ਅਚਲ ਅੱਖਰ *) compressed,
                                 compressed_len,
                                 &expected_ਮਾਪ);

  size_t allocated = hb_min ((size_t) hb_max (expected_ਮਾਪ, 4096u),
                             (size_t) max_output_len);
  ਅੱਖਰ *output = (ਅੱਖਰ *) hb_malloc (allocated);
  ਜੇ (!output)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਸਵੈਚਾਲਿਤ output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  ਪੂਰਨ ਸਥਿਤੀ = Z_OK;
  ਜਦੋਂ (ਸੱਚ)
  {
    size_t produced = (size_t) ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ;
    ਜੇ (unlikely (produced >= (size_t) max_output_len))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਜੇ (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      ਜੇ (unlikely (new_allocated <= allocated))
        ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

      ਅੱਖਰ *new_output = (ਅੱਖਰ *) hb_realloc (output, new_allocated);
      ਜੇ (unlikely (!new_output))
        ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

      output = new_output;
      allocated = new_allocated;
    }

    ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = (Bytef *) output + ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ;
    ਧਾਰਾ.avail_ਬਾਹਰਲਾ = (uInt) (allocated - (size_t) ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ);

    ਸਥਿਤੀ = inflate (&ਧਾਰਾ, Z_ਅੰਤ);
    ਜੇ (ਸਥਿਤੀ == Z_ਧਾਰਾ_ਸਿਰਾ)
      ਰੋਕੋ;

    ਜੇ ((ਸਥਿਤੀ == Z_OK || ਸਥਿਤੀ == Z_BUF_ERROR) && ਧਾਰਾ.avail_ਬਾਹਰਲਾ == 0)
      ਜਾਰੀ;

    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਜੇ (unlikely ((size_t) ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ > (size_t) max_output_len))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  output_guard.release ();
  ਵਾਪਸ hb_blob_create_or_fail (output,
                                 (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ,
                                 HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE,
                                 output,
                                 hb_free);
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਅੰਤ /* HB_ZLIB_CC pacify */
#ਜੇ_ਅੰਤ /* HB_ZLIB_CC guard */
