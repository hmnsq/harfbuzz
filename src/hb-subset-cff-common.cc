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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CFF_SERIALIZE_FDSELECT_0  0

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

ਤਾਰਕਿਕ
hb_plan_subset_cff_fdselect (ਅਚਲ hb_subset_plan_t *plan,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd_ਗਿਣਤੀ,
			     ਅਚਲ FDSelect &src, /*ਵਿੱਚ*/
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ src_ਅੰਕ_glyphs,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ &subset_fd_ਗਿਣਤੀ /*ਬਾਹਰਲਾ*/,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ &subset_fdselect_ਮਾਪ /*ਬਾਹਰਲਾ*/,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ &subset_fdselect_format /*ਬਾਹਰਲਾ*/,
			     hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t> &fdselect_ranges /*ਬਾਹਰਲਾ*/,
			     hb_inc_bimap_t &fdmap /*ਬਾਹਰਲਾ*/)
{
  subset_fd_ਗਿਣਤੀ = 0;
  subset_fdselect_ਮਾਪ = 0;
  subset_fdselect_format = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ranges = 0;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subset_ਅੰਕ_glyphs = plan->ਅੰਕ_output_glyphs ();
  ਜੇ (subset_ਅੰਕ_glyphs == 0)
    ਵਾਪਸ ਸੱਚ;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_ਸੈੱਟ_t ਸੈੱਟ;
    hb_codepoint_t prev_fd = CFF_UNDEF_ਕੋਡ;
    ਸਵੈਚਾਲਿਤ it = hb_iter (plan->new_ਮੰਜ਼ਿਲ_old_gid_list);
    ਸਵੈਚਾਲਿਤ _ = *it;
    ਲਈ (hb_codepoint_t gid = 0; gid < subset_ਅੰਕ_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      ਜੇ (gid == _.ਪਹਿਲਾ)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ਨਹੀਂਤਾਂ
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_codepoint_t> fd_range =
	likely (old_glyph < src_ਅੰਕ_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      ਜੇ (old_glyph < src_ਅੰਕ_glyphs)
	fd_range.second = hb_min (fd_range.second, src_ਅੰਕ_glyphs);
      ਬਿਨਾਨਿਸ਼ਾਨੀ fd = fd_range.ਪਹਿਲਾ;

      ਜੇ (fd != prev_fd)
      {
	ਸੈੱਟ.ਜੋੜੋ (fd);
	ਅੰਕ_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (ਕੋਡ_pair_t { fd, gid });

	ਜੇ (gid == old_glyph)
	  gid = hb_min (_.ਪਹਿਲਾ - 1, fd_range.second - 1);
      }
    }

    subset_fd_ਗਿਣਤੀ = ਸੈੱਟ.get_population ();
    ਜੇ (subset_fd_ਗਿਣਤੀ == fd_ਗਿਣਤੀ)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.ਪਛਾਣ_ਤੱਤ (fd_ਗਿਣਤੀ);
    }
    ਨਹੀਂਤਾਂ
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_ਕੋਡ;
      ਜਦੋਂ (ਸੈੱਟ.next (&fd))
	fdmap.ਜੋੜੋ (fd);
      ਜੇ (unlikely (fdmap.get_population () != subset_fd_ਗਿਣਤੀ))
	ਵਾਪਸ ਝੂਠ;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < fdselect_ranges.ਲੰਮਾਈ; i++)
      fdselect_ranges[i].ਕੋਡ = fdmap[fdselect_ranges[i].ਕੋਡ];
  }

  /* determine which FDSelect format is most compact */
  ਜੇ (subset_fd_ਗਿਣਤੀ > 0xFF)
  {
    ਜੇ (unlikely (src.format != 4))
      ਵਾਪਸ ਝੂਠ;
    subset_fdselect_format = 4;
    subset_fdselect_ਮਾਪ = FDSelect::min_ਮਾਪ + FDSelect4::min_ਮਾਪ + FDSelect4_Range::static_ਮਾਪ * ਅੰਕ_ranges + HBUINT32::static_ਮਾਪ;
  }
  ਨਹੀਂਤਾਂ
  {
#ਜੇ CFF_SERIALIZE_FDSELECT_0
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ format0_ਮਾਪ = FDSelect::min_ਮਾਪ + FDSelect0::min_ਮਾਪ + HBUINT8::static_ਮਾਪ * subset_ਅੰਕ_glyphs;
#ਜੇ_ਅੰਤ
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ format3_ਮਾਪ = FDSelect::min_ਮਾਪ + FDSelect3::min_ਮਾਪ + FDSelect3_Range::static_ਮਾਪ * ਅੰਕ_ranges + HBUINT16::static_ਮਾਪ;

#ਜੇ CFF_SERIALIZE_FDSELECT_0
    ਜੇ (format0_ਮਾਪ <= format3_ਮਾਪ)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_ਮਾਪ = format0_ਮਾਪ;
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
      subset_fdselect_format = 3;
      subset_fdselect_ਮਾਪ = format3_ਮਾਪ;
    }
  }

  ਵਾਪਸ ਸੱਚ;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ FDSELECT3_4>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs,
			ਅਚਲ FDSelect &src,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ,
			ਅਚਲ hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (ਇਹ);
  FDSELECT3_4 *p = c->allocate_ਮਾਪ<FDSELECT3_4> (ਮਾਪ);
  ਜੇ (unlikely (!p)) return_trace (ਝੂਠ);
  p->nRanges () = fdselect_ranges.ਲੰਮਾਈ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < fdselect_ranges.ਲੰਮਾਈ; i++)
  {
    p->ranges[i].ਪਹਿਲਾ = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].ਕੋਡ;
  }
  p->sentinel () = ਅੰਕ_glyphs;
  return_trace (ਸੱਚ);
}

/* Serialize a subset FDSelect format planned above. */
ਤਾਰਕਿਕ
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs,
			   ਅਚਲ FDSelect &src,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd_ਗਿਣਤੀ_2,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fdselect_format,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ,
			   ਅਚਲ hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (ਇਹ);
  FDSelect *p = c->allocate_min<FDSelect> ();
  ਜੇ (unlikely (!p)) return_trace (ਝੂਠ);
  p->format = fdselect_format;
  ਮਾਪ -= FDSelect::min_ਮਾਪ;

  ਚੋਣ (fdselect_format)
  {
#ਜੇ CFF_SERIALIZE_FDSELECT_0
  ਹਾਲਤ 0:
  {
    FDSelect0 *p = c->allocate_ਮਾਪ<FDSelect0> (ਮਾਪ);
    ਜੇ (unlikely (!p)) return_trace (ਝੂਠ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ range_ਸੂਚਕਾਂਕ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = fdselect_ranges[range_ਸੂਚਕਾਂਕ++].ਕੋਡ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_glyphs; i++)
    {
      ਜੇ ((range_ਸੂਚਕਾਂਕ < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_ਸੂਚਕਾਂਕ].glyph))
      {
	fd = fdselect_ranges[range_ਸੂਚਕਾਂਕ++].ਕੋਡ;
      }
      p->fds[i] = fd;
    }
    return_trace (ਸੱਚ);
  }
#ਜੇ_ਅੰਤ /* CFF_SERIALIZE_FDSELECT_0 */

  ਹਾਲਤ 3:
    ਵਾਪਸ serialize_fdselect_3_4<FDSelect3> (c, ਅੰਕ_glyphs, src,
					      ਮਾਪ, fdselect_ranges);

  ਹਾਲਤ 4:
    ਵਾਪਸ serialize_fdselect_3_4<FDSelect4> (c, ਅੰਕ_glyphs, src,
					      ਮਾਪ, fdselect_ranges);

  ਮੂਲ:
    return_trace (ਝੂਠ);
  }
}


#ਜੇ_ਅੰਤ
