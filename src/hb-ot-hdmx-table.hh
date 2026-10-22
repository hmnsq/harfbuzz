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

#اگر_غیر_تعریف_شدہ HB_OT_HDMX_جدول_HH
#تعریف_کرو HB_OT_HDMX_جدول_HH

#شامل "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#تعریف_کرو HB_OT_لیبل_hdmx HB_لیبل('h','d','m','x')


نامخلا OT {


ساخت Device_ریکارڈ
{
  ساکن size_t get_سائز (غیرعلامتی گنتی)
  { واپس hb_ceil_مقصد_4 (hb_unsigned_mul_جوڑو_saturate (گنتی, HBUINT8::static_سائز, min_سائز)); }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
		  غیرعلامتی pixel_سائز,
		  Iterator it,
		  ثابت hb_سمتیہ_t<hb_codepoint_pair_t> new_مقصد_old_gid_list,
		  غیرعلامتی عدد_glyphs)
  {
    TRACE_SERIALIZE (یہ);

    اگر (unlikely (!c->extend (یہ, عدد_glyphs)))  return_trace (جھوٹ);

    یہ->pixel_سائز = pixel_سائز;
    یہ->max_چوڑائی =
    + it
    | hb_reduce (hb_max, 0u);

    برائے (خودکار &_ : new_مقصد_old_gid_list)
      widthsZ[_.پہلا] = *it++;

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی سائز_Device_ریکارڈ) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  c->check_range (یہ, سائز_Device_ریکارڈ)));
  }

  HBUINT8			pixel_سائز;	/* Pixel size for following widths (as ppem). */
  HBUINT8			max_چوڑائی;	/* Maximum width. */
  Unsized_Array_کا<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  عوامی:
  DEFINE_سائز_UNBOUNDED (2);
};


ساخت hdmx
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_hdmx;

  size_t get_سائز () ثابت
  { واپس hb_unsigned_mul_جوڑو_saturate (عدد_اندراجات_2, سائز_Device_ریکارڈ, min_سائز); }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
		  غیرعلامتی نسخہ,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_مقصد_old_gid_list,
		  غیرعلامتی عدد_glyphs)
  {
    TRACE_SERIALIZE (یہ);

    اگر (unlikely (!c->extend_min ((*یہ))))  return_trace (جھوٹ);

    یہ->نسخہ = نسخہ;
    یہ->عدد_اندراجات_2 = it.len ();
    یہ->سائز_Device_ریکارڈ = Device_ریکارڈ::get_سائز (عدد_glyphs);

    برائے (ثابت hb_شے_type<Iterator>& _ : +it)
      c->آغاز_embed<Device_ریکارڈ> ()->serialize (c, _.پہلا, _.second, new_مقصد_old_gid_list, عدد_glyphs);

    return_trace (c->successful ());
  }


  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);

    خودکار *hdmx_مفرد = c->serializer->آغاز_embed <hdmx> ();

    غیرعلامتی عدد_داخلہ_glyphs = get_عدد_glyphs ();
    خودکار it =
    + hb_range ((غیرعلامتی) عدد_اندراجات_2)
    | hb_map ([c, عدد_داخلہ_glyphs, یہ] (غیرعلامتی _)
	{
	  ثابت Device_ریکارڈ *device_ریکارڈ =
	    &Struct_At_انحراف<Device_ریکارڈ> (&پہلا_Device_ریکارڈ,
					   _ * سائز_Device_ریکارڈ);
	  خودکار صف =
	    + hb_iter (c->plan->new_مقصد_old_gid_list)
	    | hb_map ([عدد_داخلہ_glyphs, device_ریکارڈ] (hb_codepoint_pair_t _)
		      {
			واپس device_ریکارڈ->widthsZ.as_array (عدد_داخلہ_glyphs) [_.second];
		      })
	    ;
	  واپس hb_pair ((غیرعلامتی) device_ریکارڈ->pixel_سائز, +صف);
	})
    ;

    hdmx_مفرد->serialize (c->serializer, نسخہ, it,
			   c->plan->new_مقصد_old_gid_list,
			   c->plan->عدد_output_glyphs ());
    return_trace (سچ);
  }

  غیرعلامتی get_عدد_glyphs () ثابت
  {
    واپس سائز_Device_ریکارڈ - Device_ریکارڈ::min_سائز;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (عدد_اندراجات_2, سائز_Device_ریکارڈ) &&
                  min_سائز + عدد_اندراجات_2 * سائز_Device_ریکارڈ > عدد_اندراجات_2 * سائز_Device_ریکارڈ &&
		  سائز_Device_ریکارڈ >= Device_ریکارڈ::min_سائز &&
		  c->check_range (یہ, get_سائز ()));
  }

  محفوظ:
  HBUINT16	نسخہ;	/* Table version number (0) */
  HBUINT16	عدد_اندراجات_2;	/* Number of device records. */
  HBUINT32	سائز_Device_ریکارڈ;
				/* Size of a device record, 32-bit aligned. */
  Device_ریکارڈ	پہلا_Device_ریکارڈ;
				/* Array of device records. */
  عوامی:
  DEFINE_سائز_MIN (8);
};

} /* namespace OT */


#اگر_ختم /* HB_OT_HDMX_TABLE_HH */
