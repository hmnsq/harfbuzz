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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_HDMX_جدول_HH
#وصف_ڪريو HB_OT_HDMX_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#وصف_ڪريو HB_OT_ٽيگ_hdmx HB_ٽيگ('h','d','m','x')


نطاق OT {


بنية Device_رڪارڊ
{
  ساكن size_t get_سائيز (بنانشان ڳڻپ)
  { أرجع hb_ceil_منزل_4 (hb_unsigned_mul_جوڙيو_saturate (ڳڻپ, HBUINT8::static_سائيز, min_سائيز)); }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  بنانشان pixel_سائيز,
		  Iterator it,
		  ثابت hb_ويڪٽر_t<hb_codepoint_pair_t> new_منزل_old_gid_list,
		  بنانشان انگ_glyphs)
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend (هذا, انگ_glyphs)))  return_trace (خطأ);

    هذا->pixel_سائيز = pixel_سائيز;
    هذا->max_ويڪر =
    + it
    | hb_reduce (hb_max, 0u);

    لكل (خودڪار &_ : new_منزل_old_gid_list)
      widthsZ[_.پهريون] = *it++;

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c, بنانشان سائيز_Device_رڪارڊ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  c->check_range (هذا, سائيز_Device_رڪارڊ)));
  }

  HBUINT8			pixel_سائيز;	/* Pixel size for following widths (as ppem). */
  HBUINT8			max_ويڪر;	/* Maximum width. */
  Unsized_Array_جو<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  عام:
  DEFINE_سائيز_UNBOUNDED (2);
};


بنية hdmx
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_hdmx;

  size_t get_سائيز () ثابت
  { أرجع hb_unsigned_mul_جوڙيو_saturate (انگ_رڪارڊون_2, سائيز_Device_رڪارڊ, min_سائيز); }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  بنانشان نسخو,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_منزل_old_gid_list,
		  بنانشان انگ_glyphs)
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend_min ((*هذا))))  return_trace (خطأ);

    هذا->نسخو = نسخو;
    هذا->انگ_رڪارڊون_2 = it.len ();
    هذا->سائيز_Device_رڪارڊ = Device_رڪارڊ::get_سائيز (انگ_glyphs);

    لكل (ثابت hb_شي_type<Iterator>& _ : +it)
      c->شروعات_embed<Device_رڪارڊ> ()->serialize (c, _.پهريون, _.second, new_منزل_old_gid_list, انگ_glyphs);

    return_trace (c->successful ());
  }


  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    خودڪار *hdmx_مفرد = c->serializer->شروعات_embed <hdmx> ();

    بنانشان انگ_داخلا_glyphs = get_انگ_glyphs ();
    خودڪار it =
    + hb_range ((بنانشان) انگ_رڪارڊون_2)
    | hb_map ([c, انگ_داخلا_glyphs, هذا] (بنانشان _)
	{
	  ثابت Device_رڪارڊ *device_رڪارڊ =
	    &Struct_At_وٿي<Device_رڪارڊ> (&پهريون_Device_رڪارڊ,
					   _ * سائيز_Device_رڪارڊ);
	  خودڪار صف =
	    + hb_iter (c->plan->new_منزل_old_gid_list)
	    | hb_map ([انگ_داخلا_glyphs, device_رڪارڊ] (hb_codepoint_pair_t _)
		      {
			أرجع device_رڪارڊ->widthsZ.as_array (انگ_داخلا_glyphs) [_.second];
		      })
	    ;
	  أرجع hb_pair ((بنانشان) device_رڪارڊ->pixel_سائيز, +صف);
	})
    ;

    hdmx_مفرد->serialize (c->serializer, نسخو, it,
			   c->plan->new_منزل_old_gid_list,
			   c->plan->انگ_output_glyphs ());
    return_trace (سچ);
  }

  بنانشان get_انگ_glyphs () ثابت
  {
    أرجع سائيز_Device_رڪارڊ - Device_رڪارڊ::min_سائيز;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (انگ_رڪارڊون_2, سائيز_Device_رڪارڊ) &&
                  min_سائيز + انگ_رڪارڊون_2 * سائيز_Device_رڪارڊ > انگ_رڪارڊون_2 * سائيز_Device_رڪارڊ &&
		  سائيز_Device_رڪارڊ >= Device_رڪارڊ::min_سائيز &&
		  c->check_range (هذا, get_سائيز ()));
  }

  محمي:
  HBUINT16	نسخو;	/* Table version number (0) */
  HBUINT16	انگ_رڪارڊون_2;	/* Number of device records. */
  HBUINT32	سائيز_Device_رڪارڊ;
				/* Size of a device record, 32-bit aligned. */
  Device_رڪارڊ	پهريون_Device_رڪارڊ;
				/* Array of device records. */
  عام:
  DEFINE_سائيز_MIN (8);
};

} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_HDMX_TABLE_HH */
