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
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
		 अचर Record_sanitize_closure_t * = nullptr) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (List16OfOffset16To<JstfPriority>::sanitize (c));
  }
};


/*
 * ExtenderGlyphs -- Extender Glyph Table
 */

ढाँचा <typename Types>
using ExtenderGlyphs = SortedArray16Of<typename Types::HBGlyphID>;


/*
 * JstfScript -- The Justification Table
 */

ढाँचा <typename Types>
संरचना JstfScript
{
  अचिन्हित पूर्णाङ्क get_lang_sys_गन्ती () अचर
  { फर्काऊ langSys.len; }
  अचर Tag& get_lang_sys_tag (अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ langSys.get_tag (i); }
  अचिन्हित पूर्णाङ्क get_lang_sys_tags (अचिन्हित पूर्णाङ्क start_offset,
				  अचिन्हित पूर्णाङ्क *lang_sys_गन्ती /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) अचर
  { फर्काऊ langSys.get_tags (start_offset, lang_sys_गन्ती, lang_sys_tags); }
  अचर JstfLangSys& get_lang_sys (अचिन्हित पूर्णाङ्क i) अचर
  {
    यदि (i == Index::NOT_FOUND_INDEX) फर्काऊ get_default_lang_sys ();
    फर्काऊ एतत्+langSys[i].offset;
  }
  तार्किक find_lang_sys_index (hb_tag_t tag, अचिन्हित पूर्णाङ्क *index) अचर
  { फर्काऊ langSys.find_index (tag, index); }

  तार्किक has_default_lang_sys () अचर               { फर्काऊ defaultLangSys != 0; }
  अचर JstfLangSys& get_default_lang_sys () अचर { फर्काऊ एतत्+defaultLangSys; }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 अचर Record_sanitize_closure_t * = nullptr) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (extenderGlyphs.sanitize (c, एतत्) &&
		  defaultLangSys.sanitize (c, एतत्) &&
		  langSys.sanitize (c, एतत्));
  }

  रक्षित:
  typename Types::ढाँचा LOffsetTo<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  typename Types::ढाँचा LOffsetTo<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  RecordArrayOf<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 * Types::LOffset::static_size + 2, langSys);
};

ढाँचा <typename Types>
संरचना JstfScriptRecord
{
  पूर्णाङ्क cmp (hb_tag_t a) अचर { फर्काऊ tag.cmp (a); }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    अचर Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (एतत्) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::ढाँचा LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + Types::LOffset::static_size);
};

ढाँचा <typename Types>
संरचना JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  अचर Tag& get_tag (अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ (*एतत्)[i].tag; }
  अचिन्हित पूर्णाङ्क get_tags (अचिन्हित पूर्णाङ्क start_offset,
			 अचिन्हित पूर्णाङ्क *script_गन्ती /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) अचर
  {
    यदि (script_गन्ती)
    {
      + एतत्->as_array ().sub_array (start_offset, script_गन्ती)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_गन्ती))
      ;
    }
    फर्काऊ एतत्->len;
  }
  अचर JstfScript<Types>& get_script (अचर शून्य *base, अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ base+(*एतत्)[i].offset; }
  तार्किक find_index (hb_tag_t tag, अचिन्हित पूर्णाङ्क *index) अचर
  {
    फर्काऊ एतत्->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

संरचना JSTF
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  अचिन्हित पूर्णाङ्क get_script_गन्ती () अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (has_script_list2 ()) फर्काऊ get_script_list2 ().len;
#endif
    फर्काऊ scriptList.len;
  }
  अचर Tag& get_script_tag (अचिन्हित पूर्णाङ्क i) अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (has_script_list2 ()) फर्काऊ get_script_list2 ().get_tag (i);
#endif
    फर्काऊ scriptList.get_tag (i);
  }
  अचिन्हित पूर्णाङ्क get_script_tags (अचिन्हित पूर्णाङ्क start_offset,
				अचिन्हित पूर्णाङ्क *script_गन्ती /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (has_script_list2 ())
      फर्काऊ get_script_list2 ().get_tags (start_offset, script_गन्ती, script_tags);
#endif
    फर्काऊ scriptList.get_tags (start_offset, script_गन्ती, script_tags);
  }
  ढाँचा <typename Types>
  अचर JstfScript<Types>& get_script (अचिन्हित पूर्णाङ्क i) अचर;
  तार्किक find_script_index (hb_tag_t tag, अचिन्हित पूर्णाङ्क *index) अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (has_script_list2 ()) फर्काऊ get_script_list2 ().find_index (tag, index);
#endif
    फर्काऊ scriptList.find_index (tag, index);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, एतत्)))
      return_trace (असत्य);
#ifndef HB_NO_BEYOND_64K
    यदि (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, एतत्)))
      return_trace (असत्य);
#endif
    return_trace (सत्य);
  }

  गुप्त:
#ifndef HB_NO_BEYOND_64K
  तार्किक has_script_list2 () अचर
  { फर्काऊ version.minor >= 1 && get_script_list2 ().len; }
  अचर JstfScriptList<Layout::MediumTypes>& get_script_list2 () अचर
  { फर्काऊ StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
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

ढाँचा <>
अन्तःस्थ अचर JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (अचिन्हित पूर्णाङ्क i) अचर
{ फर्काऊ scriptList.get_script (एतत्, i); }

#ifndef HB_NO_BEYOND_64K
ढाँचा <>
अन्तःस्थ अचर JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (अचिन्हित पूर्णाङ्क i) अचर
{ फर्काऊ get_script_list2 ().get_script (एतत्, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
