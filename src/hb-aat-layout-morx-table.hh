/*
 * Copyright © 2017  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_LAYOUT_MORX_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_LAYOUT_MORX_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_ਟੈਗ_morx HB_ਟੈਗ('m','o','r','x')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_ਟੈਗ_mort HB_ਟੈਗ('m','o','r','t')


ਨਾਮਾਕਾਸ਼ AAT {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ RearrangementSubtable
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::HBUINT HBUINT;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ ਇੰਦਰਾਜ_ਅੰਕੜੇ;

  ਗਿਣਤੀ Flags
  {
    Mark_ਪਹਿਲਾ		= 0x8000,	/* If set, make the current glyph the first
					 * glyph to be rearranged. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. This means
					 * that the glyph index doesn't change, even
					 * if the glyph at that index has changed. */
    Mark_ਆਖਰੀ		= 0x2000,	/* If set, make the current glyph the last
					 * glyph to be rearranged. */
    Reserved		= 0x1FF0,	/* These bits are reserved and should be set to 0. */
    Verb		= 0x000F,	/* The type of rearrangement specified. */
  };

  ਤਾਰਕਿਕ is_action_initiable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਵਾਪਸ (ਇੰਦਰਾਜ.flags & Mark_ਪਹਿਲਾ);
  }
  ਤਾਰਕਿਕ is_actionable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਵਾਪਸ (ਇੰਦਰਾਜ.flags & Verb);
  }

  ਬਣਤਰ driver_context_t
  {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ in_place = ਸੱਚ;

    driver_context_t (ਅਚਲ RearrangementSubtable *ਸਾਰਣੀ_2) :
	ret (ਝੂਠ),
	ਸਾਰਣੀ (ਸਾਰਣੀ_2),
	ਸ਼ੁਰੂ (0), ਸਿਰਾ (0) {}

    HB_AAT_TRANSITION_INLINE
    ਖਾਲੀ transition (hb_ਬਫਰ_t *ਬਫਰ,
		     State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> *driver,
		     ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = ਇੰਦਰਾਜ.flags;

      ਜੇ (flags & Mark_ਪਹਿਲਾ)
	ਸ਼ੁਰੂ = ਬਫਰ->idx;

      ਜੇ (flags & Mark_ਆਖਰੀ)
	ਸਿਰਾ = hb_min (ਬਫਰ->idx + 1, ਬਫਰ->len);

      ਜੇ ((flags & Verb) && ਸ਼ੁਰੂ < ਸਿਰਾ)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ map[16] =
	{
	  0x00,	/* 0	no change */
	  0x10,	/* 1	Ax => xA */
	  0x01,	/* 2	xD => Dx */
	  0x11,	/* 3	AxD => DxA */
	  0x20,	/* 4	ABx => xAB */
	  0x30,	/* 5	ABx => xBA */
	  0x02,	/* 6	xCD => CDx */
	  0x03,	/* 7	xCD => DCx */
	  0x12,	/* 8	AxCD => CDxA */
	  0x13,	/* 9	AxCD => DCxA */
	  0x21,	/* 10	ABxD => DxAB */
	  0x31,	/* 11	ABxD => DxBA */
	  0x22,	/* 12	ABxCD => CDxAB */
	  0x32,	/* 13	ABxCD => CDxBA */
	  0x23,	/* 14	ABxCD => DCxAB */
	  0x33,	/* 15	ABxCD => DCxBA */
	};

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ m = map[flags & Verb];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = hb_min (2u, m >> 4);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = hb_min (2u, m & 0x0F);
	ਤਾਰਕਿਕ ਉਲਟ_l = 3 == (m >> 4);
	ਤਾਰਕਿਕ ਉਲਟ_r = 3 == (m & 0x0F);

	ਜੇ (ਸਿਰਾ - ਸ਼ੁਰੂ >= l + r && ਸਿਰਾ-ਸ਼ੁਰੂ <= HB_MAX_CONTEXT_ਲੰਮਾਈ)
	{
	  ਬਫਰ->ਮਿਲਾਓ_clusters (ਸ਼ੁਰੂ, hb_min (ਬਫਰ->idx + 1, ਬਫਰ->len));
	  ਬਫਰ->ਮਿਲਾਓ_clusters (ਸ਼ੁਰੂ, ਸਿਰਾ);

	  hb_glyph_info_t *info = ਬਫਰ->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + ਸ਼ੁਰੂ, l * ਆਕਾਰ (buf[0]));
	  hb_memcpy (buf + 2, info + ਸਿਰਾ - r, r * ਆਕਾਰ (buf[0]));

	  ਜੇ (l != r)
	    memmove (info + ਸ਼ੁਰੂ + r, info + ਸ਼ੁਰੂ + l, (ਸਿਰਾ - ਸ਼ੁਰੂ - l - r) * ਆਕਾਰ (buf[0]));

	  hb_memcpy (info + ਸ਼ੁਰੂ, buf + 2, r * ਆਕਾਰ (buf[0]));
	  hb_memcpy (info + ਸਿਰਾ - l, buf, l * ਆਕਾਰ (buf[0]));
	  ਜੇ (ਉਲਟ_l)
	  {
	    buf[0] = info[ਸਿਰਾ - 1];
	    info[ਸਿਰਾ - 1] = info[ਸਿਰਾ - 2];
	    info[ਸਿਰਾ - 2] = buf[0];
	  }
	  ਜੇ (ਉਲਟ_r)
	  {
	    buf[0] = info[ਸ਼ੁਰੂ];
	    info[ਸ਼ੁਰੂ] = info[ਸ਼ੁਰੂ + 1];
	    info[ਸ਼ੁਰੂ + 1] = buf[0];
	  }
	}
      }
    }

    ਜਨਤਕ:
    ਤਾਰਕਿਕ ret;
    ਅਚਲ RearrangementSubtable *ਸਾਰਣੀ;
    ਨਿਜੀ:
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ;
  };

  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    driver_context_t dc (ਇਹ);

    State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (machine.sanitize (c));
  }

  ਜਨਤਕ:
  State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>	machine;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC ((State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>::static_ਮਾਪ));
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ContextualSubtable
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::HBUINT HBUINT;

  ਬਣਤਰ ਇੰਦਰਾਜ_ਅੰਕੜੇ
  {
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ std::conditional<Types::extended, HBUINT16, HBINT16>::type ਵਿੱਥ_Type;

    ਵਿੱਥ_Type	mark_ਸੂਚਕਾਂਕ_2;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    ਵਿੱਥ_Type	current_ਸੂਚਕਾਂਕ;	/* Index/offset of the substitution table for the
				 * current glyph. */
    ਜਨਤਕ:
    DEFINE_ਮਾਪ_STATIC (4);
  };

  ਗਿਣਤੀ Flags
  {
    ਸੈੱਟ_Mark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  ਤਾਰਕਿਕ is_action_initiable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਵਾਪਸ (ਇੰਦਰਾਜ.flags & ਸੈੱਟ_Mark);
  }
  ਤਾਰਕਿਕ is_actionable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਜੇ (Types::extended)
      ਵਾਪਸ ਇੰਦਰਾਜ.ਅੰਕੜੇ.mark_ਸੂਚਕਾਂਕ_2 != 0xFFFF || ਇੰਦਰਾਜ.ਅੰਕੜੇ.current_ਸੂਚਕਾਂਕ != 0xFFFF;
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ ਇੰਦਰਾਜ.ਅੰਕੜੇ.mark_ਸੂਚਕਾਂਕ_2 || ਇੰਦਰਾਜ.ਅੰਕੜੇ.current_ਸੂਚਕਾਂਕ;
  }

  ਬਣਤਰ driver_context_t
  {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ in_place = ਸੱਚ;

    driver_context_t (ਅਚਲ ContextualSubtable *ਸਾਰਣੀ_2,
			     hb_aat_apply_context_t *c_) :
	ret (ਝੂਠ),
	c (c_),
	ਸਾਰਣੀ (ਸਾਰਣੀ_2),
	mark_ਸੈੱਟ (ਝੂਠ),
	mark (0),
	subs (ਸਾਰਣੀ+ਸਾਰਣੀ->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    ਖਾਲੀ transition (hb_ਬਫਰ_t *ਬਫਰ,
		     State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> *driver,
		     ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      ਜੇ (ਬਫਰ->idx == ਬਫਰ->len && !mark_ਸੈੱਟ)
	ਵਾਪਸ;

      ਅਚਲ HBGlyphID16 *replacement;

      replacement = ਖਾਲੀ_ਸੂਚਕ;
      ਜੇ (Types::extended)
      {
	ਜੇ (ਇੰਦਰਾਜ.ਅੰਕੜੇ.mark_ਸੂਚਕਾਂਕ_2 != 0xFFFF)
	{
	  ਅਚਲ Lookup<HBGlyphID16> &lookup = subs[ਇੰਦਰਾਜ.ਅੰਕੜੇ.mark_ਸੂਚਕਾਂਕ_2];
	  replacement = lookup.get_ਮੁੱਲ (ਬਫਰ->info[mark].codepoint, driver->ਅੰਕ_glyphs);
	}
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (ਇੰਦਰਾਜ.ਅੰਕੜੇ.mark_ਸੂਚਕਾਂਕ_2)
	{
	  ਪੂਰਨ ਵਿੱਥ = (ਪੂਰਨ) ਇੰਦਰਾਜ.ਅੰਕੜੇ.mark_ਸੂਚਕਾਂਕ_2 + ਬਫਰ->info[mark].codepoint;
	  ਅਚਲ Unsized_Array_ਦਾ<HBGlyphID16> &subs_old = (ਅਚਲ Unsized_Array_ਦਾ<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::ਸ਼ਬਦ_ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (ਵਿੱਥ, ਸਾਰਣੀ, subs_old.arrayZ)];
	  ਜੇ (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = ਖਾਲੀ_ਸੂਚਕ;
	}
      }
      ਜੇ (replacement)
      {
	ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (mark, hb_min (ਬਫਰ->idx + 1, ਬਫਰ->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = ਸੱਚ;
      }

      replacement = ਖਾਲੀ_ਸੂਚਕ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx = hb_min (ਬਫਰ->idx, ਬਫਰ->len - 1);
      ਜੇ (Types::extended)
      {
	ਜੇ (ਇੰਦਰਾਜ.ਅੰਕੜੇ.current_ਸੂਚਕਾਂਕ != 0xFFFF)
	{
	  ਅਚਲ Lookup<HBGlyphID16> &lookup = subs[ਇੰਦਰਾਜ.ਅੰਕੜੇ.current_ਸੂਚਕਾਂਕ];
	  replacement = lookup.get_ਮੁੱਲ (ਬਫਰ->info[idx].codepoint, driver->ਅੰਕ_glyphs);
	}
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (ਇੰਦਰਾਜ.ਅੰਕੜੇ.current_ਸੂਚਕਾਂਕ)
	{
	  ਪੂਰਨ ਵਿੱਥ = (ਪੂਰਨ) ਇੰਦਰਾਜ.ਅੰਕੜੇ.current_ਸੂਚਕਾਂਕ + ਬਫਰ->info[idx].codepoint;
	  ਅਚਲ Unsized_Array_ਦਾ<HBGlyphID16> &subs_old = (ਅਚਲ Unsized_Array_ਦਾ<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::ਸ਼ਬਦ_ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (ਵਿੱਥ, ਸਾਰਣੀ, subs_old.arrayZ)];
	  ਜੇ (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = ਖਾਲੀ_ਸੂਚਕ;
	}
      }
      ਜੇ (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = ਸੱਚ;
      }

      ਜੇ (ਇੰਦਰਾਜ.flags & ਸੈੱਟ_Mark)
      {
	mark_ਸੈੱਟ = ਸੱਚ;
	mark = ਬਫਰ->idx;
      }
    }

    ਜਨਤਕ:
    ਤਾਰਕਿਕ ret;
    hb_aat_apply_context_t *c;
    ਅਚਲ ContextualSubtable *ਸਾਰਣੀ;
    ਨਿਜੀ:
    ਤਾਰਕਿਕ mark_ਸੈੱਟ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark;
    ਅਚਲ Unsized_List_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<Lookup<HBGlyphID16>, HBUINT, ਖਾਲੀ, ਝੂਠ> &subs;
  };

  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    driver_context_t dc (ਇਹ, c);

    State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_entries = 0;
    ਜੇ (unlikely (!machine.sanitize (c, &ਅੰਕ_entries))) return_trace (ਝੂਠ);
    hb_barrier ();

    ਜੇ (!Types::extended)
      return_trace (substitutionTables.sanitize (c, ਇਹ, 0));

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_lookups = 0;

    ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> *entries = machine.get_entries ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_entries; i++)
    {
      ਅਚਲ ਇੰਦਰਾਜ_ਅੰਕੜੇ &ਅੰਕੜੇ = entries[i].ਅੰਕੜੇ;

      ਜੇ (ਅੰਕੜੇ.mark_ਸੂਚਕਾਂਕ_2 != 0xFFFF)
	ਅੰਕ_lookups = hb_max (ਅੰਕ_lookups, 1u + ਅੰਕੜੇ.mark_ਸੂਚਕਾਂਕ_2);
      ਜੇ (ਅੰਕੜੇ.current_ਸੂਚਕਾਂਕ != 0xFFFF)
	ਅੰਕ_lookups = hb_max (ਅੰਕ_lookups, 1u + ਅੰਕੜੇ.current_ਸੂਚਕਾਂਕ);
    }

    return_trace (substitutionTables.sanitize (c, ਇਹ, ਅੰਕ_lookups));
  }

  ਜਨਤਕ:
  State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>
		machine;
  ਸੁਰੱਖਿਅਤ:
  NNOffset_ਮੰਜ਼ਿਲ<Unsized_List_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<Lookup<HBGlyphID16>, HBUINT, ਖਾਲੀ, ਝੂਠ>, HBUINT>
		substitutionTables;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC ((State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>::static_ਮਾਪ + HBUINT::static_ਮਾਪ));
};


ਸਾਂਚਾ <ਤਾਰਕਿਕ extended>
ਬਣਤਰ Ligature_ਇੰਦਰਾਜ;

ਸਾਂਚਾ <>
ਬਣਤਰ Ligature_ਇੰਦਰਾਜ<ਸੱਚ>
{

  ਬਣਤਰ ਇੰਦਰਾਜ_ਅੰਕੜੇ
  {
    HBUINT16	lig_Action_ਸੂਚਕਾਂਕ;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    ਜਨਤਕ:
    DEFINE_ਮਾਪ_STATIC (2);
  };

  ਗਿਣਤੀ Flags
  {
    ਸੈੱਟ_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  ਸਥਿਰ ਤਾਰਕਿਕ initiateAction (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.flags & ਸੈੱਟ_Component; }

  ਸਥਿਰ ਤਾਰਕਿਕ performAction (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.flags & PerformAction; }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lig_Action_ਸੂਚਕਾਂਕ (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.ਅੰਕੜੇ.lig_Action_ਸੂਚਕਾਂਕ; }
};
ਸਾਂਚਾ <>
ਬਣਤਰ Ligature_ਇੰਦਰਾਜ<ਝੂਠ>
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ ਇੰਦਰਾਜ_ਅੰਕੜੇ;

  ਗਿਣਤੀ Flags
  {
    ਸੈੱਟ_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    ਵਿੱਥ_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  ਸਥਿਰ ਤਾਰਕਿਕ initiateAction (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.flags & ਸੈੱਟ_Component; }

  ਸਥਿਰ ਤਾਰਕਿਕ performAction (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.flags & ਵਿੱਥ_2; }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lig_Action_ਸੂਚਕਾਂਕ (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.flags & ਵਿੱਥ_2; }
};


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ LigatureSubtable
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::HBUINT HBUINT;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Ligature_ਇੰਦਰਾਜ<Types::extended> Ligature_ਇੰਦਰਾਜ_T;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Ligature_ਇੰਦਰਾਜ_T::ਇੰਦਰਾਜ_ਅੰਕੜੇ ਇੰਦਰਾਜ_ਅੰਕੜੇ;

  ਗਿਣਤੀ Flags
  {
    DontAdvance	= Ligature_ਇੰਦਰਾਜ_T::DontAdvance,
  };

  ਤਾਰਕਿਕ is_action_initiable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਵਾਪਸ Ligature_ਇੰਦਰਾਜ_T::initiateAction (ਇੰਦਰਾਜ);
  }
  ਤਾਰਕਿਕ is_actionable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਵਾਪਸ Ligature_ਇੰਦਰਾਜ_T::performAction (ਇੰਦਰਾਜ);
  }

  ਬਣਤਰ driver_context_t
  {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ in_place = ਝੂਠ;
    ਗਿਣਤੀ LigActionFlags
    {
      Lig_Action_ਆਖਰੀ	= 0x80000000,	/* This is the last action in the list. This also
					 * implies storage. */
      LigActionStore	= 0x40000000,	/* Store the ligature at the current cumulated index
					 * in the ligature table in place of the marked
					 * (i.e. currently-popped) glyph. */
      Lig_Action_ਵਿੱਥ	= 0x3FFFFFFF,	/* A 30-bit value which is sign-extended to 32-bits
					 * and added to the glyph ID, resulting in an index
					 * into the component table. */
    };

    driver_context_t (ਅਚਲ LigatureSubtable *ਸਾਰਣੀ_2,
		      hb_aat_apply_context_t *c_) :
	ret (ਝੂਠ),
	c (c_),
	ਸਾਰਣੀ (ਸਾਰਣੀ_2),
	ligAction (ਸਾਰਣੀ+ਸਾਰਣੀ->ligAction),
	component (ਸਾਰਣੀ+ਸਾਰਣੀ->component),
	ligature (ਸਾਰਣੀ+ਸਾਰਣੀ->ligature),
	ਮੇਲ_ਲੰਮਾਈ (0) {}

    HB_AAT_TRANSITION_INLINE
    ਖਾਲੀ transition (hb_ਬਫਰ_t *ਬਫਰ,
		     State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> *driver,
		     ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
    {
      DEBUG_ਸੁਨੇਹਾ (APPLY, ਖਾਲੀ_ਸੂਚਕ, "Ligature transition at %u", ਬਫਰ->idx);
      ਜੇ (ਇੰਦਰਾਜ.flags & Ligature_ਇੰਦਰਾਜ_T::ਸੈੱਟ_Component)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	ਜੇ (unlikely (ਮੇਲ_ਲੰਮਾਈ && ਮੇਲ_positions[(ਮੇਲ_ਲੰਮਾਈ - 1u) % ARRAY_ਲੰਮਾਈ (ਮੇਲ_positions)] == ਬਫਰ->ਬਾਹਰਲਾ_len))
	  ਮੇਲ_ਲੰਮਾਈ--;

	ਮੇਲ_positions[ਮੇਲ_ਲੰਮਾਈ++ % ARRAY_ਲੰਮਾਈ (ਮੇਲ_positions)] = ਬਫਰ->ਬਾਹਰਲਾ_len;
	DEBUG_ਸੁਨੇਹਾ (APPLY, ਖਾਲੀ_ਸੂਚਕ, "Set component at %u", ਬਫਰ->ਬਾਹਰਲਾ_len);
      }

      ਜੇ (Ligature_ਇੰਦਰਾਜ_T::performAction (ਇੰਦਰਾਜ))
      {
	DEBUG_ਸੁਨੇਹਾ (APPLY, ਖਾਲੀ_ਸੂਚਕ, "Perform action with %u", ਮੇਲ_ਲੰਮਾਈ);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = ਬਫਰ->ਬਾਹਰਲਾ_len;

	ਜੇ (unlikely (!ਮੇਲ_ਲੰਮਾਈ))
	  ਵਾਪਸ;

	ਜੇ (ਬਫਰ->idx >= ਬਫਰ->len)
	  ਵਾਪਸ; /* TODO Work on previous instead? */

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cursor = ਮੇਲ_ਲੰਮਾਈ;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ action_idx = Ligature_ਇੰਦਰਾਜ_T::lig_Action_ਸੂਚਕਾਂਕ (ਇੰਦਰਾਜ);
	action_idx = Types::ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (action_idx, ਸਾਰਣੀ, ligAction.arrayZ);
	ਅਚਲ HBUINT32 *action_ਅੰਕੜੇ = &ligAction[action_idx];

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ligature_idx = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ action;
	ਕਰੋ
	{
	  ਜੇ (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_ਸੁਨੇਹਾ (APPLY, ਖਾਲੀ_ਸੂਚਕ, "Stack underflow");
	    ਮੇਲ_ਲੰਮਾਈ = 0;
	    ਰੋਕੋ;
	  }

	  DEBUG_ਸੁਨੇਹਾ (APPLY, ਖਾਲੀ_ਸੂਚਕ, "Moving to stack position %u", cursor - 1);
	  ਜੇ (unlikely (!ਬਫਰ->move_ਮੰਜ਼ਿਲ (ਮੇਲ_positions[--cursor % ARRAY_ਲੰਮਾਈ (ਮੇਲ_positions)]))) ਵਾਪਸ;

	  ਜੇ (unlikely (!action_ਅੰਕੜੇ->sanitize (&c->sanitizer))) ਰੋਕੋ;
	  hb_barrier ();
	  action = *action_ਅੰਕੜੇ;

	  uint32_t uoffset = action & Lig_Action_ਵਿੱਥ;
	  ਜੇ (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t ਵਿੱਥ = (int32_t) uoffset;
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ component_idx = ਬਫਰ->cur().codepoint + ਵਿੱਥ;
	  component_idx = Types::ਸ਼ਬਦ_ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (component_idx, ਸਾਰਣੀ, component.arrayZ);
	  ਅਚਲ HBUINT16 &component_ਅੰਕੜੇ = component[component_idx];
	  ਜੇ (unlikely (!component_ਅੰਕੜੇ.sanitize (&c->sanitizer))) ਰੋਕੋ;
	  hb_barrier ();
	  ligature_idx += component_ਅੰਕੜੇ;

	  DEBUG_ਸੁਨੇਹਾ (APPLY, ਖਾਲੀ_ਸੂਚਕ, "Action store %d last %d",
		     ਤਾਰਕਿਕ (action & LigActionStore),
		     ਤਾਰਕਿਕ (action & Lig_Action_ਆਖਰੀ));
	  ਜੇ (action & (LigActionStore | Lig_Action_ਆਖਰੀ))
	  {
	    ligature_idx = Types::ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (ligature_idx, ਸਾਰਣੀ, ligature.arrayZ);
	    ਅਚਲ HBGlyphID16 &ligature_ਅੰਕੜੇ = ligature[ligature_idx];
	    ਜੇ (unlikely (!ligature_ਅੰਕੜੇ.sanitize (&c->sanitizer))) ਰੋਕੋ;
	    hb_barrier ();
	    hb_codepoint_t lig = ligature_ਅੰਕੜੇ;

	    DEBUG_ਸੁਨੇਹਾ (APPLY, ਖਾਲੀ_ਸੂਚਕ, "Produced ligature %u", lig);
	    ਜੇ (unlikely (!c->replace_glyph (lig))) ਵਾਪਸ;

	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lig_ਸਿਰਾ = ਮੇਲ_positions[(ਮੇਲ_ਲੰਮਾਈ - 1u) % ARRAY_ਲੰਮਾਈ (ਮੇਲ_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    ਜਦੋਂ (ਮੇਲ_ਲੰਮਾਈ - 1u > cursor)
	    {
	      DEBUG_ਸੁਨੇਹਾ (APPLY, ਖਾਲੀ_ਸੂਚਕ, "Skipping ligature component");
	      ਜੇ (unlikely (!ਬਫਰ->move_ਮੰਜ਼ਿਲ (ਮੇਲ_positions[--ਮੇਲ_ਲੰਮਾਈ % ARRAY_ਲੰਮਾਈ (ਮੇਲ_positions)]))) ਵਾਪਸ;
	      ਜੇ (!c->delete_glyph ()) ਵਾਪਸ;
	    }

	    ਜੇ (unlikely (!ਬਫਰ->move_ਮੰਜ਼ਿਲ (lig_ਸਿਰਾ))) ਵਾਪਸ;
	    ਬਫਰ->ਮਿਲਾਓ_ਬਾਹਰਲਾ_clusters (ਮੇਲ_positions[cursor % ARRAY_ਲੰਮਾਈ (ਮੇਲ_positions)], ਬਫਰ->ਬਾਹਰਲਾ_len);
	  }

	  action_ਅੰਕੜੇ++;
	}
	ਜਦੋਂ (!(action & Lig_Action_ਆਖਰੀ));
	ਜੇ (unlikely (!ਬਫਰ->move_ਮੰਜ਼ਿਲ (ਸਿਰਾ))) ਵਾਪਸ;
      }
    }

    ਜਨਤਕ:
    ਤਾਰਕਿਕ ret;
    hb_aat_apply_context_t *c;
    ਅਚਲ LigatureSubtable *ਸਾਰਣੀ;
    ਨਿਜੀ:
    ਅਚਲ Unsized_Array_ਦਾ<HBUINT32> &ligAction;
    ਅਚਲ Unsized_Array_ਦਾ<HBUINT16> &component;
    ਅਚਲ Unsized_Array_ਦਾ<HBGlyphID16> &ligature;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੇਲ_ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੇਲ_positions[HB_MAX_CONTEXT_ਲੰਮਾਈ];
  };

  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    driver_context_t dc (ਇਹ, c);

    State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (ਇਹ) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  ਜਨਤਕ:
  State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>
		machine;
  ਸੁਰੱਖਿਅਤ:
  NNOffset_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffset_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffset_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC ((State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>::static_ਮਾਪ + 3 * HBUINT::static_ਮਾਪ));
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ NoncontextualSubtable
{
  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs = c->face->get_ਅੰਕ_glyphs ();

    hb_glyph_info_t *info = c->ਬਫਰ->info;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = c->ਬਫਰ->len;
    // If there's only one range, we already checked the flag.
    ਸਵੈਚਾਲਿਤ *ਆਖਰੀ_range = c->range_flags && (c->range_flags->ਲੰਮਾਈ > 1) ? &(*c->range_flags)[0] : ਖਾਲੀ_ਸੂਚਕ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      ਜੇ (unlikely (ਆਖਰੀ_range))
      {
	ਸਵੈਚਾਲਿਤ *range = ਆਖਰੀ_range;
	{
	  ਬਿਨਾਨਿਸ਼ਾਨੀ cluster = info[i].cluster;
	  ਜਦੋਂ (cluster < range->cluster_ਪਹਿਲਾ)
	    range--;
	  ਜਦੋਂ (cluster > range->cluster_ਆਖਰੀ)
	    range++;

	  ਆਖਰੀ_range = range;
	}
	ਜੇ (!(range->flags & c->subtable_flags))
	  ਜਾਰੀ;
      }

      ਅਚਲ HBGlyphID16 *replacement = substitute.get_ਮੁੱਲ (info[i].codepoint, ਅੰਕ_glyphs);
      ਜੇ (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = ਸੱਚ;
      }
    }

    return_trace (ret);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_initial_glyphs (ਸੈੱਟ_t &glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    substitute.collect_glyphs (glyphs, ਅੰਕ_glyphs);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (substitute.sanitize (c));
  }

  ਸੁਰੱਖਿਅਤ:
  Lookup<HBGlyphID16>	substitute;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (2);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ InsertionSubtable
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::HBUINT HBUINT;

  ਬਣਤਰ ਇੰਦਰਾਜ_ਅੰਕੜੇ
  {
    HBUINT16	current_ਪਾਓ_ਸੂਚਕਾਂਕ;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_ਪਾਓ_ਸੂਚਕਾਂਕ;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    ਜਨਤਕ:
    DEFINE_ਮਾਪ_STATIC (4);
  };

  ਗਿਣਤੀ Flags
  {
    ਸੈੱਟ_Mark		= 0x8000,     /* If set, mark the current glyph. */
    DontAdvance		= 0x4000,     /* If set, don't advance to the next glyph before
				       * going to the new state.  This does not mean
				       * that the glyph pointed to is the same one as
				       * before. If you've made insertions immediately
				       * downstream of the current glyph, the next glyph
				       * processed would in fact be the first one
				       * inserted. */
    CurrentIsKashidaLike= 0x2000,     /* If set, and the currentInsertList is nonzero,
				       * then the specified glyph list will be inserted
				       * as a kashida-like insertion, either before or
				       * after the current glyph (depending on the state
				       * of the currentInsertBefore flag). If clear, and
				       * the currentInsertList is nonzero, then the
				       * specified glyph list will be inserted as a
				       * split-vowel-like insertion, either before or
				       * after the current glyph (depending on the state
				       * of the currentInsertBefore flag). */
    MarkedIsKashidaLike= 0x1000,      /* If set, and the markedInsertList is nonzero,
				       * then the specified glyph list will be inserted
				       * as a kashida-like insertion, either before or
				       * after the marked glyph (depending on the state
				       * of the markedInsertBefore flag). If clear, and
				       * the markedInsertList is nonzero, then the
				       * specified glyph list will be inserted as a
				       * split-vowel-like insertion, either before or
				       * after the marked glyph (depending on the state
				       * of the markedInsertBefore flag). */
    Current_ਪਾਓ_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_ਪਾਓ_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_ਪਾਓ_ਗਿਣਤੀ= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_ਪਾਓ_ਗਿਣਤੀ= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  ਤਾਰਕਿਕ is_action_initiable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਵਾਪਸ (ਇੰਦਰਾਜ.flags & ਸੈੱਟ_Mark);
  }
  ਤਾਰਕਿਕ is_actionable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਵਾਪਸ (ਇੰਦਰਾਜ.flags & (Current_ਪਾਓ_ਗਿਣਤੀ | Marked_ਪਾਓ_ਗਿਣਤੀ)) &&
	   (ਇੰਦਰਾਜ.ਅੰਕੜੇ.current_ਪਾਓ_ਸੂਚਕਾਂਕ != 0xFFFF ||ਇੰਦਰਾਜ.ਅੰਕੜੇ.marked_ਪਾਓ_ਸੂਚਕਾਂਕ != 0xFFFF);
  }

  ਬਣਤਰ driver_context_t
  {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ in_place = ਝੂਠ;

    driver_context_t (ਅਚਲ InsertionSubtable *ਸਾਰਣੀ_2,
		      hb_aat_apply_context_t *c_) :
	ret (ਝੂਠ),
	c (c_),
	ਸਾਰਣੀ (ਸਾਰਣੀ_2),
	mark (0),
	insertionAction (ਸਾਰਣੀ+ਸਾਰਣੀ->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    ਖਾਲੀ transition (hb_ਬਫਰ_t *ਬਫਰ,
		     State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> *driver,
		     ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = ਇੰਦਰਾਜ.flags;

      ਬਿਨਾਨਿਸ਼ਾਨੀ mark_loc = ਬਫਰ->ਬਾਹਰਲਾ_len;

      ਜੇ (ਇੰਦਰਾਜ.ਅੰਕੜੇ.marked_ਪਾਓ_ਸੂਚਕਾਂਕ != 0xFFFF)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = (flags & Marked_ਪਾਓ_ਗਿਣਤੀ);
	ਜੇ (unlikely ((ਬਫਰ->max_ops -= ਗਿਣਤੀ_2) <= 0)) ਵਾਪਸ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = ਇੰਦਰਾਜ.ਅੰਕੜੇ.marked_ਪਾਓ_ਸੂਚਕਾਂਕ;
	ਅਚਲ HBGlyphID16 *glyphs = &insertionAction[ਸ਼ੁਰੂ];
	ਜੇ (unlikely (!c->sanitizer.check_array (glyphs, ਗਿਣਤੀ_2))) ਗਿਣਤੀ_2 = 0;
	hb_barrier ();

	ਤਾਰਕਿਕ before = flags & Marked_ਪਾਓ_Before;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = ਬਫਰ->ਬਾਹਰਲਾ_len;
	ਜੇ (unlikely (!ਬਫਰ->move_ਮੰਜ਼ਿਲ (mark))) ਵਾਪਸ;

	ਜੇ (ਬਫਰ->idx < ਬਫਰ->len && !before)
	  ਜੇ (unlikely (!ਬਫਰ->copy_glyph ())) ਵਾਪਸ;
	/* TODO We ignore KashidaLike setting. */
	ਜੇ (unlikely (!c->output_glyphs (ਗਿਣਤੀ_2, glyphs))) ਵਾਪਸ;
	ret = ਸੱਚ;
	ਜੇ (ਬਫਰ->idx < ਬਫਰ->len && !before)
	  ਬਫਰ->skip_glyph ();

	ਜੇ (unlikely (!ਬਫਰ->move_ਮੰਜ਼ਿਲ (ਸਿਰਾ + ਗਿਣਤੀ_2))) ਵਾਪਸ;

	ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break_from_outbuffer (mark, hb_min (ਬਫਰ->idx + 1, ਬਫਰ->len));
      }

      ਜੇ (flags & ਸੈੱਟ_Mark)
	mark = mark_loc;

      ਜੇ (ਇੰਦਰਾਜ.ਅੰਕੜੇ.current_ਪਾਓ_ਸੂਚਕਾਂਕ != 0xFFFF)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = (flags & Current_ਪਾਓ_ਗਿਣਤੀ) >> 5;
	ਜੇ (unlikely ((ਬਫਰ->max_ops -= ਗਿਣਤੀ_2) <= 0)) ਵਾਪਸ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = ਇੰਦਰਾਜ.ਅੰਕੜੇ.current_ਪਾਓ_ਸੂਚਕਾਂਕ;
	ਅਚਲ HBGlyphID16 *glyphs = &insertionAction[ਸ਼ੁਰੂ];
	ਜੇ (unlikely (!c->sanitizer.check_array (glyphs, ਗਿਣਤੀ_2))) ਗਿਣਤੀ_2 = 0;
	hb_barrier ();

	ਤਾਰਕਿਕ before = flags & Current_ਪਾਓ_Before;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = ਬਫਰ->ਬਾਹਰਲਾ_len;

	ਜੇ (ਬਫਰ->idx < ਬਫਰ->len && !before)
	  ਜੇ (unlikely (!ਬਫਰ->copy_glyph ())) ਵਾਪਸ;
	/* TODO We ignore KashidaLike setting. */
	ਜੇ (unlikely (!c->output_glyphs (ਗਿਣਤੀ_2, glyphs))) ਵਾਪਸ;
	ret = ਸੱਚ;
	ਜੇ (ਬਫਰ->idx < ਬਫਰ->len && !before)
	  ਬਫਰ->skip_glyph ();

	/* Humm. Not sure where to move to.  There's this wording under
	 * DontAdvance flag:
	 *
	 * "If set, don't update the glyph index before going to the new state.
	 * This does not mean that the glyph pointed to is the same one as
	 * before. If you've made insertions immediately downstream of the
	 * current glyph, the next glyph processed would in fact be the first
	 * one inserted."
	 *
	 * This suggests that if DontAdvance is NOT set, we should move to
	 * end+count.  If it *was*, then move to end, such that newly inserted
	 * glyphs are now visible.
	 *
	 * https://github.com/harfbuzz/harfbuzz/issues/1224#issuecomment-427691417
	 */
	ਜੇ (unlikely (!ਬਫਰ->move_ਮੰਜ਼ਿਲ ((flags & DontAdvance) ? ਸਿਰਾ : ਸਿਰਾ + ਗਿਣਤੀ_2))) ਵਾਪਸ;
      }
    }

    ਜਨਤਕ:
    ਤਾਰਕਿਕ ret;
    hb_aat_apply_context_t *c;
    ਅਚਲ InsertionSubtable *ਸਾਰਣੀ;
    ਨਿਜੀ:
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark;
    ਅਚਲ Unsized_Array_ਦਾ<HBGlyphID16> &insertionAction;
  };

  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    driver_context_t dc (ਇਹ, c);

    State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (ਇਹ) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  ਜਨਤਕ:
  State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>
		machine;
  ਸੁਰੱਖਿਅਤ:
  NNOffset_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC ((State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>::static_ਮਾਪ + HBUINT::static_ਮਾਪ));
};


ਬਣਤਰ Feature
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};


ਬਣਤਰ hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  ਬਣਤਰ hb_applicable_t
  {
    ਮਿੱਤਰ ਬਣਤਰ hb_accelerate_subtables_context_t;
    ਮਿੱਤਰ ਬਣਤਰ hb_aat_layout_lookup_accelerator_t;

    ਜਨਤਕ:
    hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t glyph_ਸੈੱਟ;
    ਬਦਲਣਯੋਗ hb_aat_class_cache_t class_cache;

    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
    ਸਵੈਚਾਲਿਤ init_ (ਅਚਲ T &obj_, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs, hb_ਤਰਜੀਹ<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_ਸੈੱਟ, ਅੰਕ_glyphs, obj_)
    )

    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
    ਖਾਲੀ init_ (ਅਚਲ T &obj_, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs, hb_ਤਰਜੀਹ<0>)
    {
      obj_.collect_initial_glyphs (glyph_ਸੈੱਟ, ਅੰਕ_glyphs);
    }

    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
    ਖਾਲੀ init (ਅਚਲ T &obj_, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs)
    {
      glyph_ਸੈੱਟ.init ();
      init_ (obj_, ਅੰਕ_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    ਖਾਲੀ
    fini ()
    {
      glyph_ਸੈੱਟ.fini ();
    }
  };

  /* Dispatch interface. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  return_t dispatch (ਅਚਲ T &obj)
  {
    hb_applicable_t *ਇੰਦਰਾਜ = &array[i++];

    ਇੰਦਰਾਜ->init (obj, ਅੰਕ_glyphs);

    ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t ();
  }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t (); }

  ਤਾਰਕਿਕ stop_sublookup_iteration (return_t r) ਅਚਲ { ਵਾਪਸ ਝੂਠ; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), ਅੰਕ_glyphs (ਅੰਕ_glyphs_2) {}

  hb_applicable_t *array;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs;
  ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
};

ਬਣਤਰ hb_aat_layout_chain_accelerator_t
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ TChain>
  ਸਥਿਰ hb_aat_layout_chain_accelerator_t *create (ਅਚਲ TChain &chain, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = chain.get_subtable_ਗਿਣਤੀ ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗੁਣਨਫਲ;
    ਜੇ (unlikely (hb_unsigned_mul_overflows (ਗਿਣਤੀ_2,
        ਆਕਾਰ (hb_accelerate_subtables_context_t::hb_applicable_t), &ਗੁਣਨਫਲ)))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ;
    ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (ਆਕਾਰ (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * ਆਕਾਰ (hb_accelerate_subtables_context_t::hb_applicable_t), ਗੁਣਨਫਲ, &ਮਾਪ))) {
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    ਸਵੈਚਾਲਿਤ *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, ਮਾਪ);
    ਜੇ (unlikely (!thiz))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    thiz->ਗਿਣਤੀ_2 = ਗਿਣਤੀ_2;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, ਅੰਕ_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    ਵਾਪਸ thiz;
  }

  ਖਾਲੀ destroy ()
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      subtables[i].fini ();
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ChainSubtable
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::HBUINT HBUINT;

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਮਿੱਤਰ ਬਣਤਰ Chain;

  size_t get_ਮਾਪ () ਅਚਲ     { ਵਾਪਸ ਲੰਮਾਈ; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_type () ਅਚਲ     { ਵਾਪਸ coverage & 0xFF; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_coverage () ਅਚਲ { ਵਾਪਸ coverage >> (ਆਕਾਰ (HBUINT) * 8 - 8); }

  ਗਿਣਤੀ Coverage
  {
    Vertical		= 0x80,	/* If set, this subtable will only be applied
				 * to vertical text. If clear, this subtable
				 * will only be applied to horizontal text. */
    Backwards		= 0x40,	/* If set, this subtable will process glyphs
				 * in descending order. If clear, it will
				 * process the glyphs in ascending order. */
    AllDirections	= 0x20,	/* If set, this subtable will be applied to
				 * both horizontal and vertical text (i.e.
				 * the state of bit 0x80000000 is ignored). */
    Logical		= 0x10,	/* If set, this subtable will process glyphs
				 * in logical order (or reverse logical order,
				 * depending on the value of bit 0x80000000). */
  };
  ਗਿਣਤੀ Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_type = get_type ();
    TRACE_DISPATCH (ਇਹ, subtable_type);
    ਚੋਣ (subtable_type) {
    ਹਾਲਤ Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    ਹਾਲਤ Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    ਹਾਲਤ Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    ਹਾਲਤ Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    ਹਾਲਤ Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    ਮੂਲ:			return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!(ਲੰਮਾਈ.sanitize (c) &&
	  hb_barrier () &&
	  ਲੰਮਾਈ >= min_ਮਾਪ &&
	  c->check_range (ਇਹ, ਲੰਮਾਈ)))
      return_trace (ਝੂਠ);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT	ਲੰਮਾਈ;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  ਸੰਘ {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (2 * ਆਕਾਰ (HBUINT) + 4);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Chain
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::HBUINT HBUINT;

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_subtable_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ subtable_ਗਿਣਤੀ_2; }

  hb_ਨਕਾਬ_t compile_flags (ਅਚਲ hb_aat_map_builder_t *map) ਅਚਲ
  {
    hb_ਨਕਾਬ_t flags = defaultFlags;
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = feature_ਗਿਣਤੀ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	ਅਚਲ Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, ਝੂਠ, 0 };
	ਜੇ (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	ਨਹੀਂਤਾਂ ਜੇ (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_ਹੇਠਲਾ_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_ਹੇਠਲਾ_CASE_SMALL_CAPS;
	  ਜਾਓ retry;
	}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT
	ਨਹੀਂਤਾਂ ਜੇ (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_ਟੈਗ_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->ਸਾਰਣੀ.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#ਜੇ_ਅੰਤ
      }
    }
    ਵਾਪਸ flags;
  }

  ਖਾਲੀ apply (hb_aat_apply_context_t *c,
	      ਅਚਲ hb_aat_layout_chain_accelerator_t *accel) ਅਚਲ
  {
    ਅਚਲ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ਗਿਣਤੀ));
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = subtable_ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਤਾਰਕਿਕ reverse;

      ਸਵੈਚਾਲਿਤ coverage = subtable->get_coverage ();

      hb_ਨਕਾਬ_t subtable_flags = subtable->subFeatureFlags;
      ਜੇ (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ਅਚਲ hb_aat_map_t::range_flags_t _) -> ਤਾਰਕਿਕ { ਵਾਪਸ subtable_flags & (_.flags); })))
	ਜਾਓ skip;

      ਜੇ (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->ਬਫਰ->props.direction) !=
	  ਤਾਰਕਿਕ (coverage & ChainSubtable<Types>::Vertical))
	ਜਾਓ skip;

      c->subtable_flags = subtable_flags;
      c->ਪਹਿਲਾ_ਸੈੱਟ = accel ? &accel->subtables[i].glyph_ਸੈੱਟ : &Null(hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : ਖਾਲੀ_ਸੂਚਕ;

      ਜੇ (!c->ਬਫਰ_intersects_machine ())
      {
	(ਖਾਲੀ) c->ਬਫਰ->ਸੁਨੇਹਾ (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_ਸੂਚਕਾਂਕ);
	ਜਾਓ skip;
      }

      /* Buffer contents is always in logical direction.  Determine if
       * we need to reverse before applying this subtable.  We reverse
       * back after if we did reverse indeed.
       *
       * Quoting the spac:
       * """
       * Bits 28 and 30 of the coverage field control the order in which
       * glyphs are processed when the subtable is run by the layout engine.
       * Bit 28 is used to indicate if the glyph processing direction is
       * the same as logical order or layout order. Bit 30 is used to
       * indicate whether glyphs are processed forwards or backwards within
       * that order.

		Bit 30	Bit 28	Interpretation for Horizontal Text
		0	0	The subtable is processed in layout order
				(the same order as the glyphs, which is
				always left-to-right).
		1	0	The subtable is processed in reverse layout order
				(the order opposite that of the glyphs, which is
				always right-to-left).
		0	1	The subtable is processed in logical order
				(the same order as the characters, which may be
				left-to-right or right-to-left).
		1	1	The subtable is processed in reverse logical order
				(the order opposite that of the characters, which
				may be right-to-left or left-to-right).
       */
      reverse = coverage & ChainSubtable<Types>::Logical ?
		ਤਾਰਕਿਕ (coverage & ChainSubtable<Types>::Backwards) :
		ਤਾਰਕਿਕ (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->ਬਫਰ->props.direction);

      ਜੇ (!c->ਬਫਰ->ਸੁਨੇਹਾ (c->font, "start chainsubtable %u", c->lookup_ਸੂਚਕਾਂਕ))
	ਜਾਓ skip;

      ਜੇ (reverse != c->ਬਫਰ_is_reversed)
        c->ਉਲਟ_ਬਫਰ ();

      subtable->apply (c);

      (ਖਾਲੀ) c->ਬਫਰ->ਸੁਨੇਹਾ (c->font, "end chainsubtable %u", c->lookup_ਸੂਚਕਾਂਕ);

      ਜੇ (unlikely (!c->ਬਫਰ->successful)) ਰੋਕੋ;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->ਸੈੱਟ_lookup_ਸੂਚਕਾਂਕ (c->lookup_ਸੂਚਕਾਂਕ + 1);
    }
    ਜੇ (c->ਬਫਰ_is_reversed)
      c->ਉਲਟ_ਬਫਰ ();
  }

  size_t get_ਮਾਪ () ਅਚਲ { ਵਾਪਸ ਲੰਮਾਈ; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਅਚਲ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ਗਿਣਤੀ));
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = subtable_ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਕਿਸਮ_ਨਾਂ context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      ਜੇ (c->stop_sublookup_iteration (ret))
	ਵਾਪਸ ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    ਵਾਪਸ c->default_return_ਮੁੱਲ ();
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੰਸਕਰਣ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!(ਲੰਮਾਈ.sanitize (c) &&
	  hb_barrier () &&
	  ਲੰਮਾਈ >= min_ਮਾਪ &&
	  c->check_range (ਇਹ, ਲੰਮਾਈ)))
      return_trace (ਝੂਠ);

    ਜੇ (!c->check_array (featureZ.arrayZ, feature_ਗਿਣਤੀ))
      return_trace (ਝੂਠ);

    ਅਚਲ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ਗਿਣਤੀ));
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = subtable_ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (!subtable->sanitize (c))
	return_trace (ਝੂਠ);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    ਜੇ (ਸੰਸਕਰਣ >= 3)
    {
      ਅਚਲ SubtableGlyphCoverage *coverage = (ਅਚਲ SubtableGlyphCoverage *) subtable;
      ਜੇ (!coverage->sanitize (c, ਗਿਣਤੀ_2))
        return_trace (ਝੂਠ);
    }

    return_trace (ਸੱਚ);
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	ਲੰਮਾਈ;		/* Total byte count, including this header. */
  HBUINT	feature_ਗਿਣਤੀ;	/* Number of feature subtable entries. */
  HBUINT	subtable_ਗਿਣਤੀ_2;	/* The number of subtables in the chain. */

  Unsized_Array_ਦਾ<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8 + 2 * ਆਕਾਰ (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Types, hb_ਟੈਗ_t ਟੈਗ_3>
ਬਣਤਰ mortmorx
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = ਟੈਗ_3;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ != 0; }

  ਬਣਤਰ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ਇਹ->ਸਾਰਣੀ = sc.reference_ਸਾਰਣੀ<T> (face);

      ਜੇ (unlikely (ਇਹ->ਸਾਰਣੀ->is_blocklisted (ਇਹ->ਸਾਰਣੀ.get_blob (), face)))
      {
        hb_blob_destroy (ਇਹ->ਸਾਰਣੀ.get_blob ());
        ਇਹ->ਸਾਰਣੀ = hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
      }

      ਇਹ->chain_ਗਿਣਤੀ = ਸਾਰਣੀ->get_chain_ਗਿਣਤੀ ();

      ਇਹ->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (ਇਹ->chain_ਗਿਣਤੀ, ਆਕਾਰ (*accels));
      ਜੇ (unlikely (!ਇਹ->accels))
      {
	ਇਹ->chain_ਗਿਣਤੀ = 0;
	ਇਹ->ਸਾਰਣੀ.destroy ();
	ਇਹ->ਸਾਰਣੀ = hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
      }
    }
    ~accelerator_t ()
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਇਹ->chain_ਗਿਣਤੀ; i++)
      {
	ਜੇ (ਇਹ->accels[i])
	  ਇਹ->accels[i]->destroy ();
	hb_free (ਇਹ->accels[i]);
      }
      hb_free (ਇਹ->accels);
      ਇਹ->ਸਾਰਣੀ.destroy ();
    }

    hb_blob_t *get_blob () ਅਚਲ { ਵਾਪਸ ਸਾਰਣੀ.get_blob (); }

    ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (ਬਿਨਾਨਿਸ਼ਾਨੀ chain_ਸੂਚਕਾਂਕ, ਅਚਲ Chain &chain, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
    {
      ਜੇ (unlikely (chain_ਸੂਚਕਾਂਕ >= chain_ਗਿਣਤੀ)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    retry:
      ਸਵੈਚਾਲਿਤ *accel = accels[chain_ਸੂਚਕਾਂਕ].get_acquire ();
      ਜੇ (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, ਅੰਕ_glyphs);
	ਜੇ (unlikely (!accel))
	  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

	ਜੇ (unlikely (!accels[chain_ਸੂਚਕਾਂਕ].cmpexch (ਖਾਲੀ_ਸੂਚਕ, accel)))
	{
	  hb_free (accel);
	  ਜਾਓ retry;
	}
      }

      ਵਾਪਸ accel;
    }

    hb_blob_ptr_t<T> ਸਾਰਣੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ chain_ਗਿਣਤੀ;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  ਖਾਲੀ compile_flags (ਅਚਲ hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ਅਚਲ
  {
    ਅਚਲ Chain<Types> *chain = &ਪਹਿਲਾ_Chain;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = chain_ਗਿਣਤੀ_2;
    ਜੇ (unlikely (!map->chain_flags.resize (ਗਿਣਤੀ_2)))
      ਵਾਪਸ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_ਪਹਿਲਾ,
							     mapper->range_ਆਖਰੀ});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_chain_ਗਿਣਤੀ () ਅਚਲ
  {
    ਵਾਪਸ chain_ਗਿਣਤੀ_2;
  }
  ਖਾਲੀ apply (hb_aat_apply_context_t *c,
	      ਅਚਲ hb_aat_map_t &map,
	      ਅਚਲ accelerator_t &accel) ਅਚਲ
  {
    ਜੇ (unlikely (!c->ਬਫਰ->successful)) ਵਾਪਸ;

    c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat ();

    c->ਤਿਆਰੀ_ਬਫਰ_glyph_ਸੈੱਟ ();

    c->ਸੈੱਟ_lookup_ਸੂਚਕਾਂਕ (0);
    ਅਚਲ Chain<Types> *chain = &ਪਹਿਲਾ_Chain;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = chain_ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਸਵੈਚਾਲਿਤ *chain_accel = accel.get_accel (i, *chain, c->face->get_ਅੰਕ_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      ਜੇ (unlikely (!c->ਬਫਰ->successful)) ਵਾਪਸ;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!(ਸੰਸਕਰਣ.sanitize (c) &&
	  hb_barrier () &&
	  ਸੰਸਕਰਣ &&
	  chain_ਗਿਣਤੀ_2.sanitize (c)))
      return_trace (ਝੂਠ);

    ਅਚਲ Chain<Types> *chain = &ਪਹਿਲਾ_Chain;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = chain_ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (!chain->sanitize (c, ਸੰਸਕਰਣ))
	return_trace (ਝੂਠ);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (ਸੱਚ);
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸੰਸਕਰਣ;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	ਅਣਵਰਤਿਆ_2;		/* Set to 0. */
  HBUINT32	chain_ਗਿਣਤੀ_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	ਪਹਿਲਾ_Chain;	/* Chains. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8);
};

ਬਣਤਰ morx : mortmorx<morx, ExtendedTypes, HB_AAT_ਟੈਗ_morx>
{
  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ਅਚਲ;
};

ਬਣਤਰ mort : mortmorx<mort, ObsoleteTypes, HB_AAT_ਟੈਗ_mort>
{
  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ਅਚਲ;
};

ਬਣਤਰ morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
ਬਣਤਰ mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#ਜੇ_ਅੰਤ /* HB_AAT_LAYOUT_MORX_TABLE_HH */
