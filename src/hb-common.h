/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ఒకవేళ !defined(HB_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_COMMON_H
#నిర్వచించు HB_COMMON_H

#నిర్వచితం_కాకపోతే HB_EXTERN
#నిర్వచించు HB_EXTERN బాహ్య
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_BEGIN_DECLS
# ifdef __cplusplus
#  define HB_BEGIN_DECLS	బాహ్య "C" {
#  define HB_చివర_DECLS		}
# లేకపోతే /* !__cplusplus */
#  define HB_BEGIN_DECLS
#  define HB_చివర_DECLS
# endif /* !__cplusplus */
#అయితే_ముగింపు

#ఒకవేళ defined (_AIX)
#  include <sys/inttypes.h>
#లేదావేళ defined (_MSC_VER) && _MSC_VER < 1600
/* VS 2010 (_MSC_VER 1600) has stdint.h   */
రకనిర్వచనం __int8 int8_t;
రకనిర్వచనం గుర్తులేని __int8 uint8_t;
రకనిర్వచనం __int16 int16_t;
రకనిర్వచనం గుర్తులేని __int16 uint16_t;
రకనిర్వచనం __int32 int32_t;
రకనిర్వచనం గుర్తులేని __int32 uint32_t;
రకనిర్వచనం __int64 int64_t;
రకనిర్వచనం గుర్తులేని __int64 uint64_t;
#లేదావేళ defined (_MSC_VER) && _MSC_VER < 1800
/* VS 2013 (_MSC_VER 1800) has inttypes.h */
#  include <stdint.h>
#లేకపోతే
#  include <inttypes.h>
#అయితే_ముగింపు
#అన్తర్భావ <stddef.h>

#ఒకవేళ defined(__GNUC__) && ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#నిర్వచించు HB_DEPRECATED __attribute__((__deprecated__))
#లేదావేళ defined(_MSC_VER) && (_MSC_VER >= 1300)
#నిర్వచించు HB_DEPRECATED __declspec(deprecated)
#లేకపోతే
#నిర్వచించు HB_DEPRECATED
#అయితే_ముగింపు

#ఒకవేళ defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))
#నిర్వచించు HB_DEPRECATED_FOR(f) __attribute__((__deprecated__("Use '" #f "' instead")))
#లేదావేళ defined(MSC_నిండిన_VER) && (MSC_నిండిన_VER > 140050320)
#నిర్వచించు HB_DEPRECATED_FOR(f) __declspec(deprecated("is deprecated. Use '" #f "' instead"))
#లేకపోతే
#నిర్వచించు HB_DEPRECATED_FOR(f) HB_DEPRECATED
#అయితే_ముగింపు


HB_BEGIN_DECLS

/**
 * hb_bool_t:
 * 
 * Data type for booleans.
 *
 **/
రకనిర్వచనం పూర్ణాంకం hb_bool_t;

/**
 * HB_BUDGET_DEFAULT:
 *
 * Use the implementation's finite default work budget.
 *
 * Since: 14.5.0
 **/
#నిర్వచించు HB_BUDGET_DEFAULT ((int64_t) (-0x7FFFFFFFFFFFFFFF - 1))

/**
 * HB_BUDGET_UNLIMITED:
 *
 * Use an effectively unlimited work budget. Structural safety limits remain
 * active.
 *
 * Since: 14.5.0
 **/
#నిర్వచించు HB_BUDGET_UNLIMITED ((int64_t) 0x7FFFFFFFFFFFFFFF)

/**
 * hb_codepoint_t:
 * 
 * Data type for holding Unicode codepoints. Also
 * used to hold glyph IDs.
 *
 **/
రకనిర్వచనం uint32_t hb_codepoint_t;

/**
 * HB_CODEPOINT_INVALID:
 *
 * Unused #hb_codepoint_t value.
 *
 * Since: 8.0.0
 */
#నిర్వచించు HB_CODEPOINT_INVALID ((hb_codepoint_t) -1)

/**
 * hb_position_t:
 * 
 * Data type for holding a single coordinate value.
 * Contour points and other multi-dimensional data are
 * stored as tuples of #hb_position_t's.
 *
 **/
రకనిర్వచనం int32_t hb_position_t;
/**
 * hb_mask_t:
 * 
 * Data type for bitmasks.
 *
 **/
రకనిర్వచనం uint32_t hb_ముసుగు_t;

రకనిర్వచనం సంఘం _hb_var_int_t {
  uint32_t u32;
  int32_t i32;
  uint16_t u16[2];
  int16_t i16[2];
  uint8_t u8[4];
  int8_t i8[4];
} hb_var_int_t;

రకనిర్వచనం సంఘం hb_var_అంకె_t {
  దశాంశం f;
  uint32_t u32;
  int32_t i32;
  uint16_t u16[2];
  int16_t i16[2];
  uint8_t u8[4];
  int8_t i8[4];
} hb_var_అంకె_t_2;


/* hb_tag_t */

/**
 * hb_tag_t:
 *
 * Data type for tag identifiers. Tags are four
 * byte integers, each byte representing a character.
 *
 * Tags are used to identify tables, design-variation axes,
 * scripts, languages, font features, and baselines with
 * human-readable names.
 *
 **/
రకనిర్వచనం uint32_t hb_ట్యాగ్_t;

/**
 * HB_TAG:
 * @c1: 1st character of the tag
 * @c2: 2nd character of the tag
 * @c3: 3rd character of the tag
 * @c4: 4th character of the tag
 *
 * Constructs an #hb_tag_t from four character literals.
 *
 **/
#నిర్వచించు HB_ట్యాగ్(c1,c2,c3,c4) ((hb_ట్యాగ్_t)((((uint32_t)(c1)&0xFF)<<24)|(((uint32_t)(c2)&0xFF)<<16)|(((uint32_t)(c3)&0xFF)<<8)|((uint32_t)(c4)&0xFF)))

/**
 * HB_UNTAG:
 * @tag: an #hb_tag_t
 *
 * Extracts four character literals from an #hb_tag_t.
 *
 * Since: 0.6.0
 *
 **/
#నిర్వచించు HB_UNTAG(ట్యాగ్_2)   (uint8_t)(((ట్యాగ్_2)>>24)&0xFF), (uint8_t)(((ట్యాగ్_2)>>16)&0xFF), (uint8_t)(((ట్యాగ్_2)>>8)&0xFF), (uint8_t)((ట్యాగ్_2)&0xFF)

/**
 * HB_TAG_NONE:
 *
 * Unset #hb_tag_t.
 */
#నిర్వచించు HB_ట్యాగ్_NONE HB_ట్యాగ్(0,0,0,0)
/**
 * HB_TAG_MAX:
 *
 * Maximum possible unsigned #hb_tag_t.
 *
 * Since: 0.9.26
 */
#నిర్వచించు HB_ట్యాగ్_MAX HB_ట్యాగ్(0xff,0xff,0xff,0xff)
/**
 * HB_TAG_MAX_SIGNED:
 *
 * Maximum possible signed #hb_tag_t.
 *
 * Since: 0.9.33
 */
#నిర్వచించు HB_ట్యాగ్_MAX_SIGNED HB_ట్యాగ్(0x7f,0xff,0xff,0xff)

/* len=-1 means str is NUL-terminated. */
HB_EXTERN hb_ట్యాగ్_t
hb_ట్యాగ్_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len);

/* buf should have 4 bytes. */
HB_EXTERN శూన్య
hb_ట్యాగ్_గమ్యం_string (hb_ట్యాగ్_t ట్యాగ్_2, అక్షరం *buf);


/**
 * hb_direction_t:
 * @HB_DIRECTION_INVALID: Initial, unset direction.
 * @HB_DIRECTION_LTR: Text is set horizontally from left to right.
 * @HB_DIRECTION_RTL: Text is set horizontally from right to left.
 * @HB_DIRECTION_TTB: Text is set vertically from top to bottom.
 * @HB_DIRECTION_BTT: Text is set vertically from bottom to top.
 *
 * The direction of a text segment or buffer.
 * 
 * A segment can also be tested for horizontal or vertical
 * orientation (irrespective of specific direction) with 
 * HB_DIRECTION_IS_HORIZONTAL() or HB_DIRECTION_IS_VERTICAL().
 *
 */
రకనిర్వచనం గణన {
  HB_DIRECTION_INVALID = 0,
  HB_DIRECTION_LTR = 4,
  HB_DIRECTION_RTL,
  HB_DIRECTION_TTB,
  HB_DIRECTION_BTT
} hb_direction_t;

/* len=-1 means str is NUL-terminated */
HB_EXTERN hb_direction_t
hb_direction_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len);

HB_EXTERN స్థిరరాశి అక్షరం *
hb_direction_గమ్యం_string (hb_direction_t direction);

/**
 * HB_DIRECTION_IS_VALID:
 * @dir: #hb_direction_t to test
 *
 * Tests whether a text direction is valid.
 *
 **/
#నిర్వచించు HB_DIRECTION_IS_VALID(dir)	((((గుర్తులేని పూర్ణాంకం) (dir)) & ~3U) == 4)
/* Direction must be valid for the following */
/**
 * HB_DIRECTION_IS_HORIZONTAL:
 * @dir: #hb_direction_t to test
 *
 * Tests whether a text direction is horizontal. Requires
 * that the direction be valid.
 *
 **/
#నిర్వచించు HB_DIRECTION_IS_HORIZONTAL(dir)	((((గుర్తులేని పూర్ణాంకం) (dir)) & ~1U) == 4)
/**
 * HB_DIRECTION_IS_VERTICAL:
 * @dir: #hb_direction_t to test
 *
 * Tests whether a text direction is vertical. Requires
 * that the direction be valid.
 *
 **/
#నిర్వచించు HB_DIRECTION_IS_VERTICAL(dir)	((((గుర్తులేని పూర్ణాంకం) (dir)) & ~1U) == 6)
/**
 * HB_DIRECTION_IS_FORWARD:
 * @dir: #hb_direction_t to test
 *
 * Tests whether a text direction moves forward (from left to right, or from
 * top to bottom). Requires that the direction be valid.
 *
 **/
#నిర్వచించు HB_DIRECTION_IS_FORWARD(dir)	((((గుర్తులేని పూర్ణాంకం) (dir)) & ~2U) == 4)
/**
 * HB_DIRECTION_IS_BACKWARD:
 * @dir: #hb_direction_t to test
 *
 * Tests whether a text direction moves backward (from right to left, or from
 * bottom to top). Requires that the direction be valid.
 *
 **/
#నిర్వచించు HB_DIRECTION_IS_BACKWARD(dir)	((((గుర్తులేని పూర్ణాంకం) (dir)) & ~2U) == 5)
/**
 * HB_DIRECTION_REVERSE:
 * @dir: #hb_direction_t to reverse
 *
 * Reverses a text direction. Requires that the direction
 * be valid.
 *
 **/
#నిర్వచించు HB_DIRECTION_తిరగేయి(dir)	((hb_direction_t) (((గుర్తులేని పూర్ణాంకం) (dir)) ^ 1))


/* hb_language_t */

/**
 * hb_language_t:
 *
 * Data type for languages. Each #hb_language_t corresponds to a BCP 47
 * language tag.
 *
 */
రకనిర్వచనం స్థిరరాశి నిర్మాణం hb_language_impl_t *hb_language_t;

HB_EXTERN hb_language_t
hb_language_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len);

HB_EXTERN స్థిరరాశి అక్షరం *
hb_language_గమ్యం_string (hb_language_t language);

/**
 * HB_LANGUAGE_INVALID:
 *
 * An unset #hb_language_t.
 *
 * Since: 0.6.0
 */
#నిర్వచించు HB_LANGUAGE_INVALID ((hb_language_t) 0)

HB_EXTERN hb_language_t
hb_language_get_default (శూన్య);

HB_EXTERN hb_bool_t
hb_language_matches (hb_language_t language,
		     hb_language_t specific);

#అన్తర్భావ "hb-script-list.h"

/* Script functions */

HB_EXTERN hb_script_t
hb_script_from_iso15924_ట్యాగ్ (hb_ట్యాగ్_t ట్యాగ్_2);

HB_EXTERN hb_script_t
hb_script_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len);

HB_EXTERN hb_ట్యాగ్_t
hb_script_గమ్యం_iso15924_ట్యాగ్ (hb_script_t script);

HB_EXTERN hb_direction_t
hb_script_get_horizontal_direction (hb_script_t script);


/* User data */

/**
 * hb_user_data_key_t:
 *
 * Data structure for holding user-data keys.
 *
 **/
రకనిర్వచనం నిర్మాణం hb_user_దత్తాంశం_కీలకం_t {
  /*< private >*/
  అక్షరం వాడని_2;
} hb_user_దత్తాంశం_కీలకం_t;

/**
 * hb_destroy_func_t:
 * @user_data: the data to be destroyed
 *
 * A virtual method for destroy user-data callbacks.
 *
 */
రకనిర్వచనం శూన్య (*hb_destroy_func_t) (శూన్య *user_దత్తాంశం);


/* Font features and variations. */

/**
 * HB_FEATURE_GLOBAL_START:
 *
 * Special setting for #hb_feature_t.start to apply the feature from the start
 * of the buffer.
 *
 * Since: 2.0.0
 */
#నిర్వచించు HB_FEATURE_GLOBAL_ప్రారంభం	0

/**
 * HB_FEATURE_GLOBAL_END:
 *
 * Special setting for #hb_feature_t.end to apply the feature from to the end
 * of the buffer.
 *
 * Since: 2.0.0
 */
#నిర్వచించు HB_FEATURE_GLOBAL_చివర	((గుర్తులేని పూర్ణాంకం) -1)

/**
 * hb_feature_t:
 * @tag: The #hb_tag_t tag of the feature
 * @value: The value of the feature. 0 disables the feature, non-zero (usually
 * 1) enables the feature.  For features implemented as lookup type 3 (like
 * 'salt') the @value is a one based index into the alternates.
 * @start: the cluster to start applying this feature setting (inclusive).
 * @end: the cluster to end applying this feature setting (exclusive).
 *
 * The #hb_feature_t is the structure that holds information about requested
 * feature application. The feature will be applied with the given value to all
 * glyphs which are in clusters between @start (inclusive) and @end (exclusive).
 * Setting start to #HB_FEATURE_GLOBAL_START and end to #HB_FEATURE_GLOBAL_END
 * specifies that the feature always applies to the entire buffer.
 */
రకనిర్వచనం నిర్మాణం hb_feature_t {
  hb_ట్యాగ్_t      ట్యాగ్_2;
  uint32_t      విలువ_2;
  గుర్తులేని పూర్ణాంకం  ప్రారంభం;
  గుర్తులేని పూర్ణాంకం  చివర;
} hb_feature_t;

HB_EXTERN hb_bool_t
hb_feature_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len,
			hb_feature_t *feature);

HB_EXTERN శూన్య
hb_feature_గమ్యం_string (hb_feature_t *feature,
		      అక్షరం *buf, గుర్తులేని పూర్ణాంకం కొలత);

/**
 * hb_variation_t:
 * @tag: The #hb_tag_t tag of the variation-axis name
 * @value: The value of the variation axis
 *
 * Data type for holding variation data. Registered OpenType
 * variation-axis tags are listed in
 * [OpenType Axis Tag Registry](https://docs.microsoft.com/en-us/typography/opentype/spec/dvaraxisreg).
 * 
 * Since: 1.4.2
 */
రకనిర్వచనం నిర్మాణం hb_variation_t {
  hb_ట్యాగ్_t ట్యాగ్_2;
  దశాంశం    విలువ_2;
} hb_variation_t;

HB_EXTERN hb_bool_t
hb_variation_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len,
			  hb_variation_t *variation);

HB_EXTERN శూన్య
hb_variation_గమ్యం_string (hb_variation_t *variation,
			అక్షరం *buf, గుర్తులేని పూర్ణాంకం కొలత);

/**
 * hb_color_t:
 *
 * Data type for holding color values. Colors are eight bits per
 * channel RGB plus alpha transparency.
 *
 * Since: 2.1.0
 */
రకనిర్వచనం uint32_t hb_color_t;

/**
 * HB_COLOR:
 * @b: blue channel value
 * @g: green channel value
 * @r: red channel value
 * @a: alpha channel value
 *
 * Constructs an #hb_color_t from four integers.
 *
 * Since: 2.1.0
 */
#నిర్వచించు HB_COLOR(b,g,r,a) ((hb_color_t) HB_ట్యాగ్ ((b),(g),(r),(a)))

HB_EXTERN uint8_t
hb_color_get_alpha (hb_color_t color);
#నిర్వచించు hb_color_get_alpha(color)	((color) & 0xFF)

HB_EXTERN uint8_t
hb_color_get_red (hb_color_t color);
#నిర్వచించు hb_color_get_red(color)		(((color) >> 8) & 0xFF)

HB_EXTERN uint8_t
hb_color_get_green (hb_color_t color);
#నిర్వచించు hb_color_get_green(color)	(((color) >> 16) & 0xFF)

HB_EXTERN uint8_t
hb_color_get_blue (hb_color_t color);
#నిర్వచించు hb_color_get_blue(color)	(((color) >> 24) & 0xFF)

/**
 * hb_glyph_extents_t:
 * @x_bearing: Distance from the x-origin to the left extremum of the glyph.
 * @y_bearing: Distance from the top extremum of the glyph to the y-origin.
 * @width: Distance from the left extremum of the glyph to the right extremum.
 * @height: Distance from the top extremum of the glyph to the bottom extremum.
 *
 * Glyph extent values, measured in font units.
 *
 * Note that @height is negative, in coordinate systems that grow up.
 **/
రకనిర్వచనం నిర్మాణం hb_glyph_extents_t {
  hb_position_t x_bearing;
  hb_position_t y_bearing;
  hb_position_t వెడల్పు;
  hb_position_t ఎత్తు;
} hb_glyph_extents_t;

/**
 * hb_font_t:
 *
 * Data type for holding fonts.
 *
 */
రకనిర్వచనం నిర్మాణం hb_font_t hb_font_t;

/* Not of much use to clients. */
HB_EXTERN శూన్య*
hb_malloc (size_t కొలత);
HB_EXTERN శూన్య*
hb_calloc (size_t nmemb, size_t కొలత);
HB_EXTERN శూన్య*
hb_realloc (శూన్య *ptr, size_t కొలత);
HB_EXTERN శూన్య
hb_free (శూన్య *ptr);

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_COMMON_H */
