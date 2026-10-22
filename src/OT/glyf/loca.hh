#جيڪڏهن_وصف_نه_ٿيل OT_GLYF_LOCA_HH
#وصف_ڪريو OT_GLYF_LOCA_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#وصف_ڪريو HB_OT_ٽيگ_loca HB_ٽيگ('l','o','c','a')
#وصف_ڪريو HB_OT_ٽيگ_LOCA HB_ٽيگ('L','O','C','A')

بنية loca
{
  دوست بنية glyf;
  دوست بنية glyf_accelerator_t;

  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_loca;

  منطقي sanitize (hb_sanitize_context_t *c HB_اڻ_استعمال_ٿيل) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (سچ);
  }

  محمي:
  Unsized_Array_جو<HBUINT8>
		معلومات_Z;	/* Location data. */
  عام:
  DEFINE_سائيز_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#جيڪڏهن_پورو /* OT_GLYF_LOCA_HH */
