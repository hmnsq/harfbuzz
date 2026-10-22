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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_LAYOUT_JSTF_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_JSTF_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gpos-table.hh"


ਨਾਮਾਕਾਸ਼ OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਸੂਚਕਾਂਕ_Array JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ List16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

ਬਣਤਰ Jstf_ਤਰਜੀਹ
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  shrinkageEnableGSUB.sanitize (c, ਇਹ) &&
		  shrinkageDisableGSUB.sanitize (c, ਇਹ) &&
		  shrinkageEnableGPOS.sanitize (c, ਇਹ) &&
		  shrinkageDisableGPOS.sanitize (c, ਇਹ) &&
		  shrinkageJstfMax.sanitize (c, ਇਹ) &&
		  extensionEnableGSUB.sanitize (c, ਇਹ) &&
		  extensionDisableGSUB.sanitize (c, ਇਹ) &&
		  extensionEnableGPOS.sanitize (c, ਇਹ) &&
		  extensionDisableGPOS.sanitize (c, ਇਹ) &&
		  extensionJstfMax.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Offset16_ਮੰਜ਼ਿਲ<JstfModList>
		shrinkageEnableGSUB;	/* Offset to Shrinkage Enable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_ਮੰਜ਼ਿਲ<JstfModList>
		shrinkageDisableGSUB;	/* Offset to Shrinkage Disable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_ਮੰਜ਼ਿਲ<JstfModList>
		shrinkageEnableGPOS;	/* Offset to Shrinkage Enable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_ਮੰਜ਼ਿਲ<JstfModList>
		shrinkageDisableGPOS;	/* Offset to Shrinkage Disable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_ਮੰਜ਼ਿਲ<JstfMax>
		shrinkageJstfMax;	/* Offset to Shrinkage JstfMax table--
					 * from beginning of JstfPriority table
					 * --may be NULL */
  Offset16_ਮੰਜ਼ਿਲ<JstfModList>
		extensionEnableGSUB;	/* Offset to Extension Enable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_ਮੰਜ਼ਿਲ<JstfModList>
		extensionDisableGSUB;	/* Offset to Extension Disable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_ਮੰਜ਼ਿਲ<JstfModList>
		extensionEnableGPOS;	/* Offset to Extension Enable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_ਮੰਜ਼ਿਲ<JstfModList>
		extensionDisableGPOS;	/* Offset to Extension Disable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_ਮੰਜ਼ਿਲ<JstfMax>
		extensionJstfMax;	/* Offset to Extension JstfMax table--
					 * from beginning of JstfPriority table
					 * --may be NULL */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

ਬਣਤਰ JstfLangSys : List16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<Jstf_ਤਰਜੀਹ>
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c,
		 ਅਚਲ ਰਿਕਾਰਡ_sanitize_ਘੇਰਾ_t * = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (List16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<Jstf_ਤਰਜੀਹ>::sanitize (c));
  }
};


/*
 * ExtenderGlyphs -- Extender Glyph Table
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਵਰਤੋਂ ExtenderGlyphs = Sorted_Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBGlyphID>;


/*
 * JstfScript -- The Justification Table
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ JstfScript
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_lang_sys_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ langSys.len; }
  ਅਚਲ ਟੈਗ& get_lang_sys_ਟੈਗ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ langSys.get_ਟੈਗ (i); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_lang_sys_ਟੈਗ_ਸੂਚੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *lang_sys_ਗਿਣਤੀ /* IN/OUT */,
				  hb_ਟੈਗ_t     *lang_sys_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { ਵਾਪਸ langSys.get_ਟੈਗ_ਸੂਚੀ (ਸ਼ੁਰੂ_ਵਿੱਥ, lang_sys_ਗਿਣਤੀ, lang_sys_ਟੈਗ_ਸੂਚੀ); }
  ਅਚਲ JstfLangSys& get_lang_sys (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  {
    ਜੇ (i == ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ) ਵਾਪਸ get_default_lang_sys ();
    ਵਾਪਸ ਇਹ+langSys[i].ਵਿੱਥ;
  }
  ਤਾਰਕਿਕ ਲੱਭੋ_lang_sys_ਸੂਚਕਾਂਕ (hb_ਟੈਗ_t ਟੈਗ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ_2) ਅਚਲ
  { ਵਾਪਸ langSys.ਲੱਭੋ_ਸੂਚਕਾਂਕ (ਟੈਗ_2, ਸੂਚਕਾਂਕ_2); }

  ਤਾਰਕਿਕ has_default_lang_sys () ਅਚਲ               { ਵਾਪਸ defaultLangSys != 0; }
  ਅਚਲ JstfLangSys& get_default_lang_sys () ਅਚਲ { ਵਾਪਸ ਇਹ+defaultLangSys; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c,
		 ਅਚਲ ਰਿਕਾਰਡ_sanitize_ਘੇਰਾ_t * = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (extenderGlyphs.sanitize (c, ਇਹ) &&
		  defaultLangSys.sanitize (c, ਇਹ) &&
		  langSys.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  ਰਿਕਾਰਡ_Array_ਦਾ<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 * Types::LOffset::static_ਮਾਪ + 2, langSys);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Jstf_Script_ਰਿਕਾਰਡ
{
  ਪੂਰਨ cmp (hb_ਟੈਗ_t a) ਅਚਲ { ਵਾਪਸ ਟੈਗ_2.cmp (a); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਅਚਲ ਰਿਕਾਰਡ_sanitize_ਘੇਰਾ_t ਘੇਰਾ = {ਟੈਗ_2, ਅਧਾਰ};
    return_trace (c->check_struct (ਇਹ) &&
		  ਵਿੱਥ.sanitize (c, ਅਧਾਰ, &ਘੇਰਾ));
  }

  ਟੈਗ		ਟੈਗ_2;		/* 4-byte JstfScript identification tag */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<JstfScript<Types>>
		ਵਿੱਥ;		/* Offset from beginning of JSTF header */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + Types::LOffset::static_ਮਾਪ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ JstfScriptList : Sorted_Array16_ਦਾ<Jstf_Script_ਰਿਕਾਰਡ<Types>>
{
  ਅਚਲ ਟੈਗ& get_ਟੈਗ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ (*ਇਹ)[i].ਟੈਗ_2; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਟੈਗ_ਸੂਚੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *script_ਗਿਣਤੀ /* IN/OUT */,
			 hb_ਟੈਗ_t     *script_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (script_ਗਿਣਤੀ)
    {
      + ਇਹ->as_array ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, script_ਗਿਣਤੀ)
      | hb_map (&Jstf_Script_ਰਿਕਾਰਡ<Types>::ਟੈਗ_2)
      | hb_sink (hb_array (script_ਟੈਗ_ਸੂਚੀ, *script_ਗਿਣਤੀ))
      ;
    }
    ਵਾਪਸ ਇਹ->len;
  }
  ਅਚਲ JstfScript<Types>& get_script (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ ਅਧਾਰ+(*ਇਹ)[i].ਵਿੱਥ; }
  ਤਾਰਕਿਕ ਲੱਭੋ_ਸੂਚਕਾਂਕ (hb_ਟੈਗ_t ਟੈਗ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ_2) ਅਚਲ
  {
    ਵਾਪਸ ਇਹ->bfind (ਟੈਗ_2, ਸੂਚਕਾਂਕ_2, HB_NOT_ਮਿਲਿਆ_STORE, ਸੂਚਕਾਂਕ::NOT_ਮਿਲਿਆ_ਸੂਚਕਾਂਕ);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

ਬਣਤਰ JSTF
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_JSTF;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_script_ਗਿਣਤੀ () ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (has_script_list2 ()) ਵਾਪਸ get_script_list2 ().len;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ scriptList.len;
  }
  ਅਚਲ ਟੈਗ& get_script_ਟੈਗ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (has_script_list2 ()) ਵਾਪਸ get_script_list2 ().get_ਟੈਗ (i);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ scriptList.get_ਟੈਗ (i);
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_script_ਟੈਗ_ਸੂਚੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *script_ਗਿਣਤੀ /* IN/OUT */,
				hb_ਟੈਗ_t     *script_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (has_script_list2 ())
      ਵਾਪਸ get_script_list2 ().get_ਟੈਗ_ਸੂਚੀ (ਸ਼ੁਰੂ_ਵਿੱਥ, script_ਗਿਣਤੀ, script_ਟੈਗ_ਸੂਚੀ);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ scriptList.get_ਟੈਗ_ਸੂਚੀ (ਸ਼ੁਰੂ_ਵਿੱਥ, script_ਗਿਣਤੀ, script_ਟੈਗ_ਸੂਚੀ);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
  ਅਚਲ JstfScript<Types>& get_script (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ;
  ਤਾਰਕਿਕ ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (hb_ਟੈਗ_t ਟੈਗ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ_2) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (has_script_list2 ()) ਵਾਪਸ get_script_list2 ().ਲੱਭੋ_ਸੂਚਕਾਂਕ (ਟੈਗ_2, ਸੂਚਕਾਂਕ_2);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ scriptList.ਲੱਭੋ_ਸੂਚਕਾਂਕ (ਟੈਗ_2, ਸੂਚਕਾਂਕ_2);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!ਸੰਸਕਰਣ.sanitize (c) ||
		  !hb_barrier () ||
		  ਸੰਸਕਰਣ.major != 1 ||
		  !scriptList.sanitize (c, ਇਹ)))
      return_trace (ਝੂਠ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (unlikely (ਸੰਸਕਰਣ.minor >= 1 && !get_script_list2 ().sanitize (c, ਇਹ)))
      return_trace (ਝੂਠ);
#ਜੇ_ਅੰਤ
    return_trace (ਸੱਚ);
  }

  ਨਿਜੀ:
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਤਾਰਕਿਕ has_script_list2 () ਅਚਲ
  { ਵਾਪਸ ਸੰਸਕਰਣ.minor >= 1 && get_script_list2 ().len; }
  ਅਚਲ JstfScriptList<Layout::MediumTypes>& get_script_list2 () ਅਚਲ
  { ਵਾਪਸ StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#ਜੇ_ਅੰਤ

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the JSTF table--initially set
				 * to 0x00010000u */
  JstfScriptList<Layout::SmallTypes>
		scriptList;	/* Array of JstfScripts--listed
				 * alphabetically by ScriptTag */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
/*JstfScriptList<Layout::MediumTypes>
		scriptList2;*//* Array of JstfScript2s--listed
				 * alphabetically by ScriptTag.
				 * Present only if minorVersion is 1 or greater. */
#ਜੇ_ਅੰਤ
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (6, scriptList);
};

ਸਾਂਚਾ <>
ਸਤਰਅੰਦਰ ਅਚਲ JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
{ ਵਾਪਸ scriptList.get_script (ਇਹ, i); }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
ਸਾਂਚਾ <>
ਸਤਰਅੰਦਰ ਅਚਲ JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
{ ਵਾਪਸ get_script_list2 ().get_script (ਇਹ, i); }
#ਜੇ_ਅੰਤ


} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_LAYOUT_JSTF_TABLE_HH */
