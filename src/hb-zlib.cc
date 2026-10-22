#ifndef HB_ZLIB_CC
#define HB_ZLIB_CC
#ifdef HB_ZLIB_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"

#ضمن "hb-zlib.hh"

#ifdef HAVE_ZLIB
#ضمن <zlib.h>
#endif

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len)
{
#ifndef HAVE_ZLIB
  ܦܢܐ nullptr;
#ܐܚܪܢܐ
  unsigned compressed_len = 0;
  ثابت uint8_t *compressed = (ثابت uint8_t *) hb_blob_get_data (blob, &compressed_len);
  ܐܢ (!compressed || !compressed_len)
    ܦܢܐ nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  ܐܢ (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    ܦܢܐ nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((ثابت حرف *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  حرف *output = (حرف *) hb_malloc (allocated);
  ܐܢ (!output)
    ܦܢܐ nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  صحيح status = Z_OK;
  ܟܡܐ (ܫܪܪܐ)
  {
    size_t produced = (size_t) stream.total_out;
    ܐܢ (unlikely (produced >= (size_t) max_output_len))
      ܦܢܐ nullptr;

    ܐܢ (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      ܐܢ (unlikely (new_allocated <= allocated))
        ܦܢܐ nullptr;

      حرف *new_output = (حرف *) hb_realloc (output, new_allocated);
      ܐܢ (unlikely (!new_output))
        ܦܢܐ nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    ܐܢ (status == Z_STREAM_END)
      اكسر;

    ܐܢ ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      استمر;

    ܦܢܐ nullptr;
  }

  ܐܢ (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    ܦܢܐ nullptr;

  output_guard.release ();
  ܦܢܐ hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
