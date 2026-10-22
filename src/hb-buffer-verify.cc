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

#اگر_تعریف_نشده HB_NO_میانگیر_VERIFY

#ضمن "hb-buffer.hh"


#تعریف_کن میانگیر_VERIFY_ERROR "buffer verify error: "
ساكن مضمن فراغ
میانگیر_verify_error (hb_میانگیر_t *میانگیر,
		     hb_font_t *font,
		     ثابت حرف *fmt,
		     ...) HB_PRINTF_FUNC(3, 4);

ساكن مضمن فراغ
میانگیر_verify_error (hb_میانگیر_t *میانگیر,
		     hb_font_t *font,
		     ثابت حرف *fmt,
		     ...)
{
  va_list ap;
  va_شروع (ap, fmt);
  إذا (میانگیر->messaging ())
  {
    میانگیر->پیام_impl (font, fmt, ap);
  }
  وإلا
  {
    fprintf (stderr, "harfbuzz ");
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
  }
  va_انتها (ap);
}

ساكن منطقي
میانگیر_verify_monotone (hb_میانگیر_t *میانگیر,
			hb_font_t   *font)
{
  إذا (!HB_میانگیر_CLUSTER_LEVEL_IS_MONOTONE (میانگیر->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    أرجع درست;
  }

  منطقي is_forward = HB_DIRECTION_IS_FORWARD (hb_میانگیر_get_direction (میانگیر));

  بیعلامت صحيح رقم_glyphs;
  hb_glyph_info_t *info = hb_میانگیر_get_glyph_infos (میانگیر, &رقم_glyphs);

  لكل (بیعلامت صحيح i = 1; i < رقم_glyphs; i++)
    إذا (info[i-1].cluster != info[i].cluster &&
	(info[i-1].cluster < info[i].cluster) != is_forward)
    {
      میانگیر_verify_error (میانگیر, font, میانگیر_VERIFY_ERROR "clusters are not monotone.");
      أرجع خطأ;
    }

  أرجع درست;
}

ساكن منطقي
میانگیر_verify_unsafe_مقصد_break (hb_میانگیر_t  *میانگیر,
			       hb_میانگیر_t  *متن_میانگیر,
			       hb_font_t          *font,
			       ثابت hb_feature_t *features,
			       بیعلامت صحيح        رقم_features,
			       ثابت حرف * ثابت *shapers)
{
  إذا (!HB_میانگیر_CLUSTER_LEVEL_IS_MONOTONE (میانگیر->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    أرجع درست;
  }

  /* Check that breaking up shaping at safe-to-break is indeed safe. */

  hb_unique_ptr_t<hb_میانگیر_t> fragment (hb_میانگیر_create_similar (میانگیر));
  hb_میانگیر_مجموعه_flags (fragment, (hb_میانگیر_flags_t (hb_میانگیر_get_flags (fragment) & ~HB_میانگیر_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_میانگیر_t> reconstruction (hb_میانگیر_create_similar (میانگیر));
  hb_میانگیر_مجموعه_flags (reconstruction, (hb_میانگیر_flags_t (hb_میانگیر_get_flags (reconstruction) & ~HB_میانگیر_FLAG_VERIFY)));

  بیعلامت صحيح رقم_glyphs;
  hb_glyph_info_t *info = hb_میانگیر_get_glyph_infos (میانگیر, &رقم_glyphs);

  بیعلامت صحيح رقم_chars;
  hb_glyph_info_t *متن = hb_میانگیر_get_glyph_infos (متن_میانگیر, &رقم_chars);

  /* Chop text and shape fragments. */
  منطقي forward = HB_DIRECTION_IS_FORWARD (hb_میانگیر_get_direction (میانگیر));
  بیعلامت صحيح شروع = 0;
  بیعلامت صحيح متن_شروع = forward ? 0 : رقم_chars;
  بیعلامت صحيح متن_انتها = متن_شروع;
  لكل (بیعلامت صحيح انتها = 1; انتها < رقم_glyphs + 1; انتها++)
  {
    إذا (انتها < رقم_glyphs &&
	(info[انتها].cluster == info[انتها-1].cluster ||
	 info[انتها-(forward?0:1)].ماسک & HB_GLYPH_FLAG_UNSAFE_مقصد_BREAK))
	استمر;

    /* Shape segment corresponding to glyphs start..end. */
    إذا (انتها == رقم_glyphs)
    {
      إذا (forward)
	متن_انتها = رقم_chars;
      وإلا
	متن_شروع = 0;
    }
    وإلا
    {
      إذا (forward)
      {
	بیعلامت صحيح cluster = info[انتها].cluster;
	طالما (متن_انتها < رقم_chars && متن[متن_انتها].cluster < cluster)
	  متن_انتها++;
      }
      وإلا
      {
	بیعلامت صحيح cluster = info[انتها - 1].cluster;
	طالما (متن_شروع && متن[متن_شروع - 1].cluster >= cluster)
	  متن_شروع--;
      }
    }
    إذا (unlikely (متن_شروع >= متن_انتها))
    {
      میانگیر_verify_error (میانگیر, font, میانگیر_VERIFY_ERROR "unsafe-to-break text range is invalid.");
      أرجع خطأ;
    }

    إذا (خطأ)
      چاپ_کن("start %u end %u text start %u end %u\n", شروع, انتها, متن_شروع, متن_انتها);

    hb_میانگیر_clear_contents (fragment);

    hb_میانگیر_flags_t flags = hb_میانگیر_get_flags (fragment);
    إذا (0 < متن_شروع)
      flags = (hb_میانگیر_flags_t) (flags & ~HB_میانگیر_FLAG_BOT);
    إذا (متن_انتها < رقم_chars)
      flags = (hb_میانگیر_flags_t) (flags & ~HB_میانگیر_FLAG_EOT);
    hb_میانگیر_مجموعه_flags (fragment, flags);

    hb_میانگیر_append (fragment, متن_میانگیر, متن_شروع, متن_انتها);
    إذا (!hb_shape_پر (font, fragment, features, رقم_features, shapers) ||
	!fragment->successful)
      أرجع درست;
    hb_میانگیر_append (reconstruction, fragment, 0, -1);

    شروع = انتها;
    إذا (forward)
      متن_شروع = متن_انتها;
    وإلا
      متن_انتها = متن_شروع;
  }

  منطقي ret = درست;
  إذا (likely (reconstruction->successful))
  {
    hb_میانگیر_diff_flags_t diff = hb_میانگیر_diff (reconstruction, میانگیر, (hb_codepoint_t) -1, 0);
    إذا (diff & ~HB_میانگیر_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      میانگیر_verify_error (میانگیر, font, میانگیر_VERIFY_ERROR "unsafe-to-break test failed.");
      ret = خطأ;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_میانگیر_مجموعه_درازی (میانگیر, 0);
      hb_میانگیر_append (میانگیر, reconstruction, 0, -1);
    }
  }

  أرجع ret;
}

ساكن منطقي
میانگیر_verify_unsafe_مقصد_concat (hb_میانگیر_t        *میانگیر,
				hb_میانگیر_t        *متن_میانگیر,
				hb_font_t          *font,
				ثابت hb_feature_t *features,
				بیعلامت صحيح        رقم_features,
				ثابت حرف * ثابت *shapers)
{
  إذا (!HB_میانگیر_CLUSTER_LEVEL_IS_MONOTONE (میانگیر->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    أرجع درست;
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

  hb_unique_ptr_t<hb_میانگیر_t> fragments[2] {
    hb_unique_ptr_t<hb_میانگیر_t> (hb_میانگیر_create_similar (میانگیر)),
    hb_unique_ptr_t<hb_میانگیر_t> (hb_میانگیر_create_similar (میانگیر)),
  };
  hb_میانگیر_مجموعه_flags (fragments[0], (hb_میانگیر_flags_t (hb_میانگیر_get_flags (fragments[0]) & ~HB_میانگیر_FLAG_VERIFY)));
  hb_میانگیر_مجموعه_flags (fragments[1], (hb_میانگیر_flags_t (hb_میانگیر_get_flags (fragments[1]) & ~HB_میانگیر_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_میانگیر_t> reconstruction (hb_میانگیر_create_similar (میانگیر));
  hb_میانگیر_مجموعه_flags (reconstruction, (hb_میانگیر_flags_t (hb_میانگیر_get_flags (reconstruction) & ~HB_میانگیر_FLAG_VERIFY)));
  hb_segment_properties_t props;
  hb_میانگیر_get_segment_properties (میانگیر, &props);
  hb_میانگیر_مجموعه_segment_properties (fragments[0], &props);
  hb_میانگیر_مجموعه_segment_properties (fragments[1], &props);
  hb_میانگیر_مجموعه_segment_properties (reconstruction, &props);

  بیعلامت رقم_glyphs;
  hb_glyph_info_t *info = hb_میانگیر_get_glyph_infos (میانگیر, &رقم_glyphs);

  بیعلامت رقم_chars;
  hb_glyph_info_t *متن = hb_میانگیر_get_glyph_infos (متن_میانگیر, &رقم_chars);

  منطقي forward = HB_DIRECTION_IS_FORWARD (hb_میانگیر_get_direction (میانگیر));

  إذا (!forward)
    hb_میانگیر_معکوس (میانگیر);

  /*
   * Split text into segments and collect into to fragment streams.
   */
  {
    بیعلامت fragment_idx = 0;
    بیعلامت شروع = 0;
    بیعلامت متن_شروع = 0;
    بیعلامت متن_انتها = 0;
    لكل (بیعلامت انتها = 1; انتها < رقم_glyphs + 1; انتها++)
    {
      إذا (انتها < رقم_glyphs &&
	  (info[انتها].cluster == info[انتها-1].cluster ||
	   info[انتها].ماسک & HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT))
	  استمر;

      /* Accumulate segment corresponding to glyphs start..end. */
      إذا (انتها == رقم_glyphs)
	متن_انتها = رقم_chars;
      وإلا
      {
	بیعلامت cluster = info[انتها].cluster;
	طالما (متن_انتها < رقم_chars && متن[متن_انتها].cluster < cluster)
	  متن_انتها++;
      }
      أكد (متن_شروع < متن_انتها);

      إذا (خطأ)
	چاپ_کن("start %u end %u text start %u end %u\n", شروع, انتها, متن_شروع, متن_انتها);

#إذا 0
      hb_میانگیر_flags_t flags = hb_میانگیر_get_flags (fragment);
      إذا (0 < متن_شروع)
	flags = (hb_میانگیر_flags_t) (flags & ~HB_میانگیر_FLAG_BOT);
      إذا (متن_انتها < رقم_chars)
	flags = (hb_میانگیر_flags_t) (flags & ~HB_میانگیر_FLAG_EOT);
      hb_میانگیر_مجموعه_flags (fragment, flags);
#پایان_اگر

      hb_میانگیر_append (fragments[fragment_idx], متن_میانگیر, متن_شروع, متن_انتها);

      شروع = انتها;
      متن_شروع = متن_انتها;
      fragment_idx = 1 - fragment_idx;
    }
  }

  منطقي ret = درست;
  /*
   * Shape the two fragment streams.
   */
  إذا (!hb_shape_پر (font, fragments[0], features, رقم_features, shapers) ||
      !fragments[0]->successful)
    أرجع ret;

  إذا (!hb_shape_پر (font, fragments[1], features, رقم_features, shapers) ||
      !fragments[1]->successful)
    أرجع ret;

  إذا (!forward)
  {
    hb_میانگیر_معکوس (fragments[0]);
    hb_میانگیر_معکوس (fragments[1]);
  }

  /*
   * Reconstruct results.
   */
  {
    بیعلامت fragment_idx = 0;
    بیعلامت fragment_شروع[2] {0, 0};
    بیعلامت fragment_رقم_glyphs[2];
    hb_glyph_info_t *fragment_info[2];
    لكل (بیعلامت i = 0; i < 2; i++)
      fragment_info[i] = hb_میانگیر_get_glyph_infos (fragments[i], &fragment_رقم_glyphs[i]);
    طالما (fragment_شروع[0] < fragment_رقم_glyphs[0] ||
	   fragment_شروع[1] < fragment_رقم_glyphs[1])
    {
      بیعلامت fragment_انتها = fragment_شروع[fragment_idx] + 1;
      طالما (fragment_انتها < fragment_رقم_glyphs[fragment_idx] &&
	     (fragment_info[fragment_idx][fragment_انتها].cluster == fragment_info[fragment_idx][fragment_انتها - 1].cluster ||
	      fragment_info[fragment_idx][fragment_انتها].ماسک & HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT))
	fragment_انتها++;

      hb_میانگیر_append (reconstruction, fragments[fragment_idx], fragment_شروع[fragment_idx], fragment_انتها);

      fragment_شروع[fragment_idx] = fragment_انتها;
      fragment_idx = 1 - fragment_idx;
    }
  }

  إذا (!forward)
  {
    hb_میانگیر_معکوس (میانگیر);
    hb_میانگیر_معکوس (reconstruction);
  }

  إذا (likely (reconstruction->successful))
  {
    /*
     * Diff results.
     */
    hb_میانگیر_diff_flags_t diff = hb_میانگیر_diff (reconstruction, میانگیر, (hb_codepoint_t) -1, 0);
    إذا (diff & ~HB_میانگیر_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      میانگیر_verify_error (میانگیر, font, میانگیر_VERIFY_ERROR "unsafe-to-concat test failed.");
      ret = خطأ;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_میانگیر_مجموعه_درازی (میانگیر, 0);
      hb_میانگیر_append (میانگیر, reconstruction, 0, -1);
    }
  }

  أرجع ret;
}

منطقي
hb_میانگیر_t::verify (hb_میانگیر_t        *متن_میانگیر,
		     hb_font_t          *font,
		     ثابت hb_feature_t *features,
		     بیعلامت صحيح        رقم_features,
		     ثابت حرف * ثابت *shapers)
{
  منطقي ret = درست;
  منطقي monotone = میانگیر_verify_monotone (هذا, font);
  إذا (!monotone)
    ret = خطأ;
  إذا (monotone &&
      !میانگیر_verify_unsafe_مقصد_break (هذا, متن_میانگیر, font, features, رقم_features, shapers))
    ret = خطأ;
  إذا (monotone &&
      (flags & HB_میانگیر_FLAG_PRODUCE_UNSAFE_مقصد_CONCAT) != 0 &&
      !میانگیر_verify_unsafe_مقصد_concat (هذا, متن_میانگیر, font, features, رقم_features, shapers))
    ret = خطأ;
  إذا (!ret)
  {
#اگر_تعریف_نشده HB_NO_میانگیر_SERIALIZE
    بیعلامت len = متن_میانگیر->len;
    hb_وکتور_t<حرف> بایتها;
    إذا (likely (بایتها.resize (len * 10 + 16)))
    {
      hb_میانگیر_serialize_unicode (متن_میانگیر,
				   0, len,
				   بایتها.arrayZ, بایتها.درازی,
				   &len,
				   HB_میانگیر_SERIALIZE_FORMAT_متن,
				   HB_میانگیر_SERIALIZE_FLAG_NO_CLUSTERS);
      میانگیر_verify_error (هذا, font, میانگیر_VERIFY_ERROR "text was: %s.", بایتها.arrayZ ? بایتها.arrayZ : "");
    }
#پایان_اگر
  }
  أرجع ret;
}


#پایان_اگر
