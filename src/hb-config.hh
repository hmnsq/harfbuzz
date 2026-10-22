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

#که_نه_تعریف_شوی HB_CONFIG_HH
#تعریف_کړه HB_CONFIG_HH

#إذا 0 /* Make test happy. */
#ضمن "hb.hh"
#د_که_پای

#که_تعریف_شوی HAVE_CONFIG_H
#ضمن "config.h"
#د_که_پای

#که_نه_تعریف_شوی HB_EXPERIMENTAL_API
#تعریف_کړه HB_NO_BEYOND_64K
#تعریف_کړه HB_NO_CUBIC_GLYF
#تعریف_کړه HB_NO_SUBSET_DEPEND
#د_که_پای

#که_تعریف_شوی HB_TINY
#تعریف_کړه HB_LEAN
#تعریف_کړه HB_MINI
#تعریف_کړه HB_NO_SVG
#تعریف_کړه HB_OPTIMIZE_اندازه
#تعریف_کړه HB_OPTIMIZE_اندازه_نور
#تعریف_کړه HB_MINIMIZE_حافظه_USAGE
#تعریف_کړه HB_NO_MT
#تعریف_کړه HB_NO_UCD_UNASSIGNED
#که_نه_تعریف_شوی NDEBUG
#تعریف_کړه NDEBUG
#د_که_پای
#د_که_پای

#که_تعریف_شوی HB_LEAN
#تعریف_کړه HB_DISABLE_DEPRECATED
#تعریف_کړه HB_NO_ATEXIT
#تعریف_کړه HB_NO_بفر_پیغام
#تعریف_کړه HB_NO_بفر_SERIALIZE
#تعریف_کړه HB_NO_بفر_VERIFY
#تعریف_کړه HB_NO_BITMAP
#تعریف_کړه HB_NO_ERRNO
#تعریف_کړه HB_NO_FACE_COLLECT_UNICODES
#تعریف_کړه HB_NO_GETENV
#تعریف_کړه HB_NO_HINTING
#تعریف_کړه HB_NO_LAYOUT_FEATURE_PARAMS
#تعریف_کړه HB_NO_LAYOUT_COLLECT_GLYPHS
#تعریف_کړه HB_NO_LAYOUT_RARELY_کارول_شوی
#تعریف_کړه HB_NO_LAYOUT_ناکارول_شوی
#تعریف_کړه HB_NO_MATH
#تعریف_کړه HB_NO_META
#تعریف_کړه HB_NO_METRICS
#تعریف_کړه HB_NO_MMAP
#تعریف_کړه HB_NO_NAME
#تعریف_کړه HB_NO_OPEN
#تعریف_کړه HB_NO_OT_FETCH
#تعریف_کړه HB_NO_OT_FONT_GLYPH_نومونه
#تعریف_کړه HB_NO_SETLOCALE
#تعریف_کړه HB_NO_STYLE
#تعریف_کړه HB_NO_SUBSET_DEPEND
#تعریف_کړه HB_NO_VERTICAL
#تعریف_کړه HB_NO_VAR

#إذا !(defined(HB_HAS_CAIRO) || defined(HB_HAS_RASTER) || defined(HB_HAS_ویکټور) || defined(HB_HAS_GPU))
#تعریف_کړه HB_NO_COLOR
#تعریف_کړه HB_NO_DRAW
#تعریف_کړه HB_NO_PAINT
#د_که_پای

#د_که_پای

#که_تعریف_شوی HB_MINI
#تعریف_کړه HB_NO_AAT
#تعریف_کړه HB_NO_LEGACY
#تعریف_کړه HB_NO_BEYOND_64K
#تعریف_کړه HB_NO_CUBIC_GLYF
#د_که_پای

#که_تعریف_شوی __OPTIMIZE_SIZE__
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
#تعریف_کړه HB_OPTIMIZE_اندازه
#د_که_پای
#د_که_پای

#إذا defined(HAVE_CONFIG_OVERRIDE_H) || defined(HB_CONFIG_OVERRIDE_H)
#که_نه_تعریف_شوی HB_CONFIG_OVERRIDE_H
#تعریف_کړه HB_CONFIG_OVERRIDE_H "config-override.h"
#د_که_پای
#ضمن HB_CONFIG_OVERRIDE_H
#د_که_پای

/* Closure of options. */

#که_تعریف_شوی HB_NO_VAR
#تعریف_کړه HB_NO_VAR_COMPOSITES
#د_که_پای

#که_تعریف_شوی HB_DISABLE_DEPRECATED
#تعریف_کړه HB_IF_NOT_DEPRECATED(x)
#وإلا
#تعریف_کړه HB_IF_NOT_DEPRECATED(x) x
#د_که_پای

#که_تعریف_شوی HB_NO_SHAPER
#تعریف_کړه HB_NO_OT_SHAPE
#تعریف_کړه HB_NO_AAT_SHAPE
#د_که_پای

#که_تعریف_شوی HB_NO_AAT
#تعریف_کړه HB_NO_OT_NAME_LANGUAGE_AAT
#تعریف_کړه HB_NO_AAT_SHAPE
#د_که_پای

#که_تعریف_شوی HB_NO_BITMAP
#تعریف_کړه HB_NO_OT_FONT_BITMAP
#د_که_پای

#که_تعریف_شوی HB_NO_DRAW
#تعریف_کړه HB_NO_OT_FONT_CFF
#تعریف_کړه HB_NO_OUTLINE
#تعریف_کړه HB_NO_PAINT
#د_که_پای

#که_تعریف_شوی HB_NO_CFF
#تعریف_کړه HB_NO_OT_FONT_CFF
#تعریف_کړه HB_NO_SUBSET_CFF
#د_که_پای

#که_تعریف_شوی HB_NO_LEGACY
#تعریف_کړه HB_NO_CMAP_LEGACY_SUBTABLES
#تعریف_کړه HB_NO_FALLBACK_SHAPE
#تعریف_کړه HB_NO_OT_KERN
#تعریف_کړه HB_NO_OT_LAYOUT_BLOCKLIST
#تعریف_کړه HB_NO_AAT_LAYOUT_BLOCKLIST
#تعریف_کړه HB_NO_OT_SHAPE_FALLBACK
#د_که_پای

#که_تعریف_شوی HB_NO_NAME
#تعریف_کړه HB_NO_OT_NAME_LANGUAGE
#د_که_پای

#که_تعریف_شوی HB_NO_OT
#تعریف_کړه HB_NO_OT_FONT
#تعریف_کړه HB_NO_OT_LAYOUT
#تعریف_کړه HB_NO_OT_نښان
#تعریف_کړه HB_NO_OT_SHAPE
#د_که_پای

#که_تعریف_شوی HB_NO_OT_SHAPE
#تعریف_کړه HB_NO_AAT_SHAPE
#د_که_پای

#که_تعریف_شوی HB_NO_OT_SHAPE_FALLBACK
#تعریف_کړه HB_NO_OT_SHAPER_ARABIC_FALLBACK
#تعریف_کړه HB_NO_OT_SHAPER_HEBREW_FALLBACK
#تعریف_کړه HB_NO_OT_SHAPER_THAI_FALLBACK
#تعریف_کړه HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS
#تعریف_کړه HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#د_که_پای

#که_تعریف_شوی HB_OPTIMIZE_اندازه_نور
#تعریف_کړه HB_NO_OT_RULESETS_FAST_لار
#د_که_پای

#که_تعریف_شوی HB_MINIMIZE_حافظه_USAGE
#تعریف_کړه HB_NO_GDEF_CACHE
#تعریف_کړه HB_NO_OT_LAYOUT_LOOKUP_CACHE
#تعریف_کړه HB_NO_OT_FONT_CMAP_CACHE
#د_که_پای

#إذا defined(HAVE_CONFIG_OVERRIDE_وروستی_H) || defined(HB_CONFIG_OVERRIDE_وروستی_H)
#که_نه_تعریف_شوی HB_CONFIG_OVERRIDE_وروستی_H
#تعریف_کړه HB_CONFIG_OVERRIDE_وروستی_H "config-override-last.h"
#د_که_پای
#ضمن HB_CONFIG_OVERRIDE_وروستی_H
#د_که_پای

#که_تعریف_شوی HB_OPTIMIZE_اندازه
#تعریف_کړه HB_OPTIMIZE_اندازه_VAL 1
#وإلا
#تعریف_کړه HB_OPTIMIZE_اندازه_VAL 0
#د_که_پای

#که_تعریف_شوی HB_MINIMIZE_حافظه_USAGE
#تعریف_کړه HB_MINIMIZE_حافظه_USAGE_VAL 1
#وإلا
#تعریف_کړه HB_MINIMIZE_حافظه_USAGE_VAL 0
#د_که_پای

#د_که_پای /* HB_CONFIG_HH */
