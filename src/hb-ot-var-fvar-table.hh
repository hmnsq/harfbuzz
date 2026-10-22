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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_VAR_FVAR_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_VAR_FVAR_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"

/*
 * fvar -- Font Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/fvar
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_fvar HB_ਟੈਗ('f','v','a','r')


ਨਾਮਾਕਾਸ਼ OT {

ਸਥਿਰ ਤਾਰਕਿਕ axis_coord_pinned_or_within_axis_range (ਅਚਲ hb_array_t<ਅਚਲ F16DOT16> coords,
                                                    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਸੂਚਕਾਂਕ,
                                                    Triple axis_ਹੱਦਬੰਦੀ)
{
  ਦਸ਼ਮਲਵ axis_coord = coords[axis_ਸੂਚਕਾਂਕ].ਮੰਜ਼ਿਲ_float ();
  ਜੇ (axis_ਹੱਦਬੰਦੀ.is_ਬਿੰਦੂ ())
  {
    ਜੇ (axis_ਹੱਦਬੰਦੀ.minimum != axis_coord)
      ਵਾਪਸ ਝੂਠ;
  }
  ਨਹੀਂਤਾਂ
  {
    ਜੇ (axis_coord < axis_ਹੱਦਬੰਦੀ.minimum ||
        axis_coord > axis_ਹੱਦਬੰਦੀ.ਵੱਧ_ਤੋਂ_ਵੱਧ)
      ਵਾਪਸ ਝੂਠ;
  }
  ਵਾਪਸ ਸੱਚ;
}

ਬਣਤਰ Instance_ਰਿਕਾਰਡ
{
  ਮਿੱਤਰ ਬਣਤਰ fvar;

  hb_array_t<ਅਚਲ F16DOT16> get_coordinates (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ axis_ਗਿਣਤੀ) ਅਚਲ
  { ਵਾਪਸ coordinatesZ.as_array (axis_ਗਿਣਤੀ); }

  ਤਾਰਕਿਕ keep_instance (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ,
                      ਅਚਲ hb_map_t *axes_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                      ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> *axes_location) ਅਚਲ
  {
    ਜੇ (axes_location->is_empty ()) ਵਾਪਸ ਸੱਚ;
    ਅਚਲ hb_array_t<ਅਚਲ F16DOT16> coords = get_coordinates (axis_ਗਿਣਤੀ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0 ; i < axis_ਗਿਣਤੀ; i++)
    {
      uint32_t *axis_ਟੈਗ_2;
      ਜੇ (!axes_ਸੂਚਕਾਂਕ_ਟੈਗ_map->has (i, &axis_ਟੈਗ_2))
        ਵਾਪਸ ਝੂਠ;
      ਜੇ (!axes_location->has (*axis_ਟੈਗ_2))
        ਜਾਰੀ;

      Triple axis_ਹੱਦਬੰਦੀ = axes_location->get (*axis_ਟੈਗ_2);
      ਜੇ (!axis_coord_pinned_or_within_axis_range (coords, i, axis_ਹੱਦਬੰਦੀ))
        ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ,
               ਤਾਰਕਿਕ has_postscript_nameid) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (unlikely (!c->serializer->embed (subfamilyNameID))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->serializer->embed (flags))) return_trace (ਝੂਠ);

    ਅਚਲ hb_array_t<ਅਚਲ F16DOT16> coords = get_coordinates (axis_ਗਿਣਤੀ);
    ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple> *axes_location = &c->plan->user_axes_location;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0 ; i < axis_ਗਿਣਤੀ; i++)
    {
      uint32_t *axis_ਟੈਗ_2;
      Triple *axis_ਹੱਦਬੰਦੀ;
      // only keep instances whose coordinates == pinned axis location
      ਜੇ (!c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.has (i, &axis_ਟੈਗ_2)) return_trace (ਝੂਠ);
      ਜੇ (axes_location->has (*axis_ਟੈਗ_2, &axis_ਹੱਦਬੰਦੀ))
      {
        ਜੇ (!axis_coord_pinned_or_within_axis_range (coords, i, *axis_ਹੱਦਬੰਦੀ))
          return_trace (ਝੂਠ);

        //skip axes dropped from fvar (pinned; or self-contained under avar2,
        //where other pinned axes stay in fvar as hidden)
        ਜੇ (axis_ਹੱਦਬੰਦੀ->is_ਬਿੰਦੂ () && !c->plan->axes_ਸੂਚਕਾਂਕ_map.has (i))
          ਜਾਰੀ;
      }

      ਜੇ (!c->serializer->embed (coords[i]))
        return_trace (ਝੂਠ);
    }

    ਜੇ (has_postscript_nameid)
    {
      NameID name_id;
      name_id = StructAfter<NameID> (coords);
      ਜੇ (!c->serializer->embed (name_id))
        return_trace (ਝੂਠ);
    }

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ axis_ਗਿਣਤੀ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  c->check_array (coordinatesZ.arrayZ, axis_ਗਿਣਤੀ));
  }

  ਸੁਰੱਖਿਅਤ:
  NameID	subfamilyNameID;/* The name ID for entries in the 'name' table
				 * that provide subfamily names for this instance. */
  HBUINT16	flags;		/* Reserved for future use — set to 0. */
  Unsized_Array_ਦਾ<F16DOT16>
		coordinatesZ;	/* The coordinates array for this instance. */
  //NameID	postScriptNameIDX;/*Optional. The name ID for entries in the 'name'
  //				  * table that provide PostScript names for this
  //				  * instance. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNBOUNDED (4);
};

ਬਣਤਰ Axis_ਰਿਕਾਰਡ
{
  ਪੂਰਨ cmp (hb_ਟੈਗ_t key) ਅਚਲ { ਵਾਪਸ axis_ਟੈਗ.cmp (key); }

  ਗਿਣਤੀ
  {
    AXIS_FLAG_HIDDEN	= 0x0001,
  };

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
  ਖਾਲੀ get_axis_deprecated (hb_ot_var_axis_t *info) ਅਚਲ
  {
    info->ਟੈਗ_2 = axis_ਟੈਗ;
    info->name_id = axisNameID;
    get_coordinates (info->min_ਮੁੱਲ, info->default_ਮੁੱਲ, info->max_ਮੁੱਲ);
  }
#ਜੇ_ਅੰਤ

  ਖਾਲੀ get_axis_info (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਸੂਚਕਾਂਕ, hb_ot_var_axis_info_t *info) ਅਚਲ
  {
    info->axis_ਸੂਚਕਾਂਕ = axis_ਸੂਚਕਾਂਕ;
    info->ਟੈਗ_2 = axis_ਟੈਗ;
    info->name_id = axisNameID;
    info->flags = (hb_ot_var_axis_flags_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) flags;
    get_coordinates (info->min_ਮੁੱਲ, info->default_ਮੁੱਲ, info->max_ਮੁੱਲ);
    info->reserved = 0;
  }

  hb_ਟੈਗ_t get_axis_ਟੈਗ () ਅਚਲ { ਵਾਪਸ axis_ਟੈਗ; }

  ਤਾਰਕਿਕ is_hidden () ਅਚਲ { ਵਾਪਸ flags & AXIS_FLAG_HIDDEN; }

  ਦਸ਼ਮਲਵ normalize_axis_ਮੁੱਲ (ਦਸ਼ਮਲਵ v) ਅਚਲ
  {
    ਦਸ਼ਮਲਵ min_ਮੁੱਲ, default_ਮੁੱਲ, max_ਮੁੱਲ;
    get_coordinates (min_ਮੁੱਲ, default_ਮੁੱਲ, max_ਮੁੱਲ);

    v = hb_clamp (v, min_ਮੁੱਲ, max_ਮੁੱਲ);

    ਜੇ (v == default_ਮੁੱਲ)
      ਵਾਪਸ 0;
    ਨਹੀਂਤਾਂ ਜੇ (v < default_ਮੁੱਲ)
      ਵਾਪਸ (v - default_ਮੁੱਲ) / (default_ਮੁੱਲ - min_ਮੁੱਲ);
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ (v - default_ਮੁੱਲ) / (max_ਮੁੱਲ - default_ਮੁੱਲ);
  }

  hb_ot_name_id_t get_name_id () ਅਚਲ { ਵਾਪਸ axisNameID; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ get_coordinates (ਦਸ਼ਮਲਵ &min, ਦਸ਼ਮਲਵ &default_, ਦਸ਼ਮਲਵ &max) ਅਚਲ
  {
    default_ = default_ਮੁੱਲ_2.ਮੰਜ਼ਿਲ_float ();
    /* Ensure order, to simplify client math. */
    min = hb_min (default_, min_ਮੁੱਲ_2.ਮੰਜ਼ਿਲ_float ());
    max = hb_max (default_, max_ਮੁੱਲ_2.ਮੰਜ਼ਿਲ_float ());
  }

  ਦਸ਼ਮਲਵ get_default () ਅਚਲ
  {
    ਵਾਪਸ default_ਮੁੱਲ_2.ਮੰਜ਼ਿਲ_float ();
  }

  TripleDistances get_triple_distances () ਅਚਲ
  {
    ਦਸ਼ਮਲਵ min, default_, max;
    get_coordinates (min, default_, max);
    ਵਾਪਸ TripleDistances (min, default_, max);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>& user_axes_location = c->plan->user_axes_location;
    Triple *axis_ਹੱਦਬੰਦੀ;
    ਜੇ (user_axes_location.has (axis_ਟੈਗ, &axis_ਹੱਦਬੰਦੀ))
    {
      ਜੇ (c->plan->has_avar2 && axis_ਹੱਦਬੰਦੀ->is_ਬਿੰਦੂ ())
      {
        /* Pinned axis in avar2 mode: keep in fvar as hidden with
         * min=default=max=pinned_value. */
        ਬਾਹਰਲਾ->min_ਮੁੱਲ_2.ਸੈੱਟ_float (axis_ਹੱਦਬੰਦੀ->ਵਿਚਕਾਰ);
        ਬਾਹਰਲਾ->default_ਮੁੱਲ_2.ਸੈੱਟ_float (axis_ਹੱਦਬੰਦੀ->ਵਿਚਕਾਰ);
        ਬਾਹਰਲਾ->max_ਮੁੱਲ_2.ਸੈੱਟ_float (axis_ਹੱਦਬੰਦੀ->ਵਿਚਕਾਰ);
        ਬਾਹਰਲਾ->flags = ਬਾਹਰਲਾ->flags | (HBUINT16) AXIS_FLAG_HIDDEN;
      }
      ਨਹੀਂਤਾਂ
      {
        ਬਾਹਰਲਾ->min_ਮੁੱਲ_2.ਸੈੱਟ_float (axis_ਹੱਦਬੰਦੀ->minimum);
        ਬਾਹਰਲਾ->default_ਮੁੱਲ_2.ਸੈੱਟ_float (axis_ਹੱਦਬੰਦੀ->ਵਿਚਕਾਰ);
        ਬਾਹਰਲਾ->max_ਮੁੱਲ_2.ਸੈੱਟ_float (axis_ਹੱਦਬੰਦੀ->ਵੱਧ_ਤੋਂ_ਵੱਧ);
      }
    }
    return_trace (ਸੱਚ);
  }

  ਜਨਤਕ:
  ਟੈਗ		axis_ਟੈਗ;	/* Tag identifying the design variation for the axis. */
  ਸੁਰੱਖਿਅਤ:
  F16DOT16	min_ਮੁੱਲ_2;	/* The minimum coordinate value for the axis. */
  F16DOT16	default_ਮੁੱਲ_2;	/* The default coordinate value for the axis. */
  F16DOT16	max_ਮੁੱਲ_2;	/* The maximum coordinate value for the axis. */
  ਜਨਤਕ:
  HBUINT16	flags;		/* Axis flags. */
  NameID	axisNameID;	/* The name ID for entries in the 'name' table that
				 * provide a display name for this axis. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (20);
};

ਬਣਤਰ fvar
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_fvar;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int (); }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਸੰਸਕਰਣ.sanitize (c) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 1) &&
		  c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  axis_ਮਾਪ == 20 && /* Assumed in our code. */
		  instance_ਮਾਪ >= axis_ਗਿਣਤੀ_2 * 4 + 4 &&
		  get_axes ().sanitize (c) &&
		  c->check_range (&StructAfter<Instance_ਰਿਕਾਰਡ> (get_axes ()),
				  instance_ਗਿਣਤੀ, instance_ਮਾਪ));
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_axis_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ axis_ਗਿਣਤੀ_2; }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_axes_deprecated (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ      ਸ਼ੁਰੂ_ਵਿੱਥ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ     *axes_ਗਿਣਤੀ /* IN/OUT */,
				    hb_ot_var_axis_t *axes_array /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (axes_ਗਿਣਤੀ && axes_array)
    {
      hb_array_t<ਅਚਲ Axis_ਰਿਕਾਰਡ> arr = get_axes ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, axes_ਗਿਣਤੀ);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < arr.ਲੰਮਾਈ; ++i)
	arr[i].get_axis_deprecated (&axes_array[i]);
    }
    ਵਾਪਸ axis_ਗਿਣਤੀ_2;
  }
#ਜੇ_ਅੰਤ

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_axis_infos (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ           ਸ਼ੁਰੂ_ਵਿੱਥ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          *axes_ਗਿਣਤੀ /* IN/OUT */,
			       hb_ot_var_axis_info_t *axes_array /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (axes_ਗਿਣਤੀ && axes_array)
    {
      hb_array_t<ਅਚਲ Axis_ਰਿਕਾਰਡ> arr = get_axes ().sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, axes_ਗਿਣਤੀ);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < arr.ਲੰਮਾਈ; ++i)
	arr[i].get_axis_info (ਸ਼ੁਰੂ_ਵਿੱਥ + i, &axes_array[i]);
    }
    ਵਾਪਸ axis_ਗਿਣਤੀ_2;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
  ਤਾਰਕਿਕ
  ਲੱਭੋ_axis_deprecated (hb_ਟੈਗ_t ਟੈਗ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ *axis_ਸੂਚਕਾਂਕ, hb_ot_var_axis_t *info) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ i;
    ਜੇ (!axis_ਸੂਚਕਾਂਕ) axis_ਸੂਚਕਾਂਕ = &i;
    *axis_ਸੂਚਕਾਂਕ = HB_OT_VAR_NO_AXIS_ਸੂਚਕਾਂਕ;
    ਸਵੈਚਾਲਿਤ axes = get_axes ();
    ਵਾਪਸ axes.lfind (ਟੈਗ_2, axis_ਸੂਚਕਾਂਕ) && ((ਖਾਲੀ) axes[*axis_ਸੂਚਕਾਂਕ].get_axis_deprecated (info), ਸੱਚ);
  }
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ
  ਲੱਭੋ_axis_info (hb_ਟੈਗ_t ਟੈਗ_2, hb_ot_var_axis_info_t *info) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ i;
    ਸਵੈਚਾਲਿਤ axes = get_axes ();
    ਵਾਪਸ axes.lfind (ਟੈਗ_2, &i) && ((ਖਾਲੀ) axes[i].get_axis_info (i, info), ਸੱਚ);
  }

  ਦਸ਼ਮਲਵ normalize_axis_ਮੁੱਲ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ axis_ਸੂਚਕਾਂਕ, ਦਸ਼ਮਲਵ v) ਅਚਲ
  { ਵਾਪਸ get_axes ()[axis_ਸੂਚਕਾਂਕ].normalize_axis_ਮੁੱਲ (v); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_instance_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ instance_ਗਿਣਤੀ; }

  hb_ot_name_id_t get_instance_subfamily_name_id (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instance_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਅਚਲ Instance_ਰਿਕਾਰਡ *instance = get_instance (instance_ਸੂਚਕਾਂਕ);
    ਜੇ (unlikely (!instance)) ਵਾਪਸ HB_OT_NAME_ID_INVALID;
    ਵਾਪਸ instance->subfamilyNameID;
  }

  hb_ot_name_id_t get_instance_postscript_name_id (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instance_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਅਚਲ Instance_ਰਿਕਾਰਡ *instance = get_instance (instance_ਸੂਚਕਾਂਕ);
    ਜੇ (unlikely (!instance)) ਵਾਪਸ HB_OT_NAME_ID_INVALID;
    ਜੇ (instance_ਮਾਪ >= axis_ਗਿਣਤੀ_2 * 4 + 6)
      ਵਾਪਸ StructAfter<NameID> (instance->get_coordinates (axis_ਗਿਣਤੀ_2));
    ਵਾਪਸ HB_OT_NAME_ID_INVALID;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_instance_coords (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  instance_ਸੂਚਕਾਂਕ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *coords_ਲੰਮਾਈ, /* IN/OUT */
				    ਦਸ਼ਮਲਵ        *coords         /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਅਚਲ Instance_ਰਿਕਾਰਡ *instance = get_instance (instance_ਸੂਚਕਾਂਕ);
    ਜੇ (unlikely (!instance))
    {
      ਜੇ (coords_ਲੰਮਾਈ)
	*coords_ਲੰਮਾਈ = 0;
      ਵਾਪਸ 0;
    }

    ਜੇ (coords_ਲੰਮਾਈ && *coords_ਲੰਮਾਈ && coords)
    {
      hb_array_t<ਅਚਲ F16DOT16> instanceCoords = instance->get_coordinates (axis_ਗਿਣਤੀ_2)
							 .sub_array (0, coords_ਲੰਮਾਈ);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < instanceCoords.ਲੰਮਾਈ; i++)
	coords[i] = instanceCoords.arrayZ[i].ਮੰਜ਼ਿਲ_float ();
    }
    ਵਾਪਸ axis_ਗਿਣਤੀ_2;
  }

  ਖਾਲੀ collect_name_ids (hb_hashmap_t<hb_ਟੈਗ_t, Triple> *user_axes_location,
			 hb_map_t *axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
			 hb_ਸੈੱਟ_t *nameids  /* IN/OUT */) ਅਚਲ
  {
    ਜੇ (!has_ਅੰਕੜੇ ()) ਵਾਪਸ;

    ਸਵੈਚਾਲਿਤ axis_ਰਿਕਾਰਡਾਂ = get_axes ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0 ; i < (ਬਿਨਾਨਿਸ਼ਾਨੀ)axis_ਗਿਣਤੀ_2; i++)
    {
      hb_ਟੈਗ_t axis_ਟੈਗ_2 = axis_ਰਿਕਾਰਡਾਂ[i].get_axis_ਟੈਗ ();
      ਜੇ (user_axes_location->has (axis_ਟੈਗ_2) &&
          user_axes_location->get (axis_ਟੈਗ_2).is_ਬਿੰਦੂ ())
        ਜਾਰੀ;

      nameids->ਜੋੜੋ (axis_ਰਿਕਾਰਡਾਂ[i].get_name_id ());
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0 ; i < (ਬਿਨਾਨਿਸ਼ਾਨੀ)instance_ਗਿਣਤੀ; i++)
    {
      ਅਚਲ Instance_ਰਿਕਾਰਡ *instance = get_instance (i);

      ਜੇ (!instance->keep_instance (axis_ਗਿਣਤੀ_2, axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, user_axes_location))
        ਜਾਰੀ;

      nameids->ਜੋੜੋ (instance->subfamilyNameID);

      ਜੇ (instance_ਮਾਪ >= axis_ਗਿਣਤੀ_2 * 4 + 6)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ post_script_name_id = StructAfter<NameID> (instance->get_coordinates (axis_ਗਿਣਤੀ_2));
        ਜੇ (post_script_name_id != HB_OT_NAME_ID_INVALID) nameids->ਜੋੜੋ (post_script_name_id);
      }
    }
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ retained_axis_ਗਿਣਤੀ = c->plan->axes_ਸੂਚਕਾਂਕ_map.get_population ();
    ਜੇ (!retained_axis_ਗਿਣਤੀ) //all axes are pinned
      return_trace (ਝੂਠ);

    fvar *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->axis_ਗਿਣਤੀ_2, retained_axis_ਗਿਣਤੀ, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ has_postscript_nameid = ਝੂਠ;
    ਜੇ (instance_ਮਾਪ >= axis_ਗਿਣਤੀ_2 * 4 + 6)
      has_postscript_nameid = ਸੱਚ;

    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->instance_ਮਾਪ, retained_axis_ਗਿਣਤੀ * 4 + (has_postscript_nameid ? 6 : 4),
                                      HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ axes_ਰਿਕਾਰਡਾਂ = get_axes ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0 ; i < (ਬਿਨਾਨਿਸ਼ਾਨੀ)axis_ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (!c->plan->axes_ਸੂਚਕਾਂਕ_map.has (i)) ਜਾਰੀ;
      ਜੇ (unlikely (!axes_ਰਿਕਾਰਡਾਂ[i].subset (c)))
        return_trace (ਝੂਠ);
    }

    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->ਪਹਿਲਾ_Axis, get_ਮਾਪ (), HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_retained_instances = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0 ; i < (ਬਿਨਾਨਿਸ਼ਾਨੀ)instance_ਗਿਣਤੀ; i++)
    {
      ਅਚਲ Instance_ਰਿਕਾਰਡ *instance = get_instance (i);
      ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
      ਜੇ (!instance->subset (c, axis_ਗਿਣਤੀ_2, has_postscript_nameid))
        c->serializer->revert (snap);
      ਨਹੀਂਤਾਂ
        ਅੰਕ_retained_instances++;
    }

    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->instance_ਗਿਣਤੀ, ਅੰਕ_retained_instances, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਜਨਤਕ:
  hb_array_t<ਅਚਲ Axis_ਰਿਕਾਰਡ> get_axes () ਅਚਲ
  { ਵਾਪਸ hb_array (&(ਇਹ+ਪਹਿਲਾ_Axis), axis_ਗਿਣਤੀ_2); }

  ਅਚਲ Instance_ਰਿਕਾਰਡ *get_instance (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  {
    ਜੇ (unlikely (i >= instance_ਗਿਣਤੀ)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
   ਵਾਪਸ &Struct_At_ਵਿੱਥ<Instance_ਰਿਕਾਰਡ> (&StructAfter<Instance_ਰਿਕਾਰਡ> (get_axes ()),
					   i * instance_ਮਾਪ);
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the fvar table
				 * initially set to 0x00010000u */
  Offset16_ਮੰਜ਼ਿਲ<Axis_ਰਿਕਾਰਡ>
		ਪਹਿਲਾ_Axis;	/* Offset in bytes from the beginning of the table
				 * to the start of the AxisRecord array. */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 2. */
  HBUINT16	axis_ਗਿਣਤੀ_2;	/* The number of variation axes in the font (the
				 * number of records in the axes array). */
  HBUINT16	axis_ਮਾਪ;	/* The size in bytes of each VariationAxisRecord —
				 * set to 20 (0x0014) for this version. */
  HBUINT16	instance_ਗਿਣਤੀ;	/* The number of named instances defined in the font
				 * (the number of records in the instances array). */
  HBUINT16	instance_ਮਾਪ;	/* The size in bytes of each InstanceRecord — set
				 * to either axisCount * sizeof(F16DOT16) + 4, or to
				 * axisCount * sizeof(F16DOT16) + 6. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (16);
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_VAR_FVAR_TABLE_HH */
