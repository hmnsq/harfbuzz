#समावेश "hb-subset-table.hh"

#समावेश "hb-ot-layout-gdef-table.hh"
#समावेश "hb-ot-layout-gsub-table.hh"
#समावेश "hb-ot-layout-gpos-table.hh"
#समावेश "hb-ot-layout-base-table.hh"
#समावेश "hb-ot-math-table.hh"

bool _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, bool *success)
{
#अगर_अघोषित HB_NO_SUBSET_LAYOUT
  चयन (tag)
  {
  हाल HB_TAG('G','D','E','F'): *success = _hb_subset_table<स्थिर OT::GDEF> (plan, buf); वापस true;
  हाल HB_TAG('G','S','U','B'): *success = _hb_subset_table<स्थिर OT::Layout::GSUB> (plan, buf); वापस true;
  हाल HB_TAG('G','P','O','S'): *success = _hb_subset_table<स्थिर OT::Layout::GPOS> (plan, buf); वापस true;
  हाल HB_TAG('B','A','S','E'): *success = _hb_subset_table<स्थिर OT::BASE> (plan, buf); वापस true;
  हाल HB_TAG('M','A','T','H'): *success = _hb_subset_table<स्थिर OT::MATH> (plan, buf); वापस true;
  }
#पूर्ण_अगर
  वापस false;
}
