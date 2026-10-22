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

#جيڪڏهن_وصف_نه_ٿيل HB_CONFIG_HH
#وصف_ڪريو HB_CONFIG_HH

#إذا 0 /* Make test happy. */
#ضمن "hb.hh"
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_CONFIG_H
#ضمن "config.h"
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_EXPERIMENTAL_API
#وصف_ڪريو HB_NO_BEYOND_64K
#وصف_ڪريو HB_NO_CUBIC_GLYF
#وصف_ڪريو HB_NO_SUBSET_DEPEND
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_TINY
#وصف_ڪريو HB_LEAN
#وصف_ڪريو HB_MINI
#وصف_ڪريو HB_NO_SVG
#وصف_ڪريو HB_OPTIMIZE_سائيز
#وصف_ڪريو HB_OPTIMIZE_سائيز_وڌيڪ
#وصف_ڪريو HB_MINIMIZE_يادگيري_USAGE
#وصف_ڪريو HB_NO_MT
#وصف_ڪريو HB_NO_UCD_UNASSIGNED
#جيڪڏهن_وصف_نه_ٿيل NDEBUG
#وصف_ڪريو NDEBUG
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_LEAN
#وصف_ڪريو HB_DISABLE_DEPRECATED
#وصف_ڪريو HB_NO_ATEXIT
#وصف_ڪريو HB_NO_بفر_پيغام
#وصف_ڪريو HB_NO_بفر_SERIALIZE
#وصف_ڪريو HB_NO_بفر_VERIFY
#وصف_ڪريو HB_NO_BITMAP
#وصف_ڪريو HB_NO_ERRNO
#وصف_ڪريو HB_NO_FACE_COLLECT_UNICODES
#وصف_ڪريو HB_NO_GETENV
#وصف_ڪريو HB_NO_HINTING
#وصف_ڪريو HB_NO_LAYOUT_FEATURE_PARAMS
#وصف_ڪريو HB_NO_LAYOUT_COLLECT_GLYPHS
#وصف_ڪريو HB_NO_LAYOUT_RARELY_استعمال_ٿيل
#وصف_ڪريو HB_NO_LAYOUT_اڻ_استعمال_ٿيل
#وصف_ڪريو HB_NO_MATH
#وصف_ڪريو HB_NO_META
#وصف_ڪريو HB_NO_METRICS
#وصف_ڪريو HB_NO_MMAP
#وصف_ڪريو HB_NO_NAME
#وصف_ڪريو HB_NO_OPEN
#وصف_ڪريو HB_NO_OT_FETCH
#وصف_ڪريو HB_NO_OT_FONT_GLYPH_نالا
#وصف_ڪريو HB_NO_SETLOCALE
#وصف_ڪريو HB_NO_STYLE
#وصف_ڪريو HB_NO_SUBSET_DEPEND
#وصف_ڪريو HB_NO_VERTICAL
#وصف_ڪريو HB_NO_VAR

#إذا !(defined(HB_HAS_CAIRO) || defined(HB_HAS_RASTER) || defined(HB_HAS_ويڪٽر) || defined(HB_HAS_GPU))
#وصف_ڪريو HB_NO_COLOR
#وصف_ڪريو HB_NO_DRAW
#وصف_ڪريو HB_NO_PAINT
#جيڪڏهن_پورو

#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_MINI
#وصف_ڪريو HB_NO_AAT
#وصف_ڪريو HB_NO_LEGACY
#وصف_ڪريو HB_NO_BEYOND_64K
#وصف_ڪريو HB_NO_CUBIC_GLYF
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل __OPTIMIZE_SIZE__
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
#وصف_ڪريو HB_OPTIMIZE_سائيز
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#إذا defined(HAVE_CONFIG_OVERRIDE_H) || defined(HB_CONFIG_OVERRIDE_H)
#جيڪڏهن_وصف_نه_ٿيل HB_CONFIG_OVERRIDE_H
#وصف_ڪريو HB_CONFIG_OVERRIDE_H "config-override.h"
#جيڪڏهن_پورو
#ضمن HB_CONFIG_OVERRIDE_H
#جيڪڏهن_پورو

/* Closure of options. */

#جيڪڏهن_وصف_ٿيل HB_NO_VAR
#وصف_ڪريو HB_NO_VAR_COMPOSITES
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_DISABLE_DEPRECATED
#وصف_ڪريو HB_IF_NOT_DEPRECATED(x)
#وإلا
#وصف_ڪريو HB_IF_NOT_DEPRECATED(x) x
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_SHAPER
#وصف_ڪريو HB_NO_OT_SHAPE
#وصف_ڪريو HB_NO_AAT_SHAPE
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_AAT
#وصف_ڪريو HB_NO_OT_NAME_LANGUAGE_AAT
#وصف_ڪريو HB_NO_AAT_SHAPE
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_BITMAP
#وصف_ڪريو HB_NO_OT_FONT_BITMAP
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_DRAW
#وصف_ڪريو HB_NO_OT_FONT_CFF
#وصف_ڪريو HB_NO_OUTLINE
#وصف_ڪريو HB_NO_PAINT
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_CFF
#وصف_ڪريو HB_NO_OT_FONT_CFF
#وصف_ڪريو HB_NO_SUBSET_CFF
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_LEGACY
#وصف_ڪريو HB_NO_CMAP_LEGACY_SUBTABLES
#وصف_ڪريو HB_NO_FALLBACK_SHAPE
#وصف_ڪريو HB_NO_OT_KERN
#وصف_ڪريو HB_NO_OT_LAYOUT_BLOCKLIST
#وصف_ڪريو HB_NO_AAT_LAYOUT_BLOCKLIST
#وصف_ڪريو HB_NO_OT_SHAPE_FALLBACK
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_NAME
#وصف_ڪريو HB_NO_OT_NAME_LANGUAGE
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_OT
#وصف_ڪريو HB_NO_OT_FONT
#وصف_ڪريو HB_NO_OT_LAYOUT
#وصف_ڪريو HB_NO_OT_ٽيگ
#وصف_ڪريو HB_NO_OT_SHAPE
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_OT_SHAPE
#وصف_ڪريو HB_NO_AAT_SHAPE
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_NO_OT_SHAPE_FALLBACK
#وصف_ڪريو HB_NO_OT_SHAPER_ARABIC_FALLBACK
#وصف_ڪريو HB_NO_OT_SHAPER_HEBREW_FALLBACK
#وصف_ڪريو HB_NO_OT_SHAPER_THAI_FALLBACK
#وصف_ڪريو HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS
#وصف_ڪريو HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_OPTIMIZE_سائيز_وڌيڪ
#وصف_ڪريو HB_NO_OT_RULESETS_FAST_رستو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_MINIMIZE_يادگيري_USAGE
#وصف_ڪريو HB_NO_GDEF_CACHE
#وصف_ڪريو HB_NO_OT_LAYOUT_LOOKUP_CACHE
#وصف_ڪريو HB_NO_OT_FONT_CMAP_CACHE
#جيڪڏهن_پورو

#إذا defined(HAVE_CONFIG_OVERRIDE_آخري_H) || defined(HB_CONFIG_OVERRIDE_آخري_H)
#جيڪڏهن_وصف_نه_ٿيل HB_CONFIG_OVERRIDE_آخري_H
#وصف_ڪريو HB_CONFIG_OVERRIDE_آخري_H "config-override-last.h"
#جيڪڏهن_پورو
#ضمن HB_CONFIG_OVERRIDE_آخري_H
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_OPTIMIZE_سائيز
#وصف_ڪريو HB_OPTIMIZE_سائيز_VAL 1
#وإلا
#وصف_ڪريو HB_OPTIMIZE_سائيز_VAL 0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HB_MINIMIZE_يادگيري_USAGE
#وصف_ڪريو HB_MINIMIZE_يادگيري_USAGE_VAL 1
#وإلا
#وصف_ڪريو HB_MINIMIZE_يادگيري_USAGE_VAL 0
#جيڪڏهن_پورو

#جيڪڏهن_پورو /* HB_CONFIG_HH */
