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

#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout.hh"
#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#define HB_AAT_TAG_trak HB_TAG('t','r','a','k')


ନାମାକାଶ AAT {


ସଂରଚନା TrackTableEntry
{
  friend ସଂରଚନା TrackData;

  ଦଶମିକ get_track_ମୂଲ୍ୟ () ଅଚଳ { ଫେରନ୍ତୁ track.to_float (); }

  ଦଶମିକ interpolate_at (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx,
			ଦଶମିକ ptem,
			ଅଚଳ ଶୂନ୍ୟ *base,
			hb_array_t<ଅଚଳ F16DOT16> size_table) ଅଚଳ
  {
    ଅଚଳ FWORD *values = (base+ମୂଲ୍ୟଗୁଡ଼ିକ_Z).arrayZ;

    ଦଶମିକ s0 = size_table[idx].to_float ();
    ଦଶମିକ s1 = size_table[idx + 1].to_float ();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା v0 = values[idx];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା v1 = values[idx + 1];

    // Deal with font bugs.
    ଯଦି (unlikely (s1 < s0))
    { hb_swap (s0, s1); hb_swap (v0, v1); }
    ଯଦି (unlikely (ptem < s0)) ଫେରନ୍ତୁ v0;
    ଯଦି (unlikely (ptem > s1)) ଫେରନ୍ତୁ v1;
    ଯଦି (unlikely (s0 == s1)) ଫେରନ୍ତୁ (v0 + v1) * 0.5f;

    ଦଶମିକ t = (ptem - s0) / (s1 - s0);
    ଫେରନ୍ତୁ v0 + t * (v1 - v0);
  }

  ଦଶମିକ get_ମୂଲ୍ୟ (ଦଶମିକ ptem,
		   ଅଚଳ ଶୂନ୍ୟ *base,
		   hb_array_t<ଅଚଳ F16DOT16> size_table) ଅଚଳ
  {
    ଅଚଳ FWORD *values = (base+ମୂଲ୍ୟଗୁଡ଼ିକ_Z).arrayZ;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_sizes = size_table.length;

    /*
     * Choose size.
     */
    ଯଦି (!n_sizes) ଫେରନ୍ତୁ 0.f;
    ଯଦି (n_sizes == 1) ଫେରନ୍ତୁ values[0];

    // At least two entries.

    unsigned i;
    ପାଇଁ (i = 0; i < n_sizes; i++)
      ଯଦି (size_table[i].to_float () >= ptem)
	ଭାଙ୍ଗ;

    // Boundary conditions.
    ଯଦି (i == 0)       ଫେରନ୍ତୁ values[0];
    ଯଦି (i == n_sizes) ଫେରନ୍ତୁ values[n_sizes - 1];

    // Exact match.
    ଯଦି (size_table[i].to_float () == ptem) ଫେରନ୍ତୁ values[i];

    // Interpolate.
    ଫେରନ୍ତୁ interpolate_at (i - 1, ptem, base, size_table);
  }

  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c,
		 ଅଚଳ ଶୂନ୍ୟ *base,
		 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_sizes) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  (ମୂଲ୍ୟଗୁଡ଼ିକ_Z.sanitize (c, base, n_sizes))));
  }

  ସୁରକ୍ଷିତ:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16To<UnsizedArrayOf<FWORD>>
		ମୂଲ୍ୟଗୁଡ଼ିକ_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା TrackData
{
  ଦଶମିକ get_tracking (ଅଚଳ ଶୂନ୍ୟ *base, ଦଶମିକ ptem, ଦଶମିକ track = 0.f) ଅଚଳ
  {
    unsigned ଗଣନା_2 = nTracks;
    hb_array_t<ଅଚଳ F16DOT16> size_table = (base+sizeTable).as_array (nSizes);

    ଯଦି (!ଗଣନା_2) ଫେରନ୍ତୁ 0.f;
    ଯଦି (ଗଣନା_2 == 1) ଫେରନ୍ତୁ trackTable[0].get_ମୂଲ୍ୟ (ptem, base, size_table);

    // At least two entries.

    unsigned i = 0;
    unsigned j = ଗଣନା_2 - 1;

    // Find the two entries that track is between.
    ଯେତେବେଳେ (i + 1 < ଗଣନା_2 && trackTable[i + 1].get_track_ମୂଲ୍ୟ () <= track)
      i++;
    ଯେତେବେଳେ (j > 0 && trackTable[j - 1].get_track_ମୂଲ୍ୟ () >= track)
      j--;

    // Exact match.
    ଯଦି (i == j) ଫେରନ୍ତୁ trackTable[i].get_ମୂଲ୍ୟ (ptem, base, size_table);

    // Interpolate.

    ଦଶମିକ t0 = trackTable[i].get_track_ମୂଲ୍ୟ ();
    ଦଶମିକ t1 = trackTable[j].get_track_ମୂଲ୍ୟ ();

    ଦଶମିକ t = (track - t0) / (t1 - t0);

    ଦଶମିକ a = trackTable[i].get_ମୂଲ୍ୟ (ptem, base, size_table);
    ଦଶମିକ b = trackTable[j].get_ମୂଲ୍ୟ (ptem, base, size_table);
    ଫେରନ୍ତୁ a + t * (b - a);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  sizeTable.sanitize (c, base, nSizes) &&
			  trackTable.sanitize (c, nTracks, base, nSizes)));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32To<UnsizedArrayOf<F16DOT16>>
		sizeTable;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  UnsizedArrayOf<TrackTableEntry>
		trackTable;	/* Array[nTracks] of TrackTableEntry records. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (8, trackTable);
};

ସଂରଚନା trak
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_AAT_TAG_trak;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.to_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, ଦଶମିକ track = 0.f) ଅଚଳ
  {
    ଦଶମିକ ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    ଫେରନ୍ତୁ font->em_scalef_x ((ଏହି+horizData).get_tracking (ଏହି, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, ଦଶମିକ track = 0.f) ଅଚଳ
  {
    ଦଶମିକ ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    ଫେରନ୍ତୁ font->em_scalef_y ((ଏହି+vertData).get_tracking (ଏହି, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, ଦଶମିକ track = 0.f) ଅଚଳ
  {
#ifndef HB_NO_STYLE
    ଯଦି (!font->face->table.STAT->has_data ())
      ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ 0;
#endif
  }

  ତାର୍କିକ apply (hb_aat_apply_context_t *c, ଦଶମିକ track = 0.f) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    ଦଶମିକ ptem = c->font->ptem;
    ଯଦି (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_SIZE;
    }

    hb_buffer_t *buffer = c->buffer;
    ଯଦି (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    {
      hb_position_t advance_to_add = get_h_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].x_advance = hb_saturate_add (buffer->pos[start].x_advance, advance_to_add);
    }
    ଅନ୍ୟଥା
    {
      hb_position_t advance_to_add = get_v_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].y_advance = hb_saturate_add (buffer->pos[start].y_advance, advance_to_add);
    }

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);

    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horizData.sanitize (c, ଏହି, ଏହି) &&
			  vertData.sanitize (c, ଏହି, ଏହି)));
  }

  ସୁରକ୍ଷିତ:
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

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
