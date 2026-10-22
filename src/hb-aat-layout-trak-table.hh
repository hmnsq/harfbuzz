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

#నిర్వచితం_కాకపోతే HB_AAT_LAYOUT_TRAK_పట్టిక_HH
#నిర్వచించు HB_AAT_LAYOUT_TRAK_పట్టిక_HH

#అన్తర్భావ "hb-aat-layout-common.hh"
#అన్తర్భావ "hb-ot-layout.hh"
#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-stat-table.hh"

/*
 * trak -- Tracking
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6trak.html
 */
#నిర్వచించు HB_AAT_ట్యాగ్_trak HB_ట్యాగ్('t','r','a','k')


నామాకాశ AAT {


నిర్మాణం Track_పట్టిక_నమోదు
{
  మిత్ర నిర్మాణం Track_దత్తాంశం;

  దశాంశం get_track_విలువ () స్థిరరాశి { తిరిగి track.గమ్యం_float (); }

  దశాంశం interpolate_at (గుర్తులేని పూర్ణాంకం idx,
			దశాంశం ptem,
			స్థిరరాశి శూన్య *పునాది,
			hb_array_t<స్థిరరాశి F16DOT16> కొలత_పట్టిక) స్థిరరాశి
  {
    స్థిరరాశి FWORD *values = (పునాది+విలువలు_Z).arrayZ;

    దశాంశం s0 = కొలత_పట్టిక[idx].గమ్యం_float ();
    దశాంశం s1 = కొలత_పట్టిక[idx + 1].గమ్యం_float ();
    పూర్ణాంకం v0 = values[idx];
    పూర్ణాంకం v1 = values[idx + 1];

    // Deal with font bugs.
    ఒకవేళ (unlikely (s1 < s0))
    { hb_మార్పిడి (s0, s1); hb_మార్పిడి (v0, v1); }
    ఒకవేళ (unlikely (ptem < s0)) తిరిగి v0;
    ఒకవేళ (unlikely (ptem > s1)) తిరిగి v1;
    ఒకవేళ (unlikely (s0 == s1)) తిరిగి (v0 + v1) * 0.5f;

    దశాంశం t = (ptem - s0) / (s1 - s0);
    తిరిగి v0 + t * (v1 - v0);
  }

  దశాంశం get_విలువ (దశాంశం ptem,
		   స్థిరరాశి శూన్య *పునాది,
		   hb_array_t<స్థిరరాశి F16DOT16> కొలత_పట్టిక) స్థిరరాశి
  {
    స్థిరరాశి FWORD *values = (పునాది+విలువలు_Z).arrayZ;

    గుర్తులేని పూర్ణాంకం n_sizes = కొలత_పట్టిక.నిడివి;

    /*
     * Choose size.
     */
    ఒకవేళ (!n_sizes) తిరిగి 0.f;
    ఒకవేళ (n_sizes == 1) తిరిగి values[0];

    // At least two entries.

    గుర్తులేని i;
    కొరకు (i = 0; i < n_sizes; i++)
      ఒకవేళ (కొలత_పట్టిక[i].గమ్యం_float () >= ptem)
	ఆపు;

    // Boundary conditions.
    ఒకవేళ (i == 0)       తిరిగి values[0];
    ఒకవేళ (i == n_sizes) తిరిగి values[n_sizes - 1];

    // Exact match.
    ఒకవేళ (కొలత_పట్టిక[i].గమ్యం_float () == ptem) తిరిగి values[i];

    // Interpolate.
    తిరిగి interpolate_at (i - 1, ptem, పునాది, కొలత_పట్టిక);
  }

  బహిరంగ:
  తార్కిక sanitize (hb_sanitize_context_t *c,
		 స్థిరరాశి శూన్య *పునాది,
		 గుర్తులేని పూర్ణాంకం n_sizes) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  (విలువలు_Z.sanitize (c, పునాది, n_sizes))));
  }

  రక్షిత:
  F16DOT16	track;		/* Track value for this record. */
  OT::NameID	trackNameID;	/* The 'name' table index for this track.
				 * (a short word or phrase like "loose"
				 * or "very tight") */
  NNOffset16_గమ్యం<Unsized_Array_యొక్క<FWORD>>
		విలువలు_Z;	/* Offset from start of tracking table to
				 * per-size tracking values for this track. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం Track_దత్తాంశం
{
  దశాంశం get_tracking (స్థిరరాశి శూన్య *పునాది, దశాంశం ptem, దశాంశం track = 0.f) స్థిరరాశి
  {
    గుర్తులేని లెక్క = nTracks;
    hb_array_t<స్థిరరాశి F16DOT16> కొలత_పట్టిక = (పునాది+కొలత_పట్టిక_2).as_array (nSizes);

    ఒకవేళ (!లెక్క) తిరిగి 0.f;
    ఒకవేళ (లెక్క == 1) తిరిగి track_పట్టిక[0].get_విలువ (ptem, పునాది, కొలత_పట్టిక);

    // At least two entries.

    గుర్తులేని i = 0;
    గుర్తులేని j = లెక్క - 1;

    // Find the two entries that track is between.
    అయితే (i + 1 < లెక్క && track_పట్టిక[i + 1].get_track_విలువ () <= track)
      i++;
    అయితే (j > 0 && track_పట్టిక[j - 1].get_track_విలువ () >= track)
      j--;

    // Exact match.
    ఒకవేళ (i == j) తిరిగి track_పట్టిక[i].get_విలువ (ptem, పునాది, కొలత_పట్టిక);

    // Interpolate.

    దశాంశం t0 = track_పట్టిక[i].get_track_విలువ ();
    దశాంశం t1 = track_పట్టిక[j].get_track_విలువ ();

    దశాంశం t = (track - t0) / (t1 - t0);

    దశాంశం a = track_పట్టిక[i].get_విలువ (ptem, పునాది, కొలత_పట్టిక);
    దశాంశం b = track_పట్టిక[j].get_విలువ (ptem, పునాది, కొలత_పట్టిక);
    తిరిగి a + t * (b - a);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  కొలత_పట్టిక_2.sanitize (c, పునాది, nSizes) &&
			  track_పట్టిక.sanitize (c, nTracks, పునాది, nSizes)));
  }

  రక్షిత:
  HBUINT16	nTracks;	/* Number of separate tracks included in this table. */
  HBUINT16	nSizes;		/* Number of point sizes included in this table. */
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<F16DOT16>>
		కొలత_పట్టిక_2;	/* Offset from start of the tracking table to
				 * Array[nSizes] of size values.. */
  Unsized_Array_యొక్క<Track_పట్టిక_నమోదు>
		track_పట్టిక;	/* Array[nTracks] of TrackTableEntry records. */

  బహిరంగ:
  DEFINE_కొలత_ARRAY (8, track_పట్టిక);
};

నిర్మాణం trak
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_AAT_ట్యాగ్_trak;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.గమ్యం_int (); }

  hb_position_t get_h_tracking (hb_font_t *font, దశాంశం track = 0.f) స్థిరరాశి
  {
    దశాంశం ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_కొలత;
    తిరిగి font->em_scalef_x ((ఇది+horiz_దత్తాంశం).get_tracking (ఇది, ptem, track));
  }
  hb_position_t get_v_tracking (hb_font_t *font, దశాంశం track = 0.f) స్థిరరాశి
  {
    దశాంశం ptem = font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_కొలత;
    తిరిగి font->em_scalef_y ((ఇది+vert_దత్తాంశం).get_tracking (ఇది, ptem, track));
  }
  hb_position_t get_tracking (hb_font_t *font, hb_direction_t dir, దశాంశం track = 0.f) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_STYLE
    ఒకవేళ (!font->face->పట్టిక.STAT->has_దత్తాంశం ())
      తిరిగి 0;
    తిరిగి HB_DIRECTION_IS_HORIZONTAL (dir) ?
      get_h_tracking (font, track) :
      get_v_tracking (font, track);
#లేకపోతే
    తిరిగి 0;
#అయితే_ముగింపు
  }

  తార్కిక apply (hb_aat_apply_context_t *c, దశాంశం track = 0.f) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    దశాంశం ptem = c->font->ptem;
    ఒకవేళ (unlikely (ptem <= 0.f))
    {
      /* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
      ptem = HB_CORETEXT_DEFAULT_FONT_కొలత;
    }

    hb_బఫరు_t *బఫరు = c->బఫరు;
    ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (బఫరు->props.direction))
    {
      hb_position_t advance_గమ్యం_కూడు = get_h_tracking (c->font, track);
      foreach_grapheme (బఫరు, ప్రారంభం, చివర)
	బఫరు->pos[ప్రారంభం].x_advance = hb_saturate_కూడు (బఫరు->pos[ప్రారంభం].x_advance, advance_గమ్యం_కూడు);
    }
    లేకపోతే
    {
      hb_position_t advance_గమ్యం_కూడు = get_v_tracking (c->font, track);
      foreach_grapheme (బఫరు, ప్రారంభం, చివర)
	బఫరు->pos[ప్రారంభం].y_advance = hb_saturate_కూడు (బఫరు->pos[ప్రారంభం].y_advance, advance_గమ్యం_కూడు);
    }

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);

    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  సంస్కరణ.major == 1 &&
			  horiz_దత్తాంశం.sanitize (c, ఇది, ఇది) &&
			  vert_దత్తాంశం.sanitize (c, ఇది, ఇది)));
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the tracking table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the tracking table (set to 0). */
  Offset16_గమ్యం<Track_దత్తాంశం>
		horiz_దత్తాంశం;	/* Offset from start of tracking table to TrackData
				 * for horizontal text (or 0 if none). */
  Offset16_గమ్యం<Track_దత్తాంశం>
		vert_దత్తాంశం;	/* Offset from start of tracking table to TrackData
				 * for vertical text (or 0 if none). */
  HBUINT16	reserved;	/* Reserved. Set to 0. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};

} /* namespace AAT */


#అయితే_ముగింపు /* HB_AAT_LAYOUT_TRAK_TABLE_HH */
