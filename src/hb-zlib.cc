#ifndef HB_ZLIB_CC
#define HB_ZLIB_CC
#ifdef HB_ZLIB_CC /* Pacify -Wunused-macros. */

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-zlib.hh"

#ifdef HAVE_ZLIB
#अन्तर्भाव <zlib.h>
#endif

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len)
{
#ifndef HAVE_ZLIB
  प्रत्यागम nullptr;
#अण्णहा
  unsigned compressed_len = 0;
  ध्रुव uint8_t *compressed = (ध्रुव uint8_t *) hb_blob_get_data (blob, &compressed_len);
  जइ (!compressed || !compressed_len)
    प्रत्यागम nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  जइ (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    प्रत्यागम nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((ध्रुव वर्ण *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  वर्ण *output = (वर्ण *) hb_malloc (allocated);
  जइ (!output)
    प्रत्यागम nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  पूर्णाङ्क status = Z_OK;
  जाव (सच्च)
  {
    size_t produced = (size_t) stream.total_out;
    जइ (unlikely (produced >= (size_t) max_output_len))
      प्रत्यागम nullptr;

    जइ (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      जइ (unlikely (new_allocated <= allocated))
        प्रत्यागम nullptr;

      वर्ण *new_output = (वर्ण *) hb_realloc (output, new_allocated);
      जइ (unlikely (!new_output))
        प्रत्यागम nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    जइ (status == Z_STREAM_END)
      भङ्ग;

    जइ ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      अनुवर्त;

    प्रत्यागम nullptr;
  }

  जइ (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    प्रत्यागम nullptr;

  output_guard.release ();
  प्रत्यागम hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
