#ifndef HB_ZLIB_CC
#define HB_ZLIB_CC
#ifdef HB_ZLIB_CC /* Pacify -Wunused-macros. */

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-zlib.hh"

#ifdef HAVE_ZLIB
#அந்தர்பாவ <zlib.h>
#endif

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len)
{
#ifndef HAVE_ZLIB
  திரும்பு nullptr;
#இல்லையெனில்
  unsigned compressed_len = 0;
  மாறிலி uint8_t *compressed = (மாறிலி uint8_t *) hb_blob_get_data (blob, &compressed_len);
  எனில் (!compressed || !compressed_len)
    திரும்பு nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  எனில் (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    திரும்பு nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((மாறிலி எழுத்து *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  எழுத்து *output = (எழுத்து *) hb_malloc (allocated);
  எனில் (!output)
    திரும்பு nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  முழு status = Z_OK;
  போது (உண்மை)
  {
    size_t produced = (size_t) stream.total_out;
    எனில் (unlikely (produced >= (size_t) max_output_len))
      திரும்பு nullptr;

    எனில் (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      எனில் (unlikely (new_allocated <= allocated))
        திரும்பு nullptr;

      எழுத்து *new_output = (எழுத்து *) hb_realloc (output, new_allocated);
      எனில் (unlikely (!new_output))
        திரும்பு nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    எனில் (status == Z_STREAM_END)
      நிறுத்து;

    எனில் ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      தொடர்;

    திரும்பு nullptr;
  }

  எனில் (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    திரும்பு nullptr;

  output_guard.release ();
  திரும்பு hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
