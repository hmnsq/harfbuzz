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

#که_نه_تعریف_شوی HB_OT_STAT_جدول_HH
#تعریف_کړه HB_OT_STAT_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#تعریف_کړه HB_OT_نښان_STAT HB_نښان('S','T','A','T')


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
  ELIDABLE_AXIS_ارزښت_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

ساكن منطقي axis_ارزښت_is_outside_axis_range (hb_نښان_t axis_نښان_2, عائم axis_ارزښت,
                                              ثابت hb_hashmap_t<hb_نښان_t, Triple> *user_axes_location)
{
  إذا (!user_axes_location->has (axis_نښان_2))
    أرجع خطأ;

  Triple axis_range = user_axes_location->get (axis_نښان_2);
  أرجع (axis_ارزښت < axis_range.minimum || axis_ارزښت > axis_range.اعظمي);
}

بنية Stat_Axis_ریکارډ
{
  صحيح cmp (hb_نښان_t key) ثابت { أرجع نښان_2.cmp (key); }

  hb_ot_name_id_t get_name_id () ثابت { أرجع nameID; }

  hb_نښان_t get_axis_نښان () ثابت { أرجع نښان_2; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا)));
  }

  محمي:
  نښان		نښان_2;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Axis_ارزښت_Format1
{
  بینښه صحيح get_axis_شاخص () ثابت { أرجع axis_شاخص_2; }
  عائم get_ارزښت ()             ثابت { أرجع value.منزل_float (); }

  hb_ot_name_id_t get_ارزښت_name_id () ثابت { أرجع ارزښت_Name_ID; }

  hb_نښان_t get_axis_نښان (ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه) ثابت
  {
    بینښه axis_idx = get_axis_شاخص ();
    أرجع axis_ریکارډونه[axis_idx].get_axis_نښان ();
  }

  منطقي keep_axis_ارزښت (ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه,
                        ثابت hb_hashmap_t<hb_نښان_t, Triple> *user_axes_location) ثابت
  {
    hb_نښان_t axis_نښان_2 = get_axis_نښان (axis_ریکارډونه);
    عائم axis_ارزښت = get_ارزښت ();

    أرجع !axis_ارزښت_is_outside_axis_range (axis_نښان_2, axis_ارزښت, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_نښان_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_ارزښت (axis_ریکارډونه, user_axes_location))
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
  NameID	ارزښت_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عام:
  DEFINE_اندازه_STATIC (12);
};

بنية Axis_ارزښت_Format2
{
  بینښه صحيح get_axis_شاخص () ثابت { أرجع axis_شاخص_2; }
  عائم get_ارزښت ()             ثابت { أرجع nominal_ارزښت.منزل_float (); }

  hb_ot_name_id_t get_ارزښت_name_id () ثابت { أرجع ارزښت_Name_ID; }

  hb_نښان_t get_axis_نښان (ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه) ثابت
  {
    بینښه axis_idx = get_axis_شاخص ();
    أرجع axis_ریکارډونه[axis_idx].get_axis_نښان ();
  }

  منطقي keep_axis_ارزښت (ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه,
                        ثابت hb_hashmap_t<hb_نښان_t, Triple> *user_axes_location) ثابت
  {
    hb_نښان_t axis_نښان_2 = get_axis_نښان (axis_ریکارډونه);
    عائم axis_ارزښت = get_ارزښت ();

    أرجع !axis_ارزښت_is_outside_axis_range (axis_نښان_2, axis_ارزښت, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_نښان_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_ارزښت (axis_ریکارډونه, user_axes_location))
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
  NameID	ارزښت_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_ارزښت;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_ارزښت;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_ارزښت;	/* The maximum value for a range associated
				 * with the specified name ID. */
  عام:
  DEFINE_اندازه_STATIC (20);
};

بنية Axis_ارزښت_Format3
{
  بینښه صحيح get_axis_شاخص () ثابت { أرجع axis_شاخص_2; }
  عائم get_ارزښت ()             ثابت { أرجع value.منزل_float (); }

  hb_ot_name_id_t get_ارزښت_name_id () ثابت { أرجع ارزښت_Name_ID; }

  hb_نښان_t get_axis_نښان (ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه) ثابت
  {
    بینښه axis_idx = get_axis_شاخص ();
    أرجع axis_ریکارډونه[axis_idx].get_axis_نښان ();
  }

  منطقي keep_axis_ارزښت (ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه,
                        ثابت hb_hashmap_t<hb_نښان_t, Triple> *user_axes_location) ثابت
  {
    hb_نښان_t axis_نښان_2 = get_axis_نښان (axis_ریکارډونه);
    عائم axis_ارزښت = get_ارزښت ();

    أرجع !axis_ارزښت_is_outside_axis_range (axis_نښان_2, axis_ارزښت, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_نښان_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_ارزښت (axis_ریکارډونه, user_axes_location))
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
  NameID	ارزښت_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_ارزښت;	/* The numeric value for a style-linked mapping
				 * from this value. */
  عام:
  DEFINE_اندازه_STATIC (16);
};

بنية Axis_ارزښت_ریکارډ
{
  بینښه صحيح get_axis_شاخص () ثابت { أرجع axis_شاخص_2; }
  عائم get_ارزښت ()             ثابت { أرجع value.منزل_float (); }

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

بنية Axis_ارزښت_Format4
{
  ثابت Axis_ارزښت_ریکارډ &get_axis_ریکارډ (بینښه صحيح axis_شاخص) ثابت
  { أرجع axis_ارزښتونه_2.as_array (axis_شمېر_2)[axis_شاخص]; }

  منطقي keep_axis_ارزښت (ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه,
                        ثابت hb_hashmap_t<hb_نښان_t, Triple> *user_axes_location) ثابت
  {
    hb_array_t<ثابت Axis_ارزښت_ریکارډ> axis_ارزښت_ریکارډونه = axis_ارزښتونه_2.as_array (axis_شمېر_2);

    لكل (ثابت خپلکاری& rec : axis_ارزښت_ریکارډونه)
    {
      بینښه axis_idx = rec.get_axis_شاخص ();
      عائم axis_ارزښت = rec.get_ارزښت ();
      hb_نښان_t axis_نښان_2 = axis_ریکارډونه[axis_idx].get_axis_نښان ();

      إذا (axis_ارزښت_is_outside_axis_range (axis_نښان_2, axis_ارزښت, user_axes_location))
        أرجع خطأ;
    }

    أرجع ریښتیا;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_نښان_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    إذا (!keep_axis_ارزښت (axis_ریکارډونه, user_axes_location))
      return_trace (خطأ);

    بینښه ټول_اندازه = min_اندازه + axis_شمېر_2 * Axis_ارزښت_ریکارډ::static_اندازه;
    خپلکاری *بهر = c->serializer->allocate_اندازه<Axis_ارزښت_Format4> (ټول_اندازه);
    إذا (unlikely (!بهر)) return_trace (خطأ);
    hb_memcpy (بهر, هذا, ټول_اندازه);
    return_trace (ریښتیا);
  }

  hb_ot_name_id_t get_ارزښت_name_id () ثابت { أرجع ارزښت_Name_ID; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
                          axis_ارزښتونه_2.sanitize (c, axis_شمېر_2)));
  }

  محمي:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_شمېر_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	ارزښت_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  Unsized_Array_د<Axis_ارزښت_ریکارډ>
		axis_ارزښتونه_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  عام:
  DEFINE_اندازه_ARRAY (8, axis_ارزښتونه_2);
};

بنية Axis_ارزښت
{
  عائم get_ارزښت (بینښه صحيح axis_شاخص) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_ارزښت ();
    حالة 2: hb_barrier (); أرجع u.format2.get_ارزښت ();
    حالة 3: hb_barrier (); أرجع u.format3.get_ارزښت ();
    حالة 4: hb_barrier (); أرجع u.format4.get_axis_ریکارډ (axis_شاخص).get_ارزښت ();
    افتراضي:أرجع 0.f;
    }
  }

  بینښه صحيح get_axis_شاخص () ثابت
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

  hb_ot_name_id_t get_ارزښت_name_id () ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_ارزښت_name_id ();
    حالة 2: hb_barrier (); أرجع u.format2.get_ارزښت_name_id ();
    حالة 3: hb_barrier (); أرجع u.format3.get_ارزښت_name_id ();
    حالة 4: hb_barrier (); أرجع u.format4.get_ارزښت_name_id ();
    افتراضي:أرجع HB_OT_NAME_ID_INVALID;
    }
  }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_ارزښت ());
    }
  }

  منطقي keep_axis_ارزښت (ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه,
                        hb_hashmap_t<hb_نښان_t, Triple> *user_axes_location) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.keep_axis_ارزښت (axis_ریکارډونه, user_axes_location);
    حالة 2: hb_barrier (); أرجع u.format2.keep_axis_ارزښت (axis_ریکارډونه, user_axes_location);
    حالة 3: hb_barrier (); أرجع u.format3.keep_axis_ارزښت (axis_ریکارډونه, user_axes_location);
    حالة 4: hb_barrier (); أرجع u.format4.keep_axis_ارزښت (axis_ریکارډونه, user_axes_location);
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
    افتراضي:return_trace (ریښتیا);
    }
  }

  محمي:
  اتحاد
  {
  بنية { HBUINT16 v; }	format;
  Axis_ارزښت_Format1	format1;
  Axis_ارزښت_Format2	format2;
  Axis_ارزښت_Format3	format3;
  Axis_ارزښت_Format4	format4;
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};

بنية Axis_ارزښت_دننه_واټن_Array: Unsized_Array_د<Offset16_منزل<Axis_ارزښت>>
{
  منطقي subset (hb_subset_context_t *c,
               بینښه axis_ارزښت_شمېر,
               بینښه& شمېر,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارډ> axis_ریکارډونه) ثابت
  {
    TRACE_SUBSET (هذا);

    خپلکاری axis_ارزښت_Offsets = as_array (axis_ارزښت_شمېر);
    شمېر = 0;
    لكل (ثابت خپلکاری& دننه_واټن : axis_ارزښت_Offsets)
    {
      إذا (!دننه_واټن) استمر;
      خپلکاری o_snap = c->serializer->snapshot ();
      خپلکاری *o = c->serializer->embed (دننه_واټن);
      إذا (!o) return_trace (خطأ);
      إذا (!o->serialize_subset (c, دننه_واټن, هذا, axis_ریکارډونه))
      {
        c->serializer->revert (o_snap);
        استمر;
      }
      شمېر++;
    }

    return_trace (شمېر);
  }
};

بنية STAT
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_STAT;

  منطقي has_معلومات () ثابت { أرجع نسخه.منزل_int (); }

  منطقي get_ارزښت (hb_نښان_t نښان_2, عائم *value) ثابت
  {
    بینښه صحيح axis_شاخص;
    إذا (!get_design_axes ().lfind (نښان_2, &axis_شاخص)) أرجع خطأ;

    hb_array_t<ثابت Offset16_منزل<Axis_ارزښت>> axis_ارزښتونه = get_axis_ارزښت_offsets ();
    لكل (بینښه صحيح i = 0; i < axis_ارزښتونه.اوږدتیا; i++)
    {
      ثابت Axis_ارزښت& axis_ارزښت = هذا+دننه_واټن_منزل_Axis_ارزښت_Offsets+axis_ارزښتونه[i];
      إذا (axis_ارزښت.get_axis_شاخص () == axis_شاخص)
      {
	إذا (value)
	  *value = axis_ارزښت.get_ارزښت (axis_شاخص);
	أرجع ریښتیا;
      }
    }
    أرجع خطأ;
  }

  بینښه get_design_axis_شمېر () ثابت { أرجع design_Axis_شمېر; }

  hb_ot_name_id_t get_axis_ریکارډ_name_id (بینښه axis_ریکارډ_شاخص) ثابت
  {
    إذا (unlikely (axis_ریکارډ_شاخص >= design_Axis_شمېر)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت Stat_Axis_ریکارډ &axis_ریکارډ = get_design_axes ()[axis_ریکارډ_شاخص];
    أرجع axis_ریکارډ.get_name_id ();
  }

  بینښه get_axis_ارزښت_شمېر () ثابت { أرجع axis_ارزښت_شمېر; }

  hb_ot_name_id_t get_axis_ارزښت_name_id (بینښه axis_ارزښت_شاخص) ثابت
  {
    إذا (unlikely (axis_ارزښت_شاخص >= axis_ارزښت_شمېر)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت Axis_ارزښت &axis_ارزښت = (هذا + get_axis_ارزښت_offsets ()[axis_ارزښت_شاخص]);
    أرجع axis_ارزښت.get_ارزښت_name_id ();
  }

  فراغ collect_name_ids (hb_hashmap_t<hb_نښان_t, Triple> *user_axes_location,
                         hb_سېټ_t *nameids_منزل_retain /*بهر*/) ثابت
  {
    إذا (!has_معلومات ()) أرجع;

    + get_design_axes ()
    | hb_map (&Stat_Axis_ریکارډ::get_name_id)
    | hb_sink (nameids_منزل_retain)
    ;

    خپلکاری designAxes = get_design_axes ();

    + get_axis_ارزښت_offsets ()
    | hb_map (hb_جمع_کړه (&(هذا + دننه_واټن_منزل_Axis_ارزښت_Offsets)))
    | hb_filter ([&] (ثابت Axis_ارزښت& _)
                 { أرجع _.keep_axis_ارزښت (designAxes, user_axes_location); })
    | hb_map (&Axis_ارزښت::get_ارزښت_name_id)
    | hb_sink (nameids_منزل_retain)
    ;

    nameids_منزل_retain->جمع_کړه (elidedFallbackNameID);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    STAT *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    خپلکاری designAxes = get_design_axes ();
    لكل (بینښه i = 0; i < (بینښه)design_Axis_شمېر; i++)
      إذا (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (خطأ);

    إذا (design_Axis_شمېر)
      c->serializer->check_ټاکل (بهر->design_Axes_دننه_واټن, هذا->get_اندازه (),
                                   HB_SERIALIZE_ERROR_INT_ډکېدنه);

    بینښه شمېر = 0;
    بهر->دننه_واټن_منزل_Axis_ارزښت_Offsets.serialize_subset (c, دننه_واټن_منزل_Axis_ارزښت_Offsets, هذا,
                                                    axis_ارزښت_شمېر, شمېر, designAxes);
    return_trace (c->serializer->check_ټاکل (بهر->axis_ارزښت_شمېر, شمېر, HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخه.major == 1 &&
			  نسخه.minor > 0 &&
			  design_Axes_دننه_واټن.sanitize (c, هذا, design_Axis_شمېر) &&
			  دننه_واټن_منزل_Axis_ارزښت_Offsets.sanitize (c, هذا, axis_ارزښت_شمېر, &(هذا+دننه_واټن_منزل_Axis_ارزښت_Offsets))));
  }

  محمي:
  hb_array_t<ثابت Stat_Axis_ریکارډ> ثابت get_design_axes () ثابت
  { أرجع (هذا+design_Axes_دننه_واټن).as_array (design_Axis_شمېر); }

  hb_array_t<ثابت Offset16_منزل<Axis_ارزښت>> ثابت get_axis_ارزښت_offsets () ثابت
  { أرجع (هذا+دننه_واټن_منزل_Axis_ارزښت_Offsets).as_array (axis_ارزښت_شمېر); }


  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	design_Axis_اندازه;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_شمېر;/* The number of design axis records. In a
				 * font with an 'fvar' table, this value must be
				 * greater than or equal to the axisCount value
				 * in the 'fvar' table. In all fonts, must
				 * be greater than zero if axisValueCount
				 * is greater than zero. */
  NNOffset32_منزل<Unsized_Array_د<Stat_Axis_ریکارډ>>
		design_Axes_دننه_واټن;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes array. If designAxisCount is zero,
				 * set to zero; if designAxisCount is greater
				 * than zero, must be greater than zero. */
  HBUINT16	axis_ارزښت_شمېر;	/* The number of axis value tables. */
  NNOffset32_منزل<Axis_ارزښت_دننه_واټن_Array>
		دننه_واټن_منزل_Axis_ارزښت_Offsets;
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


#د_که_پای /* HB_OT_STAT_TABLE_HH */
