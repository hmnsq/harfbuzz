#अगर_अघोषित OT_GLYF_LOCA_HH
#घोषणा OT_GLYF_LOCA_HH


#समावेश "../../hb-open-type.hh"


namespace OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#घोषणा HB_OT_TAG_loca HB_TAG('l','o','c','a')
#घोषणा HB_OT_TAG_LOCA HB_TAG('L','O','C','A')

काष्ठा loca
{
  मित्र काष्ठा glyf;
  मित्र काष्ठा glyf_accelerator_t;

  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_loca;

  bool sanitize (hb_sanitize_context_t *c HB_UNUSED) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (true);
  }

  रक्षित:
  UnsizedArrayOf<HBUINT8>
		तथ्य_Z;	/* Location data. */
  खुला:
  DEFINE_आकार_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#पूर्ण_अगर /* OT_GLYF_LOCA_HH */
