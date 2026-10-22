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

#अगर_अघोषित HB_OT_SHAPER_ARABIC_WIN1256_HH


/*
 * The macros in the first part of this file are generic macros that can
 * be used to define the bytes for OpenType table data in code in a
 * readable manner.  We can move the macros to reside with their respective
 * struct types, but since we only use these to define one data table, the
 * Windows-1256 Arabic shaping table in this file, we keep them here.
 */


/* First we measure, then we cut. */
#अगर_अघोषित OT_MEASURE
#घोषणा OT_MEASURE
#घोषणा OT_TABLE_START			अटल स्थिर काष्ठा TABLE_NAME {
#घोषणा OT_TABLE_END			}
#घोषणा OT_LABEL_START(Name)		अचिन्हित अक्षर Name[
#घोषणा OT_LABEL_END			];
#घोषणा OT_UINT8(u8)			+1/*byte*/
#घोषणा OT_UINT16(u16)			+2/*bytes*/
#अन्यथा
#अघोषित  OT_MEASURE
#घोषणा OT_TABLE_START			TABLE_NAME = {
#घोषणा OT_TABLE_END			};
#घोषणा OT_LABEL_START(Name)		{
#घोषणा OT_LABEL_END			},
#घोषणा OT_UINT8(u8)			(u8),
#घोषणा OT_UINT16(u16)			(अचिन्हित अक्षर)((u16)>>8), (अचिन्हित अक्षर)((u16)&0xFFu),
#घोषणा OT_गिनती(Name, Item_आकार)	((अचिन्हित पूर्णांक) माप(((काष्ठा TABLE_NAME*)0)->Name) \
					 / (अचिन्हित पूर्णांक)(Item_आकार) \
					 /* OT_ASSERT it's divisible (and positive). */)
#घोषणा OT_DISTANCE(From,To)		((अचिन्हित पूर्णांक) \
					 ((अक्षर*)(&((काष्ठा TABLE_NAME*)0)->To) - \
					  (अक्षर*)(&((काष्ठा TABLE_NAME*)0)->From)) \
					 /* OT_ASSERT it's positive. */)
#पूर्ण_अगर


#घोषणा OT_LABEL(Name) \
	OT_LABEL_END \
	OT_LABEL_START(Name)

/* Whenever we receive an argument that is a list, it will expand to
 * contain commas.  That cannot be passed to another macro because the
 * commas will throw off the preprocessor.  The solution is to wrap
 * the passed-in argument in OT_LIST() before passing to the next macro.
 * Unfortunately this trick requires vararg macros. */
#घोषणा OT_LIST(...) __VA_ARGS__


/*
 * Basic Types
 */

#घोषणा OT_TAG(a,b,c,d) \
	OT_UINT8(a) OT_UINT8(b) OT_UINT8(c) OT_UINT8(d)

#घोषणा OT_OFFSET(From, To) /* Offset from From to To in bytes */ \
	OT_UINT16(OT_DISTANCE(From, To))

#घोषणा OT_GLYPHID /* GlyphID */ \
	OT_UINT16
/* Shorthand. */
#घोषणा G	OT_GLYPHID

#घोषणा OT_UARRAY(Name, वस्तुएँ_2) \
	OT_LABEL_START(Name) \
	OT_UINT16(OT_गिनती(Name##Data, 2)) \
	OT_LABEL(Name##Data) \
	वस्तुएँ_2 \
	OT_LABEL_END

#घोषणा OT_UHEADLESSARRAY(Name, वस्तुएँ_2) \
	OT_LABEL_START(Name) \
	OT_UINT16(OT_गिनती(Name##Data, 2) + 1) \
	OT_LABEL(Name##Data) \
	वस्तुएँ_2 \
	OT_LABEL_END


/*
 * Common Types
 */

#घोषणा OT_LOOKUP_FLAG_IGNORE_MARKS	0x08u

#घोषणा OT_LOOKUP(Name, LookupType, LookupFlag, SubLookupOffsets) \
	OT_LABEL_START(Name) \
	OT_UINT16(LookupType) \
	OT_UINT16(LookupFlag) \
	OT_LABEL_END \
	OT_UARRAY(Name##SubLookupOffsetsArray, OT_LIST(SubLookupOffsets))

#घोषणा OT_SUBLOOKUP(Name, SubFormat, वस्तुएँ_2) \
	OT_LABEL_START(Name) \
	OT_UINT16(SubFormat) \
	वस्तुएँ_2

#घोषणा OT_COVERAGE1(Name, वस्तुएँ_2) \
	OT_LABEL_START(Name) \
	OT_UINT16(1) \
	OT_LABEL_END \
	OT_UARRAY(Name##Glyphs, OT_LIST(वस्तुएँ_2))


/*
 * GSUB
 */

#घोषणा OT_LOOKUP_TYPE_SUBST_SINGLE	1u
#घोषणा OT_LOOKUP_TYPE_SUBST_LIGATURE	4u

#घोषणा OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2(Name, FromGlyphs, ToGlyphs) \
	OT_SUBLOOKUP(Name, 2, \
		OT_OFFSET(Name, Name##Coverage) \
		OT_LABEL_END \
		OT_UARRAY(Name##Substitute, OT_LIST(ToGlyphs)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(FromGlyphs)) \
	/* static_assert_expr (len(FromGlyphs) == len(ToGlyphs)) */

#घोषणा OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1(Name, FirstGlyphs, LigatureSetOffsets) \
	OT_SUBLOOKUP(Name, 1, \
		OT_OFFSET(Name, Name##Coverage) \
		OT_LABEL_END \
		OT_UARRAY(Name##LigatureSetOffsetsArray, OT_LIST(LigatureSetOffsets)) \
	) \
	OT_COVERAGE1(Name##Coverage, OT_LIST(FirstGlyphs)) \
	/* static_assert_expr (len(FirstGlyphs) == len(LigatureSetOffsets)) */

#घोषणा OT_LIGATURE_SET(Name, LigatureSetOffsets) \
	OT_UARRAY(Name, OT_LIST(LigatureSetOffsets))

#घोषणा OT_LIGATURE(Name, Components, LigGlyph) \
	OT_LABEL_START(Name) \
	LigGlyph \
	OT_LABEL_END \
	OT_UHEADLESSARRAY(Name##ComponentsArray, OT_LIST(Components))

/*
 *
 * Start of Windows-1256 shaping table.
 *
 */

/* Table name. */
#घोषणा TABLE_NAME arabic_win1256_gsub_lookups

/* Table manifest. */
#घोषणा MANIFEST(वस्तुएँ_2) \
	OT_LABEL_START(manifest) \
	OT_UINT16(OT_गिनती(manifest_तथ्य, 6)) \
	OT_LABEL(manifest_तथ्य) \
	वस्तुएँ_2 \
	OT_LABEL_END

#घोषणा MANIFEST_LOOKUP(Tag, Name) \
	Tag \
	OT_OFFSET(manifest, Name)


/*
 * Table Start
 */
OT_TABLE_START


/*
 * Manifest
 */
MANIFEST(
	MANIFEST_LOOKUP(OT_TAG('r','l','i','g'), rligLookup)
	MANIFEST_LOOKUP(OT_TAG('i','n','i','t'), initLookup)
	MANIFEST_LOOKUP(OT_TAG('m','e','d','i'), mediLookup)
	MANIFEST_LOOKUP(OT_TAG('f','i','n','a'), finaLookup)
	MANIFEST_LOOKUP(OT_TAG('r','l','i','g'), rligMarksLookup)
)

/*
 * Lookups
 */
OT_LOOKUP(initLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_OFFSET(initLookup, initmediSubLookup)
	OT_OFFSET(initLookup, initSubLookup)
)
OT_LOOKUP(mediLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_OFFSET(mediLookup, initmediSubLookup)
	OT_OFFSET(mediLookup, mediSubLookup)
	OT_OFFSET(mediLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(finaLookup, OT_LOOKUP_TYPE_SUBST_SINGLE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_OFFSET(finaLookup, finaSubLookup)
	/* We don't need this one currently as the sequence inherits masks
	 * from the first item.  Just in case we change that in the future
	 * to be smart about Arabic masks when ligating... */
	OT_OFFSET(finaLookup, medifinaLamAlefSubLookup)
)
OT_LOOKUP(rligLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, OT_LOOKUP_FLAG_IGNORE_MARKS,
	OT_OFFSET(rligLookup, lamAlefLigaturesSubLookup)
)
OT_LOOKUP(rligMarksLookup, OT_LOOKUP_TYPE_SUBST_LIGATURE, 0,
	OT_OFFSET(rligMarksLookup, shaddaLigaturesSubLookup)
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
	OT_OFFSET(lamAlefLigaturesSubLookup, lamLigatureSet)
)
OT_LIGATURE_SET(lamLigatureSet,
	OT_OFFSET(lamLigatureSet, lamInitLigature1)
	OT_OFFSET(lamLigatureSet, lamInitLigature2)
	OT_OFFSET(lamLigatureSet, lamInitLigature3)
	OT_OFFSET(lamLigatureSet, lamInitLigature4)
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
	OT_OFFSET(shaddaLigaturesSubLookup, shaddaLigatureSet)
)
OT_LIGATURE_SET(shaddaLigatureSet,
	OT_OFFSET(shaddaLigatureSet, shaddaLigature1)
	OT_OFFSET(shaddaLigatureSet, shaddaLigature2)
	OT_OFFSET(shaddaLigatureSet, shaddaLigature3)
)
OT_LIGATURE(shaddaLigature1, G(243), G(172))
OT_LIGATURE(shaddaLigature2, G(245), G(173))
OT_LIGATURE(shaddaLigature3, G(246), G(175))

/*
 * Table end
 */
OT_TABLE_END


/*
 * Clean up
 */

#अघोषित MANIFEST
#अघोषित MANIFEST_LOOKUP

#अघोषित OT_TABLE_START
#अघोषित OT_TABLE_END
#अघोषित OT_LABEL_START
#अघोषित OT_LABEL_END
#अघोषित OT_UINT8
#अघोषित OT_UINT16
#अघोषित OT_गिनती
#अघोषित OT_DISTANCE

#अघोषित OT_LABEL
#अघोषित OT_LIST

#अघोषित OT_TAG
#अघोषित OT_OFFSET
#अघोषित OT_GLYPHID
#अघोषित G
#अघोषित OT_UARRAY
#अघोषित OT_UHEADLESSARRAY

#अघोषित OT_LOOKUP_FLAG_IGNORE_MARKS
#अघोषित OT_LOOKUP
#अघोषित OT_SUBLOOKUP
#अघोषित OT_COVERAGE1
#अघोषित OT_LOOKUP_TYPE_SUBST_SINGLE
#अघोषित OT_LOOKUP_TYPE_SUBST_LIGATURE
#अघोषित OT_SUBLOOKUP_SINGLE_SUBST_FORMAT2
#अघोषित OT_SUBLOOKUP_LIGATURE_SUBST_FORMAT1
#अघोषित OT_LIGATURE_SET
#अघोषित OT_LIGATURE


/*
 * Include a second time to get the table data...
 */
#अगर 0
#समावेश "hb.hh" /* Make check-includes.sh happy. */
#पूर्ण_अगर
#अगर_घोषित OT_MEASURE
#समावेश "hb-ot-shaper-arabic-win1256.hh"
#पूर्ण_अगर

#घोषणा HB_OT_SHAPER_ARABIC_WIN1256_HH
#पूर्ण_अगर /* HB_OT_SHAPER_ARABIC_WIN1256_HH */
