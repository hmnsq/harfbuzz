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

#جے_بنا_تعریف HB_NO_SUBSET_CFF

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#تعریف_کرو CFF_SERIALIZE_FDSELECT_0  0

ورتوں نطاق CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

منطقي
hb_plan_subset_cff_fdselect (ثابت hb_subset_plan_t *plan,
			     بنانشانی صحيح fd_گنتی,
			     ثابت FDSelect &src, /*وچ*/
			     بنانشانی صحيح src_عدد_glyphs,
			     بنانشانی صحيح &subset_fd_گنتی /*باہرلا*/,
			     بنانشانی صحيح &subset_fdselect_ناپ /*باہرلا*/,
			     بنانشانی صحيح &subset_fdselect_format /*باہرلا*/,
			     hb_سمتیہ_t<کوڈ_pair_t> &fdselect_ranges /*باہرلا*/,
			     hb_inc_bimap_t &fdmap /*باہرلا*/)
{
  subset_fd_گنتی = 0;
  subset_fdselect_ناپ = 0;
  subset_fdselect_format = 0;
  بنانشانی صحيح عدد_ranges = 0;

  بنانشانی صحيح subset_عدد_glyphs = plan->عدد_output_glyphs ();
  إذا (subset_عدد_glyphs == 0)
    أرجع سچ;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_سیٹ_t سیٹ;
    hb_codepoint_t prev_fd = CFF_UNDEF_کوڈ;
    خودکار it = hb_iter (plan->new_منزل_old_gid_list);
    خودکار _ = *it;
    لكل (hb_codepoint_t gid = 0; gid < subset_عدد_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      إذا (gid == _.پہلا)
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
      hb_pair_t<بنانشانی, hb_codepoint_t> fd_range =
	likely (old_glyph < src_عدد_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<بنانشانی, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      إذا (old_glyph < src_عدد_glyphs)
	fd_range.second = hb_min (fd_range.second, src_عدد_glyphs);
      بنانشانی fd = fd_range.پہلا;

      إذا (fd != prev_fd)
      {
	سیٹ.جوڑو (fd);
	عدد_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (کوڈ_pair_t { fd, gid });

	إذا (gid == old_glyph)
	  gid = hb_min (_.پہلا - 1, fd_range.second - 1);
      }
    }

    subset_fd_گنتی = سیٹ.get_population ();
    إذا (subset_fd_گنتی == fd_گنتی)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.پچھان_تت (fd_گنتی);
    }
    وإلا
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_کوڈ;
      طالما (سیٹ.next (&fd))
	fdmap.جوڑو (fd);
      إذا (unlikely (fdmap.get_population () != subset_fd_گنتی))
	أرجع خطأ;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    لكل (بنانشانی صحيح i = 0; i < fdselect_ranges.لمائی; i++)
      fdselect_ranges[i].کوڈ = fdmap[fdselect_ranges[i].کوڈ];
  }

  /* determine which FDSelect format is most compact */
  إذا (subset_fd_گنتی > 0xFF)
  {
    إذا (unlikely (src.format != 4))
      أرجع خطأ;
    subset_fdselect_format = 4;
    subset_fdselect_ناپ = FDSelect::min_ناپ + FDSelect4::min_ناپ + FDSelect4_Range::static_ناپ * عدد_ranges + HBUINT32::static_ناپ;
  }
  وإلا
  {
#إذا CFF_SERIALIZE_FDSELECT_0
    بنانشانی صحيح format0_ناپ = FDSelect::min_ناپ + FDSelect0::min_ناپ + HBUINT8::static_ناپ * subset_عدد_glyphs;
#جے_انت
    بنانشانی صحيح format3_ناپ = FDSelect::min_ناپ + FDSelect3::min_ناپ + FDSelect3_Range::static_ناپ * عدد_ranges + HBUINT16::static_ناپ;

#إذا CFF_SERIALIZE_FDSELECT_0
    إذا (format0_ناپ <= format3_ناپ)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_ناپ = format0_ناپ;
    }
    وإلا
#جے_انت
    {
      subset_fdselect_format = 3;
      subset_fdselect_ناپ = format3_ناپ;
    }
  }

  أرجع سچ;
}

سانچا <قسم_ناں FDSELECT3_4>
ساكن مضمن منطقي
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ثابت بنانشانی صحيح عدد_glyphs,
			ثابت FDSelect &src,
			بنانشانی صحيح ناپ,
			ثابت hb_سمتیہ_t<کوڈ_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (هذا);
  FDSELECT3_4 *p = c->allocate_ناپ<FDSELECT3_4> (ناپ);
  إذا (unlikely (!p)) return_trace (خطأ);
  p->nRanges () = fdselect_ranges.لمائی;
  لكل (بنانشانی صحيح i = 0; i < fdselect_ranges.لمائی; i++)
  {
    p->ranges[i].پہلا = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].کوڈ;
  }
  p->sentinel () = عدد_glyphs;
  return_trace (سچ);
}

/* Serialize a subset FDSelect format planned above. */
منطقي
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ثابت بنانشانی صحيح عدد_glyphs,
			   ثابت FDSelect &src,
			   بنانشانی صحيح fd_گنتی_2,
			   بنانشانی صحيح fdselect_format,
			   بنانشانی صحيح ناپ,
			   ثابت hb_سمتیہ_t<کوڈ_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (هذا);
  FDSelect *p = c->allocate_min<FDSelect> ();
  إذا (unlikely (!p)) return_trace (خطأ);
  p->format = fdselect_format;
  ناپ -= FDSelect::min_ناپ;

  بدل (fdselect_format)
  {
#إذا CFF_SERIALIZE_FDSELECT_0
  حالة 0:
  {
    FDSelect0 *p = c->allocate_ناپ<FDSelect0> (ناپ);
    إذا (unlikely (!p)) return_trace (خطأ);
    بنانشانی صحيح range_اشاریہ = 0;
    بنانشانی صحيح fd = fdselect_ranges[range_اشاریہ++].کوڈ;
    لكل (بنانشانی صحيح i = 0; i < عدد_glyphs; i++)
    {
      إذا ((range_اشاریہ < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_اشاریہ].glyph))
      {
	fd = fdselect_ranges[range_اشاریہ++].کوڈ;
      }
      p->fds[i] = fd;
    }
    return_trace (سچ);
  }
#جے_انت /* CFF_SERIALIZE_FDSELECT_0 */

  حالة 3:
    أرجع serialize_fdselect_3_4<FDSelect3> (c, عدد_glyphs, src,
					      ناپ, fdselect_ranges);

  حالة 4:
    أرجع serialize_fdselect_3_4<FDSelect4> (c, عدد_glyphs, src,
					      ناپ, fdselect_ranges);

  افتراضي:
    return_trace (خطأ);
  }
}


#جے_انت
