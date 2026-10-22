#ifndef OT_GLYF_LOCA_HH
#define OT_GLYF_LOCA_HH


#അന്തര്ഭാവ "../../hb-open-type.hh"


നാമാകാശ OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#define HB_OT_TAG_loca HB_TAG('l','o','c','a')
#define HB_OT_TAG_LOCA HB_TAG('L','O','C','A')

ഘടന loca
{
  friend ഘടന glyf;
  friend ഘടന glyf_accelerator_t;

  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_loca;

  തർക്കം sanitize (hb_sanitize_context_t *c HB_UNUSED) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (സത്യം);
  }

  സംരക്ഷിത:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Location data. */
  പൊതു:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#endif /* OT_GLYF_LOCA_HH */
