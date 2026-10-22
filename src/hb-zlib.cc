#ifndef HB_ZLIB_CC
#define HB_ZLIB_CC
#ifdef HB_ZLIB_CC /* Pacify -Wunused-macros. */

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-zlib.hh"

#ifdef HAVE_ZLIB
#अन्तर्भाव <zlib.h>
#endif

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         अचिन्हित   max_output_len)
{
#ifndef HAVE_ZLIB
  फर्काऊ nullptr;
#अन्यथा
  अचिन्हित compressed_len = 0;
  अचर uint8_t *compressed = (अचर uint8_t *) hb_blob_get_data (blob, &compressed_len);
  यदि (!compressed || !compressed_len)
    फर्काऊ nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  यदि (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    फर्काऊ nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((अचर वर्ण *) compressed,
                                 compressed_len,
                                 &expected_size);

  माप_प्रकार allocated = hb_min ((माप_प्रकार) hb_max (expected_size, 4096u),
                             (माप_प्रकार) max_output_len);
  वर्ण *output = (वर्ण *) hb_malloc (allocated);
  यदि (!output)
    फर्काऊ nullptr;
  स्वतः output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  पूर्णाङ्क status = Z_OK;
  जबसम्म (सत्य)
  {
    माप_प्रकार produced = (माप_प्रकार) stream.total_out;
    यदि (unlikely (produced >= (माप_प्रकार) max_output_len))
      फर्काऊ nullptr;

    यदि (produced == allocated)
    {
      माप_प्रकार new_allocated = hb_min (allocated * 2, (माप_प्रकार) max_output_len);
      यदि (unlikely (new_allocated <= allocated))
        फर्काऊ nullptr;

      वर्ण *new_output = (वर्ण *) hb_realloc (output, new_allocated);
      यदि (unlikely (!new_output))
        फर्काऊ nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (माप_प्रकार) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    यदि (status == Z_STREAM_END)
      रोक;

    यदि ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      जारी;

    फर्काऊ nullptr;
  }

  यदि (unlikely ((माप_प्रकार) stream.total_out > (माप_प्रकार) max_output_len))
    फर्काऊ nullptr;

  output_guard.release ();
  फर्काऊ hb_blob_create_or_fail (output,
                                 (अचिन्हित) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
