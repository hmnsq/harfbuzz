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

#অন্তর্ভাব "hb.h"

#ifdef __cplusplus

#অন্তর্ভাব <functional>
#অন্তর্ভাব <utility>

#যদি 0
#যদি !(__cplusplus >= 201103L)
#error "HarfBuzz C++ helpers require C++11"
#endif
#endif

নামাকাশ hb {


template <typename T>
কাঠামো vtable;

template <typename T>
কাঠামো shared_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit shared_ptr (T *p = nullptr) : p (p) {}
  shared_ptr (ধ্রুবক shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  shared_ptr& operator = (ধ্রুবক shared_ptr &o) { যদি (p != o.p) { destroy (); p = o.p; reference (); } ফেরত *এই; }
  shared_ptr& operator = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; ফেরত *এই; }
  ~shared_ptr () { v::destroy (p); p = nullptr; }

  T* get() ধ্রুবক { ফেরত p; }

  শূন্য swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  friend শূন্য swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  operator T * () ধ্রুবক { ফেরত p; }
  T& operator * () ধ্রুবক { ফেরত *get (); }
  T* operator -> () ধ্রুবক { ফেরত get (); }
  operator যৌক্তিক () ধ্রুবক { ফেরত p; }
  যৌক্তিক operator == (ধ্রুবক shared_ptr &o) ধ্রুবক { ফেরত p == o.p; }
  যৌক্তিক operator != (ধ্রুবক shared_ptr &o) ধ্রুবক { ফেরত p != o.p; }

  স্থির T* get_empty() { ফেরত v::get_empty (); }
  T* reference() { ফেরত v::reference (p); }
  শূন্য destroy() { v::destroy (p); }
  শূন্য set_user_তথ্য (hb_user_তথ্য_চাবি_t *key,
		      শূন্য *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::set_user_তথ্য (p, key, value, destroy, replace); } 
  শূন্য * get_user_তথ্য (hb_user_তথ্য_চাবি_t *key) { ফেরত v::get_user_তথ্য (p, key); }

  ব্যক্তিগত:
  T *p;
};

template<typename T> কাঠামো is_shared_ptr : std::false_type {};
template<typename T> কাঠামো is_shared_ptr<shared_ptr<T>> : std::true_type {};

template <typename T>
কাঠামো unique_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit unique_ptr (T *p = nullptr) : p (p) {}
  unique_ptr (ধ্রুবক unique_ptr &o) = delete;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  unique_ptr& operator = (ধ্রুবক unique_ptr &o) = delete;
  unique_ptr& operator = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; ফেরত *এই; }
  ~unique_ptr () { v::destroy (p); p = nullptr; }

  T* get() ধ্রুবক { ফেরত p; }
  T* release () { T* v = p; p = nullptr; ফেরত v; }

  শূন্য swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  friend শূন্য swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  operator T * () ধ্রুবক { ফেরত p; }
  T& operator * () ধ্রুবক { ফেরত *get (); }
  T* operator -> () ধ্রুবক { ফেরত get (); }
  operator যৌক্তিক () { ফেরত p; }

  ব্যক্তিগত:
  T *p;
};

template<typename T> কাঠামো is_unique_ptr : std::false_type {};
template<typename T> কাঠামো is_unique_ptr<unique_ptr<T>> : std::true_type {};

template <typename T,
	  T * (*_get_empty) (শূন্য),
	  T * (*_reference) (T *),
	  শূন্য (*_destroy) (T *),
	  hb_bool_t (*set_user_তথ্য_2) (T *,
				       hb_user_তথ্য_চাবি_t *,
				       শূন্য *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  শূন্য * (*get_user_তথ্য_2) (ধ্রুবক T *,
				    hb_user_তথ্য_চাবি_t *)>
কাঠামো vtable_t
{
  স্থির constexpr auto get_empty = _get_empty;
  স্থির constexpr auto reference = _reference;
  স্থির constexpr auto destroy = _destroy;
  স্থির constexpr auto set_user_তথ্য = set_user_তথ্য_2;
  স্থির constexpr auto get_user_তথ্য = get_user_তথ্য_2;
};

#define HB_DEFINE_VTABLE(name, empty) \
	template<> \
	কাঠামো vtable<hb_##name##_t> \
	     : vtable_t<hb_##name##_t, \
			empty, \
			&hb_##name##_reference, \
			&hb_##name##_destroy, \
			&hb_##name##_set_user_data, \
			&hb_##name##_get_user_data> {}

HB_DEFINE_VTABLE (বাফার, &hb_বাফার_get_empty);
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

HB_DEFINE_VTABLE (subset_প্রবেশ, nullptr);
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
নামাকাশ std {


template<typename T>
কাঠামো hash<hb::shared_ptr<T>>
{
    std::size_t operator()(ধ্রুবক hb::shared_ptr<T>& v) ধ্রুবক noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        ফেরত h;
    }
};

template<typename T>
কাঠামো hash<hb::unique_ptr<T>>
{
    std::size_t operator()(ধ্রুবক hb::unique_ptr<T>& v) ধ্রুবক noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        ফেরত h;
    }
};


} // namespace std

#endif /* __cplusplus */

#endif /* HB_CPLUSPLUS_HH */
