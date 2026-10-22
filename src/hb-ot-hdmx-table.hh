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

#ضمن "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#define HB_OT_TAG_hdmx HB_TAG('h','d','m','x')


نطاق OT {


بنية DeviceRecord
{
  ساكن size_t get_size (unsigned سانى)
  { أرجع hb_ceil_to_4 (hb_unsigned_mul_add_saturate (سانى, HBUINT8::static_size, min_size)); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  unsigned pixelSize,
		  Iterator it,
		  ثابت hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned رەقەم_glyphs)
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend (هذا, رەقەم_glyphs)))  return_trace (خطأ);

    هذا->pixelSize = pixelSize;
    هذا->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    لكل (auto &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c, unsigned sizeDeviceRecord) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  c->check_range (هذا, sizeDeviceRecord)));
  }

  HBUINT8			pixelSize;	/* Pixel size for following widths (as ppem). */
  HBUINT8			maxWidth;	/* Maximum width. */
  UnsizedArrayOf<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  عام:
  DEFINE_SIZE_UNBOUNDED (2);
};


بنية hdmx
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_hdmx;

  size_t get_size () ثابت
  { أرجع hb_unsigned_mul_add_saturate (رەقەم_Records, sizeDeviceRecord, min_size); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  unsigned version,
		  Iterator it,
		  hb_array_t<ثابت hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned رەقەم_glyphs)
  {
    TRACE_SERIALIZE (هذا);

    إذا (unlikely (!c->extend_min ((*هذا))))  return_trace (خطأ);

    هذا->version = version;
    هذا->رەقەم_Records = it.len ();
    هذا->sizeDeviceRecord = DeviceRecord::get_size (رەقەم_glyphs);

    لكل (ثابت hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, رەقەم_glyphs);

    return_trace (c->successful ());
  }


  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    auto *hdmx_ئاددىي_سان = c->serializer->start_embed <hdmx> ();

    unsigned رەقەم_كىرگۈزۈش_glyphs = get_رەقەم_glyphs ();
    auto it =
    + hb_range ((unsigned) رەقەم_Records)
    | hb_map ([c, رەقەم_كىرگۈزۈش_glyphs, هذا] (unsigned _)
	{
	  ثابت DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * sizeDeviceRecord);
	  auto row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([رەقەم_كىرگۈزۈش_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			أرجع device_record->widthsZ.as_array (رەقەم_كىرگۈزۈش_glyphs) [_.second];
		      })
	    ;
	  أرجع hb_pair ((unsigned) device_record->pixelSize, +row);
	})
    ;

    hdmx_ئاددىي_سان->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->رەقەم_output_glyphs ());
    return_trace (true);
  }

  unsigned get_رەقەم_glyphs () ثابت
  {
    أرجع sizeDeviceRecord - DeviceRecord::min_size;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (رەقەم_Records, sizeDeviceRecord) &&
                  min_size + رەقەم_Records * sizeDeviceRecord > رەقەم_Records * sizeDeviceRecord &&
		  sizeDeviceRecord >= DeviceRecord::min_size &&
		  c->check_range (هذا, get_size ()));
  }

  محمي:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	رەقەم_Records;	/* Number of device records. */
  HBUINT32	sizeDeviceRecord;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  عام:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_HDMX_TABLE_HH */
