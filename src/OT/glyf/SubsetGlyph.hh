#ifndef OT_GLYF_SUBSETGLYPH_HH
#define OT_GLYF_SUBSETGLYPH_HH


#அந்தர்பாவ "../../hb-open-type.hh"


நாமாகாஶ OT {

கட்டமைப்பு glyf_accelerator_t;

நாமாகாஶ glyf_impl {


கட்டமைப்பு SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph source_glyph;
  hb_bytes_t dest_start;  /* region of source_glyph to copy first */
  hb_bytes_t dest_end;    /* region of source_glyph to copy second */
  தருக்க allocated;

  தருக்க serialize (hb_serialize_context_t *c,
		  தருக்க use_short_loca,
		  மாறிலி hb_subset_plan_t *plan) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    hb_bytes_t dest_glyph = dest_start.copy (c);
    hb_bytes_t end_copy = dest_end.copy (c);
    எனில் (!end_copy.arrayZ || !dest_glyph.arrayZ) {
      திரும்பு பொய்;
    }

    dest_glyph = hb_bytes_t (&dest_glyph, dest_glyph.length + end_copy.length);
    unsigned முழு pad_length = use_short_loca ? padding () : 0;
    DEBUG_செய்தி (SUBSET, nullptr, "serialize %u byte glyph, width %u pad %u", dest_glyph.length, dest_glyph.length + pad_length, pad_length);

    HBUINT8 pad;
    pad = 0;
    போது (pad_length > 0)
    {
      (வெறுமை) c->embed (pad);
      pad_length--;
    }

    எனில் (unlikely (!dest_glyph.length)) return_trace (உண்மை);

    தருக்க extended = source_glyph.is_extended ();

    /* update components gids. */
    க்கு (auto &_ : Glyph (dest_glyph, old_gid, extended).get_composite_iterator ())
    {
      hb_codepoint_t new_gid;
      எனில் (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<CompositeGlyphRecord &> (_).set_gid (new_gid, extended);
    }

#ifndef HB_NO_BEYOND_64K
    எனில் (extended)
    {
      auto it = Glyph (dest_glyph, old_gid, extended).get_composite_iterator ();
      எனில் (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	எழுத்து *p = it ? (எழுத்து *) &*it : nullptr;
	எழுத்து *q = p;
	மாறிலி எழுத்து *end = dest_glyph.arrayZ + dest_glyph.length;
	போது (it)
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
	dest_glyph = hb_bytes_t (dest_glyph.arrayZ, p - (எழுத்து *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#endif

    எனில் (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    எனில் (plan->flags & HB_SUBSET_FLAGS_SET_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).set_overlaps_flag ();

    return_trace (உண்மை);
  }

  தருக்க compile_bytes_with_deltas (மாறிலி hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  மாறிலி glyf_accelerator_t &glyf)
  {
    allocated = source_glyph.compile_bytes_with_deltas (plan, font, glyf, dest_start, dest_end);
    திரும்பு allocated;
  }

  வெறுமை free_compiled_bytes ()
  {
    எனில் (likely (allocated)) {
      allocated = பொய்;
      dest_start.fini ();
      dest_end.fini ();
    }
  }

  வெறுமை drop_hints_bytes ()
  { source_glyph.drop_hints_bytes (dest_start, dest_end); }

  unsigned முழு      length () மாறிலி { திரும்பு dest_start.length + dest_end.length; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  unsigned முழு     padding () மாறிலி { திரும்பு length () % 2; }
  unsigned முழு padded_size () மாறிலி { திரும்பு length () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SUBSETGLYPH_HH */
