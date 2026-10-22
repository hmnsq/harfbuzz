#ifndef OT_GLYF_SUBSETGLYPH_HH
#define OT_GLYF_SUBSETGLYPH_HH


#ଅନ୍ତର୍ଭାଵ "../../hb-open-type.hh"


ନାମାକାଶ OT {

ସଂରଚନା glyf_accelerator_t;

ନାମାକାଶ glyf_impl {


ସଂରଚନା SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph source_glyph;
  hb_bytes_t dest_start;  /* region of source_glyph to copy first */
  hb_bytes_t dest_end;    /* region of source_glyph to copy second */
  ତାର୍କିକ allocated;

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ତାର୍କିକ use_short_loca,
		  ଅଚଳ hb_subset_plan_t *plan) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    hb_bytes_t dest_glyph = dest_start.copy (c);
    hb_bytes_t end_copy = dest_end.copy (c);
    ଯଦି (!end_copy.arrayZ || !dest_glyph.arrayZ) {
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    dest_glyph = hb_bytes_t (&dest_glyph, dest_glyph.length + end_copy.length);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pad_length = use_short_loca ? padding () : 0;
    DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "serialize %u byte glyph, width %u pad %u", dest_glyph.length, dest_glyph.length + pad_length, pad_length);

    HBUINT8 pad;
    pad = 0;
    ଯେତେବେଳେ (pad_length > 0)
    {
      (ଶୂନ୍ୟ) c->embed (pad);
      pad_length--;
    }

    ଯଦି (unlikely (!dest_glyph.length)) return_trace (ସତ୍ୟ);

    ତାର୍କିକ extended = source_glyph.is_extended ();

    /* update components gids. */
    ପାଇଁ (auto &_ : Glyph (dest_glyph, old_gid, extended).get_composite_iterator ())
    {
      hb_codepoint_t new_gid;
      ଯଦି (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<CompositeGlyphRecord &> (_).set_gid (new_gid, extended);
    }

#ifndef HB_NO_BEYOND_64K
    ଯଦି (extended)
    {
      auto it = Glyph (dest_glyph, old_gid, extended).get_composite_iterator ();
      ଯଦି (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	ଅକ୍ଷର *p = it ? (ଅକ୍ଷର *) &*it : nullptr;
	ଅକ୍ଷର *q = p;
	ଅଚଳ ଅକ୍ଷର *end = dest_glyph.arrayZ + dest_glyph.length;
	ଯେତେବେଳେ (it)
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
	dest_glyph = hb_bytes_t (dest_glyph.arrayZ, p - (ଅକ୍ଷର *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#endif

    ଯଦି (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    ଯଦି (plan->flags & HB_SUBSET_FLAGS_SET_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).set_overlaps_flag ();

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ compile_bytes_with_deltas (ଅଚଳ hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ଅଚଳ glyf_accelerator_t &glyf)
  {
    allocated = source_glyph.compile_bytes_with_deltas (plan, font, glyf, dest_start, dest_end);
    ଫେରନ୍ତୁ allocated;
  }

  ଶୂନ୍ୟ free_compiled_bytes ()
  {
    ଯଦି (likely (allocated)) {
      allocated = ମିଥ୍ୟା;
      dest_start.fini ();
      dest_end.fini ();
    }
  }

  ଶୂନ୍ୟ drop_hints_bytes ()
  { source_glyph.drop_hints_bytes (dest_start, dest_end); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା      length () ଅଚଳ { ଫେରନ୍ତୁ dest_start.length + dest_end.length; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା     padding () ଅଚଳ { ଫେରନ୍ତୁ length () % 2; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା padded_size () ଅଚଳ { ଫେରନ୍ତୁ length () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SUBSETGLYPH_HH */
