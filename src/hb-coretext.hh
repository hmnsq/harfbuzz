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


#جے_بنا_تعریف HB_CORETEXT_HH
#تعریف_کرو HB_CORETEXT_HH

#ضمن "hb.hh"

#ضمن "hb-coretext.h"

#ضمن "hb-aat-layout.hh"


HB_اندرونی CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, بنانشانی صحيح اشاریہ_2);

HB_اندرونی CGFontRef
create_cg_font (hb_blob_t *blob, بنانشانی صحيح اشاریہ_2);

HB_اندرونی CGFontRef
create_cg_font (hb_face_t *face);

HB_اندرونی CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_ناپ);


سانچا <قسم_ناں T>
بنية hb_cf_releaser_t
{
  ساكن فراغ release (T obj)
  {
    إذا (obj)
      CFRelease ((CFTypeRef) obj);
  }
};

سانچا <>
بنية hb_cf_releaser_t<CGFontRef>
{
  ساكن فراغ release (CGFontRef obj)
  {
    إذا (obj)
      CGFontRelease (obj);
  }
};

سانچا <>
بنية hb_cf_releaser_t<CGDataProviderRef>
{
  ساكن فراغ release (CGDataProviderRef obj)
  {
    إذا (obj)
      CGDataProviderRelease (obj);
  }
};

سانچا <>
بنية hb_cf_releaser_t<CGPathRef>
{
  ساكن فراغ release (CGPathRef obj)
  {
    إذا (obj)
      CGPathRelease (obj);
  }
};

سانچا <قسم_ناں T, قسم_ناں Releaser = hb_cf_releaser_t<T>>
بنية hb_cf_ptr_t
{
  ورتوں element_type = T;

  constexpr hb_cf_ptr_t () noexcept : p (خالی_سوچک) {}
  constexpr hb_cf_ptr_t (std::nullptr_t) noexcept : p (خالی_سوچک) {}
  واضح hb_cf_ptr_t (T p) noexcept : p (p) {}

  hb_cf_ptr_t (ثابت hb_cf_ptr_t &) = مٹاؤ;
  hb_cf_ptr_t &عامل = (ثابت hb_cf_ptr_t &) = مٹاؤ;

  hb_cf_ptr_t (hb_cf_ptr_t &&o) noexcept : p (o.release ()) {}
  hb_cf_ptr_t &عامل = (hb_cf_ptr_t &&o) noexcept
  {
    reset (o.release ());
    أرجع *هذا;
  }

  hb_cf_ptr_t &عامل = (std::nullptr_t) noexcept
  {
    reset ();
    أرجع *هذا;
  }

  ~hb_cf_ptr_t ()
  {
    Releaser::release (p);
    p = خالی_سوچک;
  }

  فراغ reset (T new_p = خالی_سوچک)
  {
    إذا (p != new_p)
    {
      T old_p = p;
      p = new_p;
      Releaser::release (old_p);
    }
  }

  T release () noexcept
  {
    T old_p = p;
    p = خالی_سوچک;
    أرجع old_p;
  }

  T get () ثابت noexcept { أرجع p; }

  عامل T () ثابت noexcept { أرجع p; }
  واضح عامل منطقي () ثابت noexcept { أرجع p != خالی_سوچک; }
  منطقي عامل ! () ثابت noexcept { أرجع !p; }

  فراغ swap (hb_cf_ptr_t &o) noexcept
  {
    T tmp = p;
    p = o.p;
    o.p = tmp;
  }
  یار فراغ swap (hb_cf_ptr_t &a, hb_cf_ptr_t &b) noexcept
  {
    a.swap (b);
  }

  خاص:
  T p;
};

#جے_انت /* HB_CORETEXT_HH */
