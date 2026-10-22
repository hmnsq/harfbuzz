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

#அந்தர்பாவ "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#define HB_OT_TAG_hdmx HB_TAG('h','d','m','x')


நாமாகாஶ OT {


கட்டமைப்பு DeviceRecord
{
  நிலையான size_t get_size (unsigned மொத்தம்)
  { திரும்பு hb_ceil_to_4 (hb_unsigned_mul_add_saturate (மொத்தம், HBUINT8::static_size, min_size)); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  தருக்க serialize (hb_serialize_context_t *c,
		  unsigned pixelSize,
		  Iterator it,
		  மாறிலி hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned இலக்கம்_glyphs)
  {
    TRACE_SERIALIZE (இது);

    எனில் (unlikely (!c->extend (இது, இலக்கம்_glyphs)))  return_trace (பொய்);

    இது->pixelSize = pixelSize;
    இது->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    க்கு (auto &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c, unsigned sizeDeviceRecord) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  c->check_range (இது, sizeDeviceRecord)));
  }

  HBUINT8			pixelSize;	/* Pixel size for following widths (as ppem). */
  HBUINT8			maxWidth;	/* Maximum width. */
  UnsizedArrayOf<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  பொது:
  DEFINE_SIZE_UNBOUNDED (2);
};


கட்டமைப்பு hdmx
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_hdmx;

  size_t get_size () மாறிலி
  { திரும்பு hb_unsigned_mul_add_saturate (இலக்கம்_Records, sizeDeviceRecord, min_size); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  தருக்க serialize (hb_serialize_context_t *c,
		  unsigned version,
		  Iterator it,
		  hb_array_t<மாறிலி hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned இலக்கம்_glyphs)
  {
    TRACE_SERIALIZE (இது);

    எனில் (unlikely (!c->extend_min ((*இது))))  return_trace (பொய்);

    இது->version = version;
    இது->இலக்கம்_Records = it.len ();
    இது->sizeDeviceRecord = DeviceRecord::get_size (இலக்கம்_glyphs);

    க்கு (மாறிலி hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, இலக்கம்_glyphs);

    return_trace (c->successful ());
  }


  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);

    auto *hdmx_பகாஎண் = c->serializer->start_embed <hdmx> ();

    unsigned இலக்கம்_உள்ளீடு_glyphs = get_இலக்கம்_glyphs ();
    auto it =
    + hb_range ((unsigned) இலக்கம்_Records)
    | hb_map ([c, இலக்கம்_உள்ளீடு_glyphs, இது] (unsigned _)
	{
	  மாறிலி DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * sizeDeviceRecord);
	  auto row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([இலக்கம்_உள்ளீடு_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			திரும்பு device_record->widthsZ.as_array (இலக்கம்_உள்ளீடு_glyphs) [_.second];
		      })
	    ;
	  திரும்பு hb_pair ((unsigned) device_record->pixelSize, +row);
	})
    ;

    hdmx_பகாஎண்->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->இலக்கம்_output_glyphs ());
    return_trace (உண்மை);
  }

  unsigned get_இலக்கம்_glyphs () மாறிலி
  {
    திரும்பு sizeDeviceRecord - DeviceRecord::min_size;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (இலக்கம்_Records, sizeDeviceRecord) &&
                  min_size + இலக்கம்_Records * sizeDeviceRecord > இலக்கம்_Records * sizeDeviceRecord &&
		  sizeDeviceRecord >= DeviceRecord::min_size &&
		  c->check_range (இது, get_size ()));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	இலக்கம்_Records;	/* Number of device records. */
  HBUINT32	sizeDeviceRecord;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  பொது:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_HDMX_TABLE_HH */
