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

#اگر_غیر_تعریف_شدہ HB_KERN_HH
#تعریف_کرو HB_KERN_HH

#شامل "hb-open-type.hh"
#شامل "hb-aat-layout-common.hh"
#شامل "hb-ot-layout-gpos-table.hh"


نامخلا OT {


سانچہ <قسم_نام Driver>
ساخت hb_kern_machine_t
{
  hb_kern_machine_t (ثابت Driver &driver_,
		     منطقی cross_دھارا_2 = جھوٹ) :
		       driver (driver_),
		       cross_دھارا (cross_دھارا_2) {}

  خالی kern (hb_font_t   *font,
	     hb_بفر_t *بفر,
	     hb_نقاب_t    kern_نقاب,
	     منطقی         scale = سچ) ثابت
  {
    اگر (!بفر->پیغام (font, "start kern"))
      واپس;

    بفر->unsafe_مقصد_concat ();
    OT::hb_ot_apply_context_t c (1, font, بفر, hb_blob_get_خالی_خانے ());
    c.سیٹ_lookup_نقاب (kern_نقاب);
    c.سیٹ_lookup_props (OT::LookupFlag::IgnoreMarks);
    خودکار &skippy_iter = c.iter_داخلہ;

    منطقی horizontal = HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction);
    غیرعلامتی صحیح گنتی = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    hb_glyph_position_t *pos = بفر->pos;
    برائے (غیرعلامتی صحیح idx = 0; idx < گنتی;)
    {
      اگر (!(info[idx].نقاب & kern_نقاب))
      {
	idx++;
	جاری;
      }

      skippy_iter.reset_fast (idx);
      غیرعلامتی unsafe_مقصد;
      اگر (unlikely (!skippy_iter.next (&unsafe_مقصد)))
      {
	idx = unsafe_مقصد;
	جاری;
      }

      غیرعلامتی صحیح i = idx;
      غیرعلامتی صحیح j = skippy_iter.idx;

      hb_position_t kern = driver.get_kerning (info[i].codepoint,
					       info[j].codepoint);


      اگر (likely (!kern))
	جاؤ skip;

      اگر (horizontal)
      {
	اگر (scale)
	  kern = font->em_scale_x (kern);
	اگر (cross_دھارا)
	{
	  pos[j].y_انحراف_2 = kern;
	  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	ورنہ
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].x_advance = hb_saturate_جوڑو (pos[i].x_advance, kern1);
	  pos[j].x_advance = hb_saturate_جوڑو (pos[j].x_advance, kern2);
	  pos[j].x_انحراف_2 = hb_saturate_جوڑو (pos[j].x_انحراف_2, kern2);
	}
      }
      ورنہ
      {
	اگر (scale)
	  kern = font->em_scale_y (kern);
	اگر (cross_دھارا)
	{
	  pos[j].x_انحراف_2 = kern;
	  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	ورنہ
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].y_advance = hb_saturate_جوڑو (pos[i].y_advance, kern1);
	  pos[j].y_advance = hb_saturate_جوڑو (pos[j].y_advance, kern2);
	  pos[j].y_انحراف_2 = hb_saturate_جوڑو (pos[j].y_انحراف_2, kern2);
	}
      }

      بفر->unsafe_مقصد_break (i, j + 1);

    skip:
      idx = skippy_iter.idx;
    }

    (خالی) بفر->پیغام (font, "end kern");
  }

  ثابت Driver &driver;
  منطقی cross_دھارا;
};


} /* namespace OT */


#اگر_ختم /* HB_KERN_HH */
