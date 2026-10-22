#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-var-hvar-table.hh"
#ضمن "hb-ot-var-gvar-table.hh"
#ضمن "hb-ot-var-fvar-table.hh"
#ضمن "hb-ot-var-avar-table.hh"
#ضمن "hb-ot-var-cvar-table.hh"
#ضمن "hb-ot-var-mvar-table.hh"
#اگر_تعریف_نشده HB_NO_VAR_COMPOSITES
#ضمن "OT/Var/VARC/VARC-subset.hh"
#پایان_اگر

منطقي hb_subset_جدول_var		(hb_subset_plan_t *plan, hb_وکتور_t<حرف> &buf, hb_برچسب_t برچسب_2, منطقي *success)
{
#اگر_تعریف_نشده HB_NO_VAR
  بدل (برچسب_2)
  {
  حالة HB_برچسب('H','V','A','R'): *success = hb_subset_جدول<ثابت OT::HVAR> (plan, buf); أرجع درست;
  حالة HB_برچسب('V','V','A','R'): *success = hb_subset_جدول<ثابت OT::VVAR> (plan, buf); أرجع درست;
  حالة HB_برچسب('g','v','a','r'): *success = hb_subset_جدول<ثابت OT::gvar> (plan, buf); أرجع درست;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  حالة HB_برچسب('G','V','A','R'): *success = hb_subset_جدول<ثابت OT::GVAR> (plan, buf); أرجع درست;
#پایان_اگر
#اگر_تعریف_نشده HB_NO_VAR_COMPOSITES
  حالة HB_برچسب('V','A','R','C'): *success = hb_subset_جدول<ثابت OT::VARC> (plan, buf); أرجع درست;
#پایان_اگر
  حالة HB_برچسب('f','v','a','r'):
    إذا (plan->user_axes_location.is_empty ())
      *success = hb_subset_جدول_passthrough (plan, برچسب_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::fvar> (plan, buf);
    أرجع درست;
  حالة HB_برچسب('a','v','a','r'):
    إذا (plan->user_axes_location.is_empty ())
      *success = hb_subset_جدول_passthrough (plan, برچسب_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::avar> (plan, buf);
    أرجع درست;
  حالة HB_برچسب('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    إذا (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_جدول_passthrough (plan, برچسب_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::cvar> (plan, buf);
    أرجع درست;
  حالة HB_برچسب('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    إذا (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_جدول_passthrough (plan, برچسب_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::MVAR> (plan, buf);
    أرجع درست;
  }
#پایان_اگر
  أرجع خطأ;
}
