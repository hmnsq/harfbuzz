#ifndef OT_GLYF_SUBSETGLYPH_HH
#define OT_GLYF_SUBSETGLYPH_HH


#അന്തര്ഭാവ "../../hb-open-type.hh"


നാമാകാശ OT {

ഘടന glyf_accelerator_t;

നാമാകാശ glyf_impl {


ഘടന SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph source_glyph;
  hb_bytes_t dest_start;  /* region of source_glyph to copy first */
  hb_bytes_t dest_end;    /* region of source_glyph to copy second */
  തർക്കം allocated;

  തർക്കം serialize (hb_serialize_context_t *c,
		  തർക്കം use_short_loca,
		  സ്ഥിരം hb_subset_plan_t *plan) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    hb_bytes_t dest_glyph = dest_start.copy (c);
    hb_bytes_t end_copy = dest_end.copy (c);
    എങ്കിൽ (!end_copy.arrayZ || !dest_glyph.arrayZ) {
      തിരികെ അസത്യം;
    }

    dest_glyph = hb_bytes_t (&dest_glyph, dest_glyph.length + end_copy.length);
    unsigned പൂർണ്ണസംഖ്യ pad_length = use_short_loca ? padding () : 0;
    DEBUG_സന്ദേശം (SUBSET, nullptr, "serialize %u byte glyph, width %u pad %u", dest_glyph.length, dest_glyph.length + pad_length, pad_length);

    HBUINT8 pad;
    pad = 0;
    ആകുമ്പോൾ (pad_length > 0)
    {
      (ശൂന്യം) c->embed (pad);
      pad_length--;
    }

    എങ്കിൽ (unlikely (!dest_glyph.length)) return_trace (സത്യം);

    തർക്കം extended = source_glyph.is_extended ();

    /* update components gids. */
    വേണ്ടി (auto &_ : Glyph (dest_glyph, old_gid, extended).get_composite_iterator ())
    {
      hb_codepoint_t new_gid;
      എങ്കിൽ (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<CompositeGlyphRecord &> (_).set_gid (new_gid, extended);
    }

#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (extended)
    {
      auto it = Glyph (dest_glyph, old_gid, extended).get_composite_iterator ();
      എങ്കിൽ (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	അക്ഷരം *p = it ? (അക്ഷരം *) &*it : nullptr;
	അക്ഷരം *q = p;
	സ്ഥിരം അക്ഷരം *end = dest_glyph.arrayZ + dest_glyph.length;
	ആകുമ്പോൾ (it)
	{
	  auto &rec = const_cast<CompositeGlyphRecord &> (*it);
	  ++it;

	  q += rec.get_size (extended);

	  rec.lower_gid_24_to_16 ();

	  unsigned size = rec.get_size (extended);

	  memmove (p, &rec, size);

	  p += size;
	}
	memmove (p, q, end - q);
	p += end - q;

	/* We want to shorten the glyph, but we can't do that without
	 * updating the length in the loca table, which is already
	 * written out :-(.  So we just fill the rest of the glyph with
	 * harmless instructions, since that's what they will be
	 * interpreted as.
	 *
	 * Should move the lowering to _populate_subset_glyphs() to
	 * fix this issue. */

	hb_memset (p, 0x7A /* TrueType instruction ROFF; harmless */, end - p);
	p += end - p;
	dest_glyph = hb_bytes_t (dest_glyph.arrayZ, p - (അക്ഷരം *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#endif

    എങ്കിൽ (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    എങ്കിൽ (plan->flags & HB_SUBSET_FLAGS_SET_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).set_overlaps_flag ();

    return_trace (സത്യം);
  }

  തർക്കം compile_bytes_with_deltas (സ്ഥിരം hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  സ്ഥിരം glyf_accelerator_t &glyf)
  {
    allocated = source_glyph.compile_bytes_with_deltas (plan, font, glyf, dest_start, dest_end);
    തിരികെ allocated;
  }

  ശൂന്യം free_compiled_bytes ()
  {
    എങ്കിൽ (likely (allocated)) {
      allocated = അസത്യം;
      dest_start.fini ();
      dest_end.fini ();
    }
  }

  ശൂന്യം drop_hints_bytes ()
  { source_glyph.drop_hints_bytes (dest_start, dest_end); }

  unsigned പൂർണ്ണസംഖ്യ      length () സ്ഥിരം { തിരികെ dest_start.length + dest_end.length; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  unsigned പൂർണ്ണസംഖ്യ     padding () സ്ഥിരം { തിരികെ length () % 2; }
  unsigned പൂർണ്ണസംഖ്യ padded_size () സ്ഥിരം { തിരികെ length () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SUBSETGLYPH_HH */
