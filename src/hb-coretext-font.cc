/*
 * Copyright © 2024  Google, Inc.
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
 * Author(s): Behdad Esfahbod
 */

#समावेश "hb.hh"

#अगर_घोषित HAVE_CORETEXT

#समावेश "hb-coretext.hh"
#समावेश "hb-aat-layout-trak-table.hh"

#समावेश "hb-draw.hh"
#समावेश "hb-font.hh"
#समावेश "hb-machinery.hh"

#अगर (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080) \
    || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 60000) \
    || (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 90000)
#  define kCTFontOrientationDefault kCTFontDefaultOrientation
#  define kCTFontOrientationHorizontal kCTFontHorizontalOrientation
#  define kCTFontOrientationVertical kCTFontVerticalOrientation
#पूर्ण_अगर

#घोषणा MAX_GLYPHS 256u

अटल hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       व्योम *font_तथ्य HB_UNUSED,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       व्योम *user_तथ्य HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  अचिन्हित गिनती = 0;

  अगर (unicode <= 0xFFFF)
  {
    ch[गिनती++] = unicode;
  }
  अन्यथा अगर (unicode <= 0x10FFFF)
  {
    ch[गिनती++] = (unicode >> 10) + 0xD7C0;
    ch[गिनती++] = (unicode & 0x3FF) + 0xDC00;
  }
  अन्यथा
    ch[गिनती++] = 0xFFFD;

  अगर (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, गिनती))
  {
    *glyph = cg_glyph[0];
    वापस true;
  }
  वापस false;
}

अटल अचिन्हित पूर्णांक
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				व्योम *font_तथ्य,
				अचिन्हित पूर्णांक गिनती,
				स्थिर hb_codepoint_t *first_unicode,
				अचिन्हित पूर्णांक unicode_stride,
				hb_codepoint_t *first_glyph,
				अचिन्हित पूर्णांक glyph_stride,
				व्योम *user_तथ्य HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  bool slow_path = false;
  स्वतः *unicode = first_unicode;
  क्रम (अचिन्हित i = 0; i < गिनती; i++)
  {
    अगर (*unicode > 0xFFFF)
    {
      slow_path = true;
      अवरोध;
    }
    unicode = &StructAtOffset<स्थिर hb_codepoint_t> (unicode, unicode_stride);
  }

  अगर (unlikely (slow_path))
  {
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      अगर (!hb_coretext_get_nominal_glyph (font, font_तथ्य, *first_unicode, first_glyph, nullptr))
	वापस i;
      first_unicode = &StructAtOffset<स्थिर hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    वापस गिनती;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  क्रम (अचिन्हित i = 0; i < गिनती; i += MAX_GLYPHS)
  {
    अचिन्हित c = (अचिन्हित) hb_min ((पूर्णांक) MAX_GLYPHS, (पूर्णांक) गिनती - (पूर्णांक) i);
    क्रम (अचिन्हित j = 0; j < c; j++)
    {
      ch[j] = *first_unicode;
      first_unicode = &StructAtOffset<स्थिर hb_codepoint_t> (first_unicode, unicode_stride);
    }
    अगर (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      क्रम (अचिन्हित j = 0; j < c; j++)
      {
	अगर (!hb_coretext_get_nominal_glyph (font, font_तथ्य, ch[j], first_glyph, nullptr))
	  वापस i + j;
	first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
      }
    }
    क्रम (अचिन्हित j = 0; j < c; j++)
    {
      *first_glyph = cg_glyph[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
  }

  वापस गिनती;
}

अटल hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 व्योम *font_तथ्य HB_UNUSED,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 व्योम *user_तथ्य HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  अचिन्हित गिनती = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  अगर (unicode <= 0xFFFF)
    ch[गिनती++] = unicode;
  अन्यथा अगर (unicode <= 0x10FFFF)
  {
    ch[गिनती++] = (unicode >> 10) + 0xD7C0;
    ch[गिनती++] = (unicode & 0x3FF) + 0xDC00;
  }
  अन्यथा
    ch[गिनती++] = 0xFFFD;

  अगर (variation_selector <= 0xFFFF)
    ch[गिनती++] = variation_selector;
  अन्यथा अगर (variation_selector <= 0x10FFFF)
  {
    ch[गिनती++] = (variation_selector >> 10) + 0xD7C0;
    ch[गिनती++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  अन्यथा
    ch[गिनती++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, गिनती);

  // All except for first should be zero if we succeeded
  क्रम (अचिन्हित i = 1; i < गिनती; i++)
    अगर (cg_glyph[i])
      वापस false;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  वापस true;
}

अटल व्योम
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  व्योम* font_तथ्य HB_UNUSED,
				  अचिन्हित गिनती,
				  स्थिर hb_codepoint_t *first_glyph,
				  अचिन्हित glyph_stride,
				  hb_position_t *first_advance,
				  अचिन्हित advance_stride,
				  व्योम *user_तथ्य HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext;

  CGFloat ct_font_आकार = CTFont_Get_आकार (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_आकार;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  क्रम (अचिन्हित i = 0; i < गिनती; i += MAX_GLYPHS)
  {
    अचिन्हित c = (अचिन्हित) hb_min ((पूर्णांक) MAX_GLYPHS, (पूर्णांक) गिनती - (पूर्णांक) i);
    क्रम (अचिन्हित j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<स्थिर hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    क्रम (अचिन्हित j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * x_mult) - (द्विगुन) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

#अगर_अघोषित HB_NO_VERTICAL
अटल व्योम
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  व्योम* font_तथ्य HB_UNUSED,
				  अचिन्हित गिनती,
				  स्थिर hb_codepoint_t *first_glyph,
				  अचिन्हित glyph_stride,
				  hb_position_t *first_advance,
				  अचिन्हित advance_stride,
				  व्योम *user_तथ्य HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext;

  CGFloat ct_font_आकार = CTFont_Get_आकार (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_आकार;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  क्रम (अचिन्हित i = 0; i < गिनती; i += MAX_GLYPHS)
  {
    अचिन्हित c = (अचिन्हित) hb_min ((पूर्णांक) MAX_GLYPHS, (पूर्णांक) गिनती - (पूर्णांक) i);
    क्रम (अचिन्हित j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<स्थिर hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    क्रम (अचिन्हित j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * y_mult) - (द्विगुन) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

अटल hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				व्योम *font_तथ्य HB_UNUSED,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				व्योम *user_तथ्य HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext;

  CGFloat ct_font_आकार = CTFont_Get_आकार (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_आकार;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_आकार;

  स्थिर CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_to<hb_position_t> (round (x_mult * origin.width));
  *y = hb_clamp_to<hb_position_t> (round (y_mult * origin.height));

  वापस true;
}
#पूर्ण_अगर

अटल hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       व्योम *font_तथ्य HB_UNUSED,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       व्योम *user_तथ्य HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext;

  CGFloat ct_font_आकार = CTFont_Get_आकार (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_आकार;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_आकार;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, शून्य, 1);

  द्विगुन y_bearing = round ((bounds.origin.y + bounds.आकार.height) * y_mult);

  extents->x_bearing = hb_clamp_to<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
  extents->width = hb_clamp_to<hb_position_t> (round (bounds.आकार.width * x_mult));
  extents->height = hb_clamp_to<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  वापस true;
}

अटल hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				व्योम *font_तथ्य HB_UNUSED,
				hb_font_extents_t *metrics,
				व्योम *user_तथ्य HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext;

  CGFloat ct_font_आकार = CTFont_Get_आकार (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_आकार;

  metrics->ascender = hb_clamp_to<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_to<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->पंक्ति_gap = hb_clamp_to<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  वापस true;
}

#अगर_अघोषित HB_NO_DRAW

अटल व्योम
ct_apply_func (व्योम *info, स्थिर CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  चयन (element->type)
  {
  हाल kCGPathElementMoveToPoint:
    draws->move_to (element->points[0].x, element->points[0].y);
    अवरोध;
  हाल k_CGPath_Element_Add_पंक्ति_To_Point:
    draws->पंक्ति_to (element->points[0].x, element->points[0].y);
    अवरोध;
  हाल kCGPathElementAddQuadCurveToPoint:
    draws->quadratic_to (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    अवरोध;
  हाल kCGPathElementAddCurveToPoint:
    draws->cubic_to (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    अवरोध;
  हाल kCGPathElementCloseSubpath:
    draws->close_path ();
    अवरोध;
  }
}

अटल hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				व्योम *font_तथ्य HB_UNUSED,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, व्योम *draw_तथ्य,
				व्योम *user_तथ्य)
{
  CTFontRef ct_font = (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext;

  CGFloat ct_font_आकार = CTFont_Get_आकार (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_आकार;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_आकार;

  CGAffineTransform transform = CGAffineTransformIdentity;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> path (CTFontCreatePathForGlyph (ct_font, glyph, &transform));
  अगर (!path)
    वापस false;

  hb_draw_session_t drawing {draw_funcs, draw_तथ्य};

  CGPathApply (path, &drawing, ct_apply_func);

  वापस true;
}
#पूर्ण_अगर

अटल hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    व्योम *font_तथ्य HB_UNUSED,
			    hb_codepoint_t glyph,
			    अक्षर *name, अचिन्हित पूर्णांक आकार,
			    व्योम *user_तथ्य HB_UNUSED)
{
  CGFontRef cg_font = (CGFontRef) (स्थिर व्योम *) font->face->तथ्य.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  अगर (!cf_name)
    वापस false;

  अगर (!आकार)
    वापस true;

  CFIndex len = CFString_Get_लंबाई (cf_name);
  अगर (len > (CFIndex)आकार - 1)
    len = (CFIndex)आकार - 1;

  CFStringGetBytes (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, false,
		    (UInt8 *) name, आकार, &len);

  name[len] = '\0';

  वापस true;
}

अटल hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 व्योम *font_तथ्य HB_UNUSED,
				 स्थिर अक्षर *name, पूर्णांक len,
				 hb_codepoint_t *glyph,
				 व्योम *user_तथ्य HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext;

  अगर (len == -1)
    len = म_माप (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFStringCreateWithBytes (kCFAllocatorDefault,
							     (स्थिर UInt8 *) name, len,
							     kCFStringEncodingUTF8, false));
  CGGlyph cg_glyph = CTFontGetGlyphWithName (ct_font, cf_name);
  *glyph = cg_glyph;

  // TODO Return true for .notdef; hb-ft does that.

  वापस cg_glyph != 0;
}


अटल अंतरभूत व्योम free_static_coretext_funcs ();

अटल काष्ठा hb_coretext_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_coretext_font_funcs_lazy_loader_t>
{
  अटल hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_set_nominal_glyph_func (funcs, hb_coretext_get_nominal_glyph, nullptr, nullptr);
    hb_font_funcs_set_nominal_glyphs_func (funcs, hb_coretext_get_nominal_glyphs, nullptr, nullptr);
    hb_font_funcs_set_variation_glyph_func (funcs, hb_coretext_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_set_font_h_extents_func (funcs, hb_coretext_get_font_h_extents, nullptr, nullptr);
    hb_font_funcs_set_glyph_h_advances_func (funcs, hb_coretext_get_glyph_h_advances, nullptr, nullptr);

#अगर_अघोषित HB_NO_VERTICAL
    hb_font_funcs_set_glyph_v_advances_func (funcs, hb_coretext_get_glyph_v_advances, nullptr, nullptr);
    hb_font_funcs_set_glyph_v_origin_func (funcs, hb_coretext_get_glyph_v_origin, nullptr, nullptr);
#पूर्ण_अगर

#अगर_अघोषित HB_NO_DRAW
    hb_font_funcs_set_draw_glyph_or_fail_func (funcs, hb_coretext_draw_glyph_or_fail, nullptr, nullptr);
#पूर्ण_अगर

    hb_font_funcs_set_glyph_extents_func (funcs, hb_coretext_get_glyph_extents, nullptr, nullptr);

#अगर_अघोषित HB_NO_OT_FONT_GLYPH_NAMES
    hb_font_funcs_set_glyph_name_func (funcs, hb_coretext_get_glyph_name, nullptr, nullptr);
    hb_font_funcs_set_glyph_from_name_func (funcs, hb_coretext_get_glyph_from_name, nullptr, nullptr);
#पूर्ण_अगर

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_coretext_funcs);

    वापस funcs;
  }
} static_coretext_funcs;

अटल अंतरभूत
व्योम free_static_coretext_funcs ()
{
  static_coretext_funcs.free_instance ();
}

अटल hb_font_funcs_t *
_hb_coretext_get_font_funcs ()
{
  वापस static_coretext_funcs.get_unconst ();
}


/**
 * hb_coretext_font_set_funcs:
 * @font: #hb_font_t to work upon
 *
 * Configures the font-functions structure of the specified
 * #hb_font_t font object to use CoreText font functions.
 *
 * In particular, you can use this function to configure an
 * existing #hb_face_t face object for use with CoreText font
 * functions even if that #hb_face_t face object was initially
 * created with hb_face_create(), and therefore was not
 * initially configured to use CoreText font functions.
 *
 * <note>Note: Internally, this function creates a CTFont.
* </note>
 *
 * Since: 10.1.0
 **/
व्योम
hb_coretext_font_set_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  अगर (unlikely (!ct_font))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    वापस;
  }

  hb_font_set_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     nullptr, nullptr);
}

#अघोषित MAX_GLYPHS

#पूर्ण_अगर
