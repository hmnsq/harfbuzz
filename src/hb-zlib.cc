#ifndef HB_ZLIB_CC
#define HB_ZLIB_CC
#ifdef HB_ZLIB_CC /* Pacify -Wunused-macros. */

#شامل "hb.hh"

#شامل "hb-zlib.hh"

#ifdef HAVE_ZLIB
#شامل <zlib.h>
#endif

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len)
{
#ifndef HAVE_ZLIB
  بازگشت nullptr;
#وگرنه
  unsigned compressed_len = 0;
  ثابت uint8_t *compressed = (ثابت uint8_t *) hb_blob_get_data (blob, &compressed_len);
  اگر (!compressed || !compressed_len)
    بازگشت nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  اگر (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    بازگشت nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((ثابت نویسه *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  نویسه *output = (نویسه *) hb_malloc (allocated);
  اگر (!output)
    بازگشت nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  صحیح status = Z_OK;
  وقتی (درست)
  {
    size_t produced = (size_t) stream.total_out;
    اگر (unlikely (produced >= (size_t) max_output_len))
      بازگشت nullptr;

    اگر (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      اگر (unlikely (new_allocated <= allocated))
        بازگشت nullptr;

      نویسه *new_output = (نویسه *) hb_realloc (output, new_allocated);
      اگر (unlikely (!new_output))
        بازگشت nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    اگر (status == Z_STREAM_END)
      بشکن;

    اگر ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      ادامه;

    بازگشت nullptr;
  }

  اگر (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    بازگشت nullptr;

  output_guard.release ();
  بازگشت hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
