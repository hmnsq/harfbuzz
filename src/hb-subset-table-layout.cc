#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-table.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gdef-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gsub-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gpos-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-base-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-math-table.hh"

ਤਾਰਕਿਕ hb_subset_ਸਾਰਣੀ_layout		(hb_subset_plan_t *plan, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf, hb_ਟੈਗ_t ਟੈਗ_2, ਤਾਰਕਿਕ *success)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_LAYOUT
  ਚੋਣ (ਟੈਗ_2)
  {
  ਹਾਲਤ HB_ਟੈਗ('G','D','E','F'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::GDEF> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('G','S','U','B'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::Layout::GSUB> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('G','P','O','S'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::Layout::GPOS> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('B','A','S','E'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::ਅਧਾਰ_4> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('M','A','T','H'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::MATH> (plan, buf); ਵਾਪਸ ਸੱਚ;
  }
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਝੂਠ;
}
