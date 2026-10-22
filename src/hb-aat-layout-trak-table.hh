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
  मित्र संरचना TrackData;

  दशांश get_track_मूल्य () स्थिरांक { परत track.to_float (); }

  दशांश interpolate_at (अचिन्हित पूर्णांक idx,
			दशांश ptem,
			स्थिरांक शून्य *base,
			hb_array_t<स्थिरांक F16DOT16> size_table) स्थिरांक
  {
    स्थिरांक FWORD *values = (base+मूल्ये_Z).arrayZ;

    दशांश s0 = size_table[idx].to_float ();
    दशांश s1 = size_table[idx + 1].to_float ();
    पूर्णांक v0 = values[idx];
    पूर्णांक v1 = values[idx + 1];

    // Deal with font bugs.
    जर (unlikely (s1 < s0))
    { hb_swap (s0, s1); hb_swap (v0, v1); }
    जर (unlikely (ptem < s0)) परत v0;
    जर (unlikely (ptem > s1)) परत v1;
    जर (unlikely (s0 == s1)) परत (v0 + v1) * 0.5f;

    दशांश t = (ptem - s0) / (s1 - s0);
    परत v0 + t * (v1 - v0);
  }

  दशांश get_मूल्य (दशांश ptem,
		   स्थिरांक शून्य *base,
		   hb_array_t<स्थिरांक F16DOT16> size_table) स्थिरांक
  {
    स्थिरांक FWORD *values = (base+मूल्ये_Z).arrayZ;

    अचिन्हित पूर्णांक n_sizes = size_table.length;

    /*
     * Choose size.
     */
    जर (!n_sizes) परत 0.f;
    जर (n_sizes == 1) परत values[0];

    // At least two entries.

    अचिन्हित i;
    साठी (i = 0; i < n_sizes; i++)
      जर (size_table[i].to_float () >= ptem)
	थांबा;

    // Boundary conditions.
    जर (i == 0)       परत values[0];
    जर (i == n_sizes) परत values[n_sizes - 1];

    // Exact match.
    जर (size_table[i].to_float () == ptem) परत values[i];

    // Interpolate.
    परत interpolate_at (i - 1, ptem, base, size_table);
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c,
		 स्थिरांक शून्य *base,
		 अचिन्हित पूर्णांक n_sizes) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  (मूल्ये_Z.sanitize (c, base, n_sizes))));
  }

  संरक्षित:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16To<UnsizedArrayOf<FWORD>>
		मूल्ये_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना TrackData
{
  दशांश get_tracking (स्थिरांक शून्य *base, दशांश ptem, दशांश track = 0.f) स्थिरांक
  {
    अचिन्हित मोजणी = nTracks;
    hb_array_t<स्थिरांक F16DOT16> size_table = (base+sizeTable).as_array (nSizes);

    जर (!मोजणी) परत 0.f;
    जर (मोजणी == 1) परत trackTable[0].get_मूल्य (ptem, base, size_table);

    // At least two entries.

    अचिन्हित i = 0;
    अचिन्हित j = मोजणी - 1;

    // Find the two entries that track is between.
    पर्यंत (i + 1 < मोजणी && trackTable[i + 1].get_track_मूल्य () <= track)
      i++;
    पर्यंत (j > 0 && trackTable[j - 1].get_track_मूल्य () >= track)
      j--;

    // Exact match.
    जर (i == j) परत trackTable[i].get_मूल्य (ptem, base, size_table);

    // Interpolate.

    दशांश t0 = trackTable[i].get_track_मूल्य ();
    दशांश t1 = trackTable[j].get_track_मूल्य ();

    दशांश t = (track - t0) / (t1 - t0);

    दशांश a = trackTable[i].get_मूल्य (ptem, base, size_table);
    दशांश b = trackTable[j].get_मूल्य (ptem, base, size_table);
    परत a + t * (b - a);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  hb_barrier () &&
			  sizeTable.sanitize (c, base, nSizes) &&
			  trackTable.sanitize (c, nTracks, base, nSizes)));
  }

  संरक्षित:
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

  तार्किक has_data () स्थिरांक { परत version.to_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, दशांश track = 0.f) स्थिरांक
  {
    दशांश ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    परत font->em_scalef_x ((हे+horizData).get_tracking (हे, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, दशांश track = 0.f) स्थिरांक
  {
    दशांश ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE;
    परत font->em_scalef_y ((हे+vertData).get_tracking (हे, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, दशांश track = 0.f) स्थिरांक
  {
#ifndef HB_NO_STYLE
    जर (!font->face->table.STAT->has_data ())
      परत 0;
    परत HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#अन्यथा
    परत 0;
#endif
  }

  तार्किक apply (hb_aat_apply_context_t *c, दशांश track = 0.f) स्थिरांक
  {
    TRACE_APPLY (हे);

    दशांश ptem = c->font->ptem;
    जर (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_SIZE;
    }

    hb_buffer_t *buffer = c->buffer;
    जर (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    {
      hb_position_t advance_to_add = get_h_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].x_advance = hb_saturate_add (buffer->pos[start].x_advance, advance_to_add);
    }
    अन्यथा
    {
      hb_position_t advance_to_add = get_v_tracking (c->font, track);
      foreach_grapheme (buffer, start, end)
	buffer->pos[start].y_advance = hb_saturate_add (buffer->pos[start].y_advance, advance_to_add);
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);

    return_trace (likely (c->check_struct (हे) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horizData.sanitize (c, हे, हे) &&
			  vertData.sanitize (c, हे, हे)));
  }

  संरक्षित:
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
