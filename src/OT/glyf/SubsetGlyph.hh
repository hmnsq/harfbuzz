#ifndef OT_GLYF_SUBSETGLYPH_HH
#define OT_GLYF_SUBSETGLYPH_HH


#અન્તર્ભાવ "../../hb-open-type.hh"


નામાકાશ OT {

સંરચના glyf_accelerator_t;

નામાકાશ glyf_impl {


સંરચના SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph source_glyph;
  hb_bytes_t dest_start;  /* region of source_glyph to copy first */
  hb_bytes_t dest_end;    /* region of source_glyph to copy second */
  તાર્કિક allocated;

  તાર્કિક serialize (hb_serialize_context_t *c,
		  તાર્કિક use_short_loca,
		  અચલ hb_subset_plan_t *plan) અચલ
  {
    TRACE_SERIALIZE (આ);

    hb_bytes_t dest_glyph = dest_start.copy (c);
    hb_bytes_t end_copy = dest_end.copy (c);
    જો (!end_copy.arrayZ || !dest_glyph.arrayZ) {
      પાછા ખોટું;
    }

    dest_glyph = hb_bytes_t (&dest_glyph, dest_glyph.length + end_copy.length);
    unsigned પૂર્ણાંક pad_length = use_short_loca ? padding () : 0;
    DEBUG_સંદેશ (SUBSET, nullptr, "serialize %u byte glyph, width %u pad %u", dest_glyph.length, dest_glyph.length + pad_length, pad_length);

    HBUINT8 pad;
    pad = 0;
    જ્યારે (pad_length > 0)
    {
      (શૂન્ય) c->embed (pad);
      pad_length--;
    }

    જો (unlikely (!dest_glyph.length)) return_trace (સાચું);

    તાર્કિક extended = source_glyph.is_extended ();

    /* update components gids. */
    માટે (auto &_ : Glyph (dest_glyph, old_gid, extended).get_composite_iterator ())
    {
      hb_codepoint_t new_gid;
      જો (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<CompositeGlyphRecord &> (_).set_gid (new_gid, extended);
    }

#ifndef HB_NO_BEYOND_64K
    જો (extended)
    {
      auto it = Glyph (dest_glyph, old_gid, extended).get_composite_iterator ();
      જો (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	અક્ષર *p = it ? (અક્ષર *) &*it : nullptr;
	અક્ષર *q = p;
	અચલ અક્ષર *end = dest_glyph.arrayZ + dest_glyph.length;
	જ્યારે (it)
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
	dest_glyph = hb_bytes_t (dest_glyph.arrayZ, p - (અક્ષર *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#endif

    જો (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    જો (plan->flags & HB_SUBSET_FLAGS_SET_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).set_overlaps_flag ();

    return_trace (સાચું);
  }

  તાર્કિક compile_bytes_with_deltas (અચલ hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  અચલ glyf_accelerator_t &glyf)
  {
    allocated = source_glyph.compile_bytes_with_deltas (plan, font, glyf, dest_start, dest_end);
    પાછા allocated;
  }

  શૂન્ય free_compiled_bytes ()
  {
    જો (likely (allocated)) {
      allocated = ખોટું;
      dest_start.fini ();
      dest_end.fini ();
    }
  }

  શૂન્ય drop_hints_bytes ()
  { source_glyph.drop_hints_bytes (dest_start, dest_end); }

  unsigned પૂર્ણાંક      length () અચલ { પાછા dest_start.length + dest_end.length; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  unsigned પૂર્ણાંક     padding () અચલ { પાછા length () % 2; }
  unsigned પૂર્ણાંક padded_size () અચલ { પાછા length () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SUBSETGLYPH_HH */
