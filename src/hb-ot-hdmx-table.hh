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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_HDMX_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_HDMX_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_hdmx HB_ਟੈਗ('h','d','m','x')


ਨਾਮਾਕਾਸ਼ OT {


ਬਣਤਰ Device_ਰਿਕਾਰਡ
{
  ਸਥਿਰ size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
  { ਵਾਪਸ hb_ceil_ਮੰਜ਼ਿਲ_4 (hb_unsigned_mul_ਜੋੜੋ_saturate (ਗਿਣਤੀ_2, HBUINT8::static_ਮਾਪ, min_ਮਾਪ)); }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ pixel_ਮਾਪ,
		  Iterator it,
		  ਅਚਲ hb_ਸਦਿਸ਼_t<hb_codepoint_pair_t> new_ਮੰਜ਼ਿਲ_old_gid_list,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs)
  {
    TRACE_SERIALIZE (ਇਹ);

    ਜੇ (unlikely (!c->extend (ਇਹ, ਅੰਕ_glyphs)))  return_trace (ਝੂਠ);

    ਇਹ->pixel_ਮਾਪ = pixel_ਮਾਪ;
    ਇਹ->max_ਚੌੜਾਈ =
    + it
    | hb_reduce (hb_max, 0u);

    ਲਈ (ਸਵੈਚਾਲਿਤ &_ : new_ਮੰਜ਼ਿਲ_old_gid_list)
      widthsZ[_.ਪਹਿਲਾ] = *it++;

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ_Device_ਰਿਕਾਰਡ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  c->check_range (ਇਹ, ਮਾਪ_Device_ਰਿਕਾਰਡ)));
  }

  HBUINT8			pixel_ਮਾਪ;	/* Pixel size for following widths (as ppem). */
  HBUINT8			max_ਚੌੜਾਈ;	/* Maximum width. */
  Unsized_Array_ਦਾ<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNBOUNDED (2);
};


ਬਣਤਰ hdmx
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_hdmx;

  size_t get_ਮਾਪ () ਅਚਲ
  { ਵਾਪਸ hb_unsigned_mul_ਜੋੜੋ_saturate (ਅੰਕ_ਰਿਕਾਰਡਾਂ_2, ਮਾਪ_Device_ਰਿਕਾਰਡ, min_ਮਾਪ); }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਸਕਰਣ,
		  Iterator it,
		  hb_array_t<ਅਚਲ hb_codepoint_pair_t> new_ਮੰਜ਼ਿਲ_old_gid_list,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs)
  {
    TRACE_SERIALIZE (ਇਹ);

    ਜੇ (unlikely (!c->extend_min ((*ਇਹ))))  return_trace (ਝੂਠ);

    ਇਹ->ਸੰਸਕਰਣ = ਸੰਸਕਰਣ;
    ਇਹ->ਅੰਕ_ਰਿਕਾਰਡਾਂ_2 = it.len ();
    ਇਹ->ਮਾਪ_Device_ਰਿਕਾਰਡ = Device_ਰਿਕਾਰਡ::get_ਮਾਪ (ਅੰਕ_glyphs);

    ਲਈ (ਅਚਲ hb_ਵਸਤੂ_type<Iterator>& _ : +it)
      c->ਸ਼ੁਰੂ_embed<Device_ਰਿਕਾਰਡ> ()->serialize (c, _.ਪਹਿਲਾ, _.second, new_ਮੰਜ਼ਿਲ_old_gid_list, ਅੰਕ_glyphs);

    return_trace (c->successful ());
  }


  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਸਵੈਚਾਲਿਤ *hdmx_ਅਭਾਜ = c->serializer->ਸ਼ੁਰੂ_embed <hdmx> ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_ਨਿਵੇਸ਼_glyphs = get_ਅੰਕ_glyphs ();
    ਸਵੈਚਾਲਿਤ it =
    + hb_range ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅੰਕ_ਰਿਕਾਰਡਾਂ_2)
    | hb_map ([c, ਅੰਕ_ਨਿਵੇਸ਼_glyphs, ਇਹ] (ਬਿਨਾਨਿਸ਼ਾਨੀ _)
	{
	  ਅਚਲ Device_ਰਿਕਾਰਡ *device_ਰਿਕਾਰਡ =
	    &Struct_At_ਵਿੱਥ<Device_ਰਿਕਾਰਡ> (&ਪਹਿਲਾ_Device_ਰਿਕਾਰਡ,
					   _ * ਮਾਪ_Device_ਰਿਕਾਰਡ);
	  ਸਵੈਚਾਲਿਤ ਪੰਕਤੀ =
	    + hb_iter (c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
	    | hb_map ([ਅੰਕ_ਨਿਵੇਸ਼_glyphs, device_ਰਿਕਾਰਡ] (hb_codepoint_pair_t _)
		      {
			ਵਾਪਸ device_ਰਿਕਾਰਡ->widthsZ.as_array (ਅੰਕ_ਨਿਵੇਸ਼_glyphs) [_.second];
		      })
	    ;
	  ਵਾਪਸ hb_pair ((ਬਿਨਾਨਿਸ਼ਾਨੀ) device_ਰਿਕਾਰਡ->pixel_ਮਾਪ, +ਪੰਕਤੀ);
	})
    ;

    hdmx_ਅਭਾਜ->serialize (c->serializer, ਸੰਸਕਰਣ, it,
			   c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list,
			   c->plan->ਅੰਕ_output_glyphs ());
    return_trace (ਸੱਚ);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਅੰਕ_glyphs () ਅਚਲ
  {
    ਵਾਪਸ ਮਾਪ_Device_ਰਿਕਾਰਡ - Device_ਰਿਕਾਰਡ::min_ਮਾਪ;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (ਅੰਕ_ਰਿਕਾਰਡਾਂ_2, ਮਾਪ_Device_ਰਿਕਾਰਡ) &&
                  min_ਮਾਪ + ਅੰਕ_ਰਿਕਾਰਡਾਂ_2 * ਮਾਪ_Device_ਰਿਕਾਰਡ > ਅੰਕ_ਰਿਕਾਰਡਾਂ_2 * ਮਾਪ_Device_ਰਿਕਾਰਡ &&
		  ਮਾਪ_Device_ਰਿਕਾਰਡ >= Device_ਰਿਕਾਰਡ::min_ਮਾਪ &&
		  c->check_range (ਇਹ, get_ਮਾਪ ()));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸੰਸਕਰਣ;	/* Table version number (0) */
  HBUINT16	ਅੰਕ_ਰਿਕਾਰਡਾਂ_2;	/* Number of device records. */
  HBUINT32	ਮਾਪ_Device_ਰਿਕਾਰਡ;
				/* Size of a device record, 32-bit aligned. */
  Device_ਰਿਕਾਰਡ	ਪਹਿਲਾ_Device_ਰਿਕਾਰਡ;
				/* Array of device records. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8);
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_HDMX_TABLE_HH */
