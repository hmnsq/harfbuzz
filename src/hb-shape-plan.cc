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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-shape-plan.hh"
#अन्तर्भाव "hb-shaper.hh"
#अन्तर्भाव "hb-font.hh"
#अन्तर्भाव "hb-buffer.hh"


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
बाह्य "C" शून्य _hb_harfrust_shape_plan_destroy_rs (शून्य *data);
#endif

hb_shape_plan_t::~hb_shape_plan_t ()
{
#ifdef HAVE_HARFRUST
  शून्य *data = harfrust_data.get_relaxed ();
  यदि (data)
    _hb_harfrust_shape_plan_destroy_rs (data);
#endif
  key.fini ();
}


/*
 * hb_shape_plan_key_t
 */

तार्किक
hb_shape_plan_साँचो_t::init (तार्किक                           copy,
			   hb_face_t                     *face,
			   अचर hb_segment_properties_t *props,
			   अचर hb_feature_t            *user_features,
			   अचिन्हित पूर्णाङ्क                   अङ्क_user_features,
			   अचर पूर्णाङ्क                     *coords,
			   अचिन्हित पूर्णाङ्क                   अङ्क_coords,
			   अचर वर्ण * अचर            *shaper_list)
{
  hb_feature_t *features = nullptr;
  यदि (copy && अङ्क_user_features && !(features = (hb_feature_t *) hb_calloc (अङ्क_user_features, परिमाण (hb_feature_t))))
    गच्छ bail;

  एतत्->props = *props;
  एतत्->अङ्क_user_features = अङ्क_user_features;
  एतत्->user_features = copy ? features : user_features;
  यदि (copy && अङ्क_user_features)
  {
    hb_memcpy (features, user_features, अङ्क_user_features * परिमाण (hb_feature_t));
    /* Make start/end uniform to easier catch bugs. */
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < अङ्क_user_features; i++)
    {
      यदि (features[0].start != HB_FEATURE_GLOBAL_START)
	features[0].start = 1;
      यदि (features[0].end   != HB_FEATURE_GLOBAL_END)
	features[0].end   = 2;
    }
  }
  एतत्->shaper_func = nullptr;
  एतत्->shaper_name = nullptr;
#ifndef HB_NO_OT_SHAPE
  यदि (unlikely (!एतत्->ot.init (face, coords, अङ्क_coords)))
    गच्छ bail;
#endif

  /*
   * Choose shaper.
   */

#define HB_SHAPER_PLAN(shaper) \
	HB_STMT_START { \
	  यदि (face->data.shaper) \
	  { \
	    एतत्->shaper_func = _hb_##shaper##_shape; \
	    एतत्->shaper_name = #shaper; \
	    फर्काऊ सत्य; \
	  } \
	} HB_STMT_END

  यदि (unlikely (shaper_list))
  {
    लागि (; *shaper_list; shaper_list++)
      यदि (असत्य)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      अन्यथा यदि (0 == म_भेद (*shaper_list, #shaper)) \
	HB_SHAPER_PLAN (shaper);
#अन्तर्भाव "hb-shaper-list.hh"

#undef HB_SHAPER_IMPLEMENT
  }
  अन्यथा
  {
    अचर HB_UNUSED hb_shaper_entry_t *shapers = _hb_shapers_get ();
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < HB_SHAPERS_गन्ती; i++)
      यदि (असत्य)
	;
#define HB_SHAPER_IMPLEMENT(shaper) \
      अन्यथा यदि (shapers[i].func == _hb_##shaper##_shape) \
	HB_SHAPER_PLAN (shaper);
#अन्तर्भाव "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
#undef HB_SHAPER_PLAN

bail:
#ifndef HB_NO_OT_SHAPE
  एतत्->ot.fini ();
#endif
  ::hb_free (features);
  फर्काऊ असत्य;
}

तार्किक
hb_shape_plan_साँचो_t::user_features_match (अचर hb_shape_plan_साँचो_t *other)
{
  यदि (एतत्->अङ्क_user_features != other->अङ्क_user_features)
    फर्काऊ असत्य;
  लागि (अचिन्हित पूर्णाङ्क i = 0; i < अङ्क_user_features; i++)
  {
    यदि (एतत्->user_features[i].tag   != other->user_features[i].tag   ||
	एतत्->user_features[i].value != other->user_features[i].value ||
	(एतत्->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 एतत्->user_features[i].end   == HB_FEATURE_GLOBAL_END) !=
	(other->user_features[i].start == HB_FEATURE_GLOBAL_START &&
	 other->user_features[i].end   == HB_FEATURE_GLOBAL_END))
      फर्काऊ असत्य;
  }
  फर्काऊ सत्य;
}

तार्किक
hb_shape_plan_साँचो_t::equal (अचर hb_shape_plan_साँचो_t *other)
{
  फर्काऊ hb_segment_properties_equal (&एतत्->props, &other->props) &&
	 एतत्->user_features_match (other) &&
#ifndef HB_NO_OT_SHAPE
	 एतत्->ot.equal (&other->ot) &&
#endif
	 एतत्->shaper_func == other->shaper_func;
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
		      अचर hb_segment_properties_t *props,
		      अचर hb_feature_t            *user_features,
		      अचिन्हित पूर्णाङ्क                   अङ्क_user_features,
		      अचर वर्ण * अचर            *shaper_list)
{
  फर्काऊ hb_shape_plan_create2 (face, props,
				user_features, अङ्क_user_features,
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
		       अचर hb_segment_properties_t *props,
		       अचर hb_feature_t            *user_features,
		       अचिन्हित पूर्णाङ्क                   अङ्क_user_features,
		       अचर पूर्णाङ्क                     *coords,
		       अचिन्हित पूर्णाङ्क                   अङ्क_coords,
		       अचर वर्ण * अचर            *shaper_list)
{
  DEBUG_सन्देश_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u num_coords=%u shaper_list=%p",
		  face,
		  अङ्क_user_features,
		  अङ्क_coords,
		  shaper_list);

  यदि (unlikely (!HB_DIRECTION_IS_VALID (props->direction)))
    फर्काऊ hb_shape_plan_get_empty ();

  hb_shape_plan_t *shape_plan;

  यदि (unlikely (!props))
    गच्छ bail;
  यदि (!(shape_plan = hb_object_create<hb_shape_plan_t> ()))
    गच्छ bail;

  यदि (unlikely (!face))
    face = hb_face_get_empty ();
  hb_face_make_immutable (face);
  shape_plan->face_unsafe = face;

  यदि (unlikely (!shape_plan->key.init (सत्य,
				       face,
				       props,
				       user_features,
				       अङ्क_user_features,
				       coords,
				       अङ्क_coords,
				       shaper_list)))
    गच्छ bail2;
#ifndef HB_NO_OT_SHAPE
  यदि (unlikely (!shape_plan->ot.init0 (face, &shape_plan->key)))
    गच्छ bail3;
#endif

  फर्काऊ shape_plan;

#ifndef HB_NO_OT_SHAPE
bail3:
#endif
  shape_plan->key.fini ();
bail2:
  hb_free (shape_plan);
bail:
  फर्काऊ hb_shape_plan_get_empty ();
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
  फर्काऊ const_cast<hb_shape_plan_t *> (&Null (hb_shape_plan_t));
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
  फर्काऊ hb_object_reference (shape_plan);
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
शून्य
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan)
{
  यदि (!hb_object_destroy (shape_plan)) फर्काऊ;

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
			     hb_user_data_साँचो_t *key,
			     शून्य *              data,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  फर्काऊ hb_object_set_user_data (shape_plan, key, data, destroy, replace);
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
शून्य *
hb_shape_plan_get_user_data (अचर hb_shape_plan_t *shape_plan,
			     hb_user_data_साँचो_t    *key)
{
  फर्काऊ hb_object_get_user_data (shape_plan, key);
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
अचर वर्ण *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan)
{
  फर्काऊ shape_plan->key.shaper_name;
}


स्थिर तार्किक
_hb_shape_plan_execute_internal (hb_shape_plan_t    *shape_plan,
				 hb_font_t          *font,
				 hb_buffer_t        *buffer,
				 अचर hb_feature_t *features,
				 अचिन्हित पूर्णाङ्क        अङ्क_features)
{
  DEBUG_सन्देश_FUNC (SHAPE_PLAN, shape_plan,
		  "num_features=%u shaper_func=%p, shaper_name=%s",
		  अङ्क_features,
		  shape_plan->key.shaper_func,
		  shape_plan->key.shaper_name);

  यदि (unlikely (!buffer->len))
    फर्काऊ सत्य;

  प्रतिज्ञा (!hb_object_is_immutable (buffer));

  buffer->assert_unicode ();

  यदि (unlikely (!hb_object_is_valid (shape_plan)))
    फर्काऊ असत्य;

  प्रतिज्ञा (shape_plan->face_unsafe == font->face);
  प्रतिज्ञा (hb_segment_properties_equal (&shape_plan->key.props, &buffer->props));

#define HB_SHAPER_EXECUTE(shaper) \
	HB_STMT_START { \
	  फर्काऊ font->data.shaper && \
		 _hb_##shaper##_shape (shape_plan, font, buffer, features, अङ्क_features); \
	} HB_STMT_END

  यदि (असत्य)
    ;
#define HB_SHAPER_IMPLEMENT(shaper) \
  अन्यथा यदि (shape_plan->key.shaper_func == _hb_##shaper##_shape) \
    HB_SHAPER_EXECUTE (shaper);
#अन्तर्भाव "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

#undef HB_SHAPER_EXECUTE

  फर्काऊ असत्य;
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
		       अचर hb_feature_t *features,
		       अचिन्हित पूर्णाङ्क        अङ्क_features)
{
  तार्किक ret = _hb_shape_plan_execute_internal (shape_plan, font, buffer,
					      features, अङ्क_features);

  यदि (ret && buffer->content_type == HB_BUFFER_CONTENT_TYPE_UNICODE)
    buffer->content_type = HB_BUFFER_CONTENT_TYPE_GLYPHS;

  फर्काऊ ret;
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
			     अचर hb_segment_properties_t *props,
			     अचर hb_feature_t            *user_features,
			     अचिन्हित पूर्णाङ्क                   अङ्क_user_features,
			     अचर वर्ण * अचर            *shaper_list)
{
  फर्काऊ hb_shape_plan_create_cached2 (face, props,
				       user_features, अङ्क_user_features,
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
			      अचर hb_segment_properties_t *props,
			      अचर hb_feature_t            *user_features,
			      अचिन्हित पूर्णाङ्क                   अङ्क_user_features,
			      अचर पूर्णाङ्क                     *coords,
			      अचिन्हित पूर्णाङ्क                   अङ्क_coords,
			      अचर वर्ण * अचर            *shaper_list)
{
  DEBUG_सन्देश_FUNC (SHAPE_PLAN, nullptr,
		  "face=%p num_features=%u shaper_list=%p",
		  face,
		  अङ्क_user_features,
		  shaper_list);

retry:
  hb_face_t::plan_गाँठो_t *cached_plan_nodes = face->shape_plans;

  तार्किक dont_cache = !hb_object_is_valid (face);

  यदि (likely (!dont_cache))
  {
    hb_shape_plan_साँचो_t key;
    यदि (!key.init (असत्य,
		   face,
		   props,
		   user_features,
		   अङ्क_user_features,
		   coords,
		   अङ्क_coords,
		   shaper_list))
      फर्काऊ hb_shape_plan_get_empty ();

    लागि (hb_face_t::plan_गाँठो_t *node = cached_plan_nodes; node; node = node->next)
      यदि (node->shape_plan->key.equal (&key))
      {
	DEBUG_सन्देश_FUNC (SHAPE_PLAN, node->shape_plan, "fulfilled from cache");
	फर्काऊ hb_shape_plan_reference (node->shape_plan);
      }
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create2 (face, props,
						       user_features, अङ्क_user_features,
						       coords, अङ्क_coords,
						       shaper_list);

  यदि (unlikely (dont_cache))
    फर्काऊ shape_plan;

  hb_face_t::plan_गाँठो_t *node = (hb_face_t::plan_गाँठो_t *) hb_calloc (1, परिमाण (hb_face_t::plan_गाँठो_t));
  यदि (unlikely (!node))
    फर्काऊ shape_plan;

  node->shape_plan = shape_plan;
  node->next = cached_plan_nodes;

  यदि (unlikely (!face->shape_plans.cmpexch (cached_plan_nodes, node)))
  {
    hb_shape_plan_destroy (shape_plan);
    hb_free (node);
    गच्छ retry;
  }
  DEBUG_सन्देश_FUNC (SHAPE_PLAN, shape_plan, "inserted into cache");

  फर्काऊ hb_shape_plan_reference (shape_plan);
}


#endif
