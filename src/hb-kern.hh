/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_KERN_HH
#define HB_KERN_HH

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-aat-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gpos-table.hh"


ನಾಮಾಕಾಶ OT {


template <typename Driver>
ಸಂರಚನಾ hb_kern_machine_t
{
  hb_kern_machine_t (ಧ್ರುವ Driver &driver_,
		     ತಾರ್ಕಿಕ crossStream_ = ಅಸತ್ಯ) :
		       driver (driver_),
		       crossStream (crossStream_) {}

  ಶೂನ್ಯ kern (hb_font_t   *font,
	     hb_buffer_t *buffer,
	     hb_mask_t    kern_mask,
	     ತಾರ್ಕಿಕ         scale = ಸತ್ಯ) ಧ್ರುವ
  {
    ಯದಿ (!buffer->message (font, "start kern"))
      ಪ್ರತ್ಯಾಗಮ;

    buffer->unsafe_to_concat ();
    OT::hb_ot_apply_context_t c (1, font, buffer, hb_blob_get_empty ());
    c.set_lookup_mask (kern_mask);
    c.set_lookup_props (OT::LookupFlag::IgnoreMarks);
    auto &skippy_iter = c.iter_ಒಳಹರಿವು;

    ತಾರ್ಕಿಕ horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    hb_glyph_position_t *pos = buffer->pos;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ idx = 0; idx < ಎಣಿಕೆ;)
    {
      ಯದಿ (!(info[idx].mask & kern_mask))
      {
	idx++;
	ಅನುವರ್ತ;
      }

      skippy_iter.reset_fast (idx);
      unsigned unsafe_to;
      ಯದಿ (unlikely (!skippy_iter.next (&unsafe_to)))
      {
	idx = unsafe_to;
	ಅನುವರ್ತ;
      }

      unsigned ಪೂರ್ಣಾಙ್ಕ i = idx;
      unsigned ಪೂರ್ಣಾಙ್ಕ j = skippy_iter.idx;

      hb_position_t kern = driver.get_kerning (info[i].codepoint,
					       info[j].codepoint);


      ಯದಿ (likely (!kern))
	ಗಚ್ಛ skip;

      ಯದಿ (horizontal)
      {
	ಯದಿ (scale)
	  kern = font->em_scale_x (kern);
	ಯದಿ (crossStream)
	{
	  pos[j].y_offset = kern;
	  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	ಅನ್ಯಥಾ
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].x_advance = hb_saturate_add (pos[i].x_advance, kern1);
	  pos[j].x_advance = hb_saturate_add (pos[j].x_advance, kern2);
	  pos[j].x_offset = hb_saturate_add (pos[j].x_offset, kern2);
	}
      }
      ಅನ್ಯಥಾ
      {
	ಯದಿ (scale)
	  kern = font->em_scale_y (kern);
	ಯದಿ (crossStream)
	{
	  pos[j].x_offset = kern;
	  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	ಅನ್ಯಥಾ
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].y_advance = hb_saturate_add (pos[i].y_advance, kern1);
	  pos[j].y_advance = hb_saturate_add (pos[j].y_advance, kern2);
	  pos[j].y_offset = hb_saturate_add (pos[j].y_offset, kern2);
	}
      }

      buffer->unsafe_to_break (i, j + 1);

    skip:
      idx = skippy_iter.idx;
    }

    (ಶೂನ್ಯ) buffer->message (font, "end kern");
  }

  ಧ್ರುವ Driver &driver;
  ತಾರ್ಕಿಕ crossStream;
};


} /* namespace OT */


#endif /* HB_KERN_HH */
