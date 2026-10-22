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

#که_نه_تعریف_شوی HB_SHAPER_HH
#تعریف_کړه HB_SHAPER_HH

#ضمن "hb.hh"
#ضمن "hb-machinery.hh"

تعريفنوع hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_بفر_t        *بفر,
				   ثابت hb_feature_t *features,
				   بینښه صحيح        عدد_features);

#تعریف_کړه HB_SHAPER_IMPLEMENT(name) \
	خارجي "C" HB_دننی hb_shape_func_t _hb_##name##_shape;
#ضمن "hb-shaper-list.hh"
#تعریف_لغوه HB_SHAPER_IMPLEMENT

بنية hb_shaper_ثبت_t {
  حرف name[16];
  hb_shape_func_t *func;
};

HB_دننی ثابت hb_shaper_ثبت_t *
_hb_shapers_get ();


کينډۍ <ډول_نوم معلومات_3, بینښه صحيح Wheres_معلومات, ډول_نوم T>
بنية hb_shaper_lazy_loader_t;

#تعریف_کړه HB_SHAPER_ترتیب(Shaper) \
  HB_PASTE (HB_SHAPER_ترتیب_2, Shaper)
تعداد hb_shaper_ترتیب_t
{
  HB_SHAPER_ترتیب_ترتیب_ZERO,
#تعریف_کړه HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ترتیب (Shaper),
#ضمن "hb-shaper-list.hh"
#تعریف_لغوه HB_SHAPER_IMPLEMENT
  HB_SHAPERS_شمېر_PLUS_ONE,
  HB_SHAPERS_شمېر = HB_SHAPERS_شمېر_PLUS_ONE - 1,
};

کينډۍ <تعداد hb_shaper_ترتیب_t ترتیب, ډول_نوم Object> بنية hb_shaper_object_معلومات_type_t;

#تعریف_کړه HB_SHAPER_معلومات_SUCCEEDED ((فراغ *) +1)
#تعریف_کړه HB_SHAPER_معلومات_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#تعریف_کړه HB_SHAPER_معلومات_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#تعریف_کړه HB_SHAPER_معلومات_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#تعریف_کړه HB_SHAPER_معلومات_INSTANTIATE_SHAPERS(shaper, object) \
	\
	بنية HB_SHAPER_معلومات_TYPE (shaper, object); /* Type forward declaration. */ \
	خارجي "C" HB_دننی HB_SHAPER_معلومات_TYPE (shaper, object) * \
	HB_SHAPER_معلومات_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	خارجي "C" HB_دننی فراغ \
	HB_SHAPER_معلومات_DESTROY_FUNC (shaper, object) (HB_SHAPER_معلومات_TYPE (shaper, object) *shaper##_##object); \
	\
	کينډۍ <> \
	بنية hb_shaper_object_معلومات_type_t<HB_SHAPER_ترتیب (shaper), hb_##object##_t> \
	{ \
	  تعريفنوع HB_SHAPER_معلومات_TYPE(shaper, object) value; \
	}; \
	\
	کينډۍ <بینښه صحيح Wheres_معلومات> \
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
	  ساكن Type *get_null () { أرجع تش_اشاره; } \
	  ساكن فراغ destroy (Type *p) { HB_SHAPER_معلومات_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (ریښتیا, "") /* Require semicolon after. */


کينډۍ <ډول_نوم Object>
بنية hb_shaper_object_dataset_t
{
  فراغ init0 (Object *پلار_معلومات)
  {
    هذا->پلار_معلومات = پلار_معلومات;
#تعریف_کړه HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#ضمن "hb-shaper-list.hh"
#تعریف_لغوه HB_SHAPER_IMPLEMENT
  }
  فراغ fini ()
  {
#تعریف_کړه HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#ضمن "hb-shaper-list.hh"
#تعریف_لغوه HB_SHAPER_IMPLEMENT
  }

  Object *پلار_معلومات; /* MUST be JUST before the lazy loaders. */
#تعریف_کړه HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ترتیب(shaper), \
				ډول_نوم hb_shaper_object_معلومات_type_t<HB_SHAPER_ترتیب(shaper), Object>::value \
			       > shaper;
#ضمن "hb-shaper-list.hh"
#تعریف_لغوه HB_SHAPER_IMPLEMENT
};

#د_که_پای /* HB_SHAPER_HH */
