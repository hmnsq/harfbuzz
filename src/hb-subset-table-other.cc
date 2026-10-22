#ಅನ್ತರ್ಭಾವ "hb-subset-table.hh"

#ಅನ್ತರ್ಭಾವ "hb-ot-cmap-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-glyf-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-hdmx-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-hhea-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-hmtx-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-maxp-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-os2-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-name-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-post-table.hh"

ತಾರ್ಕಿಕ _hb_subset_table_other		(hb_subset_plan_t *plan, hb_vector_t<ವರ್ಣ> &buf, hb_tag_t tag, ತಾರ್ಕಿಕ *success)
{
  ವಿಕಲ್ಪ (tag)
  {
  ಪಕ್ಷ HB_TAG('g','l','y','f'): *success = _hb_subset_table<ಧ್ರುವ OT::glyf> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('h','d','m','x'): *success = _hb_subset_table<ಧ್ರುವ OT::hdmx> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('n','a','m','e'): *success = _hb_subset_table<ಧ್ರುವ OT::name> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('h','h','e','a'): *success = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* skip hhea, handled by hmtx */
  ಪಕ್ಷ HB_TAG('h','m','t','x'): *success = _hb_subset_table<ಧ್ರುವ OT::hmtx> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('v','h','e','a'): *success = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* skip vhea, handled by vmtx */
  ಪಕ್ಷ HB_TAG('v','m','t','x'): *success = _hb_subset_table<ಧ್ರುವ OT::vmtx> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('m','a','x','p'): *success = _hb_subset_table<ಧ್ರುವ OT::maxp> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#ifndef HB_NO_BEYOND_64K
  ಪಕ್ಷ HB_TAG('G','L','Y','F'): *success = _hb_subset_table<ಧ್ರುವ OT::GLYF> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('H','H','E','A'): *success = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* skip HHEA, handled by HMTX */
  ಪಕ್ಷ HB_TAG('H','M','T','X'): *success = _hb_subset_table<ಧ್ರುವ OT::HMTX> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('V','H','E','A'): *success = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* skip VHEA, handled by VMTX */
  ಪಕ್ಷ HB_TAG('V','M','T','X'): *success = _hb_subset_table<ಧ್ರುವ OT::VMTX> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('M','A','X','P'): *success = _hb_subset_table<ಧ್ರುವ OT::MAXP> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
  ಪಕ್ಷ HB_TAG('l','o','c','a'): *success = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* skip loca, handled by glyf */
#ifndef HB_NO_BEYOND_64K
  ಪಕ್ಷ HB_TAG('L','O','C','A'): *success = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* skip LOCA, handled by GLYF */
#endif
  ಪಕ್ಷ HB_TAG('c','m','a','p'): *success = _hb_subset_table<ಧ್ರುವ OT::cmap> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('D','M','A','P'): *success = _hb_subset_table<ಧ್ರುವ OT::DMAP> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('O','S','/','2'): *success = _hb_subset_table<ಧ್ರುವ OT::OS2 > (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಪಕ್ಷ HB_TAG('p','o','s','t'): *success = _hb_subset_table<ಧ್ರುವ OT::post> (plan, buf); ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}
