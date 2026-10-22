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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_بفر_VERIFY

#شامل "hb-buffer.hh"


#تعریف_کرو بفر_VERIFY_ERROR "buffer verify error: "
ساکن اندرونی خالی
بفر_verify_error (hb_بفر_t *بفر,
		     hb_font_t *font,
		     ثابت حرف *fmt,
		     ...) HB_PRINTF_FUNC(3, 4);

ساکن اندرونی خالی
بفر_verify_error (hb_بفر_t *بفر,
		     hb_font_t *font,
		     ثابت حرف *fmt,
		     ...)
{
  va_list ap;
  va_آغاز (ap, fmt);
  اگر (بفر->messaging ())
  {
    بفر->پیغام_impl (font, fmt, ap);
  }
  ورنہ
  {
    fprintf (stderr, "harfbuzz ");
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
  }
  va_انتہا (ap);
}

ساکن منطقی
بفر_verify_monotone (hb_بفر_t *بفر,
			hb_font_t   *font)
{
  اگر (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    واپس سچ;
  }

  منطقی is_forward = HB_DIRECTION_IS_FORWARD (hb_بفر_get_direction (بفر));

  غیرعلامتی صحیح عدد_glyphs;
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, &عدد_glyphs);

  برائے (غیرعلامتی صحیح i = 1; i < عدد_glyphs; i++)
    اگر (info[i-1].cluster != info[i].cluster &&
	(info[i-1].cluster < info[i].cluster) != is_forward)
    {
      بفر_verify_error (بفر, font, بفر_VERIFY_ERROR "clusters are not monotone.");
      واپس جھوٹ;
    }

  واپس سچ;
}

ساکن منطقی
بفر_verify_unsafe_مقصد_break (hb_بفر_t  *بفر,
			       hb_بفر_t  *متن_بفر,
			       hb_font_t          *font,
			       ثابت hb_feature_t *features,
			       غیرعلامتی صحیح        عدد_features,
			       ثابت حرف * ثابت *shapers)
{
  اگر (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    واپس سچ;
  }

  /* Check that breaking up shaping at safe-to-break is indeed safe. */

  hb_unique_ptr_t<hb_بفر_t> fragment (hb_بفر_create_similar (بفر));
  hb_بفر_سیٹ_flags (fragment, (hb_بفر_flags_t (hb_بفر_get_flags (fragment) & ~HB_بفر_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_بفر_t> reconstruction (hb_بفر_create_similar (بفر));
  hb_بفر_سیٹ_flags (reconstruction, (hb_بفر_flags_t (hb_بفر_get_flags (reconstruction) & ~HB_بفر_FLAG_VERIFY)));

  غیرعلامتی صحیح عدد_glyphs;
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, &عدد_glyphs);

  غیرعلامتی صحیح عدد_chars;
  hb_glyph_info_t *متن = hb_بفر_get_glyph_infos (متن_بفر, &عدد_chars);

  /* Chop text and shape fragments. */
  منطقی forward = HB_DIRECTION_IS_FORWARD (hb_بفر_get_direction (بفر));
  غیرعلامتی صحیح آغاز = 0;
  غیرعلامتی صحیح متن_آغاز = forward ? 0 : عدد_chars;
  غیرعلامتی صحیح متن_انتہا = متن_آغاز;
  برائے (غیرعلامتی صحیح انتہا = 1; انتہا < عدد_glyphs + 1; انتہا++)
  {
    اگر (انتہا < عدد_glyphs &&
	(info[انتہا].cluster == info[انتہا-1].cluster ||
	 info[انتہا-(forward?0:1)].نقاب & HB_GLYPH_FLAG_UNSAFE_مقصد_BREAK))
	جاری;

    /* Shape segment corresponding to glyphs start..end. */
    اگر (انتہا == عدد_glyphs)
    {
      اگر (forward)
	متن_انتہا = عدد_chars;
      ورنہ
	متن_آغاز = 0;
    }
    ورنہ
    {
      اگر (forward)
      {
	غیرعلامتی صحیح cluster = info[انتہا].cluster;
	جب (متن_انتہا < عدد_chars && متن[متن_انتہا].cluster < cluster)
	  متن_انتہا++;
      }
      ورنہ
      {
	غیرعلامتی صحیح cluster = info[انتہا - 1].cluster;
	جب (متن_آغاز && متن[متن_آغاز - 1].cluster >= cluster)
	  متن_آغاز--;
      }
    }
    اگر (unlikely (متن_آغاز >= متن_انتہا))
    {
      بفر_verify_error (بفر, font, بفر_VERIFY_ERROR "unsafe-to-break text range is invalid.");
      واپس جھوٹ;
    }

    اگر (جھوٹ)
      لکھو("start %u end %u text start %u end %u\n", آغاز, انتہا, متن_آغاز, متن_انتہا);

    hb_بفر_clear_contents (fragment);

    hb_بفر_flags_t flags = hb_بفر_get_flags (fragment);
    اگر (0 < متن_آغاز)
      flags = (hb_بفر_flags_t) (flags & ~HB_بفر_FLAG_BOT);
    اگر (متن_انتہا < عدد_chars)
      flags = (hb_بفر_flags_t) (flags & ~HB_بفر_FLAG_EOT);
    hb_بفر_سیٹ_flags (fragment, flags);

    hb_بفر_append (fragment, متن_بفر, متن_آغاز, متن_انتہا);
    اگر (!hb_shape_بھرا (font, fragment, features, عدد_features, shapers) ||
	!fragment->successful)
      واپس سچ;
    hb_بفر_append (reconstruction, fragment, 0, -1);

    آغاز = انتہا;
    اگر (forward)
      متن_آغاز = متن_انتہا;
    ورنہ
      متن_انتہا = متن_آغاز;
  }

  منطقی ret = سچ;
  اگر (likely (reconstruction->successful))
  {
    hb_بفر_diff_flags_t diff = hb_بفر_diff (reconstruction, بفر, (hb_codepoint_t) -1, 0);
    اگر (diff & ~HB_بفر_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      بفر_verify_error (بفر, font, بفر_VERIFY_ERROR "unsafe-to-break test failed.");
      ret = جھوٹ;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_بفر_سیٹ_لمبائی (بفر, 0);
      hb_بفر_append (بفر, reconstruction, 0, -1);
    }
  }

  واپس ret;
}

ساکن منطقی
بفر_verify_unsafe_مقصد_concat (hb_بفر_t        *بفر,
				hb_بفر_t        *متن_بفر,
				hb_font_t          *font,
				ثابت hb_feature_t *features,
				غیرعلامتی صحیح        عدد_features,
				ثابت حرف * ثابت *shapers)
{
  اگر (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    واپس سچ;
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

  hb_unique_ptr_t<hb_بفر_t> fragments[2] {
    hb_unique_ptr_t<hb_بفر_t> (hb_بفر_create_similar (بفر)),
    hb_unique_ptr_t<hb_بفر_t> (hb_بفر_create_similar (بفر)),
  };
  hb_بفر_سیٹ_flags (fragments[0], (hb_بفر_flags_t (hb_بفر_get_flags (fragments[0]) & ~HB_بفر_FLAG_VERIFY)));
  hb_بفر_سیٹ_flags (fragments[1], (hb_بفر_flags_t (hb_بفر_get_flags (fragments[1]) & ~HB_بفر_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_بفر_t> reconstruction (hb_بفر_create_similar (بفر));
  hb_بفر_سیٹ_flags (reconstruction, (hb_بفر_flags_t (hb_بفر_get_flags (reconstruction) & ~HB_بفر_FLAG_VERIFY)));
  hb_segment_properties_t props;
  hb_بفر_get_segment_properties (بفر, &props);
  hb_بفر_سیٹ_segment_properties (fragments[0], &props);
  hb_بفر_سیٹ_segment_properties (fragments[1], &props);
  hb_بفر_سیٹ_segment_properties (reconstruction, &props);

  غیرعلامتی عدد_glyphs;
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, &عدد_glyphs);

  غیرعلامتی عدد_chars;
  hb_glyph_info_t *متن = hb_بفر_get_glyph_infos (متن_بفر, &عدد_chars);

  منطقی forward = HB_DIRECTION_IS_FORWARD (hb_بفر_get_direction (بفر));

  اگر (!forward)
    hb_بفر_الٹ (بفر);

  /*
   * Split text into segments and collect into to fragment streams.
   */
  {
    غیرعلامتی fragment_idx = 0;
    غیرعلامتی آغاز = 0;
    غیرعلامتی متن_آغاز = 0;
    غیرعلامتی متن_انتہا = 0;
    برائے (غیرعلامتی انتہا = 1; انتہا < عدد_glyphs + 1; انتہا++)
    {
      اگر (انتہا < عدد_glyphs &&
	  (info[انتہا].cluster == info[انتہا-1].cluster ||
	   info[انتہا].نقاب & HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT))
	  جاری;

      /* Accumulate segment corresponding to glyphs start..end. */
      اگر (انتہا == عدد_glyphs)
	متن_انتہا = عدد_chars;
      ورنہ
      {
	غیرعلامتی cluster = info[انتہا].cluster;
	جب (متن_انتہا < عدد_chars && متن[متن_انتہا].cluster < cluster)
	  متن_انتہا++;
      }
      أكد (متن_آغاز < متن_انتہا);

      اگر (جھوٹ)
	لکھو("start %u end %u text start %u end %u\n", آغاز, انتہا, متن_آغاز, متن_انتہا);

#اگر 0
      hb_بفر_flags_t flags = hb_بفر_get_flags (fragment);
      اگر (0 < متن_آغاز)
	flags = (hb_بفر_flags_t) (flags & ~HB_بفر_FLAG_BOT);
      اگر (متن_انتہا < عدد_chars)
	flags = (hb_بفر_flags_t) (flags & ~HB_بفر_FLAG_EOT);
      hb_بفر_سیٹ_flags (fragment, flags);
#اگر_ختم

      hb_بفر_append (fragments[fragment_idx], متن_بفر, متن_آغاز, متن_انتہا);

      آغاز = انتہا;
      متن_آغاز = متن_انتہا;
      fragment_idx = 1 - fragment_idx;
    }
  }

  منطقی ret = سچ;
  /*
   * Shape the two fragment streams.
   */
  اگر (!hb_shape_بھرا (font, fragments[0], features, عدد_features, shapers) ||
      !fragments[0]->successful)
    واپس ret;

  اگر (!hb_shape_بھرا (font, fragments[1], features, عدد_features, shapers) ||
      !fragments[1]->successful)
    واپس ret;

  اگر (!forward)
  {
    hb_بفر_الٹ (fragments[0]);
    hb_بفر_الٹ (fragments[1]);
  }

  /*
   * Reconstruct results.
   */
  {
    غیرعلامتی fragment_idx = 0;
    غیرعلامتی fragment_آغاز[2] {0, 0};
    غیرعلامتی fragment_عدد_glyphs[2];
    hb_glyph_info_t *fragment_info[2];
    برائے (غیرعلامتی i = 0; i < 2; i++)
      fragment_info[i] = hb_بفر_get_glyph_infos (fragments[i], &fragment_عدد_glyphs[i]);
    جب (fragment_آغاز[0] < fragment_عدد_glyphs[0] ||
	   fragment_آغاز[1] < fragment_عدد_glyphs[1])
    {
      غیرعلامتی fragment_انتہا = fragment_آغاز[fragment_idx] + 1;
      جب (fragment_انتہا < fragment_عدد_glyphs[fragment_idx] &&
	     (fragment_info[fragment_idx][fragment_انتہا].cluster == fragment_info[fragment_idx][fragment_انتہا - 1].cluster ||
	      fragment_info[fragment_idx][fragment_انتہا].نقاب & HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT))
	fragment_انتہا++;

      hb_بفر_append (reconstruction, fragments[fragment_idx], fragment_آغاز[fragment_idx], fragment_انتہا);

      fragment_آغاز[fragment_idx] = fragment_انتہا;
      fragment_idx = 1 - fragment_idx;
    }
  }

  اگر (!forward)
  {
    hb_بفر_الٹ (بفر);
    hb_بفر_الٹ (reconstruction);
  }

  اگر (likely (reconstruction->successful))
  {
    /*
     * Diff results.
     */
    hb_بفر_diff_flags_t diff = hb_بفر_diff (reconstruction, بفر, (hb_codepoint_t) -1, 0);
    اگر (diff & ~HB_بفر_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      بفر_verify_error (بفر, font, بفر_VERIFY_ERROR "unsafe-to-concat test failed.");
      ret = جھوٹ;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_بفر_سیٹ_لمبائی (بفر, 0);
      hb_بفر_append (بفر, reconstruction, 0, -1);
    }
  }

  واپس ret;
}

منطقی
hb_بفر_t::verify (hb_بفر_t        *متن_بفر,
		     hb_font_t          *font,
		     ثابت hb_feature_t *features,
		     غیرعلامتی صحیح        عدد_features,
		     ثابت حرف * ثابت *shapers)
{
  منطقی ret = سچ;
  منطقی monotone = بفر_verify_monotone (یہ, font);
  اگر (!monotone)
    ret = جھوٹ;
  اگر (monotone &&
      !بفر_verify_unsafe_مقصد_break (یہ, متن_بفر, font, features, عدد_features, shapers))
    ret = جھوٹ;
  اگر (monotone &&
      (flags & HB_بفر_FLAG_PRODUCE_UNSAFE_مقصد_CONCAT) != 0 &&
      !بفر_verify_unsafe_مقصد_concat (یہ, متن_بفر, font, features, عدد_features, shapers))
    ret = جھوٹ;
  اگر (!ret)
  {
#اگر_غیر_تعریف_شدہ HB_NO_بفر_SERIALIZE
    غیرعلامتی len = متن_بفر->len;
    hb_سمتیہ_t<حرف> بائٹیں;
    اگر (likely (بائٹیں.resize (len * 10 + 16)))
    {
      hb_بفر_serialize_unicode (متن_بفر,
				   0, len,
				   بائٹیں.arrayZ, بائٹیں.لمبائی,
				   &len,
				   HB_بفر_SERIALIZE_FORMAT_متن,
				   HB_بفر_SERIALIZE_FLAG_NO_CLUSTERS);
      بفر_verify_error (یہ, font, بفر_VERIFY_ERROR "text was: %s.", بائٹیں.arrayZ ? بائٹیں.arrayZ : "");
    }
#اگر_ختم
  }
  واپس ret;
}


#اگر_ختم
