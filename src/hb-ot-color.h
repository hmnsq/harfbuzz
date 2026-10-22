/*
 * Copyright © 2016  Google, Inc.
 * Copyright © 2018  Khaled Hosny
 * Copyright © 2018  Ebrahim Byagowi
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
 * Google Author(s): Sascha Brawer, Behdad Esfahbod
 */

#ଯଦି !defined(HB_OT_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#error "Include <hb-ot.h> instead."
#endif

#ifndef HB_OT_COLOR_H
#define HB_OT_COLOR_H

#ଅନ୍ତର୍ଭାଵ "hb.h"
#ଅନ୍ତର୍ଭାଵ "hb-ot-name.h"

HB_BEGIN_DECLS


/*
 * Color palettes.
 */

HB_EXTERN hb_bool_t
hb_ot_color_has_palettes (hb_face_t *face);

HB_EXTERN unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_ot_color_palette_get_ଗଣନା (hb_face_t *face);

HB_EXTERN hb_ot_name_id_t
hb_ot_color_palette_get_name_id (hb_face_t *face,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_index);

HB_EXTERN hb_ot_name_id_t
hb_ot_color_palette_color_get_name_id (hb_face_t *face,
				       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା color_index);

/**
 * hb_ot_color_palette_flags_t:
 * @HB_OT_COLOR_PALETTE_FLAG_DEFAULT: Default indicating that there is nothing special
 *   to note about a color palette.
 * @HB_OT_COLOR_PALETTE_FLAG_USABLE_WITH_LIGHT_BACKGROUND: Flag indicating that the color
 *   palette is appropriate to use when displaying the font on a light background such as white.
 * @HB_OT_COLOR_PALETTE_FLAG_USABLE_WITH_DARK_BACKGROUND: Flag indicating that the color
 *   palette is appropriate to use when displaying the font on a dark background such as black.
 *
 * Flags that describe the properties of color palette.
 *
 * Since: 2.1.0
 */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା { /*< flags >*/
  HB_OT_COLOR_PALETTE_FLAG_DEFAULT			= 0x00000000u,
  HB_OT_COLOR_PALETTE_FLAG_USABLE_WITH_LIGHT_BACKGROUND	= 0x00000001u,
  HB_OT_COLOR_PALETTE_FLAG_USABLE_WITH_DARK_BACKGROUND	= 0x00000002u
} hb_ot_color_palette_flags_t;

HB_EXTERN hb_ot_color_palette_flags_t
hb_ot_color_palette_get_flags (hb_face_t *face,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_index);

HB_EXTERN unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_ot_color_palette_get_colors (hb_face_t    *face,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  palette_index,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  start_offset,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *color_ଗଣନା,  /* IN/OUT.  May be NULL. */
				hb_color_t   *colors        /* OUT.     May be NULL. */);


/*
 * Color layers.
 */

HB_EXTERN hb_bool_t
hb_ot_color_has_layers (hb_face_t *face);

/**
 * hb_ot_color_layer_t:
 * @glyph: the glyph ID of the layer
 * @color_index: the palette color index of the layer
 *
 * Pairs of glyph and color index.
 *
 * A color index of 0xFFFF does not refer to a palette
 * color, but indicates that the foreground color should
 * be used.
 *
 * Since: 2.1.0
 **/
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା hb_ot_color_layer_t {
  hb_codepoint_t glyph;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା   color_index;
} hb_ot_color_layer_t;

HB_EXTERN unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_ot_color_glyph_get_layers (hb_face_t           *face,
			      hb_codepoint_t       glyph,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା         start_offset,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        *layer_ଗଣନା, /* IN/OUT.  May be NULL. */
			      hb_ot_color_layer_t *layers /* OUT.     May be NULL. */);

/* COLRv1 */

HB_EXTERN hb_bool_t
hb_ot_color_has_paint (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_ot_color_glyph_has_paint (hb_face_t      *face,
                             hb_codepoint_t  glyph);

/*
 * SVG
 */

HB_EXTERN hb_bool_t
hb_ot_color_has_svg (hb_face_t *face);

HB_EXTERN unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_ot_color_get_svg_document_ଗଣନା (hb_face_t *face);

HB_EXTERN hb_bool_t
hb_ot_color_glyph_get_svg_document_index (hb_face_t      *face,
                                          hb_codepoint_t  glyph,
                                          unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା   *svg_document_index /* OUT */);

HB_EXTERN hb_bool_t
hb_ot_color_get_svg_document_glyph_range (hb_face_t      *face,
                                          unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    svg_document_index,
                                          hb_codepoint_t *start_glyph_id, /* OUT */
                                          hb_codepoint_t *end_glyph_id /* OUT */);

HB_EXTERN hb_blob_t *
hb_ot_color_glyph_reference_svg (hb_face_t *face, hb_codepoint_t glyph);

/*
 * PNG: CBDT or sbix
 */

HB_EXTERN hb_bool_t
hb_ot_color_has_png (hb_face_t *face);

HB_EXTERN hb_blob_t *
hb_ot_color_glyph_reference_png (hb_font_t *font, hb_codepoint_t glyph);


HB_END_DECLS

#endif /* HB_OT_COLOR_H */
