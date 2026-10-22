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

#অন্তর্ভাব "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#define HB_OT_TAG_hdmx HB_TAG('h','d','m','x')


নামাকাশ OT {


কাঠামো DeviceRecord
{
  স্থির size_t get_আকার (unsigned গুনতি)
  { ফেরত hb_ceil_to_4 (hb_unsigned_mul_add_saturate (গুনতি, HBUINT8::static_আকার, min_আকার)); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  unsigned pixel_আকার,
		  Iterator it,
		  ধ্রুবক hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned অঙ্ক_glyphs)
  {
    TRACE_SERIALIZE (এই);

    যদি (unlikely (!c->extend (এই, অঙ্ক_glyphs)))  return_trace (মিথ্যা);

    এই->pixel_আকার = pixel_আকার;
    এই->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    জন্য (auto &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned আকার_Device_Record) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  c->check_range (এই, আকার_Device_Record)));
  }

  HBUINT8			pixel_আকার;	/* Pixel size for following widths (as ppem). */
  HBUINT8			maxWidth;	/* Maximum width. */
  UnsizedArrayOf<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  সর্বজনীন:
  DEFINE_আকার_UNBOUNDED (2);
};


কাঠামো hdmx
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_hdmx;

  size_t get_আকার () ধ্রুবক
  { ফেরত hb_unsigned_mul_add_saturate (অঙ্ক_Records, আকার_Device_Record, min_আকার); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  unsigned version,
		  Iterator it,
		  hb_array_t<ধ্রুবক hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned অঙ্ক_glyphs)
  {
    TRACE_SERIALIZE (এই);

    যদি (unlikely (!c->extend_min ((*এই))))  return_trace (মিথ্যা);

    এই->version = version;
    এই->অঙ্ক_Records = it.len ();
    এই->আকার_Device_Record = DeviceRecord::get_আকার (অঙ্ক_glyphs);

    জন্য (ধ্রুবক hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, অঙ্ক_glyphs);

    return_trace (c->successful ());
  }


  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    auto *hdmx_মৌলিক = c->serializer->start_embed <hdmx> ();

    unsigned অঙ্ক_প্রবেশ_glyphs = get_অঙ্ক_glyphs ();
    auto it =
    + hb_range ((unsigned) অঙ্ক_Records)
    | hb_map ([c, অঙ্ক_প্রবেশ_glyphs, এই] (unsigned _)
	{
	  ধ্রুবক DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * আকার_Device_Record);
	  auto row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([অঙ্ক_প্রবেশ_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			ফেরত device_record->widthsZ.as_array (অঙ্ক_প্রবেশ_glyphs) [_.second];
		      })
	    ;
	  ফেরত hb_pair ((unsigned) device_record->pixel_আকার, +row);
	})
    ;

    hdmx_মৌলিক->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->অঙ্ক_output_glyphs ());
    return_trace (সত্য);
  }

  unsigned get_অঙ্ক_glyphs () ধ্রুবক
  {
    ফেরত আকার_Device_Record - DeviceRecord::min_আকার;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (অঙ্ক_Records, আকার_Device_Record) &&
                  min_আকার + অঙ্ক_Records * আকার_Device_Record > অঙ্ক_Records * আকার_Device_Record &&
		  আকার_Device_Record >= DeviceRecord::min_আকার &&
		  c->check_range (এই, get_আকার ()));
  }

  সুরক্ষিত:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	অঙ্ক_Records;	/* Number of device records. */
  HBUINT32	আকার_Device_Record;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  সর্বজনীন:
  DEFINE_আকার_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_HDMX_TABLE_HH */
