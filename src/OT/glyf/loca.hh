#اگر_غیر_تعریف_شدہ OT_GLYF_LOCA_HH
#تعریف_کرو OT_GLYF_LOCA_HH


#شامل "../../hb-open-type.hh"


نامخلا OT {


/*
 * loca -- Index to Location
 * https://docs.microsoft.com/en-us/typography/opentype/spec/loca
 */
#تعریف_کرو HB_OT_لیبل_loca HB_لیبل('l','o','c','a')
#تعریف_کرو HB_OT_لیبل_LOCA HB_لیبل('L','O','C','A')

ساخت loca
{
  دوست ساخت glyf;
  دوست ساخت glyf_accelerator_t;

  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_loca;

  منطقی sanitize (hb_sanitize_context_t *c HB_غیر_مستعمل) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (سچ);
  }

  محفوظ:
  Unsized_Array_کا<HBUINT8>
		ڈیٹا_Z;	/* Location data. */
  عوامی:
  DEFINE_سائز_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};


} /* namespace OT */


#اگر_ختم /* OT_GLYF_LOCA_HH */
