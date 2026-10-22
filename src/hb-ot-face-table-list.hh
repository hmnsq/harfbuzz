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

#నిర్వచితం_కాకపోతే HB_OT_FACE_పట్టిక_LIST_HH
#నిర్వచించు HB_OT_FACE_పట్టిక_LIST_HH
#అయితే_ముగింపు /* HB_OT_FACE_TABLE_LIST_HH */ /* Dummy header guards */

#నిర్వచితం_కాకపోతే HB_OT_ACCELERATOR
#నిర్వచించు HB_OT_ACCELERATOR(Namespace, Type) HB_OT_పట్టిక (Namespace, Type)
#నిర్వచించు _HB_OT_ACCELERATOR_UNDEF
#అయితే_ముగింపు


/* This lists font tables that the hb_face_t will contain and lazily
 * load.  Don't add a table unless it's used though.  This is not
 * exactly zero-cost. */

/* v--- Add new tables in the right place here. */


/* OpenType fundamentals. */
HB_OT_పట్టిక (OT, తల_2)
HB_OT_ACCELERATOR (OT, maxp)
#ఒకవేళ !defined(HB_NO_FACE_COLLECT_UNICODES) || !defined(HB_NO_OT_FONT)
HB_OT_ACCELERATOR (OT, cmap)
#అయితే_ముగింపు
HB_OT_ACCELERATOR (OT, hhea)
HB_OT_ACCELERATOR (OT, hmtx)
HB_OT_పట్టిక (OT, OS2)
#ఒకవేళ !defined(HB_NO_OT_FONT_GLYPH_పేర్లు) || !defined(HB_NO_METRICS) || !defined(HB_NO_STYLE)
HB_OT_ACCELERATOR (OT, post)
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_NAME
HB_OT_ACCELERATOR (OT, name)
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_STYLE
HB_OT_పట్టిక (OT, STAT)
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_META
HB_OT_ACCELERATOR (OT, meta)
#అయితే_ముగింపు

/* Vertical layout. */
#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
HB_OT_ACCELERATOR (OT, vhea)
HB_OT_ACCELERATOR (OT, vmtx)
HB_OT_పట్టిక (OT, VORG)
#అయితే_ముగింపు

/* TrueType outlines. */
HB_OT_పట్టిక (OT, loca)
HB_OT_ACCELERATOR (OT, glyf)

/* CFF outlines. */
#నిర్వచితం_కాకపోతే HB_NO_CFF
HB_OT_ACCELERATOR (OT, cff1)
HB_OT_ACCELERATOR (OT, cff2)
#అయితే_ముగింపు

/* OpenType variations. */
#నిర్వచితం_కాకపోతే HB_NO_VAR
HB_OT_పట్టిక (OT, fvar)
HB_OT_పట్టిక (OT, avar)
HB_OT_పట్టిక (OT, cvar)
HB_OT_ACCELERATOR (OT, gvar)
HB_OT_పట్టిక (OT, MVAR)
#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES
HB_OT_ACCELERATOR (OT, VARC)
#అయితే_ముగింపు
#అయితే_ముగింపు

/* Legacy kern. */
#నిర్వచితం_కాకపోతే HB_NO_OT_KERN
HB_OT_ACCELERATOR (OT, kern)
#అయితే_ముగింపు

/* OpenType shaping. */
#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT
HB_OT_ACCELERATOR (OT, GDEF)
HB_OT_ACCELERATOR (OT, GSUB)
HB_OT_ACCELERATOR (OT, GPOS)
//HB_OT_TABLE (OT, JSTF)
#అయితే_ముగింపు

/* OpenType baseline. */
#నిర్వచితం_కాకపోతే HB_NO_పునాది
HB_OT_పట్టిక (OT, పునాది_4)
#అయితే_ముగింపు

/* AAT shaping. */
#నిర్వచితం_కాకపోతే HB_NO_AAT
HB_OT_ACCELERATOR (AAT, morx)
HB_OT_ACCELERATOR (AAT, mort)
HB_OT_ACCELERATOR (AAT, kerx)
HB_OT_పట్టిక (AAT, ankr)
HB_OT_పట్టిక (AAT, trak)
HB_OT_పట్టిక (AAT, ltag)
HB_OT_పట్టిక (AAT, feat)
// HB_OT_TABLE (AAT, opbd)
#అయితే_ముగింపు

/* OpenType color fonts. */
#నిర్వచితం_కాకపోతే HB_NO_COLOR
HB_OT_ACCELERATOR (OT, COLR)
HB_OT_పట్టిక (OT, CPAL)
HB_OT_ACCELERATOR (OT, CBDT)
HB_OT_ACCELERATOR (OT, sbix)
#నిర్వచితం_కాకపోతే HB_NO_SVG
HB_OT_ACCELERATOR (OT, SVG)
#అయితే_ముగింపు
#అయితే_ముగింపు

/* OpenType math. */
#నిర్వచితం_కాకపోతే HB_NO_MATH
HB_OT_పట్టిక (OT, MATH)
#అయితే_ముగింపు


#నిర్వచితమైతే _HB_OT_ACCELERATOR_UNDEF
#నిర్వచనం_రద్దు HB_OT_ACCELERATOR
#అయితే_ముగింపు
