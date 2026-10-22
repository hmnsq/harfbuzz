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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#അന്തര്ഭാവ "hb-ot-cff-common.hh"
#അന്തര്ഭാവ "hb-ot-cff2-table.hh"
#അന്തര്ഭാവ "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#define CFF_SERIALIZE_FDSELECT_0  0

using നാമാകാശ CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

തർക്കം
hb_plan_subset_cff_fdselect (സ്ഥിരം hb_subset_plan_t *plan,
			     unsigned പൂർണ്ണസംഖ്യ fd_എണ്ണം,
			     സ്ഥിരം FDSelect &src, /* IN */
			     unsigned പൂർണ്ണസംഖ്യ src_അക്കം_glyphs,
			     unsigned പൂർണ്ണസംഖ്യ &subset_fd_എണ്ണം /* OUT */,
			     unsigned പൂർണ്ണസംഖ്യ &subset_fdselect_size /* OUT */,
			     unsigned പൂർണ്ണസംഖ്യ &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_എണ്ണം = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  unsigned പൂർണ്ണസംഖ്യ അക്കം_ranges = 0;

  unsigned പൂർണ്ണസംഖ്യ subset_അക്കം_glyphs = plan->അക്കം_output_glyphs ();
  എങ്കിൽ (subset_അക്കം_glyphs == 0)
    തിരികെ സത്യം;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    വേണ്ടി (hb_codepoint_t gid = 0; gid < subset_അക്കം_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      എങ്കിൽ (gid == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      അല്ലെങ്കിൽ
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<unsigned, hb_codepoint_t> fd_range =
	likely (old_glyph < src_അക്കം_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<unsigned, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      എങ്കിൽ (old_glyph < src_അക്കം_glyphs)
	fd_range.second = hb_min (fd_range.second, src_അക്കം_glyphs);
      unsigned fd = fd_range.first;

      എങ്കിൽ (fd != prev_fd)
      {
	set.add (fd);
	അക്കം_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	എങ്കിൽ (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_എണ്ണം = set.get_population ();
    എങ്കിൽ (subset_fd_എണ്ണം == fd_എണ്ണം)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fd_എണ്ണം);
    }
    അല്ലെങ്കിൽ
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      ആകുമ്പോൾ (set.next (&fd))
	fdmap.add (fd);
      എങ്കിൽ (unlikely (fdmap.get_population () != subset_fd_എണ്ണം))
	തിരികെ അസത്യം;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  എങ്കിൽ (subset_fd_എണ്ണം > 0xFF)
  {
    എങ്കിൽ (unlikely (src.format != 4))
      തിരികെ അസത്യം;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * അക്കം_ranges + HBUINT32::static_size;
  }
  അല്ലെങ്കിൽ
  {
#എങ്കിൽ CFF_SERIALIZE_FDSELECT_0
    unsigned പൂർണ്ണസംഖ്യ format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_അക്കം_glyphs;
#endif
    unsigned പൂർണ്ണസംഖ്യ format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * അക്കം_ranges + HBUINT16::static_size;

#എങ്കിൽ CFF_SERIALIZE_FDSELECT_0
    എങ്കിൽ (format0_size <= format3_size)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_size = format0_size;
    }
    അല്ലെങ്കിൽ
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_size = format3_size;
    }
  }

  തിരികെ സത്യം;
}

template <typename FDSELECT3_4>
സ്ഥിതം വരിയിൽ തർക്കം
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs,
			സ്ഥിരം FDSelect &src,
			unsigned പൂർണ്ണസംഖ്യ size,
			സ്ഥിരം hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (ഇത്);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  എങ്കിൽ (unlikely (!p)) return_trace (അസത്യം);
  p->nRanges () = fdselect_ranges.length;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < fdselect_ranges.length; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = അക്കം_glyphs;
  return_trace (സത്യം);
}

/* Serialize a subset FDSelect format planned above. */
തർക്കം
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs,
			   സ്ഥിരം FDSelect &src,
			   unsigned പൂർണ്ണസംഖ്യ fd_എണ്ണം_2,
			   unsigned പൂർണ്ണസംഖ്യ fdselect_format,
			   unsigned പൂർണ്ണസംഖ്യ size,
			   സ്ഥിരം hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (ഇത്);
  FDSelect *p = c->allocate_min<FDSelect> ();
  എങ്കിൽ (unlikely (!p)) return_trace (അസത്യം);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  തിരഞ്ഞെടുപ്പ് (fdselect_format)
  {
#എങ്കിൽ CFF_SERIALIZE_FDSELECT_0
  അവസ്ഥ 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    എങ്കിൽ (unlikely (!p)) return_trace (അസത്യം);
    unsigned പൂർണ്ണസംഖ്യ range_index = 0;
    unsigned പൂർണ്ണസംഖ്യ fd = fdselect_ranges[range_index++].code;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_glyphs; i++)
    {
      എങ്കിൽ ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (സത്യം);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  അവസ്ഥ 3:
    തിരികെ serialize_fdselect_3_4<FDSelect3> (c, അക്കം_glyphs, src,
					      size, fdselect_ranges);

  അവസ്ഥ 4:
    തിരികെ serialize_fdselect_3_4<FDSelect4> (c, അക്കം_glyphs, src,
					      size, fdselect_ranges);

  സ്ഥിരസ്ഥിതി:
    return_trace (അസത്യം);
  }
}


#endif
