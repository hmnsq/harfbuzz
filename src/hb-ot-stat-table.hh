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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#define HB_OT_TAG_STAT HB_TAG('S','T','A','T')


நாமாகாஶ OT {

எண்ணிக்கை
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
  ELIDABLE_AXIS_மதிப்பு_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

நிலையான தருக்க axis_மதிப்பு_is_outside_axis_range (hb_tag_t axis_tag, தசம axis_மதிப்பு,
                                              மாறிலி hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  எனில் (!user_axes_location->has (axis_tag))
    திரும்பு பொய்;

  Triple axis_range = user_axes_location->get (axis_tag);
  திரும்பு (axis_மதிப்பு < axis_range.minimum || axis_மதிப்பு > axis_range.maximum);
}

கட்டமைப்பு StatAxisRecord
{
  முழு cmp (hb_tag_t key) மாறிலி { திரும்பு tag.cmp (key); }

  hb_ot_name_id_t get_name_id () மாறிலி { திரும்பு nameID; }

  hb_tag_t get_axis_tag () மாறிலி { திரும்பு tag; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது)));
  }

  பாதுகாக்கப்பட்ட:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு Axis_மதிப்பு_Format1
{
  unsigned முழு get_axis_index () மாறிலி { திரும்பு axisIndex; }
  தசம get_மதிப்பு ()             மாறிலி { திரும்பு value.to_float (); }

  hb_ot_name_id_t get_மதிப்பு_name_id () மாறிலி { திரும்பு மதிப்பு_Name_ID; }

  hb_tag_t get_axis_tag (மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records) மாறிலி
  {
    unsigned axis_idx = get_axis_index ();
    திரும்பு axis_records[axis_idx].get_axis_tag ();
  }

  தருக்க keep_axis_மதிப்பு (மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records,
                        மாறிலி hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) மாறிலி
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    தசம axis_மதிப்பு = get_மதிப்பு ();

    திரும்பு !axis_மதிப்பு_is_outside_axis_range (axis_tag, axis_மதிப்பு, user_axes_location);
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    எனில் (keep_axis_மதிப்பு (axis_records, user_axes_location))
      return_trace (c->serializer->embed (இது));

    return_trace (பொய்);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	மதிப்பு_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  பொது:
  DEFINE_SIZE_STATIC (12);
};

கட்டமைப்பு Axis_மதிப்பு_Format2
{
  unsigned முழு get_axis_index () மாறிலி { திரும்பு axisIndex; }
  தசம get_மதிப்பு ()             மாறிலி { திரும்பு nominal_மதிப்பு.to_float (); }

  hb_ot_name_id_t get_மதிப்பு_name_id () மாறிலி { திரும்பு மதிப்பு_Name_ID; }

  hb_tag_t get_axis_tag (மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records) மாறிலி
  {
    unsigned axis_idx = get_axis_index ();
    திரும்பு axis_records[axis_idx].get_axis_tag ();
  }

  தருக்க keep_axis_மதிப்பு (மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records,
                        மாறிலி hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) மாறிலி
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    தசம axis_மதிப்பு = get_மதிப்பு ();

    திரும்பு !axis_மதிப்பு_is_outside_axis_range (axis_tag, axis_மதிப்பு, user_axes_location);
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    எனில் (keep_axis_மதிப்பு (axis_records, user_axes_location))
      return_trace (c->serializer->embed (இது));

    return_trace (பொய்);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	மதிப்பு_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_மதிப்பு;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_மதிப்பு;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_மதிப்பு;	/* The maximum value for a range associated
				 * with the specified name ID. */
  பொது:
  DEFINE_SIZE_STATIC (20);
};

கட்டமைப்பு Axis_மதிப்பு_Format3
{
  unsigned முழு get_axis_index () மாறிலி { திரும்பு axisIndex; }
  தசம get_மதிப்பு ()             மாறிலி { திரும்பு value.to_float (); }

  hb_ot_name_id_t get_மதிப்பு_name_id () மாறிலி { திரும்பு மதிப்பு_Name_ID; }

  hb_tag_t get_axis_tag (மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records) மாறிலி
  {
    unsigned axis_idx = get_axis_index ();
    திரும்பு axis_records[axis_idx].get_axis_tag ();
  }

  தருக்க keep_axis_மதிப்பு (மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records,
                        மாறிலி hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) மாறிலி
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    தசம axis_மதிப்பு = get_மதிப்பு ();

    திரும்பு !axis_மதிப்பு_is_outside_axis_range (axis_tag, axis_மதிப்பு, user_axes_location);
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    எனில் (keep_axis_மதிப்பு (axis_records, user_axes_location))
      return_trace (c->serializer->embed (இது));

    return_trace (பொய்);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	மதிப்பு_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_மதிப்பு;	/* The numeric value for a style-linked mapping
				 * from this value. */
  பொது:
  DEFINE_SIZE_STATIC (16);
};

கட்டமைப்பு Axis_மதிப்பு_Record
{
  unsigned முழு get_axis_index () மாறிலி { திரும்பு axisIndex; }
  தசம get_மதிப்பு ()             மாறிலி { திரும்பு value.to_float (); }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு Axis_மதிப்பு_Format4
{
  மாறிலி Axis_மதிப்பு_Record &get_axis_record (unsigned முழு axis_index) மாறிலி
  { திரும்பு axis_மதிப்புகள்_2.as_array (axis_மொத்தம்_2)[axis_index]; }

  தருக்க keep_axis_மதிப்பு (மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records,
                        மாறிலி hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) மாறிலி
  {
    hb_array_t<மாறிலி Axis_மதிப்பு_Record> axis_மதிப்பு_records = axis_மதிப்புகள்_2.as_array (axis_மொத்தம்_2);

    க்கு (மாறிலி auto& rec : axis_மதிப்பு_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      தசம axis_மதிப்பு = rec.get_மதிப்பு ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      எனில் (axis_மதிப்பு_is_outside_axis_range (axis_tag, axis_மதிப்பு, user_axes_location))
        திரும்பு பொய்;
    }

    திரும்பு உண்மை;
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    எனில் (!keep_axis_மதிப்பு (axis_records, user_axes_location))
      return_trace (பொய்);

    unsigned total_size = min_size + axis_மொத்தம்_2 * Axis_மதிப்பு_Record::static_size;
    auto *out = c->serializer->allocate_size<Axis_மதிப்பு_Format4> (total_size);
    எனில் (unlikely (!out)) return_trace (பொய்);
    hb_memcpy (out, இது, total_size);
    return_trace (உண்மை);
  }

  hb_ot_name_id_t get_மதிப்பு_name_id () மாறிலி { திரும்பு மதிப்பு_Name_ID; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
                          axis_மதிப்புகள்_2.sanitize (c, axis_மொத்தம்_2)));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_மொத்தம்_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	மதிப்பு_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_மதிப்பு_Record>
		axis_மதிப்புகள்_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  பொது:
  DEFINE_SIZE_ARRAY (8, axis_மதிப்புகள்_2);
};

கட்டமைப்பு Axis_மதிப்பு
{
  தசம get_மதிப்பு (unsigned முழு axis_index) மாறிலி
  {
    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_மதிப்பு ();
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_மதிப்பு ();
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_மதிப்பு ();
    நிலை 4: hb_barrier (); திரும்பு u.format4.get_axis_record (axis_index).get_மதிப்பு ();
    இயல்பு:திரும்பு 0.f;
    }
  }

  unsigned முழு get_axis_index () மாறிலி
  {
    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_axis_index ();
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_axis_index ();
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    இயல்பு:திரும்பு -1;
    }
  }

  hb_ot_name_id_t get_மதிப்பு_name_id () மாறிலி
  {
    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_மதிப்பு_name_id ();
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_மதிப்பு_name_id ();
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_மதிப்பு_name_id ();
    நிலை 4: hb_barrier (); திரும்பு u.format4.get_மதிப்பு_name_id ();
    இயல்பு:திரும்பு HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, &u.format.v))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, u.format.v);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    நிலை 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    நிலை 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    நிலை 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }

  தருக்க keep_axis_மதிப்பு (மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) மாறிலி
  {
    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); திரும்பு u.format1.keep_axis_மதிப்பு (axis_records, user_axes_location);
    நிலை 2: hb_barrier (); திரும்பு u.format2.keep_axis_மதிப்பு (axis_records, user_axes_location);
    நிலை 3: hb_barrier (); திரும்பு u.format3.keep_axis_மதிப்பு (axis_records, user_axes_location);
    நிலை 4: hb_barrier (); திரும்பு u.format4.keep_axis_மதிப்பு (axis_records, user_axes_location);
    இயல்பு:திரும்பு பொய்;
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது)))
      return_trace (பொய்);
    hb_barrier ();

    தேர்வு (u.format.v)
    {
    நிலை 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    நிலை 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    நிலை 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    நிலை 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    இயல்பு:return_trace (உண்மை);
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு
  {
  கட்டமைப்பு { HBUINT16 v; }	format;
  Axis_மதிப்பு_Format1	format1;
  Axis_மதிப்பு_Format2	format2;
  Axis_மதிப்பு_Format3	format3;
  Axis_மதிப்பு_Format4	format4;
  } u;
  பொது:
  DEFINE_SIZE_UNION (2, format.v);
};

கட்டமைப்பு Axis_மதிப்பு_Offset_Array: UnsizedArrayOf<Offset16To<Axis_மதிப்பு>>
{
  தருக்க subset (hb_subset_context_t *c,
               unsigned axis_மதிப்பு_மொத்தம்,
               unsigned& மொத்தம்,
               மாறிலி hb_array_t<மாறிலி StatAxisRecord> axis_records) மாறிலி
  {
    TRACE_SUBSET (இது);

    auto axis_மதிப்பு_Offsets = as_array (axis_மதிப்பு_மொத்தம்);
    மொத்தம் = 0;
    க்கு (மாறிலி auto& offset : axis_மதிப்பு_Offsets)
    {
      எனில் (!offset) தொடர்;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      எனில் (!o) return_trace (பொய்);
      எனில் (!o->serialize_subset (c, offset, இது, axis_records))
      {
        c->serializer->revert (o_snap);
        தொடர்;
      }
      மொத்தம்++;
    }

    return_trace (மொத்தம்);
  }
};

கட்டமைப்பு STAT
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  தருக்க has_data () மாறிலி { திரும்பு version.to_int (); }

  தருக்க get_மதிப்பு (hb_tag_t tag, தசம *value) மாறிலி
  {
    unsigned முழு axis_index;
    எனில் (!get_design_axes ().lfind (tag, &axis_index)) திரும்பு பொய்;

    hb_array_t<மாறிலி Offset16To<Axis_மதிப்பு>> axis_மதிப்புகள் = get_axis_மதிப்பு_offsets ();
    க்கு (unsigned முழு i = 0; i < axis_மதிப்புகள்.length; i++)
    {
      மாறிலி Axis_மதிப்பு& axis_மதிப்பு = இது+offset_To_Axis_மதிப்பு_Offsets+axis_மதிப்புகள்[i];
      எனில் (axis_மதிப்பு.get_axis_index () == axis_index)
      {
	எனில் (value)
	  *value = axis_மதிப்பு.get_மதிப்பு (axis_index);
	திரும்பு உண்மை;
      }
    }
    திரும்பு பொய்;
  }

  unsigned get_design_axis_மொத்தம் () மாறிலி { திரும்பு design_Axis_மொத்தம்; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) மாறிலி
  {
    எனில் (unlikely (axis_record_index >= design_Axis_மொத்தம்)) திரும்பு HB_OT_NAME_ID_INVALID;
    மாறிலி StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    திரும்பு axis_record.get_name_id ();
  }

  unsigned get_axis_மதிப்பு_மொத்தம் () மாறிலி { திரும்பு axis_மதிப்பு_மொத்தம்; }

  hb_ot_name_id_t get_axis_மதிப்பு_name_id (unsigned axis_மதிப்பு_index) மாறிலி
  {
    எனில் (unlikely (axis_மதிப்பு_index >= axis_மதிப்பு_மொத்தம்)) திரும்பு HB_OT_NAME_ID_INVALID;
    மாறிலி Axis_மதிப்பு &axis_மதிப்பு = (இது + get_axis_மதிப்பு_offsets ()[axis_மதிப்பு_index]);
    திரும்பு axis_மதிப்பு.get_மதிப்பு_name_id ();
  }

  வெறுமை collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) மாறிலி
  {
    எனில் (!has_data ()) திரும்பு;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_மதிப்பு_offsets ()
    | hb_map (hb_add (&(இது + offset_To_Axis_மதிப்பு_Offsets)))
    | hb_filter ([&] (மாறிலி Axis_மதிப்பு& _)
                 { திரும்பு _.keep_axis_மதிப்பு (designAxes, user_axes_location); })
    | hb_map (&Axis_மதிப்பு::get_மதிப்பு_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    STAT *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    auto designAxes = get_design_axes ();
    க்கு (unsigned i = 0; i < (unsigned)design_Axis_மொத்தம்; i++)
      எனில் (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (பொய்);

    எனில் (design_Axis_மொத்தம்)
      c->serializer->check_assign (out->designAxesOffset, இது->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned மொத்தம் = 0;
    out->offset_To_Axis_மதிப்பு_Offsets.serialize_subset (c, offset_To_Axis_மதிப்பு_Offsets, இது,
                                                    axis_மதிப்பு_மொத்தம், மொத்தம், designAxes);
    return_trace (c->serializer->check_assign (out->axis_மதிப்பு_மொத்தம், மொத்தம், HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, இது, design_Axis_மொத்தம்) &&
			  offset_To_Axis_மதிப்பு_Offsets.sanitize (c, இது, axis_மதிப்பு_மொத்தம், &(இது+offset_To_Axis_மதிப்பு_Offsets))));
  }

  பாதுகாக்கப்பட்ட:
  hb_array_t<மாறிலி StatAxisRecord> மாறிலி get_design_axes () மாறிலி
  { திரும்பு (இது+designAxesOffset).as_array (design_Axis_மொத்தம்); }

  hb_array_t<மாறிலி Offset16To<Axis_மதிப்பு>> மாறிலி get_axis_மதிப்பு_offsets () மாறிலி
  { திரும்பு (இது+offset_To_Axis_மதிப்பு_Offsets).as_array (axis_மதிப்பு_மொத்தம்); }


  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_மொத்தம்;/* The number of design axis records. In a
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
  HBUINT16	axis_மதிப்பு_மொத்தம்;	/* The number of axis value tables. */
  NNOffset32To<Axis_மதிப்பு_Offset_Array>
		offset_To_Axis_மதிப்பு_Offsets;
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
  பொது:
  DEFINE_SIZE_STATIC (20);
};


} /* namespace OT */


#endif /* HB_OT_STAT_TABLE_HH */
