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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-shape-plan.hh"
#ಅನ್ತರ್ಭಾವ "hb-shaper.hh"
#ಅನ್ತರ್ಭಾವ "hb-font.hh"
#ಅನ್ತರ್ಭಾವ "hb-buffer.hh"


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
ಬಾಹ್ಯ "C" ಶೂನ್ಯ _hb_harfrust_shape_plan_destroy_rs (ಶೂನ್ಯ *data);
#endif

hb_shape_plan_t::~hb_shape_plan_t ()
{
#ifdef HAVE_HARFRUST
  ಶೂನ್ಯ *data = harfrust_data.get_relaxed ();
  ಯದಿ (data)
    _hb_harfrust_shape_plan_destroy_rs (data);
#endif
  key.fini ();
}


/*
 * hb_shape_plan_key_t
 */

ತಾರ್ಕಿಕ
hb_shape_plan_ಕೀಲಿ_t::init (ತಾರ್ಕಿಕ                           copy,
			   hb_face_t                     *face,
			   ಧ್ರುವ hb_segment_properties_t *props,
			   ಧ್ರುವ hb_feature_t            *user_features,
			   unsigned ಪೂರ್ಣಾಙ್ಕ                   ಅಂಕೆ_user_features,
			   ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ                     *coords,
			   unsigned ಪೂರ್ಣಾಙ್ಕ                   ಅಂಕೆ_coords,
			   ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ            *shaper_list)
{
  hb_feature_t *features = nullptr;
  ಯದಿ (copy && ಅಂಕೆ_user_features && !(features = (hb_feature_t *) hb_calloc (ಅಂಕೆ_user_features, ಪರಿಮಾಣ (hb_feature_t))))
    ಗಚ್ಛ bail;

  ಏತತ್->props = *props;
  ಏತತ್->ಅಂಕೆ_user_features = ಅಂಕೆ_user_features;
  ಏತತ್->user_features = copy ? features : user_features;
  ಯದಿ (copy && ಅಂಕೆ_user_features)
  {
    hb_memcpy (features, user_features, ಅಂಕೆ_user_features * ಪರಿಮಾಣ (hb_feature_t));
    /* Make start/end uniform to easier catch bugs. */
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_user_features; i++)
    {
      ಯದಿ (features[0].start != HB_FEATURE_GLOBAL_START)
	features[0].start = 1;
      ಯದಿ (features[0].end   != HB_FEATURE_GLOBAL_END)
	features[0].end   = 2;
    }
  }
  ಏತತ್->shaper_func = nullptr;
  ಏತತ್->shaper_name = nullptr;
#ifndef HB_NO_OT_SHAPE
  ಯದಿ (unlikely (!ಏತತ್->ot.init (face, coords, ಅಂಕೆ_coords)))
    ಗಚ್ಛ bail;
#endif

  /*
   * Choose shaper.
   */

#define HB_SHAPER_PLAN(shaper) \
	HB_STMT_START { \
	  ಯದಿ (face->data.shaper) \
	  { \
	    ಏತತ್->shaper_func = _hb_##shaper##_shape; \
	    ಏತತ್->shaper_name = #shaper; \
	    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; \
	  } \
	} HB_STMT_END

  ಯದಿ (unlikely (shaper_list))
  {
    ಪ್ರತಿ (; *shaper_list; shaper_list++)
      ಯದಿ (ಅಸತ್ಯ)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      ಅನ್ಯಥಾ ಯದಿ (0 == strcmp (*shaper_list, #shaper)) \
	HB_SHAPER_PLAN (shaper);
#ಅನ್ತರ್ಭಾವ "hb-shaper-list.hh"

#undef HB_SHAPER_IMPLEMENT
  }
  ಅನ್ಯಥಾ
  {
    ಧ್ರುವ HB_UNUSED hb_shaper_entry_t *shapers = _hb_shapers_get ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < HB_SHAPERS_ಎಣಿಕೆ; i++)
      ಯದಿ (ಅಸತ್ಯ)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      ಅನ್ಯಥಾ ಯದಿ (shapers[i].func == _hb_##shaper##_shape) \
	HB_SHAPER_PLAN (shaper);
#ಅನ್ತರ್ಭಾವ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
#undef HB_SHAPER_PLAN

bail:
#ifndef HB_NO_OT_SHAPE
  ಏತತ್->ot.fini ();
#endif
  ::hb_free (features);
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ತಾರ್ಕಿಕ
hb_shape_plan_ಕೀಲಿ_t::user_features_match (ಧ್ರುವ hb_shape_plan_ಕೀಲಿ_t *other)
{
  ಯದಿ (ಏತತ್->ಅಂಕೆ_user_features != other->ಅಂಕೆ_user_features)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_user_features; i++)
  {
    ಯದಿ (ಏತತ್->user_features[i].tag   != other->user_features[i].tag   ||
	ಏತತ್->user_features[i].value != other->user_features[i].value ||
	(ಏತತ್->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 ಏತತ್->user_features[i].end   == HB_FEATURE_GLOBAL_END) !=
	(other->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 other->user_features[i].end   == HB_FEATURE_GLOBAL_END))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ತಾರ್ಕಿಕ
hb_shape_plan_ಕೀಲಿ_t::equal (ಧ್ರುವ hb_shape_plan_ಕೀಲಿ_t *other)
{
  ಪ್ರತ್ಯಾಗಮ hb_segment_properties_equal (&ಏತತ್->props, &other->props) &&
	 ಏತತ್->user_features_match (other) &&
#ifndef HB_NO_OT_SHAPE
	 ಏತತ್->ot.equal (&other->ot) &&
#endif
	 ಏತತ್->shaper_func == other->shaper_func;
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
		      ಧ್ರುವ hb_segment_properties_t *props,
		      ಧ್ರುವ hb_feature_t            *user_features,
		      unsigned ಪೂರ್ಣಾಙ್ಕ                   ಅಂಕೆ_user_features,
		      ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ            *shaper_list)
{
  ಪ್ರತ್ಯಾಗಮ hb_shape_plan_create2 (face, props,
				user_features, ಅಂಕೆ_user_features,
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
		       ಧ್ರುವ hb_segment_properties_t *props,
		       ಧ್ರುವ hb_feature_t            *user_features,
		       unsigned ಪೂರ್ಣಾಙ್ಕ                   ಅಂಕೆ_user_features,
		       ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ                     *coords,
		       unsigned ಪೂರ್ಣಾಙ್ಕ                   ಅಂಕೆ_coords,
		       ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ            *shaper_list)
{
  DEBUG_ಸಂದೇಶ_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u num_coords=%u shaper_list=%p",
		  face,
		  ಅಂಕೆ_user_features,
		  ಅಂಕೆ_coords,
		  shaper_list);

  ಯದಿ (unlikely (!HB_DIRECTION_IS_VALID (props->direction)))
    ಪ್ರತ್ಯಾಗಮ hb_shape_plan_get_empty ();

  hb_shape_plan_t *shape_plan;

  ಯದಿ (unlikely (!props))
    ಗಚ್ಛ bail;
  ಯದಿ (!(shape_plan = hb_object_create<hb_shape_plan_t> ()))
    ಗಚ್ಛ bail;

  ಯದಿ (unlikely (!face))
    face = hb_face_get_empty ();
  hb_face_make_immutable (face);
  shape_plan->face_unsafe = face;

  ಯದಿ (unlikely (!shape_plan->key.init (ಸತ್ಯ,
				       face,
				       props,
				       user_features,
				       ಅಂಕೆ_user_features,
				       coords,
				       ಅಂಕೆ_coords,
				       shaper_list)))
    ಗಚ್ಛ bail2;
#ifndef HB_NO_OT_SHAPE
  ಯದಿ (unlikely (!shape_plan->ot.init0 (face, &shape_plan->key)))
    ಗಚ್ಛ bail3;
#endif

  ಪ್ರತ್ಯಾಗಮ shape_plan;

#ifndef HB_NO_OT_SHAPE
bail3:
#endif
  shape_plan->key.fini ();
bail2:
  hb_free (shape_plan);
bail:
  ಪ್ರತ್ಯಾಗಮ hb_shape_plan_get_empty ();
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
  ಪ್ರತ್ಯಾಗಮ const_cast<hb_shape_plan_t *> (&Null (hb_shape_plan_t));
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
  ಪ್ರತ್ಯಾಗಮ hb_object_reference (shape_plan);
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
ಶೂನ್ಯ
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan)
{
  ಯದಿ (!hb_object_destroy (shape_plan)) ಪ್ರತ್ಯಾಗಮ;

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
			     hb_user_data_ಕೀಲಿ_t *key,
			     ಶೂನ್ಯ *              data,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  ಪ್ರತ್ಯಾಗಮ hb_object_set_user_data (shape_plan, key, data, destroy, replace);
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
ಶೂನ್ಯ *
hb_shape_plan_get_user_data (ಧ್ರುವ hb_shape_plan_t *shape_plan,
			     hb_user_data_ಕೀಲಿ_t    *key)
{
  ಪ್ರತ್ಯಾಗಮ hb_object_get_user_data (shape_plan, key);
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
ಧ್ರುವ ವರ್ಣ *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan)
{
  ಪ್ರತ್ಯಾಗಮ shape_plan->key.shaper_name;
}


ಸ್ಥಿರ ತಾರ್ಕಿಕ
_hb_shape_plan_execute_internal (hb_shape_plan_t    *shape_plan,
				 hb_font_t          *font,
				 hb_buffer_t        *buffer,
				 ಧ್ರುವ hb_feature_t *features,
				 unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features)
{
  DEBUG_ಸಂದೇಶ_FUNC (SHAPE_PLAN, shape_plan,
		  "num_features=%u shaper_func=%p, shaper_name=%s",
		  ಅಂಕೆ_features,
		  shape_plan->key.shaper_func,
		  shape_plan->key.shaper_name);

  ಯದಿ (unlikely (!buffer->len))
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  ಪ್ರತಿಜ್ಞಾ (!hb_object_is_immutable (buffer));

  buffer->assert_unicode ();

  ಯದಿ (unlikely (!hb_object_is_valid (shape_plan)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಪ್ರತಿಜ್ಞಾ (shape_plan->face_unsafe == font->face);
  ಪ್ರತಿಜ್ಞಾ (hb_segment_properties_equal (&shape_plan->key.props, &buffer->props));

#define HB_SHAPER_EXECUTE(shaper) \
	HB_STMT_START { \
	  ಪ್ರತ್ಯಾಗಮ font->data.shaper && \
		 _hb_##shaper##_shape (shape_plan, font, buffer, features, ಅಂಕೆ_features); \
	} HB_STMT_END

  ಯದಿ (ಅಸತ್ಯ)
    ;
#define HB_SHAPER_IMPLEMENT(shaper) \
  ಅನ್ಯಥಾ ಯದಿ (shape_plan->key.shaper_func == _hb_##shaper##_shape) \
    HB_SHAPER_EXECUTE (shaper);
#ಅನ್ತರ್ಭಾವ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

#undef HB_SHAPER_EXECUTE

  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
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
		       ಧ್ರುವ hb_feature_t *features,
		       unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features)
{
  ತಾರ್ಕಿಕ ret = _hb_shape_plan_execute_internal (shape_plan, font, buffer,
					      features, ಅಂಕೆ_features);

  ಯದಿ (ret && buffer->content_type == HB_BUFFER_CONTENT_TYPE_UNICODE)
    buffer->content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;

  ಪ್ರತ್ಯಾಗಮ ret;
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
			     ಧ್ರುವ hb_segment_properties_t *props,
			     ಧ್ರುವ hb_feature_t            *user_features,
			     unsigned ಪೂರ್ಣಾಙ್ಕ                   ಅಂಕೆ_user_features,
			     ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ            *shaper_list)
{
  ಪ್ರತ್ಯಾಗಮ hb_shape_plan_create_cached2 (face, props,
				       user_features, ಅಂಕೆ_user_features,
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
			      ಧ್ರುವ hb_segment_properties_t *props,
			      ಧ್ರುವ hb_feature_t            *user_features,
			      unsigned ಪೂರ್ಣಾಙ್ಕ                   ಅಂಕೆ_user_features,
			      ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ                     *coords,
			      unsigned ಪೂರ್ಣಾಙ್ಕ                   ಅಂಕೆ_coords,
			      ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ            *shaper_list)
{
  DEBUG_ಸಂದೇಶ_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u shaper_list=%p",
		  face,
		  ಅಂಕೆ_user_features,
		  shaper_list);

retry:
  hb_face_t::plan_ಗಂಟು_t *cached_plan_nodes = face->shape_plans;

  ತಾರ್ಕಿಕ dont_cache = !hb_object_is_valid (face);

  ಯದಿ (likely (!dont_cache))
  {
    hb_shape_plan_ಕೀಲಿ_t key;
    ಯದಿ (!key.init (ಅಸತ್ಯ,
		   face,
		   props,
		   user_features,
		   ಅಂಕೆ_user_features,
		   coords,
		   ಅಂಕೆ_coords,
		   shaper_list))
      ಪ್ರತ್ಯಾಗಮ hb_shape_plan_get_empty ();

    ಪ್ರತಿ (hb_face_t::plan_ಗಂಟು_t *node = cached_plan_nodes; node; node = node->next)
      ಯದಿ (node->shape_plan->key.equal (&key))
      {
	DEBUG_ಸಂದೇಶ_FUNC (SHAPE_PLAN, node->shape_plan, "fulfilled from cache");
	ಪ್ರತ್ಯಾಗಮ hb_shape_plan_reference (node->shape_plan);
      }
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create2 (face, props,
						       user_features, ಅಂಕೆ_user_features,
						       coords, ಅಂಕೆ_coords,
						       shaper_list);

  ಯದಿ (unlikely (dont_cache))
    ಪ್ರತ್ಯಾಗಮ shape_plan;

  hb_face_t::plan_ಗಂಟು_t *node = (hb_face_t::plan_ಗಂಟು_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_face_t::plan_ಗಂಟು_t));
  ಯದಿ (unlikely (!node))
    ಪ್ರತ್ಯಾಗಮ shape_plan;

  node->shape_plan = shape_plan;
  node->next = cached_plan_nodes;

  ಯದಿ (unlikely (!face->shape_plans.cmpexch (cached_plan_nodes, node)))
  {
    hb_shape_plan_destroy (shape_plan);
    hb_free (node);
    ಗಚ್ಛ retry;
  }
  DEBUG_ಸಂದೇಶ_FUNC (SHAPE_PLAN, shape_plan, "inserted into cache");

  ಪ್ರತ್ಯಾಗಮ hb_shape_plan_reference (shape_plan);
}


#endif
