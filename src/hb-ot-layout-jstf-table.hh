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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-gpos-table.hh"


ନାମାକାଶ OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

ପ୍ରକାରବ୍ୟାଖ୍ୟା IndexArray JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

ପ୍ରକାରବ୍ୟାଖ୍ୟା List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

ସଂରଚନା JstfPriority
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  shrinkageEnableGSUB.sanitize (c, ଏହି) &&
		  shrinkageDisableGSUB.sanitize (c, ଏହି) &&
		  shrinkageEnableGPOS.sanitize (c, ଏହି) &&
		  shrinkageDisableGPOS.sanitize (c, ଏହି) &&
		  shrinkageJstfMax.sanitize (c, ଏହି) &&
		  extensionEnableGSUB.sanitize (c, ଏହି) &&
		  extensionDisableGSUB.sanitize (c, ଏହି) &&
		  extensionEnableGPOS.sanitize (c, ଏହି) &&
		  extensionDisableGPOS.sanitize (c, ଏହି) &&
		  extensionJstfMax.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
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

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

ସଂରଚନା JstfLangSys : List16OfOffset16To<JstfPriority>
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c,
		 ଅଚଳ Record_sanitize_closure_t * = nullptr) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
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
ସଂରଚନା JstfScript
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_lang_sys_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ langSys.len; }
  ଅଚଳ Tag& get_lang_sys_tag (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ langSys.get_tag (i); }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_lang_sys_tags (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *lang_sys_ଗଣନା /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ଅଚଳ
  { ଫେରନ୍ତୁ langSys.get_tags (start_offset, lang_sys_ଗଣନା, lang_sys_tags); }
  ଅଚଳ JstfLangSys& get_lang_sys (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  {
    ଯଦି (i == Index::NOT_FOUND_INDEX) ଫେରନ୍ତୁ get_default_lang_sys ();
    ଫେରନ୍ତୁ ଏହି+langSys[i].offset;
  }
  ତାର୍କିକ find_lang_sys_index (hb_tag_t tag, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index) ଅଚଳ
  { ଫେରନ୍ତୁ langSys.find_index (tag, index); }

  ତାର୍କିକ has_default_lang_sys () ଅଚଳ               { ଫେରନ୍ତୁ defaultLangSys != 0; }
  ଅଚଳ JstfLangSys& get_default_lang_sys () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+defaultLangSys; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c,
		 ଅଚଳ Record_sanitize_closure_t * = nullptr) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (extenderGlyphs.sanitize (c, ଏହି) &&
		  defaultLangSys.sanitize (c, ଏହି) &&
		  langSys.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  typename Types::template LOffsetTo<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  typename Types::template LOffsetTo<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  RecordArrayOf<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 * Types::LOffset::static_size + 2, langSys);
};

template <typename Types>
ସଂରଚନା JstfScriptRecord
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_tag_t a) ଅଚଳ { ଫେରନ୍ତୁ tag.cmp (a); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଅଚଳ Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (ଏହି) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::template LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + Types::LOffset::static_size);
};

template <typename Types>
ସଂରଚନା JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  ଅଚଳ Tag& get_tag (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ (*ଏହି)[i].tag; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_tags (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
			 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *script_ଗଣନା /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) ଅଚଳ
  {
    ଯଦି (script_ଗଣନା)
    {
      + ଏହି->as_array ().sub_array (start_offset, script_ଗଣନା)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_ଗଣନା))
      ;
    }
    ଫେରନ୍ତୁ ଏହି->len;
  }
  ଅଚଳ JstfScript<Types>& get_script (ଅଚଳ ଶୂନ୍ୟ *base, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ base+(*ଏହି)[i].offset; }
  ତାର୍କିକ find_index (hb_tag_t tag, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index) ଅଚଳ
  {
    ଫେରନ୍ତୁ ଏହି->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

ସଂରଚନା JSTF
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_script_ଗଣନା () ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (has_script_list2 ()) ଫେରନ୍ତୁ get_script_list2 ().len;
#endif
    ଫେରନ୍ତୁ scriptList.len;
  }
  ଅଚଳ Tag& get_script_tag (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (has_script_list2 ()) ଫେରନ୍ତୁ get_script_list2 ().get_tag (i);
#endif
    ଫେରନ୍ତୁ scriptList.get_tag (i);
  }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_script_tags (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_offset,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *script_ଗଣନା /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (has_script_list2 ())
      ଫେରନ୍ତୁ get_script_list2 ().get_tags (start_offset, script_ଗଣନା, script_tags);
#endif
    ଫେରନ୍ତୁ scriptList.get_tags (start_offset, script_ଗଣନା, script_tags);
  }
  template <typename Types>
  ଅଚଳ JstfScript<Types>& get_script (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ;
  ତାର୍କିକ find_script_index (hb_tag_t tag, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index) ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (has_script_list2 ()) ଫେରନ୍ତୁ get_script_list2 ().find_index (tag, index);
#endif
    ଫେରନ୍ତୁ scriptList.find_index (tag, index);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, ଏହି)))
      return_trace (ମିଥ୍ୟା);
#ifndef HB_NO_BEYOND_64K
    ଯଦି (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, ଏହି)))
      return_trace (ମିଥ୍ୟା);
#endif
    return_trace (ସତ୍ୟ);
  }

  ବ୍ୟକ୍ତିଗତ:
#ifndef HB_NO_BEYOND_64K
  ତାର୍କିକ has_script_list2 () ଅଚଳ
  { ଫେରନ୍ତୁ version.minor >= 1 && get_script_list2 ().len; }
  ଅଚଳ JstfScriptList<Layout::MediumTypes>& get_script_list2 () ଅଚଳ
  { ଫେରନ୍ତୁ StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#endif

  ସୁରକ୍ଷିତ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (6, scriptList);
};

template <>
ଧାଡ଼ିଭିତରେ ଅଚଳ JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{ ଫେରନ୍ତୁ scriptList.get_script (ଏହି, i); }

#ifndef HB_NO_BEYOND_64K
template <>
ଧାଡ଼ିଭିତରେ ଅଚଳ JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{ ଫେରନ୍ତୁ get_script_list2 ().get_script (ଏହି, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
