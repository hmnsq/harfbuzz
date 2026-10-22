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

#که_نه_تعریف_شوی HB_CPLUSPLUS_HH
#تعریف_کړه HB_CPLUSPLUS_HH

#ضمن "hb.h"

#که_تعریف_شوی __cplusplus

#ضمن <functional>
#ضمن <utility>

#إذا 0
#إذا !(__cplusplus >= 201103L)
#تېروتنه "HarfBuzz C++ helpers require C++11"
#د_که_پای
#د_که_پای

نطاق hb {


کينډۍ <ډول_نوم T>
بنية vtable;

کينډۍ <ډول_نوم T>
بنية shared_ptr
{
  کارول element_type = T;

  کارول v = vtable<T>;

  څرګند shared_ptr (T *p = تش_اشاره) : p (p) {}
  shared_ptr (ثابت shared_ptr &o) : p (v::reference (o.p)) {}
  shared_ptr (shared_ptr &&o)  noexcept : p (o.p) { o.p = تش_اشاره; }
  shared_ptr& چلوونکی = (ثابت shared_ptr &o) { إذا (p != o.p) { destroy (); p = o.p; reference (); } أرجع *هذا; }
  shared_ptr& چلوونکی = (shared_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = تش_اشاره; أرجع *هذا; }
  ~shared_ptr () { v::destroy (p); p = تش_اشاره; }

  T* get() ثابت { أرجع p; }

  فراغ swap (shared_ptr &o)  noexcept { std::swap (p, o.p); }
  ملګری فراغ swap (shared_ptr &a, shared_ptr &b)  noexcept { std::swap (a.p, b.p); }

  چلوونکی T * () ثابت { أرجع p; }
  T& چلوونکی * () ثابت { أرجع *get (); }
  T* چلوونکی -> () ثابت { أرجع get (); }
  چلوونکی منطقي () ثابت { أرجع p; }
  منطقي چلوونکی == (ثابت shared_ptr &o) ثابت { أرجع p == o.p; }
  منطقي چلوونکی != (ثابت shared_ptr &o) ثابت { أرجع p != o.p; }

  ساكن T* get_تشه() { أرجع v::get_تشه (); }
  T* reference() { أرجع v::reference (p); }
  فراغ destroy() { v::destroy (p); }
  فراغ سېټ_user_معلومات (hb_user_معلومات_کلی_t *key,
		      فراغ *value,
		      hb_destroy_func_t destroy,
		      hb_bool_t replace) { v::سېټ_user_معلومات (p, key, value, destroy, replace); } 
  فراغ * get_user_معلومات (hb_user_معلومات_کلی_t *key) { أرجع v::get_user_معلومات (p, key); }

  خاص:
  T *p;
};

کينډۍ<ډول_نوم T> بنية is_shared_ptr : std::false_type {};
کينډۍ<ډول_نوم T> بنية is_shared_ptr<shared_ptr<T>> : std::true_type {};

کينډۍ <ډول_نوم T>
بنية unique_ptr
{
  کارول element_type = T;

  کارول v = vtable<T>;

  څرګند unique_ptr (T *p = تش_اشاره) : p (p) {}
  unique_ptr (ثابت unique_ptr &o) = ړنګول;
  unique_ptr (unique_ptr &&o)  noexcept : p (o.p) { o.p = تش_اشاره; }
  unique_ptr& چلوونکی = (ثابت unique_ptr &o) = ړنګول;
  unique_ptr& چلوونکی = (unique_ptr &&o)  noexcept { v::destroy (p); p = o.p; o.p = تش_اشاره; أرجع *هذا; }
  ~unique_ptr () { v::destroy (p); p = تش_اشاره; }

  T* get() ثابت { أرجع p; }
  T* release () { T* v = p; p = تش_اشاره; أرجع v; }

  فراغ swap (unique_ptr &o)  noexcept { std::swap (p, o.p); }
  ملګری فراغ swap (unique_ptr &a, unique_ptr &b)  noexcept { std::swap (a.p, b.p); }

  چلوونکی T * () ثابت { أرجع p; }
  T& چلوونکی * () ثابت { أرجع *get (); }
  T* چلوونکی -> () ثابت { أرجع get (); }
  چلوونکی منطقي () { أرجع p; }

  خاص:
  T *p;
};

کينډۍ<ډول_نوم T> بنية is_unique_ptr : std::false_type {};
کينډۍ<ډول_نوم T> بنية is_unique_ptr<unique_ptr<T>> : std::true_type {};

کينډۍ <ډول_نوم T,
	  T * (*get_تشه_2) (فراغ),
	  T * (*_reference) (T *),
	  فراغ (*_destroy) (T *),
	  hb_bool_t (*سېټ_user_معلومات_2) (T *,
				       hb_user_معلومات_کلی_t *,
				       فراغ *,
				       hb_destroy_func_t,
				       hb_bool_t),
	  فراغ * (*get_user_معلومات_2) (ثابت T *,
				    hb_user_معلومات_کلی_t *)>
بنية vtable_t
{
  ساكن constexpr خپلکاری get_تشه = get_تشه_2;
  ساكن constexpr خپلکاری reference = _reference;
  ساكن constexpr خپلکاری destroy = _destroy;
  ساكن constexpr خپلکاری سېټ_user_معلومات = سېټ_user_معلومات_2;
  ساكن constexpr خپلکاری get_user_معلومات = get_user_معلومات_2;
};

#تعریف_کړه HB_DEFINE_VTABLE(name, تشه_2) \
	کينډۍ<> \
	بنية vtable<hb_##name##_t> \
	     : vtable_t<hb_##name##_t, \
			تشه_2, \
			&hb_##name##_reference, \
			&hb_##name##_destroy, \
			&hb_##name##_set_user_data, \
			&hb_##name##_get_user_data> {}

HB_DEFINE_VTABLE (بفر, &hb_بفر_get_تشه);
HB_DEFINE_VTABLE (blob, &hb_blob_get_تشه);
HB_DEFINE_VTABLE (face, &hb_face_get_تشه);
HB_DEFINE_VTABLE (font, &hb_font_get_تشه);
HB_DEFINE_VTABLE (font_funcs, &hb_font_funcs_get_تشه);
HB_DEFINE_VTABLE (map, &hb_map_get_تشه);
HB_DEFINE_VTABLE (سېټ, &hb_سېټ_get_تشه);
HB_DEFINE_VTABLE (shape_plan, &hb_shape_plan_get_تشه);
HB_DEFINE_VTABLE (unicode_funcs, &hb_unicode_funcs_get_تشه);
HB_DEFINE_VTABLE (draw_funcs, &hb_draw_funcs_get_تشه);
HB_DEFINE_VTABLE (paint_funcs, &hb_paint_funcs_get_تشه);


#که_تعریف_شوی HB_SUBSET_H

HB_DEFINE_VTABLE (subset_ننوت, تش_اشاره);
HB_DEFINE_VTABLE (subset_plan, تش_اشاره);

#د_که_پای


#که_تعریف_شوی HB_RASTER_H

HB_DEFINE_VTABLE (raster_image, تش_اشاره);
HB_DEFINE_VTABLE (raster_draw, تش_اشاره);
HB_DEFINE_VTABLE (raster_paint, تش_اشاره);

#د_که_پای


#که_تعریف_شوی HB_ویکټور_H

HB_DEFINE_VTABLE (ویکټور_draw, تش_اشاره);
HB_DEFINE_VTABLE (ویکټور_paint, تش_اشاره);

#د_که_پای


#که_تعریف_شوی HB_GPU_H

HB_DEFINE_VTABLE (gpu_draw, تش_اشاره);

#د_که_پای

/* HB_DEFINE_VTABLE stays available for subsystem headers' paired tail
 * blocks (see hb-raster.h, hb-vector.h, hb-gpu.h, hb-subset.h). */


} // namespace hb

/* Workaround for GCC < 7, see:
 * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
 * https://stackoverflow.com/a/25594741 */
نطاق std {


کينډۍ<ډول_نوم T>
بنية hash<hb::shared_ptr<T>>
{
    std::size_t چلوونکی()(ثابت hb::shared_ptr<T>& v) ثابت noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        أرجع h;
    }
};

کينډۍ<ډول_نوم T>
بنية hash<hb::unique_ptr<T>>
{
    std::size_t چلوونکی()(ثابت hb::unique_ptr<T>& v) ثابت noexcept
    {
        std::size_t h = std::hash<decltype (v.get ())>{}(v.get ());
        أرجع h;
    }
};


} // namespace std

#د_که_پای /* __cplusplus */

#د_که_پای /* HB_CPLUSPLUS_HH */
