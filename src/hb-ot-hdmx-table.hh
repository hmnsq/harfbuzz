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

#که_نه_تعریف_شوی HB_OT_HDMX_جدول_HH
#تعریف_کړه HB_OT_HDMX_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#تعریف_کړه HB_OT_نښان_hdmx HB_نښان('h','d','m','x')


نطاق OT {


بنية Device_ریکارډ
{
  ساكن size_t get_اندازه (بینښه شمېر)
  { أرجع hb_ceil_منزل_4 (hb_unsigned_mul_جمع_کړه_saturate (شمېر, HBUINT8::static_اندازه, min_اندازه)); }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  بینښه pixel_اندازه,
		  Iterator it,
		  ثابت hb_ویکټور_t<hb_codepoint_pair_t> new_منزل_old_gid_list,
		  بینښه عدد_glyphs)
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend (هذا, عدد_glyphs)))  return_trace (خطأ);

    هذا->pixel_اندازه = pixel_اندازه;
    هذا->max_پلنوالی =
    + it
    | hb_reduce (hb_max, 0u);

    لكل (خپلکاری &_ : new_منزل_old_gid_list)
      widthsZ[_.لومړی] = *it++;

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c, بینښه اندازه_Device_ریکارډ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  c->check_range (هذا, اندازه_Device_ریکارډ)));
  }

  HBUINT8			pixel_اندازه;	/* Pixel size for following widths (as ppem). */
  HBUINT8			max_پلنوالی;	/* Maximum width. */
  Unsized_Array_د<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  عام:
  DEFINE_اندازه_UNBOUNDED (2);
};


بنية hdmx
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_hdmx;

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کړه_saturate (عدد_ریکارډونه_2, اندازه_Device_ریکارډ, min_اندازه); }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  بینښه نسخه,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_منزل_old_gid_list,
		  بینښه عدد_glyphs)
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend_min ((*هذا))))  return_trace (خطأ);

    هذا->نسخه = نسخه;
    هذا->عدد_ریکارډونه_2 = it.len ();
    هذا->اندازه_Device_ریکارډ = Device_ریکارډ::get_اندازه (عدد_glyphs);

    لكل (ثابت hb_توکی_type<Iterator>& _ : +it)
      c->پیل_embed<Device_ریکارډ> ()->serialize (c, _.لومړی, _.second, new_منزل_old_gid_list, عدد_glyphs);

    return_trace (c->successful ());
  }


  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    خپلکاری *hdmx_اولي = c->serializer->پیل_embed <hdmx> ();

    بینښه عدد_ننوت_glyphs = get_عدد_glyphs ();
    خپلکاری it =
    + hb_range ((بینښه) عدد_ریکارډونه_2)
    | hb_map ([c, عدد_ننوت_glyphs, هذا] (بینښه _)
	{
	  ثابت Device_ریکارډ *device_ریکارډ =
	    &Struct_At_دننه_واټن<Device_ریکارډ> (&لومړی_Device_ریکارډ,
					   _ * اندازه_Device_ریکارډ);
	  خپلکاری لیکه =
	    + hb_iter (c->plan->new_منزل_old_gid_list)
	    | hb_map ([عدد_ننوت_glyphs, device_ریکارډ] (hb_codepoint_pair_t _)
		      {
			أرجع device_ریکارډ->widthsZ.as_array (عدد_ننوت_glyphs) [_.second];
		      })
	    ;
	  أرجع hb_pair ((بینښه) device_ریکارډ->pixel_اندازه, +لیکه);
	})
    ;

    hdmx_اولي->serialize (c->serializer, نسخه, it,
			   c->plan->new_منزل_old_gid_list,
			   c->plan->عدد_output_glyphs ());
    return_trace (ریښتیا);
  }

  بینښه get_عدد_glyphs () ثابت
  {
    أرجع اندازه_Device_ریکارډ - Device_ریکارډ::min_اندازه;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (عدد_ریکارډونه_2, اندازه_Device_ریکارډ) &&
                  min_اندازه + عدد_ریکارډونه_2 * اندازه_Device_ریکارډ > عدد_ریکارډونه_2 * اندازه_Device_ریکارډ &&
		  اندازه_Device_ریکارډ >= Device_ریکارډ::min_اندازه &&
		  c->check_range (هذا, get_اندازه ()));
  }

  محمي:
  HBUINT16	نسخه;	/* Table version number (0) */
  HBUINT16	عدد_ریکارډونه_2;	/* Number of device records. */
  HBUINT32	اندازه_Device_ریکارډ;
				/* Size of a device record, 32-bit aligned. */
  Device_ریکارډ	لومړی_Device_ریکارډ;
				/* Array of device records. */
  عام:
  DEFINE_اندازه_MIN (8);
};

} /* namespace OT */


#د_که_پای /* HB_OT_HDMX_TABLE_HH */
