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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_SHAPER_ARABIC_WIN1256_HH


/*
 * The macros in the first part of this file are generic macros that can
 * be used to define the bytes for OpenType table data in code in a
 * readable manner.  We can move the macros to reside with their respective
 * struct types, but since we only use these to define one data table, the
 * Windows-1256 Arabic shaping table in this file, we keep them here.
 */


/* First we measure, then we cut. */
#جيڪڏهن_وصف_نه_ٿيل OT_MEASURE
#وصف_ڪريو OT_MEASURE
#وصف_ڪريو OT_جدول_شروعات			ساكن ثابت بنية جدول_NAME {
#وصف_ڪريو OT_جدول_پڇاڙي			}
#وصف_ڪريو OT_LABEL_شروعات(Name)		بنانشان حرف Name[
#وصف_ڪريو OT_LABEL_پڇاڙي			];
#وصف_ڪريو OT_UINT8(u8)			+1/*byte*/
#وصف_ڪريو OT_UINT16(u16)			+2/*bytes*/
#وإلا
#وصف_رد  OT_MEASURE
#وصف_ڪريو OT_جدول_شروعات			جدول_NAME = {
#وصف_ڪريو OT_جدول_پڇاڙي			};
#وصف_ڪريو OT_LABEL_شروعات(Name)		{
#وصف_ڪريو OT_LABEL_پڇاڙي			},
#وصف_ڪريو OT_UINT8(u8)			(u8),
#وصف_ڪريو OT_UINT16(u16)			(بنانشان حرف)((u16)>>8), (بنانشان حرف)((u16)&0xFFu),
#وصف_ڪريو OT_ڳڻپ(Name, شي_سائيز_2)	((بنانشان صحيح) حجم(((بنية جدول_NAME*)0)->Name) \
					 / (بنانشان صحيح)(شي_سائيز_2) \
					 /* OT_ASSERT it's divisible (and positive). */)
#وصف_ڪريو OT_مفاصلو(From,منزل)		((بنانشان صحيح) \
					 ((حرف*)(&((بنية جدول_NAME*)0)->منزل) - \
					  (حرف*)(&((بنية جدول_NAME*)0)->From)) \
					 /* OT_ASSERT it's positive. */)
#جيڪڏهن_پورو


#وصف_ڪريو OT_LABEL(Name) \
	OT_LABEL_پڇاڙي \
	OT_LABEL_شروعات(Name)

/* Whenever we receive an argument that is a list, it will expand to
 * contain commas.  That cannot be passed to another macro because the
 * commas will throw off the preprocessor.  The solution is to wrap
 * the passed-in argument in OT_LIST() before passing to the next macro.
 * Unfortunately this trick requires vararg macros. */
#وصف_ڪريو OT_LIST(...) __VA_ARGS__


/*
 * Basic Types
 */

#وصف_ڪريو OT_ٽيگ(a,b,c,d) \
	OT_UINT8(a) OT_UINT8(b) OT_UINT8(c) OT_UINT8(d)

#وصف_ڪريو OT_وٿي(From, منزل) /* Offset from From to To in bytes */ \
	OT_UINT16(OT_مفاصلو(From, منزل))

#وصف_ڪريو OT_GLYPHID /* GlyphID */ \
	OT_UINT16
/* Shorthand. */
#وصف_ڪريو G	OT_GLYPHID

#وصف_ڪريو OT_UARRAY(Name, شيون_2) \
	OT_LABEL_شروعات(Name) \
	OT_UINT16(OT_ڳڻپ(Name##Data, 2)) \
	OT_LABEL(Name##Data) \
	شيون_2 \
	OT_LABEL_پڇاڙي

#وصف_ڪريو OT_UHEADLESSARRAY(Name, شيون_2) \
	OT_LABEL_شروعات(Name) \
	OT_UINT16(OT_ڳڻپ(Name##Data, 2) + 1) \
	OT_LABEL(Name##Data) \
	شيون_2 \
	OT_LABEL_پڇاڙي


/*
 * Common Types
 */

#وصف_ڪريو OT_LOOKUP_FLAG_IGNORE_MARKS	0x08u

#وصف_ڪريو OT_LOOKUP(Name, LookupType, LookupFlag, SubLookupOffsets) \
	OT_LABEL_شروعات(Name) \
	OT_UINT16(LookupType) \
	OT_UINT16(LookupFlag) \
	OT_LABEL_پڇاڙي \
	OT_UARRAY(Name##SubLookupOffsetsArray, OT_LIST(SubLookupOffsets))

#وصف_ڪريو OT_SUBLOOKUP(Name, SubFormat, شيون_2) \
	OT_LABEL_شروعات(Name) \
	OT_UINT16(SubFormat) \
	شيون_2

#وصف_ڪريو OT_COVERAGE1(Name, شيون_2) \
	OT_LABEL_شروعات(Name) \
	OT_UINT16(1) \
	OT_LABEL_پڇاڙي \
	OT_UARRAY(Name##Glyphs, OT_LIST(شيون_2))


/*
 * GSUB
 */

#وصف_ڪريو OT_LOOKUP_TYPE_SUBST_SINGLE	1u
#وصف_ڪريو OT_LOOKUP_TYPE_SUBST_LIGATURE	4u

#وصف_ڪريو OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2(Name, FromGlyphs, منزل_Glyphs) \
	OT_SUBLOOKUP(Name, 2, \
		OT_وٿي(Name, Name##Coverage) \
		OT_LABEL_پڇاڙي \
		OT_UARRAY(Name##Substitute, OT_LIST(منزل_Glyphs)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(FromGlyphs)) \
	/* static_assert_expr (len(FromGlyphs) == len(ToGlyphs)) */

#وصف_ڪريو OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1(Name, پهريون_Glyphs, Ligature_سيٽ_Offsets) \
	OT_SUBLOOKUP(Name, 1, \
		OT_وٿي(Name, Name##Coverage) \
		OT_LABEL_پڇاڙي \
		OT_UARRAY(Name##LigatureSetOffsetsArray, OT_LIST(Ligature_سيٽ_Offsets)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(پهريون_Glyphs)) \
	/* static_assert_expr (len(FirstGlyphs) == len(LigatureSetOffsets)) */

#وصف_ڪريو OT_LIGATURE_سيٽ(Name, Ligature_سيٽ_Offsets) \
	OT_UARRAY(Name, OT_LIST(Ligature_سيٽ_Offsets))

#وصف_ڪريو OT_LIGATURE(Name, Components, LigGlyph) \
	OT_LABEL_شروعات(Name) \
	LigGlyph \
	OT_LABEL_پڇاڙي \
	OT_UHEADLESSARRAY(Name##ComponentsArray, OT_LIST(Components))

/*
 *
 * Start of Windows-1256 shaping table.
 *
 */

/* Table name. */
#وصف_ڪريو جدول_NAME arabic_win1256_gsub_lookups

/* Table manifest. */
#وصف_ڪريو MANIFEST(شيون_2) \
	OT_LABEL_شروعات(manifest) \
	OT_UINT16(OT_ڳڻپ(manifest_معلومات, 6)) \
	OT_LABEL(manifest_معلومات) \
	شيون_2 \
	OT_LABEL_پڇاڙي

#وصف_ڪريو MANIFEST_LOOKUP(ٽيگ, Name) \
	ٽيگ \
	OT_وٿي(manifest, Name)


/*
 * Table Start
 */
OT_جدول_شروعات


/*
 * Manifest
 */
MANIFEST(
	MANIFEST_LOOKUP(OT_ٽيگ('r','l','i','g'), rligLookup)
	MANIFEST_LOOKUP(OT_ٽيگ('i','n','i','t'), initLookup)
	MANIFEST_LOOKUP(OT_ٽيگ('m','e','d','i'), mediLookup)
	MANIFEST_LOOKUP(OT_ٽيگ('f','i','n','a'), finaLookup)
	MANIFEST_LOOKUP(OT_ٽيگ('r','l','i','g'), rligMarksLookup)
)

/*
 * Lookups
 */
OT_LOOKUP(initLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_وٿي(initLookup, initmediSubLookup)
	OT_وٿي(initLookup, initSubLookup)
)
OT_LOOKUP(mediLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_وٿي(mediLookup, initmediSubLookup)
	OT_وٿي(mediLookup, mediSubLookup)
	OT_وٿي(mediLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(finaLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_وٿي(finaLookup, finaSubLookup)
	/* We don't need this one currently as the sequence inherits masks
	 * from the first item.  Just in case we change that in the future
	 * to be smart about Arabic masks when ligating... */
	OT_وٿي(finaLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(rligLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_وٿي(rligLookup, lamAlefLigaturesSubLookup)
)
OT_LOOKUP(rligMarksLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, 0,
	OT_وٿي(rligMarksLookup, shaddaLigaturesSubLookup)
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
	OT_وٿي(lamAlefLigaturesSubLookup, lam_Ligature_سيٽ)
)
OT_LIGATURE_سيٽ(lam_Ligature_سيٽ,
	OT_وٿي(lam_Ligature_سيٽ, lamInitLigature1)
	OT_وٿي(lam_Ligature_سيٽ, lamInitLigature2)
	OT_وٿي(lam_Ligature_سيٽ, lamInitLigature3)
	OT_وٿي(lam_Ligature_سيٽ, lamInitLigature4)
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
	OT_وٿي(shaddaLigaturesSubLookup, shadda_Ligature_سيٽ)
)
OT_LIGATURE_سيٽ(shadda_Ligature_سيٽ,
	OT_وٿي(shadda_Ligature_سيٽ, shaddaLigature1)
	OT_وٿي(shadda_Ligature_سيٽ, shaddaLigature2)
	OT_وٿي(shadda_Ligature_سيٽ, shaddaLigature3)
)
OT_LIGATURE(shaddaLigature1, G(243), G(172))
OT_LIGATURE(shaddaLigature2, G(245), G(173))
OT_LIGATURE(shaddaLigature3, G(246), G(175))

/*
 * Table end
 */
OT_جدول_پڇاڙي


/*
 * Clean up
 */

#وصف_رد MANIFEST
#وصف_رد MANIFEST_LOOKUP

#وصف_رد OT_جدول_شروعات
#وصف_رد OT_جدول_پڇاڙي
#وصف_رد OT_LABEL_شروعات
#وصف_رد OT_LABEL_پڇاڙي
#وصف_رد OT_UINT8
#وصف_رد OT_UINT16
#وصف_رد OT_ڳڻپ
#وصف_رد OT_مفاصلو

#وصف_رد OT_LABEL
#وصف_رد OT_LIST

#وصف_رد OT_ٽيگ
#وصف_رد OT_وٿي
#وصف_رد OT_GLYPHID
#وصف_رد G
#وصف_رد OT_UARRAY
#وصف_رد OT_UHEADLESSARRAY

#وصف_رد OT_LOOKUP_FLAG_IGNORE_MARKS
#وصف_رد OT_LOOKUP
#وصف_رد OT_SUBLOOKUP
#وصف_رد OT_COVERAGE1
#وصف_رد OT_LOOKUP_TYPE_SUBST_SINGLE
#وصف_رد OT_LOOKUP_TYPE_SUBST_LIGATURE
#وصف_رد OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2
#وصف_رد OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1
#وصف_رد OT_LIGATURE_سيٽ
#وصف_رد OT_LIGATURE


/*
 * Include a second time to get the table data...
 */
#إذا 0
#ضمن "hb.hh" /* Make check-includes.sh happy. */
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل OT_MEASURE
#ضمن "hb-ot-shaper-arabic-win1256.hh"
#جيڪڏهن_پورو

#وصف_ڪريو HB_OT_SHAPER_ARABIC_WIN1256_HH
#جيڪڏهن_پورو /* HB_OT_SHAPER_ARABIC_WIN1256_HH */
