#ifndef HB_ZLIB_CC
#define HB_ZLIB_CC
#ifdef HB_ZLIB_CC /* Pacify -Wunused-macros. */

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-zlib.hh"

#ifdef HAVE_ZLIB
#ಅನ್ತರ್ಭಾವ <zlib.h>
#endif

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len)
{
#ifndef HAVE_ZLIB
  ಪ್ರತ್ಯಾಗಮ nullptr;
#ಅನ್ಯಥಾ
  unsigned compressed_len = 0;
  ಧ್ರುವ uint8_t *compressed = (ಧ್ರುವ uint8_t *) hb_blob_get_data (blob, &compressed_len);
  ಯದಿ (!compressed || !compressed_len)
    ಪ್ರತ್ಯಾಗಮ nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  ಯದಿ (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    ಪ್ರತ್ಯಾಗಮ nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((ಧ್ರುವ ವರ್ಣ *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  ವರ್ಣ *output = (ವರ್ಣ *) hb_malloc (allocated);
  ಯದಿ (!output)
    ಪ್ರತ್ಯಾಗಮ nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  ಪೂರ್ಣಾಙ್ಕ status = Z_OK;
  ಯಾವತ್ (ಸತ್ಯ)
  {
    size_t produced = (size_t) stream.total_out;
    ಯದಿ (unlikely (produced >= (size_t) max_output_len))
      ಪ್ರತ್ಯಾಗಮ nullptr;

    ಯದಿ (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      ಯದಿ (unlikely (new_allocated <= allocated))
        ಪ್ರತ್ಯಾಗಮ nullptr;

      ವರ್ಣ *new_output = (ವರ್ಣ *) hb_realloc (output, new_allocated);
      ಯದಿ (unlikely (!new_output))
        ಪ್ರತ್ಯಾಗಮ nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    ಯದಿ (status == Z_STREAM_END)
      ಭಙ್ಗ;

    ಯದಿ ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      ಅನುವರ್ತ;

    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಯದಿ (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  output_guard.release ();
  ಪ್ರತ್ಯಾಗಮ hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
