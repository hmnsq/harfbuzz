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

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#define HB_OT_TAG_STAT HB_TAG('S','T','A','T')


نطاق OT {

تعداد
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
  ELIDABLE_AXIS_VALUE_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

ساكن منطقي axis_value_is_outside_axis_range (hb_tag_t axis_tag, عائم axis_value,
                                              ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  אם (!user_axes_location->has (axis_tag))
    החזר שקר;

  Triple axis_range = user_axes_location->get (axis_tag);
  החזר (axis_value < axis_range.minimum || axis_value > axis_range.maximum);
}

بنية StatAxisRecord
{
  صحيح cmp (hb_tag_t key) ثابت { החזר tag.cmp (key); }

  hb_ot_name_id_t get_name_id () ثابت { החזר nameID; }

  hb_tag_t get_axis_tag () ثابت { החזר tag; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا)));
  }

  محمي:
  Tag		tag;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية AxisValueFormat1
{
  unsigned صحيح get_axis_index () ثابت { החזר axisIndex; }
  عائم get_value ()             ثابت { החזר value.to_float (); }

  hb_ot_name_id_t get_value_name_id () ثابت { החזר valueNameID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    החזר axis_records[axis_idx].get_axis_tag ();
  }

  منطقي keep_axis_value (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    عائم axis_value = get_value ();

    החזר !axis_value_is_outside_axis_range (axis_tag, axis_value, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    אם (keep_axis_value (axis_records, user_axes_location))
      return_trace (c->serializer->embed (هذا));

    return_trace (שקר);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier — set to 1. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	valueNameID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عام:
  DEFINE_SIZE_STATIC (12);
};

بنية AxisValueFormat2
{
  unsigned صحيح get_axis_index () ثابت { החזר axisIndex; }
  عائم get_value ()             ثابت { החזר nominalValue.to_float (); }

  hb_ot_name_id_t get_value_name_id () ثابت { החזר valueNameID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    החזר axis_records[axis_idx].get_axis_tag ();
  }

  منطقي keep_axis_value (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    عائم axis_value = get_value ();

    החזר !axis_value_is_outside_axis_range (axis_tag, axis_value, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    אם (keep_axis_value (axis_records, user_axes_location))
      return_trace (c->serializer->embed (هذا));

    return_trace (שקר);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier — set to 2. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	valueNameID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominalValue;	/* A numeric value for this attribute value. */
  F16DOT16	rangeMinValue;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	rangeMaxValue;	/* The maximum value for a range associated
				 * with the specified name ID. */
  عام:
  DEFINE_SIZE_STATIC (20);
};

بنية AxisValueFormat3
{
  unsigned صحيح get_axis_index () ثابت { החזר axisIndex; }
  عائم get_value ()             ثابت { החזר value.to_float (); }

  hb_ot_name_id_t get_value_name_id () ثابت { החזר valueNameID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    החזר axis_records[axis_idx].get_axis_tag ();
  }

  منطقي keep_axis_value (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    عائم axis_value = get_value ();

    החזר !axis_value_is_outside_axis_range (axis_tag, axis_value, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    אם (keep_axis_value (axis_records, user_axes_location))
      return_trace (c->serializer->embed (هذا));

    return_trace (שקר);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier — set to 3. */
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	valueNameID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linkedValue;	/* The numeric value for a style-linked mapping
				 * from this value. */
  عام:
  DEFINE_SIZE_STATIC (16);
};

بنية AxisValueRecord
{
  unsigned صحيح get_axis_index () ثابت { החזר axisIndex; }
  عائم get_value ()             ثابت { החזר value.to_float (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	axisIndex;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية AxisValueFormat4
{
  ثابت AxisValueRecord &get_axis_record (unsigned صحيح axis_index) ثابت
  { החזר axisValues.as_array (axisCount)[axis_index]; }

  منطقي keep_axis_value (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_array_t<ثابت AxisValueRecord> axis_value_records = axisValues.as_array (axisCount);

    עבור (ثابت auto& rec : axis_value_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      عائم axis_value = rec.get_value ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      אם (axis_value_is_outside_axis_range (axis_tag, axis_value, user_axes_location))
        החזר שקר;
    }

    החזר אמת;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    אם (!keep_axis_value (axis_records, user_axes_location))
      return_trace (שקר);

    unsigned total_size = min_size + axisCount * AxisValueRecord::static_size;
    auto *out = c->serializer->allocate_size<AxisValueFormat4> (total_size);
    אם (unlikely (!out)) return_trace (שקר);
    hb_memcpy (out, هذا, total_size);
    return_trace (אמת);
  }

  hb_ot_name_id_t get_value_name_id () ثابت { החזר valueNameID; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
                          axisValues.sanitize (c, axisCount)));
  }

  محمي:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axisCount;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	valueNameID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<AxisValueRecord>
		axisValues;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  عام:
  DEFINE_SIZE_ARRAY (8, axisValues);
};

بنية AxisValue
{
  عائم get_value (unsigned صحيح axis_index) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); החזר u.format1.get_value ();
    حالة 2: hb_barrier (); החזר u.format2.get_value ();
    حالة 3: hb_barrier (); החזר u.format3.get_value ();
    حالة 4: hb_barrier (); החזר u.format4.get_axis_record (axis_index).get_value ();
    افتراضي:החזר 0.f;
    }
  }

  unsigned صحيح get_axis_index () ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); החזר u.format1.get_axis_index ();
    حالة 2: hb_barrier (); החזר u.format2.get_axis_index ();
    حالة 3: hb_barrier (); החזר u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    افتراضي:החזר -1;
    }
  }

  hb_ot_name_id_t get_value_name_id () ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); החזר u.format1.get_value_name_id ();
    حالة 2: hb_barrier (); החזר u.format2.get_value_name_id ();
    حالة 3: hb_barrier (); החזר u.format3.get_value_name_id ();
    حالة 4: hb_barrier (); החזר u.format4.get_value_name_id ();
    افتراضي:החזר HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    אם (unlikely (!c->may_dispatch (هذا, &u.format.v))) החזר c->no_dispatch_return_value ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_value ());
    }
  }

  منطقي keep_axis_value (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); החזר u.format1.keep_axis_value (axis_records, user_axes_location);
    حالة 2: hb_barrier (); החזר u.format2.keep_axis_value (axis_records, user_axes_location);
    حالة 3: hb_barrier (); החזר u.format3.keep_axis_value (axis_records, user_axes_location);
    حالة 4: hb_barrier (); החזר u.format4.keep_axis_value (axis_records, user_axes_location);
    افتراضي:החזר שקר;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    אם (unlikely (!c->check_struct (هذا)))
      return_trace (שקר);
    hb_barrier ();

    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    افتراضي:return_trace (אמת);
    }
  }

  محمي:
  اتحاد
  {
  بنية { HBUINT16 v; }	format;
  AxisValueFormat1	format1;
  AxisValueFormat2	format2;
  AxisValueFormat3	format3;
  AxisValueFormat4	format4;
  } u;
  عام:
  DEFINE_SIZE_UNION (2, format.v);
};

بنية AxisValueOffsetArray: UnsizedArrayOf<Offset16To<AxisValue>>
{
  منطقي subset (hb_subset_context_t *c,
               unsigned axisValueCount,
               unsigned& count,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);

    auto axisValueOffsets = as_array (axisValueCount);
    count = 0;
    עבור (ثابت auto& offset : axisValueOffsets)
    {
      אם (!offset) استمر;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      אם (!o) return_trace (שקר);
      אם (!o->serialize_subset (c, offset, هذا, axis_records))
      {
        c->serializer->revert (o_snap);
        استمر;
      }
      count++;
    }

    return_trace (count);
  }
};

بنية STAT
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  منطقي has_data () ثابت { החזר version.to_int (); }

  منطقي get_value (hb_tag_t tag, عائم *value) ثابت
  {
    unsigned صحيح axis_index;
    אם (!get_design_axes ().lfind (tag, &axis_index)) החזר שקר;

    hb_array_t<ثابت Offset16To<AxisValue>> axis_values = get_axis_value_offsets ();
    עבור (unsigned صحيح i = 0; i < axis_values.length; i++)
    {
      ثابت AxisValue& axis_value = هذا+offsetToAxisValueOffsets+axis_values[i];
      אם (axis_value.get_axis_index () == axis_index)
      {
	אם (value)
	  *value = axis_value.get_value (axis_index);
	החזר אמת;
      }
    }
    החזר שקר;
  }

  unsigned get_design_axis_count () ثابت { החזר designAxisCount; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) ثابت
  {
    אם (unlikely (axis_record_index >= designAxisCount)) החזר HB_OT_NAME_ID_INVALID;
    ثابت StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    החזר axis_record.get_name_id ();
  }

  unsigned get_axis_value_count () ثابت { החזר axisValueCount; }

  hb_ot_name_id_t get_axis_value_name_id (unsigned axis_value_index) ثابت
  {
    אם (unlikely (axis_value_index >= axisValueCount)) החזר HB_OT_NAME_ID_INVALID;
    ثابت AxisValue &axis_value = (هذا + get_axis_value_offsets ()[axis_value_index]);
    החזר axis_value.get_value_name_id ();
  }

  فراغ collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    אם (!has_data ()) החזר;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_value_offsets ()
    | hb_map (hb_add (&(هذا + offsetToAxisValueOffsets)))
    | hb_filter ([&] (ثابت AxisValue& _)
                 { החזר _.keep_axis_value (designAxes, user_axes_location); })
    | hb_map (&AxisValue::get_value_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    STAT *out = c->serializer->embed (هذا);
    אם (unlikely (!out)) return_trace (שקר);

    auto designAxes = get_design_axes ();
    עבור (unsigned i = 0; i < (unsigned)designAxisCount; i++)
      אם (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (שקר);

    אם (designAxisCount)
      c->serializer->check_assign (out->designAxesOffset, هذا->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned count = 0;
    out->offsetToAxisValueOffsets.serialize_subset (c, offsetToAxisValueOffsets, هذا,
                                                    axisValueCount, count, designAxes);
    return_trace (c->serializer->check_assign (out->axisValueCount, count, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, هذا, designAxisCount) &&
			  offsetToAxisValueOffsets.sanitize (c, هذا, axisValueCount, &(هذا+offsetToAxisValueOffsets))));
  }

  محمي:
  hb_array_t<ثابت StatAxisRecord> ثابت get_design_axes () ثابت
  { החזר (هذا+designAxesOffset).as_array (designAxisCount); }

  hb_array_t<ثابت Offset16To<AxisValue>> ثابت get_axis_value_offsets () ثابت
  { החזר (هذا+offsetToAxisValueOffsets).as_array (axisValueCount); }


  محمي:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	designAxisCount;/* The number of design axis records. In a
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
  HBUINT16	axisValueCount;	/* The number of axis value tables. */
  NNOffset32To<AxisValueOffsetArray>
		offsetToAxisValueOffsets;
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
  عام:
  DEFINE_SIZE_STATIC (20);
};


} /* namespace OT */


#endif /* HB_OT_STAT_TABLE_HH */
