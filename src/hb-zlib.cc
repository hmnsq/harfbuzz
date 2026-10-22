#ifndef HB_ZLIB_CC
#define HB_ZLIB_CC
#ifdef HB_ZLIB_CC /* Pacify -Wunused-macros. */

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-zlib.hh"

#ifdef HAVE_ZLIB
#અન્તર્ભાવ <zlib.h>
#endif

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len)
{
#ifndef HAVE_ZLIB
  પાછા nullptr;
#નહીંતર
  unsigned compressed_len = 0;
  અચલ uint8_t *compressed = (અચલ uint8_t *) hb_blob_get_data (blob, &compressed_len);
  જો (!compressed || !compressed_len)
    પાછા nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  જો (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    પાછા nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((અચલ અક્ષર *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  અક્ષર *output = (અક્ષર *) hb_malloc (allocated);
  જો (!output)
    પાછા nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  પૂર્ણાંક status = Z_OK;
  જ્યારે (સાચું)
  {
    size_t produced = (size_t) stream.total_out;
    જો (unlikely (produced >= (size_t) max_output_len))
      પાછા nullptr;

    જો (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      જો (unlikely (new_allocated <= allocated))
        પાછા nullptr;

      અક્ષર *new_output = (અક્ષર *) hb_realloc (output, new_allocated);
      જો (unlikely (!new_output))
        પાછા nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    જો (status == Z_STREAM_END)
      તોડો;

    જો ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      ચાલુ;

    પાછા nullptr;
  }

  જો (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    પાછા nullptr;

  output_guard.release ();
  પાછા hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
