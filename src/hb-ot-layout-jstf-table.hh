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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-layout-gpos-table.hh"


നാമാകാശ OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

തരനിർവചനം IndexArray JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

തരനിർവചനം List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

ഘടന JstfPriority
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  shrinkageEnableGSUB.sanitize (c, ഇത്) &&
		  shrinkageDisableGSUB.sanitize (c, ഇത്) &&
		  shrinkageEnableGPOS.sanitize (c, ഇത്) &&
		  shrinkageDisableGPOS.sanitize (c, ഇത്) &&
		  shrinkageJstfMax.sanitize (c, ഇത്) &&
		  extensionEnableGSUB.sanitize (c, ഇത്) &&
		  extensionDisableGSUB.sanitize (c, ഇത്) &&
		  extensionEnableGPOS.sanitize (c, ഇത്) &&
		  extensionDisableGPOS.sanitize (c, ഇത്) &&
		  extensionJstfMax.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
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

  പൊതു:
  DEFINE_SIZE_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

ഘടന JstfLangSys : List16OfOffset16To<JstfPriority>
{
  തർക്കം sanitize (hb_sanitize_context_t *c,
		 സ്ഥിരം Record_sanitize_closure_t * = nullptr) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
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
ഘടന JstfScript
{
  unsigned പൂർണ്ണസംഖ്യ get_lang_sys_എണ്ണം () സ്ഥിരം
  { തിരികെ langSys.len; }
  സ്ഥിരം Tag& get_lang_sys_tag (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ langSys.get_tag (i); }
  unsigned പൂർണ്ണസംഖ്യ get_lang_sys_tags (unsigned പൂർണ്ണസംഖ്യ start_offset,
				  unsigned പൂർണ്ണസംഖ്യ *lang_sys_എണ്ണം /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) സ്ഥിരം
  { തിരികെ langSys.get_tags (start_offset, lang_sys_എണ്ണം, lang_sys_tags); }
  സ്ഥിരം JstfLangSys& get_lang_sys (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  {
    എങ്കിൽ (i == Index::NOT_FOUND_INDEX) തിരികെ get_default_lang_sys ();
    തിരികെ ഇത്+langSys[i].offset;
  }
  തർക്കം find_lang_sys_index (hb_tag_t tag, unsigned പൂർണ്ണസംഖ്യ *index) സ്ഥിരം
  { തിരികെ langSys.find_index (tag, index); }

  തർക്കം has_default_lang_sys () സ്ഥിരം               { തിരികെ defaultLangSys != 0; }
  സ്ഥിരം JstfLangSys& get_default_lang_sys () സ്ഥിരം { തിരികെ ഇത്+defaultLangSys; }

  തർക്കം sanitize (hb_sanitize_context_t *c,
		 സ്ഥിരം Record_sanitize_closure_t * = nullptr) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (extenderGlyphs.sanitize (c, ഇത്) &&
		  defaultLangSys.sanitize (c, ഇത്) &&
		  langSys.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  typename Types::template LOffsetTo<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  typename Types::template LOffsetTo<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  RecordArrayOf<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 * Types::LOffset::static_size + 2, langSys);
};

template <typename Types>
ഘടന JstfScriptRecord
{
  പൂർണ്ണസംഖ്യ cmp (hb_tag_t a) സ്ഥിരം { തിരികെ tag.cmp (a); }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    സ്ഥിരം Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (ഇത്) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::template LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  പൊതു:
  DEFINE_SIZE_STATIC (4 + Types::LOffset::static_size);
};

template <typename Types>
ഘടന JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  സ്ഥിരം Tag& get_tag (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ (*ഇത്)[i].tag; }
  unsigned പൂർണ്ണസംഖ്യ get_tags (unsigned പൂർണ്ണസംഖ്യ start_offset,
			 unsigned പൂർണ്ണസംഖ്യ *script_എണ്ണം /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (script_എണ്ണം)
    {
      + ഇത്->as_array ().sub_array (start_offset, script_എണ്ണം)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_എണ്ണം))
      ;
    }
    തിരികെ ഇത്->len;
  }
  സ്ഥിരം JstfScript<Types>& get_script (സ്ഥിരം ശൂന്യം *base, unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ base+(*ഇത്)[i].offset; }
  തർക്കം find_index (hb_tag_t tag, unsigned പൂർണ്ണസംഖ്യ *index) സ്ഥിരം
  {
    തിരികെ ഇത്->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

ഘടന JSTF
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  unsigned പൂർണ്ണസംഖ്യ get_script_എണ്ണം () സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (has_script_list2 ()) തിരികെ get_script_list2 ().len;
#endif
    തിരികെ scriptList.len;
  }
  സ്ഥിരം Tag& get_script_tag (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (has_script_list2 ()) തിരികെ get_script_list2 ().get_tag (i);
#endif
    തിരികെ scriptList.get_tag (i);
  }
  unsigned പൂർണ്ണസംഖ്യ get_script_tags (unsigned പൂർണ്ണസംഖ്യ start_offset,
				unsigned പൂർണ്ണസംഖ്യ *script_എണ്ണം /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (has_script_list2 ())
      തിരികെ get_script_list2 ().get_tags (start_offset, script_എണ്ണം, script_tags);
#endif
    തിരികെ scriptList.get_tags (start_offset, script_എണ്ണം, script_tags);
  }
  template <typename Types>
  സ്ഥിരം JstfScript<Types>& get_script (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം;
  തർക്കം find_script_index (hb_tag_t tag, unsigned പൂർണ്ണസംഖ്യ *index) സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (has_script_list2 ()) തിരികെ get_script_list2 ().find_index (tag, index);
#endif
    തിരികെ scriptList.find_index (tag, index);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, ഇത്)))
      return_trace (അസത്യം);
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, ഇത്)))
      return_trace (അസത്യം);
#endif
    return_trace (സത്യം);
  }

  സ്വകാര്യ:
#ifndef HB_NO_BEYOND_64K
  തർക്കം has_script_list2 () സ്ഥിരം
  { തിരികെ version.minor >= 1 && get_script_list2 ().len; }
  സ്ഥിരം JstfScriptList<Layout::MediumTypes>& get_script_list2 () സ്ഥിരം
  { തിരികെ StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#endif

  സംരക്ഷിത:
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
  പൊതു:
  DEFINE_SIZE_ARRAY (6, scriptList);
};

template <>
വരിയിൽ സ്ഥിരം JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{ തിരികെ scriptList.get_script (ഇത്, i); }

#ifndef HB_NO_BEYOND_64K
template <>
വരിയിൽ സ്ഥിരം JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
{ തിരികെ get_script_list2 ().get_script (ഇത്, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
