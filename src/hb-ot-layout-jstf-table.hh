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

#अगर_अघोषित HB_OT_LAYOUT_JSTF_TABLE_HH
#घोषणा HB_OT_LAYOUT_JSTF_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-layout-gpos-table.hh"


namespace OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

प्रकार सूचकांक_Array JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

प्रकार List16OfOffset16To<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

काष्ठा JstfPriority
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  shrinkageEnableGSUB.sanitize (c, यह) &&
		  shrinkageDisableGSUB.sanitize (c, यह) &&
		  shrinkageEnableGPOS.sanitize (c, यह) &&
		  shrinkageDisableGPOS.sanitize (c, यह) &&
		  shrinkageJstfMax.sanitize (c, यह) &&
		  extensionEnableGSUB.sanitize (c, यह) &&
		  extensionDisableGSUB.sanitize (c, यह) &&
		  extensionEnableGPOS.sanitize (c, यह) &&
		  extensionDisableGPOS.sanitize (c, यह) &&
		  extensionJstfMax.sanitize (c, यह));
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

  खुला:
  DEFINE_आकार_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

काष्ठा JstfLangSys : List16OfOffset16To<JstfPriority>
{
  bool sanitize (hb_sanitize_context_t *c,
		 स्थिर Record_sanitize_closure_t * = nullptr) स्थिर
  {
    TRACE_SANITIZE (यह);
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
काष्ठा JstfScript
{
  अचिन्हित पूर्णांक get_lang_sys_गिनती () स्थिर
  { वापस langSys.len; }
  स्थिर Tag& get_lang_sys_tag (अचिन्हित पूर्णांक i) स्थिर
  { वापस langSys.get_tag (i); }
  अचिन्हित पूर्णांक get_lang_sys_tags (अचिन्हित पूर्णांक start_offset,
				  अचिन्हित पूर्णांक *lang_sys_गिनती /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) स्थिर
  { वापस langSys.get_tags (start_offset, lang_sys_गिनती, lang_sys_tags); }
  स्थिर JstfLangSys& get_lang_sys (अचिन्हित पूर्णांक i) स्थिर
  {
    अगर (i == सूचकांक::NOT_FOUND_सूचकांक) वापस get_default_lang_sys ();
    वापस यह+langSys[i].offset;
  }
  bool find_lang_sys_सूचकांक (hb_tag_t tag, अचिन्हित पूर्णांक *सूचकांक_2) स्थिर
  { वापस langSys.find_सूचकांक (tag, सूचकांक_2); }

  bool has_default_lang_sys () स्थिर               { वापस defaultLangSys != 0; }
  स्थिर JstfLangSys& get_default_lang_sys () स्थिर { वापस यह+defaultLangSys; }

  bool sanitize (hb_sanitize_context_t *c,
		 स्थिर Record_sanitize_closure_t * = nullptr) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (extenderGlyphs.sanitize (c, यह) &&
		  defaultLangSys.sanitize (c, यह) &&
		  langSys.sanitize (c, यह));
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
  खुला:
  DEFINE_आकार_ARRAY (2 * Types::LOffset::static_आकार + 2, langSys);
};

ढाँचा <typename Types>
काष्ठा JstfScriptRecord
{
  पूर्णांक cmp (hb_tag_t a) स्थिर { वापस tag.cmp (a); }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    स्थिर Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (यह) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag		tag;		/* 4-byte JstfScript identification tag */
  typename Types::ढाँचा LOffsetTo<JstfScript<Types>>
		offset;		/* Offset from beginning of JSTF header */
  खुला:
  DEFINE_आकार_STATIC (4 + Types::LOffset::static_आकार);
};

ढाँचा <typename Types>
काष्ठा JstfScriptList : SortedArray16Of<JstfScriptRecord<Types>>
{
  स्थिर Tag& get_tag (अचिन्हित पूर्णांक i) स्थिर
  { वापस (*यह)[i].tag; }
  अचिन्हित पूर्णांक get_tags (अचिन्हित पूर्णांक start_offset,
			 अचिन्हित पूर्णांक *script_गिनती /* IN/OUT */,
			 hb_tag_t     *script_tags /* OUT */) स्थिर
  {
    अगर (script_गिनती)
    {
      + यह->as_array ().sub_array (start_offset, script_गिनती)
      | hb_map (&JstfScriptRecord<Types>::tag)
      | hb_sink (hb_array (script_tags, *script_गिनती))
      ;
    }
    वापस यह->len;
  }
  स्थिर JstfScript<Types>& get_script (स्थिर व्योम *base, अचिन्हित पूर्णांक i) स्थिर
  { वापस base+(*यह)[i].offset; }
  bool find_सूचकांक (hb_tag_t tag, अचिन्हित पूर्णांक *सूचकांक_2) स्थिर
  {
    वापस यह->bfind (tag, सूचकांक_2, HB_NOT_FOUND_STORE, सूचकांक::NOT_FOUND_सूचकांक);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

काष्ठा JSTF
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_JSTF;

  अचिन्हित पूर्णांक get_script_गिनती () स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (has_script_list2 ()) वापस get_script_list2 ().len;
#पूर्ण_अगर
    वापस scriptList.len;
  }
  स्थिर Tag& get_script_tag (अचिन्हित पूर्णांक i) स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (has_script_list2 ()) वापस get_script_list2 ().get_tag (i);
#पूर्ण_अगर
    वापस scriptList.get_tag (i);
  }
  अचिन्हित पूर्णांक get_script_tags (अचिन्हित पूर्णांक start_offset,
				अचिन्हित पूर्णांक *script_गिनती /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (has_script_list2 ())
      वापस get_script_list2 ().get_tags (start_offset, script_गिनती, script_tags);
#पूर्ण_अगर
    वापस scriptList.get_tags (start_offset, script_गिनती, script_tags);
  }
  ढाँचा <typename Types>
  स्थिर JstfScript<Types>& get_script (अचिन्हित पूर्णांक i) स्थिर;
  bool find_script_सूचकांक (hb_tag_t tag, अचिन्हित पूर्णांक *सूचकांक_2) स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (has_script_list2 ()) वापस get_script_list2 ().find_सूचकांक (tag, सूचकांक_2);
#पूर्ण_अगर
    वापस scriptList.find_सूचकांक (tag, सूचकांक_2);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!version.sanitize (c) ||
		  !hb_barrier () ||
		  version.major != 1 ||
		  !scriptList.sanitize (c, यह)))
      return_trace (false);
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (unlikely (version.minor >= 1 && !get_script_list2 ().sanitize (c, यह)))
      return_trace (false);
#पूर्ण_अगर
    return_trace (true);
  }

  निजी:
#अगर_अघोषित HB_NO_BEYOND_64K
  bool has_script_list2 () स्थिर
  { वापस version.minor >= 1 && get_script_list2 ().len; }
  स्थिर JstfScriptList<Layout::MediumTypes>& get_script_list2 () स्थिर
  { वापस StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#पूर्ण_अगर

  रक्षित:
  FixedVersion<>version;	/* Version of the JSTF table--initially set
				 * to 0x00010000u */
  JstfScriptList<Layout::SmallTypes>
		scriptList;	/* Array of JstfScripts--listed
				 * alphabetically by ScriptTag */
#अगर_अघोषित HB_NO_BEYOND_64K
/*JstfScriptList<Layout::MediumTypes>
		scriptList2;*//* Array of JstfScript2s--listed
				 * alphabetically by ScriptTag.
				 * Present only if minorVersion is 1 or greater. */
#पूर्ण_अगर
  खुला:
  DEFINE_आकार_ARRAY (6, scriptList);
};

ढाँचा <>
अंतरभूत स्थिर JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (अचिन्हित पूर्णांक i) स्थिर
{ वापस scriptList.get_script (यह, i); }

#अगर_अघोषित HB_NO_BEYOND_64K
ढाँचा <>
अंतरभूत स्थिर JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (अचिन्हित पूर्णांक i) स्थिर
{ वापस get_script_list2 ().get_script (यह, i); }
#पूर्ण_अगर


} /* namespace OT */


#पूर्ण_अगर /* HB_OT_LAYOUT_JSTF_TABLE_HH */
