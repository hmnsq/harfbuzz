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
  返回 nullptr;
#否则
  unsigned compressed_len = 0;
  const uint8_t *compressed = (const uint8_t *) hb_blob_get_data (blob, &compressed_len);
  如果 (!compressed || !compressed_len)
    返回 nullptr;

  z_stream stream = {};
  stream.next_in = (Bytef *) compressed;
  stream.avail_in = compressed_len;

  如果 (inflateInit2 (&stream, 16 + MAX_WBITS) != Z_OK)
    返回 nullptr;
  HB_SCOPE_GUARD (inflateEnd (&stream));

  uint32_t expected_size = 0;
  hb_gzip_get_uncompressed_size ((const char *) compressed,
                                 compressed_len,
                                 &expected_size);

  size_t allocated = hb_min ((size_t) hb_max (expected_size, 4096u),
                             (size_t) max_output_len);
  char *output = (char *) hb_malloc (allocated);
  如果 (!output)
    返回 nullptr;
  auto output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  整数 status = Z_OK;
  当 (true)
  {
    size_t produced = (size_t) stream.total_out;
    如果 (unlikely (produced >= (size_t) max_output_len))
      返回 nullptr;

    如果 (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      如果 (unlikely (new_allocated <= allocated))
        返回 nullptr;

      char *new_output = (char *) hb_realloc (output, new_allocated);
      如果 (unlikely (!new_output))
        返回 nullptr;

      output = new_output;
      allocated = new_allocated;
    }

    stream.next_out = (Bytef *) output + stream.total_out;
    stream.avail_out = (uInt) (allocated - (size_t) stream.total_out);

    status = inflate (&stream, Z_FINISH);
    如果 (status == Z_STREAM_END)
      break;

    如果 ((status == Z_OK || status == Z_BUF_ERROR) && stream.avail_out == 0)
      continue;

    返回 nullptr;
  }

  如果 (unlikely ((size_t) stream.total_out > (size_t) max_output_len))
    返回 nullptr;

  output_guard.release ();
  返回 hb_blob_create_or_fail (output,
                                 (unsigned) stream.total_out,
                                 HB_MEMORY_MODE_WRITABLE,
                                 output,
                                 hb_free);
#endif
}

#endif /* HB_ZLIB_CC pacify */
#endif /* HB_ZLIB_CC guard */
