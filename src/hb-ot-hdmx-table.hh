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

#अगर_अघोषित HB_OT_HDMX_TABLE_HH
#घोषणा HB_OT_HDMX_TABLE_HH

#समावेश "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#घोषणा HB_OT_TAG_hdmx HB_TAG('h','d','m','x')


namespace OT {


काष्ठा DeviceRecord
{
  अटल माप_प्रकार get_आकार (अचिन्हित गिनती)
  { वापस hb_ceil_to_4 (hb_unsigned_mul_add_saturate (गिनती, HBUINT8::static_आकार, min_आकार)); }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  bool serialize (hb_serialize_context_t *c,
		  अचिन्हित pixel_आकार,
		  Iterator it,
		  स्थिर hb_vector_t<hb_codepoint_pair_t> new_to_old_gid_list,
		  अचिन्हित अंक_glyphs)
  {
    TRACE_SERIALIZE (यह);

    अगर (unlikely (!c->extend (यह, अंक_glyphs)))  return_trace (false);

    यह->pixel_आकार = pixel_आकार;
    यह->maxWidth =
    + it
    | hb_reduce (hb_max, 0u);

    क्रम (स्वतः &_ : new_to_old_gid_list)
      widthsZ[_.first] = *it++;

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c, अचिन्हित आकार_Device_Record) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  c->check_range (यह, आकार_Device_Record)));
  }

  HBUINT8			pixel_आकार;	/* Pixel size for following widths (as ppem). */
  HBUINT8			maxWidth;	/* Maximum width. */
  UnsizedArrayOf<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  खुला:
  DEFINE_आकार_UNBOUNDED (2);
};


काष्ठा hdmx
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_hdmx;

  माप_प्रकार get_आकार () स्थिर
  { वापस hb_unsigned_mul_add_saturate (अंक_Records, आकार_Device_Record, min_आकार); }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  bool serialize (hb_serialize_context_t *c,
		  अचिन्हित version,
		  Iterator it,
		  hb_array_t<स्थिर hb_codepoint_pair_t> new_to_old_gid_list,
		  अचिन्हित अंक_glyphs)
  {
    TRACE_SERIALIZE (यह);

    अगर (unlikely (!c->extend_min ((*यह))))  return_trace (false);

    यह->version = version;
    यह->अंक_Records = it.len ();
    यह->आकार_Device_Record = DeviceRecord::get_आकार (अंक_glyphs);

    क्रम (स्थिर hb_item_type<Iterator>& _ : +it)
      c->start_embed<DeviceRecord> ()->serialize (c, _.first, _.second, new_to_old_gid_list, अंक_glyphs);

    return_trace (c->successful ());
  }


  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);

    स्वतः *hdmx_अभाज्य = c->serializer->start_embed <hdmx> ();

    अचिन्हित अंक_निवेश_glyphs = get_अंक_glyphs ();
    स्वतः it =
    + hb_range ((अचिन्हित) अंक_Records)
    | hb_map ([c, अंक_निवेश_glyphs, यह] (अचिन्हित _)
	{
	  स्थिर DeviceRecord *device_record =
	    &StructAtOffset<DeviceRecord> (&firstDeviceRecord,
					   _ * आकार_Device_Record);
	  स्वतः row =
	    + hb_iter (c->plan->new_to_old_gid_list)
	    | hb_map ([अंक_निवेश_glyphs, device_record] (hb_codepoint_pair_t _)
		      {
			वापस device_record->widthsZ.as_array (अंक_निवेश_glyphs) [_.second];
		      })
	    ;
	  वापस hb_pair ((अचिन्हित) device_record->pixel_आकार, +row);
	})
    ;

    hdmx_अभाज्य->serialize (c->serializer, version, it,
			   c->plan->new_to_old_gid_list,
			   c->plan->अंक_output_glyphs ());
    return_trace (true);
  }

  अचिन्हित get_अंक_glyphs () स्थिर
  {
    वापस आकार_Device_Record - DeviceRecord::min_आकार;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (अंक_Records, आकार_Device_Record) &&
                  min_आकार + अंक_Records * आकार_Device_Record > अंक_Records * आकार_Device_Record &&
		  आकार_Device_Record >= DeviceRecord::min_आकार &&
		  c->check_range (यह, get_आकार ()));
  }

  रक्षित:
  HBUINT16	version;	/* Table version number (0) */
  HBUINT16	अंक_Records;	/* Number of device records. */
  HBUINT32	आकार_Device_Record;
				/* Size of a device record, 32-bit aligned. */
  DeviceRecord	firstDeviceRecord;
				/* Array of device records. */
  खुला:
  DEFINE_आकार_MIN (8);
};

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_HDMX_TABLE_HH */
