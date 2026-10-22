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

#ifndef HB_SHAPER_HH
#define HB_SHAPER_HH

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"

ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_buffer_t        *buffer,
				   ଅଚଳ hb_feature_t *features,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features);

#define HB_SHAPER_IMPLEMENT(name) \
	ବାହ୍ୟ "C" HB_INTERNAL hb_shape_func_t _hb_##name##_shape;
#ଅନ୍ତର୍ଭାଵ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

ସଂରଚନା hb_shaper_entry_t {
  ଅକ୍ଷର name[16];
  hb_shape_func_t *func;
};

HB_INTERNAL ଅଚଳ hb_shaper_entry_t *
_hb_shapers_get ();


template <typename Data, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା WheresData, typename T>
ସଂରଚନା hb_shaper_lazy_loader_t;

#define HB_SHAPER_ORDER(Shaper) \
  HB_PASTE (HB_SHAPER_ORDER_, Shaper)
ଗଣନା hb_shaper_order_t
{
  _HB_SHAPER_ORDER_ORDER_ZERO,
#define HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ORDER (Shaper),
#ଅନ୍ତର୍ଭାଵ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  HB_SHAPERS_ଗଣନା_PLUS_ONE,
  HB_SHAPERS_ଗଣନା = HB_SHAPERS_ଗଣନା_PLUS_ONE - 1,
};

template <ଗଣନା hb_shaper_order_t order, typename Object> ସଂରଚନା hb_shaper_object_data_type_t;

#define HB_SHAPER_DATA_SUCCEEDED ((ଶୂନ୍ୟ *) +1)
#define HB_SHAPER_DATA_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#define HB_SHAPER_DATA_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#define HB_SHAPER_DATA_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#define HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, object) \
	\
	ସଂରଚନା HB_SHAPER_DATA_TYPE (shaper, object); /* Type forward declaration. */ \
	ବାହ୍ୟ "C" HB_INTERNAL HB_SHAPER_DATA_TYPE (shaper, object) * \
	HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	ବାହ୍ୟ "C" HB_INTERNAL ଶୂନ୍ୟ \
	HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (HB_SHAPER_DATA_TYPE (shaper, object) *shaper##_##object); \
	\
	template <> \
	ସଂରଚନା hb_shaper_object_data_type_t<HB_SHAPER_ORDER (shaper), hb_##object##_t> \
	{ \
	  ପ୍ରକାରବ୍ୟାଖ୍ୟା HB_SHAPER_DATA_TYPE(shaper, object) value; \
	}; \
	\
	template <unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା WheresData> \
	ସଂରଚନା hb_shaper_lazy_loader_t<hb_##object##_t, WheresData, HB_SHAPER_DATA_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_DATA_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   WheresData, \
							   HB_SHAPER_DATA_TYPE(shaper, object)>, \
				   hb_##object##_t, WheresData> \
	{ \
	  ପ୍ରକାରବ୍ୟାଖ୍ୟା HB_SHAPER_DATA_TYPE(shaper, object) Type; \
	  ସ୍ଥିର Type* create (hb_##object##_t *data) \
	  { ଫେରନ୍ତୁ HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (data); } \
	  ସ୍ଥିର Type *get_null () { ଫେରନ୍ତୁ nullptr; } \
	  ସ୍ଥିର ଶୂନ୍ୟ destroy (Type *p) { HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (ସତ୍ୟ, "") /* Require semicolon after. */


template <typename Object>
ସଂରଚନା hb_shaper_object_dataset_t
{
  ଶୂନ୍ୟ init0 (Object *parent_data)
  {
    ଏହି->parent_data = parent_data;
#define HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#ଅନ୍ତର୍ଭାଵ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
  ଶୂନ୍ୟ fini ()
  {
#define HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#ଅନ୍ତର୍ଭାଵ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }

  Object *parent_data; /* MUST be JUST before the lazy loaders. */
#define HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ORDER(shaper), \
				typename hb_shaper_object_data_type_t<HB_SHAPER_ORDER(shaper), Object>::value \
			       > shaper;
#ଅନ୍ତର୍ଭାଵ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};

#endif /* HB_SHAPER_HH */
