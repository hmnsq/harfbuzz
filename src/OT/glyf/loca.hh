#ifndef OT_GLYF_LOCA_HH
#define OT_GLYF_LOCA_HH


#ଅନ୍ତର୍ଭାଵ "../../hb-open-type.hh"


ନାମାକାଶ OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#define HB_OT_TAG_loca HB_TAG('l','o','c','a')
#define HB_OT_TAG_LOCA HB_TAG('L','O','C','A')

ସଂରଚନା loca
{
  friend ସଂରଚନା glyf;
  friend ସଂରଚନା glyf_accelerator_t;

  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_loca;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c HB_UNUSED) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (ସତ୍ୟ);
  }

  ସୁରକ୍ଷିତ:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Location data. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#endif /* OT_GLYF_LOCA_HH */
