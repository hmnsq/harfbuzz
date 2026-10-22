#समावेश "hb-subset-table.hh"

#समावेश "hb-ot-cmap-table.hh"
#समावेश "hb-ot-glyf-table.hh"
#समावेश "hb-ot-hdmx-table.hh"
#समावेश "hb-ot-hhea-table.hh"
#समावेश "hb-ot-hmtx-table.hh"
#समावेश "hb-ot-maxp-table.hh"
#समावेश "hb-ot-os2-table.hh"
#समावेश "hb-ot-name-table.hh"
#समावेश "hb-ot-post-table.hh"

bool _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, bool *success)
{
  चयन (tag)
  {
  हाल HB_TAG('g','l','y','f'): *success = _hb_subset_table<स्थिर OT::glyf> (plan, buf); वापस true;
  हाल HB_TAG('h','d','m','x'): *success = _hb_subset_table<स्थिर OT::hdmx> (plan, buf); वापस true;
  हाल HB_TAG('n','a','m','e'): *success = _hb_subset_table<स्थिर OT::name> (plan, buf); वापस true;
  हाल HB_TAG('h','h','e','a'): *success = true; वापस true; /* skip hhea, handled by hmtx */
  हाल HB_TAG('h','m','t','x'): *success = _hb_subset_table<स्थिर OT::hmtx> (plan, buf); वापस true;
  हाल HB_TAG('v','h','e','a'): *success = true; वापस true; /* skip vhea, handled by vmtx */
  हाल HB_TAG('v','m','t','x'): *success = _hb_subset_table<स्थिर OT::vmtx> (plan, buf); वापस true;
  हाल HB_TAG('m','a','x','p'): *success = _hb_subset_table<स्थिर OT::maxp> (plan, buf); वापस true;
#अगर_अघोषित HB_NO_BEYOND_64K
  हाल HB_TAG('G','L','Y','F'): *success = _hb_subset_table<स्थिर OT::GLYF> (plan, buf); वापस true;
  हाल HB_TAG('H','H','E','A'): *success = true; वापस true; /* skip HHEA, handled by HMTX */
  हाल HB_TAG('H','M','T','X'): *success = _hb_subset_table<स्थिर OT::HMTX> (plan, buf); वापस true;
  हाल HB_TAG('V','H','E','A'): *success = true; वापस true; /* skip VHEA, handled by VMTX */
  हाल HB_TAG('V','M','T','X'): *success = _hb_subset_table<स्थिर OT::VMTX> (plan, buf); वापस true;
  हाल HB_TAG('M','A','X','P'): *success = _hb_subset_table<स्थिर OT::MAXP> (plan, buf); वापस true;
#पूर्ण_अगर
  हाल HB_TAG('l','o','c','a'): *success = true; वापस true; /* skip loca, handled by glyf */
#अगर_अघोषित HB_NO_BEYOND_64K
  हाल HB_TAG('L','O','C','A'): *success = true; वापस true; /* skip LOCA, handled by GLYF */
#पूर्ण_अगर
  हाल HB_TAG('c','m','a','p'): *success = _hb_subset_table<स्थिर OT::cmap> (plan, buf); वापस true;
  हाल HB_TAG('D','M','A','P'): *success = _hb_subset_table<स्थिर OT::DMAP> (plan, buf); वापस true;
  हाल HB_TAG('O','S','/','2'): *success = _hb_subset_table<स्थिर OT::OS2 > (plan, buf); वापस true;
  हाल HB_TAG('p','o','s','t'): *success = _hb_subset_table<स्थिर OT::post> (plan, buf); वापस true;
  }
  वापस false;
}
