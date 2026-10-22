#ifndef OT_GLYF_LOCA_HH
#define OT_GLYF_LOCA_HH


#شامل "../../hb-open-type.hh"


فضاینام OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#define HB_OT_TAG_loca HB_TAG('l','o','c','a')
#define HB_OT_TAG_LOCA HB_TAG('L','O','C','A')

ساختار loca
{
  friend ساختار glyf;
  friend ساختار glyf_accelerator_t;

  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_loca;

  منطقی sanitize (hb_sanitize_context_t *c HB_UNUSED) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (درست);
  }

  محافظتشده:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Location data. */
  عمومی:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#endif /* OT_GLYF_LOCA_HH */
