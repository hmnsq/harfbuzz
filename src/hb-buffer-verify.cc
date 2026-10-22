/*
 * Copyright © 2022  Behdad Esfahbod
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_బఫరు_VERIFY

#అన్తర్భావ "hb-buffer.hh"


#నిర్వచించు బఫరు_VERIFY_ERROR "buffer verify error: "
స్థిర అంతఃవరుస శూన్య
బఫరు_verify_error (hb_బఫరు_t *బఫరు,
		     hb_font_t *font,
		     స్థిరరాశి అక్షరం *fmt,
		     ...) HB_PRINTF_FUNC(3, 4);

స్థిర అంతఃవరుస శూన్య
బఫరు_verify_error (hb_బఫరు_t *బఫరు,
		     hb_font_t *font,
		     స్థిరరాశి అక్షరం *fmt,
		     ...)
{
  va_list ap;
  va_ప్రారంభం (ap, fmt);
  ఒకవేళ (బఫరు->messaging ())
  {
    బఫరు->సందేశం_impl (font, fmt, ap);
  }
  లేకపోతే
  {
    fprintf (stderr, "harfbuzz ");
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
  }
  va_చివర (ap);
}

స్థిర తార్కిక
బఫరు_verify_monotone (hb_బఫరు_t *బఫరు,
			hb_font_t   *font)
{
  ఒకవేళ (!HB_బఫరు_CLUSTER_LEVEL_IS_MONOTONE (బఫరు->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    తిరిగి నిజం;
  }

  తార్కిక is_forward = HB_DIRECTION_IS_FORWARD (hb_బఫరు_get_direction (బఫరు));

  గుర్తులేని పూర్ణాంకం అంకె_glyphs;
  hb_glyph_info_t *info = hb_బఫరు_get_glyph_infos (బఫరు, &అంకె_glyphs);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < అంకె_glyphs; i++)
    ఒకవేళ (info[i-1].cluster != info[i].cluster &&
	(info[i-1].cluster < info[i].cluster) != is_forward)
    {
      బఫరు_verify_error (బఫరు, font, బఫరు_VERIFY_ERROR "clusters are not monotone.");
      తిరిగి అబద్ధం;
    }

  తిరిగి నిజం;
}

స్థిర తార్కిక
బఫరు_verify_unsafe_గమ్యం_break (hb_బఫరు_t  *బఫరు,
			       hb_బఫరు_t  *పాఠ్యం_బఫరు,
			       hb_font_t          *font,
			       స్థిరరాశి hb_feature_t *features,
			       గుర్తులేని పూర్ణాంకం        అంకె_features,
			       స్థిరరాశి అక్షరం * స్థిరరాశి *shapers)
{
  ఒకవేళ (!HB_బఫరు_CLUSTER_LEVEL_IS_MONOTONE (బఫరు->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    తిరిగి నిజం;
  }

  /* Check that breaking up shaping at safe-to-break is indeed safe. */

  hb_unique_ptr_t<hb_బఫరు_t> fragment (hb_బఫరు_create_similar (బఫరు));
  hb_బఫరు_సమితి_flags (fragment, (hb_బఫరు_flags_t (hb_బఫరు_get_flags (fragment) & ~HB_బఫరు_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_బఫరు_t> reconstruction (hb_బఫరు_create_similar (బఫరు));
  hb_బఫరు_సమితి_flags (reconstruction, (hb_బఫరు_flags_t (hb_బఫరు_get_flags (reconstruction) & ~HB_బఫరు_FLAG_VERIFY)));

  గుర్తులేని పూర్ణాంకం అంకె_glyphs;
  hb_glyph_info_t *info = hb_బఫరు_get_glyph_infos (బఫరు, &అంకె_glyphs);

  గుర్తులేని పూర్ణాంకం అంకె_chars;
  hb_glyph_info_t *పాఠ్యం = hb_బఫరు_get_glyph_infos (పాఠ్యం_బఫరు, &అంకె_chars);

  /* Chop text and shape fragments. */
  తార్కిక forward = HB_DIRECTION_IS_FORWARD (hb_బఫరు_get_direction (బఫరు));
  గుర్తులేని పూర్ణాంకం ప్రారంభం = 0;
  గుర్తులేని పూర్ణాంకం పాఠ్యం_ప్రారంభం = forward ? 0 : అంకె_chars;
  గుర్తులేని పూర్ణాంకం పాఠ్యం_చివర = పాఠ్యం_ప్రారంభం;
  కొరకు (గుర్తులేని పూర్ణాంకం చివర = 1; చివర < అంకె_glyphs + 1; చివర++)
  {
    ఒకవేళ (చివర < అంకె_glyphs &&
	(info[చివర].cluster == info[చివర-1].cluster ||
	 info[చివర-(forward?0:1)].ముసుగు & HB_GLYPH_FLAG_UNSAFE_గమ్యం_BREAK))
	కొనసాగు;

    /* Shape segment corresponding to glyphs start..end. */
    ఒకవేళ (చివర == అంకె_glyphs)
    {
      ఒకవేళ (forward)
	పాఠ్యం_చివర = అంకె_chars;
      లేకపోతే
	పాఠ్యం_ప్రారంభం = 0;
    }
    లేకపోతే
    {
      ఒకవేళ (forward)
      {
	గుర్తులేని పూర్ణాంకం cluster = info[చివర].cluster;
	అయితే (పాఠ్యం_చివర < అంకె_chars && పాఠ్యం[పాఠ్యం_చివర].cluster < cluster)
	  పాఠ్యం_చివర++;
      }
      లేకపోతే
      {
	గుర్తులేని పూర్ణాంకం cluster = info[చివర - 1].cluster;
	అయితే (పాఠ్యం_ప్రారంభం && పాఠ్యం[పాఠ్యం_ప్రారంభం - 1].cluster >= cluster)
	  పాఠ్యం_ప్రారంభం--;
      }
    }
    ఒకవేళ (unlikely (పాఠ్యం_ప్రారంభం >= పాఠ్యం_చివర))
    {
      బఫరు_verify_error (బఫరు, font, బఫరు_VERIFY_ERROR "unsafe-to-break text range is invalid.");
      తిరిగి అబద్ధం;
    }

    ఒకవేళ (అబద్ధం)
      రాయి("start %u end %u text start %u end %u\n", ప్రారంభం, చివర, పాఠ్యం_ప్రారంభం, పాఠ్యం_చివర);

    hb_బఫరు_clear_contents (fragment);

    hb_బఫరు_flags_t flags = hb_బఫరు_get_flags (fragment);
    ఒకవేళ (0 < పాఠ్యం_ప్రారంభం)
      flags = (hb_బఫరు_flags_t) (flags & ~HB_బఫరు_FLAG_BOT);
    ఒకవేళ (పాఠ్యం_చివర < అంకె_chars)
      flags = (hb_బఫరు_flags_t) (flags & ~HB_బఫరు_FLAG_EOT);
    hb_బఫరు_సమితి_flags (fragment, flags);

    hb_బఫరు_append (fragment, పాఠ్యం_బఫరు, పాఠ్యం_ప్రారంభం, పాఠ్యం_చివర);
    ఒకవేళ (!hb_shape_నిండిన (font, fragment, features, అంకె_features, shapers) ||
	!fragment->successful)
      తిరిగి నిజం;
    hb_బఫరు_append (reconstruction, fragment, 0, -1);

    ప్రారంభం = చివర;
    ఒకవేళ (forward)
      పాఠ్యం_ప్రారంభం = పాఠ్యం_చివర;
    లేకపోతే
      పాఠ్యం_చివర = పాఠ్యం_ప్రారంభం;
  }

  తార్కిక ret = నిజం;
  ఒకవేళ (likely (reconstruction->successful))
  {
    hb_బఫరు_diff_flags_t diff = hb_బఫరు_diff (reconstruction, బఫరు, (hb_codepoint_t) -1, 0);
    ఒకవేళ (diff & ~HB_బఫరు_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      బఫరు_verify_error (బఫరు, font, బఫరు_VERIFY_ERROR "unsafe-to-break test failed.");
      ret = అబద్ధం;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_బఫరు_సమితి_నిడివి (బఫరు, 0);
      hb_బఫరు_append (బఫరు, reconstruction, 0, -1);
    }
  }

  తిరిగి ret;
}

స్థిర తార్కిక
బఫరు_verify_unsafe_గమ్యం_concat (hb_బఫరు_t        *బఫరు,
				hb_బఫరు_t        *పాఠ్యం_బఫరు,
				hb_font_t          *font,
				స్థిరరాశి hb_feature_t *features,
				గుర్తులేని పూర్ణాంకం        అంకె_features,
				స్థిరరాశి అక్షరం * స్థిరరాశి *shapers)
{
  ఒకవేళ (!HB_బఫరు_CLUSTER_LEVEL_IS_MONOTONE (బఫరు->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    తిరిగి నిజం;
  }

  /* Check that shuffling up text before shaping at safe-to-concat points
   * is indeed safe. */

  /* This is what we do:
   *
   * 1. We shape text once. Then segment the text at all the safe-to-concat
   *    points;
   *
   * 2. Then we create two buffers, one containing all the even segments and
   *    one all the odd segments.
   *
   * 3. Because all these segments were safe-to-concat at both ends, we
   *    expect that concatenating them and shaping should NOT change the
   *    shaping results of each segment.  As such, we expect that after
   *    shaping the two buffers, we still get cluster boundaries at the
   *    segment boundaries, and that those all are safe-to-concat points.
   *    Moreover, that there are NOT any safe-to-concat points within the
   *    segments.
   *
   * 4. Finally, we reconstruct the shaping results of the original text by
   *    simply interleaving the shaping results of the segments from the two
   *    buffers, and assert that the total shaping results is the same as
   *    the one from original buffer in step 1.
   */

  hb_unique_ptr_t<hb_బఫరు_t> fragments[2] {
    hb_unique_ptr_t<hb_బఫరు_t> (hb_బఫరు_create_similar (బఫరు)),
    hb_unique_ptr_t<hb_బఫరు_t> (hb_బఫరు_create_similar (బఫరు)),
  };
  hb_బఫరు_సమితి_flags (fragments[0], (hb_బఫరు_flags_t (hb_బఫరు_get_flags (fragments[0]) & ~HB_బఫరు_FLAG_VERIFY)));
  hb_బఫరు_సమితి_flags (fragments[1], (hb_బఫరు_flags_t (hb_బఫరు_get_flags (fragments[1]) & ~HB_బఫరు_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_బఫరు_t> reconstruction (hb_బఫరు_create_similar (బఫరు));
  hb_బఫరు_సమితి_flags (reconstruction, (hb_బఫరు_flags_t (hb_బఫరు_get_flags (reconstruction) & ~HB_బఫరు_FLAG_VERIFY)));
  hb_segment_properties_t props;
  hb_బఫరు_get_segment_properties (బఫరు, &props);
  hb_బఫరు_సమితి_segment_properties (fragments[0], &props);
  hb_బఫరు_సమితి_segment_properties (fragments[1], &props);
  hb_బఫరు_సమితి_segment_properties (reconstruction, &props);

  గుర్తులేని అంకె_glyphs;
  hb_glyph_info_t *info = hb_బఫరు_get_glyph_infos (బఫరు, &అంకె_glyphs);

  గుర్తులేని అంకె_chars;
  hb_glyph_info_t *పాఠ్యం = hb_బఫరు_get_glyph_infos (పాఠ్యం_బఫరు, &అంకె_chars);

  తార్కిక forward = HB_DIRECTION_IS_FORWARD (hb_బఫరు_get_direction (బఫరు));

  ఒకవేళ (!forward)
    hb_బఫరు_తిరగేయి (బఫరు);

  /*
   * Split text into segments and collect into to fragment streams.
   */
  {
    గుర్తులేని fragment_idx = 0;
    గుర్తులేని ప్రారంభం = 0;
    గుర్తులేని పాఠ్యం_ప్రారంభం = 0;
    గుర్తులేని పాఠ్యం_చివర = 0;
    కొరకు (గుర్తులేని చివర = 1; చివర < అంకె_glyphs + 1; చివర++)
    {
      ఒకవేళ (చివర < అంకె_glyphs &&
	  (info[చివర].cluster == info[చివర-1].cluster ||
	   info[చివర].ముసుగు & HB_GLYPH_FLAG_UNSAFE_గమ్యం_CONCAT))
	  కొనసాగు;

      /* Accumulate segment corresponding to glyphs start..end. */
      ఒకవేళ (చివర == అంకె_glyphs)
	పాఠ్యం_చివర = అంకె_chars;
      లేకపోతే
      {
	గుర్తులేని cluster = info[చివర].cluster;
	అయితే (పాఠ్యం_చివర < అంకె_chars && పాఠ్యం[పాఠ్యం_చివర].cluster < cluster)
	  పాఠ్యం_చివర++;
      }
      ప్రతిజ్ఞా (పాఠ్యం_ప్రారంభం < పాఠ్యం_చివర);

      ఒకవేళ (అబద్ధం)
	రాయి("start %u end %u text start %u end %u\n", ప్రారంభం, చివర, పాఠ్యం_ప్రారంభం, పాఠ్యం_చివర);

#ఒకవేళ 0
      hb_బఫరు_flags_t flags = hb_బఫరు_get_flags (fragment);
      ఒకవేళ (0 < పాఠ్యం_ప్రారంభం)
	flags = (hb_బఫరు_flags_t) (flags & ~HB_బఫరు_FLAG_BOT);
      ఒకవేళ (పాఠ్యం_చివర < అంకె_chars)
	flags = (hb_బఫరు_flags_t) (flags & ~HB_బఫరు_FLAG_EOT);
      hb_బఫరు_సమితి_flags (fragment, flags);
#అయితే_ముగింపు

      hb_బఫరు_append (fragments[fragment_idx], పాఠ్యం_బఫరు, పాఠ్యం_ప్రారంభం, పాఠ్యం_చివర);

      ప్రారంభం = చివర;
      పాఠ్యం_ప్రారంభం = పాఠ్యం_చివర;
      fragment_idx = 1 - fragment_idx;
    }
  }

  తార్కిక ret = నిజం;
  /*
   * Shape the two fragment streams.
   */
  ఒకవేళ (!hb_shape_నిండిన (font, fragments[0], features, అంకె_features, shapers) ||
      !fragments[0]->successful)
    తిరిగి ret;

  ఒకవేళ (!hb_shape_నిండిన (font, fragments[1], features, అంకె_features, shapers) ||
      !fragments[1]->successful)
    తిరిగి ret;

  ఒకవేళ (!forward)
  {
    hb_బఫరు_తిరగేయి (fragments[0]);
    hb_బఫరు_తిరగేయి (fragments[1]);
  }

  /*
   * Reconstruct results.
   */
  {
    గుర్తులేని fragment_idx = 0;
    గుర్తులేని fragment_ప్రారంభం[2] {0, 0};
    గుర్తులేని fragment_అంకె_glyphs[2];
    hb_glyph_info_t *fragment_info[2];
    కొరకు (గుర్తులేని i = 0; i < 2; i++)
      fragment_info[i] = hb_బఫరు_get_glyph_infos (fragments[i], &fragment_అంకె_glyphs[i]);
    అయితే (fragment_ప్రారంభం[0] < fragment_అంకె_glyphs[0] ||
	   fragment_ప్రారంభం[1] < fragment_అంకె_glyphs[1])
    {
      గుర్తులేని fragment_చివర = fragment_ప్రారంభం[fragment_idx] + 1;
      అయితే (fragment_చివర < fragment_అంకె_glyphs[fragment_idx] &&
	     (fragment_info[fragment_idx][fragment_చివర].cluster == fragment_info[fragment_idx][fragment_చివర - 1].cluster ||
	      fragment_info[fragment_idx][fragment_చివర].ముసుగు & HB_GLYPH_FLAG_UNSAFE_గమ్యం_CONCAT))
	fragment_చివర++;

      hb_బఫరు_append (reconstruction, fragments[fragment_idx], fragment_ప్రారంభం[fragment_idx], fragment_చివర);

      fragment_ప్రారంభం[fragment_idx] = fragment_చివర;
      fragment_idx = 1 - fragment_idx;
    }
  }

  ఒకవేళ (!forward)
  {
    hb_బఫరు_తిరగేయి (బఫరు);
    hb_బఫరు_తిరగేయి (reconstruction);
  }

  ఒకవేళ (likely (reconstruction->successful))
  {
    /*
     * Diff results.
     */
    hb_బఫరు_diff_flags_t diff = hb_బఫరు_diff (reconstruction, బఫరు, (hb_codepoint_t) -1, 0);
    ఒకవేళ (diff & ~HB_బఫరు_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      బఫరు_verify_error (బఫరు, font, బఫరు_VERIFY_ERROR "unsafe-to-concat test failed.");
      ret = అబద్ధం;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_బఫరు_సమితి_నిడివి (బఫరు, 0);
      hb_బఫరు_append (బఫరు, reconstruction, 0, -1);
    }
  }

  తిరిగి ret;
}

తార్కిక
hb_బఫరు_t::verify (hb_బఫరు_t        *పాఠ్యం_బఫరు,
		     hb_font_t          *font,
		     స్థిరరాశి hb_feature_t *features,
		     గుర్తులేని పూర్ణాంకం        అంకె_features,
		     స్థిరరాశి అక్షరం * స్థిరరాశి *shapers)
{
  తార్కిక ret = నిజం;
  తార్కిక monotone = బఫరు_verify_monotone (ఇది, font);
  ఒకవేళ (!monotone)
    ret = అబద్ధం;
  ఒకవేళ (monotone &&
      !బఫరు_verify_unsafe_గమ్యం_break (ఇది, పాఠ్యం_బఫరు, font, features, అంకె_features, shapers))
    ret = అబద్ధం;
  ఒకవేళ (monotone &&
      (flags & HB_బఫరు_FLAG_PRODUCE_UNSAFE_గమ్యం_CONCAT) != 0 &&
      !బఫరు_verify_unsafe_గమ్యం_concat (ఇది, పాఠ్యం_బఫరు, font, features, అంకె_features, shapers))
    ret = అబద్ధం;
  ఒకవేళ (!ret)
  {
#నిర్వచితం_కాకపోతే HB_NO_బఫరు_SERIALIZE
    గుర్తులేని len = పాఠ్యం_బఫరు->len;
    hb_సదిశ_t<అక్షరం> బైట్లు;
    ఒకవేళ (likely (బైట్లు.resize (len * 10 + 16)))
    {
      hb_బఫరు_serialize_unicode (పాఠ్యం_బఫరు,
				   0, len,
				   బైట్లు.arrayZ, బైట్లు.నిడివి,
				   &len,
				   HB_బఫరు_SERIALIZE_FORMAT_పాఠ్యం,
				   HB_బఫరు_SERIALIZE_FLAG_NO_CLUSTERS);
      బఫరు_verify_error (ఇది, font, బఫరు_VERIFY_ERROR "text was: %s.", బైట్లు.arrayZ ? బైట్లు.arrayZ : "");
    }
#అయితే_ముగింపు
  }
  తిరిగి ret;
}


#అయితే_ముగింపు
