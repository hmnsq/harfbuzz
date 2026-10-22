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

#اگر_تعریف_نشده HB_OT_FACE_جدول_LIST_HH
#تعریف_کن HB_OT_FACE_جدول_LIST_HH
#پایان_اگر /* HB_OT_FACE_TABLE_LIST_HH */ /* Dummy header guards */

#اگر_تعریف_نشده HB_OT_ACCELERATOR
#تعریف_کن HB_OT_ACCELERATOR(Namespace, Type) HB_OT_جدول (Namespace, Type)
#تعریف_کن _HB_OT_ACCELERATOR_UNDEF
#پایان_اگر


/* This lists font tables that the hb_face_t will contain and lazily
 * load.  Don't add a table unless it's used though.  This is not
 * exactly zero-cost. */

/* v--- Add new tables in the right place here. */


/* OpenType fundamentals. */
HB_OT_جدول (OT, سر_2)
HB_OT_ACCELERATOR (OT, maxp)
#إذا !defined(HB_NO_FACE_COLLECT_UNICODES) || !defined(HB_NO_OT_FONT)
HB_OT_ACCELERATOR (OT, cmap)
#پایان_اگر
HB_OT_ACCELERATOR (OT, hhea)
HB_OT_ACCELERATOR (OT, hmtx)
HB_OT_جدول (OT, OS2)
#إذا !defined(HB_NO_OT_FONT_GLYPH_نامها) || !defined(HB_NO_METRICS) || !defined(HB_NO_STYLE)
HB_OT_ACCELERATOR (OT, post)
#پایان_اگر
#اگر_تعریف_نشده HB_NO_NAME
HB_OT_ACCELERATOR (OT, name)
#پایان_اگر
#اگر_تعریف_نشده HB_NO_STYLE
HB_OT_جدول (OT, STAT)
#پایان_اگر
#اگر_تعریف_نشده HB_NO_META
HB_OT_ACCELERATOR (OT, meta)
#پایان_اگر

/* Vertical layout. */
#اگر_تعریف_نشده HB_NO_VERTICAL
HB_OT_ACCELERATOR (OT, vhea)
HB_OT_ACCELERATOR (OT, vmtx)
HB_OT_جدول (OT, VORG)
#پایان_اگر

/* TrueType outlines. */
HB_OT_جدول (OT, loca)
HB_OT_ACCELERATOR (OT, glyf)

/* CFF outlines. */
#اگر_تعریف_نشده HB_NO_CFF
HB_OT_ACCELERATOR (OT, cff1)
HB_OT_ACCELERATOR (OT, cff2)
#پایان_اگر

/* OpenType variations. */
#اگر_تعریف_نشده HB_NO_VAR
HB_OT_جدول (OT, fvar)
HB_OT_جدول (OT, avar)
HB_OT_جدول (OT, cvar)
HB_OT_ACCELERATOR (OT, gvar)
HB_OT_جدول (OT, MVAR)
#اگر_تعریف_نشده HB_NO_VAR_COMPOSITES
HB_OT_ACCELERATOR (OT, VARC)
#پایان_اگر
#پایان_اگر

/* Legacy kern. */
#اگر_تعریف_نشده HB_NO_OT_KERN
HB_OT_ACCELERATOR (OT, kern)
#پایان_اگر

/* OpenType shaping. */
#اگر_تعریف_نشده HB_NO_OT_LAYOUT
HB_OT_ACCELERATOR (OT, GDEF)
HB_OT_ACCELERATOR (OT, GSUB)
HB_OT_ACCELERATOR (OT, GPOS)
//HB_OT_TABLE (OT, JSTF)
#پایان_اگر

/* OpenType baseline. */
#اگر_تعریف_نشده HB_NO_پایه
HB_OT_جدول (OT, پایه_4)
#پایان_اگر

/* AAT shaping. */
#اگر_تعریف_نشده HB_NO_AAT
HB_OT_ACCELERATOR (AAT, morx)
HB_OT_ACCELERATOR (AAT, mort)
HB_OT_ACCELERATOR (AAT, kerx)
HB_OT_جدول (AAT, ankr)
HB_OT_جدول (AAT, trak)
HB_OT_جدول (AAT, ltag)
HB_OT_جدول (AAT, feat)
// HB_OT_TABLE (AAT, opbd)
#پایان_اگر

/* OpenType color fonts. */
#اگر_تعریف_نشده HB_NO_COLOR
HB_OT_ACCELERATOR (OT, COLR)
HB_OT_جدول (OT, CPAL)
HB_OT_ACCELERATOR (OT, CBDT)
HB_OT_ACCELERATOR (OT, sbix)
#اگر_تعریف_نشده HB_NO_SVG
HB_OT_ACCELERATOR (OT, SVG)
#پایان_اگر
#پایان_اگر

/* OpenType math. */
#اگر_تعریف_نشده HB_NO_MATH
HB_OT_جدول (OT, MATH)
#پایان_اگر


#اگر_تعریف_شده _HB_OT_ACCELERATOR_UNDEF
#لغو_تعریف HB_OT_ACCELERATOR
#پایان_اگر
