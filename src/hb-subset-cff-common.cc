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

#ضمن "hb.hh"

#که_نه_تعریف_شوی HB_NO_SUBSET_CFF

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#تعریف_کړه CFF_SERIALIZE_FDSELECT_0  0

کارول نطاق CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

منطقي
hb_plan_subset_cff_fdselect (ثابت hb_subset_plan_t *plan,
			     بینښه صحيح fd_شمېر,
			     ثابت FDSelect &src, /*دننه*/
			     بینښه صحيح src_عدد_glyphs,
			     بینښه صحيح &subset_fd_شمېر /*بهر*/,
			     بینښه صحيح &subset_fdselect_اندازه /*بهر*/,
			     بینښه صحيح &subset_fdselect_format /*بهر*/,
			     hb_ویکټور_t<کوډ_pair_t> &fdselect_ranges /*بهر*/,
			     hb_inc_bimap_t &fdmap /*بهر*/)
{
  subset_fd_شمېر = 0;
  subset_fdselect_اندازه = 0;
  subset_fdselect_format = 0;
  بینښه صحيح عدد_ranges = 0;

  بینښه صحيح subset_عدد_glyphs = plan->عدد_output_glyphs ();
  إذا (subset_عدد_glyphs == 0)
    أرجع ریښتیا;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_سېټ_t سېټ;
    hb_codepoint_t prev_fd = CFF_UNDEF_کوډ;
    خپلکاری it = hb_iter (plan->new_منزل_old_gid_list);
    خپلکاری _ = *it;
    لكل (hb_codepoint_t gid = 0; gid < subset_عدد_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      إذا (gid == _.لومړی)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وإلا
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<بینښه, hb_codepoint_t> fd_range =
	likely (old_glyph < src_عدد_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<بینښه, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      إذا (old_glyph < src_عدد_glyphs)
	fd_range.second = hb_min (fd_range.second, src_عدد_glyphs);
      بینښه fd = fd_range.لومړی;

      إذا (fd != prev_fd)
      {
	سېټ.جمع_کړه (fd);
	عدد_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (کوډ_pair_t { fd, gid });

	إذا (gid == old_glyph)
	  gid = hb_min (_.لومړی - 1, fd_range.second - 1);
      }
    }

    subset_fd_شمېر = سېټ.get_population ();
    إذا (subset_fd_شمېر == fd_شمېر)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.بې_اغېزه_عنصر (fd_شمېر);
    }
    وإلا
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_کوډ;
      طالما (سېټ.next (&fd))
	fdmap.جمع_کړه (fd);
      إذا (unlikely (fdmap.get_population () != subset_fd_شمېر))
	أرجع خطأ;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    لكل (بینښه صحيح i = 0; i < fdselect_ranges.اوږدتیا; i++)
      fdselect_ranges[i].کوډ = fdmap[fdselect_ranges[i].کوډ];
  }

  /* determine which FDSelect format is most compact */
  إذا (subset_fd_شمېر > 0xFF)
  {
    إذا (unlikely (src.format != 4))
      أرجع خطأ;
    subset_fdselect_format = 4;
    subset_fdselect_اندازه = FDSelect::min_اندازه + FDSelect4::min_اندازه + FDSelect4_Range::static_اندازه * عدد_ranges + HBUINT32::static_اندازه;
  }
  وإلا
  {
#إذا CFF_SERIALIZE_FDSELECT_0
    بینښه صحيح format0_اندازه = FDSelect::min_اندازه + FDSelect0::min_اندازه + HBUINT8::static_اندازه * subset_عدد_glyphs;
#د_که_پای
    بینښه صحيح format3_اندازه = FDSelect::min_اندازه + FDSelect3::min_اندازه + FDSelect3_Range::static_اندازه * عدد_ranges + HBUINT16::static_اندازه;

#إذا CFF_SERIALIZE_FDSELECT_0
    إذا (format0_اندازه <= format3_اندازه)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_اندازه = format0_اندازه;
    }
    وإلا
#د_که_پای
    {
      subset_fdselect_format = 3;
      subset_fdselect_اندازه = format3_اندازه;
    }
  }

  أرجع ریښتیا;
}

کينډۍ <ډول_نوم FDSELECT3_4>
ساكن مضمن منطقي
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ثابت بینښه صحيح عدد_glyphs,
			ثابت FDSelect &src,
			بینښه صحيح اندازه,
			ثابت hb_ویکټور_t<کوډ_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (هذا);
  FDSELECT3_4 *p = c->allocate_اندازه<FDSELECT3_4> (اندازه);
  إذا (unlikely (!p)) return_trace (خطأ);
  p->nRanges () = fdselect_ranges.اوږدتیا;
  لكل (بینښه صحيح i = 0; i < fdselect_ranges.اوږدتیا; i++)
  {
    p->ranges[i].لومړی = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].کوډ;
  }
  p->sentinel () = عدد_glyphs;
  return_trace (ریښتیا);
}

/* Serialize a subset FDSelect format planned above. */
منطقي
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ثابت بینښه صحيح عدد_glyphs,
			   ثابت FDSelect &src,
			   بینښه صحيح fd_شمېر_2,
			   بینښه صحيح fdselect_format,
			   بینښه صحيح اندازه,
			   ثابت hb_ویکټور_t<کوډ_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (هذا);
  FDSelect *p = c->allocate_min<FDSelect> ();
  إذا (unlikely (!p)) return_trace (خطأ);
  p->format = fdselect_format;
  اندازه -= FDSelect::min_اندازه;

  بدل (fdselect_format)
  {
#إذا CFF_SERIALIZE_FDSELECT_0
  حالة 0:
  {
    FDSelect0 *p = c->allocate_اندازه<FDSelect0> (اندازه);
    إذا (unlikely (!p)) return_trace (خطأ);
    بینښه صحيح range_شاخص = 0;
    بینښه صحيح fd = fdselect_ranges[range_شاخص++].کوډ;
    لكل (بینښه صحيح i = 0; i < عدد_glyphs; i++)
    {
      إذا ((range_شاخص < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_شاخص].glyph))
      {
	fd = fdselect_ranges[range_شاخص++].کوډ;
      }
      p->fds[i] = fd;
    }
    return_trace (ریښتیا);
  }
#د_که_پای /* CFF_SERIALIZE_FDSELECT_0 */

  حالة 3:
    أرجع serialize_fdselect_3_4<FDSelect3> (c, عدد_glyphs, src,
					      اندازه, fdselect_ranges);

  حالة 4:
    أرجع serialize_fdselect_3_4<FDSelect4> (c, عدد_glyphs, src,
					      اندازه, fdselect_ranges);

  افتراضي:
    return_trace (خطأ);
  }
}


#د_که_پای
