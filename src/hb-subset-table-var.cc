#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-var-hvar-table.hh"
#ضمن "hb-ot-var-gvar-table.hh"
#ضمن "hb-ot-var-fvar-table.hh"
#ضمن "hb-ot-var-avar-table.hh"
#ضمن "hb-ot-var-cvar-table.hh"
#ضمن "hb-ot-var-mvar-table.hh"
#جے_بنا_تعریف HB_NO_VAR_COMPOSITES
#ضمن "OT/Var/VARC/VARC-subset.hh"
#جے_انت

منطقي hb_subset_جدول_var		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_ٹیگ_t ٹیگ_2, منطقي *success)
{
#جے_بنا_تعریف HB_NO_VAR
  بدل (ٹیگ_2)
  {
  حالة HB_ٹیگ('H','V','A','R'): *success = hb_subset_جدول<ثابت OT::HVAR> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('V','V','A','R'): *success = hb_subset_جدول<ثابت OT::VVAR> (plan, buf); أرجع سچ;
  حالة HB_ٹیگ('g','v','a','r'): *success = hb_subset_جدول<ثابت OT::gvar> (plan, buf); أرجع سچ;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  حالة HB_ٹیگ('G','V','A','R'): *success = hb_subset_جدول<ثابت OT::GVAR> (plan, buf); أرجع سچ;
#جے_انت
#جے_بنا_تعریف HB_NO_VAR_COMPOSITES
  حالة HB_ٹیگ('V','A','R','C'): *success = hb_subset_جدول<ثابت OT::VARC> (plan, buf); أرجع سچ;
#جے_انت
  حالة HB_ٹیگ('f','v','a','r'):
    إذا (plan->user_axes_location.is_empty ())
      *success = hb_subset_جدول_passthrough (plan, ٹیگ_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::fvar> (plan, buf);
    أرجع سچ;
  حالة HB_ٹیگ('a','v','a','r'):
    إذا (plan->user_axes_location.is_empty ())
      *success = hb_subset_جدول_passthrough (plan, ٹیگ_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::avar> (plan, buf);
    أرجع سچ;
  حالة HB_ٹیگ('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    إذا (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_جدول_passthrough (plan, ٹیگ_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::cvar> (plan, buf);
    أرجع سچ;
  حالة HB_ٹیگ('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    إذا (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_جدول_passthrough (plan, ٹیگ_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::MVAR> (plan, buf);
    أرجع سچ;
  }
#جے_انت
  أرجع خطأ;
}
