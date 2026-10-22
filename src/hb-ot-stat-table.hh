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

#جے_بنا_تعریف HB_OT_STAT_جدول_HH
#تعریف_کرو HB_OT_STAT_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#تعریف_کرو HB_OT_ٹیگ_STAT HB_ٹیگ('S','T','A','T')


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

ساكن منطقي axis_قدر_is_outside_axis_range (hb_ٹیگ_t axis_ٹیگ_2, عائم axis_قدر,
                                              ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> *user_axes_location)
{
  إذا (!user_axes_location->has (axis_ٹیگ_2))
    أرجع خطأ;

  Triple axis_range = user_axes_location->get (axis_ٹیگ_2);
  أرجع (axis_قدر < axis_range.minimum || axis_قدر > axis_range.ودھ_توں_ودھ);
}

بنية Stat_Axis_ریکارڈ
{
  صحيح cmp (hb_ٹیگ_t key) ثابت { أرجع ٹیگ_2.cmp (key); }

  hb_ot_name_id_t get_name_id () ثابت { أرجع nameID; }

  hb_ٹیگ_t get_axis_ٹیگ () ثابت { أرجع ٹیگ_2; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا)));
  }

  محمي:
  ٹیگ		ٹیگ_2;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية Axis_قدر_Format1
{
  بنانشانی صحيح get_axis_اشاریہ () ثابت { أرجع axis_اشاریہ_2; }
  عائم get_قدر ()             ثابت { أرجع value.منزل_float (); }

  hb_ot_name_id_t get_قدر_name_id () ثابت { أرجع قدر_Name_ID; }

  hb_ٹیگ_t get_axis_ٹیگ (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں) ثابت
  {
    بنانشانی axis_idx = get_axis_اشاریہ ();
    أرجع axis_ریکارڈاں[axis_idx].get_axis_ٹیگ ();
  }

  منطقي keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں,
                        ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> *user_axes_location) ثابت
  {
    hb_ٹیگ_t axis_ٹیگ_2 = get_axis_ٹیگ (axis_ریکارڈاں);
    عائم axis_قدر = get_قدر ();

    أرجع !axis_قدر_is_outside_axis_range (axis_ٹیگ_2, axis_قدر, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_قدر (axis_ریکارڈاں, user_axes_location))
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
  HBUINT16	axis_اشاریہ_2;	/* Zero-base index into the axis record array
				 * identifying the axis of design variation
				 * to which the axis value record applies.
				 * Must be less than designAxisCount. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	قدر_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عام:
  DEFINE_ناپ_STATIC (12);
};

بنية Axis_قدر_Format2
{
  بنانشانی صحيح get_axis_اشاریہ () ثابت { أرجع axis_اشاریہ_2; }
  عائم get_قدر ()             ثابت { أرجع nominal_قدر.منزل_float (); }

  hb_ot_name_id_t get_قدر_name_id () ثابت { أرجع قدر_Name_ID; }

  hb_ٹیگ_t get_axis_ٹیگ (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں) ثابت
  {
    بنانشانی axis_idx = get_axis_اشاریہ ();
    أرجع axis_ریکارڈاں[axis_idx].get_axis_ٹیگ ();
  }

  منطقي keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں,
                        ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> *user_axes_location) ثابت
  {
    hb_ٹیگ_t axis_ٹیگ_2 = get_axis_ٹیگ (axis_ریکارڈاں);
    عائم axis_قدر = get_قدر ();

    أرجع !axis_قدر_is_outside_axis_range (axis_ٹیگ_2, axis_قدر, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_قدر (axis_ریکارڈاں, user_axes_location))
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
  HBUINT16	axis_اشاریہ_2;	/* Zero-base index into the axis record array
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
  DEFINE_ناپ_STATIC (20);
};

بنية Axis_قدر_Format3
{
  بنانشانی صحيح get_axis_اشاریہ () ثابت { أرجع axis_اشاریہ_2; }
  عائم get_قدر ()             ثابت { أرجع value.منزل_float (); }

  hb_ot_name_id_t get_قدر_name_id () ثابت { أرجع قدر_Name_ID; }

  hb_ٹیگ_t get_axis_ٹیگ (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں) ثابت
  {
    بنانشانی axis_idx = get_axis_اشاریہ ();
    أرجع axis_ریکارڈاں[axis_idx].get_axis_ٹیگ ();
  }

  منطقي keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں,
                        ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> *user_axes_location) ثابت
  {
    hb_ٹیگ_t axis_ٹیگ_2 = get_axis_ٹیگ (axis_ریکارڈاں);
    عائم axis_قدر = get_قدر ();

    أرجع !axis_قدر_is_outside_axis_range (axis_ٹیگ_2, axis_قدر, user_axes_location);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    إذا (keep_axis_قدر (axis_ریکارڈاں, user_axes_location))
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
  HBUINT16	axis_اشاریہ_2;	/* Zero-base index into the axis record array
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
  DEFINE_ناپ_STATIC (16);
};

بنية Axis_قدر_ریکارڈ
{
  بنانشانی صحيح get_axis_اشاریہ () ثابت { أرجع axis_اشاریہ_2; }
  عائم get_قدر ()             ثابت { أرجع value.منزل_float (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	axis_اشاریہ_2;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عام:
  DEFINE_ناپ_STATIC (6);
};

بنية Axis_قدر_Format4
{
  ثابت Axis_قدر_ریکارڈ &get_axis_ریکارڈ (بنانشانی صحيح axis_اشاریہ) ثابت
  { أرجع axis_اقدار_2.as_array (axis_گنتی_2)[axis_اشاریہ]; }

  منطقي keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں,
                        ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> *user_axes_location) ثابت
  {
    hb_array_t<ثابت Axis_قدر_ریکارڈ> axis_قدر_ریکارڈاں = axis_اقدار_2.as_array (axis_گنتی_2);

    لكل (ثابت خودکار& rec : axis_قدر_ریکارڈاں)
    {
      بنانشانی axis_idx = rec.get_axis_اشاریہ ();
      عائم axis_قدر = rec.get_قدر ();
      hb_ٹیگ_t axis_ٹیگ_2 = axis_ریکارڈاں[axis_idx].get_axis_ٹیگ ();

      إذا (axis_قدر_is_outside_axis_range (axis_ٹیگ_2, axis_قدر, user_axes_location))
        أرجع خطأ;
    }

    أرجع سچ;
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    إذا (!keep_axis_قدر (axis_ریکارڈاں, user_axes_location))
      return_trace (خطأ);

    بنانشانی کل_ناپ = min_ناپ + axis_گنتی_2 * Axis_قدر_ریکارڈ::static_ناپ;
    خودکار *باہرلا = c->serializer->allocate_ناپ<Axis_قدر_Format4> (کل_ناپ);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);
    hb_memcpy (باہرلا, هذا, کل_ناپ);
    return_trace (سچ);
  }

  hb_ot_name_id_t get_قدر_name_id () ثابت { أرجع قدر_Name_ID; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
                          axis_اقدار_2.sanitize (c, axis_گنتی_2)));
  }

  محمي:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_گنتی_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	قدر_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  Unsized_Array_دا<Axis_قدر_ریکارڈ>
		axis_اقدار_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  عام:
  DEFINE_ناپ_ARRAY (8, axis_اقدار_2);
};

بنية Axis_قدر
{
  عائم get_قدر (بنانشانی صحيح axis_اشاریہ) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_قدر ();
    حالة 2: hb_barrier (); أرجع u.format2.get_قدر ();
    حالة 3: hb_barrier (); أرجع u.format3.get_قدر ();
    حالة 4: hb_barrier (); أرجع u.format4.get_axis_ریکارڈ (axis_اشاریہ).get_قدر ();
    افتراضي:أرجع 0.f;
    }
  }

  بنانشانی صحيح get_axis_اشاریہ () ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.get_axis_اشاریہ ();
    حالة 2: hb_barrier (); أرجع u.format2.get_axis_اشاریہ ();
    حالة 3: hb_barrier (); أرجع u.format3.get_axis_اشاریہ ();
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

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
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

  منطقي keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں,
                        hb_hashmap_t<hb_ٹیگ_t, Triple> *user_axes_location) ثابت
  {
    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); أرجع u.format1.keep_axis_قدر (axis_ریکارڈاں, user_axes_location);
    حالة 2: hb_barrier (); أرجع u.format2.keep_axis_قدر (axis_ریکارڈاں, user_axes_location);
    حالة 3: hb_barrier (); أرجع u.format3.keep_axis_قدر (axis_ریکارڈاں, user_axes_location);
    حالة 4: hb_barrier (); أرجع u.format4.keep_axis_قدر (axis_ریکارڈاں, user_axes_location);
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
  DEFINE_ناپ_UNION (2, format.v);
};

بنية Axis_قدر_وتھ_Array: Unsized_Array_دا<Offset16_منزل<Axis_قدر>>
{
  منطقي subset (hb_subset_context_t *c,
               بنانشانی axis_قدر_گنتی,
               بنانشانی& گنتی,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_ریکارڈاں) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار axis_قدر_Offsets = as_array (axis_قدر_گنتی);
    گنتی = 0;
    لكل (ثابت خودکار& وتھ : axis_قدر_Offsets)
    {
      إذا (!وتھ) استمر;
      خودکار o_snap = c->serializer->snapshot ();
      خودکار *o = c->serializer->embed (وتھ);
      إذا (!o) return_trace (خطأ);
      إذا (!o->serialize_subset (c, وتھ, هذا, axis_ریکارڈاں))
      {
        c->serializer->revert (o_snap);
        استمر;
      }
      گنتی++;
    }

    return_trace (گنتی);
  }
};

بنية STAT
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_STAT;

  منطقي has_انکڑے () ثابت { أرجع نسخہ.منزل_int (); }

  منطقي get_قدر (hb_ٹیگ_t ٹیگ_2, عائم *value) ثابت
  {
    بنانشانی صحيح axis_اشاریہ;
    إذا (!get_design_axes ().lfind (ٹیگ_2, &axis_اشاریہ)) أرجع خطأ;

    hb_array_t<ثابت Offset16_منزل<Axis_قدر>> axis_اقدار = get_axis_قدر_offsets ();
    لكل (بنانشانی صحيح i = 0; i < axis_اقدار.لمائی; i++)
    {
      ثابت Axis_قدر& axis_قدر = هذا+وتھ_منزل_Axis_قدر_Offsets+axis_اقدار[i];
      إذا (axis_قدر.get_axis_اشاریہ () == axis_اشاریہ)
      {
	إذا (value)
	  *value = axis_قدر.get_قدر (axis_اشاریہ);
	أرجع سچ;
      }
    }
    أرجع خطأ;
  }

  بنانشانی get_design_axis_گنتی () ثابت { أرجع design_Axis_گنتی; }

  hb_ot_name_id_t get_axis_ریکارڈ_name_id (بنانشانی axis_ریکارڈ_اشاریہ) ثابت
  {
    إذا (unlikely (axis_ریکارڈ_اشاریہ >= design_Axis_گنتی)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت Stat_Axis_ریکارڈ &axis_ریکارڈ = get_design_axes ()[axis_ریکارڈ_اشاریہ];
    أرجع axis_ریکارڈ.get_name_id ();
  }

  بنانشانی get_axis_قدر_گنتی () ثابت { أرجع axis_قدر_گنتی; }

  hb_ot_name_id_t get_axis_قدر_name_id (بنانشانی axis_قدر_اشاریہ) ثابت
  {
    إذا (unlikely (axis_قدر_اشاریہ >= axis_قدر_گنتی)) أرجع HB_OT_NAME_ID_INVALID;
    ثابت Axis_قدر &axis_قدر = (هذا + get_axis_قدر_offsets ()[axis_قدر_اشاریہ]);
    أرجع axis_قدر.get_قدر_name_id ();
  }

  فراغ collect_name_ids (hb_hashmap_t<hb_ٹیگ_t, Triple> *user_axes_location,
                         hb_سیٹ_t *nameids_منزل_retain /*باہرلا*/) ثابت
  {
    إذا (!has_انکڑے ()) أرجع;

    + get_design_axes ()
    | hb_map (&Stat_Axis_ریکارڈ::get_name_id)
    | hb_sink (nameids_منزل_retain)
    ;

    خودکار designAxes = get_design_axes ();

    + get_axis_قدر_offsets ()
    | hb_map (hb_جوڑو (&(هذا + وتھ_منزل_Axis_قدر_Offsets)))
    | hb_filter ([&] (ثابت Axis_قدر& _)
                 { أرجع _.keep_axis_قدر (designAxes, user_axes_location); })
    | hb_map (&Axis_قدر::get_قدر_name_id)
    | hb_sink (nameids_منزل_retain)
    ;

    nameids_منزل_retain->جوڑو (elidedFallbackNameID);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    STAT *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    خودکار designAxes = get_design_axes ();
    لكل (بنانشانی i = 0; i < (بنانشانی)design_Axis_گنتی; i++)
      إذا (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (خطأ);

    إذا (design_Axis_گنتی)
      c->serializer->check_سونپو (باہرلا->design_Axes_وتھ, هذا->get_ناپ (),
                                   HB_SERIALIZE_ERROR_INT_حد_توں_ودھ);

    بنانشانی گنتی = 0;
    باہرلا->وتھ_منزل_Axis_قدر_Offsets.serialize_subset (c, وتھ_منزل_Axis_قدر_Offsets, هذا,
                                                    axis_قدر_گنتی, گنتی, designAxes);
    return_trace (c->serializer->check_سونپو (باہرلا->axis_قدر_گنتی, گنتی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخہ.major == 1 &&
			  نسخہ.minor > 0 &&
			  design_Axes_وتھ.sanitize (c, هذا, design_Axis_گنتی) &&
			  وتھ_منزل_Axis_قدر_Offsets.sanitize (c, هذا, axis_قدر_گنتی, &(هذا+وتھ_منزل_Axis_قدر_Offsets))));
  }

  محمي:
  hb_array_t<ثابت Stat_Axis_ریکارڈ> ثابت get_design_axes () ثابت
  { أرجع (هذا+design_Axes_وتھ).as_array (design_Axis_گنتی); }

  hb_array_t<ثابت Offset16_منزل<Axis_قدر>> ثابت get_axis_قدر_offsets () ثابت
  { أرجع (هذا+وتھ_منزل_Axis_قدر_Offsets).as_array (axis_قدر_گنتی); }


  محمي:
  Fixed_نسخہ<>نسخہ;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	design_Axis_ناپ;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_گنتی;/* The number of design axis records. In a
				 * font with an 'fvar' table, this value must be
				 * greater than or equal to the axisCount value
				 * in the 'fvar' table. In all fonts, must
				 * be greater than zero if axisValueCount
				 * is greater than zero. */
  NNOffset32_منزل<Unsized_Array_دا<Stat_Axis_ریکارڈ>>
		design_Axes_وتھ;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes array. If designAxisCount is zero,
				 * set to zero; if designAxisCount is greater
				 * than zero, must be greater than zero. */
  HBUINT16	axis_قدر_گنتی;	/* The number of axis value tables. */
  NNOffset32_منزل<Axis_قدر_وتھ_Array>
		وتھ_منزل_Axis_قدر_Offsets;
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
  DEFINE_ناپ_STATIC (20);
};


} /* namespace OT */


#جے_انت /* HB_OT_STAT_TABLE_HH */
