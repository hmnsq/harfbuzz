#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_GLYF_LOCA_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_GLYF_LOCA_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-open-type.hh"


ਨਾਮਾਕਾਸ਼ OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_loca HB_ਟੈਗ('l','o','c','a')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_LOCA HB_ਟੈਗ('L','O','C','A')

ਬਣਤਰ loca
{
  ਮਿੱਤਰ ਬਣਤਰ glyf;
  ਮਿੱਤਰ ਬਣਤਰ glyf_accelerator_t;

  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_loca;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c HB_ਅਣਵਰਤਿਆ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਸੱਚ);
  }

  ਸੁਰੱਖਿਅਤ:
  Unsized_Array_ਦਾ<HBUINT8>
		ਅੰਕੜੇ_Z;	/* Location data. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_GLYF_LOCA_HH */
