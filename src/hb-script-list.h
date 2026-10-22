/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
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
 */

#ਜੇ !defined(HB_H_ਵਿੱਚ) && !defined(HB_NO_SINGLE_ਸਿਰਲੇਖ_ERROR)
#ਗਲਤੀ "Include <hb.h> instead."
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SCRIPT_LIST_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SCRIPT_LIST_H

/* This file belongs to the middle of hb-common.h.
 * The reason it has been surgically extracted is because
 * FreeType imports types and enums from hb-common.h,
 * and since this enum is large and growing, we want to
 * make it easy to just copy the file over to FreeType.
 * https://github.com/harfbuzz/harfbuzz/issues/5271
 */

/* Dummy lines to make our checks happy.  */
#ਜੇ 0
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-common.h"
HB_BEGIN_DECLS
HB_ਸਿਰਾ_DECLS
#ਜੇ_ਅੰਤ


/**
 * hb_script_t:
 * @HB_SCRIPT_COMMON: `Zyyy`
 * @HB_SCRIPT_INHERITED: `Zinh`
 * @HB_SCRIPT_UNKNOWN: `Zzzz`
 * @HB_SCRIPT_ARABIC: `Arab`
 * @HB_SCRIPT_ARMENIAN: `Armn`
 * @HB_SCRIPT_BENGALI: `Beng`
 * @HB_SCRIPT_CYRILLIC: `Cyrl`
 * @HB_SCRIPT_DEVANAGARI: `Deva`
 * @HB_SCRIPT_GEORGIAN: `Geor`
 * @HB_SCRIPT_GREEK: `Grek`
 * @HB_SCRIPT_GUJARATI: `Gujr`
 * @HB_SCRIPT_GURMUKHI: `Guru`
 * @HB_SCRIPT_HANGUL: `Hang`
 * @HB_SCRIPT_HAN: `Hani`
 * @HB_SCRIPT_HEBREW: `Hebr`
 * @HB_SCRIPT_HIRAGANA: `Hira`
 * @HB_SCRIPT_KANNADA: `Knda`
 * @HB_SCRIPT_KATAKANA: `Kana`
 * @HB_SCRIPT_LAO: `Laoo`
 * @HB_SCRIPT_LATIN: `Latn`
 * @HB_SCRIPT_MALAYALAM: `Mlym`
 * @HB_SCRIPT_ORIYA: `Orya`
 * @HB_SCRIPT_TAMIL: `Taml`
 * @HB_SCRIPT_TELUGU: `Telu`
 * @HB_SCRIPT_THAI: `Thai`
 * @HB_SCRIPT_TIBETAN: `Tibt`
 * @HB_SCRIPT_BOPOMOFO: `Bopo`
 * @HB_SCRIPT_BRAILLE: `Brai`
 * @HB_SCRIPT_CANADIAN_SYLLABICS: `Cans`
 * @HB_SCRIPT_CHEROKEE: `Cher`
 * @HB_SCRIPT_ETHIOPIC: `Ethi`
 * @HB_SCRIPT_KHMER: `Khmr`
 * @HB_SCRIPT_MONGOLIAN: `Mong`
 * @HB_SCRIPT_MYANMAR: `Mymr`
 * @HB_SCRIPT_OGHAM: `Ogam`
 * @HB_SCRIPT_RUNIC: `Runr`
 * @HB_SCRIPT_SINHALA: `Sinh`
 * @HB_SCRIPT_SYRIAC: `Syrc`
 * @HB_SCRIPT_THAANA: `Thaa`
 * @HB_SCRIPT_YI: `Yiii`
 * @HB_SCRIPT_DESERET: `Dsrt`
 * @HB_SCRIPT_GOTHIC: `Goth`
 * @HB_SCRIPT_OLD_ITALIC: `Ital`
 * @HB_SCRIPT_BUHID: `Buhd`
 * @HB_SCRIPT_HANUNOO: `Hano`
 * @HB_SCRIPT_TAGALOG: `Tglg`
 * @HB_SCRIPT_TAGBANWA: `Tagb`
 * @HB_SCRIPT_CYPRIOT: `Cprt`
 * @HB_SCRIPT_LIMBU: `Limb`
 * @HB_SCRIPT_LINEAR_B: `Linb`
 * @HB_SCRIPT_OSMANYA: `Osma`
 * @HB_SCRIPT_SHAVIAN: `Shaw`
 * @HB_SCRIPT_TAI_LE: `Tale`
 * @HB_SCRIPT_UGARITIC: `Ugar`
 * @HB_SCRIPT_BUGINESE: `Bugi`
 * @HB_SCRIPT_COPTIC: `Copt`
 * @HB_SCRIPT_GLAGOLITIC: `Glag`
 * @HB_SCRIPT_KHAROSHTHI: `Khar`
 * @HB_SCRIPT_NEW_TAI_LUE: `Talu`
 * @HB_SCRIPT_OLD_PERSIAN: `Xpeo`
 * @HB_SCRIPT_SYLOTI_NAGRI: `Sylo`
 * @HB_SCRIPT_TIFINAGH: `Tfng`
 * @HB_SCRIPT_BALINESE: `Bali`
 * @HB_SCRIPT_CUNEIFORM: `Xsux`
 * @HB_SCRIPT_NKO: `Nkoo`
 * @HB_SCRIPT_PHAGS_PA: `Phag`
 * @HB_SCRIPT_PHOENICIAN: `Phnx`
 * @HB_SCRIPT_CARIAN: `Cari`
 * @HB_SCRIPT_CHAM: `Cham`
 * @HB_SCRIPT_KAYAH_LI: `Kali`
 * @HB_SCRIPT_LEPCHA: `Lepc`
 * @HB_SCRIPT_LYCIAN: `Lyci`
 * @HB_SCRIPT_LYDIAN: `Lydi`
 * @HB_SCRIPT_OL_CHIKI: `Olck`
 * @HB_SCRIPT_REJANG: `Rjng`
 * @HB_SCRIPT_SAURASHTRA: `Saur`
 * @HB_SCRIPT_SUNDANESE: `Sund`
 * @HB_SCRIPT_VAI: `Vaii`
 * @HB_SCRIPT_AVESTAN: `Avst`
 * @HB_SCRIPT_BAMUM: `Bamu`
 * @HB_SCRIPT_EGYPTIAN_HIEROGLYPHS: `Egyp`
 * @HB_SCRIPT_IMPERIAL_ARAMAIC: `Armi`
 * @HB_SCRIPT_INSCRIPTIONAL_PAHLAVI: `Phli`
 * @HB_SCRIPT_INSCRIPTIONAL_PARTHIAN: `Prti`
 * @HB_SCRIPT_JAVANESE: `Java`
 * @HB_SCRIPT_KAITHI: `Kthi`
 * @HB_SCRIPT_LISU: `Lisu`
 * @HB_SCRIPT_MEETEI_MAYEK: `Mtei`
 * @HB_SCRIPT_OLD_SOUTH_ARABIAN: `Sarb`
 * @HB_SCRIPT_OLD_TURKIC: `Orkh`
 * @HB_SCRIPT_SAMARITAN: `Samr`
 * @HB_SCRIPT_TAI_THAM: `Lana`
 * @HB_SCRIPT_TAI_VIET: `Tavt`
 * @HB_SCRIPT_BATAK: `Batk`
 * @HB_SCRIPT_BRAHMI: `Brah`
 * @HB_SCRIPT_MANDAIC: `Mand`
 * @HB_SCRIPT_CHAKMA: `Cakm`
 * @HB_SCRIPT_MEROITIC_CURSIVE: `Merc`
 * @HB_SCRIPT_MEROITIC_HIEROGLYPHS: `Mero`
 * @HB_SCRIPT_MIAO: `Plrd`
 * @HB_SCRIPT_SHARADA: `Shrd`
 * @HB_SCRIPT_SORA_SOMPENG: `Sora`
 * @HB_SCRIPT_TAKRI: `Takr`
 * @HB_SCRIPT_BASSA_VAH: `Bass`, Since: 0.9.30
 * @HB_SCRIPT_CAUCASIAN_ALBANIAN: `Aghb`, Since: 0.9.30
 * @HB_SCRIPT_DUPLOYAN: `Dupl`, Since: 0.9.30
 * @HB_SCRIPT_ELBASAN: `Elba`, Since: 0.9.30
 * @HB_SCRIPT_GRANTHA: `Gran`, Since: 0.9.30
 * @HB_SCRIPT_KHOJKI: `Khoj`, Since: 0.9.30
 * @HB_SCRIPT_KHUDAWADI: `Sind`, Since: 0.9.30
 * @HB_SCRIPT_LINEAR_A: `Lina`, Since: 0.9.30
 * @HB_SCRIPT_MAHAJANI: `Mahj`, Since: 0.9.30
 * @HB_SCRIPT_MANICHAEAN: `Mani`, Since: 0.9.30
 * @HB_SCRIPT_MENDE_KIKAKUI: `Mend`, Since: 0.9.30
 * @HB_SCRIPT_MODI: `Modi`, Since: 0.9.30
 * @HB_SCRIPT_MRO: `Mroo`, Since: 0.9.30
 * @HB_SCRIPT_NABATAEAN: `Nbat`, Since: 0.9.30
 * @HB_SCRIPT_OLD_NORTH_ARABIAN: `Narb`, Since: 0.9.30
 * @HB_SCRIPT_OLD_PERMIC: `Perm`, Since: 0.9.30
 * @HB_SCRIPT_PAHAWH_HMONG: `Hmng`, Since: 0.9.30
 * @HB_SCRIPT_PALMYRENE: `Palm`, Since: 0.9.30
 * @HB_SCRIPT_PAU_CIN_HAU: `Pauc`, Since: 0.9.30
 * @HB_SCRIPT_PSALTER_PAHLAVI: `Phlp`, Since: 0.9.30
 * @HB_SCRIPT_SIDDHAM: `Sidd`, Since: 0.9.30
 * @HB_SCRIPT_TIRHUTA: `Tirh`, Since: 0.9.30
 * @HB_SCRIPT_WARANG_CITI: `Wara`, Since: 0.9.30
 * @HB_SCRIPT_AHOM: `Ahom`, Since: 0.9.30
 * @HB_SCRIPT_ANATOLIAN_HIEROGLYPHS: `Hluw`, Since: 0.9.30
 * @HB_SCRIPT_HATRAN: `Hatr`, Since: 0.9.30
 * @HB_SCRIPT_MULTANI: `Mult`, Since: 0.9.30
 * @HB_SCRIPT_OLD_HUNGARIAN: `Hung`, Since: 0.9.30
 * @HB_SCRIPT_SIGNWRITING: `Sgnw`, Since: 0.9.30
 * @HB_SCRIPT_ADLAM: `Adlm`, Since: 1.3.0
 * @HB_SCRIPT_BHAIKSUKI: `Bhks`, Since: 1.3.0
 * @HB_SCRIPT_MARCHEN: `Marc`, Since: 1.3.0
 * @HB_SCRIPT_OSAGE: `Osge`, Since: 1.3.0
 * @HB_SCRIPT_TANGUT: `Tang`, Since: 1.3.0
 * @HB_SCRIPT_NEWA: `Newa`, Since: 1.3.0
 * @HB_SCRIPT_MASARAM_GONDI: `Gonm`, Since: 1.6.0
 * @HB_SCRIPT_NUSHU: `Nshu`, Since: 1.6.0
 * @HB_SCRIPT_SOYOMBO: `Soyo`, Since: 1.6.0
 * @HB_SCRIPT_ZANABAZAR_SQUARE: `Zanb`, Since: 1.6.0
 * @HB_SCRIPT_DOGRA: `Dogr`, Since: 1.8.0
 * @HB_SCRIPT_GUNJALA_GONDI: `Gong`, Since: 1.8.0
 * @HB_SCRIPT_HANIFI_ROHINGYA: `Rohg`, Since: 1.8.0
 * @HB_SCRIPT_MAKASAR: `Maka`, Since: 1.8.0
 * @HB_SCRIPT_MEDEFAIDRIN: `Medf`, Since: 1.8.0
 * @HB_SCRIPT_OLD_SOGDIAN: `Sogo`, Since: 1.8.0
 * @HB_SCRIPT_SOGDIAN: `Sogd`, Since: 1.8.0
 * @HB_SCRIPT_ELYMAIC: `Elym`, Since: 2.4.0
 * @HB_SCRIPT_NANDINAGARI: `Nand`, Since: 2.4.0
 * @HB_SCRIPT_NYIAKENG_PUACHUE_HMONG: `Hmnp`, Since: 2.4.0
 * @HB_SCRIPT_WANCHO: `Wcho`, Since: 2.4.0
 * @HB_SCRIPT_CHORASMIAN: `Chrs`, Since: 2.6.7
 * @HB_SCRIPT_DIVES_AKURU: `Diak`, Since: 2.6.7
 * @HB_SCRIPT_KHITAN_SMALL_SCRIPT: `Kits`, Since: 2.6.7
 * @HB_SCRIPT_YEZIDI: `Yezi`, Since: 2.6.7
 * @HB_SCRIPT_CYPRO_MINOAN: `Cpmn`, Since: 3.0.0
 * @HB_SCRIPT_OLD_UYGHUR: `Ougr`, Since: 3.0.0
 * @HB_SCRIPT_TANGSA: `Tnsa`, Since: 3.0.0
 * @HB_SCRIPT_TOTO: `Toto`, Since: 3.0.0
 * @HB_SCRIPT_VITHKUQI: `Vith`, Since: 3.0.0
 * @HB_SCRIPT_MATH: `Zmth`, Since: 3.4.0
 * @HB_SCRIPT_KAWI: `Kawi`, Since: 5.2.0
 * @HB_SCRIPT_NAG_MUNDARI: `Nagm`, Since: 5.2.0
 * @HB_SCRIPT_GARAY: `Gara`, Since: 10.0.0
 * @HB_SCRIPT_GURUNG_KHEMA: `Gukh`, Since: 10.0.0
 * @HB_SCRIPT_KIRAT_RAI: `Krai`, Since: 10.0.0
 * @HB_SCRIPT_OL_ONAL: `Onao`, Since: 10.0.0
 * @HB_SCRIPT_SUNUWAR: `Sunu`, Since: 10.0.0
 * @HB_SCRIPT_TODHRI: `Todr`, Since: 10.0.0
 * @HB_SCRIPT_TULU_TIGALARI: `Tutg`, Since: 10.0.0
 * @HB_SCRIPT_BERIA_ERFE: `Berf`, Since: 11.5.0
 * @HB_SCRIPT_SIDETIC: `Sidt`, Since: 11.5.0
 * @HB_SCRIPT_TAI_YO: `Tayo`, Since: 11.5.0
 * @HB_SCRIPT_TOLONG_SIKI: `Tols`, Since: 11.5.0
 * @HB_SCRIPT_JURCHEN: `Jurc`, Since: 14.5.0
 * @HB_SCRIPT_PROTO_CUNEIFORM: `Pcun`, Since: 14.5.0
 * @HB_SCRIPT_SEAL: `Seal`, Since: 14.5.0
 * @HB_SCRIPT_INVALID: No script set
 *
 * Data type for scripts. Each #hb_script_t's value is an #hb_tag_t corresponding
 * to the four-letter values defined by [ISO 15924](https://unicode.org/iso15924/).
 *
 * See also the Script (sc) property of the Unicode Character Database.
 *
 **/

/* https://docs.google.com/spreadsheets/d/1Y90M0Ie3MUJ6UVCRDOypOtijlMDLNNyyLk36T6iMu0o */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ
{
  HB_SCRIPT_COMMON			= HB_ਟੈਗ ('Z','y','y','y'), /*1.1*/
  HB_SCRIPT_INHERITED			= HB_ਟੈਗ ('Z','i','n','h'), /*1.1*/
  HB_SCRIPT_UNKNOWN			= HB_ਟੈਗ ('Z','z','z','z'), /*5.0*/

  HB_SCRIPT_ARABIC			= HB_ਟੈਗ ('A','r','a','b'), /*1.1*/
  HB_SCRIPT_ARMENIAN			= HB_ਟੈਗ ('A','r','m','n'), /*1.1*/
  HB_SCRIPT_BENGALI			= HB_ਟੈਗ ('B','e','n','g'), /*1.1*/
  HB_SCRIPT_CYRILLIC			= HB_ਟੈਗ ('C','y','r','l'), /*1.1*/
  HB_SCRIPT_DEVANAGARI			= HB_ਟੈਗ ('D','e','v','a'), /*1.1*/
  HB_SCRIPT_GEORGIAN			= HB_ਟੈਗ ('G','e','o','r'), /*1.1*/
  HB_SCRIPT_GREEK			= HB_ਟੈਗ ('G','r','e','k'), /*1.1*/
  HB_SCRIPT_GUJARATI			= HB_ਟੈਗ ('G','u','j','r'), /*1.1*/
  HB_SCRIPT_GURMUKHI			= HB_ਟੈਗ ('G','u','r','u'), /*1.1*/
  HB_SCRIPT_HANGUL			= HB_ਟੈਗ ('H','a','n','g'), /*1.1*/
  HB_SCRIPT_HAN				= HB_ਟੈਗ ('H','a','n','i'), /*1.1*/
  HB_SCRIPT_HEBREW			= HB_ਟੈਗ ('H','e','b','r'), /*1.1*/
  HB_SCRIPT_HIRAGANA			= HB_ਟੈਗ ('H','i','r','a'), /*1.1*/
  HB_SCRIPT_KANNADA			= HB_ਟੈਗ ('K','n','d','a'), /*1.1*/
  HB_SCRIPT_KATAKANA			= HB_ਟੈਗ ('K','a','n','a'), /*1.1*/
  HB_SCRIPT_LAO				= HB_ਟੈਗ ('L','a','o','o'), /*1.1*/
  HB_SCRIPT_LATIN			= HB_ਟੈਗ ('L','a','t','n'), /*1.1*/
  HB_SCRIPT_MALAYALAM			= HB_ਟੈਗ ('M','l','y','m'), /*1.1*/
  HB_SCRIPT_ORIYA			= HB_ਟੈਗ ('O','r','y','a'), /*1.1*/
  HB_SCRIPT_TAMIL			= HB_ਟੈਗ ('T','a','m','l'), /*1.1*/
  HB_SCRIPT_TELUGU			= HB_ਟੈਗ ('T','e','l','u'), /*1.1*/
  HB_SCRIPT_THAI			= HB_ਟੈਗ ('T','h','a','i'), /*1.1*/

  HB_SCRIPT_TIBETAN			= HB_ਟੈਗ ('T','i','b','t'), /*2.0*/

  HB_SCRIPT_BOPOMOFO			= HB_ਟੈਗ ('B','o','p','o'), /*3.0*/
  HB_SCRIPT_BRAILLE			= HB_ਟੈਗ ('B','r','a','i'), /*3.0*/
  HB_SCRIPT_CANADIAN_SYLLABICS		= HB_ਟੈਗ ('C','a','n','s'), /*3.0*/
  HB_SCRIPT_CHEROKEE			= HB_ਟੈਗ ('C','h','e','r'), /*3.0*/
  HB_SCRIPT_ETHIOPIC			= HB_ਟੈਗ ('E','t','h','i'), /*3.0*/
  HB_SCRIPT_KHMER			= HB_ਟੈਗ ('K','h','m','r'), /*3.0*/
  HB_SCRIPT_MONGOLIAN			= HB_ਟੈਗ ('M','o','n','g'), /*3.0*/
  HB_SCRIPT_MYANMAR			= HB_ਟੈਗ ('M','y','m','r'), /*3.0*/
  HB_SCRIPT_OGHAM			= HB_ਟੈਗ ('O','g','a','m'), /*3.0*/
  HB_SCRIPT_RUNIC			= HB_ਟੈਗ ('R','u','n','r'), /*3.0*/
  HB_SCRIPT_SINHALA			= HB_ਟੈਗ ('S','i','n','h'), /*3.0*/
  HB_SCRIPT_SYRIAC			= HB_ਟੈਗ ('S','y','r','c'), /*3.0*/
  HB_SCRIPT_THAANA			= HB_ਟੈਗ ('T','h','a','a'), /*3.0*/
  HB_SCRIPT_YI				= HB_ਟੈਗ ('Y','i','i','i'), /*3.0*/

  HB_SCRIPT_DESERET			= HB_ਟੈਗ ('D','s','r','t'), /*3.1*/
  HB_SCRIPT_GOTHIC			= HB_ਟੈਗ ('G','o','t','h'), /*3.1*/
  HB_SCRIPT_OLD_ITALIC			= HB_ਟੈਗ ('I','t','a','l'), /*3.1*/

  HB_SCRIPT_BUHID			= HB_ਟੈਗ ('B','u','h','d'), /*3.2*/
  HB_SCRIPT_HANUNOO			= HB_ਟੈਗ ('H','a','n','o'), /*3.2*/
  HB_SCRIPT_TAGALOG			= HB_ਟੈਗ ('T','g','l','g'), /*3.2*/
  HB_SCRIPT_TAGBANWA			= HB_ਟੈਗ ('T','a','g','b'), /*3.2*/

  HB_SCRIPT_CYPRIOT			= HB_ਟੈਗ ('C','p','r','t'), /*4.0*/
  HB_SCRIPT_LIMBU			= HB_ਟੈਗ ('L','i','m','b'), /*4.0*/
  HB_SCRIPT_ਰੇਖਿਕ_B			= HB_ਟੈਗ ('L','i','n','b'), /*4.0*/
  HB_SCRIPT_OSMANYA			= HB_ਟੈਗ ('O','s','m','a'), /*4.0*/
  HB_SCRIPT_SHAVIAN			= HB_ਟੈਗ ('S','h','a','w'), /*4.0*/
  HB_SCRIPT_TAI_LE			= HB_ਟੈਗ ('T','a','l','e'), /*4.0*/
  HB_SCRIPT_UGARITIC			= HB_ਟੈਗ ('U','g','a','r'), /*4.0*/

  HB_SCRIPT_BUGINESE			= HB_ਟੈਗ ('B','u','g','i'), /*4.1*/
  HB_SCRIPT_COPTIC			= HB_ਟੈਗ ('C','o','p','t'), /*4.1*/
  HB_SCRIPT_GLAGOLITIC			= HB_ਟੈਗ ('G','l','a','g'), /*4.1*/
  HB_SCRIPT_KHAROSHTHI			= HB_ਟੈਗ ('K','h','a','r'), /*4.1*/
  HB_SCRIPT_NEW_TAI_LUE			= HB_ਟੈਗ ('T','a','l','u'), /*4.1*/
  HB_SCRIPT_OLD_PERSIAN			= HB_ਟੈਗ ('X','p','e','o'), /*4.1*/
  HB_SCRIPT_SYLOTI_NAGRI		= HB_ਟੈਗ ('S','y','l','o'), /*4.1*/
  HB_SCRIPT_TIFINAGH			= HB_ਟੈਗ ('T','f','n','g'), /*4.1*/

  HB_SCRIPT_BALINESE			= HB_ਟੈਗ ('B','a','l','i'), /*5.0*/
  HB_SCRIPT_CUNEIFORM			= HB_ਟੈਗ ('X','s','u','x'), /*5.0*/
  HB_SCRIPT_NKO				= HB_ਟੈਗ ('N','k','o','o'), /*5.0*/
  HB_SCRIPT_PHAGS_PA			= HB_ਟੈਗ ('P','h','a','g'), /*5.0*/
  HB_SCRIPT_PHOENICIAN			= HB_ਟੈਗ ('P','h','n','x'), /*5.0*/

  HB_SCRIPT_CARIAN			= HB_ਟੈਗ ('C','a','r','i'), /*5.1*/
  HB_SCRIPT_CHAM			= HB_ਟੈਗ ('C','h','a','m'), /*5.1*/
  HB_SCRIPT_KAYAH_LI			= HB_ਟੈਗ ('K','a','l','i'), /*5.1*/
  HB_SCRIPT_LEPCHA			= HB_ਟੈਗ ('L','e','p','c'), /*5.1*/
  HB_SCRIPT_LYCIAN			= HB_ਟੈਗ ('L','y','c','i'), /*5.1*/
  HB_SCRIPT_LYDIAN			= HB_ਟੈਗ ('L','y','d','i'), /*5.1*/
  HB_SCRIPT_OL_CHIKI			= HB_ਟੈਗ ('O','l','c','k'), /*5.1*/
  HB_SCRIPT_REJANG			= HB_ਟੈਗ ('R','j','n','g'), /*5.1*/
  HB_SCRIPT_SAURASHTRA			= HB_ਟੈਗ ('S','a','u','r'), /*5.1*/
  HB_SCRIPT_SUNDANESE			= HB_ਟੈਗ ('S','u','n','d'), /*5.1*/
  HB_SCRIPT_VAI				= HB_ਟੈਗ ('V','a','i','i'), /*5.1*/

  HB_SCRIPT_AVESTAN			= HB_ਟੈਗ ('A','v','s','t'), /*5.2*/
  HB_SCRIPT_BAMUM			= HB_ਟੈਗ ('B','a','m','u'), /*5.2*/
  HB_SCRIPT_EGYPTIAN_HIEROGLYPHS	= HB_ਟੈਗ ('E','g','y','p'), /*5.2*/
  HB_SCRIPT_IMPERIAL_ARAMAIC		= HB_ਟੈਗ ('A','r','m','i'), /*5.2*/
  HB_SCRIPT_INSCRIPTIONAL_PAHLAVI	= HB_ਟੈਗ ('P','h','l','i'), /*5.2*/
  HB_SCRIPT_INSCRIPTIONAL_PARTHIAN	= HB_ਟੈਗ ('P','r','t','i'), /*5.2*/
  HB_SCRIPT_JAVANESE			= HB_ਟੈਗ ('J','a','v','a'), /*5.2*/
  HB_SCRIPT_KAITHI			= HB_ਟੈਗ ('K','t','h','i'), /*5.2*/
  HB_SCRIPT_LISU			= HB_ਟੈਗ ('L','i','s','u'), /*5.2*/
  HB_SCRIPT_MEETEI_MAYEK		= HB_ਟੈਗ ('M','t','e','i'), /*5.2*/
  HB_SCRIPT_OLD_SOUTH_ARABIAN		= HB_ਟੈਗ ('S','a','r','b'), /*5.2*/
  HB_SCRIPT_OLD_TURKIC			= HB_ਟੈਗ ('O','r','k','h'), /*5.2*/
  HB_SCRIPT_SAMARITAN			= HB_ਟੈਗ ('S','a','m','r'), /*5.2*/
  HB_SCRIPT_TAI_THAM			= HB_ਟੈਗ ('L','a','n','a'), /*5.2*/
  HB_SCRIPT_TAI_VIET			= HB_ਟੈਗ ('T','a','v','t'), /*5.2*/

  HB_SCRIPT_BATAK			= HB_ਟੈਗ ('B','a','t','k'), /*6.0*/
  HB_SCRIPT_BRAHMI			= HB_ਟੈਗ ('B','r','a','h'), /*6.0*/
  HB_SCRIPT_MANDAIC			= HB_ਟੈਗ ('M','a','n','d'), /*6.0*/

  HB_SCRIPT_CHAKMA			= HB_ਟੈਗ ('C','a','k','m'), /*6.1*/
  HB_SCRIPT_MEROITIC_CURSIVE		= HB_ਟੈਗ ('M','e','r','c'), /*6.1*/
  HB_SCRIPT_MEROITIC_HIEROGLYPHS	= HB_ਟੈਗ ('M','e','r','o'), /*6.1*/
  HB_SCRIPT_MIAO			= HB_ਟੈਗ ('P','l','r','d'), /*6.1*/
  HB_SCRIPT_SHARADA			= HB_ਟੈਗ ('S','h','r','d'), /*6.1*/
  HB_SCRIPT_SORA_SOMPENG		= HB_ਟੈਗ ('S','o','r','a'), /*6.1*/
  HB_SCRIPT_TAKRI			= HB_ਟੈਗ ('T','a','k','r'), /*6.1*/

  /*
   * Since: 0.9.30
   */
  HB_SCRIPT_BASSA_VAH			= HB_ਟੈਗ ('B','a','s','s'), /*7.0*/
  HB_SCRIPT_CAUCASIAN_ALBANIAN		= HB_ਟੈਗ ('A','g','h','b'), /*7.0*/
  HB_SCRIPT_DUPLOYAN			= HB_ਟੈਗ ('D','u','p','l'), /*7.0*/
  HB_SCRIPT_ELBASAN			= HB_ਟੈਗ ('E','l','b','a'), /*7.0*/
  HB_SCRIPT_GRANTHA			= HB_ਟੈਗ ('G','r','a','n'), /*7.0*/
  HB_SCRIPT_KHOJKI			= HB_ਟੈਗ ('K','h','o','j'), /*7.0*/
  HB_SCRIPT_KHUDAWADI			= HB_ਟੈਗ ('S','i','n','d'), /*7.0*/
  HB_SCRIPT_ਰੇਖਿਕ_A			= HB_ਟੈਗ ('L','i','n','a'), /*7.0*/
  HB_SCRIPT_MAHAJANI			= HB_ਟੈਗ ('M','a','h','j'), /*7.0*/
  HB_SCRIPT_MANICHAEAN			= HB_ਟੈਗ ('M','a','n','i'), /*7.0*/
  HB_SCRIPT_MENDE_KIKAKUI		= HB_ਟੈਗ ('M','e','n','d'), /*7.0*/
  HB_SCRIPT_MODI			= HB_ਟੈਗ ('M','o','d','i'), /*7.0*/
  HB_SCRIPT_MRO				= HB_ਟੈਗ ('M','r','o','o'), /*7.0*/
  HB_SCRIPT_NABATAEAN			= HB_ਟੈਗ ('N','b','a','t'), /*7.0*/
  HB_SCRIPT_OLD_NORTH_ARABIAN		= HB_ਟੈਗ ('N','a','r','b'), /*7.0*/
  HB_SCRIPT_OLD_PERMIC			= HB_ਟੈਗ ('P','e','r','m'), /*7.0*/
  HB_SCRIPT_PAHAWH_HMONG		= HB_ਟੈਗ ('H','m','n','g'), /*7.0*/
  HB_SCRIPT_PALMYRENE			= HB_ਟੈਗ ('P','a','l','m'), /*7.0*/
  HB_SCRIPT_PAU_CIN_HAU			= HB_ਟੈਗ ('P','a','u','c'), /*7.0*/
  HB_SCRIPT_PSALTER_PAHLAVI		= HB_ਟੈਗ ('P','h','l','p'), /*7.0*/
  HB_SCRIPT_SIDDHAM			= HB_ਟੈਗ ('S','i','d','d'), /*7.0*/
  HB_SCRIPT_TIRHUTA			= HB_ਟੈਗ ('T','i','r','h'), /*7.0*/
  HB_SCRIPT_WARANG_CITI			= HB_ਟੈਗ ('W','a','r','a'), /*7.0*/

  HB_SCRIPT_AHOM			= HB_ਟੈਗ ('A','h','o','m'), /*8.0*/
  HB_SCRIPT_ANATOLIAN_HIEROGLYPHS	= HB_ਟੈਗ ('H','l','u','w'), /*8.0*/
  HB_SCRIPT_HATRAN			= HB_ਟੈਗ ('H','a','t','r'), /*8.0*/
  HB_SCRIPT_MULTANI			= HB_ਟੈਗ ('M','u','l','t'), /*8.0*/
  HB_SCRIPT_OLD_HUNGARIAN		= HB_ਟੈਗ ('H','u','n','g'), /*8.0*/
  HB_SCRIPT_SIGNWRITING			= HB_ਟੈਗ ('S','g','n','w'), /*8.0*/

  /*
   * Since 1.3.0
   */
  HB_SCRIPT_ADLAM			= HB_ਟੈਗ ('A','d','l','m'), /*9.0*/
  HB_SCRIPT_BHAIKSUKI			= HB_ਟੈਗ ('B','h','k','s'), /*9.0*/
  HB_SCRIPT_MARCHEN			= HB_ਟੈਗ ('M','a','r','c'), /*9.0*/
  HB_SCRIPT_OSAGE			= HB_ਟੈਗ ('O','s','g','e'), /*9.0*/
  HB_SCRIPT_TANGUT			= HB_ਟੈਗ ('T','a','n','g'), /*9.0*/
  HB_SCRIPT_NEWA			= HB_ਟੈਗ ('N','e','w','a'), /*9.0*/

  /*
   * Since 1.6.0
   */
  HB_SCRIPT_MASARAM_GONDI		= HB_ਟੈਗ ('G','o','n','m'), /*10.0*/
  HB_SCRIPT_NUSHU			= HB_ਟੈਗ ('N','s','h','u'), /*10.0*/
  HB_SCRIPT_SOYOMBO			= HB_ਟੈਗ ('S','o','y','o'), /*10.0*/
  HB_SCRIPT_ZANABAZAR_ਵਰਗਫਲ		= HB_ਟੈਗ ('Z','a','n','b'), /*10.0*/

  /*
   * Since 1.8.0
   */
  HB_SCRIPT_DOGRA			= HB_ਟੈਗ ('D','o','g','r'), /*11.0*/
  HB_SCRIPT_GUNJALA_GONDI		= HB_ਟੈਗ ('G','o','n','g'), /*11.0*/
  HB_SCRIPT_HANIFI_ROHINGYA		= HB_ਟੈਗ ('R','o','h','g'), /*11.0*/
  HB_SCRIPT_MAKASAR			= HB_ਟੈਗ ('M','a','k','a'), /*11.0*/
  HB_SCRIPT_MEDEFAIDRIN			= HB_ਟੈਗ ('M','e','d','f'), /*11.0*/
  HB_SCRIPT_OLD_SOGDIAN			= HB_ਟੈਗ ('S','o','g','o'), /*11.0*/
  HB_SCRIPT_SOGDIAN			= HB_ਟੈਗ ('S','o','g','d'), /*11.0*/

  /*
   * Since 2.4.0
   */
  HB_SCRIPT_ELYMAIC			= HB_ਟੈਗ ('E','l','y','m'), /*12.0*/
  HB_SCRIPT_NANDINAGARI			= HB_ਟੈਗ ('N','a','n','d'), /*12.0*/
  HB_SCRIPT_NYIAKENG_PUACHUE_HMONG	= HB_ਟੈਗ ('H','m','n','p'), /*12.0*/
  HB_SCRIPT_WANCHO			= HB_ਟੈਗ ('W','c','h','o'), /*12.0*/

  /*
   * Since 2.6.7
   */
  HB_SCRIPT_CHORASMIAN			= HB_ਟੈਗ ('C','h','r','s'), /*13.0*/
  HB_SCRIPT_DIVES_AKURU			= HB_ਟੈਗ ('D','i','a','k'), /*13.0*/
  HB_SCRIPT_KHITAN_SMALL_SCRIPT		= HB_ਟੈਗ ('K','i','t','s'), /*13.0*/
  HB_SCRIPT_YEZIDI			= HB_ਟੈਗ ('Y','e','z','i'), /*13.0*/

  /*
   * Since 3.0.0
   */
  HB_SCRIPT_CYPRO_MINOAN		= HB_ਟੈਗ ('C','p','m','n'), /*14.0*/
  HB_SCRIPT_OLD_UYGHUR			= HB_ਟੈਗ ('O','u','g','r'), /*14.0*/
  HB_SCRIPT_TANGSA			= HB_ਟੈਗ ('T','n','s','a'), /*14.0*/
  HB_SCRIPT_TOTO			= HB_ਟੈਗ ('T','o','t','o'), /*14.0*/
  HB_SCRIPT_VITHKUQI			= HB_ਟੈਗ ('V','i','t','h'), /*14.0*/

  /*
   * Since 3.4.0
   */
  HB_SCRIPT_MATH			= HB_ਟੈਗ ('Z','m','t','h'),

  /*
   * Since 5.2.0
   */
  HB_SCRIPT_KAWI			= HB_ਟੈਗ ('K','a','w','i'), /*15.0*/
  HB_SCRIPT_NAG_MUNDARI			= HB_ਟੈਗ ('N','a','g','m'), /*15.0*/

  /*
   * Since 10.0.0
   */
  HB_SCRIPT_GARAY			= HB_ਟੈਗ ('G','a','r','a'), /*16.0*/
  HB_SCRIPT_GURUNG_KHEMA		= HB_ਟੈਗ ('G','u','k','h'), /*16.0*/
  HB_SCRIPT_KIRAT_RAI			= HB_ਟੈਗ ('K','r','a','i'), /*16.0*/
  HB_SCRIPT_OL_ONAL			= HB_ਟੈਗ ('O','n','a','o'), /*16.0*/
  HB_SCRIPT_SUNUWAR			= HB_ਟੈਗ ('S','u','n','u'), /*16.0*/
  HB_SCRIPT_TODHRI			= HB_ਟੈਗ ('T','o','d','r'), /*16.0*/
  HB_SCRIPT_TULU_TIGALARI		= HB_ਟੈਗ ('T','u','t','g'), /*16.0*/

  /*
   * Since 11.5.0
   */
  HB_SCRIPT_BERIA_ERFE			= HB_ਟੈਗ ('B','e','r','f'), /*17.0*/
  HB_SCRIPT_SIDETIC			= HB_ਟੈਗ ('S','i','d','t'), /*17.0*/
  HB_SCRIPT_TAI_YO			= HB_ਟੈਗ ('T','a','y','o'), /*17.0*/
  HB_SCRIPT_TOLONG_SIKI			= HB_ਟੈਗ ('T','o','l','s'), /*17.0*/

  /*
   * Since: 14.5.0
   */
  HB_SCRIPT_JURCHEN			= HB_ਟੈਗ ('J','u','r','c'), /*18.0*/
  HB_SCRIPT_PROTO_CUNEIFORM		= HB_ਟੈਗ ('P','c','u','n'), /*18.0*/
  HB_SCRIPT_SEAL				= HB_ਟੈਗ ('S','e','a','l'), /*18.0*/

  /* No script set. */
  HB_SCRIPT_INVALID			= HB_ਟੈਗ_NONE,

  /*< private >*/

  /* Dummy values to ensure any hb_tag_t value can be passed/stored as hb_script_t
   * without risking undefined behavior.  We have two, for historical reasons.
   * HB_TAG_MAX used to be unsigned, but that was invalid Ansi C, so was changed
   * to _HB_SCRIPT_MAX_VALUE to be equal to HB_TAG_MAX_SIGNED as well.
   *
   * See this thread for technicalities:
   *
   *   https://lists.freedesktop.org/archives/harfbuzz/2014-March/004150.html
   */
  HB_SCRIPT_MAX_ਮੁੱਲ				= HB_ਟੈਗ_MAX_SIGNED, /*< skip >*/
  HB_SCRIPT_MAX_ਮੁੱਲ_SIGNED			= HB_ਟੈਗ_MAX_SIGNED /*< skip >*/

} hb_script_t;

#ਜੇ_ਅੰਤ /* HB_SCRIPT_LIST_H */
