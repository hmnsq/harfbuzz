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

#ifndef HB_AAT_LAYOUT_TRAK_TABLE_HH
#define HB_AAT_LAYOUT_TRAK_TABLE_HH

#شامل "hb-aat-layout-common.hh"
#شامل "hb-ot-layout.hh"
#شامل "hb-open-type.hh"
#شامل "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#define HB_AAT_TAG_trak HB_TAG('t','r','a','k')


فضاینام AAT {


ساختار TrackTableEntry
{
  friend ساختار TrackData;

  اعشاری get_track_مقدار () ثابت { بازگشت track.to_float (); }

  اعشاری interpolate_at (unsigned صحیح idx,
			اعشاری ptem,
			ثابت تهی *base,
			hb_array_t<ثابت F16DOT16> size_table) ثابت
  {
    ثابت FWORD *values = (base+مقادیر_Z).arrayZ;

    اعشاری s0 = size_table[idx].to_float ();
    اعشاری s1 = size_table[idx + 1].to_float ();
    صحیح v0 = values[idx];
    صحیح v1 = values[idx + 1];

    // Deal with font bugs.
    اگر (unlikely (s1 < s0))
    { hb_swap (s0, s1); hb_swap (v0, v1); }
    اگر (unlikely (ptem < s0)) بازگشت v0;
    اگر (unlikely (ptem > s1)) بازگشت v1;
    اگر (unlikely (s0 == s1)) بازگشت (v0 + v1) * 0.5f;

    اعشاری t = (ptem - s0) / (s1 - s0);
    بازگشت v0 + t * (v1 - v0);
  }

  اعشاری get_مقدار (اعشاری ptem,
		   ثابت تهی *base,
		   hb_array_t<ثابت F16DOT16> size_table) ثابت
  {
    ثابت FWORD *values = (base+مقادیر_Z).arrayZ;

    unsigned صحیح n_sizes = size_table.length;

    /*
     * Choose size.
     */
    اگر (!n_sizes) بازگشت 0.f;
    اگر (n_sizes == 1) بازگشت values[0];

    // At least two entries.

    unsigned i;
    برای (i = 0; i < n_sizes; i++)
      اگر (size_table[i].to_float () >= ptem)
	بشکن;

    // Boundary conditions.
    اگر (i == 0)       بازگشت values[0];
    اگر (i == n_sizes) بازگشت values[n_sizes - 1];

    // Exact match.
    اگر (size_table[i].to_float () == ptem) بازگشت values[i];

    // Interpolate.
    بازگشت interpolate_at (i - 1, ptem, base, size_table);
  }

  عمومی:
  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت تهی *base,
		 unsigned صحیح n_sizes) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  (مقادیر_Z.sanitize (c, base, n_sizes))));
  }

  محافظتشده:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16To<UnsizedArrayOf<FWORD>>
		مقادیر_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار TrackData
{
  اعشاری get_tracking (ثابت تهی *base, اعشاری ptem, اعشاری track = 0.f) ثابت
  {
    unsigned شمار = nTracks;
    hb_array_t<ثابت F16DOT16> size_table = (base+sizeTable).as_array (nSizes);

    اگر (!شمار) بازگشت 0.f;
    اگر (شمار == 1) بازگشت trackTable[0].get_مقدار (ptem, base, size_table);

    // At least two entries.

    unsigned i = 0;
    unsigned j = شمار - 1;

    // Find the two entries that track is between.
    وقتی (i + 1 < شمار && trackTable[i + 1].get_track_مقدار () <= track)
      i++;
    وقتی (j > 0 && trackTable[j - 1].get_track_مقدار () >= track)
      j--;

    // Exact match.
    اگر (i == j) بازگشت trackTable[i].get_مقدار (ptem, base, size_table);

    // Interpolate.

    اعشاری t0 = trackTable[i].get_track_مقدار ();
    اعشاری t1 = trackTable[j].get_track_مقدار ();

    اعشاری t = (track - t0) / (t1 - t0);

    اعشاری a = trackTable[i].get_مقدار (ptem, base, size_table);
    اعشاری b = trackTable[j].get_مقدار (ptem, base, size_table);
    بازگشت a + t * (b - a);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  hb_barrier () &&
			  sizeTable.sanitize (c, base, nSizes) &&
			  trackTable.sanitize (c, nTracks, base, nSizes)));
  }

  محافظتشده:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32To<UnsizedArrayOf<F16DOT16>>
		sizeTable;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  UnsizedArrayOf<TrackTableEntry>
		trackTable;	/* Array[nTracks] of TrackTableEntry records. */

  عمومی:
  DEFINE_SIZE_ARRAY (8, trackTable);
};

ساختار trak
{
  ایستا constexpr hb_tag_t tableTag = HB_AAT_TAG_trak;

  منطقی has_data () ثابت { بازگشت version.to_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, اعشاری track = 0.f) ثابت
  {
    اعشاری ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    بازگشت font->em_scalef_x ((این+horizData).get_tracking (این, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, اعشاری track = 0.f) ثابت
  {
    اعشاری ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    بازگشت font->em_scalef_y ((این+vertData).get_tracking (این, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, اعشاری track = 0.f) ثابت
  {
#ifndef HB_NO_STYLE
    اگر (!font->face->table.STAT->has_data ())
      بازگشت 0;
    بازگشت HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#وگرنه
    بازگشت 0;
#endif
  }

  منطقی apply (hb_aat_apply_context_t *c, اعشاری track = 0.f) ثابت
  {
    TRACE_APPLY (این);

    اعشاری ptem = c->font->ptem;
    اگر (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_SIZE;
    }

    hb_buffer_t *buffer = c->buffer;
    اگر (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    {
      hb_position_t advance_to_add = get_h_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].x_advance = hb_saturate_add (buffer->pos[start].x_advance, advance_to_add);
    }
    وگرنه
    {
      hb_position_t advance_to_add = get_v_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].y_advance = hb_saturate_add (buffer->pos[start].y_advance, advance_to_add);
    }

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);

    return_trace (likely (c->check_struct (این) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horizData.sanitize (c, این, این) &&
			  vertData.sanitize (c, این, این)));
  }

  محافظتشده:
  FixedVersion<>version;	/* Version of the tracking table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the tracking table (set to 0). */
  Offset16To<TrackData>
		horizData;	/* Offset from start of tracking table to TrackData
				 * for horizontal text (or 0 if none). */
  Offset16To<TrackData>
		vertData;	/* Offset from start of tracking table to TrackData
				 * for vertical text (or 0 if none). */
  HBUINT16	reserved;	/* Reserved. Set to 0. */

  عمومی:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
