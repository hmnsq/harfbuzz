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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_BUFFER_VERIFY

#અન્તર્ભાવ "hb-buffer.hh"


#define BUFFER_VERIFY_ERROR "buffer verify error: "
સ્થિર અંતઃરેખ શૂન્ય
buffer_verify_error (hb_buffer_t *buffer,
		     hb_font_t *font,
		     અચલ અક્ષર *fmt,
		     ...) HB_PRINTF_FUNC(3, 4);

સ્થિર અંતઃરેખ શૂન્ય
buffer_verify_error (hb_buffer_t *buffer,
		     hb_font_t *font,
		     અચલ અક્ષર *fmt,
		     ...)
{
  va_list ap;
  va_start (ap, fmt);
  જો (buffer->messaging ())
  {
    buffer->message_impl (font, fmt, ap);
  }
  નહીંતર
  {
    fprintf (stderr, "harfbuzz ");
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
  }
  va_end (ap);
}

સ્થિર તાર્કિક
buffer_verify_monotone (hb_buffer_t *buffer,
			hb_font_t   *font)
{
  જો (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    પાછા સાચું;
  }

  તાર્કિક is_forward = HB_DIRECTION_IS_FORWARD (hb_buffer_get_direction (buffer));

  unsigned પૂર્ણાંક અંક_glyphs;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &અંક_glyphs);

  માટે (unsigned પૂર્ણાંક i = 1; i < અંક_glyphs; i++)
    જો (info[i-1].cluster != info[i].cluster &&
	(info[i-1].cluster < info[i].cluster) != is_forward)
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "clusters are not monotone.");
      પાછા ખોટું;
    }

  પાછા સાચું;
}

સ્થિર તાર્કિક
buffer_verify_unsafe_to_break (hb_buffer_t  *buffer,
			       hb_buffer_t  *text_buffer,
			       hb_font_t          *font,
			       અચલ hb_feature_t *features,
			       unsigned પૂર્ણાંક        અંક_features,
			       અચલ અક્ષર * અચલ *shapers)
{
  જો (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    પાછા સાચું;
  }

  /* Check that breaking up shaping at safe-to-break is indeed safe. */

  hb_unique_ptr_t<hb_buffer_t> fragment (hb_buffer_create_similar (buffer));
  hb_buffer_set_flags (fragment, (hb_buffer_flags_t (hb_buffer_get_flags (fragment) & ~HB_BUFFER_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_buffer_t> reconstruction (hb_buffer_create_similar (buffer));
  hb_buffer_set_flags (reconstruction, (hb_buffer_flags_t (hb_buffer_get_flags (reconstruction) & ~HB_BUFFER_FLAG_VERIFY)));

  unsigned પૂર્ણાંક અંક_glyphs;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &અંક_glyphs);

  unsigned પૂર્ણાંક અંક_chars;
  hb_glyph_info_t *text = hb_buffer_get_glyph_infos (text_buffer, &અંક_chars);

  /* Chop text and shape fragments. */
  તાર્કિક forward = HB_DIRECTION_IS_FORWARD (hb_buffer_get_direction (buffer));
  unsigned પૂર્ણાંક start = 0;
  unsigned પૂર્ણાંક text_start = forward ? 0 : અંક_chars;
  unsigned પૂર્ણાંક text_end = text_start;
  માટે (unsigned પૂર્ણાંક end = 1; end < અંક_glyphs + 1; end++)
  {
    જો (end < અંક_glyphs &&
	(info[end].cluster == info[end-1].cluster ||
	 info[end-(forward?0:1)].mask & HB_GLYPH_FLAG_UNSAFE_TO_BREAK))
	ચાલુ;

    /* Shape segment corresponding to glyphs start..end. */
    જો (end == અંક_glyphs)
    {
      જો (forward)
	text_end = અંક_chars;
      નહીંતર
	text_start = 0;
    }
    નહીંતર
    {
      જો (forward)
      {
	unsigned પૂર્ણાંક cluster = info[end].cluster;
	જ્યારે (text_end < અંક_chars && text[text_end].cluster < cluster)
	  text_end++;
      }
      નહીંતર
      {
	unsigned પૂર્ણાંક cluster = info[end - 1].cluster;
	જ્યારે (text_start && text[text_start - 1].cluster >= cluster)
	  text_start--;
      }
    }
    જો (unlikely (text_start >= text_end))
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "unsafe-to-break text range is invalid.");
      પાછા ખોટું;
    }

    જો (ખોટું)
      printf("start %u end %u text start %u end %u\n", start, end, text_start, text_end);

    hb_buffer_clear_contents (fragment);

    hb_buffer_flags_t flags = hb_buffer_get_flags (fragment);
    જો (0 < text_start)
      flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_BOT);
    જો (text_end < અંક_chars)
      flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_EOT);
    hb_buffer_set_flags (fragment, flags);

    hb_buffer_append (fragment, text_buffer, text_start, text_end);
    જો (!hb_shape_full (font, fragment, features, અંક_features, shapers) ||
	!fragment->successful)
      પાછા સાચું;
    hb_buffer_append (reconstruction, fragment, 0, -1);

    start = end;
    જો (forward)
      text_start = text_end;
    નહીંતર
      text_end = text_start;
  }

  તાર્કિક ret = સાચું;
  જો (likely (reconstruction->successful))
  {
    hb_buffer_diff_flags_t diff = hb_buffer_diff (reconstruction, buffer, (hb_codepoint_t) -1, 0);
    જો (diff & ~HB_BUFFER_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "unsafe-to-break test failed.");
      ret = ખોટું;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_buffer_set_length (buffer, 0);
      hb_buffer_append (buffer, reconstruction, 0, -1);
    }
  }

  પાછા ret;
}

સ્થિર તાર્કિક
buffer_verify_unsafe_to_concat (hb_buffer_t        *buffer,
				hb_buffer_t        *text_buffer,
				hb_font_t          *font,
				અચલ hb_feature_t *features,
				unsigned પૂર્ણાંક        અંક_features,
				અચલ અક્ષર * અચલ *shapers)
{
  જો (!HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    પાછા સાચું;
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

  unsigned અંક_glyphs;
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos (buffer, &અંક_glyphs);

  unsigned અંક_chars;
  hb_glyph_info_t *text = hb_buffer_get_glyph_infos (text_buffer, &અંક_chars);

  તાર્કિક forward = HB_DIRECTION_IS_FORWARD (hb_buffer_get_direction (buffer));

  જો (!forward)
    hb_buffer_reverse (buffer);

  /*
   * Split text into segments and collect into to fragment streams.
   */
  {
    unsigned fragment_idx = 0;
    unsigned start = 0;
    unsigned text_start = 0;
    unsigned text_end = 0;
    માટે (unsigned end = 1; end < અંક_glyphs + 1; end++)
    {
      જો (end < અંક_glyphs &&
	  (info[end].cluster == info[end-1].cluster ||
	   info[end].mask & HB_GLYPH_FLAG_UNSAFE_TO_CONCAT))
	  ચાલુ;

      /* Accumulate segment corresponding to glyphs start..end. */
      જો (end == અંક_glyphs)
	text_end = અંક_chars;
      નહીંતર
      {
	unsigned cluster = info[end].cluster;
	જ્યારે (text_end < અંક_chars && text[text_end].cluster < cluster)
	  text_end++;
      }
      પ્રતિજ્ઞા (text_start < text_end);

      જો (ખોટું)
	printf("start %u end %u text start %u end %u\n", start, end, text_start, text_end);

#જો 0
      hb_buffer_flags_t flags = hb_buffer_get_flags (fragment);
      જો (0 < text_start)
	flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_BOT);
      જો (text_end < અંક_chars)
	flags = (hb_buffer_flags_t) (flags & ~HB_BUFFER_FLAG_EOT);
      hb_buffer_set_flags (fragment, flags);
#endif

      hb_buffer_append (fragments[fragment_idx], text_buffer, text_start, text_end);

      start = end;
      text_start = text_end;
      fragment_idx = 1 - fragment_idx;
    }
  }

  તાર્કિક ret = સાચું;
  /*
   * Shape the two fragment streams.
   */
  જો (!hb_shape_full (font, fragments[0], features, અંક_features, shapers) ||
      !fragments[0]->successful)
    પાછા ret;

  જો (!hb_shape_full (font, fragments[1], features, અંક_features, shapers) ||
      !fragments[1]->successful)
    પાછા ret;

  જો (!forward)
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
    unsigned fragment_અંક_glyphs[2];
    hb_glyph_info_t *fragment_info[2];
    માટે (unsigned i = 0; i < 2; i++)
      fragment_info[i] = hb_buffer_get_glyph_infos (fragments[i], &fragment_અંક_glyphs[i]);
    જ્યારે (fragment_start[0] < fragment_અંક_glyphs[0] ||
	   fragment_start[1] < fragment_અંક_glyphs[1])
    {
      unsigned fragment_end = fragment_start[fragment_idx] + 1;
      જ્યારે (fragment_end < fragment_અંક_glyphs[fragment_idx] &&
	     (fragment_info[fragment_idx][fragment_end].cluster == fragment_info[fragment_idx][fragment_end - 1].cluster ||
	      fragment_info[fragment_idx][fragment_end].mask & HB_GLYPH_FLAG_UNSAFE_TO_CONCAT))
	fragment_end++;

      hb_buffer_append (reconstruction, fragments[fragment_idx], fragment_start[fragment_idx], fragment_end);

      fragment_start[fragment_idx] = fragment_end;
      fragment_idx = 1 - fragment_idx;
    }
  }

  જો (!forward)
  {
    hb_buffer_reverse (buffer);
    hb_buffer_reverse (reconstruction);
  }

  જો (likely (reconstruction->successful))
  {
    /*
     * Diff results.
     */
    hb_buffer_diff_flags_t diff = hb_buffer_diff (reconstruction, buffer, (hb_codepoint_t) -1, 0);
    જો (diff & ~HB_BUFFER_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      buffer_verify_error (buffer, font, BUFFER_VERIFY_ERROR "unsafe-to-concat test failed.");
      ret = ખોટું;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_buffer_set_length (buffer, 0);
      hb_buffer_append (buffer, reconstruction, 0, -1);
    }
  }

  પાછા ret;
}

તાર્કિક
hb_buffer_t::verify (hb_buffer_t        *text_buffer,
		     hb_font_t          *font,
		     અચલ hb_feature_t *features,
		     unsigned પૂર્ણાંક        અંક_features,
		     અચલ અક્ષર * અચલ *shapers)
{
  તાર્કિક ret = સાચું;
  તાર્કિક monotone = buffer_verify_monotone (આ, font);
  જો (!monotone)
    ret = ખોટું;
  જો (monotone &&
      !buffer_verify_unsafe_to_break (આ, text_buffer, font, features, અંક_features, shapers))
    ret = ખોટું;
  જો (monotone &&
      (flags & HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT) != 0 &&
      !buffer_verify_unsafe_to_concat (આ, text_buffer, font, features, અંક_features, shapers))
    ret = ખોટું;
  જો (!ret)
  {
#ifndef HB_NO_BUFFER_SERIALIZE
    unsigned len = text_buffer->len;
    hb_vector_t<અક્ષર> bytes;
    જો (likely (bytes.resize (len * 10 + 16)))
    {
      hb_buffer_serialize_unicode (text_buffer,
				   0, len,
				   bytes.arrayZ, bytes.length,
				   &len,
				   HB_BUFFER_SERIALIZE_FORMAT_TEXT,
				   HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS);
      buffer_verify_error (આ, font, BUFFER_VERIFY_ERROR "text was: %s.", bytes.arrayZ ? bytes.arrayZ : "");
    }
#endif
  }
  પાછા ret;
}


#endif
