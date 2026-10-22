#ifndef HB_ZLIB_CC
#define HB_ZLIB_CC
#ifdef HB_ZLIB_CC /* Pacify -Wunused-macros. */

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-zlib.hh"

#ifdef HAVE_ZLIB
#അന്തര്ഭാവ <zlib.h>
#endif

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len)
{
#ifndef HAVE_ZLIB
  തിരികെ nullptr;
#അല്ലെങ്കിൽ
  unsigned compressed_len = 0;
  സ്ഥിരം uint8_t *compressed = (സ്ഥിരം uint8_t *) hb_blob_get_data (blob, &compressed_len);
  എങ്കിൽ (!compressed || !compressed_len)
    തിരികെ nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  എങ്കിൽ (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    തിരികെ nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((സ്ഥിരം അക്ഷരം *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  അക്ഷരം *output = (അക്ഷരം *) hb_malloc (allocated);
  എങ്കിൽ (!output)
    തിരികെ nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  പൂർണ്ണസംഖ്യ status = Z_OK;
  ആകുമ്പോൾ (സത്യം)
  {
    size_t produced = (size_t) stream.total_out;
    എങ്കിൽ (unlikely (produced >= (size_t) max_output_len))
      തിരികെ nullptr;

    എങ്കിൽ (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      എങ്കിൽ (unlikely (new_allocated <= allocated))
        തിരികെ nullptr;

      അക്ഷരം *new_output = (അക്ഷരം *) hb_realloc (output, new_allocated);
      എങ്കിൽ (unlikely (!new_output))
        തിരികെ nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    എങ്കിൽ (status == Z_STREAM_END)
      നിർത്തുക;

    എങ്കിൽ ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      തുടരുക;

    തിരികെ nullptr;
  }

  എങ്കിൽ (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    തിരികെ nullptr;

  output_guard.release ();
  തിരികെ hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
