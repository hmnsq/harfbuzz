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

#جيڪڏهن_وصف_نه_ٿيل HB_AAT_LAYOUT_TRAK_جدول_HH
#وصف_ڪريو HB_AAT_LAYOUT_TRAK_جدول_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#وصف_ڪريو HB_AAT_ٽيگ_trak HB_ٽيگ('t','r','a','k')


نطاق AAT {


بنية Track_جدول_داخلا
{
  دوست بنية Track_معلومات;

  عائم get_track_قدر () ثابت { أرجع track.منزل_float (); }

  عائم interpolate_at (بنانشان صحيح idx,
			عائم ptem,
			ثابت فراغ *بنياد,
			hb_array_t<ثابت F16DOT16> سائيز_جدول) ثابت
  {
    ثابت FWORD *values = (بنياد+قدرون_Z).arrayZ;

    عائم s0 = سائيز_جدول[idx].منزل_float ();
    عائم s1 = سائيز_جدول[idx + 1].منزل_float ();
    صحيح v0 = values[idx];
    صحيح v1 = values[idx + 1];

    // Deal with font bugs.
    إذا (unlikely (s1 < s0))
    { hb_مٽا_سٽا (s0, s1); hb_مٽا_سٽا (v0, v1); }
    إذا (unlikely (ptem < s0)) أرجع v0;
    إذا (unlikely (ptem > s1)) أرجع v1;
    إذا (unlikely (s0 == s1)) أرجع (v0 + v1) * 0.5f;

    عائم t = (ptem - s0) / (s1 - s0);
    أرجع v0 + t * (v1 - v0);
  }

  عائم get_قدر (عائم ptem,
		   ثابت فراغ *بنياد,
		   hb_array_t<ثابت F16DOT16> سائيز_جدول) ثابت
  {
    ثابت FWORD *values = (بنياد+قدرون_Z).arrayZ;

    بنانشان صحيح n_sizes = سائيز_جدول.ڊيگهائي;

    /*
     * Choose size.
     */
    إذا (!n_sizes) أرجع 0.f;
    إذا (n_sizes == 1) أرجع values[0];

    // At least two entries.

    بنانشان i;
    لكل (i = 0; i < n_sizes; i++)
      إذا (سائيز_جدول[i].منزل_float () >= ptem)
	اكسر;

    // Boundary conditions.
    إذا (i == 0)       أرجع values[0];
    إذا (i == n_sizes) أرجع values[n_sizes - 1];

    // Exact match.
    إذا (سائيز_جدول[i].منزل_float () == ptem) أرجع values[i];

    // Interpolate.
    أرجع interpolate_at (i - 1, ptem, بنياد, سائيز_جدول);
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *بنياد,
		 بنانشان صحيح n_sizes) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  (قدرون_Z.sanitize (c, بنياد, n_sizes))));
  }

  محمي:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16_منزل<Unsized_Array_جو<FWORD>>
		قدرون_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية Track_معلومات
{
  عائم get_tracking (ثابت فراغ *بنياد, عائم ptem, عائم track = 0.f) ثابت
  {
    بنانشان ڳڻپ = nTracks;
    hb_array_t<ثابت F16DOT16> سائيز_جدول = (بنياد+سائيز_جدول_2).as_array (nSizes);

    إذا (!ڳڻپ) أرجع 0.f;
    إذا (ڳڻپ == 1) أرجع track_جدول[0].get_قدر (ptem, بنياد, سائيز_جدول);

    // At least two entries.

    بنانشان i = 0;
    بنانشان j = ڳڻپ - 1;

    // Find the two entries that track is between.
    طالما (i + 1 < ڳڻپ && track_جدول[i + 1].get_track_قدر () <= track)
      i++;
    طالما (j > 0 && track_جدول[j - 1].get_track_قدر () >= track)
      j--;

    // Exact match.
    إذا (i == j) أرجع track_جدول[i].get_قدر (ptem, بنياد, سائيز_جدول);

    // Interpolate.

    عائم t0 = track_جدول[i].get_track_قدر ();
    عائم t1 = track_جدول[j].get_track_قدر ();

    عائم t = (track - t0) / (t1 - t0);

    عائم a = track_جدول[i].get_قدر (ptem, بنياد, سائيز_جدول);
    عائم b = track_جدول[j].get_قدر (ptem, بنياد, سائيز_جدول);
    أرجع a + t * (b - a);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  سائيز_جدول_2.sanitize (c, بنياد, nSizes) &&
			  track_جدول.sanitize (c, nTracks, بنياد, nSizes)));
  }

  محمي:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32_منزل<Unsized_Array_جو<F16DOT16>>
		سائيز_جدول_2;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  Unsized_Array_جو<Track_جدول_داخلا>
		track_جدول;	/* Array[nTracks] of TrackTableEntry records. */

  عام:
  DEFINE_سائيز_ARRAY (8, track_جدول);
};

بنية trak
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_AAT_ٽيگ_trak;

  منطقي has_معلومات () ثابت { أرجع نسخو.منزل_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, عائم track = 0.f) ثابت
  {
    عائم ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_سائيز;
    أرجع font->em_scalef_x ((هذا+horiz_معلومات).get_tracking (هذا, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, عائم track = 0.f) ثابت
  {
    عائم ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_سائيز;
    أرجع font->em_scalef_y ((هذا+vert_معلومات).get_tracking (هذا, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, عائم track = 0.f) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_STYLE
    إذا (!font->face->جدول.STAT->has_معلومات ())
      أرجع 0;
    أرجع HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#وإلا
    أرجع 0;
#جيڪڏهن_پورو
  }

  منطقي apply (hb_aat_apply_context_t *c, عائم track = 0.f) ثابت
  {
    TRACE_APPLY (هذا);

    عائم ptem = c->font->ptem;
    إذا (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_سائيز;
    }

    hb_بفر_t *بفر = c->بفر;
    إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
    {
      hb_position_t advance_منزل_جوڙيو = get_h_tracking (c->font, track);
      foreach_grapheme (بفر, شروعات, پڇاڙي)
	بفر->pos[شروعات].x_advance = hb_saturate_جوڙيو (بفر->pos[شروعات].x_advance, advance_منزل_جوڙيو);
    }
    وإلا
    {
      hb_position_t advance_منزل_جوڙيو = get_v_tracking (c->font, track);
      foreach_grapheme (بفر, شروعات, پڇاڙي)
	بفر->pos[شروعات].y_advance = hb_saturate_جوڙيو (بفر->pos[شروعات].y_advance, advance_منزل_جوڙيو);
    }

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخو.major == 1 &&
			  horiz_معلومات.sanitize (c, هذا, هذا) &&
			  vert_معلومات.sanitize (c, هذا, هذا)));
  }

  محمي:
  Fixed_نسخو<>نسخو;	/* Version of the tracking table
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
  DEFINE_سائيز_STATIC (12);
};

} /* namespace AAT */


#جيڪڏهن_پورو /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
