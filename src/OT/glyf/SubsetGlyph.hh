#جيڪڏهن_وصف_نه_ٿيل OT_GLYF_SUBSETGLYPH_HH
#وصف_ڪريو OT_GLYF_SUBSETGLYPH_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {

بنية glyf_accelerator_t;

نطاق glyf_impl {


بنية SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph ذريعو_glyph;
  hb_بائيٽون_t dest_شروعات;  /* region of source_glyph to copy first */
  hb_بائيٽون_t dest_پڇاڙي;    /* region of source_glyph to copy second */
  منطقي allocated;

  منطقي serialize (hb_serialize_context_t *c,
		  منطقي use_short_loca,
		  ثابت hb_subset_plan_t *plan) ثابت
  {
    TRACE_SERIALIZE (هذا);

    hb_بائيٽون_t dest_glyph = dest_شروعات.copy (c);
    hb_بائيٽون_t پڇاڙي_copy = dest_پڇاڙي.copy (c);
    إذا (!پڇاڙي_copy.arrayZ || !dest_glyph.arrayZ) {
      أرجع خطأ;
    }

    dest_glyph = hb_بائيٽون_t (&dest_glyph, dest_glyph.ڊيگهائي + پڇاڙي_copy.ڊيگهائي);
    بنانشان صحيح pad_ڊيگهائي = use_short_loca ? padding () : 0;
    DEBUG_نياپو (SUBSET, خالي_اشارو, "serialize %u byte glyph, width %u pad %u", dest_glyph.ڊيگهائي, dest_glyph.ڊيگهائي + pad_ڊيگهائي, pad_ڊيگهائي);

    HBUINT8 pad;
    pad = 0;
    طالما (pad_ڊيگهائي > 0)
    {
      (فراغ) c->embed (pad);
      pad_ڊيگهائي--;
    }

    إذا (unlikely (!dest_glyph.ڊيگهائي)) return_trace (سچ);

    منطقي extended = ذريعو_glyph.is_extended ();

    /* update components gids. */
    لكل (خودڪار &_ : Glyph (dest_glyph, old_gid, extended).get_مرکب_iterator ())
    {
      hb_codepoint_t new_gid;
      إذا (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<مرکب_Glyph_رڪارڊ &> (_).سيٽ_gid (new_gid, extended);
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (extended)
    {
      خودڪار it = Glyph (dest_glyph, old_gid, extended).get_مرکب_iterator ();
      إذا (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	حرف *p = it ? (حرف *) &*it : خالي_اشارو;
	حرف *q = p;
	ثابت حرف *پڇاڙي = dest_glyph.arrayZ + dest_glyph.ڊيگهائي;
	طالما (it)
	{
	  خودڪار &rec = const_cast<مرکب_Glyph_رڪارڊ &> (*it);
	  ++it;

	  q += rec.get_سائيز (extended);

	  rec.هيٺاهون_gid_24_منزل_16 ();

	  بنانشان سائيز = rec.get_سائيز (extended);

	  memmove (p, &rec, سائيز);

	  p += سائيز;
	}
	memmove (p, q, پڇاڙي - q);
	p += پڇاڙي - q;

	/* We want to shorten the glyph, but we can't do that without
	 * updating the length in the loca table, which is already
	 * written out :-(.  So we just fill the rest of the glyph with
	 * harmless instructions, since that's what they will be
	 * interpreted as.
	 *
	 * Should move the lowering to _populate_subset_glyphs() to
	 * fix this issue. */

	hb_memset (p, 0x7A /* TrueType instruction ROFF; harmless */, پڇاڙي - p);
	p += پڇاڙي - p;
	dest_glyph = hb_بائيٽون_t (dest_glyph.arrayZ, p - (حرف *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#جيڪڏهن_پورو

    إذا (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    إذا (plan->flags & HB_SUBSET_FLAGS_سيٽ_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).سيٽ_overlaps_flag ();

    return_trace (سچ);
  }

  منطقي compile_بائيٽون_with_deltas (ثابت hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ثابت glyf_accelerator_t &glyf)
  {
    allocated = ذريعو_glyph.compile_بائيٽون_with_deltas (plan, font, glyf, dest_شروعات, dest_پڇاڙي);
    أرجع allocated;
  }

  فراغ free_compiled_بائيٽون ()
  {
    إذا (likely (allocated)) {
      allocated = خطأ;
      dest_شروعات.fini ();
      dest_پڇاڙي.fini ();
    }
  }

  فراغ drop_hints_بائيٽون ()
  { ذريعو_glyph.drop_hints_بائيٽون (dest_شروعات, dest_پڇاڙي); }

  بنانشان صحيح      ڊيگهائي () ثابت { أرجع dest_شروعات.ڊيگهائي + dest_پڇاڙي.ڊيگهائي; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  بنانشان صحيح     padding () ثابت { أرجع ڊيگهائي () % 2; }
  بنانشان صحيح padded_سائيز () ثابت { أرجع ڊيگهائي () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#جيڪڏهن_پورو /* OT_GLYF_SUBSETGLYPH_HH */
