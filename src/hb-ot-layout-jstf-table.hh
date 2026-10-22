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

#ifndef HB_OT_LAYOUT_JSTF_TABLE_HH
#define HB_OT_LAYOUT_JSTF_TABLE_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-gpos-table.hh"


نطاق OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

تعريفنوع IndexArray JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

تعريفنوع List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

بنية JstfPriority
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
  Offset16To<JstfModList>
		shrinkageEnableGSUB;	/* Offset to Shrinkage Enable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16To<JstfModList>
		shrinkageDisableGSUB;	/* Offset to Shrinkage Disable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16To<JstfModList>
		shrinkageEnableGPOS;	/* Offset to Shrinkage Enable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16To<JstfModList>
		shrinkageDisableGPOS;	/* Offset to Shrinkage Disable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16To<JstfMax>
		shrinkageJstfMax;	/* Offset to Shrinkage JstfMax table--
					 * from beginning of JstfPriority table
					 * --may be NULL */
  Offset16To<JstfModList>
		extensionEnableGSUB;	/* Offset to Extension Enable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16To<JstfModList>
		extensionDisableGSUB;	/* Offset to Extension Disable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16To<JstfModList>
		extensionEnableGPOS;	/* Offset to Extension Enable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16To<JstfModList>
		extensionDisableGPOS;	/* Offset to Extension Disable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16To<JstfMax>
		extensionJstfMax;	/* Offset to Extension JstfMax table--
					 * from beginning of JstfPriority table
					 * --may be NULL */

  عام:
  DEFINE_SIZE_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

بنية JstfLangSys : List16OfOffset16To<JstfPriority>
{
  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت Record_sanitize_closure_t * = nullptr) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (List16OfOffset16To<JstfPriority>::sanitize (c));
  }
};


/*
 * ExtenderGlyphs -- Extender Glyph Table
 */

template <typename Types>
using ExtenderGlyphs = SortedArray16Of<typename Types::HBGlyphID>;


/*
 * JstfScript -- The Justification Table
 */

template <typename Types>
بنية JstfScript
{
  unsigned صحيح get_lang_sys_کيراءن () ثابت
  { أرجع langSys.len; }
  ثابت Tag& get_lang_sys_tag (unsigned صحيح i) ثابت
  { أرجع langSys.get_tag (i); }
  unsigned صحيح get_lang_sys_tags (unsigned صحيح start_offset,
				  unsigned صحيح *lang_sys_کيراءن /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ثابت
  { أرجع langSys.get_tags (start_offset, lang_sys_کيراءن, lang_sys_tags); }
  ثابت JstfLangSys& get_lang_sys (unsigned صحيح i) ثابت
  {
    إذا (i == Index::NOT_FOUND_INDEX) أرجع get_default_lang_sys ();
    أرجع هذا+langSys[i].offset;
  }
  منطقي find_lang_sys_index (hb_tag_t tag, unsigned صحيح *index) ثابت
  { أرجع langSys.find_index (tag, index); }

  منطقي has_default_lang_sys () ثابت               { أرجع defaultLangSys != 0; }
  ثابت JstfLangSys& get_default_lang_sys () ثابت { أرجع هذا+defaultLangSys; }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت Record_sanitize_closure_t * = nullptr) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (extenderGlyphs.sanitize (c, هذا) &&
		  defaultLangSys.sanitize (c, هذا) &&
		  langSys.sanitize (c, هذا));
  }

  محمي:
  typename Types::template LOffsetTo<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  typename Types::template LOffsetTo<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  RecordArrayOf<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  عام:
  DEFINE_SIZE_ARRAY (2 * Types::LOffset::static_size + 2, langSys);
};

template <typename Types>
بنية JstfScriptRecord
{
  صحيح cmp (hb_tag_t a) ثابت { أرجع tag.cmp (a); }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (هذا) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::template LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  عام:
  DEFINE_SIZE_STATIC (4 + Types::LOffset::static_size);
};

template <typename Types>
بنية JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  ثابت Tag& get_tag (unsigned صحيح i) ثابت
  { أرجع (*هذا)[i].tag; }
  unsigned صحيح get_tags (unsigned صحيح start_offset,
			 unsigned صحيح *script_کيراءن /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) ثابت
  {
    إذا (script_کيراءن)
    {
      + هذا->as_array ().sub_array (start_offset, script_کيراءن)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_کيراءن))
      ;
    }
    أرجع هذا->len;
  }
  ثابت JstfScript<Types>& get_script (ثابت فراغ *base, unsigned صحيح i) ثابت
  { أرجع base+(*هذا)[i].offset; }
  منطقي find_index (hb_tag_t tag, unsigned صحيح *index) ثابت
  {
    أرجع هذا->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

بنية JSTF
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  unsigned صحيح get_script_کيراءن () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().len;
#endif
    أرجع scriptList.len;
  }
  ثابت Tag& get_script_tag (unsigned صحيح i) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().get_tag (i);
#endif
    أرجع scriptList.get_tag (i);
  }
  unsigned صحيح get_script_tags (unsigned صحيح start_offset,
				unsigned صحيح *script_کيراءن /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (has_script_list2 ())
      أرجع get_script_list2 ().get_tags (start_offset, script_کيراءن, script_tags);
#endif
    أرجع scriptList.get_tags (start_offset, script_کيراءن, script_tags);
  }
  template <typename Types>
  ثابت JstfScript<Types>& get_script (unsigned صحيح i) ثابت;
  منطقي find_script_index (hb_tag_t tag, unsigned صحيح *index) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (has_script_list2 ()) أرجع get_script_list2 ().find_index (tag, index);
#endif
    أرجع scriptList.find_index (tag, index);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, هذا)))
      return_trace (خطأ);
#ifndef HB_NO_BEYOND_64K
    إذا (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, هذا)))
      return_trace (خطأ);
#endif
    return_trace (true);
  }

  خاص:
#ifndef HB_NO_BEYOND_64K
  منطقي has_script_list2 () ثابت
  { أرجع version.minor >= 1 && get_script_list2 ().len; }
  ثابت JstfScriptList<Layout::MediumTypes>& get_script_list2 () ثابت
  { أرجع StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#endif

  محمي:
  FixedVersion<>version;	/* Version of the JSTF table--initially set
				 * to 0x00010000u */
  JstfScriptList<Layout::SmallTypes>
		scriptList;	/* Array of JstfScripts--listed
				 * alphabetically by ScriptTag */
#ifndef HB_NO_BEYOND_64K
/*JstfScriptList<Layout::MediumTypes>
		scriptList2;*//* Array of JstfScript2s--listed
				 * alphabetically by ScriptTag.
				 * Present only if minorVersion is 1 or greater. */
#endif
  عام:
  DEFINE_SIZE_ARRAY (6, scriptList);
};

template <>
مضمن ثابت JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (unsigned صحيح i) ثابت
{ أرجع scriptList.get_script (هذا, i); }

#ifndef HB_NO_BEYOND_64K
template <>
مضمن ثابت JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (unsigned صحيح i) ثابت
{ أرجع get_script_list2 ().get_script (هذا, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
