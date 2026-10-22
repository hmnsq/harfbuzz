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

प्रकारव्याख्या IndexArray JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

प्रकारव्याख्या List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

संरचना JstfPriority
{
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  shrinkageEnableGSUB.sanitize (c, हे) &&
		  shrinkageDisableGSUB.sanitize (c, हे) &&
		  shrinkageEnableGPOS.sanitize (c, हे) &&
		  shrinkageDisableGPOS.sanitize (c, हे) &&
		  shrinkageJstfMax.sanitize (c, हे) &&
		  extensionEnableGSUB.sanitize (c, हे) &&
		  extensionDisableGSUB.sanitize (c, हे) &&
		  extensionEnableGPOS.sanitize (c, हे) &&
		  extensionDisableGPOS.sanitize (c, हे) &&
		  extensionJstfMax.sanitize (c, हे));
  }

  संरक्षित:
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
		 स्थिरांक Record_sanitize_closure_t * = nullptr) स्थिरांक
  {
    TRACE_SANITIZE (हे);
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
  अचिन्हित पूर्णांक get_lang_sys_मोजणी () स्थिरांक
  { परत langSys.len; }
  स्थिरांक Tag& get_lang_sys_tag (अचिन्हित पूर्णांक i) स्थिरांक
  { परत langSys.get_tag (i); }
  अचिन्हित पूर्णांक get_lang_sys_tags (अचिन्हित पूर्णांक start_offset,
				  अचिन्हित पूर्णांक *lang_sys_मोजणी /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) स्थिरांक
  { परत langSys.get_tags (start_offset, lang_sys_मोजणी, lang_sys_tags); }
  स्थिरांक JstfLangSys& get_lang_sys (अचिन्हित पूर्णांक i) स्थिरांक
  {
    जर (i == Index::NOT_FOUND_INDEX) परत get_default_lang_sys ();
    परत हे+langSys[i].offset;
  }
  तार्किक find_lang_sys_index (hb_tag_t tag, अचिन्हित पूर्णांक *index) स्थिरांक
  { परत langSys.find_index (tag, index); }

  तार्किक has_default_lang_sys () स्थिरांक               { परत defaultLangSys != 0; }
  स्थिरांक JstfLangSys& get_default_lang_sys () स्थिरांक { परत हे+defaultLangSys; }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 स्थिरांक Record_sanitize_closure_t * = nullptr) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (extenderGlyphs.sanitize (c, हे) &&
		  defaultLangSys.sanitize (c, हे) &&
		  langSys.sanitize (c, हे));
  }

  संरक्षित:
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
  पूर्णांक cmp (hb_tag_t a) स्थिरांक { परत tag.cmp (a); }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    स्थिरांक Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (हे) &&
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
  स्थिरांक Tag& get_tag (अचिन्हित पूर्णांक i) स्थिरांक
  { परत (*हे)[i].tag; }
  अचिन्हित पूर्णांक get_tags (अचिन्हित पूर्णांक start_offset,
			 अचिन्हित पूर्णांक *script_मोजणी /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) स्थिरांक
  {
    जर (script_मोजणी)
    {
      + हे->as_array ().sub_array (start_offset, script_मोजणी)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_मोजणी))
      ;
    }
    परत हे->len;
  }
  स्थिरांक JstfScript<Types>& get_script (स्थिरांक शून्य *base, अचिन्हित पूर्णांक i) स्थिरांक
  { परत base+(*हे)[i].offset; }
  तार्किक find_index (hb_tag_t tag, अचिन्हित पूर्णांक *index) स्थिरांक
  {
    परत हे->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

संरचना JSTF
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  अचिन्हित पूर्णांक get_script_मोजणी () स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (has_script_list2 ()) परत get_script_list2 ().len;
#endif
    परत scriptList.len;
  }
  स्थिरांक Tag& get_script_tag (अचिन्हित पूर्णांक i) स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (has_script_list2 ()) परत get_script_list2 ().get_tag (i);
#endif
    परत scriptList.get_tag (i);
  }
  अचिन्हित पूर्णांक get_script_tags (अचिन्हित पूर्णांक start_offset,
				अचिन्हित पूर्णांक *script_मोजणी /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (has_script_list2 ())
      परत get_script_list2 ().get_tags (start_offset, script_मोजणी, script_tags);
#endif
    परत scriptList.get_tags (start_offset, script_मोजणी, script_tags);
  }
  ढाँचा <typename Types>
  स्थिरांक JstfScript<Types>& get_script (अचिन्हित पूर्णांक i) स्थिरांक;
  तार्किक find_script_index (hb_tag_t tag, अचिन्हित पूर्णांक *index) स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (has_script_list2 ()) परत get_script_list2 ().find_index (tag, index);
#endif
    परत scriptList.find_index (tag, index);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, हे)))
      return_trace (असत्य);
#ifndef HB_NO_BEYOND_64K
    जर (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, हे)))
      return_trace (असत्य);
#endif
    return_trace (सत्य);
  }

  खाजगी:
#ifndef HB_NO_BEYOND_64K
  तार्किक has_script_list2 () स्थिरांक
  { परत version.minor >= 1 && get_script_list2 ().len; }
  स्थिरांक JstfScriptList<Layout::MediumTypes>& get_script_list2 () स्थिरांक
  { परत StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#endif

  संरक्षित:
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
ओळीत स्थिरांक JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (अचिन्हित पूर्णांक i) स्थिरांक
{ परत scriptList.get_script (हे, i); }

#ifndef HB_NO_BEYOND_64K
ढाँचा <>
ओळीत स्थिरांक JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (अचिन्हित पूर्णांक i) स्थिरांक
{ परत get_script_list2 ().get_script (हे, i); }
#endif


} /* namespace OT */


#endif /* HB_OT_LAYOUT_JSTF_TABLE_HH */
