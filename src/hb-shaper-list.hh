/*
 * Copyright © 2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#اگر_غیر_تعریف_شدہ HB_SHAPER_LIST_HH
#تعریف_کرو HB_SHAPER_LIST_HH
#اگر_ختم /* HB_SHAPER_LIST_HH */ /* Dummy header guards */

#اگر_غیر_تعریف_شدہ HB_NO_SHAPER


/* v--- Add new shapers in the right place here. */

#اگر_تعریف_شدہ HAVE_WASM
/* Only picks up fonts that have a "Wasm" table. */
HB_SHAPER_IMPLEMENT (wasm)
#اگر_ختم

#اگر_تعریف_شدہ HAVE_GRAPHITE2
/* Only picks up fonts that have a "Silf" table. */
HB_SHAPER_IMPLEMENT (graphite2)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE
HB_SHAPER_IMPLEMENT (ot) /* <--- This is our main shaper. */
#اگر_ختم

#اگر_تعریف_شدہ HAVE_UNISCRIBE
HB_SHAPER_IMPLEMENT (uniscribe)
#اگر_ختم
#اگر_تعریف_شدہ HAVE_DIRECTWRITE
HB_SHAPER_IMPLEMENT (directwrite)
#اگر_ختم
#اگر_تعریف_شدہ HAVE_CORETEXT
HB_SHAPER_IMPLEMENT (coretext)
#اگر_ختم

#اگر_تعریف_شدہ HAVE_HARFRUST
HB_SHAPER_IMPLEMENT (harfrust)
#اگر_ختم

#اگر_تعریف_شدہ HAVE_KBTS
HB_SHAPER_IMPLEMENT (kbts)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_FALLBACK_SHAPE
HB_SHAPER_IMPLEMENT (fallback) /* <--- This should be last. */
#اگر_ختم


#اگر_ختم
