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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_VAR_FVAR_جدول_HH
#وصف_ڪريو HB_OT_VAR_FVAR_جدول_HH

#ضمن "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#وصف_ڪريو HB_OT_ٽيگ_fvar HB_ٽيگ('f','v','a','r')


نطاق OT {

ساكن منطقي axis_coord_pinned_or_within_axis_range (ثابت hb_array_t<ثابت F16DOT16> coords,
                                                    بنانشان axis_اشاريو,
                                                    Triple axis_حد_بندي)
{
  عائم axis_coord = coords[axis_اشاريو].منزل_float ();
  إذا (axis_حد_بندي.is_نقطو ())
  {
    إذا (axis_حد_بندي.minimum != axis_coord)
      أرجع خطأ;
  }
  وإلا
  {
    إذا (axis_coord < axis_حد_بندي.minimum ||
        axis_coord > axis_حد_بندي.وڌ_کان_وڌ)
      أرجع خطأ;
  }
  أرجع سچ;
}

بنية Instance_رڪارڊ
{
  دوست بنية fvar;

  hb_array_t<ثابت F16DOT16> get_coordinates (بنانشان صحيح axis_ڳڻپ) ثابت
  { أرجع coordinatesZ.as_array (axis_ڳڻپ); }

  منطقي keep_instance (بنانشان axis_ڳڻپ,
                      ثابت hb_map_t *axes_اشاريو_ٽيگ_map,
                      ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> *axes_location) ثابت
  {
    إذا (axes_location->is_empty ()) أرجع سچ;
    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_ڳڻپ);
    لكل (بنانشان i = 0 ; i < axis_ڳڻپ; i++)
    {
      uint32_t *axis_ٽيگ_2;
      إذا (!axes_اشاريو_ٽيگ_map->has (i, &axis_ٽيگ_2))
        أرجع خطأ;
      إذا (!axes_location->has (*axis_ٽيگ_2))
        استمر;

      Triple axis_حد_بندي = axes_location->get (*axis_ٽيگ_2);
      إذا (!axis_coord_pinned_or_within_axis_range (coords, i, axis_حد_بندي))
        أرجع خطأ;
    }
    أرجع سچ;
  }

  منطقي subset (hb_subset_context_t *c,
               بنانشان axis_ڳڻپ,
               منطقي has_postscript_nameid) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (flags))) return_trace (خطأ);

    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_ڳڻپ);
    ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> *axes_location = &c->plan->user_axes_location;
    لكل (بنانشان i = 0 ; i < axis_ڳڻپ; i++)
    {
      uint32_t *axis_ٽيگ_2;
      Triple *axis_حد_بندي;
      // only keep instances whose coordinates == pinned axis location
      إذا (!c->plan->axes_old_اشاريو_ٽيگ_map.has (i, &axis_ٽيگ_2)) return_trace (خطأ);
      إذا (axes_location->has (*axis_ٽيگ_2, &axis_حد_بندي))
      {
        إذا (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_حد_بندي))
          return_trace (خطأ);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        إذا (axis_حد_بندي->is_نقطو () && !c->plan->axes_اشاريو_map.has (i))
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

  منطقي sanitize (hb_sanitize_context_t *c, بنانشان صحيح axis_ڳڻپ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_ڳڻپ));
  }

  محمي:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  Unsized_Array_جو<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  عام:
  DEFINE_سائيز_UNBOUNDED (4);
};

بنية Axis_رڪارڊ
{
  صحيح cmp (hb_ٽيگ_t key) ثابت { أرجع axis_ٽيگ.cmp (key); }

  تعداد
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#جيڪڏهن_وصف_نه_ٿيل HB_DISABLE_DEPRECATED
  فراغ get_axis_deprecated (hb_ot_var_axis_t *info) ثابت
  {
    info->ٽيگ_2 = axis_ٽيگ;
    info->name_id = axisNameID;
    get_coordinates (info->min_قدر, info->default_قدر, info->max_قدر);
  }
#جيڪڏهن_پورو

  فراغ get_axis_info (بنانشان axis_اشاريو, hb_ot_var_axis_info_t *info) ثابت
  {
    info->axis_اشاريو = axis_اشاريو;
    info->ٽيگ_2 = axis_ٽيگ;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (بنانشان صحيح) flags;
    get_coordinates (info->min_قدر, info->default_قدر, info->max_قدر);
    info->reserved = 0;
  }

  hb_ٽيگ_t get_axis_ٽيگ () ثابت { أرجع axis_ٽيگ; }

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
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_حد_بندي;
    إذا (user_axes_location.has (axis_ٽيگ, &axis_حد_بندي))
    {
      إذا (c->plan->has_avar2 && axis_حد_بندي->is_نقطو ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        ٻاهريون->min_قدر_2.سيٽ_float (axis_حد_بندي->وچ);
        ٻاهريون->default_قدر_2.سيٽ_float (axis_حد_بندي->وچ);
        ٻاهريون->max_قدر_2.سيٽ_float (axis_حد_بندي->وچ);
        ٻاهريون->flags = ٻاهريون->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      وإلا
      {
        ٻاهريون->min_قدر_2.سيٽ_float (axis_حد_بندي->minimum);
        ٻاهريون->default_قدر_2.سيٽ_float (axis_حد_بندي->وچ);
        ٻاهريون->max_قدر_2.سيٽ_float (axis_حد_بندي->وڌ_کان_وڌ);
      }
    }
    return_trace (سچ);
  }

  عام:
  ٽيگ		axis_ٽيگ;	/* Tag identifying the design variation for the axis. */
  محمي:
  F16DOT16	min_قدر_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_قدر_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_قدر_2;	/* The maximum coordinate value for the axis. */
  عام:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  عام:
  DEFINE_سائيز_STATIC (20);
};

بنية fvar
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_fvar;

  منطقي has_معلومات () ثابت { أرجع نسخو.منزل_int (); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخو.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخو.major == 1) &&
		  c->check_struct (هذا) &&
		  hb_barrier () &&
		  axis_سائيز == 20 && /* Assumed in our code. */
		  instance_سائيز >= axis_ڳڻپ_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<Instance_رڪارڊ> (get_axes ()),
				  instance_ڳڻپ, instance_سائيز));
  }

  بنانشان صحيح get_axis_ڳڻپ () ثابت { أرجع axis_ڳڻپ_2; }

#جيڪڏهن_وصف_نه_ٿيل HB_DISABLE_DEPRECATED
  بنانشان صحيح get_axes_deprecated (بنانشان صحيح      شروعات_وٿي,
				    بنانشان صحيح     *axes_ڳڻپ /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /*ٻاهريون*/) ثابت
  {
    إذا (axes_ڳڻپ && axes_array)
    {
      hb_array_t<ثابت Axis_رڪارڊ> arr = get_axes ().sub_array (شروعات_وٿي, axes_ڳڻپ);
      لكل (بنانشان i = 0; i < arr.ڊيگهائي; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    أرجع axis_ڳڻپ_2;
  }
#جيڪڏهن_پورو

  بنانشان صحيح get_axis_infos (بنانشان صحيح           شروعات_وٿي,
			       بنانشان صحيح          *axes_ڳڻپ /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /*ٻاهريون*/) ثابت
  {
    إذا (axes_ڳڻپ && axes_array)
    {
      hb_array_t<ثابت Axis_رڪارڊ> arr = get_axes ().sub_array (شروعات_وٿي, axes_ڳڻپ);
      لكل (بنانشان i = 0; i < arr.ڊيگهائي; ++i)
	arr[i].get_axis_info (شروعات_وٿي + i, &axes_array[i]);
    }
    أرجع axis_ڳڻپ_2;
  }

#جيڪڏهن_وصف_نه_ٿيل HB_DISABLE_DEPRECATED
  منطقي
  ڳوليو_axis_deprecated (hb_ٽيگ_t ٽيگ_2, بنانشان *axis_اشاريو, hb_ot_var_axis_t *info) ثابت
  {
    بنانشان i;
    إذا (!axis_اشاريو) axis_اشاريو = &i;
    *axis_اشاريو = HB_OT_VAR_NO_AXIS_اشاريو;
    خودڪار axes = get_axes ();
    أرجع axes.lfind (ٽيگ_2, axis_اشاريو) && ((فراغ) axes[*axis_اشاريو].get_axis_deprecated (info), سچ);
  }
#جيڪڏهن_پورو
  منطقي
  ڳوليو_axis_info (hb_ٽيگ_t ٽيگ_2, hb_ot_var_axis_info_t *info) ثابت
  {
    بنانشان i;
    خودڪار axes = get_axes ();
    أرجع axes.lfind (ٽيگ_2, &i) && ((فراغ) axes[i].get_axis_info (i, info), سچ);
  }

  عائم normalize_axis_قدر (بنانشان صحيح axis_اشاريو, عائم v) ثابت
  { أرجع get_axes ()[axis_اشاريو].normalize_axis_قدر (v); }

  بنانشان صحيح get_instance_ڳڻپ () ثابت { أرجع instance_ڳڻپ; }

  hb_ot_name_id_t get_instance_subfamily_name_id (بنانشان صحيح instance_اشاريو) ثابت
  {
    ثابت Instance_رڪارڊ *instance = get_instance (instance_اشاريو);
    إذا (unlikely (!instance)) أرجع HB_OT_NAME_ID_INVALID;
    أرجع instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (بنانشان صحيح instance_اشاريو) ثابت
  {
    ثابت Instance_رڪارڊ *instance = get_instance (instance_اشاريو);
    إذا (unlikely (!instance)) أرجع HB_OT_NAME_ID_INVALID;
    إذا (instance_سائيز >= axis_ڳڻپ_2 * 4 + 6)
      أرجع StructAfter<NameID> (instance->get_coordinates (axis_ڳڻپ_2));
    أرجع HB_OT_NAME_ID_INVALID;
  }

  بنانشان صحيح get_instance_coords (بنانشان صحيح  instance_اشاريو,
				    بنانشان صحيح *coords_ڊيگهائي, /* IN/OUT */
				    عائم        *coords         /*ٻاهريون*/) ثابت
  {
    ثابت Instance_رڪارڊ *instance = get_instance (instance_اشاريو);
    إذا (unlikely (!instance))
    {
      إذا (coords_ڊيگهائي)
	*coords_ڊيگهائي = 0;
      أرجع 0;
    }

    إذا (coords_ڊيگهائي && *coords_ڊيگهائي && coords)
    {
      hb_array_t<ثابت F16DOT16> instanceCoords = instance->get_coordinates (axis_ڳڻپ_2)
							 .sub_array (0, coords_ڊيگهائي);
      لكل (بنانشان صحيح i = 0; i < instanceCoords.ڊيگهائي; i++)
	coords[i] = instanceCoords.arrayZ[i].منزل_float ();
    }
    أرجع axis_ڳڻپ_2;
  }

  فراغ collect_name_ids (hb_hashmap_t<hb_ٽيگ_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_اشاريو_ٽيگ_map,
			 hb_سيٽ_t *nameids  /* IN/OUT */) ثابت
  {
    إذا (!has_معلومات ()) أرجع;

    خودڪار axis_رڪارڊون = get_axes ();
    لكل (بنانشان i = 0 ; i < (بنانشان)axis_ڳڻپ_2; i++)
    {
      hb_ٽيگ_t axis_ٽيگ_2 = axis_رڪارڊون[i].get_axis_ٽيگ ();
      إذا (user_axes_location->has (axis_ٽيگ_2) &&
          user_axes_location->get (axis_ٽيگ_2).is_نقطو ())
        استمر;

      nameids->جوڙيو (axis_رڪارڊون[i].get_name_id ());
    }

    لكل (بنانشان i = 0 ; i < (بنانشان)instance_ڳڻپ; i++)
    {
      ثابت Instance_رڪارڊ *instance = get_instance (i);

      إذا (!instance->keep_instance (axis_ڳڻپ_2, axes_old_اشاريو_ٽيگ_map, user_axes_location))
        استمر;

      nameids->جوڙيو (instance->subfamilyNameID);

      إذا (instance_سائيز >= axis_ڳڻپ_2 * 4 + 6)
      {
        بنانشان post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_ڳڻپ_2));
        إذا (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->جوڙيو (post_script_name_id);
      }
    }
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بنانشان retained_axis_ڳڻپ = c->plan->axes_اشاريو_map.get_population ();
    إذا (!retained_axis_ڳڻپ) //all axes are pinned
      return_trace (خطأ);

    fvar *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (!c->serializer->check_سونپيو (ٻاهريون->axis_ڳڻپ_2, retained_axis_ڳڻپ, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خطأ);

    منطقي has_postscript_nameid = خطأ;
    إذا (instance_سائيز >= axis_ڳڻپ_2 * 4 + 6)
      has_postscript_nameid = سچ;

    إذا (!c->serializer->check_سونپيو (ٻاهريون->instance_سائيز, retained_axis_ڳڻپ * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خطأ);

    خودڪار axes_رڪارڊون = get_axes ();
    لكل (بنانشان i = 0 ; i < (بنانشان)axis_ڳڻپ_2; i++)
    {
      إذا (!c->plan->axes_اشاريو_map.has (i)) استمر;
      إذا (unlikely (!axes_رڪارڊون[i].subset (c)))
        return_trace (خطأ);
    }

    إذا (!c->serializer->check_سونپيو (ٻاهريون->پهريون_Axis, get_سائيز (), HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خطأ);

    بنانشان انگ_retained_instances = 0;
    لكل (بنانشان i = 0 ; i < (بنانشان)instance_ڳڻپ; i++)
    {
      ثابت Instance_رڪارڊ *instance = get_instance (i);
      خودڪار snap = c->serializer->snapshot ();
      إذا (!instance->subset (c, axis_ڳڻپ_2, has_postscript_nameid))
        c->serializer->revert (snap);
      وإلا
        انگ_retained_instances++;
    }

    return_trace (c->serializer->check_سونپيو (ٻاهريون->instance_ڳڻپ, انگ_retained_instances, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  عام:
  hb_array_t<ثابت Axis_رڪارڊ> get_axes () ثابت
  { أرجع hb_array (&(هذا+پهريون_Axis), axis_ڳڻپ_2); }

  ثابت Instance_رڪارڊ *get_instance (بنانشان صحيح i) ثابت
  {
    إذا (unlikely (i >= instance_ڳڻپ)) أرجع خالي_اشارو;
   أرجع &Struct_At_وٿي<Instance_رڪارڊ> (&StructAfter<Instance_رڪارڊ> (get_axes ()),
					   i * instance_سائيز);
  }

  محمي:
  Fixed_نسخو<>نسخو;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16_منزل<Axis_رڪارڊ>
		پهريون_Axis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_ڳڻپ_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axis_سائيز;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_ڳڻپ;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instance_سائيز;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  عام:
  DEFINE_سائيز_STATIC (16);
};

} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_VAR_FVAR_TABLE_HH */
