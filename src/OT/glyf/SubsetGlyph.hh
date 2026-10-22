#اگر_غیر_تعریف_شدہ OT_GLYF_SUBSETGLYPH_HH
#تعریف_کرو OT_GLYF_SUBSETGLYPH_HH


#شامل "../../hb-open-type.hh"


نامخلا OT {

ساخت glyf_accelerator_t;

نامخلا glyf_impl {


ساخت SubsetGlyph
{
  hb_codepoint_t old_gid;
  Glyph ماخذ_glyph;
  hb_بائٹیں_t dest_آغاز;  /* region of source_glyph to copy first */
  hb_بائٹیں_t dest_انتہا;    /* region of source_glyph to copy second */
  منطقی allocated;

  منطقی serialize (hb_serialize_context_t *c,
		  منطقی use_short_loca,
		  ثابت hb_subset_plan_t *plan) ثابت
  {
    TRACE_SERIALIZE (یہ);

    hb_بائٹیں_t dest_glyph = dest_آغاز.copy (c);
    hb_بائٹیں_t انتہا_copy = dest_انتہا.copy (c);
    اگر (!انتہا_copy.arrayZ || !dest_glyph.arrayZ) {
      واپس جھوٹ;
    }

    dest_glyph = hb_بائٹیں_t (&dest_glyph, dest_glyph.لمبائی + انتہا_copy.لمبائی);
    غیرعلامتی صحیح pad_لمبائی = use_short_loca ? padding () : 0;
    DEBUG_پیغام (SUBSET, خالی_اشاریہ, "serialize %u byte glyph, width %u pad %u", dest_glyph.لمبائی, dest_glyph.لمبائی + pad_لمبائی, pad_لمبائی);

    HBUINT8 pad;
    pad = 0;
    جب (pad_لمبائی > 0)
    {
      (خالی) c->embed (pad);
      pad_لمبائی--;
    }

    اگر (unlikely (!dest_glyph.لمبائی)) return_trace (سچ);

    منطقی extended = ماخذ_glyph.is_extended ();

    /* update components gids. */
    برائے (خودکار &_ : Glyph (dest_glyph, old_gid, extended).get_مرکب_iterator ())
    {
      hb_codepoint_t new_gid;
      اگر (plan->new_gid_for_old_gid (_.get_gid (extended), &new_gid))
	const_cast<مرکب_Glyph_ریکارڈ &> (_).سیٹ_gid (new_gid, extended);
    }

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (extended)
    {
      خودکار it = Glyph (dest_glyph, old_gid, extended).get_مرکب_iterator ();
      اگر (it)
      {
	/* lower GID24 to GID16 in components if possible. */
	حرف *p = it ? (حرف *) &*it : خالی_اشاریہ;
	حرف *q = p;
	ثابت حرف *انتہا = dest_glyph.arrayZ + dest_glyph.لمبائی;
	جب (it)
	{
	  خودکار &rec = const_cast<مرکب_Glyph_ریکارڈ &> (*it);
	  ++it;

	  q += rec.get_سائز (extended);

	  rec.زیریں_gid_24_مقصد_16 ();

	  غیرعلامتی سائز = rec.get_سائز (extended);

	  memmove (p, &rec, سائز);

	  p += سائز;
	}
	memmove (p, q, انتہا - q);
	p += انتہا - q;

	/* We want to shorten the glyph, but we can't do that without
	 * updating the length in the loca table, which is already
	 * written out :-(.  So we just fill the rest of the glyph with
	 * harmless instructions, since that's what they will be
	 * interpreted as.
	 *
	 * Should move the lowering to _populate_subset_glyphs() to
	 * fix this issue. */

	hb_memset (p, 0x7A /* TrueType instruction ROFF; harmless */, انتہا - p);
	p += انتہا - p;
	dest_glyph = hb_بائٹیں_t (dest_glyph.arrayZ, p - (حرف *) dest_glyph.arrayZ);

	// TODO: Padding; & trim serialized bytes.
	// TODO: Update length in loca. Ugh.
      }
    }
#اگر_ختم

    اگر (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      Glyph (dest_glyph, old_gid, extended).drop_hints ();

    اگر (plan->flags & HB_SUBSET_FLAGS_سیٹ_OVERLAPS_FLAG)
      Glyph (dest_glyph, old_gid, extended).سیٹ_overlaps_flag ();

    return_trace (سچ);
  }

  منطقی compile_بائٹیں_with_deltas (ثابت hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ثابت glyf_accelerator_t &glyf)
  {
    allocated = ماخذ_glyph.compile_بائٹیں_with_deltas (plan, font, glyf, dest_آغاز, dest_انتہا);
    واپس allocated;
  }

  خالی free_compiled_بائٹیں ()
  {
    اگر (likely (allocated)) {
      allocated = جھوٹ;
      dest_آغاز.fini ();
      dest_انتہا.fini ();
    }
  }

  خالی drop_hints_بائٹیں ()
  { ماخذ_glyph.drop_hints_بائٹیں (dest_آغاز, dest_انتہا); }

  غیرعلامتی صحیح      لمبائی () ثابت { واپس dest_آغاز.لمبائی + dest_انتہا.لمبائی; }
  /* pad to 2 to ensure 2-byte loca will be ok */
  غیرعلامتی صحیح     padding () ثابت { واپس لمبائی () % 2; }
  غیرعلامتی صحیح padded_سائز () ثابت { واپس لمبائی () + padding (); }
};


} /* namespace glyf_impl */
} /* namespace OT */


#اگر_ختم /* OT_GLYF_SUBSETGLYPH_HH */
