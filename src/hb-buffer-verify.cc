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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_বাফার_VERIFY

#অন্তর্ভাব "hb-buffer.hh"


#define বাফার_VERIFY_ERROR "buffer verify error: "
স্থির অন্তঃরেখ শূন্য
বাফার_verify_error (hb_বাফার_t *বাফার,
		     hb_font_t *font,
		     ধ্রুবক অক্ষর *fmt,
		     ...) HB_PRINTF_FUNC(3, 4);

স্থির অন্তঃরেখ শূন্য
বাফার_verify_error (hb_বাফার_t *বাফার,
		     hb_font_t *font,
		     ধ্রুবক অক্ষর *fmt,
		     ...)
{
  va_list ap;
  va_start (ap, fmt);
  যদি (বাফার->messaging ())
  {
    বাফার->message_impl (font, fmt, ap);
  }
  অন্যথা
  {
    fprintf (stderr, "harfbuzz ");
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
  }
  va_end (ap);
}

স্থির যৌক্তিক
বাফার_verify_monotone (hb_বাফার_t *বাফার,
			hb_font_t   *font)
{
  যদি (!HB_বাফার_CLUSTER_LEVEL_IS_MONOTONE (বাফার->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    ফেরত সত্য;
  }

  যৌক্তিক is_forward = HB_DIRECTION_IS_FORWARD (hb_বাফার_get_direction (বাফার));

  unsigned পূর্ণসংখ্যা অঙ্ক_glyphs;
  hb_glyph_info_t *info = hb_বাফার_get_glyph_infos (বাফার, &অঙ্ক_glyphs);

  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < অঙ্ক_glyphs; i++)
    যদি (info[i-1].cluster != info[i].cluster &&
	(info[i-1].cluster < info[i].cluster) != is_forward)
    {
      বাফার_verify_error (বাফার, font, বাফার_VERIFY_ERROR "clusters are not monotone.");
      ফেরত মিথ্যা;
    }

  ফেরত সত্য;
}

স্থির যৌক্তিক
বাফার_verify_unsafe_to_break (hb_বাফার_t  *বাফার,
			       hb_বাফার_t  *text_বাফার,
			       hb_font_t          *font,
			       ধ্রুবক hb_feature_t *features,
			       unsigned পূর্ণসংখ্যা        অঙ্ক_features,
			       ধ্রুবক অক্ষর * ধ্রুবক *shapers)
{
  যদি (!HB_বাফার_CLUSTER_LEVEL_IS_MONOTONE (বাফার->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    ফেরত সত্য;
  }

  /* Check that breaking up shaping at safe-to-break is indeed safe. */

  hb_unique_ptr_t<hb_বাফার_t> fragment (hb_বাফার_create_similar (বাফার));
  hb_বাফার_set_flags (fragment, (hb_বাফার_flags_t (hb_বাফার_get_flags (fragment) & ~HB_বাফার_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_বাফার_t> reconstruction (hb_বাফার_create_similar (বাফার));
  hb_বাফার_set_flags (reconstruction, (hb_বাফার_flags_t (hb_বাফার_get_flags (reconstruction) & ~HB_বাফার_FLAG_VERIFY)));

  unsigned পূর্ণসংখ্যা অঙ্ক_glyphs;
  hb_glyph_info_t *info = hb_বাফার_get_glyph_infos (বাফার, &অঙ্ক_glyphs);

  unsigned পূর্ণসংখ্যা অঙ্ক_chars;
  hb_glyph_info_t *text = hb_বাফার_get_glyph_infos (text_বাফার, &অঙ্ক_chars);

  /* Chop text and shape fragments. */
  যৌক্তিক forward = HB_DIRECTION_IS_FORWARD (hb_বাফার_get_direction (বাফার));
  unsigned পূর্ণসংখ্যা start = 0;
  unsigned পূর্ণসংখ্যা text_start = forward ? 0 : অঙ্ক_chars;
  unsigned পূর্ণসংখ্যা text_end = text_start;
  জন্য (unsigned পূর্ণসংখ্যা end = 1; end < অঙ্ক_glyphs + 1; end++)
  {
    যদি (end < অঙ্ক_glyphs &&
	(info[end].cluster == info[end-1].cluster ||
	 info[end-(forward?0:1)].mask & HB_GLYPH_FLAG_UNSAFE_TO_BREAK))
	চালু;

    /* Shape segment corresponding to glyphs start..end. */
    যদি (end == অঙ্ক_glyphs)
    {
      যদি (forward)
	text_end = অঙ্ক_chars;
      অন্যথা
	text_start = 0;
    }
    অন্যথা
    {
      যদি (forward)
      {
	unsigned পূর্ণসংখ্যা cluster = info[end].cluster;
	যখন (text_end < অঙ্ক_chars && text[text_end].cluster < cluster)
	  text_end++;
      }
      অন্যথা
      {
	unsigned পূর্ণসংখ্যা cluster = info[end - 1].cluster;
	যখন (text_start && text[text_start - 1].cluster >= cluster)
	  text_start--;
      }
    }
    যদি (unlikely (text_start >= text_end))
    {
      বাফার_verify_error (বাফার, font, বাফার_VERIFY_ERROR "unsafe-to-break text range is invalid.");
      ফেরত মিথ্যা;
    }

    যদি (মিথ্যা)
      printf("start %u end %u text start %u end %u\n", start, end, text_start, text_end);

    hb_বাফার_clear_contents (fragment);

    hb_বাফার_flags_t flags = hb_বাফার_get_flags (fragment);
    যদি (0 < text_start)
      flags = (hb_বাফার_flags_t) (flags & ~HB_বাফার_FLAG_BOT);
    যদি (text_end < অঙ্ক_chars)
      flags = (hb_বাফার_flags_t) (flags & ~HB_বাফার_FLAG_EOT);
    hb_বাফার_set_flags (fragment, flags);

    hb_বাফার_append (fragment, text_বাফার, text_start, text_end);
    যদি (!hb_shape_full (font, fragment, features, অঙ্ক_features, shapers) ||
	!fragment->successful)
      ফেরত সত্য;
    hb_বাফার_append (reconstruction, fragment, 0, -1);

    start = end;
    যদি (forward)
      text_start = text_end;
    অন্যথা
      text_end = text_start;
  }

  যৌক্তিক ret = সত্য;
  যদি (likely (reconstruction->successful))
  {
    hb_বাফার_diff_flags_t diff = hb_বাফার_diff (reconstruction, বাফার, (hb_codepoint_t) -1, 0);
    যদি (diff & ~HB_বাফার_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      বাফার_verify_error (বাফার, font, বাফার_VERIFY_ERROR "unsafe-to-break test failed.");
      ret = মিথ্যা;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_বাফার_set_দৈর্ঘ্য (বাফার, 0);
      hb_বাফার_append (বাফার, reconstruction, 0, -1);
    }
  }

  ফেরত ret;
}

স্থির যৌক্তিক
বাফার_verify_unsafe_to_concat (hb_বাফার_t        *বাফার,
				hb_বাফার_t        *text_বাফার,
				hb_font_t          *font,
				ধ্রুবক hb_feature_t *features,
				unsigned পূর্ণসংখ্যা        অঙ্ক_features,
				ধ্রুবক অক্ষর * ধ্রুবক *shapers)
{
  যদি (!HB_বাফার_CLUSTER_LEVEL_IS_MONOTONE (বাফার->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    ফেরত সত্য;
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

  hb_unique_ptr_t<hb_বাফার_t> fragments[2] {
    hb_unique_ptr_t<hb_বাফার_t> (hb_বাফার_create_similar (বাফার)),
    hb_unique_ptr_t<hb_বাফার_t> (hb_বাফার_create_similar (বাফার)),
  };
  hb_বাফার_set_flags (fragments[0], (hb_বাফার_flags_t (hb_বাফার_get_flags (fragments[0]) & ~HB_বাফার_FLAG_VERIFY)));
  hb_বাফার_set_flags (fragments[1], (hb_বাফার_flags_t (hb_বাফার_get_flags (fragments[1]) & ~HB_বাফার_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_বাফার_t> reconstruction (hb_বাফার_create_similar (বাফার));
  hb_বাফার_set_flags (reconstruction, (hb_বাফার_flags_t (hb_বাফার_get_flags (reconstruction) & ~HB_বাফার_FLAG_VERIFY)));
  hb_segment_properties_t props;
  hb_বাফার_get_segment_properties (বাফার, &props);
  hb_বাফার_set_segment_properties (fragments[0], &props);
  hb_বাফার_set_segment_properties (fragments[1], &props);
  hb_বাফার_set_segment_properties (reconstruction, &props);

  unsigned অঙ্ক_glyphs;
  hb_glyph_info_t *info = hb_বাফার_get_glyph_infos (বাফার, &অঙ্ক_glyphs);

  unsigned অঙ্ক_chars;
  hb_glyph_info_t *text = hb_বাফার_get_glyph_infos (text_বাফার, &অঙ্ক_chars);

  যৌক্তিক forward = HB_DIRECTION_IS_FORWARD (hb_বাফার_get_direction (বাফার));

  যদি (!forward)
    hb_বাফার_reverse (বাফার);

  /*
   * Split text into segments and collect into to fragment streams.
   */
  {
    unsigned fragment_idx = 0;
    unsigned start = 0;
    unsigned text_start = 0;
    unsigned text_end = 0;
    জন্য (unsigned end = 1; end < অঙ্ক_glyphs + 1; end++)
    {
      যদি (end < অঙ্ক_glyphs &&
	  (info[end].cluster == info[end-1].cluster ||
	   info[end].mask & HB_GLYPH_FLAG_UNSAFE_TO_CONCAT))
	  চালু;

      /* Accumulate segment corresponding to glyphs start..end. */
      যদি (end == অঙ্ক_glyphs)
	text_end = অঙ্ক_chars;
      অন্যথা
      {
	unsigned cluster = info[end].cluster;
	যখন (text_end < অঙ্ক_chars && text[text_end].cluster < cluster)
	  text_end++;
      }
      প্রতিজ্ঞা (text_start < text_end);

      যদি (মিথ্যা)
	printf("start %u end %u text start %u end %u\n", start, end, text_start, text_end);

#যদি 0
      hb_বাফার_flags_t flags = hb_বাফার_get_flags (fragment);
      যদি (0 < text_start)
	flags = (hb_বাফার_flags_t) (flags & ~HB_বাফার_FLAG_BOT);
      যদি (text_end < অঙ্ক_chars)
	flags = (hb_বাফার_flags_t) (flags & ~HB_বাফার_FLAG_EOT);
      hb_বাফার_set_flags (fragment, flags);
#endif

      hb_বাফার_append (fragments[fragment_idx], text_বাফার, text_start, text_end);

      start = end;
      text_start = text_end;
      fragment_idx = 1 - fragment_idx;
    }
  }

  যৌক্তিক ret = সত্য;
  /*
   * Shape the two fragment streams.
   */
  যদি (!hb_shape_full (font, fragments[0], features, অঙ্ক_features, shapers) ||
      !fragments[0]->successful)
    ফেরত ret;

  যদি (!hb_shape_full (font, fragments[1], features, অঙ্ক_features, shapers) ||
      !fragments[1]->successful)
    ফেরত ret;

  যদি (!forward)
  {
    hb_বাফার_reverse (fragments[0]);
    hb_বাফার_reverse (fragments[1]);
  }

  /*
   * Reconstruct results.
   */
  {
    unsigned fragment_idx = 0;
    unsigned fragment_start[2] {0, 0};
    unsigned fragment_অঙ্ক_glyphs[2];
    hb_glyph_info_t *fragment_info[2];
    জন্য (unsigned i = 0; i < 2; i++)
      fragment_info[i] = hb_বাফার_get_glyph_infos (fragments[i], &fragment_অঙ্ক_glyphs[i]);
    যখন (fragment_start[0] < fragment_অঙ্ক_glyphs[0] ||
	   fragment_start[1] < fragment_অঙ্ক_glyphs[1])
    {
      unsigned fragment_end = fragment_start[fragment_idx] + 1;
      যখন (fragment_end < fragment_অঙ্ক_glyphs[fragment_idx] &&
	     (fragment_info[fragment_idx][fragment_end].cluster == fragment_info[fragment_idx][fragment_end - 1].cluster ||
	      fragment_info[fragment_idx][fragment_end].mask & HB_GLYPH_FLAG_UNSAFE_TO_CONCAT))
	fragment_end++;

      hb_বাফার_append (reconstruction, fragments[fragment_idx], fragment_start[fragment_idx], fragment_end);

      fragment_start[fragment_idx] = fragment_end;
      fragment_idx = 1 - fragment_idx;
    }
  }

  যদি (!forward)
  {
    hb_বাফার_reverse (বাফার);
    hb_বাফার_reverse (reconstruction);
  }

  যদি (likely (reconstruction->successful))
  {
    /*
     * Diff results.
     */
    hb_বাফার_diff_flags_t diff = hb_বাফার_diff (reconstruction, বাফার, (hb_codepoint_t) -1, 0);
    যদি (diff & ~HB_বাফার_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      বাফার_verify_error (বাফার, font, বাফার_VERIFY_ERROR "unsafe-to-concat test failed.");
      ret = মিথ্যা;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_বাফার_set_দৈর্ঘ্য (বাফার, 0);
      hb_বাফার_append (বাফার, reconstruction, 0, -1);
    }
  }

  ফেরত ret;
}

যৌক্তিক
hb_বাফার_t::verify (hb_বাফার_t        *text_বাফার,
		     hb_font_t          *font,
		     ধ্রুবক hb_feature_t *features,
		     unsigned পূর্ণসংখ্যা        অঙ্ক_features,
		     ধ্রুবক অক্ষর * ধ্রুবক *shapers)
{
  যৌক্তিক ret = সত্য;
  যৌক্তিক monotone = বাফার_verify_monotone (এই, font);
  যদি (!monotone)
    ret = মিথ্যা;
  যদি (monotone &&
      !বাফার_verify_unsafe_to_break (এই, text_বাফার, font, features, অঙ্ক_features, shapers))
    ret = মিথ্যা;
  যদি (monotone &&
      (flags & HB_বাফার_FLAG_PRODUCE_UNSAFE_TO_CONCAT) != 0 &&
      !বাফার_verify_unsafe_to_concat (এই, text_বাফার, font, features, অঙ্ক_features, shapers))
    ret = মিথ্যা;
  যদি (!ret)
  {
#ifndef HB_NO_বাফার_SERIALIZE
    unsigned len = text_বাফার->len;
    hb_vector_t<অক্ষর> bytes;
    যদি (likely (bytes.resize (len * 10 + 16)))
    {
      hb_বাফার_serialize_unicode (text_বাফার,
				   0, len,
				   bytes.arrayZ, bytes.দৈর্ঘ্য,
				   &len,
				   HB_বাফার_SERIALIZE_FORMAT_TEXT,
				   HB_বাফার_SERIALIZE_FLAG_NO_CLUSTERS);
      বাফার_verify_error (এই, font, বাফার_VERIFY_ERROR "text was: %s.", bytes.arrayZ ? bytes.arrayZ : "");
    }
#endif
  }
  ফেরত ret;
}


#endif
