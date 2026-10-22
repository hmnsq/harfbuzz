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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_LAYOUT_JSTF_جدول_HH
#وصف_ڪريو HB_OT_LAYOUT_JSTF_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-gpos-table.hh"


نطاق OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

تعريفنوع اشاريو_Array JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

تعريفنوع List16_جو_Offset16_منزل<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

بنية Jstf_ترجيح
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
  DEFINE_سائيز_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

بنية JstfLangSys : List16_جو_Offset16_منزل<Jstf_ترجيح>
{
  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت رڪارڊ_sanitize_گهيرو_t * = خالي_اشارو) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (List16_جو_Offset16_منزل<Jstf_ترجيح>::sanitize (c));
  }
};


/*
 * ExtenderGlyphs -- Extender Glyph Table
 */

سانچو <قسم_نالو Types>
استعمال ExtenderGlyphs = Sorted_Array16_جو<قسم_نالو Types::HBGlyphID>;


/*
 * JstfScript -- The Justification Table
 */

سانچو <قسم_نالو Types>
بنية JstfScript
{
  بنانشان صحيح get_lang_sys_ڳڻپ () ثابت
  { أرجع langSys.len; }
  ثابت ٽيگ& get_lang_sys_ٽيگ (بنانشان صحيح i) ثابت
  { أرجع langSys.get_ٽيگ (i); }
  بنانشان صحيح get_lang_sys_ٽيگ_فهرست (بنانشان صحيح شروعات_وٿي,
				  بنانشان صحيح *lang_sys_ڳڻپ /* IN/OUT */,
				  hb_ٽيگ_t     *lang_sys_ٽيگ_فهرست /*ٻاهريون*/) ثابت
  { أرجع langSys.get_ٽيگ_فهرست (شروعات_وٿي, lang_sys_ڳڻپ, lang_sys_ٽيگ_فهرست); }
  ثابت JstfLangSys& get_lang_sys (بنانشان صحيح i) ثابت
  {
    إذا (i == اشاريو::NOT_مليو_اشاريو) أرجع get_default_lang_sys ();
    أرجع هذا+langSys[i].وٿي;
  }
  منطقي ڳوليو_lang_sys_اشاريو (hb_ٽيگ_t ٽيگ_2, بنانشان صحيح *اشاريو_2) ثابت
  { أرجع langSys.ڳوليو_اشاريو (ٽيگ_2, اشاريو_2); }

  منطقي has_default_lang_sys () ثابت               { أرجع defaultLangSys != 0; }
  ثابت JstfLangSys& get_default_lang_sys () ثابت { أرجع هذا+defaultLangSys; }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت رڪارڊ_sanitize_گهيرو_t * = خالي_اشارو) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (extenderGlyphs.sanitize (c, هذا) &&
		  defaultLangSys.sanitize (c, هذا) &&
		  langSys.sanitize (c, هذا));
  }

  محمي:
  قسم_نالو Types::سانچو LOffset_منزل<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  قسم_نالو Types::سانچو LOffset_منزل<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  رڪارڊ_Array_جو<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  عام:
  DEFINE_سائيز_ARRAY (2 * Types::LOffset::static_سائيز + 2, langSys);
};

سانچو <قسم_نالو Types>
بنية Jstf_Script_رڪارڊ
{
  صحيح cmp (hb_ٽيگ_t a) ثابت { أرجع ٽيگ_2.cmp (a); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت رڪارڊ_sanitize_گهيرو_t گهيرو = {ٽيگ_2, بنياد};
    return_trace (c->check_struct (هذا) &&
		  وٿي.sanitize (c, بنياد, &گهيرو));
  }

  ٽيگ		ٽيگ_2;		/* 4-byte JstfScript identification tag */
  قسم_نالو Types::سانچو LOffset_منزل<JstfScript<Types>>
		وٿي;		/* Offset from beginning of JSTF header */
  عام:
  DEFINE_سائيز_STATIC (4 + Types::LOffset::static_سائيز);
};

سانچو <قسم_نالو Types>
بنية JstfScriptList : Sorted_Array16_جو<Jstf_Script_رڪارڊ<Types>>
{
  ثابت ٽيگ& get_ٽيگ (بنانشان صحيح i) ثابت
  { أرجع (*هذا)[i].ٽيگ_2; }
  بنانشان صحيح get_ٽيگ_فهرست (بنانشان صحيح شروعات_وٿي,
			 بنانشان صحيح *script_ڳڻپ /* IN/OUT */,
			 hb_ٽيگ_t     *script_ٽيگ_فهرست /*ٻاهريون*/) ثابت
  {
    إذا (script_ڳڻپ)
    {
      + هذا->as_array ().sub_array (شروعات_وٿي, script_ڳڻپ)
      | hb_map (&Jstf_Script_رڪارڊ<Types>::ٽيگ_2)
      | hb_sink (hb_array (script_ٽيگ_فهرست, *script_ڳڻپ))
      ;
    }
    أرجع هذا->len;
  }
  ثابت JstfScript<Types>& get_script (ثابت فراغ *بنياد, بنانشان صحيح i) ثابت
  { أرجع بنياد+(*هذا)[i].وٿي; }
  منطقي ڳوليو_اشاريو (hb_ٽيگ_t ٽيگ_2, بنانشان صحيح *اشاريو_2) ثابت
  {
    أرجع هذا->bfind (ٽيگ_2, اشاريو_2, HB_NOT_مليو_STORE, اشاريو::NOT_مليو_اشاريو);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

بنية JSTF
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_JSTF;

  بنانشان صحيح get_script_ڳڻپ () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().len;
#جيڪڏهن_پورو
    أرجع scriptList.len;
  }
  ثابت ٽيگ& get_script_ٽيگ (بنانشان صحيح i) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().get_ٽيگ (i);
#جيڪڏهن_پورو
    أرجع scriptList.get_ٽيگ (i);
  }
  بنانشان صحيح get_script_ٽيگ_فهرست (بنانشان صحيح شروعات_وٿي,
				بنانشان صحيح *script_ڳڻپ /* IN/OUT */,
				hb_ٽيگ_t     *script_ٽيگ_فهرست /*ٻاهريون*/) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (has_script_list2 ())
      أرجع get_script_list2 ().get_ٽيگ_فهرست (شروعات_وٿي, script_ڳڻپ, script_ٽيگ_فهرست);
#جيڪڏهن_پورو
    أرجع scriptList.get_ٽيگ_فهرست (شروعات_وٿي, script_ڳڻپ, script_ٽيگ_فهرست);
  }
  سانچو <قسم_نالو Types>
  ثابت JstfScript<Types>& get_script (بنانشان صحيح i) ثابت;
  منطقي ڳوليو_script_اشاريو (hb_ٽيگ_t ٽيگ_2, بنانشان صحيح *اشاريو_2) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().ڳوليو_اشاريو (ٽيگ_2, اشاريو_2);
#جيڪڏهن_پورو
    أرجع scriptList.ڳوليو_اشاريو (ٽيگ_2, اشاريو_2);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!نسخو.sanitize (c) ||
		  !hb_barrier () ||
		  نسخو.major != 1 ||
		  !scriptList.sanitize (c, هذا)))
      return_trace (خطأ);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (unlikely (نسخو.minor >= 1 && !get_script_list2 ().sanitize (c, هذا)))
      return_trace (خطأ);
#جيڪڏهن_پورو
    return_trace (سچ);
  }

  خاص:
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  منطقي has_script_list2 () ثابت
  { أرجع نسخو.minor >= 1 && get_script_list2 ().len; }
  ثابت JstfScriptList<Layout::MediumTypes>& get_script_list2 () ثابت
  { أرجع StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#جيڪڏهن_پورو

  محمي:
  Fixed_نسخو<>نسخو;	/* Version of the JSTF table--initially set
				 * to 0x00010000u */
  JstfScriptList<Layout::SmallTypes>
		scriptList;	/* Array of JstfScripts--listed
				 * alphabetically by ScriptTag */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
/*JstfScriptList<Layout::MediumTypes>
		scriptList2;*//* Array of JstfScript2s--listed
				 * alphabetically by ScriptTag.
				 * Present only if minorVersion is 1 or greater. */
#جيڪڏهن_پورو
  عام:
  DEFINE_سائيز_ARRAY (6, scriptList);
};

سانچو <>
مضمن ثابت JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (بنانشان صحيح i) ثابت
{ أرجع scriptList.get_script (هذا, i); }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
سانچو <>
مضمن ثابت JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (بنانشان صحيح i) ثابت
{ أرجع get_script_list2 ().get_script (هذا, i); }
#جيڪڏهن_پورو


} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_LAYOUT_JSTF_TABLE_HH */
