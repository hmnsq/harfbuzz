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

#جے_بنا_تعریف HB_OT_VAR_FVAR_جدول_HH
#تعریف_کرو HB_OT_VAR_FVAR_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#تعریف_کرو HB_OT_ٹیگ_fvar HB_ٹیگ('f','v','a','r')


نطاق OT {

ساكن منطقي axis_coord_pinned_or_within_axis_range (ثابت hb_array_t<ثابت F16DOT16> coords,
                                                    بنانشانی axis_اشاریہ,
                                                    Triple axis_حد_بندی)
{
  عائم axis_coord = coords[axis_اشاریہ].منزل_float ();
  إذا (axis_حد_بندی.is_نقطہ ())
  {
    إذا (axis_حد_بندی.minimum != axis_coord)
      أرجع خطأ;
  }
  وإلا
  {
    إذا (axis_coord < axis_حد_بندی.minimum ||
        axis_coord > axis_حد_بندی.ودھ_توں_ودھ)
      أرجع خطأ;
  }
  أرجع سچ;
}

بنية Instance_ریکارڈ
{
  یار بنية fvar;

  hb_array_t<ثابت F16DOT16> get_coordinates (بنانشانی صحيح axis_گنتی) ثابت
  { أرجع coordinatesZ.as_array (axis_گنتی); }

  منطقي keep_instance (بنانشانی axis_گنتی,
                      ثابت hb_map_t *axes_اشاریہ_ٹیگ_map,
                      ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> *axes_location) ثابت
  {
    إذا (axes_location->is_empty ()) أرجع سچ;
    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_گنتی);
    لكل (بنانشانی i = 0 ; i < axis_گنتی; i++)
    {
      uint32_t *axis_ٹیگ_2;
      إذا (!axes_اشاریہ_ٹیگ_map->has (i, &axis_ٹیگ_2))
        أرجع خطأ;
      إذا (!axes_location->has (*axis_ٹیگ_2))
        استمر;

      Triple axis_حد_بندی = axes_location->get (*axis_ٹیگ_2);
      إذا (!axis_coord_pinned_or_within_axis_range (coords, i, axis_حد_بندی))
        أرجع خطأ;
    }
    أرجع سچ;
  }

  منطقي subset (hb_subset_context_t *c,
               بنانشانی axis_گنتی,
               منطقي has_postscript_nameid) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (flags))) return_trace (خطأ);

    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_گنتی);
    ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> *axes_location = &c->plan->user_axes_location;
    لكل (بنانشانی i = 0 ; i < axis_گنتی; i++)
    {
      uint32_t *axis_ٹیگ_2;
      Triple *axis_حد_بندی;
      // only keep instances whose coordinates == pinned axis location
      إذا (!c->plan->axes_old_اشاریہ_ٹیگ_map.has (i, &axis_ٹیگ_2)) return_trace (خطأ);
      إذا (axes_location->has (*axis_ٹیگ_2, &axis_حد_بندی))
      {
        إذا (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_حد_بندی))
          return_trace (خطأ);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        إذا (axis_حد_بندی->is_نقطہ () && !c->plan->axes_اشاریہ_map.has (i))
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

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح axis_گنتی) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_گنتی));
  }

  محمي:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  Unsized_Array_دا<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  عام:
  DEFINE_ناپ_UNBOUNDED (4);
};

بنية Axis_ریکارڈ
{
  صحيح cmp (hb_ٹیگ_t key) ثابت { أرجع axis_ٹیگ.cmp (key); }

  تعداد
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#جے_بنا_تعریف HB_DISABLE_DEPRECATED
  فراغ get_axis_deprecated (hb_ot_var_axis_t *info) ثابت
  {
    info->ٹیگ_2 = axis_ٹیگ;
    info->name_id = axisNameID;
    get_coordinates (info->min_قدر, info->default_قدر, info->max_قدر);
  }
#جے_انت

  فراغ get_axis_info (بنانشانی axis_اشاریہ, hb_ot_var_axis_info_t *info) ثابت
  {
    info->axis_اشاریہ = axis_اشاریہ;
    info->ٹیگ_2 = axis_ٹیگ;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (بنانشانی صحيح) flags;
    get_coordinates (info->min_قدر, info->default_قدر, info->max_قدر);
    info->reserved = 0;
  }

  hb_ٹیگ_t get_axis_ٹیگ () ثابت { أرجع axis_ٹیگ; }

  منطقي is_hidden () ثابت { أرجع flags & AXIS_FLAG_HIDDEN; }

  عائم normalize_axis_قدر (عائم v) ثابت
  {
    عائم min_قدر, default_قدر, max_قدر;
    get_coordinates (min_قدر, default_قدر, max_قدر);

    v = hb_clamp (v, min_قدر, max_قدر);

    إذا (v == default_قدر)
      أرجع 0;
    وإلا إذا (v < default_قدر)
      أرجع (v - default_قدر) / (default_قدر - min_قدر);
    وإلا
      أرجع (v - default_قدر) / (max_قدر - default_قدر);
  }

  hb_ot_name_id_t get_name_id () ثابت { أرجع axisNameID; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_coordinates (عائم &min, عائم &default_, عائم &max) ثابت
  {
    default_ = default_قدر_2.منزل_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_قدر_2.منزل_float ());
    max = hb_max (default_, max_قدر_2.منزل_float ());
  }

  عائم get_default () ثابت
  {
    أرجع default_قدر_2.منزل_float ();
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
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_حد_بندی;
    إذا (user_axes_location.has (axis_ٹیگ, &axis_حد_بندی))
    {
      إذا (c->plan->has_avar2 && axis_حد_بندی->is_نقطہ ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        باہرلا->min_قدر_2.سیٹ_float (axis_حد_بندی->درمیان);
        باہرلا->default_قدر_2.سیٹ_float (axis_حد_بندی->درمیان);
        باہرلا->max_قدر_2.سیٹ_float (axis_حد_بندی->درمیان);
        باہرلا->flags = باہرلا->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      وإلا
      {
        باہرلا->min_قدر_2.سیٹ_float (axis_حد_بندی->minimum);
        باہرلا->default_قدر_2.سیٹ_float (axis_حد_بندی->درمیان);
        باہرلا->max_قدر_2.سیٹ_float (axis_حد_بندی->ودھ_توں_ودھ);
      }
    }
    return_trace (سچ);
  }

  عام:
  ٹیگ		axis_ٹیگ;	/* Tag identifying the design variation for the axis. */
  محمي:
  F16DOT16	min_قدر_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_قدر_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_قدر_2;	/* The maximum coordinate value for the axis. */
  عام:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  عام:
  DEFINE_ناپ_STATIC (20);
};

بنية fvar
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_fvar;

  منطقي has_انکڑے () ثابت { أرجع نسخہ.منزل_int (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  c->check_struct (هذا) &&
		  hb_barrier () &&
		  axis_ناپ == 20 && /* Assumed in our code. */
		  instance_ناپ >= axis_گنتی_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<Instance_ریکارڈ> (get_axes ()),
				  instance_گنتی, instance_ناپ));
  }

  بنانشانی صحيح get_axis_گنتی () ثابت { أرجع axis_گنتی_2; }

#جے_بنا_تعریف HB_DISABLE_DEPRECATED
  بنانشانی صحيح get_axes_deprecated (بنانشانی صحيح      شروع_وتھ,
				    بنانشانی صحيح     *axes_گنتی /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /*باہرلا*/) ثابت
  {
    إذا (axes_گنتی && axes_array)
    {
      hb_array_t<ثابت Axis_ریکارڈ> arr = get_axes ().sub_array (شروع_وتھ, axes_گنتی);
      لكل (بنانشانی i = 0; i < arr.لمائی; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    أرجع axis_گنتی_2;
  }
#جے_انت

  بنانشانی صحيح get_axis_infos (بنانشانی صحيح           شروع_وتھ,
			       بنانشانی صحيح          *axes_گنتی /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /*باہرلا*/) ثابت
  {
    إذا (axes_گنتی && axes_array)
    {
      hb_array_t<ثابت Axis_ریکارڈ> arr = get_axes ().sub_array (شروع_وتھ, axes_گنتی);
      لكل (بنانشانی i = 0; i < arr.لمائی; ++i)
	arr[i].get_axis_info (شروع_وتھ + i, &axes_array[i]);
    }
    أرجع axis_گنتی_2;
  }

#جے_بنا_تعریف HB_DISABLE_DEPRECATED
  منطقي
  لبھو_axis_deprecated (hb_ٹیگ_t ٹیگ_2, بنانشانی *axis_اشاریہ, hb_ot_var_axis_t *info) ثابت
  {
    بنانشانی i;
    إذا (!axis_اشاریہ) axis_اشاریہ = &i;
    *axis_اشاریہ = HB_OT_VAR_NO_AXIS_اشاریہ;
    خودکار axes = get_axes ();
    أرجع axes.lfind (ٹیگ_2, axis_اشاریہ) && ((فراغ) axes[*axis_اشاریہ].get_axis_deprecated (info), سچ);
  }
#جے_انت
  منطقي
  لبھو_axis_info (hb_ٹیگ_t ٹیگ_2, hb_ot_var_axis_info_t *info) ثابت
  {
    بنانشانی i;
    خودکار axes = get_axes ();
    أرجع axes.lfind (ٹیگ_2, &i) && ((فراغ) axes[i].get_axis_info (i, info), سچ);
  }

  عائم normalize_axis_قدر (بنانشانی صحيح axis_اشاریہ, عائم v) ثابت
  { أرجع get_axes ()[axis_اشاریہ].normalize_axis_قدر (v); }

  بنانشانی صحيح get_instance_گنتی () ثابت { أرجع instance_گنتی; }

  hb_ot_name_id_t get_instance_subfamily_name_id (بنانشانی صحيح instance_اشاریہ) ثابت
  {
    ثابت Instance_ریکارڈ *instance = get_instance (instance_اشاریہ);
    إذا (unlikely (!instance)) أرجع HB_OT_NAME_ID_INVALID;
    أرجع instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (بنانشانی صحيح instance_اشاریہ) ثابت
  {
    ثابت Instance_ریکارڈ *instance = get_instance (instance_اشاریہ);
    إذا (unlikely (!instance)) أرجع HB_OT_NAME_ID_INVALID;
    إذا (instance_ناپ >= axis_گنتی_2 * 4 + 6)
      أرجع StructAfter<NameID> (instance->get_coordinates (axis_گنتی_2));
    أرجع HB_OT_NAME_ID_INVALID;
  }

  بنانشانی صحيح get_instance_coords (بنانشانی صحيح  instance_اشاریہ,
				    بنانشانی صحيح *coords_لمائی, /* IN/OUT */
				    عائم        *coords         /*باہرلا*/) ثابت
  {
    ثابت Instance_ریکارڈ *instance = get_instance (instance_اشاریہ);
    إذا (unlikely (!instance))
    {
      إذا (coords_لمائی)
	*coords_لمائی = 0;
      أرجع 0;
    }

    إذا (coords_لمائی && *coords_لمائی && coords)
    {
      hb_array_t<ثابت F16DOT16> instanceCoords = instance->get_coordinates (axis_گنتی_2)
							 .sub_array (0, coords_لمائی);
      لكل (بنانشانی صحيح i = 0; i < instanceCoords.لمائی; i++)
	coords[i] = instanceCoords.arrayZ[i].منزل_float ();
    }
    أرجع axis_گنتی_2;
  }

  فراغ collect_name_ids (hb_hashmap_t<hb_ٹیگ_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_اشاریہ_ٹیگ_map,
			 hb_سیٹ_t *nameids  /* IN/OUT */) ثابت
  {
    إذا (!has_انکڑے ()) أرجع;

    خودکار axis_ریکارڈاں = get_axes ();
    لكل (بنانشانی i = 0 ; i < (بنانشانی)axis_گنتی_2; i++)
    {
      hb_ٹیگ_t axis_ٹیگ_2 = axis_ریکارڈاں[i].get_axis_ٹیگ ();
      إذا (user_axes_location->has (axis_ٹیگ_2) &&
          user_axes_location->get (axis_ٹیگ_2).is_نقطہ ())
        استمر;

      nameids->جوڑو (axis_ریکارڈاں[i].get_name_id ());
    }

    لكل (بنانشانی i = 0 ; i < (بنانشانی)instance_گنتی; i++)
    {
      ثابت Instance_ریکارڈ *instance = get_instance (i);

      إذا (!instance->keep_instance (axis_گنتی_2, axes_old_اشاریہ_ٹیگ_map, user_axes_location))
        استمر;

      nameids->جوڑو (instance->subfamilyNameID);

      إذا (instance_ناپ >= axis_گنتی_2 * 4 + 6)
      {
        بنانشانی post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_گنتی_2));
        إذا (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->جوڑو (post_script_name_id);
      }
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بنانشانی retained_axis_گنتی = c->plan->axes_اشاریہ_map.get_population ();
    إذا (!retained_axis_گنتی) //all axes are pinned
      return_trace (خطأ);

    fvar *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (!c->serializer->check_سونپو (باہرلا->axis_گنتی_2, retained_axis_گنتی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خطأ);

    منطقي has_postscript_nameid = خطأ;
    إذا (instance_ناپ >= axis_گنتی_2 * 4 + 6)
      has_postscript_nameid = سچ;

    إذا (!c->serializer->check_سونپو (باہرلا->instance_ناپ, retained_axis_گنتی * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خطأ);

    خودکار axes_ریکارڈاں = get_axes ();
    لكل (بنانشانی i = 0 ; i < (بنانشانی)axis_گنتی_2; i++)
    {
      إذا (!c->plan->axes_اشاریہ_map.has (i)) استمر;
      إذا (unlikely (!axes_ریکارڈاں[i].subset (c)))
        return_trace (خطأ);
    }

    إذا (!c->serializer->check_سونپو (باہرلا->پہلا_Axis, get_ناپ (), HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خطأ);

    بنانشانی عدد_retained_instances = 0;
    لكل (بنانشانی i = 0 ; i < (بنانشانی)instance_گنتی; i++)
    {
      ثابت Instance_ریکارڈ *instance = get_instance (i);
      خودکار snap = c->serializer->snapshot ();
      إذا (!instance->subset (c, axis_گنتی_2, has_postscript_nameid))
        c->serializer->revert (snap);
      وإلا
        عدد_retained_instances++;
    }

    return_trace (c->serializer->check_سونپو (باہرلا->instance_گنتی, عدد_retained_instances, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  عام:
  hb_array_t<ثابت Axis_ریکارڈ> get_axes () ثابت
  { أرجع hb_array (&(هذا+پہلا_Axis), axis_گنتی_2); }

  ثابت Instance_ریکارڈ *get_instance (بنانشانی صحيح i) ثابت
  {
    إذا (unlikely (i >= instance_گنتی)) أرجع خالی_سوچک;
   أرجع &Struct_At_وتھ<Instance_ریکارڈ> (&StructAfter<Instance_ریکارڈ> (get_axes ()),
					   i * instance_ناپ);
  }

  محمي:
  Fixed_نسخہ<>نسخہ;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16_منزل<Axis_ریکارڈ>
		پہلا_Axis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_گنتی_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axis_ناپ;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_گنتی;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instance_ناپ;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  عام:
  DEFINE_ناپ_STATIC (16);
};

} /* namespace OT */


#جے_انت /* HB_OT_VAR_FVAR_TABLE_HH */
