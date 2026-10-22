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

#অন্তর্ভাব "hb-aat-layout-common.hh"
#অন্তর্ভাব "hb-ot-layout.hh"
#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#define HB_AAT_TAG_trak HB_TAG('t','r','a','k')


নামাকাশ AAT {


কাঠামো TrackTableEntry
{
  friend কাঠামো Track_তথ্য;

  দশমিক get_track_মান () ধ্রুবক { ফেরত track.to_float (); }

  দশমিক interpolate_at (unsigned পূর্ণসংখ্যা idx,
			দশমিক ptem,
			ধ্রুবক শূন্য *base,
			hb_array_t<ধ্রুবক F16DOT16> আকার_table) ধ্রুবক
  {
    ধ্রুবক FWORD *values = (base+মানসমূহ_Z).arrayZ;

    দশমিক s0 = আকার_table[idx].to_float ();
    দশমিক s1 = আকার_table[idx + 1].to_float ();
    পূর্ণসংখ্যা v0 = values[idx];
    পূর্ণসংখ্যা v1 = values[idx + 1];

    // Deal with font bugs.
    যদি (unlikely (s1 < s0))
    { hb_swap (s0, s1); hb_swap (v0, v1); }
    যদি (unlikely (ptem < s0)) ফেরত v0;
    যদি (unlikely (ptem > s1)) ফেরত v1;
    যদি (unlikely (s0 == s1)) ফেরত (v0 + v1) * 0.5f;

    দশমিক t = (ptem - s0) / (s1 - s0);
    ফেরত v0 + t * (v1 - v0);
  }

  দশমিক get_মান (দশমিক ptem,
		   ধ্রুবক শূন্য *base,
		   hb_array_t<ধ্রুবক F16DOT16> আকার_table) ধ্রুবক
  {
    ধ্রুবক FWORD *values = (base+মানসমূহ_Z).arrayZ;

    unsigned পূর্ণসংখ্যা n_sizes = আকার_table.দৈর্ঘ্য;

    /*
     * Choose size.
     */
    যদি (!n_sizes) ফেরত 0.f;
    যদি (n_sizes == 1) ফেরত values[0];

    // At least two entries.

    unsigned i;
    জন্য (i = 0; i < n_sizes; i++)
      যদি (আকার_table[i].to_float () >= ptem)
	বিরতি;

    // Boundary conditions.
    যদি (i == 0)       ফেরত values[0];
    যদি (i == n_sizes) ফেরত values[n_sizes - 1];

    // Exact match.
    যদি (আকার_table[i].to_float () == ptem) ফেরত values[i];

    // Interpolate.
    ফেরত interpolate_at (i - 1, ptem, base, আকার_table);
  }

  সর্বজনীন:
  যৌক্তিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুবক শূন্য *base,
		 unsigned পূর্ণসংখ্যা n_sizes) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  (মানসমূহ_Z.sanitize (c, base, n_sizes))));
  }

  সুরক্ষিত:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16To<UnsizedArrayOf<FWORD>>
		মানসমূহ_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো Track_তথ্য
{
  দশমিক get_tracking (ধ্রুবক শূন্য *base, দশমিক ptem, দশমিক track = 0.f) ধ্রুবক
  {
    unsigned গুনতি = nTracks;
    hb_array_t<ধ্রুবক F16DOT16> আকার_table = (base+আকার_Table).as_array (nSizes);

    যদি (!গুনতি) ফেরত 0.f;
    যদি (গুনতি == 1) ফেরত trackTable[0].get_মান (ptem, base, আকার_table);

    // At least two entries.

    unsigned i = 0;
    unsigned j = গুনতি - 1;

    // Find the two entries that track is between.
    যখন (i + 1 < গুনতি && trackTable[i + 1].get_track_মান () <= track)
      i++;
    যখন (j > 0 && trackTable[j - 1].get_track_মান () >= track)
      j--;

    // Exact match.
    যদি (i == j) ফেরত trackTable[i].get_মান (ptem, base, আকার_table);

    // Interpolate.

    দশমিক t0 = trackTable[i].get_track_মান ();
    দশমিক t1 = trackTable[j].get_track_মান ();

    দশমিক t = (track - t0) / (t1 - t0);

    দশমিক a = trackTable[i].get_মান (ptem, base, আকার_table);
    দশমিক b = trackTable[j].get_মান (ptem, base, আকার_table);
    ফেরত a + t * (b - a);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  আকার_Table.sanitize (c, base, nSizes) &&
			  trackTable.sanitize (c, nTracks, base, nSizes)));
  }

  সুরক্ষিত:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32To<UnsizedArrayOf<F16DOT16>>
		আকার_Table;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  UnsizedArrayOf<TrackTableEntry>
		trackTable;	/* Array[nTracks] of TrackTableEntry records. */

  সর্বজনীন:
  DEFINE_আকার_ARRAY (8, trackTable);
};

কাঠামো trak
{
  স্থির constexpr hb_tag_t tableTag = HB_AAT_TAG_trak;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.to_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, দশমিক track = 0.f) ধ্রুবক
  {
    দশমিক ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_আকার;
    ফেরত font->em_scalef_x ((এই+horiz_তথ্য).get_tracking (এই, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, দশমিক track = 0.f) ধ্রুবক
  {
    দশমিক ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_আকার;
    ফেরত font->em_scalef_y ((এই+vert_তথ্য).get_tracking (এই, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, দশমিক track = 0.f) ধ্রুবক
  {
#ifndef HB_NO_STYLE
    যদি (!font->face->table.STAT->has_তথ্য ())
      ফেরত 0;
    ফেরত HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#অন্যথা
    ফেরত 0;
#endif
  }

  যৌক্তিক apply (hb_aat_apply_context_t *c, দশমিক track = 0.f) ধ্রুবক
  {
    TRACE_APPLY (এই);

    দশমিক ptem = c->font->ptem;
    যদি (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_আকার;
    }

    hb_বাফার_t *বাফার = c->বাফার;
    যদি (HB_DIRECTION_IS_HORIZONTAL (বাফার->props.direction))
    {
      hb_position_t advance_to_add = get_h_tracking (c->font, track);
      foreach_grapheme (বাফার, start, end)
	বাফার->pos[start].x_advance = hb_saturate_add (বাফার->pos[start].x_advance, advance_to_add);
    }
    অন্যথা
    {
      hb_position_t advance_to_add = get_v_tracking (c->font, track);
      foreach_grapheme (বাফার, start, end)
	বাফার->pos[start].y_advance = hb_saturate_add (বাফার->pos[start].y_advance, advance_to_add);
    }

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);

    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horiz_তথ্য.sanitize (c, এই, এই) &&
			  vert_তথ্য.sanitize (c, এই, এই)));
  }

  সুরক্ষিত:
  FixedVersion<>version;	/* Version of the tracking table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the tracking table (set to 0). */
  Offset16To<Track_তথ্য>
		horiz_তথ্য;	/* Offset from start of tracking table to TrackData
				 * for horizontal text (or 0 if none). */
  Offset16To<Track_তথ্য>
		vert_তথ্য;	/* Offset from start of tracking table to TrackData
				 * for vertical text (or 0 if none). */
  HBUINT16	reserved;	/* Reserved. Set to 0. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
