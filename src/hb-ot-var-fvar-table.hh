/*
 * Copyright © 2017  Google, Inc.
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
 *
 * Google Author(s): Behdad Esfahbod
 */

#اگر_تعریف_نشده HB_OT_VAR_FVAR_جدول_HH
#تعریف_کن HB_OT_VAR_FVAR_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#تعریف_کن HB_OT_برچسب_fvar HB_برچسب('f','v','a','r')


نطاق OT {

ساكن منطقي axis_coord_pinned_or_within_axis_range (ثابت hb_array_t<ثابت F16DOT16> coords,
                                                    بیعلامت axis_شاخص,
                                                    Triple axis_حد)
{
  عائم axis_coord = coords[axis_شاخص].مقصد_float ();
  إذا (axis_حد.is_نقطه ())
  {
    إذا (axis_حد.minimum != axis_coord)
      أرجع خطأ;
  }
  وإلا
  {
    إذا (axis_coord < axis_حد.minimum ||
        axis_coord > axis_حد.بیشینه)
      أرجع خطأ;
  }
  أرجع درست;
}

بنية Instance_رکورد
{
  دوست بنية fvar;

  hb_array_t<ثابت F16DOT16> get_coordinates (بیعلامت صحيح axis_شمار) ثابت
  { أرجع coordinatesZ.as_array (axis_شمار); }

  منطقي keep_instance (بیعلامت axis_شمار,
                      ثابت hb_map_t *axes_شاخص_برچسب_map,
                      ثابت hb_hashmap_t<hb_برچسب_t, Triple> *axes_location) ثابت
  {
    إذا (axes_location->is_empty ()) أرجع درست;
    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_شمار);
    لكل (بیعلامت i = 0 ; i < axis_شمار; i++)
    {
      uint32_t *axis_برچسب_2;
      إذا (!axes_شاخص_برچسب_map->has (i, &axis_برچسب_2))
        أرجع خطأ;
      إذا (!axes_location->has (*axis_برچسب_2))
        استمر;

      Triple axis_حد = axes_location->get (*axis_برچسب_2);
      إذا (!axis_coord_pinned_or_within_axis_range (coords, i, axis_حد))
        أرجع خطأ;
    }
    أرجع درست;
  }

  منطقي subset (hb_subset_context_t *c,
               بیعلامت axis_شمار,
               منطقي has_postscript_nameid) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (flags))) return_trace (خطأ);

    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_شمار);
    ثابت hb_hashmap_t<hb_برچسب_t, Triple> *axes_location = &c->plan->user_axes_location;
    لكل (بیعلامت i = 0 ; i < axis_شمار; i++)
    {
      uint32_t *axis_برچسب_2;
      Triple *axis_حد;
      // only keep instances whose coordinates == pinned axis location
      إذا (!c->plan->axes_old_شاخص_برچسب_map.has (i, &axis_برچسب_2)) return_trace (خطأ);
      إذا (axes_location->has (*axis_برچسب_2, &axis_حد))
      {
        إذا (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_حد))
          return_trace (خطأ);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        إذا (axis_حد->is_نقطه () && !c->plan->axes_شاخص_map.has (i))
          استمر;
      }

      إذا (!c->serializer->embed (coords[i]))
        return_trace (خطأ);
    }

    إذا (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      إذا (!c->serializer->embed (name_id))
        return_trace (خطأ);
    }

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح axis_شمار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_شمار));
  }

  محمي:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  Unsized_Array_از<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  عام:
  DEFINE_اندازه_UNBOUNDED (4);
};

بنية Axis_رکورد
{
  صحيح cmp (hb_برچسب_t key) ثابت { أرجع axis_برچسب.cmp (key); }

  تعداد
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#اگر_تعریف_نشده HB_DISABLE_DEPRECATED
  فراغ get_axis_deprecated (hb_ot_var_axis_t *info) ثابت
  {
    info->برچسب_2 = axis_برچسب;
    info->name_id = axisNameID;
    get_coordinates (info->min_مقدار, info->default_مقدار, info->max_مقدار);
  }
#پایان_اگر

  فراغ get_axis_info (بیعلامت axis_شاخص, hb_ot_var_axis_info_t *info) ثابت
  {
    info->axis_شاخص = axis_شاخص;
    info->برچسب_2 = axis_برچسب;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (بیعلامت صحيح) flags;
    get_coordinates (info->min_مقدار, info->default_مقدار, info->max_مقدار);
    info->reserved = 0;
  }

  hb_برچسب_t get_axis_برچسب () ثابت { أرجع axis_برچسب; }

  منطقي is_hidden () ثابت { أرجع flags & AXIS_FLAG_HIDDEN; }

  عائم normalize_axis_مقدار (عائم v) ثابت
  {
    عائم min_مقدار, default_مقدار, max_مقدار;
    get_coordinates (min_مقدار, default_مقدار, max_مقدار);

    v = hb_clamp (v, min_مقدار, max_مقدار);

    إذا (v == default_مقدار)
      أرجع 0;
    وإلا إذا (v < default_مقدار)
      أرجع (v - default_مقدار) / (default_مقدار - min_مقدار);
    وإلا
      أرجع (v - default_مقدار) / (max_مقدار - default_مقدار);
  }

  hb_ot_name_id_t get_name_id () ثابت { أرجع axisNameID; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_coordinates (عائم &min, عائم &default_, عائم &max) ثابت
  {
    default_ = default_مقدار_2.مقصد_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_مقدار_2.مقصد_float ());
    max = hb_max (default_, max_مقدار_2.مقصد_float ());
  }

  عائم get_default () ثابت
  {
    أرجع default_مقدار_2.مقصد_float ();
  }

  TripleDistances get_triple_distances () ثابت
  {
    عائم min, default_, max;
    get_coordinates (min, default_, max);
    أرجع TripleDistances (min, default_, max);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    ثابت hb_hashmap_t<hb_برچسب_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_حد;
    إذا (user_axes_location.has (axis_برچسب, &axis_حد))
    {
      إذا (c->plan->has_avar2 && axis_حد->is_نقطه ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        بیرون->min_مقدار_2.مجموعه_float (axis_حد->میانه);
        بیرون->default_مقدار_2.مجموعه_float (axis_حد->میانه);
        بیرون->max_مقدار_2.مجموعه_float (axis_حد->میانه);
        بیرون->flags = بیرون->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      وإلا
      {
        بیرون->min_مقدار_2.مجموعه_float (axis_حد->minimum);
        بیرون->default_مقدار_2.مجموعه_float (axis_حد->میانه);
        بیرون->max_مقدار_2.مجموعه_float (axis_حد->بیشینه);
      }
    }
    return_trace (درست);
  }

  عام:
  برچسب		axis_برچسب;	/* Tag identifying the design variation for the axis. */
  محمي:
  F16DOT16	min_مقدار_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_مقدار_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_مقدار_2;	/* The maximum coordinate value for the axis. */
  عام:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  عام:
  DEFINE_اندازه_STATIC (20);
};

بنية fvar
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_fvar;

  منطقي has_داده () ثابت { أرجع نسخه.مقصد_int (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  c->check_struct (هذا) &&
		  hb_barrier () &&
		  axis_اندازه == 20 && /* Assumed in our code. */
		  instance_اندازه >= axis_شمار_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<Instance_رکورد> (get_axes ()),
				  instance_شمار, instance_اندازه));
  }

  بیعلامت صحيح get_axis_شمار () ثابت { أرجع axis_شمار_2; }

#اگر_تعریف_نشده HB_DISABLE_DEPRECATED
  بیعلامت صحيح get_axes_deprecated (بیعلامت صحيح      شروع_جابه_جایی,
				    بیعلامت صحيح     *axes_شمار /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /*بیرون*/) ثابت
  {
    إذا (axes_شمار && axes_array)
    {
      hb_array_t<ثابت Axis_رکورد> arr = get_axes ().sub_array (شروع_جابه_جایی, axes_شمار);
      لكل (بیعلامت i = 0; i < arr.درازی; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    أرجع axis_شمار_2;
  }
#پایان_اگر

  بیعلامت صحيح get_axis_infos (بیعلامت صحيح           شروع_جابه_جایی,
			       بیعلامت صحيح          *axes_شمار /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /*بیرون*/) ثابت
  {
    إذا (axes_شمار && axes_array)
    {
      hb_array_t<ثابت Axis_رکورد> arr = get_axes ().sub_array (شروع_جابه_جایی, axes_شمار);
      لكل (بیعلامت i = 0; i < arr.درازی; ++i)
	arr[i].get_axis_info (شروع_جابه_جایی + i, &axes_array[i]);
    }
    أرجع axis_شمار_2;
  }

#اگر_تعریف_نشده HB_DISABLE_DEPRECATED
  منطقي
  پیدا_کن_axis_deprecated (hb_برچسب_t برچسب_2, بیعلامت *axis_شاخص, hb_ot_var_axis_t *info) ثابت
  {
    بیعلامت i;
    إذا (!axis_شاخص) axis_شاخص = &i;
    *axis_شاخص = HB_OT_VAR_NO_AXIS_شاخص;
    خودکار axes = get_axes ();
    أرجع axes.lfind (برچسب_2, axis_شاخص) && ((فراغ) axes[*axis_شاخص].get_axis_deprecated (info), درست);
  }
#پایان_اگر
  منطقي
  پیدا_کن_axis_info (hb_برچسب_t برچسب_2, hb_ot_var_axis_info_t *info) ثابت
  {
    بیعلامت i;
    خودکار axes = get_axes ();
    أرجع axes.lfind (برچسب_2, &i) && ((فراغ) axes[i].get_axis_info (i, info), درست);
  }

  عائم normalize_axis_مقدار (بیعلامت صحيح axis_شاخص, عائم v) ثابت
  { أرجع get_axes ()[axis_شاخص].normalize_axis_مقدار (v); }

  بیعلامت صحيح get_instance_شمار () ثابت { أرجع instance_شمار; }

  hb_ot_name_id_t get_instance_subfamily_name_id (بیعلامت صحيح instance_شاخص) ثابت
  {
    ثابت Instance_رکورد *instance = get_instance (instance_شاخص);
    إذا (unlikely (!instance)) أرجع HB_OT_NAME_ID_INVALID;
    أرجع instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (بیعلامت صحيح instance_شاخص) ثابت
  {
    ثابت Instance_رکورد *instance = get_instance (instance_شاخص);
    إذا (unlikely (!instance)) أرجع HB_OT_NAME_ID_INVALID;
    إذا (instance_اندازه >= axis_شمار_2 * 4 + 6)
      أرجع StructAfter<NameID> (instance->get_coordinates (axis_شمار_2));
    أرجع HB_OT_NAME_ID_INVALID;
  }

  بیعلامت صحيح get_instance_coords (بیعلامت صحيح  instance_شاخص,
				    بیعلامت صحيح *coords_درازی, /* IN/OUT */
				    عائم        *coords         /*بیرون*/) ثابت
  {
    ثابت Instance_رکورد *instance = get_instance (instance_شاخص);
    إذا (unlikely (!instance))
    {
      إذا (coords_درازی)
	*coords_درازی = 0;
      أرجع 0;
    }

    إذا (coords_درازی && *coords_درازی && coords)
    {
      hb_array_t<ثابت F16DOT16> instanceCoords = instance->get_coordinates (axis_شمار_2)
							 .sub_array (0, coords_درازی);
      لكل (بیعلامت صحيح i = 0; i < instanceCoords.درازی; i++)
	coords[i] = instanceCoords.arrayZ[i].مقصد_float ();
    }
    أرجع axis_شمار_2;
  }

  فراغ collect_name_ids (hb_hashmap_t<hb_برچسب_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_شاخص_برچسب_map,
			 hb_مجموعه_t *nameids  /* IN/OUT */) ثابت
  {
    إذا (!has_داده ()) أرجع;

    خودکار axis_رکوردها = get_axes ();
    لكل (بیعلامت i = 0 ; i < (بیعلامت)axis_شمار_2; i++)
    {
      hb_برچسب_t axis_برچسب_2 = axis_رکوردها[i].get_axis_برچسب ();
      إذا (user_axes_location->has (axis_برچسب_2) &&
          user_axes_location->get (axis_برچسب_2).is_نقطه ())
        استمر;

      nameids->جمع_کن (axis_رکوردها[i].get_name_id ());
    }

    لكل (بیعلامت i = 0 ; i < (بیعلامت)instance_شمار; i++)
    {
      ثابت Instance_رکورد *instance = get_instance (i);

      إذا (!instance->keep_instance (axis_شمار_2, axes_old_شاخص_برچسب_map, user_axes_location))
        استمر;

      nameids->جمع_کن (instance->subfamilyNameID);

      إذا (instance_اندازه >= axis_شمار_2 * 4 + 6)
      {
        بیعلامت post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_شمار_2));
        إذا (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->جمع_کن (post_script_name_id);
      }
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بیعلامت retained_axis_شمار = c->plan->axes_شاخص_map.get_population ();
    إذا (!retained_axis_شمار) //all axes are pinned
      return_trace (خطأ);

    fvar *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (!c->serializer->check_تخصیص_بده (بیرون->axis_شمار_2, retained_axis_شمار, HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (خطأ);

    منطقي has_postscript_nameid = خطأ;
    إذا (instance_اندازه >= axis_شمار_2 * 4 + 6)
      has_postscript_nameid = درست;

    إذا (!c->serializer->check_تخصیص_بده (بیرون->instance_اندازه, retained_axis_شمار * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (خطأ);

    خودکار axes_رکوردها = get_axes ();
    لكل (بیعلامت i = 0 ; i < (بیعلامت)axis_شمار_2; i++)
    {
      إذا (!c->plan->axes_شاخص_map.has (i)) استمر;
      إذا (unlikely (!axes_رکوردها[i].subset (c)))
        return_trace (خطأ);
    }

    إذا (!c->serializer->check_تخصیص_بده (بیرون->اولی_Axis, get_اندازه (), HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (خطأ);

    بیعلامت رقم_retained_instances = 0;
    لكل (بیعلامت i = 0 ; i < (بیعلامت)instance_شمار; i++)
    {
      ثابت Instance_رکورد *instance = get_instance (i);
      خودکار snap = c->serializer->snapshot ();
      إذا (!instance->subset (c, axis_شمار_2, has_postscript_nameid))
        c->serializer->revert (snap);
      وإلا
        رقم_retained_instances++;
    }

    return_trace (c->serializer->check_تخصیص_بده (بیرون->instance_شمار, رقم_retained_instances, HB_SERIALIZE_ERROR_INT_سرریز));
  }

  عام:
  hb_array_t<ثابت Axis_رکورد> get_axes () ثابت
  { أرجع hb_array (&(هذا+اولی_Axis), axis_شمار_2); }

  ثابت Instance_رکورد *get_instance (بیعلامت صحيح i) ثابت
  {
    إذا (unlikely (i >= instance_شمار)) أرجع اشاره_تهی;
   أرجع &Struct_At_جابه_جایی<Instance_رکورد> (&StructAfter<Instance_رکورد> (get_axes ()),
					   i * instance_اندازه);
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16_مقصد<Axis_رکورد>
		اولی_Axis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_شمار_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axis_اندازه;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_شمار;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instance_اندازه;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  عام:
  DEFINE_اندازه_STATIC (16);
};

} /* namespace OT */


#پایان_اگر /* HB_OT_VAR_FVAR_TABLE_HH */
