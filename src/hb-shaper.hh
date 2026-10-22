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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-machinery.hh"

തരനിർവചനം hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_buffer_t        *buffer,
				   സ്ഥിരം hb_feature_t *features,
				   unsigned പൂർണ്ണസംഖ്യ        അക്കം_features);

#define HB_SHAPER_IMPLEMENT(name) \
	ബാഹ്യ "C" HB_INTERNAL hb_shape_func_t _hb_##name##_shape;
#അന്തര്ഭാവ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT

ഘടന hb_shaper_entry_t {
  അക്ഷരം name[16];
  hb_shape_func_t *func;
};

HB_INTERNAL സ്ഥിരം hb_shaper_entry_t *
_hb_shapers_get ();


template <typename Data, unsigned പൂർണ്ണസംഖ്യ WheresData, typename T>
ഘടന hb_shaper_lazy_loader_t;

#define HB_SHAPER_ORDER(Shaper) \
  HB_PASTE (HB_SHAPER_ORDER_, Shaper)
എണ്ണൽ hb_shaper_order_t
{
  _HB_SHAPER_ORDER_ORDER_ZERO,
#define HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ORDER (Shaper),
#അന്തര്ഭാവ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  HB_SHAPERS_എണ്ണം_PLUS_ONE,
  HB_SHAPERS_എണ്ണം = HB_SHAPERS_എണ്ണം_PLUS_ONE - 1,
};

template <എണ്ണൽ hb_shaper_order_t order, typename Object> ഘടന hb_shaper_object_data_type_t;

#define HB_SHAPER_DATA_SUCCEEDED ((ശൂന്യം *) +1)
#define HB_SHAPER_DATA_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#define HB_SHAPER_DATA_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#define HB_SHAPER_DATA_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#define HB_SHAPER_DATA_INSTANTIATE_SHAPERS(shaper, object) \
	\
	ഘടന HB_SHAPER_DATA_TYPE (shaper, object); /* Type forward declaration. */ \
	ബാഹ്യ "C" HB_INTERNAL HB_SHAPER_DATA_TYPE (shaper, object) * \
	HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	ബാഹ്യ "C" HB_INTERNAL ശൂന്യം \
	HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (HB_SHAPER_DATA_TYPE (shaper, object) *shaper##_##object); \
	\
	template <> \
	ഘടന hb_shaper_object_data_type_t<HB_SHAPER_ORDER (shaper), hb_##object##_t> \
	{ \
	  തരനിർവചനം HB_SHAPER_DATA_TYPE(shaper, object) value; \
	}; \
	\
	template <unsigned പൂർണ്ണസംഖ്യ WheresData> \
	ഘടന hb_shaper_lazy_loader_t<hb_##object##_t, WheresData, HB_SHAPER_DATA_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_DATA_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   WheresData, \
							   HB_SHAPER_DATA_TYPE(shaper, object)>, \
				   hb_##object##_t, WheresData> \
	{ \
	  തരനിർവചനം HB_SHAPER_DATA_TYPE(shaper, object) Type; \
	  സ്ഥിതം Type* create (hb_##object##_t *data) \
	  { തിരികെ HB_SHAPER_DATA_CREATE_FUNC (shaper, object) (data); } \
	  സ്ഥിതം Type *get_null () { തിരികെ nullptr; } \
	  സ്ഥിതം ശൂന്യം destroy (Type *p) { HB_SHAPER_DATA_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (സത്യം, "") /* Require semicolon after. */


template <typename Object>
ഘടന hb_shaper_object_dataset_t
{
  ശൂന്യം init0 (Object *parent_data)
  {
    ഇത്->parent_data = parent_data;
#define HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#അന്തര്ഭാവ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }
  ശൂന്യം fini ()
  {
#define HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#അന്തര്ഭാവ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
  }

  Object *parent_data; /* MUST be JUST before the lazy loaders. */
#define HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ORDER(shaper), \
				typename hb_shaper_object_data_type_t<HB_SHAPER_ORDER(shaper), Object>::value \
			       > shaper;
#അന്തര്ഭാവ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};

#endif /* HB_SHAPER_HH */
