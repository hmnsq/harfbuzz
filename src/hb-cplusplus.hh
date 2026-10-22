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

#اگر_تعریف_نشده HB_CPLUSPLUS_HH
#تعریف_کن HB_CPLUSPLUS_HH

#ضمن "hb.h"

#اگر_تعریف_شده __cplusplus

#ضمن <functional>
#ضمن <utility>

#إذا 0
#إذا !(__cplusplus >= 201103L)
#خطا "HarfBuzz C++ helpers require C++11"
#پایان_اگر
#پایان_اگر

نطاق hb {


قالب <نام_نوع T>
بنية vtable;

قالب <نام_نوع T>
بنية shared_ptr
{
  استفاده element_type = T;

  استفاده v = vtable<T>;

  صریح shared_ptr (T *p = اشاره_تهی) : p (p) {}
  shared_ptr (ثابت shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = اشاره_تهی; }
  shared_ptr& عملگر = (ثابت shared_ptr &o) { إذا (p != o.p) { destroy (); p = o.p; reference (); } أرجع *هذا; }
  shared_ptr& عملگر = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = اشاره_تهی; أرجع *هذا; }
  ~shared_ptr () { v::destroy (p); p = اشاره_تهی; }

  T* get() ثابت { أرجع p; }

  فراغ swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  دوست فراغ swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  عملگر T * () ثابت { أرجع p; }
  T& عملگر * () ثابت { أرجع *get (); }
  T* عملگر -> () ثابت { أرجع get (); }
  عملگر منطقي () ثابت { أرجع p; }
  منطقي عملگر == (ثابت shared_ptr &o) ثابت { أرجع p == o.p; }
  منطقي عملگر != (ثابت shared_ptr &o) ثابت { أرجع p != o.p; }

  ساكن T* get_خالی() { أرجع v::get_خالی (); }
  T* reference() { أرجع v::reference (p); }
  فراغ destroy() { v::destroy (p); }
  فراغ مجموعه_user_داده (hb_user_داده_کلید_t *key,
		      فراغ *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::مجموعه_user_داده (p, key, value, destroy, replace); } 
  فراغ * get_user_داده (hb_user_داده_کلید_t *key) { أرجع v::get_user_داده (p, key); }

  خاص:
  T *p;
};

قالب<نام_نوع T> بنية is_shared_ptr : std::false_type {};
قالب<نام_نوع T> بنية is_shared_ptr<shared_ptr<T>> : std::true_type {};

قالب <نام_نوع T>
بنية unique_ptr
{
  استفاده element_type = T;

  استفاده v = vtable<T>;

  صریح unique_ptr (T *p = اشاره_تهی) : p (p) {}
  unique_ptr (ثابت unique_ptr &o) = حذف;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = اشاره_تهی; }
  unique_ptr& عملگر = (ثابت unique_ptr &o) = حذف;
  unique_ptr& عملگر = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = اشاره_تهی; أرجع *هذا; }
  ~unique_ptr () { v::destroy (p); p = اشاره_تهی; }

  T* get() ثابت { أرجع p; }
  T* release () { T* v = p; p = اشاره_تهی; أرجع v; }

  فراغ swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  دوست فراغ swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  عملگر T * () ثابت { أرجع p; }
  T& عملگر * () ثابت { أرجع *get (); }
  T* عملگر -> () ثابت { أرجع get (); }
  عملگر منطقي () { أرجع p; }

  خاص:
  T *p;
};

قالب<نام_نوع T> بنية is_unique_ptr : std::false_type {};
قالب<نام_نوع T> بنية is_unique_ptr<unique_ptr<T>> : std::true_type {};

قالب <نام_نوع T,
	  T * (*get_خالی_2) (فراغ),
	  T * (*_reference) (T *),
	  فراغ (*_destroy) (T *),
	  hb_bool_t (*مجموعه_user_داده_2) (T *,
				       hb_user_داده_کلید_t *,
				       فراغ *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  فراغ * (*get_user_داده_2) (ثابت T *,
				    hb_user_داده_کلید_t *)>
بنية vtable_t
{
  ساكن constexpr خودکار get_خالی = get_خالی_2;
  ساكن constexpr خودکار reference = _reference;
  ساكن constexpr خودکار destroy = _destroy;
  ساكن constexpr خودکار مجموعه_user_داده = مجموعه_user_داده_2;
  ساكن constexpr خودکار get_user_داده = get_user_داده_2;
};

#تعریف_کن HB_DEFINE_VTABLE(name, خالی_2) \
	قالب<> \
	بنية vtable<hb_##name##_t> \
	     : vtable_t<hb_##name##_t, \
			خالی_2, \
			&hb_##name##_reference, \
			&hb_##name##_destroy, \
			&hb_##name##_set_user_data, \
			&hb_##name##_get_user_data> {}

HB_DEFINE_VTABLE (میانگیر, &hb_میانگیر_get_خالی);
HB_DEFINE_VTABLE (blob, &hb_blob_get_خالی);
HB_DEFINE_VTABLE (face, &hb_face_get_خالی);
HB_DEFINE_VTABLE (font, &hb_font_get_خالی);
HB_DEFINE_VTABLE (font_funcs, &hb_font_funcs_get_خالی);
HB_DEFINE_VTABLE (map, &hb_map_get_خالی);
HB_DEFINE_VTABLE (مجموعه, &hb_مجموعه_get_خالی);
HB_DEFINE_VTABLE (shape_plan, &hb_shape_plan_get_خالی);
HB_DEFINE_VTABLE (unicode_funcs, &hb_unicode_funcs_get_خالی);
HB_DEFINE_VTABLE (draw_funcs, &hb_draw_funcs_get_خالی);
HB_DEFINE_VTABLE (paint_funcs, &hb_paint_funcs_get_خالی);


#اگر_تعریف_شده HB_SUBSET_H

HB_DEFINE_VTABLE (subset_درون_داد, اشاره_تهی);
HB_DEFINE_VTABLE (subset_plan, اشاره_تهی);

#پایان_اگر


#اگر_تعریف_شده HB_RASTER_H

HB_DEFINE_VTABLE (raster_image, اشاره_تهی);
HB_DEFINE_VTABLE (raster_draw, اشاره_تهی);
HB_DEFINE_VTABLE (raster_paint, اشاره_تهی);

#پایان_اگر


#اگر_تعریف_شده HB_وکتور_H

HB_DEFINE_VTABLE (وکتور_draw, اشاره_تهی);
HB_DEFINE_VTABLE (وکتور_paint, اشاره_تهی);

#پایان_اگر


#اگر_تعریف_شده HB_GPU_H

HB_DEFINE_VTABLE (gpu_draw, اشاره_تهی);

#پایان_اگر

/* HB_DEFINE_VTABLE stays available for subsystem headers' paired tail
 * blocks (see hb-raster.h, hb-vector.h, hb-gpu.h, hb-subset.h). */


} // namespace hb

/* Workaround for GCC < 7, see:
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
 * https://stackoverflow.com/a/25594741 */
نطاق std {


قالب<نام_نوع T>
بنية hash<hb::shared_ptr<T>>
{
    std::size_t عملگر()(ثابت hb::shared_ptr<T>& v) ثابت noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        أرجع h;
    }
};

قالب<نام_نوع T>
بنية hash<hb::unique_ptr<T>>
{
    std::size_t عملگر()(ثابت hb::unique_ptr<T>& v) ثابت noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        أرجع h;
    }
};


} // namespace std

#پایان_اگر /* __cplusplus */

#پایان_اگر /* HB_CPLUSPLUS_HH */
