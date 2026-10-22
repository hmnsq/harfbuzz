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

#ifndef HB_NO_SUBSET_CFF

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#define CFF_SERIALIZE_FDSELECT_0  0

using نطاق CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

منطقي
hb_plan_subset_cff_fdselect (ثابت hb_subset_plan_t *plan,
			     unsigned صحيح fdCount,
			     ثابت FDSelect &src, /* IN */
			     unsigned صحيح src_num_glyphs,
			     unsigned صحيح &subset_fd_count /* OUT */,
			     unsigned صحيح &subset_fdselect_size /* OUT */,
			     unsigned صحيح &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_count = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  unsigned صحيح num_ranges = 0;

  unsigned صحيح subset_num_glyphs = plan->num_output_glyphs ();
  𐡄𐡍 (subset_num_glyphs == 0)
    أرجع 𐡒𐡔𐡈;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    لكل (hb_codepoint_t gid = 0; gid < subset_num_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      𐡄𐡍 (gid == _.first)
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
      hb_pair_t<unsigned, hb_codepoint_t> fd_range =
	likely (old_glyph < src_num_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<unsigned, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      𐡄𐡍 (old_glyph < src_num_glyphs)
	fd_range.second = hb_min (fd_range.second, src_num_glyphs);
      unsigned fd = fd_range.first;

      𐡄𐡍 (fd != prev_fd)
      {
	set.add (fd);
	num_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	𐡄𐡍 (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_count = set.get_population ();
    𐡄𐡍 (subset_fd_count == fdCount)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fdCount);
    }
    وإلا
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      طالما (set.next (&fd))
	fdmap.add (fd);
      𐡄𐡍 (unlikely (fdmap.get_population () != subset_fd_count))
	أرجع خطأ;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    لكل (unsigned صحيح i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  𐡄𐡍 (subset_fd_count > 0xFF)
  {
    𐡄𐡍 (unlikely (src.format != 4))
      أرجع خطأ;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * num_ranges + HBUINT32::static_size;
  }
  وإلا
  {
#𐡄𐡍 CFF_SERIALIZE_FDSELECT_0
    unsigned صحيح format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_num_glyphs;
#endif
    unsigned صحيح format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * num_ranges + HBUINT16::static_size;

#𐡄𐡍 CFF_SERIALIZE_FDSELECT_0
    𐡄𐡍 (format0_size <= format3_size)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_size = format0_size;
    }
    وإلا
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_size = format3_size;
    }
  }

  أرجع 𐡒𐡔𐡈;
}

template <typename FDSELECT3_4>
ساكن مضمن منطقي
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ثابت unsigned صحيح num_glyphs,
			ثابت FDSelect &src,
			unsigned صحيح size,
			ثابت hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (هذا);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  𐡄𐡍 (unlikely (!p)) return_trace (خطأ);
  p->nRanges () = fdselect_ranges.length;
  لكل (unsigned صحيح i = 0; i < fdselect_ranges.length; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = num_glyphs;
  return_trace (𐡒𐡔𐡈);
}

/* Serialize a subset FDSelect format planned above. */
منطقي
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ثابت unsigned صحيح num_glyphs,
			   ثابت FDSelect &src,
			   unsigned صحيح fd_count,
			   unsigned صحيح fdselect_format,
			   unsigned صحيح size,
			   ثابت hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (هذا);
  FDSelect *p = c->allocate_min<FDSelect> ();
  𐡄𐡍 (unlikely (!p)) return_trace (خطأ);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  بدل (fdselect_format)
  {
#𐡄𐡍 CFF_SERIALIZE_FDSELECT_0
  حالة 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    𐡄𐡍 (unlikely (!p)) return_trace (خطأ);
    unsigned صحيح range_index = 0;
    unsigned صحيح fd = fdselect_ranges[range_index++].code;
    لكل (unsigned صحيح i = 0; i < num_glyphs; i++)
    {
      𐡄𐡍 ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (𐡒𐡔𐡈);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  حالة 3:
    أرجع serialize_fdselect_3_4<FDSelect3> (c, num_glyphs, src,
					      size, fdselect_ranges);

  حالة 4:
    أرجع serialize_fdselect_3_4<FDSelect4> (c, num_glyphs, src,
					      size, fdselect_ranges);

  افتراضي:
    return_trace (خطأ);
  }
}


#endif
