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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_SUBSET_CFF

#समावेश "hb-ot-cff-common.hh"
#समावेश "hb-ot-cff2-table.hh"
#समावेश "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#घोषणा CFF_SERIALIZE_FDSELECT_0  0

using namespace CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

bool
hb_plan_subset_cff_fdselect (स्थिर hb_subset_plan_t *plan,
			     अचिन्हित पूर्णांक fd_गिनती,
			     स्थिर FDSelect &src, /* IN */
			     अचिन्हित पूर्णांक src_अंक_glyphs,
			     अचिन्हित पूर्णांक &subset_fd_गिनती /* OUT */,
			     अचिन्हित पूर्णांक &subset_fdselect_आकार /* OUT */,
			     अचिन्हित पूर्णांक &subset_fdselect_format /* OUT */,
			     hb_vector_t<code_pair_t> &fdselect_ranges /* OUT */,
			     hb_inc_bimap_t &fdmap /* OUT */)
{
  subset_fd_गिनती = 0;
  subset_fdselect_आकार = 0;
  subset_fdselect_format = 0;
  अचिन्हित पूर्णांक अंक_ranges = 0;

  अचिन्हित पूर्णांक subset_अंक_glyphs = plan->अंक_output_glyphs ();
  अगर (subset_अंक_glyphs == 0)
    वापस true;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_set_t set;
    hb_codepoint_t पूर्व_fd = CFF_UNDEF_CODE;
    स्वतः it = hb_iter (plan->new_to_old_gid_list);
    स्वतः _ = *it;
    क्रम (hb_codepoint_t gid = 0; gid < subset_अंक_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      अगर (gid == _.first)
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
      अगर (old_glyph < src_अंक_glyphs)
	fd_range.second = hb_min (fd_range.second, src_अंक_glyphs);
      अचिन्हित fd = fd_range.first;

      अगर (fd != पूर्व_fd)
      {
	set.add (fd);
	अंक_ranges++;
	पूर्व_fd = fd;
	fdselect_ranges.push (code_pair_t { fd, gid });

	अगर (gid == old_glyph)
	  gid = hb_min (_.first - 1, fd_range.second - 1);
      }
    }

    subset_fd_गिनती = set.get_population ();
    अगर (subset_fd_गिनती == fd_गिनती)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.identity (fd_गिनती);
    }
    अन्यथा
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_CODE;
      जबतक (set.next (&fd))
	fdmap.add (fd);
      अगर (unlikely (fdmap.get_population () != subset_fd_गिनती))
	वापस false;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    क्रम (अचिन्हित पूर्णांक i = 0; i < fdselect_ranges.लंबाई; i++)
      fdselect_ranges[i].code = fdmap[fdselect_ranges[i].code];
  }

  /* determine which FDSelect format is most compact */
  अगर (subset_fd_गिनती > 0xFF)
  {
    अगर (unlikely (src.format != 4))
      वापस false;
    subset_fdselect_format = 4;
    subset_fdselect_आकार = FDSelect::min_आकार + FDSelect4::min_आकार + FDSelect4_Range::static_आकार * अंक_ranges + HBUINT32::static_आकार;
  }
  अन्यथा
  {
#अगर CFF_SERIALIZE_FDSELECT_0
    अचिन्हित पूर्णांक format0_आकार = FDSelect::min_आकार + FDSelect0::min_आकार + HBUINT8::static_आकार * subset_अंक_glyphs;
#पूर्ण_अगर
    अचिन्हित पूर्णांक format3_आकार = FDSelect::min_आकार + FDSelect3::min_आकार + FDSelect3_Range::static_आकार * अंक_ranges + HBUINT16::static_आकार;

#अगर CFF_SERIALIZE_FDSELECT_0
    अगर (format0_आकार <= format3_आकार)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_आकार = format0_आकार;
    }
    अन्यथा
#पूर्ण_अगर
    {
      subset_fdselect_format = 3;
      subset_fdselect_आकार = format3_आकार;
    }
  }

  वापस true;
}

ढाँचा <typename FDSELECT3_4>
अटल अंतरभूत bool
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			स्थिर अचिन्हित पूर्णांक अंक_glyphs,
			स्थिर FDSelect &src,
			अचिन्हित पूर्णांक आकार,
			स्थिर hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (यह);
  FDSELECT3_4 *p = c->allocate_आकार<FDSELECT3_4> (आकार);
  अगर (unlikely (!p)) return_trace (false);
  p->nRanges () = fdselect_ranges.लंबाई;
  क्रम (अचिन्हित पूर्णांक i = 0; i < fdselect_ranges.लंबाई; i++)
  {
    p->ranges[i].first = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].code;
  }
  p->sentinel () = अंक_glyphs;
  return_trace (true);
}

/* Serialize a subset FDSelect format planned above. */
bool
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   स्थिर अचिन्हित पूर्णांक अंक_glyphs,
			   स्थिर FDSelect &src,
			   अचिन्हित पूर्णांक fd_गिनती_2,
			   अचिन्हित पूर्णांक fdselect_format,
			   अचिन्हित पूर्णांक आकार,
			   स्थिर hb_vector_t<code_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (यह);
  FDSelect *p = c->allocate_min<FDSelect> ();
  अगर (unlikely (!p)) return_trace (false);
  p->format = fdselect_format;
  आकार -= FDSelect::min_आकार;

  चयन (fdselect_format)
  {
#अगर CFF_SERIALIZE_FDSELECT_0
  हाल 0:
  {
    FDSelect0 *p = c->allocate_आकार<FDSelect0> (आकार);
    अगर (unlikely (!p)) return_trace (false);
    अचिन्हित पूर्णांक range_सूचकांक = 0;
    अचिन्हित पूर्णांक fd = fdselect_ranges[range_सूचकांक++].code;
    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_glyphs; i++)
    {
      अगर ((range_सूचकांक < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_सूचकांक].glyph))
      {
	fd = fdselect_ranges[range_सूचकांक++].code;
      }
      p->fds[i] = fd;
    }
    return_trace (true);
  }
#पूर्ण_अगर /* CFF_SERIALIZE_FDSELECT_0 */

  हाल 3:
    वापस serialize_fdselect_3_4<FDSelect3> (c, अंक_glyphs, src,
					      आकार, fdselect_ranges);

  हाल 4:
    वापस serialize_fdselect_3_4<FDSelect4> (c, अंक_glyphs, src,
					      आकार, fdselect_ranges);

  शेष:
    return_trace (false);
  }
}


#पूर्ण_अगर
