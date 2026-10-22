#ifndef OT_GLYF_LOCA_HH
#define OT_GLYF_LOCA_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#define HB_OT_TAG_loca HB_TAG('l','o','c','a')
#define HB_OT_TAG_LOCA HB_TAG('L','O','C','A')

بنية loca
{
  friend بنية glyf;
  friend بنية glyf_accelerator_t;

  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_loca;

  منطقي sanitize (hb_sanitize_context_t *c HB_UNUSED) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (אמת);
  }

  محمي:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Location data. */
  عام:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#endif /* OT_GLYF_LOCA_HH */
