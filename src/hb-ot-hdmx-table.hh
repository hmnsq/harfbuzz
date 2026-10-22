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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#define HB_OT_TAG_hdmx HB_TAG('h','d','m','x')


ନାମାକାଶ OT {


ସଂରଚନା DeviceRecord
{
  ସ୍ଥିର size_t get_size (unsigned ଗଣନା_2)
  { ଫେରନ୍ତୁ hb_ceil_to_4 (hb_unsigned_mul_add_saturate (ଗଣନା_2, HBUINT8::static_size, min_size)); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  unsigned pixelSize,
		  Iterator it,
		  ଅଚଳ hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned ଅଙ୍କ_glyphs)
  {
    TRACE_SERIALIZE (ଏହି);

    ଯଦି (unlikely (!c->extend (ଏହି, ଅଙ୍କ_glyphs)))  return_trace (ମିଥ୍ୟା);

    ଏହି->pixelSize = pixelSize;
    ଏହି->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    ପାଇଁ (auto &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned sizeDeviceRecord) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  c->check_range (ଏହି, sizeDeviceRecord)));
  }

  HBUINT8			pixelSize;	/* Pixel size for following widths (as ppem). */
  HBUINT8			maxWidth;	/* Maximum width. */
  UnsizedArrayOf<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNBOUNDED (2);
};


ସଂରଚନା hdmx
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_hdmx;

  size_t get_size () ଅଚଳ
  { ଫେରନ୍ତୁ hb_unsigned_mul_add_saturate (ଅଙ୍କ_Records, sizeDeviceRecord, min_size); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  unsigned version,
		  Iterator it,
		  hb_array_t<ଅଚଳ hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned ଅଙ୍କ_glyphs)
  {
    TRACE_SERIALIZE (ଏହି);

    ଯଦି (unlikely (!c->extend_min ((*ଏହି))))  return_trace (ମିଥ୍ୟା);

    ଏହି->version = version;
    ଏହି->ଅଙ୍କ_Records = it.len ();
    ଏହି->sizeDeviceRecord = DeviceRecord::get_size (ଅଙ୍କ_glyphs);

    ପାଇଁ (ଅଚଳ hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, ଅଙ୍କ_glyphs);

    return_trace (c->successful ());
  }


  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    auto *hdmx_ମୌଳିକ = c->serializer->start_embed <hdmx> ();

    unsigned ଅଙ୍କ_ନିବେଶ_glyphs = get_ଅଙ୍କ_glyphs ();
    auto it =
    + hb_range ((unsigned) ଅଙ୍କ_Records)
    | hb_map ([c, ଅଙ୍କ_ନିବେଶ_glyphs, ଏହି] (unsigned _)
	{
	  ଅଚଳ DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * sizeDeviceRecord);
	  auto row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([ଅଙ୍କ_ନିବେଶ_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			ଫେରନ୍ତୁ device_record->widthsZ.as_array (ଅଙ୍କ_ନିବେଶ_glyphs) [_.second];
		      })
	    ;
	  ଫେରନ୍ତୁ hb_pair ((unsigned) device_record->pixelSize, +row);
	})
    ;

    hdmx_ମୌଳିକ->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->ଅଙ୍କ_output_glyphs ());
    return_trace (ସତ୍ୟ);
  }

  unsigned get_ଅଙ୍କ_glyphs () ଅଚଳ
  {
    ଫେରନ୍ତୁ sizeDeviceRecord - DeviceRecord::min_size;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (ଅଙ୍କ_Records, sizeDeviceRecord) &&
                  min_size + ଅଙ୍କ_Records * sizeDeviceRecord > ଅଙ୍କ_Records * sizeDeviceRecord &&
		  sizeDeviceRecord >= DeviceRecord::min_size &&
		  c->check_range (ଏହି, get_size ()));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	ଅଙ୍କ_Records;	/* Number of device records. */
  HBUINT32	sizeDeviceRecord;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_HDMX_TABLE_HH */
