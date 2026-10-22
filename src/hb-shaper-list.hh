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

#جيڪڏهن_وصف_نه_ٿيل HB_SHAPER_LIST_HH
#وصف_ڪريو HB_SHAPER_LIST_HH
#جيڪڏهن_پورو /* HB_SHAPER_LIST_HH */ /* Dummy header guards */

#جيڪڏهن_وصف_نه_ٿيل HB_NO_SHAPER


/* v--- Add new shapers in the right place here. */

#جيڪڏهن_وصف_ٿيل HAVE_WASM
/* Only picks up fonts that have a "Wasm" table. */
HB_SHAPER_IMPLEMENT (wasm)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_GRAPHITE2
/* Only picks up fonts that have a "Silf" table. */
HB_SHAPER_IMPLEMENT (graphite2)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE
HB_SHAPER_IMPLEMENT (ot) /* <--- This is our main shaper. */
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_UNISCRIBE
HB_SHAPER_IMPLEMENT (uniscribe)
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_DIRECTWRITE
HB_SHAPER_IMPLEMENT (directwrite)
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_CORETEXT
HB_SHAPER_IMPLEMENT (coretext)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_HARFRUST
HB_SHAPER_IMPLEMENT (harfrust)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_KBTS
HB_SHAPER_IMPLEMENT (kbts)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_FALLBACK_SHAPE
HB_SHAPER_IMPLEMENT (fallback) /* <--- This should be last. */
#جيڪڏهن_پورو


#جيڪڏهن_پورو
