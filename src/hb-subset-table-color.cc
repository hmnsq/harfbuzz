#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-table.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/sbix/sbix.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/CPAL/CPAL.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/COLR/COLR.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/CBDT/CBDT.hh"

ਤਾਰਕਿਕ hb_subset_ਸਾਰਣੀ_color		(hb_subset_plan_t *plan, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf, hb_ਟੈਗ_t ਟੈਗ_2, ਤਾਰਕਿਕ *success)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_COLOR
  ਚੋਣ (ਟੈਗ_2)
  {
  ਹਾਲਤ HB_ਟੈਗ('s','b','i','x'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::sbix> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('C','O','L','R'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::COLR> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('C','P','A','L'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::CPAL> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('C','B','L','C'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::CBLC> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('C','B','D','T'): *success = ਸੱਚ; ਵਾਪਸ ਸੱਚ; /* skip CBDT, handled by CBLC */
  }
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਝੂਠ;
}
