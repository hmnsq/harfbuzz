#شامل "hb-subset-table.hh"

#شامل "OT/Color/sbix/sbix.hh"
#شامل "OT/Color/CPAL/CPAL.hh"
#شامل "OT/Color/COLR/COLR.hh"
#شامل "OT/Color/CBDT/CBDT.hh"

منطقی hb_subset_جدول_color		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_لیبل_t لیبل_2, منطقی *success)
{
#اگر_غیر_تعریف_شدہ HB_NO_COLOR
  انتخاب (لیبل_2)
  {
  حالت HB_لیبل('s','b','i','x'): *success = hb_subset_جدول<ثابت OT::sbix> (plan, buf); واپس سچ;
  حالت HB_لیبل('C','O','L','R'): *success = hb_subset_جدول<ثابت OT::COLR> (plan, buf); واپس سچ;
  حالت HB_لیبل('C','P','A','L'): *success = hb_subset_جدول<ثابت OT::CPAL> (plan, buf); واپس سچ;
  حالت HB_لیبل('C','B','L','C'): *success = hb_subset_جدول<ثابت OT::CBLC> (plan, buf); واپس سچ;
  حالت HB_لیبل('C','B','D','T'): *success = سچ; واپس سچ; /* skip CBDT, handled by CBLC */
  }
#اگر_ختم
  واپس جھوٹ;
}
