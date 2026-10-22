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


#अगर_अघोषित HB_CORETEXT_HH
#घोषणा HB_CORETEXT_HH

#समावेश "hb.hh"

#समावेश "hb-coretext.h"

#समावेश "hb-aat-layout.hh"


HB_INTERNAL CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, अचिन्हित पूर्णांक सूचकांक_2);

HB_INTERNAL CGFontRef
create_cg_font (hb_blob_t *blob, अचिन्हित पूर्णांक सूचकांक_2);

HB_INTERNAL CGFontRef
create_cg_font (hb_face_t *face);

HB_INTERNAL CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_आकार);


ढाँचा <typename T>
काष्ठा hb_cf_releaser_t
{
  अटल व्योम release (T obj)
  {
    अगर (obj)
      CFRelease ((CFTypeRef) obj);
  }
};

ढाँचा <>
काष्ठा hb_cf_releaser_t<CGFontRef>
{
  अटल व्योम release (CGFontRef obj)
  {
    अगर (obj)
      CGFontRelease (obj);
  }
};

ढाँचा <>
काष्ठा hb_cf_releaser_t<CGDataProviderRef>
{
  अटल व्योम release (CGDataProviderRef obj)
  {
    अगर (obj)
      CGDataProviderRelease (obj);
  }
};

ढाँचा <>
काष्ठा hb_cf_releaser_t<CGPathRef>
{
  अटल व्योम release (CGPathRef obj)
  {
    अगर (obj)
      CGPathRelease (obj);
  }
};

ढाँचा <typename T, typename Releaser = hb_cf_releaser_t<T>>
काष्ठा hb_cf_ptr_t
{
  using element_type = T;

  constexpr hb_cf_ptr_t () noexcept : p (nullptr) {}
  constexpr hb_cf_ptr_t (std::nullptr_t) noexcept : p (nullptr) {}
  explicit hb_cf_ptr_t (T p) noexcept : p (p) {}

  hb_cf_ptr_t (स्थिर hb_cf_ptr_t &) = मिटाओ;
  hb_cf_ptr_t &चालक = (स्थिर hb_cf_ptr_t &) = मिटाओ;

  hb_cf_ptr_t (hb_cf_ptr_t &&o) noexcept : p (o.release ()) {}
  hb_cf_ptr_t &चालक = (hb_cf_ptr_t &&o) noexcept
  {
    reset (o.release ());
    वापस *यह;
  }

  hb_cf_ptr_t &चालक = (std::nullptr_t) noexcept
  {
    reset ();
    वापस *यह;
  }

  ~hb_cf_ptr_t ()
  {
    Releaser::release (p);
    p = nullptr;
  }

  व्योम reset (T new_p = nullptr)
  {
    अगर (p != new_p)
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
    वापस old_p;
  }

  T लो () स्थिर noexcept { वापस p; }

  चालक T () स्थिर noexcept { वापस p; }
  explicit चालक bool () स्थिर noexcept { वापस p != nullptr; }
  bool चालक ! () स्थिर noexcept { वापस !p; }

  व्योम swap (hb_cf_ptr_t &o) noexcept
  {
    T tmp = p;
    p = o.p;
    o.p = tmp;
  }
  मित्र व्योम swap (hb_cf_ptr_t &a, hb_cf_ptr_t &b) noexcept
  {
    a.swap (b);
  }

  निजी:
  T p;
};

#पूर्ण_अगर /* HB_CORETEXT_HH */
