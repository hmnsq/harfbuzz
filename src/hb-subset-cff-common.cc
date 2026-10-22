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

যৌক্তিক
hb_plan_subset_cff_fdselect (ধ্রুবক hb_subset_plan_t *plan,
			     unsigned পূর্ণসংখ্যা fd_গুনতি,
			     ধ্রুবক FDSelect &src, /* IN */
			     unsigned পূর্ণসংখ্যা src_অঙ্ক_glyphs,
			     unsigned পূর্ণসংখ্যা &subset_fd_গুনতি /* OUT */,
			     unsigned পূর্ণসংখ্যা &subset_fdselect_আকার /* OUT */,
			     unsigned পূর্ণসংখ্যা &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_গুনতি = 0;
  subset_fdselect_আকার = 0;
  subset_fdselect_format = 0;
  unsigned পূর্ণসংখ্যা অঙ্ক_ranges = 0;

  unsigned পূর্ণসংখ্যা subset_অঙ্ক_glyphs = plan->অঙ্ক_output_glyphs ();
  যদি (subset_অঙ্ক_glyphs == 0)
    ফেরত সত্য;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t পূর্ব_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    জন্য (hb_codepoint_t gid = 0; gid < subset_অঙ্ক_glyphs; gid++)
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

      যদি (fd != পূর্ব_fd)
      {
	set.add (fd);
	অঙ্ক_ranges++;
	পূর্ব_fd = fd;
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
      যখন (set.next (&fd))
	fdmap.add (fd);
      যদি (unlikely (fdmap.get_population () != subset_fd_গুনতি))
	ফেরত মিথ্যা;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < fdselect_ranges.দৈর্ঘ্য; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  যদি (subset_fd_গুনতি > 0xFF)
  {
    যদি (unlikely (src.format != 4))
      ফেরত মিথ্যা;
    subset_fdselect_format = 4;
    subset_fdselect_আকার = FDSelect::min_আকার + FDSelect4::min_আকার + FDSelect4_Range::static_আকার * অঙ্ক_ranges + HBUINT32::static_আকার;
  }
  অন্যথা
  {
#যদি CFF_SERIALIZE_FDSELECT_0
    unsigned পূর্ণসংখ্যা format0_আকার = FDSelect::min_আকার + FDSelect0::min_আকার + HBUINT8::static_আকার * subset_অঙ্ক_glyphs;
#endif
    unsigned পূর্ণসংখ্যা format3_আকার = FDSelect::min_আকার + FDSelect3::min_আকার + FDSelect3_Range::static_আকার * অঙ্ক_ranges + HBUINT16::static_আকার;

#যদি CFF_SERIALIZE_FDSELECT_0
    যদি (format0_আকার <= format3_আকার)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_আকার = format0_আকার;
    }
    অন্যথা
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_আকার = format3_আকার;
    }
  }

  ফেরত সত্য;
}

template <typename FDSELECT3_4>
স্থির অন্তঃরেখ যৌক্তিক
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ধ্রুবক unsigned পূর্ণসংখ্যা অঙ্ক_glyphs,
			ধ্রুবক FDSelect &src,
			unsigned পূর্ণসংখ্যা আকার_2,
			ধ্রুবক hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (এই);
  FDSELECT3_4 *p = c->allocate_আকার<FDSELECT3_4> (আকার_2);
  যদি (unlikely (!p)) return_trace (মিথ্যা);
  p->nRanges () = fdselect_ranges.দৈর্ঘ্য;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < fdselect_ranges.দৈর্ঘ্য; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = অঙ্ক_glyphs;
  return_trace (সত্য);
}

/* Serialize a subset FDSelect format planned above. */
যৌক্তিক
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ধ্রুবক unsigned পূর্ণসংখ্যা অঙ্ক_glyphs,
			   ধ্রুবক FDSelect &src,
			   unsigned পূর্ণসংখ্যা fd_গুনতি_2,
			   unsigned পূর্ণসংখ্যা fdselect_format,
			   unsigned পূর্ণসংখ্যা আকার_2,
			   ধ্রুবক hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (এই);
  FDSelect *p = c->allocate_min<FDSelect> ();
  যদি (unlikely (!p)) return_trace (মিথ্যা);
  p->format = fdselect_format;
  আকার_2 -= FDSelect::min_আকার;

  নির্বাচন (fdselect_format)
  {
#যদি CFF_SERIALIZE_FDSELECT_0
  ক্ষেত্র 0:
  {
    FDSelect0 *p = c->allocate_আকার<FDSelect0> (আকার_2);
    যদি (unlikely (!p)) return_trace (মিথ্যা);
    unsigned পূর্ণসংখ্যা range_সূচক = 0;
    unsigned পূর্ণসংখ্যা fd = fdselect_ranges[range_সূচক++].code;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_glyphs; i++)
    {
      যদি ((range_সূচক < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_সূচক].glyph))
      {
	fd = fdselect_ranges[range_সূচক++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (সত্য);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  ক্ষেত্র 3:
    ফেরত serialize_fdselect_3_4<FDSelect3> (c, অঙ্ক_glyphs, src,
					      আকার_2, fdselect_ranges);

  ক্ষেত্র 4:
    ফেরত serialize_fdselect_3_4<FDSelect4> (c, অঙ্ক_glyphs, src,
					      আকার_2, fdselect_ranges);

  পূর্বনির্ধারিত:
    return_trace (মিথ্যা);
  }
}


#endif
