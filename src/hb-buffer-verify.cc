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

#جے_بنا_تعریف HB_NO_بفر_VERIFY

#ضمن "hb-buffer.hh"


#تعریف_کرو بفر_VERIFY_ERROR "buffer verify error: "
ساكن مضمن فراغ
بفر_verify_error (hb_بفر_t *بفر,
		     hb_font_t *font,
		     ثابت حرف *fmt,
		     ...) HB_PRINTF_FUNC(3, 4);

ساكن مضمن فراغ
بفر_verify_error (hb_بفر_t *بفر,
		     hb_font_t *font,
		     ثابت حرف *fmt,
		     ...)
{
  va_list ap;
  va_شروع (ap, fmt);
  إذا (بفر->messaging ())
  {
    بفر->سنیہا_impl (font, fmt, ap);
  }
  وإلا
  {
    fprintf (stderr, "harfbuzz ");
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
  }
  va_سرا (ap);
}

ساكن منطقي
بفر_verify_monotone (hb_بفر_t *بفر,
			hb_font_t   *font)
{
  إذا (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    أرجع سچ;
  }

  منطقي is_forward = HB_DIRECTION_IS_FORWARD (hb_بفر_get_direction (بفر));

  بنانشانی صحيح عدد_glyphs;
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, &عدد_glyphs);

  لكل (بنانشانی صحيح i = 1; i < عدد_glyphs; i++)
    إذا (info[i-1].cluster != info[i].cluster &&
	(info[i-1].cluster < info[i].cluster) != is_forward)
    {
      بفر_verify_error (بفر, font, بفر_VERIFY_ERROR "clusters are not monotone.");
      أرجع خطأ;
    }

  أرجع سچ;
}

ساكن منطقي
بفر_verify_unsafe_منزل_break (hb_بفر_t  *بفر,
			       hb_بفر_t  *متن_بفر,
			       hb_font_t          *font,
			       ثابت hb_feature_t *features,
			       بنانشانی صحيح        عدد_features,
			       ثابت حرف * ثابت *shapers)
{
  إذا (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    أرجع سچ;
  }

  /* Check that breaking up shaping at safe-to-break is indeed safe. */

  hb_unique_ptr_t<hb_بفر_t> fragment (hb_بفر_create_similar (بفر));
  hb_بفر_سیٹ_flags (fragment, (hb_بفر_flags_t (hb_بفر_get_flags (fragment) & ~HB_بفر_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_بفر_t> reconstruction (hb_بفر_create_similar (بفر));
  hb_بفر_سیٹ_flags (reconstruction, (hb_بفر_flags_t (hb_بفر_get_flags (reconstruction) & ~HB_بفر_FLAG_VERIFY)));

  بنانشانی صحيح عدد_glyphs;
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, &عدد_glyphs);

  بنانشانی صحيح عدد_chars;
  hb_glyph_info_t *متن = hb_بفر_get_glyph_infos (متن_بفر, &عدد_chars);

  /* Chop text and shape fragments. */
  منطقي forward = HB_DIRECTION_IS_FORWARD (hb_بفر_get_direction (بفر));
  بنانشانی صحيح شروع = 0;
  بنانشانی صحيح متن_شروع = forward ? 0 : عدد_chars;
  بنانشانی صحيح متن_سرا = متن_شروع;
  لكل (بنانشانی صحيح سرا = 1; سرا < عدد_glyphs + 1; سرا++)
  {
    إذا (سرا < عدد_glyphs &&
	(info[سرا].cluster == info[سرا-1].cluster ||
	 info[سرا-(forward?0:1)].نقاب & HB_GLYPH_FLAG_UNSAFE_منزل_BREAK))
	استمر;

    /* Shape segment corresponding to glyphs start..end. */
    إذا (سرا == عدد_glyphs)
    {
      إذا (forward)
	متن_سرا = عدد_chars;
      وإلا
	متن_شروع = 0;
    }
    وإلا
    {
      إذا (forward)
      {
	بنانشانی صحيح cluster = info[سرا].cluster;
	طالما (متن_سرا < عدد_chars && متن[متن_سرا].cluster < cluster)
	  متن_سرا++;
      }
      وإلا
      {
	بنانشانی صحيح cluster = info[سرا - 1].cluster;
	طالما (متن_شروع && متن[متن_شروع - 1].cluster >= cluster)
	  متن_شروع--;
      }
    }
    إذا (unlikely (متن_شروع >= متن_سرا))
    {
      بفر_verify_error (بفر, font, بفر_VERIFY_ERROR "unsafe-to-break text range is invalid.");
      أرجع خطأ;
    }

    إذا (خطأ)
      لکھو("start %u end %u text start %u end %u\n", شروع, سرا, متن_شروع, متن_سرا);

    hb_بفر_clear_contents (fragment);

    hb_بفر_flags_t flags = hb_بفر_get_flags (fragment);
    إذا (0 < متن_شروع)
      flags = (hb_بفر_flags_t) (flags & ~HB_بفر_FLAG_BOT);
    إذا (متن_سرا < عدد_chars)
      flags = (hb_بفر_flags_t) (flags & ~HB_بفر_FLAG_EOT);
    hb_بفر_سیٹ_flags (fragment, flags);

    hb_بفر_append (fragment, متن_بفر, متن_شروع, متن_سرا);
    إذا (!hb_shape_بھریا (font, fragment, features, عدد_features, shapers) ||
	!fragment->successful)
      أرجع سچ;
    hb_بفر_append (reconstruction, fragment, 0, -1);

    شروع = سرا;
    إذا (forward)
      متن_شروع = متن_سرا;
    وإلا
      متن_سرا = متن_شروع;
  }

  منطقي ret = سچ;
  إذا (likely (reconstruction->successful))
  {
    hb_بفر_diff_flags_t diff = hb_بفر_diff (reconstruction, بفر, (hb_codepoint_t) -1, 0);
    إذا (diff & ~HB_بفر_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      بفر_verify_error (بفر, font, بفر_VERIFY_ERROR "unsafe-to-break test failed.");
      ret = خطأ;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_بفر_سیٹ_لمائی (بفر, 0);
      hb_بفر_append (بفر, reconstruction, 0, -1);
    }
  }

  أرجع ret;
}

ساكن منطقي
بفر_verify_unsafe_منزل_concat (hb_بفر_t        *بفر,
				hb_بفر_t        *متن_بفر,
				hb_font_t          *font,
				ثابت hb_feature_t *features,
				بنانشانی صحيح        عدد_features,
				ثابت حرف * ثابت *shapers)
{
  إذا (!HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    أرجع سچ;
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

  بنانشانی عدد_glyphs;
  hb_glyph_info_t *info = hb_بفر_get_glyph_infos (بفر, &عدد_glyphs);

  بنانشانی عدد_chars;
  hb_glyph_info_t *متن = hb_بفر_get_glyph_infos (متن_بفر, &عدد_chars);

  منطقي forward = HB_DIRECTION_IS_FORWARD (hb_بفر_get_direction (بفر));

  إذا (!forward)
    hb_بفر_الٹ (بفر);

  /*
   * Split text into segments and collect into to fragment streams.
   */
  {
    بنانشانی fragment_idx = 0;
    بنانشانی شروع = 0;
    بنانشانی متن_شروع = 0;
    بنانشانی متن_سرا = 0;
    لكل (بنانشانی سرا = 1; سرا < عدد_glyphs + 1; سرا++)
    {
      إذا (سرا < عدد_glyphs &&
	  (info[سرا].cluster == info[سرا-1].cluster ||
	   info[سرا].نقاب & HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT))
	  استمر;

      /* Accumulate segment corresponding to glyphs start..end. */
      إذا (سرا == عدد_glyphs)
	متن_سرا = عدد_chars;
      وإلا
      {
	بنانشانی cluster = info[سرا].cluster;
	طالما (متن_سرا < عدد_chars && متن[متن_سرا].cluster < cluster)
	  متن_سرا++;
      }
      أكد (متن_شروع < متن_سرا);

      إذا (خطأ)
	لکھو("start %u end %u text start %u end %u\n", شروع, سرا, متن_شروع, متن_سرا);

#إذا 0
      hb_بفر_flags_t flags = hb_بفر_get_flags (fragment);
      إذا (0 < متن_شروع)
	flags = (hb_بفر_flags_t) (flags & ~HB_بفر_FLAG_BOT);
      إذا (متن_سرا < عدد_chars)
	flags = (hb_بفر_flags_t) (flags & ~HB_بفر_FLAG_EOT);
      hb_بفر_سیٹ_flags (fragment, flags);
#جے_انت

      hb_بفر_append (fragments[fragment_idx], متن_بفر, متن_شروع, متن_سرا);

      شروع = سرا;
      متن_شروع = متن_سرا;
      fragment_idx = 1 - fragment_idx;
    }
  }

  منطقي ret = سچ;
  /*
   * Shape the two fragment streams.
   */
  إذا (!hb_shape_بھریا (font, fragments[0], features, عدد_features, shapers) ||
      !fragments[0]->successful)
    أرجع ret;

  إذا (!hb_shape_بھریا (font, fragments[1], features, عدد_features, shapers) ||
      !fragments[1]->successful)
    أرجع ret;

  إذا (!forward)
  {
    hb_بفر_الٹ (fragments[0]);
    hb_بفر_الٹ (fragments[1]);
  }

  /*
   * Reconstruct results.
   */
  {
    بنانشانی fragment_idx = 0;
    بنانشانی fragment_شروع[2] {0, 0};
    بنانشانی fragment_عدد_glyphs[2];
    hb_glyph_info_t *fragment_info[2];
    لكل (بنانشانی i = 0; i < 2; i++)
      fragment_info[i] = hb_بفر_get_glyph_infos (fragments[i], &fragment_عدد_glyphs[i]);
    طالما (fragment_شروع[0] < fragment_عدد_glyphs[0] ||
	   fragment_شروع[1] < fragment_عدد_glyphs[1])
    {
      بنانشانی fragment_سرا = fragment_شروع[fragment_idx] + 1;
      طالما (fragment_سرا < fragment_عدد_glyphs[fragment_idx] &&
	     (fragment_info[fragment_idx][fragment_سرا].cluster == fragment_info[fragment_idx][fragment_سرا - 1].cluster ||
	      fragment_info[fragment_idx][fragment_سرا].نقاب & HB_GLYPH_FLAG_UNSAFE_منزل_CONCAT))
	fragment_سرا++;

      hb_بفر_append (reconstruction, fragments[fragment_idx], fragment_شروع[fragment_idx], fragment_سرا);

      fragment_شروع[fragment_idx] = fragment_سرا;
      fragment_idx = 1 - fragment_idx;
    }
  }

  إذا (!forward)
  {
    hb_بفر_الٹ (بفر);
    hb_بفر_الٹ (reconstruction);
  }

  إذا (likely (reconstruction->successful))
  {
    /*
     * Diff results.
     */
    hb_بفر_diff_flags_t diff = hb_بفر_diff (reconstruction, بفر, (hb_codepoint_t) -1, 0);
    إذا (diff & ~HB_بفر_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      بفر_verify_error (بفر, font, بفر_VERIFY_ERROR "unsafe-to-concat test failed.");
      ret = خطأ;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_بفر_سیٹ_لمائی (بفر, 0);
      hb_بفر_append (بفر, reconstruction, 0, -1);
    }
  }

  أرجع ret;
}

منطقي
hb_بفر_t::verify (hb_بفر_t        *متن_بفر,
		     hb_font_t          *font,
		     ثابت hb_feature_t *features,
		     بنانشانی صحيح        عدد_features,
		     ثابت حرف * ثابت *shapers)
{
  منطقي ret = سچ;
  منطقي monotone = بفر_verify_monotone (هذا, font);
  إذا (!monotone)
    ret = خطأ;
  إذا (monotone &&
      !بفر_verify_unsafe_منزل_break (هذا, متن_بفر, font, features, عدد_features, shapers))
    ret = خطأ;
  إذا (monotone &&
      (flags & HB_بفر_FLAG_PRODUCE_UNSAFE_منزل_CONCAT) != 0 &&
      !بفر_verify_unsafe_منزل_concat (هذا, متن_بفر, font, features, عدد_features, shapers))
    ret = خطأ;
  إذا (!ret)
  {
#جے_بنا_تعریف HB_NO_بفر_SERIALIZE
    بنانشانی len = متن_بفر->len;
    hb_سمتیہ_t<حرف> بائٹاں;
    إذا (likely (بائٹاں.resize (len * 10 + 16)))
    {
      hb_بفر_serialize_unicode (متن_بفر,
				   0, len,
				   بائٹاں.arrayZ, بائٹاں.لمائی,
				   &len,
				   HB_بفر_SERIALIZE_FORMAT_متن,
				   HB_بفر_SERIALIZE_FLAG_NO_CLUSTERS);
      بفر_verify_error (هذا, font, بفر_VERIFY_ERROR "text was: %s.", بائٹاں.arrayZ ? بائٹاں.arrayZ : "");
    }
#جے_انت
  }
  أرجع ret;
}


#جے_انت
