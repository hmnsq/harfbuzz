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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-paint.h"

#அந்தர்பாவ "hb-geometry.hh"


வகைவரை கட்டமைப்பு  hb_paint_bounded_context_t hb_paint_bounded_context_t;

கட்டமைப்பு hb_paint_bounded_context_t
{
  வெறுமை clear ()
  {
    clips = 0;
    bounded = உண்மை;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  தருக்க is_bounded ()
  {
    திரும்பு bounded;
  }

  வெறுமை தள்ளு_clip ()
  {
    clips++;
  }

  வெறுமை எடு_clip ()
  {
    எனில் (clips == 0) திரும்பு;
    clips--;
  }

  வெறுமை தள்ளு_group ()
  {
    groups.push (bounded);
    bounded = உண்மை;
  }

  வெறுமை எடு_group (hb_paint_composite_mode_t mode)
  {
    மாறிலி தருக்க src_bounded = bounded;
    bounded = groups.எடு ();
    தருக்க &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    தேர்வு ((முழு) mode)
    {
      நிலை HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = உண்மை;
	நிறுத்து;
      நிலை HB_PAINT_COMPOSITE_MODE_SRC:
      நிலை HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	நிறுத்து;
      நிலை HB_PAINT_COMPOSITE_MODE_DEST:
      நிலை HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	நிறுத்து;
      நிலை HB_PAINT_COMPOSITE_MODE_SRC_IN:
      நிலை HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	நிறுத்து;
      இயல்பு:
	backdrop_bounded = backdrop_bounded || src_bounded;
	நிறுத்து;
     }
  }

  வெறுமை paint ()
  {
    எனில் (!clips)
      bounded = பொய்;
  }

  பாதுகாக்கப்பட்ட:
  தருக்க bounded; // true if current drawing bounded
  unsigned clips; // number of active clips
  hb_vector_t<தருக்க> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#endif /* HB_PAINT_BOUNDED_HH */
