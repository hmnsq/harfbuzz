/*
 * Copyright © 2013  Google, Inc.
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

#ఒకవేళ !defined(HB_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEPRECATED_H
#నిర్వచించు HB_DEPRECATED_H

#అన్తర్భావ "hb-common.h"
#అన్తర్భావ "hb-unicode.h"
#అన్తర్భావ "hb-font.h"
#అన్తర్భావ "hb-set.h"


/**
 * SECTION:hb-deprecated
 * @title: hb-deprecated
 * @short_description: Deprecated API
 * @include: hb.h
 *
 * These API have been deprecated in favor of newer API, or because they
 * were deemed unnecessary.
 **/


HB_BEGIN_DECLS

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED


/**
 * HB_SCRIPT_CANADIAN_ABORIGINAL:
 *
 * Use #HB_SCRIPT_CANADIAN_SYLLABICS instead.
 *
 * Deprecated: 0.9.20
 */
#నిర్వచించు HB_SCRIPT_CANADIAN_ABORIGINAL		HB_SCRIPT_CANADIAN_SYLLABICS

/**
 * HB_BUFFER_FLAGS_DEFAULT:
 *
 * Use #HB_BUFFER_FLAG_DEFAULT instead.
 *
 * Deprecated: 0.9.20
 */
#నిర్వచించు HB_బఫరు_FLAGS_DEFAULT			HB_బఫరు_FLAG_DEFAULT
/**
 * HB_BUFFER_SERIALIZE_FLAGS_DEFAULT:
 *
 * Use #HB_BUFFER_SERIALIZE_FLAG_DEFAULT instead.
 *
 * Deprecated: 0.9.20
 */
#నిర్వచించు HB_బఫరు_SERIALIZE_FLAGS_DEFAULT	HB_బఫరు_SERIALIZE_FLAG_DEFAULT

/**
 * hb_font_get_glyph_func_t:
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
 * font, with an optional variation selector.
 *
 * Return value: `true` if data found, `false` otherwise
 * Deprecated: 1.2.3
 *
 **/
రకనిర్వచనం hb_bool_t (*hb_font_get_glyph_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
					       hb_codepoint_t unicode, hb_codepoint_t variation_selector,
					       hb_codepoint_t *glyph,
					       శూన్య *user_దత్తాంశం);

HB_DEPRECATED_FOR (hb_font_funcs_సమితి_nominal_glyph_func and hb_font_funcs_సమితి_variation_glyph_func)
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_func (hb_font_funcs_t *ffuncs,
			      hb_font_get_glyph_func_t func,
			      శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/* https://github.com/harfbuzz/harfbuzz/pull/4207 */
/**
 * HB_UNICODE_COMBINING_CLASS_CCC133:
 *
 * [Tibetan]
 *
 * Deprecated: 7.2.0
 **/
#నిర్వచించు HB_UNICODE_COMBINING_CLASS_CCC133 133

/**
 * hb_unicode_eastasian_width_func_t:
 * @ufuncs: A Unicode-functions structure
 * @unicode: The code point to query
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_unicode_funcs_t structure.
 *
 * Deprecated: 2.0.0
 */
రకనిర్వచనం గుర్తులేని పూర్ణాంకం			(*hb_unicode_eastasian_వెడల్పు_func_t)	(hb_unicode_funcs_t *ufuncs,
										 hb_codepoint_t      unicode,
										 శూన్య               *user_దత్తాంశం);

/**
 * hb_unicode_funcs_set_eastasian_width_func:
 * @ufuncs: a Unicode-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_unicode_eastasian_width_func_t.
 *
 * Since: 0.9.2
 * Deprecated: 2.0.0
 **/
HB_EXTERN HB_DEPRECATED శూన్య
hb_unicode_funcs_సమితి_eastasian_వెడల్పు_func (hb_unicode_funcs_t *ufuncs,
					   hb_unicode_eastasian_వెడల్పు_func_t func,
					   శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_unicode_eastasian_width:
 * @ufuncs: a Unicode-function structure
 * @unicode: The code point to query
 *
 * Don't use. Not used by HarfBuzz.
 *
 * Since: 0.9.2
 * Deprecated: 2.0.0
 **/
HB_EXTERN HB_DEPRECATED గుర్తులేని పూర్ణాంకం
hb_unicode_eastasian_వెడల్పు (hb_unicode_funcs_t *ufuncs,
			    hb_codepoint_t unicode);


/**
 * hb_unicode_decompose_compatibility_func_t:
 * @ufuncs: a Unicode function structure
 * @u: codepoint to decompose
 * @decomposed: address of codepoint array (of length #HB_UNICODE_MAX_DECOMPOSITION_LEN) to write decomposition into
 * @user_data: user data pointer as passed to hb_unicode_funcs_set_decompose_compatibility_func()
 *
 * Fully decompose @u to its Unicode compatibility decomposition. The codepoints of the decomposition will be written to @decomposed.
 * The complete length of the decomposition will be returned.
 *
 * If @u has no compatibility decomposition, zero should be returned.
 *
 * The Unicode standard guarantees that a buffer of length #HB_UNICODE_MAX_DECOMPOSITION_LEN codepoints will always be sufficient for any
 * compatibility decomposition plus an terminating value of 0.  Consequently, @decompose must be allocated by the caller to be at least this length.  Implementations
 * of this function type must ensure that they do not write past the provided array.
 *
 * Return value: number of codepoints in the full compatibility decomposition of @u, or 0 if no decomposition available.
 *
 * Deprecated: 2.0.0
 */
రకనిర్వచనం గుర్తులేని పూర్ణాంకం			(*hb_unicode_decompose_compatibility_func_t)	(hb_unicode_funcs_t *ufuncs,
											 hb_codepoint_t      u,
											 hb_codepoint_t     *decomposed,
											 శూన్య               *user_దత్తాంశం);

/**
 * HB_UNICODE_MAX_DECOMPOSITION_LEN:
 *
 * See Unicode 6.1 for details on the maximum decomposition length.
 *
 * Deprecated: 2.0.0
 */
#నిర్వచించు HB_UNICODE_MAX_DECOMPOSITION_LEN (18+1) /* codepoints */

/**
 * hb_unicode_funcs_set_decompose_compatibility_func:
 * @ufuncs: A Unicode-functions structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_unicode_decompose_compatibility_func_t.
 *
 * 
 *
 * Since: 0.9.2
 * Deprecated: 2.0.0
 **/
HB_EXTERN HB_DEPRECATED శూన్య
hb_unicode_funcs_సమితి_decompose_compatibility_func (hb_unicode_funcs_t *ufuncs,
						   hb_unicode_decompose_compatibility_func_t func,
						   శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

HB_EXTERN HB_DEPRECATED గుర్తులేని పూర్ణాంకం
hb_unicode_decompose_compatibility (hb_unicode_funcs_t *ufuncs,
				    hb_codepoint_t      u,
				    hb_codepoint_t     *decomposed);


/**
 * hb_font_get_glyph_v_kerning_func_t:
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * This method should retrieve the kerning-adjustment value for a glyph-pair in
 * the specified font, for vertical text segments.
 *
 **/
రకనిర్వచనం hb_font_get_glyph_kerning_func_t hb_font_get_glyph_v_kerning_func_t;

/**
 * hb_font_funcs_set_glyph_v_kerning_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_v_kerning_func_t.
 *
 * Since: 0.9.2
 * Deprecated: 2.0.0
 **/
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_v_kerning_func (hb_font_funcs_t *ffuncs,
					hb_font_get_glyph_v_kerning_func_t func,
					శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

HB_EXTERN hb_position_t
hb_font_get_glyph_v_kerning (hb_font_t *font,
			     hb_codepoint_t శిఖరం_glyph, hb_codepoint_t bottom_glyph);


/**
 * hb_font_get_glyph_shape_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @glyph: The glyph ID to query
 * @draw_funcs: The draw functions to send the shape data to
 * @draw_data: The data accompanying the draw functions
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * Since: 4.0.0
 * Deprecated: 7.0.0: Use #hb_font_draw_glyph_func_t instead
 **/
రకనిర్వచనం శూన్య (*hb_font_get_glyph_shape_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
						hb_codepoint_t glyph,
						hb_draw_funcs_t *draw_funcs, శూన్య *draw_దత్తాంశం,
						శూన్య *user_దత్తాంశం);

/**
 * hb_font_draw_glyph_func_t:
 * @font: #hb_font_t to work upon
 * @font_data: @font user data pointer
 * @glyph: The glyph ID to query
 * @draw_funcs: The draw functions to send the shape data to
 * @draw_data: The data accompanying the draw functions
 * @user_data: User data pointer passed by the caller
 *
 * A virtual method for the #hb_font_funcs_t of an #hb_font_t object.
 *
 * Since: 7.0.0
 * Deprecated: 11.2.0: Use hb_font_draw_glyph_func_or_fail_t instead.
 **/
రకనిర్వచనం శూన్య (*hb_font_draw_glyph_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
                                           hb_codepoint_t glyph,
                                           hb_draw_funcs_t *draw_funcs, శూన్య *draw_దత్తాంశం,
                                           శూన్య *user_దత్తాంశం);

/**
 * hb_font_paint_glyph_func_t:
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
 * Since: 7.0.0
 * Deprecated: 11.2.0: Use hb_font_paint_glyph_or_fail_func_t instead.
 */
రకనిర్వచనం hb_bool_t (*hb_font_paint_glyph_func_t) (hb_font_t *font, శూన్య *font_దత్తాంశం,
						 hb_codepoint_t glyph,
						 hb_paint_funcs_t *paint_funcs, శూన్య *paint_దత్తాంశం,
						 గుర్తులేని పూర్ణాంకం palette_సూచీ,
						 hb_color_t foreground,
						 శూన్య *user_దత్తాంశం);

/**
 * hb_font_funcs_set_glyph_shape_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_get_glyph_shape_func_t,
 * which is the same as #hb_font_draw_glyph_func_t.
 *
 * Since: 4.0.0
 * Deprecated: 7.0.0: Use hb_font_funcs_set_draw_glyph_func() instead
 **/
HB_DEPRECATED_FOR (hb_font_funcs_సమితి_draw_glyph_or_fail_func)
HB_EXTERN శూన్య
hb_font_funcs_సమితి_glyph_shape_func (hb_font_funcs_t *ffuncs,
				    hb_font_get_glyph_shape_func_t func,
				    శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_draw_glyph_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_font_draw_glyph_func_t.
 *
 * Since: 7.0.0
 * Deprecated: 11.2.0: Use hb_font_funcs_set_draw_glyph_or_fail_func instead.
 **/
HB_DEPRECATED_FOR (hb_font_funcs_సమితి_draw_glyph_or_fail_func)
HB_EXTERN శూన్య
hb_font_funcs_సమితి_draw_glyph_func (hb_font_funcs_t *ffuncs,
                                   hb_font_draw_glyph_func_t func,
                                   శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

/**
 * hb_font_funcs_set_paint_glyph_func:
 * @ffuncs: A font-function structure
 * @func: (closure user_data) (destroy destroy) (scope notified): The callback function to assign
 * @user_data: Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is no longer needed
 *
 * Sets the implementation function for #hb_font_paint_glyph_func_t.
 *
 * Since: 7.0.0
 * Deprecated: 11.2.0: Use hb_font_funcs_set_paint_glyph_or_fail_func() instead.
 */
HB_DEPRECATED_FOR (hb_font_funcs_సమితి_paint_glyph_or_fail_func)
HB_EXTERN శూన్య
hb_font_funcs_సమితి_paint_glyph_func (hb_font_funcs_t *ffuncs,
                                    hb_font_paint_glyph_func_t func,
                                    శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy);

HB_DEPRECATED_FOR (hb_font_draw_glyph_or_fail)
HB_EXTERN శూన్య
hb_font_get_glyph_shape (hb_font_t *font,
			 hb_codepoint_t glyph,
			 hb_draw_funcs_t *dfuncs, శూన్య *draw_దత్తాంశం);


/**
 * HB_AAT_LAYOUT_FEATURE_TYPE_CURISVE_CONNECTION:
 *
 * Use #HB_AAT_LAYOUT_FEATURE_TYPE_CURSIVE_CONNECTION instead.
 *
 * Deprecated: 8.3.0
 */
#నిర్వచించు HB_AAT_LAYOUT_FEATURE_TYPE_CURISVE_CONNECTION HB_AAT_LAYOUT_FEATURE_TYPE_CURSIVE_CONNECTION

HB_DEPRECATED_FOR (hb_font_create)
HB_EXTERN శూన్య
hb_font_సమితి_face (hb_font_t *font,
		  hb_face_t *face);

#అయితే_ముగింపు


HB_చివర_DECLS

#అయితే_ముగింపు /* HB_DEPRECATED_H */
