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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_SHAPER_ARABIC_WIN1256_HH


/*
 * The macros in the first part of this file are generic macros that can
 * be used to define the bytes for OpenType table data in code in a
 * readable manner.  We can move the macros to reside with their respective
 * struct types, but since we only use these to define one data table, the
 * Windows-1256 Arabic shaping table in this file, we keep them here.
 */


/* First we measure, then we cut. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_MEASURE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_MEASURE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_ਸਾਰਣੀ_ਸ਼ੁਰੂ			ਸਥਿਰ ਅਚਲ ਬਣਤਰ ਸਾਰਣੀ_NAME {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_ਸਾਰਣੀ_ਸਿਰਾ			}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LABEL_ਸ਼ੁਰੂ(Name)		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ Name[
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LABEL_ਸਿਰਾ			];
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_UINT8(u8)			+1/*byte*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_UINT16(u16)			+2/*bytes*/
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  OT_MEASURE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_ਸਾਰਣੀ_ਸ਼ੁਰੂ			ਸਾਰਣੀ_NAME = {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_ਸਾਰਣੀ_ਸਿਰਾ			};
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LABEL_ਸ਼ੁਰੂ(Name)		{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LABEL_ਸਿਰਾ			},
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_UINT8(u8)			(u8),
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_UINT16(u16)			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)((u16)>>8), (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)((u16)&0xFFu),
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_ਗਿਣਤੀ(Name, ਵਸਤੂ_ਮਾਪ_2)	((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ਆਕਾਰ(((ਬਣਤਰ ਸਾਰਣੀ_NAME*)0)->Name) \
					 / (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(ਵਸਤੂ_ਮਾਪ_2) \
					 /* OT_ASSERT it's divisible (and positive). */)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_ਦੂਰੀ(From,ਮੰਜ਼ਿਲ)		((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) \
					 ((ਅੱਖਰ*)(&((ਬਣਤਰ ਸਾਰਣੀ_NAME*)0)->ਮੰਜ਼ਿਲ) - \
					  (ਅੱਖਰ*)(&((ਬਣਤਰ ਸਾਰਣੀ_NAME*)0)->From)) \
					 /* OT_ASSERT it's positive. */)
#ਜੇ_ਅੰਤ


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LABEL(Name) \
	OT_LABEL_ਸਿਰਾ \
	OT_LABEL_ਸ਼ੁਰੂ(Name)

/* Whenever we receive an argument that is a list, it will expand to
 * contain commas.  That cannot be passed to another macro because the
 * commas will throw off the preprocessor.  The solution is to wrap
 * the passed-in argument in OT_LIST() before passing to the next macro.
 * Unfortunately this trick requires vararg macros. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LIST(...) __VA_ARGS__


/*
 * Basic Types
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_ਟੈਗ(a,b,c,d) \
	OT_UINT8(a) OT_UINT8(b) OT_UINT8(c) OT_UINT8(d)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_ਵਿੱਥ(From, ਮੰਜ਼ਿਲ) /* Offset from From to To in bytes */ \
	OT_UINT16(OT_ਦੂਰੀ(From, ਮੰਜ਼ਿਲ))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_GLYPHID /* GlyphID */ \
	OT_UINT16
/* Shorthand. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ G	OT_GLYPHID

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_UARRAY(Name, ਚੀਜ਼ਾਂ_2) \
	OT_LABEL_ਸ਼ੁਰੂ(Name) \
	OT_UINT16(OT_ਗਿਣਤੀ(Name##Data, 2)) \
	OT_LABEL(Name##Data) \
	ਚੀਜ਼ਾਂ_2 \
	OT_LABEL_ਸਿਰਾ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_UHEADLESSARRAY(Name, ਚੀਜ਼ਾਂ_2) \
	OT_LABEL_ਸ਼ੁਰੂ(Name) \
	OT_UINT16(OT_ਗਿਣਤੀ(Name##Data, 2) + 1) \
	OT_LABEL(Name##Data) \
	ਚੀਜ਼ਾਂ_2 \
	OT_LABEL_ਸਿਰਾ


/*
 * Common Types
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LOOKUP_FLAG_IGNORE_MARKS	0x08u

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LOOKUP(Name, LookupType, LookupFlag, SubLookupOffsets) \
	OT_LABEL_ਸ਼ੁਰੂ(Name) \
	OT_UINT16(LookupType) \
	OT_UINT16(LookupFlag) \
	OT_LABEL_ਸਿਰਾ \
	OT_UARRAY(Name##SubLookupOffsetsArray, OT_LIST(SubLookupOffsets))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_SUBLOOKUP(Name, SubFormat, ਚੀਜ਼ਾਂ_2) \
	OT_LABEL_ਸ਼ੁਰੂ(Name) \
	OT_UINT16(SubFormat) \
	ਚੀਜ਼ਾਂ_2

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_COVERAGE1(Name, ਚੀਜ਼ਾਂ_2) \
	OT_LABEL_ਸ਼ੁਰੂ(Name) \
	OT_UINT16(1) \
	OT_LABEL_ਸਿਰਾ \
	OT_UARRAY(Name##Glyphs, OT_LIST(ਚੀਜ਼ਾਂ_2))


/*
 * GSUB
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LOOKUP_TYPE_SUBST_SINGLE	1u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LOOKUP_TYPE_SUBST_LIGATURE	4u

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2(Name, FromGlyphs, ਮੰਜ਼ਿਲ_Glyphs) \
	OT_SUBLOOKUP(Name, 2, \
		OT_ਵਿੱਥ(Name, Name##Coverage) \
		OT_LABEL_ਸਿਰਾ \
		OT_UARRAY(Name##Substitute, OT_LIST(ਮੰਜ਼ਿਲ_Glyphs)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(FromGlyphs)) \
	/* static_assert_expr (len(FromGlyphs) == len(ToGlyphs)) */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1(Name, ਪਹਿਲਾ_Glyphs, Ligature_ਸੈੱਟ_Offsets) \
	OT_SUBLOOKUP(Name, 1, \
		OT_ਵਿੱਥ(Name, Name##Coverage) \
		OT_LABEL_ਸਿਰਾ \
		OT_UARRAY(Name##LigatureSetOffsetsArray, OT_LIST(Ligature_ਸੈੱਟ_Offsets)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(ਪਹਿਲਾ_Glyphs)) \
	/* static_assert_expr (len(FirstGlyphs) == len(LigatureSetOffsets)) */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LIGATURE_ਸੈੱਟ(Name, Ligature_ਸੈੱਟ_Offsets) \
	OT_UARRAY(Name, OT_LIST(Ligature_ਸੈੱਟ_Offsets))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LIGATURE(Name, Components, LigGlyph) \
	OT_LABEL_ਸ਼ੁਰੂ(Name) \
	LigGlyph \
	OT_LABEL_ਸਿਰਾ \
	OT_UHEADLESSARRAY(Name##ComponentsArray, OT_LIST(Components))

/*
 *
 * Start of Windows-1256 shaping table.
 *
 */

/* Table name. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਾਰਣੀ_NAME arabic_win1256_gsub_lookups

/* Table manifest. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MANIFEST(ਚੀਜ਼ਾਂ_2) \
	OT_LABEL_ਸ਼ੁਰੂ(manifest) \
	OT_UINT16(OT_ਗਿਣਤੀ(manifest_ਅੰਕੜੇ, 6)) \
	OT_LABEL(manifest_ਅੰਕੜੇ) \
	ਚੀਜ਼ਾਂ_2 \
	OT_LABEL_ਸਿਰਾ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MANIFEST_LOOKUP(ਟੈਗ, Name) \
	ਟੈਗ \
	OT_ਵਿੱਥ(manifest, Name)


/*
 * Table Start
 */
OT_ਸਾਰਣੀ_ਸ਼ੁਰੂ


/*
 * Manifest
 */
MANIFEST(
	MANIFEST_LOOKUP(OT_ਟੈਗ('r','l','i','g'), rligLookup)
	MANIFEST_LOOKUP(OT_ਟੈਗ('i','n','i','t'), initLookup)
	MANIFEST_LOOKUP(OT_ਟੈਗ('m','e','d','i'), mediLookup)
	MANIFEST_LOOKUP(OT_ਟੈਗ('f','i','n','a'), finaLookup)
	MANIFEST_LOOKUP(OT_ਟੈਗ('r','l','i','g'), rligMarksLookup)
)

/*
 * Lookups
 */
OT_LOOKUP(initLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_ਵਿੱਥ(initLookup, initmediSubLookup)
	OT_ਵਿੱਥ(initLookup, initSubLookup)
)
OT_LOOKUP(mediLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_ਵਿੱਥ(mediLookup, initmediSubLookup)
	OT_ਵਿੱਥ(mediLookup, mediSubLookup)
	OT_ਵਿੱਥ(mediLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(finaLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_ਵਿੱਥ(finaLookup, finaSubLookup)
	/* We don't need this one currently as the sequence inherits masks
	 * from the first item.  Just in case we change that in the future
	 * to be smart about Arabic masks when ligating... */
	OT_ਵਿੱਥ(finaLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(rligLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_ਵਿੱਥ(rligLookup, lamAlefLigaturesSubLookup)
)
OT_LOOKUP(rligMarksLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, 0,
	OT_ਵਿੱਥ(rligMarksLookup, shaddaLigaturesSubLookup)
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
	OT_ਵਿੱਥ(lamAlefLigaturesSubLookup, lam_Ligature_ਸੈੱਟ)
)
OT_LIGATURE_ਸੈੱਟ(lam_Ligature_ਸੈੱਟ,
	OT_ਵਿੱਥ(lam_Ligature_ਸੈੱਟ, lamInitLigature1)
	OT_ਵਿੱਥ(lam_Ligature_ਸੈੱਟ, lamInitLigature2)
	OT_ਵਿੱਥ(lam_Ligature_ਸੈੱਟ, lamInitLigature3)
	OT_ਵਿੱਥ(lam_Ligature_ਸੈੱਟ, lamInitLigature4)
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
	OT_ਵਿੱਥ(shaddaLigaturesSubLookup, shadda_Ligature_ਸੈੱਟ)
)
OT_LIGATURE_ਸੈੱਟ(shadda_Ligature_ਸੈੱਟ,
	OT_ਵਿੱਥ(shadda_Ligature_ਸੈੱਟ, shaddaLigature1)
	OT_ਵਿੱਥ(shadda_Ligature_ਸੈੱਟ, shaddaLigature2)
	OT_ਵਿੱਥ(shadda_Ligature_ਸੈੱਟ, shaddaLigature3)
)
OT_LIGATURE(shaddaLigature1, G(243), G(172))
OT_LIGATURE(shaddaLigature2, G(245), G(173))
OT_LIGATURE(shaddaLigature3, G(246), G(175))

/*
 * Table end
 */
OT_ਸਾਰਣੀ_ਸਿਰਾ


/*
 * Clean up
 */

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MANIFEST
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MANIFEST_LOOKUP

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_ਸਾਰਣੀ_ਸ਼ੁਰੂ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_ਸਾਰਣੀ_ਸਿਰਾ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_LABEL_ਸ਼ੁਰੂ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_LABEL_ਸਿਰਾ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_UINT8
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_UINT16
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_ਗਿਣਤੀ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_ਦੂਰੀ

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_LABEL
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_LIST

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_ਟੈਗ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_ਵਿੱਥ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_GLYPHID
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ G
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_UARRAY
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_UHEADLESSARRAY

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_LOOKUP_FLAG_IGNORE_MARKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_LOOKUP
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_SUBLOOKUP
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_COVERAGE1
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_LOOKUP_TYPE_SUBST_SINGLE
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_LOOKUP_TYPE_SUBST_LIGATURE
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_LIGATURE_ਸੈੱਟ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ OT_LIGATURE


/*
 * Include a second time to get the table data...
 */
#ਜੇ 0
#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh" /* Make check-includes.sh happy. */
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OT_MEASURE
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-arabic-win1256.hh"
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPER_ARABIC_WIN1256_HH
#ਜੇ_ਅੰਤ /* HB_OT_SHAPER_ARABIC_WIN1256_HH */
