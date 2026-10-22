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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#અન્તર્ભાવ "hb-ot-cff-common.hh"
#અન્તર્ભાવ "hb-ot-cff2-table.hh"
#અન્તર્ભાવ "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#define CFF_SERIALIZE_FDSELECT_0  0

using નામાકાશ CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

તાર્કિક
hb_plan_subset_cff_fdselect (અચલ hb_subset_plan_t *plan,
			     unsigned પૂર્ણાંક fd_ગણતરી,
			     અચલ FDSelect &src, /* IN */
			     unsigned પૂર્ણાંક src_અંક_glyphs,
			     unsigned પૂર્ણાંક &subset_fd_ગણતરી /* OUT */,
			     unsigned પૂર્ણાંક &subset_fdselect_size /* OUT */,
			     unsigned પૂર્ણાંક &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_ગણતરી = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  unsigned પૂર્ણાંક અંક_ranges = 0;

  unsigned પૂર્ણાંક subset_અંક_glyphs = plan->અંક_output_glyphs ();
  જો (subset_અંક_glyphs == 0)
    પાછા સાચું;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    માટે (hb_codepoint_t gid = 0; gid < subset_અંક_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      જો (gid == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      નહીંતર
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<unsigned, hb_codepoint_t> fd_range =
	likely (old_glyph < src_અંક_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<unsigned, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      જો (old_glyph < src_અંક_glyphs)
	fd_range.second = hb_min (fd_range.second, src_અંક_glyphs);
      unsigned fd = fd_range.first;

      જો (fd != prev_fd)
      {
	set.add (fd);
	અંક_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	જો (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_ગણતરી = set.get_population ();
    જો (subset_fd_ગણતરી == fd_ગણતરી)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fd_ગણતરી);
    }
    નહીંતર
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      જ્યારે (set.next (&fd))
	fdmap.add (fd);
      જો (unlikely (fdmap.get_population () != subset_fd_ગણતરી))
	પાછા ખોટું;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    માટે (unsigned પૂર્ણાંક i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  જો (subset_fd_ગણતરી > 0xFF)
  {
    જો (unlikely (src.format != 4))
      પાછા ખોટું;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * અંક_ranges + HBUINT32::static_size;
  }
  નહીંતર
  {
#જો CFF_SERIALIZE_FDSELECT_0
    unsigned પૂર્ણાંક format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_અંક_glyphs;
#endif
    unsigned પૂર્ણાંક format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * અંક_ranges + HBUINT16::static_size;

#જો CFF_SERIALIZE_FDSELECT_0
    જો (format0_size <= format3_size)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_size = format0_size;
    }
    નહીંતર
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_size = format3_size;
    }
  }

  પાછા સાચું;
}

template <typename FDSELECT3_4>
સ્થિર અંતઃરેખ તાર્કિક
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			અચલ unsigned પૂર્ણાંક અંક_glyphs,
			અચલ FDSelect &src,
			unsigned પૂર્ણાંક size,
			અચલ hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (આ);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  જો (unlikely (!p)) return_trace (ખોટું);
  p->nRanges () = fdselect_ranges.length;
  માટે (unsigned પૂર્ણાંક i = 0; i < fdselect_ranges.length; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = અંક_glyphs;
  return_trace (સાચું);
}

/* Serialize a subset FDSelect format planned above. */
તાર્કિક
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   અચલ unsigned પૂર્ણાંક અંક_glyphs,
			   અચલ FDSelect &src,
			   unsigned પૂર્ણાંક fd_ગણતરી_2,
			   unsigned પૂર્ણાંક fdselect_format,
			   unsigned પૂર્ણાંક size,
			   અચલ hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (આ);
  FDSelect *p = c->allocate_min<FDSelect> ();
  જો (unlikely (!p)) return_trace (ખોટું);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  પસંદગી (fdselect_format)
  {
#જો CFF_SERIALIZE_FDSELECT_0
  સ્થિતિ 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    જો (unlikely (!p)) return_trace (ખોટું);
    unsigned પૂર્ણાંક range_index = 0;
    unsigned પૂર્ણાંક fd = fdselect_ranges[range_index++].code;
    માટે (unsigned પૂર્ણાંક i = 0; i < અંક_glyphs; i++)
    {
      જો ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (સાચું);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  સ્થિતિ 3:
    પાછા serialize_fdselect_3_4<FDSelect3> (c, અંક_glyphs, src,
					      size, fdselect_ranges);

  સ્થિતિ 4:
    પાછા serialize_fdselect_3_4<FDSelect4> (c, અંક_glyphs, src,
					      size, fdselect_ranges);

  મૂળભૂત:
    return_trace (ખોટું);
  }
}


#endif
