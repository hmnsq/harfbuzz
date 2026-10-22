#నిర్వచితం_కాకపోతే OT_GLYF_SUBSETGLYPH_HH
#నిర్వచించు OT_GLYF_SUBSETGLYPH_HH


#అన్తర్భావ "../../hb-open-type.hh"


నామాకాశ OT {

నిర్మాణం glyf_accelerator_t;

నామాకాశ glyf_impl {


నిర్మాణం SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph ఆధారం_glyph;
  hb_బైట్లు_t dest_ప్రారంభం;  /* region of source_glyph to copy first */
  hb_బైట్లు_t dest_చివర;    /* region of source_glyph to copy second */
  తార్కిక allocated;

  తార్కిక serialize (hb_serialize_context_t *c,
		  తార్కిక use_short_loca,
		  స్థిరరాశి hb_subset_plan_t *plan) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    hb_బైట్లు_t dest_glyph = dest_ప్రారంభం.copy (c);
    hb_బైట్లు_t చివర_copy = dest_చివర.copy (c);
    ఒకవేళ (!చివర_copy.arrayZ || !dest_glyph.arrayZ) {
      తిరిగి అబద్ధం;
    }

    dest_glyph = hb_బైట్లు_t (&dest_glyph, dest_glyph.నిడివి + చివర_copy.నిడివి);
    గుర్తులేని పూర్ణాంకం pad_నిడివి = use_short_loca ? padding () : 0;
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "serialize %u byte glyph, width %u pad %u", dest_glyph.నిడివి, dest_glyph.నిడివి + pad_నిడివి, pad_నిడివి);

    HBUINT8 pad;
    pad = 0;
    అయితే (pad_నిడివి > 0)
    {
      (శూన్య) c->embed (pad);
      pad_నిడివి--;
    }

    ఒకవేళ (unlikely (!dest_glyph.నిడివి)) return_trace (నిజం);

    తార్కిక extended = ఆధారం_glyph.is_extended ();

    /* update components gids. */
    కొరకు (స్వయంచాలక &_ : Glyph (dest_glyph, old_gid, extended).get_సంయుక్త_iterator ())
    {
      hb_codepoint_t new_gid;
      ఒకవేళ (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<సంయుక్త_Glyph_రికార్డు &> (_).సమితి_gid (new_gid, extended);
    }

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (extended)
    {
      స్వయంచాలక it = Glyph (dest_glyph, old_gid, extended).get_సంయుక్త_iterator ();
      ఒకవేళ (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	అక్షరం *p = it ? (అక్షరం *) &*it : శూన్యసూచిక;
	అక్షరం *q = p;
	స్థిరరాశి అక్షరం *చివర = dest_glyph.arrayZ + dest_glyph.నిడివి;
	అయితే (it)
	{
	  స్వయంచాలక &rec = const_cast<సంయుక్త_Glyph_రికార్డు &> (*it);
	  ++it;

	  q += rec.get_కొలత (extended);

	  rec.క్రింది_gid_24_గమ్యం_16 ();

	  గుర్తులేని కొలత = rec.get_కొలత (extended);

	  memmove (p, &rec, కొలత);

	  p += కొలత;
	}
	memmove (p, q, చివర - q);
	p += చివర - q;

	/* We want to shorten the glyph, but we can't do that without
	 * updating the length in the loca table, which is already
	 * written out :-(.  So we just fill the rest of the glyph with
	 * harmless instructions, since that's what they will be
	 * interpreted as.
	 *
	 * Should move the lowering to _populate_subset_glyphs() to
	 * fix this issue. */

	hb_memset (p, 0x7A /* TrueType instruction ROFF; harmless */, చివర - p);
	p += చివర - p;
	dest_glyph = hb_బైట్లు_t (dest_glyph.arrayZ, p - (అక్షరం *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#అయితే_ముగింపు

    ఒకవేళ (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    ఒకవేళ (plan->flags & HB_SUBSET_FLAGS_సమితి_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).సమితి_overlaps_flag ();

    return_trace (నిజం);
  }

  తార్కిక compile_బైట్లు_with_deltas (స్థిరరాశి hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  స్థిరరాశి glyf_accelerator_t &glyf)
  {
    allocated = ఆధారం_glyph.compile_బైట్లు_with_deltas (plan, font, glyf, dest_ప్రారంభం, dest_చివర);
    తిరిగి allocated;
  }

  శూన్య free_compiled_బైట్లు ()
  {
    ఒకవేళ (likely (allocated)) {
      allocated = అబద్ధం;
      dest_ప్రారంభం.fini ();
      dest_చివర.fini ();
    }
  }

  శూన్య drop_hints_బైట్లు ()
  { ఆధారం_glyph.drop_hints_బైట్లు (dest_ప్రారంభం, dest_చివర); }

  గుర్తులేని పూర్ణాంకం      నిడివి () స్థిరరాశి { తిరిగి dest_ప్రారంభం.నిడివి + dest_చివర.నిడివి; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  గుర్తులేని పూర్ణాంకం     padding () స్థిరరాశి { తిరిగి నిడివి () % 2; }
  గుర్తులేని పూర్ణాంకం padded_కొలత () స్థిరరాశి { తిరిగి నిడివి () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#అయితే_ముగింపు /* OT_GLYF_SUBSETGLYPH_HH */
