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


#ifndef HB_CORETEXT_HH
#define HB_CORETEXT_HH

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-coretext.h"

#ଅନ୍ତର୍ଭାଵ "hb-aat-layout.hh"


HB_INTERNAL CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index);

HB_INTERNAL CGFontRef
create_cg_font (hb_blob_t *blob, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index);

HB_INTERNAL CGFontRef
create_cg_font (hb_face_t *face);

HB_INTERNAL CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_size);


template <typename T>
ସଂରଚନା hb_cf_releaser_t
{
  ସ୍ଥିର ଶୂନ୍ୟ release (T obj)
  {
    ଯଦି (obj)
      CFRelease ((CFTypeRef) obj);
  }
};

template <>
ସଂରଚନା hb_cf_releaser_t<CGFontRef>
{
  ସ୍ଥିର ଶୂନ୍ୟ release (CGFontRef obj)
  {
    ଯଦି (obj)
      CGFontRelease (obj);
  }
};

template <>
ସଂରଚନା hb_cf_releaser_t<CGDataProviderRef>
{
  ସ୍ଥିର ଶୂନ୍ୟ release (CGDataProviderRef obj)
  {
    ଯଦି (obj)
      CGDataProviderRelease (obj);
  }
};

template <>
ସଂରଚନା hb_cf_releaser_t<CGPathRef>
{
  ସ୍ଥିର ଶୂନ୍ୟ release (CGPathRef obj)
  {
    ଯଦି (obj)
      CGPathRelease (obj);
  }
};

template <typename T, typename Releaser = hb_cf_releaser_t<T>>
ସଂରଚନା hb_cf_ptr_t
{
  using element_type = T;

  constexpr hb_cf_ptr_t () noexcept : p (nullptr) {}
  constexpr hb_cf_ptr_t (std::nullptr_t) noexcept : p (nullptr) {}
  explicit hb_cf_ptr_t (T p) noexcept : p (p) {}

  hb_cf_ptr_t (ଅଚଳ hb_cf_ptr_t &) = delete;
  hb_cf_ptr_t &operator = (ଅଚଳ hb_cf_ptr_t &) = delete;

  hb_cf_ptr_t (hb_cf_ptr_t &&o) noexcept : p (o.release ()) {}
  hb_cf_ptr_t &operator = (hb_cf_ptr_t &&o) noexcept
  {
    reset (o.release ());
    ଫେରନ୍ତୁ *ଏହି;
  }

  hb_cf_ptr_t &operator = (std::nullptr_t) noexcept
  {
    reset ();
    ଫେରନ୍ତୁ *ଏହି;
  }

  ~hb_cf_ptr_t ()
  {
    Releaser::release (p);
    p = nullptr;
  }

  ଶୂନ୍ୟ reset (T new_p = nullptr)
  {
    ଯଦି (p != new_p)
    {
      T old_p = p;
      p = new_p;
      Releaser::release (old_p);
    }
  }

  T release () noexcept
  {
    T old_p = p;
    p = nullptr;
    ଫେରନ୍ତୁ old_p;
  }

  T get () ଅଚଳ noexcept { ଫେରନ୍ତୁ p; }

  operator T () ଅଚଳ noexcept { ଫେରନ୍ତୁ p; }
  explicit operator ତାର୍କିକ () ଅଚଳ noexcept { ଫେରନ୍ତୁ p != nullptr; }
  ତାର୍କିକ operator ! () ଅଚଳ noexcept { ଫେରନ୍ତୁ !p; }

  ଶୂନ୍ୟ swap (hb_cf_ptr_t &o) noexcept
  {
    T tmp = p;
    p = o.p;
    o.p = tmp;
  }
  friend ଶୂନ୍ୟ swap (hb_cf_ptr_t &a, hb_cf_ptr_t &b) noexcept
  {
    a.swap (b);
  }

  ବ୍ୟକ୍ତିଗତ:
  T p;
};

#endif /* HB_CORETEXT_HH */
