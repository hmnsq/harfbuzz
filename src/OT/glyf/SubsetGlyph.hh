#که_نه_تعریف_شوی OT_GLYF_SUBSETGLYPH_HH
#تعریف_کړه OT_GLYF_SUBSETGLYPH_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {

بنية glyf_accelerator_t;

نطاق glyf_impl {


بنية SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph سرچینه_glyph;
  hb_بایټونه_t dest_پیل;  /* region of source_glyph to copy first */
  hb_بایټونه_t dest_پای_ټکی;    /* region of source_glyph to copy second */
  منطقي allocated;

  منطقي serialize (hb_serialize_context_t *c,
		  منطقي use_short_loca,
		  ثابت hb_subset_plan_t *plan) ثابت
  {
    TRACE_SERIALIZE (هذا);

    hb_بایټونه_t dest_glyph = dest_پیل.copy (c);
    hb_بایټونه_t پای_ټکی_copy = dest_پای_ټکی.copy (c);
    إذا (!پای_ټکی_copy.arrayZ || !dest_glyph.arrayZ) {
      أرجع خطأ;
    }

    dest_glyph = hb_بایټونه_t (&dest_glyph, dest_glyph.اوږدتیا + پای_ټکی_copy.اوږدتیا);
    بینښه صحيح pad_اوږدتیا = use_short_loca ? padding () : 0;
    DEBUG_پیغام (SUBSET, تش_اشاره, "serialize %u byte glyph, width %u pad %u", dest_glyph.اوږدتیا, dest_glyph.اوږدتیا + pad_اوږدتیا, pad_اوږدتیا);

    HBUINT8 pad;
    pad = 0;
    طالما (pad_اوږدتیا > 0)
    {
      (فراغ) c->embed (pad);
      pad_اوږدتیا--;
    }

    إذا (unlikely (!dest_glyph.اوږدتیا)) return_trace (ریښتیا);

    منطقي extended = سرچینه_glyph.is_extended ();

    /* update components gids. */
    لكل (خپلکاری &_ : Glyph (dest_glyph, old_gid, extended).get_مرکب_iterator ())
    {
      hb_codepoint_t new_gid;
      إذا (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<مرکب_Glyph_ریکارډ &> (_).سېټ_gid (new_gid, extended);
    }

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (extended)
    {
      خپلکاری it = Glyph (dest_glyph, old_gid, extended).get_مرکب_iterator ();
      إذا (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	حرف *p = it ? (حرف *) &*it : تش_اشاره;
	حرف *q = p;
	ثابت حرف *پای_ټکی = dest_glyph.arrayZ + dest_glyph.اوږدتیا;
	طالما (it)
	{
	  خپلکاری &rec = const_cast<مرکب_Glyph_ریکارډ &> (*it);
	  ++it;

	  q += rec.get_اندازه (extended);

	  rec.ښکتنی_gid_24_منزل_16 ();

	  بینښه اندازه = rec.get_اندازه (extended);

	  memmove (p, &rec, اندازه);

	  p += اندازه;
	}
	memmove (p, q, پای_ټکی - q);
	p += پای_ټکی - q;

	/* We want to shorten the glyph, but we can't do that without
	 * updating the length in the loca table, which is already
	 * written out :-(.  So we just fill the rest of the glyph with
	 * harmless instructions, since that's what they will be
	 * interpreted as.
	 *
	 * Should move the lowering to _populate_subset_glyphs() to
	 * fix this issue. */

	hb_memset (p, 0x7A /* TrueType instruction ROFF; harmless */, پای_ټکی - p);
	p += پای_ټکی - p;
	dest_glyph = hb_بایټونه_t (dest_glyph.arrayZ, p - (حرف *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#د_که_پای

    إذا (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    إذا (plan->flags & HB_SUBSET_FLAGS_سېټ_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).سېټ_overlaps_flag ();

    return_trace (ریښتیا);
  }

  منطقي compile_بایټونه_with_deltas (ثابت hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ثابت glyf_accelerator_t &glyf)
  {
    allocated = سرچینه_glyph.compile_بایټونه_with_deltas (plan, font, glyf, dest_پیل, dest_پای_ټکی);
    أرجع allocated;
  }

  فراغ free_compiled_بایټونه ()
  {
    إذا (likely (allocated)) {
      allocated = خطأ;
      dest_پیل.fini ();
      dest_پای_ټکی.fini ();
    }
  }

  فراغ drop_hints_بایټونه ()
  { سرچینه_glyph.drop_hints_بایټونه (dest_پیل, dest_پای_ټکی); }

  بینښه صحيح      اوږدتیا () ثابت { أرجع dest_پیل.اوږدتیا + dest_پای_ټکی.اوږدتیا; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  بینښه صحيح     padding () ثابت { أرجع اوږدتیا () % 2; }
  بینښه صحيح padded_اندازه () ثابت { أرجع اوږدتیا () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#د_که_پای /* OT_GLYF_SUBSETGLYPH_HH */
