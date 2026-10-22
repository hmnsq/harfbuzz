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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#অন্তর্ভাব "hb-ot-cff-common.hh"
#অন্তর্ভাব "hb-ot-cff2-table.hh"
#অন্তর্ভাব "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#define CFF_SERIALIZE_FDSELECT_0  0

using নামাকাশ CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

তার্কিক
hb_plan_subset_cff_fdselect (ধ্রুব hb_subset_plan_t *plan,
			     unsigned পূর্ণাঙ্ক fd_গুনতি,
			     ধ্রুব FDSelect &src, /* IN */
			     unsigned পূর্ণাঙ্ক src_অঙ্ক_glyphs,
			     unsigned পূর্ণাঙ্ক &subset_fd_গুনতি /* OUT */,
			     unsigned পূর্ণাঙ্ক &subset_fdselect_size /* OUT */,
			     unsigned পূর্ণাঙ্ক &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_গুনতি = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  unsigned পূর্ণাঙ্ক অঙ্ক_ranges = 0;

  unsigned পূর্ণাঙ্ক subset_অঙ্ক_glyphs = plan->অঙ্ক_output_glyphs ();
  যদি (subset_অঙ্ক_glyphs == 0)
    প্রত্যাগম সত্য;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    প্রতি (hb_codepoint_t gid = 0; gid < subset_অঙ্ক_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      যদি (gid == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      অন্যথা
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<unsigned, hb_codepoint_t> fd_range =
	likely (old_glyph < src_অঙ্ক_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<unsigned, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      যদি (old_glyph < src_অঙ্ক_glyphs)
	fd_range.second = hb_min (fd_range.second, src_অঙ্ক_glyphs);
      unsigned fd = fd_range.first;

      যদি (fd != prev_fd)
      {
	set.add (fd);
	অঙ্ক_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	যদি (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_গুনতি = set.get_population ();
    যদি (subset_fd_গুনতি == fd_গুনতি)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fd_গুনতি);
    }
    অন্যথা
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      যাবত্ (set.next (&fd))
	fdmap.add (fd);
      যদি (unlikely (fdmap.get_population () != subset_fd_গুনতি))
	প্রত্যাগম অসত্য;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  যদি (subset_fd_গুনতি > 0xFF)
  {
    যদি (unlikely (src.format != 4))
      প্রত্যাগম অসত্য;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * অঙ্ক_ranges + HBUINT32::static_size;
  }
  অন্যথা
  {
#যদি CFF_SERIALIZE_FDSELECT_0
    unsigned পূর্ণাঙ্ক format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_অঙ্ক_glyphs;
#endif
    unsigned পূর্ণাঙ্ক format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * অঙ্ক_ranges + HBUINT16::static_size;

#যদি CFF_SERIALIZE_FDSELECT_0
    যদি (format0_size <= format3_size)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_size = format0_size;
    }
    অন্যথা
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_size = format3_size;
    }
  }

  প্রত্যাগম সত্য;
}

template <typename FDSELECT3_4>
স্থির অন্তঃস্থ তার্কিক
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ধ্রুব unsigned পূর্ণাঙ্ক অঙ্ক_glyphs,
			ধ্রুব FDSelect &src,
			unsigned পূর্ণাঙ্ক size,
			ধ্রুব hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (এতত্);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  যদি (unlikely (!p)) return_trace (অসত্য);
  p->nRanges () = fdselect_ranges.length;
  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < fdselect_ranges.length; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = অঙ্ক_glyphs;
  return_trace (সত্য);
}

/* Serialize a subset FDSelect format planned above. */
তার্কিক
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ধ্রুব unsigned পূর্ণাঙ্ক অঙ্ক_glyphs,
			   ধ্রুব FDSelect &src,
			   unsigned পূর্ণাঙ্ক fd_গুনতি_2,
			   unsigned পূর্ণাঙ্ক fdselect_format,
			   unsigned পূর্ণাঙ্ক size,
			   ধ্রুব hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (এতত্);
  FDSelect *p = c->allocate_min<FDSelect> ();
  যদি (unlikely (!p)) return_trace (অসত্য);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  বিকল্প (fdselect_format)
  {
#যদি CFF_SERIALIZE_FDSELECT_0
  পক্ষ 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    যদি (unlikely (!p)) return_trace (অসত্য);
    unsigned পূর্ণাঙ্ক range_index = 0;
    unsigned পূর্ণাঙ্ক fd = fdselect_ranges[range_index++].code;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_glyphs; i++)
    {
      যদি ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (সত্য);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  পক্ষ 3:
    প্রত্যাগম serialize_fdselect_3_4<FDSelect3> (c, অঙ্ক_glyphs, src,
					      size, fdselect_ranges);

  পক্ষ 4:
    প্রত্যাগম serialize_fdselect_3_4<FDSelect4> (c, অঙ্ক_glyphs, src,
					      size, fdselect_ranges);

  স্বভাব:
    return_trace (অসত্য);
  }
}


#endif
