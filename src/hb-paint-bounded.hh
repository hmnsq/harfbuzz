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

#اگر_غیر_تعریف_شدہ HB_PAINT_BOUNDED_HH
#تعریف_کرو HB_PAINT_BOUNDED_HH

#شامل "hb.hh"
#شامل "hb-paint.h"

#شامل "hb-geometry.hh"


قسمتعریف ساخت  hb_paint_bounded_context_t hb_paint_bounded_context_t;

ساخت hb_paint_bounded_context_t
{
  خالی clear ()
  {
    clips = 0;
    bounded = سچ;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  منطقی is_bounded ()
  {
    واپس bounded;
  }

  خالی ڈالو_clip ()
  {
    clips++;
  }

  خالی نکالو_clip ()
  {
    اگر (clips == 0) واپس;
    clips--;
  }

  خالی ڈالو_گروہ ()
  {
    groups.push (bounded);
    bounded = سچ;
  }

  خالی نکالو_گروہ (hb_paint_مرکب_منوال_t منوال)
  {
    ثابت منطقی src_bounded = bounded;
    bounded = groups.نکالو ();
    منطقی &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    انتخاب ((صحیح) منوال)
    {
      حالت HB_PAINT_مرکب_منوال_CLEAR:
	backdrop_bounded = سچ;
	توڑو;
      حالت HB_PAINT_مرکب_منوال_SRC:
      حالت HB_PAINT_مرکب_منوال_SRC_برآمد:
	backdrop_bounded = src_bounded;
	توڑو;
      حالت HB_PAINT_مرکب_منوال_DEST:
      حالت HB_PAINT_مرکب_منوال_DEST_برآمد:
	توڑو;
      حالت HB_PAINT_مرکب_منوال_SRC_اندر:
      حالت HB_PAINT_مرکب_منوال_DEST_اندر:
	backdrop_bounded = backdrop_bounded && src_bounded;
	توڑو;
      طےشدہ:
	backdrop_bounded = backdrop_bounded || src_bounded;
	توڑو;
     }
  }

  خالی paint ()
  {
    اگر (!clips)
      bounded = جھوٹ;
  }

  محفوظ:
  منطقی bounded; // true if current drawing bounded
  غیرعلامتی clips; // number of active clips
  hb_سمتیہ_t<منطقی> groups; // true if group bounded
};

HB_اندرونی hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#اگر_ختم /* HB_PAINT_BOUNDED_HH */
