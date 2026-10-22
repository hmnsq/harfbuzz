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

#جيڪڏهن_وصف_نه_ٿيل HB_SHAPER_HH
#وصف_ڪريو HB_SHAPER_HH

#ضمن "hb.hh"
#ضمن "hb-machinery.hh"

تعريفنوع hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_بفر_t        *بفر,
				   ثابت hb_feature_t *features,
				   بنانشان صحيح        انگ_features);

#وصف_ڪريو HB_SHAPER_IMPLEMENT(name) \
	خارجي "C" HB_اندروني hb_shape_func_t _hb_##name##_shape;
#ضمن "hb-shaper-list.hh"
#وصف_رد HB_SHAPER_IMPLEMENT

بنية hb_shaper_داخلا_t {
  حرف name[16];
  hb_shape_func_t *func;
};

HB_اندروني ثابت hb_shaper_داخلا_t *
_hb_shapers_get ();


سانچو <قسم_نالو معلومات_3, بنانشان صحيح Wheres_معلومات, قسم_نالو T>
بنية hb_shaper_lazy_loader_t;

#وصف_ڪريو HB_SHAPER_ترتيب(Shaper) \
  HB_PASTE (HB_SHAPER_ترتيب_2, Shaper)
تعداد hb_shaper_ترتيب_t
{
  HB_SHAPER_ترتيب_ترتيب_ZERO,
#وصف_ڪريو HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ترتيب (Shaper),
#ضمن "hb-shaper-list.hh"
#وصف_رد HB_SHAPER_IMPLEMENT
  HB_SHAPERS_ڳڻپ_PLUS_ONE,
  HB_SHAPERS_ڳڻپ = HB_SHAPERS_ڳڻپ_PLUS_ONE - 1,
};

سانچو <تعداد hb_shaper_ترتيب_t ترتيب, قسم_نالو Object> بنية hb_shaper_object_معلومات_type_t;

#وصف_ڪريو HB_SHAPER_معلومات_SUCCEEDED ((فراغ *) +1)
#وصف_ڪريو HB_SHAPER_معلومات_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#وصف_ڪريو HB_SHAPER_معلومات_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#وصف_ڪريو HB_SHAPER_معلومات_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#وصف_ڪريو HB_SHAPER_معلومات_INSTANTIATE_SHAPERS(shaper, object) \
	\
	بنية HB_SHAPER_معلومات_TYPE (shaper, object); /* Type forward declaration. */ \
	خارجي "C" HB_اندروني HB_SHAPER_معلومات_TYPE (shaper, object) * \
	HB_SHAPER_معلومات_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	خارجي "C" HB_اندروني فراغ \
	HB_SHAPER_معلومات_DESTROY_FUNC (shaper, object) (HB_SHAPER_معلومات_TYPE (shaper, object) *shaper##_##object); \
	\
	سانچو <> \
	بنية hb_shaper_object_معلومات_type_t<HB_SHAPER_ترتيب (shaper), hb_##object##_t> \
	{ \
	  تعريفنوع HB_SHAPER_معلومات_TYPE(shaper, object) value; \
	}; \
	\
	سانچو <بنانشان صحيح Wheres_معلومات> \
	بنية hb_shaper_lazy_loader_t<hb_##object##_t, Wheres_معلومات, HB_SHAPER_معلومات_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_معلومات_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   Wheres_معلومات, \
							   HB_SHAPER_معلومات_TYPE(shaper, object)>, \
				   hb_##object##_t, Wheres_معلومات> \
	{ \
	  تعريفنوع HB_SHAPER_معلومات_TYPE(shaper, object) Type; \
	  ساكن Type* create (hb_##object##_t *معلومات) \
	  { أرجع HB_SHAPER_معلومات_CREATE_FUNC (shaper, object) (معلومات); } \
	  ساكن Type *get_null () { أرجع خالي_اشارو; } \
	  ساكن فراغ destroy (Type *p) { HB_SHAPER_معلومات_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (سچ, "") /* Require semicolon after. */


سانچو <قسم_نالو Object>
بنية hb_shaper_object_dataset_t
{
  فراغ init0 (Object *والد_معلومات)
  {
    هذا->والد_معلومات = والد_معلومات;
#وصف_ڪريو HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#ضمن "hb-shaper-list.hh"
#وصف_رد HB_SHAPER_IMPLEMENT
  }
  فراغ fini ()
  {
#وصف_ڪريو HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#ضمن "hb-shaper-list.hh"
#وصف_رد HB_SHAPER_IMPLEMENT
  }

  Object *والد_معلومات; /* MUST be JUST before the lazy loaders. */
#وصف_ڪريو HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ترتيب(shaper), \
				قسم_نالو hb_shaper_object_معلومات_type_t<HB_SHAPER_ترتيب(shaper), Object>::value \
			       > shaper;
#ضمن "hb-shaper-list.hh"
#وصف_رد HB_SHAPER_IMPLEMENT
};

#جيڪڏهن_پورو /* HB_SHAPER_HH */
