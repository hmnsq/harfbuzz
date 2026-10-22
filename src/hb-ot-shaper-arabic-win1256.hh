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

#اگر_تعریف_نشده HB_OT_SHAPER_ARABIC_WIN1256_HH


/*
 * The macros in the first part of this file are generic macros that can
 * be used to define the bytes for OpenType table data in code in a
 * readable manner.  We can move the macros to reside with their respective
 * struct types, but since we only use these to define one data table, the
 * Windows-1256 Arabic shaping table in this file, we keep them here.
 */


/* First we measure, then we cut. */
#اگر_تعریف_نشده OT_MEASURE
#تعریف_کن OT_MEASURE
#تعریف_کن OT_جدول_شروع			ساكن ثابت بنية جدول_NAME {
#تعریف_کن OT_جدول_انتها			}
#تعریف_کن OT_LABEL_شروع(Name)		بیعلامت حرف Name[
#تعریف_کن OT_LABEL_انتها			];
#تعریف_کن OT_UINT8(u8)			+1/*byte*/
#تعریف_کن OT_UINT16(u16)			+2/*bytes*/
#وإلا
#لغو_تعریف  OT_MEASURE
#تعریف_کن OT_جدول_شروع			جدول_NAME = {
#تعریف_کن OT_جدول_انتها			};
#تعریف_کن OT_LABEL_شروع(Name)		{
#تعریف_کن OT_LABEL_انتها			},
#تعریف_کن OT_UINT8(u8)			(u8),
#تعریف_کن OT_UINT16(u16)			(بیعلامت حرف)((u16)>>8), (بیعلامت حرف)((u16)&0xFFu),
#تعریف_کن OT_شمار(Name, قلم_اندازه_2)	((بیعلامت صحيح) حجم(((بنية جدول_NAME*)0)->Name) \
					 / (بیعلامت صحيح)(قلم_اندازه_2) \
					 /* OT_ASSERT it's divisible (and positive). */)
#تعریف_کن OT_فاصله(From,مقصد)		((بیعلامت صحيح) \
					 ((حرف*)(&((بنية جدول_NAME*)0)->مقصد) - \
					  (حرف*)(&((بنية جدول_NAME*)0)->From)) \
					 /* OT_ASSERT it's positive. */)
#پایان_اگر


#تعریف_کن OT_LABEL(Name) \
	OT_LABEL_انتها \
	OT_LABEL_شروع(Name)

/* Whenever we receive an argument that is a list, it will expand to
 * contain commas.  That cannot be passed to another macro because the
 * commas will throw off the preprocessor.  The solution is to wrap
 * the passed-in argument in OT_LIST() before passing to the next macro.
 * Unfortunately this trick requires vararg macros. */
#تعریف_کن OT_LIST(...) __VA_ARGS__


/*
 * Basic Types
 */

#تعریف_کن OT_برچسب(a,b,c,d) \
	OT_UINT8(a) OT_UINT8(b) OT_UINT8(c) OT_UINT8(d)

#تعریف_کن OT_جابه_جایی(From, مقصد) /* Offset from From to To in bytes */ \
	OT_UINT16(OT_فاصله(From, مقصد))

#تعریف_کن OT_GLYPHID /* GlyphID */ \
	OT_UINT16
/* Shorthand. */
#تعریف_کن G	OT_GLYPHID

#تعریف_کن OT_UARRAY(Name, اقلام_2) \
	OT_LABEL_شروع(Name) \
	OT_UINT16(OT_شمار(Name##Data, 2)) \
	OT_LABEL(Name##Data) \
	اقلام_2 \
	OT_LABEL_انتها

#تعریف_کن OT_UHEADLESSARRAY(Name, اقلام_2) \
	OT_LABEL_شروع(Name) \
	OT_UINT16(OT_شمار(Name##Data, 2) + 1) \
	OT_LABEL(Name##Data) \
	اقلام_2 \
	OT_LABEL_انتها


/*
 * Common Types
 */

#تعریف_کن OT_LOOKUP_FLAG_IGNORE_MARKS	0x08u

#تعریف_کن OT_LOOKUP(Name, LookupType, LookupFlag, SubLookupOffsets) \
	OT_LABEL_شروع(Name) \
	OT_UINT16(LookupType) \
	OT_UINT16(LookupFlag) \
	OT_LABEL_انتها \
	OT_UARRAY(Name##SubLookupOffsetsArray, OT_LIST(SubLookupOffsets))

#تعریف_کن OT_SUBLOOKUP(Name, SubFormat, اقلام_2) \
	OT_LABEL_شروع(Name) \
	OT_UINT16(SubFormat) \
	اقلام_2

#تعریف_کن OT_COVERAGE1(Name, اقلام_2) \
	OT_LABEL_شروع(Name) \
	OT_UINT16(1) \
	OT_LABEL_انتها \
	OT_UARRAY(Name##Glyphs, OT_LIST(اقلام_2))


/*
 * GSUB
 */

#تعریف_کن OT_LOOKUP_TYPE_SUBST_SINGLE	1u
#تعریف_کن OT_LOOKUP_TYPE_SUBST_LIGATURE	4u

#تعریف_کن OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2(Name, FromGlyphs, مقصد_Glyphs) \
	OT_SUBLOOKUP(Name, 2, \
		OT_جابه_جایی(Name, Name##Coverage) \
		OT_LABEL_انتها \
		OT_UARRAY(Name##Substitute, OT_LIST(مقصد_Glyphs)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(FromGlyphs)) \
	/* static_assert_expr (len(FromGlyphs) == len(ToGlyphs)) */

#تعریف_کن OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1(Name, اولی_Glyphs, Ligature_مجموعه_Offsets) \
	OT_SUBLOOKUP(Name, 1, \
		OT_جابه_جایی(Name, Name##Coverage) \
		OT_LABEL_انتها \
		OT_UARRAY(Name##LigatureSetOffsetsArray, OT_LIST(Ligature_مجموعه_Offsets)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(اولی_Glyphs)) \
	/* static_assert_expr (len(FirstGlyphs) == len(LigatureSetOffsets)) */

#تعریف_کن OT_LIGATURE_مجموعه(Name, Ligature_مجموعه_Offsets) \
	OT_UARRAY(Name, OT_LIST(Ligature_مجموعه_Offsets))

#تعریف_کن OT_LIGATURE(Name, Components, LigGlyph) \
	OT_LABEL_شروع(Name) \
	LigGlyph \
	OT_LABEL_انتها \
	OT_UHEADLESSARRAY(Name##ComponentsArray, OT_LIST(Components))

/*
 *
 * Start of Windows-1256 shaping table.
 *
 */

/* Table name. */
#تعریف_کن جدول_NAME arabic_win1256_gsub_lookups

/* Table manifest. */
#تعریف_کن MANIFEST(اقلام_2) \
	OT_LABEL_شروع(manifest) \
	OT_UINT16(OT_شمار(manifest_داده, 6)) \
	OT_LABEL(manifest_داده) \
	اقلام_2 \
	OT_LABEL_انتها

#تعریف_کن MANIFEST_LOOKUP(برچسب, Name) \
	برچسب \
	OT_جابه_جایی(manifest, Name)


/*
 * Table Start
 */
OT_جدول_شروع


/*
 * Manifest
 */
MANIFEST(
	MANIFEST_LOOKUP(OT_برچسب('r','l','i','g'), rligLookup)
	MANIFEST_LOOKUP(OT_برچسب('i','n','i','t'), initLookup)
	MANIFEST_LOOKUP(OT_برچسب('m','e','d','i'), mediLookup)
	MANIFEST_LOOKUP(OT_برچسب('f','i','n','a'), finaLookup)
	MANIFEST_LOOKUP(OT_برچسب('r','l','i','g'), rligMarksLookup)
)

/*
 * Lookups
 */
OT_LOOKUP(initLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_جابه_جایی(initLookup, initmediSubLookup)
	OT_جابه_جایی(initLookup, initSubLookup)
)
OT_LOOKUP(mediLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_جابه_جایی(mediLookup, initmediSubLookup)
	OT_جابه_جایی(mediLookup, mediSubLookup)
	OT_جابه_جایی(mediLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(finaLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_جابه_جایی(finaLookup, finaSubLookup)
	/* We don't need this one currently as the sequence inherits masks
	 * from the first item.  Just in case we change that in the future
	 * to be smart about Arabic masks when ligating... */
	OT_جابه_جایی(finaLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(rligLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_جابه_جایی(rligLookup, lamAlefLigaturesSubLookup)
)
OT_LOOKUP(rligMarksLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, 0,
	OT_جابه_جایی(rligMarksLookup, shaddaLigaturesSubLookup)
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
	OT_جابه_جایی(lamAlefLigaturesSubLookup, lam_Ligature_مجموعه)
)
OT_LIGATURE_مجموعه(lam_Ligature_مجموعه,
	OT_جابه_جایی(lam_Ligature_مجموعه, lamInitLigature1)
	OT_جابه_جایی(lam_Ligature_مجموعه, lamInitLigature2)
	OT_جابه_جایی(lam_Ligature_مجموعه, lamInitLigature3)
	OT_جابه_جایی(lam_Ligature_مجموعه, lamInitLigature4)
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
	OT_جابه_جایی(shaddaLigaturesSubLookup, shadda_Ligature_مجموعه)
)
OT_LIGATURE_مجموعه(shadda_Ligature_مجموعه,
	OT_جابه_جایی(shadda_Ligature_مجموعه, shaddaLigature1)
	OT_جابه_جایی(shadda_Ligature_مجموعه, shaddaLigature2)
	OT_جابه_جایی(shadda_Ligature_مجموعه, shaddaLigature3)
)
OT_LIGATURE(shaddaLigature1, G(243), G(172))
OT_LIGATURE(shaddaLigature2, G(245), G(173))
OT_LIGATURE(shaddaLigature3, G(246), G(175))

/*
 * Table end
 */
OT_جدول_انتها


/*
 * Clean up
 */

#لغو_تعریف MANIFEST
#لغو_تعریف MANIFEST_LOOKUP

#لغو_تعریف OT_جدول_شروع
#لغو_تعریف OT_جدول_انتها
#لغو_تعریف OT_LABEL_شروع
#لغو_تعریف OT_LABEL_انتها
#لغو_تعریف OT_UINT8
#لغو_تعریف OT_UINT16
#لغو_تعریف OT_شمار
#لغو_تعریف OT_فاصله

#لغو_تعریف OT_LABEL
#لغو_تعریف OT_LIST

#لغو_تعریف OT_برچسب
#لغو_تعریف OT_جابه_جایی
#لغو_تعریف OT_GLYPHID
#لغو_تعریف G
#لغو_تعریف OT_UARRAY
#لغو_تعریف OT_UHEADLESSARRAY

#لغو_تعریف OT_LOOKUP_FLAG_IGNORE_MARKS
#لغو_تعریف OT_LOOKUP
#لغو_تعریف OT_SUBLOOKUP
#لغو_تعریف OT_COVERAGE1
#لغو_تعریف OT_LOOKUP_TYPE_SUBST_SINGLE
#لغو_تعریف OT_LOOKUP_TYPE_SUBST_LIGATURE
#لغو_تعریف OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2
#لغو_تعریف OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1
#لغو_تعریف OT_LIGATURE_مجموعه
#لغو_تعریف OT_LIGATURE


/*
 * Include a second time to get the table data...
 */
#إذا 0
#ضمن "hb.hh" /* Make check-includes.sh happy. */
#پایان_اگر
#اگر_تعریف_شده OT_MEASURE
#ضمن "hb-ot-shaper-arabic-win1256.hh"
#پایان_اگر

#تعریف_کن HB_OT_SHAPER_ARABIC_WIN1256_HH
#پایان_اگر /* HB_OT_SHAPER_ARABIC_WIN1256_HH */
