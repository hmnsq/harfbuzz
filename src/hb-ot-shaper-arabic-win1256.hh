/*
 * Copyright © 2014  Google, Inc.
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

#నిర్వచితం_కాకపోతే HB_OT_SHAPER_ARABIC_WIN1256_HH


/*
 * The macros in the first part of this file are generic macros that can
 * be used to define the bytes for OpenType table data in code in a
 * readable manner.  We can move the macros to reside with their respective
 * struct types, but since we only use these to define one data table, the
 * Windows-1256 Arabic shaping table in this file, we keep them here.
 */


/* First we measure, then we cut. */
#నిర్వచితం_కాకపోతే OT_MEASURE
#నిర్వచించు OT_MEASURE
#నిర్వచించు OT_పట్టిక_ప్రారంభం			స్థిర స్థిరరాశి నిర్మాణం పట్టిక_NAME {
#నిర్వచించు OT_పట్టిక_చివర			}
#నిర్వచించు OT_LABEL_ప్రారంభం(Name)		గుర్తులేని అక్షరం Name[
#నిర్వచించు OT_LABEL_చివర			];
#నిర్వచించు OT_UINT8(u8)			+1/*byte*/
#నిర్వచించు OT_UINT16(u16)			+2/*bytes*/
#లేకపోతే
#నిర్వచనం_రద్దు  OT_MEASURE
#నిర్వచించు OT_పట్టిక_ప్రారంభం			పట్టిక_NAME = {
#నిర్వచించు OT_పట్టిక_చివర			};
#నిర్వచించు OT_LABEL_ప్రారంభం(Name)		{
#నిర్వచించు OT_LABEL_చివర			},
#నిర్వచించు OT_UINT8(u8)			(u8),
#నిర్వచించు OT_UINT16(u16)			(గుర్తులేని అక్షరం)((u16)>>8), (గుర్తులేని అక్షరం)((u16)&0xFFu),
#నిర్వచించు OT_లెక్క(Name, అంశం_కొలత_2)	((గుర్తులేని పూర్ణాంకం) పరిమాణం(((నిర్మాణం పట్టిక_NAME*)0)->Name) \
					 / (గుర్తులేని పూర్ణాంకం)(అంశం_కొలత_2) \
					 /* OT_ASSERT it's divisible (and positive). */)
#నిర్వచించు OT_దూరం(From,గమ్యం)		((గుర్తులేని పూర్ణాంకం) \
					 ((అక్షరం*)(&((నిర్మాణం పట్టిక_NAME*)0)->గమ్యం) - \
					  (అక్షరం*)(&((నిర్మాణం పట్టిక_NAME*)0)->From)) \
					 /* OT_ASSERT it's positive. */)
#అయితే_ముగింపు


#నిర్వచించు OT_LABEL(Name) \
	OT_LABEL_చివర \
	OT_LABEL_ప్రారంభం(Name)

/* Whenever we receive an argument that is a list, it will expand to
 * contain commas.  That cannot be passed to another macro because the
 * commas will throw off the preprocessor.  The solution is to wrap
 * the passed-in argument in OT_LIST() before passing to the next macro.
 * Unfortunately this trick requires vararg macros. */
#నిర్వచించు OT_LIST(...) __VA_ARGS__


/*
 * Basic Types
 */

#నిర్వచించు OT_ట్యాగ్(a,b,c,d) \
	OT_UINT8(a) OT_UINT8(b) OT_UINT8(c) OT_UINT8(d)

#నిర్వచించు OT_స్థానభ్రంశం(From, గమ్యం) /* Offset from From to To in bytes */ \
	OT_UINT16(OT_దూరం(From, గమ్యం))

#నిర్వచించు OT_GLYPHID /* GlyphID */ \
	OT_UINT16
/* Shorthand. */
#నిర్వచించు G	OT_GLYPHID

#నిర్వచించు OT_UARRAY(Name, వస్తువులు_2) \
	OT_LABEL_ప్రారంభం(Name) \
	OT_UINT16(OT_లెక్క(Name##Data, 2)) \
	OT_LABEL(Name##Data) \
	వస్తువులు_2 \
	OT_LABEL_చివర

#నిర్వచించు OT_UHEADLESSARRAY(Name, వస్తువులు_2) \
	OT_LABEL_ప్రారంభం(Name) \
	OT_UINT16(OT_లెక్క(Name##Data, 2) + 1) \
	OT_LABEL(Name##Data) \
	వస్తువులు_2 \
	OT_LABEL_చివర


/*
 * Common Types
 */

#నిర్వచించు OT_LOOKUP_FLAG_IGNORE_MARKS	0x08u

#నిర్వచించు OT_LOOKUP(Name, LookupType, LookupFlag, SubLookupOffsets) \
	OT_LABEL_ప్రారంభం(Name) \
	OT_UINT16(LookupType) \
	OT_UINT16(LookupFlag) \
	OT_LABEL_చివర \
	OT_UARRAY(Name##SubLookupOffsetsArray, OT_LIST(SubLookupOffsets))

#నిర్వచించు OT_SUBLOOKUP(Name, SubFormat, వస్తువులు_2) \
	OT_LABEL_ప్రారంభం(Name) \
	OT_UINT16(SubFormat) \
	వస్తువులు_2

#నిర్వచించు OT_COVERAGE1(Name, వస్తువులు_2) \
	OT_LABEL_ప్రారంభం(Name) \
	OT_UINT16(1) \
	OT_LABEL_చివర \
	OT_UARRAY(Name##Glyphs, OT_LIST(వస్తువులు_2))


/*
 * GSUB
 */

#నిర్వచించు OT_LOOKUP_TYPE_SUBST_SINGLE	1u
#నిర్వచించు OT_LOOKUP_TYPE_SUBST_LIGATURE	4u

#నిర్వచించు OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2(Name, FromGlyphs, గమ్యం_Glyphs) \
	OT_SUBLOOKUP(Name, 2, \
		OT_స్థానభ్రంశం(Name, Name##Coverage) \
		OT_LABEL_చివర \
		OT_UARRAY(Name##Substitute, OT_LIST(గమ్యం_Glyphs)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(FromGlyphs)) \
	/* static_assert_expr (len(FromGlyphs) == len(ToGlyphs)) */

#నిర్వచించు OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1(Name, మొదటి_Glyphs, Ligature_సమితి_Offsets) \
	OT_SUBLOOKUP(Name, 1, \
		OT_స్థానభ్రంశం(Name, Name##Coverage) \
		OT_LABEL_చివర \
		OT_UARRAY(Name##LigatureSetOffsetsArray, OT_LIST(Ligature_సమితి_Offsets)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(మొదటి_Glyphs)) \
	/* static_assert_expr (len(FirstGlyphs) == len(LigatureSetOffsets)) */

#నిర్వచించు OT_LIGATURE_సమితి(Name, Ligature_సమితి_Offsets) \
	OT_UARRAY(Name, OT_LIST(Ligature_సమితి_Offsets))

#నిర్వచించు OT_LIGATURE(Name, Components, LigGlyph) \
	OT_LABEL_ప్రారంభం(Name) \
	LigGlyph \
	OT_LABEL_చివర \
	OT_UHEADLESSARRAY(Name##ComponentsArray, OT_LIST(Components))

/*
 *
 * Start of Windows-1256 shaping table.
 *
 */

/* Table name. */
#నిర్వచించు పట్టిక_NAME arabic_win1256_gsub_lookups

/* Table manifest. */
#నిర్వచించు MANIFEST(వస్తువులు_2) \
	OT_LABEL_ప్రారంభం(manifest) \
	OT_UINT16(OT_లెక్క(manifest_దత్తాంశం, 6)) \
	OT_LABEL(manifest_దత్తాంశం) \
	వస్తువులు_2 \
	OT_LABEL_చివర

#నిర్వచించు MANIFEST_LOOKUP(ట్యాగ్, Name) \
	ట్యాగ్ \
	OT_స్థానభ్రంశం(manifest, Name)


/*
 * Table Start
 */
OT_పట్టిక_ప్రారంభం


/*
 * Manifest
 */
MANIFEST(
	MANIFEST_LOOKUP(OT_ట్యాగ్('r','l','i','g'), rligLookup)
	MANIFEST_LOOKUP(OT_ట్యాగ్('i','n','i','t'), initLookup)
	MANIFEST_LOOKUP(OT_ట్యాగ్('m','e','d','i'), mediLookup)
	MANIFEST_LOOKUP(OT_ట్యాగ్('f','i','n','a'), finaLookup)
	MANIFEST_LOOKUP(OT_ట్యాగ్('r','l','i','g'), rligMarksLookup)
)

/*
 * Lookups
 */
OT_LOOKUP(initLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_స్థానభ్రంశం(initLookup, initmediSubLookup)
	OT_స్థానభ్రంశం(initLookup, initSubLookup)
)
OT_LOOKUP(mediLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_స్థానభ్రంశం(mediLookup, initmediSubLookup)
	OT_స్థానభ్రంశం(mediLookup, mediSubLookup)
	OT_స్థానభ్రంశం(mediLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(finaLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_స్థానభ్రంశం(finaLookup, finaSubLookup)
	/* We don't need this one currently as the sequence inherits masks
	 * from the first item.  Just in case we change that in the future
	 * to be smart about Arabic masks when ligating... */
	OT_స్థానభ్రంశం(finaLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(rligLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_స్థానభ్రంశం(rligLookup, lamAlefLigaturesSubLookup)
)
OT_LOOKUP(rligMarksLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, 0,
	OT_స్థానభ్రంశం(rligMarksLookup, shaddaLigaturesSubLookup)
)

/*
 * init/medi/fina forms
 */
OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2(initmediSubLookup,
	G(198)	G(200)	G(201)	G(202)	G(203)	G(204)	G(205)	G(206)	G(211)
	G(212)	G(213)	G(214)	G(223)	G(225)	G(227)	G(228)	G(236)	G(237),
	G(162)	G(4)	G(5)	G(5)	G(6)	G(7)	G(9)	G(11)	G(13)
	G(14)	G(15)	G(26)	G(140)	G(141)	G(142)	G(143)	G(154)	G(154)
)
OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2(initSubLookup,
	G(218)	G(219)	G(221)	G(222)	G(229),
	G(27)	G(30)	G(128)	G(131)	G(144)
)
OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2(mediSubLookup,
	G(218)	G(219)	G(221)	G(222)	G(229),
	G(28)	G(31)	G(129)	G(138)	G(149)
)
OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2(finaSubLookup,
	G(194)	G(195)	G(197)	G(198)	G(199)	G(201)	G(204)	G(205)	G(206)
	G(218)	G(219)	G(229)	G(236)	G(237),
	G(2)	G(1)	G(3)	G(181)	G(0)	G(159)	G(8)	G(10)	G(12)
	G(29)	G(127)	G(152) G(160)	G(156)
)
OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2(medifinaLamAlefSubLookup,
	G(165)	G(178)	G(180)	G(252),
	G(170)	G(179)	G(185)	G(255)
)

/*
 * Lam+Alef ligatures
 */
OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1(lamAlefLigaturesSubLookup,
	G(225),
	OT_స్థానభ్రంశం(lamAlefLigaturesSubLookup, lam_Ligature_సమితి)
)
OT_LIGATURE_సమితి(lam_Ligature_సమితి,
	OT_స్థానభ్రంశం(lam_Ligature_సమితి, lamInitLigature1)
	OT_స్థానభ్రంశం(lam_Ligature_సమితి, lamInitLigature2)
	OT_స్థానభ్రంశం(lam_Ligature_సమితి, lamInitLigature3)
	OT_స్థానభ్రంశం(lam_Ligature_సమితి, lamInitLigature4)
)
OT_LIGATURE(lamInitLigature1, G(199), G(165))
OT_LIGATURE(lamInitLigature2, G(195), G(178))
OT_LIGATURE(lamInitLigature3, G(194), G(180))
OT_LIGATURE(lamInitLigature4, G(197), G(252))

/*
 * Shadda ligatures
 */
OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1(shaddaLigaturesSubLookup,
	G(248),
	OT_స్థానభ్రంశం(shaddaLigaturesSubLookup, shadda_Ligature_సమితి)
)
OT_LIGATURE_సమితి(shadda_Ligature_సమితి,
	OT_స్థానభ్రంశం(shadda_Ligature_సమితి, shaddaLigature1)
	OT_స్థానభ్రంశం(shadda_Ligature_సమితి, shaddaLigature2)
	OT_స్థానభ్రంశం(shadda_Ligature_సమితి, shaddaLigature3)
)
OT_LIGATURE(shaddaLigature1, G(243), G(172))
OT_LIGATURE(shaddaLigature2, G(245), G(173))
OT_LIGATURE(shaddaLigature3, G(246), G(175))

/*
 * Table end
 */
OT_పట్టిక_చివర


/*
 * Clean up
 */

#నిర్వచనం_రద్దు MANIFEST
#నిర్వచనం_రద్దు MANIFEST_LOOKUP

#నిర్వచనం_రద్దు OT_పట్టిక_ప్రారంభం
#నిర్వచనం_రద్దు OT_పట్టిక_చివర
#నిర్వచనం_రద్దు OT_LABEL_ప్రారంభం
#నిర్వచనం_రద్దు OT_LABEL_చివర
#నిర్వచనం_రద్దు OT_UINT8
#నిర్వచనం_రద్దు OT_UINT16
#నిర్వచనం_రద్దు OT_లెక్క
#నిర్వచనం_రద్దు OT_దూరం

#నిర్వచనం_రద్దు OT_LABEL
#నిర్వచనం_రద్దు OT_LIST

#నిర్వచనం_రద్దు OT_ట్యాగ్
#నిర్వచనం_రద్దు OT_స్థానభ్రంశం
#నిర్వచనం_రద్దు OT_GLYPHID
#నిర్వచనం_రద్దు G
#నిర్వచనం_రద్దు OT_UARRAY
#నిర్వచనం_రద్దు OT_UHEADLESSARRAY

#నిర్వచనం_రద్దు OT_LOOKUP_FLAG_IGNORE_MARKS
#నిర్వచనం_రద్దు OT_LOOKUP
#నిర్వచనం_రద్దు OT_SUBLOOKUP
#నిర్వచనం_రద్దు OT_COVERAGE1
#నిర్వచనం_రద్దు OT_LOOKUP_TYPE_SUBST_SINGLE
#నిర్వచనం_రద్దు OT_LOOKUP_TYPE_SUBST_LIGATURE
#నిర్వచనం_రద్దు OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2
#నిర్వచనం_రద్దు OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1
#నిర్వచనం_రద్దు OT_LIGATURE_సమితి
#నిర్వచనం_రద్దు OT_LIGATURE


/*
 * Include a second time to get the table data...
 */
#ఒకవేళ 0
#అన్తర్భావ "hb.hh" /* Make check-includes.sh happy. */
#అయితే_ముగింపు
#నిర్వచితమైతే OT_MEASURE
#అన్తర్భావ "hb-ot-shaper-arabic-win1256.hh"
#అయితే_ముగింపు

#నిర్వచించు HB_OT_SHAPER_ARABIC_WIN1256_HH
#అయితే_ముగింపు /* HB_OT_SHAPER_ARABIC_WIN1256_HH */
