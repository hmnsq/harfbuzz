#অন্তর্ভাৱ "hb-subset-table.hh"

#অন্তর্ভাৱ "hb-ot-var-hvar-table.hh"
#অন্তর্ভাৱ "hb-ot-var-gvar-table.hh"
#অন্তর্ভাৱ "hb-ot-var-fvar-table.hh"
#অন্তর্ভাৱ "hb-ot-var-avar-table.hh"
#অন্তর্ভাৱ "hb-ot-var-cvar-table.hh"
#অন্তর্ভাৱ "hb-ot-var-mvar-table.hh"
#ifndef HB_NO_VAR_COMPOSITES
#অন্তর্ভাৱ "OT/Var/VARC/VARC-subset.hh"
#endif

তার্কিক _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<ৱর্ণ> &buf, hb_tag_t tag, তার্কিক *success)
{
#ifndef HB_NO_VAR
  ৱিকল্প (tag)
  {
  পক্ষ HB_TAG('H','V','A','R'): *success = _hb_subset_table<ধ্রুৱ OT::HVAR> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('V','V','A','R'): *success = _hb_subset_table<ধ্রুৱ OT::VVAR> (plan, buf); ঘূৰাই সঁচা;
  পক্ষ HB_TAG('g','v','a','r'): *success = _hb_subset_table<ধ্রুৱ OT::gvar> (plan, buf); ঘূৰাই সঁচা;
#ifndef HB_NO_BEYOND_64K
  পক্ষ HB_TAG('G','V','A','R'): *success = _hb_subset_table<ধ্রুৱ OT::GVAR> (plan, buf); ঘূৰাই সঁচা;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  পক্ষ HB_TAG('V','A','R','C'): *success = _hb_subset_table<ধ্রুৱ OT::VARC> (plan, buf); ঘূৰাই সঁচা;
#endif
  পক্ষ HB_TAG('f','v','a','r'):
    যদি (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুৱ OT::fvar> (plan, buf);
    ঘূৰাই সঁচা;
  পক্ষ HB_TAG('a','v','a','r'):
    যদি (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুৱ OT::avar> (plan, buf);
    ঘূৰাই সঁচা;
  পক্ষ HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    যদি (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুৱ OT::cvar> (plan, buf);
    ঘূৰাই সঁচা;
  পক্ষ HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    যদি (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুৱ OT::MVAR> (plan, buf);
    ঘূৰাই সঁচা;
  }
#endif
  ঘূৰাই মিছা;
}
