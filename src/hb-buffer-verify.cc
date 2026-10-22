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

#ضمن "hb.hh"

#ifndef HB_NO_BUFFER_VERIFY

#ضمن "hb-buffer.hh"


#define BUFFER_VERIFY_ERROR "buffer verify error: "
ساكن مضمن فراغ
buffer_verify_error (hb_buffer_t *buffer,
		     hb_font_t *font,
		     ثابت حرف *fmt,
		     ...) HB_PRINTF_FUNC(3, 4);

ساكن مضمن فراغ
buffer_verify_error (hb_buffer_t *buffer,
		     hb_font_t *font,
		     ثابت حرف *fmt,
		     ...)
{
  va_list ap;
  va_start (ap, fmt);
  إذا (buffer->messaging ())
  {
    buffer->message_impl (font, fmt, ap);
  }
  وإلا
  {
    fprintf (stderr, "harfbuzz ");
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
  }
  va_end (ap);
}

ساكن منطقي
buffer_verify_monotone (hb_buffer_t *buffer,
			hb_font_t   *font)
{
  إذا (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    أرجع true;
  }

  منطقي is_forward = HB_DIRECTION_IS_FORWARD (hb_buffer_get_direction (buffer));

  unsigned صحيح لمب_glyphs;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &لمب_glyphs);

  لكل (unsigned صحيح i = 1; i < لمب_glyphs; i++)
    إذا (info[i-1].cluster != info[i].cluster &&
	(info[i-1].cluster < info[i].cluster) != is_forward)
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "clusters are not monotone.");
      أرجع خطأ;
    }

  أرجع true;
}

ساكن منطقي
buffer_verify_unsafe_to_break (hb_buffer_t  *buffer,
			       hb_buffer_t  *text_buffer,
			       hb_font_t          *font,
			       ثابت hb_feature_t *features,
			       unsigned صحيح        لمب_features,
			       ثابت حرف * ثابت *shapers)
{
  إذا (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    أرجع true;
  }

  /* Check that breaking up shaping at safe-to-break is indeed safe. */

  hb_unique_ptr_t<hb_buffer_t> fragment (hb_buffer_create_similar (buffer));
  hb_buffer_set_flags (fragment, (hb_buffer_flags_t (hb_buffer_get_flags (fragment) & ~HB_BUFFER_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_buffer_t> reconstruction (hb_buffer_create_similar (buffer));
  hb_buffer_set_flags (reconstruction, (hb_buffer_flags_t (hb_buffer_get_flags (reconstruction) & ~HB_BUFFER_FLAG_VERIFY)));

  unsigned صحيح لمب_glyphs;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &لمب_glyphs);

  unsigned صحيح لمب_chars;
  hb_glyph_info_t *text = hb_buffer_get_glyph_infos (text_buffer, &لمب_chars);

  /* Chop text and shape fragments. */
  منطقي forward = HB_DIRECTION_IS_FORWARD (hb_buffer_get_direction (buffer));
  unsigned صحيح start = 0;
  unsigned صحيح text_start = forward ? 0 : لمب_chars;
  unsigned صحيح text_end = text_start;
  لكل (unsigned صحيح end = 1; end < لمب_glyphs + 1; end++)
  {
    إذا (end < لمب_glyphs &&
	(info[end].cluster == info[end-1].cluster ||
	 info[end-(forward?0:1)].mask & HB_GLYPH_FLAG_UNSAFE_TO_BREAK))
	استمر;

    /* Shape segment corresponding to glyphs start..end. */
    إذا (end == لمب_glyphs)
    {
      إذا (forward)
	text_end = لمب_chars;
      وإلا
	text_start = 0;
    }
    وإلا
    {
      إذا (forward)
      {
	unsigned صحيح cluster = info[end].cluster;
	طالما (text_end < لمب_chars && text[text_end].cluster < cluster)
	  text_end++;
      }
      وإلا
      {
	unsigned صحيح cluster = info[end - 1].cluster;
	طالما (text_start && text[text_start - 1].cluster >= cluster)
	  text_start--;
      }
    }
    إذا (unlikely (text_start >= text_end))
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "unsafe-to-break text range is invalid.");
      أرجع خطأ;
    }

    إذا (خطأ)
      printf("start %u end %u text start %u end %u\n", start, end, text_start, text_end);

    hb_buffer_clear_contents (fragment);

    hb_buffer_flags_t flags = hb_buffer_get_flags (fragment);
    إذا (0 < text_start)
      flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_BOT);
    إذا (text_end < لمب_chars)
      flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_EOT);
    hb_buffer_set_flags (fragment, flags);

    hb_buffer_append (fragment, text_buffer, text_start, text_end);
    إذا (!hb_shape_full (font, fragment, features, لمب_features, shapers) ||
	!fragment->successful)
      أرجع true;
    hb_buffer_append (reconstruction, fragment, 0, -1);

    start = end;
    إذا (forward)
      text_start = text_end;
    وإلا
      text_end = text_start;
  }

  منطقي ret = true;
  إذا (likely (reconstruction->successful))
  {
    hb_buffer_diff_flags_t diff = hb_buffer_diff (reconstruction, buffer, (hb_codepoint_t) -1, 0);
    إذا (diff & ~HB_BUFFER_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "unsafe-to-break test failed.");
      ret = خطأ;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_buffer_set_length (buffer, 0);
      hb_buffer_append (buffer, reconstruction, 0, -1);
    }
  }

  أرجع ret;
}

ساكن منطقي
buffer_verify_unsafe_to_concat (hb_buffer_t        *buffer,
				hb_buffer_t        *text_buffer,
				hb_font_t          *font,
				ثابت hb_feature_t *features,
				unsigned صحيح        لمب_features,
				ثابت حرف * ثابت *shapers)
{
  إذا (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    أرجع true;
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

  unsigned لمب_glyphs;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &لمب_glyphs);

  unsigned لمب_chars;
  hb_glyph_info_t *text = hb_buffer_get_glyph_infos (text_buffer, &لمب_chars);

  منطقي forward = HB_DIRECTION_IS_FORWARD (hb_buffer_get_direction (buffer));

  إذا (!forward)
    hb_buffer_reverse (buffer);

  /*
   * Split text into segments and collect into to fragment streams.
   */
  {
    unsigned fragment_idx = 0;
    unsigned start = 0;
    unsigned text_start = 0;
    unsigned text_end = 0;
    لكل (unsigned end = 1; end < لمب_glyphs + 1; end++)
    {
      إذا (end < لمب_glyphs &&
	  (info[end].cluster == info[end-1].cluster ||
	   info[end].mask & HB_GLYPH_FLAG_UNSAFE_TO_CONCAT))
	  استمر;

      /* Accumulate segment corresponding to glyphs start..end. */
      إذا (end == لمب_glyphs)
	text_end = لمب_chars;
      وإلا
      {
	unsigned cluster = info[end].cluster;
	طالما (text_end < لمب_chars && text[text_end].cluster < cluster)
	  text_end++;
      }
      أكد (text_start < text_end);

      إذا (خطأ)
	printf("start %u end %u text start %u end %u\n", start, end, text_start, text_end);

#إذا 0
      hb_buffer_flags_t flags = hb_buffer_get_flags (fragment);
      إذا (0 < text_start)
	flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_BOT);
      إذا (text_end < لمب_chars)
	flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_EOT);
      hb_buffer_set_flags (fragment, flags);
#endif

      hb_buffer_append (fragments[fragment_idx], text_buffer, text_start, text_end);

      start = end;
      text_start = text_end;
      fragment_idx = 1 - fragment_idx;
    }
  }

  منطقي ret = true;
  /*
   * Shape the two fragment streams.
   */
  إذا (!hb_shape_full (font, fragments[0], features, لمب_features, shapers) ||
      !fragments[0]->successful)
    أرجع ret;

  إذا (!hb_shape_full (font, fragments[1], features, لمب_features, shapers) ||
      !fragments[1]->successful)
    أرجع ret;

  إذا (!forward)
  {
    hb_buffer_reverse (fragments[0]);
    hb_buffer_reverse (fragments[1]);
  }

  /*
   * Reconstruct results.
   */
  {
    unsigned fragment_idx = 0;
    unsigned fragment_start[2] {0, 0};
    unsigned fragment_لمب_glyphs[2];
    hb_glyph_info_t *fragment_info[2];
    لكل (unsigned i = 0; i < 2; i++)
      fragment_info[i] = hb_buffer_get_glyph_infos (fragments[i], &fragment_لمب_glyphs[i]);
    طالما (fragment_start[0] < fragment_لمب_glyphs[0] ||
	   fragment_start[1] < fragment_لمب_glyphs[1])
    {
      unsigned fragment_end = fragment_start[fragment_idx] + 1;
      طالما (fragment_end < fragment_لمب_glyphs[fragment_idx] &&
	     (fragment_info[fragment_idx][fragment_end].cluster == fragment_info[fragment_idx][fragment_end - 1].cluster ||
	      fragment_info[fragment_idx][fragment_end].mask & HB_GLYPH_FLAG_UNSAFE_TO_CONCAT))
	fragment_end++;

      hb_buffer_append (reconstruction, fragments[fragment_idx], fragment_start[fragment_idx], fragment_end);

      fragment_start[fragment_idx] = fragment_end;
      fragment_idx = 1 - fragment_idx;
    }
  }

  إذا (!forward)
  {
    hb_buffer_reverse (buffer);
    hb_buffer_reverse (reconstruction);
  }

  إذا (likely (reconstruction->successful))
  {
    /*
     * Diff results.
     */
    hb_buffer_diff_flags_t diff = hb_buffer_diff (reconstruction, buffer, (hb_codepoint_t) -1, 0);
    إذا (diff & ~HB_BUFFER_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "unsafe-to-concat test failed.");
      ret = خطأ;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_buffer_set_length (buffer, 0);
      hb_buffer_append (buffer, reconstruction, 0, -1);
    }
  }

  أرجع ret;
}

منطقي
hb_buffer_t::verify (hb_buffer_t        *text_buffer,
		     hb_font_t          *font,
		     ثابت hb_feature_t *features,
		     unsigned صحيح        لمب_features,
		     ثابت حرف * ثابت *shapers)
{
  منطقي ret = true;
  منطقي monotone = buffer_verify_monotone (هذا, font);
  إذا (!monotone)
    ret = خطأ;
  إذا (monotone &&
      !buffer_verify_unsafe_to_break (هذا, text_buffer, font, features, لمب_features, shapers))
    ret = خطأ;
  إذا (monotone &&
      (flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) != 0 &&
      !buffer_verify_unsafe_to_concat (هذا, text_buffer, font, features, لمب_features, shapers))
    ret = خطأ;
  إذا (!ret)
  {
#ifndef HB_NO_BUFFER_SERIALIZE
    unsigned len = text_buffer->len;
    hb_vector_t<حرف> bytes;
    إذا (likely (bytes.resize (len * 10 + 16)))
    {
      hb_buffer_serialize_unicode (text_buffer,
				   0, len,
				   bytes.arrayZ, bytes.length,
				   &len,
				   HB_BUFFER_SERIALIZE_FORMAT_TEXT,
				   HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS);
      buffer_verify_error (هذا, font, BUFFER_VERIFY_ERROR "text was: %s.", bytes.arrayZ ? bytes.arrayZ : "");
    }
#endif
  }
  أرجع ret;
}


#endif
