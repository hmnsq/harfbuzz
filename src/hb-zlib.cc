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
  ပွၵ်ႈ nullptr;
#ပေႃးဢမ်ႇ
  unsigned compressed_len = 0;
  const uint8_t *compressed = (const uint8_t *) hb_blob_get_data (blob, &compressed_len);
  ပေႃး (!compressed || !compressed_len)
    ပွၵ်ႈ nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  ပေႃး (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    ပွၵ်ႈ nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((const var_na *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  var_na *output = (var_na *) hb_malloc (allocated);
  ပေႃး (!output)
    ပွၵ်ႈ nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  puur_naa_nka status = Z_OK;
  ၶၢဝ်း (true)
  {
    size_t produced = (size_t) stream.total_out;
    ပေႃး (unlikely (produced >= (size_t) max_output_len))
      ပွၵ်ႈ nullptr;

    ပေႃး (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      ပေႃး (unlikely (new_allocated <= allocated))
        ပွၵ်ႈ nullptr;

      var_na *new_output = (var_na *) hb_realloc (output, new_allocated);
      ပေႃး (unlikely (!new_output))
        ပွၵ်ႈ nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    ပေႃး (status == Z_STREAM_END)
      bha_nga;

    ပေႃး ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      continue;

    ပွၵ်ႈ nullptr;
  }

  ပေႃး (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    ပွၵ်ႈ nullptr;

  output_guard.release ();
  ပွၵ်ႈ hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
