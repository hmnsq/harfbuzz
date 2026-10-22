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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-shape-plan.hh"
#অন্তর্ভাব "hb-shaper.hh"
#অন্তর্ভাব "hb-font.hh"
#অন্তর্ভাব "hb-buffer.hh"


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
বাহ্যিক "C" শূন্য _hb_harfrust_shape_plan_destroy_rs (শূন্য *তথ্য);
#endif

hb_shape_plan_t::~hb_shape_plan_t ()
{
#ifdef HAVE_HARFRUST
  শূন্য *তথ্য = harfrust_তথ্য.get_relaxed ();
  যদি (তথ্য)
    _hb_harfrust_shape_plan_destroy_rs (তথ্য);
#endif
  key.fini ();
}


/*
 * hb_shape_plan_key_t
 */

যৌক্তিক
hb_shape_plan_চাবি_t::init (যৌক্তিক                           copy,
			   hb_face_t                     *face,
			   ধ্রুবক hb_segment_properties_t *props,
			   ধ্রুবক hb_feature_t            *user_features,
			   unsigned পূর্ণসংখ্যা                   অঙ্ক_user_features,
			   ধ্রুবক পূর্ণসংখ্যা                     *coords,
			   unsigned পূর্ণসংখ্যা                   অঙ্ক_coords,
			   ধ্রুবক অক্ষর * ধ্রুবক            *shaper_list)
{
  hb_feature_t *features = nullptr;
  যদি (copy && অঙ্ক_user_features && !(features = (hb_feature_t *) hb_calloc (অঙ্ক_user_features, আকার (hb_feature_t))))
    যাও bail;

  এই->props = *props;
  এই->অঙ্ক_user_features = অঙ্ক_user_features;
  এই->user_features = copy ? features : user_features;
  যদি (copy && অঙ্ক_user_features)
  {
    hb_memcpy (features, user_features, অঙ্ক_user_features * আকার (hb_feature_t));
    /* Make start/end uniform to easier catch bugs. */
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_user_features; i++)
    {
      যদি (features[0].start != HB_FEATURE_GLOBAL_START)
	features[0].start = 1;
      যদি (features[0].end   != HB_FEATURE_GLOBAL_END)
	features[0].end   = 2;
    }
  }
  এই->shaper_func = nullptr;
  এই->shaper_name = nullptr;
#ifndef HB_NO_OT_SHAPE
  যদি (unlikely (!এই->ot.init (face, coords, অঙ্ক_coords)))
    যাও bail;
#endif

  /*
   * Choose shaper.
   */

#define HB_SHAPER_PLAN(shaper) \
	HB_STMT_START { \
	  যদি (face->তথ্য.shaper) \
	  { \
	    এই->shaper_func = _hb_##shaper##_shape; \
	    এই->shaper_name = #shaper; \
	    ফেরত সত্য; \
	  } \
	} HB_STMT_END

  যদি (unlikely (shaper_list))
  {
    জন্য (; *shaper_list; shaper_list++)
      যদি (মিথ্যা)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      অন্যথা যদি (0 == strcmp (*shaper_list, #shaper)) \
	HB_SHAPER_PLAN (shaper);
#অন্তর্ভাব "hb-shaper-list.hh"

#undef HB_SHAPER_IMPLEMENT
  }
  অন্যথা
  {
    ধ্রুবক HB_UNUSED hb_shaper_entry_t *shapers = _hb_shapers_get ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < HB_SHAPERS_গুনতি; i++)
      যদি (মিথ্যা)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      অন্যথা যদি (shapers[i].func == _hb_##shaper##_shape) \
	HB_SHAPER_PLAN (shaper);
#অন্তর্ভাব "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
#undef HB_SHAPER_PLAN

bail:
#ifndef HB_NO_OT_SHAPE
  এই->ot.fini ();
#endif
  ::hb_free (features);
  ফেরত মিথ্যা;
}

যৌক্তিক
hb_shape_plan_চাবি_t::user_features_match (ধ্রুবক hb_shape_plan_চাবি_t *other)
{
  যদি (এই->অঙ্ক_user_features != other->অঙ্ক_user_features)
    ফেরত মিথ্যা;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_user_features; i++)
  {
    যদি (এই->user_features[i].tag   != other->user_features[i].tag   ||
	এই->user_features[i].value != other->user_features[i].value ||
	(এই->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 এই->user_features[i].end   == HB_FEATURE_GLOBAL_END) !=
	(other->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 other->user_features[i].end   == HB_FEATURE_GLOBAL_END))
      ফেরত মিথ্যা;
  }
  ফেরত সত্য;
}

যৌক্তিক
hb_shape_plan_চাবি_t::equal (ধ্রুবক hb_shape_plan_চাবি_t *other)
{
  ফেরত hb_segment_properties_equal (&এই->props, &other->props) &&
	 এই->user_features_match (other) &&
#ifndef HB_NO_OT_SHAPE
	 এই->ot.equal (&other->ot) &&
#endif
	 এই->shaper_func == other->shaper_func;
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
		      ধ্রুবক hb_segment_properties_t *props,
		      ধ্রুবক hb_feature_t            *user_features,
		      unsigned পূর্ণসংখ্যা                   অঙ্ক_user_features,
		      ধ্রুবক অক্ষর * ধ্রুবক            *shaper_list)
{
  ফেরত hb_shape_plan_create2 (face, props,
				user_features, অঙ্ক_user_features,
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
		       ধ্রুবক hb_segment_properties_t *props,
		       ধ্রুবক hb_feature_t            *user_features,
		       unsigned পূর্ণসংখ্যা                   অঙ্ক_user_features,
		       ধ্রুবক পূর্ণসংখ্যা                     *coords,
		       unsigned পূর্ণসংখ্যা                   অঙ্ক_coords,
		       ধ্রুবক অক্ষর * ধ্রুবক            *shaper_list)
{
  DEBUG_বার্তা_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u num_coords=%u shaper_list=%p",
		  face,
		  অঙ্ক_user_features,
		  অঙ্ক_coords,
		  shaper_list);

  যদি (unlikely (!HB_DIRECTION_IS_VALID (props->direction)))
    ফেরত hb_shape_plan_get_empty ();

  hb_shape_plan_t *shape_plan;

  যদি (unlikely (!props))
    যাও bail;
  যদি (!(shape_plan = hb_object_create<hb_shape_plan_t> ()))
    যাও bail;

  যদি (unlikely (!face))
    face = hb_face_get_empty ();
  hb_face_make_immutable (face);
  shape_plan->face_unsafe = face;

  যদি (unlikely (!shape_plan->key.init (সত্য,
				       face,
				       props,
				       user_features,
				       অঙ্ক_user_features,
				       coords,
				       অঙ্ক_coords,
				       shaper_list)))
    যাও bail2;
#ifndef HB_NO_OT_SHAPE
  যদি (unlikely (!shape_plan->ot.init0 (face, &shape_plan->key)))
    যাও bail3;
#endif

  ফেরত shape_plan;

#ifndef HB_NO_OT_SHAPE
bail3:
#endif
  shape_plan->key.fini ();
bail2:
  hb_free (shape_plan);
bail:
  ফেরত hb_shape_plan_get_empty ();
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
  ফেরত const_cast<hb_shape_plan_t *> (&Null (hb_shape_plan_t));
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
  ফেরত hb_object_reference (shape_plan);
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
শূন্য
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan)
{
  যদি (!hb_object_destroy (shape_plan)) ফেরত;

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
hb_shape_plan_set_user_তথ্য (hb_shape_plan_t    *shape_plan,
			     hb_user_তথ্য_চাবি_t *key,
			     শূন্য *              তথ্য,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  ফেরত hb_object_set_user_তথ্য (shape_plan, key, তথ্য, destroy, replace);
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
শূন্য *
hb_shape_plan_get_user_তথ্য (ধ্রুবক hb_shape_plan_t *shape_plan,
			     hb_user_তথ্য_চাবি_t    *key)
{
  ফেরত hb_object_get_user_তথ্য (shape_plan, key);
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
ধ্রুবক অক্ষর *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan)
{
  ফেরত shape_plan->key.shaper_name;
}


স্থির যৌক্তিক
_hb_shape_plan_execute_internal (hb_shape_plan_t    *shape_plan,
				 hb_font_t          *font,
				 hb_বাফার_t        *বাফার,
				 ধ্রুবক hb_feature_t *features,
				 unsigned পূর্ণসংখ্যা        অঙ্ক_features)
{
  DEBUG_বার্তা_FUNC (SHAPE_PLAN, shape_plan,
		  "num_features=%u shaper_func=%p, shaper_name=%s",
		  অঙ্ক_features,
		  shape_plan->key.shaper_func,
		  shape_plan->key.shaper_name);

  যদি (unlikely (!বাফার->len))
    ফেরত সত্য;

  প্রতিজ্ঞা (!hb_object_is_immutable (বাফার));

  বাফার->assert_unicode ();

  যদি (unlikely (!hb_object_is_valid (shape_plan)))
    ফেরত মিথ্যা;

  প্রতিজ্ঞা (shape_plan->face_unsafe == font->face);
  প্রতিজ্ঞা (hb_segment_properties_equal (&shape_plan->key.props, &বাফার->props));

#define HB_SHAPER_EXECUTE(shaper) \
	HB_STMT_START { \
	  ফেরত font->তথ্য.shaper && \
		 _hb_##shaper##_shape (shape_plan, font, বাফার, features, অঙ্ক_features); \
	} HB_STMT_END

  যদি (মিথ্যা)
    ;
#define HB_SHAPER_IMPLEMENT(shaper) \
  অন্যথা যদি (shape_plan->key.shaper_func == _hb_##shaper##_shape) \
    HB_SHAPER_EXECUTE (shaper);
#অন্তর্ভাব "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

#undef HB_SHAPER_EXECUTE

  ফেরত মিথ্যা;
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
		       hb_বাফার_t        *বাফার,
		       ধ্রুবক hb_feature_t *features,
		       unsigned পূর্ণসংখ্যা        অঙ্ক_features)
{
  যৌক্তিক ret = _hb_shape_plan_execute_internal (shape_plan, font, বাফার,
					      features, অঙ্ক_features);

  যদি (ret && বাফার->content_type == HB_বাফার_CONTENT_TYPE_UNICODE)
    বাফার->content_type = HB_বাফার_CONTENT_TYPE_GLYPHS;

  ফেরত ret;
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
			     ধ্রুবক hb_segment_properties_t *props,
			     ধ্রুবক hb_feature_t            *user_features,
			     unsigned পূর্ণসংখ্যা                   অঙ্ক_user_features,
			     ধ্রুবক অক্ষর * ধ্রুবক            *shaper_list)
{
  ফেরত hb_shape_plan_create_cached2 (face, props,
				       user_features, অঙ্ক_user_features,
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
			      ধ্রুবক hb_segment_properties_t *props,
			      ধ্রুবক hb_feature_t            *user_features,
			      unsigned পূর্ণসংখ্যা                   অঙ্ক_user_features,
			      ধ্রুবক পূর্ণসংখ্যা                     *coords,
			      unsigned পূর্ণসংখ্যা                   অঙ্ক_coords,
			      ধ্রুবক অক্ষর * ধ্রুবক            *shaper_list)
{
  DEBUG_বার্তা_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u shaper_list=%p",
		  face,
		  অঙ্ক_user_features,
		  shaper_list);

retry:
  hb_face_t::plan_গ্রন্থি_t *cached_plan_nodes = face->shape_plans;

  যৌক্তিক dont_cache = !hb_object_is_valid (face);

  যদি (likely (!dont_cache))
  {
    hb_shape_plan_চাবি_t key;
    যদি (!key.init (মিথ্যা,
		   face,
		   props,
		   user_features,
		   অঙ্ক_user_features,
		   coords,
		   অঙ্ক_coords,
		   shaper_list))
      ফেরত hb_shape_plan_get_empty ();

    জন্য (hb_face_t::plan_গ্রন্থি_t *node = cached_plan_nodes; node; node = node->next)
      যদি (node->shape_plan->key.equal (&key))
      {
	DEBUG_বার্তা_FUNC (SHAPE_PLAN, node->shape_plan, "fulfilled from cache");
	ফেরত hb_shape_plan_reference (node->shape_plan);
      }
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create2 (face, props,
						       user_features, অঙ্ক_user_features,
						       coords, অঙ্ক_coords,
						       shaper_list);

  যদি (unlikely (dont_cache))
    ফেরত shape_plan;

  hb_face_t::plan_গ্রন্থি_t *node = (hb_face_t::plan_গ্রন্থি_t *) hb_calloc (1, আকার (hb_face_t::plan_গ্রন্থি_t));
  যদি (unlikely (!node))
    ফেরত shape_plan;

  node->shape_plan = shape_plan;
  node->next = cached_plan_nodes;

  যদি (unlikely (!face->shape_plans.cmpexch (cached_plan_nodes, node)))
  {
    hb_shape_plan_destroy (shape_plan);
    hb_free (node);
    যাও retry;
  }
  DEBUG_বার্তা_FUNC (SHAPE_PLAN, shape_plan, "inserted into cache");

  ফেরত hb_shape_plan_reference (shape_plan);
}


#endif
