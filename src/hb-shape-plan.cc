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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-shape-plan.hh"
#അന്തര്ഭാവ "hb-shaper.hh"
#അന്തര്ഭാവ "hb-font.hh"
#അന്തര്ഭാവ "hb-buffer.hh"


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
ബാഹ്യ "C" ശൂന്യം _hb_harfrust_shape_plan_destroy_rs (ശൂന്യം *data);
#endif

hb_shape_plan_t::~hb_shape_plan_t ()
{
#ifdef HAVE_HARFRUST
  ശൂന്യം *data = harfrust_data.get_relaxed ();
  എങ്കിൽ (data)
    _hb_harfrust_shape_plan_destroy_rs (data);
#endif
  key.fini ();
}


/*
 * hb_shape_plan_key_t
 */

തർക്കം
hb_shape_plan_താക്കോൽ_t::init (തർക്കം                           copy,
			   hb_face_t                     *face,
			   സ്ഥിരം hb_segment_properties_t *props,
			   സ്ഥിരം hb_feature_t            *user_features,
			   unsigned പൂർണ്ണസംഖ്യ                   അക്കം_user_features,
			   സ്ഥിരം പൂർണ്ണസംഖ്യ                     *coords,
			   unsigned പൂർണ്ണസംഖ്യ                   അക്കം_coords,
			   സ്ഥിരം അക്ഷരം * സ്ഥിരം            *shaper_list)
{
  hb_feature_t *features = nullptr;
  എങ്കിൽ (copy && അക്കം_user_features && !(features = (hb_feature_t *) hb_calloc (അക്കം_user_features, വലുപ്പം (hb_feature_t))))
    പോകുക bail;

  ഇത്->props = *props;
  ഇത്->അക്കം_user_features = അക്കം_user_features;
  ഇത്->user_features = copy ? features : user_features;
  എങ്കിൽ (copy && അക്കം_user_features)
  {
    hb_memcpy (features, user_features, അക്കം_user_features * വലുപ്പം (hb_feature_t));
    /* Make start/end uniform to easier catch bugs. */
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_user_features; i++)
    {
      എങ്കിൽ (features[0].start != HB_FEATURE_GLOBAL_START)
	features[0].start = 1;
      എങ്കിൽ (features[0].end   != HB_FEATURE_GLOBAL_END)
	features[0].end   = 2;
    }
  }
  ഇത്->shaper_func = nullptr;
  ഇത്->shaper_name = nullptr;
#ifndef HB_NO_OT_SHAPE
  എങ്കിൽ (unlikely (!ഇത്->ot.init (face, coords, അക്കം_coords)))
    പോകുക bail;
#endif

  /*
   * Choose shaper.
   */

#define HB_SHAPER_PLAN(shaper) \
	HB_STMT_START { \
	  എങ്കിൽ (face->data.shaper) \
	  { \
	    ഇത്->shaper_func = _hb_##shaper##_shape; \
	    ഇത്->shaper_name = #shaper; \
	    തിരികെ സത്യം; \
	  } \
	} HB_STMT_END

  എങ്കിൽ (unlikely (shaper_list))
  {
    വേണ്ടി (; *shaper_list; shaper_list++)
      എങ്കിൽ (അസത്യം)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      അല്ലെങ്കിൽ എങ്കിൽ (0 == strcmp (*shaper_list, #shaper)) \
	HB_SHAPER_PLAN (shaper);
#അന്തര്ഭാവ "hb-shaper-list.hh"

#undef HB_SHAPER_IMPLEMENT
  }
  അല്ലെങ്കിൽ
  {
    സ്ഥിരം HB_UNUSED hb_shaper_entry_t *shapers = _hb_shapers_get ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < HB_SHAPERS_എണ്ണം; i++)
      എങ്കിൽ (അസത്യം)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      അല്ലെങ്കിൽ എങ്കിൽ (shapers[i].func == _hb_##shaper##_shape) \
	HB_SHAPER_PLAN (shaper);
#അന്തര്ഭാവ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
#undef HB_SHAPER_PLAN

bail:
#ifndef HB_NO_OT_SHAPE
  ഇത്->ot.fini ();
#endif
  ::hb_free (features);
  തിരികെ അസത്യം;
}

തർക്കം
hb_shape_plan_താക്കോൽ_t::user_features_match (സ്ഥിരം hb_shape_plan_താക്കോൽ_t *other)
{
  എങ്കിൽ (ഇത്->അക്കം_user_features != other->അക്കം_user_features)
    തിരികെ അസത്യം;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_user_features; i++)
  {
    എങ്കിൽ (ഇത്->user_features[i].tag   != other->user_features[i].tag   ||
	ഇത്->user_features[i].value != other->user_features[i].value ||
	(ഇത്->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 ഇത്->user_features[i].end   == HB_FEATURE_GLOBAL_END) !=
	(other->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 other->user_features[i].end   == HB_FEATURE_GLOBAL_END))
      തിരികെ അസത്യം;
  }
  തിരികെ സത്യം;
}

തർക്കം
hb_shape_plan_താക്കോൽ_t::equal (സ്ഥിരം hb_shape_plan_താക്കോൽ_t *other)
{
  തിരികെ hb_segment_properties_equal (&ഇത്->props, &other->props) &&
	 ഇത്->user_features_match (other) &&
#ifndef HB_NO_OT_SHAPE
	 ഇത്->ot.equal (&other->ot) &&
#endif
	 ഇത്->shaper_func == other->shaper_func;
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
		      സ്ഥിരം hb_segment_properties_t *props,
		      സ്ഥിരം hb_feature_t            *user_features,
		      unsigned പൂർണ്ണസംഖ്യ                   അക്കം_user_features,
		      സ്ഥിരം അക്ഷരം * സ്ഥിരം            *shaper_list)
{
  തിരികെ hb_shape_plan_create2 (face, props,
				user_features, അക്കം_user_features,
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
		       സ്ഥിരം hb_segment_properties_t *props,
		       സ്ഥിരം hb_feature_t            *user_features,
		       unsigned പൂർണ്ണസംഖ്യ                   അക്കം_user_features,
		       സ്ഥിരം പൂർണ്ണസംഖ്യ                     *coords,
		       unsigned പൂർണ്ണസംഖ്യ                   അക്കം_coords,
		       സ്ഥിരം അക്ഷരം * സ്ഥിരം            *shaper_list)
{
  DEBUG_സന്ദേശം_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u num_coords=%u shaper_list=%p",
		  face,
		  അക്കം_user_features,
		  അക്കം_coords,
		  shaper_list);

  എങ്കിൽ (unlikely (!HB_DIRECTION_IS_VALID (props->direction)))
    തിരികെ hb_shape_plan_get_empty ();

  hb_shape_plan_t *shape_plan;

  എങ്കിൽ (unlikely (!props))
    പോകുക bail;
  എങ്കിൽ (!(shape_plan = hb_object_create<hb_shape_plan_t> ()))
    പോകുക bail;

  എങ്കിൽ (unlikely (!face))
    face = hb_face_get_empty ();
  hb_face_make_immutable (face);
  shape_plan->face_unsafe = face;

  എങ്കിൽ (unlikely (!shape_plan->key.init (സത്യം,
				       face,
				       props,
				       user_features,
				       അക്കം_user_features,
				       coords,
				       അക്കം_coords,
				       shaper_list)))
    പോകുക bail2;
#ifndef HB_NO_OT_SHAPE
  എങ്കിൽ (unlikely (!shape_plan->ot.init0 (face, &shape_plan->key)))
    പോകുക bail3;
#endif

  തിരികെ shape_plan;

#ifndef HB_NO_OT_SHAPE
bail3:
#endif
  shape_plan->key.fini ();
bail2:
  hb_free (shape_plan);
bail:
  തിരികെ hb_shape_plan_get_empty ();
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
  തിരികെ const_cast<hb_shape_plan_t *> (&Null (hb_shape_plan_t));
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
  തിരികെ hb_object_reference (shape_plan);
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
ശൂന്യം
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan)
{
  എങ്കിൽ (!hb_object_destroy (shape_plan)) തിരികെ;

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
			     hb_user_data_താക്കോൽ_t *key,
			     ശൂന്യം *              data,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  തിരികെ hb_object_set_user_data (shape_plan, key, data, destroy, replace);
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
ശൂന്യം *
hb_shape_plan_get_user_data (സ്ഥിരം hb_shape_plan_t *shape_plan,
			     hb_user_data_താക്കോൽ_t    *key)
{
  തിരികെ hb_object_get_user_data (shape_plan, key);
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
സ്ഥിരം അക്ഷരം *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan)
{
  തിരികെ shape_plan->key.shaper_name;
}


സ്ഥിതം തർക്കം
_hb_shape_plan_execute_internal (hb_shape_plan_t    *shape_plan,
				 hb_font_t          *font,
				 hb_buffer_t        *buffer,
				 സ്ഥിരം hb_feature_t *features,
				 unsigned പൂർണ്ണസംഖ്യ        അക്കം_features)
{
  DEBUG_സന്ദേശം_FUNC (SHAPE_PLAN, shape_plan,
		  "num_features=%u shaper_func=%p, shaper_name=%s",
		  അക്കം_features,
		  shape_plan->key.shaper_func,
		  shape_plan->key.shaper_name);

  എങ്കിൽ (unlikely (!buffer->len))
    തിരികെ സത്യം;

  പ്രതിജ്ഞാ (!hb_object_is_immutable (buffer));

  buffer->assert_unicode ();

  എങ്കിൽ (unlikely (!hb_object_is_valid (shape_plan)))
    തിരികെ അസത്യം;

  പ്രതിജ്ഞാ (shape_plan->face_unsafe == font->face);
  പ്രതിജ്ഞാ (hb_segment_properties_equal (&shape_plan->key.props, &buffer->props));

#define HB_SHAPER_EXECUTE(shaper) \
	HB_STMT_START { \
	  തിരികെ font->data.shaper && \
		 _hb_##shaper##_shape (shape_plan, font, buffer, features, അക്കം_features); \
	} HB_STMT_END

  എങ്കിൽ (അസത്യം)
    ;
#define HB_SHAPER_IMPLEMENT(shaper) \
  അല്ലെങ്കിൽ എങ്കിൽ (shape_plan->key.shaper_func == _hb_##shaper##_shape) \
    HB_SHAPER_EXECUTE (shaper);
#അന്തര്ഭാവ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

#undef HB_SHAPER_EXECUTE

  തിരികെ അസത്യം;
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
		       സ്ഥിരം hb_feature_t *features,
		       unsigned പൂർണ്ണസംഖ്യ        അക്കം_features)
{
  തർക്കം ret = _hb_shape_plan_execute_internal (shape_plan, font, buffer,
					      features, അക്കം_features);

  എങ്കിൽ (ret && buffer->content_type == HB_BUFFER_CONTENT_TYPE_UNICODE)
    buffer->content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;

  തിരികെ ret;
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
			     സ്ഥിരം hb_segment_properties_t *props,
			     സ്ഥിരം hb_feature_t            *user_features,
			     unsigned പൂർണ്ണസംഖ്യ                   അക്കം_user_features,
			     സ്ഥിരം അക്ഷരം * സ്ഥിരം            *shaper_list)
{
  തിരികെ hb_shape_plan_create_cached2 (face, props,
				       user_features, അക്കം_user_features,
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
			      സ്ഥിരം hb_segment_properties_t *props,
			      സ്ഥിരം hb_feature_t            *user_features,
			      unsigned പൂർണ്ണസംഖ്യ                   അക്കം_user_features,
			      സ്ഥിരം പൂർണ്ണസംഖ്യ                     *coords,
			      unsigned പൂർണ്ണസംഖ്യ                   അക്കം_coords,
			      സ്ഥിരം അക്ഷരം * സ്ഥിരം            *shaper_list)
{
  DEBUG_സന്ദേശം_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u shaper_list=%p",
		  face,
		  അക്കം_user_features,
		  shaper_list);

retry:
  hb_face_t::plan_കണ്ണി_t *cached_plan_nodes = face->shape_plans;

  തർക്കം dont_cache = !hb_object_is_valid (face);

  എങ്കിൽ (likely (!dont_cache))
  {
    hb_shape_plan_താക്കോൽ_t key;
    എങ്കിൽ (!key.init (അസത്യം,
		   face,
		   props,
		   user_features,
		   അക്കം_user_features,
		   coords,
		   അക്കം_coords,
		   shaper_list))
      തിരികെ hb_shape_plan_get_empty ();

    വേണ്ടി (hb_face_t::plan_കണ്ണി_t *node = cached_plan_nodes; node; node = node->next)
      എങ്കിൽ (node->shape_plan->key.equal (&key))
      {
	DEBUG_സന്ദേശം_FUNC (SHAPE_PLAN, node->shape_plan, "fulfilled from cache");
	തിരികെ hb_shape_plan_reference (node->shape_plan);
      }
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create2 (face, props,
						       user_features, അക്കം_user_features,
						       coords, അക്കം_coords,
						       shaper_list);

  എങ്കിൽ (unlikely (dont_cache))
    തിരികെ shape_plan;

  hb_face_t::plan_കണ്ണി_t *node = (hb_face_t::plan_കണ്ണി_t *) hb_calloc (1, വലുപ്പം (hb_face_t::plan_കണ്ണി_t));
  എങ്കിൽ (unlikely (!node))
    തിരികെ shape_plan;

  node->shape_plan = shape_plan;
  node->next = cached_plan_nodes;

  എങ്കിൽ (unlikely (!face->shape_plans.cmpexch (cached_plan_nodes, node)))
  {
    hb_shape_plan_destroy (shape_plan);
    hb_free (node);
    പോകുക retry;
  }
  DEBUG_സന്ദേശം_FUNC (SHAPE_PLAN, shape_plan, "inserted into cache");

  തിരികെ hb_shape_plan_reference (shape_plan);
}


#endif
