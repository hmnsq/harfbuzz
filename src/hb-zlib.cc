#ifndef HB_ZLIB_CC
#define HB_ZLIB_CC
#ifdef HB_ZLIB_CC /* Pacify -Wunused-macros. */

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-zlib.hh"

#ifdef HAVE_ZLIB
#অন্তর্ভাব <zlib.h>
#endif

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len)
{
#ifndef HAVE_ZLIB
  প্রত্যাগম nullptr;
#অন্যথা
  unsigned compressed_len = 0;
  ধ্রুব uint8_t *compressed = (ধ্রুব uint8_t *) hb_blob_get_data (blob, &compressed_len);
  যদি (!compressed || !compressed_len)
    প্রত্যাগম nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  যদি (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    প্রত্যাগম nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((ধ্রুব বর্ণ *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  বর্ণ *output = (বর্ণ *) hb_malloc (allocated);
  যদি (!output)
    প্রত্যাগম nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  পূর্ণাঙ্ক status = Z_OK;
  যাবত্ (সত্য)
  {
    size_t produced = (size_t) stream.total_out;
    যদি (unlikely (produced >= (size_t) max_output_len))
      প্রত্যাগম nullptr;

    যদি (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      যদি (unlikely (new_allocated <= allocated))
        প্রত্যাগম nullptr;

      বর্ণ *new_output = (বর্ণ *) hb_realloc (output, new_allocated);
      যদি (unlikely (!new_output))
        প্রত্যাগম nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    যদি (status == Z_STREAM_END)
      ভঙ্গ;

    যদি ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      অনুবর্ত;

    প্রত্যাগম nullptr;
  }

  যদি (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    প্রত্যাগম nullptr;

  output_guard.release ();
  প্রত্যাগম hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
