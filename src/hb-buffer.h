/*
 * Copyright © 1998-2004  David Turner and Werner Lemberg
 * Copyright © 2004,2007,2009  Red Hat, Inc.
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
 * Red Hat Author(s): Owen Taylor, Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#اگر !defined(HB_H_اندر) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb.h> instead."
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_بفر_H
#تعریف_کرو HB_بفر_H

#شامل "hb-common.h"
#شامل "hb-unicode.h"
#شامل "hb-font.h"

HB_BEGIN_DECLS

/**
 * hb_glyph_info_t:
 * @codepoint: either a Unicode code point (before shaping) or a glyph index
 *             (after shaping).
 * @cluster: the index of the character in the original text that corresponds
 *           to this #hb_glyph_info_t, or whatever the client passes to
 *           hb_buffer_add(). More than one #hb_glyph_info_t can have the same
 *           @cluster value, if they resulted from the same character (e.g. one
 *           to many glyph substitution), and when more than one character gets
 *           merged in the same glyph (e.g. many to one glyph substitution) the
 *           #hb_glyph_info_t will have the smallest cluster value of them.
 *           By default some characters are merged into the same cluster
 *           (e.g. combining marks have the same cluster as their bases)
 *           even if they are separate glyphs, hb_buffer_set_cluster_level()
 *           allow selecting more fine-grained cluster handling.
 *
 * The #hb_glyph_info_t is the structure that holds information about the
 * glyphs and their relation to input text.
 */
قسمتعریف ساخت hb_glyph_info_t {
  hb_codepoint_t codepoint;
  /*< private >*/
  hb_نقاب_t      نقاب;
  /*< public >*/
  uint32_t       cluster;

  /*< private >*/
  hb_var_int_t   var1;
  hb_var_int_t   var2;
} hb_glyph_info_t;

/**
 * hb_glyph_flags_t:
 * @HB_GLYPH_FLAG_UNSAFE_TO_BREAK: Indicates that if input text is broken at the
 * 				   beginning of the cluster this glyph is part of,
 * 				   then both sides need to be re-shaped, as the
 * 				   result might be different.
 * 				   On the flip side, it means that when this
 * 				   flag is not present, then it is safe to break
 * 				   the glyph-run at the beginning of this
 * 				   cluster, and the two sides will represent the
 * 				   exact same result one would get if breaking
 * 				   input text at the beginning of this cluster
 * 				   and shaping the two sides separately.
 * 				   This can be used to optimize paragraph
 * 				   layout, by avoiding re-shaping of each line
 * 				   after line-breaking.
 * @HB_GLYPH_FLAG_UNSAFE_TO_CONCAT: Indicates that if input text is changed on one
 * 				   side of the beginning of the cluster this glyph
 * 				   is part of, then the shaping results for the
 * 				   other side might change.
 * 				   Note that the absence of this flag will NOT by
 * 				   itself mean that it IS safe to concat text.
 * 				   Only two pieces of text both of which clear of
 * 				   this flag can be concatenated safely.
 * 				   This can be used to optimize paragraph
 * 				   layout, by avoiding re-shaping of each line
 * 				   after line-breaking, by limiting the
 * 				   reshaping to a small piece around the
 * 				   breaking position only, even if the breaking
 * 				   position carries the
 * 				   #HB_GLYPH_FLAG_UNSAFE_TO_BREAK or when
 * 				   hyphenation or other text transformation
 * 				   happens at line-break position, in the following
 * 				   way:
 * 				   1. Iterate back from the line-break position
 * 				   until the first cluster start position that is
 * 				   NOT unsafe-to-concat, 2. shape the segment from
 * 				   there till the end of line, 3. check whether the
 * 				   resulting glyph-run also is clear of the
 * 				   unsafe-to-concat at its start-of-text position;
 * 				   if it is, just splice it into place and the line
 * 				   is shaped; If not, move on to a position further
 * 				   back that is clear of unsafe-to-concat and retry
 * 				   from there, and repeat.
 * 				   At the start of next line a similar algorithm can
 * 				   be implemented. That is: 1. Iterate forward from
 * 				   the line-break position until the first cluster
 * 				   start position that is NOT unsafe-to-concat, 2.
 * 				   shape the segment from beginning of the line to
 * 				   that position, 3. check whether the resulting
 * 				   glyph-run also is clear of the unsafe-to-concat
 * 				   at its end-of-text position; if it is, just splice
 * 				   it into place and the beginning is shaped; If not,
 * 				   move on to a position further forward that is clear
 * 				   of unsafe-to-concat and retry up to there, and repeat.
 * 				   A slight complication will arise in the
 * 				   implementation of the algorithm above,
 * 				   because while our buffer API has a way to
 * 				   return flags for position corresponding to
 * 				   start-of-text, there is currently no position
 * 				   corresponding to end-of-text.  This limitation
 * 				   can be alleviated by shaping more text than needed
 * 				   and looking for unsafe-to-concat flag within text
 * 				   clusters.
 * 				   The #HB_GLYPH_FLAG_UNSAFE_TO_BREAK flag will
 * 				   always imply this flag.
 *				   To use this flag, you must enable the buffer flag
 *				   @HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT during
 *				   shaping, otherwise the buffer flag will not be
 *				   reliably produced.
 * 				   Since: 4.0.0
 * @HB_GLYPH_FLAG_SAFE_TO_INSERT_TATWEEL: In scripts that use elongation (Arabic,
				   Mongolian, Syriac, etc.), this flag signifies
				   that it is safe to insert a U+0640 TATWEEL
				   character before this cluster for elongation.
				   This flag does not determine the
				   script-specific elongation places, but only
				   when it is safe to do the elongation without
				   interrupting text shaping.
				   Since: 5.1.0
 * @HB_GLYPH_FLAG_DEFINED: All the currently defined flags.
 *
 * Flags for #hb_glyph_info_t.
 *
 * Since: 1.5.0
 */
قسمتعریف شمار { /*< flags >*/
  HB_GLYPH_FLAG_UNSAFE_مقصد_BREAK			= 0x00000001,
  HB_GLYPH_FLAG_UNSAFE_مقصد_CONCAT		= 0x00000002,
  HB_GLYPH_FLAG_محفوظ_مقصد_داخل_کرو_TATWEEL		= 0x00000004,

  HB_GLYPH_FLAG_DEFINED				= 0x00000007 /* OR of all defined flags */
} hb_glyph_flags_t;

HB_EXTERN hb_glyph_flags_t
hb_glyph_info_get_glyph_flags (ثابت hb_glyph_info_t *info);

#تعریف_کرو hb_glyph_info_get_glyph_flags(info) \
	((hb_glyph_flags_t) ((غیرعلامتی صحیح) (info)->نقاب & HB_GLYPH_FLAG_DEFINED))


/**
 * hb_glyph_position_t:
 * @x_advance: how much the line advances after drawing this glyph when setting
 *             text in horizontal direction.
 * @y_advance: how much the line advances after drawing this glyph when setting
 *             text in vertical direction.
 * @x_offset: how much the glyph moves on the X-axis before drawing it, this
 *            should not affect how much the line advances.
 * @y_offset: how much the glyph moves on the Y-axis before drawing it, this
 *            should not affect how much the line advances.
 *
 * The #hb_glyph_position_t is the structure that holds the positions of the
 * glyph in both horizontal and vertical directions. All positions in
 * #hb_glyph_position_t are relative to the current point.
 *
 */
قسمتعریف ساخت hb_glyph_position_t {
  hb_position_t  x_advance;
  hb_position_t  y_advance;
  hb_position_t  x_انحراف_2;
  hb_position_t  y_انحراف_2;

  /*< private >*/
  hb_var_int_t   var;
} hb_glyph_position_t;

/**
 * hb_segment_properties_t:
 * @direction: the #hb_direction_t of the buffer, see hb_buffer_set_direction().
 * @script: the #hb_script_t of the buffer, see hb_buffer_set_script().
 * @language: the #hb_language_t of the buffer, see hb_buffer_set_language().
 *
 * The structure that holds various text properties of an #hb_buffer_t. Can be
 * set and retrieved using hb_buffer_set_segment_properties() and
 * hb_buffer_get_segment_properties(), respectively.
 */
قسمتعریف ساخت hb_segment_properties_t {
  hb_direction_t  direction;
  hb_script_t     script;
  hb_language_t   language;
  /*< private >*/
  خالی           *reserved1;
  خالی           *reserved2;
} hb_segment_properties_t;

/**
 * HB_SEGMENT_PROPERTIES_DEFAULT:
 *
 * The default #hb_segment_properties_t of of freshly created #hb_buffer_t.
 */
#تعریف_کرو HB_SEGMENT_PROPERTIES_DEFAULT {HB_DIRECTION_INVALID, \
				       HB_SCRIPT_INVALID, \
				       HB_LANGUAGE_INVALID, \
				       (خالی *) 0, \
				       (خالی *) 0}

HB_EXTERN hb_bool_t
hb_segment_properties_equal (ثابت hb_segment_properties_t *a,
			     ثابت hb_segment_properties_t *b);

HB_EXTERN غیرعلامتی صحیح
hb_segment_properties_hash (ثابت hb_segment_properties_t *p);

HB_EXTERN خالی
hb_segment_properties_overlay (hb_segment_properties_t *p,
			       ثابت hb_segment_properties_t *src);


/**
 * hb_buffer_t:
 *
 * The main structure holding the input text and its properties before shaping,
 * and output glyphs and their information after shaping.
 */

قسمتعریف ساخت hb_بفر_t hb_بفر_t;

HB_EXTERN hb_بفر_t *
hb_بفر_create (خالی);

HB_EXTERN hb_بفر_t *
hb_بفر_create_similar (ثابت hb_بفر_t *src);

HB_EXTERN خالی
hb_بفر_reset (hb_بفر_t *بفر);


HB_EXTERN hb_بفر_t *
hb_بفر_get_خالی_خانے (خالی);

HB_EXTERN hb_بفر_t *
hb_بفر_reference (hb_بفر_t *بفر);

HB_EXTERN خالی
hb_بفر_destroy (hb_بفر_t *بفر);

HB_EXTERN hb_bool_t
hb_بفر_سیٹ_user_ڈیٹا (hb_بفر_t        *بفر,
			 hb_user_ڈیٹا_کلید_t *کلید_2,
			 خالی *              ڈیٹا,
			 hb_destroy_func_t   destroy,
			 hb_bool_t           replace);

HB_EXTERN خالی *
hb_بفر_get_user_ڈیٹا (ثابت hb_بفر_t  *بفر,
			 hb_user_ڈیٹا_کلید_t *کلید_2);


/**
 * hb_buffer_content_type_t:
 * @HB_BUFFER_CONTENT_TYPE_INVALID: Initial value for new buffer.
 * @HB_BUFFER_CONTENT_TYPE_UNICODE: The buffer contains input characters (before shaping).
 * @HB_BUFFER_CONTENT_TYPE_GLYPHS: The buffer contains output glyphs (after shaping).
 *
 * The type of #hb_buffer_t contents.
 */
قسمتعریف شمار {
  HB_بفر_CONTENT_TYPE_INVALID = 0,
  HB_بفر_CONTENT_TYPE_UNICODE,
  HB_بفر_CONTENT_TYPE_GLYPHS
} hb_بفر_content_type_t;

HB_EXTERN خالی
hb_بفر_سیٹ_content_type (hb_بفر_t              *بفر,
			    hb_بفر_content_type_t  content_type);

HB_EXTERN hb_بفر_content_type_t
hb_بفر_get_content_type (ثابت hb_بفر_t *بفر);


HB_EXTERN خالی
hb_بفر_سیٹ_unicode_funcs (hb_بفر_t        *بفر,
			     hb_unicode_funcs_t *unicode_funcs);

HB_EXTERN hb_unicode_funcs_t *
hb_بفر_get_unicode_funcs (ثابت hb_بفر_t  *بفر);

HB_EXTERN خالی
hb_بفر_سیٹ_direction (hb_بفر_t    *بفر,
			 hb_direction_t  direction);

HB_EXTERN hb_direction_t
hb_بفر_get_direction (ثابت hb_بفر_t *بفر);

HB_EXTERN خالی
hb_بفر_سیٹ_script (hb_بفر_t *بفر,
		      hb_script_t  script);

HB_EXTERN hb_script_t
hb_بفر_get_script (ثابت hb_بفر_t *بفر);

HB_EXTERN خالی
hb_بفر_سیٹ_language (hb_بفر_t   *بفر,
			hb_language_t  language);


HB_EXTERN hb_language_t
hb_بفر_get_language (ثابت hb_بفر_t *بفر);

HB_EXTERN خالی
hb_بفر_سیٹ_segment_properties (hb_بفر_t *بفر,
				  ثابت hb_segment_properties_t *props);

HB_EXTERN خالی
hb_بفر_get_segment_properties (ثابت hb_بفر_t *بفر,
				  hb_segment_properties_t *props);

HB_EXTERN خالی
hb_بفر_guess_segment_properties (hb_بفر_t *بفر);


/**
 * hb_buffer_flags_t:
 * @HB_BUFFER_FLAG_DEFAULT: the default buffer flag.
 * @HB_BUFFER_FLAG_BOT: flag indicating that special handling of the beginning
 *                      of text paragraph can be applied to this buffer. Should usually
 *                      be set, unless you are passing to the buffer only part
 *                      of the text without the full context.
 * @HB_BUFFER_FLAG_EOT: flag indicating that special handling of the end of text
 *                      paragraph can be applied to this buffer, similar to
 *                      @HB_BUFFER_FLAG_BOT.
 * @HB_BUFFER_FLAG_PRESERVE_DEFAULT_IGNORABLES:
 *                      flag indication that character with Default_Ignorable
 *                      Unicode property should use the corresponding glyph
 *                      from the font, instead of hiding them (done by
 *                      replacing them with the space glyph and zeroing the
 *                      advance width.)  This flag takes precedence over
 *                      @HB_BUFFER_FLAG_REMOVE_DEFAULT_IGNORABLES.
 * @HB_BUFFER_FLAG_REMOVE_DEFAULT_IGNORABLES:
 *                      flag indication that character with Default_Ignorable
 *                      Unicode property should be removed from glyph string
 *                      instead of hiding them (done by replacing them with the
 *                      space glyph and zeroing the advance width.)
 *                      @HB_BUFFER_FLAG_PRESERVE_DEFAULT_IGNORABLES takes
 *                      precedence over this flag. Since: 1.8.0
 * @HB_BUFFER_FLAG_DO_NOT_INSERT_DOTTED_CIRCLE:
 *                      flag indicating that a dotted circle should
 *                      not be inserted in the rendering of incorrect
 *                      character sequences (such at <0905 093E>). Since: 2.4.0
 * @HB_BUFFER_FLAG_VERIFY:
 *                      flag indicating that the hb_shape() call and its variants
 *                      should perform various verification processes on the results
 *                      of the shaping operation on the buffer.  If the verification
 *                      fails, then either a buffer message is sent, if a message
 *                      handler is installed on the buffer, or a message is written
 *                      to standard error.  In either case, the shaping result might
 *                      be modified to show the failed output. Since: 3.4.0
 * @HB_BUFFER_FLAG_PRODUCE_UNSAFE_TO_CONCAT:
 *                      flag indicating that the @HB_GLYPH_FLAG_UNSAFE_TO_CONCAT
 *                      glyph-flag should be produced by the shaper. By default
 *                      it will not be produced since it incurs a cost. Since: 4.0.0
 * @HB_BUFFER_FLAG_PRODUCE_SAFE_TO_INSERT_TATWEEL:
 *                      flag indicating that the @HB_GLYPH_FLAG_SAFE_TO_INSERT_TATWEEL
 *                      glyph-flag should be produced by the shaper. By default
 *                      it will not be produced. Since: 5.1.0
 * @HB_BUFFER_FLAG_DEFINED: All currently defined flags: Since: 4.4.0
 *
 * Flags for #hb_buffer_t.
 *
 * Since: 0.9.20
 */
قسمتعریف شمار { /*< flags >*/
  HB_بفر_FLAG_DEFAULT			= 0x00000000u,
  HB_بفر_FLAG_BOT				= 0x00000001u, /* Beginning-of-text */
  HB_بفر_FLAG_EOT				= 0x00000002u, /* End-of-text */
  HB_بفر_FLAG_PRESERVE_DEFAULT_IGNORABLES	= 0x00000004u,
  HB_بفر_FLAG_REMOVE_DEFAULT_IGNORABLES	= 0x00000008u,
  HB_بفر_FLAG_DO_NOT_داخل_کرو_DOTTED_CIRCLE	= 0x00000010u,
  HB_بفر_FLAG_VERIFY				= 0x00000020u,
  HB_بفر_FLAG_PRODUCE_UNSAFE_مقصد_CONCAT	= 0x00000040u,
  HB_بفر_FLAG_PRODUCE_محفوظ_مقصد_داخل_کرو_TATWEEL	= 0x00000080u,

  HB_بفر_FLAG_DEFINED			= 0x000000FFu
} hb_بفر_flags_t;

HB_EXTERN خالی
hb_بفر_سیٹ_flags (hb_بفر_t       *بفر,
		     hb_بفر_flags_t  flags);

HB_EXTERN hb_بفر_flags_t
hb_بفر_get_flags (ثابت hb_بفر_t *بفر);

/**
 * hb_buffer_cluster_level_t:
 * @HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES: Return cluster values grouped by graphemes into
 *   monotone order.
 * @HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS: Return cluster values grouped into monotone order.
 * @HB_BUFFER_CLUSTER_LEVEL_CHARACTERS: Don't group cluster values.
 * @HB_BUFFER_CLUSTER_LEVEL_DEFAULT: Default cluster level,
 *   equal to @HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES.
 * @HB_BUFFER_CLUSTER_LEVEL_GRAPHEMES: Only group clusters, but don't enforce monotone order.
 *
 * Data type for holding HarfBuzz's clustering behavior options. The cluster level
 * dictates one aspect of how HarfBuzz will treat non-base characters
 * during shaping.
 *
 * In @HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES, non-base
 * characters are merged into the cluster of the base character that precedes them.
 * There is also cluster merging every time the clusters will otherwise become non-monotone.
 *
 * In @HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS, non-base characters are initially
 * assigned their own cluster values, which are not merged into preceding base
 * clusters. This allows HarfBuzz to perform additional operations like reorder
 * sequences of adjacent marks. The output is still monotone, but the cluster
 * values are more granular.
 *
 * In @HB_BUFFER_CLUSTER_LEVEL_CHARACTERS, non-base characters are assigned their
 * own cluster values, which are not merged into preceding base clusters. Moreover,
 * the cluster values are not merged into monotone order. This is the most granular
 * cluster level, and it is useful for clients that need to know the exact cluster
 * values of each character, but is harder to use for clients, since clusters
 * might appear in any order.
 *
 * In @HB_BUFFER_CLUSTER_LEVEL_GRAPHEMES, non-base characters are merged into the
 * cluster of the base character that precedes them. This is similar to the Unicode
 * Grapheme Cluster algorithm, but it is not exactly the same. The output is
 * not forced to be monotone. This is useful for clients that want to use HarfBuzz
 * as a cheap implementation of the Unicode Grapheme Cluster algorithm.
 *
 * @HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES is the default, because it maintains
 * backward compatibility with older versions of HarfBuzz. New client programs that
 * do not need to maintain such backward compatibility are recommended to use
 * @HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS instead of the default.
 *
 * Since: 0.9.42
 */
قسمتعریف شمار {
  HB_بفر_CLUSTER_LEVEL_MONOTONE_GRAPHEMES	= 0,
  HB_بفر_CLUSTER_LEVEL_MONOTONE_CHARACTERS	= 1,
  HB_بفر_CLUSTER_LEVEL_CHARACTERS		= 2,
  HB_بفر_CLUSTER_LEVEL_GRAPHEMES		= 3,
  HB_بفر_CLUSTER_LEVEL_DEFAULT = HB_بفر_CLUSTER_LEVEL_MONOTONE_GRAPHEMES
} hb_بفر_cluster_level_t;

/**
 * HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE:
 * @level: #hb_buffer_cluster_level_t to test
 *
 * Tests whether a cluster level groups cluster values into monotone order.
 * Requires that the level be valid.
 *
 * Since: 11.0.0
 */
#تعریف_کرو HB_بفر_CLUSTER_LEVEL_IS_MONOTONE(level) \
	((bool) ((1u << (غیرعلامتی) (level)) & \
		 ((1u << HB_بفر_CLUSTER_LEVEL_MONOTONE_GRAPHEMES) | \
		  (1u << HB_بفر_CLUSTER_LEVEL_MONOTONE_CHARACTERS))))

/**
 * HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES:
 * @level: #hb_buffer_cluster_level_t to test
 *
 * Tests whether a cluster level groups cluster values by graphemes. Requires
 * that the level be valid.
 *
 * Since: 11.0.0
 */
#تعریف_کرو HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES(level) \
	((bool) ((1u << (غیرعلامتی) (level)) & \
		 ((1u << HB_بفر_CLUSTER_LEVEL_MONOTONE_GRAPHEMES) | \
		  (1u << HB_بفر_CLUSTER_LEVEL_GRAPHEMES))))

/**
 * HB_BUFFER_CLUSTER_LEVEL_IS_CHARACTERS:
 * @level: #hb_buffer_cluster_level_t to test
 *
 * Tests whether a cluster level does not group cluster values by graphemes.
 * Requires that the level be valid.
 *
 * Since: 11.0.0
 */
#تعریف_کرو HB_بفر_CLUSTER_LEVEL_IS_CHARACTERS(level) \
	((bool) ((1u << (غیرعلامتی) (level)) & \
		 ((1u << HB_بفر_CLUSTER_LEVEL_MONOTONE_CHARACTERS) | \
		  (1u << HB_بفر_CLUSTER_LEVEL_CHARACTERS))))

HB_EXTERN خالی
hb_بفر_سیٹ_cluster_level (hb_بفر_t               *بفر,
			     hb_بفر_cluster_level_t  cluster_level);

HB_EXTERN hb_بفر_cluster_level_t
hb_بفر_get_cluster_level (ثابت hb_بفر_t *بفر);

/**
 * HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT:
 *
 * The default code point for replacing invalid characters in a given encoding.
 * Set to U+FFFD REPLACEMENT CHARACTER.
 *
 * Since: 0.9.31
 */
#تعریف_کرو HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT 0xFFFDu

HB_EXTERN خالی
hb_بفر_سیٹ_replacement_codepoint (hb_بفر_t    *بفر,
				     hb_codepoint_t  replacement);

HB_EXTERN hb_codepoint_t
hb_بفر_get_replacement_codepoint (ثابت hb_بفر_t *بفر);

HB_EXTERN خالی
hb_بفر_سیٹ_invisible_glyph (hb_بفر_t    *بفر,
			       hb_codepoint_t  invisible);

HB_EXTERN hb_codepoint_t
hb_بفر_get_invisible_glyph (ثابت hb_بفر_t *بفر);

HB_EXTERN خالی
hb_بفر_سیٹ_not_ملا_glyph (hb_بفر_t    *بفر,
			       hb_codepoint_t  not_ملا);

HB_EXTERN hb_codepoint_t
hb_بفر_get_not_ملا_glyph (ثابت hb_بفر_t *بفر);

HB_EXTERN خالی
hb_بفر_سیٹ_not_ملا_variation_selector_glyph (hb_بفر_t    *بفر,
						  hb_codepoint_t  not_ملا_variation_selector);

HB_EXTERN hb_codepoint_t
hb_بفر_get_not_ملا_variation_selector_glyph (ثابت hb_بفر_t *بفر);

HB_EXTERN خالی
hb_بفر_سیٹ_random_state (hb_بفر_t    *بفر,
			    غیرعلامتی        state);

HB_EXTERN غیرعلامتی
hb_بفر_get_random_state (ثابت hb_بفر_t *بفر);

/*
 * Content API.
 */

HB_EXTERN خالی
hb_بفر_clear_contents (hb_بفر_t *بفر);

HB_EXTERN hb_bool_t
hb_بفر_pre_allocate (hb_بفر_t  *بفر,
			غیرعلامتی صحیح  سائز);


HB_EXTERN hb_bool_t
hb_بفر_تخصیص_successful (hb_بفر_t  *بفر);

HB_EXTERN خالی
hb_بفر_الٹ (hb_بفر_t *بفر);

HB_EXTERN خالی
hb_بفر_الٹ_range (hb_بفر_t *بفر,
			 غیرعلامتی صحیح آغاز, غیرعلامتی صحیح انتہا);

HB_EXTERN خالی
hb_بفر_الٹ_clusters (hb_بفر_t *بفر);


/* Filling the buffer in */

HB_EXTERN خالی
hb_بفر_جوڑو (hb_بفر_t    *بفر,
	       hb_codepoint_t  codepoint,
	       غیرعلامتی صحیح    cluster);

HB_EXTERN خالی
hb_بفر_جوڑو_utf8 (hb_بفر_t  *بفر,
		    ثابت حرف   *متن,
		    صحیح           متن_لمبائی,
		    غیرعلامتی صحیح  شے_انحراف,
		    صحیح           شے_لمبائی);

HB_EXTERN خالی
hb_بفر_جوڑو_utf16 (hb_بفر_t    *بفر,
		     ثابت uint16_t *متن,
		     صحیح             متن_لمبائی,
		     غیرعلامتی صحیح    شے_انحراف,
		     صحیح             شے_لمبائی);

HB_EXTERN خالی
hb_بفر_جوڑو_utf32 (hb_بفر_t    *بفر,
		     ثابت uint32_t *متن,
		     صحیح             متن_لمبائی,
		     غیرعلامتی صحیح    شے_انحراف,
		     صحیح             شے_لمبائی);

HB_EXTERN خالی
hb_بفر_جوڑو_latin1 (hb_بفر_t   *بفر,
		      ثابت uint8_t *متن,
		      صحیح            متن_لمبائی,
		      غیرعلامتی صحیح   شے_انحراف,
		      صحیح            شے_لمبائی);

HB_EXTERN خالی
hb_بفر_جوڑو_codepoints (hb_بفر_t          *بفر,
			  ثابت hb_codepoint_t *متن,
			  صحیح                   متن_لمبائی,
			  غیرعلامتی صحیح          شے_انحراف,
			  صحیح                   شے_لمبائی);

HB_EXTERN خالی
hb_بفر_append (hb_بفر_t *بفر,
		  ثابت hb_بفر_t *ماخذ,
		  غیرعلامتی صحیح آغاز,
		  غیرعلامتی صحیح انتہا);

HB_EXTERN hb_bool_t
hb_بفر_سیٹ_لمبائی (hb_بفر_t  *بفر,
		      غیرعلامتی صحیح  لمبائی);

HB_EXTERN غیرعلامتی صحیح
hb_بفر_get_لمبائی (ثابت hb_بفر_t *بفر);

/* Getting glyphs out of the buffer */

HB_EXTERN hb_glyph_info_t *
hb_بفر_get_glyph_infos (hb_بفر_t  *بفر,
			   غیرعلامتی صحیح *لمبائی);

HB_EXTERN hb_glyph_position_t *
hb_بفر_get_glyph_positions (hb_بفر_t  *بفر,
			       غیرعلامتی صحیح *لمبائی);

HB_EXTERN hb_bool_t
hb_بفر_has_positions (hb_بفر_t  *بفر);


HB_EXTERN خالی
hb_بفر_normalize_glyphs (hb_بفر_t *بفر);


/*
 * Serialize
 */

/**
 * hb_buffer_serialize_flags_t:
 * @HB_BUFFER_SERIALIZE_FLAG_DEFAULT: serialize glyph names, clusters and positions.
 * @HB_BUFFER_SERIALIZE_FLAG_NO_CLUSTERS: do not serialize glyph cluster.
 * @HB_BUFFER_SERIALIZE_FLAG_NO_POSITIONS: do not serialize glyph position information.
 * @HB_BUFFER_SERIALIZE_FLAG_NO_GLYPH_NAMES: do no serialize glyph name.
 * @HB_BUFFER_SERIALIZE_FLAG_GLYPH_EXTENTS: serialize glyph extents.
 * @HB_BUFFER_SERIALIZE_FLAG_GLYPH_FLAGS: serialize glyph flags. Since: 1.5.0
 * @HB_BUFFER_SERIALIZE_FLAG_NO_ADVANCES: do not serialize glyph advances,
 *  glyph offsets will reflect absolute glyph positions. Since: 1.8.0.
 *  Note: when this flag is used with a partial range of the buffer (i.e.
 *  @start is not 0), calculating the absolute positions has a cost
 *  proportional to @start. If the buffer is serialized in many small
 *  chunks, this can lead to quadratic behavior. It is recommended to
 *  use a larger @buf_size to minimize this cost.
 * @HB_BUFFER_SERIALIZE_FLAG_DEFINED: All currently defined flags. Since: 4.4.0
 *
 * Flags that control what glyph information are serialized in hb_buffer_serialize_glyphs().
 *
 * Since: 0.9.20
 */
قسمتعریف شمار { /*< flags >*/
  HB_بفر_SERIALIZE_FLAG_DEFAULT		= 0x00000000u,
  HB_بفر_SERIALIZE_FLAG_NO_CLUSTERS		= 0x00000001u,
  HB_بفر_SERIALIZE_FLAG_NO_POSITIONS		= 0x00000002u,
  HB_بفر_SERIALIZE_FLAG_NO_GLYPH_نام	= 0x00000004u,
  HB_بفر_SERIALIZE_FLAG_GLYPH_EXTENTS	= 0x00000008u,
  HB_بفر_SERIALIZE_FLAG_GLYPH_FLAGS		= 0x00000010u,
  HB_بفر_SERIALIZE_FLAG_NO_ADVANCES		= 0x00000020u,

  HB_بفر_SERIALIZE_FLAG_DEFINED		= 0x0000003Fu
} hb_بفر_serialize_flags_t;

/**
 * hb_buffer_serialize_format_t:
 * @HB_BUFFER_SERIALIZE_FORMAT_TEXT: a human-readable, plain text format.
 * @HB_BUFFER_SERIALIZE_FORMAT_JSON: a machine-readable JSON format.
 * @HB_BUFFER_SERIALIZE_FORMAT_INVALID: invalid format.
 *
 * The buffer serialization and de-serialization format used in
 * hb_buffer_serialize_glyphs() and hb_buffer_deserialize_glyphs().
 *
 * Since: 0.9.2
 */
قسمتعریف شمار {
  HB_بفر_SERIALIZE_FORMAT_متن	= HB_لیبل('T','E','X','T'),
  HB_بفر_SERIALIZE_FORMAT_JSON	= HB_لیبل('J','S','O','N'),
  HB_بفر_SERIALIZE_FORMAT_INVALID	= HB_لیبل_NONE
} hb_بفر_serialize_format_t;

HB_EXTERN hb_بفر_serialize_format_t
hb_بفر_serialize_format_from_string (ثابت حرف *str, صحیح len);

HB_EXTERN ثابت حرف *
hb_بفر_serialize_format_مقصد_string (hb_بفر_serialize_format_t format);

HB_EXTERN ثابت حرف **
hb_بفر_serialize_list_formats (خالی);

HB_EXTERN غیرعلامتی صحیح
hb_بفر_serialize_glyphs (hb_بفر_t *بفر,
			    غیرعلامتی صحیح آغاز,
			    غیرعلامتی صحیح انتہا,
			    حرف *buf,
			    غیرعلامتی صحیح buf_سائز,
			    غیرعلامتی صحیح *buf_consumed,
			    hb_font_t *font,
			    hb_بفر_serialize_format_t format,
			    hb_بفر_serialize_flags_t flags);

HB_EXTERN غیرعلامتی صحیح
hb_بفر_serialize_unicode (hb_بفر_t *بفر,
			     غیرعلامتی صحیح آغاز,
			     غیرعلامتی صحیح انتہا,
			     حرف *buf,
			     غیرعلامتی صحیح buf_سائز,
			     غیرعلامتی صحیح *buf_consumed,
			     hb_بفر_serialize_format_t format,
			     hb_بفر_serialize_flags_t flags);

HB_EXTERN غیرعلامتی صحیح
hb_بفر_serialize (hb_بفر_t *بفر,
		     غیرعلامتی صحیح آغاز,
		     غیرعلامتی صحیح انتہا,
		     حرف *buf,
		     غیرعلامتی صحیح buf_سائز,
		     غیرعلامتی صحیح *buf_consumed,
		     hb_font_t *font,
		     hb_بفر_serialize_format_t format,
		     hb_بفر_serialize_flags_t flags);

HB_EXTERN hb_bool_t
hb_بفر_deserialize_glyphs (hb_بفر_t *بفر,
			      ثابت حرف *buf,
			      صحیح buf_len,
			      ثابت حرف **end_ptr,
			      hb_font_t *font,
			      hb_بفر_serialize_format_t format);

HB_EXTERN hb_bool_t
hb_بفر_deserialize_unicode (hb_بفر_t *بفر,
			       ثابت حرف *buf,
			       صحیح buf_len,
			       ثابت حرف **end_ptr,
			       hb_بفر_serialize_format_t format);



/*
 * Compare buffers
 */

/**
 * hb_buffer_diff_flags_t:
 * @HB_BUFFER_DIFF_FLAG_EQUAL: equal buffers.
 * @HB_BUFFER_DIFF_FLAG_CONTENT_TYPE_MISMATCH: buffers with different
 *     #hb_buffer_content_type_t.
 * @HB_BUFFER_DIFF_FLAG_LENGTH_MISMATCH: buffers with differing length.
 * @HB_BUFFER_DIFF_FLAG_NOTDEF_PRESENT: `.notdef` glyph is present in the
 *     reference buffer.
 * @HB_BUFFER_DIFF_FLAG_DOTTED_CIRCLE_PRESENT: dotted circle glyph is present
 *     in the reference buffer.
 * @HB_BUFFER_DIFF_FLAG_CODEPOINT_MISMATCH: difference in #hb_glyph_info_t.codepoint
 * @HB_BUFFER_DIFF_FLAG_CLUSTER_MISMATCH: difference in #hb_glyph_info_t.cluster
 * @HB_BUFFER_DIFF_FLAG_GLYPH_FLAGS_MISMATCH: difference in #hb_glyph_flags_t.
 * @HB_BUFFER_DIFF_FLAG_POSITION_MISMATCH: difference in #hb_glyph_position_t.
 *
 * Flags from comparing two #hb_buffer_t's.
 *
 * Buffer with different #hb_buffer_content_type_t cannot be meaningfully
 * compared in any further detail.
 *
 * For buffers with differing length, the per-glyph comparison is not
 * attempted, though we do still scan reference buffer for dotted circle and
 * `.notdef` glyphs.
 *
 * If the buffers have the same length, we compare them glyph-by-glyph and
 * report which aspect(s) of the glyph info/position are different.
 *
 * Since: 1.5.0
 */
قسمتعریف شمار { /*< flags >*/
  HB_بفر_DIFF_FLAG_EQUAL			= 0x0000,

  /* Buffers with different content_type cannot be meaningfully compared
   * in any further detail. */
  HB_بفر_DIFF_FLAG_CONTENT_TYPE_MISMATCH	= 0x0001,

  /* For buffers with differing length, the per-glyph comparison is not
   * attempted, though we do still scan reference for dottedcircle / .notdef
   * glyphs. */
  HB_بفر_DIFF_FLAG_لمبائی_MISMATCH		= 0x0002,

  /* We want to know if dottedcircle / .notdef glyphs are present in the
   * reference, as we may not care so much about other differences in this
   * case. */
  HB_بفر_DIFF_FLAG_NOTDEF_PRESENT		= 0x0004,
  HB_بفر_DIFF_FLAG_DOTTED_CIRCLE_PRESENT	= 0x0008,

  /* If the buffers have the same length, we compare them glyph-by-glyph
   * and report which aspect(s) of the glyph info/position are different. */
  HB_بفر_DIFF_FLAG_CODEPOINT_MISMATCH	= 0x0010,
  HB_بفر_DIFF_FLAG_CLUSTER_MISMATCH		= 0x0020,
  HB_بفر_DIFF_FLAG_GLYPH_FLAGS_MISMATCH	= 0x0040,
  HB_بفر_DIFF_FLAG_POSITION_MISMATCH		= 0x0080

} hb_بفر_diff_flags_t;

/* Compare the contents of two buffers, report types of differences. */
HB_EXTERN hb_بفر_diff_flags_t
hb_بفر_diff (hb_بفر_t *بفر,
		hb_بفر_t *reference,
		hb_codepoint_t dottedcircle_glyph,
		غیرعلامتی صحیح position_fuzz);


/*
 * Tracing.
 */

/**
 * hb_buffer_message_func_t:
 * @buffer: An #hb_buffer_t to work upon
 * @font: The #hb_font_t the @buffer is shaped with
 * @message: `NULL`-terminated message passed to the function
 * @user_data: User data pointer passed by the caller
 *
 * A callback method for #hb_buffer_t. The method gets called with the
 * #hb_buffer_t it was set on, the #hb_font_t the buffer is shaped with and a
 * message describing what step of the shaping process will be performed.
 * Returning `false` from this method will skip this shaping step and move to
 * the next one.
 *
 * Return value: `true` to perform the shaping step, `false` to skip it.
 *
 * Since: 1.1.3
 */
قسمتعریف hb_bool_t	(*hb_بفر_پیغام_func_t)	(hb_بفر_t *بفر,
							 hb_font_t   *font,
							 ثابت حرف  *پیغام,
							 خالی        *user_ڈیٹا);

HB_EXTERN خالی
hb_بفر_سیٹ_پیغام_func (hb_بفر_t *بفر,
			    hb_بفر_پیغام_func_t func,
			    خالی *user_ڈیٹا, hb_destroy_func_t destroy);

HB_EXTERN خالی
hb_بفر_بدلا (hb_بفر_t *بفر);


HB_انتہا_DECLS

#اگر_ختم /* HB_BUFFER_H */
