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
  ELIDABLE_AXIS_मूल्य_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

स्थिर तार्किक axis_मूल्य_is_outside_axis_range (hb_tag_t axis_tag, दशांश axis_मूल्य,
                                              स्थिरांक hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  जर (!user_axes_location->has (axis_tag))
    परत असत्य;

  Triple axis_range = user_axes_location->get (axis_tag);
  परत (axis_मूल्य < axis_range.minimum || axis_मूल्य > axis_range.maximum);
}

संरचना StatAxisRecord
{
  पूर्णांक cmp (hb_tag_t key) स्थिरांक { परत tag.cmp (key); }

  hb_ot_name_id_t get_name_id () स्थिरांक { परत nameID; }

  hb_tag_t get_axis_tag () स्थिरांक { परत tag; }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे)));
  }

  संरक्षित:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना Axis_मूल्य_Format1
{
  अचिन्हित पूर्णांक get_axis_index () स्थिरांक { परत axisIndex; }
  दशांश get_मूल्य ()             स्थिरांक { परत value.to_float (); }

  hb_ot_name_id_t get_मूल्य_name_id () स्थिरांक { परत मूल्य_Name_ID; }

  hb_tag_t get_axis_tag (स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records) स्थिरांक
  {
    अचिन्हित axis_idx = get_axis_index ();
    परत axis_records[axis_idx].get_axis_tag ();
  }

  तार्किक keep_axis_मूल्य (स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records,
                        स्थिरांक hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) स्थिरांक
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    दशांश axis_मूल्य = get_मूल्य ();

    परत !axis_मूल्य_is_outside_axis_range (axis_tag, axis_मूल्य, user_axes_location);
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    जर (keep_axis_मूल्य (axis_records, user_axes_location))
      return_trace (c->serializer->embed (हे));

    return_trace (असत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मूल्य_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

संरचना Axis_मूल्य_Format2
{
  अचिन्हित पूर्णांक get_axis_index () स्थिरांक { परत axisIndex; }
  दशांश get_मूल्य ()             स्थिरांक { परत nominal_मूल्य.to_float (); }

  hb_ot_name_id_t get_मूल्य_name_id () स्थिरांक { परत मूल्य_Name_ID; }

  hb_tag_t get_axis_tag (स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records) स्थिरांक
  {
    अचिन्हित axis_idx = get_axis_index ();
    परत axis_records[axis_idx].get_axis_tag ();
  }

  तार्किक keep_axis_मूल्य (स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records,
                        स्थिरांक hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) स्थिरांक
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    दशांश axis_मूल्य = get_मूल्य ();

    परत !axis_मूल्य_is_outside_axis_range (axis_tag, axis_मूल्य, user_axes_location);
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    जर (keep_axis_मूल्य (axis_records, user_axes_location))
      return_trace (c->serializer->embed (हे));

    return_trace (असत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मूल्य_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_मूल्य;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_मूल्य;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_मूल्य;	/* The maximum value for a range associated
				 * with the specified name ID. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (20);
};

संरचना Axis_मूल्य_Format3
{
  अचिन्हित पूर्णांक get_axis_index () स्थिरांक { परत axisIndex; }
  दशांश get_मूल्य ()             स्थिरांक { परत value.to_float (); }

  hb_ot_name_id_t get_मूल्य_name_id () स्थिरांक { परत मूल्य_Name_ID; }

  hb_tag_t get_axis_tag (स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records) स्थिरांक
  {
    अचिन्हित axis_idx = get_axis_index ();
    परत axis_records[axis_idx].get_axis_tag ();
  }

  तार्किक keep_axis_मूल्य (स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records,
                        स्थिरांक hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) स्थिरांक
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    दशांश axis_मूल्य = get_मूल्य ();

    परत !axis_मूल्य_is_outside_axis_range (axis_tag, axis_मूल्य, user_axes_location);
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    जर (keep_axis_मूल्य (axis_records, user_axes_location))
      return_trace (c->serializer->embed (हे));

    return_trace (असत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मूल्य_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_मूल्य;	/* The numeric value for a style-linked mapping
				 * from this value. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (16);
};

संरचना Axis_मूल्य_Record
{
  अचिन्हित पूर्णांक get_axis_index () स्थिरांक { परत axisIndex; }
  दशांश get_मूल्य ()             स्थिरांक { परत value.to_float (); }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  संरक्षित:
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना Axis_मूल्य_Format4
{
  स्थिरांक Axis_मूल्य_Record &get_axis_record (अचिन्हित पूर्णांक axis_index) स्थिरांक
  { परत axis_मूल्ये_2.as_array (axis_मोजणी_2)[axis_index]; }

  तार्किक keep_axis_मूल्य (स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records,
                        स्थिरांक hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) स्थिरांक
  {
    hb_array_t<स्थिरांक Axis_मूल्य_Record> axis_मूल्य_records = axis_मूल्ये_2.as_array (axis_मोजणी_2);

    साठी (स्थिरांक स्वतः& rec : axis_मूल्य_records)
    {
      अचिन्हित axis_idx = rec.get_axis_index ();
      दशांश axis_मूल्य = rec.get_मूल्य ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      जर (axis_मूल्य_is_outside_axis_range (axis_tag, axis_मूल्य, user_axes_location))
        परत असत्य;
    }

    परत सत्य;
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    जर (!keep_axis_मूल्य (axis_records, user_axes_location))
      return_trace (असत्य);

    अचिन्हित total_size = min_size + axis_मोजणी_2 * Axis_मूल्य_Record::static_size;
    स्वतः *out = c->serializer->allocate_size<Axis_मूल्य_Format4> (total_size);
    जर (unlikely (!out)) return_trace (असत्य);
    hb_memcpy (out, हे, total_size);
    return_trace (सत्य);
  }

  hb_ot_name_id_t get_मूल्य_name_id () स्थिरांक { परत मूल्य_Name_ID; }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  hb_barrier () &&
                          axis_मूल्ये_2.sanitize (c, axis_मोजणी_2)));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_मोजणी_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	मूल्य_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_मूल्य_Record>
		axis_मूल्ये_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, axis_मूल्ये_2);
};

संरचना Axis_मूल्य
{
  दशांश get_मूल्य (अचिन्हित पूर्णांक axis_index) स्थिरांक
  {
    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); परत u.format1.get_मूल्य ();
    स्थिती 2: hb_barrier (); परत u.format2.get_मूल्य ();
    स्थिती 3: hb_barrier (); परत u.format3.get_मूल्य ();
    स्थिती 4: hb_barrier (); परत u.format4.get_axis_record (axis_index).get_मूल्य ();
    पूर्वनिर्धारित:परत 0.f;
    }
  }

  अचिन्हित पूर्णांक get_axis_index () स्थिरांक
  {
    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); परत u.format1.get_axis_index ();
    स्थिती 2: hb_barrier (); परत u.format2.get_axis_index ();
    स्थिती 3: hb_barrier (); परत u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    पूर्वनिर्धारित:परत -1;
    }
  }

  hb_ot_name_id_t get_मूल्य_name_id () स्थिरांक
  {
    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); परत u.format1.get_मूल्य_name_id ();
    स्थिती 2: hb_barrier (); परत u.format2.get_मूल्य_name_id ();
    स्थिती 3: hb_barrier (); परत u.format3.get_मूल्य_name_id ();
    स्थिती 4: hb_barrier (); परत u.format4.get_मूल्य_name_id ();
    पूर्वनिर्धारित:परत HB_OT_NAME_ID_INVALID;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, &u.format.v))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, u.format.v);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    स्थिती 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    स्थिती 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    स्थिती 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }

  तार्किक keep_axis_मूल्य (स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) स्थिरांक
  {
    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); परत u.format1.keep_axis_मूल्य (axis_records, user_axes_location);
    स्थिती 2: hb_barrier (); परत u.format2.keep_axis_मूल्य (axis_records, user_axes_location);
    स्थिती 3: hb_barrier (); परत u.format3.keep_axis_मूल्य (axis_records, user_axes_location);
    स्थिती 4: hb_barrier (); परत u.format4.keep_axis_मूल्य (axis_records, user_axes_location);
    पूर्वनिर्धारित:परत असत्य;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!c->check_struct (हे)))
      return_trace (असत्य);
    hb_barrier ();

    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    स्थिती 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    स्थिती 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    स्थिती 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    पूर्वनिर्धारित:return_trace (सत्य);
    }
  }

  संरक्षित:
  संघ
  {
  संरचना { HBUINT16 v; }	format;
  Axis_मूल्य_Format1	format1;
  Axis_मूल्य_Format2	format2;
  Axis_मूल्य_Format3	format3;
  Axis_मूल्य_Format4	format4;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};

संरचना Axis_मूल्य_Offset_Array: UnsizedArrayOf<Offset16To<Axis_मूल्य>>
{
  तार्किक subset (hb_subset_context_t *c,
               अचिन्हित axis_मूल्य_मोजणी,
               अचिन्हित& मोजणी,
               स्थिरांक hb_array_t<स्थिरांक StatAxisRecord> axis_records) स्थिरांक
  {
    TRACE_SUBSET (हे);

    स्वतः axis_मूल्य_Offsets = as_array (axis_मूल्य_मोजणी);
    मोजणी = 0;
    साठी (स्थिरांक स्वतः& offset : axis_मूल्य_Offsets)
    {
      जर (!offset) सुरूठेवा;
      स्वतः o_snap = c->serializer->snapshot ();
      स्वतः *o = c->serializer->embed (offset);
      जर (!o) return_trace (असत्य);
      जर (!o->serialize_subset (c, offset, हे, axis_records))
      {
        c->serializer->revert (o_snap);
        सुरूठेवा;
      }
      मोजणी++;
    }

    return_trace (मोजणी);
  }
};

संरचना STAT
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  तार्किक has_data () स्थिरांक { परत version.to_int (); }

  तार्किक get_मूल्य (hb_tag_t tag, दशांश *value) स्थिरांक
  {
    अचिन्हित पूर्णांक axis_index;
    जर (!get_design_axes ().lfind (tag, &axis_index)) परत असत्य;

    hb_array_t<स्थिरांक Offset16To<Axis_मूल्य>> axis_मूल्ये = get_axis_मूल्य_offsets ();
    साठी (अचिन्हित पूर्णांक i = 0; i < axis_मूल्ये.length; i++)
    {
      स्थिरांक Axis_मूल्य& axis_मूल्य = हे+offset_To_Axis_मूल्य_Offsets+axis_मूल्ये[i];
      जर (axis_मूल्य.get_axis_index () == axis_index)
      {
	जर (value)
	  *value = axis_मूल्य.get_मूल्य (axis_index);
	परत सत्य;
      }
    }
    परत असत्य;
  }

  अचिन्हित get_design_axis_मोजणी () स्थिरांक { परत design_Axis_मोजणी; }

  hb_ot_name_id_t get_axis_record_name_id (अचिन्हित axis_record_index) स्थिरांक
  {
    जर (unlikely (axis_record_index >= design_Axis_मोजणी)) परत HB_OT_NAME_ID_INVALID;
    स्थिरांक StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    परत axis_record.get_name_id ();
  }

  अचिन्हित get_axis_मूल्य_मोजणी () स्थिरांक { परत axis_मूल्य_मोजणी; }

  hb_ot_name_id_t get_axis_मूल्य_name_id (अचिन्हित axis_मूल्य_index) स्थिरांक
  {
    जर (unlikely (axis_मूल्य_index >= axis_मूल्य_मोजणी)) परत HB_OT_NAME_ID_INVALID;
    स्थिरांक Axis_मूल्य &axis_मूल्य = (हे + get_axis_मूल्य_offsets ()[axis_मूल्य_index]);
    परत axis_मूल्य.get_मूल्य_name_id ();
  }

  शून्य collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) स्थिरांक
  {
    जर (!has_data ()) परत;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    स्वतः designAxes = get_design_axes ();

    + get_axis_मूल्य_offsets ()
    | hb_map (hb_add (&(हे + offset_To_Axis_मूल्य_Offsets)))
    | hb_filter ([&] (स्थिरांक Axis_मूल्य& _)
                 { परत _.keep_axis_मूल्य (designAxes, user_axes_location); })
    | hb_map (&Axis_मूल्य::get_मूल्य_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    STAT *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    स्वतः designAxes = get_design_axes ();
    साठी (अचिन्हित i = 0; i < (अचिन्हित)design_Axis_मोजणी; i++)
      जर (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (असत्य);

    जर (design_Axis_मोजणी)
      c->serializer->check_assign (out->designAxesOffset, हे->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    अचिन्हित मोजणी = 0;
    out->offset_To_Axis_मूल्य_Offsets.serialize_subset (c, offset_To_Axis_मूल्य_Offsets, हे,
                                                    axis_मूल्य_मोजणी, मोजणी, designAxes);
    return_trace (c->serializer->check_assign (out->axis_मूल्य_मोजणी, मोजणी, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, हे, design_Axis_मोजणी) &&
			  offset_To_Axis_मूल्य_Offsets.sanitize (c, हे, axis_मूल्य_मोजणी, &(हे+offset_To_Axis_मूल्य_Offsets))));
  }

  संरक्षित:
  hb_array_t<स्थिरांक StatAxisRecord> स्थिरांक get_design_axes () स्थिरांक
  { परत (हे+designAxesOffset).as_array (design_Axis_मोजणी); }

  hb_array_t<स्थिरांक Offset16To<Axis_मूल्य>> स्थिरांक get_axis_मूल्य_offsets () स्थिरांक
  { परत (हे+offset_To_Axis_मूल्य_Offsets).as_array (axis_मूल्य_मोजणी); }


  संरक्षित:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_मोजणी;/* The number of design axis records. In a
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
  HBUINT16	axis_मूल्य_मोजणी;	/* The number of axis value tables. */
  NNOffset32To<Axis_मूल्य_Offset_Array>
		offset_To_Axis_मूल्य_Offsets;
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
