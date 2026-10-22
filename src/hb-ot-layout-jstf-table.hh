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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-layout-gpos-table.hh"


नामाकाश OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

प्रकारनिर्देश IndexArray JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

प्रकारनिर्देश List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

संरचना JstfPriority
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  shrinkageEnableGSUB.sanitize (c, एतत्) &&
		  shrinkageDisableGSUB.sanitize (c, एतत्) &&
		  shrinkageEnableGPOS.sanitize (c, एतत्) &&
		  shrinkageDisableGPOS.sanitize (c, एतत्) &&
		  shrinkageJstfMax.sanitize (c, एतत्) &&
		  extensionEnableGSUB.sanitize (c, एतत्) &&
		  extensionDisableGSUB.sanitize (c, एतत्) &&
		  extensionEnableGPOS.sanitize (c, एतत्) &&
		  extensionDisableGPOS.sanitize (c, एतत्) &&
		  extensionJstfMax.sanitize (c, एतत्));
  }

  रक्षित:
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

  सार्वजनिक:
  DEFINE_SIZE_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

संरचना JstfLangSys : List16OfOffset16To<JstfPriority>
{
  तार्किक sanitize (hb_sanitize_context_t *c,
		 ध्रुव Record_sanitize_closure_t * = nullptr) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
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
संरचना JstfScript
{
  unsigned पूर्णाङ्क get_lang_sys_गणनम् () ध्रुव
  { पटिनिवत्त langSys.len; }
  ध्रुव Tag& get_lang_sys_tag (unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त langSys.get_tag (i); }
  unsigned पूर्णाङ्क get_lang_sys_tags (unsigned पूर्णाङ्क start_offset,
				  unsigned पूर्णाङ्क *lang_sys_गणनम् /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ध्रुव
  { पटिनिवत्त langSys.get_tags (start_offset, lang_sys_गणनम्, lang_sys_tags); }
  ध्रुव JstfLangSys& get_lang_sys (unsigned पूर्णाङ्क i) ध्रुव
  {
    सचे (i == Index::NOT_FOUND_INDEX) पटिनिवत्त get_default_lang_sys ();
    पटिनिवत्त एतत्+langSys[i].offset;
  }
  तार्किक find_lang_sys_index (hb_tag_t tag, unsigned पूर्णाङ्क *index) ध्रुव
  { पटिनिवत्त langSys.find_index (tag, index); }

  तार्किक has_default_lang_sys () ध्रुव               { पटिनिवत्त defaultLangSys != 0; }
  ध्रुव JstfLangSys& get_default_lang_sys () ध्रुव { पटिनिवत्त एतत्+defaultLangSys; }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 ध्रुव Record_sanitize_closure_t * = nullptr) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (extenderGlyphs.sanitize (c, एतत्) &&
		  defaultLangSys.sanitize (c, एतत्) &&
		  langSys.sanitize (c, एतत्));
  }

  रक्षित:
  typename Types::template LOffsetTo<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  typename Types::template LOffsetTo<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  RecordArrayOf<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 * Types::LOffset::static_size + 2, langSys);
};

template <typename Types>
संरचना JstfScriptRecord
{
  पूर्णाङ्क cmp (hb_tag_t a) ध्रुव { पटिनिवत्त tag.cmp (a); }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    ध्रुव Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (एतत्) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::template LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + Types::LOffset::static_size);
};

template <typename Types>
संरचना JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  ध्रुव Tag& get_tag (unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त (*एतत्)[i].tag; }
  unsigned पूर्णाङ्क get_tags (unsigned पूर्णाङ्क start_offset,
			 unsigned पूर्णाङ्क *script_गणनम् /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) ध्रुव
  {
    सचे (script_गणनम्)
    {
      + एतत्->as_array ().sub_array (start_offset, script_गणनम्)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_गणनम्))
      ;
    }
    पटिनिवत्त एतत्->len;
  }
  ध्रुव JstfScript<Types>& get_script (ध्रुव शून्य *base, unsigned पूर्णाङ्क i) ध्रुव
  { पटिनिवत्त base+(*एतत्)[i].offset; }
  तार्किक find_index (hb_tag_t tag, unsigned पूर्णाङ्क *index) ध्रुव
  {
    पटिनिवत्त एतत्->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

संरचना JSTF
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  unsigned पूर्णाङ्क get_script_गणनम् () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (has_script_list2 ()) पटिनिवत्त get_script_list2 ().len;
#endif
    पटिनिवत्त scriptList.len;
  }
  ध्रुव Tag& get_script_tag (unsigned पूर्णाङ्क i) ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (has_script_list2 ()) पटिनिवत्त get_script_list2 ().get_tag (i);
#endif
    पटिनिवत्त scriptList.get_tag (i);
  }
  unsigned पूर्णाङ्क get_script_tags (unsigned पूर्णाङ्क start_offset,
				unsigned पूर्णाङ्क *script_गणनम् /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (has_script_list2 ())
      पटिनिवत्त get_script_list2 ().get_tags (start_offset, script_गणनम्, script_tags);
#endif
    पटिनिवत्त scriptList.get_tags (start_offset, script_गणनम्, script_tags);
  }
  template <typename Types>
  ध्रुव JstfScript<Types>& get_script (unsigned पूर्णाङ्क i) ध्रुव;
  तार्किक find_script_index (hb_tag_t tag, unsigned पूर्णाङ्क *index) ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (has_script_list2 ()) पटिनिवत्त get_script_list2 ().find_index (tag, index);
#endif
    पटिनिवत्त scriptList.find_index (tag, index);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, एतत्)))
      return_trace (मिच्छा);
#ifndef HB_NO_BEYOND_64K
    सचे (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, एतत्)))
      return_trace (मिच्छा);
#endif
    return_trace (सच्च);
  }

  गुप्त:
#ifndef HB_NO_BEYOND_64K
  तार्किक has_script_list2 () ध्रुव
  { पटिनिवत्त version.minor >= 1 && get_script_list2 ().len; }
  ध्रुव JstfScriptList<Layout::MediumTypes>& get_script_list2 () ध्रुव
  { पटिनिवत्त StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#endif

  रक्षित:
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
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, scriptList);
};

template <>
अन्तःस्थ ध्रुव JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (unsigned पूर्णाङ्क i) ध्रुव
{ पटिनिवत्त scriptList.get_script (एतत्, i); }

#ifndef HB_NO_BEYOND_64K
template <>
अन्तःस्थ ध्रुव JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (unsigned पूर्णाङ्क i) ध्रुव
{ पटिनिवत्त get_script_list2 ().get_script (एतत्, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
