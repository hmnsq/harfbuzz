#جے_بنا_تعریف OT_GLYF_LOCA_HH
#تعریف_کرو OT_GLYF_LOCA_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#تعریف_کرو HB_OT_ٹیگ_loca HB_ٹیگ('l','o','c','a')
#تعریف_کرو HB_OT_ٹیگ_LOCA HB_ٹیگ('L','O','C','A')

بنية loca
{
  یار بنية glyf;
  یار بنية glyf_accelerator_t;

  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_loca;

  منطقي sanitize (hb_sanitize_context_t *c HB_بن_ورتیا) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (سچ);
  }

  محمي:
  Unsized_Array_دا<HBUINT8>
		انکڑے_Z;	/* Location data. */
  عام:
  DEFINE_ناپ_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#جے_انت /* OT_GLYF_LOCA_HH */
