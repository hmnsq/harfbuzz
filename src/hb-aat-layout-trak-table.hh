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

#അന്തര്ഭാവ "hb-aat-layout-common.hh"
#അന്തര്ഭാവ "hb-ot-layout.hh"
#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#define HB_AAT_TAG_trak HB_TAG('t','r','a','k')


നാമാകാശ AAT {


ഘടന TrackTableEntry
{
  friend ഘടന TrackData;

  ദശാംശം get_track_മൂല്യം () സ്ഥിരം { തിരികെ track.to_float (); }

  ദശാംശം interpolate_at (unsigned പൂർണ്ണസംഖ്യ idx,
			ദശാംശം ptem,
			സ്ഥിരം ശൂന്യം *base,
			hb_array_t<സ്ഥിരം F16DOT16> size_table) സ്ഥിരം
  {
    സ്ഥിരം FWORD *values = (base+മൂല്യങ്ങൾ_Z).arrayZ;

    ദശാംശം s0 = size_table[idx].to_float ();
    ദശാംശം s1 = size_table[idx + 1].to_float ();
    പൂർണ്ണസംഖ്യ v0 = values[idx];
    പൂർണ്ണസംഖ്യ v1 = values[idx + 1];

    // Deal with font bugs.
    എങ്കിൽ (unlikely (s1 < s0))
    { hb_swap (s0, s1); hb_swap (v0, v1); }
    എങ്കിൽ (unlikely (ptem < s0)) തിരികെ v0;
    എങ്കിൽ (unlikely (ptem > s1)) തിരികെ v1;
    എങ്കിൽ (unlikely (s0 == s1)) തിരികെ (v0 + v1) * 0.5f;

    ദശാംശം t = (ptem - s0) / (s1 - s0);
    തിരികെ v0 + t * (v1 - v0);
  }

  ദശാംശം get_മൂല്യം (ദശാംശം ptem,
		   സ്ഥിരം ശൂന്യം *base,
		   hb_array_t<സ്ഥിരം F16DOT16> size_table) സ്ഥിരം
  {
    സ്ഥിരം FWORD *values = (base+മൂല്യങ്ങൾ_Z).arrayZ;

    unsigned പൂർണ്ണസംഖ്യ n_sizes = size_table.length;

    /*
     * Choose size.
     */
    എങ്കിൽ (!n_sizes) തിരികെ 0.f;
    എങ്കിൽ (n_sizes == 1) തിരികെ values[0];

    // At least two entries.

    unsigned i;
    വേണ്ടി (i = 0; i < n_sizes; i++)
      എങ്കിൽ (size_table[i].to_float () >= ptem)
	നിർത്തുക;

    // Boundary conditions.
    എങ്കിൽ (i == 0)       തിരികെ values[0];
    എങ്കിൽ (i == n_sizes) തിരികെ values[n_sizes - 1];

    // Exact match.
    എങ്കിൽ (size_table[i].to_float () == ptem) തിരികെ values[i];

    // Interpolate.
    തിരികെ interpolate_at (i - 1, ptem, base, size_table);
  }

  പൊതു:
  തർക്കം sanitize (hb_sanitize_context_t *c,
		 സ്ഥിരം ശൂന്യം *base,
		 unsigned പൂർണ്ണസംഖ്യ n_sizes) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  (മൂല്യങ്ങൾ_Z.sanitize (c, base, n_sizes))));
  }

  സംരക്ഷിത:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16To<UnsizedArrayOf<FWORD>>
		മൂല്യങ്ങൾ_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന TrackData
{
  ദശാംശം get_tracking (സ്ഥിരം ശൂന്യം *base, ദശാംശം ptem, ദശാംശം track = 0.f) സ്ഥിരം
  {
    unsigned എണ്ണം = nTracks;
    hb_array_t<സ്ഥിരം F16DOT16> size_table = (base+sizeTable).as_array (nSizes);

    എങ്കിൽ (!എണ്ണം) തിരികെ 0.f;
    എങ്കിൽ (എണ്ണം == 1) തിരികെ trackTable[0].get_മൂല്യം (ptem, base, size_table);

    // At least two entries.

    unsigned i = 0;
    unsigned j = എണ്ണം - 1;

    // Find the two entries that track is between.
    ആകുമ്പോൾ (i + 1 < എണ്ണം && trackTable[i + 1].get_track_മൂല്യം () <= track)
      i++;
    ആകുമ്പോൾ (j > 0 && trackTable[j - 1].get_track_മൂല്യം () >= track)
      j--;

    // Exact match.
    എങ്കിൽ (i == j) തിരികെ trackTable[i].get_മൂല്യം (ptem, base, size_table);

    // Interpolate.

    ദശാംശം t0 = trackTable[i].get_track_മൂല്യം ();
    ദശാംശം t1 = trackTable[j].get_track_മൂല്യം ();

    ദശാംശം t = (track - t0) / (t1 - t0);

    ദശാംശം a = trackTable[i].get_മൂല്യം (ptem, base, size_table);
    ദശാംശം b = trackTable[j].get_മൂല്യം (ptem, base, size_table);
    തിരികെ a + t * (b - a);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  sizeTable.sanitize (c, base, nSizes) &&
			  trackTable.sanitize (c, nTracks, base, nSizes)));
  }

  സംരക്ഷിത:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32To<UnsizedArrayOf<F16DOT16>>
		sizeTable;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  UnsizedArrayOf<TrackTableEntry>
		trackTable;	/* Array[nTracks] of TrackTableEntry records. */

  പൊതു:
  DEFINE_SIZE_ARRAY (8, trackTable);
};

ഘടന trak
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_AAT_TAG_trak;

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.to_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, ദശാംശം track = 0.f) സ്ഥിരം
  {
    ദശാംശം ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    തിരികെ font->em_scalef_x ((ഇത്+horizData).get_tracking (ഇത്, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, ദശാംശം track = 0.f) സ്ഥിരം
  {
    ദശാംശം ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    തിരികെ font->em_scalef_y ((ഇത്+vertData).get_tracking (ഇത്, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, ദശാംശം track = 0.f) സ്ഥിരം
  {
#ifndef HB_NO_STYLE
    എങ്കിൽ (!font->face->table.STAT->has_data ())
      തിരികെ 0;
    തിരികെ HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#അല്ലെങ്കിൽ
    തിരികെ 0;
#endif
  }

  തർക്കം apply (hb_aat_apply_context_t *c, ദശാംശം track = 0.f) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    ദശാംശം ptem = c->font->ptem;
    എങ്കിൽ (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_SIZE;
    }

    hb_buffer_t *buffer = c->buffer;
    എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    {
      hb_position_t advance_to_add = get_h_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].x_advance = hb_saturate_add (buffer->pos[start].x_advance, advance_to_add);
    }
    അല്ലെങ്കിൽ
    {
      hb_position_t advance_to_add = get_v_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].y_advance = hb_saturate_add (buffer->pos[start].y_advance, advance_to_add);
    }

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);

    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horizData.sanitize (c, ഇത്, ഇത്) &&
			  vertData.sanitize (c, ഇത്, ഇത്)));
  }

  സംരക്ഷിത:
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

  പൊതു:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
