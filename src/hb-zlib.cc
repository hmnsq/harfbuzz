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
  ᱨᱩᱣᱟᱹᱲ nullptr;
#ᱵᱟᱝ
  unsigned compressed_len = 0;
  const uint8_t *compressed = (const uint8_t *) hb_blob_get_data (blob, &compressed_len);
  ᱡᱩᱫᱤ (!compressed || !compressed_len)
    ᱨᱩᱣᱟᱹᱲ nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  ᱡᱩᱫᱤ (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    ᱨᱩᱣᱟᱹᱲ nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((const var_na *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  var_na *output = (var_na *) hb_malloc (allocated);
  ᱡᱩᱫᱤ (!output)
    ᱨᱩᱣᱟᱹᱲ nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  puur_naa_nka status = Z_OK;
  ᱡᱚᱠᱷᱚᱱ (true)
  {
    size_t produced = (size_t) stream.total_out;
    ᱡᱩᱫᱤ (unlikely (produced >= (size_t) max_output_len))
      ᱨᱩᱣᱟᱹᱲ nullptr;

    ᱡᱩᱫᱤ (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      ᱡᱩᱫᱤ (unlikely (new_allocated <= allocated))
        ᱨᱩᱣᱟᱹᱲ nullptr;

      var_na *new_output = (var_na *) hb_realloc (output, new_allocated);
      ᱡᱩᱫᱤ (unlikely (!new_output))
        ᱨᱩᱣᱟᱹᱲ nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    ᱡᱩᱫᱤ (status == Z_STREAM_END)
      bha_nga;

    ᱡᱩᱫᱤ ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      continue;

    ᱨᱩᱣᱟᱹᱲ nullptr;
  }

  ᱡᱩᱫᱤ (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    ᱨᱩᱣᱟᱹᱲ nullptr;

  output_guard.release ();
  ᱨᱩᱣᱟᱹᱲ hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
