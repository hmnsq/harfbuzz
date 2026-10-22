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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_VERIFY

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer.hh"


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਬਫਰ_VERIFY_ERROR "buffer verify error: "
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਬਫਰ_verify_error (hb_ਬਫਰ_t *ਬਫਰ,
		     hb_font_t *font,
		     ਅਚਲ ਅੱਖਰ *fmt,
		     ...) HB_PRINTF_FUNC(3, 4);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਬਫਰ_verify_error (hb_ਬਫਰ_t *ਬਫਰ,
		     hb_font_t *font,
		     ਅਚਲ ਅੱਖਰ *fmt,
		     ...)
{
  va_list ap;
  va_ਸ਼ੁਰੂ (ap, fmt);
  ਜੇ (ਬਫਰ->messaging ())
  {
    ਬਫਰ->ਸੁਨੇਹਾ_impl (font, fmt, ap);
  }
  ਨਹੀਂਤਾਂ
  {
    fprintf (stderr, "harfbuzz ");
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
  }
  va_ਸਿਰਾ (ap);
}

ਸਥਿਰ ਤਾਰਕਿਕ
ਬਫਰ_verify_monotone (hb_ਬਫਰ_t *ਬਫਰ,
			hb_font_t   *font)
{
  ਜੇ (!HB_ਬਫਰ_CLUSTER_LEVEL_IS_MONOTONE (ਬਫਰ->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ is_forward = HB_DIRECTION_IS_FORWARD (hb_ਬਫਰ_get_direction (ਬਫਰ));

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs;
  hb_glyph_info_t *info = hb_ਬਫਰ_get_glyph_infos (ਬਫਰ, &ਅੰਕ_glyphs);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਅੰਕ_glyphs; i++)
    ਜੇ (info[i-1].cluster != info[i].cluster &&
	(info[i-1].cluster < info[i].cluster) != is_forward)
    {
      ਬਫਰ_verify_error (ਬਫਰ, font, ਬਫਰ_VERIFY_ERROR "clusters are not monotone.");
      ਵਾਪਸ ਝੂਠ;
    }

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
ਬਫਰ_verify_unsafe_ਮੰਜ਼ਿਲ_break (hb_ਬਫਰ_t  *ਬਫਰ,
			       hb_ਬਫਰ_t  *ਪਾਠ_ਬਫਰ,
			       hb_font_t          *font,
			       ਅਚਲ hb_feature_t *features,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features,
			       ਅਚਲ ਅੱਖਰ * ਅਚਲ *shapers)
{
  ਜੇ (!HB_ਬਫਰ_CLUSTER_LEVEL_IS_MONOTONE (ਬਫਰ->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    ਵਾਪਸ ਸੱਚ;
  }

  /* Check that breaking up shaping at safe-to-break is indeed safe. */

  hb_unique_ptr_t<hb_ਬਫਰ_t> fragment (hb_ਬਫਰ_create_similar (ਬਫਰ));
  hb_ਬਫਰ_ਸੈੱਟ_flags (fragment, (hb_ਬਫਰ_flags_t (hb_ਬਫਰ_get_flags (fragment) & ~HB_ਬਫਰ_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_ਬਫਰ_t> reconstruction (hb_ਬਫਰ_create_similar (ਬਫਰ));
  hb_ਬਫਰ_ਸੈੱਟ_flags (reconstruction, (hb_ਬਫਰ_flags_t (hb_ਬਫਰ_get_flags (reconstruction) & ~HB_ਬਫਰ_FLAG_VERIFY)));

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs;
  hb_glyph_info_t *info = hb_ਬਫਰ_get_glyph_infos (ਬਫਰ, &ਅੰਕ_glyphs);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_chars;
  hb_glyph_info_t *ਪਾਠ = hb_ਬਫਰ_get_glyph_infos (ਪਾਠ_ਬਫਰ, &ਅੰਕ_chars);

  /* Chop text and shape fragments. */
  ਤਾਰਕਿਕ forward = HB_DIRECTION_IS_FORWARD (hb_ਬਫਰ_get_direction (ਬਫਰ));
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪਾਠ_ਸ਼ੁਰੂ = forward ? 0 : ਅੰਕ_chars;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪਾਠ_ਸਿਰਾ = ਪਾਠ_ਸ਼ੁਰੂ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = 1; ਸਿਰਾ < ਅੰਕ_glyphs + 1; ਸਿਰਾ++)
  {
    ਜੇ (ਸਿਰਾ < ਅੰਕ_glyphs &&
	(info[ਸਿਰਾ].cluster == info[ਸਿਰਾ-1].cluster ||
	 info[ਸਿਰਾ-(forward?0:1)].ਨਕਾਬ & HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_BREAK))
	ਜਾਰੀ;

    /* Shape segment corresponding to glyphs start..end. */
    ਜੇ (ਸਿਰਾ == ਅੰਕ_glyphs)
    {
      ਜੇ (forward)
	ਪਾਠ_ਸਿਰਾ = ਅੰਕ_chars;
      ਨਹੀਂਤਾਂ
	ਪਾਠ_ਸ਼ੁਰੂ = 0;
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (forward)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = info[ਸਿਰਾ].cluster;
	ਜਦੋਂ (ਪਾਠ_ਸਿਰਾ < ਅੰਕ_chars && ਪਾਠ[ਪਾਠ_ਸਿਰਾ].cluster < cluster)
	  ਪਾਠ_ਸਿਰਾ++;
      }
      ਨਹੀਂਤਾਂ
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = info[ਸਿਰਾ - 1].cluster;
	ਜਦੋਂ (ਪਾਠ_ਸ਼ੁਰੂ && ਪਾਠ[ਪਾਠ_ਸ਼ੁਰੂ - 1].cluster >= cluster)
	  ਪਾਠ_ਸ਼ੁਰੂ--;
      }
    }
    ਜੇ (unlikely (ਪਾਠ_ਸ਼ੁਰੂ >= ਪਾਠ_ਸਿਰਾ))
    {
      ਬਫਰ_verify_error (ਬਫਰ, font, ਬਫਰ_VERIFY_ERROR "unsafe-to-break text range is invalid.");
      ਵਾਪਸ ਝੂਠ;
    }

    ਜੇ (ਝੂਠ)
      ਲਿਖੋ("start %u end %u text start %u end %u\n", ਸ਼ੁਰੂ, ਸਿਰਾ, ਪਾਠ_ਸ਼ੁਰੂ, ਪਾਠ_ਸਿਰਾ);

    hb_ਬਫਰ_clear_contents (fragment);

    hb_ਬਫਰ_flags_t flags = hb_ਬਫਰ_get_flags (fragment);
    ਜੇ (0 < ਪਾਠ_ਸ਼ੁਰੂ)
      flags = (hb_ਬਫਰ_flags_t) (flags & ~HB_ਬਫਰ_FLAG_BOT);
    ਜੇ (ਪਾਠ_ਸਿਰਾ < ਅੰਕ_chars)
      flags = (hb_ਬਫਰ_flags_t) (flags & ~HB_ਬਫਰ_FLAG_EOT);
    hb_ਬਫਰ_ਸੈੱਟ_flags (fragment, flags);

    hb_ਬਫਰ_append (fragment, ਪਾਠ_ਬਫਰ, ਪਾਠ_ਸ਼ੁਰੂ, ਪਾਠ_ਸਿਰਾ);
    ਜੇ (!hb_shape_ਭਰਿਆ (font, fragment, features, ਅੰਕ_features, shapers) ||
	!fragment->successful)
      ਵਾਪਸ ਸੱਚ;
    hb_ਬਫਰ_append (reconstruction, fragment, 0, -1);

    ਸ਼ੁਰੂ = ਸਿਰਾ;
    ਜੇ (forward)
      ਪਾਠ_ਸ਼ੁਰੂ = ਪਾਠ_ਸਿਰਾ;
    ਨਹੀਂਤਾਂ
      ਪਾਠ_ਸਿਰਾ = ਪਾਠ_ਸ਼ੁਰੂ;
  }

  ਤਾਰਕਿਕ ret = ਸੱਚ;
  ਜੇ (likely (reconstruction->successful))
  {
    hb_ਬਫਰ_diff_flags_t diff = hb_ਬਫਰ_diff (reconstruction, ਬਫਰ, (hb_codepoint_t) -1, 0);
    ਜੇ (diff & ~HB_ਬਫਰ_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      ਬਫਰ_verify_error (ਬਫਰ, font, ਬਫਰ_VERIFY_ERROR "unsafe-to-break test failed.");
      ret = ਝੂਠ;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_ਬਫਰ_ਸੈੱਟ_ਲੰਮਾਈ (ਬਫਰ, 0);
      hb_ਬਫਰ_append (ਬਫਰ, reconstruction, 0, -1);
    }
  }

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਤਾਰਕਿਕ
ਬਫਰ_verify_unsafe_ਮੰਜ਼ਿਲ_concat (hb_ਬਫਰ_t        *ਬਫਰ,
				hb_ਬਫਰ_t        *ਪਾਠ_ਬਫਰ,
				hb_font_t          *font,
				ਅਚਲ hb_feature_t *features,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features,
				ਅਚਲ ਅੱਖਰ * ਅਚਲ *shapers)
{
  ਜੇ (!HB_ਬਫਰ_CLUSTER_LEVEL_IS_MONOTONE (ਬਫਰ->cluster_level))
  {
    /* Cannot perform this check without monotone clusters. */
    ਵਾਪਸ ਸੱਚ;
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

  hb_unique_ptr_t<hb_ਬਫਰ_t> fragments[2] {
    hb_unique_ptr_t<hb_ਬਫਰ_t> (hb_ਬਫਰ_create_similar (ਬਫਰ)),
    hb_unique_ptr_t<hb_ਬਫਰ_t> (hb_ਬਫਰ_create_similar (ਬਫਰ)),
  };
  hb_ਬਫਰ_ਸੈੱਟ_flags (fragments[0], (hb_ਬਫਰ_flags_t (hb_ਬਫਰ_get_flags (fragments[0]) & ~HB_ਬਫਰ_FLAG_VERIFY)));
  hb_ਬਫਰ_ਸੈੱਟ_flags (fragments[1], (hb_ਬਫਰ_flags_t (hb_ਬਫਰ_get_flags (fragments[1]) & ~HB_ਬਫਰ_FLAG_VERIFY)));
  hb_unique_ptr_t<hb_ਬਫਰ_t> reconstruction (hb_ਬਫਰ_create_similar (ਬਫਰ));
  hb_ਬਫਰ_ਸੈੱਟ_flags (reconstruction, (hb_ਬਫਰ_flags_t (hb_ਬਫਰ_get_flags (reconstruction) & ~HB_ਬਫਰ_FLAG_VERIFY)));
  hb_segment_properties_t props;
  hb_ਬਫਰ_get_segment_properties (ਬਫਰ, &props);
  hb_ਬਫਰ_ਸੈੱਟ_segment_properties (fragments[0], &props);
  hb_ਬਫਰ_ਸੈੱਟ_segment_properties (fragments[1], &props);
  hb_ਬਫਰ_ਸੈੱਟ_segment_properties (reconstruction, &props);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs;
  hb_glyph_info_t *info = hb_ਬਫਰ_get_glyph_infos (ਬਫਰ, &ਅੰਕ_glyphs);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_chars;
  hb_glyph_info_t *ਪਾਠ = hb_ਬਫਰ_get_glyph_infos (ਪਾਠ_ਬਫਰ, &ਅੰਕ_chars);

  ਤਾਰਕਿਕ forward = HB_DIRECTION_IS_FORWARD (hb_ਬਫਰ_get_direction (ਬਫਰ));

  ਜੇ (!forward)
    hb_ਬਫਰ_ਉਲਟ (ਬਫਰ);

  /*
   * Split text into segments and collect into to fragment streams.
   */
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ fragment_idx = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਾਠ_ਸ਼ੁਰੂ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਾਠ_ਸਿਰਾ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = 1; ਸਿਰਾ < ਅੰਕ_glyphs + 1; ਸਿਰਾ++)
    {
      ਜੇ (ਸਿਰਾ < ਅੰਕ_glyphs &&
	  (info[ਸਿਰਾ].cluster == info[ਸਿਰਾ-1].cluster ||
	   info[ਸਿਰਾ].ਨਕਾਬ & HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT))
	  ਜਾਰੀ;

      /* Accumulate segment corresponding to glyphs start..end. */
      ਜੇ (ਸਿਰਾ == ਅੰਕ_glyphs)
	ਪਾਠ_ਸਿਰਾ = ਅੰਕ_chars;
      ਨਹੀਂਤਾਂ
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ cluster = info[ਸਿਰਾ].cluster;
	ਜਦੋਂ (ਪਾਠ_ਸਿਰਾ < ਅੰਕ_chars && ਪਾਠ[ਪਾਠ_ਸਿਰਾ].cluster < cluster)
	  ਪਾਠ_ਸਿਰਾ++;
      }
      ਪ੍ਰਤਿਜ੍ਞਾ (ਪਾਠ_ਸ਼ੁਰੂ < ਪਾਠ_ਸਿਰਾ);

      ਜੇ (ਝੂਠ)
	ਲਿਖੋ("start %u end %u text start %u end %u\n", ਸ਼ੁਰੂ, ਸਿਰਾ, ਪਾਠ_ਸ਼ੁਰੂ, ਪਾਠ_ਸਿਰਾ);

#ਜੇ 0
      hb_ਬਫਰ_flags_t flags = hb_ਬਫਰ_get_flags (fragment);
      ਜੇ (0 < ਪਾਠ_ਸ਼ੁਰੂ)
	flags = (hb_ਬਫਰ_flags_t) (flags & ~HB_ਬਫਰ_FLAG_BOT);
      ਜੇ (ਪਾਠ_ਸਿਰਾ < ਅੰਕ_chars)
	flags = (hb_ਬਫਰ_flags_t) (flags & ~HB_ਬਫਰ_FLAG_EOT);
      hb_ਬਫਰ_ਸੈੱਟ_flags (fragment, flags);
#ਜੇ_ਅੰਤ

      hb_ਬਫਰ_append (fragments[fragment_idx], ਪਾਠ_ਬਫਰ, ਪਾਠ_ਸ਼ੁਰੂ, ਪਾਠ_ਸਿਰਾ);

      ਸ਼ੁਰੂ = ਸਿਰਾ;
      ਪਾਠ_ਸ਼ੁਰੂ = ਪਾਠ_ਸਿਰਾ;
      fragment_idx = 1 - fragment_idx;
    }
  }

  ਤਾਰਕਿਕ ret = ਸੱਚ;
  /*
   * Shape the two fragment streams.
   */
  ਜੇ (!hb_shape_ਭਰਿਆ (font, fragments[0], features, ਅੰਕ_features, shapers) ||
      !fragments[0]->successful)
    ਵਾਪਸ ret;

  ਜੇ (!hb_shape_ਭਰਿਆ (font, fragments[1], features, ਅੰਕ_features, shapers) ||
      !fragments[1]->successful)
    ਵਾਪਸ ret;

  ਜੇ (!forward)
  {
    hb_ਬਫਰ_ਉਲਟ (fragments[0]);
    hb_ਬਫਰ_ਉਲਟ (fragments[1]);
  }

  /*
   * Reconstruct results.
   */
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ fragment_idx = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ fragment_ਸ਼ੁਰੂ[2] {0, 0};
    ਬਿਨਾਨਿਸ਼ਾਨੀ fragment_ਅੰਕ_glyphs[2];
    hb_glyph_info_t *fragment_info[2];
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 2; i++)
      fragment_info[i] = hb_ਬਫਰ_get_glyph_infos (fragments[i], &fragment_ਅੰਕ_glyphs[i]);
    ਜਦੋਂ (fragment_ਸ਼ੁਰੂ[0] < fragment_ਅੰਕ_glyphs[0] ||
	   fragment_ਸ਼ੁਰੂ[1] < fragment_ਅੰਕ_glyphs[1])
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ fragment_ਸਿਰਾ = fragment_ਸ਼ੁਰੂ[fragment_idx] + 1;
      ਜਦੋਂ (fragment_ਸਿਰਾ < fragment_ਅੰਕ_glyphs[fragment_idx] &&
	     (fragment_info[fragment_idx][fragment_ਸਿਰਾ].cluster == fragment_info[fragment_idx][fragment_ਸਿਰਾ - 1].cluster ||
	      fragment_info[fragment_idx][fragment_ਸਿਰਾ].ਨਕਾਬ & HB_GLYPH_FLAG_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT))
	fragment_ਸਿਰਾ++;

      hb_ਬਫਰ_append (reconstruction, fragments[fragment_idx], fragment_ਸ਼ੁਰੂ[fragment_idx], fragment_ਸਿਰਾ);

      fragment_ਸ਼ੁਰੂ[fragment_idx] = fragment_ਸਿਰਾ;
      fragment_idx = 1 - fragment_idx;
    }
  }

  ਜੇ (!forward)
  {
    hb_ਬਫਰ_ਉਲਟ (ਬਫਰ);
    hb_ਬਫਰ_ਉਲਟ (reconstruction);
  }

  ਜੇ (likely (reconstruction->successful))
  {
    /*
     * Diff results.
     */
    hb_ਬਫਰ_diff_flags_t diff = hb_ਬਫਰ_diff (reconstruction, ਬਫਰ, (hb_codepoint_t) -1, 0);
    ਜੇ (diff & ~HB_ਬਫਰ_DIFF_FLAG_GLYPH_FLAGS_MISMATCH)
    {
      ਬਫਰ_verify_error (ਬਫਰ, font, ਬਫਰ_VERIFY_ERROR "unsafe-to-concat test failed.");
      ret = ਝੂਠ;

      /* Return the reconstructed result instead so it can be inspected. */
      hb_ਬਫਰ_ਸੈੱਟ_ਲੰਮਾਈ (ਬਫਰ, 0);
      hb_ਬਫਰ_append (ਬਫਰ, reconstruction, 0, -1);
    }
  }

  ਵਾਪਸ ret;
}

ਤਾਰਕਿਕ
hb_ਬਫਰ_t::verify (hb_ਬਫਰ_t        *ਪਾਠ_ਬਫਰ,
		     hb_font_t          *font,
		     ਅਚਲ hb_feature_t *features,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features,
		     ਅਚਲ ਅੱਖਰ * ਅਚਲ *shapers)
{
  ਤਾਰਕਿਕ ret = ਸੱਚ;
  ਤਾਰਕਿਕ monotone = ਬਫਰ_verify_monotone (ਇਹ, font);
  ਜੇ (!monotone)
    ret = ਝੂਠ;
  ਜੇ (monotone &&
      !ਬਫਰ_verify_unsafe_ਮੰਜ਼ਿਲ_break (ਇਹ, ਪਾਠ_ਬਫਰ, font, features, ਅੰਕ_features, shapers))
    ret = ਝੂਠ;
  ਜੇ (monotone &&
      (flags & HB_ਬਫਰ_FLAG_PRODUCE_UNSAFE_ਮੰਜ਼ਿਲ_CONCAT) != 0 &&
      !ਬਫਰ_verify_unsafe_ਮੰਜ਼ਿਲ_concat (ਇਹ, ਪਾਠ_ਬਫਰ, font, features, ਅੰਕ_features, shapers))
    ret = ਝੂਠ;
  ਜੇ (!ret)
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_SERIALIZE
    ਬਿਨਾਨਿਸ਼ਾਨੀ len = ਪਾਠ_ਬਫਰ->len;
    hb_ਸਦਿਸ਼_t<ਅੱਖਰ> ਬਾਈਟਾਂ;
    ਜੇ (likely (ਬਾਈਟਾਂ.resize (len * 10 + 16)))
    {
      hb_ਬਫਰ_serialize_unicode (ਪਾਠ_ਬਫਰ,
				   0, len,
				   ਬਾਈਟਾਂ.arrayZ, ਬਾਈਟਾਂ.ਲੰਮਾਈ,
				   &len,
				   HB_ਬਫਰ_SERIALIZE_FORMAT_ਪਾਠ,
				   HB_ਬਫਰ_SERIALIZE_FLAG_NO_CLUSTERS);
      ਬਫਰ_verify_error (ਇਹ, font, ਬਫਰ_VERIFY_ERROR "text was: %s.", ਬਾਈਟਾਂ.arrayZ ? ਬਾਈਟਾਂ.arrayZ : "");
    }
#ਜੇ_ਅੰਤ
  }
  ਵਾਪਸ ret;
}


#ਜੇ_ਅੰਤ
