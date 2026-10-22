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

#अन्तर्भाव "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#define HB_OT_TAG_hdmx HB_TAG('h','d','m','x')


नामाकाश OT {


संरचना DeviceRecord
{
  स्थिर माप_प्रकार get_size (अचिन्हित मोजणी)
  { परत hb_ceil_to_4 (hb_unsigned_mul_add_saturate (मोजणी, HBUINT8::static_size, min_size)); }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize (hb_serialize_context_t *c,
		  अचिन्हित pixelSize,
		  Iterator it,
		  स्थिरांक hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  अचिन्हित अंक_glyphs)
  {
    TRACE_SERIALIZE (हे);

    जर (unlikely (!c->extend (हे, अंक_glyphs)))  return_trace (असत्य);

    हे->pixelSize = pixelSize;
    हे->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    साठी (स्वतः &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित sizeDeviceRecord) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  hb_barrier () &&
			  c->check_range (हे, sizeDeviceRecord)));
  }

  HBUINT8			pixelSize;	/* Pixel size for following widths (as ppem). */
  HBUINT8			maxWidth;	/* Maximum width. */
  UnsizedArrayOf<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  सार्वजनिक:
  DEFINE_SIZE_UNBOUNDED (2);
};


संरचना hdmx
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_hdmx;

  माप_प्रकार get_size () स्थिरांक
  { परत hb_unsigned_mul_add_saturate (अंक_Records, sizeDeviceRecord, min_size); }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize (hb_serialize_context_t *c,
		  अचिन्हित version,
		  Iterator it,
		  hb_array_t<स्थिरांक hb_codepoint_pair_t> new_to_old_gid_list,
		  अचिन्हित अंक_glyphs)
  {
    TRACE_SERIALIZE (हे);

    जर (unlikely (!c->extend_min ((*हे))))  return_trace (असत्य);

    हे->version = version;
    हे->अंक_Records = it.len ();
    हे->sizeDeviceRecord = DeviceRecord::get_size (अंक_glyphs);

    साठी (स्थिरांक hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, अंक_glyphs);

    return_trace (c->successful ());
  }


  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);

    स्वतः *hdmx_मूळसंख्या = c->serializer->start_embed <hdmx> ();

    अचिन्हित अंक_आदान_glyphs = get_अंक_glyphs ();
    स्वतः it =
    + hb_range ((अचिन्हित) अंक_Records)
    | hb_map ([c, अंक_आदान_glyphs, हे] (अचिन्हित _)
	{
	  स्थिरांक DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * sizeDeviceRecord);
	  स्वतः row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([अंक_आदान_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			परत device_record->widthsZ.as_array (अंक_आदान_glyphs) [_.second];
		      })
	    ;
	  परत hb_pair ((अचिन्हित) device_record->pixelSize, +row);
	})
    ;

    hdmx_मूळसंख्या->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->अंक_output_glyphs ());
    return_trace (सत्य);
  }

  अचिन्हित get_अंक_glyphs () स्थिरांक
  {
    परत sizeDeviceRecord - DeviceRecord::min_size;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (अंक_Records, sizeDeviceRecord) &&
                  min_size + अंक_Records * sizeDeviceRecord > अंक_Records * sizeDeviceRecord &&
		  sizeDeviceRecord >= DeviceRecord::min_size &&
		  c->check_range (हे, get_size ()));
  }

  संरक्षित:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	अंक_Records;	/* Number of device records. */
  HBUINT32	sizeDeviceRecord;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_HDMX_TABLE_HH */
