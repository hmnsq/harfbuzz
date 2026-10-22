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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_CFF

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#وصف_ڪريو CFF_SERIALIZE_FDSELECT_0  0

استعمال نطاق CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

منطقي
hb_plan_subset_cff_fdselect (ثابت hb_subset_plan_t *plan,
			     بنانشان صحيح fd_ڳڻپ,
			     ثابت FDSelect &src, /*اندريون*/
			     بنانشان صحيح src_انگ_glyphs,
			     بنانشان صحيح &subset_fd_ڳڻپ /*ٻاهريون*/,
			     بنانشان صحيح &subset_fdselect_سائيز /*ٻاهريون*/,
			     بنانشان صحيح &subset_fdselect_format /*ٻاهريون*/,
			     hb_ويڪٽر_t<ڪوڊ_pair_t> &fdselect_ranges /*ٻاهريون*/,
			     hb_inc_bimap_t &fdmap /*ٻاهريون*/)
{
  subset_fd_ڳڻپ = 0;
  subset_fdselect_سائيز = 0;
  subset_fdselect_format = 0;
  بنانشان صحيح انگ_ranges = 0;

  بنانشان صحيح subset_انگ_glyphs = plan->انگ_output_glyphs ();
  إذا (subset_انگ_glyphs == 0)
    أرجع سچ;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_سيٽ_t سيٽ;
    hb_codepoint_t prev_fd = CFF_UNDEF_ڪوڊ;
    خودڪار it = hb_iter (plan->new_منزل_old_gid_list);
    خودڪار _ = *it;
    لكل (hb_codepoint_t gid = 0; gid < subset_انگ_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      إذا (gid == _.پهريون)
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
      hb_pair_t<بنانشان, hb_codepoint_t> fd_range =
	likely (old_glyph < src_انگ_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<بنانشان, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      إذا (old_glyph < src_انگ_glyphs)
	fd_range.second = hb_min (fd_range.second, src_انگ_glyphs);
      بنانشان fd = fd_range.پهريون;

      إذا (fd != prev_fd)
      {
	سيٽ.جوڙيو (fd);
	انگ_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (ڪوڊ_pair_t { fd, gid });

	إذا (gid == old_glyph)
	  gid = hb_min (_.پهريون - 1, fd_range.second - 1);
      }
    }

    subset_fd_ڳڻپ = سيٽ.get_population ();
    إذا (subset_fd_ڳڻپ == fd_ڳڻپ)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.سڃاڻپ_عنصر (fd_ڳڻپ);
    }
    وإلا
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_ڪوڊ;
      طالما (سيٽ.next (&fd))
	fdmap.جوڙيو (fd);
      إذا (unlikely (fdmap.get_population () != subset_fd_ڳڻپ))
	أرجع خطأ;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    لكل (بنانشان صحيح i = 0; i < fdselect_ranges.ڊيگهائي; i++)
      fdselect_ranges[i].ڪوڊ = fdmap[fdselect_ranges[i].ڪوڊ];
  }

  /* determine which FDSelect format is most compact */
  إذا (subset_fd_ڳڻپ > 0xFF)
  {
    إذا (unlikely (src.format != 4))
      أرجع خطأ;
    subset_fdselect_format = 4;
    subset_fdselect_سائيز = FDSelect::min_سائيز + FDSelect4::min_سائيز + FDSelect4_Range::static_سائيز * انگ_ranges + HBUINT32::static_سائيز;
  }
  وإلا
  {
#إذا CFF_SERIALIZE_FDSELECT_0
    بنانشان صحيح format0_سائيز = FDSelect::min_سائيز + FDSelect0::min_سائيز + HBUINT8::static_سائيز * subset_انگ_glyphs;
#جيڪڏهن_پورو
    بنانشان صحيح format3_سائيز = FDSelect::min_سائيز + FDSelect3::min_سائيز + FDSelect3_Range::static_سائيز * انگ_ranges + HBUINT16::static_سائيز;

#إذا CFF_SERIALIZE_FDSELECT_0
    إذا (format0_سائيز <= format3_سائيز)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_سائيز = format0_سائيز;
    }
    وإلا
#جيڪڏهن_پورو
    {
      subset_fdselect_format = 3;
      subset_fdselect_سائيز = format3_سائيز;
    }
  }

  أرجع سچ;
}

سانچو <قسم_نالو FDSELECT3_4>
ساكن مضمن منطقي
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ثابت بنانشان صحيح انگ_glyphs,
			ثابت FDSelect &src,
			بنانشان صحيح سائيز,
			ثابت hb_ويڪٽر_t<ڪوڊ_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (هذا);
  FDSELECT3_4 *p = c->allocate_سائيز<FDSELECT3_4> (سائيز);
  إذا (unlikely (!p)) return_trace (خطأ);
  p->nRanges () = fdselect_ranges.ڊيگهائي;
  لكل (بنانشان صحيح i = 0; i < fdselect_ranges.ڊيگهائي; i++)
  {
    p->ranges[i].پهريون = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].ڪوڊ;
  }
  p->sentinel () = انگ_glyphs;
  return_trace (سچ);
}

/* Serialize a subset FDSelect format planned above. */
منطقي
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ثابت بنانشان صحيح انگ_glyphs,
			   ثابت FDSelect &src,
			   بنانشان صحيح fd_ڳڻپ_2,
			   بنانشان صحيح fdselect_format,
			   بنانشان صحيح سائيز,
			   ثابت hb_ويڪٽر_t<ڪوڊ_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (هذا);
  FDSelect *p = c->allocate_min<FDSelect> ();
  إذا (unlikely (!p)) return_trace (خطأ);
  p->format = fdselect_format;
  سائيز -= FDSelect::min_سائيز;

  بدل (fdselect_format)
  {
#إذا CFF_SERIALIZE_FDSELECT_0
  حالة 0:
  {
    FDSelect0 *p = c->allocate_سائيز<FDSelect0> (سائيز);
    إذا (unlikely (!p)) return_trace (خطأ);
    بنانشان صحيح range_اشاريو = 0;
    بنانشان صحيح fd = fdselect_ranges[range_اشاريو++].ڪوڊ;
    لكل (بنانشان صحيح i = 0; i < انگ_glyphs; i++)
    {
      إذا ((range_اشاريو < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_اشاريو].glyph))
      {
	fd = fdselect_ranges[range_اشاريو++].ڪوڊ;
      }
      p->fds[i] = fd;
    }
    return_trace (سچ);
  }
#جيڪڏهن_پورو /* CFF_SERIALIZE_FDSELECT_0 */

  حالة 3:
    أرجع serialize_fdselect_3_4<FDSelect3> (c, انگ_glyphs, src,
					      سائيز, fdselect_ranges);

  حالة 4:
    أرجع serialize_fdselect_3_4<FDSelect4> (c, انگ_glyphs, src,
					      سائيز, fdselect_ranges);

  افتراضي:
    return_trace (خطأ);
  }
}


#جيڪڏهن_پورو
