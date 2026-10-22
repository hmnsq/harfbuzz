#समावेश "hb-subset-table.hh"

#समावेश "hb-ot-var-hvar-table.hh"
#समावेश "hb-ot-var-gvar-table.hh"
#समावेश "hb-ot-var-fvar-table.hh"
#समावेश "hb-ot-var-avar-table.hh"
#समावेश "hb-ot-var-cvar-table.hh"
#समावेश "hb-ot-var-mvar-table.hh"
#अगर_अघोषित HB_NO_VAR_COMPOSITES
#समावेश "OT/Var/VARC/VARC-subset.hh"
#पूर्ण_अगर

bool _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, bool *success)
{
#अगर_अघोषित HB_NO_VAR
  चयन (tag)
  {
  हाल HB_TAG('H','V','A','R'): *success = _hb_subset_table<स्थिर OT::HVAR> (plan, buf); वापस true;
  हाल HB_TAG('V','V','A','R'): *success = _hb_subset_table<स्थिर OT::VVAR> (plan, buf); वापस true;
  हाल HB_TAG('g','v','a','r'): *success = _hb_subset_table<स्थिर OT::gvar> (plan, buf); वापस true;
#अगर_अघोषित HB_NO_BEYOND_64K
  हाल HB_TAG('G','V','A','R'): *success = _hb_subset_table<स्थिर OT::GVAR> (plan, buf); वापस true;
#पूर्ण_अगर
#अगर_अघोषित HB_NO_VAR_COMPOSITES
  हाल HB_TAG('V','A','R','C'): *success = _hb_subset_table<स्थिर OT::VARC> (plan, buf); वापस true;
#पूर्ण_अगर
  हाल HB_TAG('f','v','a','r'):
    अगर (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<स्थिर OT::fvar> (plan, buf);
    वापस true;
  हाल HB_TAG('a','v','a','r'):
    अगर (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<स्थिर OT::avar> (plan, buf);
    वापस true;
  हाल HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    अगर (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<स्थिर OT::cvar> (plan, buf);
    वापस true;
  हाल HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    अगर (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<स्थिर OT::MVAR> (plan, buf);
    वापस true;
  }
#पूर्ण_अगर
  वापस false;
}
