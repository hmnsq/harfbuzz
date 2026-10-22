#ifndef OT_GLYF_LOCA_HH
#define OT_GLYF_LOCA_HH


#ಅನ್ತರ್ಭಾವ "../../hb-open-type.hh"


ನಾಮಾಕಾಶ OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#define HB_OT_TAG_loca HB_TAG('l','o','c','a')
#define HB_OT_TAG_LOCA HB_TAG('L','O','C','A')

ಸಂರಚನಾ loca
{
  friend ಸಂರಚನಾ glyf;
  friend ಸಂರಚನಾ glyf_accelerator_t;

  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_loca;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c HB_UNUSED) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (ಸತ್ಯ);
  }

  ರಕ್ಷಿತ:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Location data. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#endif /* OT_GLYF_LOCA_HH */
