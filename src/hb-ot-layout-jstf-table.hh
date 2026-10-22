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

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gpos-table.hh"


ನಾಮಾಕಾಶ OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

ಪ್ರಕಾರನಿರ್ದೇಶ IndexArray JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

ಪ್ರಕಾರನಿರ್ದೇಶ List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

ಸಂರಚನಾ JstfPriority
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  shrinkageEnableGSUB.sanitize (c, ಏತತ್) &&
		  shrinkageDisableGSUB.sanitize (c, ಏತತ್) &&
		  shrinkageEnableGPOS.sanitize (c, ಏತತ್) &&
		  shrinkageDisableGPOS.sanitize (c, ಏತತ್) &&
		  shrinkageJstfMax.sanitize (c, ಏತತ್) &&
		  extensionEnableGSUB.sanitize (c, ಏತತ್) &&
		  extensionDisableGSUB.sanitize (c, ಏತತ್) &&
		  extensionEnableGPOS.sanitize (c, ಏತತ್) &&
		  extensionDisableGPOS.sanitize (c, ಏತತ್) &&
		  extensionJstfMax.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
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

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

ಸಂರಚನಾ JstfLangSys : List16OfOffset16To<JstfPriority>
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c,
		 ಧ್ರುವ Record_sanitize_closure_t * = nullptr) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
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
ಸಂರಚನಾ JstfScript
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_lang_sys_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ langSys.len; }
  ಧ್ರುವ Tag& get_lang_sys_tag (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ langSys.get_tag (i); }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_lang_sys_tags (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
				  unsigned ಪೂರ್ಣಾಙ್ಕ *lang_sys_ಎಣಿಕೆ /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ langSys.get_tags (start_offset, lang_sys_ಎಣಿಕೆ, lang_sys_tags); }
  ಧ್ರುವ JstfLangSys& get_lang_sys (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  {
    ಯದಿ (i == Index::NOT_FOUND_INDEX) ಪ್ರತ್ಯಾಗಮ get_default_lang_sys ();
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+langSys[i].offset;
  }
  ತಾರ್ಕಿಕ find_lang_sys_index (hb_tag_t tag, unsigned ಪೂರ್ಣಾಙ್ಕ *index) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ langSys.find_index (tag, index); }

  ತಾರ್ಕಿಕ has_default_lang_sys () ಧ್ರುವ               { ಪ್ರತ್ಯಾಗಮ defaultLangSys != 0; }
  ಧ್ರುವ JstfLangSys& get_default_lang_sys () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+defaultLangSys; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c,
		 ಧ್ರುವ Record_sanitize_closure_t * = nullptr) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (extenderGlyphs.sanitize (c, ಏತತ್) &&
		  defaultLangSys.sanitize (c, ಏತತ್) &&
		  langSys.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  typename Types::template LOffsetTo<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  typename Types::template LOffsetTo<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  RecordArrayOf<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 * Types::LOffset::static_size + 2, langSys);
};

template <typename Types>
ಸಂರಚನಾ JstfScriptRecord
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_tag_t a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tag.cmp (a); }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಧ್ರುವ Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (ಏತತ್) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::template LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + Types::LOffset::static_size);
};

template <typename Types>
ಸಂರಚನಾ JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  ಧ್ರುವ Tag& get_tag (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)[i].tag; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_tags (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
			 unsigned ಪೂರ್ಣಾಙ್ಕ *script_ಎಣಿಕೆ /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (script_ಎಣಿಕೆ)
    {
      + ಏತತ್->as_array ().sub_array (start_offset, script_ಎಣಿಕೆ)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_ಎಣಿಕೆ))
      ;
    }
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->len;
  }
  ಧ್ರುವ JstfScript<Types>& get_script (ಧ್ರುವ ಶೂನ್ಯ *base, unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ base+(*ಏತತ್)[i].offset; }
  ತಾರ್ಕಿಕ find_index (hb_tag_t tag, unsigned ಪೂರ್ಣಾಙ್ಕ *index) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

ಸಂರಚನಾ JSTF
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  unsigned ಪೂರ್ಣಾಙ್ಕ get_script_ಎಣಿಕೆ () ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (has_script_list2 ()) ಪ್ರತ್ಯಾಗಮ get_script_list2 ().len;
#endif
    ಪ್ರತ್ಯಾಗಮ scriptList.len;
  }
  ಧ್ರುವ Tag& get_script_tag (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (has_script_list2 ()) ಪ್ರತ್ಯಾಗಮ get_script_list2 ().get_tag (i);
#endif
    ಪ್ರತ್ಯಾಗಮ scriptList.get_tag (i);
  }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_script_tags (unsigned ಪೂರ್ಣಾಙ್ಕ start_offset,
				unsigned ಪೂರ್ಣಾಙ್ಕ *script_ಎಣಿಕೆ /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (has_script_list2 ())
      ಪ್ರತ್ಯಾಗಮ get_script_list2 ().get_tags (start_offset, script_ಎಣಿಕೆ, script_tags);
#endif
    ಪ್ರತ್ಯಾಗಮ scriptList.get_tags (start_offset, script_ಎಣಿಕೆ, script_tags);
  }
  template <typename Types>
  ಧ್ರುವ JstfScript<Types>& get_script (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ;
  ತಾರ್ಕಿಕ find_script_index (hb_tag_t tag, unsigned ಪೂರ್ಣಾಙ್ಕ *index) ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (has_script_list2 ()) ಪ್ರತ್ಯಾಗಮ get_script_list2 ().find_index (tag, index);
#endif
    ಪ್ರತ್ಯಾಗಮ scriptList.find_index (tag, index);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, ಏತತ್)))
      return_trace (ಅಸತ್ಯ);
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, ಏತತ್)))
      return_trace (ಅಸತ್ಯ);
#endif
    return_trace (ಸತ್ಯ);
  }

  ಗುಪ್ತ:
#ifndef HB_NO_BEYOND_64K
  ತಾರ್ಕಿಕ has_script_list2 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ version.minor >= 1 && get_script_list2 ().len; }
  ಧ್ರುವ JstfScriptList<Layout::MediumTypes>& get_script_list2 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#endif

  ರಕ್ಷಿತ:
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (6, scriptList);
};

template <>
ಅನ್ತಃಸ್ಥ ಧ್ರುವ JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ scriptList.get_script (ಏತತ್, i); }

#ifndef HB_NO_BEYOND_64K
template <>
ಅನ್ತಃಸ್ಥ ಧ್ರುವ JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ get_script_list2 ().get_script (ಏತತ್, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
