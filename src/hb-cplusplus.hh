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

#ଅନ୍ତର୍ଭାଵ "hb.h"

#ifdef __cplusplus

#ଅନ୍ତର୍ଭାଵ <functional>
#ଅନ୍ତର୍ଭାଵ <utility>

#ଯଦି 0
#ଯଦି !(__cplusplus >= 201103L)
#error "HarfBuzz C++ helpers require C++11"
#endif
#endif

ନାମାକାଶ hb {


template <typename T>
ସଂରଚନା vtable;

template <typename T>
ସଂରଚନା shared_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit shared_ptr (T *p = nullptr) : p (p) {}
  shared_ptr (ଅଚଳ shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  shared_ptr& operator = (ଅଚଳ shared_ptr &o) { ଯଦି (p != o.p) { destroy (); p = o.p; reference (); } ଫେରନ୍ତୁ *ଏହି; }
  shared_ptr& operator = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; ଫେରନ୍ତୁ *ଏହି; }
  ~shared_ptr () { v::destroy (p); p = nullptr; }

  T* get() ଅଚଳ { ଫେରନ୍ତୁ p; }

  ଶୂନ୍ୟ swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  friend ଶୂନ୍ୟ swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  operator T * () ଅଚଳ { ଫେରନ୍ତୁ p; }
  T& operator * () ଅଚଳ { ଫେରନ୍ତୁ *get (); }
  T* operator -> () ଅଚଳ { ଫେରନ୍ତୁ get (); }
  operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ p; }
  ତାର୍କିକ operator == (ଅଚଳ shared_ptr &o) ଅଚଳ { ଫେରନ୍ତୁ p == o.p; }
  ତାର୍କିକ operator != (ଅଚଳ shared_ptr &o) ଅଚଳ { ଫେରନ୍ତୁ p != o.p; }

  ସ୍ଥିର T* get_empty() { ଫେରନ୍ତୁ v::get_empty (); }
  T* reference() { ଫେରନ୍ତୁ v::reference (p); }
  ଶୂନ୍ୟ destroy() { v::destroy (p); }
  ଶୂନ୍ୟ set_user_data (hb_user_data_ଚାବି_t *key,
		      ଶୂନ୍ୟ *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::set_user_data (p, key, value, destroy, replace); } 
  ଶୂନ୍ୟ * get_user_data (hb_user_data_ଚାବି_t *key) { ଫେରନ୍ତୁ v::get_user_data (p, key); }

  ବ୍ୟକ୍ତିଗତ:
  T *p;
};

template<typename T> ସଂରଚନା is_shared_ptr : std::false_type {};
template<typename T> ସଂରଚନା is_shared_ptr<shared_ptr<T>> : std::true_type {};

template <typename T>
ସଂରଚନା unique_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit unique_ptr (T *p = nullptr) : p (p) {}
  unique_ptr (ଅଚଳ unique_ptr &o) = delete;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  unique_ptr& operator = (ଅଚଳ unique_ptr &o) = delete;
  unique_ptr& operator = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; ଫେରନ୍ତୁ *ଏହି; }
  ~unique_ptr () { v::destroy (p); p = nullptr; }

  T* get() ଅଚଳ { ଫେରନ୍ତୁ p; }
  T* release () { T* v = p; p = nullptr; ଫେରନ୍ତୁ v; }

  ଶୂନ୍ୟ swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  friend ଶୂନ୍ୟ swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  operator T * () ଅଚଳ { ଫେରନ୍ତୁ p; }
  T& operator * () ଅଚଳ { ଫେରନ୍ତୁ *get (); }
  T* operator -> () ଅଚଳ { ଫେରନ୍ତୁ get (); }
  operator ତାର୍କିକ () { ଫେରନ୍ତୁ p; }

  ବ୍ୟକ୍ତିଗତ:
  T *p;
};

template<typename T> ସଂରଚନା is_unique_ptr : std::false_type {};
template<typename T> ସଂରଚନା is_unique_ptr<unique_ptr<T>> : std::true_type {};

template <typename T,
	  T * (*_get_empty) (ଶୂନ୍ୟ),
	  T * (*_reference) (T *),
	  ଶୂନ୍ୟ (*_destroy) (T *),
	  hb_bool_t (*_set_user_data) (T *,
				       hb_user_data_ଚାବି_t *,
				       ଶୂନ୍ୟ *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  ଶୂନ୍ୟ * (*_get_user_data) (ଅଚଳ T *,
				    hb_user_data_ଚାବି_t *)>
ସଂରଚନା vtable_t
{
  ସ୍ଥିର constexpr auto get_empty = _get_empty;
  ସ୍ଥିର constexpr auto reference = _reference;
  ସ୍ଥିର constexpr auto destroy = _destroy;
  ସ୍ଥିର constexpr auto set_user_data = _set_user_data;
  ସ୍ଥିର constexpr auto get_user_data = _get_user_data;
};

#define HB_DEFINE_VTABLE(name, empty) \
	template<> \
	ସଂରଚନା vtable<hb_##name##_t> \
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

HB_DEFINE_VTABLE (subset_ନିବେଶ, nullptr);
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
ନାମାକାଶ std {


template<typename T>
ସଂରଚନା hash<hb::shared_ptr<T>>
{
    std::size_t operator()(ଅଚଳ hb::shared_ptr<T>& v) ଅଚଳ noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        ଫେରନ୍ତୁ h;
    }
};

template<typename T>
ସଂରଚନା hash<hb::unique_ptr<T>>
{
    std::size_t operator()(ଅଚଳ hb::unique_ptr<T>& v) ଅଚଳ noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        ଫେରନ୍ତୁ h;
    }
};


} // namespace std

#endif /* __cplusplus */

#endif /* HB_CPLUSPLUS_HH */
