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

#నిర్వచితం_కాకపోతే HB_KERN_HH
#నిర్వచించు HB_KERN_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-aat-layout-common.hh"
#అన్తర్భావ "hb-ot-layout-gpos-table.hh"


నామాకాశ OT {


మూస <రకం_పేరు Driver>
నిర్మాణం hb_kern_machine_t
{
  hb_kern_machine_t (స్థిరరాశి Driver &driver_,
		     తార్కిక cross_ప్రవాహం_2 = అబద్ధం) :
		       driver (driver_),
		       cross_ప్రవాహం (cross_ప్రవాహం_2) {}

  శూన్య kern (hb_font_t   *font,
	     hb_బఫరు_t *బఫరు,
	     hb_ముసుగు_t    kern_ముసుగు,
	     తార్కిక         scale = నిజం) స్థిరరాశి
  {
    ఒకవేళ (!బఫరు->సందేశం (font, "start kern"))
      తిరిగి;

    బఫరు->unsafe_గమ్యం_concat ();
    OT::hb_ot_apply_context_t c (1, font, బఫరు, hb_blob_get_ఖాళీలు ());
    c.సమితి_lookup_ముసుగు (kern_ముసుగు);
    c.సమితి_lookup_props (OT::LookupFlag::IgnoreMarks);
    స్వయంచాలక &skippy_iter = c.iter_ఆదానం;

    తార్కిక horizontal = HB_DIRECTION_IS_HORIZONTAL (బఫరు->props.direction);
    గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
    hb_glyph_info_t *info = బఫరు->info;
    hb_glyph_position_t *pos = బఫరు->pos;
    కొరకు (గుర్తులేని పూర్ణాంకం idx = 0; idx < లెక్క;)
    {
      ఒకవేళ (!(info[idx].ముసుగు & kern_ముసుగు))
      {
	idx++;
	కొనసాగు;
      }

      skippy_iter.reset_fast (idx);
      గుర్తులేని unsafe_గమ్యం;
      ఒకవేళ (unlikely (!skippy_iter.next (&unsafe_గమ్యం)))
      {
	idx = unsafe_గమ్యం;
	కొనసాగు;
      }

      గుర్తులేని పూర్ణాంకం i = idx;
      గుర్తులేని పూర్ణాంకం j = skippy_iter.idx;

      hb_position_t kern = driver.get_kerning (info[i].codepoint,
					       info[j].codepoint);


      ఒకవేళ (likely (!kern))
	వెళ్ళు skip;

      ఒకవేళ (horizontal)
      {
	ఒకవేళ (scale)
	  kern = font->em_scale_x (kern);
	ఒకవేళ (cross_ప్రవాహం)
	{
	  pos[j].y_స్థానభ్రంశం_2 = kern;
	  బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	లేకపోతే
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].x_advance = hb_saturate_కూడు (pos[i].x_advance, kern1);
	  pos[j].x_advance = hb_saturate_కూడు (pos[j].x_advance, kern2);
	  pos[j].x_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[j].x_స్థానభ్రంశం_2, kern2);
	}
      }
      లేకపోతే
      {
	ఒకవేళ (scale)
	  kern = font->em_scale_y (kern);
	ఒకవేళ (cross_ప్రవాహం)
	{
	  pos[j].x_స్థానభ్రంశం_2 = kern;
	  బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	లేకపోతే
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].y_advance = hb_saturate_కూడు (pos[i].y_advance, kern1);
	  pos[j].y_advance = hb_saturate_కూడు (pos[j].y_advance, kern2);
	  pos[j].y_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[j].y_స్థానభ్రంశం_2, kern2);
	}
      }

      బఫరు->unsafe_గమ్యం_break (i, j + 1);

    skip:
      idx = skippy_iter.idx;
    }

    (శూన్య) బఫరు->సందేశం (font, "end kern");
  }

  స్థిరరాశి Driver &driver;
  తార్కిక cross_ప్రవాహం;
};


} /* namespace OT */


#అయితే_ముగింపు /* HB_KERN_HH */
