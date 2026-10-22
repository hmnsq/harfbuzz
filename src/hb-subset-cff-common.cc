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

#ifndef HB_NO_SUBSET_CFF

#شامل "hb-ot-cff-common.hh"
#شامل "hb-ot-cff2-table.hh"
#شامل "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#define CFF_SERIALIZE_FDSELECT_0  0

using فضاینام CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

منطقی
hb_plan_subset_cff_fdselect (ثابت hb_subset_plan_t *plan,
			     unsigned صحیح fd_شمار,
			     ثابت FDSelect &src, /* IN */
			     unsigned صحیح src_رقم_glyphs,
			     unsigned صحیح &subset_fd_شمار /* OUT */,
			     unsigned صحیح &subset_fdselect_size /* OUT */,
			     unsigned صحیح &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_شمار = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  unsigned صحیح رقم_ranges = 0;

  unsigned صحیح subset_رقم_glyphs = plan->رقم_output_glyphs ();
  اگر (subset_رقم_glyphs == 0)
    بازگشت درست;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    برای (hb_codepoint_t gid = 0; gid < subset_رقم_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      اگر (gid == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وگرنه
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<unsigned, hb_codepoint_t> fd_range =
	likely (old_glyph < src_رقم_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<unsigned, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      اگر (old_glyph < src_رقم_glyphs)
	fd_range.second = hb_min (fd_range.second, src_رقم_glyphs);
      unsigned fd = fd_range.first;

      اگر (fd != prev_fd)
      {
	set.add (fd);
	رقم_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	اگر (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_شمار = set.get_population ();
    اگر (subset_fd_شمار == fd_شمار)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fd_شمار);
    }
    وگرنه
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      وقتی (set.next (&fd))
	fdmap.add (fd);
      اگر (unlikely (fdmap.get_population () != subset_fd_شمار))
	بازگشت نادرست;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    برای (unsigned صحیح i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  اگر (subset_fd_شمار > 0xFF)
  {
    اگر (unlikely (src.format != 4))
      بازگشت نادرست;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * رقم_ranges + HBUINT32::static_size;
  }
  وگرنه
  {
#اگر CFF_SERIALIZE_FDSELECT_0
    unsigned صحیح format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_رقم_glyphs;
#endif
    unsigned صحیح format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * رقم_ranges + HBUINT16::static_size;

#اگر CFF_SERIALIZE_FDSELECT_0
    اگر (format0_size <= format3_size)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_size = format0_size;
    }
    وگرنه
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_size = format3_size;
    }
  }

  بازگشت درست;
}

template <typename FDSELECT3_4>
ایستا درونخطی منطقی
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ثابت unsigned صحیح رقم_glyphs,
			ثابت FDSelect &src,
			unsigned صحیح size,
			ثابت hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (این);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  اگر (unlikely (!p)) return_trace (نادرست);
  p->nRanges () = fdselect_ranges.length;
  برای (unsigned صحیح i = 0; i < fdselect_ranges.length; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = رقم_glyphs;
  return_trace (درست);
}

/* Serialize a subset FDSelect format planned above. */
منطقی
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ثابت unsigned صحیح رقم_glyphs,
			   ثابت FDSelect &src,
			   unsigned صحیح fd_شمار_2,
			   unsigned صحیح fdselect_format,
			   unsigned صحیح size,
			   ثابت hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (این);
  FDSelect *p = c->allocate_min<FDSelect> ();
  اگر (unlikely (!p)) return_trace (نادرست);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  انتخاب (fdselect_format)
  {
#اگر CFF_SERIALIZE_FDSELECT_0
  حالت 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    اگر (unlikely (!p)) return_trace (نادرست);
    unsigned صحیح range_index = 0;
    unsigned صحیح fd = fdselect_ranges[range_index++].code;
    برای (unsigned صحیح i = 0; i < رقم_glyphs; i++)
    {
      اگر ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (درست);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  حالت 3:
    بازگشت serialize_fdselect_3_4<FDSelect3> (c, رقم_glyphs, src,
					      size, fdselect_ranges);

  حالت 4:
    بازگشت serialize_fdselect_3_4<FDSelect4> (c, رقم_glyphs, src,
					      size, fdselect_ranges);

  پیشفرض:
    return_trace (نادرست);
  }
}


#endif
