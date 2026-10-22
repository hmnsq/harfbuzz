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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#अन्तर्भाव "hb-ot-cff-common.hh"
#अन्तर्भाव "hb-ot-cff2-table.hh"
#अन्तर्भाव "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#define CFF_SERIALIZE_FDSELECT_0  0

using नामाकाश CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

तार्किक
hb_plan_subset_cff_fdselect (ध्रुव hb_subset_plan_t *plan,
			     unsigned पूर्णाङ्क fd_मोजणी,
			     ध्रुव FDSelect &src, /* IN */
			     unsigned पूर्णाङ्क src_अंक_glyphs,
			     unsigned पूर्णाङ्क &subset_fd_मोजणी /* OUT */,
			     unsigned पूर्णाङ्क &subset_fdselect_size /* OUT */,
			     unsigned पूर्णाङ्क &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_मोजणी = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  unsigned पूर्णाङ्क अंक_ranges = 0;

  unsigned पूर्णाङ्क subset_अंक_glyphs = plan->अंक_output_glyphs ();
  यदि (subset_अंक_glyphs == 0)
    प्रत्यागम सत्य;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    प्रति (hb_codepoint_t gid = 0; gid < subset_अंक_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      यदि (gid == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      अन्यथा
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<unsigned, hb_codepoint_t> fd_range =
	likely (old_glyph < src_अंक_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<unsigned, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      यदि (old_glyph < src_अंक_glyphs)
	fd_range.second = hb_min (fd_range.second, src_अंक_glyphs);
      unsigned fd = fd_range.first;

      यदि (fd != prev_fd)
      {
	set.add (fd);
	अंक_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	यदि (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_मोजणी = set.get_population ();
    यदि (subset_fd_मोजणी == fd_मोजणी)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fd_मोजणी);
    }
    अन्यथा
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      यावत् (set.next (&fd))
	fdmap.add (fd);
      यदि (unlikely (fdmap.get_population () != subset_fd_मोजणी))
	प्रत्यागम असत्य;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    प्रति (unsigned पूर्णाङ्क i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  यदि (subset_fd_मोजणी > 0xFF)
  {
    यदि (unlikely (src.format != 4))
      प्रत्यागम असत्य;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * अंक_ranges + HBUINT32::static_size;
  }
  अन्यथा
  {
#यदि CFF_SERIALIZE_FDSELECT_0
    unsigned पूर्णाङ्क format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_अंक_glyphs;
#endif
    unsigned पूर्णाङ्क format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * अंक_ranges + HBUINT16::static_size;

#यदि CFF_SERIALIZE_FDSELECT_0
    यदि (format0_size <= format3_size)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_size = format0_size;
    }
    अन्यथा
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_size = format3_size;
    }
  }

  प्रत्यागम सत्य;
}

template <typename FDSELECT3_4>
स्थिर अन्तःस्थ तार्किक
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ध्रुव unsigned पूर्णाङ्क अंक_glyphs,
			ध्रुव FDSelect &src,
			unsigned पूर्णाङ्क size,
			ध्रुव hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (एतत्);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  यदि (unlikely (!p)) return_trace (असत्य);
  p->nRanges () = fdselect_ranges.length;
  प्रति (unsigned पूर्णाङ्क i = 0; i < fdselect_ranges.length; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = अंक_glyphs;
  return_trace (सत्य);
}

/* Serialize a subset FDSelect format planned above. */
तार्किक
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ध्रुव unsigned पूर्णाङ्क अंक_glyphs,
			   ध्रुव FDSelect &src,
			   unsigned पूर्णाङ्क fd_मोजणी_2,
			   unsigned पूर्णाङ्क fdselect_format,
			   unsigned पूर्णाङ्क size,
			   ध्रुव hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (एतत्);
  FDSelect *p = c->allocate_min<FDSelect> ();
  यदि (unlikely (!p)) return_trace (असत्य);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  विकल्प (fdselect_format)
  {
#यदि CFF_SERIALIZE_FDSELECT_0
  पक्ष 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    यदि (unlikely (!p)) return_trace (असत्य);
    unsigned पूर्णाङ्क range_index = 0;
    unsigned पूर्णाङ्क fd = fdselect_ranges[range_index++].code;
    प्रति (unsigned पूर्णाङ्क i = 0; i < अंक_glyphs; i++)
    {
      यदि ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (सत्य);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  पक्ष 3:
    प्रत्यागम serialize_fdselect_3_4<FDSelect3> (c, अंक_glyphs, src,
					      size, fdselect_ranges);

  पक्ष 4:
    प्रत्यागम serialize_fdselect_3_4<FDSelect4> (c, अंक_glyphs, src,
					      size, fdselect_ranges);

  स्वभाव:
    return_trace (असत्य);
  }
}


#endif
