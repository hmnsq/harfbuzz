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

#که_نه_تعریف_شوی HB_AAT_LAYOUT_TRAK_جدول_HH
#تعریف_کړه HB_AAT_LAYOUT_TRAK_جدول_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#تعریف_کړه HB_AAT_نښان_trak HB_نښان('t','r','a','k')


نطاق AAT {


بنية Track_جدول_ثبت
{
  ملګری بنية Track_معلومات;

  عائم get_track_ارزښت () ثابت { أرجع track.منزل_float (); }

  عائم interpolate_at (بینښه صحيح idx,
			عائم ptem,
			ثابت فراغ *بنسټ,
			hb_array_t<ثابت F16DOT16> اندازه_جدول) ثابت
  {
    ثابت FWORD *values = (بنسټ+ارزښتونه_Z).arrayZ;

    عائم s0 = اندازه_جدول[idx].منزل_float ();
    عائم s1 = اندازه_جدول[idx + 1].منزل_float ();
    صحيح v0 = values[idx];
    صحيح v1 = values[idx + 1];

    // Deal with font bugs.
    إذا (unlikely (s1 < s0))
    { hb_بدلون (s0, s1); hb_بدلون (v0, v1); }
    إذا (unlikely (ptem < s0)) أرجع v0;
    إذا (unlikely (ptem > s1)) أرجع v1;
    إذا (unlikely (s0 == s1)) أرجع (v0 + v1) * 0.5f;

    عائم t = (ptem - s0) / (s1 - s0);
    أرجع v0 + t * (v1 - v0);
  }

  عائم get_ارزښت (عائم ptem,
		   ثابت فراغ *بنسټ,
		   hb_array_t<ثابت F16DOT16> اندازه_جدول) ثابت
  {
    ثابت FWORD *values = (بنسټ+ارزښتونه_Z).arrayZ;

    بینښه صحيح n_sizes = اندازه_جدول.اوږدتیا;

    /*
     * Choose size.
     */
    إذا (!n_sizes) أرجع 0.f;
    إذا (n_sizes == 1) أرجع values[0];

    // At least two entries.

    بینښه i;
    لكل (i = 0; i < n_sizes; i++)
      إذا (اندازه_جدول[i].منزل_float () >= ptem)
	اكسر;

    // Boundary conditions.
    إذا (i == 0)       أرجع values[0];
    إذا (i == n_sizes) أرجع values[n_sizes - 1];

    // Exact match.
    إذا (اندازه_جدول[i].منزل_float () == ptem) أرجع values[i];

    // Interpolate.
    أرجع interpolate_at (i - 1, ptem, بنسټ, اندازه_جدول);
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *بنسټ,
		 بینښه صحيح n_sizes) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  (ارزښتونه_Z.sanitize (c, بنسټ, n_sizes))));
  }

  محمي:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16_منزل<Unsized_Array_د<FWORD>>
		ارزښتونه_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Track_معلومات
{
  عائم get_tracking (ثابت فراغ *بنسټ, عائم ptem, عائم track = 0.f) ثابت
  {
    بینښه شمېر = nTracks;
    hb_array_t<ثابت F16DOT16> اندازه_جدول = (بنسټ+اندازه_جدول_2).as_array (nSizes);

    إذا (!شمېر) أرجع 0.f;
    إذا (شمېر == 1) أرجع track_جدول[0].get_ارزښت (ptem, بنسټ, اندازه_جدول);

    // At least two entries.

    بینښه i = 0;
    بینښه j = شمېر - 1;

    // Find the two entries that track is between.
    طالما (i + 1 < شمېر && track_جدول[i + 1].get_track_ارزښت () <= track)
      i++;
    طالما (j > 0 && track_جدول[j - 1].get_track_ارزښت () >= track)
      j--;

    // Exact match.
    إذا (i == j) أرجع track_جدول[i].get_ارزښت (ptem, بنسټ, اندازه_جدول);

    // Interpolate.

    عائم t0 = track_جدول[i].get_track_ارزښت ();
    عائم t1 = track_جدول[j].get_track_ارزښت ();

    عائم t = (track - t0) / (t1 - t0);

    عائم a = track_جدول[i].get_ارزښت (ptem, بنسټ, اندازه_جدول);
    عائم b = track_جدول[j].get_ارزښت (ptem, بنسټ, اندازه_جدول);
    أرجع a + t * (b - a);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  اندازه_جدول_2.sanitize (c, بنسټ, nSizes) &&
			  track_جدول.sanitize (c, nTracks, بنسټ, nSizes)));
  }

  محمي:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32_منزل<Unsized_Array_د<F16DOT16>>
		اندازه_جدول_2;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  Unsized_Array_د<Track_جدول_ثبت>
		track_جدول;	/* Array[nTracks] of TrackTableEntry records. */

  عام:
  DEFINE_اندازه_ARRAY (8, track_جدول);
};

بنية trak
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_AAT_نښان_trak;

  منطقي has_معلومات () ثابت { أرجع نسخه.منزل_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, عائم track = 0.f) ثابت
  {
    عائم ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_اندازه;
    أرجع font->em_scalef_x ((هذا+horiz_معلومات).get_tracking (هذا, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, عائم track = 0.f) ثابت
  {
    عائم ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_اندازه;
    أرجع font->em_scalef_y ((هذا+vert_معلومات).get_tracking (هذا, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, عائم track = 0.f) ثابت
  {
#که_نه_تعریف_شوی HB_NO_STYLE
    إذا (!font->face->جدول.STAT->has_معلومات ())
      أرجع 0;
    أرجع HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#وإلا
    أرجع 0;
#د_که_پای
  }

  منطقي apply (hb_aat_apply_context_t *c, عائم track = 0.f) ثابت
  {
    TRACE_APPLY (هذا);

    عائم ptem = c->font->ptem;
    إذا (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_اندازه;
    }

    hb_بفر_t *بفر = c->بفر;
    إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
    {
      hb_position_t advance_منزل_جمع_کړه = get_h_tracking (c->font, track);
      foreach_grapheme (بفر, پیل, پای_ټکی)
	بفر->pos[پیل].x_advance = hb_saturate_جمع_کړه (بفر->pos[پیل].x_advance, advance_منزل_جمع_کړه);
    }
    وإلا
    {
      hb_position_t advance_منزل_جمع_کړه = get_v_tracking (c->font, track);
      foreach_grapheme (بفر, پیل, پای_ټکی)
	بفر->pos[پیل].y_advance = hb_saturate_جمع_کړه (بفر->pos[پیل].y_advance, advance_منزل_جمع_کړه);
    }

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخه.major == 1 &&
			  horiz_معلومات.sanitize (c, هذا, هذا) &&
			  vert_معلومات.sanitize (c, هذا, هذا)));
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the tracking table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the tracking table (set to 0). */
  Offset16_منزل<Track_معلومات>
		horiz_معلومات;	/* Offset from start of tracking table to TrackData
				 * for horizontal text (or 0 if none). */
  Offset16_منزل<Track_معلومات>
		vert_معلومات;	/* Offset from start of tracking table to TrackData
				 * for vertical text (or 0 if none). */
  HBUINT16	reserved;	/* Reserved. Set to 0. */

  عام:
  DEFINE_اندازه_STATIC (12);
};

} /* namespace AAT */


#د_که_پای /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
