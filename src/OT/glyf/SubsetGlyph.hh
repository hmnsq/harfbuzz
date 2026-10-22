#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_GLYF_SUBSETGLYPH_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_GLYF_SUBSETGLYPH_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-open-type.hh"


ਨਾਮਾਕਾਸ਼ OT {

ਬਣਤਰ glyf_accelerator_t;

ਨਾਮਾਕਾਸ਼ glyf_impl {


ਬਣਤਰ SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph ਸਰੋਤ_glyph;
  hb_ਬਾਈਟਾਂ_t dest_ਸ਼ੁਰੂ;  /* region of source_glyph to copy first */
  hb_ਬਾਈਟਾਂ_t dest_ਸਿਰਾ;    /* region of source_glyph to copy second */
  ਤਾਰਕਿਕ allocated;

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਤਾਰਕਿਕ use_short_loca,
		  ਅਚਲ hb_subset_plan_t *plan) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    hb_ਬਾਈਟਾਂ_t dest_glyph = dest_ਸ਼ੁਰੂ.copy (c);
    hb_ਬਾਈਟਾਂ_t ਸਿਰਾ_copy = dest_ਸਿਰਾ.copy (c);
    ਜੇ (!ਸਿਰਾ_copy.arrayZ || !dest_glyph.arrayZ) {
      ਵਾਪਸ ਝੂਠ;
    }

    dest_glyph = hb_ਬਾਈਟਾਂ_t (&dest_glyph, dest_glyph.ਲੰਮਾਈ + ਸਿਰਾ_copy.ਲੰਮਾਈ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pad_ਲੰਮਾਈ = use_short_loca ? padding () : 0;
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "serialize %u byte glyph, width %u pad %u", dest_glyph.ਲੰਮਾਈ, dest_glyph.ਲੰਮਾਈ + pad_ਲੰਮਾਈ, pad_ਲੰਮਾਈ);

    HBUINT8 pad;
    pad = 0;
    ਜਦੋਂ (pad_ਲੰਮਾਈ > 0)
    {
      (ਖਾਲੀ) c->embed (pad);
      pad_ਲੰਮਾਈ--;
    }

    ਜੇ (unlikely (!dest_glyph.ਲੰਮਾਈ)) return_trace (ਸੱਚ);

    ਤਾਰਕਿਕ extended = ਸਰੋਤ_glyph.is_extended ();

    /* update components gids. */
    ਲਈ (ਸਵੈਚਾਲਿਤ &_ : Glyph (dest_glyph, old_gid, extended).get_ਸੰਯੁਕਤ_iterator ())
    {
      hb_codepoint_t new_gid;
      ਜੇ (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ &> (_).ਸੈੱਟ_gid (new_gid, extended);
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (extended)
    {
      ਸਵੈਚਾਲਿਤ it = Glyph (dest_glyph, old_gid, extended).get_ਸੰਯੁਕਤ_iterator ();
      ਜੇ (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	ਅੱਖਰ *p = it ? (ਅੱਖਰ *) &*it : ਖਾਲੀ_ਸੂਚਕ;
	ਅੱਖਰ *q = p;
	ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = dest_glyph.arrayZ + dest_glyph.ਲੰਮਾਈ;
	ਜਦੋਂ (it)
	{
	  ਸਵੈਚਾਲਿਤ &rec = const_cast<ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ &> (*it);
	  ++it;

	  q += rec.get_ਮਾਪ (extended);

	  rec.ਹੇਠਲਾ_gid_24_ਮੰਜ਼ਿਲ_16 ();

	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = rec.get_ਮਾਪ (extended);

	  memmove (p, &rec, ਮਾਪ);

	  p += ਮਾਪ;
	}
	memmove (p, q, ਸਿਰਾ - q);
	p += ਸਿਰਾ - q;

	/* We want to shorten the glyph, but we can't do that without
	 * updating the length in the loca table, which is already
	 * written out :-(.  So we just fill the rest of the glyph with
	 * harmless instructions, since that's what they will be
	 * interpreted as.
	 *
	 * Should move the lowering to _populate_subset_glyphs() to
	 * fix this issue. */

	hb_memset (p, 0x7A /* TrueType instruction ROFF; harmless */, ਸਿਰਾ - p);
	p += ਸਿਰਾ - p;
	dest_glyph = hb_ਬਾਈਟਾਂ_t (dest_glyph.arrayZ, p - (ਅੱਖਰ *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#ਜੇ_ਅੰਤ

    ਜੇ (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    ਜੇ (plan->flags & HB_SUBSET_FLAGS_ਸੈੱਟ_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).ਸੈੱਟ_overlaps_flag ();

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ compile_ਬਾਈਟਾਂ_with_deltas (ਅਚਲ hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ਅਚਲ glyf_accelerator_t &glyf)
  {
    allocated = ਸਰੋਤ_glyph.compile_ਬਾਈਟਾਂ_with_deltas (plan, font, glyf, dest_ਸ਼ੁਰੂ, dest_ਸਿਰਾ);
    ਵਾਪਸ allocated;
  }

  ਖਾਲੀ free_compiled_ਬਾਈਟਾਂ ()
  {
    ਜੇ (likely (allocated)) {
      allocated = ਝੂਠ;
      dest_ਸ਼ੁਰੂ.fini ();
      dest_ਸਿਰਾ.fini ();
    }
  }

  ਖਾਲੀ drop_hints_ਬਾਈਟਾਂ ()
  { ਸਰੋਤ_glyph.drop_hints_ਬਾਈਟਾਂ (dest_ਸ਼ੁਰੂ, dest_ਸਿਰਾ); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ      ਲੰਮਾਈ () ਅਚਲ { ਵਾਪਸ dest_ਸ਼ੁਰੂ.ਲੰਮਾਈ + dest_ਸਿਰਾ.ਲੰਮਾਈ; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ     padding () ਅਚਲ { ਵਾਪਸ ਲੰਮਾਈ () % 2; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ padded_ਮਾਪ () ਅਚਲ { ਵਾਪਸ ਲੰਮਾਈ () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_GLYF_SUBSETGLYPH_HH */
