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

#অন্তর্ভাৱ "hb-aat-layout-common.hh"
#অন্তর্ভাৱ "hb-ot-layout.hh"
#অন্তর্ভাৱ "hb-open-type.hh"
#অন্তর্ভাৱ "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#define HB_AAT_TAG_trak HB_TAG('t','r','a','k')


নামাকাশ AAT {


সংরচনা TrackTableEntry
{
  friend সংরচনা TrackData;

  ভিন্ন get_track_মান () ধ্রুৱ { ঘূৰাই track.to_float (); }

  ভিন্ন interpolate_at (unsigned পূর্ণাঙ্ক idx,
			ভিন্ন ptem,
			ধ্রুৱ শূন্য *base,
			hb_array_t<ধ্রুৱ F16DOT16> size_table) ধ্রুৱ
  {
    ধ্রুৱ FWORD *values = (base+মানসমূহ_Z).arrayZ;

    ভিন্ন s0 = size_table[idx].to_float ();
    ভিন্ন s1 = size_table[idx + 1].to_float ();
    পূর্ণাঙ্ক v0 = values[idx];
    পূর্ণাঙ্ক v1 = values[idx + 1];

    // Deal with font bugs.
    যদি (unlikely (s1 < s0))
    { hb_swap (s0, s1); hb_swap (v0, v1); }
    যদি (unlikely (ptem < s0)) ঘূৰাই v0;
    যদি (unlikely (ptem > s1)) ঘূৰাই v1;
    যদি (unlikely (s0 == s1)) ঘূৰাই (v0 + v1) * 0.5f;

    ভিন্ন t = (ptem - s0) / (s1 - s0);
    ঘূৰাই v0 + t * (v1 - v0);
  }

  ভিন্ন get_মান (ভিন্ন ptem,
		   ধ্রুৱ শূন্য *base,
		   hb_array_t<ধ্রুৱ F16DOT16> size_table) ধ্রুৱ
  {
    ধ্রুৱ FWORD *values = (base+মানসমূহ_Z).arrayZ;

    unsigned পূর্ণাঙ্ক n_sizes = size_table.length;

    /*
     * Choose size.
     */
    যদি (!n_sizes) ঘূৰাই 0.f;
    যদি (n_sizes == 1) ঘূৰাই values[0];

    // At least two entries.

    unsigned i;
    বাবে (i = 0; i < n_sizes; i++)
      যদি (size_table[i].to_float () >= ptem)
	ভঙ্গ;

    // Boundary conditions.
    যদি (i == 0)       ঘূৰাই values[0];
    যদি (i == n_sizes) ঘূৰাই values[n_sizes - 1];

    // Exact match.
    যদি (size_table[i].to_float () == ptem) ঘূৰাই values[i];

    // Interpolate.
    ঘূৰাই interpolate_at (i - 1, ptem, base, size_table);
  }

  সার্ৱজনিক:
  তার্কিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুৱ শূন্য *base,
		 unsigned পূর্ণাঙ্ক n_sizes) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  (মানসমূহ_Z.sanitize (c, base, n_sizes))));
  }

  রক্ষিত:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16To<UnsizedArrayOf<FWORD>>
		মানসমূহ_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা TrackData
{
  ভিন্ন get_tracking (ধ্রুৱ শূন্য *base, ভিন্ন ptem, ভিন্ন track = 0.f) ধ্রুৱ
  {
    unsigned গুনতি = nTracks;
    hb_array_t<ধ্রুৱ F16DOT16> size_table = (base+sizeTable).as_array (nSizes);

    যদি (!গুনতি) ঘূৰাই 0.f;
    যদি (গুনতি == 1) ঘূৰাই trackTable[0].get_মান (ptem, base, size_table);

    // At least two entries.

    unsigned i = 0;
    unsigned j = গুনতি - 1;

    // Find the two entries that track is between.
    যেতিয়ালৈ (i + 1 < গুনতি && trackTable[i + 1].get_track_মান () <= track)
      i++;
    যেতিয়ালৈ (j > 0 && trackTable[j - 1].get_track_মান () >= track)
      j--;

    // Exact match.
    যদি (i == j) ঘূৰাই trackTable[i].get_মান (ptem, base, size_table);

    // Interpolate.

    ভিন্ন t0 = trackTable[i].get_track_মান ();
    ভিন্ন t1 = trackTable[j].get_track_মান ();

    ভিন্ন t = (track - t0) / (t1 - t0);

    ভিন্ন a = trackTable[i].get_মান (ptem, base, size_table);
    ভিন্ন b = trackTable[j].get_মান (ptem, base, size_table);
    ঘূৰাই a + t * (b - a);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  hb_barrier () &&
			  sizeTable.sanitize (c, base, nSizes) &&
			  trackTable.sanitize (c, nTracks, base, nSizes)));
  }

  রক্ষিত:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32To<UnsizedArrayOf<F16DOT16>>
		sizeTable;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  UnsizedArrayOf<TrackTableEntry>
		trackTable;	/* Array[nTracks] of TrackTableEntry records. */

  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (8, trackTable);
};

সংরচনা trak
{
  স্থির constexpr hb_tag_t tableTag = HB_AAT_TAG_trak;

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই version.to_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, ভিন্ন track = 0.f) ধ্রুৱ
  {
    ভিন্ন ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    ঘূৰাই font->em_scalef_x ((এতত্+horizData).get_tracking (এতত্, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, ভিন্ন track = 0.f) ধ্রুৱ
  {
    ভিন্ন ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    ঘূৰাই font->em_scalef_y ((এতত্+vertData).get_tracking (এতত্, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, ভিন্ন track = 0.f) ধ্রুৱ
  {
#ifndef HB_NO_STYLE
    যদি (!font->face->table.STAT->has_data ())
      ঘূৰাই 0;
    ঘূৰাই HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#অন্যথা
    ঘূৰাই 0;
#endif
  }

  তার্কিক apply (hb_aat_apply_context_t *c, ভিন্ন track = 0.f) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);

    ভিন্ন ptem = c->font->ptem;
    যদি (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_SIZE;
    }

    hb_buffer_t *buffer = c->buffer;
    যদি (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    {
      hb_position_t advance_to_add = get_h_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].x_advance = hb_saturate_add (buffer->pos[start].x_advance, advance_to_add);
    }
    অন্যথা
    {
      hb_position_t advance_to_add = get_v_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].y_advance = hb_saturate_add (buffer->pos[start].y_advance, advance_to_add);
    }

    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);

    return_trace (likely (c->check_struct (এতত্) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horizData.sanitize (c, এতত্, এতত্) &&
			  vertData.sanitize (c, এতত্, এতত্)));
  }

  রক্ষিত:
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

  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
