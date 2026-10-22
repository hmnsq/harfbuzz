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


#నిర్వచితం_కాకపోతే HB_CORETEXT_HH
#నిర్వచించు HB_CORETEXT_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-coretext.h"

#అన్తర్భావ "hb-aat-layout.hh"


HB_అంతర్గత CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, గుర్తులేని పూర్ణాంకం సూచీ_2);

HB_అంతర్గత CGFontRef
create_cg_font (hb_blob_t *blob, గుర్తులేని పూర్ణాంకం సూచీ_2);

HB_అంతర్గత CGFontRef
create_cg_font (hb_face_t *face);

HB_అంతర్గత CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_కొలత);


మూస <రకం_పేరు T>
నిర్మాణం hb_cf_releaser_t
{
  స్థిర శూన్య release (T obj)
  {
    ఒకవేళ (obj)
      CFRelease ((CFTypeRef) obj);
  }
};

మూస <>
నిర్మాణం hb_cf_releaser_t<CGFontRef>
{
  స్థిర శూన్య release (CGFontRef obj)
  {
    ఒకవేళ (obj)
      CGFontRelease (obj);
  }
};

మూస <>
నిర్మాణం hb_cf_releaser_t<CGDataProviderRef>
{
  స్థిర శూన్య release (CGDataProviderRef obj)
  {
    ఒకవేళ (obj)
      CGDataProviderRelease (obj);
  }
};

మూస <>
నిర్మాణం hb_cf_releaser_t<CGPathRef>
{
  స్థిర శూన్య release (CGPathRef obj)
  {
    ఒకవేళ (obj)
      CGPathRelease (obj);
  }
};

మూస <రకం_పేరు T, రకం_పేరు Releaser = hb_cf_releaser_t<T>>
నిర్మాణం hb_cf_ptr_t
{
  ఉపయోగించి element_type = T;

  constexpr hb_cf_ptr_t () noexcept : p (శూన్యసూచిక) {}
  constexpr hb_cf_ptr_t (std::nullptr_t) noexcept : p (శూన్యసూచిక) {}
  స్పష్టమైన hb_cf_ptr_t (T p) noexcept : p (p) {}

  hb_cf_ptr_t (స్థిరరాశి hb_cf_ptr_t &) = తొలగించు;
  hb_cf_ptr_t &క్రియాకారకం = (స్థిరరాశి hb_cf_ptr_t &) = తొలగించు;

  hb_cf_ptr_t (hb_cf_ptr_t &&o) noexcept : p (o.release ()) {}
  hb_cf_ptr_t &క్రియాకారకం = (hb_cf_ptr_t &&o) noexcept
  {
    reset (o.release ());
    తిరిగి *ఇది;
  }

  hb_cf_ptr_t &క్రియాకారకం = (std::nullptr_t) noexcept
  {
    reset ();
    తిరిగి *ఇది;
  }

  ~hb_cf_ptr_t ()
  {
    Releaser::release (p);
    p = శూన్యసూచిక;
  }

  శూన్య reset (T new_p = శూన్యసూచిక)
  {
    ఒకవేళ (p != new_p)
    {
      T old_p = p;
      p = new_p;
      Releaser::release (old_p);
    }
  }

  T release () noexcept
  {
    T old_p = p;
    p = శూన్యసూచిక;
    తిరిగి old_p;
  }

  T get () స్థిరరాశి noexcept { తిరిగి p; }

  క్రియాకారకం T () స్థిరరాశి noexcept { తిరిగి p; }
  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి noexcept { తిరిగి p != శూన్యసూచిక; }
  తార్కిక క్రియాకారకం ! () స్థిరరాశి noexcept { తిరిగి !p; }

  శూన్య swap (hb_cf_ptr_t &o) noexcept
  {
    T tmp = p;
    p = o.p;
    o.p = tmp;
  }
  మిత్ర శూన్య swap (hb_cf_ptr_t &a, hb_cf_ptr_t &b) noexcept
  {
    a.swap (b);
  }

  అంతరంగ:
  T p;
};

#అయితే_ముగింపు /* HB_CORETEXT_HH */
