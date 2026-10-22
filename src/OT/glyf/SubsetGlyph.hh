#अगर_अघोषित OT_GLYF_SUBSETGLYPH_HH
#घोषणा OT_GLYF_SUBSETGLYPH_HH


#समावेश "../../hb-open-type.hh"


namespace OT {

काष्ठा glyf_accelerator_t;

namespace glyf_impl {


काष्ठा SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph source_glyph;
  hb_bytes_t dest_start;  /* region of source_glyph to copy first */
  hb_bytes_t dest_end;    /* region of source_glyph to copy second */
  bool allocated;

  bool serialize (hb_serialize_context_t *c,
		  bool use_short_loca,
		  स्थिर hb_subset_plan_t *plan) स्थिर
  {
    TRACE_SERIALIZE (यह);

    hb_bytes_t dest_glyph = dest_start.copy (c);
    hb_bytes_t end_copy = dest_end.copy (c);
    अगर (!end_copy.arrayZ || !dest_glyph.arrayZ) {
      वापस false;
    }

    dest_glyph = hb_bytes_t (&dest_glyph, dest_glyph.लंबाई + end_copy.लंबाई);
    अचिन्हित पूर्णांक pad_लंबाई = use_short_loca ? padding () : 0;
    DEBUG_संदेश (SUBSET, nullptr, "serialize %u byte glyph, width %u pad %u", dest_glyph.लंबाई, dest_glyph.लंबाई + pad_लंबाई, pad_लंबाई);

    HBUINT8 pad;
    pad = 0;
    जबतक (pad_लंबाई > 0)
    {
      (व्योम) c->embed (pad);
      pad_लंबाई--;
    }

    अगर (unlikely (!dest_glyph.लंबाई)) return_trace (true);

    bool extended = source_glyph.is_extended ();

    /* update components gids. */
    क्रम (स्वतः &_ : Glyph (dest_glyph, old_gid, extended).get_composite_iterator ())
    {
      hb_codepoint_t new_gid;
      अगर (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<CompositeGlyphRecord &> (_).set_gid (new_gid, extended);
    }

#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (extended)
    {
      स्वतः it = Glyph (dest_glyph, old_gid, extended).get_composite_iterator ();
      अगर (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	अक्षर *p = it ? (अक्षर *) &*it : nullptr;
	अक्षर *q = p;
	स्थिर अक्षर *end = dest_glyph.arrayZ + dest_glyph.लंबाई;
	जबतक (it)
	{
	  स्वतः &rec = const_cast<CompositeGlyphRecord &> (*it);
	  ++it;

	  q += rec.get_आकार (extended);

	  rec.lower_gid_24_to_16 ();

	  अचिन्हित आकार = rec.get_आकार (extended);

	  स_हटाओ (p, &rec, आकार);

	  p += आकार;
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
	dest_glyph = hb_bytes_t (dest_glyph.arrayZ, p - (अक्षर *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#पूर्ण_अगर

    अगर (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    अगर (plan->flags & HB_SUBSET_FLAGS_SET_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).set_overlaps_flag ();

    return_trace (true);
  }

  bool compile_bytes_with_deltas (स्थिर hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  स्थिर glyf_accelerator_t &glyf)
  {
    allocated = source_glyph.compile_bytes_with_deltas (plan, font, glyf, dest_start, dest_end);
    वापस allocated;
  }

  व्योम free_compiled_bytes ()
  {
    अगर (likely (allocated)) {
      allocated = false;
      dest_start.fini ();
      dest_end.fini ();
    }
  }

  व्योम drop_hints_bytes ()
  { source_glyph.drop_hints_bytes (dest_start, dest_end); }

  अचिन्हित पूर्णांक      लंबाई () स्थिर { वापस dest_start.लंबाई + dest_end.लंबाई; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  अचिन्हित पूर्णांक     padding () स्थिर { वापस लंबाई () % 2; }
  अचिन्हित पूर्णांक padded_आकार () स्थिर { वापस लंबाई () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#पूर्ण_अगर /* OT_GLYF_SUBSETGLYPH_HH */
