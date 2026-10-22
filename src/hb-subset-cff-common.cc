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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF

#అన్తర్భావ "hb-ot-cff-common.hh"
#అన్తర్భావ "hb-ot-cff2-table.hh"
#అన్తర్భావ "hb-subset-cff-common.hh"

/* Disable FDSelect format 0 for compatibility with fonttools which doesn't seem choose it.
 * Rarely any/much smaller than format 3 anyway. */
#నిర్వచించు CFF_SERIALIZE_FDSELECT_0  0

ఉపయోగించి నామాకాశ CFF;


/* Determine an optimal FDSelect format according to a provided plan.
 *
 * Return value: FDSelect format, size, and ranges for the most compact subset FDSelect
 * along with a font index remapping table
 */

తార్కిక
hb_plan_subset_cff_fdselect (స్థిరరాశి hb_subset_plan_t *plan,
			     గుర్తులేని పూర్ణాంకం fd_లెక్క,
			     స్థిరరాశి FDSelect &src, /*లోపల*/
			     గుర్తులేని పూర్ణాంకం src_అంకె_glyphs,
			     గుర్తులేని పూర్ణాంకం &subset_fd_లెక్క /*బహిర్గతం*/,
			     గుర్తులేని పూర్ణాంకం &subset_fdselect_కొలత /*బహిర్గతం*/,
			     గుర్తులేని పూర్ణాంకం &subset_fdselect_format /*బహిర్గతం*/,
			     hb_సదిశ_t<సంకేతం_pair_t> &fdselect_ranges /*బహిర్గతం*/,
			     hb_inc_bimap_t &fdmap /*బహిర్గతం*/)
{
  subset_fd_లెక్క = 0;
  subset_fdselect_కొలత = 0;
  subset_fdselect_format = 0;
  గుర్తులేని పూర్ణాంకం అంకె_ranges = 0;

  గుర్తులేని పూర్ణాంకం subset_అంకె_glyphs = plan->అంకె_output_glyphs ();
  ఒకవేళ (subset_అంకె_glyphs == 0)
    తిరిగి నిజం;

  {
    /* use hb_set to determine the subset of font dicts */
    hb_సమితి_t సమితి;
    hb_codepoint_t prev_fd = CFF_UNDEF_సంకేతం;
    స్వయంచాలక it = hb_iter (plan->new_గమ్యం_old_gid_list);
    స్వయంచాలక _ = *it;
    కొరకు (hb_codepoint_t gid = 0; gid < subset_అంకె_glyphs; gid++)
    {
      hb_codepoint_t old_glyph;
      ఒకవేళ (gid == _.మొదటి)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      లేకపోతే
      {
	/* fonttools retains FDSelect & font dicts for missing glyphs. do the same */
	old_glyph = gid;
      }
      /* FDSelect may only cover the CharStrings glyphs; see subset_get_fd (). */
      hb_pair_t<గుర్తులేని, hb_codepoint_t> fd_range =
	likely (old_glyph < src_అంకె_glyphs) ?
	src.get_fd_range (old_glyph) :
	hb_pair_t<గుర్తులేని, hb_codepoint_t> {0, HB_CODEPOINT_INVALID};
      ఒకవేళ (old_glyph < src_అంకె_glyphs)
	fd_range.second = hb_min (fd_range.second, src_అంకె_glyphs);
      గుర్తులేని fd = fd_range.మొదటి;

      ఒకవేళ (fd != prev_fd)
      {
	సమితి.కూడు (fd);
	అంకె_ranges++;
	prev_fd = fd;
	fdselect_ranges.push (సంకేతం_pair_t { fd, gid });

	ఒకవేళ (gid == old_glyph)
	  gid = hb_min (_.మొదటి - 1, fd_range.second - 1);
      }
    }

    subset_fd_లెక్క = సమితి.get_population ();
    ఒకవేళ (subset_fd_లెక్క == fd_లెక్క)
    {
      /* all font dicts belong to the subset. no need to subset FDSelect & FDArray */
      fdmap.తటస్థం (fd_లెక్క);
    }
    లేకపోతే
    {
      /* create a fdmap */
      fdmap.reset ();

      hb_codepoint_t fd = CFF_UNDEF_సంకేతం;
      అయితే (సమితి.next (&fd))
	fdmap.కూడు (fd);
      ఒకవేళ (unlikely (fdmap.get_population () != subset_fd_లెక్క))
	తిరిగి అబద్ధం;
    }

    /* update each font dict index stored as "code" in fdselect_ranges */
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < fdselect_ranges.నిడివి; i++)
      fdselect_ranges[i].సంకేతం_2 = fdmap[fdselect_ranges[i].సంకేతం_2];
  }

  /* determine which FDSelect format is most compact */
  ఒకవేళ (subset_fd_లెక్క > 0xFF)
  {
    ఒకవేళ (unlikely (src.format != 4))
      తిరిగి అబద్ధం;
    subset_fdselect_format = 4;
    subset_fdselect_కొలత = FDSelect::min_కొలత + FDSelect4::min_కొలత + FDSelect4_Range::static_కొలత * అంకె_ranges + HBUINT32::static_కొలత;
  }
  లేకపోతే
  {
#ఒకవేళ CFF_SERIALIZE_FDSELECT_0
    గుర్తులేని పూర్ణాంకం format0_కొలత = FDSelect::min_కొలత + FDSelect0::min_కొలత + HBUINT8::static_కొలత * subset_అంకె_glyphs;
#అయితే_ముగింపు
    గుర్తులేని పూర్ణాంకం format3_కొలత = FDSelect::min_కొలత + FDSelect3::min_కొలత + FDSelect3_Range::static_కొలత * అంకె_ranges + HBUINT16::static_కొలత;

#ఒకవేళ CFF_SERIALIZE_FDSELECT_0
    ఒకవేళ (format0_కొలత <= format3_కొలత)
    {
      // subset_fdselect_format = 0;
      subset_fdselect_కొలత = format0_కొలత;
    }
    లేకపోతే
#అయితే_ముగింపు
    {
      subset_fdselect_format = 3;
      subset_fdselect_కొలత = format3_కొలత;
    }
  }

  తిరిగి నిజం;
}

మూస <రకం_పేరు FDSELECT3_4>
స్థిర అంతఃవరుస తార్కిక
serialize_fdselect_3_4 (hb_serialize_context_t *c,
			స్థిరరాశి గుర్తులేని పూర్ణాంకం అంకె_glyphs,
			స్థిరరాశి FDSelect &src,
			గుర్తులేని పూర్ణాంకం కొలత,
			స్థిరరాశి hb_సదిశ_t<సంకేతం_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (ఇది);
  FDSELECT3_4 *p = c->allocate_కొలత<FDSELECT3_4> (కొలత);
  ఒకవేళ (unlikely (!p)) return_trace (అబద్ధం);
  p->nRanges () = fdselect_ranges.నిడివి;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < fdselect_ranges.నిడివి; i++)
  {
    p->ranges[i].మొదటి = fdselect_ranges[i].glyph;
    p->ranges[i].fd = fdselect_ranges[i].సంకేతం_2;
  }
  p->sentinel () = అంకె_glyphs;
  return_trace (నిజం);
}

/* Serialize a subset FDSelect format planned above. */
తార్కిక
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			   స్థిరరాశి గుర్తులేని పూర్ణాంకం అంకె_glyphs,
			   స్థిరరాశి FDSelect &src,
			   గుర్తులేని పూర్ణాంకం fd_లెక్క_2,
			   గుర్తులేని పూర్ణాంకం fdselect_format,
			   గుర్తులేని పూర్ణాంకం కొలత,
			   స్థిరరాశి hb_సదిశ_t<సంకేతం_pair_t> &fdselect_ranges)
{
  TRACE_SERIALIZE (ఇది);
  FDSelect *p = c->allocate_min<FDSelect> ();
  ఒకవేళ (unlikely (!p)) return_trace (అబద్ధం);
  p->format = fdselect_format;
  కొలత -= FDSelect::min_కొలత;

  ఎంపిక (fdselect_format)
  {
#ఒకవేళ CFF_SERIALIZE_FDSELECT_0
  స్థితి 0:
  {
    FDSelect0 *p = c->allocate_కొలత<FDSelect0> (కొలత);
    ఒకవేళ (unlikely (!p)) return_trace (అబద్ధం);
    గుర్తులేని పూర్ణాంకం range_సూచీ = 0;
    గుర్తులేని పూర్ణాంకం fd = fdselect_ranges[range_సూచీ++].సంకేతం_2;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_glyphs; i++)
    {
      ఒకవేళ ((range_సూచీ < fdselect_ranges.len) &&
	  (i >= fdselect_ranges[range_సూచీ].glyph))
      {
	fd = fdselect_ranges[range_సూచీ++].సంకేతం_2;
      }
      p->fds[i] = fd;
    }
    return_trace (నిజం);
  }
#అయితే_ముగింపు /* CFF_SERIALIZE_FDSELECT_0 */

  స్థితి 3:
    తిరిగి serialize_fdselect_3_4<FDSelect3> (c, అంకె_glyphs, src,
					      కొలత, fdselect_ranges);

  స్థితి 4:
    తిరిగి serialize_fdselect_3_4<FDSelect4> (c, అంకె_glyphs, src,
					      కొలత, fdselect_ranges);

  అప్రమేయ:
    return_trace (అబద్ధం);
  }
}


#అయితే_ముగింపు
