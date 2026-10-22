#جے_بنا_تعریف OT_GLYF_SUBSETGLYPH_HH
#تعریف_کرو OT_GLYF_SUBSETGLYPH_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {

بنية glyf_accelerator_t;

نطاق glyf_impl {


بنية SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph سروت_glyph;
  hb_بائٹاں_t dest_شروع;  /* region of source_glyph to copy first */
  hb_بائٹاں_t dest_سرا;    /* region of source_glyph to copy second */
  منطقي allocated;

  منطقي serialize (hb_serialize_context_t *c,
		  منطقي use_short_loca,
		  ثابت hb_subset_plan_t *plan) ثابت
  {
    TRACE_SERIALIZE (هذا);

    hb_بائٹاں_t dest_glyph = dest_شروع.copy (c);
    hb_بائٹاں_t سرا_copy = dest_سرا.copy (c);
    إذا (!سرا_copy.arrayZ || !dest_glyph.arrayZ) {
      أرجع خطأ;
    }

    dest_glyph = hb_بائٹاں_t (&dest_glyph, dest_glyph.لمائی + سرا_copy.لمائی);
    بنانشانی صحيح pad_لمائی = use_short_loca ? padding () : 0;
    DEBUG_پیغام (SUBSET, خالی_سوچک, "serialize %u byte glyph, width %u pad %u", dest_glyph.لمائی, dest_glyph.لمائی + pad_لمائی, pad_لمائی);

    HBUINT8 pad;
    pad = 0;
    طالما (pad_لمائی > 0)
    {
      (فراغ) c->embed (pad);
      pad_لمائی--;
    }

    إذا (unlikely (!dest_glyph.لمائی)) return_trace (سچ);

    منطقي extended = سروت_glyph.is_extended ();

    /* update components gids. */
    لكل (خودکار &_ : Glyph (dest_glyph, old_gid, extended).get_مرکب_iterator ())
    {
      hb_codepoint_t new_gid;
      إذا (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<مرکب_Glyph_ریکارڈ &> (_).سیٹ_gid (new_gid, extended);
    }

#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (extended)
    {
      خودکار it = Glyph (dest_glyph, old_gid, extended).get_مرکب_iterator ();
      إذا (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	حرف *p = it ? (حرف *) &*it : خالی_سوچک;
	حرف *q = p;
	ثابت حرف *سرا = dest_glyph.arrayZ + dest_glyph.لمائی;
	طالما (it)
	{
	  خودکار &rec = const_cast<مرکب_Glyph_ریکارڈ &> (*it);
	  ++it;

	  q += rec.get_ناپ (extended);

	  rec.ہیٹھلا_gid_24_منزل_16 ();

	  بنانشانی ناپ = rec.get_ناپ (extended);

	  memmove (p, &rec, ناپ);

	  p += ناپ;
	}
	memmove (p, q, سرا - q);
	p += سرا - q;

	/* We want to shorten the glyph, but we can't do that without
	 * updating the length in the loca table, which is already
	 * written out :-(.  So we just fill the rest of the glyph with
	 * harmless instructions, since that's what they will be
	 * interpreted as.
	 *
	 * Should move the lowering to _populate_subset_glyphs() to
	 * fix this issue. */

	hb_memset (p, 0x7A /* TrueType instruction ROFF; harmless */, سرا - p);
	p += سرا - p;
	dest_glyph = hb_بائٹاں_t (dest_glyph.arrayZ, p - (حرف *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#جے_انت

    إذا (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    إذا (plan->flags & HB_SUBSET_FLAGS_سیٹ_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).سیٹ_overlaps_flag ();

    return_trace (سچ);
  }

  منطقي compile_بائٹاں_with_deltas (ثابت hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ثابت glyf_accelerator_t &glyf)
  {
    allocated = سروت_glyph.compile_بائٹاں_with_deltas (plan, font, glyf, dest_شروع, dest_سرا);
    أرجع allocated;
  }

  فراغ free_compiled_بائٹاں ()
  {
    إذا (likely (allocated)) {
      allocated = خطأ;
      dest_شروع.fini ();
      dest_سرا.fini ();
    }
  }

  فراغ drop_hints_بائٹاں ()
  { سروت_glyph.drop_hints_بائٹاں (dest_شروع, dest_سرا); }

  بنانشانی صحيح      لمائی () ثابت { أرجع dest_شروع.لمائی + dest_سرا.لمائی; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  بنانشانی صحيح     padding () ثابت { أرجع لمائی () % 2; }
  بنانشانی صحيح padded_ناپ () ثابت { أرجع لمائی () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#جے_انت /* OT_GLYF_SUBSETGLYPH_HH */
