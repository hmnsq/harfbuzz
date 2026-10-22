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

#ಅನ್ತರ್ಭಾವ "hb-aat-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout.hh"
#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#define HB_AAT_TAG_trak HB_TAG('t','r','a','k')


ನಾಮಾಕಾಶ AAT {


ಸಂರಚನಾ TrackTableEntry
{
  friend ಸಂರಚನಾ TrackData;

  ಭಿನ್ನ get_track_ಮೌಲ್ಯ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ track.to_float (); }

  ಭಿನ್ನ interpolate_at (unsigned ಪೂರ್ಣಾಙ್ಕ idx,
			ಭಿನ್ನ ptem,
			ಧ್ರುವ ಶೂನ್ಯ *base,
			hb_array_t<ಧ್ರುವ F16DOT16> size_table) ಧ್ರುವ
  {
    ಧ್ರುವ FWORD *values = (base+ಮೌಲ್ಯಗಳು_Z).arrayZ;

    ಭಿನ್ನ s0 = size_table[idx].to_float ();
    ಭಿನ್ನ s1 = size_table[idx + 1].to_float ();
    ಪೂರ್ಣಾಙ್ಕ v0 = values[idx];
    ಪೂರ್ಣಾಙ್ಕ v1 = values[idx + 1];

    // Deal with font bugs.
    ಯದಿ (unlikely (s1 < s0))
    { hb_swap (s0, s1); hb_swap (v0, v1); }
    ಯದಿ (unlikely (ptem < s0)) ಪ್ರತ್ಯಾಗಮ v0;
    ಯದಿ (unlikely (ptem > s1)) ಪ್ರತ್ಯಾಗಮ v1;
    ಯದಿ (unlikely (s0 == s1)) ಪ್ರತ್ಯಾಗಮ (v0 + v1) * 0.5f;

    ಭಿನ್ನ t = (ptem - s0) / (s1 - s0);
    ಪ್ರತ್ಯಾಗಮ v0 + t * (v1 - v0);
  }

  ಭಿನ್ನ get_ಮೌಲ್ಯ (ಭಿನ್ನ ptem,
		   ಧ್ರುವ ಶೂನ್ಯ *base,
		   hb_array_t<ಧ್ರುವ F16DOT16> size_table) ಧ್ರುವ
  {
    ಧ್ರುವ FWORD *values = (base+ಮೌಲ್ಯಗಳು_Z).arrayZ;

    unsigned ಪೂರ್ಣಾಙ್ಕ n_sizes = size_table.length;

    /*
     * Choose size.
     */
    ಯದಿ (!n_sizes) ಪ್ರತ್ಯಾಗಮ 0.f;
    ಯದಿ (n_sizes == 1) ಪ್ರತ್ಯಾಗಮ values[0];

    // At least two entries.

    unsigned i;
    ಪ್ರತಿ (i = 0; i < n_sizes; i++)
      ಯದಿ (size_table[i].to_float () >= ptem)
	ಭಙ್ಗ;

    // Boundary conditions.
    ಯದಿ (i == 0)       ಪ್ರತ್ಯಾಗಮ values[0];
    ಯದಿ (i == n_sizes) ಪ್ರತ್ಯಾಗಮ values[n_sizes - 1];

    // Exact match.
    ಯದಿ (size_table[i].to_float () == ptem) ಪ್ರತ್ಯಾಗಮ values[i];

    // Interpolate.
    ಪ್ರತ್ಯಾಗಮ interpolate_at (i - 1, ptem, base, size_table);
  }

  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c,
		 ಧ್ರುವ ಶೂನ್ಯ *base,
		 unsigned ಪೂರ್ಣಾಙ್ಕ n_sizes) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  (ಮೌಲ್ಯಗಳು_Z.sanitize (c, base, n_sizes))));
  }

  ರಕ್ಷಿತ:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16To<UnsizedArrayOf<FWORD>>
		ಮೌಲ್ಯಗಳು_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ TrackData
{
  ಭಿನ್ನ get_tracking (ಧ್ರುವ ಶೂನ್ಯ *base, ಭಿನ್ನ ptem, ಭಿನ್ನ track = 0.f) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = nTracks;
    hb_array_t<ಧ್ರುವ F16DOT16> size_table = (base+sizeTable).as_array (nSizes);

    ಯದಿ (!ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ 0.f;
    ಯದಿ (ಎಣಿಕೆ == 1) ಪ್ರತ್ಯಾಗಮ trackTable[0].get_ಮೌಲ್ಯ (ptem, base, size_table);

    // At least two entries.

    unsigned i = 0;
    unsigned j = ಎಣಿಕೆ - 1;

    // Find the two entries that track is between.
    ಯಾವತ್ (i + 1 < ಎಣಿಕೆ && trackTable[i + 1].get_track_ಮೌಲ್ಯ () <= track)
      i++;
    ಯಾವತ್ (j > 0 && trackTable[j - 1].get_track_ಮೌಲ್ಯ () >= track)
      j--;

    // Exact match.
    ಯದಿ (i == j) ಪ್ರತ್ಯಾಗಮ trackTable[i].get_ಮೌಲ್ಯ (ptem, base, size_table);

    // Interpolate.

    ಭಿನ್ನ t0 = trackTable[i].get_track_ಮೌಲ್ಯ ();
    ಭಿನ್ನ t1 = trackTable[j].get_track_ಮೌಲ್ಯ ();

    ಭಿನ್ನ t = (track - t0) / (t1 - t0);

    ಭಿನ್ನ a = trackTable[i].get_ಮೌಲ್ಯ (ptem, base, size_table);
    ಭಿನ್ನ b = trackTable[j].get_ಮೌಲ್ಯ (ptem, base, size_table);
    ಪ್ರತ್ಯಾಗಮ a + t * (b - a);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
			  sizeTable.sanitize (c, base, nSizes) &&
			  trackTable.sanitize (c, nTracks, base, nSizes)));
  }

  ರಕ್ಷಿತ:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32To<UnsizedArrayOf<F16DOT16>>
		sizeTable;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  UnsizedArrayOf<TrackTableEntry>
		trackTable;	/* Array[nTracks] of TrackTableEntry records. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (8, trackTable);
};

ಸಂರಚನಾ trak
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_AAT_TAG_trak;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.to_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, ಭಿನ್ನ track = 0.f) ಧ್ರುವ
  {
    ಭಿನ್ನ ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    ಪ್ರತ್ಯಾಗಮ font->em_scalef_x ((ಏತತ್+horizData).get_tracking (ಏತತ್, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, ಭಿನ್ನ track = 0.f) ಧ್ರುವ
  {
    ಭಿನ್ನ ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    ಪ್ರತ್ಯಾಗಮ font->em_scalef_y ((ಏತತ್+vertData).get_tracking (ಏತತ್, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, ಭಿನ್ನ track = 0.f) ಧ್ರುವ
  {
#ifndef HB_NO_STYLE
    ಯದಿ (!font->face->table.STAT->has_data ())
      ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ 0;
#endif
  }

  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c, ಭಿನ್ನ track = 0.f) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    ಭಿನ್ನ ptem = c->font->ptem;
    ಯದಿ (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_SIZE;
    }

    hb_buffer_t *buffer = c->buffer;
    ಯದಿ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    {
      hb_position_t advance_to_add = get_h_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].x_advance = hb_saturate_add (buffer->pos[start].x_advance, advance_to_add);
    }
    ಅನ್ಯಥಾ
    {
      hb_position_t advance_to_add = get_v_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].y_advance = hb_saturate_add (buffer->pos[start].y_advance, advance_to_add);
    }

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);

    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horizData.sanitize (c, ಏತತ್, ಏತತ್) &&
			  vertData.sanitize (c, ಏತತ್, ಏತತ್)));
  }

  ರಕ್ಷಿತ:
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

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
