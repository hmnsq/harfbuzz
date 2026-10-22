/*
 * Copyright © 2018  Ebrahim Byagowi
 * Copyright © 2018  Google, Inc.
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_LAYOUT_KERX_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_LAYOUT_KERX_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-kern.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-ankr-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_ਟੈਗ_kerx HB_ਟੈਗ('k','e','r','x')


ਨਾਮਾਕਾਸ਼ AAT {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;


ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
kerxTupleKern (ਪੂਰਨ value,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ tuple_ਗਿਣਤੀ,
	       ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
	       hb_aat_apply_context_t *c)
{
  ਜੇ (likely (!tuple_ਗਿਣਤੀ || !c)) ਵਾਪਸ value;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = value;
  ਅਚਲ FWORD *pv = &Struct_At_ਵਿੱਥ<FWORD> (ਅਧਾਰ, ਵਿੱਥ);
  ਜੇ (unlikely (!c->sanitizer.check_array (pv, tuple_ਗਿਣਤੀ))) ਵਾਪਸ 0;
  hb_barrier ();
  ਵਾਪਸ *pv;
}


ਬਣਤਰ hb_glyph_pair_t
{
  hb_codepoint_t ਖੱਬੇ;
  hb_codepoint_t ਸੱਜੇ;
};

ਬਣਤਰ KernPair
{
  ਪੂਰਨ get_kerning () ਅਚਲ { ਵਾਪਸ value; }

  ਪੂਰਨ cmp (ਅਚਲ hb_glyph_pair_t &o) ਅਚਲ
  {
    ਪੂਰਨ ret = ਖੱਬੇ.cmp (o.ਖੱਬੇ);
    ਜੇ (ret) ਵਾਪਸ ret;
    ਵਾਪਸ ਸੱਜੇ.cmp (o.ਸੱਜੇ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBGlyphID16	ਖੱਬੇ;
  HBGlyphID16	ਸੱਜੇ;
  FWORD		value;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>
ਬਣਤਰ Kerx_Sub_ਸਾਰਣੀ_Format0
{
  ਪੂਰਨ get_kerning (hb_codepoint_t ਖੱਬੇ, hb_codepoint_t ਸੱਜੇ,
		   hb_aat_apply_context_t *c = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    hb_glyph_pair_t pair = {ਖੱਬੇ, ਸੱਜੇ};
    ਪੂਰਨ v = ਜੋੜੇ.bsearch (pair).get_kerning ();
    ਵਾਪਸ kerxTupleKern (v, ਸਿਰਲੇਖ.tuple_ਗਿਣਤੀ_2 (), ਇਹ, c);
  }

  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਜੇ (!c->plan->requested_kerning)
      return_trace (ਝੂਠ);

    ਜੇ (ਸਿਰਲੇਖ.coverage & ਸਿਰਲੇਖ.Backwards)
      return_trace (ਝੂਠ);

    accelerator_t accel (*ਇਹ, c);
    hb_kern_machine_t<accelerator_t> machine (accel, ਸਿਰਲੇਖ.coverage & ਸਿਰਲੇਖ.Cross_ਧਾਰਾ);
    machine.kern (c->font, c->ਬਫਰ, c->plan->kern_ਨਕਾਬ);

    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &ਪਹਿਲਾ_ਸੈੱਟ, ਸੈੱਟ_t &ਦੂਜਾ_ਸੈੱਟ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਲਈ (ਅਚਲ KernPair& pair : ਜੋੜੇ)
    {
      ਪਹਿਲਾ_ਸੈੱਟ.ਜੋੜੋ (pair.ਖੱਬੇ);
      ਦੂਜਾ_ਸੈੱਟ.ਜੋੜੋ (pair.ਸੱਜੇ);
    }
  }

  ਬਣਤਰ accelerator_t
  {
    ਅਚਲ Kerx_Sub_ਸਾਰਣੀ_Format0 &ਸਾਰਣੀ;
    hb_aat_apply_context_t *c;

    accelerator_t (ਅਚਲ Kerx_Sub_ਸਾਰਣੀ_Format0 &ਸਾਰਣੀ_2,
		   hb_aat_apply_context_t *c_) :
		     ਸਾਰਣੀ (ਸਾਰਣੀ_2), c (c_) {}

    ਪੂਰਨ get_kerning (hb_codepoint_t ਖੱਬੇ, hb_codepoint_t ਸੱਜੇ) ਅਚਲ
    {
      ਜੇ (!(*c->ਪਹਿਲਾ_ਸੈੱਟ)[ਖੱਬੇ] || !(*c->ਦੂਜਾ_ਸੈੱਟ)[ਸੱਜੇ]) ਵਾਪਸ 0;
      ਵਾਪਸ ਸਾਰਣੀ.get_kerning (ਖੱਬੇ, ਸੱਜੇ, c);
    }
  };


  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (ਜੋੜੇ.sanitize (c)));
  }

  ਸੁਰੱਖਿਅਤ:
  Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ	ਸਿਰਲੇਖ;
  Bin_ਖੋਜ_Array_ਦਾ<KernPair, ਕਿਸਮ_ਨਾਂ Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::Types::HBUINT>
			ਜੋੜੇ;	/* Sorted kern records. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::static_ਮਾਪ + 16, ਜੋੜੇ);
};


ਸਾਂਚਾ <ਤਾਰਕਿਕ extended>
ਬਣਤਰ Format1_ਇੰਦਰਾਜ;

ਸਾਂਚਾ <>
ਬਣਤਰ Format1_ਇੰਦਰਾਜ<ਸੱਚ>
{
  ਗਿਣਤੀ Flags
  {
    ਧੱਕੋ		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  ਬਣਤਰ ਇੰਦਰਾਜ_ਅੰਕੜੇ
  {
    HBUINT16	kern_Action_ਸੂਚਕਾਂਕ;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    ਜਨਤਕ:
    DEFINE_ਮਾਪ_STATIC (2);
  };

  ਸਥਿਰ ਤਾਰਕਿਕ initiateAction (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.flags & ਧੱਕੋ; }

  ਸਥਿਰ ਤਾਰਕਿਕ performAction (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.ਅੰਕੜੇ.kern_Action_ਸੂਚਕਾਂਕ != 0xFFFF; }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ kern_Action_ਸੂਚਕਾਂਕ (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.ਅੰਕੜੇ.kern_Action_ਸੂਚਕਾਂਕ; }
};
ਸਾਂਚਾ <>
ਬਣਤਰ Format1_ਇੰਦਰਾਜ<ਝੂਠ>
{
  ਗਿਣਤੀ Flags
  {
    ਧੱਕੋ		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    ਵਿੱਥ_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ ਇੰਦਰਾਜ_ਅੰਕੜੇ;

  ਸਥਿਰ ਤਾਰਕਿਕ initiateAction (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.flags & ਧੱਕੋ; }

  ਸਥਿਰ ਤਾਰਕਿਕ performAction (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.flags & ਵਿੱਥ_2; }

  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ kern_Action_ਸੂਚਕਾਂਕ (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
  { ਵਾਪਸ ਇੰਦਰਾਜ.flags & ਵਿੱਥ_2; }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>
ਬਣਤਰ Kerx_Sub_ਸਾਰਣੀ_Format1
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::Types Types;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::HBUINT HBUINT;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Format1_ਇੰਦਰਾਜ<Types::extended> Format1_ਇੰਦਰਾਜ_T;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Format1_ਇੰਦਰਾਜ_T::ਇੰਦਰਾਜ_ਅੰਕੜੇ ਇੰਦਰਾਜ_ਅੰਕੜੇ;

  ਗਿਣਤੀ Flags
  {
    DontAdvance	= Format1_ਇੰਦਰਾਜ_T::DontAdvance,
  };

  ਤਾਰਕਿਕ is_action_initiable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਵਾਪਸ Format1_ਇੰਦਰਾਜ_T::initiateAction (ਇੰਦਰਾਜ);
  }
  ਤਾਰਕਿਕ is_actionable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਵਾਪਸ Format1_ਇੰਦਰਾਜ_T::performAction (ਇੰਦਰਾਜ);
  }

  ਬਣਤਰ driver_context_t
  {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ in_place = ਸੱਚ;

    driver_context_t (ਅਚਲ Kerx_Sub_ਸਾਰਣੀ_Format1 *ਸਾਰਣੀ_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	ਸਾਰਣੀ (ਸਾਰਣੀ_2),
	/* Apparently the offset kernAction is from the beginning of the state-machine,
	 * similar to offsets in morx table, NOT from beginning of this table, like
	 * other subtables in kerx.  Discovered via testing. */
	kernAction (&ਸਾਰਣੀ->machine + ਸਾਰਣੀ->kernAction),
	ਡੂੰਘਾਈ (0),
	cross_ਧਾਰਾ (ਸਾਰਣੀ->ਸਿਰਲੇਖ.coverage & ਸਾਰਣੀ->ਸਿਰਲੇਖ.Cross_ਧਾਰਾ) {}

    HB_AAT_TRANSITION_INLINE
    ਖਾਲੀ transition (hb_ਬਫਰ_t *ਬਫਰ,
		     State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> *driver,
		     ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = ਇੰਦਰਾਜ.flags;

      ਜੇ (flags & Format1_ਇੰਦਰਾਜ_T::Reset)
	ਡੂੰਘਾਈ = 0;

      ਜੇ (flags & Format1_ਇੰਦਰਾਜ_T::ਧੱਕੋ)
      {
	ਜੇ (likely (ਡੂੰਘਾਈ < ARRAY_ਲੰਮਾਈ (ਢੇਰ)))
	  ਢੇਰ[ਡੂੰਘਾਈ++] = ਬਫਰ->idx;
	ਨਹੀਂਤਾਂ
	  ਡੂੰਘਾਈ = 0; /* Probably not what CoreText does, but better? */
      }

      ਜੇ (Format1_ਇੰਦਰਾਜ_T::performAction (ਇੰਦਰਾਜ) && ਡੂੰਘਾਈ)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ tuple_ਗਿਣਤੀ_2 = hb_max (1u, ਸਾਰਣੀ->ਸਿਰਲੇਖ.tuple_ਗਿਣਤੀ_2 ());

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ kern_idx = Format1_ਇੰਦਰਾਜ_T::kern_Action_ਸੂਚਕਾਂਕ (ਇੰਦਰਾਜ);
	kern_idx = Types::byte_ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (kern_idx, &ਸਾਰਣੀ->machine, kernAction.arrayZ);
	ਅਚਲ FWORD *actions = &kernAction[kern_idx];
	ਜੇ (!c->sanitizer.check_array (actions, ਡੂੰਘਾਈ, tuple_ਗਿਣਤੀ_2))
	{
	  ਡੂੰਘਾਈ = 0;
	  ਵਾਪਸ;
	}
	hb_barrier ();

	hb_ਨਕਾਬ_t kern_ਨਕਾਬ = c->plan->kern_ਨਕਾਬ;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	ਤਾਰਕਿਕ ਆਖਰੀ = ਝੂਠ;
	ਜਦੋਂ (!ਆਖਰੀ && ਡੂੰਘਾਈ)
	{
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx = ਢੇਰ[--ਡੂੰਘਾਈ];
	  ਪੂਰਨ v = *actions;
	  actions += tuple_ਗਿਣਤੀ_2;
	  ਜੇ (idx >= ਬਫਰ->len) ਜਾਰੀ;

	  /* "The end of the list is marked by an odd value..." */
	  ਆਖਰੀ = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = ਬਫਰ->pos[idx];

	  ਜੇ (HB_DIRECTION_IS_HORIZONTAL (ਬਫਰ->props.direction))
	  {
	    ਜੇ (cross_ਧਾਰਾ)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      ਜੇ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_ਵਿੱਥ_2 = 0;
	      }
	      ਨਹੀਂਤਾਂ ਜੇ (o.attach_type())
	      {
		o.y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (o.y_ਵਿੱਥ_2, c->font->em_scale_y (v));
		ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    ਨਹੀਂਤਾਂ ਜੇ (ਬਫਰ->info[idx].ਨਕਾਬ & kern_ਨਕਾਬ)
	    {
	      ਸਵੈਚਾਲਿਤ scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_ਜੋੜੋ (o.x_advance, scaled);
	      o.x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (o.x_ਵਿੱਥ_2, scaled);
	    }
	  }
	  ਨਹੀਂਤਾਂ
	  {
	    ਜੇ (cross_ਧਾਰਾ)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      ਜੇ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_ਵਿੱਥ_2 = 0;
	      }
	      ਨਹੀਂਤਾਂ ਜੇ (o.attach_type())
	      {
		o.x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (o.x_ਵਿੱਥ_2, c->font->em_scale_x (v));
		ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    ਨਹੀਂਤਾਂ ਜੇ (ਬਫਰ->info[idx].ਨਕਾਬ & kern_ਨਕਾਬ)
	    {
	      ਸਵੈਚਾਲਿਤ scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_ਜੋੜੋ (o.y_advance, scaled);
	      o.y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (o.y_ਵਿੱਥ_2, scaled);
	    }
	  }
	}
      }
    }

    ਜਨਤਕ:
    hb_aat_apply_context_t *c;
    ਅਚਲ Kerx_Sub_ਸਾਰਣੀ_Format1 *ਸਾਰਣੀ;
    ਨਿਜੀ:
    ਅਚਲ Unsized_Array_ਦਾ<FWORD> &kernAction;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਢੇਰ[8];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਡੂੰਘਾਈ;
    ਤਾਰਕਿਕ cross_ਧਾਰਾ;
  };

  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਜੇ (!c->plan->requested_kerning &&
	!(ਸਿਰਲੇਖ.coverage & ਸਿਰਲੇਖ.Cross_ਧਾਰਾ))
      ਵਾਪਸ ਝੂਠ;

    driver_context_t dc (ਇਹ, c);

    State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (ਇਹ) &&
			  machine.sanitize (c)));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &ਪਹਿਲਾ_ਸੈੱਟ, ਸੈੱਟ_t &ਦੂਜਾ_ਸੈੱਟ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    machine.collect_initial_glyphs (ਪਹਿਲਾ_ਸੈੱਟ, ਅੰਕ_glyphs, *ਇਹ);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  ਸੁਰੱਖਿਅਤ:
  Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ				ਸਿਰਲੇਖ;
  State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>			machine;
  NNOffset_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<FWORD>, HBUINT>	kernAction;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::static_ਮਾਪ + (State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>::static_ਮਾਪ + HBUINT::static_ਮਾਪ));
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>
ਬਣਤਰ Kerx_Sub_ਸਾਰਣੀ_Format2
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::Types Types;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::HBUINT HBUINT;

  ਪੂਰਨ get_kerning (hb_codepoint_t ਖੱਬੇ, hb_codepoint_t ਸੱਜੇ,
		   hb_aat_apply_context_t *c) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs = c->sanitizer.get_ਅੰਕ_glyphs ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = (ਇਹ+ਖੱਬਾ_Class_ਸਾਰਣੀ).get_class (ਖੱਬੇ, ਅੰਕ_glyphs, 0);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = (ਇਹ+ਸੱਜਾ_Class_ਸਾਰਣੀ).get_class (ਸੱਜੇ, ਅੰਕ_glyphs, 0);

    ਅਚਲ Unsized_Array_ਦਾ<FWORD> &arrayZ = ਇਹ+array;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ kern_idx = l + r;
    kern_idx = Types::ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (kern_idx, ਇਹ, arrayZ.arrayZ);
    ਅਚਲ FWORD *v = &arrayZ[kern_idx];
    ਜੇ (unlikely (!v->sanitize (&c->sanitizer))) ਵਾਪਸ 0;
    hb_barrier ();

    ਵਾਪਸ kerxTupleKern (*v, ਸਿਰਲੇਖ.tuple_ਗਿਣਤੀ_2 (), ਇਹ, c);
  }

  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਜੇ (!c->plan->requested_kerning)
      return_trace (ਝੂਠ);

    ਜੇ (ਸਿਰਲੇਖ.coverage & ਸਿਰਲੇਖ.Backwards)
      return_trace (ਝੂਠ);

    accelerator_t accel (*ਇਹ, c);
    hb_kern_machine_t<accelerator_t> machine (accel, ਸਿਰਲੇਖ.coverage & ਸਿਰਲੇਖ.Cross_ਧਾਰਾ);
    machine.kern (c->font, c->ਬਫਰ, c->plan->kern_ਨਕਾਬ);

    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &ਪਹਿਲਾ_ਸੈੱਟ, ਸੈੱਟ_t &ਦੂਜਾ_ਸੈੱਟ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    (ਇਹ+ਖੱਬਾ_Class_ਸਾਰਣੀ).collect_glyphs (ਪਹਿਲਾ_ਸੈੱਟ, ਅੰਕ_glyphs);
    (ਇਹ+ਸੱਜਾ_Class_ਸਾਰਣੀ).collect_glyphs (ਦੂਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs);
  }

  ਬਣਤਰ accelerator_t
  {
    ਅਚਲ Kerx_Sub_ਸਾਰਣੀ_Format2 &ਸਾਰਣੀ;
    hb_aat_apply_context_t *c;

    accelerator_t (ਅਚਲ Kerx_Sub_ਸਾਰਣੀ_Format2 &ਸਾਰਣੀ_2,
		   hb_aat_apply_context_t *c_) :
		     ਸਾਰਣੀ (ਸਾਰਣੀ_2), c (c_) {}

    ਪੂਰਨ get_kerning (hb_codepoint_t ਖੱਬੇ, hb_codepoint_t ਸੱਜੇ) ਅਚਲ
    {
      ਜੇ (!(*c->ਪਹਿਲਾ_ਸੈੱਟ)[ਖੱਬੇ] || !(*c->ਦੂਜਾ_ਸੈੱਟ)[ਸੱਜੇ]) ਵਾਪਸ 0;
      ਵਾਪਸ ਸਾਰਣੀ.get_kerning (ਖੱਬੇ, ਸੱਜੇ, c);
    }
  };

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  ਖੱਬਾ_Class_ਸਾਰਣੀ.sanitize (c, ਇਹ) &&
			  ਸੱਜਾ_Class_ਸਾਰਣੀ.sanitize (c, ਇਹ) &&
			  hb_barrier () &&
			  c->check_range (ਇਹ, array)));
  }

  ਸੁਰੱਖਿਅਤ:
  Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ	ਸਿਰਲੇਖ;
  HBUINT		ਪੰਕਤੀ_ਚੌੜਾਈ;	/* The width, in bytes, of a row in the table. */
  NNOffset_ਮੰਜ਼ਿਲ<ਕਿਸਮ_ਨਾਂ Types::ClassTypeWide, HBUINT>
			ਖੱਬਾ_Class_ਸਾਰਣੀ;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffset_ਮੰਜ਼ਿਲ<ਕਿਸਮ_ਨਾਂ Types::ClassTypeWide, HBUINT>
			ਸੱਜਾ_Class_ਸਾਰਣੀ;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffset_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::static_ਮਾਪ + 4 * ਆਕਾਰ (HBUINT));
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>
ਬਣਤਰ Kerx_Sub_ਸਾਰਣੀ_Format4
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ExtendedTypes Types;

  ਬਣਤਰ ਇੰਦਰਾਜ_ਅੰਕੜੇ
  {
    HBUINT16	ankr_Action_ਸੂਚਕਾਂਕ;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    ਜਨਤਕ:
    DEFINE_ਮਾਪ_STATIC (2);
  };

  ਗਿਣਤੀ Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  ਤਾਰਕਿਕ is_action_initiable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਵਾਪਸ (ਇੰਦਰਾਜ.flags & Mark);
  }
  ਤਾਰਕਿਕ is_actionable (ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ) ਅਚਲ
  {
    ਵਾਪਸ ਇੰਦਰਾਜ.ਅੰਕੜੇ.ankr_Action_ਸੂਚਕਾਂਕ != 0xFFFF;
  }

  ਬਣਤਰ driver_context_t
  {
    ਸਥਿਰ constexpr ਤਾਰਕਿਕ in_place = ਸੱਚ;
    ਗਿਣਤੀ Sub_ਸਾਰਣੀ_Flags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      ਅਣਵਰਤਿਆ		= 0x3F000000,	/* Unused - must be zero. */
      ਵਿੱਥ_2		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ਅਚਲ Kerx_Sub_ਸਾਰਣੀ_Format4 *ਸਾਰਣੀ_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	ਸਾਰਣੀ (ਸਾਰਣੀ_2),
	action_type ((ਸਾਰਣੀ->flags & ActionType) >> 30),
	ankr_ਅੰਕੜੇ ((HBUINT16 *) ((ਅਚਲ ਅੱਖਰ *) &ਸਾਰਣੀ->machine + (ਸਾਰਣੀ->flags & ਵਿੱਥ_2))),
	mark_ਸੈੱਟ (ਝੂਠ),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    ਖਾਲੀ transition (hb_ਬਫਰ_t *ਬਫਰ,
		     State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> *driver,
		     ਅਚਲ ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ> &ਇੰਦਰਾਜ)
    {
      ਜੇ (mark_ਸੈੱਟ && ਇੰਦਰਾਜ.ਅੰਕੜੇ.ankr_Action_ਸੂਚਕਾਂਕ != 0xFFFF && ਬਫਰ->idx < ਬਫਰ->len)
      {
	hb_glyph_position_t &o = ਬਫਰ->cur_pos();
	ਚੋਣ (action_type)
	{
	  ਹਾਲਤ 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ਅਚਲ HBUINT16 *ਅੰਕੜੇ = &ankr_ਅੰਕੜੇ[ਇੰਦਰਾਜ.ਅੰਕੜੇ.ankr_Action_ਸੂਚਕਾਂਕ * 2];
	    ਜੇ (!c->sanitizer.check_array (ਅੰਕੜੇ, 2)) ਵਾਪਸ;
	    hb_barrier ();
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark_Control_ਬਿੰਦੂ = *ਅੰਕੜੇ++;
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ curr_Control_ਬਿੰਦੂ = *ਅੰਕੜੇ++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    ਜੇ (!c->font->get_glyph_contour_ਬਿੰਦੂ_for_origin (c->ਬਫਰ->info[mark].codepoint,
							      mark_Control_ਬਿੰਦੂ,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_ਬਿੰਦੂ_for_origin (c->ਬਫਰ->cur ().codepoint,
							      curr_Control_ਬਿੰਦੂ,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      ਵਾਪਸ;

	    o.x_ਵਿੱਥ_2 = hb_saturate_sub (markX, currX);
	    o.y_ਵਿੱਥ_2 = hb_saturate_sub (markY, currY);
	  }
	  ਰੋਕੋ;

	  ਹਾਲਤ 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ਅਚਲ HBUINT16 *ਅੰਕੜੇ = &ankr_ਅੰਕੜੇ[ਇੰਦਰਾਜ.ਅੰਕੜੇ.ankr_Action_ਸੂਚਕਾਂਕ * 2];
	    ਜੇ (!c->sanitizer.check_array (ਅੰਕੜੇ, 2)) ਵਾਪਸ;
	    hb_barrier ();
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark_Anchor_ਬਿੰਦੂ = *ਅੰਕੜੇ++;
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ curr_Anchor_ਬਿੰਦੂ = *ਅੰਕੜੇ++;
	    ਅਚਲ Anchor &markAnchor = c->ankr_ਸਾਰਣੀ->get_anchor (c->ਬਫਰ->info[mark].codepoint,
								  mark_Anchor_ਬਿੰਦੂ,
								  c->sanitizer.get_ਅੰਕ_glyphs ());
	    ਅਚਲ Anchor &currAnchor = c->ankr_ਸਾਰਣੀ->get_anchor (c->ਬਫਰ->cur ().codepoint,
								  curr_Anchor_ਬਿੰਦੂ,
								  c->sanitizer.get_ਅੰਕ_glyphs ());

	    o.x_ਵਿੱਥ_2 = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_ਵਿੱਥ_2 = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  ਰੋਕੋ;

	  ਹਾਲਤ 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ਅਚਲ FWORD *ਅੰਕੜੇ = (ਅਚਲ FWORD *) &ankr_ਅੰਕੜੇ[ਇੰਦਰਾਜ.ਅੰਕੜੇ.ankr_Action_ਸੂਚਕਾਂਕ * 4];
	    ਜੇ (!c->sanitizer.check_array (ਅੰਕੜੇ, 4)) ਵਾਪਸ;
	    hb_barrier ();
	    ਪੂਰਨ markX = *ਅੰਕੜੇ++;
	    ਪੂਰਨ markY = *ਅੰਕੜੇ++;
	    ਪੂਰਨ currX = *ਅੰਕੜੇ++;
	    ਪੂਰਨ currY = *ਅੰਕੜੇ++;

	    o.x_ਵਿੱਥ_2 = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_ਵਿੱਥ_2 = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  ਰੋਕੋ;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (ਪੂਰਨ) mark - (ਪੂਰਨ) ਬਫਰ->idx;
	ਜੇ (c->ਬਫਰ_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      ਜੇ (ਇੰਦਰਾਜ.flags & Mark)
      {
	mark_ਸੈੱਟ = ਸੱਚ;
	mark = ਬਫਰ->idx;
      }
    }

    ਜਨਤਕ:
    hb_aat_apply_context_t *c;
    ਅਚਲ Kerx_Sub_ਸਾਰਣੀ_Format4 *ਸਾਰਣੀ;
    ਨਿਜੀ:
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ action_type;
    ਅਚਲ HBUINT16 *ankr_ਅੰਕੜੇ;
    ਤਾਰਕਿਕ mark_ਸੈੱਟ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark;
  };

  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    driver_context_t dc (ਇਹ, c);

    State_ਸਾਰਣੀ_Driver<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (ਇਹ) &&
			  machine.sanitize (c)));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &ਪਹਿਲਾ_ਸੈੱਟ, ਸੈੱਟ_t &ਦੂਜਾ_ਸੈੱਟ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    machine.collect_initial_glyphs (ਪਹਿਲਾ_ਸੈੱਟ, ਅੰਕ_glyphs, *ਇਹ);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  ਸੁਰੱਖਿਅਤ:
  Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ		ਸਿਰਲੇਖ;
  State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>	machine;
  HBUINT32			flags;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::static_ਮਾਪ + (State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>::static_ਮਾਪ + HBUINT32::static_ਮਾਪ));
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>
ਬਣਤਰ Kerx_Sub_ਸਾਰਣੀ_Format6
{
  ਗਿਣਤੀ Flags
  {
    ਮੁੱਲਾਂ_Are_Long	= 0x00000001,
  };

  ਤਾਰਕਿਕ is_long () ਅਚਲ { ਵਾਪਸ flags & ਮੁੱਲਾਂ_Are_Long; }

  ਪੂਰਨ get_kerning (hb_codepoint_t ਖੱਬੇ, hb_codepoint_t ਸੱਜੇ,
		   hb_aat_apply_context_t *c) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs = c->sanitizer.get_ਅੰਕ_glyphs ();
    ਜੇ (is_long ())
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &t = u.l;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = (ਇਹ+t.ਪੰਕਤੀ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ).get_ਮੁੱਲ_or_null (ਖੱਬੇ, ਅੰਕ_glyphs);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = (ਇਹ+t.ਕਾਲਮ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ).get_ਮੁੱਲ_or_null (ਸੱਜੇ, ਅੰਕ_glyphs);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = l + r;
      ਜੇ (unlikely (ਵਿੱਥ < l)) ਵਾਪਸ 0; /* Addition overflow. */
      ਜੇ (unlikely (hb_unsigned_mul_overflows (ਵਿੱਥ, ਆਕਾਰ (FWORD32)))) ਵਾਪਸ 0;
      ਅਚਲ FWORD32 *v = &Struct_At_ਵਿੱਥ<FWORD32> (&(ਇਹ+t.array), ਵਿੱਥ * ਆਕਾਰ (FWORD32));
      ਜੇ (unlikely (!v->sanitize (&c->sanitizer))) ਵਾਪਸ 0;
      hb_barrier ();
      ਵਾਪਸ kerxTupleKern (*v, ਸਿਰਲੇਖ.tuple_ਗਿਣਤੀ_2 (), &(ਇਹ+vector), c);
    }
    ਨਹੀਂਤਾਂ
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &t = u.s;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ l = (ਇਹ+t.ਪੰਕਤੀ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ).get_ਮੁੱਲ_or_null (ਖੱਬੇ, ਅੰਕ_glyphs);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r = (ਇਹ+t.ਕਾਲਮ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ).get_ਮੁੱਲ_or_null (ਸੱਜੇ, ਅੰਕ_glyphs);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = l + r;
      ਅਚਲ FWORD *v = &Struct_At_ਵਿੱਥ<FWORD> (&(ਇਹ+t.array), ਵਿੱਥ * ਆਕਾਰ (FWORD));
      ਜੇ (unlikely (!v->sanitize (&c->sanitizer))) ਵਾਪਸ 0;
      hb_barrier ();
      ਵਾਪਸ kerxTupleKern (*v, ਸਿਰਲੇਖ.tuple_ਗਿਣਤੀ_2 (), &(ਇਹ+vector), c);
    }
  }

  ਤਾਰਕਿਕ apply (hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਜੇ (!c->plan->requested_kerning)
      return_trace (ਝੂਠ);

    ਜੇ (ਸਿਰਲੇਖ.coverage & ਸਿਰਲੇਖ.Backwards)
      return_trace (ਝੂਠ);

    accelerator_t accel (*ਇਹ, c);
    hb_kern_machine_t<accelerator_t> machine (accel, ਸਿਰਲੇਖ.coverage & ਸਿਰਲੇਖ.Cross_ਧਾਰਾ);
    machine.kern (c->font, c->ਬਫਰ, c->plan->kern_ਨਕਾਬ);

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.ਪੰਕਤੀ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ.sanitize (c, ਇਹ) &&
			     u.l.ਕਾਲਮ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ.sanitize (c, ਇਹ) &&
			     c->check_range (ਇਹ, u.l.array)
			   ) : (
			     u.s.ਪੰਕਤੀ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ.sanitize (c, ਇਹ) &&
			     u.s.ਕਾਲਮ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ.sanitize (c, ਇਹ) &&
			     c->check_range (ਇਹ, u.s.array)
			   )) &&
			  (ਸਿਰਲੇਖ.tuple_ਗਿਣਤੀ_2 () == 0 ||
			   c->check_range (ਇਹ, vector))));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &ਪਹਿਲਾ_ਸੈੱਟ, ਸੈੱਟ_t &ਦੂਜਾ_ਸੈੱਟ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਜੇ (is_long ())
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &t = u.l;
      (ਇਹ+t.ਪੰਕਤੀ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ).collect_glyphs (ਪਹਿਲਾ_ਸੈੱਟ, ਅੰਕ_glyphs);
      (ਇਹ+t.ਕਾਲਮ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ).collect_glyphs (ਦੂਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs);
    }
    ਨਹੀਂਤਾਂ
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &t = u.s;
      (ਇਹ+t.ਪੰਕਤੀ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ).collect_glyphs (ਪਹਿਲਾ_ਸੈੱਟ, ਅੰਕ_glyphs);
      (ਇਹ+t.ਕਾਲਮ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ).collect_glyphs (ਦੂਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs);
    }
  }

  ਬਣਤਰ accelerator_t
  {
    ਅਚਲ Kerx_Sub_ਸਾਰਣੀ_Format6 &ਸਾਰਣੀ;
    hb_aat_apply_context_t *c;

    accelerator_t (ਅਚਲ Kerx_Sub_ਸਾਰਣੀ_Format6 &ਸਾਰਣੀ_2,
		   hb_aat_apply_context_t *c_) :
		     ਸਾਰਣੀ (ਸਾਰਣੀ_2), c (c_) {}

    ਪੂਰਨ get_kerning (hb_codepoint_t ਖੱਬੇ, hb_codepoint_t ਸੱਜੇ) ਅਚਲ
    {
      ਜੇ (!(*c->ਪਹਿਲਾ_ਸੈੱਟ)[ਖੱਬੇ] || !(*c->ਦੂਜਾ_ਸੈੱਟ)[ਸੱਜੇ]) ਵਾਪਸ 0;
      ਵਾਪਸ ਸਾਰਣੀ.get_kerning (ਖੱਬੇ, ਸੱਜੇ, c);
    }
  };

  ਸੁਰੱਖਿਅਤ:
  Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ		ਸਿਰਲੇਖ;
  HBUINT32			flags;
  HBUINT16			ਪੰਕਤੀ_ਗਿਣਤੀ_2;
  HBUINT16			ਕਾਲਮ_ਗਿਣਤੀ;
  ਸੰਘ U
  {
    ਬਣਤਰ Long
    {
      NNOffset32_ਮੰਜ਼ਿਲ<Lookup<HBUINT32>>		ਪੰਕਤੀ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ;
      NNOffset32_ਮੰਜ਼ਿਲ<Lookup<HBUINT32>>		ਕਾਲਮ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ;
      NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<FWORD32>>	array;
    } l;
    ਬਣਤਰ Short
    {
      NNOffset32_ਮੰਜ਼ਿਲ<Lookup<HBUINT16>>		ਪੰਕਤੀ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ;
      NNOffset32_ਮੰਜ਼ਿਲ<Lookup<HBUINT16>>		ਕਾਲਮ_ਸੂਚਕਾਂਕ_ਸਾਰਣੀ;
      NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<FWORD>>	array;
    } s;
  } u;
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<FWORD>>	vector;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::static_ਮਾਪ + 24);
};


ਬਣਤਰ Kerx_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ExtendedTypes Types;

  ਬਿਨਾਨਿਸ਼ਾਨੀ   tuple_ਗਿਣਤੀ_2 () ਅਚਲ { ਵਾਪਸ tuple_ਗਿਣਤੀ; }
  ਤਾਰਕਿਕ     is_horizontal () ਅਚਲ { ਵਾਪਸ !(coverage & Vertical); }

  ਗਿਣਤੀ Coverage
  {
    Vertical	= 0x80000000u,	/* Set if table has vertical kerning values. */
    Cross_ਧਾਰਾ	= 0x40000000u,	/* Set if table has cross-stream kerning values. */
    Variation	= 0x20000000u,	/* Set if table has variation kerning values. */
    Backwards	= 0x10000000u,	/* If clear, process the glyphs forwards, that
				 * is, from first to last in the glyph stream.
				 * If we, process them from last to first.
				 * This flag only applies to state-table based
				 * 'kerx' subtables (types 1 and 4). */
    Reserved	= 0x0FFFFF00u,	/* Reserved, set to zero. */
    SubtableType= 0x000000FFu,	/* Subtable type. */
  };

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBUINT32	ਲੰਮਾਈ;
  HBUINT32	coverage;
  HBUINT32	tuple_ਗਿਣਤੀ;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (12);
};

ਬਣਤਰ Kerx_Sub_ਸਾਰਣੀ
{
  ਮਿੱਤਰ ਬਣਤਰ kerx;

  size_t get_ਮਾਪ () ਅਚਲ { ਵਾਪਸ u.ਸਿਰਲੇਖ.ਲੰਮਾਈ; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_type () ਅਚਲ { ਵਾਪਸ u.ਸਿਰਲੇਖ.coverage & u.ਸਿਰਲੇਖ.SubtableType; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_type = get_type ();
    TRACE_DISPATCH (ਇਹ, subtable_type);
    ਚੋਣ (subtable_type) {
    ਹਾਲਤ 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    ਹਾਲਤ 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ਹਾਲਤ 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ਹਾਲਤ 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ਹਾਲਤ 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    ਮੂਲ:	return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &ਪਹਿਲਾ_ਸੈੱਟ, ਸੈੱਟ_t &ਦੂਜਾ_ਸੈੱਟ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_type = get_type ();
    ਚੋਣ (subtable_type) {
    ਹਾਲਤ 0:	hb_barrier (); u.format0.collect_glyphs (ਪਹਿਲਾ_ਸੈੱਟ, ਦੂਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਹਾਲਤ 1:	hb_barrier (); u.format1.collect_glyphs (ਪਹਿਲਾ_ਸੈੱਟ, ਦੂਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਹਾਲਤ 2:	hb_barrier (); u.format2.collect_glyphs (ਪਹਿਲਾ_ਸੈੱਟ, ਦੂਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਹਾਲਤ 4:	hb_barrier (); u.format4.collect_glyphs (ਪਹਿਲਾ_ਸੈੱਟ, ਦੂਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਹਾਲਤ 6:	hb_barrier (); u.format6.collect_glyphs (ਪਹਿਲਾ_ਸੈੱਟ, ਦੂਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਮੂਲ:	ਵਾਪਸ;
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!(u.ਸਿਰਲੇਖ.sanitize (c) &&
	  hb_barrier () &&
	  u.ਸਿਰਲੇਖ.ਲੰਮਾਈ >= u.ਸਿਰਲੇਖ.static_ਮਾਪ &&
	  c->check_range (ਇਹ, u.ਸਿਰਲੇਖ.ਲੰਮਾਈ)))
      return_trace (ਝੂਠ);

    return_trace (dispatch (c));
  }

  ਜਨਤਕ:
  ਸੰਘ {
  Kerx_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ				ਸਿਰਲੇਖ;
  Kerx_Sub_ਸਾਰਣੀ_Format0<Kerx_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>	format0;
  Kerx_Sub_ਸਾਰਣੀ_Format1<Kerx_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>	format1;
  Kerx_Sub_ਸਾਰਣੀ_Format2<Kerx_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>	format2;
  Kerx_Sub_ਸਾਰਣੀ_Format4<Kerx_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>	format4;
  Kerx_Sub_ਸਾਰਣੀ_Format6<Kerx_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>	format6;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (12);
};


/*
 * The 'kerx' Table
 */

ਬਣਤਰ kern_subtable_accelerator_ਅੰਕੜੇ_t
{
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t ਪਹਿਲਾ_ਸੈੱਟ;
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t ਦੂਜਾ_ਸੈੱਟ;
  ਬਦਲਣਯੋਗ hb_aat_class_cache_t class_cache;
};

ਬਣਤਰ kern_accelerator_ਅੰਕੜੇ_t
{
  hb_ਸਦਿਸ਼_t<kern_subtable_accelerator_ਅੰਕੜੇ_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ Kerx_ਸਾਰਣੀ
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ਅਚਲ T* thiz () ਅਚਲ { ਵਾਪਸ static_cast<ਅਚਲ T *> (ਇਹ); }

  ਤਾਰਕਿਕ has_state_machine () ਅਚਲ
  {
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ Sub_ਸਾਰਣੀ;

    ਅਚਲ Sub_ਸਾਰਣੀ *st = &thiz()->ਪਹਿਲਾ_Sub_ਸਾਰਣੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = thiz()->ਸਾਰਣੀ_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (st->get_type () == 1)
	ਵਾਪਸ ਸੱਚ;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<Sub_ਸਾਰਣੀ> (*st);
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ has_cross_ਧਾਰਾ () ਅਚਲ
  {
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ Sub_ਸਾਰਣੀ;

    ਅਚਲ Sub_ਸਾਰਣੀ *st = &thiz()->ਪਹਿਲਾ_Sub_ਸਾਰਣੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = thiz()->ਸਾਰਣੀ_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (st->u.ਸਿਰਲੇਖ.coverage & st->u.ਸਿਰਲੇਖ.Cross_ਧਾਰਾ)
	ਵਾਪਸ ਸੱਚ;
      st = &StructAfter<Sub_ਸਾਰਣੀ> (*st);
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਪੂਰਨ get_h_kerning (hb_codepoint_t ਖੱਬੇ, hb_codepoint_t ਸੱਜੇ) ਅਚਲ
  {
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ Sub_ਸਾਰਣੀ;

    ਪੂਰਨ v = 0;
    ਅਚਲ Sub_ਸਾਰਣੀ *st = &thiz()->ਪਹਿਲਾ_Sub_ਸਾਰਣੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = thiz()->ਸਾਰਣੀ_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ ((st->u.ਸਿਰਲੇਖ.coverage & (st->u.ਸਿਰਲੇਖ.Variation | st->u.ਸਿਰਲੇਖ.Cross_ਧਾਰਾ)) ||
	  !st->u.ਸਿਰਲੇਖ.is_horizontal ())
	ਜਾਰੀ;
      v += st->get_kerning (ਖੱਬੇ, ਸੱਜੇ);
      st = &StructAfter<Sub_ਸਾਰਣੀ> (*st);
    }
    ਵਾਪਸ v;
  }

  ਤਾਰਕਿਕ apply (AAT::hb_aat_apply_context_t *c,
	      ਅਚਲ kern_accelerator_ਅੰਕੜੇ_t &accel_ਅੰਕੜੇ) ਅਚਲ
  {
    c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat ();

    c->ਤਿਆਰੀ_ਬਫਰ_glyph_ਸੈੱਟ ();

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ Sub_ਸਾਰਣੀ;

    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ਤਾਰਕਿਕ ਵੇਖਿਆ_Cross_ਧਾਰਾ = ਝੂਠ;
    c->ਸੈੱਟ_lookup_ਸੂਚਕਾਂਕ (0);
    ਅਚਲ Sub_ਸਾਰਣੀ *st = &thiz()->ਪਹਿਲਾ_Sub_ਸਾਰਣੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = thiz()->ਸਾਰਣੀ_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਤਾਰਕਿਕ reverse;

      ਸਵੈਚਾਲਿਤ &subtable_accel = accel_ਅੰਕੜੇ.subtable_accels[i];

      ਜੇ (!T::Types::extended && (st->u.ਸਿਰਲੇਖ.coverage & st->u.ਸਿਰਲੇਖ.Variation))
	ਜਾਓ skip;

      ਜੇ (HB_DIRECTION_IS_HORIZONTAL (c->ਬਫਰ->props.direction) != st->u.ਸਿਰਲੇਖ.is_horizontal ())
	ਜਾਓ skip;

      c->ਪਹਿਲਾ_ਸੈੱਟ = &subtable_accel.ਪਹਿਲਾ_ਸੈੱਟ;
      c->ਦੂਜਾ_ਸੈੱਟ = &subtable_accel.ਦੂਜਾ_ਸੈੱਟ;
      c->machine_class_cache = &subtable_accel.class_cache;

      ਜੇ (!c->ਬਫਰ_intersects_machine ())
      {
	(ਖਾਲੀ) c->ਬਫਰ->ਸੁਨੇਹਾ (c->font, "skipped subtable %u because no glyph matches", c->lookup_ਸੂਚਕਾਂਕ);
	ਜਾਓ skip;
      }

      reverse = ਤਾਰਕਿਕ (st->u.ਸਿਰਲੇਖ.coverage & st->u.ਸਿਰਲੇਖ.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->ਬਫਰ->props.direction);

      ਜੇ (!c->ਬਫਰ->ਸੁਨੇਹਾ (c->font, "start subtable %u", c->lookup_ਸੂਚਕਾਂਕ))
	ਜਾਓ skip;

      ਜੇ (!ਵੇਖਿਆ_Cross_ਧਾਰਾ &&
	  (st->u.ਸਿਰਲੇਖ.coverage & st->u.ਸਿਰਲੇਖ.Cross_ਧਾਰਾ))
      {
	/* Attach all glyphs into a chain. */
	ਵੇਖਿਆ_Cross_ਧਾਰਾ = ਸੱਚ;
	hb_glyph_position_t *pos = c->ਬਫਰ->pos;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = c->ਬਫਰ->len;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->ਬਫਰ->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      ਜੇ (reverse != c->ਬਫਰ_is_reversed)
        c->ਉਲਟ_ਬਫਰ ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < ਗਿਣਤੀ_2 - 1 ? st : (ਅਚਲ Sub_ਸਾਰਣੀ *) ਖਾਲੀ_ਸੂਚਕ);
	ret |= st->dispatch (c);
      }

      (ਖਾਲੀ) c->ਬਫਰ->ਸੁਨੇਹਾ (c->font, "end subtable %u", c->lookup_ਸੂਚਕਾਂਕ);

    skip:
      st = &StructAfter<Sub_ਸਾਰਣੀ> (*st);
      c->ਸੈੱਟ_lookup_ਸੂਚਕਾਂਕ (c->lookup_ਸੂਚਕਾਂਕ + 1);
    }
    ਜੇ (c->ਬਫਰ_is_reversed)
      c->ਉਲਟ_ਬਫਰ ();

    ਵਾਪਸ ret;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!(thiz()->ਸੰਸਕਰਣ.sanitize (c) &&
		    hb_barrier () &&
		    (ਬਿਨਾਨਿਸ਼ਾਨੀ) thiz()->ਸੰਸਕਰਣ >= (ਬਿਨਾਨਿਸ਼ਾਨੀ) T::min_ਸੰਸਕਰਣ &&
		    thiz()->ਸਾਰਣੀ_ਗਿਣਤੀ.sanitize (c))))
      return_trace (ਝੂਠ);

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ Sub_ਸਾਰਣੀ;

    ਅਚਲ Sub_ਸਾਰਣੀ *st = &thiz()->ਪਹਿਲਾ_Sub_ਸਾਰਣੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = thiz()->ਸਾਰਣੀ_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (unlikely (!st->u.ਸਿਰਲੇਖ.sanitize (c)))
	return_trace (ਝੂਠ);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < ਗਿਣਤੀ_2 - 1 ? st : (ਅਚਲ Sub_ਸਾਰਣੀ *) ਖਾਲੀ_ਸੂਚਕ);

      ਜੇ (unlikely (!st->sanitize (c)))
	return_trace (ਝੂਠ);

      st = &StructAfter<Sub_ਸਾਰਣੀ> (*st);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ major_ਸੰਸਕਰਣ = thiz()->ਸੰਸਕਰਣ;
    ਜੇ (ਆਕਾਰ (thiz()->ਸੰਸਕਰਣ) == 4)
      major_ਸੰਸਕਰਣ = major_ਸੰਸਕਰਣ >> 16;
    ਜੇ (major_ਸੰਸਕਰਣ >= 3)
    {
      ਅਚਲ SubtableGlyphCoverage *coverage = (ਅਚਲ SubtableGlyphCoverage *) st;
      ਜੇ (!coverage->sanitize (c, ਗਿਣਤੀ_2))
        return_trace (ਝੂਠ);
    }

    return_trace (ਸੱਚ);
  }

  kern_accelerator_ਅੰਕੜੇ_t create_accelerator_ਅੰਕੜੇ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    kern_accelerator_ਅੰਕੜੇ_t accel_ਅੰਕੜੇ;

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ T::Sub_ਸਾਰਣੀ Sub_ਸਾਰਣੀ;

    ਅਚਲ Sub_ਸਾਰਣੀ *st = &thiz()->ਪਹਿਲਾ_Sub_ਸਾਰਣੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = thiz()->ਸਾਰਣੀ_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਸਵੈਚਾਲਿਤ &subtable_accel = *accel_ਅੰਕੜੇ.subtable_accels.push ();
      ਜੇ (unlikely (accel_ਅੰਕੜੇ.subtable_accels.ਵਿੱਚ_error ()))
	  ਵਾਪਸ accel_ਅੰਕੜੇ;

      st->collect_glyphs (subtable_accel.ਪਹਿਲਾ_ਸੈੱਟ, subtable_accel.ਦੂਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<Sub_ਸਾਰਣੀ> (*st);
    }

    ਵਾਪਸ accel_ਅੰਕੜੇ;
  }

  ਬਣਤਰ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ਇਹ->ਸਾਰਣੀ = sc.reference_ਸਾਰਣੀ<T> (face);
      ਇਹ->accel_ਅੰਕੜੇ = ਇਹ->ਸਾਰਣੀ->create_accelerator_ਅੰਕੜੇ (face->get_ਅੰਕ_glyphs ());
    }
    ~accelerator_t ()
    {
      ਇਹ->ਸਾਰਣੀ.destroy ();
    }

    hb_blob_t *get_blob () ਅਚਲ { ਵਾਪਸ ਸਾਰਣੀ.get_blob (); }

    ਤਾਰਕਿਕ apply (AAT::hb_aat_apply_context_t *c) ਅਚਲ
    {
      ਵਾਪਸ ਸਾਰਣੀ->apply (c, accel_ਅੰਕੜੇ);
    }

    hb_blob_ptr_t<T> ਸਾਰਣੀ;
    kern_accelerator_ਅੰਕੜੇ_t accel_ਅੰਕੜੇ;
    hb_aat_scratch_t scratch;
  };
};

ਬਣਤਰ kerx : Kerx_ਸਾਰਣੀ<kerx>
{
  ਮਿੱਤਰ ਬਣਤਰ Kerx_ਸਾਰਣੀ<kerx>;

  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_AAT_ਟੈਗ_kerx;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਸੰਸਕਰਣ = 2u;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Kerx_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::Types Types;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Kerx_Sub_ਸਾਰਣੀ Sub_ਸਾਰਣੀ;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ; }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸੰਸਕਰਣ;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	ਅਣਵਰਤਿਆ_2;		/* Set to 0. */
  HBUINT32	ਸਾਰਣੀ_ਗਿਣਤੀ;	/* The number of subtables included in the extended kerning
				 * table. */
  Sub_ਸਾਰਣੀ	ਪਹਿਲਾ_Sub_ਸਾਰਣੀ;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8);
};

ਬਣਤਰ kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#ਜੇ_ਅੰਤ /* HB_AAT_LAYOUT_KERX_TABLE_HH */
