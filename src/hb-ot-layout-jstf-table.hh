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

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-layout-gpos-table.hh"


નામાકાશ OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

પ્રકારવ્યાખ્યા IndexArray JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

પ્રકારવ્યાખ્યા List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

સંરચના JstfPriority
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  shrinkageEnableGSUB.sanitize (c, આ) &&
		  shrinkageDisableGSUB.sanitize (c, આ) &&
		  shrinkageEnableGPOS.sanitize (c, આ) &&
		  shrinkageDisableGPOS.sanitize (c, આ) &&
		  shrinkageJstfMax.sanitize (c, આ) &&
		  extensionEnableGSUB.sanitize (c, આ) &&
		  extensionDisableGSUB.sanitize (c, આ) &&
		  extensionEnableGPOS.sanitize (c, આ) &&
		  extensionDisableGPOS.sanitize (c, આ) &&
		  extensionJstfMax.sanitize (c, આ));
  }

  સુરક્ષિત:
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

  જાહેર:
  DEFINE_SIZE_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

સંરચના JstfLangSys : List16OfOffset16To<JstfPriority>
{
  તાર્કિક sanitize (hb_sanitize_context_t *c,
		 અચલ Record_sanitize_closure_t * = nullptr) અચલ
  {
    TRACE_SANITIZE (આ);
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
સંરચના JstfScript
{
  unsigned પૂર્ણાંક get_lang_sys_ગણતરી () અચલ
  { પાછા langSys.len; }
  અચલ Tag& get_lang_sys_tag (unsigned પૂર્ણાંક i) અચલ
  { પાછા langSys.get_tag (i); }
  unsigned પૂર્ણાંક get_lang_sys_tags (unsigned પૂર્ણાંક start_offset,
				  unsigned પૂર્ણાંક *lang_sys_ગણતરી /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) અચલ
  { પાછા langSys.get_tags (start_offset, lang_sys_ગણતરી, lang_sys_tags); }
  અચલ JstfLangSys& get_lang_sys (unsigned પૂર્ણાંક i) અચલ
  {
    જો (i == Index::NOT_FOUND_INDEX) પાછા get_default_lang_sys ();
    પાછા આ+langSys[i].offset;
  }
  તાર્કિક find_lang_sys_index (hb_tag_t tag, unsigned પૂર્ણાંક *index) અચલ
  { પાછા langSys.find_index (tag, index); }

  તાર્કિક has_default_lang_sys () અચલ               { પાછા defaultLangSys != 0; }
  અચલ JstfLangSys& get_default_lang_sys () અચલ { પાછા આ+defaultLangSys; }

  તાર્કિક sanitize (hb_sanitize_context_t *c,
		 અચલ Record_sanitize_closure_t * = nullptr) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (extenderGlyphs.sanitize (c, આ) &&
		  defaultLangSys.sanitize (c, આ) &&
		  langSys.sanitize (c, આ));
  }

  સુરક્ષિત:
  typename Types::template LOffsetTo<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  typename Types::template LOffsetTo<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  RecordArrayOf<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 * Types::LOffset::static_size + 2, langSys);
};

template <typename Types>
સંરચના JstfScriptRecord
{
  પૂર્ણાંક cmp (hb_tag_t a) અચલ { પાછા tag.cmp (a); }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    અચલ Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (આ) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::template LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  જાહેર:
  DEFINE_SIZE_STATIC (4 + Types::LOffset::static_size);
};

template <typename Types>
સંરચના JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  અચલ Tag& get_tag (unsigned પૂર્ણાંક i) અચલ
  { પાછા (*આ)[i].tag; }
  unsigned પૂર્ણાંક get_tags (unsigned પૂર્ણાંક start_offset,
			 unsigned પૂર્ણાંક *script_ગણતરી /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) અચલ
  {
    જો (script_ગણતરી)
    {
      + આ->as_array ().sub_array (start_offset, script_ગણતરી)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_ગણતરી))
      ;
    }
    પાછા આ->len;
  }
  અચલ JstfScript<Types>& get_script (અચલ શૂન્ય *base, unsigned પૂર્ણાંક i) અચલ
  { પાછા base+(*આ)[i].offset; }
  તાર્કિક find_index (hb_tag_t tag, unsigned પૂર્ણાંક *index) અચલ
  {
    પાછા આ->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

સંરચના JSTF
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  unsigned પૂર્ણાંક get_script_ગણતરી () અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (has_script_list2 ()) પાછા get_script_list2 ().len;
#endif
    પાછા scriptList.len;
  }
  અચલ Tag& get_script_tag (unsigned પૂર્ણાંક i) અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (has_script_list2 ()) પાછા get_script_list2 ().get_tag (i);
#endif
    પાછા scriptList.get_tag (i);
  }
  unsigned પૂર્ણાંક get_script_tags (unsigned પૂર્ણાંક start_offset,
				unsigned પૂર્ણાંક *script_ગણતરી /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (has_script_list2 ())
      પાછા get_script_list2 ().get_tags (start_offset, script_ગણતરી, script_tags);
#endif
    પાછા scriptList.get_tags (start_offset, script_ગણતરી, script_tags);
  }
  template <typename Types>
  અચલ JstfScript<Types>& get_script (unsigned પૂર્ણાંક i) અચલ;
  તાર્કિક find_script_index (hb_tag_t tag, unsigned પૂર્ણાંક *index) અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (has_script_list2 ()) પાછા get_script_list2 ().find_index (tag, index);
#endif
    પાછા scriptList.find_index (tag, index);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, આ)))
      return_trace (ખોટું);
#ifndef HB_NO_BEYOND_64K
    જો (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, આ)))
      return_trace (ખોટું);
#endif
    return_trace (સાચું);
  }

  ખાનગી:
#ifndef HB_NO_BEYOND_64K
  તાર્કિક has_script_list2 () અચલ
  { પાછા version.minor >= 1 && get_script_list2 ().len; }
  અચલ JstfScriptList<Layout::MediumTypes>& get_script_list2 () અચલ
  { પાછા StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#endif

  સુરક્ષિત:
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
  જાહેર:
  DEFINE_SIZE_ARRAY (6, scriptList);
};

template <>
અંતઃરેખ અચલ JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (unsigned પૂર્ણાંક i) અચલ
{ પાછા scriptList.get_script (આ, i); }

#ifndef HB_NO_BEYOND_64K
template <>
અંતઃરેખ અચલ JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (unsigned પૂર્ણાંક i) અચલ
{ પાછા get_script_list2 ().get_script (આ, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
