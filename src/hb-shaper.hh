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

#اگر_تعریف_نشده HB_SHAPER_HH
#تعریف_کن HB_SHAPER_HH

#ضمن "hb.hh"
#ضمن "hb-machinery.hh"

تعريفنوع hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_میانگیر_t        *میانگیر,
				   ثابت hb_feature_t *features,
				   بیعلامت صحيح        رقم_features);

#تعریف_کن HB_SHAPER_IMPLEMENT(name) \
	خارجي "C" HB_داخلی hb_shape_func_t _hb_##name##_shape;
#ضمن "hb-shaper-list.hh"
#لغو_تعریف HB_SHAPER_IMPLEMENT

بنية hb_shaper_مدخل_t {
  حرف name[16];
  hb_shape_func_t *func;
};

HB_داخلی ثابت hb_shaper_مدخل_t *
_hb_shapers_get ();


قالب <نام_نوع داده_3, بیعلامت صحيح Wheres_داده, نام_نوع T>
بنية hb_shaper_lazy_loader_t;

#تعریف_کن HB_SHAPER_ترتیب(Shaper) \
  HB_PASTE (HB_SHAPER_ترتیب_2, Shaper)
تعداد hb_shaper_ترتیب_t
{
  HB_SHAPER_ترتیب_ترتیب_ZERO,
#تعریف_کن HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ترتیب (Shaper),
#ضمن "hb-shaper-list.hh"
#لغو_تعریف HB_SHAPER_IMPLEMENT
  HB_SHAPERS_شمار_PLUS_ONE,
  HB_SHAPERS_شمار = HB_SHAPERS_شمار_PLUS_ONE - 1,
};

قالب <تعداد hb_shaper_ترتیب_t ترتیب, نام_نوع Object> بنية hb_shaper_object_داده_type_t;

#تعریف_کن HB_SHAPER_داده_SUCCEEDED ((فراغ *) +1)
#تعریف_کن HB_SHAPER_داده_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#تعریف_کن HB_SHAPER_داده_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#تعریف_کن HB_SHAPER_داده_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#تعریف_کن HB_SHAPER_داده_INSTANTIATE_SHAPERS(shaper, object) \
	\
	بنية HB_SHAPER_داده_TYPE (shaper, object); /* Type forward declaration. */ \
	خارجي "C" HB_داخلی HB_SHAPER_داده_TYPE (shaper, object) * \
	HB_SHAPER_داده_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	خارجي "C" HB_داخلی فراغ \
	HB_SHAPER_داده_DESTROY_FUNC (shaper, object) (HB_SHAPER_داده_TYPE (shaper, object) *shaper##_##object); \
	\
	قالب <> \
	بنية hb_shaper_object_داده_type_t<HB_SHAPER_ترتیب (shaper), hb_##object##_t> \
	{ \
	  تعريفنوع HB_SHAPER_داده_TYPE(shaper, object) value; \
	}; \
	\
	قالب <بیعلامت صحيح Wheres_داده> \
	بنية hb_shaper_lazy_loader_t<hb_##object##_t, Wheres_داده, HB_SHAPER_داده_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_داده_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   Wheres_داده, \
							   HB_SHAPER_داده_TYPE(shaper, object)>, \
				   hb_##object##_t, Wheres_داده> \
	{ \
	  تعريفنوع HB_SHAPER_داده_TYPE(shaper, object) Type; \
	  ساكن Type* create (hb_##object##_t *داده) \
	  { أرجع HB_SHAPER_داده_CREATE_FUNC (shaper, object) (داده); } \
	  ساكن Type *get_null () { أرجع اشاره_تهی; } \
	  ساكن فراغ destroy (Type *p) { HB_SHAPER_داده_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (درست, "") /* Require semicolon after. */


قالب <نام_نوع Object>
بنية hb_shaper_object_dataset_t
{
  فراغ init0 (Object *والد_داده)
  {
    هذا->والد_داده = والد_داده;
#تعریف_کن HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#ضمن "hb-shaper-list.hh"
#لغو_تعریف HB_SHAPER_IMPLEMENT
  }
  فراغ fini ()
  {
#تعریف_کن HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#ضمن "hb-shaper-list.hh"
#لغو_تعریف HB_SHAPER_IMPLEMENT
  }

  Object *والد_داده; /* MUST be JUST before the lazy loaders. */
#تعریف_کن HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ترتیب(shaper), \
				نام_نوع hb_shaper_object_داده_type_t<HB_SHAPER_ترتیب(shaper), Object>::value \
			       > shaper;
#ضمن "hb-shaper-list.hh"
#لغو_تعریف HB_SHAPER_IMPLEMENT
};

#پایان_اگر /* HB_SHAPER_HH */
