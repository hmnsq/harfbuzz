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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#ಅನ್ತರ್ಭಾವ "hb-ot-cff-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-cff2-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#define CFF_SERIALIZE_FDSELECT_0  0

using ನಾಮಾಕಾಶ CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

ತಾರ್ಕಿಕ
hb_plan_subset_cff_fdselect (ಧ್ರುವ hb_subset_plan_t *plan,
			     unsigned ಪೂರ್ಣಾಙ್ಕ fd_ಎಣಿಕೆ,
			     ಧ್ರುವ FDSelect &src, /* IN */
			     unsigned ಪೂರ್ಣಾಙ್ಕ src_ಅಂಕೆ_glyphs,
			     unsigned ಪೂರ್ಣಾಙ್ಕ &subset_fd_ಎಣಿಕೆ /* OUT */,
			     unsigned ಪೂರ್ಣಾಙ್ಕ &subset_fdselect_size /* OUT */,
			     unsigned ಪೂರ್ಣಾಙ್ಕ &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_ಎಣಿಕೆ = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_ranges = 0;

  unsigned ಪೂರ್ಣಾಙ್ಕ subset_ಅಂಕೆ_glyphs = plan->ಅಂಕೆ_output_glyphs ();
  ಯದಿ (subset_ಅಂಕೆ_glyphs == 0)
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    ಪ್ರತಿ (hb_codepoint_t gid = 0; gid < subset_ಅಂಕೆ_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      ಯದಿ (gid == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ಅನ್ಯಥಾ
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<unsigned, hb_codepoint_t> fd_range =
	likely (old_glyph < src_ಅಂಕೆ_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<unsigned, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      ಯದಿ (old_glyph < src_ಅಂಕೆ_glyphs)
	fd_range.second = hb_min (fd_range.second, src_ಅಂಕೆ_glyphs);
      unsigned fd = fd_range.first;

      ಯದಿ (fd != prev_fd)
      {
	set.add (fd);
	ಅಂಕೆ_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	ಯದಿ (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_ಎಣಿಕೆ = set.get_population ();
    ಯದಿ (subset_fd_ಎಣಿಕೆ == fd_ಎಣಿಕೆ)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fd_ಎಣಿಕೆ);
    }
    ಅನ್ಯಥಾ
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      ಯಾವತ್ (set.next (&fd))
	fdmap.add (fd);
      ಯದಿ (unlikely (fdmap.get_population () != subset_fd_ಎಣಿಕೆ))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  ಯದಿ (subset_fd_ಎಣಿಕೆ > 0xFF)
  {
    ಯದಿ (unlikely (src.format != 4))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * ಅಂಕೆ_ranges + HBUINT32::static_size;
  }
  ಅನ್ಯಥಾ
  {
#ಯದಿ CFF_SERIALIZE_FDSELECT_0
    unsigned ಪೂರ್ಣಾಙ್ಕ format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_ಅಂಕೆ_glyphs;
#endif
    unsigned ಪೂರ್ಣಾಙ್ಕ format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * ಅಂಕೆ_ranges + HBUINT16::static_size;

#ಯದಿ CFF_SERIALIZE_FDSELECT_0
    ಯದಿ (format0_size <= format3_size)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_size = format0_size;
    }
    ಅನ್ಯಥಾ
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_size = format3_size;
    }
  }

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

template <typename FDSELECT3_4>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs,
			ಧ್ರುವ FDSelect &src,
			unsigned ಪೂರ್ಣಾಙ್ಕ size,
			ಧ್ರುವ hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (ಏತತ್);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  ಯದಿ (unlikely (!p)) return_trace (ಅಸತ್ಯ);
  p->nRanges () = fdselect_ranges.length;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < fdselect_ranges.length; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = ಅಂಕೆ_glyphs;
  return_trace (ಸತ್ಯ);
}

/* Serialize a subset FDSelect format planned above. */
ತಾರ್ಕಿಕ
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs,
			   ಧ್ರುವ FDSelect &src,
			   unsigned ಪೂರ್ಣಾಙ್ಕ fd_ಎಣಿಕೆ_2,
			   unsigned ಪೂರ್ಣಾಙ್ಕ fdselect_format,
			   unsigned ಪೂರ್ಣಾಙ್ಕ size,
			   ಧ್ರುವ hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (ಏತತ್);
  FDSelect *p = c->allocate_min<FDSelect> ();
  ಯದಿ (unlikely (!p)) return_trace (ಅಸತ್ಯ);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  ವಿಕಲ್ಪ (fdselect_format)
  {
#ಯದಿ CFF_SERIALIZE_FDSELECT_0
  ಪಕ್ಷ 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    ಯದಿ (unlikely (!p)) return_trace (ಅಸತ್ಯ);
    unsigned ಪೂರ್ಣಾಙ್ಕ range_index = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ fd = fdselect_ranges[range_index++].code;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_glyphs; i++)
    {
      ಯದಿ ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (ಸತ್ಯ);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  ಪಕ್ಷ 3:
    ಪ್ರತ್ಯಾಗಮ serialize_fdselect_3_4<FDSelect3> (c, ಅಂಕೆ_glyphs, src,
					      size, fdselect_ranges);

  ಪಕ್ಷ 4:
    ಪ್ರತ್ಯಾಗಮ serialize_fdselect_3_4<FDSelect4> (c, ಅಂಕೆ_glyphs, src,
					      size, fdselect_ranges);

  ಸ್ವಭಾವ:
    return_trace (ಅಸತ್ಯ);
  }
}


#endif
