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

#অন্তর্ভাৱ "hb-open-type.hh"
#অন্তর্ভাৱ "hb-ot-layout-gpos-table.hh"


নামাকাশ OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

প্রকারনির্দেশ IndexArray JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

প্রকারনির্দেশ List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

সংরচনা JstfPriority
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  shrinkageEnableGSUB.sanitize (c, এতত্) &&
		  shrinkageDisableGSUB.sanitize (c, এতত্) &&
		  shrinkageEnableGPOS.sanitize (c, এতত্) &&
		  shrinkageDisableGPOS.sanitize (c, এতত্) &&
		  shrinkageJstfMax.sanitize (c, এতত্) &&
		  extensionEnableGSUB.sanitize (c, এতত্) &&
		  extensionDisableGSUB.sanitize (c, এতত্) &&
		  extensionEnableGPOS.sanitize (c, এতত্) &&
		  extensionDisableGPOS.sanitize (c, এতত্) &&
		  extensionJstfMax.sanitize (c, এতত্));
  }

  রক্ষিত:
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

  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

সংরচনা JstfLangSys : List16OfOffset16To<JstfPriority>
{
  তার্কিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুৱ Record_sanitize_closure_t * = nullptr) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
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
সংরচনা JstfScript
{
  unsigned পূর্ণাঙ্ক get_lang_sys_গুনতি () ধ্রুৱ
  { ঘূৰাই langSys.len; }
  ধ্রুৱ Tag& get_lang_sys_tag (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই langSys.get_tag (i); }
  unsigned পূর্ণাঙ্ক get_lang_sys_tags (unsigned পূর্ণাঙ্ক start_offset,
				  unsigned পূর্ণাঙ্ক *lang_sys_গুনতি /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ধ্রুৱ
  { ঘূৰাই langSys.get_tags (start_offset, lang_sys_গুনতি, lang_sys_tags); }
  ধ্রুৱ JstfLangSys& get_lang_sys (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  {
    যদি (i == Index::NOT_FOUND_INDEX) ঘূৰাই get_default_lang_sys ();
    ঘূৰাই এতত্+langSys[i].offset;
  }
  তার্কিক find_lang_sys_index (hb_tag_t tag, unsigned পূর্ণাঙ্ক *index) ধ্রুৱ
  { ঘূৰাই langSys.find_index (tag, index); }

  তার্কিক has_default_lang_sys () ধ্রুৱ               { ঘূৰাই defaultLangSys != 0; }
  ধ্রুৱ JstfLangSys& get_default_lang_sys () ধ্রুৱ { ঘূৰাই এতত্+defaultLangSys; }

  তার্কিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুৱ Record_sanitize_closure_t * = nullptr) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (extenderGlyphs.sanitize (c, এতত্) &&
		  defaultLangSys.sanitize (c, এতত্) &&
		  langSys.sanitize (c, এতত্));
  }

  রক্ষিত:
  typename Types::template LOffsetTo<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  typename Types::template LOffsetTo<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  RecordArrayOf<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2 * Types::LOffset::static_size + 2, langSys);
};

template <typename Types>
সংরচনা JstfScriptRecord
{
  পূর্ণাঙ্ক cmp (hb_tag_t a) ধ্রুৱ { ঘূৰাই tag.cmp (a); }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    ধ্রুৱ Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (এতত্) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::template LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + Types::LOffset::static_size);
};

template <typename Types>
সংরচনা JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  ধ্রুৱ Tag& get_tag (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই (*এতত্)[i].tag; }
  unsigned পূর্ণাঙ্ক get_tags (unsigned পূর্ণাঙ্ক start_offset,
			 unsigned পূর্ণাঙ্ক *script_গুনতি /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) ধ্রুৱ
  {
    যদি (script_গুনতি)
    {
      + এতত্->as_array ().sub_array (start_offset, script_গুনতি)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_গুনতি))
      ;
    }
    ঘূৰাই এতত্->len;
  }
  ধ্রুৱ JstfScript<Types>& get_script (ধ্রুৱ শূন্য *base, unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই base+(*এতত্)[i].offset; }
  তার্কিক find_index (hb_tag_t tag, unsigned পূর্ণাঙ্ক *index) ধ্রুৱ
  {
    ঘূৰাই এতত্->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

সংরচনা JSTF
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  unsigned পূর্ণাঙ্ক get_script_গুনতি () ধ্রুৱ
  {
#ifndef HB_NO_BEYOND_64K
    যদি (has_script_list2 ()) ঘূৰাই get_script_list2 ().len;
#endif
    ঘূৰাই scriptList.len;
  }
  ধ্রুৱ Tag& get_script_tag (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  {
#ifndef HB_NO_BEYOND_64K
    যদি (has_script_list2 ()) ঘূৰাই get_script_list2 ().get_tag (i);
#endif
    ঘূৰাই scriptList.get_tag (i);
  }
  unsigned পূর্ণাঙ্ক get_script_tags (unsigned পূর্ণাঙ্ক start_offset,
				unsigned পূর্ণাঙ্ক *script_গুনতি /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ধ্রুৱ
  {
#ifndef HB_NO_BEYOND_64K
    যদি (has_script_list2 ())
      ঘূৰাই get_script_list2 ().get_tags (start_offset, script_গুনতি, script_tags);
#endif
    ঘূৰাই scriptList.get_tags (start_offset, script_গুনতি, script_tags);
  }
  template <typename Types>
  ধ্রুৱ JstfScript<Types>& get_script (unsigned পূর্ণাঙ্ক i) ধ্রুৱ;
  তার্কিক find_script_index (hb_tag_t tag, unsigned পূর্ণাঙ্ক *index) ধ্রুৱ
  {
#ifndef HB_NO_BEYOND_64K
    যদি (has_script_list2 ()) ঘূৰাই get_script_list2 ().find_index (tag, index);
#endif
    ঘূৰাই scriptList.find_index (tag, index);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, এতত্)))
      return_trace (মিছা);
#ifndef HB_NO_BEYOND_64K
    যদি (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, এতত্)))
      return_trace (মিছা);
#endif
    return_trace (সঁচা);
  }

  গুপ্ত:
#ifndef HB_NO_BEYOND_64K
  তার্কিক has_script_list2 () ধ্রুৱ
  { ঘূৰাই version.minor >= 1 && get_script_list2 ().len; }
  ধ্রুৱ JstfScriptList<Layout::MediumTypes>& get_script_list2 () ধ্রুৱ
  { ঘূৰাই StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#endif

  রক্ষিত:
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
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (6, scriptList);
};

template <>
অন্তঃস্থ ধ্রুৱ JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
{ ঘূৰাই scriptList.get_script (এতত্, i); }

#ifndef HB_NO_BEYOND_64K
template <>
অন্তঃস্থ ধ্রুৱ JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
{ ঘূৰাই get_script_list2 ().get_script (এতত্, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
