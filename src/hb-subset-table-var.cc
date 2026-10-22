#include "hb-subset-table.hh"

#include "hb-ot-var-hvar-table.hh"
#include "hb-ot-var-gvar-table.hh"
#include "hb-ot-var-fvar-table.hh"
#include "hb-ot-var-avar-table.hh"
#include "hb-ot-var-cvar-table.hh"
#include "hb-ot-var-mvar-table.hh"
#ifndef HB_NO_VAR_COMPOSITES
#include "OT/Var/VARC/VARC-subset.hh"
#endif

bool _hb_subset_table_var		(hb_subset_plan_t *plan, hb_vector_t<var_na> &buf, hb_tag_t tag, bool *success)
{
#ifndef HB_NO_VAR
  switch (tag)
  {
  pak_sa HB_TAG('H','V','A','R'): *success = _hb_subset_table<const OT::HVAR> (plan, buf); คืน true;
  pak_sa HB_TAG('V','V','A','R'): *success = _hb_subset_table<const OT::VVAR> (plan, buf); คืน true;
  pak_sa HB_TAG('g','v','a','r'): *success = _hb_subset_table<const OT::gvar> (plan, buf); คืน true;
#ifndef HB_NO_BEYOND_64K
  pak_sa HB_TAG('G','V','A','R'): *success = _hb_subset_table<const OT::GVAR> (plan, buf); คืน true;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  pak_sa HB_TAG('V','A','R','C'): *success = _hb_subset_table<const OT::VARC> (plan, buf); คืน true;
#endif
  pak_sa HB_TAG('f','v','a','r'):
    ถ้า (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    มิฉะนั้น
      *success = _hb_subset_table<const OT::fvar> (plan, buf);
    คืน true;
  pak_sa HB_TAG('a','v','a','r'):
    ถ้า (plan->user_axes_location.is_empty ())
      *success = _hb_subset_table_passthrough (plan, tag);
    มิฉะนั้น
      *success = _hb_subset_table<const OT::avar> (plan, buf);
    คืน true;
  pak_sa HB_TAG('c','v','a','r'):
    /* TODO: For avar2 partial instancing, cull unreachable tuple
     * variations (avar2_reachable_ranges) instead of passing through. */
    ถ้า (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    มิฉะนั้น
      *success = _hb_subset_table<const OT::cvar> (plan, buf);
    คืน true;
  pak_sa HB_TAG('M','V','A','R'):
    /* TODO: For avar2 partial instancing, cull unreachable VarStore
     * regions (avar2_reachable_ranges) instead of passing through. */
    ถ้า (plan->user_axes_location.is_empty () ||
        (plan->has_avar2 && !plan->normalized_coords))
      *success = _hb_subset_table_passthrough (plan, tag);
    มิฉะนั้น
      *success = _hb_subset_table<const OT::MVAR> (plan, buf);
    คืน true;
  }
#endif
  คืน false;
}
