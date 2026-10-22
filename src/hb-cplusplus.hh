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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CPLUSPLUS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CPLUSPLUS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus

#ਅਨ੍ਤਰ੍ਭਾਵ <functional>
#ਅਨ੍ਤਰ੍ਭਾਵ <utility>

#ਜੇ 0
#ਜੇ !(__cplusplus >= 201103L)
#ਗਲਤੀ "HarfBuzz C++ helpers require C++11"
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ hb {


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ vtable;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ shared_ptr
{
  ਵਰਤੋਂ element_type = T;

  ਵਰਤੋਂ v = vtable<T>;

  ਸਪਸ਼ਟ shared_ptr (T *p = ਖਾਲੀ_ਸੂਚਕ) : p (p) {}
  shared_ptr (ਅਚਲ shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = ਖਾਲੀ_ਸੂਚਕ; }
  shared_ptr& ਸੰਚਾਲਕ = (ਅਚਲ shared_ptr &o) { ਜੇ (p != o.p) { destroy (); p = o.p; reference (); } ਵਾਪਸ *ਇਹ; }
  shared_ptr& ਸੰਚਾਲਕ = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = ਖਾਲੀ_ਸੂਚਕ; ਵਾਪਸ *ਇਹ; }
  ~shared_ptr () { v::destroy (p); p = ਖਾਲੀ_ਸੂਚਕ; }

  T* get() ਅਚਲ { ਵਾਪਸ p; }

  ਖਾਲੀ swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  ਮਿੱਤਰ ਖਾਲੀ swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  ਸੰਚਾਲਕ T * () ਅਚਲ { ਵਾਪਸ p; }
  T& ਸੰਚਾਲਕ * () ਅਚਲ { ਵਾਪਸ *get (); }
  T* ਸੰਚਾਲਕ -> () ਅਚਲ { ਵਾਪਸ get (); }
  ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ p; }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ shared_ptr &o) ਅਚਲ { ਵਾਪਸ p == o.p; }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ shared_ptr &o) ਅਚਲ { ਵਾਪਸ p != o.p; }

  ਸਥਿਰ T* get_ਖਾਲੀ_ਥਾਵਾਂ() { ਵਾਪਸ v::get_ਖਾਲੀ_ਥਾਵਾਂ (); }
  T* reference() { ਵਾਪਸ v::reference (p); }
  ਖਾਲੀ destroy() { v::destroy (p); }
  ਖਾਲੀ ਸੈੱਟ_user_ਅੰਕੜੇ (hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
		      ਖਾਲੀ *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::ਸੈੱਟ_user_ਅੰਕੜੇ (p, key, value, destroy, replace); } 
  ਖਾਲੀ * get_user_ਅੰਕੜੇ (hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key) { ਵਾਪਸ v::get_user_ਅੰਕੜੇ (p, key); }

  ਨਿਜੀ:
  T *p;
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ is_shared_ptr : std::false_type {};
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ is_shared_ptr<shared_ptr<T>> : std::true_type {};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ unique_ptr
{
  ਵਰਤੋਂ element_type = T;

  ਵਰਤੋਂ v = vtable<T>;

  ਸਪਸ਼ਟ unique_ptr (T *p = ਖਾਲੀ_ਸੂਚਕ) : p (p) {}
  unique_ptr (ਅਚਲ unique_ptr &o) = ਮਿਟਾਓ;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = ਖਾਲੀ_ਸੂਚਕ; }
  unique_ptr& ਸੰਚਾਲਕ = (ਅਚਲ unique_ptr &o) = ਮਿਟਾਓ;
  unique_ptr& ਸੰਚਾਲਕ = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = ਖਾਲੀ_ਸੂਚਕ; ਵਾਪਸ *ਇਹ; }
  ~unique_ptr () { v::destroy (p); p = ਖਾਲੀ_ਸੂਚਕ; }

  T* get() ਅਚਲ { ਵਾਪਸ p; }
  T* release () { T* v = p; p = ਖਾਲੀ_ਸੂਚਕ; ਵਾਪਸ v; }

  ਖਾਲੀ swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  ਮਿੱਤਰ ਖਾਲੀ swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  ਸੰਚਾਲਕ T * () ਅਚਲ { ਵਾਪਸ p; }
  T& ਸੰਚਾਲਕ * () ਅਚਲ { ਵਾਪਸ *get (); }
  T* ਸੰਚਾਲਕ -> () ਅਚਲ { ਵਾਪਸ get (); }
  ਸੰਚਾਲਕ ਤਾਰਕਿਕ () { ਵਾਪਸ p; }

  ਨਿਜੀ:
  T *p;
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ is_unique_ptr : std::false_type {};
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ is_unique_ptr<unique_ptr<T>> : std::true_type {};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T,
	  T * (*get_ਖਾਲੀ_ਥਾਵਾਂ_2) (ਖਾਲੀ),
	  T * (*_reference) (T *),
	  ਖਾਲੀ (*_destroy) (T *),
	  hb_bool_t (*ਸੈੱਟ_user_ਅੰਕੜੇ_2) (T *,
				       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *,
				       ਖਾਲੀ *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  ਖਾਲੀ * (*get_user_ਅੰਕੜੇ_2) (ਅਚਲ T *,
				    hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *)>
ਬਣਤਰ vtable_t
{
  ਸਥਿਰ constexpr ਸਵੈਚਾਲਿਤ get_ਖਾਲੀ_ਥਾਵਾਂ = get_ਖਾਲੀ_ਥਾਵਾਂ_2;
  ਸਥਿਰ constexpr ਸਵੈਚਾਲਿਤ reference = _reference;
  ਸਥਿਰ constexpr ਸਵੈਚਾਲਿਤ destroy = _destroy;
  ਸਥਿਰ constexpr ਸਵੈਚਾਲਿਤ ਸੈੱਟ_user_ਅੰਕੜੇ = ਸੈੱਟ_user_ਅੰਕੜੇ_2;
  ਸਥਿਰ constexpr ਸਵੈਚਾਲਿਤ get_user_ਅੰਕੜੇ = get_user_ਅੰਕੜੇ_2;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEFINE_VTABLE(name, ਖਾਲੀ_ਥਾਵਾਂ_2) \
	ਸਾਂਚਾ<> \
	ਬਣਤਰ vtable<hb_##name##_t> \
	     : vtable_t<hb_##name##_t, \
			ਖਾਲੀ_ਥਾਵਾਂ_2, \
			&hb_##name##_reference, \
			&hb_##name##_destroy, \
			&hb_##name##_set_user_data, \
			&hb_##name##_get_user_data> {}

HB_DEFINE_VTABLE (ਬਫਰ, &hb_ਬਫਰ_get_ਖਾਲੀ_ਥਾਵਾਂ);
HB_DEFINE_VTABLE (blob, &hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ);
HB_DEFINE_VTABLE (face, &hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ);
HB_DEFINE_VTABLE (font, &hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ);
HB_DEFINE_VTABLE (font_funcs, &hb_font_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ);
HB_DEFINE_VTABLE (map, &hb_map_get_ਖਾਲੀ_ਥਾਵਾਂ);
HB_DEFINE_VTABLE (ਸੈੱਟ, &hb_ਸੈੱਟ_get_ਖਾਲੀ_ਥਾਵਾਂ);
HB_DEFINE_VTABLE (shape_plan, &hb_shape_plan_get_ਖਾਲੀ_ਥਾਵਾਂ);
HB_DEFINE_VTABLE (unicode_funcs, &hb_unicode_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ);
HB_DEFINE_VTABLE (draw_funcs, &hb_draw_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ);
HB_DEFINE_VTABLE (paint_funcs, &hb_paint_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ);


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_H

HB_DEFINE_VTABLE (subset_ਨਿਵੇਸ਼, ਖਾਲੀ_ਸੂਚਕ);
HB_DEFINE_VTABLE (subset_plan, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅੰਤ


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_RASTER_H

HB_DEFINE_VTABLE (raster_image, ਖਾਲੀ_ਸੂਚਕ);
HB_DEFINE_VTABLE (raster_draw, ਖਾਲੀ_ਸੂਚਕ);
HB_DEFINE_VTABLE (raster_paint, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅੰਤ


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_ਸਦਿਸ਼_H

HB_DEFINE_VTABLE (ਸਦਿਸ਼_draw, ਖਾਲੀ_ਸੂਚਕ);
HB_DEFINE_VTABLE (ਸਦਿਸ਼_paint, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅੰਤ


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_GPU_H

HB_DEFINE_VTABLE (gpu_draw, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅੰਤ

/* HB_DEFINE_VTABLE stays available for subsystem headers' paired tail
 * blocks (see hb-raster.h, hb-vector.h, hb-gpu.h, hb-subset.h). */


} // namespace hb

/* Workaround for GCC < 7, see:
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
 * https://stackoverflow.com/a/25594741 */
ਨਾਮਾਕਾਸ਼ std {


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hash<hb::shared_ptr<T>>
{
    std::size_t ਸੰਚਾਲਕ()(ਅਚਲ hb::shared_ptr<T>& v) ਅਚਲ noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        ਵਾਪਸ h;
    }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hash<hb::unique_ptr<T>>
{
    std::size_t ਸੰਚਾਲਕ()(ਅਚਲ hb::unique_ptr<T>& v) ਅਚਲ noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        ਵਾਪਸ h;
    }
};


} // namespace std

#ਜੇ_ਅੰਤ /* __cplusplus */

#ਜੇ_ਅੰਤ /* HB_CPLUSPLUS_HH */
