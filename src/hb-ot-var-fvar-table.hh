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

#اگر_غیر_تعریف_شدہ HB_OT_VAR_FVAR_جدول_HH
#تعریف_کرو HB_OT_VAR_FVAR_جدول_HH

#شامل "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#تعریف_کرو HB_OT_لیبل_fvar HB_لیبل('f','v','a','r')


نامخلا OT {

ساکن منطقی axis_coord_pinned_or_within_axis_range (ثابت hb_array_t<ثابت F16DOT16> coords,
                                                    غیرعلامتی axis_اشاریہ,
                                                    Triple axis_حد_بندی)
{
  اعشاری axis_coord = coords[axis_اشاریہ].مقصد_float ();
  اگر (axis_حد_بندی.is_نقطہ ())
  {
    اگر (axis_حد_بندی.minimum != axis_coord)
      واپس جھوٹ;
  }
  ورنہ
  {
    اگر (axis_coord < axis_حد_بندی.minimum ||
        axis_coord > axis_حد_بندی.زیادہ_سے_زیادہ)
      واپس جھوٹ;
  }
  واپس سچ;
}

ساخت Instance_ریکارڈ
{
  دوست ساخت fvar;

  hb_array_t<ثابت F16DOT16> get_coordinates (غیرعلامتی صحیح axis_گنتی) ثابت
  { واپس coordinatesZ.as_array (axis_گنتی); }

  منطقی keep_instance (غیرعلامتی axis_گنتی,
                      ثابت hb_map_t *axes_اشاریہ_لیبل_map,
                      ثابت hb_hashmap_t<hb_لیبل_t, Triple> *axes_location) ثابت
  {
    اگر (axes_location->is_empty ()) واپس سچ;
    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_گنتی);
    برائے (غیرعلامتی i = 0 ; i < axis_گنتی; i++)
    {
      uint32_t *axis_لیبل_2;
      اگر (!axes_اشاریہ_لیبل_map->has (i, &axis_لیبل_2))
        واپس جھوٹ;
      اگر (!axes_location->has (*axis_لیبل_2))
        جاری;

      Triple axis_حد_بندی = axes_location->get (*axis_لیبل_2);
      اگر (!axis_coord_pinned_or_within_axis_range (coords, i, axis_حد_بندی))
        واپس جھوٹ;
    }
    واپس سچ;
  }

  منطقی subset (hb_subset_context_t *c,
               غیرعلامتی axis_گنتی,
               منطقی has_postscript_nameid) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (جھوٹ);
    اگر (unlikely (!c->serializer->embed (flags))) return_trace (جھوٹ);

    ثابت hb_array_t<ثابت F16DOT16> coords = get_coordinates (axis_گنتی);
    ثابت hb_hashmap_t<hb_لیبل_t, Triple> *axes_location = &c->plan->user_axes_location;
    برائے (غیرعلامتی i = 0 ; i < axis_گنتی; i++)
    {
      uint32_t *axis_لیبل_2;
      Triple *axis_حد_بندی;
      // only keep instances whose coordinates == pinned axis location
      اگر (!c->plan->axes_old_اشاریہ_لیبل_map.has (i, &axis_لیبل_2)) return_trace (جھوٹ);
      اگر (axes_location->has (*axis_لیبل_2, &axis_حد_بندی))
      {
        اگر (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_حد_بندی))
          return_trace (جھوٹ);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        اگر (axis_حد_بندی->is_نقطہ () && !c->plan->axes_اشاریہ_map.has (i))
          جاری;
      }

      اگر (!c->serializer->embed (coords[i]))
        return_trace (جھوٹ);
    }

    اگر (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      اگر (!c->serializer->embed (name_id))
        return_trace (جھوٹ);
    }

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح axis_گنتی) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_گنتی));
  }

  محفوظ:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  Unsized_Array_کا<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  عوامی:
  DEFINE_سائز_UNBOUNDED (4);
};

ساخت Axis_ریکارڈ
{
  صحیح cmp (hb_لیبل_t key) ثابت { واپس axis_لیبل.cmp (key); }

  شمار
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED
  خالی get_axis_deprecated (hb_ot_var_axis_t *info) ثابت
  {
    info->لیبل_2 = axis_لیبل;
    info->name_id = axisNameID;
    get_coordinates (info->min_قدر, info->default_قدر, info->max_قدر);
  }
#اگر_ختم

  خالی get_axis_info (غیرعلامتی axis_اشاریہ, hb_ot_var_axis_info_t *info) ثابت
  {
    info->axis_اشاریہ = axis_اشاریہ;
    info->لیبل_2 = axis_لیبل;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (غیرعلامتی صحیح) flags;
    get_coordinates (info->min_قدر, info->default_قدر, info->max_قدر);
    info->reserved = 0;
  }

  hb_لیبل_t get_axis_لیبل () ثابت { واپس axis_لیبل; }

  منطقی is_hidden () ثابت { واپس flags & AXIS_FLAG_HIDDEN; }

  اعشاری normalize_axis_قدر (اعشاری v) ثابت
  {
    اعشاری min_قدر, default_قدر, max_قدر;
    get_coordinates (min_قدر, default_قدر, max_قدر);

    v = hb_clamp (v, min_قدر, max_قدر);

    اگر (v == default_قدر)
      واپس 0;
    ورنہ اگر (v < default_قدر)
      واپس (v - default_قدر) / (default_قدر - min_قدر);
    ورنہ
      واپس (v - default_قدر) / (max_قدر - default_قدر);
  }

  hb_ot_name_id_t get_name_id () ثابت { واپس axisNameID; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  خالی get_coordinates (اعشاری &min, اعشاری &default_, اعشاری &max) ثابت
  {
    default_ = default_قدر_2.مقصد_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_قدر_2.مقصد_float ());
    max = hb_max (default_, max_قدر_2.مقصد_float ());
  }

  اعشاری get_default () ثابت
  {
    واپس default_قدر_2.مقصد_float ();
  }

  TripleDistances get_triple_distances () ثابت
  {
    اعشاری min, default_, max;
    get_coordinates (min, default_, max);
    واپس TripleDistances (min, default_, max);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    ثابت hb_hashmap_t<hb_لیبل_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_حد_بندی;
    اگر (user_axes_location.has (axis_لیبل, &axis_حد_بندی))
    {
      اگر (c->plan->has_avar2 && axis_حد_بندی->is_نقطہ ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        برآمد->min_قدر_2.سیٹ_float (axis_حد_بندی->درمیان);
        برآمد->default_قدر_2.سیٹ_float (axis_حد_بندی->درمیان);
        برآمد->max_قدر_2.سیٹ_float (axis_حد_بندی->درمیان);
        برآمد->flags = برآمد->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      ورنہ
      {
        برآمد->min_قدر_2.سیٹ_float (axis_حد_بندی->minimum);
        برآمد->default_قدر_2.سیٹ_float (axis_حد_بندی->درمیان);
        برآمد->max_قدر_2.سیٹ_float (axis_حد_بندی->زیادہ_سے_زیادہ);
      }
    }
    return_trace (سچ);
  }

  عوامی:
  لیبل		axis_لیبل;	/* Tag identifying the design variation for the axis. */
  محفوظ:
  F16DOT16	min_قدر_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_قدر_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_قدر_2;	/* The maximum coordinate value for the axis. */
  عوامی:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  عوامی:
  DEFINE_سائز_STATIC (20);
};

ساخت fvar
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_fvar;

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.مقصد_int (); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  c->check_struct (یہ) &&
		  hb_barrier () &&
		  axis_سائز == 20 && /* Assumed in our code. */
		  instance_سائز >= axis_گنتی_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<Instance_ریکارڈ> (get_axes ()),
				  instance_گنتی, instance_سائز));
  }

  غیرعلامتی صحیح get_axis_گنتی () ثابت { واپس axis_گنتی_2; }

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED
  غیرعلامتی صحیح get_axes_deprecated (غیرعلامتی صحیح      آغاز_انحراف,
				    غیرعلامتی صحیح     *axes_گنتی /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /*برآمد*/) ثابت
  {
    اگر (axes_گنتی && axes_array)
    {
      hb_array_t<ثابت Axis_ریکارڈ> arr = get_axes ().sub_array (آغاز_انحراف, axes_گنتی);
      برائے (غیرعلامتی i = 0; i < arr.لمبائی; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    واپس axis_گنتی_2;
  }
#اگر_ختم

  غیرعلامتی صحیح get_axis_infos (غیرعلامتی صحیح           آغاز_انحراف,
			       غیرعلامتی صحیح          *axes_گنتی /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /*برآمد*/) ثابت
  {
    اگر (axes_گنتی && axes_array)
    {
      hb_array_t<ثابت Axis_ریکارڈ> arr = get_axes ().sub_array (آغاز_انحراف, axes_گنتی);
      برائے (غیرعلامتی i = 0; i < arr.لمبائی; ++i)
	arr[i].get_axis_info (آغاز_انحراف + i, &axes_array[i]);
    }
    واپس axis_گنتی_2;
  }

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED
  منطقی
  ڈھونڈو_axis_deprecated (hb_لیبل_t لیبل_2, غیرعلامتی *axis_اشاریہ, hb_ot_var_axis_t *info) ثابت
  {
    غیرعلامتی i;
    اگر (!axis_اشاریہ) axis_اشاریہ = &i;
    *axis_اشاریہ = HB_OT_VAR_NO_AXIS_اشاریہ;
    خودکار axes = get_axes ();
    واپس axes.lfind (لیبل_2, axis_اشاریہ) && ((خالی) axes[*axis_اشاریہ].get_axis_deprecated (info), سچ);
  }
#اگر_ختم
  منطقی
  ڈھونڈو_axis_info (hb_لیبل_t لیبل_2, hb_ot_var_axis_info_t *info) ثابت
  {
    غیرعلامتی i;
    خودکار axes = get_axes ();
    واپس axes.lfind (لیبل_2, &i) && ((خالی) axes[i].get_axis_info (i, info), سچ);
  }

  اعشاری normalize_axis_قدر (غیرعلامتی صحیح axis_اشاریہ, اعشاری v) ثابت
  { واپس get_axes ()[axis_اشاریہ].normalize_axis_قدر (v); }

  غیرعلامتی صحیح get_instance_گنتی () ثابت { واپس instance_گنتی; }

  hb_ot_name_id_t get_instance_subfamily_name_id (غیرعلامتی صحیح instance_اشاریہ) ثابت
  {
    ثابت Instance_ریکارڈ *instance = get_instance (instance_اشاریہ);
    اگر (unlikely (!instance)) واپس HB_OT_NAME_ID_INVALID;
    واپس instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (غیرعلامتی صحیح instance_اشاریہ) ثابت
  {
    ثابت Instance_ریکارڈ *instance = get_instance (instance_اشاریہ);
    اگر (unlikely (!instance)) واپس HB_OT_NAME_ID_INVALID;
    اگر (instance_سائز >= axis_گنتی_2 * 4 + 6)
      واپس StructAfter<NameID> (instance->get_coordinates (axis_گنتی_2));
    واپس HB_OT_NAME_ID_INVALID;
  }

  غیرعلامتی صحیح get_instance_coords (غیرعلامتی صحیح  instance_اشاریہ,
				    غیرعلامتی صحیح *coords_لمبائی, /* IN/OUT */
				    اعشاری        *coords         /*برآمد*/) ثابت
  {
    ثابت Instance_ریکارڈ *instance = get_instance (instance_اشاریہ);
    اگر (unlikely (!instance))
    {
      اگر (coords_لمبائی)
	*coords_لمبائی = 0;
      واپس 0;
    }

    اگر (coords_لمبائی && *coords_لمبائی && coords)
    {
      hb_array_t<ثابت F16DOT16> instanceCoords = instance->get_coordinates (axis_گنتی_2)
							 .sub_array (0, coords_لمبائی);
      برائے (غیرعلامتی صحیح i = 0; i < instanceCoords.لمبائی; i++)
	coords[i] = instanceCoords.arrayZ[i].مقصد_float ();
    }
    واپس axis_گنتی_2;
  }

  خالی collect_name_ids (hb_hashmap_t<hb_لیبل_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_اشاریہ_لیبل_map,
			 hb_سیٹ_t *nameids  /* IN/OUT */) ثابت
  {
    اگر (!has_ڈیٹا ()) واپس;

    خودکار axis_اندراجات = get_axes ();
    برائے (غیرعلامتی i = 0 ; i < (غیرعلامتی)axis_گنتی_2; i++)
    {
      hb_لیبل_t axis_لیبل_2 = axis_اندراجات[i].get_axis_لیبل ();
      اگر (user_axes_location->has (axis_لیبل_2) &&
          user_axes_location->get (axis_لیبل_2).is_نقطہ ())
        جاری;

      nameids->جوڑو (axis_اندراجات[i].get_name_id ());
    }

    برائے (غیرعلامتی i = 0 ; i < (غیرعلامتی)instance_گنتی; i++)
    {
      ثابت Instance_ریکارڈ *instance = get_instance (i);

      اگر (!instance->keep_instance (axis_گنتی_2, axes_old_اشاریہ_لیبل_map, user_axes_location))
        جاری;

      nameids->جوڑو (instance->subfamilyNameID);

      اگر (instance_سائز >= axis_گنتی_2 * 4 + 6)
      {
        غیرعلامتی post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_گنتی_2));
        اگر (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->جوڑو (post_script_name_id);
      }
    }
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    غیرعلامتی retained_axis_گنتی = c->plan->axes_اشاریہ_map.get_population ();
    اگر (!retained_axis_گنتی) //all axes are pinned
      return_trace (جھوٹ);

    fvar *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (!c->serializer->check_تفویض (برآمد->axis_گنتی_2, retained_axis_گنتی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);

    منطقی has_postscript_nameid = جھوٹ;
    اگر (instance_سائز >= axis_گنتی_2 * 4 + 6)
      has_postscript_nameid = سچ;

    اگر (!c->serializer->check_تفویض (برآمد->instance_سائز, retained_axis_گنتی * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);

    خودکار axes_اندراجات = get_axes ();
    برائے (غیرعلامتی i = 0 ; i < (غیرعلامتی)axis_گنتی_2; i++)
    {
      اگر (!c->plan->axes_اشاریہ_map.has (i)) جاری;
      اگر (unlikely (!axes_اندراجات[i].subset (c)))
        return_trace (جھوٹ);
    }

    اگر (!c->serializer->check_تفویض (برآمد->پہلا_Axis, get_سائز (), HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);

    غیرعلامتی عدد_retained_instances = 0;
    برائے (غیرعلامتی i = 0 ; i < (غیرعلامتی)instance_گنتی; i++)
    {
      ثابت Instance_ریکارڈ *instance = get_instance (i);
      خودکار snap = c->serializer->snapshot ();
      اگر (!instance->subset (c, axis_گنتی_2, has_postscript_nameid))
        c->serializer->revert (snap);
      ورنہ
        عدد_retained_instances++;
    }

    return_trace (c->serializer->check_تفویض (برآمد->instance_گنتی, عدد_retained_instances, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  عوامی:
  hb_array_t<ثابت Axis_ریکارڈ> get_axes () ثابت
  { واپس hb_array (&(یہ+پہلا_Axis), axis_گنتی_2); }

  ثابت Instance_ریکارڈ *get_instance (غیرعلامتی صحیح i) ثابت
  {
    اگر (unlikely (i >= instance_گنتی)) واپس خالی_اشاریہ;
   واپس &Struct_At_انحراف<Instance_ریکارڈ> (&StructAfter<Instance_ریکارڈ> (get_axes ()),
					   i * instance_سائز);
  }

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16_مقصد<Axis_ریکارڈ>
		پہلا_Axis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_گنتی_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axis_سائز;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_گنتی;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instance_سائز;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  عوامی:
  DEFINE_سائز_STATIC (16);
};

} /* namespace OT */


#اگر_ختم /* HB_OT_VAR_FVAR_TABLE_HH */
