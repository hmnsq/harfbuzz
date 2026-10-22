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

#شامل "hb-open-type.hh"
#شامل "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#define HB_OT_TAG_STAT HB_TAG('S','T','A','T')


فضاینام OT {

شمارش
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
  ELIDABLE_AXIS_مقدار_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

ایستا منطقی axis_مقدار_is_outside_axis_range (hb_tag_t axis_tag, اعشاری axis_مقدار,
                                              ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  اگر (!user_axes_location->has (axis_tag))
    بازگشت نادرست;

  Triple axis_range = user_axes_location->get (axis_tag);
  بازگشت (axis_مقدار < axis_range.minimum || axis_مقدار > axis_range.maximum);
}

ساختار StatAxisRecord
{
  صحیح cmp (hb_tag_t key) ثابت { بازگشت tag.cmp (key); }

  hb_ot_name_id_t get_name_id () ثابت { بازگشت nameID; }

  hb_tag_t get_axis_tag () ثابت { بازگشت tag; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این)));
  }

  محافظتشده:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار Axis_مقدار_Format1
{
  unsigned صحیح get_axis_index () ثابت { بازگشت axisIndex; }
  اعشاری get_مقدار ()             ثابت { بازگشت value.to_float (); }

  hb_ot_name_id_t get_مقدار_name_id () ثابت { بازگشت مقدار_Name_ID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    بازگشت axis_records[axis_idx].get_axis_tag ();
  }

  منطقی keep_axis_مقدار (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    اعشاری axis_مقدار = get_مقدار ();

    بازگشت !axis_مقدار_is_outside_axis_range (axis_tag, axis_مقدار, user_axes_location);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    اگر (keep_axis_مقدار (axis_records, user_axes_location))
      return_trace (c->serializer->embed (این));

    return_trace (نادرست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	مقدار_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عمومی:
  DEFINE_SIZE_STATIC (12);
};

ساختار Axis_مقدار_Format2
{
  unsigned صحیح get_axis_index () ثابت { بازگشت axisIndex; }
  اعشاری get_مقدار ()             ثابت { بازگشت nominal_مقدار.to_float (); }

  hb_ot_name_id_t get_مقدار_name_id () ثابت { بازگشت مقدار_Name_ID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    بازگشت axis_records[axis_idx].get_axis_tag ();
  }

  منطقی keep_axis_مقدار (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    اعشاری axis_مقدار = get_مقدار ();

    بازگشت !axis_مقدار_is_outside_axis_range (axis_tag, axis_مقدار, user_axes_location);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    اگر (keep_axis_مقدار (axis_records, user_axes_location))
      return_trace (c->serializer->embed (این));

    return_trace (نادرست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	مقدار_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_مقدار;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_مقدار;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_مقدار;	/* The maximum value for a range associated
				 * with the specified name ID. */
  عمومی:
  DEFINE_SIZE_STATIC (20);
};

ساختار Axis_مقدار_Format3
{
  unsigned صحیح get_axis_index () ثابت { بازگشت axisIndex; }
  اعشاری get_مقدار ()             ثابت { بازگشت value.to_float (); }

  hb_ot_name_id_t get_مقدار_name_id () ثابت { بازگشت مقدار_Name_ID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    بازگشت axis_records[axis_idx].get_axis_tag ();
  }

  منطقی keep_axis_مقدار (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    اعشاری axis_مقدار = get_مقدار ();

    بازگشت !axis_مقدار_is_outside_axis_range (axis_tag, axis_مقدار, user_axes_location);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    اگر (keep_axis_مقدار (axis_records, user_axes_location))
      return_trace (c->serializer->embed (این));

    return_trace (نادرست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	مقدار_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_مقدار;	/* The numeric value for a style-linked mapping
				 * from this value. */
  عمومی:
  DEFINE_SIZE_STATIC (16);
};

ساختار Axis_مقدار_Record
{
  unsigned صحیح get_axis_index () ثابت { بازگشت axisIndex; }
  اعشاری get_مقدار ()             ثابت { بازگشت value.to_float (); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  محافظتشده:
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار Axis_مقدار_Format4
{
  ثابت Axis_مقدار_Record &get_axis_record (unsigned صحیح axis_index) ثابت
  { بازگشت axis_مقادیر_2.as_array (axis_شمار_2)[axis_index]; }

  منطقی keep_axis_مقدار (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_array_t<ثابت Axis_مقدار_Record> axis_مقدار_records = axis_مقادیر_2.as_array (axis_شمار_2);

    برای (ثابت auto& rec : axis_مقدار_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      اعشاری axis_مقدار = rec.get_مقدار ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      اگر (axis_مقدار_is_outside_axis_range (axis_tag, axis_مقدار, user_axes_location))
        بازگشت نادرست;
    }

    بازگشت درست;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    اگر (!keep_axis_مقدار (axis_records, user_axes_location))
      return_trace (نادرست);

    unsigned total_size = min_size + axis_شمار_2 * Axis_مقدار_Record::static_size;
    auto *out = c->serializer->allocate_size<Axis_مقدار_Format4> (total_size);
    اگر (unlikely (!out)) return_trace (نادرست);
    hb_memcpy (out, این, total_size);
    return_trace (درست);
  }

  hb_ot_name_id_t get_مقدار_name_id () ثابت { بازگشت مقدار_Name_ID; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  hb_barrier () &&
                          axis_مقادیر_2.sanitize (c, axis_شمار_2)));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_شمار_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	مقدار_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_مقدار_Record>
		axis_مقادیر_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  عمومی:
  DEFINE_SIZE_ARRAY (8, axis_مقادیر_2);
};

ساختار Axis_مقدار
{
  اعشاری get_مقدار (unsigned صحیح axis_index) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); بازگشت u.format1.get_مقدار ();
    حالت 2: hb_barrier (); بازگشت u.format2.get_مقدار ();
    حالت 3: hb_barrier (); بازگشت u.format3.get_مقدار ();
    حالت 4: hb_barrier (); بازگشت u.format4.get_axis_record (axis_index).get_مقدار ();
    پیشفرض:بازگشت 0.f;
    }
  }

  unsigned صحیح get_axis_index () ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); بازگشت u.format1.get_axis_index ();
    حالت 2: hb_barrier (); بازگشت u.format2.get_axis_index ();
    حالت 3: hb_barrier (); بازگشت u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    پیشفرض:بازگشت -1;
    }
  }

  hb_ot_name_id_t get_مقدار_name_id () ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); بازگشت u.format1.get_مقدار_name_id ();
    حالت 2: hb_barrier (); بازگشت u.format2.get_مقدار_name_id ();
    حالت 3: hb_barrier (); بازگشت u.format3.get_مقدار_name_id ();
    حالت 4: hb_barrier (); بازگشت u.format4.get_مقدار_name_id ();
    پیشفرض:بازگشت HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }

  منطقی keep_axis_مقدار (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); بازگشت u.format1.keep_axis_مقدار (axis_records, user_axes_location);
    حالت 2: hb_barrier (); بازگشت u.format2.keep_axis_مقدار (axis_records, user_axes_location);
    حالت 3: hb_barrier (); بازگشت u.format3.keep_axis_مقدار (axis_records, user_axes_location);
    حالت 4: hb_barrier (); بازگشت u.format4.keep_axis_مقدار (axis_records, user_axes_location);
    پیشفرض:بازگشت نادرست;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!c->check_struct (این)))
      return_trace (نادرست);
    hb_barrier ();

    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    پیشفرض:return_trace (درست);
    }
  }

  محافظتشده:
  اجتماع
  {
  ساختار { HBUINT16 v; }	format;
  Axis_مقدار_Format1	format1;
  Axis_مقدار_Format2	format2;
  Axis_مقدار_Format3	format3;
  Axis_مقدار_Format4	format4;
  } u;
  عمومی:
  DEFINE_SIZE_UNION (2, format.v);
};

ساختار Axis_مقدار_Offset_Array: UnsizedArrayOf<Offset16To<Axis_مقدار>>
{
  منطقی subset (hb_subset_context_t *c,
               unsigned axis_مقدار_شمار,
               unsigned& شمار,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (این);

    auto axis_مقدار_Offsets = as_array (axis_مقدار_شمار);
    شمار = 0;
    برای (ثابت auto& offset : axis_مقدار_Offsets)
    {
      اگر (!offset) ادامه;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      اگر (!o) return_trace (نادرست);
      اگر (!o->serialize_subset (c, offset, این, axis_records))
      {
        c->serializer->revert (o_snap);
        ادامه;
      }
      شمار++;
    }

    return_trace (شمار);
  }
};

ساختار STAT
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  منطقی has_data () ثابت { بازگشت version.to_int (); }

  منطقی get_مقدار (hb_tag_t tag, اعشاری *value) ثابت
  {
    unsigned صحیح axis_index;
    اگر (!get_design_axes ().lfind (tag, &axis_index)) بازگشت نادرست;

    hb_array_t<ثابت Offset16To<Axis_مقدار>> axis_مقادیر = get_axis_مقدار_offsets ();
    برای (unsigned صحیح i = 0; i < axis_مقادیر.length; i++)
    {
      ثابت Axis_مقدار& axis_مقدار = این+offset_To_Axis_مقدار_Offsets+axis_مقادیر[i];
      اگر (axis_مقدار.get_axis_index () == axis_index)
      {
	اگر (value)
	  *value = axis_مقدار.get_مقدار (axis_index);
	بازگشت درست;
      }
    }
    بازگشت نادرست;
  }

  unsigned get_design_axis_شمار () ثابت { بازگشت design_Axis_شمار; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) ثابت
  {
    اگر (unlikely (axis_record_index >= design_Axis_شمار)) بازگشت HB_OT_NAME_ID_INVALID;
    ثابت StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    بازگشت axis_record.get_name_id ();
  }

  unsigned get_axis_مقدار_شمار () ثابت { بازگشت axis_مقدار_شمار; }

  hb_ot_name_id_t get_axis_مقدار_name_id (unsigned axis_مقدار_index) ثابت
  {
    اگر (unlikely (axis_مقدار_index >= axis_مقدار_شمار)) بازگشت HB_OT_NAME_ID_INVALID;
    ثابت Axis_مقدار &axis_مقدار = (این + get_axis_مقدار_offsets ()[axis_مقدار_index]);
    بازگشت axis_مقدار.get_مقدار_name_id ();
  }

  تهی collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    اگر (!has_data ()) بازگشت;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_مقدار_offsets ()
    | hb_map (hb_add (&(این + offset_To_Axis_مقدار_Offsets)))
    | hb_filter ([&] (ثابت Axis_مقدار& _)
                 { بازگشت _.keep_axis_مقدار (designAxes, user_axes_location); })
    | hb_map (&Axis_مقدار::get_مقدار_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    STAT *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    auto designAxes = get_design_axes ();
    برای (unsigned i = 0; i < (unsigned)design_Axis_شمار; i++)
      اگر (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (نادرست);

    اگر (design_Axis_شمار)
      c->serializer->check_assign (out->designAxesOffset, این->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned شمار = 0;
    out->offset_To_Axis_مقدار_Offsets.serialize_subset (c, offset_To_Axis_مقدار_Offsets, این,
                                                    axis_مقدار_شمار, شمار, designAxes);
    return_trace (c->serializer->check_assign (out->axis_مقدار_شمار, شمار, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, این, design_Axis_شمار) &&
			  offset_To_Axis_مقدار_Offsets.sanitize (c, این, axis_مقدار_شمار, &(این+offset_To_Axis_مقدار_Offsets))));
  }

  محافظتشده:
  hb_array_t<ثابت StatAxisRecord> ثابت get_design_axes () ثابت
  { بازگشت (این+designAxesOffset).as_array (design_Axis_شمار); }

  hb_array_t<ثابت Offset16To<Axis_مقدار>> ثابت get_axis_مقدار_offsets () ثابت
  { بازگشت (این+offset_To_Axis_مقدار_Offsets).as_array (axis_مقدار_شمار); }


  محافظتشده:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_شمار;/* The number of design axis records. In a
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
  HBUINT16	axis_مقدار_شمار;	/* The number of axis value tables. */
  NNOffset32To<Axis_مقدار_Offset_Array>
		offset_To_Axis_مقدار_Offsets;
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
  عمومی:
  DEFINE_SIZE_STATIC (20);
};


} /* namespace OT */


#endif /* HB_OT_STAT_TABLE_HH */
