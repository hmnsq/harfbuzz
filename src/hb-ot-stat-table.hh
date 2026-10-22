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
  ELIDABLE_AXIS_मान_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

स्थिर तार्किक axis_मान_is_outside_axis_range (hb_tag_t axis_tag, भिन्न axis_मान,
                                              अचर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  यदि (!user_axes_location->has (axis_tag))
    फर्काऊ असत्य;

  Triple axis_range = user_axes_location->get (axis_tag);
  फर्काऊ (axis_मान < axis_range.minimum || axis_मान > axis_range.maximum);
}

संरचना StatAxisRecord
{
  पूर्णाङ्क cmp (hb_tag_t key) अचर { फर्काऊ tag.cmp (key); }

  hb_ot_name_id_t get_name_id () अचर { फर्काऊ nameID; }

  hb_tag_t get_axis_tag () अचर { फर्काऊ tag; }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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

संरचना Axis_मान_Format1
{
  अचिन्हित पूर्णाङ्क get_axis_index () अचर { फर्काऊ axisIndex; }
  भिन्न get_मान ()             अचर { फर्काऊ value.to_float (); }

  hb_ot_name_id_t get_मान_name_id () अचर { फर्काऊ मान_Name_ID; }

  hb_tag_t get_axis_tag (अचर hb_array_t<अचर StatAxisRecord> axis_records) अचर
  {
    अचिन्हित axis_idx = get_axis_index ();
    फर्काऊ axis_records[axis_idx].get_axis_tag ();
  }

  तार्किक keep_axis_मान (अचर hb_array_t<अचर StatAxisRecord> axis_records,
                        अचर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) अचर
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    भिन्न axis_मान = get_मान ();

    फर्काऊ !axis_मान_is_outside_axis_range (axis_tag, axis_मान, user_axes_location);
  }

  तार्किक subset (hb_subset_context_t *c,
               अचर hb_array_t<अचर StatAxisRecord> axis_records) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    यदि (keep_axis_मान (axis_records, user_axes_location))
      return_trace (c->serializer->embed (एतत्));

    return_trace (असत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  NameID	मान_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

संरचना Axis_मान_Format2
{
  अचिन्हित पूर्णाङ्क get_axis_index () अचर { फर्काऊ axisIndex; }
  भिन्न get_मान ()             अचर { फर्काऊ nominal_मान.to_float (); }

  hb_ot_name_id_t get_मान_name_id () अचर { फर्काऊ मान_Name_ID; }

  hb_tag_t get_axis_tag (अचर hb_array_t<अचर StatAxisRecord> axis_records) अचर
  {
    अचिन्हित axis_idx = get_axis_index ();
    फर्काऊ axis_records[axis_idx].get_axis_tag ();
  }

  तार्किक keep_axis_मान (अचर hb_array_t<अचर StatAxisRecord> axis_records,
                        अचर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) अचर
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    भिन्न axis_मान = get_मान ();

    फर्काऊ !axis_मान_is_outside_axis_range (axis_tag, axis_मान, user_axes_location);
  }

  तार्किक subset (hb_subset_context_t *c,
               अचर hb_array_t<अचर StatAxisRecord> axis_records) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    यदि (keep_axis_मान (axis_records, user_axes_location))
      return_trace (c->serializer->embed (एतत्));

    return_trace (असत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  NameID	मान_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_मान;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_मान;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_मान;	/* The maximum value for a range associated
				 * with the specified name ID. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (20);
};

संरचना Axis_मान_Format3
{
  अचिन्हित पूर्णाङ्क get_axis_index () अचर { फर्काऊ axisIndex; }
  भिन्न get_मान ()             अचर { फर्काऊ value.to_float (); }

  hb_ot_name_id_t get_मान_name_id () अचर { फर्काऊ मान_Name_ID; }

  hb_tag_t get_axis_tag (अचर hb_array_t<अचर StatAxisRecord> axis_records) अचर
  {
    अचिन्हित axis_idx = get_axis_index ();
    फर्काऊ axis_records[axis_idx].get_axis_tag ();
  }

  तार्किक keep_axis_मान (अचर hb_array_t<अचर StatAxisRecord> axis_records,
                        अचर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) अचर
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    भिन्न axis_मान = get_मान ();

    फर्काऊ !axis_मान_is_outside_axis_range (axis_tag, axis_मान, user_axes_location);
  }

  तार्किक subset (hb_subset_context_t *c,
               अचर hb_array_t<अचर StatAxisRecord> axis_records) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    यदि (keep_axis_मान (axis_records, user_axes_location))
      return_trace (c->serializer->embed (एतत्));

    return_trace (असत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  NameID	मान_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_मान;	/* The numeric value for a style-linked mapping
				 * from this value. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (16);
};

संरचना Axis_मान_Record
{
  अचिन्हित पूर्णाङ्क get_axis_index () अचर { फर्काऊ axisIndex; }
  भिन्न get_मान ()             अचर { फर्काऊ value.to_float (); }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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

संरचना Axis_मान_Format4
{
  अचर Axis_मान_Record &get_axis_record (अचिन्हित पूर्णाङ्क axis_index) अचर
  { फर्काऊ axis_मानहरू_2.as_array (axis_गन्ती_2)[axis_index]; }

  तार्किक keep_axis_मान (अचर hb_array_t<अचर StatAxisRecord> axis_records,
                        अचर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) अचर
  {
    hb_array_t<अचर Axis_मान_Record> axis_मान_records = axis_मानहरू_2.as_array (axis_गन्ती_2);

    लागि (अचर स्वतः& rec : axis_मान_records)
    {
      अचिन्हित axis_idx = rec.get_axis_index ();
      भिन्न axis_मान = rec.get_मान ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      यदि (axis_मान_is_outside_axis_range (axis_tag, axis_मान, user_axes_location))
        फर्काऊ असत्य;
    }

    फर्काऊ सत्य;
  }

  तार्किक subset (hb_subset_context_t *c,
               अचर hb_array_t<अचर StatAxisRecord> axis_records) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    यदि (!keep_axis_मान (axis_records, user_axes_location))
      return_trace (असत्य);

    अचिन्हित total_size = min_size + axis_गन्ती_2 * Axis_मान_Record::static_size;
    स्वतः *out = c->serializer->allocate_size<Axis_मान_Format4> (total_size);
    यदि (unlikely (!out)) return_trace (असत्य);
    hb_memcpy (out, एतत्, total_size);
    return_trace (सत्य);
  }

  hb_ot_name_id_t get_मान_name_id () अचर { फर्काऊ मान_Name_ID; }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
                          axis_मानहरू_2.sanitize (c, axis_गन्ती_2)));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_गन्ती_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मान_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_मान_Record>
		axis_मानहरू_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, axis_मानहरू_2);
};

संरचना Axis_मान
{
  भिन्न get_मान (अचिन्हित पूर्णाङ्क axis_index) अचर
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.get_मान ();
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.get_मान ();
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.get_मान ();
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.get_axis_record (axis_index).get_मान ();
    स्वभाव:फर्काऊ 0.f;
    }
  }

  अचिन्हित पूर्णाङ्क get_axis_index () अचर
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.get_axis_index ();
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.get_axis_index ();
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    स्वभाव:फर्काऊ -1;
    }
  }

  hb_ot_name_id_t get_मान_name_id () अचर
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.get_मान_name_id ();
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.get_मान_name_id ();
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.get_मान_name_id ();
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.get_मान_name_id ();
    स्वभाव:फर्काऊ HB_OT_NAME_ID_INVALID;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) अचर
  {
    यदि (unlikely (!c->may_dispatch (एतत्, &u.format.v))) फर्काऊ c->no_dispatch_return_मान ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    पक्ष 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    स्वभाव:return_trace (c->default_return_मान ());
    }
  }

  तार्किक keep_axis_मान (अचर hb_array_t<अचर StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) अचर
  {
    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.keep_axis_मान (axis_records, user_axes_location);
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.keep_axis_मान (axis_records, user_axes_location);
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.keep_axis_मान (axis_records, user_axes_location);
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.keep_axis_मान (axis_records, user_axes_location);
    स्वभाव:फर्काऊ असत्य;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!c->check_struct (एतत्)))
      return_trace (असत्य);
    hb_barrier ();

    विकल्प (u.format.v)
    {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    पक्ष 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    स्वभाव:return_trace (सत्य);
    }
  }

  रक्षित:
  सङ्घ
  {
  संरचना { HBUINT16 v; }	format;
  Axis_मान_Format1	format1;
  Axis_मान_Format2	format2;
  Axis_मान_Format3	format3;
  Axis_मान_Format4	format4;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};

संरचना Axis_मान_Offset_Array: UnsizedArrayOf<Offset16To<Axis_मान>>
{
  तार्किक subset (hb_subset_context_t *c,
               अचिन्हित axis_मान_गन्ती,
               अचिन्हित& गन्ती,
               अचर hb_array_t<अचर StatAxisRecord> axis_records) अचर
  {
    TRACE_SUBSET (एतत्);

    स्वतः axis_मान_Offsets = as_array (axis_मान_गन्ती);
    गन्ती = 0;
    लागि (अचर स्वतः& offset : axis_मान_Offsets)
    {
      यदि (!offset) जारी;
      स्वतः o_snap = c->serializer->snapshot ();
      स्वतः *o = c->serializer->embed (offset);
      यदि (!o) return_trace (असत्य);
      यदि (!o->serialize_subset (c, offset, एतत्, axis_records))
      {
        c->serializer->revert (o_snap);
        जारी;
      }
      गन्ती++;
    }

    return_trace (गन्ती);
  }
};

संरचना STAT
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  तार्किक has_data () अचर { फर्काऊ version.to_int (); }

  तार्किक get_मान (hb_tag_t tag, भिन्न *value) अचर
  {
    अचिन्हित पूर्णाङ्क axis_index;
    यदि (!get_design_axes ().lfind (tag, &axis_index)) फर्काऊ असत्य;

    hb_array_t<अचर Offset16To<Axis_मान>> axis_मानहरू = get_axis_मान_offsets ();
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < axis_मानहरू.length; i++)
    {
      अचर Axis_मान& axis_मान = एतत्+offset_To_Axis_मान_Offsets+axis_मानहरू[i];
      यदि (axis_मान.get_axis_index () == axis_index)
      {
	यदि (value)
	  *value = axis_मान.get_मान (axis_index);
	फर्काऊ सत्य;
      }
    }
    फर्काऊ असत्य;
  }

  अचिन्हित get_design_axis_गन्ती () अचर { फर्काऊ design_Axis_गन्ती; }

  hb_ot_name_id_t get_axis_record_name_id (अचिन्हित axis_record_index) अचर
  {
    यदि (unlikely (axis_record_index >= design_Axis_गन्ती)) फर्काऊ HB_OT_NAME_ID_INVALID;
    अचर StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    फर्काऊ axis_record.get_name_id ();
  }

  अचिन्हित get_axis_मान_गन्ती () अचर { फर्काऊ axis_मान_गन्ती; }

  hb_ot_name_id_t get_axis_मान_name_id (अचिन्हित axis_मान_index) अचर
  {
    यदि (unlikely (axis_मान_index >= axis_मान_गन्ती)) फर्काऊ HB_OT_NAME_ID_INVALID;
    अचर Axis_मान &axis_मान = (एतत् + get_axis_मान_offsets ()[axis_मान_index]);
    फर्काऊ axis_मान.get_मान_name_id ();
  }

  शून्य collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) अचर
  {
    यदि (!has_data ()) फर्काऊ;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    स्वतः designAxes = get_design_axes ();

    + get_axis_मान_offsets ()
    | hb_map (hb_add (&(एतत् + offset_To_Axis_मान_Offsets)))
    | hb_filter ([&] (अचर Axis_मान& _)
                 { फर्काऊ _.keep_axis_मान (designAxes, user_axes_location); })
    | hb_map (&Axis_मान::get_मान_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    STAT *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    स्वतः designAxes = get_design_axes ();
    लागि (अचिन्हित i = 0; i < (अचिन्हित)design_Axis_गन्ती; i++)
      यदि (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (असत्य);

    यदि (design_Axis_गन्ती)
      c->serializer->check_assign (out->designAxesOffset, एतत्->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    अचिन्हित गन्ती = 0;
    out->offset_To_Axis_मान_Offsets.serialize_subset (c, offset_To_Axis_मान_Offsets, एतत्,
                                                    axis_मान_गन्ती, गन्ती, designAxes);
    return_trace (c->serializer->check_assign (out->axis_मान_गन्ती, गन्ती, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, एतत्, design_Axis_गन्ती) &&
			  offset_To_Axis_मान_Offsets.sanitize (c, एतत्, axis_मान_गन्ती, &(एतत्+offset_To_Axis_मान_Offsets))));
  }

  रक्षित:
  hb_array_t<अचर StatAxisRecord> अचर get_design_axes () अचर
  { फर्काऊ (एतत्+designAxesOffset).as_array (design_Axis_गन्ती); }

  hb_array_t<अचर Offset16To<Axis_मान>> अचर get_axis_मान_offsets () अचर
  { फर्काऊ (एतत्+offset_To_Axis_मान_Offsets).as_array (axis_मान_गन्ती); }


  रक्षित:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_गन्ती;/* The number of design axis records. In a
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
  HBUINT16	axis_मान_गन्ती;	/* The number of axis value tables. */
  NNOffset32To<Axis_मान_Offset_Array>
		offset_To_Axis_मान_Offsets;
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
