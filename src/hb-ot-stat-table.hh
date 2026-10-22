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

#অন্তর্ভাৱ "hb-open-type.hh"
#অন্তর্ভাৱ "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#define HB_OT_TAG_STAT HB_TAG('S','T','A','T')


নামাকাশ OT {

গণনা
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
  ELIDABLE_AXIS_মান_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

স্থির তার্কিক axis_মান_is_outside_axis_range (hb_tag_t axis_tag, ভিন্ন axis_মান,
                                              ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  যদি (!user_axes_location->has (axis_tag))
    ঘূৰাই মিছা;

  Triple axis_range = user_axes_location->get (axis_tag);
  ঘূৰাই (axis_মান < axis_range.minimum || axis_মান > axis_range.maximum);
}

সংরচনা StatAxisRecord
{
  পূর্ণাঙ্ক cmp (hb_tag_t key) ধ্রুৱ { ঘূৰাই tag.cmp (key); }

  hb_ot_name_id_t get_name_id () ধ্রুৱ { ঘূৰাই nameID; }

  hb_tag_t get_axis_tag () ধ্রুৱ { ঘূৰাই tag; }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্)));
  }

  রক্ষিত:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা Axis_মান_Format1
{
  unsigned পূর্ণাঙ্ক get_axis_index () ধ্রুৱ { ঘূৰাই axisIndex; }
  ভিন্ন get_মান ()             ধ্রুৱ { ঘূৰাই value.to_float (); }

  hb_ot_name_id_t get_মান_name_id () ধ্রুৱ { ঘূৰাই মান_Name_ID; }

  hb_tag_t get_axis_tag (ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records) ধ্রুৱ
  {
    unsigned axis_idx = get_axis_index ();
    ঘূৰাই axis_records[axis_idx].get_axis_tag ();
  }

  তার্কিক keep_axis_মান (ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records,
                        ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ধ্রুৱ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ভিন্ন axis_মান = get_মান ();

    ঘূৰাই !axis_মান_is_outside_axis_range (axis_tag, axis_মান, user_axes_location);
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    যদি (keep_axis_মান (axis_records, user_axes_location))
      return_trace (c->serializer->embed (এতত্));

    return_trace (মিছা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	মান_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (12);
};

সংরচনা Axis_মান_Format2
{
  unsigned পূর্ণাঙ্ক get_axis_index () ধ্রুৱ { ঘূৰাই axisIndex; }
  ভিন্ন get_মান ()             ধ্রুৱ { ঘূৰাই nominal_মান.to_float (); }

  hb_ot_name_id_t get_মান_name_id () ধ্রুৱ { ঘূৰাই মান_Name_ID; }

  hb_tag_t get_axis_tag (ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records) ধ্রুৱ
  {
    unsigned axis_idx = get_axis_index ();
    ঘূৰাই axis_records[axis_idx].get_axis_tag ();
  }

  তার্কিক keep_axis_মান (ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records,
                        ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ধ্রুৱ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ভিন্ন axis_মান = get_মান ();

    ঘূৰাই !axis_মান_is_outside_axis_range (axis_tag, axis_মান, user_axes_location);
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    যদি (keep_axis_মান (axis_records, user_axes_location))
      return_trace (c->serializer->embed (এতত্));

    return_trace (মিছা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	মান_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_মান;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_মান;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_মান;	/* The maximum value for a range associated
				 * with the specified name ID. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (20);
};

সংরচনা Axis_মান_Format3
{
  unsigned পূর্ণাঙ্ক get_axis_index () ধ্রুৱ { ঘূৰাই axisIndex; }
  ভিন্ন get_মান ()             ধ্রুৱ { ঘূৰাই value.to_float (); }

  hb_ot_name_id_t get_মান_name_id () ধ্রুৱ { ঘূৰাই মান_Name_ID; }

  hb_tag_t get_axis_tag (ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records) ধ্রুৱ
  {
    unsigned axis_idx = get_axis_index ();
    ঘূৰাই axis_records[axis_idx].get_axis_tag ();
  }

  তার্কিক keep_axis_মান (ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records,
                        ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ধ্রুৱ
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    ভিন্ন axis_মান = get_মান ();

    ঘূৰাই !axis_মান_is_outside_axis_range (axis_tag, axis_মান, user_axes_location);
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    যদি (keep_axis_মান (axis_records, user_axes_location))
      return_trace (c->serializer->embed (এতত্));

    return_trace (মিছা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	মান_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_মান;	/* The numeric value for a style-linked mapping
				 * from this value. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (16);
};

সংরচনা Axis_মান_Record
{
  unsigned পূর্ণাঙ্ক get_axis_index () ধ্রুৱ { ঘূৰাই axisIndex; }
  ভিন্ন get_মান ()             ধ্রুৱ { ঘূৰাই value.to_float (); }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  রক্ষিত:
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা Axis_মান_Format4
{
  ধ্রুৱ Axis_মান_Record &get_axis_record (unsigned পূর্ণাঙ্ক axis_index) ধ্রুৱ
  { ঘূৰাই axis_মানসমূহ_2.as_array (axis_গুনতি_2)[axis_index]; }

  তার্কিক keep_axis_মান (ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records,
                        ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ধ্রুৱ
  {
    hb_array_t<ধ্রুৱ Axis_মান_Record> axis_মান_records = axis_মানসমূহ_2.as_array (axis_গুনতি_2);

    বাবে (ধ্রুৱ auto& rec : axis_মান_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      ভিন্ন axis_মান = rec.get_মান ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      যদি (axis_মান_is_outside_axis_range (axis_tag, axis_মান, user_axes_location))
        ঘূৰাই মিছা;
    }

    ঘূৰাই সঁচা;
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    যদি (!keep_axis_মান (axis_records, user_axes_location))
      return_trace (মিছা);

    unsigned total_size = min_size + axis_গুনতি_2 * Axis_মান_Record::static_size;
    auto *out = c->serializer->allocate_size<Axis_মান_Format4> (total_size);
    যদি (unlikely (!out)) return_trace (মিছা);
    hb_memcpy (out, এতত্, total_size);
    return_trace (সঁচা);
  }

  hb_ot_name_id_t get_মান_name_id () ধ্রুৱ { ঘূৰাই মান_Name_ID; }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  hb_barrier () &&
                          axis_মানসমূহ_2.sanitize (c, axis_গুনতি_2)));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_গুনতি_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	মান_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_মান_Record>
		axis_মানসমূহ_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (8, axis_মানসমূহ_2);
};

সংরচনা Axis_মান
{
  ভিন্ন get_মান (unsigned পূর্ণাঙ্ক axis_index) ধ্রুৱ
  {
    ৱিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_মান ();
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.get_মান ();
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.get_মান ();
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.get_axis_record (axis_index).get_মান ();
    স্ৱভাৱ:ঘূৰাই 0.f;
    }
  }

  unsigned পূর্ণাঙ্ক get_axis_index () ধ্রুৱ
  {
    ৱিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_axis_index ();
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.get_axis_index ();
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    স্ৱভাৱ:ঘূৰাই -1;
    }
  }

  hb_ot_name_id_t get_মান_name_id () ধ্রুৱ
  {
    ৱিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_মান_name_id ();
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.get_মান_name_id ();
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.get_মান_name_id ();
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.get_মান_name_id ();
    স্ৱভাৱ:ঘূৰাই HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    যদি (unlikely (!c->may_dispatch (এতত্, &u.format.v))) ঘূৰাই c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, u.format.v);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    পক্ষ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    পক্ষ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    পক্ষ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    স্ৱভাৱ:return_trace (c->default_return_মান ());
    }
  }

  তার্কিক keep_axis_মান (ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ধ্রুৱ
  {
    ৱিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.keep_axis_মান (axis_records, user_axes_location);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.keep_axis_মান (axis_records, user_axes_location);
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.keep_axis_মান (axis_records, user_axes_location);
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.keep_axis_মান (axis_records, user_axes_location);
    স্ৱভাৱ:ঘূৰাই মিছা;
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্)))
      return_trace (মিছা);
    hb_barrier ();

    ৱিকল্প (u.format.v)
    {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    পক্ষ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    পক্ষ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    স্ৱভাৱ:return_trace (সঁচা);
    }
  }

  রক্ষিত:
  সঙ্ঘ
  {
  সংরচনা { HBUINT16 v; }	format;
  Axis_মান_Format1	format1;
  Axis_মান_Format2	format2;
  Axis_মান_Format3	format3;
  Axis_মান_Format4	format4;
  } u;
  সার্ৱজনিক:
  DEFINE_SIZE_UNION (2, format.v);
};

সংরচনা Axis_মান_Offset_Array: UnsizedArrayOf<Offset16To<Axis_মান>>
{
  তার্কিক subset (hb_subset_context_t *c,
               unsigned axis_মান_গুনতি,
               unsigned& গুনতি,
               ধ্রুৱ hb_array_t<ধ্রুৱ StatAxisRecord> axis_records) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);

    auto axis_মান_Offsets = as_array (axis_মান_গুনতি);
    গুনতি = 0;
    বাবে (ধ্রুৱ auto& offset : axis_মান_Offsets)
    {
      যদি (!offset) অনুৱর্ত;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      যদি (!o) return_trace (মিছা);
      যদি (!o->serialize_subset (c, offset, এতত্, axis_records))
      {
        c->serializer->revert (o_snap);
        অনুৱর্ত;
      }
      গুনতি++;
    }

    return_trace (গুনতি);
  }
};

সংরচনা STAT
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই version.to_int (); }

  তার্কিক get_মান (hb_tag_t tag, ভিন্ন *value) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক axis_index;
    যদি (!get_design_axes ().lfind (tag, &axis_index)) ঘূৰাই মিছা;

    hb_array_t<ধ্রুৱ Offset16To<Axis_মান>> axis_মানসমূহ = get_axis_মান_offsets ();
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < axis_মানসমূহ.length; i++)
    {
      ধ্রুৱ Axis_মান& axis_মান = এতত্+offset_To_Axis_মান_Offsets+axis_মানসমূহ[i];
      যদি (axis_মান.get_axis_index () == axis_index)
      {
	যদি (value)
	  *value = axis_মান.get_মান (axis_index);
	ঘূৰাই সঁচা;
      }
    }
    ঘূৰাই মিছা;
  }

  unsigned get_design_axis_গুনতি () ধ্রুৱ { ঘূৰাই design_Axis_গুনতি; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) ধ্রুৱ
  {
    যদি (unlikely (axis_record_index >= design_Axis_গুনতি)) ঘূৰাই HB_OT_NAME_ID_INVALID;
    ধ্রুৱ StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    ঘূৰাই axis_record.get_name_id ();
  }

  unsigned get_axis_মান_গুনতি () ধ্রুৱ { ঘূৰাই axis_মান_গুনতি; }

  hb_ot_name_id_t get_axis_মান_name_id (unsigned axis_মান_index) ধ্রুৱ
  {
    যদি (unlikely (axis_মান_index >= axis_মান_গুনতি)) ঘূৰাই HB_OT_NAME_ID_INVALID;
    ধ্রুৱ Axis_মান &axis_মান = (এতত্ + get_axis_মান_offsets ()[axis_মান_index]);
    ঘূৰাই axis_মান.get_মান_name_id ();
  }

  শূন্য collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) ধ্রুৱ
  {
    যদি (!has_data ()) ঘূৰাই;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_মান_offsets ()
    | hb_map (hb_add (&(এতত্ + offset_To_Axis_মান_Offsets)))
    | hb_filter ([&] (ধ্রুৱ Axis_মান& _)
                 { ঘূৰাই _.keep_axis_মান (designAxes, user_axes_location); })
    | hb_map (&Axis_মান::get_মান_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    STAT *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    auto designAxes = get_design_axes ();
    বাবে (unsigned i = 0; i < (unsigned)design_Axis_গুনতি; i++)
      যদি (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (মিছা);

    যদি (design_Axis_গুনতি)
      c->serializer->check_assign (out->designAxesOffset, এতত্->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned গুনতি = 0;
    out->offset_To_Axis_মান_Offsets.serialize_subset (c, offset_To_Axis_মান_Offsets, এতত্,
                                                    axis_মান_গুনতি, গুনতি, designAxes);
    return_trace (c->serializer->check_assign (out->axis_মান_গুনতি, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, এতত্, design_Axis_গুনতি) &&
			  offset_To_Axis_মান_Offsets.sanitize (c, এতত্, axis_মান_গুনতি, &(এতত্+offset_To_Axis_মান_Offsets))));
  }

  রক্ষিত:
  hb_array_t<ধ্রুৱ StatAxisRecord> ধ্রুৱ get_design_axes () ধ্রুৱ
  { ঘূৰাই (এতত্+designAxesOffset).as_array (design_Axis_গুনতি); }

  hb_array_t<ধ্রুৱ Offset16To<Axis_মান>> ধ্রুৱ get_axis_মান_offsets () ধ্রুৱ
  { ঘূৰাই (এতত্+offset_To_Axis_মান_Offsets).as_array (axis_মান_গুনতি); }


  রক্ষিত:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_গুনতি;/* The number of design axis records. In a
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
  HBUINT16	axis_মান_গুনতি;	/* The number of axis value tables. */
  NNOffset32To<Axis_মান_Offset_Array>
		offset_To_Axis_মান_Offsets;
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
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (20);
};


} /* namespace OT */


#endif /* HB_OT_STAT_TABLE_HH */
