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

#અન્તર્ભાવ "hb.h"

#ifdef __cplusplus

#અન્તર્ભાવ <functional>
#અન્તર્ભાવ <utility>

#જો 0
#જો !(__cplusplus >= 201103L)
#error "HarfBuzz C++ helpers require C++11"
#endif
#endif

નામાકાશ hb {


template <typename T>
સંરચના vtable;

template <typename T>
સંરચના shared_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit shared_ptr (T *p = nullptr) : p (p) {}
  shared_ptr (અચલ shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  shared_ptr& operator = (અચલ shared_ptr &o) { જો (p != o.p) { destroy (); p = o.p; reference (); } પાછા *આ; }
  shared_ptr& operator = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; પાછા *આ; }
  ~shared_ptr () { v::destroy (p); p = nullptr; }

  T* get() અચલ { પાછા p; }

  શૂન્ય swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  friend શૂન્ય swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  operator T * () અચલ { પાછા p; }
  T& operator * () અચલ { પાછા *get (); }
  T* operator -> () અચલ { પાછા get (); }
  operator તાર્કિક () અચલ { પાછા p; }
  તાર્કિક operator == (અચલ shared_ptr &o) અચલ { પાછા p == o.p; }
  તાર્કિક operator != (અચલ shared_ptr &o) અચલ { પાછા p != o.p; }

  સ્થિર T* get_empty() { પાછા v::get_empty (); }
  T* reference() { પાછા v::reference (p); }
  શૂન્ય destroy() { v::destroy (p); }
  શૂન્ય set_user_data (hb_user_data_ચાવી_t *key,
		      શૂન્ય *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::set_user_data (p, key, value, destroy, replace); } 
  શૂન્ય * get_user_data (hb_user_data_ચાવી_t *key) { પાછા v::get_user_data (p, key); }

  ખાનગી:
  T *p;
};

template<typename T> સંરચના is_shared_ptr : std::false_type {};
template<typename T> સંરચના is_shared_ptr<shared_ptr<T>> : std::true_type {};

template <typename T>
સંરચના unique_ptr
{
  using element_type = T;

  using v = vtable<T>;

  explicit unique_ptr (T *p = nullptr) : p (p) {}
  unique_ptr (અચલ unique_ptr &o) = delete;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = nullptr; }
  unique_ptr& operator = (અચલ unique_ptr &o) = delete;
  unique_ptr& operator = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = nullptr; પાછા *આ; }
  ~unique_ptr () { v::destroy (p); p = nullptr; }

  T* get() અચલ { પાછા p; }
  T* release () { T* v = p; p = nullptr; પાછા v; }

  શૂન્ય swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  friend શૂન્ય swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  operator T * () અચલ { પાછા p; }
  T& operator * () અચલ { પાછા *get (); }
  T* operator -> () અચલ { પાછા get (); }
  operator તાર્કિક () { પાછા p; }

  ખાનગી:
  T *p;
};

template<typename T> સંરચના is_unique_ptr : std::false_type {};
template<typename T> સંરચના is_unique_ptr<unique_ptr<T>> : std::true_type {};

template <typename T,
	  T * (*_get_empty) (શૂન્ય),
	  T * (*_reference) (T *),
	  શૂન્ય (*_destroy) (T *),
	  hb_bool_t (*_set_user_data) (T *,
				       hb_user_data_ચાવી_t *,
				       શૂન્ય *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  શૂન્ય * (*_get_user_data) (અચલ T *,
				    hb_user_data_ચાવી_t *)>
સંરચના vtable_t
{
  સ્થિર constexpr auto get_empty = _get_empty;
  સ્થિર constexpr auto reference = _reference;
  સ્થિર constexpr auto destroy = _destroy;
  સ્થિર constexpr auto set_user_data = _set_user_data;
  સ્થિર constexpr auto get_user_data = _get_user_data;
};

#define HB_DEFINE_VTABLE(name, empty) \
	template<> \
	સંરચના vtable<hb_##name##_t> \
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

HB_DEFINE_VTABLE (subset_આગત, nullptr);
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
નામાકાશ std {


template<typename T>
સંરચના hash<hb::shared_ptr<T>>
{
    std::size_t operator()(અચલ hb::shared_ptr<T>& v) અચલ noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        પાછા h;
    }
};

template<typename T>
સંરચના hash<hb::unique_ptr<T>>
{
    std::size_t operator()(અચલ hb::unique_ptr<T>& v) અચલ noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        પાછા h;
    }
};


} // namespace std

#endif /* __cplusplus */

#endif /* HB_CPLUSPLUS_HH */
