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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-shape-plan.hh"
#ଅନ୍ତର୍ଭାଵ "hb-shaper.hh"
#ଅନ୍ତର୍ଭାଵ "hb-font.hh"
#ଅନ୍ତର୍ଭାଵ "hb-buffer.hh"


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
ବାହ୍ୟ "C" ଶୂନ୍ୟ _hb_harfrust_shape_plan_destroy_rs (ଶୂନ୍ୟ *data);
#endif

hb_shape_plan_t::~hb_shape_plan_t ()
{
#ifdef HAVE_HARFRUST
  ଶୂନ୍ୟ *data = harfrust_data.get_relaxed ();
  ଯଦି (data)
    _hb_harfrust_shape_plan_destroy_rs (data);
#endif
  key.fini ();
}


/*
 * hb_shape_plan_key_t
 */

ତାର୍କିକ
hb_shape_plan_ଚାବି_t::init (ତାର୍କିକ                           copy,
			   hb_face_t                     *face,
			   ଅଚଳ hb_segment_properties_t *props,
			   ଅଚଳ hb_feature_t            *user_features,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                   ଅଙ୍କ_user_features,
			   ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା                     *coords,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                   ଅଙ୍କ_coords,
			   ଅଚଳ ଅକ୍ଷର * ଅଚଳ            *shaper_list)
{
  hb_feature_t *features = nullptr;
  ଯଦି (copy && ଅଙ୍କ_user_features && !(features = (hb_feature_t *) hb_calloc (ଅଙ୍କ_user_features, ଆକାର (hb_feature_t))))
    ଯାଅ bail;

  ଏହି->props = *props;
  ଏହି->ଅଙ୍କ_user_features = ଅଙ୍କ_user_features;
  ଏହି->user_features = copy ? features : user_features;
  ଯଦି (copy && ଅଙ୍କ_user_features)
  {
    hb_memcpy (features, user_features, ଅଙ୍କ_user_features * ଆକାର (hb_feature_t));
    /* Make start/end uniform to easier catch bugs. */
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_user_features; i++)
    {
      ଯଦି (features[0].start != HB_FEATURE_GLOBAL_START)
	features[0].start = 1;
      ଯଦି (features[0].end   != HB_FEATURE_GLOBAL_END)
	features[0].end   = 2;
    }
  }
  ଏହି->shaper_func = nullptr;
  ଏହି->shaper_name = nullptr;
#ifndef HB_NO_OT_SHAPE
  ଯଦି (unlikely (!ଏହି->ot.init (face, coords, ଅଙ୍କ_coords)))
    ଯାଅ bail;
#endif

  /*
   * Choose shaper.
   */

#define HB_SHAPER_PLAN(shaper) \
	HB_STMT_START { \
	  ଯଦି (face->data.shaper) \
	  { \
	    ଏହି->shaper_func = _hb_##shaper##_shape; \
	    ଏହି->shaper_name = #shaper; \
	    ଫେରନ୍ତୁ ସତ୍ୟ; \
	  } \
	} HB_STMT_END

  ଯଦି (unlikely (shaper_list))
  {
    ପାଇଁ (; *shaper_list; shaper_list++)
      ଯଦି (ମିଥ୍ୟା)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      ଅନ୍ୟଥା ଯଦି (0 == strcmp (*shaper_list, #shaper)) \
	HB_SHAPER_PLAN (shaper);
#ଅନ୍ତର୍ଭାଵ "hb-shaper-list.hh"

#undef HB_SHAPER_IMPLEMENT
  }
  ଅନ୍ୟଥା
  {
    ଅଚଳ HB_UNUSED hb_shaper_entry_t *shapers = _hb_shapers_get ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < HB_SHAPERS_ଗଣନା; i++)
      ଯଦି (ମିଥ୍ୟା)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      ଅନ୍ୟଥା ଯଦି (shapers[i].func == _hb_##shaper##_shape) \
	HB_SHAPER_PLAN (shaper);
#ଅନ୍ତର୍ଭାଵ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
#undef HB_SHAPER_PLAN

bail:
#ifndef HB_NO_OT_SHAPE
  ଏହି->ot.fini ();
#endif
  ::hb_free (features);
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ତାର୍କିକ
hb_shape_plan_ଚାବି_t::user_features_match (ଅଚଳ hb_shape_plan_ଚାବି_t *other)
{
  ଯଦି (ଏହି->ଅଙ୍କ_user_features != other->ଅଙ୍କ_user_features)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_user_features; i++)
  {
    ଯଦି (ଏହି->user_features[i].tag   != other->user_features[i].tag   ||
	ଏହି->user_features[i].value != other->user_features[i].value ||
	(ଏହି->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 ଏହି->user_features[i].end   == HB_FEATURE_GLOBAL_END) !=
	(other->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 other->user_features[i].end   == HB_FEATURE_GLOBAL_END))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ତାର୍କିକ
hb_shape_plan_ଚାବି_t::equal (ଅଚଳ hb_shape_plan_ଚାବି_t *other)
{
  ଫେରନ୍ତୁ hb_segment_properties_equal (&ଏହି->props, &other->props) &&
	 ଏହି->user_features_match (other) &&
#ifndef HB_NO_OT_SHAPE
	 ଏହି->ot.equal (&other->ot) &&
#endif
	 ଏହି->shaper_func == other->shaper_func;
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
		      ଅଚଳ hb_segment_properties_t *props,
		      ଅଚଳ hb_feature_t            *user_features,
		      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                   ଅଙ୍କ_user_features,
		      ଅଚଳ ଅକ୍ଷର * ଅଚଳ            *shaper_list)
{
  ଫେରନ୍ତୁ hb_shape_plan_create2 (face, props,
				user_features, ଅଙ୍କ_user_features,
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
		       ଅଚଳ hb_segment_properties_t *props,
		       ଅଚଳ hb_feature_t            *user_features,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                   ଅଙ୍କ_user_features,
		       ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା                     *coords,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                   ଅଙ୍କ_coords,
		       ଅଚଳ ଅକ୍ଷର * ଅଚଳ            *shaper_list)
{
  DEBUG_ସନ୍ଦେଶ_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u num_coords=%u shaper_list=%p",
		  face,
		  ଅଙ୍କ_user_features,
		  ଅଙ୍କ_coords,
		  shaper_list);

  ଯଦି (unlikely (!HB_DIRECTION_IS_VALID (props->direction)))
    ଫେରନ୍ତୁ hb_shape_plan_get_empty ();

  hb_shape_plan_t *shape_plan;

  ଯଦି (unlikely (!props))
    ଯାଅ bail;
  ଯଦି (!(shape_plan = hb_object_create<hb_shape_plan_t> ()))
    ଯାଅ bail;

  ଯଦି (unlikely (!face))
    face = hb_face_get_empty ();
  hb_face_make_immutable (face);
  shape_plan->face_unsafe = face;

  ଯଦି (unlikely (!shape_plan->key.init (ସତ୍ୟ,
				       face,
				       props,
				       user_features,
				       ଅଙ୍କ_user_features,
				       coords,
				       ଅଙ୍କ_coords,
				       shaper_list)))
    ଯାଅ bail2;
#ifndef HB_NO_OT_SHAPE
  ଯଦି (unlikely (!shape_plan->ot.init0 (face, &shape_plan->key)))
    ଯାଅ bail3;
#endif

  ଫେରନ୍ତୁ shape_plan;

#ifndef HB_NO_OT_SHAPE
bail3:
#endif
  shape_plan->key.fini ();
bail2:
  hb_free (shape_plan);
bail:
  ଫେରନ୍ତୁ hb_shape_plan_get_empty ();
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
  ଫେରନ୍ତୁ const_cast<hb_shape_plan_t *> (&Null (hb_shape_plan_t));
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
  ଫେରନ୍ତୁ hb_object_reference (shape_plan);
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
ଶୂନ୍ୟ
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan)
{
  ଯଦି (!hb_object_destroy (shape_plan)) ଫେରନ୍ତୁ;

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
			     hb_user_data_ଚାବି_t *key,
			     ଶୂନ୍ୟ *              data,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  ଫେରନ୍ତୁ hb_object_set_user_data (shape_plan, key, data, destroy, replace);
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
ଶୂନ୍ୟ *
hb_shape_plan_get_user_data (ଅଚଳ hb_shape_plan_t *shape_plan,
			     hb_user_data_ଚାବି_t    *key)
{
  ଫେରନ୍ତୁ hb_object_get_user_data (shape_plan, key);
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
ଅଚଳ ଅକ୍ଷର *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan)
{
  ଫେରନ୍ତୁ shape_plan->key.shaper_name;
}


ସ୍ଥିର ତାର୍କିକ
_hb_shape_plan_execute_internal (hb_shape_plan_t    *shape_plan,
				 hb_font_t          *font,
				 hb_buffer_t        *buffer,
				 ଅଚଳ hb_feature_t *features,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features)
{
  DEBUG_ସନ୍ଦେଶ_FUNC (SHAPE_PLAN, shape_plan,
		  "num_features=%u shaper_func=%p, shaper_name=%s",
		  ଅଙ୍କ_features,
		  shape_plan->key.shaper_func,
		  shape_plan->key.shaper_name);

  ଯଦି (unlikely (!buffer->len))
    ଫେରନ୍ତୁ ସତ୍ୟ;

  ପ୍ରତିଜ୍ଞା (!hb_object_is_immutable (buffer));

  buffer->assert_unicode ();

  ଯଦି (unlikely (!hb_object_is_valid (shape_plan)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ପ୍ରତିଜ୍ଞା (shape_plan->face_unsafe == font->face);
  ପ୍ରତିଜ୍ଞା (hb_segment_properties_equal (&shape_plan->key.props, &buffer->props));

#define HB_SHAPER_EXECUTE(shaper) \
	HB_STMT_START { \
	  ଫେରନ୍ତୁ font->data.shaper && \
		 _hb_##shaper##_shape (shape_plan, font, buffer, features, ଅଙ୍କ_features); \
	} HB_STMT_END

  ଯଦି (ମିଥ୍ୟା)
    ;
#define HB_SHAPER_IMPLEMENT(shaper) \
  ଅନ୍ୟଥା ଯଦି (shape_plan->key.shaper_func == _hb_##shaper##_shape) \
    HB_SHAPER_EXECUTE (shaper);
#ଅନ୍ତର୍ଭାଵ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

#undef HB_SHAPER_EXECUTE

  ଫେରନ୍ତୁ ମିଥ୍ୟା;
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
		       ଅଚଳ hb_feature_t *features,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features)
{
  ତାର୍କିକ ret = _hb_shape_plan_execute_internal (shape_plan, font, buffer,
					      features, ଅଙ୍କ_features);

  ଯଦି (ret && buffer->content_type == HB_BUFFER_CONTENT_TYPE_UNICODE)
    buffer->content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;

  ଫେରନ୍ତୁ ret;
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
			     ଅଚଳ hb_segment_properties_t *props,
			     ଅଚଳ hb_feature_t            *user_features,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                   ଅଙ୍କ_user_features,
			     ଅଚଳ ଅକ୍ଷର * ଅଚଳ            *shaper_list)
{
  ଫେରନ୍ତୁ hb_shape_plan_create_cached2 (face, props,
				       user_features, ଅଙ୍କ_user_features,
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
			      ଅଚଳ hb_segment_properties_t *props,
			      ଅଚଳ hb_feature_t            *user_features,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                   ଅଙ୍କ_user_features,
			      ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା                     *coords,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା                   ଅଙ୍କ_coords,
			      ଅଚଳ ଅକ୍ଷର * ଅଚଳ            *shaper_list)
{
  DEBUG_ସନ୍ଦେଶ_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u shaper_list=%p",
		  face,
		  ଅଙ୍କ_user_features,
		  shaper_list);

retry:
  hb_face_t::plan_ଗଣ୍ଠି_t *cached_plan_nodes = face->shape_plans;

  ତାର୍କିକ dont_cache = !hb_object_is_valid (face);

  ଯଦି (likely (!dont_cache))
  {
    hb_shape_plan_ଚାବି_t key;
    ଯଦି (!key.init (ମିଥ୍ୟା,
		   face,
		   props,
		   user_features,
		   ଅଙ୍କ_user_features,
		   coords,
		   ଅଙ୍କ_coords,
		   shaper_list))
      ଫେରନ୍ତୁ hb_shape_plan_get_empty ();

    ପାଇଁ (hb_face_t::plan_ଗଣ୍ଠି_t *node = cached_plan_nodes; node; node = node->next)
      ଯଦି (node->shape_plan->key.equal (&key))
      {
	DEBUG_ସନ୍ଦେଶ_FUNC (SHAPE_PLAN, node->shape_plan, "fulfilled from cache");
	ଫେରନ୍ତୁ hb_shape_plan_reference (node->shape_plan);
      }
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create2 (face, props,
						       user_features, ଅଙ୍କ_user_features,
						       coords, ଅଙ୍କ_coords,
						       shaper_list);

  ଯଦି (unlikely (dont_cache))
    ଫେରନ୍ତୁ shape_plan;

  hb_face_t::plan_ଗଣ୍ଠି_t *node = (hb_face_t::plan_ଗଣ୍ଠି_t *) hb_calloc (1, ଆକାର (hb_face_t::plan_ଗଣ୍ଠି_t));
  ଯଦି (unlikely (!node))
    ଫେରନ୍ତୁ shape_plan;

  node->shape_plan = shape_plan;
  node->next = cached_plan_nodes;

  ଯଦି (unlikely (!face->shape_plans.cmpexch (cached_plan_nodes, node)))
  {
    hb_shape_plan_destroy (shape_plan);
    hb_free (node);
    ଯାଅ retry;
  }
  DEBUG_ସନ୍ଦେଶ_FUNC (SHAPE_PLAN, shape_plan, "inserted into cache");

  ଫେରନ୍ତୁ hb_shape_plan_reference (shape_plan);
}


#endif
