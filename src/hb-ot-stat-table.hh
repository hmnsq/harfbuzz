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
  ELIDABLE_AXIS_بەها_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

ساكن منطقي axis_بەها_is_outside_axis_range (hb_tag_t axis_tag, عائم axis_بەها,
                                              ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  إذا (!user_axes_location->has (axis_tag))
    أرجع خطأ;

  Triple axis_range = user_axes_location->get (axis_tag);
  أرجع (axis_بەها < axis_range.minimum || axis_بەها > axis_range.maximum);
}

بنية StatAxisRecord
{
  صحيح cmp (hb_tag_t key) ثابت { أرجع tag.cmp (key); }

  hb_ot_name_id_t get_name_id () ثابت { أرجع nameID; }

  hb_tag_t get_axis_tag () ثابت { أرجع tag; }

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

بنية Axis_بەها_Format1
{
  unsigned صحيح get_axis_index () ثابت { أرجع axisIndex; }
  عائم get_بەها ()             ثابت { أرجع value.to_float (); }

  hb_ot_name_id_t get_بەها_name_id () ثابت { أرجع بەها_Name_ID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    أرجع axis_records[axis_idx].get_axis_tag ();
  }

  منطقي keep_axis_بەها (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    عائم axis_بەها = get_بەها ();

    أرجع !axis_بەها_is_outside_axis_range (axis_tag, axis_بەها, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_بەها (axis_records, user_axes_location))
      return_trace (c->serializer->embed (هذا));

    return_trace (خطأ);
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
  NameID	بەها_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عام:
  DEFINE_SIZE_STATIC (12);
};

بنية Axis_بەها_Format2
{
  unsigned صحيح get_axis_index () ثابت { أرجع axisIndex; }
  عائم get_بەها ()             ثابت { أرجع nominal_بەها.to_float (); }

  hb_ot_name_id_t get_بەها_name_id () ثابت { أرجع بەها_Name_ID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    أرجع axis_records[axis_idx].get_axis_tag ();
  }

  منطقي keep_axis_بەها (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    عائم axis_بەها = get_بەها ();

    أرجع !axis_بەها_is_outside_axis_range (axis_tag, axis_بەها, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_بەها (axis_records, user_axes_location))
      return_trace (c->serializer->embed (هذا));

    return_trace (خطأ);
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
  NameID	بەها_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_بەها;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_بەها;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_بەها;	/* The maximum value for a range associated
				 * with the specified name ID. */
  عام:
  DEFINE_SIZE_STATIC (20);
};

بنية Axis_بەها_Format3
{
  unsigned صحيح get_axis_index () ثابت { أرجع axisIndex; }
  عائم get_بەها ()             ثابت { أرجع value.to_float (); }

  hb_ot_name_id_t get_بەها_name_id () ثابت { أرجع بەها_Name_ID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    أرجع axis_records[axis_idx].get_axis_tag ();
  }

  منطقي keep_axis_بەها (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    عائم axis_بەها = get_بەها ();

    أرجع !axis_بەها_is_outside_axis_range (axis_tag, axis_بەها, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_بەها (axis_records, user_axes_location))
      return_trace (c->serializer->embed (هذا));

    return_trace (خطأ);
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
  NameID	بەها_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_بەها;	/* The numeric value for a style-linked mapping
				 * from this value. */
  عام:
  DEFINE_SIZE_STATIC (16);
};

بنية Axis_بەها_Record
{
  unsigned صحيح get_axis_index () ثابت { أرجع axisIndex; }
  عائم get_بەها ()             ثابت { أرجع value.to_float (); }

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

بنية Axis_بەها_Format4
{
  ثابت Axis_بەها_Record &get_axis_record (unsigned صحيح axis_index) ثابت
  { أرجع axis_بەهاکان_2.as_array (axis_ژماردن_2)[axis_index]; }

  منطقي keep_axis_بەها (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_array_t<ثابت Axis_بەها_Record> axis_بەها_records = axis_بەهاکان_2.as_array (axis_ژماردن_2);

    لكل (ثابت auto& rec : axis_بەها_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      عائم axis_بەها = rec.get_بەها ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      إذا (axis_بەها_is_outside_axis_range (axis_tag, axis_بەها, user_axes_location))
        أرجع خطأ;
    }

    أرجع true;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    إذا (!keep_axis_بەها (axis_records, user_axes_location))
      return_trace (خطأ);

    unsigned total_size = min_size + axis_ژماردن_2 * Axis_بەها_Record::static_size;
    auto *out = c->serializer->allocate_size<Axis_بەها_Format4> (total_size);
    إذا (unlikely (!out)) return_trace (خطأ);
    hb_memcpy (out, هذا, total_size);
    return_trace (true);
  }

  hb_ot_name_id_t get_بەها_name_id () ثابت { أرجع بەها_Name_ID; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
                          axis_بەهاکان_2.sanitize (c, axis_ژماردن_2)));
  }

  محمي:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_ژماردن_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	بەها_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_بەها_Record>
		axis_بەهاکان_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  عام:
  DEFINE_SIZE_ARRAY (8, axis_بەهاکان_2);
};

بنية Axis_بەها
{
  عائم get_بەها (unsigned صحيح axis_index) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_بەها ();
    حالة 2: hb_barrier (); أرجع u.format2.get_بەها ();
    حالة 3: hb_barrier (); أرجع u.format3.get_بەها ();
    حالة 4: hb_barrier (); أرجع u.format4.get_axis_record (axis_index).get_بەها ();
    افتراضي:أرجع 0.f;
    }
  }

  unsigned صحيح get_axis_index () ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_axis_index ();
    حالة 2: hb_barrier (); أرجع u.format2.get_axis_index ();
    حالة 3: hb_barrier (); أرجع u.format3.get_axis_index ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    افتراضي:أرجع -1;
    }
  }

  hb_ot_name_id_t get_بەها_name_id () ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_بەها_name_id ();
    حالة 2: hb_barrier (); أرجع u.format2.get_بەها_name_id ();
    حالة 3: hb_barrier (); أرجع u.format3.get_بەها_name_id ();
    حالة 4: hb_barrier (); أرجع u.format4.get_بەها_name_id ();
    افتراضي:أرجع HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_بەها ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_بەها ());
    }
  }

  منطقي keep_axis_بەها (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.keep_axis_بەها (axis_records, user_axes_location);
    حالة 2: hb_barrier (); أرجع u.format2.keep_axis_بەها (axis_records, user_axes_location);
    حالة 3: hb_barrier (); أرجع u.format3.keep_axis_بەها (axis_records, user_axes_location);
    حالة 4: hb_barrier (); أرجع u.format4.keep_axis_بەها (axis_records, user_axes_location);
    افتراضي:أرجع خطأ;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    افتراضي:return_trace (true);
    }
  }

  محمي:
  اتحاد
  {
  بنية { HBUINT16 v; }	format;
  Axis_بەها_Format1	format1;
  Axis_بەها_Format2	format2;
  Axis_بەها_Format3	format3;
  Axis_بەها_Format4	format4;
  } u;
  عام:
  DEFINE_SIZE_UNION (2, format.v);
};

بنية Axis_بەها_Offset_Array: UnsizedArrayOf<Offset16To<Axis_بەها>>
{
  منطقي subset (hb_subset_context_t *c,
               unsigned axis_بەها_ژماردن,
               unsigned& ژماردن,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);

    auto axis_بەها_Offsets = as_array (axis_بەها_ژماردن);
    ژماردن = 0;
    لكل (ثابت auto& offset : axis_بەها_Offsets)
    {
      إذا (!offset) استمر;
      auto o_snap = c->serializer->snapshot ();
      auto *o = c->serializer->embed (offset);
      إذا (!o) return_trace (خطأ);
      إذا (!o->serialize_subset (c, offset, هذا, axis_records))
      {
        c->serializer->revert (o_snap);
        استمر;
      }
      ژماردن++;
    }

    return_trace (ژماردن);
  }
};

بنية STAT
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  منطقي has_data () ثابت { أرجع version.to_int (); }

  منطقي get_بەها (hb_tag_t tag, عائم *value) ثابت
  {
    unsigned صحيح axis_index;
    إذا (!get_design_axes ().lfind (tag, &axis_index)) أرجع خطأ;

    hb_array_t<ثابت Offset16To<Axis_بەها>> axis_بەهاکان = get_axis_بەها_offsets ();
    لكل (unsigned صحيح i = 0; i < axis_بەهاکان.length; i++)
    {
      ثابت Axis_بەها& axis_بەها = هذا+offset_To_Axis_بەها_Offsets+axis_بەهاکان[i];
      إذا (axis_بەها.get_axis_index () == axis_index)
      {
	إذا (value)
	  *value = axis_بەها.get_بەها (axis_index);
	أرجع true;
      }
    }
    أرجع خطأ;
  }

  unsigned get_design_axis_ژماردن () ثابت { أرجع design_Axis_ژماردن; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) ثابت
  {
    إذا (unlikely (axis_record_index >= design_Axis_ژماردن)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    أرجع axis_record.get_name_id ();
  }

  unsigned get_axis_بەها_ژماردن () ثابت { أرجع axis_بەها_ژماردن; }

  hb_ot_name_id_t get_axis_بەها_name_id (unsigned axis_بەها_index) ثابت
  {
    إذا (unlikely (axis_بەها_index >= axis_بەها_ژماردن)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت Axis_بەها &axis_بەها = (هذا + get_axis_بەها_offsets ()[axis_بەها_index]);
    أرجع axis_بەها.get_بەها_name_id ();
  }

  فراغ collect_name_ids (hb_hashmap_t<hb_tag_t, Triple> *user_axes_location,
                         hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    إذا (!has_data ()) أرجع;

    + get_design_axes ()
    | hb_map (&StatAxisRecord::get_name_id)
    | hb_sink (nameids_to_retain)
    ;

    auto designAxes = get_design_axes ();

    + get_axis_بەها_offsets ()
    | hb_map (hb_add (&(هذا + offset_To_Axis_بەها_Offsets)))
    | hb_filter ([&] (ثابت Axis_بەها& _)
                 { أرجع _.keep_axis_بەها (designAxes, user_axes_location); })
    | hb_map (&Axis_بەها::get_بەها_name_id)
    | hb_sink (nameids_to_retain)
    ;

    nameids_to_retain->add (elidedFallbackNameID);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    STAT *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    auto designAxes = get_design_axes ();
    لكل (unsigned i = 0; i < (unsigned)design_Axis_ژماردن; i++)
      إذا (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (خطأ);

    إذا (design_Axis_ژماردن)
      c->serializer->check_assign (out->designAxesOffset, هذا->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned ژماردن = 0;
    out->offset_To_Axis_بەها_Offsets.serialize_subset (c, offset_To_Axis_بەها_Offsets, هذا,
                                                    axis_بەها_ژماردن, ژماردن, designAxes);
    return_trace (c->serializer->check_assign (out->axis_بەها_ژماردن, ژماردن, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, هذا, design_Axis_ژماردن) &&
			  offset_To_Axis_بەها_Offsets.sanitize (c, هذا, axis_بەها_ژماردن, &(هذا+offset_To_Axis_بەها_Offsets))));
  }

  محمي:
  hb_array_t<ثابت StatAxisRecord> ثابت get_design_axes () ثابت
  { أرجع (هذا+designAxesOffset).as_array (design_Axis_ژماردن); }

  hb_array_t<ثابت Offset16To<Axis_بەها>> ثابت get_axis_بەها_offsets () ثابت
  { أرجع (هذا+offset_To_Axis_بەها_Offsets).as_array (axis_بەها_ژماردن); }


  محمي:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_ژماردن;/* The number of design axis records. In a
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
  HBUINT16	axis_بەها_ژماردن;	/* The number of axis value tables. */
  NNOffset32To<Axis_بەها_Offset_Array>
		offset_To_Axis_بەها_Offsets;
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
