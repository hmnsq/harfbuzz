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

#اگر_تعریف_نشده HB_OT_LAYOUT_JSTF_جدول_HH
#تعریف_کن HB_OT_LAYOUT_JSTF_جدول_HH

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

تعريفنوع List16_از_Offset16_مقصد<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

بنية Jstf_اولویت
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
  Offset16_مقصد<JstfModList>
		shrinkageEnableGSUB;	/* Offset to Shrinkage Enable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_مقصد<JstfModList>
		shrinkageDisableGSUB;	/* Offset to Shrinkage Disable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_مقصد<JstfModList>
		shrinkageEnableGPOS;	/* Offset to Shrinkage Enable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_مقصد<JstfModList>
		shrinkageDisableGPOS;	/* Offset to Shrinkage Disable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_مقصد<JstfMax>
		shrinkageJstfMax;	/* Offset to Shrinkage JstfMax table--
					 * from beginning of JstfPriority table
					 * --may be NULL */
  Offset16_مقصد<JstfModList>
		extensionEnableGSUB;	/* Offset to Extension Enable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_مقصد<JstfModList>
		extensionDisableGSUB;	/* Offset to Extension Disable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_مقصد<JstfModList>
		extensionEnableGPOS;	/* Offset to Extension Enable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_مقصد<JstfModList>
		extensionDisableGPOS;	/* Offset to Extension Disable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_مقصد<JstfMax>
		extensionJstfMax;	/* Offset to Extension JstfMax table--
					 * from beginning of JstfPriority table
					 * --may be NULL */

  عام:
  DEFINE_اندازه_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

بنية JstfLangSys : List16_از_Offset16_مقصد<Jstf_اولویت>
{
  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت رکورد_sanitize_بستگی_t * = اشاره_تهی) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (List16_از_Offset16_مقصد<Jstf_اولویت>::sanitize (c));
  }
};


/*
 * ExtenderGlyphs -- Extender Glyph Table
 */

قالب <نام_نوع Types>
استفاده ExtenderGlyphs = Sorted_Array16_از<نام_نوع Types::HBGlyphID>;


/*
 * JstfScript -- The Justification Table
 */

قالب <نام_نوع Types>
بنية JstfScript
{
  بیعلامت صحيح get_lang_sys_شمار () ثابت
  { أرجع langSys.len; }
  ثابت برچسب& get_lang_sys_برچسب (بیعلامت صحيح i) ثابت
  { أرجع langSys.get_برچسب (i); }
  بیعلامت صحيح get_lang_sys_برچسبها (بیعلامت صحيح شروع_جابه_جایی,
				  بیعلامت صحيح *lang_sys_شمار /* IN/OUT */,
				  hb_برچسب_t     *lang_sys_برچسبها /*بیرون*/) ثابت
  { أرجع langSys.get_برچسبها (شروع_جابه_جایی, lang_sys_شمار, lang_sys_برچسبها); }
  ثابت JstfLangSys& get_lang_sys (بیعلامت صحيح i) ثابت
  {
    إذا (i == شاخص::NOT_یافت_شاخص) أرجع get_default_lang_sys ();
    أرجع هذا+langSys[i].جابه_جایی;
  }
  منطقي پیدا_کن_lang_sys_شاخص (hb_برچسب_t برچسب_2, بیعلامت صحيح *شاخص_2) ثابت
  { أرجع langSys.پیدا_کن_شاخص (برچسب_2, شاخص_2); }

  منطقي has_default_lang_sys () ثابت               { أرجع defaultLangSys != 0; }
  ثابت JstfLangSys& get_default_lang_sys () ثابت { أرجع هذا+defaultLangSys; }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت رکورد_sanitize_بستگی_t * = اشاره_تهی) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (extenderGlyphs.sanitize (c, هذا) &&
		  defaultLangSys.sanitize (c, هذا) &&
		  langSys.sanitize (c, هذا));
  }

  محمي:
  نام_نوع Types::قالب LOffset_مقصد<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  نام_نوع Types::قالب LOffset_مقصد<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  رکورد_Array_از<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  عام:
  DEFINE_اندازه_ARRAY (2 * Types::LOffset::static_اندازه + 2, langSys);
};

قالب <نام_نوع Types>
بنية Jstf_Script_رکورد
{
  صحيح cmp (hb_برچسب_t a) ثابت { أرجع برچسب_2.cmp (a); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت رکورد_sanitize_بستگی_t بستگی = {برچسب_2, پایه};
    return_trace (c->check_struct (هذا) &&
		  جابه_جایی.sanitize (c, پایه, &بستگی));
  }

  برچسب		برچسب_2;		/* 4-byte JstfScript identification tag */
  نام_نوع Types::قالب LOffset_مقصد<JstfScript<Types>>
		جابه_جایی;		/* Offset from beginning of JSTF header */
  عام:
  DEFINE_اندازه_STATIC (4 + Types::LOffset::static_اندازه);
};

قالب <نام_نوع Types>
بنية JstfScriptList : Sorted_Array16_از<Jstf_Script_رکورد<Types>>
{
  ثابت برچسب& get_برچسب (بیعلامت صحيح i) ثابت
  { أرجع (*هذا)[i].برچسب_2; }
  بیعلامت صحيح get_برچسبها (بیعلامت صحيح شروع_جابه_جایی,
			 بیعلامت صحيح *script_شمار /* IN/OUT */,
			 hb_برچسب_t     *script_برچسبها /*بیرون*/) ثابت
  {
    إذا (script_شمار)
    {
      + هذا->as_array ().sub_array (شروع_جابه_جایی, script_شمار)
      | hb_map (&Jstf_Script_رکورد<Types>::برچسب_2)
      | hb_sink (hb_array (script_برچسبها, *script_شمار))
      ;
    }
    أرجع هذا->len;
  }
  ثابت JstfScript<Types>& get_script (ثابت فراغ *پایه, بیعلامت صحيح i) ثابت
  { أرجع پایه+(*هذا)[i].جابه_جایی; }
  منطقي پیدا_کن_شاخص (hb_برچسب_t برچسب_2, بیعلامت صحيح *شاخص_2) ثابت
  {
    أرجع هذا->bfind (برچسب_2, شاخص_2, HB_NOT_یافت_STORE, شاخص::NOT_یافت_شاخص);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

بنية JSTF
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_JSTF;

  بیعلامت صحيح get_script_شمار () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().len;
#پایان_اگر
    أرجع scriptList.len;
  }
  ثابت برچسب& get_script_برچسب (بیعلامت صحيح i) ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().get_برچسب (i);
#پایان_اگر
    أرجع scriptList.get_برچسب (i);
  }
  بیعلامت صحيح get_script_برچسبها (بیعلامت صحيح شروع_جابه_جایی,
				بیعلامت صحيح *script_شمار /* IN/OUT */,
				hb_برچسب_t     *script_برچسبها /*بیرون*/) ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (has_script_list2 ())
      أرجع get_script_list2 ().get_برچسبها (شروع_جابه_جایی, script_شمار, script_برچسبها);
#پایان_اگر
    أرجع scriptList.get_برچسبها (شروع_جابه_جایی, script_شمار, script_برچسبها);
  }
  قالب <نام_نوع Types>
  ثابت JstfScript<Types>& get_script (بیعلامت صحيح i) ثابت;
  منطقي پیدا_کن_script_شاخص (hb_برچسب_t برچسب_2, بیعلامت صحيح *شاخص_2) ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().پیدا_کن_شاخص (برچسب_2, شاخص_2);
#پایان_اگر
    أرجع scriptList.پیدا_کن_شاخص (برچسب_2, شاخص_2);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!نسخه.sanitize (c) ||
		  !hb_barrier () ||
		  نسخه.major != 1 ||
		  !scriptList.sanitize (c, هذا)))
      return_trace (خطأ);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (unlikely (نسخه.minor >= 1 && !get_script_list2 ().sanitize (c, هذا)))
      return_trace (خطأ);
#پایان_اگر
    return_trace (درست);
  }

  خاص:
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  منطقي has_script_list2 () ثابت
  { أرجع نسخه.minor >= 1 && get_script_list2 ().len; }
  ثابت JstfScriptList<Layout::MediumTypes>& get_script_list2 () ثابت
  { أرجع StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#پایان_اگر

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the JSTF table--initially set
				 * to 0x00010000u */
  JstfScriptList<Layout::SmallTypes>
		scriptList;	/* Array of JstfScripts--listed
				 * alphabetically by ScriptTag */
#اگر_تعریف_نشده HB_NO_BEYOND_64K
/*JstfScriptList<Layout::MediumTypes>
		scriptList2;*//* Array of JstfScript2s--listed
				 * alphabetically by ScriptTag.
				 * Present only if minorVersion is 1 or greater. */
#پایان_اگر
  عام:
  DEFINE_اندازه_ARRAY (6, scriptList);
};

قالب <>
مضمن ثابت JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (بیعلامت صحيح i) ثابت
{ أرجع scriptList.get_script (هذا, i); }

#اگر_تعریف_نشده HB_NO_BEYOND_64K
قالب <>
مضمن ثابت JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (بیعلامت صحيح i) ثابت
{ أرجع get_script_list2 ().get_script (هذا, i); }
#پایان_اگر


} /* namespace OT */


#پایان_اگر /* HB_OT_LAYOUT_JSTF_TABLE_HH */
