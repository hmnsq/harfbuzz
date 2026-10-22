/*
 * Copyright © 2009  Red Hat, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 */

#ఒకవేళ !defined(HB_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_FONT_H
#నిర్వచించు HB_FONT_H

#అన్తర్భావ "hb-common.h"
#అన్తర్భావ "hb-face.h"
#అన్తర్భావ "hb-draw.h"
#అన్తర్భావ "hb-paint.h"

HB_BEGIN_DECLS

/*
 * hb_font_funcs_t
 */

/**
 * hb_font_funcs_t:
 *
 * Data type containing a set of virtual methods used for
 * working on #hb_font_t font objects.
 *
 * HarfBuzz provides a lightweight default function for each of 
 * the methods in #hb_font_funcs_t. Client programs can implement
 * their own replacements for the individual font functions, as
 * needed, and replace the default by calling the setter for a
 * method.
 *
 **/
రకనిర్వచనం నిర్మాణం hb_font_funcs_t hb_font_funcs_t;

HB_EXTERN hb_font_funcs_t *
hb_font_funcs_create (శూన్య);

HB_EXTERN hb_font_funcs_t *
hb_font_funcs_get_ఖాళీలు (శూన్య);

HB_EXTERN hb_font_funcs_t *
hb_font_funcs_reference (hb_font_funcs_t *ffuncs);

HB_EXTERN శూన్య
hb_font_funcs_destroy (hb_font_funcs_t *ffuncs);

HB_EXTERN hb_bool_t
hb_font_funcs_సమితి_user_దత్తాంశం (hb_font_funcs_t    *ffuncs,
			     hb_user_దత్తాంశం_కీలకం_t *కీలకం_2,
			     శూన్య *              దత్తాంశం,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace);


HB_EXTERN శూన్య *
hb_font_funcs_get_user_దత్తాంశం (స్థిరరాశి hb_font_funcs_t *ffuncs,
			     hb_user_దత్తాంశం_కీలకం_t    *కీలకం_2);


HB_EXTERN శూన్య
hb_font_funcs_make_immutable (hb_font_funcs_t *ffuncs);

HB_EXTERN hb_bool_t
hb_font_funcs_is_immutable (hb_font_funcs_t *ffuncs);


/* font extents */

/**
 * hb_font_extents_t:
 * @ascender: The height of typographic ascenders.
 * @descender: The depth of typographic descenders.
 * @line_gap: The suggested line-spacing gap.
 *
 * Font-wide extent values, measured in scaled units.
 *
 * Note that typically @ascender is positive and @descender
 * negative, in coordinate systems that grow up.
 **/
రకనిర్వచనం నిర్మాణం hb_font_extents_t {
  hb_position_t ascender;
  hb_position_t descender;
  hb_position_t పంక్తి_gap;
  /*< private >*/
  hb_position_t reserved9;
  hb_position_t reserved8;
  hb_position_t reserved7;
  hb_position_t reserved6;
  hb_position_t reserved5;
  hb_position_t reserved4;
  hb_position_t reserved3;
  hb_position_t reserved2;
  hb_position_t reserved1;
} hb_font_extents_t;

/* func types */

/**
 * hb_font_get_font_extents_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @extents: (out): The font extents retrieved
 * @user_data: User data pointer passed by the caller
 *
 * This method should retrieve the extents for a font.
 *
 **/
రకనిర్వచనం hb_bool_t (*hb_font_get_font_extents_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
						       hb_font_extents_t *extents,
						       శూన్య *user_దత్తాంశం);

/**
 * hb_font_get_font_h_extents_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the extents for a font, for horizontal-direction
 * text segments. Extents must be returned in an #hb_glyph_extents output
 * parameter.
 * 
 **/
రకనిర్వచనం hb_font_get_font_extents_func_t hb_font_get_font_h_extents_func_t;

/**
 * hb_font_get_font_v_extents_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the extents for a font, for vertical-direction
 * text segments. Extents must be returned in an #hb_glyph_extents output
 * parameter.
 * 
 **/
రకనిర్వచనం hb_font_get_font_extents_func_t hb_font_get_font_v_extents_func_t;


/**
 * hb_font_get_nominal_glyph_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @unicode: The Unicode code point to query
 * @glyph: (out): The glyph ID retrieved
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the nominal glyph ID for a specified Unicode code
 * point. Glyph IDs must be returned in a #hb_codepoint_t output parameter.
 * 
 * Return value: `true` if data found, `false` otherwise
 *
 **/
రకనిర్వచనం hb_bool_t (*hb_font_get_nominal_glyph_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
						       hb_codepoint_t unicode,
						       hb_codepoint_t *glyph,
						       శూన్య *user_దత్తాంశం);

/**
 * hb_font_get_variation_glyph_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @unicode: The Unicode code point to query
 * @variation_selector: The  variation-selector code point to query
 * @glyph: (out): The glyph ID retrieved
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the glyph ID for a specified Unicode code point
 * followed by a specified Variation Selector code point. Glyph IDs must be
 * returned in a #hb_codepoint_t output parameter.
 * 
 * Return value: `true` if data found, `false` otherwise
 *
 **/
రకనిర్వచనం hb_bool_t (*hb_font_get_variation_glyph_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
							 hb_codepoint_t unicode, hb_codepoint_t variation_selector,
							 hb_codepoint_t *glyph,
							 శూన్య *user_దత్తాంశం);


/**
 * hb_font_get_nominal_glyphs_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @count: number of code points to query
 * @first_unicode: The first Unicode code point to query
 * @unicode_stride: The stride between successive code points
 * @first_glyph: (out): The first glyph ID retrieved
 * @glyph_stride: The stride between successive glyph IDs
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the nominal glyph IDs for a sequence of
 * Unicode code points. Glyph IDs must be returned in a #hb_codepoint_t
 * output parameter.
 *
 * Return value: the number of code points processed
 * 
 **/
రకనిర్వచనం గుర్తులేని పూర్ణాంకం (*hb_font_get_nominal_glyphs_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
							   గుర్తులేని పూర్ణాంకం లెక్క,
							   స్థిరరాశి hb_codepoint_t *మొదటి_unicode,
							   గుర్తులేని పూర్ణాంకం unicode_stride,
							   hb_codepoint_t *మొదటి_glyph,
							   గుర్తులేని పూర్ణాంకం glyph_stride,
							   శూన్య *user_దత్తాంశం);

/**
 * hb_font_get_glyph_advance_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @glyph: The glyph ID to query
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the advance for a specified glyph. The
 * method must return an #hb_position_t.
 * 
 * Return value: The advance of @glyph within @font
 *
 **/
రకనిర్వచనం hb_position_t (*hb_font_get_glyph_advance_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
							   hb_codepoint_t glyph,
							   శూన్య *user_దత్తాంశం);

/**
 * hb_font_get_glyph_h_advance_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the advance for a specified glyph, in
 * horizontal-direction text segments. Advances must be returned in
 * an #hb_position_t output parameter.
 * 
 **/
రకనిర్వచనం hb_font_get_glyph_advance_func_t hb_font_get_glyph_h_advance_func_t;

/**
 * hb_font_get_glyph_v_advance_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the advance for a specified glyph, in
 * vertical-direction text segments. Advances must be returned in
 * an #hb_position_t output parameter.
 * 
 **/
రకనిర్వచనం hb_font_get_glyph_advance_func_t hb_font_get_glyph_v_advance_func_t;

/**
 * hb_font_get_glyph_advances_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @count: The number of glyph IDs in the sequence queried
 * @first_glyph: The first glyph ID to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_advance: (out): The first advance retrieved
 * @advance_stride: The stride between successive advances
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the advances for a sequence of glyphs.
 * 
 **/
రకనిర్వచనం శూన్య (*hb_font_get_glyph_advances_func_t) (hb_font_t* font, శూన్య* font_దత్తాంశం,
						   గుర్తులేని పూర్ణాంకం లెక్క,
						   స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
						   గుర్తులేని glyph_stride,
						   hb_position_t *మొదటి_advance,
						   గుర్తులేని advance_stride,
						   శూన్య *user_దత్తాంశం);

/**
 * hb_font_get_glyph_h_advances_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the advances for a sequence of glyphs, in
 * horizontal-direction text segments.
 * 
 **/
రకనిర్వచనం hb_font_get_glyph_advances_func_t hb_font_get_glyph_h_advances_func_t;

/**
 * hb_font_get_glyph_v_advances_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the advances for a sequence of glyphs, in
 * vertical-direction text segments.
 * 
 **/
రకనిర్వచనం hb_font_get_glyph_advances_func_t hb_font_get_glyph_v_advances_func_t;

/**
 * hb_font_get_glyph_origin_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @glyph: The glyph ID to query
 * @x: (out): The X coordinate of the origin
 * @y: (out): The Y coordinate of the origin
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the (X,Y) coordinates (in scaled units) of the
 * origin for a glyph. Each coordinate must be returned in an #hb_position_t
 * output parameter.
 *
 * Return value: `true` if data found, `false` otherwise
 * 
 **/
రకనిర్వచనం hb_bool_t (*hb_font_get_glyph_origin_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
						      hb_codepoint_t glyph,
						      hb_position_t *x, hb_position_t *y,
						      శూన్య *user_దత్తాంశం);

/**
 * hb_font_get_glyph_h_origin_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the (X,Y) coordinates (in scaled units) of the
 * origin for a glyph, for horizontal-direction text segments. Each
 * coordinate must be returned in an #hb_position_t output parameter.
 * 
 **/
రకనిర్వచనం hb_font_get_glyph_origin_func_t hb_font_get_glyph_h_origin_func_t;

/**
 * hb_font_get_glyph_v_origin_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the (X,Y) coordinates (in scaled units) of the
 * origin for a glyph, for vertical-direction text segments. Each coordinate
 * must be returned in an #hb_position_t output parameter.
 * 
 **/
రకనిర్వచనం hb_font_get_glyph_origin_func_t hb_font_get_glyph_v_origin_func_t;

/**
 * hb_font_get_glyph_origins_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @first_glyph: The first glyph ID to query
 * @count: number of glyphs to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_x: (out): The first origin X coordinate retrieved
 * @x_stride: The stride between successive origin X coordinates
 * @first_y: (out): The first origin Y coordinate retrieved
 * @y_stride: The stride between successive origin Y coordinates
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the (X,Y) coordinates (in scaled units) of the
 * origin for each requested glyph. Each coordinate value must be returned in
 * an #hb_position_t in the two output parameters.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 11.3.0
 **/
రకనిర్వచనం hb_bool_t (*hb_font_get_glyph_origins_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
						       గుర్తులేని పూర్ణాంకం లెక్క,
						       స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
						       గుర్తులేని glyph_stride,
						       hb_position_t *మొదటి_x,
						       గుర్తులేని x_stride,
						       hb_position_t *మొదటి_y,
						       గుర్తులేని y_stride,
						       శూన్య *user_దత్తాంశం);

/**
 * hb_font_get_glyph_h_origins_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the (X,Y) coordinates (in scaled units) of the
 * origin for requested glyph, for horizontal-direction text segments. Each
 * coordinate must be returned in a the x/y #hb_position_t output parameters.
 *
 * Since: 11.3.0
 **/
రకనిర్వచనం hb_font_get_glyph_origins_func_t hb_font_get_glyph_h_origins_func_t;

/**
 * hb_font_get_glyph_v_origins_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the (X,Y) coordinates (in scaled units) of the
 * origin for requested glyph, for vertical-direction text segments. Each
 * coordinate must be returned in a the x/y #hb_position_t output parameters.
 *
 * Since: 11.3.0
 **/
రకనిర్వచనం hb_font_get_glyph_origins_func_t hb_font_get_glyph_v_origins_func_t;

/**
 * hb_font_get_glyph_kerning_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @first_glyph: The glyph ID of the first glyph in the glyph pair
 * @second_glyph: The glyph ID of the second glyph in the glyph pair
 * @user_data: User data pointer passed by the caller
 *
 * This method should retrieve the kerning-adjustment value for a glyph-pair in
 * the specified font, for horizontal text segments.
 *
 **/
రకనిర్వచనం hb_position_t (*hb_font_get_glyph_kerning_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
							   hb_codepoint_t మొదటి_glyph, hb_codepoint_t రెండవ_glyph,
							   శూన్య *user_దత్తాంశం);
/**
 * hb_font_get_glyph_h_kerning_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the kerning-adjustment value for a glyph-pair in
 * the specified font, for horizontal text segments.
 *
 **/
రకనిర్వచనం hb_font_get_glyph_kerning_func_t hb_font_get_glyph_h_kerning_func_t;


/**
 * hb_font_get_glyph_extents_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @glyph: The glyph ID to query
 * @extents: (out): The #hb_glyph_extents_t retrieved
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the extents for a specified glyph. Extents must be 
 * returned in an #hb_glyph_extents output parameter.
 *
 * Return value: `true` if data found, `false` otherwise
 * 
 **/
రకనిర్వచనం hb_bool_t (*hb_font_get_glyph_extents_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
						       hb_codepoint_t glyph,
						       hb_glyph_extents_t *extents,
						       శూన్య *user_దత్తాంశం);

/**
 * hb_font_get_glyph_contour_point_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @glyph: The glyph ID to query
 * @point_index: The contour-point index to query
 * @x: (out): The X value retrieved for the contour point
 * @y: (out): The Y value retrieved for the contour point
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the (X,Y) coordinates (in scaled units) for a
 * specified contour point in a glyph. Each coordinate must be returned as
 * an #hb_position_t output parameter.
 * 
 * Return value: `true` if data found, `false` otherwise
 *
 **/
రకనిర్వచనం hb_bool_t (*hb_font_get_glyph_contour_బిందువు_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
							     hb_codepoint_t glyph, గుర్తులేని పూర్ణాంకం బిందువు_సూచీ,
							     hb_position_t *x, hb_position_t *y,
							     శూన్య *user_దత్తాంశం);


/**
 * hb_font_get_glyph_name_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @glyph: The glyph ID to query
 * @name: (out) (array length=size): Name string retrieved for the glyph ID
 * @size: Length of the glyph-name string retrieved
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the glyph name that corresponds to a
 * glyph ID. The name should be returned in a string output parameter.
 * 
 * Return value: `true` if data found, `false` otherwise
 *
 **/
రకనిర్వచనం hb_bool_t (*hb_font_get_glyph_name_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
						    hb_codepoint_t glyph,
						    అక్షరం *name, గుర్తులేని పూర్ణాంకం కొలత,
						    శూన్య *user_దత్తాంశం);

/**
 * hb_font_get_glyph_from_name_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @name: (array length=len): The name string to query
 * @len: The length of the name queried
 * @glyph: (out): The glyph ID retrieved
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the glyph ID that corresponds to a glyph-name
 * string. 
 * 
 * Return value: `true` if data found, `false` otherwise
 *
 **/
రకనిర్వచనం hb_bool_t (*hb_font_get_glyph_from_name_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
							 స్థిరరాశి అక్షరం *name, పూర్ణాంకం len, /* -1 means nul-terminated */
							 hb_codepoint_t *glyph,
							 శూన్య *user_దత్తాంశం);

/**
 * hb_font_draw_glyph_or_fail_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @glyph: The glyph ID to query
 * @draw_funcs: The draw functions to send the shape data to
 * @draw_data: The data accompanying the draw functions
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * Return value: `true` if glyph was drawn, `false` otherwise
 *
 * Since: 11.2.0
 **/
రకనిర్వచనం hb_bool_t (*hb_font_draw_glyph_or_fail_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
							hb_codepoint_t glyph,
							hb_draw_funcs_t *draw_funcs, శూన్య *draw_దత్తాంశం,
							శూన్య *user_దత్తాంశం);

/**
 * hb_font_paint_glyph_or_fail_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @glyph: The glyph ID to query
 * @paint_funcs: The paint functions to use
 * @paint_data: The data accompanying the paint functions
 * @palette_index: The color palette to use
 * @foreground: The foreground color
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * Return value: `true` if glyph was painted, `false` otherwise
 *
 * Since: 11.2.0
 */
రకనిర్వచనం hb_bool_t (*hb_font_paint_glyph_or_fail_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
							 hb_codepoint_t glyph,
							 hb_paint_funcs_t *paint_funcs, శూన్య *paint_దత్తాంశం,
							 గుర్తులేని పూర్ణాంకం palette_సూచీ,
							 hb_color_t foreground,
							 శూన్య *user_దత్తాంశం);

/* func setters */

/**
 * hb_font_funcs_set_font_h_extents_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_font_h_extents_func_t.
 *
 * Since: 1.1.2
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_font_h_extents_func (hb_font_funcs_t *ffuncs,
				       hb_font_get_font_h_extents_func_t func,
				       శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_font_v_extents_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_font_v_extents_func_t.
 *
 * Since: 1.1.2
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_font_v_extents_func (hb_font_funcs_t *ffuncs,
				       hb_font_get_font_v_extents_func_t func,
				       శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_nominal_glyph_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_nominal_glyph_func_t.
 *
 * Since: 1.2.3
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_nominal_glyph_func (hb_font_funcs_t *ffuncs,
				      hb_font_get_nominal_glyph_func_t func,
				      శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_nominal_glyphs_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_nominal_glyphs_func_t.
 *
 * Since: 2.0.0
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_nominal_glyphs_func (hb_font_funcs_t *ffuncs,
				       hb_font_get_nominal_glyphs_func_t func,
				       శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_variation_glyph_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_variation_glyph_func_t.
 *
 * Since: 1.2.3
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_variation_glyph_func (hb_font_funcs_t *ffuncs,
					hb_font_get_variation_glyph_func_t func,
					శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_h_advance_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_h_advance_func_t.
 *
 * Since: 0.9.2
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_h_advance_func (hb_font_funcs_t *ffuncs,
					hb_font_get_glyph_h_advance_func_t func,
					శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_v_advance_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_v_advance_func_t.
 *
 * Since: 0.9.2
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_v_advance_func (hb_font_funcs_t *ffuncs,
					hb_font_get_glyph_v_advance_func_t func,
					శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_h_advances_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_h_advances_func_t.
 *
 * Since: 1.8.6
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_h_advances_func (hb_font_funcs_t *ffuncs,
					hb_font_get_glyph_h_advances_func_t func,
					శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_v_advances_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_v_advances_func_t.
 *
 * Since: 1.8.6
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_v_advances_func (hb_font_funcs_t *ffuncs,
					hb_font_get_glyph_v_advances_func_t func,
					శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_h_origin_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_h_origin_func_t.
 *
 * Since: 0.9.2
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_h_origin_func (hb_font_funcs_t *ffuncs,
				       hb_font_get_glyph_h_origin_func_t func,
				       శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_v_origin_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_v_origin_func_t.
 *
 * Since: 0.9.2
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_v_origin_func (hb_font_funcs_t *ffuncs,
				       hb_font_get_glyph_v_origin_func_t func,
				       శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_h_origins_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_h_origins_func_t.
 *
 * Since: 11.3.0
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_h_origins_func (hb_font_funcs_t *ffuncs,
					hb_font_get_glyph_h_origins_func_t func,
					శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_v_origins_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_v_origins_func_t.
 *
 * Since: 11.3.0
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_v_origins_func (hb_font_funcs_t *ffuncs,
					hb_font_get_glyph_v_origins_func_t func,
					శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_h_kerning_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_h_kerning_func_t.
 *
 * Since: 0.9.2
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_h_kerning_func (hb_font_funcs_t *ffuncs,
					hb_font_get_glyph_h_kerning_func_t func,
					శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_extents_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_extents_func_t.
 *
 * Since: 0.9.2
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_extents_func (hb_font_funcs_t *ffuncs,
				      hb_font_get_glyph_extents_func_t func,
				      శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_contour_point_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_contour_point_func_t.
 *
 * Since: 0.9.2
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_contour_బిందువు_func (hb_font_funcs_t *ffuncs,
					    hb_font_get_glyph_contour_బిందువు_func_t func,
					    శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_name_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_name_func_t.
 *
 * Since: 0.9.2
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_name_func (hb_font_funcs_t *ffuncs,
				   hb_font_get_glyph_name_func_t func,
				   శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_glyph_from_name_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_from_name_func_t.
 *
 * Since: 0.9.2
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_from_name_func (hb_font_funcs_t *ffuncs,
					hb_font_get_glyph_from_name_func_t func,
					శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_draw_glyph_or_fail_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_draw_glyph_or_fail_func_t.
 *
 * Since: 11.2.0
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_draw_glyph_or_fail_func (hb_font_funcs_t *ffuncs,
					   hb_font_draw_glyph_or_fail_func_t func,
					   శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_paint_glyph_or_fail_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is no longer needed
 *
 * Sets the implementation function for #hb_font_paint_glyph_or_fail_func_t.
 *
 * Since: 11.2.0
 */
HB_EXTERN శూన్య
hb_font_funcs_సమితి_paint_glyph_or_fail_func (hb_font_funcs_t *ffuncs,
					    hb_font_paint_glyph_or_fail_func_t func,
					    శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/* func dispatch */

HB_EXTERN hb_bool_t
hb_font_get_h_extents (hb_font_t *font,
		       hb_font_extents_t *extents);
HB_EXTERN hb_bool_t
hb_font_get_v_extents (hb_font_t *font,
		       hb_font_extents_t *extents);

HB_EXTERN hb_bool_t
hb_font_get_nominal_glyph (hb_font_t *font,
			   hb_codepoint_t unicode,
			   hb_codepoint_t *glyph);
HB_EXTERN hb_bool_t
hb_font_get_variation_glyph (hb_font_t *font,
			     hb_codepoint_t unicode, hb_codepoint_t variation_selector,
			     hb_codepoint_t *glyph);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_font_get_nominal_glyphs (hb_font_t *font,
			    గుర్తులేని పూర్ణాంకం లెక్క,
			    స్థిరరాశి hb_codepoint_t *మొదటి_unicode,
			    గుర్తులేని పూర్ణాంకం unicode_stride,
			    hb_codepoint_t *మొదటి_glyph,
			    గుర్తులేని పూర్ణాంకం glyph_stride);

HB_EXTERN hb_position_t
hb_font_get_glyph_h_advance (hb_font_t *font,
			     hb_codepoint_t glyph);
HB_EXTERN hb_position_t
hb_font_get_glyph_v_advance (hb_font_t *font,
			     hb_codepoint_t glyph);

HB_EXTERN శూన్య
hb_font_get_glyph_h_advances (hb_font_t* font,
			      గుర్తులేని పూర్ణాంకం లెక్క,
			      స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			      గుర్తులేని glyph_stride,
			      hb_position_t *మొదటి_advance,
			      గుర్తులేని advance_stride);
HB_EXTERN శూన్య
hb_font_get_glyph_v_advances (hb_font_t* font,
			      గుర్తులేని పూర్ణాంకం లెక్క,
			      స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			      గుర్తులేని glyph_stride,
			      hb_position_t *మొదటి_advance,
			      గుర్తులేని advance_stride);

HB_EXTERN hb_bool_t
hb_font_get_glyph_h_origin (hb_font_t *font,
			    hb_codepoint_t glyph,
			    hb_position_t *x, hb_position_t *y);
HB_EXTERN hb_bool_t
hb_font_get_glyph_v_origin (hb_font_t *font,
			    hb_codepoint_t glyph,
			    hb_position_t *x, hb_position_t *y);

HB_EXTERN hb_bool_t
hb_font_get_glyph_h_origins (hb_font_t *font,
			     గుర్తులేని పూర్ణాంకం లెక్క,
			     స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			     గుర్తులేని glyph_stride,
			     hb_position_t *మొదటి_x,
			     గుర్తులేని x_stride,
			     hb_position_t *మొదటి_y,
			     గుర్తులేని y_stride);

HB_EXTERN hb_bool_t
hb_font_get_glyph_v_origins (hb_font_t *font,
			     గుర్తులేని పూర్ణాంకం లెక్క,
			     స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			     గుర్తులేని glyph_stride,
			     hb_position_t *మొదటి_x,
			     గుర్తులేని x_stride,
			     hb_position_t *మొదటి_y,
			     గుర్తులేని y_stride);

HB_EXTERN hb_position_t
hb_font_get_glyph_h_kerning (hb_font_t *font,
			     hb_codepoint_t ఎడమ_glyph, hb_codepoint_t కుడి_glyph);

HB_EXTERN hb_bool_t
hb_font_get_glyph_extents (hb_font_t *font,
			   hb_codepoint_t glyph,
			   hb_glyph_extents_t *extents);

HB_EXTERN hb_bool_t
hb_font_get_glyph_contour_బిందువు (hb_font_t *font,
				 hb_codepoint_t glyph, గుర్తులేని పూర్ణాంకం బిందువు_సూచీ,
				 hb_position_t *x, hb_position_t *y);

HB_EXTERN hb_bool_t
hb_font_get_glyph_name (hb_font_t *font,
			hb_codepoint_t glyph,
			అక్షరం *name, గుర్తులేని పూర్ణాంకం కొలత);
HB_EXTERN hb_bool_t
hb_font_get_glyph_from_name (hb_font_t *font,
			     స్థిరరాశి అక్షరం *name, పూర్ణాంకం len, /* -1 means nul-terminated */
			     hb_codepoint_t *glyph);

HB_EXTERN hb_bool_t
hb_font_draw_glyph_or_fail (hb_font_t *font,
			    hb_codepoint_t glyph,
			    hb_draw_funcs_t *dfuncs, శూన్య *draw_దత్తాంశం);

HB_EXTERN hb_bool_t
hb_font_paint_glyph_or_fail (hb_font_t *font,
			     hb_codepoint_t glyph,
			     hb_paint_funcs_t *pfuncs, శూన్య *paint_దత్తాంశం,
			     గుర్తులేని పూర్ణాంకం palette_సూచీ,
			     hb_color_t foreground);

/* high-level funcs, with fallback */

/* Calls either hb_font_get_nominal_glyph() if variation_selector is 0,
 * otherwise calls hb_font_get_variation_glyph(). */
HB_EXTERN hb_bool_t
hb_font_get_glyph (hb_font_t *font,
		   hb_codepoint_t unicode, hb_codepoint_t variation_selector,
		   hb_codepoint_t *glyph);

HB_EXTERN శూన్య
hb_font_get_extents_for_direction (hb_font_t *font,
				   hb_direction_t direction,
				   hb_font_extents_t *extents);
HB_EXTERN శూన్య
hb_font_get_glyph_advance_for_direction (hb_font_t *font,
					 hb_codepoint_t glyph,
					 hb_direction_t direction,
					 hb_position_t *x, hb_position_t *y);
HB_EXTERN శూన్య
hb_font_get_glyph_advances_for_direction (hb_font_t* font,
					  hb_direction_t direction,
					  గుర్తులేని పూర్ణాంకం లెక్క,
					  స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
					  గుర్తులేని glyph_stride,
					  hb_position_t *మొదటి_advance,
					  గుర్తులేని advance_stride);
HB_EXTERN శూన్య
hb_font_get_glyph_origin_for_direction (hb_font_t *font,
					hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y);
HB_EXTERN శూన్య
hb_font_కూడు_glyph_origin_for_direction (hb_font_t *font,
					hb_codepoint_t glyph,
					hb_direction_t direction,
					hb_position_t *x, hb_position_t *y);
HB_EXTERN శూన్య
hb_font_వ్యవకలనం_glyph_origin_for_direction (hb_font_t *font,
					     hb_codepoint_t glyph,
					     hb_direction_t direction,
					     hb_position_t *x, hb_position_t *y);

HB_EXTERN శూన్య
hb_font_get_glyph_kerning_for_direction (hb_font_t *font,
					 hb_codepoint_t మొదటి_glyph, hb_codepoint_t రెండవ_glyph,
					 hb_direction_t direction,
					 hb_position_t *x, hb_position_t *y);

HB_EXTERN hb_bool_t
hb_font_get_glyph_extents_for_origin (hb_font_t *font,
				      hb_codepoint_t glyph,
				      hb_direction_t direction,
				      hb_glyph_extents_t *extents);

HB_EXTERN hb_bool_t
hb_font_get_glyph_contour_బిందువు_for_origin (hb_font_t *font,
					    hb_codepoint_t glyph, గుర్తులేని పూర్ణాంకం బిందువు_సూచీ,
					    hb_direction_t direction,
					    hb_position_t *x, hb_position_t *y);

/* Generates gidDDD if glyph has no name. */
HB_EXTERN శూన్య
hb_font_glyph_గమ్యం_string (hb_font_t *font,
			 hb_codepoint_t glyph,
			 అక్షరం *s, గుర్తులేని పూర్ణాంకం కొలత);
/* Parses gidDDD and uniUUUU strings automatically. */
HB_EXTERN hb_bool_t
hb_font_glyph_from_string (hb_font_t *font,
			   స్థిరరాశి అక్షరం *s, పూర్ణాంకం len, /* -1 means nul-terminated */
			   hb_codepoint_t *glyph);

/* Older alias for hb_font_draw_glyph_or_fail() with no return value. */
HB_EXTERN శూన్య
hb_font_draw_glyph (hb_font_t *font,
		    hb_codepoint_t glyph,
		    hb_draw_funcs_t *dfuncs, శూన్య *draw_దత్తాంశం);

/* Paints color glyph; if failed, draws outline glyph. */
HB_EXTERN శూన్య
hb_font_paint_glyph (hb_font_t *font,
		     hb_codepoint_t glyph,
		     hb_paint_funcs_t *pfuncs, శూన్య *paint_దత్తాంశం,
		     గుర్తులేని పూర్ణాంకం palette_సూచీ,
		     hb_color_t foreground);

/*
 * hb_font_t
 */

/* Fonts are very light-weight objects */

HB_EXTERN hb_font_t *
hb_font_create (hb_face_t *face);

HB_EXTERN hb_font_t *
hb_font_create_sub_font (hb_font_t *మాతృ);

HB_EXTERN hb_font_t *
hb_font_get_ఖాళీలు (శూన్య);

HB_EXTERN hb_font_t *
hb_font_reference (hb_font_t *font);

HB_EXTERN శూన్య
hb_font_destroy (hb_font_t *font);

HB_EXTERN hb_bool_t
hb_font_సమితి_user_దత్తాంశం (hb_font_t          *font,
		       hb_user_దత్తాంశం_కీలకం_t *కీలకం_2,
		       శూన్య *              దత్తాంశం,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace);


HB_EXTERN శూన్య *
hb_font_get_user_దత్తాంశం (స్థిరరాశి hb_font_t    *font,
		       hb_user_దత్తాంశం_కీలకం_t *కీలకం_2);

HB_EXTERN శూన్య
hb_font_make_immutable (hb_font_t *font);

HB_EXTERN hb_bool_t
hb_font_is_immutable (hb_font_t *font);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_font_get_serial (hb_font_t *font);

HB_EXTERN శూన్య
hb_font_మారింది (hb_font_t *font);

HB_EXTERN శూన్య
hb_font_సమితి_మాతృ (hb_font_t *font,
		    hb_font_t *మాతృ);

HB_EXTERN hb_font_t *
hb_font_get_మాతృ (hb_font_t *font);

HB_EXTERN hb_face_t *
hb_font_get_face (hb_font_t *font);


HB_EXTERN శూన్య
hb_font_సమితి_funcs (hb_font_t         *font,
		   hb_font_funcs_t   *klass,
		   శూన్య              *font_దత్తాంశం,
		   hb_destroy_func_t  destroy);

/* Be *very* careful with this function! */
HB_EXTERN శూన్య
hb_font_సమితి_funcs_దత్తాంశం (hb_font_t         *font,
			శూన్య              *font_దత్తాంశం,
			hb_destroy_func_t  destroy);

HB_EXTERN hb_bool_t
hb_font_సమితి_funcs_using (hb_font_t  *font,
			 స్థిరరాశి అక్షరం *name);

HB_EXTERN స్థిరరాశి అక్షరం **
hb_font_list_funcs (శూన్య);

HB_EXTERN శూన్య
hb_font_సమితి_scale (hb_font_t *font,
		   పూర్ణాంకం x_scale,
		   పూర్ణాంకం y_scale);

HB_EXTERN శూన్య
hb_font_get_scale (hb_font_t *font,
		   పూర్ణాంకం *x_scale,
		   పూర్ణాంకం *y_scale);

/*
 * A zero value means "no hinting in that direction"
 */
HB_EXTERN శూన్య
hb_font_సమితి_ppem (hb_font_t *font,
		  గుర్తులేని పూర్ణాంకం x_ppem,
		  గుర్తులేని పూర్ణాంకం y_ppem);

HB_EXTERN శూన్య
hb_font_get_ppem (hb_font_t *font,
		  గుర్తులేని పూర్ణాంకం *x_ppem,
		  గుర్తులేని పూర్ణాంకం *y_ppem);

/*
 * Point size per EM.  Used for optical-sizing in CoreText.
 * A value of zero means "not set".
 */
HB_EXTERN శూన్య
hb_font_సమితి_ptem (hb_font_t *font, దశాంశం ptem);

HB_EXTERN దశాంశం
hb_font_get_ptem (hb_font_t *font);

HB_EXTERN hb_bool_t
hb_font_is_synthetic (hb_font_t *font);

HB_EXTERN శూన్య
hb_font_సమితి_synthetic_bold (hb_font_t *font,
			    దశాంశం x_embolden, దశాంశం y_embolden,
			    hb_bool_t లోపల_ఉంచు);

HB_EXTERN శూన్య
hb_font_get_synthetic_bold (hb_font_t *font,
			    దశాంశం *x_embolden, దశాంశం *y_embolden,
			    hb_bool_t *లోపల_ఉంచు);

HB_EXTERN శూన్య
hb_font_సమితి_synthetic_slant (hb_font_t *font, దశాంశం slant);

HB_EXTERN దశాంశం
hb_font_get_synthetic_slant (hb_font_t *font);

HB_EXTERN శూన్య
hb_font_సమితి_variations (hb_font_t *font,
			స్థిరరాశి hb_variation_t *variations,
			గుర్తులేని పూర్ణాంకం variations_నిడివి);

HB_EXTERN శూన్య
hb_font_సమితి_variation (hb_font_t *font,
		       hb_ట్యాగ్_t ట్యాగ్_2,
		       దశాంశం    విలువ_2);

HB_EXTERN శూన్య
hb_font_సమితి_var_coords_design (hb_font_t *font,
			       స్థిరరాశి దశాంశం *coords,
			       గుర్తులేని పూర్ణాంకం coords_నిడివి);

HB_EXTERN స్థిరరాశి దశాంశం *
hb_font_get_var_coords_design (hb_font_t *font,
			       గుర్తులేని పూర్ణాంకం *నిడివి);

HB_EXTERN శూన్య
hb_font_సమితి_var_coords_normalized (hb_font_t *font,
				   స్థిరరాశి పూర్ణాంకం *coords, /* 2.14 normalized */
				   గుర్తులేని పూర్ణాంకం coords_నిడివి);

HB_EXTERN స్థిరరాశి పూర్ణాంకం *
hb_font_get_var_coords_normalized (hb_font_t *font,
				   గుర్తులేని పూర్ణాంకం *నిడివి);

/**
 * HB_FONT_NO_VAR_NAMED_INSTANCE:
 *
 * Constant signifying that a font does not have any
 * named-instance index set.  This is the default of
 * a font.
 *
 * Since: 7.0.0
 */
#నిర్వచించు HB_FONT_NO_VAR_NAMED_INSTANCE 0xFFFFFFFF

HB_EXTERN శూన్య
hb_font_సమితి_var_named_instance (hb_font_t *font,
				గుర్తులేని పూర్ణాంకం instance_సూచీ);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_font_get_var_named_instance (hb_font_t *font);

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_FONT_H */
