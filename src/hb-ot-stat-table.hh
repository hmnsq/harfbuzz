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

#اگر_تعریف_نشده HB_OT_STAT_جدول_HH
#تعریف_کن HB_OT_STAT_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#تعریف_کن HB_OT_برچسب_STAT HB_برچسب('S','T','A','T')


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
  ELIDABLE_AXIS_مقدار_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

ساكن منطقي axis_مقدار_is_outside_axis_range (hb_برچسب_t axis_برچسب_2, عائم axis_مقدار,
                                              ثابت hb_hashmap_t<hb_برچسب_t, Triple> *user_axes_location)
{
  إذا (!user_axes_location->has (axis_برچسب_2))
    أرجع خطأ;

  Triple axis_range = user_axes_location->get (axis_برچسب_2);
  أرجع (axis_مقدار < axis_range.minimum || axis_مقدار > axis_range.بیشینه);
}

بنية Stat_Axis_رکورد
{
  صحيح cmp (hb_برچسب_t key) ثابت { أرجع برچسب_2.cmp (key); }

  hb_ot_name_id_t get_name_id () ثابت { أرجع nameID; }

  hb_برچسب_t get_axis_برچسب () ثابت { أرجع برچسب_2; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا)));
  }

  محمي:
  برچسب		برچسب_2;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Axis_مقدار_Format1
{
  بیعلامت صحيح get_axis_شاخص () ثابت { أرجع axis_شاخص_2; }
  عائم get_مقدار ()             ثابت { أرجع value.مقصد_float (); }

  hb_ot_name_id_t get_مقدار_name_id () ثابت { أرجع مقدار_Name_ID; }

  hb_برچسب_t get_axis_برچسب (ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها) ثابت
  {
    بیعلامت axis_idx = get_axis_شاخص ();
    أرجع axis_رکوردها[axis_idx].get_axis_برچسب ();
  }

  منطقي keep_axis_مقدار (ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها,
                        ثابت hb_hashmap_t<hb_برچسب_t, Triple> *user_axes_location) ثابت
  {
    hb_برچسب_t axis_برچسب_2 = get_axis_برچسب (axis_رکوردها);
    عائم axis_مقدار = get_مقدار ();

    أرجع !axis_مقدار_is_outside_axis_range (axis_برچسب_2, axis_مقدار, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_برچسب_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_مقدار (axis_رکوردها, user_axes_location))
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
  HBUINT16	axis_شاخص_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	مقدار_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عام:
  DEFINE_اندازه_STATIC (12);
};

بنية Axis_مقدار_Format2
{
  بیعلامت صحيح get_axis_شاخص () ثابت { أرجع axis_شاخص_2; }
  عائم get_مقدار ()             ثابت { أرجع nominal_مقدار.مقصد_float (); }

  hb_ot_name_id_t get_مقدار_name_id () ثابت { أرجع مقدار_Name_ID; }

  hb_برچسب_t get_axis_برچسب (ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها) ثابت
  {
    بیعلامت axis_idx = get_axis_شاخص ();
    أرجع axis_رکوردها[axis_idx].get_axis_برچسب ();
  }

  منطقي keep_axis_مقدار (ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها,
                        ثابت hb_hashmap_t<hb_برچسب_t, Triple> *user_axes_location) ثابت
  {
    hb_برچسب_t axis_برچسب_2 = get_axis_برچسب (axis_رکوردها);
    عائم axis_مقدار = get_مقدار ();

    أرجع !axis_مقدار_is_outside_axis_range (axis_برچسب_2, axis_مقدار, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_برچسب_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_مقدار (axis_رکوردها, user_axes_location))
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
  HBUINT16	axis_شاخص_2;	/* Zero-base index into the axis record array
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
  عام:
  DEFINE_اندازه_STATIC (20);
};

بنية Axis_مقدار_Format3
{
  بیعلامت صحيح get_axis_شاخص () ثابت { أرجع axis_شاخص_2; }
  عائم get_مقدار ()             ثابت { أرجع value.مقصد_float (); }

  hb_ot_name_id_t get_مقدار_name_id () ثابت { أرجع مقدار_Name_ID; }

  hb_برچسب_t get_axis_برچسب (ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها) ثابت
  {
    بیعلامت axis_idx = get_axis_شاخص ();
    أرجع axis_رکوردها[axis_idx].get_axis_برچسب ();
  }

  منطقي keep_axis_مقدار (ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها,
                        ثابت hb_hashmap_t<hb_برچسب_t, Triple> *user_axes_location) ثابت
  {
    hb_برچسب_t axis_برچسب_2 = get_axis_برچسب (axis_رکوردها);
    عائم axis_مقدار = get_مقدار ();

    أرجع !axis_مقدار_is_outside_axis_range (axis_برچسب_2, axis_مقدار, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_برچسب_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_مقدار (axis_رکوردها, user_axes_location))
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
  HBUINT16	axis_شاخص_2;	/* Zero-base index into the axis record array
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
  عام:
  DEFINE_اندازه_STATIC (16);
};

بنية Axis_مقدار_رکورد
{
  بیعلامت صحيح get_axis_شاخص () ثابت { أرجع axis_شاخص_2; }
  عائم get_مقدار ()             ثابت { أرجع value.مقصد_float (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	axis_شاخص_2;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية Axis_مقدار_Format4
{
  ثابت Axis_مقدار_رکورد &get_axis_رکورد (بیعلامت صحيح axis_شاخص) ثابت
  { أرجع axis_مقادیر_2.as_array (axis_شمار_2)[axis_شاخص]; }

  منطقي keep_axis_مقدار (ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها,
                        ثابت hb_hashmap_t<hb_برچسب_t, Triple> *user_axes_location) ثابت
  {
    hb_array_t<ثابت Axis_مقدار_رکورد> axis_مقدار_رکوردها = axis_مقادیر_2.as_array (axis_شمار_2);

    لكل (ثابت خودکار& rec : axis_مقدار_رکوردها)
    {
      بیعلامت axis_idx = rec.get_axis_شاخص ();
      عائم axis_مقدار = rec.get_مقدار ();
      hb_برچسب_t axis_برچسب_2 = axis_رکوردها[axis_idx].get_axis_برچسب ();

      إذا (axis_مقدار_is_outside_axis_range (axis_برچسب_2, axis_مقدار, user_axes_location))
        أرجع خطأ;
    }

    أرجع درست;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_برچسب_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    إذا (!keep_axis_مقدار (axis_رکوردها, user_axes_location))
      return_trace (خطأ);

    بیعلامت مجموع_اندازه = min_اندازه + axis_شمار_2 * Axis_مقدار_رکورد::static_اندازه;
    خودکار *بیرون = c->serializer->allocate_اندازه<Axis_مقدار_Format4> (مجموع_اندازه);
    إذا (unlikely (!بیرون)) return_trace (خطأ);
    hb_memcpy (بیرون, هذا, مجموع_اندازه);
    return_trace (درست);
  }

  hb_ot_name_id_t get_مقدار_name_id () ثابت { أرجع مقدار_Name_ID; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
                          axis_مقادیر_2.sanitize (c, axis_شمار_2)));
  }

  محمي:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_شمار_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	مقدار_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  Unsized_Array_از<Axis_مقدار_رکورد>
		axis_مقادیر_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  عام:
  DEFINE_اندازه_ARRAY (8, axis_مقادیر_2);
};

بنية Axis_مقدار
{
  عائم get_مقدار (بیعلامت صحيح axis_شاخص) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_مقدار ();
    حالة 2: hb_barrier (); أرجع u.format2.get_مقدار ();
    حالة 3: hb_barrier (); أرجع u.format3.get_مقدار ();
    حالة 4: hb_barrier (); أرجع u.format4.get_axis_رکورد (axis_شاخص).get_مقدار ();
    افتراضي:أرجع 0.f;
    }
  }

  بیعلامت صحيح get_axis_شاخص () ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_axis_شاخص ();
    حالة 2: hb_barrier (); أرجع u.format2.get_axis_شاخص ();
    حالة 3: hb_barrier (); أرجع u.format3.get_axis_شاخص ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    افتراضي:أرجع -1;
    }
  }

  hb_ot_name_id_t get_مقدار_name_id () ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_مقدار_name_id ();
    حالة 2: hb_barrier (); أرجع u.format2.get_مقدار_name_id ();
    حالة 3: hb_barrier (); أرجع u.format3.get_مقدار_name_id ();
    حالة 4: hb_barrier (); أرجع u.format4.get_مقدار_name_id ();
    افتراضي:أرجع HB_OT_NAME_ID_INVALID;
    }
  }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }

  منطقي keep_axis_مقدار (ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها,
                        hb_hashmap_t<hb_برچسب_t, Triple> *user_axes_location) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.keep_axis_مقدار (axis_رکوردها, user_axes_location);
    حالة 2: hb_barrier (); أرجع u.format2.keep_axis_مقدار (axis_رکوردها, user_axes_location);
    حالة 3: hb_barrier (); أرجع u.format3.keep_axis_مقدار (axis_رکوردها, user_axes_location);
    حالة 4: hb_barrier (); أرجع u.format4.keep_axis_مقدار (axis_رکوردها, user_axes_location);
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
    افتراضي:return_trace (درست);
    }
  }

  محمي:
  اتحاد
  {
  بنية { HBUINT16 v; }	format;
  Axis_مقدار_Format1	format1;
  Axis_مقدار_Format2	format2;
  Axis_مقدار_Format3	format3;
  Axis_مقدار_Format4	format4;
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};

بنية Axis_مقدار_جابه_جایی_Array: Unsized_Array_از<Offset16_مقصد<Axis_مقدار>>
{
  منطقي subset (hb_subset_context_t *c,
               بیعلامت axis_مقدار_شمار,
               بیعلامت& شمار,
               ثابت hb_array_t<ثابت Stat_Axis_رکورد> axis_رکوردها) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار axis_مقدار_Offsets = as_array (axis_مقدار_شمار);
    شمار = 0;
    لكل (ثابت خودکار& جابه_جایی : axis_مقدار_Offsets)
    {
      إذا (!جابه_جایی) استمر;
      خودکار o_snap = c->serializer->snapshot ();
      خودکار *o = c->serializer->embed (جابه_جایی);
      إذا (!o) return_trace (خطأ);
      إذا (!o->serialize_subset (c, جابه_جایی, هذا, axis_رکوردها))
      {
        c->serializer->revert (o_snap);
        استمر;
      }
      شمار++;
    }

    return_trace (شمار);
  }
};

بنية STAT
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_STAT;

  منطقي has_داده () ثابت { أرجع نسخه.مقصد_int (); }

  منطقي get_مقدار (hb_برچسب_t برچسب_2, عائم *value) ثابت
  {
    بیعلامت صحيح axis_شاخص;
    إذا (!get_design_axes ().lfind (برچسب_2, &axis_شاخص)) أرجع خطأ;

    hb_array_t<ثابت Offset16_مقصد<Axis_مقدار>> axis_مقادیر = get_axis_مقدار_offsets ();
    لكل (بیعلامت صحيح i = 0; i < axis_مقادیر.درازی; i++)
    {
      ثابت Axis_مقدار& axis_مقدار = هذا+جابه_جایی_مقصد_Axis_مقدار_Offsets+axis_مقادیر[i];
      إذا (axis_مقدار.get_axis_شاخص () == axis_شاخص)
      {
	إذا (value)
	  *value = axis_مقدار.get_مقدار (axis_شاخص);
	أرجع درست;
      }
    }
    أرجع خطأ;
  }

  بیعلامت get_design_axis_شمار () ثابت { أرجع design_Axis_شمار; }

  hb_ot_name_id_t get_axis_رکورد_name_id (بیعلامت axis_رکورد_شاخص) ثابت
  {
    إذا (unlikely (axis_رکورد_شاخص >= design_Axis_شمار)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت Stat_Axis_رکورد &axis_رکورد = get_design_axes ()[axis_رکورد_شاخص];
    أرجع axis_رکورد.get_name_id ();
  }

  بیعلامت get_axis_مقدار_شمار () ثابت { أرجع axis_مقدار_شمار; }

  hb_ot_name_id_t get_axis_مقدار_name_id (بیعلامت axis_مقدار_شاخص) ثابت
  {
    إذا (unlikely (axis_مقدار_شاخص >= axis_مقدار_شمار)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت Axis_مقدار &axis_مقدار = (هذا + get_axis_مقدار_offsets ()[axis_مقدار_شاخص]);
    أرجع axis_مقدار.get_مقدار_name_id ();
  }

  فراغ collect_name_ids (hb_hashmap_t<hb_برچسب_t, Triple> *user_axes_location,
                         hb_مجموعه_t *nameids_مقصد_retain /*بیرون*/) ثابت
  {
    إذا (!has_داده ()) أرجع;

    + get_design_axes ()
    | hb_map (&Stat_Axis_رکورد::get_name_id)
    | hb_sink (nameids_مقصد_retain)
    ;

    خودکار designAxes = get_design_axes ();

    + get_axis_مقدار_offsets ()
    | hb_map (hb_جمع_کن (&(هذا + جابه_جایی_مقصد_Axis_مقدار_Offsets)))
    | hb_filter ([&] (ثابت Axis_مقدار& _)
                 { أرجع _.keep_axis_مقدار (designAxes, user_axes_location); })
    | hb_map (&Axis_مقدار::get_مقدار_name_id)
    | hb_sink (nameids_مقصد_retain)
    ;

    nameids_مقصد_retain->جمع_کن (elidedFallbackNameID);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    STAT *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    خودکار designAxes = get_design_axes ();
    لكل (بیعلامت i = 0; i < (بیعلامت)design_Axis_شمار; i++)
      إذا (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (خطأ);

    إذا (design_Axis_شمار)
      c->serializer->check_تخصیص_بده (بیرون->design_Axes_جابه_جایی, هذا->get_اندازه (),
                                   HB_SERIALIZE_ERROR_INT_سرریز);

    بیعلامت شمار = 0;
    بیرون->جابه_جایی_مقصد_Axis_مقدار_Offsets.serialize_subset (c, جابه_جایی_مقصد_Axis_مقدار_Offsets, هذا,
                                                    axis_مقدار_شمار, شمار, designAxes);
    return_trace (c->serializer->check_تخصیص_بده (بیرون->axis_مقدار_شمار, شمار, HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخه.major == 1 &&
			  نسخه.minor > 0 &&
			  design_Axes_جابه_جایی.sanitize (c, هذا, design_Axis_شمار) &&
			  جابه_جایی_مقصد_Axis_مقدار_Offsets.sanitize (c, هذا, axis_مقدار_شمار, &(هذا+جابه_جایی_مقصد_Axis_مقدار_Offsets))));
  }

  محمي:
  hb_array_t<ثابت Stat_Axis_رکورد> ثابت get_design_axes () ثابت
  { أرجع (هذا+design_Axes_جابه_جایی).as_array (design_Axis_شمار); }

  hb_array_t<ثابت Offset16_مقصد<Axis_مقدار>> ثابت get_axis_مقدار_offsets () ثابت
  { أرجع (هذا+جابه_جایی_مقصد_Axis_مقدار_Offsets).as_array (axis_مقدار_شمار); }


  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	design_Axis_اندازه;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_شمار;/* The number of design axis records. In a
				 * font with an 'fvar' table, this value must be
				 * greater than or equal to the axisCount value
				 * in the 'fvar' table. In all fonts, must
				 * be greater than zero if axisValueCount
				 * is greater than zero. */
  NNOffset32_مقصد<Unsized_Array_از<Stat_Axis_رکورد>>
		design_Axes_جابه_جایی;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes array. If designAxisCount is zero,
				 * set to zero; if designAxisCount is greater
				 * than zero, must be greater than zero. */
  HBUINT16	axis_مقدار_شمار;	/* The number of axis value tables. */
  NNOffset32_مقصد<Axis_مقدار_جابه_جایی_Array>
		جابه_جایی_مقصد_Axis_مقدار_Offsets;
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
  DEFINE_اندازه_STATIC (20);
};


} /* namespace OT */


#پایان_اگر /* HB_OT_STAT_TABLE_HH */
