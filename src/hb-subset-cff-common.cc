/*
 * Copyright © 2018 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF

#شامل "hb-ot-cff-common.hh"
#شامل "hb-ot-cff2-table.hh"
#شامل "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#تعریف_کرو CFF_SERIALIZE_FDSELECT_0  0

استعمال نامخلا CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

منطقی
hb_plan_subset_cff_fdselect (ثابت hb_subset_plan_t *plan,
			     غیرعلامتی صحیح fd_گنتی,
			     ثابت FDSelect &src, /*اندر*/
			     غیرعلامتی صحیح src_عدد_glyphs,
			     غیرعلامتی صحیح &subset_fd_گنتی /*برآمد*/,
			     غیرعلامتی صحیح &subset_fdselect_سائز /*برآمد*/,
			     غیرعلامتی صحیح &subset_fdselect_format /*برآمد*/,
			     hb_سمتیہ_t<رمز_pair_t> &fdselect_ranges /*برآمد*/,
			     hb_inc_bimap_t &fdmap /*برآمد*/)
{
  subset_fd_گنتی = 0;
  subset_fdselect_سائز = 0;
  subset_fdselect_format = 0;
  غیرعلامتی صحیح عدد_ranges = 0;

  غیرعلامتی صحیح subset_عدد_glyphs = plan->عدد_output_glyphs ();
  اگر (subset_عدد_glyphs == 0)
    واپس سچ;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_سیٹ_t سیٹ;
    hb_codepoint_t پچھلا_fd = CFF_UNDEF_رمز;
    خودکار it = hb_iter (plan->new_مقصد_old_gid_list);
    خودکار _ = *it;
    برائے (hb_codepoint_t gid = 0; gid < subset_عدد_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      اگر (gid == _.پہلا)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ورنہ
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<غیرعلامتی, hb_codepoint_t> fd_range =
	likely (old_glyph < src_عدد_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<غیرعلامتی, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      اگر (old_glyph < src_عدد_glyphs)
	fd_range.second = hb_min (fd_range.second, src_عدد_glyphs);
      غیرعلامتی fd = fd_range.پہلا;

      اگر (fd != پچھلا_fd)
      {
	سیٹ.جوڑو (fd);
	عدد_ranges++;
	پچھلا_fd = fd;
	fdselect_ranges.push (رمز_pair_t { fd, gid });

	اگر (gid == old_glyph)
	  gid = hb_min (_.پہلا - 1, fd_range.second - 1);
      }
    }

    subset_fd_گنتی = سیٹ.get_population ();
    اگر (subset_fd_گنتی == fd_گنتی)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.شناختی_عنصر (fd_گنتی);
    }
    ورنہ
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_رمز;
      جب (سیٹ.next (&fd))
	fdmap.جوڑو (fd);
      اگر (unlikely (fdmap.get_population () != subset_fd_گنتی))
	واپس جھوٹ;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    برائے (غیرعلامتی صحیح i = 0; i < fdselect_ranges.لمبائی; i++)
      fdselect_ranges[i].رمز = fdmap[fdselect_ranges[i].رمز];
  }

  /* determine which FDSelect format is most compact */
  اگر (subset_fd_گنتی > 0xFF)
  {
    اگر (unlikely (src.format != 4))
      واپس جھوٹ;
    subset_fdselect_format = 4;
    subset_fdselect_سائز = FDSelect::min_سائز + FDSelect4::min_سائز + FDSelect4_Range::static_سائز * عدد_ranges + HBUINT32::static_سائز;
  }
  ورنہ
  {
#اگر CFF_SERIALIZE_FDSELECT_0
    غیرعلامتی صحیح format0_سائز = FDSelect::min_سائز + FDSelect0::min_سائز + HBUINT8::static_سائز * subset_عدد_glyphs;
#اگر_ختم
    غیرعلامتی صحیح format3_سائز = FDSelect::min_سائز + FDSelect3::min_سائز + FDSelect3_Range::static_سائز * عدد_ranges + HBUINT16::static_سائز;

#اگر CFF_SERIALIZE_FDSELECT_0
    اگر (format0_سائز <= format3_سائز)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_سائز = format0_سائز;
    }
    ورنہ
#اگر_ختم
    {
      subset_fdselect_format = 3;
      subset_fdselect_سائز = format3_سائز;
    }
  }

  واپس سچ;
}

سانچہ <قسم_نام FDSELECT3_4>
ساکن اندرونی منطقی
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ثابت غیرعلامتی صحیح عدد_glyphs,
			ثابت FDSelect &src,
			غیرعلامتی صحیح سائز,
			ثابت hb_سمتیہ_t<رمز_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (یہ);
  FDSELECT3_4 *p = c->allocate_سائز<FDSELECT3_4> (سائز);
  اگر (unlikely (!p)) return_trace (جھوٹ);
  p->nRanges () = fdselect_ranges.لمبائی;
  برائے (غیرعلامتی صحیح i = 0; i < fdselect_ranges.لمبائی; i++)
  {
    p->ranges[i].پہلا = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].رمز;
  }
  p->sentinel () = عدد_glyphs;
  return_trace (سچ);
}

/* Serialize a subset FDSelect format planned above. */
منطقی
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ثابت غیرعلامتی صحیح عدد_glyphs,
			   ثابت FDSelect &src,
			   غیرعلامتی صحیح fd_گنتی_2,
			   غیرعلامتی صحیح fdselect_format,
			   غیرعلامتی صحیح سائز,
			   ثابت hb_سمتیہ_t<رمز_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (یہ);
  FDSelect *p = c->allocate_min<FDSelect> ();
  اگر (unlikely (!p)) return_trace (جھوٹ);
  p->format = fdselect_format;
  سائز -= FDSelect::min_سائز;

  انتخاب (fdselect_format)
  {
#اگر CFF_SERIALIZE_FDSELECT_0
  حالت 0:
  {
    FDSelect0 *p = c->allocate_سائز<FDSelect0> (سائز);
    اگر (unlikely (!p)) return_trace (جھوٹ);
    غیرعلامتی صحیح range_اشاریہ = 0;
    غیرعلامتی صحیح fd = fdselect_ranges[range_اشاریہ++].رمز;
    برائے (غیرعلامتی صحیح i = 0; i < عدد_glyphs; i++)
    {
      اگر ((range_اشاریہ < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_اشاریہ].glyph))
      {
	fd = fdselect_ranges[range_اشاریہ++].رمز;
      }
      p->fds[i] = fd;
    }
    return_trace (سچ);
  }
#اگر_ختم /* CFF_SERIALIZE_FDSELECT_0 */

  حالت 3:
    واپس serialize_fdselect_3_4<FDSelect3> (c, عدد_glyphs, src,
					      سائز, fdselect_ranges);

  حالت 4:
    واپس serialize_fdselect_3_4<FDSelect4> (c, عدد_glyphs, src,
					      سائز, fdselect_ranges);

  طےشدہ:
    return_trace (جھوٹ);
  }
}


#اگر_ختم
