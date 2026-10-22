#ifndef OT_GLYF_SUBSETGLYPH_HH
#define OT_GLYF_SUBSETGLYPH_HH


#अन्तर्भाव "../../hb-open-type.hh"


नामाकाश OT {

संरचना glyf_accelerator_t;

नामाकाश glyf_impl {


संरचना SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph source_glyph;
  hb_bytes_t dest_start;  /* region of source_glyph to copy first */
  hb_bytes_t dest_end;    /* region of source_glyph to copy second */
  तार्किक allocated;

  तार्किक serialize (hb_serialize_context_t *c,
		  तार्किक use_short_loca,
		  अचर hb_subset_plan_t *plan) अचर
  {
    TRACE_SERIALIZE (एतत्);

    hb_bytes_t dest_glyph = dest_start.copy (c);
    hb_bytes_t end_copy = dest_end.copy (c);
    यदि (!end_copy.arrayZ || !dest_glyph.arrayZ) {
      फर्काऊ असत्य;
    }

    dest_glyph = hb_bytes_t (&dest_glyph, dest_glyph.length + end_copy.length);
    अचिन्हित पूर्णाङ्क pad_length = use_short_loca ? padding () : 0;
    DEBUG_सन्देश (SUBSET, nullptr, "serialize %u byte glyph, width %u pad %u", dest_glyph.length, dest_glyph.length + pad_length, pad_length);

    HBUINT8 pad;
    pad = 0;
    जबसम्म (pad_length > 0)
    {
      (शून्य) c->embed (pad);
      pad_length--;
    }

    यदि (unlikely (!dest_glyph.length)) return_trace (सत्य);

    तार्किक extended = source_glyph.is_extended ();

    /* update components gids. */
    लागि (स्वतः &_ : Glyph (dest_glyph, old_gid, extended).get_composite_iterator ())
    {
      hb_codepoint_t new_gid;
      यदि (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<CompositeGlyphRecord &> (_).set_gid (new_gid, extended);
    }

#ifndef HB_NO_BEYOND_64K
    यदि (extended)
    {
      स्वतः it = Glyph (dest_glyph, old_gid, extended).get_composite_iterator ();
      यदि (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	वर्ण *p = it ? (वर्ण *) &*it : nullptr;
	वर्ण *q = p;
	अचर वर्ण *end = dest_glyph.arrayZ + dest_glyph.length;
	जबसम्म (it)
	{
	  स्वतः &rec = const_cast<CompositeGlyphRecord &> (*it);
	  ++it;

	  q += rec.get_size (extended);

	  rec.lower_gid_24_to_16 ();

	  अचिन्हित size = rec.get_size (extended);

	  स_हटाओ (p, &rec, size);

	  p += size;
	}
	स_हटाओ (p, q, end - q);
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
	dest_glyph = hb_bytes_t (dest_glyph.arrayZ, p - (वर्ण *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#endif

    यदि (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    यदि (plan->flags & HB_SUBSET_FLAGS_SET_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).set_overlaps_flag ();

    return_trace (सत्य);
  }

  तार्किक compile_bytes_with_deltas (अचर hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  अचर glyf_accelerator_t &glyf)
  {
    allocated = source_glyph.compile_bytes_with_deltas (plan, font, glyf, dest_start, dest_end);
    फर्काऊ allocated;
  }

  शून्य free_compiled_bytes ()
  {
    यदि (likely (allocated)) {
      allocated = असत्य;
      dest_start.fini ();
      dest_end.fini ();
    }
  }

  शून्य drop_hints_bytes ()
  { source_glyph.drop_hints_bytes (dest_start, dest_end); }

  अचिन्हित पूर्णाङ्क      length () अचर { फर्काऊ dest_start.length + dest_end.length; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  अचिन्हित पूर्णाङ्क     padding () अचर { फर्काऊ length () % 2; }
  अचिन्हित पूर्णाङ्क padded_size () अचर { फर्काऊ length () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SUBSETGLYPH_HH */
