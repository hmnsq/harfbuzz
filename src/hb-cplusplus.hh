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

#اگر_غیر_تعریف_شدہ HB_CPLUSPLUS_HH
#تعریف_کرو HB_CPLUSPLUS_HH

#شامل "hb.h"

#اگر_تعریف_شدہ __cplusplus

#شامل <functional>
#شامل <utility>

#اگر 0
#اگر !(__cplusplus >= 201103L)
#غلطی "HarfBuzz C++ helpers require C++11"
#اگر_ختم
#اگر_ختم

نامخلا hb {


سانچہ <قسم_نام T>
ساخت vtable;

سانچہ <قسم_نام T>
ساخت shared_ptr
{
  استعمال element_type = T;

  استعمال v = vtable<T>;

  صریح shared_ptr (T *p = خالی_اشاریہ) : p (p) {}
  shared_ptr (ثابت shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = خالی_اشاریہ; }
  shared_ptr& عامل = (ثابت shared_ptr &o) { اگر (p != o.p) { destroy (); p = o.p; reference (); } واپس *یہ; }
  shared_ptr& عامل = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = خالی_اشاریہ; واپس *یہ; }
  ~shared_ptr () { v::destroy (p); p = خالی_اشاریہ; }

  T* get() ثابت { واپس p; }

  خالی swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  دوست خالی swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  عامل T * () ثابت { واپس p; }
  T& عامل * () ثابت { واپس *get (); }
  T* عامل -> () ثابت { واپس get (); }
  عامل منطقی () ثابت { واپس p; }
  منطقی عامل == (ثابت shared_ptr &o) ثابت { واپس p == o.p; }
  منطقی عامل != (ثابت shared_ptr &o) ثابت { واپس p != o.p; }

  ساکن T* get_خالی_خانے() { واپس v::get_خالی_خانے (); }
  T* reference() { واپس v::reference (p); }
  خالی destroy() { v::destroy (p); }
  خالی سیٹ_user_ڈیٹا (hb_user_ڈیٹا_کلید_t *key,
		      خالی *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::سیٹ_user_ڈیٹا (p, key, value, destroy, replace); } 
  خالی * get_user_ڈیٹا (hb_user_ڈیٹا_کلید_t *key) { واپس v::get_user_ڈیٹا (p, key); }

  نجی:
  T *p;
};

سانچہ<قسم_نام T> ساخت is_shared_ptr : std::false_type {};
سانچہ<قسم_نام T> ساخت is_shared_ptr<shared_ptr<T>> : std::true_type {};

سانچہ <قسم_نام T>
ساخت unique_ptr
{
  استعمال element_type = T;

  استعمال v = vtable<T>;

  صریح unique_ptr (T *p = خالی_اشاریہ) : p (p) {}
  unique_ptr (ثابت unique_ptr &o) = حذف;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = خالی_اشاریہ; }
  unique_ptr& عامل = (ثابت unique_ptr &o) = حذف;
  unique_ptr& عامل = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = خالی_اشاریہ; واپس *یہ; }
  ~unique_ptr () { v::destroy (p); p = خالی_اشاریہ; }

  T* get() ثابت { واپس p; }
  T* release () { T* v = p; p = خالی_اشاریہ; واپس v; }

  خالی swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  دوست خالی swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  عامل T * () ثابت { واپس p; }
  T& عامل * () ثابت { واپس *get (); }
  T* عامل -> () ثابت { واپس get (); }
  عامل منطقی () { واپس p; }

  نجی:
  T *p;
};

سانچہ<قسم_نام T> ساخت is_unique_ptr : std::false_type {};
سانچہ<قسم_نام T> ساخت is_unique_ptr<unique_ptr<T>> : std::true_type {};

سانچہ <قسم_نام T,
	  T * (*get_خالی_خانے_2) (خالی),
	  T * (*_reference) (T *),
	  خالی (*_destroy) (T *),
	  hb_bool_t (*سیٹ_user_ڈیٹا_2) (T *,
				       hb_user_ڈیٹا_کلید_t *,
				       خالی *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  خالی * (*get_user_ڈیٹا_2) (ثابت T *,
				    hb_user_ڈیٹا_کلید_t *)>
ساخت vtable_t
{
  ساکن constexpr خودکار get_خالی_خانے = get_خالی_خانے_2;
  ساکن constexpr خودکار reference = _reference;
  ساکن constexpr خودکار destroy = _destroy;
  ساکن constexpr خودکار سیٹ_user_ڈیٹا = سیٹ_user_ڈیٹا_2;
  ساکن constexpr خودکار get_user_ڈیٹا = get_user_ڈیٹا_2;
};

#تعریف_کرو HB_DEFINE_VTABLE(name, خالی_خانے_2) \
	سانچہ<> \
	ساخت vtable<hb_##name##_t> \
	     : vtable_t<hb_##name##_t, \
			خالی_خانے_2, \
			&hb_##name##_reference, \
			&hb_##name##_destroy, \
			&hb_##name##_set_user_data, \
			&hb_##name##_get_user_data> {}

HB_DEFINE_VTABLE (بفر, &hb_بفر_get_خالی_خانے);
HB_DEFINE_VTABLE (blob, &hb_blob_get_خالی_خانے);
HB_DEFINE_VTABLE (face, &hb_face_get_خالی_خانے);
HB_DEFINE_VTABLE (font, &hb_font_get_خالی_خانے);
HB_DEFINE_VTABLE (font_funcs, &hb_font_funcs_get_خالی_خانے);
HB_DEFINE_VTABLE (map, &hb_map_get_خالی_خانے);
HB_DEFINE_VTABLE (سیٹ, &hb_سیٹ_get_خالی_خانے);
HB_DEFINE_VTABLE (shape_plan, &hb_shape_plan_get_خالی_خانے);
HB_DEFINE_VTABLE (unicode_funcs, &hb_unicode_funcs_get_خالی_خانے);
HB_DEFINE_VTABLE (draw_funcs, &hb_draw_funcs_get_خالی_خانے);
HB_DEFINE_VTABLE (paint_funcs, &hb_paint_funcs_get_خالی_خانے);


#اگر_تعریف_شدہ HB_SUBSET_H

HB_DEFINE_VTABLE (subset_داخلہ, خالی_اشاریہ);
HB_DEFINE_VTABLE (subset_plan, خالی_اشاریہ);

#اگر_ختم


#اگر_تعریف_شدہ HB_RASTER_H

HB_DEFINE_VTABLE (raster_image, خالی_اشاریہ);
HB_DEFINE_VTABLE (raster_draw, خالی_اشاریہ);
HB_DEFINE_VTABLE (raster_paint, خالی_اشاریہ);

#اگر_ختم


#اگر_تعریف_شدہ HB_سمتیہ_H

HB_DEFINE_VTABLE (سمتیہ_draw, خالی_اشاریہ);
HB_DEFINE_VTABLE (سمتیہ_paint, خالی_اشاریہ);

#اگر_ختم


#اگر_تعریف_شدہ HB_GPU_H

HB_DEFINE_VTABLE (gpu_draw, خالی_اشاریہ);

#اگر_ختم

/* HB_DEFINE_VTABLE stays available for subsystem headers' paired tail
 * blocks (see hb-raster.h, hb-vector.h, hb-gpu.h, hb-subset.h). */


} // namespace hb

/* Workaround for GCC < 7, see:
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
 * https://stackoverflow.com/a/25594741 */
نامخلا std {


سانچہ<قسم_نام T>
ساخت hash<hb::shared_ptr<T>>
{
    std::size_t عامل()(ثابت hb::shared_ptr<T>& v) ثابت noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        واپس h;
    }
};

سانچہ<قسم_نام T>
ساخت hash<hb::unique_ptr<T>>
{
    std::size_t عامل()(ثابت hb::unique_ptr<T>& v) ثابت noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        واپس h;
    }
};


} // namespace std

#اگر_ختم /* __cplusplus */

#اگر_ختم /* HB_CPLUSPLUS_HH */
