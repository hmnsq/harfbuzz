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

#که_نه_تعریف_شوی HB_OT_VAR_FVAR_جدول_HH
#تعریف_کړه HB_OT_VAR_FVAR_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#تعریف_کړه HB_OT_نښان_fvar HB_نښان('f','v','a','r')


نطاق OT {

ساكن منطقي axis_coord_pinned_or_within_axis_range (ثابت hb_array_t<ثابت F16DOT16> coords,
                                                    بینښه axis_شاخص,
                                                    Triple axis_حد)
{
  عائم axis_coord = coords[axis_شاخص].منزل_float ();
  إذا (axis_حد.is_ټکی ())
  {
    إذا (axis_حد.minimum != axis_coord)
      أرجع خطأ;
  }
  وإلا
  {
    إذا (axis_coord < axis_حد.minimum ||
        axis_coord > axis_حد.اعظمي)
      أرجع خطأ;
  }
  أرجع ریښتیا;
}

بنية Instance_ریکارډ
{
  ملګری بنية fvar;

  hb_array_t<ثابت F16DOT16> get_coordinates (بینښه صحيح axis_شمېر) ثابت
  { أرجع coordinatesZ.as_array (axis_شمېر); }

  منطقي keep_instance (بینښه axis_شمېر,
                      ثابت hb_map_t *axes_شاخص_نښان_map,
                      ثابت hb_hashmap_t<hb_نښان_t, Triple> *axes_location) ثابت
  {
    إذا (axes_location->is_empty ()) أرجع ریښتیا;
    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_شمېر);
    لكل (بینښه i = 0 ; i < axis_شمېر; i++)
    {
      uint32_t *axis_نښان_2;
      إذا (!axes_شاخص_نښان_map->has (i, &axis_نښان_2))
        أرجع خطأ;
      إذا (!axes_location->has (*axis_نښان_2))
        استمر;

      Triple axis_حد = axes_location->get (*axis_نښان_2);
      إذا (!axis_coord_pinned_or_within_axis_range (coords, i, axis_حد))
        أرجع خطأ;
    }
    أرجع ریښتیا;
  }

  منطقي subset (hb_subset_context_t *c,
               بینښه axis_شمېر,
               منطقي has_postscript_nameid) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (flags))) return_trace (خطأ);

    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_شمېر);
    ثابت hb_hashmap_t<hb_نښان_t, Triple> *axes_location = &c->plan->user_axes_location;
    لكل (بینښه i = 0 ; i < axis_شمېر; i++)
    {
      uint32_t *axis_نښان_2;
      Triple *axis_حد;
      // only keep instances whose coordinates == pinned axis location
      إذا (!c->plan->axes_old_شاخص_نښان_map.has (i, &axis_نښان_2)) return_trace (خطأ);
      إذا (axes_location->has (*axis_نښان_2, &axis_حد))
      {
        إذا (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_حد))
          return_trace (خطأ);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        إذا (axis_حد->is_ټکی () && !c->plan->axes_شاخص_map.has (i))
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

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح axis_شمېر) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_شمېر));
  }

  محمي:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  Unsized_Array_د<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  عام:
  DEFINE_اندازه_UNBOUNDED (4);
};

بنية Axis_ریکارډ
{
  صحيح cmp (hb_نښان_t key) ثابت { أرجع axis_نښان.cmp (key); }

  تعداد
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED
  فراغ get_axis_deprecated (hb_ot_var_axis_t *info) ثابت
  {
    info->نښان_2 = axis_نښان;
    info->name_id = axisNameID;
    get_coordinates (info->min_ارزښت, info->default_ارزښت, info->max_ارزښت);
  }
#د_که_پای

  فراغ get_axis_info (بینښه axis_شاخص, hb_ot_var_axis_info_t *info) ثابت
  {
    info->axis_شاخص = axis_شاخص;
    info->نښان_2 = axis_نښان;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (بینښه صحيح) flags;
    get_coordinates (info->min_ارزښت, info->default_ارزښت, info->max_ارزښت);
    info->reserved = 0;
  }

  hb_نښان_t get_axis_نښان () ثابت { أرجع axis_نښان; }

  منطقي is_hidden () ثابت { أرجع flags & AXIS_FLAG_HIDDEN; }

  عائم normalize_axis_ارزښت (عائم v) ثابت
  {
    عائم min_ارزښت, default_ارزښت, max_ارزښت;
    get_coordinates (min_ارزښت, default_ارزښت, max_ارزښت);

    v = hb_clamp (v, min_ارزښت, max_ارزښت);

    إذا (v == default_ارزښت)
      أرجع 0;
    وإلا إذا (v < default_ارزښت)
      أرجع (v - default_ارزښت) / (default_ارزښت - min_ارزښت);
    وإلا
      أرجع (v - default_ارزښت) / (max_ارزښت - default_ارزښت);
  }

  hb_ot_name_id_t get_name_id () ثابت { أرجع axisNameID; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_coordinates (عائم &min, عائم &default_, عائم &max) ثابت
  {
    default_ = default_ارزښت_2.منزل_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_ارزښت_2.منزل_float ());
    max = hb_max (default_, max_ارزښت_2.منزل_float ());
  }

  عائم get_default () ثابت
  {
    أرجع default_ارزښت_2.منزل_float ();
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
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    ثابت hb_hashmap_t<hb_نښان_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_حد;
    إذا (user_axes_location.has (axis_نښان, &axis_حد))
    {
      إذا (c->plan->has_avar2 && axis_حد->is_ټکی ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        بهر->min_ارزښت_2.سېټ_float (axis_حد->منځ);
        بهر->default_ارزښت_2.سېټ_float (axis_حد->منځ);
        بهر->max_ارزښت_2.سېټ_float (axis_حد->منځ);
        بهر->flags = بهر->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      وإلا
      {
        بهر->min_ارزښت_2.سېټ_float (axis_حد->minimum);
        بهر->default_ارزښت_2.سېټ_float (axis_حد->منځ);
        بهر->max_ارزښت_2.سېټ_float (axis_حد->اعظمي);
      }
    }
    return_trace (ریښتیا);
  }

  عام:
  نښان		axis_نښان;	/* Tag identifying the design variation for the axis. */
  محمي:
  F16DOT16	min_ارزښت_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_ارزښت_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_ارزښت_2;	/* The maximum coordinate value for the axis. */
  عام:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  عام:
  DEFINE_اندازه_STATIC (20);
};

بنية fvar
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_fvar;

  منطقي has_معلومات () ثابت { أرجع نسخه.منزل_int (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  c->check_struct (هذا) &&
		  hb_barrier () &&
		  axis_اندازه == 20 && /* Assumed in our code. */
		  instance_اندازه >= axis_شمېر_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<Instance_ریکارډ> (get_axes ()),
				  instance_شمېر, instance_اندازه));
  }

  بینښه صحيح get_axis_شمېر () ثابت { أرجع axis_شمېر_2; }

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED
  بینښه صحيح get_axes_deprecated (بینښه صحيح      پیل_دننه_واټن,
				    بینښه صحيح     *axes_شمېر /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /*بهر*/) ثابت
  {
    إذا (axes_شمېر && axes_array)
    {
      hb_array_t<ثابت Axis_ریکارډ> arr = get_axes ().sub_array (پیل_دننه_واټن, axes_شمېر);
      لكل (بینښه i = 0; i < arr.اوږدتیا; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    أرجع axis_شمېر_2;
  }
#د_که_پای

  بینښه صحيح get_axis_infos (بینښه صحيح           پیل_دننه_واټن,
			       بینښه صحيح          *axes_شمېر /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /*بهر*/) ثابت
  {
    إذا (axes_شمېر && axes_array)
    {
      hb_array_t<ثابت Axis_ریکارډ> arr = get_axes ().sub_array (پیل_دننه_واټن, axes_شمېر);
      لكل (بینښه i = 0; i < arr.اوږدتیا; ++i)
	arr[i].get_axis_info (پیل_دننه_واټن + i, &axes_array[i]);
    }
    أرجع axis_شمېر_2;
  }

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED
  منطقي
  پیدا_کړه_axis_deprecated (hb_نښان_t نښان_2, بینښه *axis_شاخص, hb_ot_var_axis_t *info) ثابت
  {
    بینښه i;
    إذا (!axis_شاخص) axis_شاخص = &i;
    *axis_شاخص = HB_OT_VAR_NO_AXIS_شاخص;
    خپلکاری axes = get_axes ();
    أرجع axes.lfind (نښان_2, axis_شاخص) && ((فراغ) axes[*axis_شاخص].get_axis_deprecated (info), ریښتیا);
  }
#د_که_پای
  منطقي
  پیدا_کړه_axis_info (hb_نښان_t نښان_2, hb_ot_var_axis_info_t *info) ثابت
  {
    بینښه i;
    خپلکاری axes = get_axes ();
    أرجع axes.lfind (نښان_2, &i) && ((فراغ) axes[i].get_axis_info (i, info), ریښتیا);
  }

  عائم normalize_axis_ارزښت (بینښه صحيح axis_شاخص, عائم v) ثابت
  { أرجع get_axes ()[axis_شاخص].normalize_axis_ارزښت (v); }

  بینښه صحيح get_instance_شمېر () ثابت { أرجع instance_شمېر; }

  hb_ot_name_id_t get_instance_subfamily_name_id (بینښه صحيح instance_شاخص) ثابت
  {
    ثابت Instance_ریکارډ *instance = get_instance (instance_شاخص);
    إذا (unlikely (!instance)) أرجع HB_OT_NAME_ID_INVALID;
    أرجع instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (بینښه صحيح instance_شاخص) ثابت
  {
    ثابت Instance_ریکارډ *instance = get_instance (instance_شاخص);
    إذا (unlikely (!instance)) أرجع HB_OT_NAME_ID_INVALID;
    إذا (instance_اندازه >= axis_شمېر_2 * 4 + 6)
      أرجع StructAfter<NameID> (instance->get_coordinates (axis_شمېر_2));
    أرجع HB_OT_NAME_ID_INVALID;
  }

  بینښه صحيح get_instance_coords (بینښه صحيح  instance_شاخص,
				    بینښه صحيح *coords_اوږدتیا, /* IN/OUT */
				    عائم        *coords         /*بهر*/) ثابت
  {
    ثابت Instance_ریکارډ *instance = get_instance (instance_شاخص);
    إذا (unlikely (!instance))
    {
      إذا (coords_اوږدتیا)
	*coords_اوږدتیا = 0;
      أرجع 0;
    }

    إذا (coords_اوږدتیا && *coords_اوږدتیا && coords)
    {
      hb_array_t<ثابت F16DOT16> instanceCoords = instance->get_coordinates (axis_شمېر_2)
							 .sub_array (0, coords_اوږدتیا);
      لكل (بینښه صحيح i = 0; i < instanceCoords.اوږدتیا; i++)
	coords[i] = instanceCoords.arrayZ[i].منزل_float ();
    }
    أرجع axis_شمېر_2;
  }

  فراغ collect_name_ids (hb_hashmap_t<hb_نښان_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_شاخص_نښان_map,
			 hb_سېټ_t *nameids  /* IN/OUT */) ثابت
  {
    إذا (!has_معلومات ()) أرجع;

    خپلکاری axis_ریکارډونه = get_axes ();
    لكل (بینښه i = 0 ; i < (بینښه)axis_شمېر_2; i++)
    {
      hb_نښان_t axis_نښان_2 = axis_ریکارډونه[i].get_axis_نښان ();
      إذا (user_axes_location->has (axis_نښان_2) &&
          user_axes_location->get (axis_نښان_2).is_ټکی ())
        استمر;

      nameids->جمع_کړه (axis_ریکارډونه[i].get_name_id ());
    }

    لكل (بینښه i = 0 ; i < (بینښه)instance_شمېر; i++)
    {
      ثابت Instance_ریکارډ *instance = get_instance (i);

      إذا (!instance->keep_instance (axis_شمېر_2, axes_old_شاخص_نښان_map, user_axes_location))
        استمر;

      nameids->جمع_کړه (instance->subfamilyNameID);

      إذا (instance_اندازه >= axis_شمېر_2 * 4 + 6)
      {
        بینښه post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_شمېر_2));
        إذا (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->جمع_کړه (post_script_name_id);
      }
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بینښه retained_axis_شمېر = c->plan->axes_شاخص_map.get_population ();
    إذا (!retained_axis_شمېر) //all axes are pinned
      return_trace (خطأ);

    fvar *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (!c->serializer->check_ټاکل (بهر->axis_شمېر_2, retained_axis_شمېر, HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);

    منطقي has_postscript_nameid = خطأ;
    إذا (instance_اندازه >= axis_شمېر_2 * 4 + 6)
      has_postscript_nameid = ریښتیا;

    إذا (!c->serializer->check_ټاکل (بهر->instance_اندازه, retained_axis_شمېر * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);

    خپلکاری axes_ریکارډونه = get_axes ();
    لكل (بینښه i = 0 ; i < (بینښه)axis_شمېر_2; i++)
    {
      إذا (!c->plan->axes_شاخص_map.has (i)) استمر;
      إذا (unlikely (!axes_ریکارډونه[i].subset (c)))
        return_trace (خطأ);
    }

    إذا (!c->serializer->check_ټاکل (بهر->لومړی_Axis, get_اندازه (), HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);

    بینښه عدد_retained_instances = 0;
    لكل (بینښه i = 0 ; i < (بینښه)instance_شمېر; i++)
    {
      ثابت Instance_ریکارډ *instance = get_instance (i);
      خپلکاری snap = c->serializer->snapshot ();
      إذا (!instance->subset (c, axis_شمېر_2, has_postscript_nameid))
        c->serializer->revert (snap);
      وإلا
        عدد_retained_instances++;
    }

    return_trace (c->serializer->check_ټاکل (بهر->instance_شمېر, عدد_retained_instances, HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  عام:
  hb_array_t<ثابت Axis_ریکارډ> get_axes () ثابت
  { أرجع hb_array (&(هذا+لومړی_Axis), axis_شمېر_2); }

  ثابت Instance_ریکارډ *get_instance (بینښه صحيح i) ثابت
  {
    إذا (unlikely (i >= instance_شمېر)) أرجع تش_اشاره;
   أرجع &Struct_At_دننه_واټن<Instance_ریکارډ> (&StructAfter<Instance_ریکارډ> (get_axes ()),
					   i * instance_اندازه);
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16_منزل<Axis_ریکارډ>
		لومړی_Axis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_شمېر_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axis_اندازه;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_شمېر;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instance_اندازه;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  عام:
  DEFINE_اندازه_STATIC (16);
};

} /* namespace OT */


#د_که_پای /* HB_OT_VAR_FVAR_TABLE_HH */
