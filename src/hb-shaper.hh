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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"

ಪ್ರಕಾರನಿರ್ದೇಶ hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_buffer_t        *buffer,
				   ಧ್ರುವ hb_feature_t *features,
				   unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features);

#define HB_SHAPER_IMPLEMENT(name) \
	ಬಾಹ್ಯ "C" HB_INTERNAL hb_shape_func_t _hb_##name##_shape;
#ಅನ್ತರ್ಭಾವ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

ಸಂರಚನಾ hb_shaper_entry_t {
  ವರ್ಣ name[16];
  hb_shape_func_t *func;
};

HB_INTERNAL ಧ್ರುವ hb_shaper_entry_t *
_hb_shapers_get ();


template <typename Data, unsigned ಪೂರ್ಣಾಙ್ಕ WheresData, typename T>
ಸಂರಚನಾ hb_shaper_lazy_loader_t;

#define HB_SHAPER_ORDER(Shaper) \
  HB_PASTE (HB_SHAPER_ORDER_, Shaper)
ಗಣನಾ hb_shaper_order_t
{
  _HB_SHAPER_ORDER_ORDER_ZERO,
#define HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ORDER (Shaper),
#ಅನ್ತರ್ಭಾವ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  HB_SHAPERS_ಎಣಿಕೆ_PLUS_ONE,
  HB_SHAPERS_ಎಣಿಕೆ = HB_SHAPERS_ಎಣಿಕೆ_PLUS_ONE - 1,
};

template <ಗಣನಾ hb_shaper_order_t order, typename Object> ಸಂರಚನಾ hb_shaper_object_data_type_t;

#define HB_SHAPER_DATA_SUCCEEDED ((ಶೂನ್ಯ *) +1)
#define HB_SHAPER_DATA_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#define HB_SHAPER_DATA_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#define HB_SHAPER_DATA_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#define HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, object) \
	\
	ಸಂರಚನಾ HB_SHAPER_DATA_TYPE (shaper, object); /* Type forward declaration. */ \
	ಬಾಹ್ಯ "C" HB_INTERNAL HB_SHAPER_DATA_TYPE (shaper, object) * \
	HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	ಬಾಹ್ಯ "C" HB_INTERNAL ಶೂನ್ಯ \
	HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (HB_SHAPER_DATA_TYPE (shaper, object) *shaper##_##object); \
	\
	template <> \
	ಸಂರಚನಾ hb_shaper_object_data_type_t<HB_SHAPER_ORDER (shaper), hb_##object##_t> \
	{ \
	  ಪ್ರಕಾರನಿರ್ದೇಶ HB_SHAPER_DATA_TYPE(shaper, object) value; \
	}; \
	\
	template <unsigned ಪೂರ್ಣಾಙ್ಕ WheresData> \
	ಸಂರಚನಾ hb_shaper_lazy_loader_t<hb_##object##_t, WheresData, HB_SHAPER_DATA_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_DATA_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   WheresData, \
							   HB_SHAPER_DATA_TYPE(shaper, object)>, \
				   hb_##object##_t, WheresData> \
	{ \
	  ಪ್ರಕಾರನಿರ್ದೇಶ HB_SHAPER_DATA_TYPE(shaper, object) Type; \
	  ಸ್ಥಿರ Type* create (hb_##object##_t *data) \
	  { ಪ್ರತ್ಯಾಗಮ HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (data); } \
	  ಸ್ಥಿರ Type *get_null () { ಪ್ರತ್ಯಾಗಮ nullptr; } \
	  ಸ್ಥಿರ ಶೂನ್ಯ destroy (Type *p) { HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (ಸತ್ಯ, "") /* Require semicolon after. */


template <typename Object>
ಸಂರಚನಾ hb_shaper_object_dataset_t
{
  ಶೂನ್ಯ init0 (Object *parent_data)
  {
    ಏತತ್->parent_data = parent_data;
#define HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#ಅನ್ತರ್ಭಾವ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
  ಶೂನ್ಯ fini ()
  {
#define HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#ಅನ್ತರ್ಭಾವ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }

  Object *parent_data; /* MUST be JUST before the lazy loaders. */
#define HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ORDER(shaper), \
				typename hb_shaper_object_data_type_t<HB_SHAPER_ORDER(shaper), Object>::value \
			       > shaper;
#ಅನ್ತರ್ಭಾವ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};

#endif /* HB_SHAPER_HH */
