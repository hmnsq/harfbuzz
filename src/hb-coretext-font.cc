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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_CORETEXT

#అన్తర్భావ "hb-coretext.hh"
#అన్తర్భావ "hb-aat-layout-trak-table.hh"

#అన్తర్భావ "hb-draw.hh"
#అన్తర్భావ "hb-font.hh"
#అన్తర్భావ "hb-machinery.hh"

#ఒకవేళ (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080) \
    || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 60000) \
    || (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 90000)
#  define kCTFontOrientationDefault kCTFontDefaultOrientation
#  define kCTFontOrientationHorizontal kCTFontHorizontalOrientation
#  define kCTFontOrientationVertical kCTFontVerticalOrientation
#అయితే_ముగింపు

#నిర్వచించు MAX_GLYPHS 256u

స్థిర hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       శూన్య *font_దత్తాంశం HB_వాడని,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       శూన్య *user_దత్తాంశం HB_వాడని)
{
  CTFontRef ct_font = (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  గుర్తులేని లెక్క = 0;

  ఒకవేళ (unicode <= 0xFFFF)
  {
    ch[లెక్క++] = unicode;
  }
  లేకపోతే ఒకవేళ (unicode <= 0x10FFFF)
  {
    ch[లెక్క++] = (unicode >> 10) + 0xD7C0;
    ch[లెక్క++] = (unicode & 0x3FF) + 0xDC00;
  }
  లేకపోతే
    ch[లెక్క++] = 0xFFFD;

  ఒకవేళ (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, లెక్క))
  {
    *glyph = cg_glyph[0];
    తిరిగి నిజం;
  }
  తిరిగి అబద్ధం;
}

స్థిర గుర్తులేని పూర్ణాంకం
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				శూన్య *font_దత్తాంశం,
				గుర్తులేని పూర్ణాంకం లెక్క,
				స్థిరరాశి hb_codepoint_t *మొదటి_unicode,
				గుర్తులేని పూర్ణాంకం unicode_stride,
				hb_codepoint_t *మొదటి_glyph,
				గుర్తులేని పూర్ణాంకం glyph_stride,
				శూన్య *user_దత్తాంశం HB_వాడని)
{
  CTFontRef ct_font = (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  తార్కిక slow_మార్గం = అబద్ధం;
  స్వయంచాలక *unicode = మొదటి_unicode;
  కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
  {
    ఒకవేళ (*unicode > 0xFFFF)
    {
      slow_మార్గం = నిజం;
      ఆపు;
    }
    unicode = &Struct_At_స్థానభ్రంశం<స్థిరరాశి hb_codepoint_t> (unicode, unicode_stride);
  }

  ఒకవేళ (unlikely (slow_మార్గం))
  {
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (!hb_coretext_get_nominal_glyph (font, font_దత్తాంశం, *మొదటి_unicode, మొదటి_glyph, శూన్యసూచిక))
	తిరిగి i;
      మొదటి_unicode = &Struct_At_స్థానభ్రంశం<స్థిరరాశి hb_codepoint_t> (మొదటి_unicode, unicode_stride);
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
    }
    తిరిగి లెక్క;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  కొరకు (గుర్తులేని i = 0; i < లెక్క; i += MAX_GLYPHS)
  {
    గుర్తులేని c = (గుర్తులేని) hb_min ((పూర్ణాంకం) MAX_GLYPHS, (పూర్ణాంకం) లెక్క - (పూర్ణాంకం) i);
    కొరకు (గుర్తులేని j = 0; j < c; j++)
    {
      ch[j] = *మొదటి_unicode;
      మొదటి_unicode = &Struct_At_స్థానభ్రంశం<స్థిరరాశి hb_codepoint_t> (మొదటి_unicode, unicode_stride);
    }
    ఒకవేళ (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      కొరకు (గుర్తులేని j = 0; j < c; j++)
      {
	ఒకవేళ (!hb_coretext_get_nominal_glyph (font, font_దత్తాంశం, ch[j], మొదటి_glyph, శూన్యసూచిక))
	  తిరిగి i + j;
	మొదటి_glyph = &Struct_At_స్థానభ్రంశం<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      }
    }
    కొరకు (గుర్తులేని j = 0; j < c; j++)
    {
      *మొదటి_glyph = cg_glyph[j];
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
    }
  }

  తిరిగి లెక్క;
}

స్థిర hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 శూన్య *font_దత్తాంశం HB_వాడని,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 శూన్య *user_దత్తాంశం HB_వాడని)
{
  CTFontRef ct_font = (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  గుర్తులేని లెక్క = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  ఒకవేళ (unicode <= 0xFFFF)
    ch[లెక్క++] = unicode;
  లేకపోతే ఒకవేళ (unicode <= 0x10FFFF)
  {
    ch[లెక్క++] = (unicode >> 10) + 0xD7C0;
    ch[లెక్క++] = (unicode & 0x3FF) + 0xDC00;
  }
  లేకపోతే
    ch[లెక్క++] = 0xFFFD;

  ఒకవేళ (variation_selector <= 0xFFFF)
    ch[లెక్క++] = variation_selector;
  లేకపోతే ఒకవేళ (variation_selector <= 0x10FFFF)
  {
    ch[లెక్క++] = (variation_selector >> 10) + 0xD7C0;
    ch[లెక్క++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  లేకపోతే
    ch[లెక్క++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, లెక్క);

  // All except for first should be zero if we succeeded
  కొరకు (గుర్తులేని i = 1; i < లెక్క; i++)
    ఒకవేళ (cg_glyph[i])
      తిరిగి అబద్ధం;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  తిరిగి నిజం;
}

స్థిర శూన్య
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  శూన్య* font_దత్తాంశం HB_వాడని,
				  గుర్తులేని లెక్క,
				  స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
				  గుర్తులేని glyph_stride,
				  hb_position_t *మొదటి_advance,
				  గుర్తులేని advance_stride,
				  శూన్య *user_దత్తాంశం HB_వాడని)
{
  CTFontRef ct_font = (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext;

  CGFloat ct_font_కొలత = CTFont_Get_కొలత (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_కొలత;
  hb_position_t tracking = font->face->పట్టిక.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  కొరకు (గుర్తులేని i = 0; i < లెక్క; i += MAX_GLYPHS)
  {
    గుర్తులేని c = (గుర్తులేని) hb_min ((పూర్ణాంకం) MAX_GLYPHS, (పూర్ణాంకం) లెక్క - (పూర్ణాంకం) i);
    కొరకు (గుర్తులేని j = 0; j < c; j++)
    {
      cg_glyph[j] = *మొదటి_glyph;
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం<స్థిరరాశి hb_codepoint_t> (మొదటి_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    కొరకు (గుర్తులేని j = 0; j < c; j++)
    {
      *మొదటి_advance = hb_clamp_గమ్యం<hb_position_t> (
	  round (advances[j].వెడల్పు * x_mult) - (ద్విగుణ) tracking);
      మొదటి_advance = &Struct_At_స్థానభ్రంశం<hb_position_t> (మొదటి_advance, advance_stride);
    }
  }
}

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
స్థిర శూన్య
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  శూన్య* font_దత్తాంశం HB_వాడని,
				  గుర్తులేని లెక్క,
				  స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
				  గుర్తులేని glyph_stride,
				  hb_position_t *మొదటి_advance,
				  గుర్తులేని advance_stride,
				  శూన్య *user_దత్తాంశం HB_వాడని)
{
  CTFontRef ct_font = (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext;

  CGFloat ct_font_కొలత = CTFont_Get_కొలత (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_కొలత;
  hb_position_t tracking = font->face->పట్టిక.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  కొరకు (గుర్తులేని i = 0; i < లెక్క; i += MAX_GLYPHS)
  {
    గుర్తులేని c = (గుర్తులేని) hb_min ((పూర్ణాంకం) MAX_GLYPHS, (పూర్ణాంకం) లెక్క - (పూర్ణాంకం) i);
    కొరకు (గుర్తులేని j = 0; j < c; j++)
    {
      cg_glyph[j] = *మొదటి_glyph;
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం<స్థిరరాశి hb_codepoint_t> (మొదటి_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    కొరకు (గుర్తులేని j = 0; j < c; j++)
    {
      *మొదటి_advance = hb_clamp_గమ్యం<hb_position_t> (
	  round (advances[j].వెడల్పు * y_mult) - (ద్విగుణ) tracking);
      మొదటి_advance = &Struct_At_స్థానభ్రంశం<hb_position_t> (మొదటి_advance, advance_stride);
    }
  }
}

స్థిర hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				శూన్య *font_దత్తాంశం HB_వాడని,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				శూన్య *user_దత్తాంశం HB_వాడని)
{
  CTFontRef ct_font = (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext;

  CGFloat ct_font_కొలత = CTFont_Get_కొలత (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_కొలత;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_కొలత;

  స్థిరరాశి CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_గమ్యం<hb_position_t> (round (x_mult * origin.వెడల్పు));
  *y = hb_clamp_గమ్యం<hb_position_t> (round (y_mult * origin.ఎత్తు));

  తిరిగి నిజం;
}
#అయితే_ముగింపు

స్థిర hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       శూన్య *font_దత్తాంశం HB_వాడని,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       శూన్య *user_దత్తాంశం HB_వాడని)
{
  CTFontRef ct_font = (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext;

  CGFloat ct_font_కొలత = CTFont_Get_కొలత (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_కొలత;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_కొలత;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, ఖాళీ, 1);

  ద్విగుణ y_bearing = round ((bounds.origin.y + bounds.కొలత.ఎత్తు) * y_mult);

  extents->x_bearing = hb_clamp_గమ్యం<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_గమ్యం<hb_position_t> (y_bearing);
  extents->వెడల్పు = hb_clamp_గమ్యం<hb_position_t> (round (bounds.కొలత.వెడల్పు * x_mult));
  extents->ఎత్తు = hb_clamp_గమ్యం<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  తిరిగి నిజం;
}

స్థిర hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				శూన్య *font_దత్తాంశం HB_వాడని,
				hb_font_extents_t *metrics,
				శూన్య *user_దత్తాంశం HB_వాడని)
{
  CTFontRef ct_font = (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext;

  CGFloat ct_font_కొలత = CTFont_Get_కొలత (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_కొలత;

  metrics->ascender = hb_clamp_గమ్యం<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_గమ్యం<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->పంక్తి_gap = hb_clamp_గమ్యం<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  తిరిగి నిజం;
}

#నిర్వచితం_కాకపోతే HB_NO_DRAW

స్థిర శూన్య
ct_apply_func (శూన్య *info, స్థిరరాశి CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  ఎంపిక (element->type)
  {
  స్థితి k_CGPath_Element_Move_గమ్యం_బిందువు:
    draws->move_గమ్యం (element->points[0].x, element->points[0].y);
    ఆపు;
  స్థితి k_CGPath_Element_కూడు_పంక్తి_గమ్యం_బిందువు:
    draws->పంక్తి_గమ్యం (element->points[0].x, element->points[0].y);
    ఆపు;
  స్థితి k_CGPath_Element_కూడు_Quad_వక్రం_గమ్యం_బిందువు:
    draws->quadratic_గమ్యం (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    ఆపు;
  స్థితి k_CGPath_Element_కూడు_వక్రం_గమ్యం_బిందువు:
    draws->cubic_గమ్యం (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    ఆపు;
  స్థితి kCGPathElementCloseSubpath:
    draws->close_మార్గం ();
    ఆపు;
  }
}

స్థిర hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				శూన్య *font_దత్తాంశం HB_వాడని,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, శూన్య *draw_దత్తాంశం,
				శూన్య *user_దత్తాంశం)
{
  CTFontRef ct_font = (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext;

  CGFloat ct_font_కొలత = CTFont_Get_కొలత (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_కొలత;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_కొలత;

  CGAffineTransform transform = CGAffine_Transform_తటస్థం;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> మార్గం (CTFont_Create_మార్గం_For_Glyph (ct_font, glyph, &transform));
  ఒకవేళ (!మార్గం)
    తిరిగి అబద్ధం;

  hb_draw_session_t drawing {draw_funcs, draw_దత్తాంశం};

  CGPathApply (మార్గం, &drawing, ct_apply_func);

  తిరిగి నిజం;
}
#అయితే_ముగింపు

స్థిర hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    శూన్య *font_దత్తాంశం HB_వాడని,
			    hb_codepoint_t glyph,
			    అక్షరం *name, గుర్తులేని పూర్ణాంకం కొలత,
			    శూన్య *user_దత్తాంశం HB_వాడని)
{
  CGFontRef cg_font = (CGFontRef) (స్థిరరాశి శూన్య *) font->face->దత్తాంశం.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  ఒకవేళ (!cf_name)
    తిరిగి అబద్ధం;

  ఒకవేళ (!కొలత)
    తిరిగి నిజం;

  CFIndex len = CFString_Get_నిడివి (cf_name);
  ఒకవేళ (len > (CFIndex)కొలత - 1)
    len = (CFIndex)కొలత - 1;

  CFString_Get_బైట్లు (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, అబద్ధం,
		    (UInt8 *) name, కొలత, &len);

  name[len] = '\0';

  తిరిగి నిజం;
}

స్థిర hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 శూన్య *font_దత్తాంశం HB_వాడని,
				 స్థిరరాశి అక్షరం *name, పూర్ణాంకం len,
				 hb_codepoint_t *glyph,
				 శూన్య *user_దత్తాంశం HB_వాడని)
{
  CTFontRef ct_font = (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext;

  ఒకవేళ (len == -1)
    len = అక్షరశ్రేణి_పొడవు (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFString_Create_With_బైట్లు (kCFAllocatorDefault,
							     (స్థిరరాశి UInt8 *) name, len,
							     kCFStringEncodingUTF8, అబద్ధం));
  CGGlyph cg_glyph = CTFontGetGlyphWithName (ct_font, cf_name);
  *glyph = cg_glyph;

  // TODO Return true for .notdef; hb-ft does that.

  తిరిగి cg_glyph != 0;
}


స్థిర అంతఃవరుస శూన్య free_static_coretext_funcs ();

స్థిర నిర్మాణం hb_coretext_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_coretext_font_funcs_lazy_loader_t>
{
  స్థిర hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_సమితి_nominal_glyph_func (funcs, hb_coretext_get_nominal_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_nominal_glyphs_func (funcs, hb_coretext_get_nominal_glyphs, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_variation_glyph_func (funcs, hb_coretext_get_variation_glyph, శూన్యసూచిక, శూన్యసూచిక);

    hb_font_funcs_సమితి_font_h_extents_func (funcs, hb_coretext_get_font_h_extents, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_h_advances_func (funcs, hb_coretext_get_glyph_h_advances, శూన్యసూచిక, శూన్యసూచిక);

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
    hb_font_funcs_సమితి_glyph_v_advances_func (funcs, hb_coretext_get_glyph_v_advances, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_v_origin_func (funcs, hb_coretext_get_glyph_v_origin, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_DRAW
    hb_font_funcs_సమితి_draw_glyph_or_fail_func (funcs, hb_coretext_draw_glyph_or_fail, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు

    hb_font_funcs_సమితి_glyph_extents_func (funcs, hb_coretext_get_glyph_extents, శూన్యసూచిక, శూన్యసూచిక);

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_GLYPH_పేర్లు
    hb_font_funcs_సమితి_glyph_name_func (funcs, hb_coretext_get_glyph_name, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_from_name_func (funcs, hb_coretext_get_glyph_from_name, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_coretext_funcs);

    తిరిగి funcs;
  }
} static_coretext_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_coretext_funcs ()
{
  static_coretext_funcs.free_instance ();
}

స్థిర hb_font_funcs_t *
_hb_coretext_get_font_funcs ()
{
  తిరిగి static_coretext_funcs.get_unconst ();
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
శూన్య
hb_coretext_font_సమితి_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  ఒకవేళ (unlikely (!ct_font))
  {
    hb_font_సమితి_funcs (font,
		       hb_font_funcs_get_ఖాళీలు (),
		       శూన్యసూచిక, శూన్యసూచిక);
    తిరిగి;
  }

  hb_font_సమితి_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     శూన్యసూచిక, శూన్యసూచిక);
}

#నిర్వచనం_రద్దు MAX_GLYPHS

#అయితే_ముగింపు
