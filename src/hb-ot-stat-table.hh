/*
 * Copyright © 2018  Ebrahim Byagowi
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
 */

#నిర్వచితం_కాకపోతే HB_OT_STAT_పట్టిక_HH
#నిర్వచించు HB_OT_STAT_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#నిర్వచించు HB_OT_ట్యాగ్_STAT HB_ట్యాగ్('S','T','A','T')


నామాకాశ OT {

గణన
{
  OLDER_SIBLING_FONT_ATTRIBUTE = 0x0001,	/* If set, this axis value table
						 * provides axis value information
						 * that is applicable to other fonts
						 * within the same font family. This
						 * is used if the other fonts were
						 * released earlier and did not include
						 * information about values for some axis.
						 * If newer versions of the other
						 * fonts include the information
						 * themselves and are present,
						 * then this record is ignored. */
  ELIDABLE_AXIS_విలువ_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

స్థిర తార్కిక axis_విలువ_is_outside_axis_range (hb_ట్యాగ్_t axis_ట్యాగ్_2, దశాంశం axis_విలువ,
                                              స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> *user_axes_location)
{
  ఒకవేళ (!user_axes_location->has (axis_ట్యాగ్_2))
    తిరిగి అబద్ధం;

  Triple axis_range = user_axes_location->get (axis_ట్యాగ్_2);
  తిరిగి (axis_విలువ < axis_range.minimum || axis_విలువ > axis_range.గరిష్ఠ);
}

నిర్మాణం Stat_Axis_రికార్డు
{
  పూర్ణాంకం cmp (hb_ట్యాగ్_t key) స్థిరరాశి { తిరిగి ట్యాగ్_2.cmp (key); }

  hb_ot_name_id_t get_name_id () స్థిరరాశి { తిరిగి nameID; }

  hb_ట్యాగ్_t get_axis_ట్యాగ్ () స్థిరరాశి { తిరిగి ట్యాగ్_2; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది)));
  }

  రక్షిత:
  ట్యాగ్		ట్యాగ్_2;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం Axis_విలువ_Format1
{
  గుర్తులేని పూర్ణాంకం get_axis_సూచీ () స్థిరరాశి { తిరిగి axis_సూచీ_2; }
  దశాంశం get_విలువ ()             స్థిరరాశి { తిరిగి value.గమ్యం_float (); }

  hb_ot_name_id_t get_విలువ_name_id () స్థిరరాశి { తిరిగి విలువ_Name_ID; }

  hb_ట్యాగ్_t get_axis_ట్యాగ్ (స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు) స్థిరరాశి
  {
    గుర్తులేని axis_idx = get_axis_సూచీ ();
    తిరిగి axis_రికార్డులు[axis_idx].get_axis_ట్యాగ్ ();
  }

  తార్కిక keep_axis_విలువ (స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు,
                        స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> *user_axes_location) స్థిరరాశి
  {
    hb_ట్యాగ్_t axis_ట్యాగ్_2 = get_axis_ట్యాగ్ (axis_రికార్డులు);
    దశాంశం axis_విలువ = get_విలువ ();

    తిరిగి !axis_విలువ_is_outside_axis_range (axis_ట్యాగ్_2, axis_విలువ, user_axes_location);
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ఒకవేళ (keep_axis_విలువ (axis_రికార్డులు, user_axes_location))
      return_trace (c->serializer->embed (ఇది));

    return_trace (అబద్ధం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axis_సూచీ_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	విలువ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};

నిర్మాణం Axis_విలువ_Format2
{
  గుర్తులేని పూర్ణాంకం get_axis_సూచీ () స్థిరరాశి { తిరిగి axis_సూచీ_2; }
  దశాంశం get_విలువ ()             స్థిరరాశి { తిరిగి nominal_విలువ.గమ్యం_float (); }

  hb_ot_name_id_t get_విలువ_name_id () స్థిరరాశి { తిరిగి విలువ_Name_ID; }

  hb_ట్యాగ్_t get_axis_ట్యాగ్ (స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు) స్థిరరాశి
  {
    గుర్తులేని axis_idx = get_axis_సూచీ ();
    తిరిగి axis_రికార్డులు[axis_idx].get_axis_ట్యాగ్ ();
  }

  తార్కిక keep_axis_విలువ (స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు,
                        స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> *user_axes_location) స్థిరరాశి
  {
    hb_ట్యాగ్_t axis_ట్యాగ్_2 = get_axis_ట్యాగ్ (axis_రికార్డులు);
    దశాంశం axis_విలువ = get_విలువ ();

    తిరిగి !axis_విలువ_is_outside_axis_range (axis_ట్యాగ్_2, axis_విలువ, user_axes_location);
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ఒకవేళ (keep_axis_విలువ (axis_రికార్డులు, user_axes_location))
      return_trace (c->serializer->embed (ఇది));

    return_trace (అబద్ధం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axis_సూచీ_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	విలువ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_విలువ;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_విలువ;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_విలువ;	/* The maximum value for a range associated
				 * with the specified name ID. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (20);
};

నిర్మాణం Axis_విలువ_Format3
{
  గుర్తులేని పూర్ణాంకం get_axis_సూచీ () స్థిరరాశి { తిరిగి axis_సూచీ_2; }
  దశాంశం get_విలువ ()             స్థిరరాశి { తిరిగి value.గమ్యం_float (); }

  hb_ot_name_id_t get_విలువ_name_id () స్థిరరాశి { తిరిగి విలువ_Name_ID; }

  hb_ట్యాగ్_t get_axis_ట్యాగ్ (స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు) స్థిరరాశి
  {
    గుర్తులేని axis_idx = get_axis_సూచీ ();
    తిరిగి axis_రికార్డులు[axis_idx].get_axis_ట్యాగ్ ();
  }

  తార్కిక keep_axis_విలువ (స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు,
                        స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> *user_axes_location) స్థిరరాశి
  {
    hb_ట్యాగ్_t axis_ట్యాగ్_2 = get_axis_ట్యాగ్ (axis_రికార్డులు);
    దశాంశం axis_విలువ = get_విలువ ();

    తిరిగి !axis_విలువ_is_outside_axis_range (axis_ట్యాగ్_2, axis_విలువ, user_axes_location);
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    ఒకవేళ (keep_axis_విలువ (axis_రికార్డులు, user_axes_location))
      return_trace (c->serializer->embed (ఇది));

    return_trace (అబద్ధం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axis_సూచీ_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	విలువ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_విలువ;	/* The numeric value for a style-linked mapping
				 * from this value. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (16);
};

నిర్మాణం Axis_విలువ_రికార్డు
{
  గుర్తులేని పూర్ణాంకం get_axis_సూచీ () స్థిరరాశి { తిరిగి axis_సూచీ_2; }
  దశాంశం get_విలువ ()             స్థిరరాశి { తిరిగి value.గమ్యం_float (); }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	axis_సూచీ_2;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం Axis_విలువ_Format4
{
  స్థిరరాశి Axis_విలువ_రికార్డు &get_axis_రికార్డు (గుర్తులేని పూర్ణాంకం axis_సూచీ) స్థిరరాశి
  { తిరిగి axis_విలువలు_2.as_array (axis_లెక్క_2)[axis_సూచీ]; }

  తార్కిక keep_axis_విలువ (స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు,
                        స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> *user_axes_location) స్థిరరాశి
  {
    hb_array_t<స్థిరరాశి Axis_విలువ_రికార్డు> axis_విలువ_రికార్డులు = axis_విలువలు_2.as_array (axis_లెక్క_2);

    కొరకు (స్థిరరాశి స్వయంచాలక& rec : axis_విలువ_రికార్డులు)
    {
      గుర్తులేని axis_idx = rec.get_axis_సూచీ ();
      దశాంశం axis_విలువ = rec.get_విలువ ();
      hb_ట్యాగ్_t axis_ట్యాగ్_2 = axis_రికార్డులు[axis_idx].get_axis_ట్యాగ్ ();

      ఒకవేళ (axis_విలువ_is_outside_axis_range (axis_ట్యాగ్_2, axis_విలువ, user_axes_location))
        తిరిగి అబద్ధం;
    }

    తిరిగి నిజం;
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    ఒకవేళ (!keep_axis_విలువ (axis_రికార్డులు, user_axes_location))
      return_trace (అబద్ధం);

    గుర్తులేని మొత్తం_కొలత = min_కొలత + axis_లెక్క_2 * Axis_విలువ_రికార్డు::static_కొలత;
    స్వయంచాలక *బహిర్గతం = c->serializer->allocate_కొలత<Axis_విలువ_Format4> (మొత్తం_కొలత);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);
    hb_memcpy (బహిర్గతం, ఇది, మొత్తం_కొలత);
    return_trace (నిజం);
  }

  hb_ot_name_id_t get_విలువ_name_id () స్థిరరాశి { తిరిగి విలువ_Name_ID; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
                          axis_విలువలు_2.sanitize (c, axis_లెక్క_2)));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_లెక్క_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	విలువ_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  Unsized_Array_యొక్క<Axis_విలువ_రికార్డు>
		axis_విలువలు_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (8, axis_విలువలు_2);
};

నిర్మాణం Axis_విలువ
{
  దశాంశం get_విలువ (గుర్తులేని పూర్ణాంకం axis_సూచీ) స్థిరరాశి
  {
    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_విలువ ();
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_విలువ ();
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_విలువ ();
    స్థితి 4: hb_barrier (); తిరిగి u.format4.get_axis_రికార్డు (axis_సూచీ).get_విలువ ();
    అప్రమేయ:తిరిగి 0.f;
    }
  }

  గుర్తులేని పూర్ణాంకం get_axis_సూచీ () స్థిరరాశి
  {
    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_axis_సూచీ ();
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_axis_సూచీ ();
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_axis_సూచీ ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    అప్రమేయ:తిరిగి -1;
    }
  }

  hb_ot_name_id_t get_విలువ_name_id () స్థిరరాశి
  {
    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_విలువ_name_id ();
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_విలువ_name_id ();
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_విలువ_name_id ();
    స్థితి 4: hb_barrier (); తిరిగి u.format4.get_విలువ_name_id ();
    అప్రమేయ:తిరిగి HB_OT_NAME_ID_INVALID;
    }
  }

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!c->may_dispatch (ఇది, &u.format.v))) తిరిగి c->no_dispatch_return_విలువ ();
    TRACE_DISPATCH (ఇది, u.format.v);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    స్థితి 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    స్థితి 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    స్థితి 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }

  తార్కిక keep_axis_విలువ (స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు,
                        hb_hashmap_t<hb_ట్యాగ్_t, Triple> *user_axes_location) స్థిరరాశి
  {
    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.keep_axis_విలువ (axis_రికార్డులు, user_axes_location);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.keep_axis_విలువ (axis_రికార్డులు, user_axes_location);
    స్థితి 3: hb_barrier (); తిరిగి u.format3.keep_axis_విలువ (axis_రికార్డులు, user_axes_location);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.keep_axis_విలువ (axis_రికార్డులు, user_axes_location);
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది)))
      return_trace (అబద్ధం);
    hb_barrier ();

    ఎంపిక (u.format.v)
    {
    స్థితి 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    స్థితి 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    స్థితి 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    స్థితి 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    అప్రమేయ:return_trace (నిజం);
    }
  }

  రక్షిత:
  సంఘం
  {
  నిర్మాణం { HBUINT16 v; }	format;
  Axis_విలువ_Format1	format1;
  Axis_విలువ_Format2	format2;
  Axis_విలువ_Format3	format3;
  Axis_విలువ_Format4	format4;
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (2, format.v);
};

నిర్మాణం Axis_విలువ_స్థానభ్రంశం_Array: Unsized_Array_యొక్క<Offset16_గమ్యం<Axis_విలువ>>
{
  తార్కిక subset (hb_subset_context_t *c,
               గుర్తులేని axis_విలువ_లెక్క,
               గుర్తులేని& లెక్క,
               స్థిరరాశి hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> axis_రికార్డులు) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    స్వయంచాలక axis_విలువ_Offsets = as_array (axis_విలువ_లెక్క);
    లెక్క = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& స్థానభ్రంశం : axis_విలువ_Offsets)
    {
      ఒకవేళ (!స్థానభ్రంశం) కొనసాగు;
      స్వయంచాలక o_snap = c->serializer->snapshot ();
      స్వయంచాలక *o = c->serializer->embed (స్థానభ్రంశం);
      ఒకవేళ (!o) return_trace (అబద్ధం);
      ఒకవేళ (!o->serialize_subset (c, స్థానభ్రంశం, ఇది, axis_రికార్డులు))
      {
        c->serializer->revert (o_snap);
        కొనసాగు;
      }
      లెక్క++;
    }

    return_trace (లెక్క);
  }
};

నిర్మాణం STAT
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_STAT;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.గమ్యం_int (); }

  తార్కిక get_విలువ (hb_ట్యాగ్_t ట్యాగ్_2, దశాంశం *value) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం axis_సూచీ;
    ఒకవేళ (!get_design_axes ().lfind (ట్యాగ్_2, &axis_సూచీ)) తిరిగి అబద్ధం;

    hb_array_t<స్థిరరాశి Offset16_గమ్యం<Axis_విలువ>> axis_విలువలు = get_axis_విలువ_offsets ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < axis_విలువలు.నిడివి; i++)
    {
      స్థిరరాశి Axis_విలువ& axis_విలువ = ఇది+స్థానభ్రంశం_గమ్యం_Axis_విలువ_Offsets+axis_విలువలు[i];
      ఒకవేళ (axis_విలువ.get_axis_సూచీ () == axis_సూచీ)
      {
	ఒకవేళ (value)
	  *value = axis_విలువ.get_విలువ (axis_సూచీ);
	తిరిగి నిజం;
      }
    }
    తిరిగి అబద్ధం;
  }

  గుర్తులేని get_design_axis_లెక్క () స్థిరరాశి { తిరిగి design_Axis_లెక్క; }

  hb_ot_name_id_t get_axis_రికార్డు_name_id (గుర్తులేని axis_రికార్డు_సూచీ) స్థిరరాశి
  {
    ఒకవేళ (unlikely (axis_రికార్డు_సూచీ >= design_Axis_లెక్క)) తిరిగి HB_OT_NAME_ID_INVALID;
    స్థిరరాశి Stat_Axis_రికార్డు &axis_రికార్డు = get_design_axes ()[axis_రికార్డు_సూచీ];
    తిరిగి axis_రికార్డు.get_name_id ();
  }

  గుర్తులేని get_axis_విలువ_లెక్క () స్థిరరాశి { తిరిగి axis_విలువ_లెక్క; }

  hb_ot_name_id_t get_axis_విలువ_name_id (గుర్తులేని axis_విలువ_సూచీ) స్థిరరాశి
  {
    ఒకవేళ (unlikely (axis_విలువ_సూచీ >= axis_విలువ_లెక్క)) తిరిగి HB_OT_NAME_ID_INVALID;
    స్థిరరాశి Axis_విలువ &axis_విలువ = (ఇది + get_axis_విలువ_offsets ()[axis_విలువ_సూచీ]);
    తిరిగి axis_విలువ.get_విలువ_name_id ();
  }

  శూన్య collect_name_ids (hb_hashmap_t<hb_ట్యాగ్_t, Triple> *user_axes_location,
                         hb_సమితి_t *nameids_గమ్యం_retain /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (!has_దత్తాంశం ()) తిరిగి;

    + get_design_axes ()
    | hb_map (&Stat_Axis_రికార్డు::get_name_id)
    | hb_sink (nameids_గమ్యం_retain)
    ;

    స్వయంచాలక designAxes = get_design_axes ();

    + get_axis_విలువ_offsets ()
    | hb_map (hb_కూడు (&(ఇది + స్థానభ్రంశం_గమ్యం_Axis_విలువ_Offsets)))
    | hb_filter ([&] (స్థిరరాశి Axis_విలువ& _)
                 { తిరిగి _.keep_axis_విలువ (designAxes, user_axes_location); })
    | hb_map (&Axis_విలువ::get_విలువ_name_id)
    | hb_sink (nameids_గమ్యం_retain)
    ;

    nameids_గమ్యం_retain->కూడు (elidedFallbackNameID);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    STAT *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    స్వయంచాలక designAxes = get_design_axes ();
    కొరకు (గుర్తులేని i = 0; i < (గుర్తులేని)design_Axis_లెక్క; i++)
      ఒకవేళ (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (అబద్ధం);

    ఒకవేళ (design_Axis_లెక్క)
      c->serializer->check_కేటాయించు (బహిర్గతం->design_Axes_స్థానభ్రంశం, ఇది->get_కొలత (),
                                   HB_SERIALIZE_ERROR_INT_అధికప్రవాహం);

    గుర్తులేని లెక్క = 0;
    బహిర్గతం->స్థానభ్రంశం_గమ్యం_Axis_విలువ_Offsets.serialize_subset (c, స్థానభ్రంశం_గమ్యం_Axis_విలువ_Offsets, ఇది,
                                                    axis_విలువ_లెక్క, లెక్క, designAxes);
    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->axis_విలువ_లెక్క, లెక్క, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  సంస్కరణ.major == 1 &&
			  సంస్కరణ.minor > 0 &&
			  design_Axes_స్థానభ్రంశం.sanitize (c, ఇది, design_Axis_లెక్క) &&
			  స్థానభ్రంశం_గమ్యం_Axis_విలువ_Offsets.sanitize (c, ఇది, axis_విలువ_లెక్క, &(ఇది+స్థానభ్రంశం_గమ్యం_Axis_విలువ_Offsets))));
  }

  రక్షిత:
  hb_array_t<స్థిరరాశి Stat_Axis_రికార్డు> స్థిరరాశి get_design_axes () స్థిరరాశి
  { తిరిగి (ఇది+design_Axes_స్థానభ్రంశం).as_array (design_Axis_లెక్క); }

  hb_array_t<స్థిరరాశి Offset16_గమ్యం<Axis_విలువ>> స్థిరరాశి get_axis_విలువ_offsets () స్థిరరాశి
  { తిరిగి (ఇది+స్థానభ్రంశం_గమ్యం_Axis_విలువ_Offsets).as_array (axis_విలువ_లెక్క); }


  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	design_Axis_కొలత;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_లెక్క;/* The number of design axis records. In a
				 * font with an 'fvar' table, this value must be
				 * greater than or equal to the axisCount value
				 * in the 'fvar' table. In all fonts, must
				 * be greater than zero if axisValueCount
				 * is greater than zero. */
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<Stat_Axis_రికార్డు>>
		design_Axes_స్థానభ్రంశం;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes array. If designAxisCount is zero,
				 * set to zero; if designAxisCount is greater
				 * than zero, must be greater than zero. */
  HBUINT16	axis_విలువ_లెక్క;	/* The number of axis value tables. */
  NNOffset32_గమ్యం<Axis_విలువ_స్థానభ్రంశం_Array>
		స్థానభ్రంశం_గమ్యం_Axis_విలువ_Offsets;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes value offsets array. If axisValueCount
				 * is zero, set to zero; if axisValueCount is
				 * greater than zero, must be greater than zero. */
  NameID	elidedFallbackNameID;
				/* Name ID used as fallback when projection of
				 * names into a particular font model produces
				 * a subfamily name containing only elidable
				 * elements. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (20);
};


} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_STAT_TABLE_HH */
