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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_KERN_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_KERN_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gpos-table.hh"


ਨਾਮਾਕਾਸ਼ OT {


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Driver>
ਬਣਤਰ hb_kern_machine_t
{
  hb_kern_machine_t (ਅਚਲ Driver &driver_,
		     ਤਾਰਕਿਕ cross_ਧਾਰਾ_2 = ਝੂਠ) :
		       driver (driver_),
		       cross_ਧਾਰਾ (cross_ਧਾਰਾ_2) {}

  ਖਾਲੀ kern (hb_font_t   *font,
	     hb_ਬਫਰ_t *ਬਫਰ,
	     hb_ਨਕਾਬ_t    kern_ਨਕਾਬ,
	     ਤਾਰਕਿਕ         scale = ਸੱਚ) ਅਚਲ
  {
    ਜੇ (!ਬਫਰ->ਸੁਨੇਹਾ (font, "start kern"))
      ਵਾਪਸ;

    ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat ();
    OT::hb_ot_apply_context_t c (1, font, ਬਫਰ, hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ());
    c.ਸੈੱਟ_lookup_ਨਕਾਬ (kern_ਨਕਾਬ);
    c.ਸੈੱਟ_lookup_props (OT::LookupFlag::IgnoreMarks);
    ਸਵੈਚਾਲਿਤ &skippy_iter = c.iter_ਨਿਵੇਸ਼;

    ਤਾਰਕਿਕ horizontal = HB_DIRECTION_IS_HORIZONTAL (ਬਫਰ->props.direction);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
    hb_glyph_info_t *info = ਬਫਰ->info;
    hb_glyph_position_t *pos = ਬਫਰ->pos;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx = 0; idx < ਗਿਣਤੀ_2;)
    {
      ਜੇ (!(info[idx].ਨਕਾਬ & kern_ਨਕਾਬ))
      {
	idx++;
	ਜਾਰੀ;
      }

      skippy_iter.reset_fast (idx);
      ਬਿਨਾਨਿਸ਼ਾਨੀ unsafe_ਮੰਜ਼ਿਲ;
      ਜੇ (unlikely (!skippy_iter.next (&unsafe_ਮੰਜ਼ਿਲ)))
      {
	idx = unsafe_ਮੰਜ਼ਿਲ;
	ਜਾਰੀ;
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = idx;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = skippy_iter.idx;

      hb_position_t kern = driver.get_kerning (info[i].codepoint,
					       info[j].codepoint);


      ਜੇ (likely (!kern))
	ਜਾਓ skip;

      ਜੇ (horizontal)
      {
	ਜੇ (scale)
	  kern = font->em_scale_x (kern);
	ਜੇ (cross_ਧਾਰਾ)
	{
	  pos[j].y_ਵਿੱਥ_2 = kern;
	  ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	ਨਹੀਂਤਾਂ
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].x_advance = hb_saturate_ਜੋੜੋ (pos[i].x_advance, kern1);
	  pos[j].x_advance = hb_saturate_ਜੋੜੋ (pos[j].x_advance, kern2);
	  pos[j].x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[j].x_ਵਿੱਥ_2, kern2);
	}
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (scale)
	  kern = font->em_scale_y (kern);
	ਜੇ (cross_ਧਾਰਾ)
	{
	  pos[j].x_ਵਿੱਥ_2 = kern;
	  ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	}
	ਨਹੀਂਤਾਂ
	{
	  hb_position_t kern1 = kern >> 1;
	  hb_position_t kern2 = kern - kern1;
	  pos[i].y_advance = hb_saturate_ਜੋੜੋ (pos[i].y_advance, kern1);
	  pos[j].y_advance = hb_saturate_ਜੋੜੋ (pos[j].y_advance, kern2);
	  pos[j].y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[j].y_ਵਿੱਥ_2, kern2);
	}
      }

      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (i, j + 1);

    skip:
      idx = skippy_iter.idx;
    }

    (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end kern");
  }

  ਅਚਲ Driver &driver;
  ਤਾਰਕਿਕ cross_ਧਾਰਾ;
};


} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_KERN_HH */
