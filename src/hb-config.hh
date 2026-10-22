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

#اگر_غیر_تعریف_شدہ HB_CONFIG_HH
#تعریف_کرو HB_CONFIG_HH

#اگر 0 /* Make test happy. */
#شامل "hb.hh"
#اگر_ختم

#اگر_تعریف_شدہ HAVE_CONFIG_H
#شامل "config.h"
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_EXPERIMENTAL_API
#تعریف_کرو HB_NO_BEYOND_64K
#تعریف_کرو HB_NO_CUBIC_GLYF
#تعریف_کرو HB_NO_SUBSET_DEPEND
#اگر_ختم

#اگر_تعریف_شدہ HB_TINY
#تعریف_کرو HB_LEAN
#تعریف_کرو HB_MINI
#تعریف_کرو HB_NO_SVG
#تعریف_کرو HB_OPTIMIZE_سائز
#تعریف_کرو HB_OPTIMIZE_سائز_مزید
#تعریف_کرو HB_MINIMIZE_حافظہ_USAGE
#تعریف_کرو HB_NO_MT
#تعریف_کرو HB_NO_UCD_UNASSIGNED
#اگر_غیر_تعریف_شدہ NDEBUG
#تعریف_کرو NDEBUG
#اگر_ختم
#اگر_ختم

#اگر_تعریف_شدہ HB_LEAN
#تعریف_کرو HB_DISABLE_DEPRECATED
#تعریف_کرو HB_NO_ATEXIT
#تعریف_کرو HB_NO_بفر_پیغام
#تعریف_کرو HB_NO_بفر_SERIALIZE
#تعریف_کرو HB_NO_بفر_VERIFY
#تعریف_کرو HB_NO_BITMAP
#تعریف_کرو HB_NO_ERRNO
#تعریف_کرو HB_NO_FACE_COLLECT_UNICODES
#تعریف_کرو HB_NO_GETENV
#تعریف_کرو HB_NO_HINTING
#تعریف_کرو HB_NO_LAYOUT_FEATURE_PARAMS
#تعریف_کرو HB_NO_LAYOUT_COLLECT_GLYPHS
#تعریف_کرو HB_NO_LAYOUT_RARELY_مستعمل
#تعریف_کرو HB_NO_LAYOUT_غیر_مستعمل
#تعریف_کرو HB_NO_MATH
#تعریف_کرو HB_NO_META
#تعریف_کرو HB_NO_METRICS
#تعریف_کرو HB_NO_MMAP
#تعریف_کرو HB_NO_NAME
#تعریف_کرو HB_NO_OPEN
#تعریف_کرو HB_NO_OT_FETCH
#تعریف_کرو HB_NO_OT_FONT_GLYPH_نام
#تعریف_کرو HB_NO_SETLOCALE
#تعریف_کرو HB_NO_STYLE
#تعریف_کرو HB_NO_SUBSET_DEPEND
#تعریف_کرو HB_NO_VERTICAL
#تعریف_کرو HB_NO_VAR

#اگر !(defined(HB_HAS_CAIRO) || defined(HB_HAS_RASTER) || defined(HB_HAS_سمتیہ) || defined(HB_HAS_GPU))
#تعریف_کرو HB_NO_COLOR
#تعریف_کرو HB_NO_DRAW
#تعریف_کرو HB_NO_PAINT
#اگر_ختم

#اگر_ختم

#اگر_تعریف_شدہ HB_MINI
#تعریف_کرو HB_NO_AAT
#تعریف_کرو HB_NO_LEGACY
#تعریف_کرو HB_NO_BEYOND_64K
#تعریف_کرو HB_NO_CUBIC_GLYF
#اگر_ختم

#اگر_تعریف_شدہ __OPTIMIZE_SIZE__
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
#تعریف_کرو HB_OPTIMIZE_سائز
#اگر_ختم
#اگر_ختم

#اگر defined(HAVE_CONFIG_OVERRIDE_H) || defined(HB_CONFIG_OVERRIDE_H)
#اگر_غیر_تعریف_شدہ HB_CONFIG_OVERRIDE_H
#تعریف_کرو HB_CONFIG_OVERRIDE_H "config-override.h"
#اگر_ختم
#شامل HB_CONFIG_OVERRIDE_H
#اگر_ختم

/* Closure of options. */

#اگر_تعریف_شدہ HB_NO_VAR
#تعریف_کرو HB_NO_VAR_COMPOSITES
#اگر_ختم

#اگر_تعریف_شدہ HB_DISABLE_DEPRECATED
#تعریف_کرو HB_IF_NOT_DEPRECATED(x)
#ورنہ
#تعریف_کرو HB_IF_NOT_DEPRECATED(x) x
#اگر_ختم

#اگر_تعریف_شدہ HB_NO_SHAPER
#تعریف_کرو HB_NO_OT_SHAPE
#تعریف_کرو HB_NO_AAT_SHAPE
#اگر_ختم

#اگر_تعریف_شدہ HB_NO_AAT
#تعریف_کرو HB_NO_OT_NAME_LANGUAGE_AAT
#تعریف_کرو HB_NO_AAT_SHAPE
#اگر_ختم

#اگر_تعریف_شدہ HB_NO_BITMAP
#تعریف_کرو HB_NO_OT_FONT_BITMAP
#اگر_ختم

#اگر_تعریف_شدہ HB_NO_DRAW
#تعریف_کرو HB_NO_OT_FONT_CFF
#تعریف_کرو HB_NO_OUTLINE
#تعریف_کرو HB_NO_PAINT
#اگر_ختم

#اگر_تعریف_شدہ HB_NO_CFF
#تعریف_کرو HB_NO_OT_FONT_CFF
#تعریف_کرو HB_NO_SUBSET_CFF
#اگر_ختم

#اگر_تعریف_شدہ HB_NO_LEGACY
#تعریف_کرو HB_NO_CMAP_LEGACY_SUBTABLES
#تعریف_کرو HB_NO_FALLBACK_SHAPE
#تعریف_کرو HB_NO_OT_KERN
#تعریف_کرو HB_NO_OT_LAYOUT_BLOCKLIST
#تعریف_کرو HB_NO_AAT_LAYOUT_BLOCKLIST
#تعریف_کرو HB_NO_OT_SHAPE_FALLBACK
#اگر_ختم

#اگر_تعریف_شدہ HB_NO_NAME
#تعریف_کرو HB_NO_OT_NAME_LANGUAGE
#اگر_ختم

#اگر_تعریف_شدہ HB_NO_OT
#تعریف_کرو HB_NO_OT_FONT
#تعریف_کرو HB_NO_OT_LAYOUT
#تعریف_کرو HB_NO_OT_لیبل
#تعریف_کرو HB_NO_OT_SHAPE
#اگر_ختم

#اگر_تعریف_شدہ HB_NO_OT_SHAPE
#تعریف_کرو HB_NO_AAT_SHAPE
#اگر_ختم

#اگر_تعریف_شدہ HB_NO_OT_SHAPE_FALLBACK
#تعریف_کرو HB_NO_OT_SHAPER_ARABIC_FALLBACK
#تعریف_کرو HB_NO_OT_SHAPER_HEBREW_FALLBACK
#تعریف_کرو HB_NO_OT_SHAPER_THAI_FALLBACK
#تعریف_کرو HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS
#تعریف_کرو HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#اگر_ختم

#اگر_تعریف_شدہ HB_OPTIMIZE_سائز_مزید
#تعریف_کرو HB_NO_OT_RULESETS_FAST_راستہ
#اگر_ختم

#اگر_تعریف_شدہ HB_MINIMIZE_حافظہ_USAGE
#تعریف_کرو HB_NO_GDEF_CACHE
#تعریف_کرو HB_NO_OT_LAYOUT_LOOKUP_CACHE
#تعریف_کرو HB_NO_OT_FONT_CMAP_CACHE
#اگر_ختم

#اگر defined(HAVE_CONFIG_OVERRIDE_آخری_H) || defined(HB_CONFIG_OVERRIDE_آخری_H)
#اگر_غیر_تعریف_شدہ HB_CONFIG_OVERRIDE_آخری_H
#تعریف_کرو HB_CONFIG_OVERRIDE_آخری_H "config-override-last.h"
#اگر_ختم
#شامل HB_CONFIG_OVERRIDE_آخری_H
#اگر_ختم

#اگر_تعریف_شدہ HB_OPTIMIZE_سائز
#تعریف_کرو HB_OPTIMIZE_سائز_VAL 1
#ورنہ
#تعریف_کرو HB_OPTIMIZE_سائز_VAL 0
#اگر_ختم

#اگر_تعریف_شدہ HB_MINIMIZE_حافظہ_USAGE
#تعریف_کرو HB_MINIMIZE_حافظہ_USAGE_VAL 1
#ورنہ
#تعریف_کرو HB_MINIMIZE_حافظہ_USAGE_VAL 0
#اگر_ختم

#اگر_ختم /* HB_CONFIG_HH */
