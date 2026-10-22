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

#અન્તર્ભાવ "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#define HB_OT_TAG_hdmx HB_TAG('h','d','m','x')


નામાકાશ OT {


સંરચના DeviceRecord
{
  સ્થિર size_t get_size (unsigned ગણતરી)
  { પાછા hb_ceil_to_4 (hb_unsigned_mul_add_saturate (ગણતરી, HBUINT8::static_size, min_size)); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  unsigned pixelSize,
		  Iterator it,
		  અચલ hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned અંક_glyphs)
  {
    TRACE_SERIALIZE (આ);

    જો (unlikely (!c->extend (આ, અંક_glyphs)))  return_trace (ખોટું);

    આ->pixelSize = pixelSize;
    આ->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    માટે (auto &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned sizeDeviceRecord) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  hb_barrier () &&
			  c->check_range (આ, sizeDeviceRecord)));
  }

  HBUINT8			pixelSize;	/* Pixel size for following widths (as ppem). */
  HBUINT8			maxWidth;	/* Maximum width. */
  UnsizedArrayOf<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  જાહેર:
  DEFINE_SIZE_UNBOUNDED (2);
};


સંરચના hdmx
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_hdmx;

  size_t get_size () અચલ
  { પાછા hb_unsigned_mul_add_saturate (અંક_Records, sizeDeviceRecord, min_size); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  unsigned version,
		  Iterator it,
		  hb_array_t<અચલ hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned અંક_glyphs)
  {
    TRACE_SERIALIZE (આ);

    જો (unlikely (!c->extend_min ((*આ))))  return_trace (ખોટું);

    આ->version = version;
    આ->અંક_Records = it.len ();
    આ->sizeDeviceRecord = DeviceRecord::get_size (અંક_glyphs);

    માટે (અચલ hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, અંક_glyphs);

    return_trace (c->successful ());
  }


  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);

    auto *hdmx_અવિભાજ્ય = c->serializer->start_embed <hdmx> ();

    unsigned અંક_આગત_glyphs = get_અંક_glyphs ();
    auto it =
    + hb_range ((unsigned) અંક_Records)
    | hb_map ([c, અંક_આગત_glyphs, આ] (unsigned _)
	{
	  અચલ DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * sizeDeviceRecord);
	  auto row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([અંક_આગત_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			પાછા device_record->widthsZ.as_array (અંક_આગત_glyphs) [_.second];
		      })
	    ;
	  પાછા hb_pair ((unsigned) device_record->pixelSize, +row);
	})
    ;

    hdmx_અવિભાજ્ય->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->અંક_output_glyphs ());
    return_trace (સાચું);
  }

  unsigned get_અંક_glyphs () અચલ
  {
    પાછા sizeDeviceRecord - DeviceRecord::min_size;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (અંક_Records, sizeDeviceRecord) &&
                  min_size + અંક_Records * sizeDeviceRecord > અંક_Records * sizeDeviceRecord &&
		  sizeDeviceRecord >= DeviceRecord::min_size &&
		  c->check_range (આ, get_size ()));
  }

  સુરક્ષિત:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	અંક_Records;	/* Number of device records. */
  HBUINT32	sizeDeviceRecord;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  જાહેર:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_HDMX_TABLE_HH */
