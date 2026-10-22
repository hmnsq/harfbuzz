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

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#define HB_AAT_TAG_trak HB_TAG('t','r','a','k')


نطاق AAT {


بنية TrackTableEntry
{
  friend بنية TrackData;

  عائم get_track_قدر () ثابت { أرجع track.to_float (); }

  عائم interpolate_at (unsigned صحيح idx,
			عائم ptem,
			ثابت فراغ *base,
			hb_array_t<ثابت F16DOT16> size_table) ثابت
  {
    ثابت FWORD *values = (base+اقدار_Z).arrayZ;

    عائم s0 = size_table[idx].to_float ();
    عائم s1 = size_table[idx + 1].to_float ();
    صحيح v0 = values[idx];
    صحيح v1 = values[idx + 1];

    // Deal with font bugs.
    إذا (unlikely (s1 < s0))
    { hb_swap (s0, s1); hb_swap (v0, v1); }
    إذا (unlikely (ptem < s0)) أرجع v0;
    إذا (unlikely (ptem > s1)) أرجع v1;
    إذا (unlikely (s0 == s1)) أرجع (v0 + v1) * 0.5f;

    عائم t = (ptem - s0) / (s1 - s0);
    أرجع v0 + t * (v1 - v0);
  }

  عائم get_قدر (عائم ptem,
		   ثابت فراغ *base,
		   hb_array_t<ثابت F16DOT16> size_table) ثابت
  {
    ثابت FWORD *values = (base+اقدار_Z).arrayZ;

    unsigned صحيح n_sizes = size_table.length;

    /*
     * Choose size.
     */
    إذا (!n_sizes) أرجع 0.f;
    إذا (n_sizes == 1) أرجع values[0];

    // At least two entries.

    unsigned i;
    لكل (i = 0; i < n_sizes; i++)
      إذا (size_table[i].to_float () >= ptem)
	اكسر;

    // Boundary conditions.
    إذا (i == 0)       أرجع values[0];
    إذا (i == n_sizes) أرجع values[n_sizes - 1];

    // Exact match.
    إذا (size_table[i].to_float () == ptem) أرجع values[i];

    // Interpolate.
    أرجع interpolate_at (i - 1, ptem, base, size_table);
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *base,
		 unsigned صحيح n_sizes) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  (اقدار_Z.sanitize (c, base, n_sizes))));
  }

  محمي:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16To<UnsizedArrayOf<FWORD>>
		اقدار_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية TrackData
{
  عائم get_tracking (ثابت فراغ *base, عائم ptem, عائم track = 0.f) ثابت
  {
    unsigned گنتی = nTracks;
    hb_array_t<ثابت F16DOT16> size_table = (base+sizeTable).as_array (nSizes);

    إذا (!گنتی) أرجع 0.f;
    إذا (گنتی == 1) أرجع trackTable[0].get_قدر (ptem, base, size_table);

    // At least two entries.

    unsigned i = 0;
    unsigned j = گنتی - 1;

    // Find the two entries that track is between.
    طالما (i + 1 < گنتی && trackTable[i + 1].get_track_قدر () <= track)
      i++;
    طالما (j > 0 && trackTable[j - 1].get_track_قدر () >= track)
      j--;

    // Exact match.
    إذا (i == j) أرجع trackTable[i].get_قدر (ptem, base, size_table);

    // Interpolate.

    عائم t0 = trackTable[i].get_track_قدر ();
    عائم t1 = trackTable[j].get_track_قدر ();

    عائم t = (track - t0) / (t1 - t0);

    عائم a = trackTable[i].get_قدر (ptem, base, size_table);
    عائم b = trackTable[j].get_قدر (ptem, base, size_table);
    أرجع a + t * (b - a);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  sizeTable.sanitize (c, base, nSizes) &&
			  trackTable.sanitize (c, nTracks, base, nSizes)));
  }

  محمي:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32To<UnsizedArrayOf<F16DOT16>>
		sizeTable;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  UnsizedArrayOf<TrackTableEntry>
		trackTable;	/* Array[nTracks] of TrackTableEntry records. */

  عام:
  DEFINE_SIZE_ARRAY (8, trackTable);
};

بنية trak
{
  ساكن constexpr hb_tag_t tableTag = HB_AAT_TAG_trak;

  منطقي has_data () ثابت { أرجع version.to_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, عائم track = 0.f) ثابت
  {
    عائم ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    أرجع font->em_scalef_x ((هذا+horizData).get_tracking (هذا, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, عائم track = 0.f) ثابت
  {
    عائم ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    أرجع font->em_scalef_y ((هذا+vertData).get_tracking (هذا, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, عائم track = 0.f) ثابت
  {
#ifndef HB_NO_STYLE
    إذا (!font->face->table.STAT->has_data ())
      أرجع 0;
    أرجع HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#وإلا
    أرجع 0;
#endif
  }

  منطقي apply (hb_aat_apply_context_t *c, عائم track = 0.f) ثابت
  {
    TRACE_APPLY (هذا);

    عائم ptem = c->font->ptem;
    إذا (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_SIZE;
    }

    hb_buffer_t *buffer = c->buffer;
    إذا (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    {
      hb_position_t advance_to_add = get_h_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].x_advance = hb_saturate_add (buffer->pos[start].x_advance, advance_to_add);
    }
    وإلا
    {
      hb_position_t advance_to_add = get_v_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].y_advance = hb_saturate_add (buffer->pos[start].y_advance, advance_to_add);
    }

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horizData.sanitize (c, هذا, هذا) &&
			  vertData.sanitize (c, هذا, هذا)));
  }

  محمي:
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

  عام:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
