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

#ಅನ್ತರ್ಭಾವ "hb.h"

#ifdef __cplusplus

#ಅನ್ತರ್ಭಾವ <functional>
#ಅನ್ತರ್ಭಾವ <utility>

#ಯದಿ 0
#ಯದಿ !(__cplusplus >= 201103L)
#error "HarfBuzz C++ helpers require C++11"
#endif
#endif

ನಾಮಾಕಾಶ hb {


template <typename T>
ಸಂರಚನಾ vtable;

template <typename T>
ಸಂರಚನಾ shared_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit shared_ptr (T *p = nullptr) : p (p) {}
  shared_ptr (ಧ್ರುವ shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  shared_ptr& operator = (ಧ್ರುವ shared_ptr &o) { ಯದಿ (p != o.p) { destroy (); p = o.p; reference (); } ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  shared_ptr& operator = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ~shared_ptr () { v::destroy (p); p = nullptr; }

  T* get() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ p; }

  ಶೂನ್ಯ swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  friend ಶೂನ್ಯ swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  operator T * () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ p; }
  T& operator * () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ *get (); }
  T* operator -> () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (); }
  operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ p; }
  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ shared_ptr &o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ p == o.p; }
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ shared_ptr &o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ p != o.p; }

  ಸ್ಥಿರ T* get_empty() { ಪ್ರತ್ಯಾಗಮ v::get_empty (); }
  T* reference() { ಪ್ರತ್ಯಾಗಮ v::reference (p); }
  ಶೂನ್ಯ destroy() { v::destroy (p); }
  ಶೂನ್ಯ set_user_data (hb_user_data_ಕೀಲಿ_t *key,
		      ಶೂನ್ಯ *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::set_user_data (p, key, value, destroy, replace); } 
  ಶೂನ್ಯ * get_user_data (hb_user_data_ಕೀಲಿ_t *key) { ಪ್ರತ್ಯಾಗಮ v::get_user_data (p, key); }

  ಗುಪ್ತ:
  T *p;
};

template<typename T> ಸಂರಚನಾ is_shared_ptr : std::false_type {};
template<typename T> ಸಂರಚನಾ is_shared_ptr<shared_ptr<T>> : std::true_type {};

template <typename T>
ಸಂರಚನಾ unique_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit unique_ptr (T *p = nullptr) : p (p) {}
  unique_ptr (ಧ್ರುವ unique_ptr &o) = delete;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  unique_ptr& operator = (ಧ್ರುವ unique_ptr &o) = delete;
  unique_ptr& operator = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ~unique_ptr () { v::destroy (p); p = nullptr; }

  T* get() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ p; }
  T* release () { T* v = p; p = nullptr; ಪ್ರತ್ಯಾಗಮ v; }

  ಶೂನ್ಯ swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  friend ಶೂನ್ಯ swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  operator T * () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ p; }
  T& operator * () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ *get (); }
  T* operator -> () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (); }
  operator ತಾರ್ಕಿಕ () { ಪ್ರತ್ಯಾಗಮ p; }

  ಗುಪ್ತ:
  T *p;
};

template<typename T> ಸಂರಚನಾ is_unique_ptr : std::false_type {};
template<typename T> ಸಂರಚನಾ is_unique_ptr<unique_ptr<T>> : std::true_type {};

template <typename T,
	  T * (*_get_empty) (ಶೂನ್ಯ),
	  T * (*_reference) (T *),
	  ಶೂನ್ಯ (*_destroy) (T *),
	  hb_bool_t (*_set_user_data) (T *,
				       hb_user_data_ಕೀಲಿ_t *,
				       ಶೂನ್ಯ *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  ಶೂನ್ಯ * (*_get_user_data) (ಧ್ರುವ T *,
				    hb_user_data_ಕೀಲಿ_t *)>
ಸಂರಚನಾ vtable_t
{
  ಸ್ಥಿರ constexpr auto get_empty = _get_empty;
  ಸ್ಥಿರ constexpr auto reference = _reference;
  ಸ್ಥಿರ constexpr auto destroy = _destroy;
  ಸ್ಥಿರ constexpr auto set_user_data = _set_user_data;
  ಸ್ಥಿರ constexpr auto get_user_data = _get_user_data;
};

#define HB_DEFINE_VTABLE(name, empty) \
	template<> \
	ಸಂರಚನಾ vtable<hb_##name##_t> \
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

HB_DEFINE_VTABLE (subset_ಒಳಹರಿವು, nullptr);
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
ನಾಮಾಕಾಶ std {


template<typename T>
ಸಂರಚನಾ hash<hb::shared_ptr<T>>
{
    std::size_t operator()(ಧ್ರುವ hb::shared_ptr<T>& v) ಧ್ರುವ noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        ಪ್ರತ್ಯಾಗಮ h;
    }
};

template<typename T>
ಸಂರಚನಾ hash<hb::unique_ptr<T>>
{
    std::size_t operator()(ಧ್ರುವ hb::unique_ptr<T>& v) ಧ್ರುವ noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        ಪ್ರತ್ಯಾಗಮ h;
    }
};


} // namespace std

#endif /* __cplusplus */

#endif /* HB_CPLUSPLUS_HH */
