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
  स्थिर माप_प्रकार get_size (अचिन्हित गन्ती)
  { फर्काऊ hb_ceil_to_4 (hb_unsigned_mul_add_saturate (गन्ती, HBUINT8::static_size, min_size)); }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize (hb_serialize_context_t *c,
		  अचिन्हित pixelSize,
		  Iterator it,
		  अचर hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  अचिन्हित अङ्क_glyphs)
  {
    TRACE_SERIALIZE (एतत्);

    यदि (unlikely (!c->extend (एतत्, अङ्क_glyphs)))  return_trace (असत्य);

    एतत्->pixelSize = pixelSize;
    एतत्->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    लागि (स्वतः &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित sizeDeviceRecord) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
			  c->check_range (एतत्, sizeDeviceRecord)));
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

  माप_प्रकार get_size () अचर
  { फर्काऊ hb_unsigned_mul_add_saturate (अङ्क_Records, sizeDeviceRecord, min_size); }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize (hb_serialize_context_t *c,
		  अचिन्हित version,
		  Iterator it,
		  hb_array_t<अचर hb_codepoint_pair_t> new_to_old_gid_list,
		  अचिन्हित अङ्क_glyphs)
  {
    TRACE_SERIALIZE (एतत्);

    यदि (unlikely (!c->extend_min ((*एतत्))))  return_trace (असत्य);

    एतत्->version = version;
    एतत्->अङ्क_Records = it.len ();
    एतत्->sizeDeviceRecord = DeviceRecord::get_size (अङ्क_glyphs);

    लागि (अचर hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, अङ्क_glyphs);

    return_trace (c->successful ());
  }


  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);

    स्वतः *hdmx_अविभाज्य = c->serializer->start_embed <hdmx> ();

    अचिन्हित अङ्क_आगत_glyphs = get_अङ्क_glyphs ();
    स्वतः it =
    + hb_range ((अचिन्हित) अङ्क_Records)
    | hb_map ([c, अङ्क_आगत_glyphs, एतत्] (अचिन्हित _)
	{
	  अचर DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * sizeDeviceRecord);
	  स्वतः row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([अङ्क_आगत_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			फर्काऊ device_record->widthsZ.as_array (अङ्क_आगत_glyphs) [_.second];
		      })
	    ;
	  फर्काऊ hb_pair ((अचिन्हित) device_record->pixelSize, +row);
	})
    ;

    hdmx_अविभाज्य->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->अङ्क_output_glyphs ());
    return_trace (सत्य);
  }

  अचिन्हित get_अङ्क_glyphs () अचर
  {
    फर्काऊ sizeDeviceRecord - DeviceRecord::min_size;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (अङ्क_Records, sizeDeviceRecord) &&
                  min_size + अङ्क_Records * sizeDeviceRecord > अङ्क_Records * sizeDeviceRecord &&
		  sizeDeviceRecord >= DeviceRecord::min_size &&
		  c->check_range (एतत्, get_size ()));
  }

  रक्षित:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	अङ्क_Records;	/* Number of device records. */
  HBUINT32	sizeDeviceRecord;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_HDMX_TABLE_HH */
