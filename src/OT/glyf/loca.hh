#ifndef OT_GLYF_LOCA_HH
#define OT_GLYF_LOCA_HH


#অন্তর্ভাৱ "../../hb-open-type.hh"


নামাকাশ OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#define HB_OT_TAG_loca HB_TAG('l','o','c','a')
#define HB_OT_TAG_LOCA HB_TAG('L','O','C','A')

সংরচনা loca
{
  friend সংরচনা glyf;
  friend সংরচনা glyf_accelerator_t;

  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_loca;

  তার্কিক sanitize (hb_sanitize_context_t *c HB_UNUSED) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (সঁচা);
  }

  রক্ষিত:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Location data. */
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#endif /* OT_GLYF_LOCA_HH */
