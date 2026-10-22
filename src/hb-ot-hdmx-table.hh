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

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#define HB_OT_TAG_hdmx HB_TAG('h','d','m','x')


ನಾಮಾಕಾಶ OT {


ಸಂರಚನಾ DeviceRecord
{
  ಸ್ಥಿರ size_t get_size (unsigned ಎಣಿಕೆ)
  { ಪ್ರತ್ಯಾಗಮ hb_ceil_to_4 (hb_unsigned_mul_add_saturate (ಎಣಿಕೆ, HBUINT8::static_size, min_size)); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  unsigned pixelSize,
		  Iterator it,
		  ಧ್ರುವ hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned ಅಂಕೆ_glyphs)
  {
    TRACE_SERIALIZE (ಏತತ್);

    ಯದಿ (unlikely (!c->extend (ಏತತ್, ಅಂಕೆ_glyphs)))  return_trace (ಅಸತ್ಯ);

    ಏತತ್->pixelSize = pixelSize;
    ಏತತ್->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    ಪ್ರತಿ (auto &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned sizeDeviceRecord) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
			  c->check_range (ಏತತ್, sizeDeviceRecord)));
  }

  HBUINT8			pixelSize;	/* Pixel size for following widths (as ppem). */
  HBUINT8			maxWidth;	/* Maximum width. */
  UnsizedArrayOf<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNBOUNDED (2);
};


ಸಂರಚನಾ hdmx
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_hdmx;

  size_t get_size () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_add_saturate (ಅಂಕೆ_Records, sizeDeviceRecord, min_size); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  unsigned version,
		  Iterator it,
		  hb_array_t<ಧ್ರುವ hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned ಅಂಕೆ_glyphs)
  {
    TRACE_SERIALIZE (ಏತತ್);

    ಯದಿ (unlikely (!c->extend_min ((*ಏತತ್))))  return_trace (ಅಸತ್ಯ);

    ಏತತ್->version = version;
    ಏತತ್->ಅಂಕೆ_Records = it.len ();
    ಏತತ್->sizeDeviceRecord = DeviceRecord::get_size (ಅಂಕೆ_glyphs);

    ಪ್ರತಿ (ಧ್ರುವ hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, ಅಂಕೆ_glyphs);

    return_trace (c->successful ());
  }


  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    auto *hdmx_ಅವಿಭಾಜ್ಯ = c->serializer->start_embed <hdmx> ();

    unsigned ಅಂಕೆ_ಒಳಹರಿವು_glyphs = get_ಅಂಕೆ_glyphs ();
    auto it =
    + hb_range ((unsigned) ಅಂಕೆ_Records)
    | hb_map ([c, ಅಂಕೆ_ಒಳಹರಿವು_glyphs, ಏತತ್] (unsigned _)
	{
	  ಧ್ರುವ DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * sizeDeviceRecord);
	  auto row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([ಅಂಕೆ_ಒಳಹರಿವು_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			ಪ್ರತ್ಯಾಗಮ device_record->widthsZ.as_array (ಅಂಕೆ_ಒಳಹರಿವು_glyphs) [_.second];
		      })
	    ;
	  ಪ್ರತ್ಯಾಗಮ hb_pair ((unsigned) device_record->pixelSize, +row);
	})
    ;

    hdmx_ಅವಿಭಾಜ್ಯ->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->ಅಂಕೆ_output_glyphs ());
    return_trace (ಸತ್ಯ);
  }

  unsigned get_ಅಂಕೆ_glyphs () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ sizeDeviceRecord - DeviceRecord::min_size;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (ಅಂಕೆ_Records, sizeDeviceRecord) &&
                  min_size + ಅಂಕೆ_Records * sizeDeviceRecord > ಅಂಕೆ_Records * sizeDeviceRecord &&
		  sizeDeviceRecord >= DeviceRecord::min_size &&
		  c->check_range (ಏತತ್, get_size ()));
  }

  ರಕ್ಷಿತ:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	ಅಂಕೆ_Records;	/* Number of device records. */
  HBUINT32	sizeDeviceRecord;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_HDMX_TABLE_HH */
