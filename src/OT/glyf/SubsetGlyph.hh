#ifndef OT_GLYF_SUBSETGLYPH_HH
#define OT_GLYF_SUBSETGLYPH_HH


#অন্তর্ভাব "../../hb-open-type.hh"


নামাকাশ OT {

সংরচনা glyf_accelerator_t;

নামাকাশ glyf_impl {


সংরচনা SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph source_glyph;
  hb_bytes_t dest_start;  /* region of source_glyph to copy first */
  hb_bytes_t dest_end;    /* region of source_glyph to copy second */
  তার্কিক allocated;

  তার্কিক serialize (hb_serialize_context_t *c,
		  তার্কিক use_short_loca,
		  ধ্রুব hb_subset_plan_t *plan) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);

    hb_bytes_t dest_glyph = dest_start.copy (c);
    hb_bytes_t end_copy = dest_end.copy (c);
    যদি (!end_copy.arrayZ || !dest_glyph.arrayZ) {
      প্রত্যাগম অসত্য;
    }

    dest_glyph = hb_bytes_t (&dest_glyph, dest_glyph.length + end_copy.length);
    unsigned পূর্ণাঙ্ক pad_length = use_short_loca ? padding () : 0;
    DEBUG_বার্তা (SUBSET, nullptr, "serialize %u byte glyph, width %u pad %u", dest_glyph.length, dest_glyph.length + pad_length, pad_length);

    HBUINT8 pad;
    pad = 0;
    যাবত্ (pad_length > 0)
    {
      (শূন্য) c->embed (pad);
      pad_length--;
    }

    যদি (unlikely (!dest_glyph.length)) return_trace (সত্য);

    তার্কিক extended = source_glyph.is_extended ();

    /* update components gids. */
    প্রতি (auto &_ : Glyph (dest_glyph, old_gid, extended).get_composite_iterator ())
    {
      hb_codepoint_t new_gid;
      যদি (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<CompositeGlyphRecord &> (_).set_gid (new_gid, extended);
    }

#ifndef HB_NO_BEYOND_64K
    যদি (extended)
    {
      auto it = Glyph (dest_glyph, old_gid, extended).get_composite_iterator ();
      যদি (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	বর্ণ *p = it ? (বর্ণ *) &*it : nullptr;
	বর্ণ *q = p;
	ধ্রুব বর্ণ *end = dest_glyph.arrayZ + dest_glyph.length;
	যাবত্ (it)
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
	dest_glyph = hb_bytes_t (dest_glyph.arrayZ, p - (বর্ণ *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#endif

    যদি (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    যদি (plan->flags & HB_SUBSET_FLAGS_SET_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).set_overlaps_flag ();

    return_trace (সত্য);
  }

  তার্কিক compile_bytes_with_deltas (ধ্রুব hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ধ্রুব glyf_accelerator_t &glyf)
  {
    allocated = source_glyph.compile_bytes_with_deltas (plan, font, glyf, dest_start, dest_end);
    প্রত্যাগম allocated;
  }

  শূন্য free_compiled_bytes ()
  {
    যদি (likely (allocated)) {
      allocated = অসত্য;
      dest_start.fini ();
      dest_end.fini ();
    }
  }

  শূন্য drop_hints_bytes ()
  { source_glyph.drop_hints_bytes (dest_start, dest_end); }

  unsigned পূর্ণাঙ্ক      length () ধ্রুব { প্রত্যাগম dest_start.length + dest_end.length; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  unsigned পূর্ণাঙ্ক     padding () ধ্রুব { প্রত্যাগম length () % 2; }
  unsigned পূর্ণাঙ্ক padded_size () ধ্রুব { প্রত্যাগম length () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SUBSETGLYPH_HH */
