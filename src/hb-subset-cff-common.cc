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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#அந்தர்பாவ "hb-ot-cff-common.hh"
#அந்தர்பாவ "hb-ot-cff2-table.hh"
#அந்தர்பாவ "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#define CFF_SERIALIZE_FDSELECT_0  0

using நாமாகாஶ CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

தருக்க
hb_plan_subset_cff_fdselect (மாறிலி hb_subset_plan_t *plan,
			     unsigned முழு fd_மொத்தம்,
			     மாறிலி FDSelect &src, /* IN */
			     unsigned முழு src_இலக்கம்_glyphs,
			     unsigned முழு &subset_fd_மொத்தம் /* OUT */,
			     unsigned முழு &subset_fdselect_size /* OUT */,
			     unsigned முழு &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_மொத்தம் = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  unsigned முழு இலக்கம்_ranges = 0;

  unsigned முழு subset_இலக்கம்_glyphs = plan->இலக்கம்_output_glyphs ();
  எனில் (subset_இலக்கம்_glyphs == 0)
    திரும்பு உண்மை;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    auto it = hb_iter (plan->new_to_old_gid_list);
    auto _ = *it;
    க்கு (hb_codepoint_t gid = 0; gid < subset_இலக்கம்_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      எனில் (gid == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      இல்லையெனில்
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<unsigned, hb_codepoint_t> fd_range =
	likely (old_glyph < src_இலக்கம்_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<unsigned, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      எனில் (old_glyph < src_இலக்கம்_glyphs)
	fd_range.second = hb_min (fd_range.second, src_இலக்கம்_glyphs);
      unsigned fd = fd_range.first;

      எனில் (fd != prev_fd)
      {
	set.add (fd);
	இலக்கம்_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	எனில் (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_மொத்தம் = set.get_population ();
    எனில் (subset_fd_மொத்தம் == fd_மொத்தம்)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fd_மொத்தம்);
    }
    இல்லையெனில்
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      போது (set.next (&fd))
	fdmap.add (fd);
      எனில் (unlikely (fdmap.get_population () != subset_fd_மொத்தம்))
	திரும்பு பொய்;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    க்கு (unsigned முழு i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  எனில் (subset_fd_மொத்தம் > 0xFF)
  {
    எனில் (unlikely (src.format != 4))
      திரும்பு பொய்;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * இலக்கம்_ranges + HBUINT32::static_size;
  }
  இல்லையெனில்
  {
#எனில் CFF_SERIALIZE_FDSELECT_0
    unsigned முழு format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_இலக்கம்_glyphs;
#endif
    unsigned முழு format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * இலக்கம்_ranges + HBUINT16::static_size;

#எனில் CFF_SERIALIZE_FDSELECT_0
    எனில் (format0_size <= format3_size)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_size = format0_size;
    }
    இல்லையெனில்
#endif
    {
      subset_fdselect_format = 3;
      subset_fdselect_size = format3_size;
    }
  }

  திரும்பு உண்மை;
}

template <typename FDSELECT3_4>
நிலையான உள்வரி தருக்க
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			மாறிலி unsigned முழு இலக்கம்_glyphs,
			மாறிலி FDSelect &src,
			unsigned முழு size,
			மாறிலி hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (இது);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  எனில் (unlikely (!p)) return_trace (பொய்);
  p->nRanges () = fdselect_ranges.length;
  க்கு (unsigned முழு i = 0; i < fdselect_ranges.length; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = இலக்கம்_glyphs;
  return_trace (உண்மை);
}

/* Serialize a subset FDSelect format planned above. */
தருக்க
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   மாறிலி unsigned முழு இலக்கம்_glyphs,
			   மாறிலி FDSelect &src,
			   unsigned முழு fd_மொத்தம்_2,
			   unsigned முழு fdselect_format,
			   unsigned முழு size,
			   மாறிலி hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (இது);
  FDSelect *p = c->allocate_min<FDSelect> ();
  எனில் (unlikely (!p)) return_trace (பொய்);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  தேர்வு (fdselect_format)
  {
#எனில் CFF_SERIALIZE_FDSELECT_0
  நிலை 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    எனில் (unlikely (!p)) return_trace (பொய்);
    unsigned முழு range_index = 0;
    unsigned முழு fd = fdselect_ranges[range_index++].code;
    க்கு (unsigned முழு i = 0; i < இலக்கம்_glyphs; i++)
    {
      எனில் ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (உண்மை);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  நிலை 3:
    திரும்பு serialize_fdselect_3_4<FDSelect3> (c, இலக்கம்_glyphs, src,
					      size, fdselect_ranges);

  நிலை 4:
    திரும்பு serialize_fdselect_3_4<FDSelect4> (c, இலக்கம்_glyphs, src,
					      size, fdselect_ranges);

  இயல்பு:
    return_trace (பொய்);
  }
}


#endif
