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
			     unsigned ကိန်းပြည့် fdCount,
			     const FDSelect &src, /* IN */
			     unsigned ကိန်းပြည့် src_num_glyphs,
			     unsigned ကိန်းပြည့် &subset_fd_count /* OUT */,
			     unsigned ကိန်းပြည့် &subset_fdselect_size /* OUT */,
			     unsigned ကိန်းပြည့် &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_count = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  unsigned ကိန်းပြည့် num_ranges = 0;

  unsigned ကိန်းပြည့် subset_num_glyphs = plan->num_output_glyphs ();
  အကယ်၍ (subset_num_glyphs == 0)
    ပြန်ပေး true;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    အတွက် (hb_codepoint_t gid = 0; gid < subset_num_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      အကယ်၍ (gid == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      မဟုတ်ပါက
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<unsigned, hb_codepoint_t> fd_range =
	likely (old_glyph < src_num_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<unsigned, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      အကယ်၍ (old_glyph < src_num_glyphs)
	fd_range.second = hb_min (fd_range.second, src_num_glyphs);
      unsigned fd = fd_range.first;

      အကယ်၍ (fd != prev_fd)
      {
	set.add (fd);
	num_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	အကယ်၍ (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_count = set.get_population ();
    အကယ်၍ (subset_fd_count == fdCount)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fdCount);
    }
    မဟုတ်ပါက
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      နေစဉ် (set.next (&fd))
	fdmap.add (fd);
      အကယ်၍ (unlikely (fdmap.get_population () != subset_fd_count))
	ပြန်ပေး false;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  အကယ်၍ (subset_fd_count > 0xFF)
  {
    အကယ်၍ (unlikely (src.format != 4))
      ပြန်ပေး false;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * num_ranges + HBUINT32::static_size;
  }
  မဟုတ်ပါက
  {
#အကယ်၍ CFF_SERIALIZE_FDSELECT_0
    unsigned ကိန်းပြည့် format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_num_glyphs;
#endif
    unsigned ကိန်းပြည့် format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * num_ranges + HBUINT16::static_size;

#အကယ်၍ CFF_SERIALIZE_FDSELECT_0
    အကယ်၍ (format0_size <= format3_size)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_size = format0_size;
    }
    မဟုတ်ပါက
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_size = format3_size;
    }
  }

  ပြန်ပေး true;
}

template <typename FDSELECT3_4>
static anta_hstha bool
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			const unsigned ကိန်းပြည့် num_glyphs,
			const FDSelect &src,
			unsigned ကိန်းပြည့် size,
			const hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (this);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  အကယ်၍ (unlikely (!p)) return_trace (false);
  p->nRanges () = fdselect_ranges.length;
  အတွက် (unsigned ကိန်းပြည့် i = 0; i < fdselect_ranges.length; i++)
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
			   const unsigned ကိန်းပြည့် num_glyphs,
			   const FDSelect &src,
			   unsigned ကိန်းပြည့် fd_count,
			   unsigned ကိန်းပြည့် fdselect_format,
			   unsigned ကိန်းပြည့် size,
			   const hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (this);
  FDSelect *p = c->allocate_min<FDSelect> ();
  အကယ်၍ (unlikely (!p)) return_trace (false);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  switch (fdselect_format)
  {
#အကယ်၍ CFF_SERIALIZE_FDSELECT_0
  pak_sa 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    အကယ်၍ (unlikely (!p)) return_trace (false);
    unsigned ကိန်းပြည့် range_index = 0;
    unsigned ကိန်းပြည့် fd = fdselect_ranges[range_index++].code;
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < num_glyphs; i++)
    {
      အကယ်၍ ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (true);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  pak_sa 3:
    ပြန်ပေး serialize_fdselect_3_4<FDSelect3> (c, num_glyphs, src,
					      size, fdselect_ranges);

  pak_sa 4:
    ပြန်ပေး serialize_fdselect_3_4<FDSelect4> (c, num_glyphs, src,
					      size, fdselect_ranges);

  default:
    return_trace (false);
  }
}


#endif
