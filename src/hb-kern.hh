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

#که_نه_تعریف_شوی HB_KERN_HH
#تعریف_کړه HB_KERN_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout-gpos-table.hh"


نطاق OT {


کينډۍ <ډول_نوم Driver>
بنية hb_kern_machine_t
{
  hb_kern_machine_t (ثابت Driver &driver_,
		     منطقي cross_بهیر_2 = خطأ) :
		       driver (driver_),
		       cross_بهیر (cross_بهیر_2) {}

  فراغ kern (hb_font_t   *font,
	     hb_بفر_t *بفر,
	     hb_ماسک_t    kern_ماسک,
	     منطقي         scale = ریښتیا) ثابت
  {
    إذا (!بفر->پیغام (font, "start kern"))
      أرجع;

    بفر->unsafe_منزل_concat ();
    OT::hb_ot_apply_context_t c (1, font, بفر, hb_blob_get_تشه ());
    c.سېټ_lookup_ماسک (kern_ماسک);
    c.سېټ_lookup_props (OT::LookupFlag::IgnoreMarks);
    خپلکاری &skippy_iter = c.iter_ننوت;

    منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction);
    بینښه صحيح شمېر = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    hb_glyph_position_t *pos = بفر->pos;
    لكل (بینښه صحيح idx = 0; idx < شمېر;)
    {
      إذا (!(info[idx].ماسک & kern_ماسک))
      {
	idx++;
	استمر;
      }

      skippy_iter.reset_fast (idx);
      بینښه unsafe_منزل;
      إذا (unlikely (!skippy_iter.next (&unsafe_منزل)))
      {
	idx = unsafe_منزل;
	استمر;
      }

      بینښه صحيح i = idx;
      بینښه صحيح j = skippy_iter.idx;

      hb_position_t kern = driver.get_kerning (info[i].codepoint,
					       info[j].codepoint);


      إذا (likely (!kern))
	اذهب skip;

      إذا (horizontal)
      {
	إذا (scale)
	  kern = font->em_scale_x (kern);
	إذا (cross_بهیر)
	{
	  pos[j].y_دننه_واټن_2 = kern;
	  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	وإلا
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].x_advance = hb_saturate_جمع_کړه (pos[i].x_advance, kern1);
	  pos[j].x_advance = hb_saturate_جمع_کړه (pos[j].x_advance, kern2);
	  pos[j].x_دننه_واټن_2 = hb_saturate_جمع_کړه (pos[j].x_دننه_واټن_2, kern2);
	}
      }
      وإلا
      {
	إذا (scale)
	  kern = font->em_scale_y (kern);
	إذا (cross_بهیر)
	{
	  pos[j].x_دننه_واټن_2 = kern;
	  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	وإلا
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].y_advance = hb_saturate_جمع_کړه (pos[i].y_advance, kern1);
	  pos[j].y_advance = hb_saturate_جمع_کړه (pos[j].y_advance, kern2);
	  pos[j].y_دننه_واټن_2 = hb_saturate_جمع_کړه (pos[j].y_دننه_واټن_2, kern2);
	}
      }

      بفر->unsafe_منزل_break (i, j + 1);

    skip:
      idx = skippy_iter.idx;
    }

    (فراغ) بفر->پیغام (font, "end kern");
  }

  ثابت Driver &driver;
  منطقي cross_بهیر;
};


} /* namespace OT */


#د_که_پای /* HB_KERN_HH */
