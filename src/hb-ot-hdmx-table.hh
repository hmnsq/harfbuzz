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


সংরচনা DeviceRecord
{
  স্থির size_t get_size (unsigned গুনতি)
  { প্রত্যাগম hb_ceil_to_4 (hb_unsigned_mul_add_saturate (গুনতি, HBUINT8::static_size, min_size)); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  তার্কিক serialize (hb_serialize_context_t *c,
		  unsigned pixelSize,
		  Iterator it,
		  ধ্রুব hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned অঙ্ক_glyphs)
  {
    TRACE_SERIALIZE (এতত্);

    যদি (unlikely (!c->extend (এতত্, অঙ্ক_glyphs)))  return_trace (অসত্য);

    এতত্->pixelSize = pixelSize;
    এতত্->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    প্রতি (auto &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (সত্য);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned sizeDeviceRecord) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  hb_barrier () &&
			  c->check_range (এতত্, sizeDeviceRecord)));
  }

  HBUINT8			pixelSize;	/* Pixel size for following widths (as ppem). */
  HBUINT8			maxWidth;	/* Maximum width. */
  UnsizedArrayOf<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  সার্বজনিক:
  DEFINE_SIZE_UNBOUNDED (2);
};


সংরচনা hdmx
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_hdmx;

  size_t get_size () ধ্রুব
  { প্রত্যাগম hb_unsigned_mul_add_saturate (অঙ্ক_Records, sizeDeviceRecord, min_size); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  তার্কিক serialize (hb_serialize_context_t *c,
		  unsigned version,
		  Iterator it,
		  hb_array_t<ধ্রুব hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned অঙ্ক_glyphs)
  {
    TRACE_SERIALIZE (এতত্);

    যদি (unlikely (!c->extend_min ((*এতত্))))  return_trace (অসত্য);

    এতত্->version = version;
    এতত্->অঙ্ক_Records = it.len ();
    এতত্->sizeDeviceRecord = DeviceRecord::get_size (অঙ্ক_glyphs);

    প্রতি (ধ্রুব hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, অঙ্ক_glyphs);

    return_trace (c->successful ());
  }


  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);

    auto *hdmx_মৌলিক = c->serializer->start_embed <hdmx> ();

    unsigned অঙ্ক_প্রবেশ_glyphs = get_অঙ্ক_glyphs ();
    auto it =
    + hb_range ((unsigned) অঙ্ক_Records)
    | hb_map ([c, অঙ্ক_প্রবেশ_glyphs, এতত্] (unsigned _)
	{
	  ধ্রুব DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * sizeDeviceRecord);
	  auto row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([অঙ্ক_প্রবেশ_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			প্রত্যাগম device_record->widthsZ.as_array (অঙ্ক_প্রবেশ_glyphs) [_.second];
		      })
	    ;
	  প্রত্যাগম hb_pair ((unsigned) device_record->pixelSize, +row);
	})
    ;

    hdmx_মৌলিক->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->অঙ্ক_output_glyphs ());
    return_trace (সত্য);
  }

  unsigned get_অঙ্ক_glyphs () ধ্রুব
  {
    প্রত্যাগম sizeDeviceRecord - DeviceRecord::min_size;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (অঙ্ক_Records, sizeDeviceRecord) &&
                  min_size + অঙ্ক_Records * sizeDeviceRecord > অঙ্ক_Records * sizeDeviceRecord &&
		  sizeDeviceRecord >= DeviceRecord::min_size &&
		  c->check_range (এতত্, get_size ()));
  }

  রক্ষিত:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	অঙ্ক_Records;	/* Number of device records. */
  HBUINT32	sizeDeviceRecord;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  সার্বজনিক:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_HDMX_TABLE_HH */
