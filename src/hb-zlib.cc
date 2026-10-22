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
  पटिनिवत्त nullptr;
#अञ्ञथा
  unsigned compressed_len = 0;
  ध्रुव uint8_t *compressed = (ध्रुव uint8_t *) hb_blob_get_data (blob, &compressed_len);
  सचे (!compressed || !compressed_len)
    पटिनिवत्त nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  सचे (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    पटिनिवत्त nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((ध्रुव वर्ण *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  वर्ण *output = (वर्ण *) hb_malloc (allocated);
  सचे (!output)
    पटिनिवत्त nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  पूर्णाङ्क status = Z_OK;
  याव (सच्च)
  {
    size_t produced = (size_t) stream.total_out;
    सचे (unlikely (produced >= (size_t) max_output_len))
      पटिनिवत्त nullptr;

    सचे (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      सचे (unlikely (new_allocated <= allocated))
        पटिनिवत्त nullptr;

      वर्ण *new_output = (वर्ण *) hb_realloc (output, new_allocated);
      सचे (unlikely (!new_output))
        पटिनिवत्त nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    सचे (status == Z_STREAM_END)
      भङ्ग;

    सचे ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      अनुवर्त;

    पटिनिवत्त nullptr;
  }

  सचे (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    पटिनिवत्त nullptr;

  output_guard.release ();
  पटिनिवत्त hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
