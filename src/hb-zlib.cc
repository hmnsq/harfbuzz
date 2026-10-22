#अगर_अघोषित HB_ZLIB_CC
#घोषणा HB_ZLIB_CC
#अगर_घोषित HB_ZLIB_CC /* Pacify -Wunused-macros. */

#समावेश "hb.hh"

#समावेश "hb-zlib.hh"

#अगर_घोषित HAVE_ZLIB
#समावेश <zlib.h>
#पूर्ण_अगर

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         अचिन्हित   max_output_len)
{
#अगर_अघोषित HAVE_ZLIB
  वापस nullptr;
#अन्यथा
  अचिन्हित compressed_len = 0;
  स्थिर uint8_t *compressed = (स्थिर uint8_t *) hb_blob_get_तथ्य (blob, &compressed_len);
  अगर (!compressed || !compressed_len)
    वापस nullptr;

  z_stream stream = {};
  stream.अग्र_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  अगर (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    वापस nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_आकार = 0;
  hb_gzip_get_uncompressed_आकार ((स्थिर अक्षर *) compressed,
                                 compressed_len,
                                 &expected_आकार);

  माप_प्रकार allocated = hb_min ((माप_प्रकार) hb_max (expected_आकार, 4096u),
                             (माप_प्रकार) max_output_len);
  अक्षर *output = (अक्षर *) hb_malloc (allocated);
  अगर (!output)
    वापस nullptr;
  स्वतः output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  पूर्णांक status = Z_OK;
  जबतक (true)
  {
    माप_प्रकार produced = (माप_प्रकार) stream.योग_out;
    अगर (unlikely (produced >= (माप_प्रकार) max_output_len))
      वापस nullptr;

    अगर (produced == allocated)
    {
      माप_प्रकार new_allocated = hb_min (allocated * 2, (माप_प्रकार) max_output_len);
      अगर (unlikely (new_allocated <= allocated))
        वापस nullptr;

      अक्षर *new_output = (अक्षर *) hb_realloc (output, new_allocated);
      अगर (unlikely (!new_output))
        वापस nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.अग्र_out = (Bytef *) output + stream.योग_out;
    stream.avail_out = (uInt) (allocated - (माप_प्रकार) stream.योग_out);

    status = inflate (&stream, Z_FINISH);
    अगर (status == Z_STREAM_END)
      अवरोध;

    अगर ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      जारी;

    वापस nullptr;
  }

  अगर (unlikely ((माप_प्रकार) stream.योग_out > (माप_प्रकार) max_output_len))
    वापस nullptr;

  output_guard.release ();
  वापस hb_blob_create_or_fail (output,
                                 (अचिन्हित) stream.योग_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#पूर्ण_अगर
}

#पूर्ण_अगर /* HB_ZLIB_CC pacify */
#पूर्ण_अगर /* HB_ZLIB_CC guard */
