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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-paint.h"

#ଅନ୍ତର୍ଭାଵ "hb-geometry.hh"


ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା  hb_paint_bounded_context_t hb_paint_bounded_context_t;

ସଂରଚନା hb_paint_bounded_context_t
{
  ଶୂନ୍ୟ clear ()
  {
    clips = 0;
    bounded = ସତ୍ୟ;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  ତାର୍କିକ is_bounded ()
  {
    ଫେରନ୍ତୁ bounded;
  }

  ଶୂନ୍ୟ ଠେଲ_clip ()
  {
    clips++;
  }

  ଶୂନ୍ୟ କାଢ଼_clip ()
  {
    ଯଦି (clips == 0) ଫେରନ୍ତୁ;
    clips--;
  }

  ଶୂନ୍ୟ ଠେଲ_group ()
  {
    groups.push (bounded);
    bounded = ସତ୍ୟ;
  }

  ଶୂନ୍ୟ କାଢ଼_group (hb_paint_composite_mode_t mode)
  {
    ଅଚଳ ତାର୍କିକ src_bounded = bounded;
    bounded = groups.କାଢ଼ ();
    ତାର୍କିକ &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    ବାଛ ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mode)
    {
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = ସତ୍ୟ;
	ଭାଙ୍ଗ;
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	ଭାଙ୍ଗ;
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	ଭାଙ୍ଗ;
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_SRC_IN:
      ସ୍ଥିତି HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	ଭାଙ୍ଗ;
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	backdrop_bounded = backdrop_bounded || src_bounded;
	ଭାଙ୍ଗ;
     }
  }

  ଶୂନ୍ୟ paint ()
  {
    ଯଦି (!clips)
      bounded = ମିଥ୍ୟା;
  }

  ସୁରକ୍ଷିତ:
  ତାର୍କିକ bounded; // true if current drawing bounded
  unsigned clips; // number of active clips
  hb_vector_t<ତାର୍କିକ> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#endif /* HB_PAINT_BOUNDED_HH */
