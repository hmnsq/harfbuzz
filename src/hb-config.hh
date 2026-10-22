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

#अगर_अघोषित HB_CONFIG_HH
#घोषणा HB_CONFIG_HH

#अगर 0 /* Make test happy. */
#समावेश "hb.hh"
#पूर्ण_अगर

#अगर_घोषित HAVE_CONFIG_H
#समावेश "config.h"
#पूर्ण_अगर

#अगर_अघोषित HB_EXPERIMENTAL_API
#घोषणा HB_NO_BEYOND_64K
#घोषणा HB_NO_CUBIC_GLYF
#घोषणा HB_NO_SUBSET_DEPEND
#पूर्ण_अगर

#अगर_घोषित HB_TINY
#घोषणा HB_LEAN
#घोषणा HB_MINI
#घोषणा HB_NO_SVG
#घोषणा HB_OPTIMIZE_आकार
#घोषणा HB_OPTIMIZE_आकार_MORE
#घोषणा HB_MINIMIZE_MEMORY_USAGE
#घोषणा HB_NO_MT
#घोषणा HB_NO_UCD_UNASSIGNED
#अगर_अघोषित न_संशोधन
#घोषणा न_संशोधन
#पूर्ण_अगर
#पूर्ण_अगर

#अगर_घोषित HB_LEAN
#घोषणा HB_DISABLE_DEPRECATED
#घोषणा HB_NO_ATEXIT
#घोषणा HB_NO_बफर_MESSAGE
#घोषणा HB_NO_बफर_SERIALIZE
#घोषणा HB_NO_बफर_VERIFY
#घोषणा HB_NO_BITMAP
#घोषणा HB_NO_ERRNO
#घोषणा HB_NO_FACE_COLLECT_UNICODES
#घोषणा HB_NO_GETENV
#घोषणा HB_NO_HINTING
#घोषणा HB_NO_LAYOUT_FEATURE_PARAMS
#घोषणा HB_NO_LAYOUT_COLLECT_GLYPHS
#घोषणा HB_NO_LAYOUT_RARELY_USED
#घोषणा HB_NO_LAYOUT_UNUSED
#घोषणा HB_NO_MATH
#घोषणा HB_NO_META
#घोषणा HB_NO_METRICS
#घोषणा HB_NO_MMAP
#घोषणा HB_NO_NAME
#घोषणा HB_NO_OPEN
#घोषणा HB_NO_OT_FETCH
#घोषणा HB_NO_OT_FONT_GLYPH_NAMES
#घोषणा HB_NO_SETLOCALE
#घोषणा HB_NO_STYLE
#घोषणा HB_NO_SUBSET_DEPEND
#घोषणा HB_NO_VERTICAL
#घोषणा HB_NO_VAR

#अगर !(defined(HB_HAS_CAIRO) || defined(HB_HAS_RASTER) || defined(HB_HAS_VECTOR) || defined(HB_HAS_GPU))
#घोषणा HB_NO_COLOR
#घोषणा HB_NO_DRAW
#घोषणा HB_NO_PAINT
#पूर्ण_अगर

#पूर्ण_अगर

#अगर_घोषित HB_MINI
#घोषणा HB_NO_AAT
#घोषणा HB_NO_LEGACY
#घोषणा HB_NO_BEYOND_64K
#घोषणा HB_NO_CUBIC_GLYF
#पूर्ण_अगर

#अगर_घोषित __OPTIMIZE_SIZE__
#अगर_अघोषित HB_OPTIMIZE_आकार
#घोषणा HB_OPTIMIZE_आकार
#पूर्ण_अगर
#पूर्ण_अगर

#अगर defined(HAVE_CONFIG_OVERRIDE_H) || defined(HB_CONFIG_OVERRIDE_H)
#अगर_अघोषित HB_CONFIG_OVERRIDE_H
#घोषणा HB_CONFIG_OVERRIDE_H "config-override.h"
#पूर्ण_अगर
#समावेश HB_CONFIG_OVERRIDE_H
#पूर्ण_अगर

/* Closure of options. */

#अगर_घोषित HB_NO_VAR
#घोषणा HB_NO_VAR_COMPOSITES
#पूर्ण_अगर

#अगर_घोषित HB_DISABLE_DEPRECATED
#घोषणा HB_IF_NOT_DEPRECATED(x)
#अन्यथा
#घोषणा HB_IF_NOT_DEPRECATED(x) x
#पूर्ण_अगर

#अगर_घोषित HB_NO_SHAPER
#घोषणा HB_NO_OT_SHAPE
#घोषणा HB_NO_AAT_SHAPE
#पूर्ण_अगर

#अगर_घोषित HB_NO_AAT
#घोषणा HB_NO_OT_NAME_LANGUAGE_AAT
#घोषणा HB_NO_AAT_SHAPE
#पूर्ण_अगर

#अगर_घोषित HB_NO_BITMAP
#घोषणा HB_NO_OT_FONT_BITMAP
#पूर्ण_अगर

#अगर_घोषित HB_NO_DRAW
#घोषणा HB_NO_OT_FONT_CFF
#घोषणा HB_NO_OUTLINE
#घोषणा HB_NO_PAINT
#पूर्ण_अगर

#अगर_घोषित HB_NO_CFF
#घोषणा HB_NO_OT_FONT_CFF
#घोषणा HB_NO_SUBSET_CFF
#पूर्ण_अगर

#अगर_घोषित HB_NO_LEGACY
#घोषणा HB_NO_CMAP_LEGACY_SUBTABLES
#घोषणा HB_NO_FALLBACK_SHAPE
#घोषणा HB_NO_OT_KERN
#घोषणा HB_NO_OT_LAYOUT_BLOCKLIST
#घोषणा HB_NO_AAT_LAYOUT_BLOCKLIST
#घोषणा HB_NO_OT_SHAPE_FALLBACK
#पूर्ण_अगर

#अगर_घोषित HB_NO_NAME
#घोषणा HB_NO_OT_NAME_LANGUAGE
#पूर्ण_अगर

#अगर_घोषित HB_NO_OT
#घोषणा HB_NO_OT_FONT
#घोषणा HB_NO_OT_LAYOUT
#घोषणा HB_NO_OT_TAG
#घोषणा HB_NO_OT_SHAPE
#पूर्ण_अगर

#अगर_घोषित HB_NO_OT_SHAPE
#घोषणा HB_NO_AAT_SHAPE
#पूर्ण_अगर

#अगर_घोषित HB_NO_OT_SHAPE_FALLBACK
#घोषणा HB_NO_OT_SHAPER_ARABIC_FALLBACK
#घोषणा HB_NO_OT_SHAPER_HEBREW_FALLBACK
#घोषणा HB_NO_OT_SHAPER_THAI_FALLBACK
#घोषणा HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS
#घोषणा HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#पूर्ण_अगर

#अगर_घोषित HB_OPTIMIZE_आकार_MORE
#घोषणा HB_NO_OT_RULESETS_FAST_PATH
#पूर्ण_अगर

#अगर_घोषित HB_MINIMIZE_MEMORY_USAGE
#घोषणा HB_NO_GDEF_CACHE
#घोषणा HB_NO_OT_LAYOUT_LOOKUP_CACHE
#घोषणा HB_NO_OT_FONT_CMAP_CACHE
#पूर्ण_अगर

#अगर defined(HAVE_CONFIG_OVERRIDE_LAST_H) || defined(HB_CONFIG_OVERRIDE_LAST_H)
#अगर_अघोषित HB_CONFIG_OVERRIDE_LAST_H
#घोषणा HB_CONFIG_OVERRIDE_LAST_H "config-override-last.h"
#पूर्ण_अगर
#समावेश HB_CONFIG_OVERRIDE_LAST_H
#पूर्ण_अगर

#अगर_घोषित HB_OPTIMIZE_आकार
#घोषणा HB_OPTIMIZE_आकार_VAL 1
#अन्यथा
#घोषणा HB_OPTIMIZE_आकार_VAL 0
#पूर्ण_अगर

#अगर_घोषित HB_MINIMIZE_MEMORY_USAGE
#घोषणा HB_MINIMIZE_MEMORY_USAGE_VAL 1
#अन्यथा
#घोषणा HB_MINIMIZE_MEMORY_USAGE_VAL 0
#पूर्ण_अगर

#पूर्ण_अगर /* HB_CONFIG_HH */
