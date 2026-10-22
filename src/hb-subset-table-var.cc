#شامل "hb-subset-table.hh"

#شامل "hb-ot-var-hvar-table.hh"
#شامل "hb-ot-var-gvar-table.hh"
#شامل "hb-ot-var-fvar-table.hh"
#شامل "hb-ot-var-avar-table.hh"
#شامل "hb-ot-var-cvar-table.hh"
#شامل "hb-ot-var-mvar-table.hh"
#اگر_غیر_تعریف_شدہ HB_NO_VAR_COMPOSITES
#شامل "OT/Var/VARC/VARC-subset.hh"
#اگر_ختم

منطقی hb_subset_جدول_var		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_لیبل_t لیبل_2, منطقی *success)
{
#اگر_غیر_تعریف_شدہ HB_NO_VAR
  انتخاب (لیبل_2)
  {
  حالت HB_لیبل('H','V','A','R'): *success = hb_subset_جدول<ثابت OT::HVAR> (plan, buf); واپس سچ;
  حالت HB_لیبل('V','V','A','R'): *success = hb_subset_جدول<ثابت OT::VVAR> (plan, buf); واپس سچ;
  حالت HB_لیبل('g','v','a','r'): *success = hb_subset_جدول<ثابت OT::gvar> (plan, buf); واپس سچ;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  حالت HB_لیبل('G','V','A','R'): *success = hb_subset_جدول<ثابت OT::GVAR> (plan, buf); واپس سچ;
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_VAR_COMPOSITES
  حالت HB_لیبل('V','A','R','C'): *success = hb_subset_جدول<ثابت OT::VARC> (plan, buf); واپس سچ;
#اگر_ختم
  حالت HB_لیبل('f','v','a','r'):
    اگر (plan->user_axes_location.is_empty ())
      *success = hb_subset_جدول_passthrough (plan, لیبل_2);
    ورنہ
      *success = hb_subset_جدول<ثابت OT::fvar> (plan, buf);
    واپس سچ;
  حالت HB_لیبل('a','v','a','r'):
    اگر (plan->user_axes_location.is_empty ())
      *success = hb_subset_جدول_passthrough (plan, لیبل_2);
    ورنہ
      *success = hb_subset_جدول<ثابت OT::avar> (plan, buf);
    واپس سچ;
  حالت HB_لیبل('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    اگر (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_جدول_passthrough (plan, لیبل_2);
    ورنہ
      *success = hb_subset_جدول<ثابت OT::cvar> (plan, buf);
    واپس سچ;
  حالت HB_لیبل('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    اگر (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = hb_subset_جدول_passthrough (plan, لیبل_2);
    ورنہ
      *success = hb_subset_جدول<ثابت OT::MVAR> (plan, buf);
    واپس سچ;
  }
#اگر_ختم
  واپس جھوٹ;
}
