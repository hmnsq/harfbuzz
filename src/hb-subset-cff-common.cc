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
hb_plan_subset_cff_fdselect (स्थिरांक hb_subset_plan_t *plan,
			     अचिन्हित पूर्णांक fd_मोजणी,
			     स्थिरांक FDSelect &src, /* IN */
			     अचिन्हित पूर्णांक src_अंक_glyphs,
			     अचिन्हित पूर्णांक &subset_fd_मोजणी /* OUT */,
			     अचिन्हित पूर्णांक &subset_fdselect_size /* OUT */,
			     अचिन्हित पूर्णांक &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_मोजणी = 0;
  subset_fdselect_size = 0;
  subset_fdselect_format = 0;
  अचिन्हित पूर्णांक अंक_ranges = 0;

  अचिन्हित पूर्णांक subset_अंक_glyphs = plan->अंक_output_glyphs ();
  जर (subset_अंक_glyphs == 0)
    परत सत्य;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t prev_fd = CFF_UNDEF_CODE;
    स्वतः it = hb_iter (plan->new_to_old_gid_list);
    स्वतः _ = *it;
    साठी (hb_codepoint_t gid = 0; gid < subset_अंक_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      जर (gid == _.first)
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
      hb_pair_t<अचिन्हित, hb_codepoint_t> fd_range =
	likely (old_glyph < src_अंक_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<अचिन्हित, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      जर (old_glyph < src_अंक_glyphs)
	fd_range.second = hb_min (fd_range.second, src_अंक_glyphs);
      अचिन्हित fd = fd_range.first;

      जर (fd != prev_fd)
      {
	set.add (fd);
	अंक_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	जर (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_मोजणी = set.get_population ();
    जर (subset_fd_मोजणी == fd_मोजणी)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fd_मोजणी);
    }
    अन्यथा
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      पर्यंत (set.next (&fd))
	fdmap.add (fd);
      जर (unlikely (fdmap.get_population () != subset_fd_मोजणी))
	परत असत्य;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    साठी (अचिन्हित पूर्णांक i = 0; i < fdselect_ranges.length; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  जर (subset_fd_मोजणी > 0xFF)
  {
    जर (unlikely (src.format != 4))
      परत असत्य;
    subset_fdselect_format = 4;
    subset_fdselect_size = FDSelect::min_size + FDSelect4::min_size + FDSelect4_Range::static_size * अंक_ranges + HBUINT32::static_size;
  }
  अन्यथा
  {
#जर CFF_SERIALIZE_FDSELECT_0
    अचिन्हित पूर्णांक format0_size = FDSelect::min_size + FDSelect0::min_size + HBUINT8::static_size * subset_अंक_glyphs;
#endif
    अचिन्हित पूर्णांक format3_size = FDSelect::min_size + FDSelect3::min_size + FDSelect3_Range::static_size * अंक_ranges + HBUINT16::static_size;

#जर CFF_SERIALIZE_FDSELECT_0
    जर (format0_size <= format3_size)
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

  परत सत्य;
}

ढाँचा <typename FDSELECT3_4>
स्थिर ओळीत तार्किक
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			स्थिरांक अचिन्हित पूर्णांक अंक_glyphs,
			स्थिरांक FDSelect &src,
			अचिन्हित पूर्णांक size,
			स्थिरांक hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (हे);
  FDSELECT3_4 *p = c->allocate_size<FDSELECT3_4> (size);
  जर (unlikely (!p)) return_trace (असत्य);
  p->nRanges () = fdselect_ranges.length;
  साठी (अचिन्हित पूर्णांक i = 0; i < fdselect_ranges.length; i++)
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
			   स्थिरांक अचिन्हित पूर्णांक अंक_glyphs,
			   स्थिरांक FDSelect &src,
			   अचिन्हित पूर्णांक fd_मोजणी_2,
			   अचिन्हित पूर्णांक fdselect_format,
			   अचिन्हित पूर्णांक size,
			   स्थिरांक hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (हे);
  FDSelect *p = c->allocate_min<FDSelect> ();
  जर (unlikely (!p)) return_trace (असत्य);
  p->format = fdselect_format;
  size -= FDSelect::min_size;

  निवड (fdselect_format)
  {
#जर CFF_SERIALIZE_FDSELECT_0
  स्थिती 0:
  {
    FDSelect0 *p = c->allocate_size<FDSelect0> (size);
    जर (unlikely (!p)) return_trace (असत्य);
    अचिन्हित पूर्णांक range_index = 0;
    अचिन्हित पूर्णांक fd = fdselect_ranges[range_index++].code;
    साठी (अचिन्हित पूर्णांक i = 0; i < अंक_glyphs; i++)
    {
      जर ((range_index < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_index].glyph))
      {
	fd = fdselect_ranges[range_index++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (सत्य);
  }
#endif /* CFF_SERIALIZE_FDSELECT_0 */

  स्थिती 3:
    परत serialize_fdselect_3_4<FDSelect3> (c, अंक_glyphs, src,
					      size, fdselect_ranges);

  स्थिती 4:
    परत serialize_fdselect_3_4<FDSelect4> (c, अंक_glyphs, src,
					      size, fdselect_ranges);

  पूर्वनिर्धारित:
    return_trace (असत्य);
  }
}


#endif
