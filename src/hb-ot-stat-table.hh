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

#اگر_غیر_تعریف_شدہ HB_OT_STAT_جدول_HH
#تعریف_کرو HB_OT_STAT_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-layout-common.hh"

/*
 * STAT -- Style Attributes
 * https://docs.microsoft.com/en-us/typography/opentype/spec/stat
 */
#تعریف_کرو HB_OT_لیبل_STAT HB_لیبل('S','T','A','T')


نامخلا OT {

شمار
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

ساکن منطقی axis_قدر_is_outside_axis_range (hb_لیبل_t axis_لیبل_2, اعشاری axis_قدر,
                                              ثابت hb_hashmap_t<hb_لیبل_t, Triple> *user_axes_location)
{
  اگر (!user_axes_location->has (axis_لیبل_2))
    واپس جھوٹ;

  Triple axis_range = user_axes_location->get (axis_لیبل_2);
  واپس (axis_قدر < axis_range.minimum || axis_قدر > axis_range.زیادہ_سے_زیادہ);
}

ساخت Stat_Axis_ریکارڈ
{
  صحیح cmp (hb_لیبل_t key) ثابت { واپس لیبل_2.cmp (key); }

  hb_ot_name_id_t get_name_id () ثابت { واپس nameID; }

  hb_لیبل_t get_axis_لیبل () ثابت { واپس لیبل_2; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ)));
  }

  محفوظ:
  لیبل		لیبل_2;		/* A tag identifying the axis of design variation. */
  NameID	nameID;		/* The name ID for entries in the 'name' table that
				 * provide a display string for this axis. */
  HBUINT16	ordering;	/* A value that applications can use to determine
				 * primary sorting of face names, or for ordering
				 * of descriptors when composing family or face names. */
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت Axis_قدر_Format1
{
  غیرعلامتی صحیح get_axis_اشاریہ () ثابت { واپس axis_اشاریہ_2; }
  اعشاری get_قدر ()             ثابت { واپس value.مقصد_float (); }

  hb_ot_name_id_t get_قدر_name_id () ثابت { واپس قدر_Name_ID; }

  hb_لیبل_t get_axis_لیبل (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات) ثابت
  {
    غیرعلامتی axis_idx = get_axis_اشاریہ ();
    واپس axis_اندراجات[axis_idx].get_axis_لیبل ();
  }

  منطقی keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات,
                        ثابت hb_hashmap_t<hb_لیبل_t, Triple> *user_axes_location) ثابت
  {
    hb_لیبل_t axis_لیبل_2 = get_axis_لیبل (axis_اندراجات);
    اعشاری axis_قدر = get_قدر ();

    واپس !axis_قدر_is_outside_axis_range (axis_لیبل_2, axis_قدر, user_axes_location);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_hashmap_t<hb_لیبل_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    اگر (keep_axis_قدر (axis_اندراجات, user_axes_location))
      return_trace (c->serializer->embed (یہ));

    return_trace (جھوٹ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
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
  عوامی:
  DEFINE_سائز_STATIC (12);
};

ساخت Axis_قدر_Format2
{
  غیرعلامتی صحیح get_axis_اشاریہ () ثابت { واپس axis_اشاریہ_2; }
  اعشاری get_قدر ()             ثابت { واپس nominal_قدر.مقصد_float (); }

  hb_ot_name_id_t get_قدر_name_id () ثابت { واپس قدر_Name_ID; }

  hb_لیبل_t get_axis_لیبل (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات) ثابت
  {
    غیرعلامتی axis_idx = get_axis_اشاریہ ();
    واپس axis_اندراجات[axis_idx].get_axis_لیبل ();
  }

  منطقی keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات,
                        ثابت hb_hashmap_t<hb_لیبل_t, Triple> *user_axes_location) ثابت
  {
    hb_لیبل_t axis_لیبل_2 = get_axis_لیبل (axis_اندراجات);
    اعشاری axis_قدر = get_قدر ();

    واپس !axis_قدر_is_outside_axis_range (axis_لیبل_2, axis_قدر, user_axes_location);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_hashmap_t<hb_لیبل_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    اگر (keep_axis_قدر (axis_اندراجات, user_axes_location))
      return_trace (c->serializer->embed (یہ));

    return_trace (جھوٹ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
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
  عوامی:
  DEFINE_سائز_STATIC (20);
};

ساخت Axis_قدر_Format3
{
  غیرعلامتی صحیح get_axis_اشاریہ () ثابت { واپس axis_اشاریہ_2; }
  اعشاری get_قدر ()             ثابت { واپس value.مقصد_float (); }

  hb_ot_name_id_t get_قدر_name_id () ثابت { واپس قدر_Name_ID; }

  hb_لیبل_t get_axis_لیبل (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات) ثابت
  {
    غیرعلامتی axis_idx = get_axis_اشاریہ ();
    واپس axis_اندراجات[axis_idx].get_axis_لیبل ();
  }

  منطقی keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات,
                        ثابت hb_hashmap_t<hb_لیبل_t, Triple> *user_axes_location) ثابت
  {
    hb_لیبل_t axis_لیبل_2 = get_axis_لیبل (axis_اندراجات);
    اعشاری axis_قدر = get_قدر ();

    واپس !axis_قدر_is_outside_axis_range (axis_لیبل_2, axis_قدر, user_axes_location);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_hashmap_t<hb_لیبل_t, Triple>* user_axes_location = &c->plan->user_axes_location;

    اگر (keep_axis_قدر (axis_اندراجات, user_axes_location))
      return_trace (c->serializer->embed (یہ));

    return_trace (جھوٹ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
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
  عوامی:
  DEFINE_سائز_STATIC (16);
};

ساخت Axis_قدر_ریکارڈ
{
  غیرعلامتی صحیح get_axis_اشاریہ () ثابت { واپس axis_اشاریہ_2; }
  اعشاری get_قدر ()             ثابت { واپس value.مقصد_float (); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  HBUINT16	axis_اشاریہ_2;	/* Zero-base index into the axis record array
				 * identifying the axis to which this value
				 * applies. Must be less than designAxisCount. */
  F16DOT16	value;		/* A numeric value for this attribute value. */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت Axis_قدر_Format4
{
  ثابت Axis_قدر_ریکارڈ &get_axis_ریکارڈ (غیرعلامتی صحیح axis_اشاریہ) ثابت
  { واپس axis_اقدار_2.as_array (axis_گنتی_2)[axis_اشاریہ]; }

  منطقی keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات,
                        ثابت hb_hashmap_t<hb_لیبل_t, Triple> *user_axes_location) ثابت
  {
    hb_array_t<ثابت Axis_قدر_ریکارڈ> axis_قدر_اندراجات = axis_اقدار_2.as_array (axis_گنتی_2);

    برائے (ثابت خودکار& rec : axis_قدر_اندراجات)
    {
      غیرعلامتی axis_idx = rec.get_axis_اشاریہ ();
      اعشاری axis_قدر = rec.get_قدر ();
      hb_لیبل_t axis_لیبل_2 = axis_اندراجات[axis_idx].get_axis_لیبل ();

      اگر (axis_قدر_is_outside_axis_range (axis_لیبل_2, axis_قدر, user_axes_location))
        واپس جھوٹ;
    }

    واپس سچ;
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_hashmap_t<hb_لیبل_t, Triple> *user_axes_location = &c->plan->user_axes_location;
    اگر (!keep_axis_قدر (axis_اندراجات, user_axes_location))
      return_trace (جھوٹ);

    غیرعلامتی کل_سائز = min_سائز + axis_گنتی_2 * Axis_قدر_ریکارڈ::static_سائز;
    خودکار *برآمد = c->serializer->allocate_سائز<Axis_قدر_Format4> (کل_سائز);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);
    hb_memcpy (برآمد, یہ, کل_سائز);
    return_trace (سچ);
  }

  hb_ot_name_id_t get_قدر_name_id () ثابت { واپس قدر_Name_ID; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
                          axis_اقدار_2.sanitize (c, axis_گنتی_2)));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier — set to 4. */
  HBUINT16	axis_گنتی_2;	/* The total number of axes contributing to
				 * this axis-values combination. */
  HBUINT16	flags;		/* Flags — see below for details. */
  NameID	قدر_Name_ID;	/* The name ID for entries in the 'name' table
				 * that provide a display string for this
				 * attribute value. */
  Unsized_Array_کا<Axis_قدر_ریکارڈ>
		axis_اقدار_2;	/* Array of AxisValue records that provide the
				 * combination of axis values, one for each
				 * contributing axis. */
  عوامی:
  DEFINE_سائز_ARRAY (8, axis_اقدار_2);
};

ساخت Axis_قدر
{
  اعشاری get_قدر (غیرعلامتی صحیح axis_اشاریہ) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); واپس u.format1.get_قدر ();
    حالت 2: hb_barrier (); واپس u.format2.get_قدر ();
    حالت 3: hb_barrier (); واپس u.format3.get_قدر ();
    حالت 4: hb_barrier (); واپس u.format4.get_axis_ریکارڈ (axis_اشاریہ).get_قدر ();
    طےشدہ:واپس 0.f;
    }
  }

  غیرعلامتی صحیح get_axis_اشاریہ () ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); واپس u.format1.get_axis_اشاریہ ();
    حالت 2: hb_barrier (); واپس u.format2.get_axis_اشاریہ ();
    حالت 3: hb_barrier (); واپس u.format3.get_axis_اشاریہ ();
    /* case 4: Makes more sense for variable fonts which are handled by fvar in hb-style */
    طےشدہ:واپس -1;
    }
  }

  hb_ot_name_id_t get_قدر_name_id () ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); واپس u.format1.get_قدر_name_id ();
    حالت 2: hb_barrier (); واپس u.format2.get_قدر_name_id ();
    حالت 3: hb_barrier (); واپس u.format3.get_قدر_name_id ();
    حالت 4: hb_barrier (); واپس u.format4.get_قدر_name_id ();
    طےشدہ:واپس HB_OT_NAME_ID_INVALID;
    }
  }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  منطقی keep_axis_قدر (ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات,
                        hb_hashmap_t<hb_لیبل_t, Triple> *user_axes_location) ثابت
  {
    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); واپس u.format1.keep_axis_قدر (axis_اندراجات, user_axes_location);
    حالت 2: hb_barrier (); واپس u.format2.keep_axis_قدر (axis_اندراجات, user_axes_location);
    حالت 3: hb_barrier (); واپس u.format3.keep_axis_قدر (axis_اندراجات, user_axes_location);
    حالت 4: hb_barrier (); واپس u.format4.keep_axis_قدر (axis_اندراجات, user_axes_location);
    طےشدہ:واپس جھوٹ;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ)))
      return_trace (جھوٹ);
    hb_barrier ();

    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    طےشدہ:return_trace (سچ);
    }
  }

  محفوظ:
  اتحاد
  {
  ساخت { HBUINT16 v; }	format;
  Axis_قدر_Format1	format1;
  Axis_قدر_Format2	format2;
  Axis_قدر_Format3	format3;
  Axis_قدر_Format4	format4;
  } u;
  عوامی:
  DEFINE_سائز_UNION (2, format.v);
};

ساخت Axis_قدر_انحراف_Array: Unsized_Array_کا<Offset16_مقصد<Axis_قدر>>
{
  منطقی subset (hb_subset_context_t *c,
               غیرعلامتی axis_قدر_گنتی,
               غیرعلامتی& گنتی,
               ثابت hb_array_t<ثابت Stat_Axis_ریکارڈ> axis_اندراجات) ثابت
  {
    TRACE_SUBSET (یہ);

    خودکار axis_قدر_Offsets = as_array (axis_قدر_گنتی);
    گنتی = 0;
    برائے (ثابت خودکار& انحراف : axis_قدر_Offsets)
    {
      اگر (!انحراف) جاری;
      خودکار o_snap = c->serializer->snapshot ();
      خودکار *o = c->serializer->embed (انحراف);
      اگر (!o) return_trace (جھوٹ);
      اگر (!o->serialize_subset (c, انحراف, یہ, axis_اندراجات))
      {
        c->serializer->revert (o_snap);
        جاری;
      }
      گنتی++;
    }

    return_trace (گنتی);
  }
};

ساخت STAT
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_STAT;

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.مقصد_int (); }

  منطقی get_قدر (hb_لیبل_t لیبل_2, اعشاری *value) ثابت
  {
    غیرعلامتی صحیح axis_اشاریہ;
    اگر (!get_design_axes ().lfind (لیبل_2, &axis_اشاریہ)) واپس جھوٹ;

    hb_array_t<ثابت Offset16_مقصد<Axis_قدر>> axis_اقدار = get_axis_قدر_offsets ();
    برائے (غیرعلامتی صحیح i = 0; i < axis_اقدار.لمبائی; i++)
    {
      ثابت Axis_قدر& axis_قدر = یہ+انحراف_مقصد_Axis_قدر_Offsets+axis_اقدار[i];
      اگر (axis_قدر.get_axis_اشاریہ () == axis_اشاریہ)
      {
	اگر (value)
	  *value = axis_قدر.get_قدر (axis_اشاریہ);
	واپس سچ;
      }
    }
    واپس جھوٹ;
  }

  غیرعلامتی get_design_axis_گنتی () ثابت { واپس design_Axis_گنتی; }

  hb_ot_name_id_t get_axis_ریکارڈ_name_id (غیرعلامتی axis_ریکارڈ_اشاریہ) ثابت
  {
    اگر (unlikely (axis_ریکارڈ_اشاریہ >= design_Axis_گنتی)) واپس HB_OT_NAME_ID_INVALID;
    ثابت Stat_Axis_ریکارڈ &axis_ریکارڈ = get_design_axes ()[axis_ریکارڈ_اشاریہ];
    واپس axis_ریکارڈ.get_name_id ();
  }

  غیرعلامتی get_axis_قدر_گنتی () ثابت { واپس axis_قدر_گنتی; }

  hb_ot_name_id_t get_axis_قدر_name_id (غیرعلامتی axis_قدر_اشاریہ) ثابت
  {
    اگر (unlikely (axis_قدر_اشاریہ >= axis_قدر_گنتی)) واپس HB_OT_NAME_ID_INVALID;
    ثابت Axis_قدر &axis_قدر = (یہ + get_axis_قدر_offsets ()[axis_قدر_اشاریہ]);
    واپس axis_قدر.get_قدر_name_id ();
  }

  خالی collect_name_ids (hb_hashmap_t<hb_لیبل_t, Triple> *user_axes_location,
                         hb_سیٹ_t *nameids_مقصد_retain /*برآمد*/) ثابت
  {
    اگر (!has_ڈیٹا ()) واپس;

    + get_design_axes ()
    | hb_map (&Stat_Axis_ریکارڈ::get_name_id)
    | hb_sink (nameids_مقصد_retain)
    ;

    خودکار designAxes = get_design_axes ();

    + get_axis_قدر_offsets ()
    | hb_map (hb_جوڑو (&(یہ + انحراف_مقصد_Axis_قدر_Offsets)))
    | hb_filter ([&] (ثابت Axis_قدر& _)
                 { واپس _.keep_axis_قدر (designAxes, user_axes_location); })
    | hb_map (&Axis_قدر::get_قدر_name_id)
    | hb_sink (nameids_مقصد_retain)
    ;

    nameids_مقصد_retain->جوڑو (elidedFallbackNameID);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    STAT *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    خودکار designAxes = get_design_axes ();
    برائے (غیرعلامتی i = 0; i < (غیرعلامتی)design_Axis_گنتی; i++)
      اگر (unlikely (!c->serializer->embed (designAxes[i])))
          return_trace (جھوٹ);

    اگر (design_Axis_گنتی)
      c->serializer->check_تفویض (برآمد->design_Axes_انحراف, یہ->get_سائز (),
                                   HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز);

    غیرعلامتی گنتی = 0;
    برآمد->انحراف_مقصد_Axis_قدر_Offsets.serialize_subset (c, انحراف_مقصد_Axis_قدر_Offsets, یہ,
                                                    axis_قدر_گنتی, گنتی, designAxes);
    return_trace (c->serializer->check_تفویض (برآمد->axis_قدر_گنتی, گنتی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  نسخہ.major == 1 &&
			  نسخہ.minor > 0 &&
			  design_Axes_انحراف.sanitize (c, یہ, design_Axis_گنتی) &&
			  انحراف_مقصد_Axis_قدر_Offsets.sanitize (c, یہ, axis_قدر_گنتی, &(یہ+انحراف_مقصد_Axis_قدر_Offsets))));
  }

  محفوظ:
  hb_array_t<ثابت Stat_Axis_ریکارڈ> ثابت get_design_axes () ثابت
  { واپس (یہ+design_Axes_انحراف).as_array (design_Axis_گنتی); }

  hb_array_t<ثابت Offset16_مقصد<Axis_قدر>> ثابت get_axis_قدر_offsets () ثابت
  { واپس (یہ+انحراف_مقصد_Axis_قدر_Offsets).as_array (axis_قدر_گنتی); }


  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version of the stat table
				 * initially set to 0x00010002u */
  HBUINT16	design_Axis_سائز;	/* The size in bytes of each axis record. */
  HBUINT16	design_Axis_گنتی;/* The number of design axis records. In a
				 * font with an 'fvar' table, this value must be
				 * greater than or equal to the axisCount value
				 * in the 'fvar' table. In all fonts, must
				 * be greater than zero if axisValueCount
				 * is greater than zero. */
  NNOffset32_مقصد<Unsized_Array_کا<Stat_Axis_ریکارڈ>>
		design_Axes_انحراف;
				/* Offset in bytes from the beginning of
				 * the STAT table to the start of the design
				 * axes array. If designAxisCount is zero,
				 * set to zero; if designAxisCount is greater
				 * than zero, must be greater than zero. */
  HBUINT16	axis_قدر_گنتی;	/* The number of axis value tables. */
  NNOffset32_مقصد<Axis_قدر_انحراف_Array>
		انحراف_مقصد_Axis_قدر_Offsets;
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
  عوامی:
  DEFINE_سائز_STATIC (20);
};


} /* namespace OT */


#اگر_ختم /* HB_OT_STAT_TABLE_HH */
