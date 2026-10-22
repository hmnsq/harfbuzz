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

#अगर_अघोषित HB_KERN_HH
#घोषणा HB_KERN_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-aat-layout-common.hh"
#समावेश "hb-ot-layout-gpos-table.hh"


namespace OT {


ढाँचा <typename Driver>
काष्ठा hb_kern_machine_t
{
  hb_kern_machine_t (स्थिर Driver &driver_,
		     bool crossStream_ = false) :
		       driver (driver_),
		       crossStream (crossStream_) {}

  व्योम kern (hb_font_t   *font,
	     hb_बफर_t *बफर,
	     hb_mask_t    kern_mask,
	     bool         scale = true) स्थिर
  {
    अगर (!बफर->message (font, "start kern"))
      वापस;

    बफर->unsafe_to_concat ();
    OT::hb_ot_apply_context_t c (1, font, बफर, hb_blob_get_empty ());
    c.set_lookup_mask (kern_mask);
    c.set_lookup_props (OT::LookupFlag::IgnoreMarks);
    स्वतः &skippy_iter = c.iter_निवेश;

    bool horizontal = HB_DIRECTION_IS_HORIZONTAL (बफर->props.direction);
    अचिन्हित पूर्णांक गिनती = बफर->len;
    hb_glyph_info_t *info = बफर->info;
    hb_glyph_position_t *pos = बफर->pos;
    क्रम (अचिन्हित पूर्णांक idx = 0; idx < गिनती;)
    {
      अगर (!(info[idx].mask & kern_mask))
      {
	idx++;
	जारी;
      }

      skippy_iter.reset_fast (idx);
      अचिन्हित unsafe_to;
      अगर (unlikely (!skippy_iter.next (&unsafe_to)))
      {
	idx = unsafe_to;
	जारी;
      }

      अचिन्हित पूर्णांक i = idx;
      अचिन्हित पूर्णांक j = skippy_iter.idx;

      hb_position_t kern = driver.get_kerning (info[i].codepoint,
					       info[j].codepoint);


      अगर (likely (!kern))
	जाओ skip;

      अगर (horizontal)
      {
	अगर (scale)
	  kern = font->em_scale_x (kern);
	अगर (crossStream)
	{
	  pos[j].y_offset = kern;
	  बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
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
	अगर (scale)
	  kern = font->em_scale_y (kern);
	अगर (crossStream)
	{
	  pos[j].x_offset = kern;
	  बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
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

      बफर->unsafe_to_break (i, j + 1);

    skip:
      idx = skippy_iter.idx;
    }

    (व्योम) बफर->message (font, "end kern");
  }

  स्थिर Driver &driver;
  bool crossStream;
};


} /* namespace OT */


#पूर्ण_अगर /* HB_KERN_HH */
