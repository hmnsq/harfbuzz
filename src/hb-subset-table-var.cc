#অন্তর্ভাব "hb-subset-table.hh"

#অন্তর্ভাব "hb-ot-var-hvar-table.hh"
#অন্তর্ভাব "hb-ot-var-gvar-table.hh"
#অন্তর্ভাব "hb-ot-var-fvar-table.hh"
#অন্তর্ভাব "hb-ot-var-avar-table.hh"
#অন্তর্ভাব "hb-ot-var-cvar-table.hh"
#অন্তর্ভাব "hb-ot-var-mvar-table.hh"
#ifndef HB_NO_VAR_COMPOSITES
#অন্তর্ভাব "OT/Var/VARC/VARC-subset.hh"
#endif

তার্কিক _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<বর্ণ> &buf, hb_tag_t tag, তার্কিক *success)
{
#ifndef HB_NO_VAR
  বিকল্প (tag)
  {
  পক্ষ HB_TAG('H','V','A','R'): *success = _hb_subset_table<ধ্রুব OT::HVAR> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('V','V','A','R'): *success = _hb_subset_table<ধ্রুব OT::VVAR> (plan, buf); প্রত্যাগম সত্য;
  পক্ষ HB_TAG('g','v','a','r'): *success = _hb_subset_table<ধ্রুব OT::gvar> (plan, buf); প্রত্যাগম সত্য;
#ifndef HB_NO_BEYOND_64K
  পক্ষ HB_TAG('G','V','A','R'): *success = _hb_subset_table<ধ্রুব OT::GVAR> (plan, buf); প্রত্যাগম সত্য;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  পক্ষ HB_TAG('V','A','R','C'): *success = _hb_subset_table<ধ্রুব OT::VARC> (plan, buf); প্রত্যাগম সত্য;
#endif
  পক্ষ HB_TAG('f','v','a','r'):
    যদি (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুব OT::fvar> (plan, buf);
    প্রত্যাগম সত্য;
  পক্ষ HB_TAG('a','v','a','r'):
    যদি (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুব OT::avar> (plan, buf);
    প্রত্যাগম সত্য;
  পক্ষ HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    যদি (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুব OT::cvar> (plan, buf);
    প্রত্যাগম সত্য;
  পক্ষ HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    যদি (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুব OT::MVAR> (plan, buf);
    প্রত্যাগম সত্য;
  }
#endif
  প্রত্যাগম অসত্য;
}
