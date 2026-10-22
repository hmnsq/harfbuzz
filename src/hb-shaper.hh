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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-machinery.hh"

প্রকারনির্দেশ hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_buffer_t        *buffer,
				   ধ্রুৱ hb_feature_t *features,
				   unsigned পূর্ণাঙ্ক        অঙ্ক_features);

#define HB_SHAPER_IMPLEMENT(name) \
	বাহ্য "C" HB_INTERNAL hb_shape_func_t _hb_##name##_shape;
#অন্তর্ভাৱ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

সংরচনা hb_shaper_entry_t {
  ৱর্ণ name[16];
  hb_shape_func_t *func;
};

HB_INTERNAL ধ্রুৱ hb_shaper_entry_t *
_hb_shapers_get ();


template <typename Data, unsigned পূর্ণাঙ্ক WheresData, typename T>
সংরচনা hb_shaper_lazy_loader_t;

#define HB_SHAPER_ORDER(Shaper) \
  HB_PASTE (HB_SHAPER_ORDER_, Shaper)
গণনা hb_shaper_order_t
{
  _HB_SHAPER_ORDER_ORDER_ZERO,
#define HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ORDER (Shaper),
#অন্তর্ভাৱ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  HB_SHAPERS_গুনতি_PLUS_ONE,
  HB_SHAPERS_গুনতি = HB_SHAPERS_গুনতি_PLUS_ONE - 1,
};

template <গণনা hb_shaper_order_t order, typename Object> সংরচনা hb_shaper_object_data_type_t;

#define HB_SHAPER_DATA_SUCCEEDED ((শূন্য *) +1)
#define HB_SHAPER_DATA_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#define HB_SHAPER_DATA_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#define HB_SHAPER_DATA_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#define HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, object) \
	\
	সংরচনা HB_SHAPER_DATA_TYPE (shaper, object); /* Type forward declaration. */ \
	বাহ্য "C" HB_INTERNAL HB_SHAPER_DATA_TYPE (shaper, object) * \
	HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	বাহ্য "C" HB_INTERNAL শূন্য \
	HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (HB_SHAPER_DATA_TYPE (shaper, object) *shaper##_##object); \
	\
	template <> \
	সংরচনা hb_shaper_object_data_type_t<HB_SHAPER_ORDER (shaper), hb_##object##_t> \
	{ \
	  প্রকারনির্দেশ HB_SHAPER_DATA_TYPE(shaper, object) value; \
	}; \
	\
	template <unsigned পূর্ণাঙ্ক WheresData> \
	সংরচনা hb_shaper_lazy_loader_t<hb_##object##_t, WheresData, HB_SHAPER_DATA_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_DATA_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   WheresData, \
							   HB_SHAPER_DATA_TYPE(shaper, object)>, \
				   hb_##object##_t, WheresData> \
	{ \
	  প্রকারনির্দেশ HB_SHAPER_DATA_TYPE(shaper, object) Type; \
	  স্থির Type* create (hb_##object##_t *data) \
	  { ঘূৰাই HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (data); } \
	  স্থির Type *get_null () { ঘূৰাই nullptr; } \
	  স্থির শূন্য destroy (Type *p) { HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (সঁচা, "") /* Require semicolon after. */


template <typename Object>
সংরচনা hb_shaper_object_dataset_t
{
  শূন্য init0 (Object *parent_data)
  {
    এতত্->parent_data = parent_data;
#define HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#অন্তর্ভাৱ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
  শূন্য fini ()
  {
#define HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#অন্তর্ভাৱ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }

  Object *parent_data; /* MUST be JUST before the lazy loaders. */
#define HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ORDER(shaper), \
				typename hb_shaper_object_data_type_t<HB_SHAPER_ORDER(shaper), Object>::value \
			       > shaper;
#অন্তর্ভাৱ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};

#endif /* HB_SHAPER_HH */
