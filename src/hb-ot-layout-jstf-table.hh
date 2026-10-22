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

#اگر_غیر_تعریف_شدہ HB_OT_LAYOUT_JSTF_جدول_HH
#تعریف_کرو HB_OT_LAYOUT_JSTF_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-layout-gpos-table.hh"


نامخلا OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

قسمتعریف اشاریہ_Array JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

قسمتعریف List16_کا_Offset16_مقصد<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

ساخت Jstf_ترجیح
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  shrinkageEnableGSUB.sanitize (c, یہ) &&
		  shrinkageDisableGSUB.sanitize (c, یہ) &&
		  shrinkageEnableGPOS.sanitize (c, یہ) &&
		  shrinkageDisableGPOS.sanitize (c, یہ) &&
		  shrinkageJstfMax.sanitize (c, یہ) &&
		  extensionEnableGSUB.sanitize (c, یہ) &&
		  extensionDisableGSUB.sanitize (c, یہ) &&
		  extensionEnableGPOS.sanitize (c, یہ) &&
		  extensionDisableGPOS.sanitize (c, یہ) &&
		  extensionJstfMax.sanitize (c, یہ));
  }

  محفوظ:
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

  عوامی:
  DEFINE_سائز_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

ساخت JstfLangSys : List16_کا_Offset16_مقصد<Jstf_ترجیح>
{
  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارڈ_sanitize_احاطہ_t * = خالی_اشاریہ) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (List16_کا_Offset16_مقصد<Jstf_ترجیح>::sanitize (c));
  }
};


/*
 * ExtenderGlyphs -- Extender Glyph Table
 */

سانچہ <قسم_نام Types>
استعمال ExtenderGlyphs = Sorted_Array16_کا<قسم_نام Types::HBGlyphID>;


/*
 * JstfScript -- The Justification Table
 */

سانچہ <قسم_نام Types>
ساخت JstfScript
{
  غیرعلامتی صحیح get_lang_sys_گنتی () ثابت
  { واپس langSys.len; }
  ثابت لیبل& get_lang_sys_لیبل (غیرعلامتی صحیح i) ثابت
  { واپس langSys.get_لیبل (i); }
  غیرعلامتی صحیح get_lang_sys_لیبل_فہرست (غیرعلامتی صحیح آغاز_انحراف,
				  غیرعلامتی صحیح *lang_sys_گنتی /* IN/OUT */,
				  hb_لیبل_t     *lang_sys_لیبل_فہرست /*برآمد*/) ثابت
  { واپس langSys.get_لیبل_فہرست (آغاز_انحراف, lang_sys_گنتی, lang_sys_لیبل_فہرست); }
  ثابت JstfLangSys& get_lang_sys (غیرعلامتی صحیح i) ثابت
  {
    اگر (i == اشاریہ::NOT_ملا_اشاریہ) واپس get_default_lang_sys ();
    واپس یہ+langSys[i].انحراف;
  }
  منطقی ڈھونڈو_lang_sys_اشاریہ (hb_لیبل_t لیبل_2, غیرعلامتی صحیح *اشاریہ_2) ثابت
  { واپس langSys.ڈھونڈو_اشاریہ (لیبل_2, اشاریہ_2); }

  منطقی has_default_lang_sys () ثابت               { واپس defaultLangSys != 0; }
  ثابت JstfLangSys& get_default_lang_sys () ثابت { واپس یہ+defaultLangSys; }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارڈ_sanitize_احاطہ_t * = خالی_اشاریہ) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (extenderGlyphs.sanitize (c, یہ) &&
		  defaultLangSys.sanitize (c, یہ) &&
		  langSys.sanitize (c, یہ));
  }

  محفوظ:
  قسم_نام Types::سانچہ LOffset_مقصد<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  قسم_نام Types::سانچہ LOffset_مقصد<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  ریکارڈ_Array_کا<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  عوامی:
  DEFINE_سائز_ARRAY (2 * Types::LOffset::static_سائز + 2, langSys);
};

سانچہ <قسم_نام Types>
ساخت Jstf_Script_ریکارڈ
{
  صحیح cmp (hb_لیبل_t a) ثابت { واپس لیبل_2.cmp (a); }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    ثابت ریکارڈ_sanitize_احاطہ_t احاطہ = {لیبل_2, بنیاد};
    return_trace (c->check_struct (یہ) &&
		  انحراف.sanitize (c, بنیاد, &احاطہ));
  }

  لیبل		لیبل_2;		/* 4-byte JstfScript identification tag */
  قسم_نام Types::سانچہ LOffset_مقصد<JstfScript<Types>>
		انحراف;		/* Offset from beginning of JSTF header */
  عوامی:
  DEFINE_سائز_STATIC (4 + Types::LOffset::static_سائز);
};

سانچہ <قسم_نام Types>
ساخت JstfScriptList : Sorted_Array16_کا<Jstf_Script_ریکارڈ<Types>>
{
  ثابت لیبل& get_لیبل (غیرعلامتی صحیح i) ثابت
  { واپس (*یہ)[i].لیبل_2; }
  غیرعلامتی صحیح get_لیبل_فہرست (غیرعلامتی صحیح آغاز_انحراف,
			 غیرعلامتی صحیح *script_گنتی /* IN/OUT */,
			 hb_لیبل_t     *script_لیبل_فہرست /*برآمد*/) ثابت
  {
    اگر (script_گنتی)
    {
      + یہ->as_array ().sub_array (آغاز_انحراف, script_گنتی)
      | hb_map (&Jstf_Script_ریکارڈ<Types>::لیبل_2)
      | hb_sink (hb_array (script_لیبل_فہرست, *script_گنتی))
      ;
    }
    واپس یہ->len;
  }
  ثابت JstfScript<Types>& get_script (ثابت خالی *بنیاد, غیرعلامتی صحیح i) ثابت
  { واپس بنیاد+(*یہ)[i].انحراف; }
  منطقی ڈھونڈو_اشاریہ (hb_لیبل_t لیبل_2, غیرعلامتی صحیح *اشاریہ_2) ثابت
  {
    واپس یہ->bfind (لیبل_2, اشاریہ_2, HB_NOT_ملا_STORE, اشاریہ::NOT_ملا_اشاریہ);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

ساخت JSTF
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_JSTF;

  غیرعلامتی صحیح get_script_گنتی () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (has_script_list2 ()) واپس get_script_list2 ().len;
#اگر_ختم
    واپس scriptList.len;
  }
  ثابت لیبل& get_script_لیبل (غیرعلامتی صحیح i) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (has_script_list2 ()) واپس get_script_list2 ().get_لیبل (i);
#اگر_ختم
    واپس scriptList.get_لیبل (i);
  }
  غیرعلامتی صحیح get_script_لیبل_فہرست (غیرعلامتی صحیح آغاز_انحراف,
				غیرعلامتی صحیح *script_گنتی /* IN/OUT */,
				hb_لیبل_t     *script_لیبل_فہرست /*برآمد*/) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (has_script_list2 ())
      واپس get_script_list2 ().get_لیبل_فہرست (آغاز_انحراف, script_گنتی, script_لیبل_فہرست);
#اگر_ختم
    واپس scriptList.get_لیبل_فہرست (آغاز_انحراف, script_گنتی, script_لیبل_فہرست);
  }
  سانچہ <قسم_نام Types>
  ثابت JstfScript<Types>& get_script (غیرعلامتی صحیح i) ثابت;
  منطقی ڈھونڈو_script_اشاریہ (hb_لیبل_t لیبل_2, غیرعلامتی صحیح *اشاریہ_2) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (has_script_list2 ()) واپس get_script_list2 ().ڈھونڈو_اشاریہ (لیبل_2, اشاریہ_2);
#اگر_ختم
    واپس scriptList.ڈھونڈو_اشاریہ (لیبل_2, اشاریہ_2);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!نسخہ.sanitize (c) ||
		  !hb_barrier () ||
		  نسخہ.major != 1 ||
		  !scriptList.sanitize (c, یہ)))
      return_trace (جھوٹ);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (unlikely (نسخہ.minor >= 1 && !get_script_list2 ().sanitize (c, یہ)))
      return_trace (جھوٹ);
#اگر_ختم
    return_trace (سچ);
  }

  نجی:
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  منطقی has_script_list2 () ثابت
  { واپس نسخہ.minor >= 1 && get_script_list2 ().len; }
  ثابت JstfScriptList<Layout::MediumTypes>& get_script_list2 () ثابت
  { واپس StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#اگر_ختم

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version of the JSTF table--initially set
				 * to 0x00010000u */
  JstfScriptList<Layout::SmallTypes>
		scriptList;	/* Array of JstfScripts--listed
				 * alphabetically by ScriptTag */
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
/*JstfScriptList<Layout::MediumTypes>
		scriptList2;*//* Array of JstfScript2s--listed
				 * alphabetically by ScriptTag.
				 * Present only if minorVersion is 1 or greater. */
#اگر_ختم
  عوامی:
  DEFINE_سائز_ARRAY (6, scriptList);
};

سانچہ <>
اندرونی ثابت JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (غیرعلامتی صحیح i) ثابت
{ واپس scriptList.get_script (یہ, i); }

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
سانچہ <>
اندرونی ثابت JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (غیرعلامتی صحیح i) ثابت
{ واپس get_script_list2 ().get_script (یہ, i); }
#اگر_ختم


} /* namespace OT */


#اگر_ختم /* HB_OT_LAYOUT_JSTF_TABLE_HH */
