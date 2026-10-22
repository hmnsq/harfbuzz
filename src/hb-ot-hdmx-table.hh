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

#അന്തര്ഭാവ "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#define HB_OT_TAG_hdmx HB_TAG('h','d','m','x')


നാമാകാശ OT {


ഘടന DeviceRecord
{
  സ്ഥിതം size_t get_size (unsigned എണ്ണം)
  { തിരികെ hb_ceil_to_4 (hb_unsigned_mul_add_saturate (എണ്ണം, HBUINT8::static_size, min_size)); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  തർക്കം serialize (hb_serialize_context_t *c,
		  unsigned pixelSize,
		  Iterator it,
		  സ്ഥിരം hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned അക്കം_glyphs)
  {
    TRACE_SERIALIZE (ഇത്);

    എങ്കിൽ (unlikely (!c->extend (ഇത്, അക്കം_glyphs)))  return_trace (അസത്യം);

    ഇത്->pixelSize = pixelSize;
    ഇത്->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    വേണ്ടി (auto &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned sizeDeviceRecord) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  c->check_range (ഇത്, sizeDeviceRecord)));
  }

  HBUINT8			pixelSize;	/* Pixel size for following widths (as ppem). */
  HBUINT8			maxWidth;	/* Maximum width. */
  UnsizedArrayOf<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  പൊതു:
  DEFINE_SIZE_UNBOUNDED (2);
};


ഘടന hdmx
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_hdmx;

  size_t get_size () സ്ഥിരം
  { തിരികെ hb_unsigned_mul_add_saturate (അക്കം_Records, sizeDeviceRecord, min_size); }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  തർക്കം serialize (hb_serialize_context_t *c,
		  unsigned version,
		  Iterator it,
		  hb_array_t<സ്ഥിരം hb_codepoint_pair_t> new_to_old_gid_list,
		  unsigned അക്കം_glyphs)
  {
    TRACE_SERIALIZE (ഇത്);

    എങ്കിൽ (unlikely (!c->extend_min ((*ഇത്))))  return_trace (അസത്യം);

    ഇത്->version = version;
    ഇത്->അക്കം_Records = it.len ();
    ഇത്->sizeDeviceRecord = DeviceRecord::get_size (അക്കം_glyphs);

    വേണ്ടി (സ്ഥിരം hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, അക്കം_glyphs);

    return_trace (c->successful ());
  }


  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    auto *hdmx_അഭാജ്യം = c->serializer->start_embed <hdmx> ();

    unsigned അക്കം_നിവേശം_glyphs = get_അക്കം_glyphs ();
    auto it =
    + hb_range ((unsigned) അക്കം_Records)
    | hb_map ([c, അക്കം_നിവേശം_glyphs, ഇത്] (unsigned _)
	{
	  സ്ഥിരം DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * sizeDeviceRecord);
	  auto row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([അക്കം_നിവേശം_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			തിരികെ device_record->widthsZ.as_array (അക്കം_നിവേശം_glyphs) [_.second];
		      })
	    ;
	  തിരികെ hb_pair ((unsigned) device_record->pixelSize, +row);
	})
    ;

    hdmx_അഭാജ്യം->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->അക്കം_output_glyphs ());
    return_trace (സത്യം);
  }

  unsigned get_അക്കം_glyphs () സ്ഥിരം
  {
    തിരികെ sizeDeviceRecord - DeviceRecord::min_size;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (അക്കം_Records, sizeDeviceRecord) &&
                  min_size + അക്കം_Records * sizeDeviceRecord > അക്കം_Records * sizeDeviceRecord &&
		  sizeDeviceRecord >= DeviceRecord::min_size &&
		  c->check_range (ഇത്, get_size ()));
  }

  സംരക്ഷിത:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	അക്കം_Records;	/* Number of device records. */
  HBUINT32	sizeDeviceRecord;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  പൊതു:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_HDMX_TABLE_HH */
