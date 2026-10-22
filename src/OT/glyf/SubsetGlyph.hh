#اگر_تعریف_نشده OT_GLYF_SUBSETGLYPH_HH
#تعریف_کن OT_GLYF_SUBSETGLYPH_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {

بنية glyf_accelerator_t;

نطاق glyf_impl {


بنية SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph منبع_glyph;
  hb_بایتها_t dest_شروع;  /* region of source_glyph to copy first */
  hb_بایتها_t dest_انتها;    /* region of source_glyph to copy second */
  منطقي allocated;

  منطقي serialize (hb_serialize_context_t *c,
		  منطقي use_short_loca,
		  ثابت hb_subset_plan_t *plan) ثابت
  {
    TRACE_SERIALIZE (هذا);

    hb_بایتها_t dest_glyph = dest_شروع.copy (c);
    hb_بایتها_t انتها_copy = dest_انتها.copy (c);
    إذا (!انتها_copy.arrayZ || !dest_glyph.arrayZ) {
      أرجع خطأ;
    }

    dest_glyph = hb_بایتها_t (&dest_glyph, dest_glyph.درازی + انتها_copy.درازی);
    بیعلامت صحيح pad_درازی = use_short_loca ? padding () : 0;
    DEBUG_پیام (SUBSET, اشاره_تهی, "serialize %u byte glyph, width %u pad %u", dest_glyph.درازی, dest_glyph.درازی + pad_درازی, pad_درازی);

    HBUINT8 pad;
    pad = 0;
    طالما (pad_درازی > 0)
    {
      (فراغ) c->embed (pad);
      pad_درازی--;
    }

    إذا (unlikely (!dest_glyph.درازی)) return_trace (درست);

    منطقي extended = منبع_glyph.is_extended ();

    /* update components gids. */
    لكل (خودکار &_ : Glyph (dest_glyph, old_gid, extended).get_مرکب_iterator ())
    {
      hb_codepoint_t new_gid;
      إذا (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<مرکب_Glyph_رکورد &> (_).مجموعه_gid (new_gid, extended);
    }

#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (extended)
    {
      خودکار it = Glyph (dest_glyph, old_gid, extended).get_مرکب_iterator ();
      إذا (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	حرف *p = it ? (حرف *) &*it : اشاره_تهی;
	حرف *q = p;
	ثابت حرف *انتها = dest_glyph.arrayZ + dest_glyph.درازی;
	طالما (it)
	{
	  خودکار &rec = const_cast<مرکب_Glyph_رکورد &> (*it);
	  ++it;

	  q += rec.get_اندازه (extended);

	  rec.پایینی_gid_24_مقصد_16 ();

	  بیعلامت اندازه = rec.get_اندازه (extended);

	  memmove (p, &rec, اندازه);

	  p += اندازه;
	}
	memmove (p, q, انتها - q);
	p += انتها - q;

	/* We want to shorten the glyph, but we can't do that without
	 * updating the length in the loca table, which is already
	 * written out :-(.  So we just fill the rest of the glyph with
	 * harmless instructions, since that's what they will be
	 * interpreted as.
	 *
	 * Should move the lowering to _populate_subset_glyphs() to
	 * fix this issue. */

	hb_memset (p, 0x7A /* TrueType instruction ROFF; harmless */, انتها - p);
	p += انتها - p;
	dest_glyph = hb_بایتها_t (dest_glyph.arrayZ, p - (حرف *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#پایان_اگر

    إذا (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    إذا (plan->flags & HB_SUBSET_FLAGS_مجموعه_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).مجموعه_overlaps_flag ();

    return_trace (درست);
  }

  منطقي compile_بایتها_with_deltas (ثابت hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ثابت glyf_accelerator_t &glyf)
  {
    allocated = منبع_glyph.compile_بایتها_with_deltas (plan, font, glyf, dest_شروع, dest_انتها);
    أرجع allocated;
  }

  فراغ free_compiled_بایتها ()
  {
    إذا (likely (allocated)) {
      allocated = خطأ;
      dest_شروع.fini ();
      dest_انتها.fini ();
    }
  }

  فراغ drop_hints_بایتها ()
  { منبع_glyph.drop_hints_بایتها (dest_شروع, dest_انتها); }

  بیعلامت صحيح      درازی () ثابت { أرجع dest_شروع.درازی + dest_انتها.درازی; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  بیعلامت صحيح     padding () ثابت { أرجع درازی () % 2; }
  بیعلامت صحيح padded_اندازه () ثابت { أرجع درازی () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#پایان_اگر /* OT_GLYF_SUBSETGLYPH_HH */
