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

#ifndef HB_OT_STAT_TABLE_HH
#define HB_OT_STAT_TABLE_HH

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#define HB_OT_TAG_STAT HB_TAG('S','T','A','T')


नामाकाश OT {

गणना
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
  ELIDABLE_AXIS_मानम्_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

स्थिर तार्किक axis_मानम्_is_outside_axis_range (hb_tag_t axis_tag, भिन्न axis_मानम्,
                                              ध्रुव hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  सचे (!user_axes_location->has (axis_tag))
    पटिनिवत्त मिच्छा;

  Triple axis_range = user_axes_location->get (axis_tag);
  पटिनिवत्त (axis_मानम् < axis_range.minimum || axis_मानम् > axis_range.maximum);
}

संरचना StatAxisRecord
{
  पूर्णाङ्क cmp (hb_tag_t key) ध्रुव { पटिनिवत्त tag.cmp (key); }

  hb_ot_name_id_t get_name_id () ध्रुव { पटिनिवत्त nameID; }

  hb_tag_t get_axis_tag () ध्रुव { पटिनिवत्त tag; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्)));
  }

  रक्षित:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना Axis_मानम्_Format1
{
  unsigned पूर्णाङ्क get_axis_index () ध्रुव { पटिनिवत्त axisIndex; }
  भिन्न get_मानम् ()             ध्रुव { पटिनिवत्त value.to_float (); }

  hb_ot_name_id_t get_मानम्_name_id () ध्रुव { पटिनिवत्त मानम्_Name_ID; }

  hb_tag_t get_axis_tag (ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records) ध्रुव
  {
    unsigned axis_idx = get_axis_index ();
    पटिनिवत्त axis_records[axis_idx].get_axis_tag ();
  }

  तार्किक keep_axis_मानम् (ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records,
                        ध्रुव hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ध्रुव
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    भिन्न axis_मानम् = get_मानम् ();

    पटिनिवत्त !axis_मानम्_is_outside_axis_range (axis_tag, axis_मानम्, user_axes_location);
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    सचे (keep_axis_मानम् (axis_records, user_axes_location))
      return_trace (c->serializer->embed (एतत्));

    return_trace (मिच्छा);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मानम्_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

संरचना Axis_मानम्_Format2
{
  unsigned पूर्णाङ्क get_axis_index () ध्रुव { पटिनिवत्त axisIndex; }
  भिन्न get_मानम् ()             ध्रुव { पटिनिवत्त nominal_मानम्.to_float (); }

  hb_ot_name_id_t get_मानम्_name_id () ध्रुव { पटिनिवत्त मानम्_Name_ID; }

  hb_tag_t get_axis_tag (ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records) ध्रुव
  {
    unsigned axis_idx = get_axis_index ();
    पटिनिवत्त axis_records[axis_idx].get_axis_tag ();
  }

  तार्किक keep_axis_मानम् (ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records,
                        ध्रुव hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ध्रुव
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    भिन्न axis_मानम् = get_मानम् ();

    पटिनिवत्त !axis_मानम्_is_outside_axis_range (axis_tag, axis_मानम्, user_axes_location);
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    सचे (keep_axis_मानम् (axis_records, user_axes_location))
      return_trace (c->serializer->embed (एतत्));

    return_trace (मिच्छा);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मानम्_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_मानम्;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_मानम्;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_मानम्;	/* The maximum value for a range associated
				 * with the specified name ID. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (20);
};

संरचना Axis_मानम्_Format3
{
  unsigned पूर्णाङ्क get_axis_index () ध्रुव { पटिनिवत्त axisIndex; }
  भिन्न get_मानम् ()             ध्रुव { पटिनिवत्त value.to_float (); }

  hb_ot_name_id_t get_मानम्_name_id () ध्रुव { पटिनिवत्त मानम्_Name_ID; }

  hb_tag_t get_axis_tag (ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records) ध्रुव
  {
    unsigned axis_idx = get_axis_index ();
    पटिनिवत्त axis_records[axis_idx].get_axis_tag ();
  }

  तार्किक keep_axis_मानम् (ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records,
                        ध्रुव hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ध्रुव
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    भिन्न axis_मानम् = get_मानम् ();

    पटिनिवत्त !axis_मानम्_is_outside_axis_range (axis_tag, axis_मानम्, user_axes_location);
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    सचे (keep_axis_मानम् (axis_records, user_axes_location))
      return_trace (c->serializer->embed (एतत्));

    return_trace (मिच्छा);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मानम्_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_मानम्;	/* The numeric value for a style-linked mapping
				 * from this value. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (16);
};

संरचना Axis_मानम्_Record
{
  unsigned पूर्णाङ्क get_axis_index () ध्रुव { पटिनिवत्त axisIndex; }
  भिन्न get_मानम् ()             ध्रुव { पटिनिवत्त value.to_float (); }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना Axis_मानम्_Format4
{
  ध्रुव Axis_मानम्_Record &get_axis_record (unsigned पूर्णाङ्क axis_index) ध्रुव
  { पटिनिवत्त axis_मानानि_2.as_array (axis_गणनम्_2)[axis_index]; }

  तार्किक keep_axis_मानम् (ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records,
                        ध्रुव hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ध्रुव
  {
    hb_array_t<ध्रुव Axis_मानम्_Record> axis_मानम्_records = axis_मानानि_2.as_array (axis_गणनम्_2);

    अत्थाय (ध्रुव auto& rec : axis_मानम्_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      भिन्न axis_मानम् = rec.get_मानम् ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      सचे (axis_मानम्_is_outside_axis_range (axis_tag, axis_मानम्, user_axes_location))
        पटिनिवत्त मिच्छा;
    }

    पटिनिवत्त सच्च;
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    सचे (!keep_axis_मानम् (axis_records, user_axes_location))
      return_trace (मिच्छा);

    unsigned total_size = min_size + axis_गणनम्_2 * Axis_मानम्_Record::static_size;
    auto *out = c->serializer->allocate_size<Axis_मानम्_Format4> (total_size);
    सचे (unlikely (!out)) return_trace (मिच्छा);
    hb_memcpy (out, एतत्, total_size);
    return_trace (सच्च);
  }

  hb_ot_name_id_t get_मानम्_name_id () ध्रुव { पटिनिवत्त मानम्_Name_ID; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
                          axis_मानानि_2.sanitize (c, axis_गणनम्_2)));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_गणनम्_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मानम्_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_मानम्_Record>
		axis_मानानि_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, axis_मानानि_2);
};

संरचना Axis_मानम्
{
  भिन्न get_मानम् (unsigned पूर्णाङ्क axis_index) ध्रुव
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.get_मानम् ();
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.get_मानम् ();
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.get_मानम् ();
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.get_axis_record (axis_index).get_मानम् ();
    स्वभाव:पटिनिवत्त 0.f;
    }
  }

  unsigned पूर्णाङ्क get_axis_index () ध्रुव
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.get_axis_index ();
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.get_axis_index ();
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    स्वभाव:पटिनिवत्त -1;
    }
  }

  hb_ot_name_id_t get_मानम्_name_id () ध्रुव
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.get_मानम्_name_id ();
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.get_मानम्_name_id ();
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.get_मानम्_name_id ();
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.get_मानम्_name_id ();
    स्वभाव:पटिनिवत्त HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    सचे (unlikely (!c->may_dispatch (एतत्, &u.format.v))) पटिनिवत्त c->no_dispatch_return_मानम् ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    पक्ष 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    स्वभाव:return_trace (c->default_return_मानम् ());
    }
  }

  तार्किक keep_axis_मानम् (ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ध्रुव
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); पटिनिवत्त u.format1.keep_axis_मानम् (axis_records, user_axes_location);
    पक्ष 2: hb_barrier (); पटिनिवत्त u.format2.keep_axis_मानम् (axis_records, user_axes_location);
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.keep_axis_मानम् (axis_records, user_axes_location);
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.keep_axis_मानम् (axis_records, user_axes_location);
    स्वभाव:पटिनिवत्त मिच्छा;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!c->check_struct (एतत्)))
      return_trace (मिच्छा);
    hb_barrier ();

    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    पक्ष 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    स्वभाव:return_trace (सच्च);
    }
  }

  रक्षित:
  सङ्घ
  {
  संरचना { HBUINT16 v; }	format;
  Axis_मानम्_Format1	format1;
  Axis_मानम्_Format2	format2;
  Axis_मानम्_Format3	format3;
  Axis_मानम्_Format4	format4;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};

संरचना Axis_मानम्_Offset_Array: UnsizedArrayOf<Offset16To<Axis_मानम्>>
{
  तार्किक subset (hb_subset_context_t *c,
               unsigned axis_मानम्_गणनम्,
               unsigned& गणनम्,
               ध्रुव hb_array_t<ध्रुव StatAxisRecord> axis_records) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    auto axis_मानम्_Offsets = as_array (axis_मानम्_गणनम्);
    गणनम् = 0;
    अत्थाय (ध्रुव auto& offset : axis_मानम्_Offsets)
    {
      सचे (!offset) अनुवर्त;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      सचे (!o) return_trace (मिच्छा);
      सचे (!o->serialize_subset (c, offset, एतत्, axis_records))
      {
        c->serializer->revert (o_snap);
        अनुवर्त;
      }
      गणनम्++;
    }

    return_trace (गणनम्);
  }
};

संरचना STAT
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  तार्किक has_data () ध्रुव { पटिनिवत्त version.to_int (); }

  तार्किक get_मानम् (hb_tag_t tag, भिन्न *value) ध्रुव
  {
    unsigned पूर्णाङ्क axis_index;
    सचे (!get_design_axes ().lfind (tag, &axis_index)) पटिनिवत्त मिच्छा;

    hb_array_t<ध्रुव Offset16To<Axis_मानम्>> axis_मानानि = get_axis_मानम्_offsets ();
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < axis_मानानि.length; i++)
    {
      ध्रुव Axis_मानम्& axis_मानम् = एतत्+offset_To_Axis_मानम्_Offsets+axis_मानानि[i];
      सचे (axis_मानम्.get_axis_index () == axis_index)
      {
	सचे (value)
	  *value = axis_मानम्.get_मानम् (axis_index);
	पटिनिवत्त सच्च;
      }
    }
    पटिनिवत्त मिच्छा;
  }

  unsigned get_design_axis_गणनम् () ध्रुव { पटिनिवत्त design_Axis_गणनम्; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) ध्रुव
  {
    सचे (unlikely (axis_record_index >= design_Axis_गणनम्)) पटिनिवत्त HB_OT_NAME_ID_INVALID;
    ध्रुव StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    पटिनिवत्त axis_record.get_name_id ();
  }

  unsigned get_axis_मानम्_गणनम् () ध्रुव { पटिनिवत्त axis_मानम्_गणनम्; }

  hb_ot_name_id_t get_axis_मानम्_name_id (unsigned axis_मानम्_index) ध्रुव
  {
    सचे (unlikely (axis_मानम्_index >= axis_मानम्_गणनम्)) पटिनिवत्त HB_OT_NAME_ID_INVALID;
    ध्रुव Axis_मानम् &axis_मानम् = (एतत् + get_axis_मानम्_offsets ()[axis_मानम्_index]);
    पटिनिवत्त axis_मानम्.get_मानम्_name_id ();
  }

  शून्य collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) ध्रुव
  {
    सचे (!has_data ()) पटिनिवत्त;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_मानम्_offsets ()
    | hb_map (hb_add (&(एतत् + offset_To_Axis_मानम्_Offsets)))
    | hb_filter ([&] (ध्रुव Axis_मानम्& _)
                 { पटिनिवत्त _.keep_axis_मानम् (designAxes, user_axes_location); })
    | hb_map (&Axis_मानम्::get_मानम्_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    STAT *out = c->serializer->embed (एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);

    auto designAxes = get_design_axes ();
    अत्थाय (unsigned i = 0; i < (unsigned)design_Axis_गणनम्; i++)
      सचे (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (मिच्छा);

    सचे (design_Axis_गणनम्)
      c->serializer->check_assign (out->designAxesOffset, एतत्->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned गणनम् = 0;
    out->offset_To_Axis_मानम्_Offsets.serialize_subset (c, offset_To_Axis_मानम्_Offsets, एतत्,
                                                    axis_मानम्_गणनम्, गणनम्, designAxes);
    return_trace (c->serializer->check_assign (out->axis_मानम्_गणनम्, गणनम्, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, एतत्, design_Axis_गणनम्) &&
			  offset_To_Axis_मानम्_Offsets.sanitize (c, एतत्, axis_मानम्_गणनम्, &(एतत्+offset_To_Axis_मानम्_Offsets))));
  }

  रक्षित:
  hb_array_t<ध्रुव StatAxisRecord> ध्रुव get_design_axes () ध्रुव
  { पटिनिवत्त (एतत्+designAxesOffset).as_array (design_Axis_गणनम्); }

  hb_array_t<ध्रुव Offset16To<Axis_मानम्>> ध्रुव get_axis_मानम्_offsets () ध्रुव
  { पटिनिवत्त (एतत्+offset_To_Axis_मानम्_Offsets).as_array (axis_मानम्_गणनम्); }


  रक्षित:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_गणनम्;/* The number of design axis records. In a
				 * font with an 'fvar' table, this value must be
				 * greater than or equal to the axisCount value
				 * in the 'fvar' table. In all fonts, must
				 * be greater than zero if axisValueCount
				 * is greater than zero. */
  NNOffset32To<UnsizedArrayOf<StatAxisRecord>>
		designAxesOffset;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes array. If designAxisCount is zero,
				 * set to zero; if designAxisCount is greater
				 * than zero, must be greater than zero. */
  HBUINT16	axis_मानम्_गणनम्;	/* The number of axis value tables. */
  NNOffset32To<Axis_मानम्_Offset_Array>
		offset_To_Axis_मानम्_Offsets;
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (20);
};


} /* namespace OT */


#endif /* HB_OT_STAT_TABLE_HH */
