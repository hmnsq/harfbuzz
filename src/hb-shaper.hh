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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SHAPER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_ਬਫਰ_t        *ਬਫਰ,
				   ਅਚਲ hb_feature_t *features,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_IMPLEMENT(name) \
	ਬਾਹਰੀ "C" HB_ਅੰਦਰੂਨੀ hb_shape_func_t _hb_##name##_shape;
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_IMPLEMENT

ਬਣਤਰ hb_shaper_ਇੰਦਰਾਜ_t {
  ਅੱਖਰ name[16];
  hb_shape_func_t *func;
};

HB_ਅੰਦਰੂਨੀ ਅਚਲ hb_shaper_ਇੰਦਰਾਜ_t *
_hb_shapers_get ();


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਅੰਕੜੇ_3, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ Wheres_ਅੰਕੜੇ, ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_shaper_lazy_loader_t;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_ਕ੍ਰਮ(Shaper) \
  HB_PASTE (HB_SHAPER_ਕ੍ਰਮ_2, Shaper)
ਗਿਣਤੀ hb_shaper_ਕ੍ਰਮ_t
{
  HB_SHAPER_ਕ੍ਰਮ_ਕ੍ਰਮ_ZERO,
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ਕ੍ਰਮ (Shaper),
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_IMPLEMENT
  HB_SHAPERS_ਗਿਣਤੀ_PLUS_ONE,
  HB_SHAPERS_ਗਿਣਤੀ = HB_SHAPERS_ਗਿਣਤੀ_PLUS_ONE - 1,
};

ਸਾਂਚਾ <ਗਿਣਤੀ hb_shaper_ਕ੍ਰਮ_t ਕ੍ਰਮ, ਕਿਸਮ_ਨਾਂ Object> ਬਣਤਰ hb_shaper_object_ਅੰਕੜੇ_type_t;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_ਅੰਕੜੇ_SUCCEEDED ((ਖਾਲੀ *) +1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_ਅੰਕੜੇ_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_ਅੰਕੜੇ_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_ਅੰਕੜੇ_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_ਅੰਕੜੇ_INSTANTIATE_SHAPERS(shaper, object) \
	\
	ਬਣਤਰ HB_SHAPER_ਅੰਕੜੇ_TYPE (shaper, object); /* Type forward declaration. */ \
	ਬਾਹਰੀ "C" HB_ਅੰਦਰੂਨੀ HB_SHAPER_ਅੰਕੜੇ_TYPE (shaper, object) * \
	HB_SHAPER_ਅੰਕੜੇ_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	ਬਾਹਰੀ "C" HB_ਅੰਦਰੂਨੀ ਖਾਲੀ \
	HB_SHAPER_ਅੰਕੜੇ_DESTROY_FUNC (shaper, object) (HB_SHAPER_ਅੰਕੜੇ_TYPE (shaper, object) *shaper##_##object); \
	\
	ਸਾਂਚਾ <> \
	ਬਣਤਰ hb_shaper_object_ਅੰਕੜੇ_type_t<HB_SHAPER_ਕ੍ਰਮ (shaper), hb_##object##_t> \
	{ \
	  ਕਿਸਮਪਰਿਭਾਸ਼ਾ HB_SHAPER_ਅੰਕੜੇ_TYPE(shaper, object) value; \
	}; \
	\
	ਸਾਂਚਾ <ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ Wheres_ਅੰਕੜੇ> \
	ਬਣਤਰ hb_shaper_lazy_loader_t<hb_##object##_t, Wheres_ਅੰਕੜੇ, HB_SHAPER_ਅੰਕੜੇ_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_ਅੰਕੜੇ_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   Wheres_ਅੰਕੜੇ, \
							   HB_SHAPER_ਅੰਕੜੇ_TYPE(shaper, object)>, \
				   hb_##object##_t, Wheres_ਅੰਕੜੇ> \
	{ \
	  ਕਿਸਮਪਰਿਭਾਸ਼ਾ HB_SHAPER_ਅੰਕੜੇ_TYPE(shaper, object) Type; \
	  ਸਥਿਰ Type* create (hb_##object##_t *ਅੰਕੜੇ) \
	  { ਵਾਪਸ HB_SHAPER_ਅੰਕੜੇ_CREATE_FUNC (shaper, object) (ਅੰਕੜੇ); } \
	  ਸਥਿਰ Type *get_null () { ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; } \
	  ਸਥਿਰ ਖਾਲੀ destroy (Type *p) { HB_SHAPER_ਅੰਕੜੇ_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (ਸੱਚ, "") /* Require semicolon after. */


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Object>
ਬਣਤਰ hb_shaper_object_dataset_t
{
  ਖਾਲੀ init0 (Object *ਮਾਪਾ_ਅੰਕੜੇ)
  {
    ਇਹ->ਮਾਪਾ_ਅੰਕੜੇ = ਮਾਪਾ_ਅੰਕੜੇ;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_IMPLEMENT
  }
  ਖਾਲੀ fini ()
  {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_IMPLEMENT
  }

  Object *ਮਾਪਾ_ਅੰਕੜੇ; /* MUST be JUST before the lazy loaders. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ਕ੍ਰਮ(shaper), \
				ਕਿਸਮ_ਨਾਂ hb_shaper_object_ਅੰਕੜੇ_type_t<HB_SHAPER_ਕ੍ਰਮ(shaper), Object>::value \
			       > shaper;
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_IMPLEMENT
};

#ਜੇ_ਅੰਤ /* HB_SHAPER_HH */
