#شامل "hb-subset-table.hh"

#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-ot-layout-gsub-table.hh"
#شامل "hb-ot-layout-gpos-table.hh"
#شامل "hb-ot-layout-base-table.hh"
#شامل "hb-ot-math-table.hh"

منطقی hb_subset_جدول_layout		(hb_subset_plan_t *plan, hb_سمتیہ_t<حرف> &buf, hb_لیبل_t لیبل_2, منطقی *success)
{
#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_LAYOUT
  انتخاب (لیبل_2)
  {
  حالت HB_لیبل('G','D','E','F'): *success = hb_subset_جدول<ثابت OT::GDEF> (plan, buf); واپس سچ;
  حالت HB_لیبل('G','S','U','B'): *success = hb_subset_جدول<ثابت OT::Layout::GSUB> (plan, buf); واپس سچ;
  حالت HB_لیبل('G','P','O','S'): *success = hb_subset_جدول<ثابت OT::Layout::GPOS> (plan, buf); واپس سچ;
  حالت HB_لیبل('B','A','S','E'): *success = hb_subset_جدول<ثابت OT::بنیاد_4> (plan, buf); واپس سچ;
  حالت HB_لیبل('M','A','T','H'): *success = hb_subset_جدول<ثابت OT::MATH> (plan, buf); واپس سچ;
  }
#اگر_ختم
  واپس جھوٹ;
}
