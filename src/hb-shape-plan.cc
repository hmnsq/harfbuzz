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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-shape-plan.hh"
#அந்தர்பாவ "hb-shaper.hh"
#அந்தர்பாவ "hb-font.hh"
#அந்தர்பாவ "hb-buffer.hh"


#ifndef HB_NO_SHAPER

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

#ifdef HAVE_HARFRUST
வெளி "C" வெறுமை _hb_harfrust_shape_plan_destroy_rs (வெறுமை *data);
#endif

hb_shape_plan_t::~hb_shape_plan_t ()
{
#ifdef HAVE_HARFRUST
  வெறுமை *data = harfrust_data.get_relaxed ();
  எனில் (data)
    _hb_harfrust_shape_plan_destroy_rs (data);
#endif
  key.fini ();
}


/*
 * hb_shape_plan_key_t
 */

தருக்க
hb_shape_plan_சாவி_t::init (தருக்க                           copy,
			   hb_face_t                     *face,
			   மாறிலி hb_segment_properties_t *props,
			   மாறிலி hb_feature_t            *user_features,
			   unsigned முழு                   இலக்கம்_user_features,
			   மாறிலி முழு                     *coords,
			   unsigned முழு                   இலக்கம்_coords,
			   மாறிலி எழுத்து * மாறிலி            *shaper_list)
{
  hb_feature_t *features = nullptr;
  எனில் (copy && இலக்கம்_user_features && !(features = (hb_feature_t *) hb_calloc (இலக்கம்_user_features, அளவு (hb_feature_t))))
    செல் bail;

  இது->props = *props;
  இது->இலக்கம்_user_features = இலக்கம்_user_features;
  இது->user_features = copy ? features : user_features;
  எனில் (copy && இலக்கம்_user_features)
  {
    hb_memcpy (features, user_features, இலக்கம்_user_features * அளவு (hb_feature_t));
    /* Make start/end uniform to easier catch bugs. */
    க்கு (unsigned முழு i = 0; i < இலக்கம்_user_features; i++)
    {
      எனில் (features[0].start != HB_FEATURE_GLOBAL_START)
	features[0].start = 1;
      எனில் (features[0].end   != HB_FEATURE_GLOBAL_END)
	features[0].end   = 2;
    }
  }
  இது->shaper_func = nullptr;
  இது->shaper_name = nullptr;
#ifndef HB_NO_OT_SHAPE
  எனில் (unlikely (!இது->ot.init (face, coords, இலக்கம்_coords)))
    செல் bail;
#endif

  /*
   * Choose shaper.
   */

#define HB_SHAPER_PLAN(shaper) \
	HB_STMT_START { \
	  எனில் (face->data.shaper) \
	  { \
	    இது->shaper_func = _hb_##shaper##_shape; \
	    இது->shaper_name = #shaper; \
	    திரும்பு உண்மை; \
	  } \
	} HB_STMT_END

  எனில் (unlikely (shaper_list))
  {
    க்கு (; *shaper_list; shaper_list++)
      எனில் (பொய்)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      இல்லையெனில் எனில் (0 == strcmp (*shaper_list, #shaper)) \
	HB_SHAPER_PLAN (shaper);
#அந்தர்பாவ "hb-shaper-list.hh"

#undef HB_SHAPER_IMPLEMENT
  }
  இல்லையெனில்
  {
    மாறிலி HB_UNUSED hb_shaper_entry_t *shapers = _hb_shapers_get ();
    க்கு (unsigned முழு i = 0; i < HB_SHAPERS_மொத்தம்; i++)
      எனில் (பொய்)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      இல்லையெனில் எனில் (shapers[i].func == _hb_##shaper##_shape) \
	HB_SHAPER_PLAN (shaper);
#அந்தர்பாவ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
#undef HB_SHAPER_PLAN

bail:
#ifndef HB_NO_OT_SHAPE
  இது->ot.fini ();
#endif
  ::hb_free (features);
  திரும்பு பொய்;
}

தருக்க
hb_shape_plan_சாவி_t::user_features_match (மாறிலி hb_shape_plan_சாவி_t *other)
{
  எனில் (இது->இலக்கம்_user_features != other->இலக்கம்_user_features)
    திரும்பு பொய்;
  க்கு (unsigned முழு i = 0; i < இலக்கம்_user_features; i++)
  {
    எனில் (இது->user_features[i].tag   != other->user_features[i].tag   ||
	இது->user_features[i].value != other->user_features[i].value ||
	(இது->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 இது->user_features[i].end   == HB_FEATURE_GLOBAL_END) !=
	(other->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 other->user_features[i].end   == HB_FEATURE_GLOBAL_END))
      திரும்பு பொய்;
  }
  திரும்பு உண்மை;
}

தருக்க
hb_shape_plan_சாவி_t::equal (மாறிலி hb_shape_plan_சாவி_t *other)
{
  திரும்பு hb_segment_properties_equal (&இது->props, &other->props) &&
	 இது->user_features_match (other) &&
#ifndef HB_NO_OT_SHAPE
	 இது->ot.equal (&other->ot) &&
#endif
	 இது->shaper_func == other->shaper_func;
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
		      மாறிலி hb_segment_properties_t *props,
		      மாறிலி hb_feature_t            *user_features,
		      unsigned முழு                   இலக்கம்_user_features,
		      மாறிலி எழுத்து * மாறிலி            *shaper_list)
{
  திரும்பு hb_shape_plan_create2 (face, props,
				user_features, இலக்கம்_user_features,
				nullptr, 0,
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
		       மாறிலி hb_segment_properties_t *props,
		       மாறிலி hb_feature_t            *user_features,
		       unsigned முழு                   இலக்கம்_user_features,
		       மாறிலி முழு                     *coords,
		       unsigned முழு                   இலக்கம்_coords,
		       மாறிலி எழுத்து * மாறிலி            *shaper_list)
{
  DEBUG_செய்தி_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u num_coords=%u shaper_list=%p",
		  face,
		  இலக்கம்_user_features,
		  இலக்கம்_coords,
		  shaper_list);

  எனில் (unlikely (!HB_DIRECTION_IS_VALID (props->direction)))
    திரும்பு hb_shape_plan_get_empty ();

  hb_shape_plan_t *shape_plan;

  எனில் (unlikely (!props))
    செல் bail;
  எனில் (!(shape_plan = hb_object_create<hb_shape_plan_t> ()))
    செல் bail;

  எனில் (unlikely (!face))
    face = hb_face_get_empty ();
  hb_face_make_immutable (face);
  shape_plan->face_unsafe = face;

  எனில் (unlikely (!shape_plan->key.init (உண்மை,
				       face,
				       props,
				       user_features,
				       இலக்கம்_user_features,
				       coords,
				       இலக்கம்_coords,
				       shaper_list)))
    செல் bail2;
#ifndef HB_NO_OT_SHAPE
  எனில் (unlikely (!shape_plan->ot.init0 (face, &shape_plan->key)))
    செல் bail3;
#endif

  திரும்பு shape_plan;

#ifndef HB_NO_OT_SHAPE
bail3:
#endif
  shape_plan->key.fini ();
bail2:
  hb_free (shape_plan);
bail:
  திரும்பு hb_shape_plan_get_empty ();
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
hb_shape_plan_get_empty ()
{
  திரும்பு const_cast<hb_shape_plan_t *> (&Null (hb_shape_plan_t));
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
  திரும்பு hb_object_reference (shape_plan);
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
வெறுமை
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan)
{
  எனில் (!hb_object_destroy (shape_plan)) திரும்பு;

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
hb_shape_plan_set_user_data (hb_shape_plan_t    *shape_plan,
			     hb_user_data_சாவி_t *key,
			     வெறுமை *              data,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  திரும்பு hb_object_set_user_data (shape_plan, key, data, destroy, replace);
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
வெறுமை *
hb_shape_plan_get_user_data (மாறிலி hb_shape_plan_t *shape_plan,
			     hb_user_data_சாவி_t    *key)
{
  திரும்பு hb_object_get_user_data (shape_plan, key);
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
மாறிலி எழுத்து *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan)
{
  திரும்பு shape_plan->key.shaper_name;
}


நிலையான தருக்க
_hb_shape_plan_execute_internal (hb_shape_plan_t    *shape_plan,
				 hb_font_t          *font,
				 hb_buffer_t        *buffer,
				 மாறிலி hb_feature_t *features,
				 unsigned முழு        இலக்கம்_features)
{
  DEBUG_செய்தி_FUNC (SHAPE_PLAN, shape_plan,
		  "num_features=%u shaper_func=%p, shaper_name=%s",
		  இலக்கம்_features,
		  shape_plan->key.shaper_func,
		  shape_plan->key.shaper_name);

  எனில் (unlikely (!buffer->len))
    திரும்பு உண்மை;

  ப்ரதிஜ்ஞா (!hb_object_is_immutable (buffer));

  buffer->assert_unicode ();

  எனில் (unlikely (!hb_object_is_valid (shape_plan)))
    திரும்பு பொய்;

  ப்ரதிஜ்ஞா (shape_plan->face_unsafe == font->face);
  ப்ரதிஜ்ஞா (hb_segment_properties_equal (&shape_plan->key.props, &buffer->props));

#define HB_SHAPER_EXECUTE(shaper) \
	HB_STMT_START { \
	  திரும்பு font->data.shaper && \
		 _hb_##shaper##_shape (shape_plan, font, buffer, features, இலக்கம்_features); \
	} HB_STMT_END

  எனில் (பொய்)
    ;
#define HB_SHAPER_IMPLEMENT(shaper) \
  இல்லையெனில் எனில் (shape_plan->key.shaper_func == _hb_##shaper##_shape) \
    HB_SHAPER_EXECUTE (shaper);
#அந்தர்பாவ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

#undef HB_SHAPER_EXECUTE

  திரும்பு பொய்;
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
		       hb_buffer_t        *buffer,
		       மாறிலி hb_feature_t *features,
		       unsigned முழு        இலக்கம்_features)
{
  தருக்க ret = _hb_shape_plan_execute_internal (shape_plan, font, buffer,
					      features, இலக்கம்_features);

  எனில் (ret && buffer->content_type == HB_BUFFER_CONTENT_TYPE_UNICODE)
    buffer->content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;

  திரும்பு ret;
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
			     மாறிலி hb_segment_properties_t *props,
			     மாறிலி hb_feature_t            *user_features,
			     unsigned முழு                   இலக்கம்_user_features,
			     மாறிலி எழுத்து * மாறிலி            *shaper_list)
{
  திரும்பு hb_shape_plan_create_cached2 (face, props,
				       user_features, இலக்கம்_user_features,
				       nullptr, 0,
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
			      மாறிலி hb_segment_properties_t *props,
			      மாறிலி hb_feature_t            *user_features,
			      unsigned முழு                   இலக்கம்_user_features,
			      மாறிலி முழு                     *coords,
			      unsigned முழு                   இலக்கம்_coords,
			      மாறிலி எழுத்து * மாறிலி            *shaper_list)
{
  DEBUG_செய்தி_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u shaper_list=%p",
		  face,
		  இலக்கம்_user_features,
		  shaper_list);

retry:
  hb_face_t::plan_முனை_t *cached_plan_nodes = face->shape_plans;

  தருக்க dont_cache = !hb_object_is_valid (face);

  எனில் (likely (!dont_cache))
  {
    hb_shape_plan_சாவி_t key;
    எனில் (!key.init (பொய்,
		   face,
		   props,
		   user_features,
		   இலக்கம்_user_features,
		   coords,
		   இலக்கம்_coords,
		   shaper_list))
      திரும்பு hb_shape_plan_get_empty ();

    க்கு (hb_face_t::plan_முனை_t *node = cached_plan_nodes; node; node = node->next)
      எனில் (node->shape_plan->key.equal (&key))
      {
	DEBUG_செய்தி_FUNC (SHAPE_PLAN, node->shape_plan, "fulfilled from cache");
	திரும்பு hb_shape_plan_reference (node->shape_plan);
      }
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create2 (face, props,
						       user_features, இலக்கம்_user_features,
						       coords, இலக்கம்_coords,
						       shaper_list);

  எனில் (unlikely (dont_cache))
    திரும்பு shape_plan;

  hb_face_t::plan_முனை_t *node = (hb_face_t::plan_முனை_t *) hb_calloc (1, அளவு (hb_face_t::plan_முனை_t));
  எனில் (unlikely (!node))
    திரும்பு shape_plan;

  node->shape_plan = shape_plan;
  node->next = cached_plan_nodes;

  எனில் (unlikely (!face->shape_plans.cmpexch (cached_plan_nodes, node)))
  {
    hb_shape_plan_destroy (shape_plan);
    hb_free (node);
    செல் retry;
  }
  DEBUG_செய்தி_FUNC (SHAPE_PLAN, shape_plan, "inserted into cache");

  திரும்பு hb_shape_plan_reference (shape_plan);
}


#endif
