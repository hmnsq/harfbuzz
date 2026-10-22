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

#جے_بنا_تعریف HB_CPLUSPLUS_HH
#تعریف_کرو HB_CPLUSPLUS_HH

#ضمن "hb.h"

#جے_تعریف_شدہ __cplusplus

#ضمن <functional>
#ضمن <utility>

#إذا 0
#إذا !(__cplusplus >= 201103L)
#غلطی "HarfBuzz C++ helpers require C++11"
#جے_انت
#جے_انت

نطاق hb {


سانچا <قسم_ناں T>
بنية vtable;

سانچا <قسم_ناں T>
بنية shared_ptr
{
  ورتوں element_type = T;

  ورتوں v = vtable<T>;

  واضح shared_ptr (T *p = خالی_سوچک) : p (p) {}
  shared_ptr (ثابت shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = خالی_سوچک; }
  shared_ptr& عامل = (ثابت shared_ptr &o) { إذا (p != o.p) { destroy (); p = o.p; reference (); } أرجع *هذا; }
  shared_ptr& عامل = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = خالی_سوچک; أرجع *هذا; }
  ~shared_ptr () { v::destroy (p); p = خالی_سوچک; }

  T* get() ثابت { أرجع p; }

  فراغ swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  یار فراغ swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  عامل T * () ثابت { أرجع p; }
  T& عامل * () ثابت { أرجع *get (); }
  T* عامل -> () ثابت { أرجع get (); }
  عامل منطقي () ثابت { أرجع p; }
  منطقي عامل == (ثابت shared_ptr &o) ثابت { أرجع p == o.p; }
  منطقي عامل != (ثابت shared_ptr &o) ثابت { أرجع p != o.p; }

  ساكن T* get_خالی_تھاواں() { أرجع v::get_خالی_تھاواں (); }
  T* reference() { أرجع v::reference (p); }
  فراغ destroy() { v::destroy (p); }
  فراغ سیٹ_user_انکڑے (hb_user_انکڑے_کلید_t *key,
		      فراغ *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::سیٹ_user_انکڑے (p, key, value, destroy, replace); } 
  فراغ * get_user_انکڑے (hb_user_انکڑے_کلید_t *key) { أرجع v::get_user_انکڑے (p, key); }

  خاص:
  T *p;
};

سانچا<قسم_ناں T> بنية is_shared_ptr : std::false_type {};
سانچا<قسم_ناں T> بنية is_shared_ptr<shared_ptr<T>> : std::true_type {};

سانچا <قسم_ناں T>
بنية unique_ptr
{
  ورتوں element_type = T;

  ورتوں v = vtable<T>;

  واضح unique_ptr (T *p = خالی_سوچک) : p (p) {}
  unique_ptr (ثابت unique_ptr &o) = مٹاؤ;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = خالی_سوچک; }
  unique_ptr& عامل = (ثابت unique_ptr &o) = مٹاؤ;
  unique_ptr& عامل = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = خالی_سوچک; أرجع *هذا; }
  ~unique_ptr () { v::destroy (p); p = خالی_سوچک; }

  T* get() ثابت { أرجع p; }
  T* release () { T* v = p; p = خالی_سوچک; أرجع v; }

  فراغ swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  یار فراغ swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  عامل T * () ثابت { أرجع p; }
  T& عامل * () ثابت { أرجع *get (); }
  T* عامل -> () ثابت { أرجع get (); }
  عامل منطقي () { أرجع p; }

  خاص:
  T *p;
};

سانچا<قسم_ناں T> بنية is_unique_ptr : std::false_type {};
سانچا<قسم_ناں T> بنية is_unique_ptr<unique_ptr<T>> : std::true_type {};

سانچا <قسم_ناں T,
	  T * (*get_خالی_تھاواں_2) (فراغ),
	  T * (*_reference) (T *),
	  فراغ (*_destroy) (T *),
	  hb_bool_t (*سیٹ_user_انکڑے_2) (T *,
				       hb_user_انکڑے_کلید_t *,
				       فراغ *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  فراغ * (*get_user_انکڑے_2) (ثابت T *,
				    hb_user_انکڑے_کلید_t *)>
بنية vtable_t
{
  ساكن constexpr خودکار get_خالی_تھاواں = get_خالی_تھاواں_2;
  ساكن constexpr خودکار reference = _reference;
  ساكن constexpr خودکار destroy = _destroy;
  ساكن constexpr خودکار سیٹ_user_انکڑے = سیٹ_user_انکڑے_2;
  ساكن constexpr خودکار get_user_انکڑے = get_user_انکڑے_2;
};

#تعریف_کرو HB_DEFINE_VTABLE(name, خالی_تھاواں_2) \
	سانچا<> \
	بنية vtable<hb_##name##_t> \
	     : vtable_t<hb_##name##_t, \
			خالی_تھاواں_2, \
			&hb_##name##_reference, \
			&hb_##name##_destroy, \
			&hb_##name##_set_user_data, \
			&hb_##name##_get_user_data> {}

HB_DEFINE_VTABLE (بفر, &hb_بفر_get_خالی_تھاواں);
HB_DEFINE_VTABLE (blob, &hb_blob_get_خالی_تھاواں);
HB_DEFINE_VTABLE (face, &hb_face_get_خالی_تھاواں);
HB_DEFINE_VTABLE (font, &hb_font_get_خالی_تھاواں);
HB_DEFINE_VTABLE (font_funcs, &hb_font_funcs_get_خالی_تھاواں);
HB_DEFINE_VTABLE (map, &hb_map_get_خالی_تھاواں);
HB_DEFINE_VTABLE (سیٹ, &hb_سیٹ_get_خالی_تھاواں);
HB_DEFINE_VTABLE (shape_plan, &hb_shape_plan_get_خالی_تھاواں);
HB_DEFINE_VTABLE (unicode_funcs, &hb_unicode_funcs_get_خالی_تھاواں);
HB_DEFINE_VTABLE (draw_funcs, &hb_draw_funcs_get_خالی_تھاواں);
HB_DEFINE_VTABLE (paint_funcs, &hb_paint_funcs_get_خالی_تھاواں);


#جے_تعریف_شدہ HB_SUBSET_H

HB_DEFINE_VTABLE (subset_داخلہ, خالی_سوچک);
HB_DEFINE_VTABLE (subset_plan, خالی_سوچک);

#جے_انت


#جے_تعریف_شدہ HB_RASTER_H

HB_DEFINE_VTABLE (raster_image, خالی_سوچک);
HB_DEFINE_VTABLE (raster_draw, خالی_سوچک);
HB_DEFINE_VTABLE (raster_paint, خالی_سوچک);

#جے_انت


#جے_تعریف_شدہ HB_سمتیہ_H

HB_DEFINE_VTABLE (سمتیہ_draw, خالی_سوچک);
HB_DEFINE_VTABLE (سمتیہ_paint, خالی_سوچک);

#جے_انت


#جے_تعریف_شدہ HB_GPU_H

HB_DEFINE_VTABLE (gpu_draw, خالی_سوچک);

#جے_انت

/* HB_DEFINE_VTABLE stays available for subsystem headers' paired tail
 * blocks (see hb-raster.h, hb-vector.h, hb-gpu.h, hb-subset.h). */


} // namespace hb

/* Workaround for GCC < 7, see:
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
 * https://stackoverflow.com/a/25594741 */
نطاق std {


سانچا<قسم_ناں T>
بنية hash<hb::shared_ptr<T>>
{
    std::size_t عامل()(ثابت hb::shared_ptr<T>& v) ثابت noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        أرجع h;
    }
};

سانچا<قسم_ناں T>
بنية hash<hb::unique_ptr<T>>
{
    std::size_t عامل()(ثابت hb::unique_ptr<T>& v) ثابت noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        أرجع h;
    }
};


} // namespace std

#جے_انت /* __cplusplus */

#جے_انت /* HB_CPLUSPLUS_HH */
