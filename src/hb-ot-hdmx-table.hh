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

#నిర్వచితం_కాకపోతే HB_OT_HDMX_పట్టిక_HH
#నిర్వచించు HB_OT_HDMX_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"

/*
 * hdmx -- Horizontal Device Metrics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hdmx
 */
#నిర్వచించు HB_OT_ట్యాగ్_hdmx HB_ట్యాగ్('h','d','m','x')


నామాకాశ OT {


నిర్మాణం Device_రికార్డు
{
  స్థిర size_t get_కొలత (గుర్తులేని లెక్క)
  { తిరిగి hb_ceil_గమ్యం_4 (hb_unsigned_mul_కూడు_saturate (లెక్క, HBUINT8::static_కొలత, min_కొలత)); }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  తార్కిక serialize (hb_serialize_context_t *c,
		  గుర్తులేని pixel_కొలత,
		  Iterator it,
		  స్థిరరాశి hb_సదిశ_t<hb_codepoint_pair_t> new_గమ్యం_old_gid_list,
		  గుర్తులేని అంకె_glyphs)
  {
    TRACE_SERIALIZE (ఇది);

    ఒకవేళ (unlikely (!c->extend (ఇది, అంకె_glyphs)))  return_trace (అబద్ధం);

    ఇది->pixel_కొలత = pixel_కొలత;
    ఇది->max_వెడల్పు =
    + it
    | hb_reduce (hb_max, 0u);

    కొరకు (స్వయంచాలక &_ : new_గమ్యం_old_gid_list)
      widthsZ[_.మొదటి] = *it++;

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని కొలత_Device_రికార్డు) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  c->check_range (ఇది, కొలత_Device_రికార్డు)));
  }

  HBUINT8			pixel_కొలత;	/* Pixel size for following widths (as ppem). */
  HBUINT8			max_వెడల్పు;	/* Maximum width. */
  Unsized_Array_యొక్క<HBUINT8>	widthsZ;	/* Array of widths (numGlyphs is from the 'maxp' table). */
  బహిరంగ:
  DEFINE_కొలత_UNBOUNDED (2);
};


నిర్మాణం hdmx
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_hdmx;

  size_t get_కొలత () స్థిరరాశి
  { తిరిగి hb_unsigned_mul_కూడు_saturate (అంకె_రికార్డులు_2, కొలత_Device_రికార్డు, min_కొలత); }

  మూస<రకం_పేరు Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  తార్కిక serialize (hb_serialize_context_t *c,
		  గుర్తులేని సంస్కరణ,
		  Iterator it,
		  hb_array_t<స్థిరరాశి hb_codepoint_pair_t> new_గమ్యం_old_gid_list,
		  గుర్తులేని అంకె_glyphs)
  {
    TRACE_SERIALIZE (ఇది);

    ఒకవేళ (unlikely (!c->extend_min ((*ఇది))))  return_trace (అబద్ధం);

    ఇది->సంస్కరణ = సంస్కరణ;
    ఇది->అంకె_రికార్డులు_2 = it.len ();
    ఇది->కొలత_Device_రికార్డు = Device_రికార్డు::get_కొలత (అంకె_glyphs);

    కొరకు (స్థిరరాశి hb_అంశం_type<Iterator>& _ : +it)
      c->ప్రారంభం_embed<Device_రికార్డు> ()->serialize (c, _.మొదటి, _.second, new_గమ్యం_old_gid_list, అంకె_glyphs);

    return_trace (c->successful ());
  }


  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    స్వయంచాలక *hdmx_ప్రధానసంఖ్య = c->serializer->ప్రారంభం_embed <hdmx> ();

    గుర్తులేని అంకె_ఆదానం_glyphs = get_అంకె_glyphs ();
    స్వయంచాలక it =
    + hb_range ((గుర్తులేని) అంకె_రికార్డులు_2)
    | hb_map ([c, అంకె_ఆదానం_glyphs, ఇది] (గుర్తులేని _)
	{
	  స్థిరరాశి Device_రికార్డు *device_రికార్డు =
	    &Struct_At_స్థానభ్రంశం<Device_రికార్డు> (&మొదటి_Device_రికార్డు,
					   _ * కొలత_Device_రికార్డు);
	  స్వయంచాలక అడ్డువరుస =
	    + hb_iter (c->plan->new_గమ్యం_old_gid_list)
	    | hb_map ([అంకె_ఆదానం_glyphs, device_రికార్డు] (hb_codepoint_pair_t _)
		      {
			తిరిగి device_రికార్డు->widthsZ.as_array (అంకె_ఆదానం_glyphs) [_.second];
		      })
	    ;
	  తిరిగి hb_pair ((గుర్తులేని) device_రికార్డు->pixel_కొలత, +అడ్డువరుస);
	})
    ;

    hdmx_ప్రధానసంఖ్య->serialize (c->serializer, సంస్కరణ, it,
			   c->plan->new_గమ్యం_old_gid_list,
			   c->plan->అంకె_output_glyphs ());
    return_trace (నిజం);
  }

  గుర్తులేని get_అంకె_glyphs () స్థిరరాశి
  {
    తిరిగి కొలత_Device_రికార్డు - Device_రికార్డు::min_కొలత;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  !hb_unsigned_mul_overflows (అంకె_రికార్డులు_2, కొలత_Device_రికార్డు) &&
                  min_కొలత + అంకె_రికార్డులు_2 * కొలత_Device_రికార్డు > అంకె_రికార్డులు_2 * కొలత_Device_రికార్డు &&
		  కొలత_Device_రికార్డు >= Device_రికార్డు::min_కొలత &&
		  c->check_range (ఇది, get_కొలత ()));
  }

  రక్షిత:
  HBUINT16	సంస్కరణ;	/* Table version number (0) */
  HBUINT16	అంకె_రికార్డులు_2;	/* Number of device records. */
  HBUINT32	కొలత_Device_రికార్డు;
				/* Size of a device record, 32-bit aligned. */
  Device_రికార్డు	మొదటి_Device_రికార్డు;
				/* Array of device records. */
  బహిరంగ:
  DEFINE_కొలత_MIN (8);
};

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_HDMX_TABLE_HH */
