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

#اگر_تعریف_نشده HB_AAT_LAYOUT_TRAK_جدول_HH
#تعریف_کن HB_AAT_LAYOUT_TRAK_جدول_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#تعریف_کن HB_AAT_برچسب_trak HB_برچسب('t','r','a','k')


نطاق AAT {


بنية Track_جدول_مدخل
{
  دوست بنية Track_داده;

  عائم get_track_مقدار () ثابت { أرجع track.مقصد_float (); }

  عائم interpolate_at (بیعلامت صحيح idx,
			عائم ptem,
			ثابت فراغ *پایه,
			hb_array_t<ثابت F16DOT16> اندازه_جدول) ثابت
  {
    ثابت FWORD *values = (پایه+مقادیر_Z).arrayZ;

    عائم s0 = اندازه_جدول[idx].مقصد_float ();
    عائم s1 = اندازه_جدول[idx + 1].مقصد_float ();
    صحيح v0 = values[idx];
    صحيح v1 = values[idx + 1];

    // Deal with font bugs.
    إذا (unlikely (s1 < s0))
    { hb_جابجا (s0, s1); hb_جابجا (v0, v1); }
    إذا (unlikely (ptem < s0)) أرجع v0;
    إذا (unlikely (ptem > s1)) أرجع v1;
    إذا (unlikely (s0 == s1)) أرجع (v0 + v1) * 0.5f;

    عائم t = (ptem - s0) / (s1 - s0);
    أرجع v0 + t * (v1 - v0);
  }

  عائم get_مقدار (عائم ptem,
		   ثابت فراغ *پایه,
		   hb_array_t<ثابت F16DOT16> اندازه_جدول) ثابت
  {
    ثابت FWORD *values = (پایه+مقادیر_Z).arrayZ;

    بیعلامت صحيح n_sizes = اندازه_جدول.درازی;

    /*
     * Choose size.
     */
    إذا (!n_sizes) أرجع 0.f;
    إذا (n_sizes == 1) أرجع values[0];

    // At least two entries.

    بیعلامت i;
    لكل (i = 0; i < n_sizes; i++)
      إذا (اندازه_جدول[i].مقصد_float () >= ptem)
	اكسر;

    // Boundary conditions.
    إذا (i == 0)       أرجع values[0];
    إذا (i == n_sizes) أرجع values[n_sizes - 1];

    // Exact match.
    إذا (اندازه_جدول[i].مقصد_float () == ptem) أرجع values[i];

    // Interpolate.
    أرجع interpolate_at (i - 1, ptem, پایه, اندازه_جدول);
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *پایه,
		 بیعلامت صحيح n_sizes) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  (مقادیر_Z.sanitize (c, پایه, n_sizes))));
  }

  محمي:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16_مقصد<Unsized_Array_از<FWORD>>
		مقادیر_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Track_داده
{
  عائم get_tracking (ثابت فراغ *پایه, عائم ptem, عائم track = 0.f) ثابت
  {
    بیعلامت شمار = nTracks;
    hb_array_t<ثابت F16DOT16> اندازه_جدول = (پایه+اندازه_جدول_2).as_array (nSizes);

    إذا (!شمار) أرجع 0.f;
    إذا (شمار == 1) أرجع track_جدول[0].get_مقدار (ptem, پایه, اندازه_جدول);

    // At least two entries.

    بیعلامت i = 0;
    بیعلامت j = شمار - 1;

    // Find the two entries that track is between.
    طالما (i + 1 < شمار && track_جدول[i + 1].get_track_مقدار () <= track)
      i++;
    طالما (j > 0 && track_جدول[j - 1].get_track_مقدار () >= track)
      j--;

    // Exact match.
    إذا (i == j) أرجع track_جدول[i].get_مقدار (ptem, پایه, اندازه_جدول);

    // Interpolate.

    عائم t0 = track_جدول[i].get_track_مقدار ();
    عائم t1 = track_جدول[j].get_track_مقدار ();

    عائم t = (track - t0) / (t1 - t0);

    عائم a = track_جدول[i].get_مقدار (ptem, پایه, اندازه_جدول);
    عائم b = track_جدول[j].get_مقدار (ptem, پایه, اندازه_جدول);
    أرجع a + t * (b - a);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  اندازه_جدول_2.sanitize (c, پایه, nSizes) &&
			  track_جدول.sanitize (c, nTracks, پایه, nSizes)));
  }

  محمي:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32_مقصد<Unsized_Array_از<F16DOT16>>
		اندازه_جدول_2;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  Unsized_Array_از<Track_جدول_مدخل>
		track_جدول;	/* Array[nTracks] of TrackTableEntry records. */

  عام:
  DEFINE_اندازه_ARRAY (8, track_جدول);
};

بنية trak
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_AAT_برچسب_trak;

  منطقي has_داده () ثابت { أرجع نسخه.مقصد_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, عائم track = 0.f) ثابت
  {
    عائم ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_اندازه;
    أرجع font->em_scalef_x ((هذا+horiz_داده).get_tracking (هذا, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, عائم track = 0.f) ثابت
  {
    عائم ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_اندازه;
    أرجع font->em_scalef_y ((هذا+vert_داده).get_tracking (هذا, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, عائم track = 0.f) ثابت
  {
#اگر_تعریف_نشده HB_NO_STYLE
    إذا (!font->face->جدول.STAT->has_داده ())
      أرجع 0;
    أرجع HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#وإلا
    أرجع 0;
#پایان_اگر
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

    hb_میانگیر_t *میانگیر = c->میانگیر;
    إذا (HB_DIRECTION_IS_HORIZONTAL (میانگیر->props.direction))
    {
      hb_position_t advance_مقصد_جمع_کن = get_h_tracking (c->font, track);
      foreach_grapheme (میانگیر, شروع, انتها)
	میانگیر->pos[شروع].x_advance = hb_saturate_جمع_کن (میانگیر->pos[شروع].x_advance, advance_مقصد_جمع_کن);
    }
    وإلا
    {
      hb_position_t advance_مقصد_جمع_کن = get_v_tracking (c->font, track);
      foreach_grapheme (میانگیر, شروع, انتها)
	میانگیر->pos[شروع].y_advance = hb_saturate_جمع_کن (میانگیر->pos[شروع].y_advance, advance_مقصد_جمع_کن);
    }

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخه.major == 1 &&
			  horiz_داده.sanitize (c, هذا, هذا) &&
			  vert_داده.sanitize (c, هذا, هذا)));
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the tracking table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the tracking table (set to 0). */
  Offset16_مقصد<Track_داده>
		horiz_داده;	/* Offset from start of tracking table to TrackData
				 * for horizontal text (or 0 if none). */
  Offset16_مقصد<Track_داده>
		vert_داده;	/* Offset from start of tracking table to TrackData
				 * for vertical text (or 0 if none). */
  HBUINT16	reserved;	/* Reserved. Set to 0. */

  عام:
  DEFINE_اندازه_STATIC (12);
};

} /* namespace AAT */


#پایان_اگر /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
