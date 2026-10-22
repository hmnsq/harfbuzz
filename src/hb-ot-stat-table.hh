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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_STAT_جدول_HH
#وصف_ڪريو HB_OT_STAT_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#وصف_ڪريو HB_OT_ٽيگ_STAT HB_ٽيگ('S','T','A','T')


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
  ELIDABLE_AXIS_قدر_NAME = 0x0002		/* If set, it indicates that the axis
						 * value represents the “normal” value
						 * for the axis and may be omitted when
						 * composing name strings. */
  // Reserved = 0xFFFC				/* Reserved for future use — set to zero. */
};

ساكن منطقي axis_قدر_is_outside_axis_range (hb_ٽيگ_t axis_ٽيگ_2, عائم axis_قدر,
                                              ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> *user_axes_location)
{
  إذا (!user_axes_location->has (axis_ٽيگ_2))
    أرجع خطأ;

  Triple axis_range = user_axes_location->get (axis_ٽيگ_2);
  أرجع (axis_قدر < axis_range.minimum || axis_قدر > axis_range.وڌ_کان_وڌ);
}

بنية Stat_Axis_رڪارڊ
{
  صحيح cmp (hb_ٽيگ_t key) ثابت { أرجع ٽيگ_2.cmp (key); }

  hb_ot_name_id_t get_name_id () ثابت { أرجع nameID; }

  hb_ٽيگ_t get_axis_ٽيگ () ثابت { أرجع ٽيگ_2; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا)));
  }

  محمي:
  ٽيگ		ٽيگ_2;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية Axis_قدر_Format1
{
  بنانشان صحيح get_axis_اشاريو () ثابت { أرجع axis_اشاريو_2; }
  عائم get_قدر ()             ثابت { أرجع value.منزل_float (); }

  hb_ot_name_id_t get_قدر_name_id () ثابت { أرجع قدر_Name_ID; }

  hb_ٽيگ_t get_axis_ٽيگ (ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون) ثابت
  {
    بنانشان axis_idx = get_axis_اشاريو ();
    أرجع axis_رڪارڊون[axis_idx].get_axis_ٽيگ ();
  }

  منطقي keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون,
                        ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> *user_axes_location) ثابت
  {
    hb_ٽيگ_t axis_ٽيگ_2 = get_axis_ٽيگ (axis_رڪارڊون);
    عائم axis_قدر = get_قدر ();

    أرجع !axis_قدر_is_outside_axis_range (axis_ٽيگ_2, axis_قدر, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_قدر (axis_رڪارڊون, user_axes_location))
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
  HBUINT16	axis_اشاريو_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	قدر_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عام:
  DEFINE_سائيز_STATIC (12);
};

بنية Axis_قدر_Format2
{
  بنانشان صحيح get_axis_اشاريو () ثابت { أرجع axis_اشاريو_2; }
  عائم get_قدر ()             ثابت { أرجع nominal_قدر.منزل_float (); }

  hb_ot_name_id_t get_قدر_name_id () ثابت { أرجع قدر_Name_ID; }

  hb_ٽيگ_t get_axis_ٽيگ (ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون) ثابت
  {
    بنانشان axis_idx = get_axis_اشاريو ();
    أرجع axis_رڪارڊون[axis_idx].get_axis_ٽيگ ();
  }

  منطقي keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون,
                        ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> *user_axes_location) ثابت
  {
    hb_ٽيگ_t axis_ٽيگ_2 = get_axis_ٽيگ (axis_رڪارڊون);
    عائم axis_قدر = get_قدر ();

    أرجع !axis_قدر_is_outside_axis_range (axis_ٽيگ_2, axis_قدر, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_قدر (axis_رڪارڊون, user_axes_location))
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
  HBUINT16	axis_اشاريو_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	قدر_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	nominal_قدر;	/* A numeric value for this attribute value. */
  F16DOT16	range_Min_قدر;	/* The minimum value for a range associated
				 * with the specified name ID. */
  F16DOT16	range_Max_قدر;	/* The maximum value for a range associated
				 * with the specified name ID. */
  عام:
  DEFINE_سائيز_STATIC (20);
};

بنية Axis_قدر_Format3
{
  بنانشان صحيح get_axis_اشاريو () ثابت { أرجع axis_اشاريو_2; }
  عائم get_قدر ()             ثابت { أرجع value.منزل_float (); }

  hb_ot_name_id_t get_قدر_name_id () ثابت { أرجع قدر_Name_ID; }

  hb_ٽيگ_t get_axis_ٽيگ (ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون) ثابت
  {
    بنانشان axis_idx = get_axis_اشاريو ();
    أرجع axis_رڪارڊون[axis_idx].get_axis_ٽيگ ();
  }

  منطقي keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون,
                        ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> *user_axes_location) ثابت
  {
    hb_ٽيگ_t axis_ٽيگ_2 = get_axis_ٽيگ (axis_رڪارڊون);
    عائم axis_قدر = get_قدر ();

    أرجع !axis_قدر_is_outside_axis_range (axis_ٽيگ_2, axis_قدر, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_قدر (axis_رڪارڊون, user_axes_location))
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
  HBUINT16	axis_اشاريو_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	قدر_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  F16DOT16	linked_قدر;	/* The numeric value for a style-linked mapping
				 * from this value. */
  عام:
  DEFINE_سائيز_STATIC (16);
};

بنية Axis_قدر_رڪارڊ
{
  بنانشان صحيح get_axis_اشاريو () ثابت { أرجع axis_اشاريو_2; }
  عائم get_قدر ()             ثابت { أرجع value.منزل_float (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	axis_اشاريو_2;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عام:
  DEFINE_سائيز_STATIC (6);
};

بنية Axis_قدر_Format4
{
  ثابت Axis_قدر_رڪارڊ &get_axis_رڪارڊ (بنانشان صحيح axis_اشاريو) ثابت
  { أرجع axis_قدرون_2.as_array (axis_ڳڻپ_2)[axis_اشاريو]; }

  منطقي keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون,
                        ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> *user_axes_location) ثابت
  {
    hb_array_t<ثابت Axis_قدر_رڪارڊ> axis_قدر_رڪارڊون = axis_قدرون_2.as_array (axis_ڳڻپ_2);

    لكل (ثابت خودڪار& rec : axis_قدر_رڪارڊون)
    {
      بنانشان axis_idx = rec.get_axis_اشاريو ();
      عائم axis_قدر = rec.get_قدر ();
      hb_ٽيگ_t axis_ٽيگ_2 = axis_رڪارڊون[axis_idx].get_axis_ٽيگ ();

      إذا (axis_قدر_is_outside_axis_range (axis_ٽيگ_2, axis_قدر, user_axes_location))
        أرجع خطأ;
    }

    أرجع سچ;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    إذا (!keep_axis_قدر (axis_رڪارڊون, user_axes_location))
      return_trace (خطأ);

    بنانشان ڪل_سائيز = min_سائيز + axis_ڳڻپ_2 * Axis_قدر_رڪارڊ::static_سائيز;
    خودڪار *ٻاهريون = c->serializer->allocate_سائيز<Axis_قدر_Format4> (ڪل_سائيز);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);
    hb_memcpy (ٻاهريون, هذا, ڪل_سائيز);
    return_trace (سچ);
  }

  hb_ot_name_id_t get_قدر_name_id () ثابت { أرجع قدر_Name_ID; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
                          axis_قدرون_2.sanitize (c, axis_ڳڻپ_2)));
  }

  محمي:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_ڳڻپ_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	قدر_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  Unsized_Array_جو<Axis_قدر_رڪارڊ>
		axis_قدرون_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  عام:
  DEFINE_سائيز_ARRAY (8, axis_قدرون_2);
};

بنية Axis_قدر
{
  عائم get_قدر (بنانشان صحيح axis_اشاريو) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_قدر ();
    حالة 2: hb_barrier (); أرجع u.format2.get_قدر ();
    حالة 3: hb_barrier (); أرجع u.format3.get_قدر ();
    حالة 4: hb_barrier (); أرجع u.format4.get_axis_رڪارڊ (axis_اشاريو).get_قدر ();
    افتراضي:أرجع 0.f;
    }
  }

  بنانشان صحيح get_axis_اشاريو () ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_axis_اشاريو ();
    حالة 2: hb_barrier (); أرجع u.format2.get_axis_اشاريو ();
    حالة 3: hb_barrier (); أرجع u.format3.get_axis_اشاريو ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    افتراضي:أرجع -1;
    }
  }

  hb_ot_name_id_t get_قدر_name_id () ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_قدر_name_id ();
    حالة 2: hb_barrier (); أرجع u.format2.get_قدر_name_id ();
    حالة 3: hb_barrier (); أرجع u.format3.get_قدر_name_id ();
    حالة 4: hb_barrier (); أرجع u.format4.get_قدر_name_id ();
    افتراضي:أرجع HB_OT_NAME_ID_INVALID;
    }
  }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  منطقي keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون,
                        hb_hashmap_t<hb_ٽيگ_t, Triple> *user_axes_location) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.keep_axis_قدر (axis_رڪارڊون, user_axes_location);
    حالة 2: hb_barrier (); أرجع u.format2.keep_axis_قدر (axis_رڪارڊون, user_axes_location);
    حالة 3: hb_barrier (); أرجع u.format3.keep_axis_قدر (axis_رڪارڊون, user_axes_location);
    حالة 4: hb_barrier (); أرجع u.format4.keep_axis_قدر (axis_رڪارڊون, user_axes_location);
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
    افتراضي:return_trace (سچ);
    }
  }

  محمي:
  اتحاد
  {
  بنية { HBUINT16 v; }	format;
  Axis_قدر_Format1	format1;
  Axis_قدر_Format2	format2;
  Axis_قدر_Format3	format3;
  Axis_قدر_Format4	format4;
  } u;
  عام:
  DEFINE_سائيز_UNION (2, format.v);
};

بنية Axis_قدر_وٿي_Array: Unsized_Array_جو<Offset16_منزل<Axis_قدر>>
{
  منطقي subset (hb_subset_context_t *c,
               بنانشان axis_قدر_ڳڻپ,
               بنانشان& ڳڻپ,
               ثابت hb_array_t<ثابت Stat_Axis_رڪارڊ> axis_رڪارڊون) ثابت
  {
    TRACE_SUBSET (هذا);

    خودڪار axis_قدر_Offsets = as_array (axis_قدر_ڳڻپ);
    ڳڻپ = 0;
    لكل (ثابت خودڪار& وٿي : axis_قدر_Offsets)
    {
      إذا (!وٿي) استمر;
      خودڪار o_snap = c->serializer->snapshot ();
      خودڪار *o = c->serializer->embed (وٿي);
      إذا (!o) return_trace (خطأ);
      إذا (!o->serialize_subset (c, وٿي, هذا, axis_رڪارڊون))
      {
        c->serializer->revert (o_snap);
        استمر;
      }
      ڳڻپ++;
    }

    return_trace (ڳڻپ);
  }
};

بنية STAT
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_STAT;

  منطقي has_معلومات () ثابت { أرجع نسخو.منزل_int (); }

  منطقي get_قدر (hb_ٽيگ_t ٽيگ_2, عائم *value) ثابت
  {
    بنانشان صحيح axis_اشاريو;
    إذا (!get_design_axes ().lfind (ٽيگ_2, &axis_اشاريو)) أرجع خطأ;

    hb_array_t<ثابت Offset16_منزل<Axis_قدر>> axis_قدرون = get_axis_قدر_offsets ();
    لكل (بنانشان صحيح i = 0; i < axis_قدرون.ڊيگهائي; i++)
    {
      ثابت Axis_قدر& axis_قدر = هذا+وٿي_منزل_Axis_قدر_Offsets+axis_قدرون[i];
      إذا (axis_قدر.get_axis_اشاريو () == axis_اشاريو)
      {
	إذا (value)
	  *value = axis_قدر.get_قدر (axis_اشاريو);
	أرجع سچ;
      }
    }
    أرجع خطأ;
  }

  بنانشان get_design_axis_ڳڻپ () ثابت { أرجع design_Axis_ڳڻپ; }

  hb_ot_name_id_t get_axis_رڪارڊ_name_id (بنانشان axis_رڪارڊ_اشاريو) ثابت
  {
    إذا (unlikely (axis_رڪارڊ_اشاريو >= design_Axis_ڳڻپ)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت Stat_Axis_رڪارڊ &axis_رڪارڊ = get_design_axes ()[axis_رڪارڊ_اشاريو];
    أرجع axis_رڪارڊ.get_name_id ();
  }

  بنانشان get_axis_قدر_ڳڻپ () ثابت { أرجع axis_قدر_ڳڻپ; }

  hb_ot_name_id_t get_axis_قدر_name_id (بنانشان axis_قدر_اشاريو) ثابت
  {
    إذا (unlikely (axis_قدر_اشاريو >= axis_قدر_ڳڻپ)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت Axis_قدر &axis_قدر = (هذا + get_axis_قدر_offsets ()[axis_قدر_اشاريو]);
    أرجع axis_قدر.get_قدر_name_id ();
  }

  فراغ collect_name_ids (hb_hashmap_t<hb_ٽيگ_t, Triple> *user_axes_location,
                         hb_سيٽ_t *nameids_منزل_retain /*ٻاهريون*/) ثابت
  {
    إذا (!has_معلومات ()) أرجع;

    + get_design_axes ()
    | hb_map (&Stat_Axis_رڪارڊ::get_name_id)
    | hb_sink (nameids_منزل_retain)
    ;

    خودڪار designAxes = get_design_axes ();

    + get_axis_قدر_offsets ()
    | hb_map (hb_جوڙيو (&(هذا + وٿي_منزل_Axis_قدر_Offsets)))
    | hb_filter ([&] (ثابت Axis_قدر& _)
                 { أرجع _.keep_axis_قدر (designAxes, user_axes_location); })
    | hb_map (&Axis_قدر::get_قدر_name_id)
    | hb_sink (nameids_منزل_retain)
    ;

    nameids_منزل_retain->جوڙيو (elidedFallbackNameID);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    STAT *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    خودڪار designAxes = get_design_axes ();
    لكل (بنانشان i = 0; i < (بنانشان)design_Axis_ڳڻپ; i++)
      إذا (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (خطأ);

    إذا (design_Axis_ڳڻپ)
      c->serializer->check_سونپيو (ٻاهريون->design_Axes_وٿي, هذا->get_سائيز (),
                                   HB_SERIALIZE_ERROR_INT_حد_کان_مٿي);

    بنانشان ڳڻپ = 0;
    ٻاهريون->وٿي_منزل_Axis_قدر_Offsets.serialize_subset (c, وٿي_منزل_Axis_قدر_Offsets, هذا,
                                                    axis_قدر_ڳڻپ, ڳڻپ, designAxes);
    return_trace (c->serializer->check_سونپيو (ٻاهريون->axis_قدر_ڳڻپ, ڳڻپ, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخو.major == 1 &&
			  نسخو.minor > 0 &&
			  design_Axes_وٿي.sanitize (c, هذا, design_Axis_ڳڻپ) &&
			  وٿي_منزل_Axis_قدر_Offsets.sanitize (c, هذا, axis_قدر_ڳڻپ, &(هذا+وٿي_منزل_Axis_قدر_Offsets))));
  }

  محمي:
  hb_array_t<ثابت Stat_Axis_رڪارڊ> ثابت get_design_axes () ثابت
  { أرجع (هذا+design_Axes_وٿي).as_array (design_Axis_ڳڻپ); }

  hb_array_t<ثابت Offset16_منزل<Axis_قدر>> ثابت get_axis_قدر_offsets () ثابت
  { أرجع (هذا+وٿي_منزل_Axis_قدر_Offsets).as_array (axis_قدر_ڳڻپ); }


  محمي:
  Fixed_نسخو<>نسخو;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	design_Axis_سائيز;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_ڳڻپ;/* The number of design axis records. In a
				 * font with an 'fvar' table, this value must be
				 * greater than or equal to the axisCount value
				 * in the 'fvar' table. In all fonts, must
				 * be greater than zero if axisValueCount
				 * is greater than zero. */
  NNOffset32_منزل<Unsized_Array_جو<Stat_Axis_رڪارڊ>>
		design_Axes_وٿي;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes array. If designAxisCount is zero,
				 * set to zero; if designAxisCount is greater
				 * than zero, must be greater than zero. */
  HBUINT16	axis_قدر_ڳڻپ;	/* The number of axis value tables. */
  NNOffset32_منزل<Axis_قدر_وٿي_Array>
		وٿي_منزل_Axis_قدر_Offsets;
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
  DEFINE_سائيز_STATIC (20);
};


} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_STAT_TABLE_HH */
