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

#شامل "hb.hh"
#شامل "hb-machinery.hh"

تعریفنوع hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_buffer_t        *buffer,
				   ثابت hb_feature_t *features,
				   unsigned صحیح        رقم_features);

#define HB_SHAPER_IMPLEMENT(name) \
	بیرونی "C" HB_INTERNAL hb_shape_func_t _hb_##name##_shape;
#شامل "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

ساختار hb_shaper_entry_t {
  نویسه name[16];
  hb_shape_func_t *func;
};

HB_INTERNAL ثابت hb_shaper_entry_t *
_hb_shapers_get ();


template <typename Data, unsigned صحیح WheresData, typename T>
ساختار hb_shaper_lazy_loader_t;

#define HB_SHAPER_ORDER(Shaper) \
  HB_PASTE (HB_SHAPER_ORDER_, Shaper)
شمارش hb_shaper_order_t
{
  _HB_SHAPER_ORDER_ORDER_ZERO,
#define HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ORDER (Shaper),
#شامل "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  HB_SHAPERS_شمار_PLUS_ONE,
  HB_SHAPERS_شمار = HB_SHAPERS_شمار_PLUS_ONE - 1,
};

template <شمارش hb_shaper_order_t order, typename Object> ساختار hb_shaper_object_data_type_t;

#define HB_SHAPER_DATA_SUCCEEDED ((تهی *) +1)
#define HB_SHAPER_DATA_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#define HB_SHAPER_DATA_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#define HB_SHAPER_DATA_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#define HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, object) \
	\
	ساختار HB_SHAPER_DATA_TYPE (shaper, object); /* Type forward declaration. */ \
	بیرونی "C" HB_INTERNAL HB_SHAPER_DATA_TYPE (shaper, object) * \
	HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	بیرونی "C" HB_INTERNAL تهی \
	HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (HB_SHAPER_DATA_TYPE (shaper, object) *shaper##_##object); \
	\
	template <> \
	ساختار hb_shaper_object_data_type_t<HB_SHAPER_ORDER (shaper), hb_##object##_t> \
	{ \
	  تعریفنوع HB_SHAPER_DATA_TYPE(shaper, object) value; \
	}; \
	\
	template <unsigned صحیح WheresData> \
	ساختار hb_shaper_lazy_loader_t<hb_##object##_t, WheresData, HB_SHAPER_DATA_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_DATA_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   WheresData, \
							   HB_SHAPER_DATA_TYPE(shaper, object)>, \
				   hb_##object##_t, WheresData> \
	{ \
	  تعریفنوع HB_SHAPER_DATA_TYPE(shaper, object) Type; \
	  ایستا Type* create (hb_##object##_t *data) \
	  { بازگشت HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (data); } \
	  ایستا Type *get_null () { بازگشت nullptr; } \
	  ایستا تهی destroy (Type *p) { HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (درست, "") /* Require semicolon after. */


template <typename Object>
ساختار hb_shaper_object_dataset_t
{
  تهی init0 (Object *parent_data)
  {
    این->parent_data = parent_data;
#define HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#شامل "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
  تهی fini ()
  {
#define HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#شامل "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }

  Object *parent_data; /* MUST be JUST before the lazy loaders. */
#define HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ORDER(shaper), \
				typename hb_shaper_object_data_type_t<HB_SHAPER_ORDER(shaper), Object>::value \
			       > shaper;
#شامل "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};

#endif /* HB_SHAPER_HH */
