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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-aat-layout-common.hh"
#അന്തര്ഭാവ "hb-ot-layout-gpos-table.hh"


നാമാകാശ OT {


template <typename Driver>
ഘടന hb_kern_machine_t
{
  hb_kern_machine_t (സ്ഥിരം Driver &driver_,
		     തർക്കം crossStream_ = അസത്യം) :
		       driver (driver_),
		       crossStream (crossStream_) {}

  ശൂന്യം kern (hb_font_t   *font,
	     hb_buffer_t *buffer,
	     hb_mask_t    kern_mask,
	     തർക്കം         scale = സത്യം) സ്ഥിരം
  {
    എങ്കിൽ (!buffer->message (font, "start kern"))
      തിരികെ;

    buffer->unsafe_to_concat ();
    OT::hb_ot_apply_context_t c (1, font, buffer, hb_blob_get_empty ());
    c.set_lookup_mask (kern_mask);
    c.set_lookup_props (OT::LookupFlag::IgnoreMarks);
    auto &skippy_iter = c.iter_നിവേശം;

    തർക്കം horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    hb_glyph_position_t *pos = buffer->pos;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ idx = 0; idx < എണ്ണം;)
    {
      എങ്കിൽ (!(info[idx].mask & kern_mask))
      {
	idx++;
	തുടരുക;
      }

      skippy_iter.reset_fast (idx);
      unsigned unsafe_to;
      എങ്കിൽ (unlikely (!skippy_iter.next (&unsafe_to)))
      {
	idx = unsafe_to;
	തുടരുക;
      }

      unsigned പൂർണ്ണസംഖ്യ i = idx;
      unsigned പൂർണ്ണസംഖ്യ j = skippy_iter.idx;

      hb_position_t kern = driver.get_kerning (info[i].codepoint,
					       info[j].codepoint);


      എങ്കിൽ (likely (!kern))
	പോകുക skip;

      എങ്കിൽ (horizontal)
      {
	എങ്കിൽ (scale)
	  kern = font->em_scale_x (kern);
	എങ്കിൽ (crossStream)
	{
	  pos[j].y_offset = kern;
	  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	അല്ലെങ്കിൽ
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].x_advance = hb_saturate_add (pos[i].x_advance, kern1);
	  pos[j].x_advance = hb_saturate_add (pos[j].x_advance, kern2);
	  pos[j].x_offset = hb_saturate_add (pos[j].x_offset, kern2);
	}
      }
      അല്ലെങ്കിൽ
      {
	എങ്കിൽ (scale)
	  kern = font->em_scale_y (kern);
	എങ്കിൽ (crossStream)
	{
	  pos[j].x_offset = kern;
	  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	അല്ലെങ്കിൽ
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

    (ശൂന്യം) buffer->message (font, "end kern");
  }

  സ്ഥിരം Driver &driver;
  തർക്കം crossStream;
};


} /* namespace OT */


#endif /* HB_KERN_HH */
