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

যৌক্তিক _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<অক্ষর> &buf, hb_tag_t tag, যৌক্তিক *success)
{
#ifndef HB_NO_VAR
  নির্বাচন (tag)
  {
  ক্ষেত্র HB_TAG('H','V','A','R'): *success = _hb_subset_table<ধ্রুবক OT::HVAR> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('V','V','A','R'): *success = _hb_subset_table<ধ্রুবক OT::VVAR> (plan, buf); ফেরত সত্য;
  ক্ষেত্র HB_TAG('g','v','a','r'): *success = _hb_subset_table<ধ্রুবক OT::gvar> (plan, buf); ফেরত সত্য;
#ifndef HB_NO_BEYOND_64K
  ক্ষেত্র HB_TAG('G','V','A','R'): *success = _hb_subset_table<ধ্রুবক OT::GVAR> (plan, buf); ফেরত সত্য;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  ক্ষেত্র HB_TAG('V','A','R','C'): *success = _hb_subset_table<ধ্রুবক OT::VARC> (plan, buf); ফেরত সত্য;
#endif
  ক্ষেত্র HB_TAG('f','v','a','r'):
    যদি (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুবক OT::fvar> (plan, buf);
    ফেরত সত্য;
  ক্ষেত্র HB_TAG('a','v','a','r'):
    যদি (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুবক OT::avar> (plan, buf);
    ফেরত সত্য;
  ক্ষেত্র HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    যদি (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুবক OT::cvar> (plan, buf);
    ফেরত সত্য;
  ক্ষেত্র HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    যদি (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    অন্যথা
      *success = _hb_subset_table<ধ্রুবক OT::MVAR> (plan, buf);
    ফেরত সত্য;
  }
#endif
  ফেরত মিথ্যা;
}
