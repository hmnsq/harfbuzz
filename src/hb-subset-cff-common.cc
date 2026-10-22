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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#ଅନ୍ତର୍ଭାଵ "hb-ot-cff-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff2-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#define CFF_SERIALIZE_FDSELECT_0  0

using ନାମାକାଶ CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

ତାର୍କିକ
hb_plan_subset_cff_fdselect (ଅଚଳ hb_subset_plan_t *plan,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd_ଗଣନା,
			     ଅଚଳ FDSelect &src, /* IN */
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_ଅଙ୍କ_glyphs,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା &subset_fd_ଗଣନା /* OUT */,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା &subset_fdselect_size /* OUT */,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_ଗଣନା = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_ranges = 0;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subset_ଅଙ୍କ_glyphs = plan->ଅଙ୍କ_output_glyphs ();
  ଯଦି (subset_ଅଙ୍କ_glyphs == 0)
    ଫେରନ୍ତୁ ସତ୍ୟ;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    ପାଇଁ (hb_codepoint_t gid = 0; gid < subset_ଅଙ୍କ_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      ଯଦି (gid == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ଅନ୍ୟଥା
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<unsigned, hb_codepoint_t> fd_range =
	likely (old_glyph < src_ଅଙ୍କ_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<unsigned, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      ଯଦି (old_glyph < src_ଅଙ୍କ_glyphs)
	fd_range.second = hb_min (fd_range.second, src_ଅଙ୍କ_glyphs);
      unsigned fd = fd_range.first;

      ଯଦି (fd != prev_fd)
      {
	set.add (fd);
	ଅଙ୍କ_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	ଯଦି (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_ଗଣନା = set.get_population ();
    ଯଦି (subset_fd_ଗଣନା == fd_ଗଣନା)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fd_ଗଣନା);
    }
    ଅନ୍ୟଥା
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      ଯେତେବେଳେ (set.next (&fd))
	fdmap.add (fd);
      ଯଦି (unlikely (fdmap.get_population () != subset_fd_ଗଣନା))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  ଯଦି (subset_fd_ଗଣନା > 0xFF)
  {
    ଯଦି (unlikely (src.format != 4))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * ଅଙ୍କ_ranges + HBUINT32::static_size;
  }
  ଅନ୍ୟଥା
  {
#ଯଦି CFF_SERIALIZE_FDSELECT_0
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_ଅଙ୍କ_glyphs;
#endif
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * ଅଙ୍କ_ranges + HBUINT16::static_size;

#ଯଦି CFF_SERIALIZE_FDSELECT_0
    ଯଦି (format0_size <= format3_size)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_size = format0_size;
    }
    ଅନ୍ୟଥା
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_size = format3_size;
    }
  }

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

template <typename FDSELECT3_4>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs,
			ଅଚଳ FDSelect &src,
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size,
			ଅଚଳ hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (ଏହି);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  ଯଦି (unlikely (!p)) return_trace (ମିଥ୍ୟା);
  p->nRanges () = fdselect_ranges.length;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < fdselect_ranges.length; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = ଅଙ୍କ_glyphs;
  return_trace (ସତ୍ୟ);
}

/* Serialize a subset FDSelect format planned above. */
ତାର୍କିକ
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs,
			   ଅଚଳ FDSelect &src,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd_ଗଣନା_2,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdselect_format,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size,
			   ଅଚଳ hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (ଏହି);
  FDSelect *p = c->allocate_min<FDSelect> ();
  ଯଦି (unlikely (!p)) return_trace (ମିଥ୍ୟା);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  ବାଛ (fdselect_format)
  {
#ଯଦି CFF_SERIALIZE_FDSELECT_0
  ସ୍ଥିତି 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    ଯଦି (unlikely (!p)) return_trace (ମିଥ୍ୟା);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା range_index = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = fdselect_ranges[range_index++].code;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_glyphs; i++)
    {
      ଯଦି ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (ସତ୍ୟ);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  ସ୍ଥିତି 3:
    ଫେରନ୍ତୁ serialize_fdselect_3_4<FDSelect3> (c, ଅଙ୍କ_glyphs, src,
					      size, fdselect_ranges);

  ସ୍ଥିତି 4:
    ଫେରନ୍ତୁ serialize_fdselect_3_4<FDSelect4> (c, ଅଙ୍କ_glyphs, src,
					      size, fdselect_ranges);

  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    return_trace (ମିଥ୍ୟା);
  }
}


#endif
