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

#અન્તર્ભાવ "hb-aat-layout-common.hh"
#અન્તર્ભાવ "hb-ot-layout.hh"
#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#define HB_AAT_TAG_trak HB_TAG('t','r','a','k')


નામાકાશ AAT {


સંરચના TrackTableEntry
{
  friend સંરચના TrackData;

  દશાંશ get_track_કિંમત () અચલ { પાછા track.to_float (); }

  દશાંશ interpolate_at (unsigned પૂર્ણાંક idx,
			દશાંશ ptem,
			અચલ શૂન્ય *base,
			hb_array_t<અચલ F16DOT16> size_table) અચલ
  {
    અચલ FWORD *values = (base+કિંમતો_Z).arrayZ;

    દશાંશ s0 = size_table[idx].to_float ();
    દશાંશ s1 = size_table[idx + 1].to_float ();
    પૂર્ણાંક v0 = values[idx];
    પૂર્ણાંક v1 = values[idx + 1];

    // Deal with font bugs.
    જો (unlikely (s1 < s0))
    { hb_swap (s0, s1); hb_swap (v0, v1); }
    જો (unlikely (ptem < s0)) પાછા v0;
    જો (unlikely (ptem > s1)) પાછા v1;
    જો (unlikely (s0 == s1)) પાછા (v0 + v1) * 0.5f;

    દશાંશ t = (ptem - s0) / (s1 - s0);
    પાછા v0 + t * (v1 - v0);
  }

  દશાંશ get_કિંમત (દશાંશ ptem,
		   અચલ શૂન્ય *base,
		   hb_array_t<અચલ F16DOT16> size_table) અચલ
  {
    અચલ FWORD *values = (base+કિંમતો_Z).arrayZ;

    unsigned પૂર્ણાંક n_sizes = size_table.length;

    /*
     * Choose size.
     */
    જો (!n_sizes) પાછા 0.f;
    જો (n_sizes == 1) પાછા values[0];

    // At least two entries.

    unsigned i;
    માટે (i = 0; i < n_sizes; i++)
      જો (size_table[i].to_float () >= ptem)
	તોડો;

    // Boundary conditions.
    જો (i == 0)       પાછા values[0];
    જો (i == n_sizes) પાછા values[n_sizes - 1];

    // Exact match.
    જો (size_table[i].to_float () == ptem) પાછા values[i];

    // Interpolate.
    પાછા interpolate_at (i - 1, ptem, base, size_table);
  }

  જાહેર:
  તાર્કિક sanitize (hb_sanitize_context_t *c,
		 અચલ શૂન્ય *base,
		 unsigned પૂર્ણાંક n_sizes) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  (કિંમતો_Z.sanitize (c, base, n_sizes))));
  }

  સુરક્ષિત:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16To<UnsizedArrayOf<FWORD>>
		કિંમતો_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના TrackData
{
  દશાંશ get_tracking (અચલ શૂન્ય *base, દશાંશ ptem, દશાંશ track = 0.f) અચલ
  {
    unsigned ગણતરી = nTracks;
    hb_array_t<અચલ F16DOT16> size_table = (base+sizeTable).as_array (nSizes);

    જો (!ગણતરી) પાછા 0.f;
    જો (ગણતરી == 1) પાછા trackTable[0].get_કિંમત (ptem, base, size_table);

    // At least two entries.

    unsigned i = 0;
    unsigned j = ગણતરી - 1;

    // Find the two entries that track is between.
    જ્યારે (i + 1 < ગણતરી && trackTable[i + 1].get_track_કિંમત () <= track)
      i++;
    જ્યારે (j > 0 && trackTable[j - 1].get_track_કિંમત () >= track)
      j--;

    // Exact match.
    જો (i == j) પાછા trackTable[i].get_કિંમત (ptem, base, size_table);

    // Interpolate.

    દશાંશ t0 = trackTable[i].get_track_કિંમત ();
    દશાંશ t1 = trackTable[j].get_track_કિંમત ();

    દશાંશ t = (track - t0) / (t1 - t0);

    દશાંશ a = trackTable[i].get_કિંમત (ptem, base, size_table);
    દશાંશ b = trackTable[j].get_કિંમત (ptem, base, size_table);
    પાછા a + t * (b - a);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  hb_barrier () &&
			  sizeTable.sanitize (c, base, nSizes) &&
			  trackTable.sanitize (c, nTracks, base, nSizes)));
  }

  સુરક્ષિત:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32To<UnsizedArrayOf<F16DOT16>>
		sizeTable;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  UnsizedArrayOf<TrackTableEntry>
		trackTable;	/* Array[nTracks] of TrackTableEntry records. */

  જાહેર:
  DEFINE_SIZE_ARRAY (8, trackTable);
};

સંરચના trak
{
  સ્થિર constexpr hb_tag_t tableTag = HB_AAT_TAG_trak;

  તાર્કિક has_data () અચલ { પાછા version.to_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, દશાંશ track = 0.f) અચલ
  {
    દશાંશ ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    પાછા font->em_scalef_x ((આ+horizData).get_tracking (આ, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, દશાંશ track = 0.f) અચલ
  {
    દશાંશ ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    પાછા font->em_scalef_y ((આ+vertData).get_tracking (આ, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, દશાંશ track = 0.f) અચલ
  {
#ifndef HB_NO_STYLE
    જો (!font->face->table.STAT->has_data ())
      પાછા 0;
    પાછા HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#નહીંતર
    પાછા 0;
#endif
  }

  તાર્કિક apply (hb_aat_apply_context_t *c, દશાંશ track = 0.f) અચલ
  {
    TRACE_APPLY (આ);

    દશાંશ ptem = c->font->ptem;
    જો (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_SIZE;
    }

    hb_buffer_t *buffer = c->buffer;
    જો (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    {
      hb_position_t advance_to_add = get_h_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].x_advance = hb_saturate_add (buffer->pos[start].x_advance, advance_to_add);
    }
    નહીંતર
    {
      hb_position_t advance_to_add = get_v_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].y_advance = hb_saturate_add (buffer->pos[start].y_advance, advance_to_add);
    }

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);

    return_trace (likely (c->check_struct (આ) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horizData.sanitize (c, આ, આ) &&
			  vertData.sanitize (c, આ, આ)));
  }

  સુરક્ષિત:
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

  જાહેર:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
