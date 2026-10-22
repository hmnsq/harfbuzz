/*
 * Copyright © 2019  Ebrahim Byagowi
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
 */

#నిర్వచితం_కాకపోతే HB_AAT_LAYOUT_OPBD_పట్టిక_HH
#నిర్వచించు HB_AAT_LAYOUT_OPBD_పట్టిక_HH

#అన్తర్భావ "hb-aat-layout-common.hh"
#అన్తర్భావ "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#నిర్వచించు HB_AAT_ట్యాగ్_opbd HB_ట్యాగ్('o','p','b','d')


నామాకాశ AAT {

నిర్మాణం OpticalBounds
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  FWORD		ఎడమ_Side;
  FWORD		శిఖరం_Side;
  FWORD		కుడి_Side;
  FWORD		bottomSide;
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం opbdFormat0
{
  తార్కిక get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    స్థిరరాశి Offset16_గమ్యం<OpticalBounds> *bounds_స్థానభ్రంశం = lookup_పట్టిక.get_విలువ (glyph_id, font->face->get_అంకె_glyphs ());
    ఒకవేళ (!bounds_స్థానభ్రంశం) తిరిగి అబద్ధం;
    స్థిరరాశి OpticalBounds &bounds = పునాది+*bounds_స్థానభ్రంశం;

    ఒకవేళ (extents)
      *extents = {
	font->em_scale_x (bounds.ఎడమ_Side),
	font->em_scale_y (bounds.శిఖరం_Side),
	font->em_scale_x (bounds.కుడి_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    తిరిగి నిజం;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) && lookup_పట్టిక.sanitize (c, పునాది)));
  }

  రక్షిత:
  Lookup<Offset16_గమ్యం<OpticalBounds>>
		lookup_పట్టిక;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  బహిరంగ:
  DEFINE_కొలత_MIN (2);
};

నిర్మాణం opbdFormat1
{
  తార్కిక get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    స్థిరరాశి Offset16_గమ్యం<OpticalBounds> *bounds_స్థానభ్రంశం = lookup_పట్టిక.get_విలువ (glyph_id, font->face->get_అంకె_glyphs ());
    ఒకవేళ (!bounds_స్థానభ్రంశం) తిరిగి అబద్ధం;
    స్థిరరాశి OpticalBounds &bounds = పునాది+*bounds_స్థానభ్రంశం;

    hb_position_t ఎడమ = 0, శిఖరం_2 = 0, కుడి = 0, bottom = 0, ignore;
    ఒకవేళ (font->get_glyph_contour_బిందువు (glyph_id, bounds.ఎడమ_Side, &ఎడమ, &ignore) ||
	font->get_glyph_contour_బిందువు (glyph_id, bounds.శిఖరం_Side, &ignore, &శిఖరం_2) ||
	font->get_glyph_contour_బిందువు (glyph_id, bounds.కుడి_Side, &కుడి, &ignore) ||
	font->get_glyph_contour_బిందువు (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      ఒకవేళ (extents)
	*extents = {ఎడమ, శిఖరం_2, కుడి, bottom};
      తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) && lookup_పట్టిక.sanitize (c, పునాది)));
  }

  రక్షిత:
  Lookup<Offset16_గమ్యం<OpticalBounds>>
		lookup_పట్టిక;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  బహిరంగ:
  DEFINE_కొలత_MIN (2);
};

నిర్మాణం opbd
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_AAT_ట్యాగ్_opbd;

  తార్కిక get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) స్థిరరాశి
  {
    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_bounds (font, glyph_id, extents, ఇది);
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_bounds (font, glyph_id, extents, ఇది);
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది) || సంస్కరణ.major != 1))
      return_trace (అబద్ధం);
    hb_barrier ();

    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); return_trace (u.format0.sanitize (c, ఇది));
    స్థితి 1: hb_barrier (); return_trace (u.format1.sanitize (c, ఇది));
    అప్రమేయ:return_trace (నిజం);
    }
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  సంఘం {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  బహిరంగ:
  DEFINE_కొలత_MIN (8);
};

} /* namespace AAT */


#అయితే_ముగింపు /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
