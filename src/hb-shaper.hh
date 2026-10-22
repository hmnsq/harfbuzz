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

#నిర్వచితం_కాకపోతే HB_SHAPER_HH
#నిర్వచించు HB_SHAPER_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-machinery.hh"

రకనిర్వచనం hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_బఫరు_t        *బఫరు,
				   స్థిరరాశి hb_feature_t *features,
				   గుర్తులేని పూర్ణాంకం        అంకె_features);

#నిర్వచించు HB_SHAPER_IMPLEMENT(name) \
	బాహ్య "C" HB_అంతర్గత hb_shape_func_t _hb_##name##_shape;
#అన్తర్భావ "hb-shaper-list.hh"
#నిర్వచనం_రద్దు HB_SHAPER_IMPLEMENT

నిర్మాణం hb_shaper_నమోదు_t {
  అక్షరం name[16];
  hb_shape_func_t *func;
};

HB_అంతర్గత స్థిరరాశి hb_shaper_నమోదు_t *
_hb_shapers_get ();


మూస <రకం_పేరు దత్తాంశం_3, గుర్తులేని పూర్ణాంకం Wheres_దత్తాంశం, రకం_పేరు T>
నిర్మాణం hb_shaper_lazy_loader_t;

#నిర్వచించు HB_SHAPER_క్రమం(Shaper) \
  HB_PASTE (HB_SHAPER_క్రమం_2, Shaper)
గణన hb_shaper_క్రమం_t
{
  HB_SHAPER_క్రమం_క్రమం_ZERO,
#నిర్వచించు HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_క్రమం (Shaper),
#అన్తర్భావ "hb-shaper-list.hh"
#నిర్వచనం_రద్దు HB_SHAPER_IMPLEMENT
  HB_SHAPERS_లెక్క_PLUS_ONE,
  HB_SHAPERS_లెక్క = HB_SHAPERS_లెక్క_PLUS_ONE - 1,
};

మూస <గణన hb_shaper_క్రమం_t క్రమం, రకం_పేరు Object> నిర్మాణం hb_shaper_object_దత్తాంశం_type_t;

#నిర్వచించు HB_SHAPER_దత్తాంశం_SUCCEEDED ((శూన్య *) +1)
#నిర్వచించు HB_SHAPER_దత్తాంశం_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#నిర్వచించు HB_SHAPER_దత్తాంశం_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#నిర్వచించు HB_SHAPER_దత్తాంశం_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#నిర్వచించు HB_SHAPER_దత్తాంశం_INSTANTIATE_SHAPERS(shaper, object) \
	\
	నిర్మాణం HB_SHAPER_దత్తాంశం_TYPE (shaper, object); /* Type forward declaration. */ \
	బాహ్య "C" HB_అంతర్గత HB_SHAPER_దత్తాంశం_TYPE (shaper, object) * \
	HB_SHAPER_దత్తాంశం_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	బాహ్య "C" HB_అంతర్గత శూన్య \
	HB_SHAPER_దత్తాంశం_DESTROY_FUNC (shaper, object) (HB_SHAPER_దత్తాంశం_TYPE (shaper, object) *shaper##_##object); \
	\
	మూస <> \
	నిర్మాణం hb_shaper_object_దత్తాంశం_type_t<HB_SHAPER_క్రమం (shaper), hb_##object##_t> \
	{ \
	  రకనిర్వచనం HB_SHAPER_దత్తాంశం_TYPE(shaper, object) value; \
	}; \
	\
	మూస <గుర్తులేని పూర్ణాంకం Wheres_దత్తాంశం> \
	నిర్మాణం hb_shaper_lazy_loader_t<hb_##object##_t, Wheres_దత్తాంశం, HB_SHAPER_దత్తాంశం_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_దత్తాంశం_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   Wheres_దత్తాంశం, \
							   HB_SHAPER_దత్తాంశం_TYPE(shaper, object)>, \
				   hb_##object##_t, Wheres_దత్తాంశం> \
	{ \
	  రకనిర్వచనం HB_SHAPER_దత్తాంశం_TYPE(shaper, object) Type; \
	  స్థిర Type* create (hb_##object##_t *దత్తాంశం) \
	  { తిరిగి HB_SHAPER_దత్తాంశం_CREATE_FUNC (shaper, object) (దత్తాంశం); } \
	  స్థిర Type *get_null () { తిరిగి శూన్యసూచిక; } \
	  స్థిర శూన్య destroy (Type *p) { HB_SHAPER_దత్తాంశం_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (నిజం, "") /* Require semicolon after. */


మూస <రకం_పేరు Object>
నిర్మాణం hb_shaper_object_dataset_t
{
  శూన్య init0 (Object *మాతృ_దత్తాంశం)
  {
    ఇది->మాతృ_దత్తాంశం = మాతృ_దత్తాంశం;
#నిర్వచించు HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#అన్తర్భావ "hb-shaper-list.hh"
#నిర్వచనం_రద్దు HB_SHAPER_IMPLEMENT
  }
  శూన్య fini ()
  {
#నిర్వచించు HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#అన్తర్భావ "hb-shaper-list.hh"
#నిర్వచనం_రద్దు HB_SHAPER_IMPLEMENT
  }

  Object *మాతృ_దత్తాంశం; /* MUST be JUST before the lazy loaders. */
#నిర్వచించు HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_క్రమం(shaper), \
				రకం_పేరు hb_shaper_object_దత్తాంశం_type_t<HB_SHAPER_క్రమం(shaper), Object>::value \
			       > shaper;
#అన్తర్భావ "hb-shaper-list.hh"
#నిర్వచనం_రద్దు HB_SHAPER_IMPLEMENT
};

#అయితే_ముగింపు /* HB_SHAPER_HH */
