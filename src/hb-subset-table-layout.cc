#ಅನ್ತರ್ಭಾವ "hb-subset-table.hh"

#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gdef-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gsub-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gpos-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-base-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-math-table.hh"

ತಾರ್ಕಿಕ _hb_subset_table_layout		(hb_subset_plan_t *plan, hb_vector_t<ವರ್ಣ> &buf, hb_tag_t tag, ತಾರ್ಕಿಕ *success)
{
#ifndef HB_NO_SUBSET_LAYOUT
  ವಿಕಲ್ಪ (tag)
  {
  ಪಕ್ಷ HB_TAG('G','D','E','F'): *success = _hb_subset_table<ಧ್ರುವ OT::GDEF> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('G','S','U','B'): *success = _hb_subset_table<ಧ್ರುವ OT::Layout::GSUB> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('G','P','O','S'): *success = _hb_subset_table<ಧ್ರುವ OT::Layout::GPOS> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('B','A','S','E'): *success = _hb_subset_table<ಧ್ರುವ OT::BASE> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('M','A','T','H'): *success = _hb_subset_table<ಧ್ರುವ OT::MATH> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
#endif
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}
