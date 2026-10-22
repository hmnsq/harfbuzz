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

#اگر_غیر_تعریف_شدہ HB_SHAPER_HH
#تعریف_کرو HB_SHAPER_HH

#شامل "hb.hh"
#شامل "hb-machinery.hh"

قسمتعریف hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_بفر_t        *بفر,
				   ثابت hb_feature_t *features,
				   غیرعلامتی صحیح        عدد_features);

#تعریف_کرو HB_SHAPER_IMPLEMENT(name) \
	خارجی "C" HB_اندرونی hb_shape_func_t _hb_##name##_shape;
#شامل "hb-shaper-list.hh"
#تعریف_منسوخ HB_SHAPER_IMPLEMENT

ساخت hb_shaper_اندراج_t {
  حرف name[16];
  hb_shape_func_t *func;
};

HB_اندرونی ثابت hb_shaper_اندراج_t *
_hb_shapers_get ();


سانچہ <قسم_نام ڈیٹا_3, غیرعلامتی صحیح Wheres_ڈیٹا, قسم_نام T>
ساخت hb_shaper_lazy_loader_t;

#تعریف_کرو HB_SHAPER_ترتیب(Shaper) \
  HB_PASTE (HB_SHAPER_ترتیب_2, Shaper)
شمار hb_shaper_ترتیب_t
{
  HB_SHAPER_ترتیب_ترتیب_ZERO,
#تعریف_کرو HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ترتیب (Shaper),
#شامل "hb-shaper-list.hh"
#تعریف_منسوخ HB_SHAPER_IMPLEMENT
  HB_SHAPERS_گنتی_PLUS_ONE,
  HB_SHAPERS_گنتی = HB_SHAPERS_گنتی_PLUS_ONE - 1,
};

سانچہ <شمار hb_shaper_ترتیب_t ترتیب, قسم_نام Object> ساخت hb_shaper_object_ڈیٹا_type_t;

#تعریف_کرو HB_SHAPER_ڈیٹا_SUCCEEDED ((خالی *) +1)
#تعریف_کرو HB_SHAPER_ڈیٹا_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#تعریف_کرو HB_SHAPER_ڈیٹا_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#تعریف_کرو HB_SHAPER_ڈیٹا_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#تعریف_کرو HB_SHAPER_ڈیٹا_INSTANTIATE_SHAPERS(shaper, object) \
	\
	ساخت HB_SHAPER_ڈیٹا_TYPE (shaper, object); /* Type forward declaration. */ \
	خارجی "C" HB_اندرونی HB_SHAPER_ڈیٹا_TYPE (shaper, object) * \
	HB_SHAPER_ڈیٹا_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	خارجی "C" HB_اندرونی خالی \
	HB_SHAPER_ڈیٹا_DESTROY_FUNC (shaper, object) (HB_SHAPER_ڈیٹا_TYPE (shaper, object) *shaper##_##object); \
	\
	سانچہ <> \
	ساخت hb_shaper_object_ڈیٹا_type_t<HB_SHAPER_ترتیب (shaper), hb_##object##_t> \
	{ \
	  قسمتعریف HB_SHAPER_ڈیٹا_TYPE(shaper, object) value; \
	}; \
	\
	سانچہ <غیرعلامتی صحیح Wheres_ڈیٹا> \
	ساخت hb_shaper_lazy_loader_t<hb_##object##_t, Wheres_ڈیٹا, HB_SHAPER_ڈیٹا_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_ڈیٹا_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   Wheres_ڈیٹا, \
							   HB_SHAPER_ڈیٹا_TYPE(shaper, object)>, \
				   hb_##object##_t, Wheres_ڈیٹا> \
	{ \
	  قسمتعریف HB_SHAPER_ڈیٹا_TYPE(shaper, object) Type; \
	  ساکن Type* create (hb_##object##_t *ڈیٹا) \
	  { واپس HB_SHAPER_ڈیٹا_CREATE_FUNC (shaper, object) (ڈیٹا); } \
	  ساکن Type *get_null () { واپس خالی_اشاریہ; } \
	  ساکن خالی destroy (Type *p) { HB_SHAPER_ڈیٹا_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (سچ, "") /* Require semicolon after. */


سانچہ <قسم_نام Object>
ساخت hb_shaper_object_dataset_t
{
  خالی init0 (Object *والد_ڈیٹا)
  {
    یہ->والد_ڈیٹا = والد_ڈیٹا;
#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#شامل "hb-shaper-list.hh"
#تعریف_منسوخ HB_SHAPER_IMPLEMENT
  }
  خالی fini ()
  {
#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#شامل "hb-shaper-list.hh"
#تعریف_منسوخ HB_SHAPER_IMPLEMENT
  }

  Object *والد_ڈیٹا; /* MUST be JUST before the lazy loaders. */
#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ترتیب(shaper), \
				قسم_نام hb_shaper_object_ڈیٹا_type_t<HB_SHAPER_ترتیب(shaper), Object>::value \
			       > shaper;
#شامل "hb-shaper-list.hh"
#تعریف_منسوخ HB_SHAPER_IMPLEMENT
};

#اگر_ختم /* HB_SHAPER_HH */
