/*
 * Copyright © 2012,2013  Mozilla Foundation.
 * Copyright © 2012,2013  Google, Inc.
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
 *
 * Mozilla Author(s): Jonathan Kew
 * Google Author(s): Behdad Esfahbod
 */


#اگر_غیر_تعریف_شدہ HB_CORETEXT_HH
#تعریف_کرو HB_CORETEXT_HH

#شامل "hb.hh"

#شامل "hb-coretext.h"

#شامل "hb-aat-layout.hh"


HB_اندرونی CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, غیرعلامتی صحیح اشاریہ_2);

HB_اندرونی CGFontRef
create_cg_font (hb_blob_t *blob, غیرعلامتی صحیح اشاریہ_2);

HB_اندرونی CGFontRef
create_cg_font (hb_face_t *face);

HB_اندرونی CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_سائز);


سانچہ <قسم_نام T>
ساخت hb_cf_releaser_t
{
  ساکن خالی release (T obj)
  {
    اگر (obj)
      CFRelease ((CFTypeRef) obj);
  }
};

سانچہ <>
ساخت hb_cf_releaser_t<CGFontRef>
{
  ساکن خالی release (CGFontRef obj)
  {
    اگر (obj)
      CGFontRelease (obj);
  }
};

سانچہ <>
ساخت hb_cf_releaser_t<CGDataProviderRef>
{
  ساکن خالی release (CGDataProviderRef obj)
  {
    اگر (obj)
      CGDataProviderRelease (obj);
  }
};

سانچہ <>
ساخت hb_cf_releaser_t<CGPathRef>
{
  ساکن خالی release (CGPathRef obj)
  {
    اگر (obj)
      CGPathRelease (obj);
  }
};

سانچہ <قسم_نام T, قسم_نام Releaser = hb_cf_releaser_t<T>>
ساخت hb_cf_ptr_t
{
  استعمال element_type = T;

  constexpr hb_cf_ptr_t () noexcept : p (خالی_اشاریہ) {}
  constexpr hb_cf_ptr_t (std::nullptr_t) noexcept : p (خالی_اشاریہ) {}
  صریح hb_cf_ptr_t (T p) noexcept : p (p) {}

  hb_cf_ptr_t (ثابت hb_cf_ptr_t &) = حذف;
  hb_cf_ptr_t &عامل = (ثابت hb_cf_ptr_t &) = حذف;

  hb_cf_ptr_t (hb_cf_ptr_t &&o) noexcept : p (o.release ()) {}
  hb_cf_ptr_t &عامل = (hb_cf_ptr_t &&o) noexcept
  {
    reset (o.release ());
    واپس *یہ;
  }

  hb_cf_ptr_t &عامل = (std::nullptr_t) noexcept
  {
    reset ();
    واپس *یہ;
  }

  ~hb_cf_ptr_t ()
  {
    Releaser::release (p);
    p = خالی_اشاریہ;
  }

  خالی reset (T new_p = خالی_اشاریہ)
  {
    اگر (p != new_p)
    {
      T old_p = p;
      p = new_p;
      Releaser::release (old_p);
    }
  }

  T release () noexcept
  {
    T old_p = p;
    p = خالی_اشاریہ;
    واپس old_p;
  }

  T get () ثابت noexcept { واپس p; }

  عامل T () ثابت noexcept { واپس p; }
  صریح عامل منطقی () ثابت noexcept { واپس p != خالی_اشاریہ; }
  منطقی عامل ! () ثابت noexcept { واپس !p; }

  خالی swap (hb_cf_ptr_t &o) noexcept
  {
    T tmp = p;
    p = o.p;
    o.p = tmp;
  }
  دوست خالی swap (hb_cf_ptr_t &a, hb_cf_ptr_t &b) noexcept
  {
    a.swap (b);
  }

  نجی:
  T p;
};

#اگر_ختم /* HB_CORETEXT_HH */
