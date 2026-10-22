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

#اگر_غیر_تعریف_شدہ HB_AAT_LAYOUT_TRAK_جدول_HH
#تعریف_کرو HB_AAT_LAYOUT_TRAK_جدول_HH

#شامل "hb-aat-layout-common.hh"
#شامل "hb-ot-layout.hh"
#شامل "hb-open-type.hh"
#شامل "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#تعریف_کرو HB_AAT_لیبل_trak HB_لیبل('t','r','a','k')


نامخلا AAT {


ساخت Track_جدول_اندراج
{
  دوست ساخت Track_ڈیٹا;

  اعشاری get_track_قدر () ثابت { واپس track.مقصد_float (); }

  اعشاری interpolate_at (غیرعلامتی صحیح idx,
			اعشاری ptem,
			ثابت خالی *بنیاد,
			hb_array_t<ثابت F16DOT16> سائز_جدول) ثابت
  {
    ثابت FWORD *values = (بنیاد+اقدار_Z).arrayZ;

    اعشاری s0 = سائز_جدول[idx].مقصد_float ();
    اعشاری s1 = سائز_جدول[idx + 1].مقصد_float ();
    صحیح v0 = values[idx];
    صحیح v1 = values[idx + 1];

    // Deal with font bugs.
    اگر (unlikely (s1 < s0))
    { hb_تبادلہ (s0, s1); hb_تبادلہ (v0, v1); }
    اگر (unlikely (ptem < s0)) واپس v0;
    اگر (unlikely (ptem > s1)) واپس v1;
    اگر (unlikely (s0 == s1)) واپس (v0 + v1) * 0.5f;

    اعشاری t = (ptem - s0) / (s1 - s0);
    واپس v0 + t * (v1 - v0);
  }

  اعشاری get_قدر (اعشاری ptem,
		   ثابت خالی *بنیاد,
		   hb_array_t<ثابت F16DOT16> سائز_جدول) ثابت
  {
    ثابت FWORD *values = (بنیاد+اقدار_Z).arrayZ;

    غیرعلامتی صحیح n_sizes = سائز_جدول.لمبائی;

    /*
     * Choose size.
     */
    اگر (!n_sizes) واپس 0.f;
    اگر (n_sizes == 1) واپس values[0];

    // At least two entries.

    غیرعلامتی i;
    برائے (i = 0; i < n_sizes; i++)
      اگر (سائز_جدول[i].مقصد_float () >= ptem)
	توڑو;

    // Boundary conditions.
    اگر (i == 0)       واپس values[0];
    اگر (i == n_sizes) واپس values[n_sizes - 1];

    // Exact match.
    اگر (سائز_جدول[i].مقصد_float () == ptem) واپس values[i];

    // Interpolate.
    واپس interpolate_at (i - 1, ptem, بنیاد, سائز_جدول);
  }

  عوامی:
  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت خالی *بنیاد,
		 غیرعلامتی صحیح n_sizes) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  (اقدار_Z.sanitize (c, بنیاد, n_sizes))));
  }

  محفوظ:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16_مقصد<Unsized_Array_کا<FWORD>>
		اقدار_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت Track_ڈیٹا
{
  اعشاری get_tracking (ثابت خالی *بنیاد, اعشاری ptem, اعشاری track = 0.f) ثابت
  {
    غیرعلامتی گنتی = nTracks;
    hb_array_t<ثابت F16DOT16> سائز_جدول = (بنیاد+سائز_جدول_2).as_array (nSizes);

    اگر (!گنتی) واپس 0.f;
    اگر (گنتی == 1) واپس track_جدول[0].get_قدر (ptem, بنیاد, سائز_جدول);

    // At least two entries.

    غیرعلامتی i = 0;
    غیرعلامتی j = گنتی - 1;

    // Find the two entries that track is between.
    جب (i + 1 < گنتی && track_جدول[i + 1].get_track_قدر () <= track)
      i++;
    جب (j > 0 && track_جدول[j - 1].get_track_قدر () >= track)
      j--;

    // Exact match.
    اگر (i == j) واپس track_جدول[i].get_قدر (ptem, بنیاد, سائز_جدول);

    // Interpolate.

    اعشاری t0 = track_جدول[i].get_track_قدر ();
    اعشاری t1 = track_جدول[j].get_track_قدر ();

    اعشاری t = (track - t0) / (t1 - t0);

    اعشاری a = track_جدول[i].get_قدر (ptem, بنیاد, سائز_جدول);
    اعشاری b = track_جدول[j].get_قدر (ptem, بنیاد, سائز_جدول);
    واپس a + t * (b - a);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  سائز_جدول_2.sanitize (c, بنیاد, nSizes) &&
			  track_جدول.sanitize (c, nTracks, بنیاد, nSizes)));
  }

  محفوظ:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32_مقصد<Unsized_Array_کا<F16DOT16>>
		سائز_جدول_2;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  Unsized_Array_کا<Track_جدول_اندراج>
		track_جدول;	/* Array[nTracks] of TrackTableEntry records. */

  عوامی:
  DEFINE_سائز_ARRAY (8, track_جدول);
};

ساخت trak
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_AAT_لیبل_trak;

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.مقصد_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, اعشاری track = 0.f) ثابت
  {
    اعشاری ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_سائز;
    واپس font->em_scalef_x ((یہ+horiz_ڈیٹا).get_tracking (یہ, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, اعشاری track = 0.f) ثابت
  {
    اعشاری ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_سائز;
    واپس font->em_scalef_y ((یہ+vert_ڈیٹا).get_tracking (یہ, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, اعشاری track = 0.f) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_STYLE
    اگر (!font->face->جدول.STAT->has_ڈیٹا ())
      واپس 0;
    واپس HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#ورنہ
    واپس 0;
#اگر_ختم
  }

  منطقی apply (hb_aat_apply_context_t *c, اعشاری track = 0.f) ثابت
  {
    TRACE_APPLY (یہ);

    اعشاری ptem = c->font->ptem;
    اگر (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_سائز;
    }

    hb_بفر_t *بفر = c->بفر;
    اگر (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
    {
      hb_position_t advance_مقصد_جوڑو = get_h_tracking (c->font, track);
      foreach_grapheme (بفر, آغاز, انتہا)
	بفر->pos[آغاز].x_advance = hb_saturate_جوڑو (بفر->pos[آغاز].x_advance, advance_مقصد_جوڑو);
    }
    ورنہ
    {
      hb_position_t advance_مقصد_جوڑو = get_v_tracking (c->font, track);
      foreach_grapheme (بفر, آغاز, انتہا)
	بفر->pos[آغاز].y_advance = hb_saturate_جوڑو (بفر->pos[آغاز].y_advance, advance_مقصد_جوڑو);
    }

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);

    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  نسخہ.major == 1 &&
			  horiz_ڈیٹا.sanitize (c, یہ, یہ) &&
			  vert_ڈیٹا.sanitize (c, یہ, یہ)));
  }

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version of the tracking table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the tracking table (set to 0). */
  Offset16_مقصد<Track_ڈیٹا>
		horiz_ڈیٹا;	/* Offset from start of tracking table to TrackData
				 * for horizontal text (or 0 if none). */
  Offset16_مقصد<Track_ڈیٹا>
		vert_ڈیٹا;	/* Offset from start of tracking table to TrackData
				 * for vertical text (or 0 if none). */
  HBUINT16	reserved;	/* Reserved. Set to 0. */

  عوامی:
  DEFINE_سائز_STATIC (12);
};

} /* namespace AAT */


#اگر_ختم /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
