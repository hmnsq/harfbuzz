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
  ELIDABLE_AXIS_نيلاي_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

ساكن منطقي axis_نيلاي_is_outside_axis_range (hb_tag_t axis_tag, عائم axis_نيلاي,
                                              ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location)
{
  إذا (!user_axes_location->has (axis_tag))
    أرجع خطأ;

  Triple axis_range = user_axes_location->get (axis_tag);
  أرجع (axis_نيلاي < axis_range.minimum || axis_نيلاي > axis_range.maximum);
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

بنية Axis_نيلاي_Format1
{
  unsigned صحيح get_axis_index () ثابت { أرجع axisIndex; }
  عائم get_نيلاي ()             ثابت { أرجع value.to_float (); }

  hb_ot_name_id_t get_نيلاي_name_id () ثابت { أرجع نيلاي_Name_ID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    أرجع axis_records[axis_idx].get_axis_tag ();
  }

  منطقي keep_axis_نيلاي (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    عائم axis_نيلاي = get_نيلاي ();

    أرجع !axis_نيلاي_is_outside_axis_range (axis_tag, axis_نيلاي, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_نيلاي (axis_records, user_axes_location))
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
  NameID	نيلاي_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عام:
  DEFINE_SIZE_STATIC (12);
};

بنية Axis_نيلاي_Format2
{
  unsigned صحيح get_axis_index () ثابت { أرجع axisIndex; }
  عائم get_نيلاي ()             ثابت { أرجع nominal_نيلاي.to_float (); }

  hb_ot_name_id_t get_نيلاي_name_id () ثابت { أرجع نيلاي_Name_ID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    أرجع axis_records[axis_idx].get_axis_tag ();
  }

  منطقي keep_axis_نيلاي (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    عائم axis_نيلاي = get_نيلاي ();

    أرجع !axis_نيلاي_is_outside_axis_range (axis_tag, axis_نيلاي, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_نيلاي (axis_records, user_axes_location))
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
  NameID	نيلاي_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_نيلاي;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_نيلاي;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_نيلاي;	/* The maximum value for a range associated
				 * with the specified name ID. */
  عام:
  DEFINE_SIZE_STATIC (20);
};

بنية Axis_نيلاي_Format3
{
  unsigned صحيح get_axis_index () ثابت { أرجع axisIndex; }
  عائم get_نيلاي ()             ثابت { أرجع value.to_float (); }

  hb_ot_name_id_t get_نيلاي_name_id () ثابت { أرجع نيلاي_Name_ID; }

  hb_tag_t get_axis_tag (ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    unsigned axis_idx = get_axis_index ();
    أرجع axis_records[axis_idx].get_axis_tag ();
  }

  منطقي keep_axis_نيلاي (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_tag_t axis_tag = get_axis_tag (axis_records);
    عائم axis_نيلاي = get_نيلاي ();

    أرجع !axis_نيلاي_is_outside_axis_range (axis_tag, axis_نيلاي, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_نيلاي (axis_records, user_axes_location))
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
  NameID	نيلاي_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_نيلاي;	/* The numeric value for a style-linked mapping
				 * from this value. */
  عام:
  DEFINE_SIZE_STATIC (16);
};

بنية Axis_نيلاي_Record
{
  unsigned صحيح get_axis_index () ثابت { أرجع axisIndex; }
  عائم get_نيلاي ()             ثابت { أرجع value.to_float (); }

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

بنية Axis_نيلاي_Format4
{
  ثابت Axis_نيلاي_Record &get_axis_record (unsigned صحيح axis_index) ثابت
  { أرجع axis_نيلاي_سموا_2.as_array (axis_کيراءن_2)[axis_index]; }

  منطقي keep_axis_نيلاي (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    hb_array_t<ثابت Axis_نيلاي_Record> axis_نيلاي_records = axis_نيلاي_سموا_2.as_array (axis_کيراءن_2);

    لكل (ثابت auto& rec : axis_نيلاي_records)
    {
      unsigned axis_idx = rec.get_axis_index ();
      عائم axis_نيلاي = rec.get_نيلاي ();
      hb_tag_t axis_tag = axis_records[axis_idx].get_axis_tag ();

      إذا (axis_نيلاي_is_outside_axis_range (axis_tag, axis_نيلاي, user_axes_location))
        أرجع خطأ;
    }

    أرجع true;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_tag_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    إذا (!keep_axis_نيلاي (axis_records, user_axes_location))
      return_trace (خطأ);

    unsigned total_size = min_size + axis_کيراءن_2 * Axis_نيلاي_Record::static_size;
    auto *out = c->serializer->allocate_size<Axis_نيلاي_Format4> (total_size);
    إذا (unlikely (!out)) return_trace (خطأ);
    hb_memcpy (out, هذا, total_size);
    return_trace (true);
  }

  hb_ot_name_id_t get_نيلاي_name_id () ثابت { أرجع نيلاي_Name_ID; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
                          axis_نيلاي_سموا_2.sanitize (c, axis_کيراءن_2)));
  }

  محمي:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_کيراءن_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	نيلاي_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  UnsizedArrayOf<Axis_نيلاي_Record>
		axis_نيلاي_سموا_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  عام:
  DEFINE_SIZE_ARRAY (8, axis_نيلاي_سموا_2);
};

بنية Axis_نيلاي
{
  عائم get_نيلاي (unsigned صحيح axis_index) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_نيلاي ();
    حالة 2: hb_barrier (); أرجع u.format2.get_نيلاي ();
    حالة 3: hb_barrier (); أرجع u.format3.get_نيلاي ();
    حالة 4: hb_barrier (); أرجع u.format4.get_axis_record (axis_index).get_نيلاي ();
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

  hb_ot_name_id_t get_نيلاي_name_id () ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_نيلاي_name_id ();
    حالة 2: hb_barrier (); أرجع u.format2.get_نيلاي_name_id ();
    حالة 3: hb_barrier (); أرجع u.format3.get_نيلاي_name_id ();
    حالة 4: hb_barrier (); أرجع u.format4.get_نيلاي_name_id ();
    افتراضي:أرجع HB_OT_NAME_ID_INVALID;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_نيلاي ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_نيلاي ());
    }
  }

  منطقي keep_axis_نيلاي (ثابت hb_array_t<ثابت StatAxisRecord> axis_records,
                        hb_hashmap_t<hb_tag_t, Triple> *user_axes_location) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.keep_axis_نيلاي (axis_records, user_axes_location);
    حالة 2: hb_barrier (); أرجع u.format2.keep_axis_نيلاي (axis_records, user_axes_location);
    حالة 3: hb_barrier (); أرجع u.format3.keep_axis_نيلاي (axis_records, user_axes_location);
    حالة 4: hb_barrier (); أرجع u.format4.keep_axis_نيلاي (axis_records, user_axes_location);
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
  Axis_نيلاي_Format1	format1;
  Axis_نيلاي_Format2	format2;
  Axis_نيلاي_Format3	format3;
  Axis_نيلاي_Format4	format4;
  } u;
  عام:
  DEFINE_SIZE_UNION (2, format.v);
};

بنية Axis_نيلاي_Offset_Array: UnsizedArrayOf<Offset16To<Axis_نيلاي>>
{
  منطقي subset (hb_subset_context_t *c,
               unsigned axis_نيلاي_کيراءن,
               unsigned& کيراءن,
               ثابت hb_array_t<ثابت StatAxisRecord> axis_records) ثابت
  {
    TRACE_SUBSET (هذا);

    auto axis_نيلاي_Offsets = as_array (axis_نيلاي_کيراءن);
    کيراءن = 0;
    لكل (ثابت auto& offset : axis_نيلاي_Offsets)
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
      کيراءن++;
    }

    return_trace (کيراءن);
  }
};

بنية STAT
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_STAT;

  منطقي has_data () ثابت { أرجع version.to_int (); }

  منطقي get_نيلاي (hb_tag_t tag, عائم *value) ثابت
  {
    unsigned صحيح axis_index;
    إذا (!get_design_axes ().lfind (tag, &axis_index)) أرجع خطأ;

    hb_array_t<ثابت Offset16To<Axis_نيلاي>> axis_نيلاي_سموا = get_axis_نيلاي_offsets ();
    لكل (unsigned صحيح i = 0; i < axis_نيلاي_سموا.length; i++)
    {
      ثابت Axis_نيلاي& axis_نيلاي = هذا+offset_To_Axis_نيلاي_Offsets+axis_نيلاي_سموا[i];
      إذا (axis_نيلاي.get_axis_index () == axis_index)
      {
	إذا (value)
	  *value = axis_نيلاي.get_نيلاي (axis_index);
	أرجع true;
      }
    }
    أرجع خطأ;
  }

  unsigned get_design_axis_کيراءن () ثابت { أرجع design_Axis_کيراءن; }

  hb_ot_name_id_t get_axis_record_name_id (unsigned axis_record_index) ثابت
  {
    إذا (unlikely (axis_record_index >= design_Axis_کيراءن)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت StatAxisRecord &axis_record = get_design_axes ()[axis_record_index];
    أرجع axis_record.get_name_id ();
  }

  unsigned get_axis_نيلاي_کيراءن () ثابت { أرجع axis_نيلاي_کيراءن; }

  hb_ot_name_id_t get_axis_نيلاي_name_id (unsigned axis_نيلاي_index) ثابت
  {
    إذا (unlikely (axis_نيلاي_index >= axis_نيلاي_کيراءن)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت Axis_نيلاي &axis_نيلاي = (هذا + get_axis_نيلاي_offsets ()[axis_نيلاي_index]);
    أرجع axis_نيلاي.get_نيلاي_name_id ();
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

    + get_axis_نيلاي_offsets ()
    | hb_map (hb_add (&(هذا + offset_To_Axis_نيلاي_Offsets)))
    | hb_filter ([&] (ثابت Axis_نيلاي& _)
                 { أرجع _.keep_axis_نيلاي (designAxes, user_axes_location); })
    | hb_map (&Axis_نيلاي::get_نيلاي_name_id)
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
    لكل (unsigned i = 0; i < (unsigned)design_Axis_کيراءن; i++)
      إذا (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (خطأ);

    إذا (design_Axis_کيراءن)
      c->serializer->check_assign (out->designAxesOffset, هذا->get_size (),
                                   HB_SERIALIZE_ERROR_INT_OVERFLOW);

    unsigned کيراءن = 0;
    out->offset_To_Axis_نيلاي_Offsets.serialize_subset (c, offset_To_Axis_نيلاي_Offsets, هذا,
                                                    axis_نيلاي_کيراءن, کيراءن, designAxes);
    return_trace (c->serializer->check_assign (out->axis_نيلاي_کيراءن, کيراءن, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  version.minor > 0 &&
			  designAxesOffset.sanitize (c, هذا, design_Axis_کيراءن) &&
			  offset_To_Axis_نيلاي_Offsets.sanitize (c, هذا, axis_نيلاي_کيراءن, &(هذا+offset_To_Axis_نيلاي_Offsets))));
  }

  محمي:
  hb_array_t<ثابت StatAxisRecord> ثابت get_design_axes () ثابت
  { أرجع (هذا+designAxesOffset).as_array (design_Axis_کيراءن); }

  hb_array_t<ثابت Offset16To<Axis_نيلاي>> ثابت get_axis_نيلاي_offsets () ثابت
  { أرجع (هذا+offset_To_Axis_نيلاي_Offsets).as_array (axis_نيلاي_کيراءن); }


  محمي:
  FixedVersion<>version;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	designAxisSize;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_کيراءن;/* The number of design axis records. In a
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
  HBUINT16	axis_نيلاي_کيراءن;	/* The number of axis value tables. */
  NNOffset32To<Axis_نيلاي_Offset_Array>
		offset_To_Axis_نيلاي_Offsets;
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
