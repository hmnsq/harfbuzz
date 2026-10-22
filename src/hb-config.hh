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

#اگر_تعریف_نشده HB_CONFIG_HH
#تعریف_کن HB_CONFIG_HH

#إذا 0 /* Make test happy. */
#ضمن "hb.hh"
#پایان_اگر

#اگر_تعریف_شده HAVE_CONFIG_H
#ضمن "config.h"
#پایان_اگر

#اگر_تعریف_نشده HB_EXPERIMENTAL_API
#تعریف_کن HB_NO_BEYOND_64K
#تعریف_کن HB_NO_CUBIC_GLYF
#تعریف_کن HB_NO_SUBSET_DEPEND
#پایان_اگر

#اگر_تعریف_شده HB_TINY
#تعریف_کن HB_LEAN
#تعریف_کن HB_MINI
#تعریف_کن HB_NO_SVG
#تعریف_کن HB_OPTIMIZE_اندازه
#تعریف_کن HB_OPTIMIZE_اندازه_بیشتر
#تعریف_کن HB_MINIMIZE_حافظه_USAGE
#تعریف_کن HB_NO_MT
#تعریف_کن HB_NO_UCD_UNASSIGNED
#اگر_تعریف_نشده NDEBUG
#تعریف_کن NDEBUG
#پایان_اگر
#پایان_اگر

#اگر_تعریف_شده HB_LEAN
#تعریف_کن HB_DISABLE_DEPRECATED
#تعریف_کن HB_NO_ATEXIT
#تعریف_کن HB_NO_میانگیر_پیام
#تعریف_کن HB_NO_میانگیر_SERIALIZE
#تعریف_کن HB_NO_میانگیر_VERIFY
#تعریف_کن HB_NO_BITMAP
#تعریف_کن HB_NO_ERRNO
#تعریف_کن HB_NO_FACE_COLLECT_UNICODES
#تعریف_کن HB_NO_GETENV
#تعریف_کن HB_NO_HINTING
#تعریف_کن HB_NO_LAYOUT_FEATURE_PARAMS
#تعریف_کن HB_NO_LAYOUT_COLLECT_GLYPHS
#تعریف_کن HB_NO_LAYOUT_RARELY_استفاده_شده
#تعریف_کن HB_NO_LAYOUT_استفادهنشده
#تعریف_کن HB_NO_MATH
#تعریف_کن HB_NO_META
#تعریف_کن HB_NO_METRICS
#تعریف_کن HB_NO_MMAP
#تعریف_کن HB_NO_NAME
#تعریف_کن HB_NO_OPEN
#تعریف_کن HB_NO_OT_FETCH
#تعریف_کن HB_NO_OT_FONT_GLYPH_نامها
#تعریف_کن HB_NO_SETLOCALE
#تعریف_کن HB_NO_STYLE
#تعریف_کن HB_NO_SUBSET_DEPEND
#تعریف_کن HB_NO_VERTICAL
#تعریف_کن HB_NO_VAR

#إذا !(defined(HB_HAS_CAIRO) || defined(HB_HAS_RASTER) || defined(HB_HAS_وکتور) || defined(HB_HAS_GPU))
#تعریف_کن HB_NO_COLOR
#تعریف_کن HB_NO_DRAW
#تعریف_کن HB_NO_PAINT
#پایان_اگر

#پایان_اگر

#اگر_تعریف_شده HB_MINI
#تعریف_کن HB_NO_AAT
#تعریف_کن HB_NO_LEGACY
#تعریف_کن HB_NO_BEYOND_64K
#تعریف_کن HB_NO_CUBIC_GLYF
#پایان_اگر

#اگر_تعریف_شده __OPTIMIZE_SIZE__
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
#تعریف_کن HB_OPTIMIZE_اندازه
#پایان_اگر
#پایان_اگر

#إذا defined(HAVE_CONFIG_OVERRIDE_H) || defined(HB_CONFIG_OVERRIDE_H)
#اگر_تعریف_نشده HB_CONFIG_OVERRIDE_H
#تعریف_کن HB_CONFIG_OVERRIDE_H "config-override.h"
#پایان_اگر
#ضمن HB_CONFIG_OVERRIDE_H
#پایان_اگر

/* Closure of options. */

#اگر_تعریف_شده HB_NO_VAR
#تعریف_کن HB_NO_VAR_COMPOSITES
#پایان_اگر

#اگر_تعریف_شده HB_DISABLE_DEPRECATED
#تعریف_کن HB_IF_NOT_DEPRECATED(x)
#وإلا
#تعریف_کن HB_IF_NOT_DEPRECATED(x) x
#پایان_اگر

#اگر_تعریف_شده HB_NO_SHAPER
#تعریف_کن HB_NO_OT_SHAPE
#تعریف_کن HB_NO_AAT_SHAPE
#پایان_اگر

#اگر_تعریف_شده HB_NO_AAT
#تعریف_کن HB_NO_OT_NAME_LANGUAGE_AAT
#تعریف_کن HB_NO_AAT_SHAPE
#پایان_اگر

#اگر_تعریف_شده HB_NO_BITMAP
#تعریف_کن HB_NO_OT_FONT_BITMAP
#پایان_اگر

#اگر_تعریف_شده HB_NO_DRAW
#تعریف_کن HB_NO_OT_FONT_CFF
#تعریف_کن HB_NO_OUTLINE
#تعریف_کن HB_NO_PAINT
#پایان_اگر

#اگر_تعریف_شده HB_NO_CFF
#تعریف_کن HB_NO_OT_FONT_CFF
#تعریف_کن HB_NO_SUBSET_CFF
#پایان_اگر

#اگر_تعریف_شده HB_NO_LEGACY
#تعریف_کن HB_NO_CMAP_LEGACY_SUBTABLES
#تعریف_کن HB_NO_FALLBACK_SHAPE
#تعریف_کن HB_NO_OT_KERN
#تعریف_کن HB_NO_OT_LAYOUT_BLOCKLIST
#تعریف_کن HB_NO_AAT_LAYOUT_BLOCKLIST
#تعریف_کن HB_NO_OT_SHAPE_FALLBACK
#پایان_اگر

#اگر_تعریف_شده HB_NO_NAME
#تعریف_کن HB_NO_OT_NAME_LANGUAGE
#پایان_اگر

#اگر_تعریف_شده HB_NO_OT
#تعریف_کن HB_NO_OT_FONT
#تعریف_کن HB_NO_OT_LAYOUT
#تعریف_کن HB_NO_OT_برچسب
#تعریف_کن HB_NO_OT_SHAPE
#پایان_اگر

#اگر_تعریف_شده HB_NO_OT_SHAPE
#تعریف_کن HB_NO_AAT_SHAPE
#پایان_اگر

#اگر_تعریف_شده HB_NO_OT_SHAPE_FALLBACK
#تعریف_کن HB_NO_OT_SHAPER_ARABIC_FALLBACK
#تعریف_کن HB_NO_OT_SHAPER_HEBREW_FALLBACK
#تعریف_کن HB_NO_OT_SHAPER_THAI_FALLBACK
#تعریف_کن HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS
#تعریف_کن HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#پایان_اگر

#اگر_تعریف_شده HB_OPTIMIZE_اندازه_بیشتر
#تعریف_کن HB_NO_OT_RULESETS_FAST_مسیر
#پایان_اگر

#اگر_تعریف_شده HB_MINIMIZE_حافظه_USAGE
#تعریف_کن HB_NO_GDEF_CACHE
#تعریف_کن HB_NO_OT_LAYOUT_LOOKUP_CACHE
#تعریف_کن HB_NO_OT_FONT_CMAP_CACHE
#پایان_اگر

#إذا defined(HAVE_CONFIG_OVERRIDE_آخرین_H) || defined(HB_CONFIG_OVERRIDE_آخرین_H)
#اگر_تعریف_نشده HB_CONFIG_OVERRIDE_آخرین_H
#تعریف_کن HB_CONFIG_OVERRIDE_آخرین_H "config-override-last.h"
#پایان_اگر
#ضمن HB_CONFIG_OVERRIDE_آخرین_H
#پایان_اگر

#اگر_تعریف_شده HB_OPTIMIZE_اندازه
#تعریف_کن HB_OPTIMIZE_اندازه_VAL 1
#وإلا
#تعریف_کن HB_OPTIMIZE_اندازه_VAL 0
#پایان_اگر

#اگر_تعریف_شده HB_MINIMIZE_حافظه_USAGE
#تعریف_کن HB_MINIMIZE_حافظه_USAGE_VAL 1
#وإلا
#تعریف_کن HB_MINIMIZE_حافظه_USAGE_VAL 0
#پایان_اگر

#پایان_اگر /* HB_CONFIG_HH */
