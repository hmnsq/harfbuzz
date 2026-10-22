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

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-shape-plan.hh"
#అన్తర్భావ "hb-shaper.hh"
#అన్తర్భావ "hb-font.hh"
#అన్తర్భావ "hb-buffer.hh"


#నిర్వచితం_కాకపోతే HB_NO_SHAPER

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

#నిర్వచితమైతే HAVE_HARFRUST
బాహ్య "C" శూన్య _hb_harfrust_shape_plan_destroy_rs (శూన్య *దత్తాంశం);
#అయితే_ముగింపు

hb_shape_plan_t::~hb_shape_plan_t ()
{
#నిర్వచితమైతే HAVE_HARFRUST
  శూన్య *దత్తాంశం = harfrust_దత్తాంశం.get_relaxed ();
  ఒకవేళ (దత్తాంశం)
    _hb_harfrust_shape_plan_destroy_rs (దత్తాంశం);
#అయితే_ముగింపు
  key.fini ();
}


/*
 * hb_shape_plan_key_t
 */

తార్కిక
hb_shape_plan_కీలకం_t::init (తార్కిక                           copy,
			   hb_face_t                     *face,
			   స్థిరరాశి hb_segment_properties_t *props,
			   స్థిరరాశి hb_feature_t            *user_features,
			   గుర్తులేని పూర్ణాంకం                   అంకె_user_features,
			   స్థిరరాశి పూర్ణాంకం                     *coords,
			   గుర్తులేని పూర్ణాంకం                   అంకె_coords,
			   స్థిరరాశి అక్షరం * స్థిరరాశి            *shaper_list)
{
  hb_feature_t *features = శూన్యసూచిక;
  ఒకవేళ (copy && అంకె_user_features && !(features = (hb_feature_t *) hb_calloc (అంకె_user_features, పరిమాణం (hb_feature_t))))
    వెళ్ళు bail;

  ఇది->props = *props;
  ఇది->అంకె_user_features = అంకె_user_features;
  ఇది->user_features = copy ? features : user_features;
  ఒకవేళ (copy && అంకె_user_features)
  {
    hb_memcpy (features, user_features, అంకె_user_features * పరిమాణం (hb_feature_t));
    /* Make start/end uniform to easier catch bugs. */
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_user_features; i++)
    {
      ఒకవేళ (features[0].ప్రారంభం != HB_FEATURE_GLOBAL_ప్రారంభం)
	features[0].ప్రారంభం = 1;
      ఒకవేళ (features[0].చివర   != HB_FEATURE_GLOBAL_చివర)
	features[0].చివర   = 2;
    }
  }
  ఇది->shaper_func = శూన్యసూచిక;
  ఇది->shaper_name = శూన్యసూచిక;
#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE
  ఒకవేళ (unlikely (!ఇది->ot.init (face, coords, అంకె_coords)))
    వెళ్ళు bail;
#అయితే_ముగింపు

  /*
   * Choose shaper.
   */

#నిర్వచించు HB_SHAPER_PLAN(shaper) \
	HB_STMT_ప్రారంభం { \
	  ఒకవేళ (face->దత్తాంశం.shaper) \
	  { \
	    ఇది->shaper_func = _hb_##shaper##_shape; \
	    ఇది->shaper_name = #shaper; \
	    తిరిగి నిజం; \
	  } \
	} HB_STMT_చివర

  ఒకవేళ (unlikely (shaper_list))
  {
    కొరకు (; *shaper_list; shaper_list++)
      ఒకవేళ (అబద్ధం)
	;
#నిర్వచించు HB_SHAPER_IMPLEMENT(shaper) \
      లేకపోతే ఒకవేళ (0 == strcmp (*shaper_list, #shaper)) \
	HB_SHAPER_PLAN (shaper);
#అన్తర్భావ "hb-shaper-list.hh"

#నిర్వచనం_రద్దు HB_SHAPER_IMPLEMENT
  }
  లేకపోతే
  {
    స్థిరరాశి HB_వాడని hb_shaper_నమోదు_t *shapers = _hb_shapers_get ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < HB_SHAPERS_లెక్క; i++)
      ఒకవేళ (అబద్ధం)
	;
#నిర్వచించు HB_SHAPER_IMPLEMENT(shaper) \
      లేకపోతే ఒకవేళ (shapers[i].func == _hb_##shaper##_shape) \
	HB_SHAPER_PLAN (shaper);
#అన్తర్భావ "hb-shaper-list.hh"
#నిర్వచనం_రద్దు HB_SHAPER_IMPLEMENT
  }
#నిర్వచనం_రద్దు HB_SHAPER_PLAN

bail:
#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE
  ఇది->ot.fini ();
#అయితే_ముగింపు
  ::hb_free (features);
  తిరిగి అబద్ధం;
}

తార్కిక
hb_shape_plan_కీలకం_t::user_features_సరిపోలిక (స్థిరరాశి hb_shape_plan_కీలకం_t *other)
{
  ఒకవేళ (ఇది->అంకె_user_features != other->అంకె_user_features)
    తిరిగి అబద్ధం;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_user_features; i++)
  {
    ఒకవేళ (ఇది->user_features[i].ట్యాగ్_2   != other->user_features[i].ట్యాగ్_2   ||
	ఇది->user_features[i].value != other->user_features[i].value ||
	(ఇది->user_features[i].ప్రారంభం == HB_FEATURE_GLOBAL_ప్రారంభం &&
	 ఇది->user_features[i].చివర   == HB_FEATURE_GLOBAL_చివర) !=
	(other->user_features[i].ప్రారంభం == HB_FEATURE_GLOBAL_ప్రారంభం &&
	 other->user_features[i].చివర   == HB_FEATURE_GLOBAL_చివర))
      తిరిగి అబద్ధం;
  }
  తిరిగి నిజం;
}

తార్కిక
hb_shape_plan_కీలకం_t::equal (స్థిరరాశి hb_shape_plan_కీలకం_t *other)
{
  తిరిగి hb_segment_properties_equal (&ఇది->props, &other->props) &&
	 ఇది->user_features_సరిపోలిక (other) &&
#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE
	 ఇది->ot.equal (&other->ot) &&
#అయితే_ముగింపు
	 ఇది->shaper_func == other->shaper_func;
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
		      స్థిరరాశి hb_segment_properties_t *props,
		      స్థిరరాశి hb_feature_t            *user_features,
		      గుర్తులేని పూర్ణాంకం                   అంకె_user_features,
		      స్థిరరాశి అక్షరం * స్థిరరాశి            *shaper_list)
{
  తిరిగి hb_shape_plan_create2 (face, props,
				user_features, అంకె_user_features,
				శూన్యసూచిక, 0,
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
		       స్థిరరాశి hb_segment_properties_t *props,
		       స్థిరరాశి hb_feature_t            *user_features,
		       గుర్తులేని పూర్ణాంకం                   అంకె_user_features,
		       స్థిరరాశి పూర్ణాంకం                     *coords,
		       గుర్తులేని పూర్ణాంకం                   అంకె_coords,
		       స్థిరరాశి అక్షరం * స్థిరరాశి            *shaper_list)
{
  DEBUG_సందేశం_FUNC (SHAPE_PLAN, శూన్యసూచిక,
		  "face=%p num_features=%u num_coords=%u shaper_list=%p",
		  face,
		  అంకె_user_features,
		  అంకె_coords,
		  shaper_list);

  ఒకవేళ (unlikely (!HB_DIRECTION_IS_VALID (props->direction)))
    తిరిగి hb_shape_plan_get_ఖాళీలు ();

  hb_shape_plan_t *shape_plan;

  ఒకవేళ (unlikely (!props))
    వెళ్ళు bail;
  ఒకవేళ (!(shape_plan = hb_object_create<hb_shape_plan_t> ()))
    వెళ్ళు bail;

  ఒకవేళ (unlikely (!face))
    face = hb_face_get_ఖాళీలు ();
  hb_face_make_immutable (face);
  shape_plan->face_unsafe = face;

  ఒకవేళ (unlikely (!shape_plan->key.init (నిజం,
				       face,
				       props,
				       user_features,
				       అంకె_user_features,
				       coords,
				       అంకె_coords,
				       shaper_list)))
    వెళ్ళు bail2;
#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE
  ఒకవేళ (unlikely (!shape_plan->ot.init0 (face, &shape_plan->key)))
    వెళ్ళు bail3;
#అయితే_ముగింపు

  తిరిగి shape_plan;

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE
bail3:
#అయితే_ముగింపు
  shape_plan->key.fini ();
bail2:
  hb_free (shape_plan);
bail:
  తిరిగి hb_shape_plan_get_ఖాళీలు ();
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
hb_shape_plan_get_ఖాళీలు ()
{
  తిరిగి const_cast<hb_shape_plan_t *> (&Null (hb_shape_plan_t));
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
  తిరిగి hb_object_reference (shape_plan);
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
శూన్య
hb_shape_plan_destroy (hb_shape_plan_t *shape_plan)
{
  ఒకవేళ (!hb_object_destroy (shape_plan)) తిరిగి;

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
hb_shape_plan_సమితి_user_దత్తాంశం (hb_shape_plan_t    *shape_plan,
			     hb_user_దత్తాంశం_కీలకం_t *key,
			     శూన్య *              దత్తాంశం,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (shape_plan, key, దత్తాంశం, destroy, replace);
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
శూన్య *
hb_shape_plan_get_user_దత్తాంశం (స్థిరరాశి hb_shape_plan_t *shape_plan,
			     hb_user_దత్తాంశం_కీలకం_t    *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (shape_plan, key);
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
స్థిరరాశి అక్షరం *
hb_shape_plan_get_shaper (hb_shape_plan_t *shape_plan)
{
  తిరిగి shape_plan->key.shaper_name;
}


స్థిర తార్కిక
hb_shape_plan_execute_అంతర్గత (hb_shape_plan_t    *shape_plan,
				 hb_font_t          *font,
				 hb_బఫరు_t        *బఫరు,
				 స్థిరరాశి hb_feature_t *features,
				 గుర్తులేని పూర్ణాంకం        అంకె_features)
{
  DEBUG_సందేశం_FUNC (SHAPE_PLAN, shape_plan,
		  "num_features=%u shaper_func=%p, shaper_name=%s",
		  అంకె_features,
		  shape_plan->key.shaper_func,
		  shape_plan->key.shaper_name);

  ఒకవేళ (unlikely (!బఫరు->len))
    తిరిగి నిజం;

  ప్రతిజ్ఞా (!hb_object_is_immutable (బఫరు));

  బఫరు->assert_unicode ();

  ఒకవేళ (unlikely (!hb_object_is_valid (shape_plan)))
    తిరిగి అబద్ధం;

  ప్రతిజ్ఞా (shape_plan->face_unsafe == font->face);
  ప్రతిజ్ఞా (hb_segment_properties_equal (&shape_plan->key.props, &బఫరు->props));

#నిర్వచించు HB_SHAPER_EXECUTE(shaper) \
	HB_STMT_ప్రారంభం { \
	  తిరిగి font->దత్తాంశం.shaper && \
		 _hb_##shaper##_shape (shape_plan, font, బఫరు, features, అంకె_features); \
	} HB_STMT_చివర

  ఒకవేళ (అబద్ధం)
    ;
#నిర్వచించు HB_SHAPER_IMPLEMENT(shaper) \
  లేకపోతే ఒకవేళ (shape_plan->key.shaper_func == _hb_##shaper##_shape) \
    HB_SHAPER_EXECUTE (shaper);
#అన్తర్భావ "hb-shaper-list.hh"
#నిర్వచనం_రద్దు HB_SHAPER_IMPLEMENT

#నిర్వచనం_రద్దు HB_SHAPER_EXECUTE

  తిరిగి అబద్ధం;
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
		       hb_బఫరు_t        *బఫరు,
		       స్థిరరాశి hb_feature_t *features,
		       గుర్తులేని పూర్ణాంకం        అంకె_features)
{
  తార్కిక ret = hb_shape_plan_execute_అంతర్గత (shape_plan, font, బఫరు,
					      features, అంకె_features);

  ఒకవేళ (ret && బఫరు->content_type == HB_బఫరు_CONTENT_TYPE_UNICODE)
    బఫరు->content_type = HB_బఫరు_CONTENT_TYPE_GLYPHS;

  తిరిగి ret;
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
			     స్థిరరాశి hb_segment_properties_t *props,
			     స్థిరరాశి hb_feature_t            *user_features,
			     గుర్తులేని పూర్ణాంకం                   అంకె_user_features,
			     స్థిరరాశి అక్షరం * స్థిరరాశి            *shaper_list)
{
  తిరిగి hb_shape_plan_create_cached2 (face, props,
				       user_features, అంకె_user_features,
				       శూన్యసూచిక, 0,
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
			      స్థిరరాశి hb_segment_properties_t *props,
			      స్థిరరాశి hb_feature_t            *user_features,
			      గుర్తులేని పూర్ణాంకం                   అంకె_user_features,
			      స్థిరరాశి పూర్ణాంకం                     *coords,
			      గుర్తులేని పూర్ణాంకం                   అంకె_coords,
			      స్థిరరాశి అక్షరం * స్థిరరాశి            *shaper_list)
{
  DEBUG_సందేశం_FUNC (SHAPE_PLAN, శూన్యసూచిక,
		  "face=%p num_features=%u shaper_list=%p",
		  face,
		  అంకె_user_features,
		  shaper_list);

retry:
  hb_face_t::plan_కణుపు_t *cached_plan_nodes = face->shape_plans;

  తార్కిక dont_cache = !hb_object_is_valid (face);

  ఒకవేళ (likely (!dont_cache))
  {
    hb_shape_plan_కీలకం_t key;
    ఒకవేళ (!key.init (అబద్ధం,
		   face,
		   props,
		   user_features,
		   అంకె_user_features,
		   coords,
		   అంకె_coords,
		   shaper_list))
      తిరిగి hb_shape_plan_get_ఖాళీలు ();

    కొరకు (hb_face_t::plan_కణుపు_t *node = cached_plan_nodes; node; node = node->next)
      ఒకవేళ (node->shape_plan->key.equal (&key))
      {
	DEBUG_సందేశం_FUNC (SHAPE_PLAN, node->shape_plan, "fulfilled from cache");
	తిరిగి hb_shape_plan_reference (node->shape_plan);
      }
  }

  hb_shape_plan_t *shape_plan = hb_shape_plan_create2 (face, props,
						       user_features, అంకె_user_features,
						       coords, అంకె_coords,
						       shaper_list);

  ఒకవేళ (unlikely (dont_cache))
    తిరిగి shape_plan;

  hb_face_t::plan_కణుపు_t *node = (hb_face_t::plan_కణుపు_t *) hb_calloc (1, పరిమాణం (hb_face_t::plan_కణుపు_t));
  ఒకవేళ (unlikely (!node))
    తిరిగి shape_plan;

  node->shape_plan = shape_plan;
  node->next = cached_plan_nodes;

  ఒకవేళ (unlikely (!face->shape_plans.cmpexch (cached_plan_nodes, node)))
  {
    hb_shape_plan_destroy (shape_plan);
    hb_free (node);
    వెళ్ళు retry;
  }
  DEBUG_సందేశం_FUNC (SHAPE_PLAN, shape_plan, "inserted into cache");

  తిరిగి hb_shape_plan_reference (shape_plan);
}


#అయితే_ముగింపు
