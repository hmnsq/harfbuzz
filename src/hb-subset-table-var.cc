#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-var-hvar-table.hh"
#ضمن "hb-ot-var-gvar-table.hh"
#ضمن "hb-ot-var-fvar-table.hh"
#ضمن "hb-ot-var-avar-table.hh"
#ضمن "hb-ot-var-cvar-table.hh"
#ضمن "hb-ot-var-mvar-table.hh"
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR_COMPOSITES
#ضمن "OT/Var/VARC/VARC-subset.hh"
#جيڪڏهن_پورو

منطقي hb_subset_جدول_var		(hb_subset_plan_t *plan, hb_ويڪٽر_t<حرف> &buf, hb_ٽيگ_t ٽيگ_2, منطقي *success)
{
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  بدل (ٽيگ_2)
  {
  حالة HB_ٽيگ('H','V','A','R'): *success = hb_subset_جدول<ثابت OT::HVAR> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('V','V','A','R'): *success = hb_subset_جدول<ثابت OT::VVAR> (plan, buf); أرجع سچ;
  حالة HB_ٽيگ('g','v','a','r'): *success = hb_subset_جدول<ثابت OT::gvar> (plan, buf); أرجع سچ;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  حالة HB_ٽيگ('G','V','A','R'): *success = hb_subset_جدول<ثابت OT::GVAR> (plan, buf); أرجع سچ;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR_COMPOSITES
  حالة HB_ٽيگ('V','A','R','C'): *success = hb_subset_جدول<ثابت OT::VARC> (plan, buf); أرجع سچ;
#جيڪڏهن_پورو
  حالة HB_ٽيگ('f','v','a','r'):
    إذا (plan->user_axes_location.is_empty ())
      *success = hb_subset_جدول_passthrough (plan, ٽيگ_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::fvar> (plan, buf);
    أرجع سچ;
  حالة HB_ٽيگ('a','v','a','r'):
    إذا (plan->user_axes_location.is_empty ())
      *success = hb_subset_جدول_passthrough (plan, ٽيگ_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::avar> (plan, buf);
    أرجع سچ;
  حالة HB_ٽيگ('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    إذا (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_جدول_passthrough (plan, ٽيگ_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::cvar> (plan, buf);
    أرجع سچ;
  حالة HB_ٽيگ('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    إذا (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_جدول_passthrough (plan, ٽيگ_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::MVAR> (plan, buf);
    أرجع سچ;
  }
#جيڪڏهن_پورو
  أرجع خطأ;
}
