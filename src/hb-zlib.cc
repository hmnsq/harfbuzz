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
  ফেরত nullptr;
#অন্যথা
  unsigned compressed_len = 0;
  ধ্রুবক uint8_t *compressed = (ধ্রুবক uint8_t *) hb_blob_get_তথ্য (blob, &compressed_len);
  যদি (!compressed || !compressed_len)
    ফেরত nullptr;

  z_stream stream = {};
  stream.পরবর্তী_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  যদি (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    ফেরত nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_আকার = 0;
  hb_gzip_get_uncompressed_আকার ((ধ্রুবক অক্ষর *) compressed,
                                 compressed_len,
                                 &expected_আকার);

  size_t allocated = hb_min ((size_t) hb_max (expected_আকার, 4096u),
                             (size_t) max_output_len);
  অক্ষর *output = (অক্ষর *) hb_malloc (allocated);
  যদি (!output)
    ফেরত nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  পূর্ণসংখ্যা status = Z_OK;
  যখন (সত্য)
  {
    size_t produced = (size_t) stream.যোগ_out;
    যদি (unlikely (produced >= (size_t) max_output_len))
      ফেরত nullptr;

    যদি (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      যদি (unlikely (new_allocated <= allocated))
        ফেরত nullptr;

      অক্ষর *new_output = (অক্ষর *) hb_realloc (output, new_allocated);
      যদি (unlikely (!new_output))
        ফেরত nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.পরবর্তী_out = (Bytef *) output + stream.যোগ_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.যোগ_out);

    status = inflate (&stream, Z_FINISH);
    যদি (status == Z_STREAM_END)
      বিরতি;

    যদি ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      চালু;

    ফেরত nullptr;
  }

  যদি (unlikely ((size_t) stream.যোগ_out > (size_t) max_output_len))
    ফেরত nullptr;

  output_guard.release ();
  ফেরত hb_blob_create_or_fail (output,
                                 (unsigned) stream.যোগ_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
