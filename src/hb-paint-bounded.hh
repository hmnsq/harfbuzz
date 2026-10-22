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

#अगर_अघोषित HB_PAINT_BOUNDED_HH
#घोषणा HB_PAINT_BOUNDED_HH

#समावेश "hb.hh"
#समावेश "hb-paint.h"

#समावेश "hb-geometry.hh"


प्रकार काष्ठा  hb_paint_bounded_context_t hb_paint_bounded_context_t;

काष्ठा hb_paint_bounded_context_t
{
  व्योम सफा ()
  {
    clips = 0;
    bounded = true;
    groups.सफा ();
  }

  hb_paint_bounded_context_t ()
  {
    सफा ();
  }

  bool is_bounded ()
  {
    वापस bounded;
  }

  व्योम धकेलो_clip ()
  {
    clips++;
  }

  व्योम निकालो_clip ()
  {
    अगर (clips == 0) वापस;
    clips--;
  }

  व्योम धकेलो_group ()
  {
    groups.push (bounded);
    bounded = true;
  }

  व्योम निकालो_group (hb_paint_composite_mode_t mode)
  {
    स्थिर bool src_bounded = bounded;
    bounded = groups.निकालो ();
    bool &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    चयन ((पूर्णांक) mode)
    {
      हाल HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = true;
	अवरोध;
      हाल HB_PAINT_COMPOSITE_MODE_SRC:
      हाल HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	अवरोध;
      हाल HB_PAINT_COMPOSITE_MODE_DEST:
      हाल HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	अवरोध;
      हाल HB_PAINT_COMPOSITE_MODE_SRC_IN:
      हाल HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	अवरोध;
      शेष:
	backdrop_bounded = backdrop_bounded || src_bounded;
	अवरोध;
     }
  }

  व्योम paint ()
  {
    अगर (!clips)
      bounded = false;
  }

  रक्षित:
  bool bounded; // true if current drawing bounded
  अचिन्हित clips; // number of active clips
  hb_vector_t<bool> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#पूर्ण_अगर /* HB_PAINT_BOUNDED_HH */
