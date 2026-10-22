#ifndef OT_GLYF_LOCA_HH
#define OT_GLYF_LOCA_HH


#અન્તર્ભાવ "../../hb-open-type.hh"


નામાકાશ OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#define HB_OT_TAG_loca HB_TAG('l','o','c','a')
#define HB_OT_TAG_LOCA HB_TAG('L','O','C','A')

સંરચના loca
{
  friend સંરચના glyf;
  friend સંરચના glyf_accelerator_t;

  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_loca;

  તાર્કિક sanitize (hb_sanitize_context_t *c HB_UNUSED) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (સાચું);
  }

  સુરક્ષિત:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Location data. */
  જાહેર:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#endif /* OT_GLYF_LOCA_HH */
