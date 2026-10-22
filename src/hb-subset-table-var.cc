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

तार्किक _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<वर्ण> &buf, hb_tag_t tag, तार्किक *success)
{
#ifndef HB_NO_VAR
  विकल्प (tag)
  {
  पक्ष HB_TAG('H','V','A','R'): *success = _hb_subset_table<ध्रुव OT::HVAR> (plan, buf); प्रत्यागम सत्य;
  पक्ष HB_TAG('V','V','A','R'): *success = _hb_subset_table<ध्रुव OT::VVAR> (plan, buf); प्रत्यागम सत्य;
  पक्ष HB_TAG('g','v','a','r'): *success = _hb_subset_table<ध्रुव OT::gvar> (plan, buf); प्रत्यागम सत्य;
#ifndef HB_NO_BEYOND_64K
  पक्ष HB_TAG('G','V','A','R'): *success = _hb_subset_table<ध्रुव OT::GVAR> (plan, buf); प्रत्यागम सत्य;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  पक्ष HB_TAG('V','A','R','C'): *success = _hb_subset_table<ध्रुव OT::VARC> (plan, buf); प्रत्यागम सत्य;
#endif
  पक्ष HB_TAG('f','v','a','r'):
    यदि (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<ध्रुव OT::fvar> (plan, buf);
    प्रत्यागम सत्य;
  पक्ष HB_TAG('a','v','a','r'):
    यदि (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<ध्रुव OT::avar> (plan, buf);
    प्रत्यागम सत्य;
  पक्ष HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    यदि (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<ध्रुव OT::cvar> (plan, buf);
    प्रत्यागम सत्य;
  पक्ष HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    यदि (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    अन्यथा
      *success = _hb_subset_table<ध्रुव OT::MVAR> (plan, buf);
    प्रत्यागम सत्य;
  }
#endif
  प्रत्यागम असत्य;
}
