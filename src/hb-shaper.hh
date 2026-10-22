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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-machinery.hh"

প্রকারলেখ hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_বাফার_t        *বাফার,
				   ধ্রুবক hb_feature_t *features,
				   unsigned পূর্ণসংখ্যা        অঙ্ক_features);

#define HB_SHAPER_IMPLEMENT(name) \
	বাহ্যিক "C" HB_INTERNAL hb_shape_func_t _hb_##name##_shape;
#অন্তর্ভাব "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

কাঠামো hb_shaper_entry_t {
  অক্ষর name[16];
  hb_shape_func_t *func;
};

HB_INTERNAL ধ্রুবক hb_shaper_entry_t *
_hb_shapers_get ();


template <typename তথ্য_3, unsigned পূর্ণসংখ্যা Wheres_তথ্য, typename T>
কাঠামো hb_shaper_lazy_loader_t;

#define HB_SHAPER_ORDER(Shaper) \
  HB_PASTE (HB_SHAPER_ORDER_, Shaper)
গণনা hb_shaper_order_t
{
  _HB_SHAPER_ORDER_ORDER_ZERO,
#define HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ORDER (Shaper),
#অন্তর্ভাব "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  HB_SHAPERS_গুনতি_PLUS_ONE,
  HB_SHAPERS_গুনতি = HB_SHAPERS_গুনতি_PLUS_ONE - 1,
};

template <গণনা hb_shaper_order_t order, typename Object> কাঠামো hb_shaper_object_তথ্য_type_t;

#define HB_SHAPER_তথ্য_SUCCEEDED ((শূন্য *) +1)
#define HB_SHAPER_তথ্য_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#define HB_SHAPER_তথ্য_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#define HB_SHAPER_তথ্য_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#define HB_SHAPER_তথ্য_INSTANTIATE_SHAPERS(shaper, object) \
	\
	কাঠামো HB_SHAPER_তথ্য_TYPE (shaper, object); /* Type forward declaration. */ \
	বাহ্যিক "C" HB_INTERNAL HB_SHAPER_তথ্য_TYPE (shaper, object) * \
	HB_SHAPER_তথ্য_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	বাহ্যিক "C" HB_INTERNAL শূন্য \
	HB_SHAPER_তথ্য_DESTROY_FUNC (shaper, object) (HB_SHAPER_তথ্য_TYPE (shaper, object) *shaper##_##object); \
	\
	template <> \
	কাঠামো hb_shaper_object_তথ্য_type_t<HB_SHAPER_ORDER (shaper), hb_##object##_t> \
	{ \
	  প্রকারলেখ HB_SHAPER_তথ্য_TYPE(shaper, object) value; \
	}; \
	\
	template <unsigned পূর্ণসংখ্যা Wheres_তথ্য> \
	কাঠামো hb_shaper_lazy_loader_t<hb_##object##_t, Wheres_তথ্য, HB_SHAPER_তথ্য_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_তথ্য_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   Wheres_তথ্য, \
							   HB_SHAPER_তথ্য_TYPE(shaper, object)>, \
				   hb_##object##_t, Wheres_তথ্য> \
	{ \
	  প্রকারলেখ HB_SHAPER_তথ্য_TYPE(shaper, object) Type; \
	  স্থির Type* create (hb_##object##_t *তথ্য) \
	  { ফেরত HB_SHAPER_তথ্য_CREATE_FUNC (shaper, object) (তথ্য); } \
	  স্থির Type *get_null () { ফেরত nullptr; } \
	  স্থির শূন্য destroy (Type *p) { HB_SHAPER_তথ্য_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (সত্য, "") /* Require semicolon after. */


template <typename Object>
কাঠামো hb_shaper_object_dataset_t
{
  শূন্য init0 (Object *parent_তথ্য)
  {
    এই->parent_তথ্য = parent_তথ্য;
#define HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#অন্তর্ভাব "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
  শূন্য fini ()
  {
#define HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#অন্তর্ভাব "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }

  Object *parent_তথ্য; /* MUST be JUST before the lazy loaders. */
#define HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ORDER(shaper), \
				typename hb_shaper_object_তথ্য_type_t<HB_SHAPER_ORDER(shaper), Object>::value \
			       > shaper;
#অন্তর্ভাব "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};

#endif /* HB_SHAPER_HH */
