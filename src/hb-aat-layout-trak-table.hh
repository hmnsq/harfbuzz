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

#अगर_अघोषित HB_AAT_LAYOUT_TRAK_TABLE_HH
#घोषणा HB_AAT_LAYOUT_TRAK_TABLE_HH

#समावेश "hb-aat-layout-common.hh"
#समावेश "hb-ot-layout.hh"
#समावेश "hb-open-type.hh"
#समावेश "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#घोषणा HB_AAT_TAG_trak HB_TAG('t','r','a','k')


namespace AAT {


काष्ठा TrackTableEntry
{
  मित्र काष्ठा Track_तथ्य;

  भग्न get_track_मूल्य () स्थिर { वापस track.to_float (); }

  भग्न interpolate_at (अचिन्हित पूर्णांक idx,
			भग्न ptem,
			स्थिर व्योम *base,
			hb_array_t<स्थिर F16DOT16> आकार_table) स्थिर
  {
    स्थिर FWORD *values = (base+मूल्यावली_Z).arrayZ;

    भग्न s0 = आकार_table[idx].to_float ();
    भग्न s1 = आकार_table[idx + 1].to_float ();
    पूर्णांक v0 = values[idx];
    पूर्णांक v1 = values[idx + 1];

    // Deal with font bugs.
    अगर (unlikely (s1 < s0))
    { hb_swap (s0, s1); hb_swap (v0, v1); }
    अगर (unlikely (ptem < s0)) वापस v0;
    अगर (unlikely (ptem > s1)) वापस v1;
    अगर (unlikely (s0 == s1)) वापस (v0 + v1) * 0.5f;

    भग्न t = (ptem - s0) / (s1 - s0);
    वापस v0 + t * (v1 - v0);
  }

  भग्न get_मूल्य (भग्न ptem,
		   स्थिर व्योम *base,
		   hb_array_t<स्थिर F16DOT16> आकार_table) स्थिर
  {
    स्थिर FWORD *values = (base+मूल्यावली_Z).arrayZ;

    अचिन्हित पूर्णांक n_sizes = आकार_table.लंबाई;

    /*
     * Choose size.
     */
    अगर (!n_sizes) वापस 0.f;
    अगर (n_sizes == 1) वापस values[0];

    // At least two entries.

    अचिन्हित i;
    क्रम (i = 0; i < n_sizes; i++)
      अगर (आकार_table[i].to_float () >= ptem)
	अवरोध;

    // Boundary conditions.
    अगर (i == 0)       वापस values[0];
    अगर (i == n_sizes) वापस values[n_sizes - 1];

    // Exact match.
    अगर (आकार_table[i].to_float () == ptem) वापस values[i];

    // Interpolate.
    वापस interpolate_at (i - 1, ptem, base, आकार_table);
  }

  खुला:
  bool sanitize (hb_sanitize_context_t *c,
		 स्थिर व्योम *base,
		 अचिन्हित पूर्णांक n_sizes) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  (मूल्यावली_Z.sanitize (c, base, n_sizes))));
  }

  रक्षित:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16To<UnsizedArrayOf<FWORD>>
		मूल्यावली_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा Track_तथ्य
{
  भग्न get_tracking (स्थिर व्योम *base, भग्न ptem, भग्न track = 0.f) स्थिर
  {
    अचिन्हित गिनती = nTracks;
    hb_array_t<स्थिर F16DOT16> आकार_table = (base+आकार_Table).as_array (nSizes);

    अगर (!गिनती) वापस 0.f;
    अगर (गिनती == 1) वापस trackTable[0].get_मूल्य (ptem, base, आकार_table);

    // At least two entries.

    अचिन्हित i = 0;
    अचिन्हित j = गिनती - 1;

    // Find the two entries that track is between.
    जबतक (i + 1 < गिनती && trackTable[i + 1].get_track_मूल्य () <= track)
      i++;
    जबतक (j > 0 && trackTable[j - 1].get_track_मूल्य () >= track)
      j--;

    // Exact match.
    अगर (i == j) वापस trackTable[i].get_मूल्य (ptem, base, आकार_table);

    // Interpolate.

    भग्न t0 = trackTable[i].get_track_मूल्य ();
    भग्न t1 = trackTable[j].get_track_मूल्य ();

    भग्न t = (track - t0) / (t1 - t0);

    भग्न a = trackTable[i].get_मूल्य (ptem, base, आकार_table);
    भग्न b = trackTable[j].get_मूल्य (ptem, base, आकार_table);
    वापस a + t * (b - a);
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  आकार_Table.sanitize (c, base, nSizes) &&
			  trackTable.sanitize (c, nTracks, base, nSizes)));
  }

  रक्षित:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32To<UnsizedArrayOf<F16DOT16>>
		आकार_Table;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  UnsizedArrayOf<TrackTableEntry>
		trackTable;	/* Array[nTracks] of TrackTableEntry records. */

  खुला:
  DEFINE_आकार_ARRAY (8, trackTable);
};

काष्ठा trak
{
  अटल constexpr hb_tag_t tableTag = HB_AAT_TAG_trak;

  bool has_तथ्य () स्थिर { वापस version.to_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, भग्न track = 0.f) स्थिर
  {
    भग्न ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_आकार;
    वापस font->em_scalef_x ((यह+horiz_तथ्य).get_tracking (यह, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, भग्न track = 0.f) स्थिर
  {
    भग्न ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_आकार;
    वापस font->em_scalef_y ((यह+vert_तथ्य).get_tracking (यह, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, भग्न track = 0.f) स्थिर
  {
#अगर_अघोषित HB_NO_STYLE
    अगर (!font->face->table.STAT->has_तथ्य ())
      वापस 0;
    वापस HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#अन्यथा
    वापस 0;
#पूर्ण_अगर
  }

  bool apply (hb_aat_apply_context_t *c, भग्न track = 0.f) स्थिर
  {
    TRACE_APPLY (यह);

    भग्न ptem = c->font->ptem;
    अगर (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_आकार;
    }

    hb_बफर_t *बफर = c->बफर;
    अगर (HB_DIRECTION_IS_HORIZONTAL (बफर->props.direction))
    {
      hb_position_t advance_to_add = get_h_tracking (c->font, track);
      foreach_grapheme (बफर, start, end)
	बफर->pos[start].x_advance = hb_saturate_add (बफर->pos[start].x_advance, advance_to_add);
    }
    अन्यथा
    {
      hb_position_t advance_to_add = get_v_tracking (c->font, track);
      foreach_grapheme (बफर, start, end)
	बफर->pos[start].y_advance = hb_saturate_add (बफर->pos[start].y_advance, advance_to_add);
    }

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);

    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horiz_तथ्य.sanitize (c, यह, यह) &&
			  vert_तथ्य.sanitize (c, यह, यह)));
  }

  रक्षित:
  FixedVersion<>version;	/* Version of the tracking table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the tracking table (set to 0). */
  Offset16To<Track_तथ्य>
		horiz_तथ्य;	/* Offset from start of tracking table to TrackData
				 * for horizontal text (or 0 if none). */
  Offset16To<Track_तथ्य>
		vert_तथ्य;	/* Offset from start of tracking table to TrackData
				 * for vertical text (or 0 if none). */
  HBUINT16	reserved;	/* Reserved. Set to 0. */

  खुला:
  DEFINE_आकार_STATIC (12);
};

} /* namespace AAT */


#पूर्ण_अगर /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
