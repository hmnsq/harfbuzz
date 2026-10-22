#अन्तर्भाव "hb-subset-table.hh"

#अन्तर्भाव "hb-ot-var-hvar-table.hh"
#अन्तर्भाव "hb-ot-var-gvar-table.hh"
#अन्तर्भाव "hb-ot-var-fvar-table.hh"
#अन्तर्भाव "hb-ot-var-avar-table.hh"
#अन्तर्भाव "hb-ot-var-cvar-table.hh"
#अन्तर्भाव "hb-ot-var-mvar-table.hh"
#ifndef HB_NO_VAR_COMPOSITES
#अन्तर्भाव "OT/Var/VARC/VARC-subset.hh"
#endif

तार्किक _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<अक्षर> &buf, hb_tag_t tag, तार्किक *success)
{
#ifndef HB_NO_VAR
  निवड (tag)
  {
  स्थिती HB_TAG('H','V','A','R'): *success = _hb_subset_table<स्थिरांक OT::HVAR> (plan, buf); परत सत्य;
  स्थिती HB_TAG('V','V','A','R'): *success = _hb_subset_table<स्थिरांक OT::VVAR> (plan, buf); परत सत्य;
  स्थिती HB_TAG('g','v','a','r'): *success = _hb_subset_table<स्थिरांक OT::gvar> (plan, buf); परत सत्य;
#ifndef HB_NO_BEYOND_64K
  स्थिती HB_TAG('G','V','A','R'): *success = _hb_subset_table<स्थिरांक OT::GVAR> (plan, buf); परत सत्य;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  स्थिती HB_TAG('V','A','R','C'): *success = _hb_subset_table<स्थिरांक OT::VARC> (plan, buf); परत सत्य;
#endif
  स्थिती HB_TAG('f','v','a','r'):
    जर (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<स्थिरांक OT::fvar> (plan, buf);
    परत सत्य;
  स्थिती HB_TAG('a','v','a','r'):
    जर (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<स्थिरांक OT::avar> (plan, buf);
    परत सत्य;
  स्थिती HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    जर (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<स्थिरांक OT::cvar> (plan, buf);
    परत सत्य;
  स्थिती HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    जर (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<स्थिरांक OT::MVAR> (plan, buf);
    परत सत्य;
  }
#endif
  परत असत्य;
}
