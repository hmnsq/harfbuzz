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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_बफर_VERIFY

#समावेश "hb-buffer.hh"


#घोषणा बफर_VERIFY_ERROR "buffer verify error: "
अटल अंतरभूत व्योम
बफर_verify_error (hb_बफर_t *बफर,
		     hb_font_t *font,
		     स्थिर अक्षर *fmt,
		     ...) HB_PRINTF_FUNC(3, 4);

अटल अंतरभूत व्योम
बफर_verify_error (hb_बफर_t *बफर,
		     hb_font_t *font,
		     स्थिर अक्षर *fmt,
		     ...)
{
  बहु_सूची ap;
  बहु_शुरू (ap, fmt);
  अगर (बफर->messaging ())
  {
    बफर->message_impl (font, fmt, ap);
  }
  अन्यथा
  {
    ख_लिखो (मानक_त्रुटि, "harfbuzz ");
    भख_लिखो (मानक_त्रुटि, fmt, ap);
    ख_लिखो (मानक_त्रुटि, "\n");
  }
  बहु_पूर्ण (ap);
}

अटल bool
बफर_verify_monotone (hb_बफर_t *बफर,
			hb_font_t   *font)
{
  अगर (!HB_बफर_CLUSTER_LEVEL_IS_MONOTONE (बफर->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    वापस true;
  }

  bool is_forward = HB_DIRECTION_IS_FORWARD (hb_बफर_get_direction (बफर));

  अचिन्हित पूर्णांक अंक_glyphs;
  hb_glyph_info_t *info = hb_बफर_get_glyph_infos (बफर, &अंक_glyphs);

  क्रम (अचिन्हित पूर्णांक i = 1; i < अंक_glyphs; i++)
    अगर (info[i-1].cluster != info[i].cluster &&
	(info[i-1].cluster < info[i].cluster) != is_forward)
    {
      बफर_verify_error (बफर, font, बफर_VERIFY_ERROR "clusters are not monotone.");
      वापस false;
    }

  वापस true;
}

अटल bool
बफर_verify_unsafe_to_break (hb_बफर_t  *बफर,
			       hb_बफर_t  *text_बफर,
			       hb_font_t          *font,
			       स्थिर hb_feature_t *features,
			       अचिन्हित पूर्णांक        अंक_features,
			       स्थिर अक्षर * स्थिर *shapers)
{
  अगर (!HB_बफर_CLUSTER_LEVEL_IS_MONOTONE (बफर->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    वापस true;
  }

  /* Check that breaking up shaping at safe-to-break is indeed safe. */

  hb_unique_ptr_t<hb_बफर_t> fragment (hb_बफर_create_similar (बफर));
  hb_बफर_set_flags (fragment, (hb_बफर_flags_t (hb_बफर_get_flags (fragment) & ~HB_बफर_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_बफर_t> reconstruction (hb_बफर_create_similar (बफर));
  hb_बफर_set_flags (reconstruction, (hb_बफर_flags_t (hb_बफर_get_flags (reconstruction) & ~HB_बफर_FLAG_VERIFY)));

  अचिन्हित पूर्णांक अंक_glyphs;
  hb_glyph_info_t *info = hb_बफर_get_glyph_infos (बफर, &अंक_glyphs);

  अचिन्हित पूर्णांक अंक_chars;
  hb_glyph_info_t *text = hb_बफर_get_glyph_infos (text_बफर, &अंक_chars);

  /* Chop text and shape fragments. */
  bool forward = HB_DIRECTION_IS_FORWARD (hb_बफर_get_direction (बफर));
  अचिन्हित पूर्णांक start = 0;
  अचिन्हित पूर्णांक text_start = forward ? 0 : अंक_chars;
  अचिन्हित पूर्णांक text_end = text_start;
  क्रम (अचिन्हित पूर्णांक end = 1; end < अंक_glyphs + 1; end++)
  {
    अगर (end < अंक_glyphs &&
	(info[end].cluster == info[end-1].cluster ||
	 info[end-(forward?0:1)].mask & HB_GLYPH_FLAG_UNSAFE_TO_BREAK))
	जारी;

    /* Shape segment corresponding to glyphs start..end. */
    अगर (end == अंक_glyphs)
    {
      अगर (forward)
	text_end = अंक_chars;
      अन्यथा
	text_start = 0;
    }
    अन्यथा
    {
      अगर (forward)
      {
	अचिन्हित पूर्णांक cluster = info[end].cluster;
	जबतक (text_end < अंक_chars && text[text_end].cluster < cluster)
	  text_end++;
      }
      अन्यथा
      {
	अचिन्हित पूर्णांक cluster = info[end - 1].cluster;
	जबतक (text_start && text[text_start - 1].cluster >= cluster)
	  text_start--;
      }
    }
    अगर (unlikely (text_start >= text_end))
    {
      बफर_verify_error (बफर, font, बफर_VERIFY_ERROR "unsafe-to-break text range is invalid.");
      वापस false;
    }

    अगर (false)
      म_लिखो("start %u end %u text start %u end %u\n", start, end, text_start, text_end);

    hb_बफर_clear_contents (fragment);

    hb_बफर_flags_t flags = hb_बफर_get_flags (fragment);
    अगर (0 < text_start)
      flags = (hb_बफर_flags_t) (flags & ~HB_बफर_FLAG_BOT);
    अगर (text_end < अंक_chars)
      flags = (hb_बफर_flags_t) (flags & ~HB_बफर_FLAG_EOT);
    hb_बफर_set_flags (fragment, flags);

    hb_बफर_append (fragment, text_बफर, text_start, text_end);
    अगर (!hb_shape_full (font, fragment, features, अंक_features, shapers) ||
	!fragment->successful)
      वापस true;
    hb_बफर_append (reconstruction, fragment, 0, -1);

    start = end;
    अगर (forward)
      text_start = text_end;
    अन्यथा
      text_end = text_start;
  }

  bool ret = true;
  अगर (likely (reconstruction->successful))
  {
    hb_बफर_diff_flags_t diff = hb_बफर_diff (reconstruction, बफर, (hb_codepoint_t) -1, 0);
    अगर (diff & ~HB_बफर_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      बफर_verify_error (बफर, font, बफर_VERIFY_ERROR "unsafe-to-break test failed.");
      ret = false;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_बफर_set_लंबाई (बफर, 0);
      hb_बफर_append (बफर, reconstruction, 0, -1);
    }
  }

  वापस ret;
}

अटल bool
बफर_verify_unsafe_to_concat (hb_बफर_t        *बफर,
				hb_बफर_t        *text_बफर,
				hb_font_t          *font,
				स्थिर hb_feature_t *features,
				अचिन्हित पूर्णांक        अंक_features,
				स्थिर अक्षर * स्थिर *shapers)
{
  अगर (!HB_बफर_CLUSTER_LEVEL_IS_MONOTONE (बफर->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    वापस true;
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

  hb_unique_ptr_t<hb_बफर_t> fragments[2] {
    hb_unique_ptr_t<hb_बफर_t> (hb_बफर_create_similar (बफर)),
    hb_unique_ptr_t<hb_बफर_t> (hb_बफर_create_similar (बफर)),
  };
  hb_बफर_set_flags (fragments[0], (hb_बफर_flags_t (hb_बफर_get_flags (fragments[0]) & ~HB_बफर_FLAG_VERIFY)));
  hb_बफर_set_flags (fragments[1], (hb_बफर_flags_t (hb_बफर_get_flags (fragments[1]) & ~HB_बफर_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_बफर_t> reconstruction (hb_बफर_create_similar (बफर));
  hb_बफर_set_flags (reconstruction, (hb_बफर_flags_t (hb_बफर_get_flags (reconstruction) & ~HB_बफर_FLAG_VERIFY)));
  hb_segment_properties_t props;
  hb_बफर_get_segment_properties (बफर, &props);
  hb_बफर_set_segment_properties (fragments[0], &props);
  hb_बफर_set_segment_properties (fragments[1], &props);
  hb_बफर_set_segment_properties (reconstruction, &props);

  अचिन्हित अंक_glyphs;
  hb_glyph_info_t *info = hb_बफर_get_glyph_infos (बफर, &अंक_glyphs);

  अचिन्हित अंक_chars;
  hb_glyph_info_t *text = hb_बफर_get_glyph_infos (text_बफर, &अंक_chars);

  bool forward = HB_DIRECTION_IS_FORWARD (hb_बफर_get_direction (बफर));

  अगर (!forward)
    hb_बफर_reverse (बफर);

  /*
   * Split text into segments and collect into to fragment streams.
   */
  {
    अचिन्हित fragment_idx = 0;
    अचिन्हित start = 0;
    अचिन्हित text_start = 0;
    अचिन्हित text_end = 0;
    क्रम (अचिन्हित end = 1; end < अंक_glyphs + 1; end++)
    {
      अगर (end < अंक_glyphs &&
	  (info[end].cluster == info[end-1].cluster ||
	   info[end].mask & HB_GLYPH_FLAG_UNSAFE_TO_CONCAT))
	  जारी;

      /* Accumulate segment corresponding to glyphs start..end. */
      अगर (end == अंक_glyphs)
	text_end = अंक_chars;
      अन्यथा
      {
	अचिन्हित cluster = info[end].cluster;
	जबतक (text_end < अंक_chars && text[text_end].cluster < cluster)
	  text_end++;
      }
      निश्चित (text_start < text_end);

      अगर (false)
	म_लिखो("start %u end %u text start %u end %u\n", start, end, text_start, text_end);

#अगर 0
      hb_बफर_flags_t flags = hb_बफर_get_flags (fragment);
      अगर (0 < text_start)
	flags = (hb_बफर_flags_t) (flags & ~HB_बफर_FLAG_BOT);
      अगर (text_end < अंक_chars)
	flags = (hb_बफर_flags_t) (flags & ~HB_बफर_FLAG_EOT);
      hb_बफर_set_flags (fragment, flags);
#पूर्ण_अगर

      hb_बफर_append (fragments[fragment_idx], text_बफर, text_start, text_end);

      start = end;
      text_start = text_end;
      fragment_idx = 1 - fragment_idx;
    }
  }

  bool ret = true;
  /*
   * Shape the two fragment streams.
   */
  अगर (!hb_shape_full (font, fragments[0], features, अंक_features, shapers) ||
      !fragments[0]->successful)
    वापस ret;

  अगर (!hb_shape_full (font, fragments[1], features, अंक_features, shapers) ||
      !fragments[1]->successful)
    वापस ret;

  अगर (!forward)
  {
    hb_बफर_reverse (fragments[0]);
    hb_बफर_reverse (fragments[1]);
  }

  /*
   * Reconstruct results.
   */
  {
    अचिन्हित fragment_idx = 0;
    अचिन्हित fragment_start[2] {0, 0};
    अचिन्हित fragment_अंक_glyphs[2];
    hb_glyph_info_t *fragment_info[2];
    क्रम (अचिन्हित i = 0; i < 2; i++)
      fragment_info[i] = hb_बफर_get_glyph_infos (fragments[i], &fragment_अंक_glyphs[i]);
    जबतक (fragment_start[0] < fragment_अंक_glyphs[0] ||
	   fragment_start[1] < fragment_अंक_glyphs[1])
    {
      अचिन्हित fragment_end = fragment_start[fragment_idx] + 1;
      जबतक (fragment_end < fragment_अंक_glyphs[fragment_idx] &&
	     (fragment_info[fragment_idx][fragment_end].cluster == fragment_info[fragment_idx][fragment_end - 1].cluster ||
	      fragment_info[fragment_idx][fragment_end].mask & HB_GLYPH_FLAG_UNSAFE_TO_CONCAT))
	fragment_end++;

      hb_बफर_append (reconstruction, fragments[fragment_idx], fragment_start[fragment_idx], fragment_end);

      fragment_start[fragment_idx] = fragment_end;
      fragment_idx = 1 - fragment_idx;
    }
  }

  अगर (!forward)
  {
    hb_बफर_reverse (बफर);
    hb_बफर_reverse (reconstruction);
  }

  अगर (likely (reconstruction->successful))
  {
    /*
     * Diff results.
     */
    hb_बफर_diff_flags_t diff = hb_बफर_diff (reconstruction, बफर, (hb_codepoint_t) -1, 0);
    अगर (diff & ~HB_बफर_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      बफर_verify_error (बफर, font, बफर_VERIFY_ERROR "unsafe-to-concat test failed.");
      ret = false;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_बफर_set_लंबाई (बफर, 0);
      hb_बफर_append (बफर, reconstruction, 0, -1);
    }
  }

  वापस ret;
}

bool
hb_बफर_t::verify (hb_बफर_t        *text_बफर,
		     hb_font_t          *font,
		     स्थिर hb_feature_t *features,
		     अचिन्हित पूर्णांक        अंक_features,
		     स्थिर अक्षर * स्थिर *shapers)
{
  bool ret = true;
  bool monotone = बफर_verify_monotone (यह, font);
  अगर (!monotone)
    ret = false;
  अगर (monotone &&
      !बफर_verify_unsafe_to_break (यह, text_बफर, font, features, अंक_features, shapers))
    ret = false;
  अगर (monotone &&
      (flags & HB_बफर_FLAG_PRODUCE_UNSAFE_TO_CONCAT) != 0 &&
      !बफर_verify_unsafe_to_concat (यह, text_बफर, font, features, अंक_features, shapers))
    ret = false;
  अगर (!ret)
  {
#अगर_अघोषित HB_NO_बफर_SERIALIZE
    अचिन्हित len = text_बफर->len;
    hb_vector_t<अक्षर> bytes;
    अगर (likely (bytes.resize (len * 10 + 16)))
    {
      hb_बफर_serialize_unicode (text_बफर,
				   0, len,
				   bytes.arrayZ, bytes.लंबाई,
				   &len,
				   HB_बफर_SERIALIZE_FORMAT_TEXT,
				   HB_बफर_SERIALIZE_FLAG_NO_CLUSTERS);
      बफर_verify_error (यह, font, बफर_VERIFY_ERROR "text was: %s.", bytes.arrayZ ? bytes.arrayZ : "");
    }
#पूर्ण_अगर
  }
  वापस ret;
}


#पूर्ण_अगर
