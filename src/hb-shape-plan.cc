/*
 * Copyright © 2012  Google, Inc.
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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shape-plan.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer.hh"


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SHAPER

/**
 * SECTION:hb-shape-plan
 * @title: hb-shape-plan
 * @short_description: Object representing a shaping plan
 * @include: hb.h
 *
 * Shape plans are an internal mechanism. Each plan contains state
 * describing how HarfBuzz will shape a particular text segment, based on
 * the combination of segment properties and the capabilities in the
 * font face in use.
 *
 * Shape plans are not used for shaping directly, but can be queried to
 * access certain information about how shaping will perform, given a set
 * of specific input parameters (script, language, direction, features,
 * etc.).
 *
 * Most client programs will not need to deal with shape plans directly.
 **/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_HARFRUST
ਬਾਹਰੀ "C" ਖਾਲੀ _hb_harfrust_shape_plan_destroy_rs (ਖਾਲੀ *ਅੰਕੜੇ);
#ਜੇ_ਅੰਤ

hb_shape_plan_t::~hb_shape_plan_t ()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_HARFRUST
  ਖਾਲੀ *ਅੰਕੜੇ = harfrust_ਅੰਕੜੇ.get_relaxed ();
  ਜੇ (ਅੰਕੜੇ)
    _hb_harfrust_shape_plan_destroy_rs (ਅੰਕੜੇ);
#ਜੇ_ਅੰਤ
  key.fini ();
}


/*
 * hb_shape_plan_key_t
 */

ਤਾਰਕਿਕ
hb_shape_plan_ਕੁੰਜੀ_t::init (ਤਾਰਕਿਕ                           copy,
			   hb_face_t                     *face,
			   ਅਚਲ hb_segment_properties_t *props,
			   ਅਚਲ hb_feature_t            *user_features,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_user_features,
			   ਅਚਲ ਪੂਰਨ                     *coords,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_coords,
			   ਅਚਲ ਅੱਖਰ * ਅਚਲ            *shaper_list)
{
  hb_feature_t *features = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (copy && ਅੰਕ_user_features && !(features = (hb_feature_t *) hb_calloc (ਅੰਕ_user_features, ਆਕਾਰ (hb_feature_t))))
    ਜਾਓ bail;

  ਇਹ->props = *props;
  ਇਹ->ਅੰਕ_user_features = ਅੰਕ_user_features;
  ਇਹ->user_features = copy ? features : user_features;
  ਜੇ (copy && ਅੰਕ_user_features)
  {
    hb_memcpy (features, user_features, ਅੰਕ_user_features * ਆਕਾਰ (hb_feature_t));
    /* Make start/end uniform to easier catch bugs. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_user_features; i++)
    {
      ਜੇ (features[0].ਸ਼ੁਰੂ != HB_FEATURE_GLOBAL_ਸ਼ੁਰੂ)
	features[0].ਸ਼ੁਰੂ = 1;
      ਜੇ (features[0].ਸਿਰਾ   != HB_FEATURE_GLOBAL_ਸਿਰਾ)
	features[0].ਸਿਰਾ   = 2;
    }
  }
  ਇਹ->shaper_func = ਖਾਲੀ_ਸੂਚਕ;
  ਇਹ->shaper_name = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE
  ਜੇ (unlikely (!ਇਹ->ot.init (face, coords, ਅੰਕ_coords)))
    ਜਾਓ bail;
#ਜੇ_ਅੰਤ

  /*
   * Choose shaper.
   */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_PLAN(shaper) \
	HB_STMT_ਸ਼ੁਰੂ { \
	  ਜੇ (face->ਅੰਕੜੇ.shaper) \
	  { \
	    ਇਹ->shaper_func = _hb_##shaper##_shape; \
	    ਇਹ->shaper_name = #shaper; \
	    ਵਾਪਸ ਸੱਚ; \
	  } \
	} HB_STMT_ਸਿਰਾ

  ਜੇ (unlikely (shaper_list))
  {
    ਲਈ (; *shaper_list; shaper_list++)
      ਜੇ (ਝੂਠ)
	;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_IMPLEMENT(shaper) \
      ਨਹੀਂਤਾਂ ਜੇ (0 == strcmp (*shaper_list, #shaper)) \
	HB_SHAPER_PLAN (shaper);
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-list.hh"

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_IMPLEMENT
  }
  ਨਹੀਂਤਾਂ
  {
    ਅਚਲ HB_ਅਣਵਰਤਿਆ hb_shaper_ਇੰਦਰਾਜ_t *shapers = _hb_shapers_get ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < HB_SHAPERS_ਗਿਣਤੀ; i++)
      ਜੇ (ਝੂਠ)
	;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_IMPLEMENT(shaper) \
      ਨਹੀਂਤਾਂ ਜੇ (shapers[i].func == _hb_##shaper##_shape) \
	HB_SHAPER_PLAN (shaper);
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_IMPLEMENT
  }
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_PLAN

bail:
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE
  ਇਹ->ot.fini ();
#ਜੇ_ਅੰਤ
  ::hb_free (features);
  ਵਾਪਸ ਝੂਠ;
}

ਤਾਰਕਿਕ
hb_shape_plan_ਕੁੰਜੀ_t::user_features_ਮੇਲ (ਅਚਲ hb_shape_plan_ਕੁੰਜੀ_t *other)
{
  ਜੇ (ਇਹ->ਅੰਕ_user_features != other->ਅੰਕ_user_features)
    ਵਾਪਸ ਝੂਠ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_user_features; i++)
  {
    ਜੇ (ਇਹ->user_features[i].ਟੈਗ_2   != other->user_features[i].ਟੈਗ_2   ||
	ਇਹ->user_features[i].value != other->user_features[i].value ||
	(ਇਹ->user_features[i].ਸ਼ੁਰੂ == HB_FEATURE_GLOBAL_ਸ਼ੁਰੂ &&
	 ਇਹ->user_features[i].ਸਿਰਾ   == HB_FEATURE_GLOBAL_ਸਿਰਾ) !=
	(other->user_features[i].ਸ਼ੁਰੂ == HB_FEATURE_GLOBAL_ਸ਼ੁਰੂ &&
	 other->user_features[i].ਸਿਰਾ   == HB_FEATURE_GLOBAL_ਸਿਰਾ))
      ਵਾਪਸ ਝੂਠ;
  }
  ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ
hb_shape_plan_ਕੁੰਜੀ_t::equal (ਅਚਲ hb_shape_plan_ਕੁੰਜੀ_t *other)
{
  ਵਾਪਸ hb_segment_properties_equal (&ਇਹ->props, &other->props) &&
	 ਇਹ->user_features_ਮੇਲ (other) &&
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE
	 ਇਹ->ot.equal (&other->ot) &&
#ਜੇ_ਅੰਤ
	 ਇਹ->shaper_func == other->shaper_func;
}


/*
 * hb_shape_plan_t
 */


/**
 * hb_shape_plan_create:
 * @face: #hb_face_t to use
 * @props: The #hb_segment_properties_t of the segment
 * @user_features: (array length=num_user_features): The list of user-selected features
 * @num_user_features: The number of user-selected features
 * @shaper_list: (array zero-terminated=1): List of shapers to try
 *
 * Constructs a shaping plan for a combination of @face, @user_features, @props,
 * and @shaper_list.
 *
 * Return value: (transfer full): The shaping plan
 *
 * Since: 0.9.7
 **/
hb_shape_plan_t *
hb_shape_plan_create (hb_face_t                     *face,
		      ਅਚਲ hb_segment_properties_t *props,
		      ਅਚਲ hb_feature_t            *user_features,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_user_features,
		      ਅਚਲ ਅੱਖਰ * ਅਚਲ            *shaper_list)
{
  ਵਾਪਸ hb_shape_plan_create2 (face, props,
				user_features, ਅੰਕ_user_features,
				ਖਾਲੀ_ਸੂਚਕ, 0,
				shaper_list);
}

/**
 * hb_shape_plan_create2:
 * @face: #hb_face_t to use
 * @props: The #hb_segment_properties_t of the segment
 * @user_features: (array length=num_user_features): The list of user-selected features
 * @num_user_features: The number of user-selected features
 * @coords: (array length=num_coords): The list of variation-space coordinates
 * @num_coords: The number of variation-space coordinates
 * @shaper_list: (array zero-terminated=1): List of shapers to try
 *
 * The variable-font version of #hb_shape_plan_create.
 * Constructs a shaping plan for a combination of @face, @user_features, @props,
 * and @shaper_list, plus the variation-space coordinates @coords.
 *
 * Return value: (transfer full): The shaping plan
 *
 * Since: 1.4.0
 **/
hb_shape_plan_t *
hb_shape_plan_create2 (hb_face_t                     *face,
		       ਅਚਲ hb_segment_properties_t *props,
		       ਅਚਲ hb_feature_t            *user_features,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_user_features,
		       ਅਚਲ ਪੂਰਨ                     *coords,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_coords,
		       ਅਚਲ ਅੱਖਰ * ਅਚਲ            *shaper_list)
{
  DEBUG_ਸੁਨੇਹਾ_FUNC (SHAPE_PLAN, ਖਾਲੀ_ਸੂਚਕ,
		  "face=%p num_features=%u num_coords=%u shaper_list=%p",
		  face,
		  ਅੰਕ_user_features,
		  ਅੰਕ_coords,
		  shaper_list);

  ਜੇ (unlikely (!HB_DIRECTION_IS_VALID (props->direction)))
    ਵਾਪਸ hb_shape_plan_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  hb_shape_plan_t *shape_plan;

  ਜੇ (unlikely (!props))
    ਜਾਓ bail;
  ਜੇ (!(shape_plan = hb_object_create<hb_shape_plan_t> ()))
    ਜਾਓ bail;

  ਜੇ (unlikely (!face))
    face = hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  hb_face_make_immutable (face);
  shape_plan->face_unsafe = face;

  ਜੇ (unlikely (!shape_plan->key.init (ਸੱਚ,
				       face,
				       props,
				       user_features,
				       ਅੰਕ_user_features,
				       coords,
				       ਅੰਕ_coords,
				       shaper_list)))
    ਜਾਓ bail2;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE
  ਜੇ (unlikely (!shape_plan->ot.init0 (face, &shape_plan->key)))
    ਜਾਓ bail3;
#ਜੇ_ਅੰਤ

  ਵਾਪਸ shape_plan;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE
bail3:
#ਜੇ_ਅੰਤ
  shape_plan->key.fini ();
bail2:
  hb_free (shape_plan);
bail:
  ਵਾਪਸ hb_shape_plan_get_ਖਾਲੀ_ਥਾਵਾਂ ();
}

/**
 * hb_shape_plan_get_empty:
 *
 * Fetches the singleton empty shaping plan.
 *
 * Return value: (transfer full): The empty shaping plan
 *
 * Since: 0.9.7
 **/
hb_shape_plan_t *
hb_shape_plan_get_ਖਾਲੀ_ਥਾਵਾਂ ()
{
  ਵਾਪਸ const_cast<hb_shape_plan_t *> (&Null (hb_shape_plan_t));
}

/**
 * hb_shape_plan_reference: (skip)
 * @shape_plan: A shaping plan
 *
 * Increases the reference count on the given shaping plan.
 *
 * Return value: (transfer full): @shape_plan
 *
 * Since: 0.9.7
 **/
hb_shape_plan_t *
hb_shape_plan_reference (hb_shape_plan_t *shape_plan)
{
  ਵਾਪਸ hb_object_reference (shape_plan);
}

/**
 * hb_shape_plan_destroy: (skip)
 * @shape_plan: A shaping plan
 *
 * Decreases the reference count on the given shaping plan. When the
 * reference count reaches zero, the shaping plan is destroyed,
 * freeing all memory.
 *
 * Since: 0.9.7
 **/
ਖਾਲੀ
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan)
{
  ਜੇ (!hb_object_destroy (shape_plan)) ਵਾਪਸ;

  hb_free (shape_plan);
}

/**
 * hb_shape_plan_set_user_data: (skip)
 * @shape_plan: A shaping plan
 * @key: The user-data key to set
 * @data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the given shaping plan.
 *
 * Return value: `true` if success, `false` otherwise.
 *
 * Since: 0.9.7
 **/
hb_bool_t
hb_shape_plan_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_shape_plan_t    *shape_plan,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
			     ਖਾਲੀ *              ਅੰਕੜੇ,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (shape_plan, key, ਅੰਕੜੇ, destroy, replace);
}

/**
 * hb_shape_plan_get_user_data: (skip)
 * @shape_plan: A shaping plan
 * @key: The user-data key to query
 *
 * Fetches the user data associated with the specified key,
 * attached to the specified shaping plan.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 0.9.7
 **/
ਖਾਲੀ *
hb_shape_plan_get_user_ਅੰਕੜੇ (ਅਚਲ hb_shape_plan_t *shape_plan,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t    *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (shape_plan, key);
}

/**
 * hb_shape_plan_get_shaper:
 * @shape_plan: A shaping plan
 *
 * Fetches the shaper from a given shaping plan.
 *
 * Return value: (transfer none): The shaper
 *
 * Since: 0.9.7
 **/
ਅਚਲ ਅੱਖਰ *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan)
{
  ਵਾਪਸ shape_plan->key.shaper_name;
}


ਸਥਿਰ ਤਾਰਕਿਕ
hb_shape_plan_execute_ਅੰਦਰੂਨੀ (hb_shape_plan_t    *shape_plan,
				 hb_font_t          *font,
				 hb_ਬਫਰ_t        *ਬਫਰ,
				 ਅਚਲ hb_feature_t *features,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features)
{
  DEBUG_ਸੁਨੇਹਾ_FUNC (SHAPE_PLAN, shape_plan,
		  "num_features=%u shaper_func=%p, shaper_name=%s",
		  ਅੰਕ_features,
		  shape_plan->key.shaper_func,
		  shape_plan->key.shaper_name);

  ਜੇ (unlikely (!ਬਫਰ->len))
    ਵਾਪਸ ਸੱਚ;

  ਪ੍ਰਤਿਜ੍ਞਾ (!hb_object_is_immutable (ਬਫਰ));

  ਬਫਰ->assert_unicode ();

  ਜੇ (unlikely (!hb_object_is_valid (shape_plan)))
    ਵਾਪਸ ਝੂਠ;

  ਪ੍ਰਤਿਜ੍ਞਾ (shape_plan->face_unsafe == font->face);
  ਪ੍ਰਤਿਜ੍ਞਾ (hb_segment_properties_equal (&shape_plan->key.props, &ਬਫਰ->props));

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_EXECUTE(shaper) \
	HB_STMT_ਸ਼ੁਰੂ { \
	  ਵਾਪਸ font->ਅੰਕੜੇ.shaper && \
		 _hb_##shaper##_shape (shape_plan, font, ਬਫਰ, features, ਅੰਕ_features); \
	} HB_STMT_ਸਿਰਾ

  ਜੇ (ਝੂਠ)
    ;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_IMPLEMENT(shaper) \
  ਨਹੀਂਤਾਂ ਜੇ (shape_plan->key.shaper_func == _hb_##shaper##_shape) \
    HB_SHAPER_EXECUTE (shaper);
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_IMPLEMENT

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_EXECUTE

  ਵਾਪਸ ਝੂਠ;
}
/**
 * hb_shape_plan_execute:
 * @shape_plan: A shaping plan
 * @font: The #hb_font_t to use
 * @buffer: The #hb_buffer_t to work upon
 * @features: (array length=num_features): Features to enable
 * @num_features: The number of features to enable
 *
 * Executes the given shaping plan on the specified buffer, using
 * the given @font and @features.
 *
 * Return value: `true` if success, `false` otherwise.
 *
 * Since: 0.9.7
 **/
hb_bool_t
hb_shape_plan_execute (hb_shape_plan_t    *shape_plan,
		       hb_font_t          *font,
		       hb_ਬਫਰ_t        *ਬਫਰ,
		       ਅਚਲ hb_feature_t *features,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features)
{
  ਤਾਰਕਿਕ ret = hb_shape_plan_execute_ਅੰਦਰੂਨੀ (shape_plan, font, ਬਫਰ,
					      features, ਅੰਕ_features);

  ਜੇ (ret && ਬਫਰ->content_type == HB_ਬਫਰ_CONTENT_TYPE_UNICODE)
    ਬਫਰ->content_type = HB_ਬਫਰ_CONTENT_TYPE_GLYPHS;

  ਵਾਪਸ ret;
}


/*
 * Caching
 */

/**
 * hb_shape_plan_create_cached:
 * @face: #hb_face_t to use
 * @props: The #hb_segment_properties_t of the segment
 * @user_features: (array length=num_user_features): The list of user-selected features
 * @num_user_features: The number of user-selected features
 * @shaper_list: (array zero-terminated=1): List of shapers to try
 *
 * Creates a cached shaping plan suitable for reuse, for a combination
 * of @face, @user_features, @props, and @shaper_list.
 *
 * Return value: (transfer full): The shaping plan
 *
 * Since: 0.9.7
 **/
hb_shape_plan_t *
hb_shape_plan_create_cached (hb_face_t                     *face,
			     ਅਚਲ hb_segment_properties_t *props,
			     ਅਚਲ hb_feature_t            *user_features,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_user_features,
			     ਅਚਲ ਅੱਖਰ * ਅਚਲ            *shaper_list)
{
  ਵਾਪਸ hb_shape_plan_create_cached2 (face, props,
				       user_features, ਅੰਕ_user_features,
				       ਖਾਲੀ_ਸੂਚਕ, 0,
				       shaper_list);
}

/**
 * hb_shape_plan_create_cached2:
 * @face: #hb_face_t to use
 * @props: The #hb_segment_properties_t of the segment
 * @user_features: (array length=num_user_features): The list of user-selected features
 * @num_user_features: The number of user-selected features
 * @coords: (array length=num_coords): The list of variation-space coordinates
 * @num_coords: The number of variation-space coordinates
 * @shaper_list: (array zero-terminated=1): List of shapers to try
 *
 * The variable-font version of #hb_shape_plan_create_cached.
 * Creates a cached shaping plan suitable for reuse, for a combination
 * of @face, @user_features, @props, and @shaper_list, plus the
 * variation-space coordinates @coords.
 *
 * Return value: (transfer full): The shaping plan
 *
 * Since: 1.4.0
 **/
hb_shape_plan_t *
hb_shape_plan_create_cached2 (hb_face_t                     *face,
			      ਅਚਲ hb_segment_properties_t *props,
			      ਅਚਲ hb_feature_t            *user_features,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_user_features,
			      ਅਚਲ ਪੂਰਨ                     *coords,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                   ਅੰਕ_coords,
			      ਅਚਲ ਅੱਖਰ * ਅਚਲ            *shaper_list)
{
  DEBUG_ਸੁਨੇਹਾ_FUNC (SHAPE_PLAN, ਖਾਲੀ_ਸੂਚਕ,
		  "face=%p num_features=%u shaper_list=%p",
		  face,
		  ਅੰਕ_user_features,
		  shaper_list);

retry:
  hb_face_t::plan_ਗੰਢ_t *cached_plan_nodes = face->shape_plans;

  ਤਾਰਕਿਕ dont_cache = !hb_object_is_valid (face);

  ਜੇ (likely (!dont_cache))
  {
    hb_shape_plan_ਕੁੰਜੀ_t key;
    ਜੇ (!key.init (ਝੂਠ,
		   face,
		   props,
		   user_features,
		   ਅੰਕ_user_features,
		   coords,
		   ਅੰਕ_coords,
		   shaper_list))
      ਵਾਪਸ hb_shape_plan_get_ਖਾਲੀ_ਥਾਵਾਂ ();

    ਲਈ (hb_face_t::plan_ਗੰਢ_t *node = cached_plan_nodes; node; node = node->next)
      ਜੇ (node->shape_plan->key.equal (&key))
      {
	DEBUG_ਸੁਨੇਹਾ_FUNC (SHAPE_PLAN, node->shape_plan, "fulfilled from cache");
	ਵਾਪਸ hb_shape_plan_reference (node->shape_plan);
      }
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create2 (face, props,
						       user_features, ਅੰਕ_user_features,
						       coords, ਅੰਕ_coords,
						       shaper_list);

  ਜੇ (unlikely (dont_cache))
    ਵਾਪਸ shape_plan;

  hb_face_t::plan_ਗੰਢ_t *node = (hb_face_t::plan_ਗੰਢ_t *) hb_calloc (1, ਆਕਾਰ (hb_face_t::plan_ਗੰਢ_t));
  ਜੇ (unlikely (!node))
    ਵਾਪਸ shape_plan;

  node->shape_plan = shape_plan;
  node->next = cached_plan_nodes;

  ਜੇ (unlikely (!face->shape_plans.cmpexch (cached_plan_nodes, node)))
  {
    hb_shape_plan_destroy (shape_plan);
    hb_free (node);
    ਜਾਓ retry;
  }
  DEBUG_ਸੁਨੇਹਾ_FUNC (SHAPE_PLAN, shape_plan, "inserted into cache");

  ਵਾਪਸ hb_shape_plan_reference (shape_plan);
}


#ਜੇ_ਅੰਤ
