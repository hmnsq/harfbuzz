/*
 * Copyright © 2019  Facebook, Inc.
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
 * Facebook Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CONFIG_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CONFIG_HH

#ਜੇ 0 /* Make test happy. */
#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CONFIG_H
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_BEYOND_64K
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_CUBIC_GLYF
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_SUBSET_DEPEND
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_TINY
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_LEAN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MINI
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_SVG
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OPTIMIZE_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OPTIMIZE_ਮਾਪ_ਹੋਰ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MINIMIZE_ਸਮ੍ਰਿਤੀ_USAGE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_MT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_UCD_UNASSIGNED
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NDEBUG
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NDEBUG
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_LEAN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DISABLE_DEPRECATED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_ATEXIT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_ਬਫਰ_ਸੁਨੇਹਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_ਬਫਰ_SERIALIZE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_ਬਫਰ_VERIFY
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_BITMAP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_ERRNO
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_FACE_COLLECT_UNICODES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_GETENV
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_HINTING
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_LAYOUT_FEATURE_PARAMS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_LAYOUT_COLLECT_GLYPHS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_LAYOUT_RARELY_ਵਰਤਿਆ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_LAYOUT_ਅਣਵਰਤਿਆ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_MATH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_META
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_METRICS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_MMAP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_NAME
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OPEN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_FETCH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_FONT_GLYPH_ਨਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_SETLOCALE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_STYLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_SUBSET_DEPEND
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_VERTICAL
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_VAR

#ਜੇ !(defined(HB_HAS_CAIRO) || defined(HB_HAS_RASTER) || defined(HB_HAS_ਸਦਿਸ਼) || defined(HB_HAS_GPU))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_COLOR
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_DRAW
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_PAINT
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_MINI
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_AAT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_LEGACY
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_BEYOND_64K
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_CUBIC_GLYF
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __OPTIMIZE_SIZE__
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OPTIMIZE_ਮਾਪ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ defined(HAVE_CONFIG_OVERRIDE_H) || defined(HB_CONFIG_OVERRIDE_H)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CONFIG_OVERRIDE_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CONFIG_OVERRIDE_H "config-override.h"
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ HB_CONFIG_OVERRIDE_H
#ਜੇ_ਅੰਤ

/* Closure of options. */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_VAR_COMPOSITES
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_IF_NOT_DEPRECATED(x)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_IF_NOT_DEPRECATED(x) x
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_SHAPER
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_SHAPE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_AAT_SHAPE
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_NAME_LANGUAGE_AAT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_AAT_SHAPE
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_BITMAP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_FONT_BITMAP
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_FONT_CFF
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OUTLINE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_PAINT
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_CFF
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_FONT_CFF
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_SUBSET_CFF
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_LEGACY
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_CMAP_LEGACY_SUBTABLES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_FALLBACK_SHAPE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_KERN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_LAYOUT_BLOCKLIST
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_AAT_LAYOUT_BLOCKLIST
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_SHAPE_FALLBACK
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_NAME
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_NAME_LANGUAGE
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_FONT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_LAYOUT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_ਟੈਗ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_SHAPE
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_AAT_SHAPE
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE_FALLBACK
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_SHAPER_ARABIC_FALLBACK
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_SHAPER_HEBREW_FALLBACK
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_SHAPER_THAI_FALLBACK
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ_ਹੋਰ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_RULESETS_FAST_ਰਸਤਾ
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_MINIMIZE_ਸਮ੍ਰਿਤੀ_USAGE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_GDEF_CACHE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_LAYOUT_LOOKUP_CACHE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_OT_FONT_CMAP_CACHE
#ਜੇ_ਅੰਤ

#ਜੇ defined(HAVE_CONFIG_OVERRIDE_ਆਖਰੀ_H) || defined(HB_CONFIG_OVERRIDE_ਆਖਰੀ_H)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CONFIG_OVERRIDE_ਆਖਰੀ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CONFIG_OVERRIDE_ਆਖਰੀ_H "config-override-last.h"
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ HB_CONFIG_OVERRIDE_ਆਖਰੀ_H
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OPTIMIZE_ਮਾਪ_VAL 1
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OPTIMIZE_ਮਾਪ_VAL 0
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_MINIMIZE_ਸਮ੍ਰਿਤੀ_USAGE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MINIMIZE_ਸਮ੍ਰਿਤੀ_USAGE_VAL 1
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MINIMIZE_ਸਮ੍ਰਿਤੀ_USAGE_VAL 0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* HB_CONFIG_HH */
