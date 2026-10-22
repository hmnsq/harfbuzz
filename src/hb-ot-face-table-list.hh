/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2012,2013  Google, Inc.
 * Copyright © 2019, Facebook Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 * Facebook Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_FACE_ਸਾਰਣੀ_LIST_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_FACE_ਸਾਰਣੀ_LIST_HH
#ਜੇ_ਅੰਤ /* HB_OT_FACE_TABLE_LIST_HH */ /* Dummy header guards */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_ACCELERATOR
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ACCELERATOR(Namespace, Type) HB_OT_ਸਾਰਣੀ (Namespace, Type)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _HB_OT_ACCELERATOR_UNDEF
#ਜੇ_ਅੰਤ


/* This lists font tables that the hb_face_t will contain and lazily
 * load.  Don't add a table unless it's used though.  This is not
 * exactly zero-cost. */

/* v--- Add new tables in the right place here. */


/* OpenType fundamentals. */
HB_OT_ਸਾਰਣੀ (OT, ਸਿਰ_2)
HB_OT_ACCELERATOR (OT, maxp)
#ਜੇ !defined(HB_NO_FACE_COLLECT_UNICODES) || !defined(HB_NO_OT_FONT)
HB_OT_ACCELERATOR (OT, cmap)
#ਜੇ_ਅੰਤ
HB_OT_ACCELERATOR (OT, hhea)
HB_OT_ACCELERATOR (OT, hmtx)
HB_OT_ਸਾਰਣੀ (OT, OS2)
#ਜੇ !defined(HB_NO_OT_FONT_GLYPH_ਨਾਂ) || !defined(HB_NO_METRICS) || !defined(HB_NO_STYLE)
HB_OT_ACCELERATOR (OT, post)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_NAME
HB_OT_ACCELERATOR (OT, name)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_STYLE
HB_OT_ਸਾਰਣੀ (OT, STAT)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_META
HB_OT_ACCELERATOR (OT, meta)
#ਜੇ_ਅੰਤ

/* Vertical layout. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
HB_OT_ACCELERATOR (OT, vhea)
HB_OT_ACCELERATOR (OT, vmtx)
HB_OT_ਸਾਰਣੀ (OT, VORG)
#ਜੇ_ਅੰਤ

/* TrueType outlines. */
HB_OT_ਸਾਰਣੀ (OT, loca)
HB_OT_ACCELERATOR (OT, glyf)

/* CFF outlines. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_CFF
HB_OT_ACCELERATOR (OT, cff1)
HB_OT_ACCELERATOR (OT, cff2)
#ਜੇ_ਅੰਤ

/* OpenType variations. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
HB_OT_ਸਾਰਣੀ (OT, fvar)
HB_OT_ਸਾਰਣੀ (OT, avar)
HB_OT_ਸਾਰਣੀ (OT, cvar)
HB_OT_ACCELERATOR (OT, gvar)
HB_OT_ਸਾਰਣੀ (OT, MVAR)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR_COMPOSITES
HB_OT_ACCELERATOR (OT, VARC)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

/* Legacy kern. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_KERN
HB_OT_ACCELERATOR (OT, kern)
#ਜੇ_ਅੰਤ

/* OpenType shaping. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT
HB_OT_ACCELERATOR (OT, GDEF)
HB_OT_ACCELERATOR (OT, GSUB)
HB_OT_ACCELERATOR (OT, GPOS)
//HB_OT_TABLE (OT, JSTF)
#ਜੇ_ਅੰਤ

/* OpenType baseline. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਅਧਾਰ
HB_OT_ਸਾਰਣੀ (OT, ਅਧਾਰ_4)
#ਜੇ_ਅੰਤ

/* AAT shaping. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT
HB_OT_ACCELERATOR (AAT, morx)
HB_OT_ACCELERATOR (AAT, mort)
HB_OT_ACCELERATOR (AAT, kerx)
HB_OT_ਸਾਰਣੀ (AAT, ankr)
HB_OT_ਸਾਰਣੀ (AAT, trak)
HB_OT_ਸਾਰਣੀ (AAT, ltag)
HB_OT_ਸਾਰਣੀ (AAT, feat)
// HB_OT_TABLE (AAT, opbd)
#ਜੇ_ਅੰਤ

/* OpenType color fonts. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_COLOR
HB_OT_ACCELERATOR (OT, COLR)
HB_OT_ਸਾਰਣੀ (OT, CPAL)
HB_OT_ACCELERATOR (OT, CBDT)
HB_OT_ACCELERATOR (OT, sbix)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SVG
HB_OT_ACCELERATOR (OT, SVG)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

/* OpenType math. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_MATH
HB_OT_ਸਾਰਣੀ (OT, MATH)
#ਜੇ_ਅੰਤ


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _HB_OT_ACCELERATOR_UNDEF
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_OT_ACCELERATOR
#ਜੇ_ਅੰਤ
