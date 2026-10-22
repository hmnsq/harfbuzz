#که_نه_تعریف_شوی OT_GLYF_LOCA_HH
#تعریف_کړه OT_GLYF_LOCA_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#تعریف_کړه HB_OT_نښان_loca HB_نښان('l','o','c','a')
#تعریف_کړه HB_OT_نښان_LOCA HB_نښان('L','O','C','A')

بنية loca
{
  ملګری بنية glyf;
  ملګری بنية glyf_accelerator_t;

  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_loca;

  منطقي sanitize (hb_sanitize_context_t *c HB_ناکارول_شوی) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ریښتیا);
  }

  محمي:
  Unsized_Array_د<HBUINT8>
		معلومات_Z;	/* Location data. */
  عام:
  DEFINE_اندازه_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#د_که_پای /* OT_GLYF_LOCA_HH */
