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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_LAYOUT_OPBD_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_LAYOUT_OPBD_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"

/*
 * opbd -- Optical Bounds
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6opbd.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_ਟੈਗ_opbd HB_ਟੈਗ('o','p','b','d')


ਨਾਮਾਕਾਸ਼ AAT {

ਬਣਤਰ OpticalBounds
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  FWORD		ਖੱਬਾ_Side;
  FWORD		ਸਿਖਰ_Side;
  FWORD		ਸੱਜਾ_Side;
  FWORD		bottomSide;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ opbdFormat0
{
  ਤਾਰਕਿਕ get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<OpticalBounds> *bounds_ਵਿੱਥ = lookup_ਸਾਰਣੀ.get_ਮੁੱਲ (glyph_id, font->face->get_ਅੰਕ_glyphs ());
    ਜੇ (!bounds_ਵਿੱਥ) ਵਾਪਸ ਝੂਠ;
    ਅਚਲ OpticalBounds &bounds = ਅਧਾਰ+*bounds_ਵਿੱਥ;

    ਜੇ (extents)
      *extents = {
	font->em_scale_x (bounds.ਖੱਬਾ_Side),
	font->em_scale_y (bounds.ਸਿਖਰ_Side),
	font->em_scale_x (bounds.ਸੱਜਾ_Side),
	font->em_scale_y (bounds.bottomSide)
      };
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) && lookup_ਸਾਰਣੀ.sanitize (c, ਅਧਾਰ)));
  }

  ਸੁਰੱਖਿਅਤ:
  Lookup<Offset16_ਮੰਜ਼ਿਲ<OpticalBounds>>
		lookup_ਸਾਰਣੀ;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (2);
};

ਬਣਤਰ opbdFormat1
{
  ਤਾਰਕਿਕ get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<OpticalBounds> *bounds_ਵਿੱਥ = lookup_ਸਾਰਣੀ.get_ਮੁੱਲ (glyph_id, font->face->get_ਅੰਕ_glyphs ());
    ਜੇ (!bounds_ਵਿੱਥ) ਵਾਪਸ ਝੂਠ;
    ਅਚਲ OpticalBounds &bounds = ਅਧਾਰ+*bounds_ਵਿੱਥ;

    hb_position_t ਖੱਬੇ = 0, ਸਿਖਰ_2 = 0, ਸੱਜੇ = 0, bottom = 0, ignore;
    ਜੇ (font->get_glyph_contour_ਬਿੰਦੂ (glyph_id, bounds.ਖੱਬਾ_Side, &ਖੱਬੇ, &ignore) ||
	font->get_glyph_contour_ਬਿੰਦੂ (glyph_id, bounds.ਸਿਖਰ_Side, &ignore, &ਸਿਖਰ_2) ||
	font->get_glyph_contour_ਬਿੰਦੂ (glyph_id, bounds.ਸੱਜਾ_Side, &ਸੱਜੇ, &ignore) ||
	font->get_glyph_contour_ਬਿੰਦੂ (glyph_id, bounds.bottomSide, &ignore, &bottom))
    {
      ਜੇ (extents)
	*extents = {ਖੱਬੇ, ਸਿਖਰ_2, ਸੱਜੇ, bottom};
      ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) && lookup_ਸਾਰਣੀ.sanitize (c, ਅਧਾਰ)));
  }

  ਸੁਰੱਖਿਅਤ:
  Lookup<Offset16_ਮੰਜ਼ਿਲ<OpticalBounds>>
		lookup_ਸਾਰਣੀ;	/* Lookup table associating glyphs with the four
				 * int16 values for the left-side, top-side,
				 * right-side, and bottom-side optical bounds. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (2);
};

ਬਣਤਰ opbd
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_AAT_ਟੈਗ_opbd;

  ਤਾਰਕਿਕ get_bounds (hb_font_t *font, hb_codepoint_t glyph_id,
		   hb_glyph_extents_t *extents) ਅਚਲ
  {
    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_bounds (font, glyph_id, extents, ਇਹ);
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_bounds (font, glyph_id, extents, ਇਹ);
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ) || ਸੰਸਕਰਣ.major != 1))
      return_trace (ਝੂਠ);
    hb_barrier ();

    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); return_trace (u.format0.sanitize (c, ਇਹ));
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.sanitize (c, ਇਹ));
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version number of the optical bounds
				 * table (0x00010000 for the current version). */
  HBUINT16	format;		/* Format of the optical bounds table.
				 * Format 0 indicates distance and Format 1 indicates
				 * control point. */
  ਸੰਘ {
  opbdFormat0	format0;
  opbdFormat1	format1;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8);
};

} /* namespace AAT */


#ਜੇ_ਅੰਤ /* HB_AAT_LAYOUT_OPBD_TABLE_HH */
