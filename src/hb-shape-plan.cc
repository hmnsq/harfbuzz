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

#समावेश "hb.hh"
#समावेश "hb-shape-plan.hh"
#समावेश "hb-shaper.hh"
#समावेश "hb-font.hh"
#समावेश "hb-buffer.hh"


#अगर_अघोषित HB_NO_SHAPER

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

#अगर_घोषित HAVE_HARFRUST
बाह्य "C" व्योम _hb_harfrust_shape_plan_destroy_rs (व्योम *तथ्य);
#पूर्ण_अगर

hb_shape_plan_t::~hb_shape_plan_t ()
{
#अगर_घोषित HAVE_HARFRUST
  व्योम *तथ्य = harfrust_तथ्य.get_relaxed ();
  अगर (तथ्य)
    _hb_harfrust_shape_plan_destroy_rs (तथ्य);
#पूर्ण_अगर
  key.fini ();
}


/*
 * hb_shape_plan_key_t
 */

bool
hb_shape_plan_कुंजी_t::init (bool                           copy,
			   hb_face_t                     *face,
			   स्थिर hb_segment_properties_t *props,
			   स्थिर hb_feature_t            *user_features,
			   अचिन्हित पूर्णांक                   अंक_user_features,
			   स्थिर पूर्णांक                     *coords,
			   अचिन्हित पूर्णांक                   अंक_coords,
			   स्थिर अक्षर * स्थिर            *shaper_list)
{
  hb_feature_t *features = nullptr;
  अगर (copy && अंक_user_features && !(features = (hb_feature_t *) hb_calloc (अंक_user_features, माप (hb_feature_t))))
    जाओ bail;

  यह->props = *props;
  यह->अंक_user_features = अंक_user_features;
  यह->user_features = copy ? features : user_features;
  अगर (copy && अंक_user_features)
  {
    hb_memcpy (features, user_features, अंक_user_features * माप (hb_feature_t));
    /* Make start/end uniform to easier catch bugs. */
    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_user_features; i++)
    {
      अगर (features[0].start != HB_FEATURE_GLOBAL_START)
	features[0].start = 1;
      अगर (features[0].end   != HB_FEATURE_GLOBAL_END)
	features[0].end   = 2;
    }
  }
  यह->shaper_func = nullptr;
  यह->shaper_name = nullptr;
#अगर_अघोषित HB_NO_OT_SHAPE
  अगर (unlikely (!यह->ot.init (face, coords, अंक_coords)))
    जाओ bail;
#पूर्ण_अगर

  /*
   * Choose shaper.
   */

#घोषणा HB_SHAPER_PLAN(shaper) \
	HB_STMT_START { \
	  अगर (face->तथ्य.shaper) \
	  { \
	    यह->shaper_func = _hb_##shaper##_shape; \
	    यह->shaper_name = #shaper; \
	    वापस true; \
	  } \
	} HB_STMT_END

  अगर (unlikely (shaper_list))
  {
    क्रम (; *shaper_list; shaper_list++)
      अगर (false)
	;
#घोषणा HB_SHAPER_IMPLEMENT(shaper) \
      अन्यथा अगर (0 == म_भेद (*shaper_list, #shaper)) \
	HB_SHAPER_PLAN (shaper);
#समावेश "hb-shaper-list.hh"

#अघोषित HB_SHAPER_IMPLEMENT
  }
  अन्यथा
  {
    स्थिर HB_UNUSED hb_shaper_entry_t *shapers = _hb_shapers_get ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < HB_SHAPERS_गिनती; i++)
      अगर (false)
	;
#घोषणा HB_SHAPER_IMPLEMENT(shaper) \
      अन्यथा अगर (shapers[i].func == _hb_##shaper##_shape) \
	HB_SHAPER_PLAN (shaper);
#समावेश "hb-shaper-list.hh"
#अघोषित HB_SHAPER_IMPLEMENT
  }
#अघोषित HB_SHAPER_PLAN

bail:
#अगर_अघोषित HB_NO_OT_SHAPE
  यह->ot.fini ();
#पूर्ण_अगर
  ::hb_free (features);
  वापस false;
}

bool
hb_shape_plan_कुंजी_t::user_features_match (स्थिर hb_shape_plan_कुंजी_t *other)
{
  अगर (यह->अंक_user_features != other->अंक_user_features)
    वापस false;
  क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_user_features; i++)
  {
    अगर (यह->user_features[i].tag   != other->user_features[i].tag   ||
	यह->user_features[i].value != other->user_features[i].value ||
	(यह->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 यह->user_features[i].end   == HB_FEATURE_GLOBAL_END) !=
	(other->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 other->user_features[i].end   == HB_FEATURE_GLOBAL_END))
      वापस false;
  }
  वापस true;
}

bool
hb_shape_plan_कुंजी_t::equal (स्थिर hb_shape_plan_कुंजी_t *other)
{
  वापस hb_segment_properties_equal (&यह->props, &other->props) &&
	 यह->user_features_match (other) &&
#अगर_अघोषित HB_NO_OT_SHAPE
	 यह->ot.equal (&other->ot) &&
#पूर्ण_अगर
	 यह->shaper_func == other->shaper_func;
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
		      स्थिर hb_segment_properties_t *props,
		      स्थिर hb_feature_t            *user_features,
		      अचिन्हित पूर्णांक                   अंक_user_features,
		      स्थिर अक्षर * स्थिर            *shaper_list)
{
  वापस hb_shape_plan_create2 (face, props,
				user_features, अंक_user_features,
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
		       स्थिर hb_segment_properties_t *props,
		       स्थिर hb_feature_t            *user_features,
		       अचिन्हित पूर्णांक                   अंक_user_features,
		       स्थिर पूर्णांक                     *coords,
		       अचिन्हित पूर्णांक                   अंक_coords,
		       स्थिर अक्षर * स्थिर            *shaper_list)
{
  DEBUG_संदेश_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u num_coords=%u shaper_list=%p",
		  face,
		  अंक_user_features,
		  अंक_coords,
		  shaper_list);

  अगर (unlikely (!HB_DIRECTION_IS_VALID (props->direction)))
    वापस hb_shape_plan_get_empty ();

  hb_shape_plan_t *shape_plan;

  अगर (unlikely (!props))
    जाओ bail;
  अगर (!(shape_plan = hb_object_create<hb_shape_plan_t> ()))
    जाओ bail;

  अगर (unlikely (!face))
    face = hb_face_get_empty ();
  hb_face_make_immutable (face);
  shape_plan->face_unsafe = face;

  अगर (unlikely (!shape_plan->key.init (true,
				       face,
				       props,
				       user_features,
				       अंक_user_features,
				       coords,
				       अंक_coords,
				       shaper_list)))
    जाओ bail2;
#अगर_अघोषित HB_NO_OT_SHAPE
  अगर (unlikely (!shape_plan->ot.init0 (face, &shape_plan->key)))
    जाओ bail3;
#पूर्ण_अगर

  वापस shape_plan;

#अगर_अघोषित HB_NO_OT_SHAPE
bail3:
#पूर्ण_अगर
  shape_plan->key.fini ();
bail2:
  hb_free (shape_plan);
bail:
  वापस hb_shape_plan_get_empty ();
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
  वापस const_cast<hb_shape_plan_t *> (&Null (hb_shape_plan_t));
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
  वापस hb_object_reference (shape_plan);
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
व्योम
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan)
{
  अगर (!hb_object_destroy (shape_plan)) वापस;

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
hb_shape_plan_set_user_तथ्य (hb_shape_plan_t    *shape_plan,
			     hb_user_तथ्य_कुंजी_t *key,
			     व्योम *              तथ्य,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  वापस hb_object_set_user_तथ्य (shape_plan, key, तथ्य, destroy, replace);
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
व्योम *
hb_shape_plan_get_user_तथ्य (स्थिर hb_shape_plan_t *shape_plan,
			     hb_user_तथ्य_कुंजी_t    *key)
{
  वापस hb_object_get_user_तथ्य (shape_plan, key);
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
स्थिर अक्षर *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan)
{
  वापस shape_plan->key.shaper_name;
}


अटल bool
_hb_shape_plan_execute_internal (hb_shape_plan_t    *shape_plan,
				 hb_font_t          *font,
				 hb_बफर_t        *बफर,
				 स्थिर hb_feature_t *features,
				 अचिन्हित पूर्णांक        अंक_features)
{
  DEBUG_संदेश_FUNC (SHAPE_PLAN, shape_plan,
		  "num_features=%u shaper_func=%p, shaper_name=%s",
		  अंक_features,
		  shape_plan->key.shaper_func,
		  shape_plan->key.shaper_name);

  अगर (unlikely (!बफर->len))
    वापस true;

  निश्चित (!hb_object_is_immutable (बफर));

  बफर->assert_unicode ();

  अगर (unlikely (!hb_object_is_valid (shape_plan)))
    वापस false;

  निश्चित (shape_plan->face_unsafe == font->face);
  निश्चित (hb_segment_properties_equal (&shape_plan->key.props, &बफर->props));

#घोषणा HB_SHAPER_EXECUTE(shaper) \
	HB_STMT_START { \
	  वापस font->तथ्य.shaper && \
		 _hb_##shaper##_shape (shape_plan, font, बफर, features, अंक_features); \
	} HB_STMT_END

  अगर (false)
    ;
#घोषणा HB_SHAPER_IMPLEMENT(shaper) \
  अन्यथा अगर (shape_plan->key.shaper_func == _hb_##shaper##_shape) \
    HB_SHAPER_EXECUTE (shaper);
#समावेश "hb-shaper-list.hh"
#अघोषित HB_SHAPER_IMPLEMENT

#अघोषित HB_SHAPER_EXECUTE

  वापस false;
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
		       hb_बफर_t        *बफर,
		       स्थिर hb_feature_t *features,
		       अचिन्हित पूर्णांक        अंक_features)
{
  bool ret = _hb_shape_plan_execute_internal (shape_plan, font, बफर,
					      features, अंक_features);

  अगर (ret && बफर->content_type == HB_बफर_CONTENT_TYPE_UNICODE)
    बफर->content_type = HB_बफर_CONTENT_TYPE_GLYPHS;

  वापस ret;
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
			     स्थिर hb_segment_properties_t *props,
			     स्थिर hb_feature_t            *user_features,
			     अचिन्हित पूर्णांक                   अंक_user_features,
			     स्थिर अक्षर * स्थिर            *shaper_list)
{
  वापस hb_shape_plan_create_cached2 (face, props,
				       user_features, अंक_user_features,
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
			      स्थिर hb_segment_properties_t *props,
			      स्थिर hb_feature_t            *user_features,
			      अचिन्हित पूर्णांक                   अंक_user_features,
			      स्थिर पूर्णांक                     *coords,
			      अचिन्हित पूर्णांक                   अंक_coords,
			      स्थिर अक्षर * स्थिर            *shaper_list)
{
  DEBUG_संदेश_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u shaper_list=%p",
		  face,
		  अंक_user_features,
		  shaper_list);

retry:
  hb_face_t::plan_गाँठ_t *cached_plan_nodes = face->shape_plans;

  bool dont_cache = !hb_object_is_valid (face);

  अगर (likely (!dont_cache))
  {
    hb_shape_plan_कुंजी_t key;
    अगर (!key.init (false,
		   face,
		   props,
		   user_features,
		   अंक_user_features,
		   coords,
		   अंक_coords,
		   shaper_list))
      वापस hb_shape_plan_get_empty ();

    क्रम (hb_face_t::plan_गाँठ_t *node = cached_plan_nodes; node; node = node->next)
      अगर (node->shape_plan->key.equal (&key))
      {
	DEBUG_संदेश_FUNC (SHAPE_PLAN, node->shape_plan, "fulfilled from cache");
	वापस hb_shape_plan_reference (node->shape_plan);
      }
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create2 (face, props,
						       user_features, अंक_user_features,
						       coords, अंक_coords,
						       shaper_list);

  अगर (unlikely (dont_cache))
    वापस shape_plan;

  hb_face_t::plan_गाँठ_t *node = (hb_face_t::plan_गाँठ_t *) hb_calloc (1, माप (hb_face_t::plan_गाँठ_t));
  अगर (unlikely (!node))
    वापस shape_plan;

  node->shape_plan = shape_plan;
  node->next = cached_plan_nodes;

  अगर (unlikely (!face->shape_plans.cmpexch (cached_plan_nodes, node)))
  {
    hb_shape_plan_destroy (shape_plan);
    hb_free (node);
    जाओ retry;
  }
  DEBUG_संदेश_FUNC (SHAPE_PLAN, shape_plan, "inserted into cache");

  वापस hb_shape_plan_reference (shape_plan);
}


#पूर्ण_अगर
