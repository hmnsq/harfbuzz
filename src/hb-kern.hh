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

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-aat-layout-common.hh"
#અન્તર્ભાવ "hb-ot-layout-gpos-table.hh"


નામાકાશ OT {


template <typename Driver>
સંરચના hb_kern_machine_t
{
  hb_kern_machine_t (અચલ Driver &driver_,
		     તાર્કિક crossStream_ = ખોટું) :
		       driver (driver_),
		       crossStream (crossStream_) {}

  શૂન્ય kern (hb_font_t   *font,
	     hb_buffer_t *buffer,
	     hb_mask_t    kern_mask,
	     તાર્કિક         scale = સાચું) અચલ
  {
    જો (!buffer->message (font, "start kern"))
      પાછા;

    buffer->unsafe_to_concat ();
    OT::hb_ot_apply_context_t c (1, font, buffer, hb_blob_get_empty ());
    c.set_lookup_mask (kern_mask);
    c.set_lookup_props (OT::LookupFlag::IgnoreMarks);
    auto &skippy_iter = c.iter_આગત;

    તાર્કિક horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
    unsigned પૂર્ણાંક ગણતરી = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    hb_glyph_position_t *pos = buffer->pos;
    માટે (unsigned પૂર્ણાંક idx = 0; idx < ગણતરી;)
    {
      જો (!(info[idx].mask & kern_mask))
      {
	idx++;
	ચાલુ;
      }

      skippy_iter.reset_fast (idx);
      unsigned unsafe_to;
      જો (unlikely (!skippy_iter.next (&unsafe_to)))
      {
	idx = unsafe_to;
	ચાલુ;
      }

      unsigned પૂર્ણાંક i = idx;
      unsigned પૂર્ણાંક j = skippy_iter.idx;

      hb_position_t kern = driver.get_kerning (info[i].codepoint,
					       info[j].codepoint);


      જો (likely (!kern))
	જાઓ skip;

      જો (horizontal)
      {
	જો (scale)
	  kern = font->em_scale_x (kern);
	જો (crossStream)
	{
	  pos[j].y_offset = kern;
	  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	નહીંતર
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].x_advance = hb_saturate_add (pos[i].x_advance, kern1);
	  pos[j].x_advance = hb_saturate_add (pos[j].x_advance, kern2);
	  pos[j].x_offset = hb_saturate_add (pos[j].x_offset, kern2);
	}
      }
      નહીંતર
      {
	જો (scale)
	  kern = font->em_scale_y (kern);
	જો (crossStream)
	{
	  pos[j].x_offset = kern;
	  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	નહીંતર
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

    (શૂન્ય) buffer->message (font, "end kern");
  }

  અચલ Driver &driver;
  તાર્કિક crossStream;
};


} /* namespace OT */


#endif /* HB_KERN_HH */
