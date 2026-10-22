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

#شامل "hb.hh"
#شامل "hb-shape-plan.hh"
#شامل "hb-shaper.hh"
#شامل "hb-font.hh"
#شامل "hb-buffer.hh"


#اگر_غیر_تعریف_شدہ HB_NO_SHAPER

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

#اگر_تعریف_شدہ HAVE_HARFRUST
خارجی "C" خالی _hb_harfrust_shape_plan_destroy_rs (خالی *ڈیٹا);
#اگر_ختم

hb_shape_plan_t::~hb_shape_plan_t ()
{
#اگر_تعریف_شدہ HAVE_HARFRUST
  خالی *ڈیٹا = harfrust_ڈیٹا.get_relaxed ();
  اگر (ڈیٹا)
    _hb_harfrust_shape_plan_destroy_rs (ڈیٹا);
#اگر_ختم
  key.fini ();
}


/*
 * hb_shape_plan_key_t
 */

منطقی
hb_shape_plan_کلید_t::init (منطقی                           copy,
			   hb_face_t                     *face,
			   ثابت hb_segment_properties_t *props,
			   ثابت hb_feature_t            *user_features,
			   غیرعلامتی صحیح                   عدد_user_features,
			   ثابت صحیح                     *coords,
			   غیرعلامتی صحیح                   عدد_coords,
			   ثابت حرف * ثابت            *shaper_list)
{
  hb_feature_t *features = خالی_اشاریہ;
  اگر (copy && عدد_user_features && !(features = (hb_feature_t *) hb_calloc (عدد_user_features, حجم (hb_feature_t))))
    جاؤ bail;

  یہ->props = *props;
  یہ->عدد_user_features = عدد_user_features;
  یہ->user_features = copy ? features : user_features;
  اگر (copy && عدد_user_features)
  {
    hb_memcpy (features, user_features, عدد_user_features * حجم (hb_feature_t));
    /* Make start/end uniform to easier catch bugs. */
    برائے (غیرعلامتی صحیح i = 0; i < عدد_user_features; i++)
    {
      اگر (features[0].آغاز != HB_FEATURE_GLOBAL_آغاز)
	features[0].آغاز = 1;
      اگر (features[0].انتہا   != HB_FEATURE_GLOBAL_انتہا)
	features[0].انتہا   = 2;
    }
  }
  یہ->shaper_func = خالی_اشاریہ;
  یہ->shaper_name = خالی_اشاریہ;
#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE
  اگر (unlikely (!یہ->ot.init (face, coords, عدد_coords)))
    جاؤ bail;
#اگر_ختم

  /*
   * Choose shaper.
   */

#تعریف_کرو HB_SHAPER_PLAN(shaper) \
	HB_STMT_آغاز { \
	  اگر (face->ڈیٹا.shaper) \
	  { \
	    یہ->shaper_func = _hb_##shaper##_shape; \
	    یہ->shaper_name = #shaper; \
	    واپس سچ; \
	  } \
	} HB_STMT_انتہا

  اگر (unlikely (shaper_list))
  {
    برائے (; *shaper_list; shaper_list++)
      اگر (جھوٹ)
	;
#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) \
      ورنہ اگر (0 == strcmp (*shaper_list, #shaper)) \
	HB_SHAPER_PLAN (shaper);
#شامل "hb-shaper-list.hh"

#تعریف_منسوخ HB_SHAPER_IMPLEMENT
  }
  ورنہ
  {
    ثابت HB_غیر_مستعمل hb_shaper_اندراج_t *shapers = _hb_shapers_get ();
    برائے (غیرعلامتی صحیح i = 0; i < HB_SHAPERS_گنتی; i++)
      اگر (جھوٹ)
	;
#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) \
      ورنہ اگر (shapers[i].func == _hb_##shaper##_shape) \
	HB_SHAPER_PLAN (shaper);
#شامل "hb-shaper-list.hh"
#تعریف_منسوخ HB_SHAPER_IMPLEMENT
  }
#تعریف_منسوخ HB_SHAPER_PLAN

bail:
#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE
  یہ->ot.fini ();
#اگر_ختم
  ::hb_free (features);
  واپس جھوٹ;
}

منطقی
hb_shape_plan_کلید_t::user_features_مطابقت (ثابت hb_shape_plan_کلید_t *other)
{
  اگر (یہ->عدد_user_features != other->عدد_user_features)
    واپس جھوٹ;
  برائے (غیرعلامتی صحیح i = 0; i < عدد_user_features; i++)
  {
    اگر (یہ->user_features[i].لیبل_2   != other->user_features[i].لیبل_2   ||
	یہ->user_features[i].value != other->user_features[i].value ||
	(یہ->user_features[i].آغاز == HB_FEATURE_GLOBAL_آغاز &&
	 یہ->user_features[i].انتہا   == HB_FEATURE_GLOBAL_انتہا) !=
	(other->user_features[i].آغاز == HB_FEATURE_GLOBAL_آغاز &&
	 other->user_features[i].انتہا   == HB_FEATURE_GLOBAL_انتہا))
      واپس جھوٹ;
  }
  واپس سچ;
}

منطقی
hb_shape_plan_کلید_t::equal (ثابت hb_shape_plan_کلید_t *other)
{
  واپس hb_segment_properties_equal (&یہ->props, &other->props) &&
	 یہ->user_features_مطابقت (other) &&
#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE
	 یہ->ot.equal (&other->ot) &&
#اگر_ختم
	 یہ->shaper_func == other->shaper_func;
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
		      ثابت hb_segment_properties_t *props,
		      ثابت hb_feature_t            *user_features,
		      غیرعلامتی صحیح                   عدد_user_features,
		      ثابت حرف * ثابت            *shaper_list)
{
  واپس hb_shape_plan_create2 (face, props,
				user_features, عدد_user_features,
				خالی_اشاریہ, 0,
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
		       ثابت hb_segment_properties_t *props,
		       ثابت hb_feature_t            *user_features,
		       غیرعلامتی صحیح                   عدد_user_features,
		       ثابت صحیح                     *coords,
		       غیرعلامتی صحیح                   عدد_coords,
		       ثابت حرف * ثابت            *shaper_list)
{
  DEBUG_پیغام_FUNC (SHAPE_PLAN, خالی_اشاریہ,
		  "face=%p num_features=%u num_coords=%u shaper_list=%p",
		  face,
		  عدد_user_features,
		  عدد_coords,
		  shaper_list);

  اگر (unlikely (!HB_DIRECTION_IS_VALID (props->direction)))
    واپس hb_shape_plan_get_خالی_خانے ();

  hb_shape_plan_t *shape_plan;

  اگر (unlikely (!props))
    جاؤ bail;
  اگر (!(shape_plan = hb_object_create<hb_shape_plan_t> ()))
    جاؤ bail;

  اگر (unlikely (!face))
    face = hb_face_get_خالی_خانے ();
  hb_face_make_immutable (face);
  shape_plan->face_unsafe = face;

  اگر (unlikely (!shape_plan->key.init (سچ,
				       face,
				       props,
				       user_features,
				       عدد_user_features,
				       coords,
				       عدد_coords,
				       shaper_list)))
    جاؤ bail2;
#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE
  اگر (unlikely (!shape_plan->ot.init0 (face, &shape_plan->key)))
    جاؤ bail3;
#اگر_ختم

  واپس shape_plan;

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE
bail3:
#اگر_ختم
  shape_plan->key.fini ();
bail2:
  hb_free (shape_plan);
bail:
  واپس hb_shape_plan_get_خالی_خانے ();
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
hb_shape_plan_get_خالی_خانے ()
{
  واپس const_cast<hb_shape_plan_t *> (&Null (hb_shape_plan_t));
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
  واپس hb_object_reference (shape_plan);
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
خالی
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan)
{
  اگر (!hb_object_destroy (shape_plan)) واپس;

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
hb_shape_plan_سیٹ_user_ڈیٹا (hb_shape_plan_t    *shape_plan,
			     hb_user_ڈیٹا_کلید_t *key,
			     خالی *              ڈیٹا,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  واپس hb_object_سیٹ_user_ڈیٹا (shape_plan, key, ڈیٹا, destroy, replace);
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
خالی *
hb_shape_plan_get_user_ڈیٹا (ثابت hb_shape_plan_t *shape_plan,
			     hb_user_ڈیٹا_کلید_t    *key)
{
  واپس hb_object_get_user_ڈیٹا (shape_plan, key);
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
ثابت حرف *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan)
{
  واپس shape_plan->key.shaper_name;
}


ساکن منطقی
hb_shape_plan_execute_اندرونی (hb_shape_plan_t    *shape_plan,
				 hb_font_t          *font,
				 hb_بفر_t        *بفر,
				 ثابت hb_feature_t *features,
				 غیرعلامتی صحیح        عدد_features)
{
  DEBUG_پیغام_FUNC (SHAPE_PLAN, shape_plan,
		  "num_features=%u shaper_func=%p, shaper_name=%s",
		  عدد_features,
		  shape_plan->key.shaper_func,
		  shape_plan->key.shaper_name);

  اگر (unlikely (!بفر->len))
    واپس سچ;

  أكد (!hb_object_is_immutable (بفر));

  بفر->assert_unicode ();

  اگر (unlikely (!hb_object_is_valid (shape_plan)))
    واپس جھوٹ;

  أكد (shape_plan->face_unsafe == font->face);
  أكد (hb_segment_properties_equal (&shape_plan->key.props, &بفر->props));

#تعریف_کرو HB_SHAPER_EXECUTE(shaper) \
	HB_STMT_آغاز { \
	  واپس font->ڈیٹا.shaper && \
		 _hb_##shaper##_shape (shape_plan, font, بفر, features, عدد_features); \
	} HB_STMT_انتہا

  اگر (جھوٹ)
    ;
#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) \
  ورنہ اگر (shape_plan->key.shaper_func == _hb_##shaper##_shape) \
    HB_SHAPER_EXECUTE (shaper);
#شامل "hb-shaper-list.hh"
#تعریف_منسوخ HB_SHAPER_IMPLEMENT

#تعریف_منسوخ HB_SHAPER_EXECUTE

  واپس جھوٹ;
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
		       hb_بفر_t        *بفر,
		       ثابت hb_feature_t *features,
		       غیرعلامتی صحیح        عدد_features)
{
  منطقی ret = hb_shape_plan_execute_اندرونی (shape_plan, font, بفر,
					      features, عدد_features);

  اگر (ret && بفر->content_type == HB_بفر_CONTENT_TYPE_UNICODE)
    بفر->content_type = HB_بفر_CONTENT_TYPE_GLYPHS;

  واپس ret;
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
			     ثابت hb_segment_properties_t *props,
			     ثابت hb_feature_t            *user_features,
			     غیرعلامتی صحیح                   عدد_user_features,
			     ثابت حرف * ثابت            *shaper_list)
{
  واپس hb_shape_plan_create_cached2 (face, props,
				       user_features, عدد_user_features,
				       خالی_اشاریہ, 0,
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
			      ثابت hb_segment_properties_t *props,
			      ثابت hb_feature_t            *user_features,
			      غیرعلامتی صحیح                   عدد_user_features,
			      ثابت صحیح                     *coords,
			      غیرعلامتی صحیح                   عدد_coords,
			      ثابت حرف * ثابت            *shaper_list)
{
  DEBUG_پیغام_FUNC (SHAPE_PLAN, خالی_اشاریہ,
		  "face=%p num_features=%u shaper_list=%p",
		  face,
		  عدد_user_features,
		  shaper_list);

retry:
  hb_face_t::plan_گرہ_t *cached_plan_nodes = face->shape_plans;

  منطقی dont_cache = !hb_object_is_valid (face);

  اگر (likely (!dont_cache))
  {
    hb_shape_plan_کلید_t key;
    اگر (!key.init (جھوٹ,
		   face,
		   props,
		   user_features,
		   عدد_user_features,
		   coords,
		   عدد_coords,
		   shaper_list))
      واپس hb_shape_plan_get_خالی_خانے ();

    برائے (hb_face_t::plan_گرہ_t *node = cached_plan_nodes; node; node = node->next)
      اگر (node->shape_plan->key.equal (&key))
      {
	DEBUG_پیغام_FUNC (SHAPE_PLAN, node->shape_plan, "fulfilled from cache");
	واپس hb_shape_plan_reference (node->shape_plan);
      }
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create2 (face, props,
						       user_features, عدد_user_features,
						       coords, عدد_coords,
						       shaper_list);

  اگر (unlikely (dont_cache))
    واپس shape_plan;

  hb_face_t::plan_گرہ_t *node = (hb_face_t::plan_گرہ_t *) hb_calloc (1, حجم (hb_face_t::plan_گرہ_t));
  اگر (unlikely (!node))
    واپس shape_plan;

  node->shape_plan = shape_plan;
  node->next = cached_plan_nodes;

  اگر (unlikely (!face->shape_plans.cmpexch (cached_plan_nodes, node)))
  {
    hb_shape_plan_destroy (shape_plan);
    hb_free (node);
    جاؤ retry;
  }
  DEBUG_پیغام_FUNC (SHAPE_PLAN, shape_plan, "inserted into cache");

  واپس hb_shape_plan_reference (shape_plan);
}


#اگر_ختم
