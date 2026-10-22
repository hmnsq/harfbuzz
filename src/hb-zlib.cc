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
  परत nullptr;
#अन्यथा
  अचिन्हित compressed_len = 0;
  स्थिरांक uint8_t *compressed = (स्थिरांक uint8_t *) hb_blob_get_data (blob, &compressed_len);
  जर (!compressed || !compressed_len)
    परत nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  जर (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    परत nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((स्थिरांक अक्षर *) compressed,
                                 compressed_len,
                                 &expected_size);

  माप_प्रकार allocated = hb_min ((माप_प्रकार) hb_max (expected_size, 4096u),
                             (माप_प्रकार) max_output_len);
  अक्षर *output = (अक्षर *) hb_malloc (allocated);
  जर (!output)
    परत nullptr;
  स्वतः output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  पूर्णांक status = Z_OK;
  पर्यंत (सत्य)
  {
    माप_प्रकार produced = (माप_प्रकार) stream.total_out;
    जर (unlikely (produced >= (माप_प्रकार) max_output_len))
      परत nullptr;

    जर (produced == allocated)
    {
      माप_प्रकार new_allocated = hb_min (allocated * 2, (माप_प्रकार) max_output_len);
      जर (unlikely (new_allocated <= allocated))
        परत nullptr;

      अक्षर *new_output = (अक्षर *) hb_realloc (output, new_allocated);
      जर (unlikely (!new_output))
        परत nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (माप_प्रकार) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    जर (status == Z_STREAM_END)
      थांबा;

    जर ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      सुरूठेवा;

    परत nullptr;
  }

  जर (unlikely ((माप_प्रकार) stream.total_out > (माप_प्रकार) max_output_len))
    परत nullptr;

  output_guard.release ();
  परत hb_blob_create_or_fail (output,
                                 (अचिन्हित) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
