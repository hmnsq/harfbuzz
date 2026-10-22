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

#شامل "hb-open-type.hh"
#شامل "hb-ot-layout-gpos-table.hh"


فضاینام OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

تعریفنوع IndexArray JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

تعریفنوع List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

ساختار JstfPriority
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  shrinkageEnableGSUB.sanitize (c, این) &&
		  shrinkageDisableGSUB.sanitize (c, این) &&
		  shrinkageEnableGPOS.sanitize (c, این) &&
		  shrinkageDisableGPOS.sanitize (c, این) &&
		  shrinkageJstfMax.sanitize (c, این) &&
		  extensionEnableGSUB.sanitize (c, این) &&
		  extensionDisableGSUB.sanitize (c, این) &&
		  extensionEnableGPOS.sanitize (c, این) &&
		  extensionDisableGPOS.sanitize (c, این) &&
		  extensionJstfMax.sanitize (c, این));
  }

  محافظتشده:
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

  عمومی:
  DEFINE_SIZE_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

ساختار JstfLangSys : List16OfOffset16To<JstfPriority>
{
  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت Record_sanitize_closure_t * = nullptr) ثابت
  {
    TRACE_SANITIZE (این);
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
ساختار JstfScript
{
  unsigned صحیح get_lang_sys_شمار () ثابت
  { بازگشت langSys.len; }
  ثابت Tag& get_lang_sys_tag (unsigned صحیح i) ثابت
  { بازگشت langSys.get_tag (i); }
  unsigned صحیح get_lang_sys_tags (unsigned صحیح start_offset,
				  unsigned صحیح *lang_sys_شمار /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ثابت
  { بازگشت langSys.get_tags (start_offset, lang_sys_شمار, lang_sys_tags); }
  ثابت JstfLangSys& get_lang_sys (unsigned صحیح i) ثابت
  {
    اگر (i == Index::NOT_FOUND_INDEX) بازگشت get_default_lang_sys ();
    بازگشت این+langSys[i].offset;
  }
  منطقی find_lang_sys_index (hb_tag_t tag, unsigned صحیح *index) ثابت
  { بازگشت langSys.find_index (tag, index); }

  منطقی has_default_lang_sys () ثابت               { بازگشت defaultLangSys != 0; }
  ثابت JstfLangSys& get_default_lang_sys () ثابت { بازگشت این+defaultLangSys; }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت Record_sanitize_closure_t * = nullptr) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (extenderGlyphs.sanitize (c, این) &&
		  defaultLangSys.sanitize (c, این) &&
		  langSys.sanitize (c, این));
  }

  محافظتشده:
  typename Types::template LOffsetTo<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  typename Types::template LOffsetTo<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  RecordArrayOf<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  عمومی:
  DEFINE_SIZE_ARRAY (2 * Types::LOffset::static_size + 2, langSys);
};

template <typename Types>
ساختار JstfScriptRecord
{
  صحیح cmp (hb_tag_t a) ثابت { بازگشت tag.cmp (a); }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    ثابت Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (این) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::template LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  عمومی:
  DEFINE_SIZE_STATIC (4 + Types::LOffset::static_size);
};

template <typename Types>
ساختار JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  ثابت Tag& get_tag (unsigned صحیح i) ثابت
  { بازگشت (*این)[i].tag; }
  unsigned صحیح get_tags (unsigned صحیح start_offset,
			 unsigned صحیح *script_شمار /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) ثابت
  {
    اگر (script_شمار)
    {
      + این->as_array ().sub_array (start_offset, script_شمار)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_شمار))
      ;
    }
    بازگشت این->len;
  }
  ثابت JstfScript<Types>& get_script (ثابت تهی *base, unsigned صحیح i) ثابت
  { بازگشت base+(*این)[i].offset; }
  منطقی find_index (hb_tag_t tag, unsigned صحیح *index) ثابت
  {
    بازگشت این->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

ساختار JSTF
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  unsigned صحیح get_script_شمار () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (has_script_list2 ()) بازگشت get_script_list2 ().len;
#endif
    بازگشت scriptList.len;
  }
  ثابت Tag& get_script_tag (unsigned صحیح i) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (has_script_list2 ()) بازگشت get_script_list2 ().get_tag (i);
#endif
    بازگشت scriptList.get_tag (i);
  }
  unsigned صحیح get_script_tags (unsigned صحیح start_offset,
				unsigned صحیح *script_شمار /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (has_script_list2 ())
      بازگشت get_script_list2 ().get_tags (start_offset, script_شمار, script_tags);
#endif
    بازگشت scriptList.get_tags (start_offset, script_شمار, script_tags);
  }
  template <typename Types>
  ثابت JstfScript<Types>& get_script (unsigned صحیح i) ثابت;
  منطقی find_script_index (hb_tag_t tag, unsigned صحیح *index) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (has_script_list2 ()) بازگشت get_script_list2 ().find_index (tag, index);
#endif
    بازگشت scriptList.find_index (tag, index);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, این)))
      return_trace (نادرست);
#ifndef HB_NO_BEYOND_64K
    اگر (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, این)))
      return_trace (نادرست);
#endif
    return_trace (درست);
  }

  خصوصی:
#ifndef HB_NO_BEYOND_64K
  منطقی has_script_list2 () ثابت
  { بازگشت version.minor >= 1 && get_script_list2 ().len; }
  ثابت JstfScriptList<Layout::MediumTypes>& get_script_list2 () ثابت
  { بازگشت StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#endif

  محافظتشده:
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
  عمومی:
  DEFINE_SIZE_ARRAY (6, scriptList);
};

template <>
درونخطی ثابت JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (unsigned صحیح i) ثابت
{ بازگشت scriptList.get_script (این, i); }

#ifndef HB_NO_BEYOND_64K
template <>
درونخطی ثابت JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (unsigned صحیح i) ثابت
{ بازگشت get_script_list2 ().get_script (این, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
