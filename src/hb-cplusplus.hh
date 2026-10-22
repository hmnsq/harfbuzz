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

#جيڪڏهن_وصف_نه_ٿيل HB_CPLUSPLUS_HH
#وصف_ڪريو HB_CPLUSPLUS_HH

#ضمن "hb.h"

#جيڪڏهن_وصف_ٿيل __cplusplus

#ضمن <functional>
#ضمن <utility>

#إذا 0
#إذا !(__cplusplus >= 201103L)
#غلطي "HarfBuzz C++ helpers require C++11"
#جيڪڏهن_پورو
#جيڪڏهن_پورو

نطاق hb {


سانچو <قسم_نالو T>
بنية vtable;

سانچو <قسم_نالو T>
بنية shared_ptr
{
  استعمال element_type = T;

  استعمال v = vtable<T>;

  واضح shared_ptr (T *p = خالي_اشارو) : p (p) {}
  shared_ptr (ثابت shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = خالي_اشارو; }
  shared_ptr& عامل = (ثابت shared_ptr &o) { إذا (p != o.p) { destroy (); p = o.p; reference (); } أرجع *هذا; }
  shared_ptr& عامل = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = خالي_اشارو; أرجع *هذا; }
  ~shared_ptr () { v::destroy (p); p = خالي_اشارو; }

  T* get() ثابت { أرجع p; }

  فراغ swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  دوست فراغ swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  عامل T * () ثابت { أرجع p; }
  T& عامل * () ثابت { أرجع *get (); }
  T* عامل -> () ثابت { أرجع get (); }
  عامل منطقي () ثابت { أرجع p; }
  منطقي عامل == (ثابت shared_ptr &o) ثابت { أرجع p == o.p; }
  منطقي عامل != (ثابت shared_ptr &o) ثابت { أرجع p != o.p; }

  ساكن T* get_خالي_جايون() { أرجع v::get_خالي_جايون (); }
  T* reference() { أرجع v::reference (p); }
  فراغ destroy() { v::destroy (p); }
  فراغ سيٽ_user_معلومات (hb_user_معلومات_ڪنجي_t *key,
		      فراغ *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::سيٽ_user_معلومات (p, key, value, destroy, replace); } 
  فراغ * get_user_معلومات (hb_user_معلومات_ڪنجي_t *key) { أرجع v::get_user_معلومات (p, key); }

  خاص:
  T *p;
};

سانچو<قسم_نالو T> بنية is_shared_ptr : std::false_type {};
سانچو<قسم_نالو T> بنية is_shared_ptr<shared_ptr<T>> : std::true_type {};

سانچو <قسم_نالو T>
بنية unique_ptr
{
  استعمال element_type = T;

  استعمال v = vtable<T>;

  واضح unique_ptr (T *p = خالي_اشارو) : p (p) {}
  unique_ptr (ثابت unique_ptr &o) = ميٽيو;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = خالي_اشارو; }
  unique_ptr& عامل = (ثابت unique_ptr &o) = ميٽيو;
  unique_ptr& عامل = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = خالي_اشارو; أرجع *هذا; }
  ~unique_ptr () { v::destroy (p); p = خالي_اشارو; }

  T* get() ثابت { أرجع p; }
  T* release () { T* v = p; p = خالي_اشارو; أرجع v; }

  فراغ swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  دوست فراغ swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  عامل T * () ثابت { أرجع p; }
  T& عامل * () ثابت { أرجع *get (); }
  T* عامل -> () ثابت { أرجع get (); }
  عامل منطقي () { أرجع p; }

  خاص:
  T *p;
};

سانچو<قسم_نالو T> بنية is_unique_ptr : std::false_type {};
سانچو<قسم_نالو T> بنية is_unique_ptr<unique_ptr<T>> : std::true_type {};

سانچو <قسم_نالو T,
	  T * (*get_خالي_جايون_2) (فراغ),
	  T * (*_reference) (T *),
	  فراغ (*_destroy) (T *),
	  hb_bool_t (*سيٽ_user_معلومات_2) (T *,
				       hb_user_معلومات_ڪنجي_t *,
				       فراغ *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  فراغ * (*get_user_معلومات_2) (ثابت T *,
				    hb_user_معلومات_ڪنجي_t *)>
بنية vtable_t
{
  ساكن constexpr خودڪار get_خالي_جايون = get_خالي_جايون_2;
  ساكن constexpr خودڪار reference = _reference;
  ساكن constexpr خودڪار destroy = _destroy;
  ساكن constexpr خودڪار سيٽ_user_معلومات = سيٽ_user_معلومات_2;
  ساكن constexpr خودڪار get_user_معلومات = get_user_معلومات_2;
};

#وصف_ڪريو HB_DEFINE_VTABLE(name, خالي_جايون_2) \
	سانچو<> \
	بنية vtable<hb_##name##_t> \
	     : vtable_t<hb_##name##_t, \
			خالي_جايون_2, \
			&hb_##name##_reference, \
			&hb_##name##_destroy, \
			&hb_##name##_set_user_data, \
			&hb_##name##_get_user_data> {}

HB_DEFINE_VTABLE (بفر, &hb_بفر_get_خالي_جايون);
HB_DEFINE_VTABLE (blob, &hb_blob_get_خالي_جايون);
HB_DEFINE_VTABLE (face, &hb_face_get_خالي_جايون);
HB_DEFINE_VTABLE (font, &hb_font_get_خالي_جايون);
HB_DEFINE_VTABLE (font_funcs, &hb_font_funcs_get_خالي_جايون);
HB_DEFINE_VTABLE (map, &hb_map_get_خالي_جايون);
HB_DEFINE_VTABLE (سيٽ, &hb_سيٽ_get_خالي_جايون);
HB_DEFINE_VTABLE (shape_plan, &hb_shape_plan_get_خالي_جايون);
HB_DEFINE_VTABLE (unicode_funcs, &hb_unicode_funcs_get_خالي_جايون);
HB_DEFINE_VTABLE (draw_funcs, &hb_draw_funcs_get_خالي_جايون);
HB_DEFINE_VTABLE (paint_funcs, &hb_paint_funcs_get_خالي_جايون);


#جيڪڏهن_وصف_ٿيل HB_SUBSET_H

HB_DEFINE_VTABLE (subset_داخلا, خالي_اشارو);
HB_DEFINE_VTABLE (subset_plan, خالي_اشارو);

#جيڪڏهن_پورو


#جيڪڏهن_وصف_ٿيل HB_RASTER_H

HB_DEFINE_VTABLE (raster_image, خالي_اشارو);
HB_DEFINE_VTABLE (raster_draw, خالي_اشارو);
HB_DEFINE_VTABLE (raster_paint, خالي_اشارو);

#جيڪڏهن_پورو


#جيڪڏهن_وصف_ٿيل HB_ويڪٽر_H

HB_DEFINE_VTABLE (ويڪٽر_draw, خالي_اشارو);
HB_DEFINE_VTABLE (ويڪٽر_paint, خالي_اشارو);

#جيڪڏهن_پورو


#جيڪڏهن_وصف_ٿيل HB_GPU_H

HB_DEFINE_VTABLE (gpu_draw, خالي_اشارو);

#جيڪڏهن_پورو

/* HB_DEFINE_VTABLE stays available for subsystem headers' paired tail
 * blocks (see hb-raster.h, hb-vector.h, hb-gpu.h, hb-subset.h). */


} // namespace hb

/* Workaround for GCC < 7, see:
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
 * https://stackoverflow.com/a/25594741 */
نطاق std {


سانچو<قسم_نالو T>
بنية hash<hb::shared_ptr<T>>
{
    std::size_t عامل()(ثابت hb::shared_ptr<T>& v) ثابت noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        أرجع h;
    }
};

سانچو<قسم_نالو T>
بنية hash<hb::unique_ptr<T>>
{
    std::size_t عامل()(ثابت hb::unique_ptr<T>& v) ثابت noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        أرجع h;
    }
};


} // namespace std

#جيڪڏهن_پورو /* __cplusplus */

#جيڪڏهن_پورو /* HB_CPLUSPLUS_HH */
