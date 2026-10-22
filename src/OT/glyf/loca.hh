#اگر_تعریف_نشده OT_GLYF_LOCA_HH
#تعریف_کن OT_GLYF_LOCA_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#تعریف_کن HB_OT_برچسب_loca HB_برچسب('l','o','c','a')
#تعریف_کن HB_OT_برچسب_LOCA HB_برچسب('L','O','C','A')

بنية loca
{
  دوست بنية glyf;
  دوست بنية glyf_accelerator_t;

  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_loca;

  منطقي sanitize (hb_sanitize_context_t *c HB_استفادهنشده) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (درست);
  }

  محمي:
  Unsized_Array_از<HBUINT8>
		داده_Z;	/* Location data. */
  عام:
  DEFINE_اندازه_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#پایان_اگر /* OT_GLYF_LOCA_HH */
