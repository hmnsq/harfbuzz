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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-layout-gpos-table.hh"


நாமாகாஶ OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

வகைவரை IndexArray JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

வகைவரை List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

கட்டமைப்பு JstfPriority
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  shrinkageEnableGSUB.sanitize (c, இது) &&
		  shrinkageDisableGSUB.sanitize (c, இது) &&
		  shrinkageEnableGPOS.sanitize (c, இது) &&
		  shrinkageDisableGPOS.sanitize (c, இது) &&
		  shrinkageJstfMax.sanitize (c, இது) &&
		  extensionEnableGSUB.sanitize (c, இது) &&
		  extensionDisableGSUB.sanitize (c, இது) &&
		  extensionEnableGPOS.sanitize (c, இது) &&
		  extensionDisableGPOS.sanitize (c, இது) &&
		  extensionJstfMax.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
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

  பொது:
  DEFINE_SIZE_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

கட்டமைப்பு JstfLangSys : List16OfOffset16To<JstfPriority>
{
  தருக்க sanitize (hb_sanitize_context_t *c,
		 மாறிலி Record_sanitize_closure_t * = nullptr) மாறிலி
  {
    TRACE_SANITIZE (இது);
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
கட்டமைப்பு JstfScript
{
  unsigned முழு get_lang_sys_மொத்தம் () மாறிலி
  { திரும்பு langSys.len; }
  மாறிலி Tag& get_lang_sys_tag (unsigned முழு i) மாறிலி
  { திரும்பு langSys.get_tag (i); }
  unsigned முழு get_lang_sys_tags (unsigned முழு start_offset,
				  unsigned முழு *lang_sys_மொத்தம் /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) மாறிலி
  { திரும்பு langSys.get_tags (start_offset, lang_sys_மொத்தம், lang_sys_tags); }
  மாறிலி JstfLangSys& get_lang_sys (unsigned முழு i) மாறிலி
  {
    எனில் (i == Index::NOT_FOUND_INDEX) திரும்பு get_default_lang_sys ();
    திரும்பு இது+langSys[i].offset;
  }
  தருக்க find_lang_sys_index (hb_tag_t tag, unsigned முழு *index) மாறிலி
  { திரும்பு langSys.find_index (tag, index); }

  தருக்க has_default_lang_sys () மாறிலி               { திரும்பு defaultLangSys != 0; }
  மாறிலி JstfLangSys& get_default_lang_sys () மாறிலி { திரும்பு இது+defaultLangSys; }

  தருக்க sanitize (hb_sanitize_context_t *c,
		 மாறிலி Record_sanitize_closure_t * = nullptr) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (extenderGlyphs.sanitize (c, இது) &&
		  defaultLangSys.sanitize (c, இது) &&
		  langSys.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  typename Types::template LOffsetTo<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  typename Types::template LOffsetTo<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  RecordArrayOf<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  பொது:
  DEFINE_SIZE_ARRAY (2 * Types::LOffset::static_size + 2, langSys);
};

template <typename Types>
கட்டமைப்பு JstfScriptRecord
{
  முழு cmp (hb_tag_t a) மாறிலி { திரும்பு tag.cmp (a); }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    மாறிலி Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (இது) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::template LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  பொது:
  DEFINE_SIZE_STATIC (4 + Types::LOffset::static_size);
};

template <typename Types>
கட்டமைப்பு JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  மாறிலி Tag& get_tag (unsigned முழு i) மாறிலி
  { திரும்பு (*இது)[i].tag; }
  unsigned முழு get_tags (unsigned முழு start_offset,
			 unsigned முழு *script_மொத்தம் /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) மாறிலி
  {
    எனில் (script_மொத்தம்)
    {
      + இது->as_array ().sub_array (start_offset, script_மொத்தம்)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_மொத்தம்))
      ;
    }
    திரும்பு இது->len;
  }
  மாறிலி JstfScript<Types>& get_script (மாறிலி வெறுமை *base, unsigned முழு i) மாறிலி
  { திரும்பு base+(*இது)[i].offset; }
  தருக்க find_index (hb_tag_t tag, unsigned முழு *index) மாறிலி
  {
    திரும்பு இது->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

கட்டமைப்பு JSTF
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  unsigned முழு get_script_மொத்தம் () மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (has_script_list2 ()) திரும்பு get_script_list2 ().len;
#endif
    திரும்பு scriptList.len;
  }
  மாறிலி Tag& get_script_tag (unsigned முழு i) மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (has_script_list2 ()) திரும்பு get_script_list2 ().get_tag (i);
#endif
    திரும்பு scriptList.get_tag (i);
  }
  unsigned முழு get_script_tags (unsigned முழு start_offset,
				unsigned முழு *script_மொத்தம் /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (has_script_list2 ())
      திரும்பு get_script_list2 ().get_tags (start_offset, script_மொத்தம், script_tags);
#endif
    திரும்பு scriptList.get_tags (start_offset, script_மொத்தம், script_tags);
  }
  template <typename Types>
  மாறிலி JstfScript<Types>& get_script (unsigned முழு i) மாறிலி;
  தருக்க find_script_index (hb_tag_t tag, unsigned முழு *index) மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (has_script_list2 ()) திரும்பு get_script_list2 ().find_index (tag, index);
#endif
    திரும்பு scriptList.find_index (tag, index);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, இது)))
      return_trace (பொய்);
#ifndef HB_NO_BEYOND_64K
    எனில் (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, இது)))
      return_trace (பொய்);
#endif
    return_trace (உண்மை);
  }

  தனிப்பட்ட:
#ifndef HB_NO_BEYOND_64K
  தருக்க has_script_list2 () மாறிலி
  { திரும்பு version.minor >= 1 && get_script_list2 ().len; }
  மாறிலி JstfScriptList<Layout::MediumTypes>& get_script_list2 () மாறிலி
  { திரும்பு StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#endif

  பாதுகாக்கப்பட்ட:
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
  பொது:
  DEFINE_SIZE_ARRAY (6, scriptList);
};

template <>
உள்வரி மாறிலி JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (unsigned முழு i) மாறிலி
{ திரும்பு scriptList.get_script (இது, i); }

#ifndef HB_NO_BEYOND_64K
template <>
உள்வரி மாறிலி JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (unsigned முழு i) மாறிலி
{ திரும்பு get_script_list2 ().get_script (இது, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
