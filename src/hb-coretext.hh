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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-coretext.h"

#अन्तर्भाव "hb-aat-layout.hh"


HB_INTERNAL CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, unsigned पूर्णाङ्क index);

HB_INTERNAL CGFontRef
create_cg_font (hb_blob_t *blob, unsigned पूर्णाङ्क index);

HB_INTERNAL CGFontRef
create_cg_font (hb_face_t *face);

HB_INTERNAL CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_size);


template <typename T>
संरचना hb_cf_releaser_t
{
  स्थिर शून्य release (T obj)
  {
    यदि (obj)
      CFRelease ((CFTypeRef) obj);
  }
};

template <>
संरचना hb_cf_releaser_t<CGFontRef>
{
  स्थिर शून्य release (CGFontRef obj)
  {
    यदि (obj)
      CGFontRelease (obj);
  }
};

template <>
संरचना hb_cf_releaser_t<CGDataProviderRef>
{
  स्थिर शून्य release (CGDataProviderRef obj)
  {
    यदि (obj)
      CGDataProviderRelease (obj);
  }
};

template <>
संरचना hb_cf_releaser_t<CGPathRef>
{
  स्थिर शून्य release (CGPathRef obj)
  {
    यदि (obj)
      CGPathRelease (obj);
  }
};

template <typename T, typename Releaser = hb_cf_releaser_t<T>>
संरचना hb_cf_ptr_t
{
  using element_type = T;

  constexpr hb_cf_ptr_t () noexcept : p (nullptr) {}
  constexpr hb_cf_ptr_t (std::nullptr_t) noexcept : p (nullptr) {}
  explicit hb_cf_ptr_t (T p) noexcept : p (p) {}

  hb_cf_ptr_t (ध्रुव hb_cf_ptr_t &) = delete;
  hb_cf_ptr_t &operator = (ध्रुव hb_cf_ptr_t &) = delete;

  hb_cf_ptr_t (hb_cf_ptr_t &&o) noexcept : p (o.release ()) {}
  hb_cf_ptr_t &operator = (hb_cf_ptr_t &&o) noexcept
  {
    reset (o.release ());
    प्रत्यागम *एतत्;
  }

  hb_cf_ptr_t &operator = (std::nullptr_t) noexcept
  {
    reset ();
    प्रत्यागम *एतत्;
  }

  ~hb_cf_ptr_t ()
  {
    Releaser::release (p);
    p = nullptr;
  }

  शून्य reset (T new_p = nullptr)
  {
    यदि (p != new_p)
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
    प्रत्यागम old_p;
  }

  T get () ध्रुव noexcept { प्रत्यागम p; }

  operator T () ध्रुव noexcept { प्रत्यागम p; }
  explicit operator तार्किक () ध्रुव noexcept { प्रत्यागम p != nullptr; }
  तार्किक operator ! () ध्रुव noexcept { प्रत्यागम !p; }

  शून्य swap (hb_cf_ptr_t &o) noexcept
  {
    T tmp = p;
    p = o.p;
    o.p = tmp;
  }
  friend शून्य swap (hb_cf_ptr_t &a, hb_cf_ptr_t &b) noexcept
  {
    a.swap (b);
  }

  गुप्त:
  T p;
};

#endif /* HB_CORETEXT_HH */
