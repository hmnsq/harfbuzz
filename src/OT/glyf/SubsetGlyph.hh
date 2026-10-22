#ifndef OT_GLYF_SUBSETGLYPH_HH
#define OT_GLYF_SUBSETGLYPH_HH


#অন্তর্ভাব "../../hb-open-type.hh"


নামাকাশ OT {

কাঠামো glyf_accelerator_t;

নামাকাশ glyf_impl {


কাঠামো SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph source_glyph;
  hb_bytes_t dest_start;  /* region of source_glyph to copy first */
  hb_bytes_t dest_end;    /* region of source_glyph to copy second */
  যৌক্তিক allocated;

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  যৌক্তিক use_short_loca,
		  ধ্রুবক hb_subset_plan_t *plan) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    hb_bytes_t dest_glyph = dest_start.copy (c);
    hb_bytes_t end_copy = dest_end.copy (c);
    যদি (!end_copy.arrayZ || !dest_glyph.arrayZ) {
      ফেরত মিথ্যা;
    }

    dest_glyph = hb_bytes_t (&dest_glyph, dest_glyph.দৈর্ঘ্য + end_copy.দৈর্ঘ্য);
    unsigned পূর্ণসংখ্যা pad_দৈর্ঘ্য = use_short_loca ? padding () : 0;
    DEBUG_বার্তা (SUBSET, nullptr, "serialize %u byte glyph, width %u pad %u", dest_glyph.দৈর্ঘ্য, dest_glyph.দৈর্ঘ্য + pad_দৈর্ঘ্য, pad_দৈর্ঘ্য);

    HBUINT8 pad;
    pad = 0;
    যখন (pad_দৈর্ঘ্য > 0)
    {
      (শূন্য) c->embed (pad);
      pad_দৈর্ঘ্য--;
    }

    যদি (unlikely (!dest_glyph.দৈর্ঘ্য)) return_trace (সত্য);

    যৌক্তিক extended = source_glyph.is_extended ();

    /* update components gids. */
    জন্য (auto &_ : Glyph (dest_glyph, old_gid, extended).get_composite_iterator ())
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
	অক্ষর *p = it ? (অক্ষর *) &*it : nullptr;
	অক্ষর *q = p;
	ধ্রুবক অক্ষর *end = dest_glyph.arrayZ + dest_glyph.দৈর্ঘ্য;
	যখন (it)
	{
	  auto &rec = const_cast<CompositeGlyphRecord &> (*it);
	  ++it;

	  q += rec.get_আকার (extended);

	  rec.lower_gid_24_to_16 ();

	  unsigned আকার_2 = rec.get_আকার (extended);

	  memmove (p, &rec, আকার_2);

	  p += আকার_2;
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
	dest_glyph = hb_bytes_t (dest_glyph.arrayZ, p - (অক্ষর *) dest_glyph.arrayZ);

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

  যৌক্তিক compile_bytes_with_deltas (ধ্রুবক hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ধ্রুবক glyf_accelerator_t &glyf)
  {
    allocated = source_glyph.compile_bytes_with_deltas (plan, font, glyf, dest_start, dest_end);
    ফেরত allocated;
  }

  শূন্য free_compiled_bytes ()
  {
    যদি (likely (allocated)) {
      allocated = মিথ্যা;
      dest_start.fini ();
      dest_end.fini ();
    }
  }

  শূন্য drop_hints_bytes ()
  { source_glyph.drop_hints_bytes (dest_start, dest_end); }

  unsigned পূর্ণসংখ্যা      দৈর্ঘ্য () ধ্রুবক { ফেরত dest_start.দৈর্ঘ্য + dest_end.দৈর্ঘ্য; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  unsigned পূর্ণসংখ্যা     padding () ধ্রুবক { ফেরত দৈর্ঘ্য () % 2; }
  unsigned পূর্ণসংখ্যা padded_আকার () ধ্রুবক { ফেরত দৈর্ঘ্য () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SUBSETGLYPH_HH */
