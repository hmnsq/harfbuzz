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

#अगर_अघोषित HB_SHAPER_HH
#घोषणा HB_SHAPER_HH

#समावेश "hb.hh"
#समावेश "hb-machinery.hh"

प्रकार hb_bool_t hb_shape_func_t (hb_shape_plan_t    *shape_plan,
				   hb_font_t          *font,
				   hb_बफर_t        *बफर,
				   स्थिर hb_feature_t *features,
				   अचिन्हित पूर्णांक        अंक_features);

#घोषणा HB_SHAPER_IMPLEMENT(name) \
	बाह्य "C" HB_INTERNAL hb_shape_func_t _hb_##name##_shape;
#समावेश "hb-shaper-list.hh"
#अघोषित HB_SHAPER_IMPLEMENT

काष्ठा hb_shaper_entry_t {
  अक्षर name[16];
  hb_shape_func_t *func;
};

HB_INTERNAL स्थिर hb_shaper_entry_t *
_hb_shapers_get ();


ढाँचा <typename तथ्य_3, अचिन्हित पूर्णांक Wheres_तथ्य, typename T>
काष्ठा hb_shaper_lazy_loader_t;

#घोषणा HB_SHAPER_ORDER(Shaper) \
  HB_PASTE (HB_SHAPER_ORDER_, Shaper)
क्रमागत hb_shaper_order_t
{
  _HB_SHAPER_ORDER_ORDER_ZERO,
#घोषणा HB_SHAPER_IMPLEMENT(Shaper) \
      HB_SHAPER_ORDER (Shaper),
#समावेश "hb-shaper-list.hh"
#अघोषित HB_SHAPER_IMPLEMENT
  HB_SHAPERS_गिनती_PLUS_ONE,
  HB_SHAPERS_गिनती = HB_SHAPERS_गिनती_PLUS_ONE - 1,
};

ढाँचा <क्रमागत hb_shaper_order_t order, typename Object> काष्ठा hb_shaper_object_तथ्य_type_t;

#घोषणा HB_SHAPER_तथ्य_SUCCEEDED ((व्योम *) +1)
#घोषणा HB_SHAPER_तथ्य_TYPE(shaper, object)		hb_##shaper##_##object##_data_t
#घोषणा HB_SHAPER_तथ्य_CREATE_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_create
#घोषणा HB_SHAPER_तथ्य_DESTROY_FUNC(shaper, object)	_hb_##shaper##_shaper_##object##_data_destroy

#घोषणा HB_SHAPER_तथ्य_INSTANTIATE_SHAPERS(shaper, object) \
	\
	काष्ठा HB_SHAPER_तथ्य_TYPE (shaper, object); /* Type forward declaration. */ \
	बाह्य "C" HB_INTERNAL HB_SHAPER_तथ्य_TYPE (shaper, object) * \
	HB_SHAPER_तथ्य_CREATE_FUNC (shaper, object) (hb_##object##_t *object); \
	बाह्य "C" HB_INTERNAL व्योम \
	HB_SHAPER_तथ्य_DESTROY_FUNC (shaper, object) (HB_SHAPER_तथ्य_TYPE (shaper, object) *shaper##_##object); \
	\
	ढाँचा <> \
	काष्ठा hb_shaper_object_तथ्य_type_t<HB_SHAPER_ORDER (shaper), hb_##object##_t> \
	{ \
	  प्रकार HB_SHAPER_तथ्य_TYPE(shaper, object) value; \
	}; \
	\
	ढाँचा <अचिन्हित पूर्णांक Wheres_तथ्य> \
	काष्ठा hb_shaper_lazy_loader_t<hb_##object##_t, Wheres_तथ्य, HB_SHAPER_तथ्य_TYPE(shaper, object)> \
		: hb_lazy_loader_t<HB_SHAPER_तथ्य_TYPE(shaper, object), \
				   hb_shaper_lazy_loader_t<hb_##object##_t, \
							   Wheres_तथ्य, \
							   HB_SHAPER_तथ्य_TYPE(shaper, object)>, \
				   hb_##object##_t, Wheres_तथ्य> \
	{ \
	  प्रकार HB_SHAPER_तथ्य_TYPE(shaper, object) Type; \
	  अटल Type* create (hb_##object##_t *तथ्य) \
	  { वापस HB_SHAPER_तथ्य_CREATE_FUNC (shaper, object) (तथ्य); } \
	  अटल Type *get_null () { वापस nullptr; } \
	  अटल व्योम destroy (Type *p) { HB_SHAPER_तथ्य_DESTROY_FUNC (shaper, object) (p); } \
	}; \
	\
	static_assert (true, "") /* Require semicolon after. */


ढाँचा <typename Object>
काष्ठा hb_shaper_object_dataset_t
{
  व्योम init0 (Object *parent_तथ्य)
  {
    यह->parent_तथ्य = parent_तथ्य;
#घोषणा HB_SHAPER_IMPLEMENT(shaper) shaper.init0 ();
#समावेश "hb-shaper-list.hh"
#अघोषित HB_SHAPER_IMPLEMENT
  }
  व्योम fini ()
  {
#घोषणा HB_SHAPER_IMPLEMENT(shaper) shaper.fini ();
#समावेश "hb-shaper-list.hh"
#अघोषित HB_SHAPER_IMPLEMENT
  }

  Object *parent_तथ्य; /* MUST be JUST before the lazy loaders. */
#घोषणा HB_SHAPER_IMPLEMENT(shaper) \
	hb_shaper_lazy_loader_t<Object, HB_SHAPER_ORDER(shaper), \
				typename hb_shaper_object_तथ्य_type_t<HB_SHAPER_ORDER(shaper), Object>::value \
			       > shaper;
#समावेश "hb-shaper-list.hh"
#अघोषित HB_SHAPER_IMPLEMENT
};

#पूर्ण_अगर /* HB_SHAPER_HH */
