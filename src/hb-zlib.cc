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
  إرجاع nullptr;
#وإلا
  unsigned compressed_len = 0;
  ثابت uint8_t *compressed = (ثابت uint8_t *) hb_blob_get_data (blob, &compressed_len);
  إذا (!compressed || !compressed_len)
    إرجاع nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  إذا (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    إرجاع nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((ثابت حرف *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  حرف *output = (حرف *) hb_malloc (allocated);
  إذا (!output)
    إرجاع nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  صحيح status = Z_OK;
  بينما (true)
  {
    size_t produced = (size_t) stream.total_out;
    إذا (unlikely (produced >= (size_t) max_output_len))
      إرجاع nullptr;

    إذا (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      إذا (unlikely (new_allocated <= allocated))
        إرجاع nullptr;

      حرف *new_output = (حرف *) hb_realloc (output, new_allocated);
      إذا (unlikely (!new_output))
        إرجاع nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    إذا (status == Z_STREAM_END)
      اكسر;

    إذا ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      استمر;

    إرجاع nullptr;
  }

  إذا (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    إرجاع nullptr;

  output_guard.release ();
  إرجاع hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
