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

#अगर !defined(HB_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#त्रुटि "Include <hb.h> instead."
#पूर्ण_अगर

#अगर_अघोषित HB_COMMON_H
#घोषणा HB_COMMON_H

#अगर_अघोषित HB_EXTERN
#घोषणा HB_EXTERN बाह्य
#पूर्ण_अगर

#अगर_अघोषित HB_BEGIN_DECLS
# ifdef __cplusplus
#  define HB_BEGIN_DECLS	बाह्य "C" {
#  define HB_END_DECLS		}
# अन्यथा /* !__cplusplus */
#  define HB_BEGIN_DECLS
#  define HB_END_DECLS
# endif /* !__cplusplus */
#पूर्ण_अगर

#अगर defined (_AIX)
#  include <sys/inttypes.h>
#या_अगर defined (_MSC_VER) && _MSC_VER < 1600
/* VS 2010 (_MSC_VER 1600) has stdint.h   */
प्रकार __int8 int8_t;
प्रकार अचिन्हित __int8 uint8_t;
प्रकार __int16 int16_t;
प्रकार अचिन्हित __int16 uint16_t;
प्रकार __int32 int32_t;
प्रकार अचिन्हित __int32 uint32_t;
प्रकार __int64 int64_t;
प्रकार अचिन्हित __int64 uint64_t;
#या_अगर defined (_MSC_VER) && _MSC_VER < 1800
/* VS 2013 (_MSC_VER 1800) has inttypes.h */
#  include <stdint.h>
#अन्यथा
#  include <inttypes.h>
#पूर्ण_अगर
#समावेश <stddef.h>

#अगर defined(__GNUC__) && ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#घोषणा HB_DEPRECATED __attribute__((__deprecated__))
#या_अगर defined(_MSC_VER) && (_MSC_VER >= 1300)
#घोषणा HB_DEPRECATED __declspec(deprecated)
#अन्यथा
#घोषणा HB_DEPRECATED
#पूर्ण_अगर

#अगर defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))
#घोषणा HB_DEPRECATED_FOR(f) __attribute__((__deprecated__("Use '" #f "' instead")))
#या_अगर defined(_MSC_FULL_VER) && (_MSC_FULL_VER > 140050320)
#घोषणा HB_DEPRECATED_FOR(f) __declspec(deprecated("is deprecated. Use '" #f "' instead"))
#अन्यथा
#घोषणा HB_DEPRECATED_FOR(f) HB_DEPRECATED
#पूर्ण_अगर


HB_BEGIN_DECLS

/**
 * hb_bool_t:
 * 
 * Data type for booleans.
 *
 **/
प्रकार पूर्णांक hb_bool_t;

/**
 * HB_BUDGET_DEFAULT:
 *
 * Use the implementation's finite default work budget.
 *
 * Since: 14.5.0
 **/
#घोषणा HB_BUDGET_DEFAULT ((int64_t) (-0x7FFFFFFFFFFFFFFF - 1))

/**
 * HB_BUDGET_UNLIMITED:
 *
 * Use an effectively unlimited work budget. Structural safety limits remain
 * active.
 *
 * Since: 14.5.0
 **/
#घोषणा HB_BUDGET_UNLIMITED ((int64_t) 0x7FFFFFFFFFFFFFFF)

/**
 * hb_codepoint_t:
 * 
 * Data type for holding Unicode codepoints. Also
 * used to hold glyph IDs.
 *
 **/
प्रकार uint32_t hb_codepoint_t;

/**
 * HB_CODEPOINT_INVALID:
 *
 * Unused #hb_codepoint_t value.
 *
 * Since: 8.0.0
 */
#घोषणा HB_CODEPOINT_INVALID ((hb_codepoint_t) -1)

/**
 * hb_position_t:
 * 
 * Data type for holding a single coordinate value.
 * Contour points and other multi-dimensional data are
 * stored as tuples of #hb_position_t's.
 *
 **/
प्रकार int32_t hb_position_t;
/**
 * hb_mask_t:
 * 
 * Data type for bitmasks.
 *
 **/
प्रकार uint32_t hb_mask_t;

प्रकार जोड़ _hb_var_int_t {
  uint32_t u32;
  int32_t i32;
  uint16_t u16[2];
  int16_t i16[2];
  uint8_t u8[4];
  int8_t i8[4];
} hb_var_int_t;

प्रकार जोड़ hb_var_अंक_t {
  भग्न f;
  uint32_t u32;
  int32_t i32;
  uint16_t u16[2];
  int16_t i16[2];
  uint8_t u8[4];
  int8_t i8[4];
} hb_var_अंक_t_2;


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
प्रकार uint32_t hb_tag_t;

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
#घोषणा HB_TAG(c1,c2,c3,c4) ((hb_tag_t)((((uint32_t)(c1)&0xFF)<<24)|(((uint32_t)(c2)&0xFF)<<16)|(((uint32_t)(c3)&0xFF)<<8)|((uint32_t)(c4)&0xFF)))

/**
 * HB_UNTAG:
 * @tag: an #hb_tag_t
 *
 * Extracts four character literals from an #hb_tag_t.
 *
 * Since: 0.6.0
 *
 **/
#घोषणा HB_UNTAG(tag)   (uint8_t)(((tag)>>24)&0xFF), (uint8_t)(((tag)>>16)&0xFF), (uint8_t)(((tag)>>8)&0xFF), (uint8_t)((tag)&0xFF)

/**
 * HB_TAG_NONE:
 *
 * Unset #hb_tag_t.
 */
#घोषणा HB_TAG_NONE HB_TAG(0,0,0,0)
/**
 * HB_TAG_MAX:
 *
 * Maximum possible unsigned #hb_tag_t.
 *
 * Since: 0.9.26
 */
#घोषणा HB_TAG_MAX HB_TAG(0xff,0xff,0xff,0xff)
/**
 * HB_TAG_MAX_SIGNED:
 *
 * Maximum possible signed #hb_tag_t.
 *
 * Since: 0.9.33
 */
#घोषणा HB_TAG_MAX_SIGNED HB_TAG(0x7f,0xff,0xff,0xff)

/* len=-1 means str is NUL-terminated. */
HB_EXTERN hb_tag_t
hb_tag_from_string (स्थिर अक्षर *str, पूर्णांक len);

/* buf should have 4 bytes. */
HB_EXTERN व्योम
hb_tag_to_string (hb_tag_t tag, अक्षर *buf);


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
प्रकार क्रमागत {
  HB_DIRECTION_INVALID = 0,
  HB_DIRECTION_LTR = 4,
  HB_DIRECTION_RTL,
  HB_DIRECTION_TTB,
  HB_DIRECTION_BTT
} hb_direction_t;

/* len=-1 means str is NUL-terminated */
HB_EXTERN hb_direction_t
hb_direction_from_string (स्थिर अक्षर *str, पूर्णांक len);

HB_EXTERN स्थिर अक्षर *
hb_direction_to_string (hb_direction_t direction);

/**
 * HB_DIRECTION_IS_VALID:
 * @dir: #hb_direction_t to test
 *
 * Tests whether a text direction is valid.
 *
 **/
#घोषणा HB_DIRECTION_IS_VALID(dir)	((((अचिन्हित पूर्णांक) (dir)) & ~3U) == 4)
/* Direction must be valid for the following */
/**
 * HB_DIRECTION_IS_HORIZONTAL:
 * @dir: #hb_direction_t to test
 *
 * Tests whether a text direction is horizontal. Requires
 * that the direction be valid.
 *
 **/
#घोषणा HB_DIRECTION_IS_HORIZONTAL(dir)	((((अचिन्हित पूर्णांक) (dir)) & ~1U) == 4)
/**
 * HB_DIRECTION_IS_VERTICAL:
 * @dir: #hb_direction_t to test
 *
 * Tests whether a text direction is vertical. Requires
 * that the direction be valid.
 *
 **/
#घोषणा HB_DIRECTION_IS_VERTICAL(dir)	((((अचिन्हित पूर्णांक) (dir)) & ~1U) == 6)
/**
 * HB_DIRECTION_IS_FORWARD:
 * @dir: #hb_direction_t to test
 *
 * Tests whether a text direction moves forward (from left to right, or from
 * top to bottom). Requires that the direction be valid.
 *
 **/
#घोषणा HB_DIRECTION_IS_FORWARD(dir)	((((अचिन्हित पूर्णांक) (dir)) & ~2U) == 4)
/**
 * HB_DIRECTION_IS_BACKWARD:
 * @dir: #hb_direction_t to test
 *
 * Tests whether a text direction moves backward (from right to left, or from
 * bottom to top). Requires that the direction be valid.
 *
 **/
#घोषणा HB_DIRECTION_IS_BACKWARD(dir)	((((अचिन्हित पूर्णांक) (dir)) & ~2U) == 5)
/**
 * HB_DIRECTION_REVERSE:
 * @dir: #hb_direction_t to reverse
 *
 * Reverses a text direction. Requires that the direction
 * be valid.
 *
 **/
#घोषणा HB_DIRECTION_REVERSE(dir)	((hb_direction_t) (((अचिन्हित पूर्णांक) (dir)) ^ 1))


/* hb_language_t */

/**
 * hb_language_t:
 *
 * Data type for languages. Each #hb_language_t corresponds to a BCP 47
 * language tag.
 *
 */
प्रकार स्थिर काष्ठा hb_language_impl_t *hb_language_t;

HB_EXTERN hb_language_t
hb_language_from_string (स्थिर अक्षर *str, पूर्णांक len);

HB_EXTERN स्थिर अक्षर *
hb_language_to_string (hb_language_t language);

/**
 * HB_LANGUAGE_INVALID:
 *
 * An unset #hb_language_t.
 *
 * Since: 0.6.0
 */
#घोषणा HB_LANGUAGE_INVALID ((hb_language_t) 0)

HB_EXTERN hb_language_t
hb_language_get_default (व्योम);

HB_EXTERN hb_bool_t
hb_language_matches (hb_language_t language,
		     hb_language_t specific);

#समावेश "hb-script-list.h"

/* Script functions */

HB_EXTERN hb_script_t
hb_script_from_iso15924_tag (hb_tag_t tag);

HB_EXTERN hb_script_t
hb_script_from_string (स्थिर अक्षर *str, पूर्णांक len);

HB_EXTERN hb_tag_t
hb_script_to_iso15924_tag (hb_script_t script);

HB_EXTERN hb_direction_t
hb_script_get_horizontal_direction (hb_script_t script);


/* User data */

/**
 * hb_user_data_key_t:
 *
 * Data structure for holding user-data keys.
 *
 **/
प्रकार काष्ठा hb_user_तथ्य_कुंजी_t {
  /*< private >*/
  अक्षर unused;
} hb_user_तथ्य_कुंजी_t;

/**
 * hb_destroy_func_t:
 * @user_data: the data to be destroyed
 *
 * A virtual method for destroy user-data callbacks.
 *
 */
प्रकार व्योम (*hb_destroy_func_t) (व्योम *user_तथ्य);


/* Font features and variations. */

/**
 * HB_FEATURE_GLOBAL_START:
 *
 * Special setting for #hb_feature_t.start to apply the feature from the start
 * of the buffer.
 *
 * Since: 2.0.0
 */
#घोषणा HB_FEATURE_GLOBAL_START	0

/**
 * HB_FEATURE_GLOBAL_END:
 *
 * Special setting for #hb_feature_t.end to apply the feature from to the end
 * of the buffer.
 *
 * Since: 2.0.0
 */
#घोषणा HB_FEATURE_GLOBAL_END	((अचिन्हित पूर्णांक) -1)

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
प्रकार काष्ठा hb_feature_t {
  hb_tag_t      tag;
  uint32_t      मूल्य_2;
  अचिन्हित पूर्णांक  start;
  अचिन्हित पूर्णांक  end;
} hb_feature_t;

HB_EXTERN hb_bool_t
hb_feature_from_string (स्थिर अक्षर *str, पूर्णांक len,
			hb_feature_t *feature);

HB_EXTERN व्योम
hb_feature_to_string (hb_feature_t *feature,
		      अक्षर *buf, अचिन्हित पूर्णांक आकार);

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
प्रकार काष्ठा hb_variation_t {
  hb_tag_t tag;
  भग्न    मूल्य_2;
} hb_variation_t;

HB_EXTERN hb_bool_t
hb_variation_from_string (स्थिर अक्षर *str, पूर्णांक len,
			  hb_variation_t *variation);

HB_EXTERN व्योम
hb_variation_to_string (hb_variation_t *variation,
			अक्षर *buf, अचिन्हित पूर्णांक आकार);

/**
 * hb_color_t:
 *
 * Data type for holding color values. Colors are eight bits per
 * channel RGB plus alpha transparency.
 *
 * Since: 2.1.0
 */
प्रकार uint32_t hb_color_t;

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
#घोषणा HB_COLOR(b,g,r,a) ((hb_color_t) HB_TAG ((b),(g),(r),(a)))

HB_EXTERN uint8_t
hb_color_get_alpha (hb_color_t color);
#घोषणा hb_color_get_alpha(color)	((color) & 0xFF)

HB_EXTERN uint8_t
hb_color_get_red (hb_color_t color);
#घोषणा hb_color_get_red(color)		(((color) >> 8) & 0xFF)

HB_EXTERN uint8_t
hb_color_get_green (hb_color_t color);
#घोषणा hb_color_get_green(color)	(((color) >> 16) & 0xFF)

HB_EXTERN uint8_t
hb_color_get_blue (hb_color_t color);
#घोषणा hb_color_get_blue(color)	(((color) >> 24) & 0xFF)

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
प्रकार काष्ठा hb_glyph_extents_t {
  hb_position_t x_bearing;
  hb_position_t y_bearing;
  hb_position_t width;
  hb_position_t height;
} hb_glyph_extents_t;

/**
 * hb_font_t:
 *
 * Data type for holding fonts.
 *
 */
प्रकार काष्ठा hb_font_t hb_font_t;

/* Not of much use to clients. */
HB_EXTERN व्योम*
hb_malloc (माप_प्रकार आकार);
HB_EXTERN व्योम*
hb_calloc (माप_प्रकार nmemb, माप_प्रकार आकार);
HB_EXTERN व्योम*
hb_realloc (व्योम *ptr, माप_प्रकार आकार);
HB_EXTERN व्योम
hb_free (व्योम *ptr);

HB_END_DECLS

#पूर्ण_अगर /* HB_COMMON_H */
