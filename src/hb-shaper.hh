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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-machinery.hh"

प्रकारनिर्देश hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_buffer_t        *buffer,
				   अचर hb_feature_t *features,
				   अचिन्हित पूर्णाङ्क        अङ्क_features);

#define HB_SHAPER_IMPLEMENT(name) \
	बाह्य "C" HB_INTERNAL hb_shape_func_t _hb_##name##_shape;
#अन्तर्भाव "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

संरचना hb_shaper_entry_t {
  वर्ण name[16];
  hb_shape_func_t *func;
};

HB_INTERNAL अचर hb_shaper_entry_t *
_hb_shapers_get ();


ढाँचा <typename Data, अचिन्हित पूर्णाङ्क WheresData, typename T>
संरचना hb_shaper_lazy_loader_t;

#define HB_SHAPER_ORDER(Shaper) \
  HB_PASTE (HB_SHAPER_ORDER_, Shaper)
गणना hb_shaper_order_t
{
  _HB_SHAPER_ORDER_ORDER_ZERO,
#define HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ORDER (Shaper),
#अन्तर्भाव "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  HB_SHAPERS_गन्ती_PLUS_ONE,
  HB_SHAPERS_गन्ती = HB_SHAPERS_गन्ती_PLUS_ONE - 1,
};

ढाँचा <गणना hb_shaper_order_t order, typename Object> संरचना hb_shaper_object_data_type_t;

#define HB_SHAPER_DATA_SUCCEEDED ((शून्य *) +1)
#define HB_SHAPER_DATA_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#define HB_SHAPER_DATA_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#define HB_SHAPER_DATA_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#define HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, object) \
	\
	संरचना HB_SHAPER_DATA_TYPE (shaper, object); /* Type forward declaration. */ \
	बाह्य "C" HB_INTERNAL HB_SHAPER_DATA_TYPE (shaper, object) * \
	HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	बाह्य "C" HB_INTERNAL शून्य \
	HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (HB_SHAPER_DATA_TYPE (shaper, object) *shaper##_##object); \
	\
	ढाँचा <> \
	संरचना hb_shaper_object_data_type_t<HB_SHAPER_ORDER (shaper), hb_##object##_t> \
	{ \
	  प्रकारनिर्देश HB_SHAPER_DATA_TYPE(shaper, object) value; \
	}; \
	\
	ढाँचा <अचिन्हित पूर्णाङ्क WheresData> \
	संरचना hb_shaper_lazy_loader_t<hb_##object##_t, WheresData, HB_SHAPER_DATA_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_DATA_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   WheresData, \
							   HB_SHAPER_DATA_TYPE(shaper, object)>, \
				   hb_##object##_t, WheresData> \
	{ \
	  प्रकारनिर्देश HB_SHAPER_DATA_TYPE(shaper, object) Type; \
	  स्थिर Type* create (hb_##object##_t *data) \
	  { फर्काऊ HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (data); } \
	  स्थिर Type *get_null () { फर्काऊ nullptr; } \
	  स्थिर शून्य destroy (Type *p) { HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (सत्य, "") /* Require semicolon after. */


ढाँचा <typename Object>
संरचना hb_shaper_object_dataset_t
{
  शून्य init0 (Object *parent_data)
  {
    एतत्->parent_data = parent_data;
#define HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#अन्तर्भाव "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
  शून्य fini ()
  {
#define HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#अन्तर्भाव "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }

  Object *parent_data; /* MUST be JUST before the lazy loaders. */
#define HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ORDER(shaper), \
				typename hb_shaper_object_data_type_t<HB_SHAPER_ORDER(shaper), Object>::value \
			       > shaper;
#अन्तर्भाव "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};

#endif /* HB_SHAPER_HH */
