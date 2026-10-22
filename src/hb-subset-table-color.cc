#ಅನ್ತರ್ಭಾವ "hb-subset-table.hh"

#ಅನ್ತರ್ಭಾವ "OT/Color/sbix/sbix.hh"
#ಅನ್ತರ್ಭಾವ "OT/Color/CPAL/CPAL.hh"
#ಅನ್ತರ್ಭಾವ "OT/Color/COLR/COLR.hh"
#ಅನ್ತರ್ಭಾವ "OT/Color/CBDT/CBDT.hh"

ತಾರ್ಕಿಕ _hb_subset_table_color		(hb_subset_plan_t *plan, hb_vector_t<ವರ್ಣ> &buf, hb_tag_t tag, ತಾರ್ಕಿಕ *success)
{
#ifndef HB_NO_COLOR
  ವಿಕಲ್ಪ (tag)
  {
  ಪಕ್ಷ HB_TAG('s','b','i','x'): *success = _hb_subset_table<ಧ್ರುವ OT::sbix> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('C','O','L','R'): *success = _hb_subset_table<ಧ್ರುವ OT::COLR> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('C','P','A','L'): *success = _hb_subset_table<ಧ್ರುವ OT::CPAL> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('C','B','L','C'): *success = _hb_subset_table<ಧ್ರುವ OT::CBLC> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('C','B','D','T'): *success = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* skip CBDT, handled by CBLC */
  }
#endif
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}
