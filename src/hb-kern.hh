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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-aat-layout-common.hh"
#अन्तर्भाव "hb-ot-layout-gpos-table.hh"


नामाकाश OT {


template <typename Driver>
संरचना hb_kern_machine_t
{
  hb_kern_machine_t (ध्रुव Driver &driver_,
		     तार्किक crossStream_ = असत्य) :
		       driver (driver_),
		       crossStream (crossStream_) {}

  शून्य kern (hb_font_t   *font,
	     hb_buffer_t *buffer,
	     hb_mask_t    kern_mask,
	     तार्किक         scale = सत्य) ध्रुव
  {
    यदि (!buffer->message (font, "start kern"))
      प्रत्यागम;

    buffer->unsafe_to_concat ();
    OT::hb_ot_apply_context_t c (1, font, buffer, hb_blob_get_empty ());
    c.set_lookup_mask (kern_mask);
    c.set_lookup_props (OT::LookupFlag::IgnoreMarks);
    auto &skippy_iter = c.iter_निवेश;

    तार्किक horizontal = HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction);
    unsigned पूर्णाङ्क गिनती = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    hb_glyph_position_t *pos = buffer->pos;
    प्रति (unsigned पूर्णाङ्क idx = 0; idx < गिनती;)
    {
      यदि (!(info[idx].mask & kern_mask))
      {
	idx++;
	अनुवर्त;
      }

      skippy_iter.reset_fast (idx);
      unsigned unsafe_to;
      यदि (unlikely (!skippy_iter.next (&unsafe_to)))
      {
	idx = unsafe_to;
	अनुवर्त;
      }

      unsigned पूर्णाङ्क i = idx;
      unsigned पूर्णाङ्क j = skippy_iter.idx;

      hb_position_t kern = driver.get_kerning (info[i].codepoint,
					       info[j].codepoint);


      यदि (likely (!kern))
	गच्छ skip;

      यदि (horizontal)
      {
	यदि (scale)
	  kern = font->em_scale_x (kern);
	यदि (crossStream)
	{
	  pos[j].y_offset = kern;
	  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	अन्यथा
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].x_advance = hb_saturate_add (pos[i].x_advance, kern1);
	  pos[j].x_advance = hb_saturate_add (pos[j].x_advance, kern2);
	  pos[j].x_offset = hb_saturate_add (pos[j].x_offset, kern2);
	}
      }
      अन्यथा
      {
	यदि (scale)
	  kern = font->em_scale_y (kern);
	यदि (crossStream)
	{
	  pos[j].x_offset = kern;
	  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	अन्यथा
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

    (शून्य) buffer->message (font, "end kern");
  }

  ध्रुव Driver &driver;
  तार्किक crossStream;
};


} /* namespace OT */


#endif /* HB_KERN_HH */
