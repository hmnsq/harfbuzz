/*
 * Copyright © 2013  Google, Inc.
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

#جے_بنا_تعریف HB_OT_LAYOUT_JSTF_جدول_HH
#تعریف_کرو HB_OT_LAYOUT_JSTF_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-gpos-table.hh"


نطاق OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

تعريفنوع اشاریہ_Array JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

تعريفنوع List16_دا_Offset16_منزل<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

بنية Jstf_ترجیح
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  shrinkageEnableGSUB.sanitize (c, هذا) &&
		  shrinkageDisableGSUB.sanitize (c, هذا) &&
		  shrinkageEnableGPOS.sanitize (c, هذا) &&
		  shrinkageDisableGPOS.sanitize (c, هذا) &&
		  shrinkageJstfMax.sanitize (c, هذا) &&
		  extensionEnableGSUB.sanitize (c, هذا) &&
		  extensionDisableGSUB.sanitize (c, هذا) &&
		  extensionEnableGPOS.sanitize (c, هذا) &&
		  extensionDisableGPOS.sanitize (c, هذا) &&
		  extensionJstfMax.sanitize (c, هذا));
  }

  محمي:
  Offset16_منزل<JstfModList>
		shrinkageEnableGSUB;	/* Offset to Shrinkage Enable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_منزل<JstfModList>
		shrinkageDisableGSUB;	/* Offset to Shrinkage Disable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_منزل<JstfModList>
		shrinkageEnableGPOS;	/* Offset to Shrinkage Enable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_منزل<JstfModList>
		shrinkageDisableGPOS;	/* Offset to Shrinkage Disable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_منزل<JstfMax>
		shrinkageJstfMax;	/* Offset to Shrinkage JstfMax table--
					 * from beginning of JstfPriority table
					 * --may be NULL */
  Offset16_منزل<JstfModList>
		extensionEnableGSUB;	/* Offset to Extension Enable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_منزل<JstfModList>
		extensionDisableGSUB;	/* Offset to Extension Disable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_منزل<JstfModList>
		extensionEnableGPOS;	/* Offset to Extension Enable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_منزل<JstfModList>
		extensionDisableGPOS;	/* Offset to Extension Disable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_منزل<JstfMax>
		extensionJstfMax;	/* Offset to Extension JstfMax table--
					 * from beginning of JstfPriority table
					 * --may be NULL */

  عام:
  DEFINE_ناپ_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

بنية JstfLangSys : List16_دا_Offset16_منزل<Jstf_ترجیح>
{
  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارڈ_sanitize_گھیرا_t * = خالی_سوچک) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (List16_دا_Offset16_منزل<Jstf_ترجیح>::sanitize (c));
  }
};


/*
 * ExtenderGlyphs -- Extender Glyph Table
 */

سانچا <قسم_ناں Types>
ورتوں ExtenderGlyphs = Sorted_Array16_دا<قسم_ناں Types::HBGlyphID>;


/*
 * JstfScript -- The Justification Table
 */

سانچا <قسم_ناں Types>
بنية JstfScript
{
  بنانشانی صحيح get_lang_sys_گنتی () ثابت
  { أرجع langSys.len; }
  ثابت ٹیگ& get_lang_sys_ٹیگ (بنانشانی صحيح i) ثابت
  { أرجع langSys.get_ٹیگ (i); }
  بنانشانی صحيح get_lang_sys_ٹیگ_سوچی (بنانشانی صحيح شروع_وتھ,
				  بنانشانی صحيح *lang_sys_گنتی /* IN/OUT */,
				  hb_ٹیگ_t     *lang_sys_ٹیگ_سوچی /*باہرلا*/) ثابت
  { أرجع langSys.get_ٹیگ_سوچی (شروع_وتھ, lang_sys_گنتی, lang_sys_ٹیگ_سوچی); }
  ثابت JstfLangSys& get_lang_sys (بنانشانی صحيح i) ثابت
  {
    إذا (i == اشاریہ::NOT_لبھیا_اشاریہ) أرجع get_default_lang_sys ();
    أرجع هذا+langSys[i].وتھ;
  }
  منطقي لبھو_lang_sys_اشاریہ (hb_ٹیگ_t ٹیگ_2, بنانشانی صحيح *اشاریہ_2) ثابت
  { أرجع langSys.لبھو_اشاریہ (ٹیگ_2, اشاریہ_2); }

  منطقي has_default_lang_sys () ثابت               { أرجع defaultLangSys != 0; }
  ثابت JstfLangSys& get_default_lang_sys () ثابت { أرجع هذا+defaultLangSys; }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارڈ_sanitize_گھیرا_t * = خالی_سوچک) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (extenderGlyphs.sanitize (c, هذا) &&
		  defaultLangSys.sanitize (c, هذا) &&
		  langSys.sanitize (c, هذا));
  }

  محمي:
  قسم_ناں Types::سانچا LOffset_منزل<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  قسم_ناں Types::سانچا LOffset_منزل<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  ریکارڈ_Array_دا<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  عام:
  DEFINE_ناپ_ARRAY (2 * Types::LOffset::static_ناپ + 2, langSys);
};

سانچا <قسم_ناں Types>
بنية Jstf_Script_ریکارڈ
{
  صحيح cmp (hb_ٹیگ_t a) ثابت { أرجع ٹیگ_2.cmp (a); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت ریکارڈ_sanitize_گھیرا_t گھیرا = {ٹیگ_2, ادھار};
    return_trace (c->check_struct (هذا) &&
		  وتھ.sanitize (c, ادھار, &گھیرا));
  }

  ٹیگ		ٹیگ_2;		/* 4-byte JstfScript identification tag */
  قسم_ناں Types::سانچا LOffset_منزل<JstfScript<Types>>
		وتھ;		/* Offset from beginning of JSTF header */
  عام:
  DEFINE_ناپ_STATIC (4 + Types::LOffset::static_ناپ);
};

سانچا <قسم_ناں Types>
بنية JstfScriptList : Sorted_Array16_دا<Jstf_Script_ریکارڈ<Types>>
{
  ثابت ٹیگ& get_ٹیگ (بنانشانی صحيح i) ثابت
  { أرجع (*هذا)[i].ٹیگ_2; }
  بنانشانی صحيح get_ٹیگ_سوچی (بنانشانی صحيح شروع_وتھ,
			 بنانشانی صحيح *script_گنتی /* IN/OUT */,
			 hb_ٹیگ_t     *script_ٹیگ_سوچی /*باہرلا*/) ثابت
  {
    إذا (script_گنتی)
    {
      + هذا->as_array ().sub_array (شروع_وتھ, script_گنتی)
      | hb_map (&Jstf_Script_ریکارڈ<Types>::ٹیگ_2)
      | hb_sink (hb_array (script_ٹیگ_سوچی, *script_گنتی))
      ;
    }
    أرجع هذا->len;
  }
  ثابت JstfScript<Types>& get_script (ثابت فراغ *ادھار, بنانشانی صحيح i) ثابت
  { أرجع ادھار+(*هذا)[i].وتھ; }
  منطقي لبھو_اشاریہ (hb_ٹیگ_t ٹیگ_2, بنانشانی صحيح *اشاریہ_2) ثابت
  {
    أرجع هذا->bfind (ٹیگ_2, اشاریہ_2, HB_NOT_لبھیا_STORE, اشاریہ::NOT_لبھیا_اشاریہ);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

بنية JSTF
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_JSTF;

  بنانشانی صحيح get_script_گنتی () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().len;
#جے_انت
    أرجع scriptList.len;
  }
  ثابت ٹیگ& get_script_ٹیگ (بنانشانی صحيح i) ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().get_ٹیگ (i);
#جے_انت
    أرجع scriptList.get_ٹیگ (i);
  }
  بنانشانی صحيح get_script_ٹیگ_سوچی (بنانشانی صحيح شروع_وتھ,
				بنانشانی صحيح *script_گنتی /* IN/OUT */,
				hb_ٹیگ_t     *script_ٹیگ_سوچی /*باہرلا*/) ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (has_script_list2 ())
      أرجع get_script_list2 ().get_ٹیگ_سوچی (شروع_وتھ, script_گنتی, script_ٹیگ_سوچی);
#جے_انت
    أرجع scriptList.get_ٹیگ_سوچی (شروع_وتھ, script_گنتی, script_ٹیگ_سوچی);
  }
  سانچا <قسم_ناں Types>
  ثابت JstfScript<Types>& get_script (بنانشانی صحيح i) ثابت;
  منطقي لبھو_script_اشاریہ (hb_ٹیگ_t ٹیگ_2, بنانشانی صحيح *اشاریہ_2) ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().لبھو_اشاریہ (ٹیگ_2, اشاریہ_2);
#جے_انت
    أرجع scriptList.لبھو_اشاریہ (ٹیگ_2, اشاریہ_2);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!نسخہ.sanitize (c) ||
		  !hb_barrier () ||
		  نسخہ.major != 1 ||
		  !scriptList.sanitize (c, هذا)))
      return_trace (خطأ);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (unlikely (نسخہ.minor >= 1 && !get_script_list2 ().sanitize (c, هذا)))
      return_trace (خطأ);
#جے_انت
    return_trace (سچ);
  }

  خاص:
#جے_بنا_تعریف HB_NO_BEYOND_64K
  منطقي has_script_list2 () ثابت
  { أرجع نسخہ.minor >= 1 && get_script_list2 ().len; }
  ثابت JstfScriptList<Layout::MediumTypes>& get_script_list2 () ثابت
  { أرجع StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#جے_انت

  محمي:
  Fixed_نسخہ<>نسخہ;	/* Version of the JSTF table--initially set
				 * to 0x00010000u */
  JstfScriptList<Layout::SmallTypes>
		scriptList;	/* Array of JstfScripts--listed
				 * alphabetically by ScriptTag */
#جے_بنا_تعریف HB_NO_BEYOND_64K
/*JstfScriptList<Layout::MediumTypes>
		scriptList2;*//* Array of JstfScript2s--listed
				 * alphabetically by ScriptTag.
				 * Present only if minorVersion is 1 or greater. */
#جے_انت
  عام:
  DEFINE_ناپ_ARRAY (6, scriptList);
};

سانچا <>
مضمن ثابت JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (بنانشانی صحيح i) ثابت
{ أرجع scriptList.get_script (هذا, i); }

#جے_بنا_تعریف HB_NO_BEYOND_64K
سانچا <>
مضمن ثابت JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (بنانشانی صحيح i) ثابت
{ أرجع get_script_list2 ().get_script (هذا, i); }
#جے_انت


} /* namespace OT */


#جے_انت /* HB_OT_LAYOUT_JSTF_TABLE_HH */
