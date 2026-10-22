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

#که_نه_تعریف_شوی HB_OT_LAYOUT_JSTF_جدول_HH
#تعریف_کړه HB_OT_LAYOUT_JSTF_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-gpos-table.hh"


نطاق OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

تعريفنوع شاخص_Array JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

تعريفنوع List16_د_Offset16_منزل<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

بنية Jstf_لومړیتوب
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
  DEFINE_اندازه_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

بنية JstfLangSys : List16_د_Offset16_منزل<Jstf_لومړیتوب>
{
  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارډ_sanitize_تړون_t * = تش_اشاره) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (List16_د_Offset16_منزل<Jstf_لومړیتوب>::sanitize (c));
  }
};


/*
 * ExtenderGlyphs -- Extender Glyph Table
 */

کينډۍ <ډول_نوم Types>
کارول ExtenderGlyphs = Sorted_Array16_د<ډول_نوم Types::HBGlyphID>;


/*
 * JstfScript -- The Justification Table
 */

کينډۍ <ډول_نوم Types>
بنية JstfScript
{
  بینښه صحيح get_lang_sys_شمېر () ثابت
  { أرجع langSys.len; }
  ثابت نښان& get_lang_sys_نښان (بینښه صحيح i) ثابت
  { أرجع langSys.get_نښان (i); }
  بینښه صحيح get_lang_sys_نښانونه (بینښه صحيح پیل_دننه_واټن,
				  بینښه صحيح *lang_sys_شمېر /* IN/OUT */,
				  hb_نښان_t     *lang_sys_نښانونه /*بهر*/) ثابت
  { أرجع langSys.get_نښانونه (پیل_دننه_واټن, lang_sys_شمېر, lang_sys_نښانونه); }
  ثابت JstfLangSys& get_lang_sys (بینښه صحيح i) ثابت
  {
    إذا (i == شاخص::NOT_وموندل_شاخص) أرجع get_default_lang_sys ();
    أرجع هذا+langSys[i].دننه_واټن;
  }
  منطقي پیدا_کړه_lang_sys_شاخص (hb_نښان_t نښان_2, بینښه صحيح *شاخص_2) ثابت
  { أرجع langSys.پیدا_کړه_شاخص (نښان_2, شاخص_2); }

  منطقي has_default_lang_sys () ثابت               { أرجع defaultLangSys != 0; }
  ثابت JstfLangSys& get_default_lang_sys () ثابت { أرجع هذا+defaultLangSys; }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارډ_sanitize_تړون_t * = تش_اشاره) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (extenderGlyphs.sanitize (c, هذا) &&
		  defaultLangSys.sanitize (c, هذا) &&
		  langSys.sanitize (c, هذا));
  }

  محمي:
  ډول_نوم Types::کينډۍ LOffset_منزل<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  ډول_نوم Types::کينډۍ LOffset_منزل<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  ریکارډ_Array_د<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  عام:
  DEFINE_اندازه_ARRAY (2 * Types::LOffset::static_اندازه + 2, langSys);
};

کينډۍ <ډول_نوم Types>
بنية Jstf_Script_ریکارډ
{
  صحيح cmp (hb_نښان_t a) ثابت { أرجع نښان_2.cmp (a); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت ریکارډ_sanitize_تړون_t تړون = {نښان_2, بنسټ};
    return_trace (c->check_struct (هذا) &&
		  دننه_واټن.sanitize (c, بنسټ, &تړون));
  }

  نښان		نښان_2;		/* 4-byte JstfScript identification tag */
  ډول_نوم Types::کينډۍ LOffset_منزل<JstfScript<Types>>
		دننه_واټن;		/* Offset from beginning of JSTF header */
  عام:
  DEFINE_اندازه_STATIC (4 + Types::LOffset::static_اندازه);
};

کينډۍ <ډول_نوم Types>
بنية JstfScriptList : Sorted_Array16_د<Jstf_Script_ریکارډ<Types>>
{
  ثابت نښان& get_نښان (بینښه صحيح i) ثابت
  { أرجع (*هذا)[i].نښان_2; }
  بینښه صحيح get_نښانونه (بینښه صحيح پیل_دننه_واټن,
			 بینښه صحيح *script_شمېر /* IN/OUT */,
			 hb_نښان_t     *script_نښانونه /*بهر*/) ثابت
  {
    إذا (script_شمېر)
    {
      + هذا->as_array ().sub_array (پیل_دننه_واټن, script_شمېر)
      | hb_map (&Jstf_Script_ریکارډ<Types>::نښان_2)
      | hb_sink (hb_array (script_نښانونه, *script_شمېر))
      ;
    }
    أرجع هذا->len;
  }
  ثابت JstfScript<Types>& get_script (ثابت فراغ *بنسټ, بینښه صحيح i) ثابت
  { أرجع بنسټ+(*هذا)[i].دننه_واټن; }
  منطقي پیدا_کړه_شاخص (hb_نښان_t نښان_2, بینښه صحيح *شاخص_2) ثابت
  {
    أرجع هذا->bfind (نښان_2, شاخص_2, HB_NOT_وموندل_STORE, شاخص::NOT_وموندل_شاخص);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

بنية JSTF
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_JSTF;

  بینښه صحيح get_script_شمېر () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().len;
#د_که_پای
    أرجع scriptList.len;
  }
  ثابت نښان& get_script_نښان (بینښه صحيح i) ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().get_نښان (i);
#د_که_پای
    أرجع scriptList.get_نښان (i);
  }
  بینښه صحيح get_script_نښانونه (بینښه صحيح پیل_دننه_واټن,
				بینښه صحيح *script_شمېر /* IN/OUT */,
				hb_نښان_t     *script_نښانونه /*بهر*/) ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (has_script_list2 ())
      أرجع get_script_list2 ().get_نښانونه (پیل_دننه_واټن, script_شمېر, script_نښانونه);
#د_که_پای
    أرجع scriptList.get_نښانونه (پیل_دننه_واټن, script_شمېر, script_نښانونه);
  }
  کينډۍ <ډول_نوم Types>
  ثابت JstfScript<Types>& get_script (بینښه صحيح i) ثابت;
  منطقي پیدا_کړه_script_شاخص (hb_نښان_t نښان_2, بینښه صحيح *شاخص_2) ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().پیدا_کړه_شاخص (نښان_2, شاخص_2);
#د_که_پای
    أرجع scriptList.پیدا_کړه_شاخص (نښان_2, شاخص_2);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!نسخه.sanitize (c) ||
		  !hb_barrier () ||
		  نسخه.major != 1 ||
		  !scriptList.sanitize (c, هذا)))
      return_trace (خطأ);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (unlikely (نسخه.minor >= 1 && !get_script_list2 ().sanitize (c, هذا)))
      return_trace (خطأ);
#د_که_پای
    return_trace (ریښتیا);
  }

  خاص:
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  منطقي has_script_list2 () ثابت
  { أرجع نسخه.minor >= 1 && get_script_list2 ().len; }
  ثابت JstfScriptList<Layout::MediumTypes>& get_script_list2 () ثابت
  { أرجع StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#د_که_پای

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the JSTF table--initially set
				 * to 0x00010000u */
  JstfScriptList<Layout::SmallTypes>
		scriptList;	/* Array of JstfScripts--listed
				 * alphabetically by ScriptTag */
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
/*JstfScriptList<Layout::MediumTypes>
		scriptList2;*//* Array of JstfScript2s--listed
				 * alphabetically by ScriptTag.
				 * Present only if minorVersion is 1 or greater. */
#د_که_پای
  عام:
  DEFINE_اندازه_ARRAY (6, scriptList);
};

کينډۍ <>
مضمن ثابت JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (بینښه صحيح i) ثابت
{ أرجع scriptList.get_script (هذا, i); }

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
کينډۍ <>
مضمن ثابت JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (بینښه صحيح i) ثابت
{ أرجع get_script_list2 ().get_script (هذا, i); }
#د_که_پای


} /* namespace OT */


#د_که_پای /* HB_OT_LAYOUT_JSTF_TABLE_HH */
