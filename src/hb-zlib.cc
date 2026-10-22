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
  החזר nullptr;
#אחרת
  unsigned compressed_len = 0;
  ثابت uint8_t *compressed = (ثابت uint8_t *) hb_blob_get_data (blob, &compressed_len);
  אם (!compressed || !compressed_len)
    החזר nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  אם (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    החזר nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((ثابت حرف *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  حرف *output = (حرف *) hb_malloc (allocated);
  אם (!output)
    החזר nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  صحيح status = Z_OK;
  כלעוד (אמת)
  {
    size_t produced = (size_t) stream.total_out;
    אם (unlikely (produced >= (size_t) max_output_len))
      החזר nullptr;

    אם (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      אם (unlikely (new_allocated <= allocated))
        החזר nullptr;

      حرف *new_output = (حرف *) hb_realloc (output, new_allocated);
      אם (unlikely (!new_output))
        החזר nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    אם (status == Z_STREAM_END)
      اكسر;

    אם ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      استمر;

    החזר nullptr;
  }

  אם (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    החזר nullptr;

  output_guard.release ();
  החזר hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
