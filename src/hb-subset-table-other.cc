#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-table.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cmap-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-glyf-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-hdmx-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-hhea-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-hmtx-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-maxp-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-os2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-name-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-post-table.hh"

ਤਾਰਕਿਕ hb_subset_ਸਾਰਣੀ_other		(hb_subset_plan_t *plan, hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf, hb_ਟੈਗ_t ਟੈਗ_2, ਤਾਰਕਿਕ *success)
{
  ਚੋਣ (ਟੈਗ_2)
  {
  ਹਾਲਤ HB_ਟੈਗ('g','l','y','f'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::glyf> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('h','d','m','x'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::hdmx> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('n','a','m','e'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::name> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('h','h','e','a'): *success = ਸੱਚ; ਵਾਪਸ ਸੱਚ; /* skip hhea, handled by hmtx */
  ਹਾਲਤ HB_ਟੈਗ('h','m','t','x'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::hmtx> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('v','h','e','a'): *success = ਸੱਚ; ਵਾਪਸ ਸੱਚ; /* skip vhea, handled by vmtx */
  ਹਾਲਤ HB_ਟੈਗ('v','m','t','x'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::vmtx> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('m','a','x','p'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::maxp> (plan, buf); ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਹਾਲਤ HB_ਟੈਗ('G','L','Y','F'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::GLYF> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('H','H','E','A'): *success = ਸੱਚ; ਵਾਪਸ ਸੱਚ; /* skip HHEA, handled by HMTX */
  ਹਾਲਤ HB_ਟੈਗ('H','M','T','X'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::HMTX> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('V','H','E','A'): *success = ਸੱਚ; ਵਾਪਸ ਸੱਚ; /* skip VHEA, handled by VMTX */
  ਹਾਲਤ HB_ਟੈਗ('V','M','T','X'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::VMTX> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('M','A','X','P'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::MAXP> (plan, buf); ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
  ਹਾਲਤ HB_ਟੈਗ('l','o','c','a'): *success = ਸੱਚ; ਵਾਪਸ ਸੱਚ; /* skip loca, handled by glyf */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਹਾਲਤ HB_ਟੈਗ('L','O','C','A'): *success = ਸੱਚ; ਵਾਪਸ ਸੱਚ; /* skip LOCA, handled by GLYF */
#ਜੇ_ਅੰਤ
  ਹਾਲਤ HB_ਟੈਗ('c','m','a','p'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::cmap> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('D','M','A','P'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::DMAP> (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('O','S','/','2'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::OS2 > (plan, buf); ਵਾਪਸ ਸੱਚ;
  ਹਾਲਤ HB_ਟੈਗ('p','o','s','t'): *success = hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::post> (plan, buf); ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
}
