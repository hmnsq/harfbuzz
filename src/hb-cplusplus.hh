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

#ifndef HB_CPLUSPLUS_HH
#define HB_CPLUSPLUS_HH

#अन्तर्भाव "hb.h"

#ifdef __cplusplus

#अन्तर्भाव <functional>
#अन्तर्भाव <utility>

#जर 0
#जर !(__cplusplus >= 201103L)
#error "HarfBuzz C++ helpers require C++11"
#endif
#endif

नामाकाश hb {


ढाँचा <typename T>
संरचना vtable;

ढाँचा <typename T>
संरचना shared_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit shared_ptr (T *p = nullptr) : p (p) {}
  shared_ptr (स्थिरांक shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  shared_ptr& चालक = (स्थिरांक shared_ptr &o) { जर (p != o.p) { destroy (); p = o.p; reference (); } परत *हे; }
  shared_ptr& चालक = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; परत *हे; }
  ~shared_ptr () { v::destroy (p); p = nullptr; }

  T* get() स्थिरांक { परत p; }

  शून्य swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  मित्र शून्य swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  चालक T * () स्थिरांक { परत p; }
  T& चालक * () स्थिरांक { परत *get (); }
  T* चालक -> () स्थिरांक { परत get (); }
  चालक तार्किक () स्थिरांक { परत p; }
  तार्किक चालक == (स्थिरांक shared_ptr &o) स्थिरांक { परत p == o.p; }
  तार्किक चालक != (स्थिरांक shared_ptr &o) स्थिरांक { परत p != o.p; }

  स्थिर T* get_empty() { परत v::get_empty (); }
  T* reference() { परत v::reference (p); }
  शून्य destroy() { v::destroy (p); }
  शून्य set_user_data (hb_user_data_किल्ली_t *key,
		      शून्य *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::set_user_data (p, key, value, destroy, replace); } 
  शून्य * get_user_data (hb_user_data_किल्ली_t *key) { परत v::get_user_data (p, key); }

  खाजगी:
  T *p;
};

ढाँचा<typename T> संरचना is_shared_ptr : std::false_type {};
ढाँचा<typename T> संरचना is_shared_ptr<shared_ptr<T>> : std::true_type {};

ढाँचा <typename T>
संरचना unique_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit unique_ptr (T *p = nullptr) : p (p) {}
  unique_ptr (स्थिरांक unique_ptr &o) = delete;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  unique_ptr& चालक = (स्थिरांक unique_ptr &o) = delete;
  unique_ptr& चालक = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; परत *हे; }
  ~unique_ptr () { v::destroy (p); p = nullptr; }

  T* get() स्थिरांक { परत p; }
  T* release () { T* v = p; p = nullptr; परत v; }

  शून्य swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  मित्र शून्य swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  चालक T * () स्थिरांक { परत p; }
  T& चालक * () स्थिरांक { परत *get (); }
  T* चालक -> () स्थिरांक { परत get (); }
  चालक तार्किक () { परत p; }

  खाजगी:
  T *p;
};

ढाँचा<typename T> संरचना is_unique_ptr : std::false_type {};
ढाँचा<typename T> संरचना is_unique_ptr<unique_ptr<T>> : std::true_type {};

ढाँचा <typename T,
	  T * (*_get_empty) (शून्य),
	  T * (*_reference) (T *),
	  शून्य (*_destroy) (T *),
	  hb_bool_t (*_set_user_data) (T *,
				       hb_user_data_किल्ली_t *,
				       शून्य *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  शून्य * (*_get_user_data) (स्थिरांक T *,
				    hb_user_data_किल्ली_t *)>
संरचना vtable_t
{
  स्थिर constexpr स्वतः get_empty = _get_empty;
  स्थिर constexpr स्वतः reference = _reference;
  स्थिर constexpr स्वतः destroy = _destroy;
  स्थिर constexpr स्वतः set_user_data = _set_user_data;
  स्थिर constexpr स्वतः get_user_data = _get_user_data;
};

#define HB_DEFINE_VTABLE(name, empty) \
	ढाँचा<> \
	संरचना vtable<hb_##name##_t> \
	     : vtable_t<hb_##name##_t, \
			empty, \
			&hb_##name##_reference, \
			&hb_##name##_destroy, \
			&hb_##name##_set_user_data, \
			&hb_##name##_get_user_data> {}

HB_DEFINE_VTABLE (buffer, &hb_buffer_get_empty);
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


#ifdef HB_SUBSET_H

HB_DEFINE_VTABLE (subset_आदान, nullptr);
HB_DEFINE_VTABLE (subset_plan, nullptr);

#endif


#ifdef HB_RASTER_H

HB_DEFINE_VTABLE (raster_image, nullptr);
HB_DEFINE_VTABLE (raster_draw, nullptr);
HB_DEFINE_VTABLE (raster_paint, nullptr);

#endif


#ifdef HB_VECTOR_H

HB_DEFINE_VTABLE (vector_draw, nullptr);
HB_DEFINE_VTABLE (vector_paint, nullptr);

#endif


#ifdef HB_GPU_H

HB_DEFINE_VTABLE (gpu_draw, nullptr);

#endif

/* HB_DEFINE_VTABLE stays available for subsystem headers' paired tail
 * blocks (see hb-raster.h, hb-vector.h, hb-gpu.h, hb-subset.h). */


} // namespace hb

/* Workaround for GCC < 7, see:
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
 * https://stackoverflow.com/a/25594741 */
नामाकाश std {


ढाँचा<typename T>
संरचना hash<hb::shared_ptr<T>>
{
    std::माप_प्रकार चालक()(स्थिरांक hb::shared_ptr<T>& v) स्थिरांक noexcept
    {
        std::माप_प्रकार h = std::hash<decltype (v.get ())>{}(v.get ());
        परत h;
    }
};

ढाँचा<typename T>
संरचना hash<hb::unique_ptr<T>>
{
    std::माप_प्रकार चालक()(स्थिरांक hb::unique_ptr<T>& v) स्थिरांक noexcept
    {
        std::माप_प्रकार h = std::hash<decltype (v.get ())>{}(v.get ());
        परत h;
    }
};


} // namespace std

#endif /* __cplusplus */

#endif /* HB_CPLUSPLUS_HH */
