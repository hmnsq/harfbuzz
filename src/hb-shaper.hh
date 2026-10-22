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

#جے_بنا_تعریف HB_SHAPER_HH
#تعریف_کرو HB_SHAPER_HH

#ضمن "hb.hh"
#ضمن "hb-machinery.hh"

تعريفنوع hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_بفر_t        *بفر,
				   ثابت hb_feature_t *features,
				   بنانشانی صحيح        عدد_features);

#تعریف_کرو HB_SHAPER_IMPLEMENT(name) \
	خارجي "C" HB_اندرونی hb_shape_func_t _hb_##name##_shape;
#ضمن "hb-shaper-list.hh"
#تعریف_رد HB_SHAPER_IMPLEMENT

بنية hb_shaper_اندراج_t {
  حرف name[16];
  hb_shape_func_t *func;
};

HB_اندرونی ثابت hb_shaper_اندراج_t *
_hb_shapers_get ();


سانچا <قسم_ناں انکڑے_3, بنانشانی صحيح Wheres_انکڑے, قسم_ناں T>
بنية hb_shaper_lazy_loader_t;

#تعریف_کرو HB_SHAPER_ترتیب(Shaper) \
  HB_PASTE (HB_SHAPER_ترتیب_2, Shaper)
تعداد hb_shaper_ترتیب_t
{
  HB_SHAPER_ترتیب_ترتیب_ZERO,
#تعریف_کرو HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ترتیب (Shaper),
#ضمن "hb-shaper-list.hh"
#تعریف_رد HB_SHAPER_IMPLEMENT
  HB_SHAPERS_گنتی_PLUS_ONE,
  HB_SHAPERS_گنتی = HB_SHAPERS_گنتی_PLUS_ONE - 1,
};

سانچا <تعداد hb_shaper_ترتیب_t ترتیب, قسم_ناں Object> بنية hb_shaper_object_انکڑے_type_t;

#تعریف_کرو HB_SHAPER_انکڑے_SUCCEEDED ((فراغ *) +1)
#تعریف_کرو HB_SHAPER_انکڑے_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#تعریف_کرو HB_SHAPER_انکڑے_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#تعریف_کرو HB_SHAPER_انکڑے_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#تعریف_کرو HB_SHAPER_انکڑے_INSTANTIATE_SHAPERS(shaper, object) \
	\
	بنية HB_SHAPER_انکڑے_TYPE (shaper, object); /* Type forward declaration. */ \
	خارجي "C" HB_اندرونی HB_SHAPER_انکڑے_TYPE (shaper, object) * \
	HB_SHAPER_انکڑے_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	خارجي "C" HB_اندرونی فراغ \
	HB_SHAPER_انکڑے_DESTROY_FUNC (shaper, object) (HB_SHAPER_انکڑے_TYPE (shaper, object) *shaper##_##object); \
	\
	سانچا <> \
	بنية hb_shaper_object_انکڑے_type_t<HB_SHAPER_ترتیب (shaper), hb_##object##_t> \
	{ \
	  تعريفنوع HB_SHAPER_انکڑے_TYPE(shaper, object) value; \
	}; \
	\
	سانچا <بنانشانی صحيح Wheres_انکڑے> \
	بنية hb_shaper_lazy_loader_t<hb_##object##_t, Wheres_انکڑے, HB_SHAPER_انکڑے_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_انکڑے_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   Wheres_انکڑے, \
							   HB_SHAPER_انکڑے_TYPE(shaper, object)>, \
				   hb_##object##_t, Wheres_انکڑے> \
	{ \
	  تعريفنوع HB_SHAPER_انکڑے_TYPE(shaper, object) Type; \
	  ساكن Type* create (hb_##object##_t *انکڑے) \
	  { أرجع HB_SHAPER_انکڑے_CREATE_FUNC (shaper, object) (انکڑے); } \
	  ساكن Type *get_null () { أرجع خالی_سوچک; } \
	  ساكن فراغ destroy (Type *p) { HB_SHAPER_انکڑے_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (سچ, "") /* Require semicolon after. */


سانچا <قسم_ناں Object>
بنية hb_shaper_object_dataset_t
{
  فراغ init0 (Object *ماپا_انکڑے)
  {
    هذا->ماپا_انکڑے = ماپا_انکڑے;
#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#ضمن "hb-shaper-list.hh"
#تعریف_رد HB_SHAPER_IMPLEMENT
  }
  فراغ fini ()
  {
#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#ضمن "hb-shaper-list.hh"
#تعریف_رد HB_SHAPER_IMPLEMENT
  }

  Object *ماپا_انکڑے; /* MUST be JUST before the lazy loaders. */
#تعریف_کرو HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ترتیب(shaper), \
				قسم_ناں hb_shaper_object_انکڑے_type_t<HB_SHAPER_ترتیب(shaper), Object>::value \
			       > shaper;
#ضمن "hb-shaper-list.hh"
#تعریف_رد HB_SHAPER_IMPLEMENT
};

#جے_انت /* HB_SHAPER_HH */
