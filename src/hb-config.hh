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

#నిర్వచితం_కాకపోతే HB_CONFIG_HH
#నిర్వచించు HB_CONFIG_HH

#ఒకవేళ 0 /* Make test happy. */
#అన్తర్భావ "hb.hh"
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_CONFIG_H
#అన్తర్భావ "config.h"
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_EXPERIMENTAL_API
#నిర్వచించు HB_NO_BEYOND_64K
#నిర్వచించు HB_NO_CUBIC_GLYF
#నిర్వచించు HB_NO_SUBSET_DEPEND
#అయితే_ముగింపు

#నిర్వచితమైతే HB_TINY
#నిర్వచించు HB_LEAN
#నిర్వచించు HB_MINI
#నిర్వచించు HB_NO_SVG
#నిర్వచించు HB_OPTIMIZE_కొలత
#నిర్వచించు HB_OPTIMIZE_కొలత_మరిన్ని
#నిర్వచించు HB_MINIMIZE_జ్ఞాపకం_USAGE
#నిర్వచించు HB_NO_MT
#నిర్వచించు HB_NO_UCD_UNASSIGNED
#నిర్వచితం_కాకపోతే NDEBUG
#నిర్వచించు NDEBUG
#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితమైతే HB_LEAN
#నిర్వచించు HB_DISABLE_DEPRECATED
#నిర్వచించు HB_NO_ATEXIT
#నిర్వచించు HB_NO_బఫరు_సందేశం
#నిర్వచించు HB_NO_బఫరు_SERIALIZE
#నిర్వచించు HB_NO_బఫరు_VERIFY
#నిర్వచించు HB_NO_BITMAP
#నిర్వచించు HB_NO_ERRNO
#నిర్వచించు HB_NO_FACE_COLLECT_UNICODES
#నిర్వచించు HB_NO_GETENV
#నిర్వచించు HB_NO_HINTING
#నిర్వచించు HB_NO_LAYOUT_FEATURE_PARAMS
#నిర్వచించు HB_NO_LAYOUT_COLLECT_GLYPHS
#నిర్వచించు HB_NO_LAYOUT_RARELY_వాడిన
#నిర్వచించు HB_NO_LAYOUT_వాడని
#నిర్వచించు HB_NO_MATH
#నిర్వచించు HB_NO_META
#నిర్వచించు HB_NO_METRICS
#నిర్వచించు HB_NO_MMAP
#నిర్వచించు HB_NO_NAME
#నిర్వచించు HB_NO_OPEN
#నిర్వచించు HB_NO_OT_FETCH
#నిర్వచించు HB_NO_OT_FONT_GLYPH_పేర్లు
#నిర్వచించు HB_NO_SETLOCALE
#నిర్వచించు HB_NO_STYLE
#నిర్వచించు HB_NO_SUBSET_DEPEND
#నిర్వచించు HB_NO_VERTICAL
#నిర్వచించు HB_NO_VAR

#ఒకవేళ !(defined(HB_HAS_CAIRO) || defined(HB_HAS_RASTER) || defined(HB_HAS_సదిశ) || defined(HB_HAS_GPU))
#నిర్వచించు HB_NO_COLOR
#నిర్వచించు HB_NO_DRAW
#నిర్వచించు HB_NO_PAINT
#అయితే_ముగింపు

#అయితే_ముగింపు

#నిర్వచితమైతే HB_MINI
#నిర్వచించు HB_NO_AAT
#నిర్వచించు HB_NO_LEGACY
#నిర్వచించు HB_NO_BEYOND_64K
#నిర్వచించు HB_NO_CUBIC_GLYF
#అయితే_ముగింపు

#నిర్వచితమైతే __OPTIMIZE_SIZE__
#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
#నిర్వచించు HB_OPTIMIZE_కొలత
#అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ defined(HAVE_CONFIG_OVERRIDE_H) || defined(HB_CONFIG_OVERRIDE_H)
#నిర్వచితం_కాకపోతే HB_CONFIG_OVERRIDE_H
#నిర్వచించు HB_CONFIG_OVERRIDE_H "config-override.h"
#అయితే_ముగింపు
#అన్తర్భావ HB_CONFIG_OVERRIDE_H
#అయితే_ముగింపు

/* Closure of options. */

#నిర్వచితమైతే HB_NO_VAR
#నిర్వచించు HB_NO_VAR_COMPOSITES
#అయితే_ముగింపు

#నిర్వచితమైతే HB_DISABLE_DEPRECATED
#నిర్వచించు HB_IF_NOT_DEPRECATED(x)
#లేకపోతే
#నిర్వచించు HB_IF_NOT_DEPRECATED(x) x
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_SHAPER
#నిర్వచించు HB_NO_OT_SHAPE
#నిర్వచించు HB_NO_AAT_SHAPE
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_AAT
#నిర్వచించు HB_NO_OT_NAME_LANGUAGE_AAT
#నిర్వచించు HB_NO_AAT_SHAPE
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_BITMAP
#నిర్వచించు HB_NO_OT_FONT_BITMAP
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_DRAW
#నిర్వచించు HB_NO_OT_FONT_CFF
#నిర్వచించు HB_NO_OUTLINE
#నిర్వచించు HB_NO_PAINT
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_CFF
#నిర్వచించు HB_NO_OT_FONT_CFF
#నిర్వచించు HB_NO_SUBSET_CFF
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_LEGACY
#నిర్వచించు HB_NO_CMAP_LEGACY_SUBTABLES
#నిర్వచించు HB_NO_FALLBACK_SHAPE
#నిర్వచించు HB_NO_OT_KERN
#నిర్వచించు HB_NO_OT_LAYOUT_BLOCKLIST
#నిర్వచించు HB_NO_AAT_LAYOUT_BLOCKLIST
#నిర్వచించు HB_NO_OT_SHAPE_FALLBACK
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_NAME
#నిర్వచించు HB_NO_OT_NAME_LANGUAGE
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_OT
#నిర్వచించు HB_NO_OT_FONT
#నిర్వచించు HB_NO_OT_LAYOUT
#నిర్వచించు HB_NO_OT_ట్యాగ్
#నిర్వచించు HB_NO_OT_SHAPE
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_OT_SHAPE
#నిర్వచించు HB_NO_AAT_SHAPE
#అయితే_ముగింపు

#నిర్వచితమైతే HB_NO_OT_SHAPE_FALLBACK
#నిర్వచించు HB_NO_OT_SHAPER_ARABIC_FALLBACK
#నిర్వచించు HB_NO_OT_SHAPER_HEBREW_FALLBACK
#నిర్వచించు HB_NO_OT_SHAPER_THAI_FALLBACK
#నిర్వచించు HB_NO_OT_SHAPER_VOWEL_CONSTRAINTS
#నిర్వచించు HB_NO_OT_SHAPER_MYANMAR_ZAWGYI
#అయితే_ముగింపు

#నిర్వచితమైతే HB_OPTIMIZE_కొలత_మరిన్ని
#నిర్వచించు HB_NO_OT_RULESETS_FAST_మార్గం
#అయితే_ముగింపు

#నిర్వచితమైతే HB_MINIMIZE_జ్ఞాపకం_USAGE
#నిర్వచించు HB_NO_GDEF_CACHE
#నిర్వచించు HB_NO_OT_LAYOUT_LOOKUP_CACHE
#నిర్వచించు HB_NO_OT_FONT_CMAP_CACHE
#అయితే_ముగింపు

#ఒకవేళ defined(HAVE_CONFIG_OVERRIDE_చివరి_H) || defined(HB_CONFIG_OVERRIDE_చివరి_H)
#నిర్వచితం_కాకపోతే HB_CONFIG_OVERRIDE_చివరి_H
#నిర్వచించు HB_CONFIG_OVERRIDE_చివరి_H "config-override-last.h"
#అయితే_ముగింపు
#అన్తర్భావ HB_CONFIG_OVERRIDE_చివరి_H
#అయితే_ముగింపు

#నిర్వచితమైతే HB_OPTIMIZE_కొలత
#నిర్వచించు HB_OPTIMIZE_కొలత_VAL 1
#లేకపోతే
#నిర్వచించు HB_OPTIMIZE_కొలత_VAL 0
#అయితే_ముగింపు

#నిర్వచితమైతే HB_MINIMIZE_జ్ఞాపకం_USAGE
#నిర్వచించు HB_MINIMIZE_జ్ఞాపకం_USAGE_VAL 1
#లేకపోతే
#నిర్వచించు HB_MINIMIZE_జ్ఞాపకం_USAGE_VAL 0
#అయితే_ముగింపు

#అయితే_ముగింపు /* HB_CONFIG_HH */
