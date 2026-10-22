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

#அந்தர்பாவ "hb.h"

#ifdef __cplusplus

#அந்தர்பாவ <functional>
#அந்தர்பாவ <utility>

#எனில் 0
#எனில் !(__cplusplus >= 201103L)
#error "HarfBuzz C++ helpers require C++11"
#endif
#endif

நாமாகாஶ hb {


template <typename T>
கட்டமைப்பு vtable;

template <typename T>
கட்டமைப்பு shared_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit shared_ptr (T *p = nullptr) : p (p) {}
  shared_ptr (மாறிலி shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  shared_ptr& operator = (மாறிலி shared_ptr &o) { எனில் (p != o.p) { destroy (); p = o.p; reference (); } திரும்பு *இது; }
  shared_ptr& operator = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; திரும்பு *இது; }
  ~shared_ptr () { v::destroy (p); p = nullptr; }

  T* get() மாறிலி { திரும்பு p; }

  வெறுமை swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  friend வெறுமை swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  operator T * () மாறிலி { திரும்பு p; }
  T& operator * () மாறிலி { திரும்பு *get (); }
  T* operator -> () மாறிலி { திரும்பு get (); }
  operator தருக்க () மாறிலி { திரும்பு p; }
  தருக்க operator == (மாறிலி shared_ptr &o) மாறிலி { திரும்பு p == o.p; }
  தருக்க operator != (மாறிலி shared_ptr &o) மாறிலி { திரும்பு p != o.p; }

  நிலையான T* get_empty() { திரும்பு v::get_empty (); }
  T* reference() { திரும்பு v::reference (p); }
  வெறுமை destroy() { v::destroy (p); }
  வெறுமை set_user_data (hb_user_data_சாவி_t *key,
		      வெறுமை *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::set_user_data (p, key, value, destroy, replace); } 
  வெறுமை * get_user_data (hb_user_data_சாவி_t *key) { திரும்பு v::get_user_data (p, key); }

  தனிப்பட்ட:
  T *p;
};

template<typename T> கட்டமைப்பு is_shared_ptr : std::false_type {};
template<typename T> கட்டமைப்பு is_shared_ptr<shared_ptr<T>> : std::true_type {};

template <typename T>
கட்டமைப்பு unique_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit unique_ptr (T *p = nullptr) : p (p) {}
  unique_ptr (மாறிலி unique_ptr &o) = delete;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  unique_ptr& operator = (மாறிலி unique_ptr &o) = delete;
  unique_ptr& operator = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; திரும்பு *இது; }
  ~unique_ptr () { v::destroy (p); p = nullptr; }

  T* get() மாறிலி { திரும்பு p; }
  T* release () { T* v = p; p = nullptr; திரும்பு v; }

  வெறுமை swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  friend வெறுமை swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  operator T * () மாறிலி { திரும்பு p; }
  T& operator * () மாறிலி { திரும்பு *get (); }
  T* operator -> () மாறிலி { திரும்பு get (); }
  operator தருக்க () { திரும்பு p; }

  தனிப்பட்ட:
  T *p;
};

template<typename T> கட்டமைப்பு is_unique_ptr : std::false_type {};
template<typename T> கட்டமைப்பு is_unique_ptr<unique_ptr<T>> : std::true_type {};

template <typename T,
	  T * (*_get_empty) (வெறுமை),
	  T * (*_reference) (T *),
	  வெறுமை (*_destroy) (T *),
	  hb_bool_t (*_set_user_data) (T *,
				       hb_user_data_சாவி_t *,
				       வெறுமை *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  வெறுமை * (*_get_user_data) (மாறிலி T *,
				    hb_user_data_சாவி_t *)>
கட்டமைப்பு vtable_t
{
  நிலையான constexpr auto get_empty = _get_empty;
  நிலையான constexpr auto reference = _reference;
  நிலையான constexpr auto destroy = _destroy;
  நிலையான constexpr auto set_user_data = _set_user_data;
  நிலையான constexpr auto get_user_data = _get_user_data;
};

#define HB_DEFINE_VTABLE(name, empty) \
	template<> \
	கட்டமைப்பு vtable<hb_##name##_t> \
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

HB_DEFINE_VTABLE (subset_உள்ளீடு, nullptr);
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
நாமாகாஶ std {


template<typename T>
கட்டமைப்பு hash<hb::shared_ptr<T>>
{
    std::size_t operator()(மாறிலி hb::shared_ptr<T>& v) மாறிலி noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        திரும்பு h;
    }
};

template<typename T>
கட்டமைப்பு hash<hb::unique_ptr<T>>
{
    std::size_t operator()(மாறிலி hb::unique_ptr<T>& v) மாறிலி noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        திரும்பு h;
    }
};


} // namespace std

#endif /* __cplusplus */

#endif /* HB_CPLUSPLUS_HH */
