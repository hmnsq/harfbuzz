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

#include "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#include "hb-ot-cff-common.hh"
#include "hb-ot-cff2-table.hh"
#include "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#define CFF_SERIALIZE_FDSELECT_0  0

using namespace CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

bool
hb_plan_subset_cff_fdselect (const hb_subset_plan_t *plan,
			     unsigned ціле fdCount,
			     const FDSelect &src, /* IN */
			     unsigned ціле src_num_glyphs,
			     unsigned ціле &subset_fd_count /* OUT */,
			     unsigned ціле &subset_fdselect_size /* OUT */,
			     unsigned ціле &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_count = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  unsigned ціле num_ranges = 0;

  unsigned ціле subset_num_glyphs = plan->num_output_glyphs ();
  якщо (subset_num_glyphs == 0)
    повернути true;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    для (hb_codepoint_t gid = 0; gid < subset_num_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      якщо (gid == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      інакше
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<unsigned, hb_codepoint_t> fd_range =
	likely (old_glyph < src_num_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<unsigned, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      якщо (old_glyph < src_num_glyphs)
	fd_range.second = hb_min (fd_range.second, src_num_glyphs);
      unsigned fd = fd_range.first;

      якщо (fd != prev_fd)
      {
	set.add (fd);
	num_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	якщо (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_count = set.get_population ();
    якщо (subset_fd_count == fdCount)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fdCount);
    }
    інакше
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      поки (set.next (&fd))
	fdmap.add (fd);
      якщо (unlikely (fdmap.get_population () != subset_fd_count))
	повернути false;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    для (unsigned ціле i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  якщо (subset_fd_count > 0xFF)
  {
    якщо (unlikely (src.format != 4))
      повернути false;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * num_ranges + HBUINT32::static_size;
  }
  інакше
  {
#якщо CFF_SERIALIZE_FDSELECT_0
    unsigned ціле format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_num_glyphs;
#endif
    unsigned ціле format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * num_ranges + HBUINT16::static_size;

#якщо CFF_SERIALIZE_FDSELECT_0
    якщо (format0_size <= format3_size)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_size = format0_size;
    }
    інакше
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_size = format3_size;
    }
  }

  повернути true;
}

template <typename FDSELECT3_4>
static inline bool
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			const unsigned ціле num_glyphs,
			const FDSelect &src,
			unsigned ціле size,
			const hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (this);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  якщо (unlikely (!p)) return_trace (false);
  p->nRanges () = fdselect_ranges.length;
  для (unsigned ціле i = 0; i < fdselect_ranges.length; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = num_glyphs;
  return_trace (true);
}

/* Serialize a subset FDSelect format planned above. */
bool
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   const unsigned ціле num_glyphs,
			   const FDSelect &src,
			   unsigned ціле fd_count,
			   unsigned ціле fdselect_format,
			   unsigned ціле size,
			   const hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (this);
  FDSelect *p = c->allocate_min<FDSelect> ();
  якщо (unlikely (!p)) return_trace (false);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  switch (fdselect_format)
  {
#якщо CFF_SERIALIZE_FDSELECT_0
  case 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    якщо (unlikely (!p)) return_trace (false);
    unsigned ціле range_index = 0;
    unsigned ціле fd = fdselect_ranges[range_index++].code;
    для (unsigned ціле i = 0; i < num_glyphs; i++)
    {
      якщо ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (true);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  case 3:
    повернути serialize_fdselect_3_4<FDSelect3> (c, num_glyphs, src,
					      size, fdselect_ranges);

  case 4:
    повернути serialize_fdselect_3_4<FDSelect4> (c, num_glyphs, src,
					      size, fdselect_ranges);

  default:
    return_trace (false);
  }
}


#endif
