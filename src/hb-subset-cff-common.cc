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

#اگر_تعریف_نشده HB_NO_SUBSET_CFF

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#تعریف_کن CFF_SERIALIZE_FDSELECT_0  0

استفاده نطاق CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

منطقي
hb_plan_subset_cff_fdselect (ثابت hb_subset_plan_t *plan,
			     بیعلامت صحيح fd_شمار,
			     ثابت FDSelect &src, /*درون*/
			     بیعلامت صحيح src_رقم_glyphs,
			     بیعلامت صحيح &subset_fd_شمار /*بیرون*/,
			     بیعلامت صحيح &subset_fdselect_اندازه /*بیرون*/,
			     بیعلامت صحيح &subset_fdselect_format /*بیرون*/,
			     hb_وکتور_t<کد_pair_t> &fdselect_ranges /*بیرون*/,
			     hb_inc_bimap_t &fdmap /*بیرون*/)
{
  subset_fd_شمار = 0;
  subset_fdselect_اندازه = 0;
  subset_fdselect_format = 0;
  بیعلامت صحيح رقم_ranges = 0;

  بیعلامت صحيح subset_رقم_glyphs = plan->رقم_output_glyphs ();
  إذا (subset_رقم_glyphs == 0)
    أرجع درست;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_مجموعه_t مجموعه;
    hb_codepoint_t prev_fd = CFF_UNDEF_کد;
    خودکار it = hb_iter (plan->new_مقصد_old_gid_list);
    خودکار _ = *it;
    لكل (hb_codepoint_t gid = 0; gid < subset_رقم_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      إذا (gid == _.اولی)
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
      hb_pair_t<بیعلامت, hb_codepoint_t> fd_range =
	likely (old_glyph < src_رقم_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<بیعلامت, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      إذا (old_glyph < src_رقم_glyphs)
	fd_range.second = hb_min (fd_range.second, src_رقم_glyphs);
      بیعلامت fd = fd_range.اولی;

      إذا (fd != prev_fd)
      {
	مجموعه.جمع_کن (fd);
	رقم_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (کد_pair_t { fd, gid });

	إذا (gid == old_glyph)
	  gid = hb_min (_.اولی - 1, fd_range.second - 1);
      }
    }

    subset_fd_شمار = مجموعه.get_population ();
    إذا (subset_fd_شمار == fd_شمار)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.عنصر_خنثی (fd_شمار);
    }
    وإلا
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_کد;
      طالما (مجموعه.next (&fd))
	fdmap.جمع_کن (fd);
      إذا (unlikely (fdmap.get_population () != subset_fd_شمار))
	أرجع خطأ;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    لكل (بیعلامت صحيح i = 0; i < fdselect_ranges.درازی; i++)
      fdselect_ranges[i].کد = fdmap[fdselect_ranges[i].کد];
  }

  /* determine which FDSelect format is most compact */
  إذا (subset_fd_شمار > 0xFF)
  {
    إذا (unlikely (src.format != 4))
      أرجع خطأ;
    subset_fdselect_format = 4;
    subset_fdselect_اندازه = FDSelect::min_اندازه + FDSelect4::min_اندازه + FDSelect4_Range::static_اندازه * رقم_ranges + HBUINT32::static_اندازه;
  }
  وإلا
  {
#إذا CFF_SERIALIZE_FDSELECT_0
    بیعلامت صحيح format0_اندازه = FDSelect::min_اندازه + FDSelect0::min_اندازه + HBUINT8::static_اندازه * subset_رقم_glyphs;
#پایان_اگر
    بیعلامت صحيح format3_اندازه = FDSelect::min_اندازه + FDSelect3::min_اندازه + FDSelect3_Range::static_اندازه * رقم_ranges + HBUINT16::static_اندازه;

#إذا CFF_SERIALIZE_FDSELECT_0
    إذا (format0_اندازه <= format3_اندازه)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_اندازه = format0_اندازه;
    }
    وإلا
#پایان_اگر
    {
      subset_fdselect_format = 3;
      subset_fdselect_اندازه = format3_اندازه;
    }
  }

  أرجع درست;
}

قالب <نام_نوع FDSELECT3_4>
ساكن مضمن منطقي
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ثابت بیعلامت صحيح رقم_glyphs,
			ثابت FDSelect &src,
			بیعلامت صحيح اندازه,
			ثابت hb_وکتور_t<کد_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (هذا);
  FDSELECT3_4 *p = c->allocate_اندازه<FDSELECT3_4> (اندازه);
  إذا (unlikely (!p)) return_trace (خطأ);
  p->nRanges () = fdselect_ranges.درازی;
  لكل (بیعلامت صحيح i = 0; i < fdselect_ranges.درازی; i++)
  {
    p->ranges[i].اولی = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].کد;
  }
  p->sentinel () = رقم_glyphs;
  return_trace (درست);
}

/* Serialize a subset FDSelect format planned above. */
منطقي
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ثابت بیعلامت صحيح رقم_glyphs,
			   ثابت FDSelect &src,
			   بیعلامت صحيح fd_شمار_2,
			   بیعلامت صحيح fdselect_format,
			   بیعلامت صحيح اندازه,
			   ثابت hb_وکتور_t<کد_pair_t> &fdselect_ranges)
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
    بیعلامت صحيح range_شاخص = 0;
    بیعلامت صحيح fd = fdselect_ranges[range_شاخص++].کد;
    لكل (بیعلامت صحيح i = 0; i < رقم_glyphs; i++)
    {
      إذا ((range_شاخص < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_شاخص].glyph))
      {
	fd = fdselect_ranges[range_شاخص++].کد;
      }
      p->fds[i] = fd;
    }
    return_trace (درست);
  }
#پایان_اگر /* CFF_SERIALIZE_FDSELECT_0 */

  حالة 3:
    أرجع serialize_fdselect_3_4<FDSelect3> (c, رقم_glyphs, src,
					      اندازه, fdselect_ranges);

  حالة 4:
    أرجع serialize_fdselect_3_4<FDSelect4> (c, رقم_glyphs, src,
					      اندازه, fdselect_ranges);

  افتراضي:
    return_trace (خطأ);
  }
}


#پایان_اگر
