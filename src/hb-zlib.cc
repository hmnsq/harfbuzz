#ifndef HB_ZLIB_CC
#define HB_ZLIB_CC
#ifdef HB_ZLIB_CC /* Pacify -Wunused-macros. */

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-zlib.hh"

#ifdef HAVE_ZLIB
#ଅନ୍ତର୍ଭାଵ <zlib.h>
#endif

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len)
{
#ifndef HAVE_ZLIB
  ଫେରନ୍ତୁ nullptr;
#ଅନ୍ୟଥା
  unsigned compressed_len = 0;
  ଅଚଳ uint8_t *compressed = (ଅଚଳ uint8_t *) hb_blob_get_data (blob, &compressed_len);
  ଯଦି (!compressed || !compressed_len)
    ଫେରନ୍ତୁ nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  ଯଦି (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    ଫେରନ୍ତୁ nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((ଅଚଳ ଅକ୍ଷର *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  ଅକ୍ଷର *output = (ଅକ୍ଷର *) hb_malloc (allocated);
  ଯଦି (!output)
    ଫେରନ୍ତୁ nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = Z_OK;
  ଯେତେବେଳେ (ସତ୍ୟ)
  {
    size_t produced = (size_t) stream.total_out;
    ଯଦି (unlikely (produced >= (size_t) max_output_len))
      ଫେରନ୍ତୁ nullptr;

    ଯଦି (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      ଯଦି (unlikely (new_allocated <= allocated))
        ଫେରନ୍ତୁ nullptr;

      ଅକ୍ଷର *new_output = (ଅକ୍ଷର *) hb_realloc (output, new_allocated);
      ଯଦି (unlikely (!new_output))
        ଫେରନ୍ତୁ nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    ଯଦି (status == Z_STREAM_END)
      ଭାଙ୍ଗ;

    ଯଦି ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      ଜାରି;

    ଫେରନ୍ତୁ nullptr;
  }

  ଯଦି (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    ଫେରନ୍ତୁ nullptr;

  output_guard.release ();
  ଫେରନ୍ତୁ hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
