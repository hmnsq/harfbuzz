/*
 * Copyright © 2022 Behdad Esfahbod
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
 */

#अगर_अघोषित HB_CPLUSPLUS_HH
#घोषणा HB_CPLUSPLUS_HH

#समावेश "hb.h"

#अगर_घोषित __cplusplus

#समावेश <functional>
#समावेश <utility>

#अगर 0
#अगर !(__cplusplus >= 201103L)
#त्रुटि "HarfBuzz C++ helpers require C++11"
#पूर्ण_अगर
#पूर्ण_अगर

namespace hb {


ढाँचा <typename T>
काष्ठा vtable;

ढाँचा <typename T>
काष्ठा shared_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit shared_ptr (T *p = nullptr) : p (p) {}
  shared_ptr (स्थिर shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  shared_ptr& चालक = (स्थिर shared_ptr &o) { अगर (p != o.p) { destroy (); p = o.p; reference (); } वापस *यह; }
  shared_ptr& चालक = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; वापस *यह; }
  ~shared_ptr () { v::destroy (p); p = nullptr; }

  T* लो() स्थिर { वापस p; }

  व्योम swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  मित्र व्योम swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  चालक T * () स्थिर { वापस p; }
  T& चालक * () स्थिर { वापस *लो (); }
  T* चालक -> () स्थिर { वापस लो (); }
  चालक bool () स्थिर { वापस p; }
  bool चालक == (स्थिर shared_ptr &o) स्थिर { वापस p == o.p; }
  bool चालक != (स्थिर shared_ptr &o) स्थिर { वापस p != o.p; }

  अटल T* get_empty() { वापस v::get_empty (); }
  T* reference() { वापस v::reference (p); }
  व्योम destroy() { v::destroy (p); }
  व्योम set_user_तथ्य (hb_user_तथ्य_कुंजी_t *key,
		      व्योम *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::set_user_तथ्य (p, key, value, destroy, replace); } 
  व्योम * get_user_तथ्य (hb_user_तथ्य_कुंजी_t *key) { वापस v::get_user_तथ्य (p, key); }

  निजी:
  T *p;
};

ढाँचा<typename T> काष्ठा is_shared_ptr : std::false_type {};
ढाँचा<typename T> काष्ठा is_shared_ptr<shared_ptr<T>> : std::true_type {};

ढाँचा <typename T>
काष्ठा unique_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit unique_ptr (T *p = nullptr) : p (p) {}
  unique_ptr (स्थिर unique_ptr &o) = मिटाओ;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  unique_ptr& चालक = (स्थिर unique_ptr &o) = मिटाओ;
  unique_ptr& चालक = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; वापस *यह; }
  ~unique_ptr () { v::destroy (p); p = nullptr; }

  T* लो() स्थिर { वापस p; }
  T* release () { T* v = p; p = nullptr; वापस v; }

  व्योम swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  मित्र व्योम swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  चालक T * () स्थिर { वापस p; }
  T& चालक * () स्थिर { वापस *लो (); }
  T* चालक -> () स्थिर { वापस लो (); }
  चालक bool () { वापस p; }

  निजी:
  T *p;
};

ढाँचा<typename T> काष्ठा is_unique_ptr : std::false_type {};
ढाँचा<typename T> काष्ठा is_unique_ptr<unique_ptr<T>> : std::true_type {};

ढाँचा <typename T,
	  T * (*_get_empty) (व्योम),
	  T * (*_reference) (T *),
	  व्योम (*_destroy) (T *),
	  hb_bool_t (*set_user_तथ्य_2) (T *,
				       hb_user_तथ्य_कुंजी_t *,
				       व्योम *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  व्योम * (*get_user_तथ्य_2) (स्थिर T *,
				    hb_user_तथ्य_कुंजी_t *)>
काष्ठा vtable_t
{
  अटल constexpr स्वतः get_empty = _get_empty;
  अटल constexpr स्वतः reference = _reference;
  अटल constexpr स्वतः destroy = _destroy;
  अटल constexpr स्वतः set_user_तथ्य = set_user_तथ्य_2;
  अटल constexpr स्वतः get_user_तथ्य = get_user_तथ्य_2;
};

#घोषणा HB_DEFINE_VTABLE(name, empty) \
	ढाँचा<> \
	काष्ठा vtable<hb_##name##_t> \
	     : vtable_t<hb_##name##_t, \
			empty, \
			&hb_##name##_reference, \
			&hb_##name##_destroy, \
			&hb_##name##_set_user_data, \
			&hb_##name##_get_user_data> {}

HB_DEFINE_VTABLE (बफर, &hb_बफर_get_empty);
HB_DEFINE_VTABLE (blob, &hb_blob_get_empty);
HB_DEFINE_VTABLE (face, &hb_face_get_empty);
HB_DEFINE_VTABLE (font, &hb_font_get_empty);
HB_DEFINE_VTABLE (font_funcs, &hb_font_funcs_get_empty);
HB_DEFINE_VTABLE (map, &hb_map_get_empty);
HB_DEFINE_VTABLE (set, &hb_set_get_empty);
HB_DEFINE_VTABLE (shape_plan, &hb_shape_plan_get_empty);
HB_DEFINE_VTABLE (unicode_funcs, &hb_unicode_funcs_get_empty);
HB_DEFINE_VTABLE (draw_funcs, &hb_draw_funcs_get_empty);
HB_DEFINE_VTABLE (paint_funcs, &hb_paint_funcs_get_empty);


#अगर_घोषित HB_SUBSET_H

HB_DEFINE_VTABLE (subset_निवेश, nullptr);
HB_DEFINE_VTABLE (subset_plan, nullptr);

#पूर्ण_अगर


#अगर_घोषित HB_RASTER_H

HB_DEFINE_VTABLE (raster_image, nullptr);
HB_DEFINE_VTABLE (raster_draw, nullptr);
HB_DEFINE_VTABLE (raster_paint, nullptr);

#पूर्ण_अगर


#अगर_घोषित HB_VECTOR_H

HB_DEFINE_VTABLE (vector_draw, nullptr);
HB_DEFINE_VTABLE (vector_paint, nullptr);

#पूर्ण_अगर


#अगर_घोषित HB_GPU_H

HB_DEFINE_VTABLE (gpu_draw, nullptr);

#पूर्ण_अगर

/* HB_DEFINE_VTABLE stays available for subsystem headers' paired tail
 * blocks (see hb-raster.h, hb-vector.h, hb-gpu.h, hb-subset.h). */


} // namespace hb

/* Workaround for GCC < 7, see:
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
 * https://stackoverflow.com/a/25594741 */
namespace std {


ढाँचा<typename T>
काष्ठा hash<hb::shared_ptr<T>>
{
    std::माप_प्रकार चालक()(स्थिर hb::shared_ptr<T>& v) स्थिर noexcept
    {
        std::माप_प्रकार h = std::hash<decltype (v.लो ())>{}(v.लो ());
        वापस h;
    }
};

ढाँचा<typename T>
काष्ठा hash<hb::unique_ptr<T>>
{
    std::माप_प्रकार चालक()(स्थिर hb::unique_ptr<T>& v) स्थिर noexcept
    {
        std::माप_प्रकार h = std::hash<decltype (v.लो ())>{}(v.लो ());
        वापस h;
    }
};


} // namespace std

#पूर्ण_अगर /* __cplusplus */

#पूर्ण_अगर /* HB_CPLUSPLUS_HH */
