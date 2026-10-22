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

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-layout-gpos-table.hh"


নামাকাশ OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

প্রকারলেখ সূচক_Array JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

প্রকারলেখ List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

কাঠামো JstfPriority
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  shrinkageEnableGSUB.sanitize (c, এই) &&
		  shrinkageDisableGSUB.sanitize (c, এই) &&
		  shrinkageEnableGPOS.sanitize (c, এই) &&
		  shrinkageDisableGPOS.sanitize (c, এই) &&
		  shrinkageJstfMax.sanitize (c, এই) &&
		  extensionEnableGSUB.sanitize (c, এই) &&
		  extensionDisableGSUB.sanitize (c, এই) &&
		  extensionEnableGPOS.sanitize (c, এই) &&
		  extensionDisableGPOS.sanitize (c, এই) &&
		  extensionJstfMax.sanitize (c, এই));
  }

  সুরক্ষিত:
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

  সর্বজনীন:
  DEFINE_আকার_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

কাঠামো JstfLangSys : List16OfOffset16To<JstfPriority>
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুবক Record_sanitize_closure_t * = nullptr) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
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
কাঠামো JstfScript
{
  unsigned পূর্ণসংখ্যা get_lang_sys_গুনতি () ধ্রুবক
  { ফেরত langSys.len; }
  ধ্রুবক Tag& get_lang_sys_tag (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত langSys.get_tag (i); }
  unsigned পূর্ণসংখ্যা get_lang_sys_tags (unsigned পূর্ণসংখ্যা start_offset,
				  unsigned পূর্ণসংখ্যা *lang_sys_গুনতি /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ধ্রুবক
  { ফেরত langSys.get_tags (start_offset, lang_sys_গুনতি, lang_sys_tags); }
  ধ্রুবক JstfLangSys& get_lang_sys (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  {
    যদি (i == সূচক::NOT_FOUND_সূচক) ফেরত get_default_lang_sys ();
    ফেরত এই+langSys[i].offset;
  }
  যৌক্তিক find_lang_sys_সূচক (hb_tag_t tag, unsigned পূর্ণসংখ্যা *সূচক_2) ধ্রুবক
  { ফেরত langSys.find_সূচক (tag, সূচক_2); }

  যৌক্তিক has_default_lang_sys () ধ্রুবক               { ফেরত defaultLangSys != 0; }
  ধ্রুবক JstfLangSys& get_default_lang_sys () ধ্রুবক { ফেরত এই+defaultLangSys; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুবক Record_sanitize_closure_t * = nullptr) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (extenderGlyphs.sanitize (c, এই) &&
		  defaultLangSys.sanitize (c, এই) &&
		  langSys.sanitize (c, এই));
  }

  সুরক্ষিত:
  typename Types::template LOffsetTo<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  typename Types::template LOffsetTo<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  RecordArrayOf<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 * Types::LOffset::static_আকার + 2, langSys);
};

template <typename Types>
কাঠামো JstfScriptRecord
{
  পূর্ণসংখ্যা cmp (hb_tag_t a) ধ্রুবক { ফেরত tag.cmp (a); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    ধ্রুবক Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (এই) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::template LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + Types::LOffset::static_আকার);
};

template <typename Types>
কাঠামো JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  ধ্রুবক Tag& get_tag (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত (*এই)[i].tag; }
  unsigned পূর্ণসংখ্যা get_tags (unsigned পূর্ণসংখ্যা start_offset,
			 unsigned পূর্ণসংখ্যা *script_গুনতি /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) ধ্রুবক
  {
    যদি (script_গুনতি)
    {
      + এই->as_array ().sub_array (start_offset, script_গুনতি)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_গুনতি))
      ;
    }
    ফেরত এই->len;
  }
  ধ্রুবক JstfScript<Types>& get_script (ধ্রুবক শূন্য *base, unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত base+(*এই)[i].offset; }
  যৌক্তিক find_সূচক (hb_tag_t tag, unsigned পূর্ণসংখ্যা *সূচক_2) ধ্রুবক
  {
    ফেরত এই->bfind (tag, সূচক_2, HB_NOT_FOUND_STORE, সূচক::NOT_FOUND_সূচক);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

কাঠামো JSTF
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  unsigned পূর্ণসংখ্যা get_script_গুনতি () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (has_script_list2 ()) ফেরত get_script_list2 ().len;
#endif
    ফেরত scriptList.len;
  }
  ধ্রুবক Tag& get_script_tag (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (has_script_list2 ()) ফেরত get_script_list2 ().get_tag (i);
#endif
    ফেরত scriptList.get_tag (i);
  }
  unsigned পূর্ণসংখ্যা get_script_tags (unsigned পূর্ণসংখ্যা start_offset,
				unsigned পূর্ণসংখ্যা *script_গুনতি /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (has_script_list2 ())
      ফেরত get_script_list2 ().get_tags (start_offset, script_গুনতি, script_tags);
#endif
    ফেরত scriptList.get_tags (start_offset, script_গুনতি, script_tags);
  }
  template <typename Types>
  ধ্রুবক JstfScript<Types>& get_script (unsigned পূর্ণসংখ্যা i) ধ্রুবক;
  যৌক্তিক find_script_সূচক (hb_tag_t tag, unsigned পূর্ণসংখ্যা *সূচক_2) ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (has_script_list2 ()) ফেরত get_script_list2 ().find_সূচক (tag, সূচক_2);
#endif
    ফেরত scriptList.find_সূচক (tag, সূচক_2);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, এই)))
      return_trace (মিথ্যা);
#ifndef HB_NO_BEYOND_64K
    যদি (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, এই)))
      return_trace (মিথ্যা);
#endif
    return_trace (সত্য);
  }

  ব্যক্তিগত:
#ifndef HB_NO_BEYOND_64K
  যৌক্তিক has_script_list2 () ধ্রুবক
  { ফেরত version.minor >= 1 && get_script_list2 ().len; }
  ধ্রুবক JstfScriptList<Layout::MediumTypes>& get_script_list2 () ধ্রুবক
  { ফেরত StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#endif

  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_ARRAY (6, scriptList);
};

template <>
অন্তঃরেখ ধ্রুবক JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (unsigned পূর্ণসংখ্যা i) ধ্রুবক
{ ফেরত scriptList.get_script (এই, i); }

#ifndef HB_NO_BEYOND_64K
template <>
অন্তঃরেখ ধ্রুবক JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (unsigned পূর্ণসংখ্যা i) ধ্রুবক
{ ফেরত get_script_list2 ().get_script (এই, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
