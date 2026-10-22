/*
 * Copyright © 2018  Ebrahim Byagowi
 * Copyright © 2018  Google, Inc.
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_LAYOUT_TRAK_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_LAYOUT_TRAK_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_ਟੈਗ_trak HB_ਟੈਗ('t','r','a','k')


ਨਾਮਾਕਾਸ਼ AAT {


ਬਣਤਰ Track_ਸਾਰਣੀ_ਇੰਦਰਾਜ
{
  ਮਿੱਤਰ ਬਣਤਰ Track_ਅੰਕੜੇ;

  ਦਸ਼ਮਲਵ get_track_ਮੁੱਲ () ਅਚਲ { ਵਾਪਸ track.ਮੰਜ਼ਿਲ_float (); }

  ਦਸ਼ਮਲਵ interpolate_at (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx,
			ਦਸ਼ਮਲਵ ptem,
			ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
			hb_array_t<ਅਚਲ F16DOT16> ਮਾਪ_ਸਾਰਣੀ) ਅਚਲ
  {
    ਅਚਲ FWORD *values = (ਅਧਾਰ+ਮੁੱਲਾਂ_Z).arrayZ;

    ਦਸ਼ਮਲਵ s0 = ਮਾਪ_ਸਾਰਣੀ[idx].ਮੰਜ਼ਿਲ_float ();
    ਦਸ਼ਮਲਵ s1 = ਮਾਪ_ਸਾਰਣੀ[idx + 1].ਮੰਜ਼ਿਲ_float ();
    ਪੂਰਨ v0 = values[idx];
    ਪੂਰਨ v1 = values[idx + 1];

    // Deal with font bugs.
    ਜੇ (unlikely (s1 < s0))
    { hb_ਅਦਲਾ_ਬਦਲੀ (s0, s1); hb_ਅਦਲਾ_ਬਦਲੀ (v0, v1); }
    ਜੇ (unlikely (ptem < s0)) ਵਾਪਸ v0;
    ਜੇ (unlikely (ptem > s1)) ਵਾਪਸ v1;
    ਜੇ (unlikely (s0 == s1)) ਵਾਪਸ (v0 + v1) * 0.5f;

    ਦਸ਼ਮਲਵ t = (ptem - s0) / (s1 - s0);
    ਵਾਪਸ v0 + t * (v1 - v0);
  }

  ਦਸ਼ਮਲਵ get_ਮੁੱਲ (ਦਸ਼ਮਲਵ ptem,
		   ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
		   hb_array_t<ਅਚਲ F16DOT16> ਮਾਪ_ਸਾਰਣੀ) ਅਚਲ
  {
    ਅਚਲ FWORD *values = (ਅਧਾਰ+ਮੁੱਲਾਂ_Z).arrayZ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n_sizes = ਮਾਪ_ਸਾਰਣੀ.ਲੰਮਾਈ;

    /*
     * Choose size.
     */
    ਜੇ (!n_sizes) ਵਾਪਸ 0.f;
    ਜੇ (n_sizes == 1) ਵਾਪਸ values[0];

    // At least two entries.

    ਬਿਨਾਨਿਸ਼ਾਨੀ i;
    ਲਈ (i = 0; i < n_sizes; i++)
      ਜੇ (ਮਾਪ_ਸਾਰਣੀ[i].ਮੰਜ਼ਿਲ_float () >= ptem)
	ਰੋਕੋ;

    // Boundary conditions.
    ਜੇ (i == 0)       ਵਾਪਸ values[0];
    ਜੇ (i == n_sizes) ਵਾਪਸ values[n_sizes - 1];

    // Exact match.
    ਜੇ (ਮਾਪ_ਸਾਰਣੀ[i].ਮੰਜ਼ਿਲ_float () == ptem) ਵਾਪਸ values[i];

    // Interpolate.
    ਵਾਪਸ interpolate_at (i - 1, ptem, ਅਧਾਰ, ਮਾਪ_ਸਾਰਣੀ);
  }

  ਜਨਤਕ:
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c,
		 ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
		 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n_sizes) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  (ਮੁੱਲਾਂ_Z.sanitize (c, ਅਧਾਰ, n_sizes))));
  }

  ਸੁਰੱਖਿਅਤ:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<FWORD>>
		ਮੁੱਲਾਂ_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ Track_ਅੰਕੜੇ
{
  ਦਸ਼ਮਲਵ get_tracking (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, ਦਸ਼ਮਲਵ ptem, ਦਸ਼ਮਲਵ track = 0.f) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = nTracks;
    hb_array_t<ਅਚਲ F16DOT16> ਮਾਪ_ਸਾਰਣੀ = (ਅਧਾਰ+ਮਾਪ_ਸਾਰਣੀ_2).as_array (nSizes);

    ਜੇ (!ਗਿਣਤੀ_2) ਵਾਪਸ 0.f;
    ਜੇ (ਗਿਣਤੀ_2 == 1) ਵਾਪਸ track_ਸਾਰਣੀ[0].get_ਮੁੱਲ (ptem, ਅਧਾਰ, ਮਾਪ_ਸਾਰਣੀ);

    // At least two entries.

    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ j = ਗਿਣਤੀ_2 - 1;

    // Find the two entries that track is between.
    ਜਦੋਂ (i + 1 < ਗਿਣਤੀ_2 && track_ਸਾਰਣੀ[i + 1].get_track_ਮੁੱਲ () <= track)
      i++;
    ਜਦੋਂ (j > 0 && track_ਸਾਰਣੀ[j - 1].get_track_ਮੁੱਲ () >= track)
      j--;

    // Exact match.
    ਜੇ (i == j) ਵਾਪਸ track_ਸਾਰਣੀ[i].get_ਮੁੱਲ (ptem, ਅਧਾਰ, ਮਾਪ_ਸਾਰਣੀ);

    // Interpolate.

    ਦਸ਼ਮਲਵ t0 = track_ਸਾਰਣੀ[i].get_track_ਮੁੱਲ ();
    ਦਸ਼ਮਲਵ t1 = track_ਸਾਰਣੀ[j].get_track_ਮੁੱਲ ();

    ਦਸ਼ਮਲਵ t = (track - t0) / (t1 - t0);

    ਦਸ਼ਮਲਵ a = track_ਸਾਰਣੀ[i].get_ਮੁੱਲ (ptem, ਅਧਾਰ, ਮਾਪ_ਸਾਰਣੀ);
    ਦਸ਼ਮਲਵ b = track_ਸਾਰਣੀ[j].get_ਮੁੱਲ (ptem, ਅਧਾਰ, ਮਾਪ_ਸਾਰਣੀ);
    ਵਾਪਸ a + t * (b - a);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  ਮਾਪ_ਸਾਰਣੀ_2.sanitize (c, ਅਧਾਰ, nSizes) &&
			  track_ਸਾਰਣੀ.sanitize (c, nTracks, ਅਧਾਰ, nSizes)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<F16DOT16>>
		ਮਾਪ_ਸਾਰਣੀ_2;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  Unsized_Array_ਦਾ<Track_ਸਾਰਣੀ_ਇੰਦਰਾਜ>
		track_ਸਾਰਣੀ;	/* Array[nTracks] of TrackTableEntry records. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (8, track_ਸਾਰਣੀ);
};

ਬਣਤਰ trak
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_AAT_ਟੈਗ_trak;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, ਦਸ਼ਮਲਵ track = 0.f) ਅਚਲ
  {
    ਦਸ਼ਮਲਵ ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_ਮਾਪ;
    ਵਾਪਸ font->em_scalef_x ((ਇਹ+horiz_ਅੰਕੜੇ).get_tracking (ਇਹ, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, ਦਸ਼ਮਲਵ track = 0.f) ਅਚਲ
  {
    ਦਸ਼ਮਲਵ ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_ਮਾਪ;
    ਵਾਪਸ font->em_scalef_y ((ਇਹ+vert_ਅੰਕੜੇ).get_tracking (ਇਹ, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, ਦਸ਼ਮਲਵ track = 0.f) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_STYLE
    ਜੇ (!font->face->ਸਾਰਣੀ.STAT->has_ਅੰਕੜੇ ())
      ਵਾਪਸ 0;
    ਵਾਪਸ HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
  }

  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c, ਦਸ਼ਮਲਵ track = 0.f) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਦਸ਼ਮਲਵ ptem = c->font->ptem;
    ਜੇ (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_ਮਾਪ;
    }

    hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
    ਜੇ (HB_DIRECTION_IS_HORIZONTAL (ਬਫਰ->props.direction))
    {
      hb_position_t advance_ਮੰਜ਼ਿਲ_ਜੋੜੋ = get_h_tracking (c->font, track);
      foreach_grapheme (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
	ਬਫਰ->pos[ਸ਼ੁਰੂ].x_advance = hb_saturate_ਜੋੜੋ (ਬਫਰ->pos[ਸ਼ੁਰੂ].x_advance, advance_ਮੰਜ਼ਿਲ_ਜੋੜੋ);
    }
    ਨਹੀਂਤਾਂ
    {
      hb_position_t advance_ਮੰਜ਼ਿਲ_ਜੋੜੋ = get_v_tracking (c->font, track);
      foreach_grapheme (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
	ਬਫਰ->pos[ਸ਼ੁਰੂ].y_advance = hb_saturate_ਜੋੜੋ (ਬਫਰ->pos[ਸ਼ੁਰੂ].y_advance, advance_ਮੰਜ਼ਿਲ_ਜੋੜੋ);
    }

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);

    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  ਸੰਸਕਰਣ.major == 1 &&
			  horiz_ਅੰਕੜੇ.sanitize (c, ਇਹ, ਇਹ) &&
			  vert_ਅੰਕੜੇ.sanitize (c, ਇਹ, ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the tracking table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the tracking table (set to 0). */
  Offset16_ਮੰਜ਼ਿਲ<Track_ਅੰਕੜੇ>
		horiz_ਅੰਕੜੇ;	/* Offset from start of tracking table to TrackData
				 * for horizontal text (or 0 if none). */
  Offset16_ਮੰਜ਼ਿਲ<Track_ਅੰਕੜੇ>
		vert_ਅੰਕੜੇ;	/* Offset from start of tracking table to TrackData
				 * for vertical text (or 0 if none). */
  HBUINT16	reserved;	/* Reserved. Set to 0. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};

} /* namespace AAT */


#ਜੇ_ਅੰਤ /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
