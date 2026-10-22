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

#ifndef HB_OT_HDMX_TABLE_HH
#define HB_OT_HDMX_TABLE_HH

#شامل "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#define HB_OT_TAG_hdmx HB_TAG('h','d','m','x')


فضاینام OT {


ساختار DeviceRecord
{
  ایستا size_t get_size (unsigned شمار)
  { بازگشت hb_ceil_to_4 (hb_unsigned_mul_add_saturate (شمار, HBUINT8::static_size, min_size)); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
		  unsigned pixelSize,
		  Iterator it,
		  ثابت hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned رقم_glyphs)
  {
    TRACE_SERIALIZE (این);

    اگر (unlikely (!c->extend (این, رقم_glyphs)))  return_trace (نادرست);

    این->pixelSize = pixelSize;
    این->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    برای (auto &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c, unsigned sizeDeviceRecord) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  hb_barrier () &&
			  c->check_range (این, sizeDeviceRecord)));
  }

  HBUINT8			pixelSize;	/* Pixel size for following widths (as ppem). */
  HBUINT8			maxWidth;	/* Maximum width. */
  UnsizedArrayOf<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  عمومی:
  DEFINE_SIZE_UNBOUNDED (2);
};


ساختار hdmx
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_hdmx;

  size_t get_size () ثابت
  { بازگشت hb_unsigned_mul_add_saturate (رقم_Records, sizeDeviceRecord, min_size); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
		  unsigned version,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned رقم_glyphs)
  {
    TRACE_SERIALIZE (این);

    اگر (unlikely (!c->extend_min ((*این))))  return_trace (نادرست);

    این->version = version;
    این->رقم_Records = it.len ();
    این->sizeDeviceRecord = DeviceRecord::get_size (رقم_glyphs);

    برای (ثابت hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, رقم_glyphs);

    return_trace (c->successful ());
  }


  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);

    auto *hdmx_اول = c->serializer->start_embed <hdmx> ();

    unsigned رقم_درون_داد_glyphs = get_رقم_glyphs ();
    auto it =
    + hb_range ((unsigned) رقم_Records)
    | hb_map ([c, رقم_درون_داد_glyphs, این] (unsigned _)
	{
	  ثابت DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * sizeDeviceRecord);
	  auto row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([رقم_درون_داد_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			بازگشت device_record->widthsZ.as_array (رقم_درون_داد_glyphs) [_.second];
		      })
	    ;
	  بازگشت hb_pair ((unsigned) device_record->pixelSize, +row);
	})
    ;

    hdmx_اول->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->رقم_output_glyphs ());
    return_trace (درست);
  }

  unsigned get_رقم_glyphs () ثابت
  {
    بازگشت sizeDeviceRecord - DeviceRecord::min_size;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (رقم_Records, sizeDeviceRecord) &&
                  min_size + رقم_Records * sizeDeviceRecord > رقم_Records * sizeDeviceRecord &&
		  sizeDeviceRecord >= DeviceRecord::min_size &&
		  c->check_range (این, get_size ()));
  }

  محافظتشده:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	رقم_Records;	/* Number of device records. */
  HBUINT32	sizeDeviceRecord;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  عمومی:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_HDMX_TABLE_HH */
