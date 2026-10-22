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

#که_نه_تعریف_شوی HB_OT_SHAPER_ARABIC_WIN1256_HH


/*
 * The macros in the first part of this file are generic macros that can
 * be used to define the bytes for OpenType table data in code in a
 * readable manner.  We can move the macros to reside with their respective
 * struct types, but since we only use these to define one data table, the
 * Windows-1256 Arabic shaping table in this file, we keep them here.
 */


/* First we measure, then we cut. */
#که_نه_تعریف_شوی OT_MEASURE
#تعریف_کړه OT_MEASURE
#تعریف_کړه OT_جدول_پیل			ساكن ثابت بنية جدول_NAME {
#تعریف_کړه OT_جدول_پای_ټکی			}
#تعریف_کړه OT_LABEL_پیل(Name)		بینښه حرف Name[
#تعریف_کړه OT_LABEL_پای_ټکی			];
#تعریف_کړه OT_UINT8(u8)			+1/*byte*/
#تعریف_کړه OT_UINT16(u16)			+2/*bytes*/
#وإلا
#تعریف_لغوه  OT_MEASURE
#تعریف_کړه OT_جدول_پیل			جدول_NAME = {
#تعریف_کړه OT_جدول_پای_ټکی			};
#تعریف_کړه OT_LABEL_پیل(Name)		{
#تعریف_کړه OT_LABEL_پای_ټکی			},
#تعریف_کړه OT_UINT8(u8)			(u8),
#تعریف_کړه OT_UINT16(u16)			(بینښه حرف)((u16)>>8), (بینښه حرف)((u16)&0xFFu),
#تعریف_کړه OT_شمېر(Name, توکی_اندازه_2)	((بینښه صحيح) حجم(((بنية جدول_NAME*)0)->Name) \
					 / (بینښه صحيح)(توکی_اندازه_2) \
					 /* OT_ASSERT it's divisible (and positive). */)
#تعریف_کړه OT_واټن(From,منزل)		((بینښه صحيح) \
					 ((حرف*)(&((بنية جدول_NAME*)0)->منزل) - \
					  (حرف*)(&((بنية جدول_NAME*)0)->From)) \
					 /* OT_ASSERT it's positive. */)
#د_که_پای


#تعریف_کړه OT_LABEL(Name) \
	OT_LABEL_پای_ټکی \
	OT_LABEL_پیل(Name)

/* Whenever we receive an argument that is a list, it will expand to
 * contain commas.  That cannot be passed to another macro because the
 * commas will throw off the preprocessor.  The solution is to wrap
 * the passed-in argument in OT_LIST() before passing to the next macro.
 * Unfortunately this trick requires vararg macros. */
#تعریف_کړه OT_LIST(...) __VA_ARGS__


/*
 * Basic Types
 */

#تعریف_کړه OT_نښان(a,b,c,d) \
	OT_UINT8(a) OT_UINT8(b) OT_UINT8(c) OT_UINT8(d)

#تعریف_کړه OT_دننه_واټن(From, منزل) /* Offset from From to To in bytes */ \
	OT_UINT16(OT_واټن(From, منزل))

#تعریف_کړه OT_GLYPHID /* GlyphID */ \
	OT_UINT16
/* Shorthand. */
#تعریف_کړه G	OT_GLYPHID

#تعریف_کړه OT_UARRAY(Name, توکي_2) \
	OT_LABEL_پیل(Name) \
	OT_UINT16(OT_شمېر(Name##Data, 2)) \
	OT_LABEL(Name##Data) \
	توکي_2 \
	OT_LABEL_پای_ټکی

#تعریف_کړه OT_UHEADLESSARRAY(Name, توکي_2) \
	OT_LABEL_پیل(Name) \
	OT_UINT16(OT_شمېر(Name##Data, 2) + 1) \
	OT_LABEL(Name##Data) \
	توکي_2 \
	OT_LABEL_پای_ټکی


/*
 * Common Types
 */

#تعریف_کړه OT_LOOKUP_FLAG_IGNORE_MARKS	0x08u

#تعریف_کړه OT_LOOKUP(Name, LookupType, LookupFlag, SubLookupOffsets) \
	OT_LABEL_پیل(Name) \
	OT_UINT16(LookupType) \
	OT_UINT16(LookupFlag) \
	OT_LABEL_پای_ټکی \
	OT_UARRAY(Name##SubLookupOffsetsArray, OT_LIST(SubLookupOffsets))

#تعریف_کړه OT_SUBLOOKUP(Name, SubFormat, توکي_2) \
	OT_LABEL_پیل(Name) \
	OT_UINT16(SubFormat) \
	توکي_2

#تعریف_کړه OT_COVERAGE1(Name, توکي_2) \
	OT_LABEL_پیل(Name) \
	OT_UINT16(1) \
	OT_LABEL_پای_ټکی \
	OT_UARRAY(Name##Glyphs, OT_LIST(توکي_2))


/*
 * GSUB
 */

#تعریف_کړه OT_LOOKUP_TYPE_SUBST_SINGLE	1u
#تعریف_کړه OT_LOOKUP_TYPE_SUBST_LIGATURE	4u

#تعریف_کړه OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2(Name, FromGlyphs, منزل_Glyphs) \
	OT_SUBLOOKUP(Name, 2, \
		OT_دننه_واټن(Name, Name##Coverage) \
		OT_LABEL_پای_ټکی \
		OT_UARRAY(Name##Substitute, OT_LIST(منزل_Glyphs)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(FromGlyphs)) \
	/* static_assert_expr (len(FromGlyphs) == len(ToGlyphs)) */

#تعریف_کړه OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1(Name, لومړی_Glyphs, Ligature_سېټ_Offsets) \
	OT_SUBLOOKUP(Name, 1, \
		OT_دننه_واټن(Name, Name##Coverage) \
		OT_LABEL_پای_ټکی \
		OT_UARRAY(Name##LigatureSetOffsetsArray, OT_LIST(Ligature_سېټ_Offsets)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(لومړی_Glyphs)) \
	/* static_assert_expr (len(FirstGlyphs) == len(LigatureSetOffsets)) */

#تعریف_کړه OT_LIGATURE_سېټ(Name, Ligature_سېټ_Offsets) \
	OT_UARRAY(Name, OT_LIST(Ligature_سېټ_Offsets))

#تعریف_کړه OT_LIGATURE(Name, Components, LigGlyph) \
	OT_LABEL_پیل(Name) \
	LigGlyph \
	OT_LABEL_پای_ټکی \
	OT_UHEADLESSARRAY(Name##ComponentsArray, OT_LIST(Components))

/*
 *
 * Start of Windows-1256 shaping table.
 *
 */

/* Table name. */
#تعریف_کړه جدول_NAME arabic_win1256_gsub_lookups

/* Table manifest. */
#تعریف_کړه MANIFEST(توکي_2) \
	OT_LABEL_پیل(manifest) \
	OT_UINT16(OT_شمېر(manifest_معلومات, 6)) \
	OT_LABEL(manifest_معلومات) \
	توکي_2 \
	OT_LABEL_پای_ټکی

#تعریف_کړه MANIFEST_LOOKUP(نښان, Name) \
	نښان \
	OT_دننه_واټن(manifest, Name)


/*
 * Table Start
 */
OT_جدول_پیل


/*
 * Manifest
 */
MANIFEST(
	MANIFEST_LOOKUP(OT_نښان('r','l','i','g'), rligLookup)
	MANIFEST_LOOKUP(OT_نښان('i','n','i','t'), initLookup)
	MANIFEST_LOOKUP(OT_نښان('m','e','d','i'), mediLookup)
	MANIFEST_LOOKUP(OT_نښان('f','i','n','a'), finaLookup)
	MANIFEST_LOOKUP(OT_نښان('r','l','i','g'), rligMarksLookup)
)

/*
 * Lookups
 */
OT_LOOKUP(initLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_دننه_واټن(initLookup, initmediSubLookup)
	OT_دننه_واټن(initLookup, initSubLookup)
)
OT_LOOKUP(mediLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_دننه_واټن(mediLookup, initmediSubLookup)
	OT_دننه_واټن(mediLookup, mediSubLookup)
	OT_دننه_واټن(mediLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(finaLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_دننه_واټن(finaLookup, finaSubLookup)
	/* We don't need this one currently as the sequence inherits masks
	 * from the first item.  Just in case we change that in the future
	 * to be smart about Arabic masks when ligating... */
	OT_دننه_واټن(finaLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(rligLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_دننه_واټن(rligLookup, lamAlefLigaturesSubLookup)
)
OT_LOOKUP(rligMarksLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, 0,
	OT_دننه_واټن(rligMarksLookup, shaddaLigaturesSubLookup)
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
	OT_دننه_واټن(lamAlefLigaturesSubLookup, lam_Ligature_سېټ)
)
OT_LIGATURE_سېټ(lam_Ligature_سېټ,
	OT_دننه_واټن(lam_Ligature_سېټ, lamInitLigature1)
	OT_دننه_واټن(lam_Ligature_سېټ, lamInitLigature2)
	OT_دننه_واټن(lam_Ligature_سېټ, lamInitLigature3)
	OT_دننه_واټن(lam_Ligature_سېټ, lamInitLigature4)
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
	OT_دننه_واټن(shaddaLigaturesSubLookup, shadda_Ligature_سېټ)
)
OT_LIGATURE_سېټ(shadda_Ligature_سېټ,
	OT_دننه_واټن(shadda_Ligature_سېټ, shaddaLigature1)
	OT_دننه_واټن(shadda_Ligature_سېټ, shaddaLigature2)
	OT_دننه_واټن(shadda_Ligature_سېټ, shaddaLigature3)
)
OT_LIGATURE(shaddaLigature1, G(243), G(172))
OT_LIGATURE(shaddaLigature2, G(245), G(173))
OT_LIGATURE(shaddaLigature3, G(246), G(175))

/*
 * Table end
 */
OT_جدول_پای_ټکی


/*
 * Clean up
 */

#تعریف_لغوه MANIFEST
#تعریف_لغوه MANIFEST_LOOKUP

#تعریف_لغوه OT_جدول_پیل
#تعریف_لغوه OT_جدول_پای_ټکی
#تعریف_لغوه OT_LABEL_پیل
#تعریف_لغوه OT_LABEL_پای_ټکی
#تعریف_لغوه OT_UINT8
#تعریف_لغوه OT_UINT16
#تعریف_لغوه OT_شمېر
#تعریف_لغوه OT_واټن

#تعریف_لغوه OT_LABEL
#تعریف_لغوه OT_LIST

#تعریف_لغوه OT_نښان
#تعریف_لغوه OT_دننه_واټن
#تعریف_لغوه OT_GLYPHID
#تعریف_لغوه G
#تعریف_لغوه OT_UARRAY
#تعریف_لغوه OT_UHEADLESSARRAY

#تعریف_لغوه OT_LOOKUP_FLAG_IGNORE_MARKS
#تعریف_لغوه OT_LOOKUP
#تعریف_لغوه OT_SUBLOOKUP
#تعریف_لغوه OT_COVERAGE1
#تعریف_لغوه OT_LOOKUP_TYPE_SUBST_SINGLE
#تعریف_لغوه OT_LOOKUP_TYPE_SUBST_LIGATURE
#تعریف_لغوه OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2
#تعریف_لغوه OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1
#تعریف_لغوه OT_LIGATURE_سېټ
#تعریف_لغوه OT_LIGATURE


/*
 * Include a second time to get the table data...
 */
#إذا 0
#ضمن "hb.hh" /* Make check-includes.sh happy. */
#د_که_پای
#که_تعریف_شوی OT_MEASURE
#ضمن "hb-ot-shaper-arabic-win1256.hh"
#د_که_پای

#تعریف_کړه HB_OT_SHAPER_ARABIC_WIN1256_HH
#د_که_پای /* HB_OT_SHAPER_ARABIC_WIN1256_HH */
