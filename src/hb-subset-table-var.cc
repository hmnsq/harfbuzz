#ضمن "hb-subset-table.hh"

#ضمن "hb-ot-var-hvar-table.hh"
#ضمن "hb-ot-var-gvar-table.hh"
#ضمن "hb-ot-var-fvar-table.hh"
#ضمن "hb-ot-var-avar-table.hh"
#ضمن "hb-ot-var-cvar-table.hh"
#ضمن "hb-ot-var-mvar-table.hh"
#که_نه_تعریف_شوی HB_NO_VAR_COMPOSITES
#ضمن "OT/Var/VARC/VARC-subset.hh"
#د_که_پای

منطقي hb_subset_جدول_var		(hb_subset_plan_t *plan, hb_ویکټور_t<حرف> &buf, hb_نښان_t نښان_2, منطقي *success)
{
#که_نه_تعریف_شوی HB_NO_VAR
  بدل (نښان_2)
  {
  حالة HB_نښان('H','V','A','R'): *success = hb_subset_جدول<ثابت OT::HVAR> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('V','V','A','R'): *success = hb_subset_جدول<ثابت OT::VVAR> (plan, buf); أرجع ریښتیا;
  حالة HB_نښان('g','v','a','r'): *success = hb_subset_جدول<ثابت OT::gvar> (plan, buf); أرجع ریښتیا;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  حالة HB_نښان('G','V','A','R'): *success = hb_subset_جدول<ثابت OT::GVAR> (plan, buf); أرجع ریښتیا;
#د_که_پای
#که_نه_تعریف_شوی HB_NO_VAR_COMPOSITES
  حالة HB_نښان('V','A','R','C'): *success = hb_subset_جدول<ثابت OT::VARC> (plan, buf); أرجع ریښتیا;
#د_که_پای
  حالة HB_نښان('f','v','a','r'):
    إذا (plan->user_axes_location.is_empty ())
      *success = hb_subset_جدول_passthrough (plan, نښان_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::fvar> (plan, buf);
    أرجع ریښتیا;
  حالة HB_نښان('a','v','a','r'):
    إذا (plan->user_axes_location.is_empty ())
      *success = hb_subset_جدول_passthrough (plan, نښان_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::avar> (plan, buf);
    أرجع ریښتیا;
  حالة HB_نښان('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    إذا (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_جدول_passthrough (plan, نښان_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::cvar> (plan, buf);
    أرجع ریښتیا;
  حالة HB_نښان('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    إذا (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_جدول_passthrough (plan, نښان_2);
    وإلا
      *success = hb_subset_جدول<ثابت OT::MVAR> (plan, buf);
    أرجع ریښتیا;
  }
#د_که_پای
  أرجع خطأ;
}
