#నిర్వచితం_కాకపోతే HB_ZLIB_CC
#నిర్వచించు HB_ZLIB_CC
#నిర్వచితమైతే HB_ZLIB_CC /* Pacify -Wunused-macros. */

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-zlib.hh"

#నిర్వచితమైతే HAVE_ZLIB
#అన్తర్భావ <zlib.h>
#అయితే_ముగింపు

hb_blob_t *
hb_blob_decompress_gzip (hb_blob_t *blob,
                         గుర్తులేని   max_output_len)
{
#నిర్వచితం_కాకపోతే HAVE_ZLIB
  తిరిగి శూన్యసూచిక;
#లేకపోతే
  గుర్తులేని compressed_len = 0;
  స్థిరరాశి uint8_t *compressed = (స్థిరరాశి uint8_t *) hb_blob_get_దత్తాంశం (blob, &compressed_len);
  ఒకవేళ (!compressed || !compressed_len)
    తిరిగి శూన్యసూచిక;

  z_ప్రవాహం ప్రవాహం = {};
  ప్రవాహం.తదుపరి_లోపల = (Bytef *) compressed;
  ప్రవాహం.avail_లోపల = compressed_len;

  ఒకవేళ (inflateInit2 (&ప్రవాహం, 16 + MAX_WBITS) != Z_OK)
    తిరిగి శూన్యసూచిక;
  HB_SCOPE_GUARD (inflate_చివర (&ప్రవాహం));

  uint32_t expected_కొలత = 0;
  hb_gzip_get_uncompressed_కొలత ((స్థిరరాశి అక్షరం *) compressed,
                                 compressed_len,
                                 &expected_కొలత);

  size_t allocated = hb_min ((size_t) hb_max (expected_కొలత, 4096u),
                             (size_t) max_output_len);
  అక్షరం *output = (అక్షరం *) hb_malloc (allocated);
  ఒకవేళ (!output)
    తిరిగి శూన్యసూచిక;
  స్వయంచాలక output_guard = hb_make_scope_guard ([&]() { hb_free (output); });

  పూర్ణాంకం స్థితి_2 = Z_OK;
  అయితే (నిజం)
  {
    size_t produced = (size_t) ప్రవాహం.మొత్తం_బహిర్గతం;
    ఒకవేళ (unlikely (produced >= (size_t) max_output_len))
      తిరిగి శూన్యసూచిక;

    ఒకవేళ (produced == allocated)
    {
      size_t new_allocated = hb_min (allocated * 2, (size_t) max_output_len);
      ఒకవేళ (unlikely (new_allocated <= allocated))
        తిరిగి శూన్యసూచిక;

      అక్షరం *new_output = (అక్షరం *) hb_realloc (output, new_allocated);
      ఒకవేళ (unlikely (!new_output))
        తిరిగి శూన్యసూచిక;

      output = new_output;
      allocated = new_allocated;
    }

    ప్రవాహం.తదుపరి_బహిర్గతం = (Bytef *) output + ప్రవాహం.మొత్తం_బహిర్గతం;
    ప్రవాహం.avail_బహిర్గతం = (uInt) (allocated - (size_t) ప్రవాహం.మొత్తం_బహిర్గతం);

    స్థితి_2 = inflate (&ప్రవాహం, Z_ముగింపు);
    ఒకవేళ (స్థితి_2 == Z_ప్రవాహం_చివర)
      ఆపు;

    ఒకవేళ ((స్థితి_2 == Z_OK || స్థితి_2 == Z_BUF_ERROR) && ప్రవాహం.avail_బహిర్గతం == 0)
      కొనసాగు;

    తిరిగి శూన్యసూచిక;
  }

  ఒకవేళ (unlikely ((size_t) ప్రవాహం.మొత్తం_బహిర్గతం > (size_t) max_output_len))
    తిరిగి శూన్యసూచిక;

  output_guard.release ();
  తిరిగి hb_blob_create_or_fail (output,
                                 (గుర్తులేని) ప్రవాహం.మొత్తం_బహిర్గతం,
                                 HB_జ్ఞాపకం_బహుళకం_WRITABLE,
                                 output,
                                 hb_free);
#అయితే_ముగింపు
}

#అయితే_ముగింపు /* HB_ZLIB_CC pacify */
#అయితే_ముగింపు /* HB_ZLIB_CC guard */
