/*
 * Copyright © 2018  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Garret Rieger
 */

#اگر_تعریف_نشده HB_OT_HDMX_جدول_HH
#تعریف_کن HB_OT_HDMX_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#تعریف_کن HB_OT_برچسب_hdmx HB_برچسب('h','d','m','x')


نطاق OT {


بنية Device_رکورد
{
  ساكن size_t get_اندازه (بیعلامت شمار)
  { أرجع hb_ceil_مقصد_4 (hb_unsigned_mul_جمع_کن_saturate (شمار, HBUINT8::static_اندازه, min_اندازه)); }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  بیعلامت pixel_اندازه,
		  Iterator it,
		  ثابت hb_وکتور_t<hb_codepoint_pair_t> new_مقصد_old_gid_list,
		  بیعلامت رقم_glyphs)
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend (هذا, رقم_glyphs)))  return_trace (خطأ);

    هذا->pixel_اندازه = pixel_اندازه;
    هذا->max_عرض =
    + it
    | hb_reduce (hb_max, 0u);

    لكل (خودکار &_ : new_مقصد_old_gid_list)
      widthsZ[_.اولی] = *it++;

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت اندازه_Device_رکورد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  c->check_range (هذا, اندازه_Device_رکورد)));
  }

  HBUINT8			pixel_اندازه;	/* Pixel size for following widths (as ppem). */
  HBUINT8			max_عرض;	/* Maximum width. */
  Unsized_Array_از<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  عام:
  DEFINE_اندازه_UNBOUNDED (2);
};


بنية hdmx
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_hdmx;

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کن_saturate (رقم_رکوردها_2, اندازه_Device_رکورد, min_اندازه); }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  بیعلامت نسخه,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_مقصد_old_gid_list,
		  بیعلامت رقم_glyphs)
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend_min ((*هذا))))  return_trace (خطأ);

    هذا->نسخه = نسخه;
    هذا->رقم_رکوردها_2 = it.len ();
    هذا->اندازه_Device_رکورد = Device_رکورد::get_اندازه (رقم_glyphs);

    لكل (ثابت hb_قلم_type<Iterator>& _ : +it)
      c->شروع_embed<Device_رکورد> ()->serialize (c, _.اولی, _.second, new_مقصد_old_gid_list, رقم_glyphs);

    return_trace (c->successful ());
  }


  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار *hdmx_اول = c->serializer->شروع_embed <hdmx> ();

    بیعلامت رقم_درون_داد_glyphs = get_رقم_glyphs ();
    خودکار it =
    + hb_range ((بیعلامت) رقم_رکوردها_2)
    | hb_map ([c, رقم_درون_داد_glyphs, هذا] (بیعلامت _)
	{
	  ثابت Device_رکورد *device_رکورد =
	    &Struct_At_جابه_جایی<Device_رکورد> (&اولی_Device_رکورد,
					   _ * اندازه_Device_رکورد);
	  خودکار ردیف =
	    + hb_iter (c->plan->new_مقصد_old_gid_list)
	    | hb_map ([رقم_درون_داد_glyphs, device_رکورد] (hb_codepoint_pair_t _)
		      {
			أرجع device_رکورد->widthsZ.as_array (رقم_درون_داد_glyphs) [_.second];
		      })
	    ;
	  أرجع hb_pair ((بیعلامت) device_رکورد->pixel_اندازه, +ردیف);
	})
    ;

    hdmx_اول->serialize (c->serializer, نسخه, it,
			   c->plan->new_مقصد_old_gid_list,
			   c->plan->رقم_output_glyphs ());
    return_trace (درست);
  }

  بیعلامت get_رقم_glyphs () ثابت
  {
    أرجع اندازه_Device_رکورد - Device_رکورد::min_اندازه;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (رقم_رکوردها_2, اندازه_Device_رکورد) &&
                  min_اندازه + رقم_رکوردها_2 * اندازه_Device_رکورد > رقم_رکوردها_2 * اندازه_Device_رکورد &&
		  اندازه_Device_رکورد >= Device_رکورد::min_اندازه &&
		  c->check_range (هذا, get_اندازه ()));
  }

  محمي:
  HBUINT16	نسخه;	/* Table version number (0) */
  HBUINT16	رقم_رکوردها_2;	/* Number of device records. */
  HBUINT32	اندازه_Device_رکورد;
				/* Size of a device record, 32-bit aligned. */
  Device_رکورد	اولی_Device_رکورد;
				/* Array of device records. */
  عام:
  DEFINE_اندازه_MIN (8);
};

} /* namespace OT */


#پایان_اگر /* HB_OT_HDMX_TABLE_HH */
