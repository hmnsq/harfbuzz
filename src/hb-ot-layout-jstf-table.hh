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

#నిర్వచితం_కాకపోతే HB_OT_LAYOUT_JSTF_పట్టిక_HH
#నిర్వచించు HB_OT_LAYOUT_JSTF_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-layout-gpos-table.hh"


నామాకాశ OT {


/*
 * JstfModList -- Justification Modification List Tables
 */

రకనిర్వచనం సూచీ_Array JstfModList;


/*
 * JstfMax -- Justification Maximum Table
 */

రకనిర్వచనం List16_యొక్క_Offset16_గమ్యం<PosLookup> JstfMax;


/*
 * JstfPriority -- Justification Priority Table
 */

నిర్మాణం Jstf_ప్రాధాన్యత
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  shrinkageEnableGSUB.sanitize (c, ఇది) &&
		  shrinkageDisableGSUB.sanitize (c, ఇది) &&
		  shrinkageEnableGPOS.sanitize (c, ఇది) &&
		  shrinkageDisableGPOS.sanitize (c, ఇది) &&
		  shrinkageJstfMax.sanitize (c, ఇది) &&
		  extensionEnableGSUB.sanitize (c, ఇది) &&
		  extensionDisableGSUB.sanitize (c, ఇది) &&
		  extensionEnableGPOS.sanitize (c, ఇది) &&
		  extensionDisableGPOS.sanitize (c, ఇది) &&
		  extensionJstfMax.sanitize (c, ఇది));
  }

  రక్షిత:
  Offset16_గమ్యం<JstfModList>
		shrinkageEnableGSUB;	/* Offset to Shrinkage Enable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_గమ్యం<JstfModList>
		shrinkageDisableGSUB;	/* Offset to Shrinkage Disable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_గమ్యం<JstfModList>
		shrinkageEnableGPOS;	/* Offset to Shrinkage Enable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_గమ్యం<JstfModList>
		shrinkageDisableGPOS;	/* Offset to Shrinkage Disable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_గమ్యం<JstfMax>
		shrinkageJstfMax;	/* Offset to Shrinkage JstfMax table--
					 * from beginning of JstfPriority table
					 * --may be NULL */
  Offset16_గమ్యం<JstfModList>
		extensionEnableGSUB;	/* Offset to Extension Enable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_గమ్యం<JstfModList>
		extensionDisableGSUB;	/* Offset to Extension Disable GSUB
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_గమ్యం<JstfModList>
		extensionEnableGPOS;	/* Offset to Extension Enable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_గమ్యం<JstfModList>
		extensionDisableGPOS;	/* Offset to Extension Disable GPOS
					 * JstfModList table--from beginning of
					 * JstfPriority table--may be NULL */
  Offset16_గమ్యం<JstfMax>
		extensionJstfMax;	/* Offset to Extension JstfMax table--
					 * from beginning of JstfPriority table
					 * --may be NULL */

  బహిరంగ:
  DEFINE_కొలత_STATIC (20);
};


/*
 * JstfLangSys -- Justification Language System Table
 */

నిర్మాణం JstfLangSys : List16_యొక్క_Offset16_గమ్యం<Jstf_ప్రాధాన్యత>
{
  తార్కిక sanitize (hb_sanitize_context_t *c,
		 స్థిరరాశి రికార్డు_sanitize_సంవృతం_t * = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (List16_యొక్క_Offset16_గమ్యం<Jstf_ప్రాధాన్యత>::sanitize (c));
  }
};


/*
 * ExtenderGlyphs -- Extender Glyph Table
 */

మూస <రకం_పేరు Types>
ఉపయోగించి ExtenderGlyphs = Sorted_Array16_యొక్క<రకం_పేరు Types::HBGlyphID>;


/*
 * JstfScript -- The Justification Table
 */

మూస <రకం_పేరు Types>
నిర్మాణం JstfScript
{
  గుర్తులేని పూర్ణాంకం get_lang_sys_లెక్క () స్థిరరాశి
  { తిరిగి langSys.len; }
  స్థిరరాశి ట్యాగ్& get_lang_sys_ట్యాగ్ (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి langSys.get_ట్యాగ్ (i); }
  గుర్తులేని పూర్ణాంకం get_lang_sys_ట్యాగులు (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				  గుర్తులేని పూర్ణాంకం *lang_sys_లెక్క /* IN/OUT */,
				  hb_ట్యాగ్_t     *lang_sys_ట్యాగులు /*బహిర్గతం*/) స్థిరరాశి
  { తిరిగి langSys.get_ట్యాగులు (ప్రారంభం_స్థానభ్రంశం, lang_sys_లెక్క, lang_sys_ట్యాగులు); }
  స్థిరరాశి JstfLangSys& get_lang_sys (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  {
    ఒకవేళ (i == సూచీ::NOT_దొరికింది_సూచీ) తిరిగి get_default_lang_sys ();
    తిరిగి ఇది+langSys[i].స్థానభ్రంశం;
  }
  తార్కిక కనుగొను_lang_sys_సూచీ (hb_ట్యాగ్_t ట్యాగ్_2, గుర్తులేని పూర్ణాంకం *సూచీ_2) స్థిరరాశి
  { తిరిగి langSys.కనుగొను_సూచీ (ట్యాగ్_2, సూచీ_2); }

  తార్కిక has_default_lang_sys () స్థిరరాశి               { తిరిగి defaultLangSys != 0; }
  స్థిరరాశి JstfLangSys& get_default_lang_sys () స్థిరరాశి { తిరిగి ఇది+defaultLangSys; }

  తార్కిక sanitize (hb_sanitize_context_t *c,
		 స్థిరరాశి రికార్డు_sanitize_సంవృతం_t * = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (extenderGlyphs.sanitize (c, ఇది) &&
		  defaultLangSys.sanitize (c, ఇది) &&
		  langSys.sanitize (c, ఇది));
  }

  రక్షిత:
  రకం_పేరు Types::మూస LOffset_గమ్యం<ExtenderGlyphs<Types>>
		extenderGlyphs;	/* Offset to ExtenderGlyph table--from beginning
				 * of JstfScript table-may be NULL */
  రకం_పేరు Types::మూస LOffset_గమ్యం<JstfLangSys>
		defaultLangSys;	/* Offset to DefaultJstfLangSys table--from
				 * beginning of JstfScript table--may be Null */
  రికార్డు_Array_యొక్క<JstfLangSys>
		langSys;	/* Array of JstfLangSysRecords--listed
				 * alphabetically by LangSysTag */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 * Types::LOffset::static_కొలత + 2, langSys);
};

మూస <రకం_పేరు Types>
నిర్మాణం Jstf_Script_రికార్డు
{
  పూర్ణాంకం cmp (hb_ట్యాగ్_t a) స్థిరరాశి { తిరిగి ట్యాగ్_2.cmp (a); }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    స్థిరరాశి రికార్డు_sanitize_సంవృతం_t సంవృతం = {ట్యాగ్_2, పునాది};
    return_trace (c->check_struct (ఇది) &&
		  స్థానభ్రంశం.sanitize (c, పునాది, &సంవృతం));
  }

  ట్యాగ్		ట్యాగ్_2;		/* 4-byte JstfScript identification tag */
  రకం_పేరు Types::మూస LOffset_గమ్యం<JstfScript<Types>>
		స్థానభ్రంశం;		/* Offset from beginning of JSTF header */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + Types::LOffset::static_కొలత);
};

మూస <రకం_పేరు Types>
నిర్మాణం JstfScriptList : Sorted_Array16_యొక్క<Jstf_Script_రికార్డు<Types>>
{
  స్థిరరాశి ట్యాగ్& get_ట్యాగ్ (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి (*ఇది)[i].ట్యాగ్_2; }
  గుర్తులేని పూర్ణాంకం get_ట్యాగులు (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			 గుర్తులేని పూర్ణాంకం *script_లెక్క /* IN/OUT */,
			 hb_ట్యాగ్_t     *script_ట్యాగులు /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (script_లెక్క)
    {
      + ఇది->as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, script_లెక్క)
      | hb_map (&Jstf_Script_రికార్డు<Types>::ట్యాగ్_2)
      | hb_sink (hb_array (script_ట్యాగులు, *script_లెక్క))
      ;
    }
    తిరిగి ఇది->len;
  }
  స్థిరరాశి JstfScript<Types>& get_script (స్థిరరాశి శూన్య *పునాది, గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి పునాది+(*ఇది)[i].స్థానభ్రంశం; }
  తార్కిక కనుగొను_సూచీ (hb_ట్యాగ్_t ట్యాగ్_2, గుర్తులేని పూర్ణాంకం *సూచీ_2) స్థిరరాశి
  {
    తిరిగి ఇది->bfind (ట్యాగ్_2, సూచీ_2, HB_NOT_దొరికింది_STORE, సూచీ::NOT_దొరికింది_సూచీ);
  }
};


/*
 * JSTF -- Justification
 * https://docs.microsoft.com/en-us/typography/opentype/spec/jstf
 */

నిర్మాణం JSTF
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_JSTF;

  గుర్తులేని పూర్ణాంకం get_script_లెక్క () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (has_script_list2 ()) తిరిగి get_script_list2 ().len;
#అయితే_ముగింపు
    తిరిగి scriptList.len;
  }
  స్థిరరాశి ట్యాగ్& get_script_ట్యాగ్ (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (has_script_list2 ()) తిరిగి get_script_list2 ().get_ట్యాగ్ (i);
#అయితే_ముగింపు
    తిరిగి scriptList.get_ట్యాగ్ (i);
  }
  గుర్తులేని పూర్ణాంకం get_script_ట్యాగులు (గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				గుర్తులేని పూర్ణాంకం *script_లెక్క /* IN/OUT */,
				hb_ట్యాగ్_t     *script_ట్యాగులు /*బహిర్గతం*/) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (has_script_list2 ())
      తిరిగి get_script_list2 ().get_ట్యాగులు (ప్రారంభం_స్థానభ్రంశం, script_లెక్క, script_ట్యాగులు);
#అయితే_ముగింపు
    తిరిగి scriptList.get_ట్యాగులు (ప్రారంభం_స్థానభ్రంశం, script_లెక్క, script_ట్యాగులు);
  }
  మూస <రకం_పేరు Types>
  స్థిరరాశి JstfScript<Types>& get_script (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి;
  తార్కిక కనుగొను_script_సూచీ (hb_ట్యాగ్_t ట్యాగ్_2, గుర్తులేని పూర్ణాంకం *సూచీ_2) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (has_script_list2 ()) తిరిగి get_script_list2 ().కనుగొను_సూచీ (ట్యాగ్_2, సూచీ_2);
#అయితే_ముగింపు
    తిరిగి scriptList.కనుగొను_సూచీ (ట్యాగ్_2, సూచీ_2);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!సంస్కరణ.sanitize (c) ||
		  !hb_barrier () ||
		  సంస్కరణ.major != 1 ||
		  !scriptList.sanitize (c, ఇది)))
      return_trace (అబద్ధం);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (unlikely (సంస్కరణ.minor >= 1 && !get_script_list2 ().sanitize (c, ఇది)))
      return_trace (అబద్ధం);
#అయితే_ముగింపు
    return_trace (నిజం);
  }

  అంతరంగ:
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  తార్కిక has_script_list2 () స్థిరరాశి
  { తిరిగి సంస్కరణ.minor >= 1 && get_script_list2 ().len; }
  స్థిరరాశి JstfScriptList<Layout::MediumTypes>& get_script_list2 () స్థిరరాశి
  { తిరిగి StructAfter<JstfScriptList<Layout::MediumTypes>> (scriptList); }
#అయితే_ముగింపు

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the JSTF table--initially set
				 * to 0x00010000u */
  JstfScriptList<Layout::SmallTypes>
		scriptList;	/* Array of JstfScripts--listed
				 * alphabetically by ScriptTag */
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
/*JstfScriptList<Layout::MediumTypes>
		scriptList2;*//* Array of JstfScript2s--listed
				 * alphabetically by ScriptTag.
				 * Present only if minorVersion is 1 or greater. */
#అయితే_ముగింపు
  బహిరంగ:
  DEFINE_కొలత_ARRAY (6, scriptList);
};

మూస <>
అంతఃవరుస స్థిరరాశి JstfScript<Layout::SmallTypes>&
JSTF::get_script<Layout::SmallTypes> (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
{ తిరిగి scriptList.get_script (ఇది, i); }

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
మూస <>
అంతఃవరుస స్థిరరాశి JstfScript<Layout::MediumTypes>&
JSTF::get_script<Layout::MediumTypes> (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
{ తిరిగి get_script_list2 ().get_script (ఇది, i); }
#అయితే_ముగింపు


} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_LAYOUT_JSTF_TABLE_HH */
