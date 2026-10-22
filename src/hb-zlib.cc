#ifndef HB_ZLIB_CC
#define HB_ZLIB_CC
#ifdef HB_ZLIB_CC /* Pacify -Wunused-macros. */

#include "hb.hh"

#include "hb-zlib.hh"

#ifdef HAVE_ZLIB
#include <zlib.h>
#endif

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         unsigned   max_output_len)
{
#ifndef HAVE_ZLIB
  ᤀᤡᤴ nullptr;
#ᤀᤡ᤺ᤆᤡ
  unsigned compressed_len = 0;
  const uint8_t *compressed = (const uint8_t *) hb_blob_get_data (blob, &compressed_len);
  ᤀᤡ᤺ (!compressed || !compressed_len)
    ᤀᤡᤴ nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  ᤀᤡ᤺ (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    ᤀᤡᤴ nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((const var_na *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  var_na *output = (var_na *) hb_malloc (allocated);
  ᤀᤡ᤺ (!output)
    ᤀᤡᤴ nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  puur_naa_nka status = Z_OK;
  ᤀᤠ᤺ (true)
  {
    size_t produced = (size_t) stream.total_out;
    ᤀᤡ᤺ (unlikely (produced >= (size_t) max_output_len))
      ᤀᤡᤴ nullptr;

    ᤀᤡ᤺ (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      ᤀᤡ᤺ (unlikely (new_allocated <= allocated))
        ᤀᤡᤴ nullptr;

      var_na *new_output = (var_na *) hb_realloc (output, new_allocated);
      ᤀᤡ᤺ (unlikely (!new_output))
        ᤀᤡᤴ nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    ᤀᤡ᤺ (status == Z_STREAM_END)
      bha_nga;

    ᤀᤡ᤺ ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      continue;

    ᤀᤡᤴ nullptr;
  }

  ᤀᤡ᤺ (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    ᤀᤡᤴ nullptr;

  output_guard.release ();
  ᤀᤡᤴ hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
