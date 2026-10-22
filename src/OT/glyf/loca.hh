#ifndef OT_GLYF_LOCA_HH
#define OT_GLYF_LOCA_HH


#அந்தர்பாவ "../../hb-open-type.hh"


நாமாகாஶ OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#define HB_OT_TAG_loca HB_TAG('l','o','c','a')
#define HB_OT_TAG_LOCA HB_TAG('L','O','C','A')

கட்டமைப்பு loca
{
  friend கட்டமைப்பு glyf;
  friend கட்டமைப்பு glyf_accelerator_t;

  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_loca;

  தருக்க sanitize (hb_sanitize_context_t *c HB_UNUSED) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (உண்மை);
  }

  பாதுகாக்கப்பட்ட:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Location data. */
  பொது:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#endif /* OT_GLYF_LOCA_HH */
