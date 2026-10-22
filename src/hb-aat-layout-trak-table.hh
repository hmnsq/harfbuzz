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

#अन्तर्भाव "hb-aat-layout-common.hh"
#अन्तर्भाव "hb-ot-layout.hh"
#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#define HB_AAT_TAG_trak HB_TAG('t','r','a','k')


नामाकाश AAT {


संरचना TrackTableEntry
{
  friend संरचना TrackData;

  भिन्न get_track_मानम् () ध्रुव { पटिनिवत्त track.to_float (); }

  भिन्न interpolate_at (unsigned पूर्णाङ्क idx,
			भिन्न ptem,
			ध्रुव शून्य *base,
			hb_array_t<ध्रुव F16DOT16> size_table) ध्रुव
  {
    ध्रुव FWORD *values = (base+मानानि_Z).arrayZ;

    भिन्न s0 = size_table[idx].to_float ();
    भिन्न s1 = size_table[idx + 1].to_float ();
    पूर्णाङ्क v0 = values[idx];
    पूर्णाङ्क v1 = values[idx + 1];

    // Deal with font bugs.
    सचे (unlikely (s1 < s0))
    { hb_swap (s0, s1); hb_swap (v0, v1); }
    सचे (unlikely (ptem < s0)) पटिनिवत्त v0;
    सचे (unlikely (ptem > s1)) पटिनिवत्त v1;
    सचे (unlikely (s0 == s1)) पटिनिवत्त (v0 + v1) * 0.5f;

    भिन्न t = (ptem - s0) / (s1 - s0);
    पटिनिवत्त v0 + t * (v1 - v0);
  }

  भिन्न get_मानम् (भिन्न ptem,
		   ध्रुव शून्य *base,
		   hb_array_t<ध्रुव F16DOT16> size_table) ध्रुव
  {
    ध्रुव FWORD *values = (base+मानानि_Z).arrayZ;

    unsigned पूर्णाङ्क n_sizes = size_table.length;

    /*
     * Choose size.
     */
    सचे (!n_sizes) पटिनिवत्त 0.f;
    सचे (n_sizes == 1) पटिनिवत्त values[0];

    // At least two entries.

    unsigned i;
    अत्थाय (i = 0; i < n_sizes; i++)
      सचे (size_table[i].to_float () >= ptem)
	भङ्ग;

    // Boundary conditions.
    सचे (i == 0)       पटिनिवत्त values[0];
    सचे (i == n_sizes) पटिनिवत्त values[n_sizes - 1];

    // Exact match.
    सचे (size_table[i].to_float () == ptem) पटिनिवत्त values[i];

    // Interpolate.
    पटिनिवत्त interpolate_at (i - 1, ptem, base, size_table);
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c,
		 ध्रुव शून्य *base,
		 unsigned पूर्णाङ्क n_sizes) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  (मानानि_Z.sanitize (c, base, n_sizes))));
  }

  रक्षित:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16To<UnsizedArrayOf<FWORD>>
		मानानि_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना TrackData
{
  भिन्न get_tracking (ध्रुव शून्य *base, भिन्न ptem, भिन्न track = 0.f) ध्रुव
  {
    unsigned गणनम् = nTracks;
    hb_array_t<ध्रुव F16DOT16> size_table = (base+sizeTable).as_array (nSizes);

    सचे (!गणनम्) पटिनिवत्त 0.f;
    सचे (गणनम् == 1) पटिनिवत्त trackTable[0].get_मानम् (ptem, base, size_table);

    // At least two entries.

    unsigned i = 0;
    unsigned j = गणनम् - 1;

    // Find the two entries that track is between.
    याव (i + 1 < गणनम् && trackTable[i + 1].get_track_मानम् () <= track)
      i++;
    याव (j > 0 && trackTable[j - 1].get_track_मानम् () >= track)
      j--;

    // Exact match.
    सचे (i == j) पटिनिवत्त trackTable[i].get_मानम् (ptem, base, size_table);

    // Interpolate.

    भिन्न t0 = trackTable[i].get_track_मानम् ();
    भिन्न t1 = trackTable[j].get_track_मानम् ();

    भिन्न t = (track - t0) / (t1 - t0);

    भिन्न a = trackTable[i].get_मानम् (ptem, base, size_table);
    भिन्न b = trackTable[j].get_मानम् (ptem, base, size_table);
    पटिनिवत्त a + t * (b - a);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
			  sizeTable.sanitize (c, base, nSizes) &&
			  trackTable.sanitize (c, nTracks, base, nSizes)));
  }

  रक्षित:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32To<UnsizedArrayOf<F16DOT16>>
		sizeTable;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  UnsizedArrayOf<TrackTableEntry>
		trackTable;	/* Array[nTracks] of TrackTableEntry records. */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, trackTable);
};

संरचना trak
{
  स्थिर constexpr hb_tag_t tableTag = HB_AAT_TAG_trak;

  तार्किक has_data () ध्रुव { पटिनिवत्त version.to_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, भिन्न track = 0.f) ध्रुव
  {
    भिन्न ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    पटिनिवत्त font->em_scalef_x ((एतत्+horizData).get_tracking (एतत्, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, भिन्न track = 0.f) ध्रुव
  {
    भिन्न ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    पटिनिवत्त font->em_scalef_y ((एतत्+vertData).get_tracking (एतत्, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, भिन्न track = 0.f) ध्रुव
  {
#ifndef HB_NO_STYLE
    सचे (!font->face->table.STAT->has_data ())
      पटिनिवत्त 0;
    पटिनिवत्त HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#अञ्ञथा
    पटिनिवत्त 0;
#endif
  }

  तार्किक apply (hb_aat_apply_context_t *c, भिन्न track = 0.f) ध्रुव
  {
    TRACE_APPLY (एतत्);

    भिन्न ptem = c->font->ptem;
    सचे (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_SIZE;
    }

    hb_buffer_t *buffer = c->buffer;
    सचे (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    {
      hb_position_t advance_to_add = get_h_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].x_advance = hb_saturate_add (buffer->pos[start].x_advance, advance_to_add);
    }
    अञ्ञथा
    {
      hb_position_t advance_to_add = get_v_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].y_advance = hb_saturate_add (buffer->pos[start].y_advance, advance_to_add);
    }

    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horizData.sanitize (c, एतत्, एतत्) &&
			  vertData.sanitize (c, एतत्, एतत्)));
  }

  रक्षित:
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

  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
