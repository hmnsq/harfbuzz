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


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CORETEXT_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CORETEXT_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-coretext.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout.hh"


HB_ਅੰਦਰੂਨੀ CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2);

HB_ਅੰਦਰੂਨੀ CGFontRef
create_cg_font (hb_blob_t *blob, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2);

HB_ਅੰਦਰੂਨੀ CGFontRef
create_cg_font (hb_face_t *face);

HB_ਅੰਦਰੂਨੀ CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_ਮਾਪ);


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_cf_releaser_t
{
  ਸਥਿਰ ਖਾਲੀ release (T obj)
  {
    ਜੇ (obj)
      CFRelease ((CFTypeRef) obj);
  }
};

ਸਾਂਚਾ <>
ਬਣਤਰ hb_cf_releaser_t<CGFontRef>
{
  ਸਥਿਰ ਖਾਲੀ release (CGFontRef obj)
  {
    ਜੇ (obj)
      CGFontRelease (obj);
  }
};

ਸਾਂਚਾ <>
ਬਣਤਰ hb_cf_releaser_t<CGDataProviderRef>
{
  ਸਥਿਰ ਖਾਲੀ release (CGDataProviderRef obj)
  {
    ਜੇ (obj)
      CGDataProviderRelease (obj);
  }
};

ਸਾਂਚਾ <>
ਬਣਤਰ hb_cf_releaser_t<CGPathRef>
{
  ਸਥਿਰ ਖਾਲੀ release (CGPathRef obj)
  {
    ਜੇ (obj)
      CGPathRelease (obj);
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Releaser = hb_cf_releaser_t<T>>
ਬਣਤਰ hb_cf_ptr_t
{
  ਵਰਤੋਂ element_type = T;

  constexpr hb_cf_ptr_t () noexcept : p (ਖਾਲੀ_ਸੂਚਕ) {}
  constexpr hb_cf_ptr_t (std::nullptr_t) noexcept : p (ਖਾਲੀ_ਸੂਚਕ) {}
  ਸਪਸ਼ਟ hb_cf_ptr_t (T p) noexcept : p (p) {}

  hb_cf_ptr_t (ਅਚਲ hb_cf_ptr_t &) = ਮਿਟਾਓ;
  hb_cf_ptr_t &ਸੰਚਾਲਕ = (ਅਚਲ hb_cf_ptr_t &) = ਮਿਟਾਓ;

  hb_cf_ptr_t (hb_cf_ptr_t &&o) noexcept : p (o.release ()) {}
  hb_cf_ptr_t &ਸੰਚਾਲਕ = (hb_cf_ptr_t &&o) noexcept
  {
    reset (o.release ());
    ਵਾਪਸ *ਇਹ;
  }

  hb_cf_ptr_t &ਸੰਚਾਲਕ = (std::nullptr_t) noexcept
  {
    reset ();
    ਵਾਪਸ *ਇਹ;
  }

  ~hb_cf_ptr_t ()
  {
    Releaser::release (p);
    p = ਖਾਲੀ_ਸੂਚਕ;
  }

  ਖਾਲੀ reset (T new_p = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਜੇ (p != new_p)
    {
      T old_p = p;
      p = new_p;
      Releaser::release (old_p);
    }
  }

  T release () noexcept
  {
    T old_p = p;
    p = ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ old_p;
  }

  T get () ਅਚਲ noexcept { ਵਾਪਸ p; }

  ਸੰਚਾਲਕ T () ਅਚਲ noexcept { ਵਾਪਸ p; }
  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ noexcept { ਵਾਪਸ p != ਖਾਲੀ_ਸੂਚਕ; }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ ! () ਅਚਲ noexcept { ਵਾਪਸ !p; }

  ਖਾਲੀ swap (hb_cf_ptr_t &o) noexcept
  {
    T tmp = p;
    p = o.p;
    o.p = tmp;
  }
  ਮਿੱਤਰ ਖਾਲੀ swap (hb_cf_ptr_t &a, hb_cf_ptr_t &b) noexcept
  {
    a.swap (b);
  }

  ਨਿਜੀ:
  T p;
};

#ਜੇ_ਅੰਤ /* HB_CORETEXT_HH */
