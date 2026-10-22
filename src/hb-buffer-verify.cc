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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_BUFFER_VERIFY

#अन्तर्भाव "hb-buffer.hh"


#define BUFFER_VERIFY_ERROR "buffer verify error: "
स्थिर ओळीत शून्य
buffer_verify_error (hb_buffer_t *buffer,
		     hb_font_t *font,
		     स्थिरांक अक्षर *fmt,
		     ...) HB_PRINTF_FUNC(3, 4);

स्थिर ओळीत शून्य
buffer_verify_error (hb_buffer_t *buffer,
		     hb_font_t *font,
		     स्थिरांक अक्षर *fmt,
		     ...)
{
  बहु_सूची ap;
  बहु_शुरू (ap, fmt);
  जर (buffer->messaging ())
  {
    buffer->message_impl (font, fmt, ap);
  }
  अन्यथा
  {
    ख_लिखो (मानक_त्रुटि, "harfbuzz ");
    भख_लिखो (मानक_त्रुटि, fmt, ap);
    ख_लिखो (मानक_त्रुटि, "\n");
  }
  बहु_पूर्ण (ap);
}

स्थिर तार्किक
buffer_verify_monotone (hb_buffer_t *buffer,
			hb_font_t   *font)
{
  जर (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    परत सत्य;
  }

  तार्किक is_forward = HB_DIRECTION_IS_FORWARD (hb_buffer_get_direction (buffer));

  अचिन्हित पूर्णांक अंक_glyphs;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &अंक_glyphs);

  साठी (अचिन्हित पूर्णांक i = 1; i < अंक_glyphs; i++)
    जर (info[i-1].cluster != info[i].cluster &&
	(info[i-1].cluster < info[i].cluster) != is_forward)
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "clusters are not monotone.");
      परत असत्य;
    }

  परत सत्य;
}

स्थिर तार्किक
buffer_verify_unsafe_to_break (hb_buffer_t  *buffer,
			       hb_buffer_t  *text_buffer,
			       hb_font_t          *font,
			       स्थिरांक hb_feature_t *features,
			       अचिन्हित पूर्णांक        अंक_features,
			       स्थिरांक अक्षर * स्थिरांक *shapers)
{
  जर (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    परत सत्य;
  }

  /* Check that breaking up shaping at safe-to-break is indeed safe. */

  hb_unique_ptr_t<hb_buffer_t> fragment (hb_buffer_create_similar (buffer));
  hb_buffer_set_flags (fragment, (hb_buffer_flags_t (hb_buffer_get_flags (fragment) & ~HB_BUFFER_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_buffer_t> reconstruction (hb_buffer_create_similar (buffer));
  hb_buffer_set_flags (reconstruction, (hb_buffer_flags_t (hb_buffer_get_flags (reconstruction) & ~HB_BUFFER_FLAG_VERIFY)));

  अचिन्हित पूर्णांक अंक_glyphs;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &अंक_glyphs);

  अचिन्हित पूर्णांक अंक_chars;
  hb_glyph_info_t *text = hb_buffer_get_glyph_infos (text_buffer, &अंक_chars);

  /* Chop text and shape fragments. */
  तार्किक forward = HB_DIRECTION_IS_FORWARD (hb_buffer_get_direction (buffer));
  अचिन्हित पूर्णांक start = 0;
  अचिन्हित पूर्णांक text_start = forward ? 0 : अंक_chars;
  अचिन्हित पूर्णांक text_end = text_start;
  साठी (अचिन्हित पूर्णांक end = 1; end < अंक_glyphs + 1; end++)
  {
    जर (end < अंक_glyphs &&
	(info[end].cluster == info[end-1].cluster ||
	 info[end-(forward?0:1)].mask & HB_GLYPH_FLAG_UNSAFE_TO_BREAK))
	सुरूठेवा;

    /* Shape segment corresponding to glyphs start..end. */
    जर (end == अंक_glyphs)
    {
      जर (forward)
	text_end = अंक_chars;
      अन्यथा
	text_start = 0;
    }
    अन्यथा
    {
      जर (forward)
      {
	अचिन्हित पूर्णांक cluster = info[end].cluster;
	पर्यंत (text_end < अंक_chars && text[text_end].cluster < cluster)
	  text_end++;
      }
      अन्यथा
      {
	अचिन्हित पूर्णांक cluster = info[end - 1].cluster;
	पर्यंत (text_start && text[text_start - 1].cluster >= cluster)
	  text_start--;
      }
    }
    जर (unlikely (text_start >= text_end))
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "unsafe-to-break text range is invalid.");
      परत असत्य;
    }

    जर (असत्य)
      म_लिखो("start %u end %u text start %u end %u\n", start, end, text_start, text_end);

    hb_buffer_clear_contents (fragment);

    hb_buffer_flags_t flags = hb_buffer_get_flags (fragment);
    जर (0 < text_start)
      flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_BOT);
    जर (text_end < अंक_chars)
      flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_EOT);
    hb_buffer_set_flags (fragment, flags);

    hb_buffer_append (fragment, text_buffer, text_start, text_end);
    जर (!hb_shape_full (font, fragment, features, अंक_features, shapers) ||
	!fragment->successful)
      परत सत्य;
    hb_buffer_append (reconstruction, fragment, 0, -1);

    start = end;
    जर (forward)
      text_start = text_end;
    अन्यथा
      text_end = text_start;
  }

  तार्किक ret = सत्य;
  जर (likely (reconstruction->successful))
  {
    hb_buffer_diff_flags_t diff = hb_buffer_diff (reconstruction, buffer, (hb_codepoint_t) -1, 0);
    जर (diff & ~HB_BUFFER_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "unsafe-to-break test failed.");
      ret = असत्य;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_buffer_set_length (buffer, 0);
      hb_buffer_append (buffer, reconstruction, 0, -1);
    }
  }

  परत ret;
}

स्थिर तार्किक
buffer_verify_unsafe_to_concat (hb_buffer_t        *buffer,
				hb_buffer_t        *text_buffer,
				hb_font_t          *font,
				स्थिरांक hb_feature_t *features,
				अचिन्हित पूर्णांक        अंक_features,
				स्थिरांक अक्षर * स्थिरांक *shapers)
{
  जर (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    परत सत्य;
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

  hb_unique_ptr_t<hb_buffer_t> fragments[2] {
    hb_unique_ptr_t<hb_buffer_t> (hb_buffer_create_similar (buffer)),
    hb_unique_ptr_t<hb_buffer_t> (hb_buffer_create_similar (buffer)),
  };
  hb_buffer_set_flags (fragments[0], (hb_buffer_flags_t (hb_buffer_get_flags (fragments[0]) & ~HB_BUFFER_FLAG_VERIFY)));
  hb_buffer_set_flags (fragments[1], (hb_buffer_flags_t (hb_buffer_get_flags (fragments[1]) & ~HB_BUFFER_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_buffer_t> reconstruction (hb_buffer_create_similar (buffer));
  hb_buffer_set_flags (reconstruction, (hb_buffer_flags_t (hb_buffer_get_flags (reconstruction) & ~HB_BUFFER_FLAG_VERIFY)));
  hb_segment_properties_t props;
  hb_buffer_get_segment_properties (buffer, &props);
  hb_buffer_set_segment_properties (fragments[0], &props);
  hb_buffer_set_segment_properties (fragments[1], &props);
  hb_buffer_set_segment_properties (reconstruction, &props);

  अचिन्हित अंक_glyphs;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &अंक_glyphs);

  अचिन्हित अंक_chars;
  hb_glyph_info_t *text = hb_buffer_get_glyph_infos (text_buffer, &अंक_chars);

  तार्किक forward = HB_DIRECTION_IS_FORWARD (hb_buffer_get_direction (buffer));

  जर (!forward)
    hb_buffer_reverse (buffer);

  /*
   * Split text into segments and collect into to fragment streams.
   */
  {
    अचिन्हित fragment_idx = 0;
    अचिन्हित start = 0;
    अचिन्हित text_start = 0;
    अचिन्हित text_end = 0;
    साठी (अचिन्हित end = 1; end < अंक_glyphs + 1; end++)
    {
      जर (end < अंक_glyphs &&
	  (info[end].cluster == info[end-1].cluster ||
	   info[end].mask & HB_GLYPH_FLAG_UNSAFE_TO_CONCAT))
	  सुरूठेवा;

      /* Accumulate segment corresponding to glyphs start..end. */
      जर (end == अंक_glyphs)
	text_end = अंक_chars;
      अन्यथा
      {
	अचिन्हित cluster = info[end].cluster;
	पर्यंत (text_end < अंक_chars && text[text_end].cluster < cluster)
	  text_end++;
      }
      प्रतिज्ञा (text_start < text_end);

      जर (असत्य)
	म_लिखो("start %u end %u text start %u end %u\n", start, end, text_start, text_end);

#जर 0
      hb_buffer_flags_t flags = hb_buffer_get_flags (fragment);
      जर (0 < text_start)
	flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_BOT);
      जर (text_end < अंक_chars)
	flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_EOT);
      hb_buffer_set_flags (fragment, flags);
#endif

      hb_buffer_append (fragments[fragment_idx], text_buffer, text_start, text_end);

      start = end;
      text_start = text_end;
      fragment_idx = 1 - fragment_idx;
    }
  }

  तार्किक ret = सत्य;
  /*
   * Shape the two fragment streams.
   */
  जर (!hb_shape_full (font, fragments[0], features, अंक_features, shapers) ||
      !fragments[0]->successful)
    परत ret;

  जर (!hb_shape_full (font, fragments[1], features, अंक_features, shapers) ||
      !fragments[1]->successful)
    परत ret;

  जर (!forward)
  {
    hb_buffer_reverse (fragments[0]);
    hb_buffer_reverse (fragments[1]);
  }

  /*
   * Reconstruct results.
   */
  {
    अचिन्हित fragment_idx = 0;
    अचिन्हित fragment_start[2] {0, 0};
    अचिन्हित fragment_अंक_glyphs[2];
    hb_glyph_info_t *fragment_info[2];
    साठी (अचिन्हित i = 0; i < 2; i++)
      fragment_info[i] = hb_buffer_get_glyph_infos (fragments[i], &fragment_अंक_glyphs[i]);
    पर्यंत (fragment_start[0] < fragment_अंक_glyphs[0] ||
	   fragment_start[1] < fragment_अंक_glyphs[1])
    {
      अचिन्हित fragment_end = fragment_start[fragment_idx] + 1;
      पर्यंत (fragment_end < fragment_अंक_glyphs[fragment_idx] &&
	     (fragment_info[fragment_idx][fragment_end].cluster == fragment_info[fragment_idx][fragment_end - 1].cluster ||
	      fragment_info[fragment_idx][fragment_end].mask & HB_GLYPH_FLAG_UNSAFE_TO_CONCAT))
	fragment_end++;

      hb_buffer_append (reconstruction, fragments[fragment_idx], fragment_start[fragment_idx], fragment_end);

      fragment_start[fragment_idx] = fragment_end;
      fragment_idx = 1 - fragment_idx;
    }
  }

  जर (!forward)
  {
    hb_buffer_reverse (buffer);
    hb_buffer_reverse (reconstruction);
  }

  जर (likely (reconstruction->successful))
  {
    /*
     * Diff results.
     */
    hb_buffer_diff_flags_t diff = hb_buffer_diff (reconstruction, buffer, (hb_codepoint_t) -1, 0);
    जर (diff & ~HB_BUFFER_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "unsafe-to-concat test failed.");
      ret = असत्य;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_buffer_set_length (buffer, 0);
      hb_buffer_append (buffer, reconstruction, 0, -1);
    }
  }

  परत ret;
}

तार्किक
hb_buffer_t::verify (hb_buffer_t        *text_buffer,
		     hb_font_t          *font,
		     स्थिरांक hb_feature_t *features,
		     अचिन्हित पूर्णांक        अंक_features,
		     स्थिरांक अक्षर * स्थिरांक *shapers)
{
  तार्किक ret = सत्य;
  तार्किक monotone = buffer_verify_monotone (हे, font);
  जर (!monotone)
    ret = असत्य;
  जर (monotone &&
      !buffer_verify_unsafe_to_break (हे, text_buffer, font, features, अंक_features, shapers))
    ret = असत्य;
  जर (monotone &&
      (flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) != 0 &&
      !buffer_verify_unsafe_to_concat (हे, text_buffer, font, features, अंक_features, shapers))
    ret = असत्य;
  जर (!ret)
  {
#ifndef HB_NO_BUFFER_SERIALIZE
    अचिन्हित len = text_buffer->len;
    hb_vector_t<अक्षर> bytes;
    जर (likely (bytes.resize (len * 10 + 16)))
    {
      hb_buffer_serialize_unicode (text_buffer,
				   0, len,
				   bytes.arrayZ, bytes.length,
				   &len,
				   HB_BUFFER_SERIALIZE_FORMAT_TEXT,
				   HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS);
      buffer_verify_error (हे, font, BUFFER_VERIFY_ERROR "text was: %s.", bytes.arrayZ ? bytes.arrayZ : "");
    }
#endif
  }
  परत ret;
}


#endif
