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

#جے_بنا_تعریف HB_OT_HDMX_جدول_HH
#تعریف_کرو HB_OT_HDMX_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#تعریف_کرو HB_OT_ٹیگ_hdmx HB_ٹیگ('h','d','m','x')


نطاق OT {


بنية Device_ریکارڈ
{
  ساكن size_t get_ناپ (بنانشانی گنتی)
  { أرجع hb_ceil_منزل_4 (hb_unsigned_mul_جوڑو_saturate (گنتی, HBUINT8::static_ناپ, min_ناپ)); }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  بنانشانی pixel_ناپ,
		  Iterator it,
		  ثابت hb_سمتیہ_t<hb_codepoint_pair_t> new_منزل_old_gid_list,
		  بنانشانی عدد_glyphs)
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend (هذا, عدد_glyphs)))  return_trace (خطأ);

    هذا->pixel_ناپ = pixel_ناپ;
    هذا->max_چوڑائی =
    + it
    | hb_reduce (hb_max, 0u);

    لكل (خودکار &_ : new_منزل_old_gid_list)
      widthsZ[_.پہلا] = *it++;

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی ناپ_Device_ریکارڈ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  c->check_range (هذا, ناپ_Device_ریکارڈ)));
  }

  HBUINT8			pixel_ناپ;	/* Pixel size for following widths (as ppem). */
  HBUINT8			max_چوڑائی;	/* Maximum width. */
  Unsized_Array_دا<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  عام:
  DEFINE_ناپ_UNBOUNDED (2);
};


بنية hdmx
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_hdmx;

  size_t get_ناپ () ثابت
  { أرجع hb_unsigned_mul_جوڑو_saturate (عدد_ریکارڈاں_2, ناپ_Device_ریکارڈ, min_ناپ); }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  بنانشانی نسخہ,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_منزل_old_gid_list,
		  بنانشانی عدد_glyphs)
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend_min ((*هذا))))  return_trace (خطأ);

    هذا->نسخہ = نسخہ;
    هذا->عدد_ریکارڈاں_2 = it.len ();
    هذا->ناپ_Device_ریکارڈ = Device_ریکارڈ::get_ناپ (عدد_glyphs);

    لكل (ثابت hb_شے_type<Iterator>& _ : +it)
      c->شروع_embed<Device_ریکارڈ> ()->serialize (c, _.پہلا, _.second, new_منزل_old_gid_list, عدد_glyphs);

    return_trace (c->successful ());
  }


  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار *hdmx_مفرد = c->serializer->شروع_embed <hdmx> ();

    بنانشانی عدد_داخلہ_glyphs = get_عدد_glyphs ();
    خودکار it =
    + hb_range ((بنانشانی) عدد_ریکارڈاں_2)
    | hb_map ([c, عدد_داخلہ_glyphs, هذا] (بنانشانی _)
	{
	  ثابت Device_ریکارڈ *device_ریکارڈ =
	    &Struct_At_وتھ<Device_ریکارڈ> (&پہلا_Device_ریکارڈ,
					   _ * ناپ_Device_ریکارڈ);
	  خودکار صف =
	    + hb_iter (c->plan->new_منزل_old_gid_list)
	    | hb_map ([عدد_داخلہ_glyphs, device_ریکارڈ] (hb_codepoint_pair_t _)
		      {
			أرجع device_ریکارڈ->widthsZ.as_array (عدد_داخلہ_glyphs) [_.second];
		      })
	    ;
	  أرجع hb_pair ((بنانشانی) device_ریکارڈ->pixel_ناپ, +صف);
	})
    ;

    hdmx_مفرد->serialize (c->serializer, نسخہ, it,
			   c->plan->new_منزل_old_gid_list,
			   c->plan->عدد_output_glyphs ());
    return_trace (سچ);
  }

  بنانشانی get_عدد_glyphs () ثابت
  {
    أرجع ناپ_Device_ریکارڈ - Device_ریکارڈ::min_ناپ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (عدد_ریکارڈاں_2, ناپ_Device_ریکارڈ) &&
                  min_ناپ + عدد_ریکارڈاں_2 * ناپ_Device_ریکارڈ > عدد_ریکارڈاں_2 * ناپ_Device_ریکارڈ &&
		  ناپ_Device_ریکارڈ >= Device_ریکارڈ::min_ناپ &&
		  c->check_range (هذا, get_ناپ ()));
  }

  محمي:
  HBUINT16	نسخہ;	/* Table version number (0) */
  HBUINT16	عدد_ریکارڈاں_2;	/* Number of device records. */
  HBUINT32	ناپ_Device_ریکارڈ;
				/* Size of a device record, 32-bit aligned. */
  Device_ریکارڈ	پہلا_Device_ریکارڈ;
				/* Array of device records. */
  عام:
  DEFINE_ناپ_MIN (8);
};

} /* namespace OT */


#جے_انت /* HB_OT_HDMX_TABLE_HH */
