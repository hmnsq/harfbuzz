#समावेश "hb-subset-table.hh"

#समावेश "OT/Color/sbix/sbix.hh"
#समावेश "OT/Color/CPAL/CPAL.hh"
#समावेश "OT/Color/COLR/COLR.hh"
#समावेश "OT/Color/CBDT/CBDT.hh"

bool _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, bool *success)
{
#अगर_अघोषित HB_NO_COLOR
  चयन (tag)
  {
  हाल HB_TAG('s','b','i','x'): *success = _hb_subset_table<स्थिर OT::sbix> (plan, buf); वापस true;
  हाल HB_TAG('C','O','L','R'): *success = _hb_subset_table<स्थिर OT::COLR> (plan, buf); वापस true;
  हाल HB_TAG('C','P','A','L'): *success = _hb_subset_table<स्थिर OT::CPAL> (plan, buf); वापस true;
  हाल HB_TAG('C','B','L','C'): *success = _hb_subset_table<स्थिर OT::CBLC> (plan, buf); वापस true;
  हाल HB_TAG('C','B','D','T'): *success = true; वापस true; /* skip CBDT, handled by CBLC */
  }
#पूर्ण_अगर
  वापस false;
}
