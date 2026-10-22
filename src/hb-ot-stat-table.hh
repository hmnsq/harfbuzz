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

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-layout-common.hh"

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

স্থির যৌক্তিক axis_মান_is_outside_axis_range (hb_tag_t axis_tag, দশমিক axis_মান,
                                              ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  যদি (!user_axes_location->has (axis_tag))
    ফেরত মিথ্যা;

  Triple axis_range = user_axes_location->get (axis_tag);
  ফেরত (axis_মান < axis_range.minimum || axis_মান > axis_range.maximum);
}

কাঠামো StatAxisRecord
{
  পূর্ণসংখ্যা cmp (hb_tag_t key) ধ্রুবক { ফেরত tag.cmp (key); }

  hb_ot_name_id_t get_name_id () ধ্রুবক { ফেরত nameID; }

  hb_tag_t get_axis_tag () ধ্রুবক { ফেরত tag; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই)));
  }

  সুরক্ষিত:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো Axis_মান_Format1
{
  unsigned পূর্ণসংখ্যা get_axis_সূচক () ধ্রুবক { ফেরত axis_সূচক_2; }
  দশমিক get_মান ()             ধ্রুবক { ফেরত value.to_float (); }

  hb_ot_name_id_t get_মান_name_id () ধ্রুবক { ফেরত মান_Name_ID; }

  hb_tag_t get_axis_tag (ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records) ধ্রুবক
  {
    unsigned axis_idx = get_axis_সূচক ();
    ফেরত axis_records[axis_idx].get_axis_tag ();
  }

  যৌক্তিক keep_axis_মান (ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records,
                        ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ধ্রুবক
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    দশমিক axis_মান = get_মান ();

    ফেরত !axis_মান_is_outside_axis_range (axis_tag, axis_মান, user_axes_location);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    যদি (keep_axis_মান (axis_records, user_axes_location))
      return_trace (c->serializer->embed (এই));

    return_trace (মিথ্যা);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axis_সূচক_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	মান_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};

কাঠামো Axis_মান_Format2
{
  unsigned পূর্ণসংখ্যা get_axis_সূচক () ধ্রুবক { ফেরত axis_সূচক_2; }
  দশমিক get_মান ()             ধ্রুবক { ফেরত nominal_মান.to_float (); }

  hb_ot_name_id_t get_মান_name_id () ধ্রুবক { ফেরত মান_Name_ID; }

  hb_tag_t get_axis_tag (ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records) ধ্রুবক
  {
    unsigned axis_idx = get_axis_সূচক ();
    ফেরত axis_records[axis_idx].get_axis_tag ();
  }

  যৌক্তিক keep_axis_মান (ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records,
                        ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ধ্রুবক
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    দশমিক axis_মান = get_মান ();

    ফেরত !axis_মান_is_outside_axis_range (axis_tag, axis_মান, user_axes_location);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    যদি (keep_axis_মান (axis_records, user_axes_location))
      return_trace (c->serializer->embed (এই));

    return_trace (মিথ্যা);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axis_সূচক_2;	/* Zero-base index into the axis record array
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (20);
};

কাঠামো Axis_মান_Format3
{
  unsigned পূর্ণসংখ্যা get_axis_সূচক () ধ্রুবক { ফেরত axis_সূচক_2; }
  দশমিক get_মান ()             ধ্রুবক { ফেরত value.to_float (); }

  hb_ot_name_id_t get_মান_name_id () ধ্রুবক { ফেরত মান_Name_ID; }

  hb_tag_t get_axis_tag (ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records) ধ্রুবক
  {
    unsigned axis_idx = get_axis_সূচক ();
    ফেরত axis_records[axis_idx].get_axis_tag ();
  }

  যৌক্তিক keep_axis_মান (ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records,
                        ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ধ্রুবক
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    দশমিক axis_মান = get_মান ();

    ফেরত !axis_মান_is_outside_axis_range (axis_tag, axis_মান, user_axes_location);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    যদি (keep_axis_মান (axis_records, user_axes_location))
      return_trace (c->serializer->embed (এই));

    return_trace (মিথ্যা);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axis_সূচক_2;	/* Zero-base index into the axis record array
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (16);
};

কাঠামো Axis_মান_Record
{
  unsigned পূর্ণসংখ্যা get_axis_সূচক () ধ্রুবক { ফেরত axis_সূচক_2; }
  দশমিক get_মান ()             ধ্রুবক { ফেরত value.to_float (); }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	axis_সূচক_2;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো Axis_মান_Format4
{
  ধ্রুবক Axis_মান_Record &get_axis_record (unsigned পূর্ণসংখ্যা axis_সূচক) ধ্রুবক
  { ফেরত axis_মানসমূহ_2.as_array (axis_গুনতি_2)[axis_সূচক]; }

  যৌক্তিক keep_axis_মান (ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records,
                        ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ধ্রুবক
  {
    hb_array_t<ধ্রুবক Axis_মান_Record> axis_মান_records = axis_মানসমূহ_2.as_array (axis_গুনতি_2);

    জন্য (ধ্রুবক auto& rec : axis_মান_records)
    {
      unsigned axis_idx = rec.get_axis_সূচক ();
      দশমিক axis_মান = rec.get_মান ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      যদি (axis_মান_is_outside_axis_range (axis_tag, axis_মান, user_axes_location))
        ফেরত মিথ্যা;
    }

    ফেরত সত্য;
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    যদি (!keep_axis_মান (axis_records, user_axes_location))
      return_trace (মিথ্যা);

    unsigned যোগ_আকার = min_আকার + axis_গুনতি_2 * Axis_মান_Record::static_আকার;
    auto *out = c->serializer->allocate_আকার<Axis_মান_Format4> (যোগ_আকার);
    যদি (unlikely (!out)) return_trace (মিথ্যা);
    hb_memcpy (out, এই, যোগ_আকার);
    return_trace (সত্য);
  }

  hb_ot_name_id_t get_মান_name_id () ধ্রুবক { ফেরত মান_Name_ID; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
                          axis_মানসমূহ_2.sanitize (c, axis_গুনতি_2)));
  }

  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_ARRAY (8, axis_মানসমূহ_2);
};

কাঠামো Axis_মান
{
  দশমিক get_মান (unsigned পূর্ণসংখ্যা axis_সূচক) ধ্রুবক
  {
    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_মান ();
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_মান ();
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_মান ();
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.get_axis_record (axis_সূচক).get_মান ();
    পূর্বনির্ধারিত:ফেরত 0.f;
    }
  }

  unsigned পূর্ণসংখ্যা get_axis_সূচক () ধ্রুবক
  {
    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_axis_সূচক ();
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_axis_সূচক ();
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_axis_সূচক ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    পূর্বনির্ধারিত:ফেরত -1;
    }
  }

  hb_ot_name_id_t get_মান_name_id () ধ্রুবক
  {
    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_মান_name_id ();
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_মান_name_id ();
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_মান_name_id ();
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.get_মান_name_id ();
    পূর্বনির্ধারিত:ফেরত HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    যদি (unlikely (!c->may_dispatch (এই, &u.format.v))) ফেরত c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এই, u.format.v);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ক্ষেত্র 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ক্ষেত্র 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ক্ষেত্র 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    পূর্বনির্ধারিত:return_trace (c->default_return_মান ());
    }
  }

  যৌক্তিক keep_axis_মান (ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ধ্রুবক
  {
    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.keep_axis_মান (axis_records, user_axes_location);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.keep_axis_মান (axis_records, user_axes_location);
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.keep_axis_মান (axis_records, user_axes_location);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.keep_axis_মান (axis_records, user_axes_location);
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই)))
      return_trace (মিথ্যা);
    hb_barrier ();

    নির্বাচন (u.format.v)
    {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    ক্ষেত্র 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }

  সুরক্ষিত:
  সংঘ
  {
  কাঠামো { HBUINT16 v; }	format;
  Axis_মান_Format1	format1;
  Axis_মান_Format2	format2;
  Axis_মান_Format3	format3;
  Axis_মান_Format4	format4;
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (2, format.v);
};

কাঠামো Axis_মান_Offset_Array: UnsizedArrayOf<Offset16To<Axis_মান>>
{
  যৌক্তিক subset (hb_subset_context_t *c,
               unsigned axis_মান_গুনতি,
               unsigned& গুনতি,
               ধ্রুবক hb_array_t<ধ্রুবক StatAxisRecord> axis_records) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    auto axis_মান_Offsets = as_array (axis_মান_গুনতি);
    গুনতি = 0;
    জন্য (ধ্রুবক auto& offset : axis_মান_Offsets)
    {
      যদি (!offset) চালু;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      যদি (!o) return_trace (মিথ্যা);
      যদি (!o->serialize_subset (c, offset, এই, axis_records))
      {
        c->serializer->revert (o_snap);
        চালু;
      }
      গুনতি++;
    }

    return_trace (গুনতি);
  }
};

কাঠামো STAT
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.to_int (); }

  যৌক্তিক get_মান (hb_tag_t tag, দশমিক *value) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা axis_সূচক;
    যদি (!get_design_axes ().lfind (tag, &axis_সূচক)) ফেরত মিথ্যা;

    hb_array_t<ধ্রুবক Offset16To<Axis_মান>> axis_মানসমূহ = get_axis_মান_offsets ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < axis_মানসমূহ.দৈর্ঘ্য; i++)
    {
      ধ্রুবক Axis_মান& axis_মান = এই+offset_To_Axis_মান_Offsets+axis_মানসমূহ[i];
      যদি (axis_মান.get_axis_সূচক () == axis_সূচক)
      {
	যদি (value)
	  *value = axis_মান.get_মান (axis_সূচক);
	ফেরত সত্য;
      }
    }
    ফেরত মিথ্যা;
  }

  unsigned get_design_axis_গুনতি () ধ্রুবক { ফেরত design_Axis_গুনতি; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_সূচক) ধ্রুবক
  {
    যদি (unlikely (axis_record_সূচক >= design_Axis_গুনতি)) ফেরত HB_OT_NAME_ID_INVALID;
    ধ্রুবক StatAxisRecord &axis_record = get_design_axes ()[axis_record_সূচক];
    ফেরত axis_record.get_name_id ();
  }

  unsigned get_axis_মান_গুনতি () ধ্রুবক { ফেরত axis_মান_গুনতি; }

  hb_ot_name_id_t get_axis_মান_name_id (unsigned axis_মান_সূচক) ধ্রুবক
  {
    যদি (unlikely (axis_মান_সূচক >= axis_মান_গুনতি)) ফেরত HB_OT_NAME_ID_INVALID;
    ধ্রুবক Axis_মান &axis_মান = (এই + get_axis_মান_offsets ()[axis_মান_সূচক]);
    ফেরত axis_মান.get_মান_name_id ();
  }

  শূন্য collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) ধ্রুবক
  {
    যদি (!has_তথ্য ()) ফেরত;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_মান_offsets ()
    | hb_map (hb_add (&(এই + offset_To_Axis_মান_Offsets)))
    | hb_filter ([&] (ধ্রুবক Axis_মান& _)
                 { ফেরত _.keep_axis_মান (designAxes, user_axes_location); })
    | hb_map (&Axis_মান::get_মান_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    STAT *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    auto designAxes = get_design_axes ();
    জন্য (unsigned i = 0; i < (unsigned)design_Axis_গুনতি; i++)
      যদি (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (মিথ্যা);

    যদি (design_Axis_গুনতি)
      c->serializer->check_assign (out->designAxesOffset, এই->get_আকার (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned গুনতি = 0;
    out->offset_To_Axis_মান_Offsets.serialize_subset (c, offset_To_Axis_মান_Offsets, এই,
                                                    axis_মান_গুনতি, গুনতি, designAxes);
    return_trace (c->serializer->check_assign (out->axis_মান_গুনতি, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, এই, design_Axis_গুনতি) &&
			  offset_To_Axis_মান_Offsets.sanitize (c, এই, axis_মান_গুনতি, &(এই+offset_To_Axis_মান_Offsets))));
  }

  সুরক্ষিত:
  hb_array_t<ধ্রুবক StatAxisRecord> ধ্রুবক get_design_axes () ধ্রুবক
  { ফেরত (এই+designAxesOffset).as_array (design_Axis_গুনতি); }

  hb_array_t<ধ্রুবক Offset16To<Axis_মান>> ধ্রুবক get_axis_মান_offsets () ধ্রুবক
  { ফেরত (এই+offset_To_Axis_মান_Offsets).as_array (axis_মান_গুনতি); }


  সুরক্ষিত:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	design_Axis_আকার;	/* The size in bytes of each axis record. */
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (20);
};


} /* namespace OT */


#endif /* HB_OT_STAT_TABLE_HH */
