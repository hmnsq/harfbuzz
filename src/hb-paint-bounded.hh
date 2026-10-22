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

#ifndef HB_PAINT_BOUNDED_HH
#define HB_PAINT_BOUNDED_HH

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-paint.h"

#അന്തര്ഭാവ "hb-geometry.hh"


തരനിർവചനം ഘടന  hb_paint_bounded_context_t hb_paint_bounded_context_t;

ഘടന hb_paint_bounded_context_t
{
  ശൂന്യം clear ()
  {
    clips = 0;
    bounded = സത്യം;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  തർക്കം is_bounded ()
  {
    തിരികെ bounded;
  }

  ശൂന്യം തള്ളുക_clip ()
  {
    clips++;
  }

  ശൂന്യം എടുക്കുക_clip ()
  {
    എങ്കിൽ (clips == 0) തിരികെ;
    clips--;
  }

  ശൂന്യം തള്ളുക_group ()
  {
    groups.push (bounded);
    bounded = സത്യം;
  }

  ശൂന്യം എടുക്കുക_group (hb_paint_composite_mode_t mode)
  {
    സ്ഥിരം തർക്കം src_bounded = bounded;
    bounded = groups.എടുക്കുക ();
    തർക്കം &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    തിരഞ്ഞെടുപ്പ് ((പൂർണ്ണസംഖ്യ) mode)
    {
      അവസ്ഥ HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = സത്യം;
	നിർത്തുക;
      അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC:
      അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	നിർത്തുക;
      അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST:
      അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	നിർത്തുക;
      അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_IN:
      അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	നിർത്തുക;
      സ്ഥിരസ്ഥിതി:
	backdrop_bounded = backdrop_bounded || src_bounded;
	നിർത്തുക;
     }
  }

  ശൂന്യം paint ()
  {
    എങ്കിൽ (!clips)
      bounded = അസത്യം;
  }

  സംരക്ഷിത:
  തർക്കം bounded; // true if current drawing bounded
  unsigned clips; // number of active clips
  hb_vector_t<തർക്കം> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#endif /* HB_PAINT_BOUNDED_HH */
