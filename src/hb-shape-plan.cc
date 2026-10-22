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

#ضمن "hb.hh"
#ضمن "hb-shape-plan.hh"
#ضمن "hb-shaper.hh"
#ضمن "hb-font.hh"
#ضمن "hb-buffer.hh"


#اگر_تعریف_نشده HB_NO_SHAPER

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

#اگر_تعریف_شده HAVE_HARFRUST
خارجي "C" فراغ _hb_harfrust_shape_plan_destroy_rs (فراغ *داده);
#پایان_اگر

hb_shape_plan_t::~hb_shape_plan_t ()
{
#اگر_تعریف_شده HAVE_HARFRUST
  فراغ *داده = harfrust_داده.get_relaxed ();
  إذا (داده)
    _hb_harfrust_shape_plan_destroy_rs (داده);
#پایان_اگر
  key.fini ();
}


/*
 * hb_shape_plan_key_t
 */

منطقي
hb_shape_plan_کلید_t::init (منطقي                           copy,
			   hb_face_t                     *face,
			   ثابت hb_segment_properties_t *props,
			   ثابت hb_feature_t            *user_features,
			   بیعلامت صحيح                   رقم_user_features,
			   ثابت صحيح                     *coords,
			   بیعلامت صحيح                   رقم_coords,
			   ثابت حرف * ثابت            *shaper_list)
{
  hb_feature_t *features = اشاره_تهی;
  إذا (copy && رقم_user_features && !(features = (hb_feature_t *) hb_calloc (رقم_user_features, حجم (hb_feature_t))))
    اذهب bail;

  هذا->props = *props;
  هذا->رقم_user_features = رقم_user_features;
  هذا->user_features = copy ? features : user_features;
  إذا (copy && رقم_user_features)
  {
    hb_memcpy (features, user_features, رقم_user_features * حجم (hb_feature_t));
    /* Make start/end uniform to easier catch bugs. */
    لكل (بیعلامت صحيح i = 0; i < رقم_user_features; i++)
    {
      إذا (features[0].شروع != HB_FEATURE_GLOBAL_شروع)
	features[0].شروع = 1;
      إذا (features[0].انتها   != HB_FEATURE_GLOBAL_انتها)
	features[0].انتها   = 2;
    }
  }
  هذا->shaper_func = اشاره_تهی;
  هذا->shaper_name = اشاره_تهی;
#اگر_تعریف_نشده HB_NO_OT_SHAPE
  إذا (unlikely (!هذا->ot.init (face, coords, رقم_coords)))
    اذهب bail;
#پایان_اگر

  /*
   * Choose shaper.
   */

#تعریف_کن HB_SHAPER_PLAN(shaper) \
	HB_STMT_شروع { \
	  إذا (face->داده.shaper) \
	  { \
	    هذا->shaper_func = _hb_##shaper##_shape; \
	    هذا->shaper_name = #shaper; \
	    أرجع درست; \
	  } \
	} HB_STMT_انتها

  إذا (unlikely (shaper_list))
  {
    لكل (; *shaper_list; shaper_list++)
      إذا (خطأ)
	;
#تعریف_کن HB_SHAPER_IMPLEMENT(shaper) \
      وإلا إذا (0 == strcmp (*shaper_list, #shaper)) \
	HB_SHAPER_PLAN (shaper);
#ضمن "hb-shaper-list.hh"

#لغو_تعریف HB_SHAPER_IMPLEMENT
  }
  وإلا
  {
    ثابت HB_استفادهنشده hb_shaper_مدخل_t *shapers = _hb_shapers_get ();
    لكل (بیعلامت صحيح i = 0; i < HB_SHAPERS_شمار; i++)
      إذا (خطأ)
	;
#تعریف_کن HB_SHAPER_IMPLEMENT(shaper) \
      وإلا إذا (shapers[i].func == _hb_##shaper##_shape) \
	HB_SHAPER_PLAN (shaper);
#ضمن "hb-shaper-list.hh"
#لغو_تعریف HB_SHAPER_IMPLEMENT
  }
#لغو_تعریف HB_SHAPER_PLAN

bail:
#اگر_تعریف_نشده HB_NO_OT_SHAPE
  هذا->ot.fini ();
#پایان_اگر
  ::hb_free (features);
  أرجع خطأ;
}

منطقي
hb_shape_plan_کلید_t::user_features_تطابق (ثابت hb_shape_plan_کلید_t *other)
{
  إذا (هذا->رقم_user_features != other->رقم_user_features)
    أرجع خطأ;
  لكل (بیعلامت صحيح i = 0; i < رقم_user_features; i++)
  {
    إذا (هذا->user_features[i].برچسب_2   != other->user_features[i].برچسب_2   ||
	هذا->user_features[i].value != other->user_features[i].value ||
	(هذا->user_features[i].شروع == HB_FEATURE_GLOBAL_شروع &&
	 هذا->user_features[i].انتها   == HB_FEATURE_GLOBAL_انتها) !=
	(other->user_features[i].شروع == HB_FEATURE_GLOBAL_شروع &&
	 other->user_features[i].انتها   == HB_FEATURE_GLOBAL_انتها))
      أرجع خطأ;
  }
  أرجع درست;
}

منطقي
hb_shape_plan_کلید_t::equal (ثابت hb_shape_plan_کلید_t *other)
{
  أرجع hb_segment_properties_equal (&هذا->props, &other->props) &&
	 هذا->user_features_تطابق (other) &&
#اگر_تعریف_نشده HB_NO_OT_SHAPE
	 هذا->ot.equal (&other->ot) &&
#پایان_اگر
	 هذا->shaper_func == other->shaper_func;
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
		      بیعلامت صحيح                   رقم_user_features,
		      ثابت حرف * ثابت            *shaper_list)
{
  أرجع hb_shape_plan_create2 (face, props,
				user_features, رقم_user_features,
				اشاره_تهی, 0,
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
		       بیعلامت صحيح                   رقم_user_features,
		       ثابت صحيح                     *coords,
		       بیعلامت صحيح                   رقم_coords,
		       ثابت حرف * ثابت            *shaper_list)
{
  DEBUG_پیام_FUNC (SHAPE_PLAN, اشاره_تهی,
		  "face=%p num_features=%u num_coords=%u shaper_list=%p",
		  face,
		  رقم_user_features,
		  رقم_coords,
		  shaper_list);

  إذا (unlikely (!HB_DIRECTION_IS_VALID (props->direction)))
    أرجع hb_shape_plan_get_خالی ();

  hb_shape_plan_t *shape_plan;

  إذا (unlikely (!props))
    اذهب bail;
  إذا (!(shape_plan = hb_object_create<hb_shape_plan_t> ()))
    اذهب bail;

  إذا (unlikely (!face))
    face = hb_face_get_خالی ();
  hb_face_make_immutable (face);
  shape_plan->face_unsafe = face;

  إذا (unlikely (!shape_plan->key.init (درست,
				       face,
				       props,
				       user_features,
				       رقم_user_features,
				       coords,
				       رقم_coords,
				       shaper_list)))
    اذهب bail2;
#اگر_تعریف_نشده HB_NO_OT_SHAPE
  إذا (unlikely (!shape_plan->ot.init0 (face, &shape_plan->key)))
    اذهب bail3;
#پایان_اگر

  أرجع shape_plan;

#اگر_تعریف_نشده HB_NO_OT_SHAPE
bail3:
#پایان_اگر
  shape_plan->key.fini ();
bail2:
  hb_free (shape_plan);
bail:
  أرجع hb_shape_plan_get_خالی ();
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
hb_shape_plan_get_خالی ()
{
  أرجع const_cast<hb_shape_plan_t *> (&Null (hb_shape_plan_t));
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
  أرجع hb_object_reference (shape_plan);
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
فراغ
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan)
{
  إذا (!hb_object_destroy (shape_plan)) أرجع;

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
hb_shape_plan_مجموعه_user_داده (hb_shape_plan_t    *shape_plan,
			     hb_user_داده_کلید_t *key,
			     فراغ *              داده,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  أرجع hb_object_مجموعه_user_داده (shape_plan, key, داده, destroy, replace);
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
فراغ *
hb_shape_plan_get_user_داده (ثابت hb_shape_plan_t *shape_plan,
			     hb_user_داده_کلید_t    *key)
{
  أرجع hb_object_get_user_داده (shape_plan, key);
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
  أرجع shape_plan->key.shaper_name;
}


ساكن منطقي
hb_shape_plan_execute_داخلی (hb_shape_plan_t    *shape_plan,
				 hb_font_t          *font,
				 hb_میانگیر_t        *میانگیر,
				 ثابت hb_feature_t *features,
				 بیعلامت صحيح        رقم_features)
{
  DEBUG_پیام_FUNC (SHAPE_PLAN, shape_plan,
		  "num_features=%u shaper_func=%p, shaper_name=%s",
		  رقم_features,
		  shape_plan->key.shaper_func,
		  shape_plan->key.shaper_name);

  إذا (unlikely (!میانگیر->len))
    أرجع درست;

  أكد (!hb_object_is_immutable (میانگیر));

  میانگیر->assert_unicode ();

  إذا (unlikely (!hb_object_is_valid (shape_plan)))
    أرجع خطأ;

  أكد (shape_plan->face_unsafe == font->face);
  أكد (hb_segment_properties_equal (&shape_plan->key.props, &میانگیر->props));

#تعریف_کن HB_SHAPER_EXECUTE(shaper) \
	HB_STMT_شروع { \
	  أرجع font->داده.shaper && \
		 _hb_##shaper##_shape (shape_plan, font, میانگیر, features, رقم_features); \
	} HB_STMT_انتها

  إذا (خطأ)
    ;
#تعریف_کن HB_SHAPER_IMPLEMENT(shaper) \
  وإلا إذا (shape_plan->key.shaper_func == _hb_##shaper##_shape) \
    HB_SHAPER_EXECUTE (shaper);
#ضمن "hb-shaper-list.hh"
#لغو_تعریف HB_SHAPER_IMPLEMENT

#لغو_تعریف HB_SHAPER_EXECUTE

  أرجع خطأ;
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
		       hb_میانگیر_t        *میانگیر,
		       ثابت hb_feature_t *features,
		       بیعلامت صحيح        رقم_features)
{
  منطقي ret = hb_shape_plan_execute_داخلی (shape_plan, font, میانگیر,
					      features, رقم_features);

  إذا (ret && میانگیر->content_type == HB_میانگیر_CONTENT_TYPE_UNICODE)
    میانگیر->content_type = HB_میانگیر_CONTENT_TYPE_GLYPHS;

  أرجع ret;
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
			     بیعلامت صحيح                   رقم_user_features,
			     ثابت حرف * ثابت            *shaper_list)
{
  أرجع hb_shape_plan_create_cached2 (face, props,
				       user_features, رقم_user_features,
				       اشاره_تهی, 0,
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
			      بیعلامت صحيح                   رقم_user_features,
			      ثابت صحيح                     *coords,
			      بیعلامت صحيح                   رقم_coords,
			      ثابت حرف * ثابت            *shaper_list)
{
  DEBUG_پیام_FUNC (SHAPE_PLAN, اشاره_تهی,
		  "face=%p num_features=%u shaper_list=%p",
		  face,
		  رقم_user_features,
		  shaper_list);

retry:
  hb_face_t::plan_گره_t *cached_plan_nodes = face->shape_plans;

  منطقي dont_cache = !hb_object_is_valid (face);

  إذا (likely (!dont_cache))
  {
    hb_shape_plan_کلید_t key;
    إذا (!key.init (خطأ,
		   face,
		   props,
		   user_features,
		   رقم_user_features,
		   coords,
		   رقم_coords,
		   shaper_list))
      أرجع hb_shape_plan_get_خالی ();

    لكل (hb_face_t::plan_گره_t *node = cached_plan_nodes; node; node = node->next)
      إذا (node->shape_plan->key.equal (&key))
      {
	DEBUG_پیام_FUNC (SHAPE_PLAN, node->shape_plan, "fulfilled from cache");
	أرجع hb_shape_plan_reference (node->shape_plan);
      }
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create2 (face, props,
						       user_features, رقم_user_features,
						       coords, رقم_coords,
						       shaper_list);

  إذا (unlikely (dont_cache))
    أرجع shape_plan;

  hb_face_t::plan_گره_t *node = (hb_face_t::plan_گره_t *) hb_calloc (1, حجم (hb_face_t::plan_گره_t));
  إذا (unlikely (!node))
    أرجع shape_plan;

  node->shape_plan = shape_plan;
  node->next = cached_plan_nodes;

  إذا (unlikely (!face->shape_plans.cmpexch (cached_plan_nodes, node)))
  {
    hb_shape_plan_destroy (shape_plan);
    hb_free (node);
    اذهب retry;
  }
  DEBUG_پیام_FUNC (SHAPE_PLAN, shape_plan, "inserted into cache");

  أرجع hb_shape_plan_reference (shape_plan);
}


#پایان_اگر
