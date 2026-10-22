/*
 * Copyright © 2018 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CFF2_INTERP_CS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CFF2_INTERP_CS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff-interp-cs-common.hh"

ਨਾਮਾਕਾਸ਼ CFF {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;

ਬਣਤਰ blend_arg_t : ਸੰਖਿਆ_t
{
  ਖਾਲੀ ਸੈੱਟ_int (ਪੂਰਨ v) { reset_blends (); ਸੰਖਿਆ_t::ਸੈੱਟ_int (v); }
  ਖਾਲੀ ਸੈੱਟ_fixed (int32_t v) { reset_blends (); ਸੰਖਿਆ_t::ਸੈੱਟ_fixed (v); }
  ਖਾਲੀ ਸੈੱਟ_real (ਦੁੱਗਣਾ v) { reset_blends (); ਸੰਖਿਆ_t::ਸੈੱਟ_real (v); }

  ਖਾਲੀ ਸੈੱਟ_blends (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ਮੁੱਲਾਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ_ਸੂਚਕਾਂਕ,
		   hb_array_t<ਅਚਲ blend_arg_t> blends_)
  {
    ਅੰਕ_ਮੁੱਲਾਂ_2 = ਅੰਕ_ਮੁੱਲਾਂ;
    ਮੁੱਲ_ਸੂਚਕਾਂਕ_2 = ਮੁੱਲ_ਸੂਚਕਾਂਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_Blends = blends_.ਲੰਮਾਈ;
    ਜੇ (unlikely (!deltas.resize_exact (ਅੰਕ_Blends)))
      ਵਾਪਸ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  ਤਾਰਕਿਕ blending () ਅਚਲ { ਵਾਪਸ deltas.ਲੰਮਾਈ > 0; }
  ਖਾਲੀ reset_blends ()
  {
    ਅੰਕ_ਮੁੱਲਾਂ_2 = ਮੁੱਲ_ਸੂਚਕਾਂਕ_2 = 0;
    deltas.clear ();
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ਮੁੱਲਾਂ_2;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ_ਸੂਚਕਾਂਕ_2;
  hb_ਸਦਿਸ਼_t<ਸੰਖਿਆ_t> deltas;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ELEM>
ਬਣਤਰ cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ACC>
  cff2_cs_interp_env_t (ਅਚਲ hb_ubytes_t &str, ACC &acc, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd,
			ਅਚਲ ਪੂਰਨ *coords_=ਖਾਲੀ_ਸੂਚਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.ਨਿੱਜੀ_Dicts[fd].localSubrs),
      region_ਗਿਣਤੀ_2 (0), cached_scalars_ਸਦਿਸ਼ (&acc.cached_scalars_ਸਦਿਸ਼)
  {
    coords = coords_;
    ਅੰਕ_coords = ਅੰਕ_coords_2;
    varStore = acc.varStore;
    do_blend = ਅੰਕ_coords && varStore->ਮਾਪ;
    orig_ivs = acc.ਨਿੱਜੀ_Dicts[fd].ivs;
    ਸੈੱਟ_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_ਸਦਿਸ਼ (scalars);
  }

  hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> *acquire_scalars_ਸਦਿਸ਼ () ਅਚਲ
  {
    hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> *scalars = cached_scalars_ਸਦਿਸ਼->get_acquire ();

    ਜੇ (!scalars || !cached_scalars_ਸਦਿਸ਼->cmpexch (scalars, ਖਾਲੀ_ਸੂਚਕ))
    {
      scalars = (hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> *) hb_calloc (1, ਆਕਾਰ (hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ>));
      ਜੇ (unlikely (!scalars))
	ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
      scalars->init ();
    }

    ਵਾਪਸ scalars;
  }

  ਖਾਲੀ release_scalars_ਸਦਿਸ਼ (hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> *scalars) ਅਚਲ
  {
    ਜੇ (!scalars)
      ਵਾਪਸ;

    scalars->clear ();

    ਜੇ (!cached_scalars_ਸਦਿਸ਼->cmpexch (ਖਾਲੀ_ਸੂਚਕ, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = ਖਾਲੀ_ਸੂਚਕ;
  }

  op_ਕੋਡ_t fetch_op ()
  {
    ਜੇ (ਇਹ->str_ref.avail ())
      ਵਾਪਸ SUPER::fetch_op ();

    /* make up return or endchar op */
    ਜੇ (ਇਹ->call_ਢੇਰ.is_empty ())
      ਵਾਪਸ Op_ਕੋਡ_endchar;
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ Op_ਕੋਡ_return;
  }

  ਅਚਲ ELEM& eval_arg (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
  {
    ਵਾਪਸ SUPER::arg_ਢੇਰ[i];
  }

  ਅਚਲ ELEM& ਕੱਢੋ_arg ()
  {
    ਵਾਪਸ SUPER::arg_ਢੇਰ.ਕੱਢੋ ();
  }

  ਖਾਲੀ process_blend ()
  {
    ਜੇ (!ਵੇਖਿਆ_blend)
    {
      scalars = acquire_scalars_ਸਦਿਸ਼ ();
      ਜੇ (unlikely (!scalars))
	SUPER::ਸੈੱਟ_error ();
      ਨਹੀਂਤਾਂ
      {
	region_ਗਿਣਤੀ_2 = varStore->varStore.get_region_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ (get_ivs ());
	ਜੇ (do_blend)
	{
	  ਜੇ (unlikely (!scalars->resize_exact (region_ਗਿਣਤੀ_2)))
	    SUPER::ਸੈੱਟ_error ();
	  ਨਹੀਂਤਾਂ
	    varStore->varStore.get_region_scalars (get_ivs (), coords, ਅੰਕ_coords,
						   &(*scalars)[0], region_ਗਿਣਤੀ_2);
	}
      }
      ਵੇਖਿਆ_blend = ਸੱਚ;
    }
  }

  ਖਾਲੀ process_vsindex ()
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2 = SUPER::arg_ਢੇਰ.ਕੱਢੋ_uint ();
    ਜੇ (unlikely (ਵੇਖਿਆ_vsindex () || ਵੇਖਿਆ_blend))
    {
     SUPER::ਸੈੱਟ_error ();
    }
    ਨਹੀਂਤਾਂ
    {
      ਸੈੱਟ_ivs (ਸੂਚਕਾਂਕ_2);
    }
    ਵੇਖਿਆ_vsindex_2 = ਸੱਚ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_region_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ region_ਗਿਣਤੀ_2; }
  ਖਾਲੀ	 ਸੈੱਟ_region_ਗਿਣਤੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ region_ਗਿਣਤੀ_3) { region_ਗਿਣਤੀ_2 = region_ਗਿਣਤੀ_3; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ivs () ਅਚਲ { ਵਾਪਸ ivs; }
  ਖਾਲੀ	 ਸੈੱਟ_ivs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_orig_ivs () ਅਚਲ { ਵਾਪਸ orig_ivs; }
  ਤਾਰਕਿਕ	 ਵੇਖਿਆ_vsindex () ਅਚਲ { ਵਾਪਸ ਵੇਖਿਆ_vsindex_2; }

  ਦੁੱਗਣਾ blend_deltas (hb_array_t<ਅਚਲ ELEM> deltas) ਅਚਲ
  {
    ਦੁੱਗਣਾ v = 0;
    ਜੇ (do_blend)
    {
      ਜੇ (likely (scalars && scalars->ਲੰਮਾਈ == deltas.ਲੰਮਾਈ))
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = scalars->ਲੰਮਾਈ;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
	  v += (ਦੁੱਗਣਾ) scalars->arrayZ[i] * deltas.arrayZ[i].ਮੰਜ਼ਿਲ_real ();
      }
    }
    ਵਾਪਸ v;
  }

  ਤਾਰਕਿਕ have_coords () ਅਚਲ { ਵਾਪਸ ਅੰਕ_coords; }

  ਸੁਰੱਖਿਅਤ:
  ਅਚਲ ਪੂਰਨ     *coords;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਅੰਕ_coords;
  ਅਚਲ	 CFF2_ਵਸਤੂ_Variation_Store *varStore;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  region_ਗਿਣਤੀ_2;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ivs;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  orig_ivs;
  hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ>  *scalars = ਖਾਲੀ_ਸੂਚਕ;
  hb_atomic_t<hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> *> *cached_scalars_ਸਦਿਸ਼ = ਖਾਲੀ_ਸੂਚਕ;
  ਤਾਰਕਿਕ	  do_blend;
  ਤਾਰਕਿਕ	  ਵੇਖਿਆ_vsindex_2 = ਝੂਠ;
  ਤਾਰਕਿਕ	  ਵੇਖਿਆ_blend = ਝੂਠ;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ OPSET, ਕਿਸਮ_ਨਾਂ PARAM, ਕਿਸਮ_ਨਾਂ ELEM, ਕਿਸਮ_ਨਾਂ ਰਸਤਾ_3=ਰਸਤਾ_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
ਬਣਤਰ cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, ਰਸਤਾ_3>
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_callsubr:
      ਹਾਲਤ Op_ਕੋਡ_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#ਜੇ 0
	ਜੇ (unlikely (env.arg_ਢੇਰ.peek ().blending ()))
	{
	  env.ਸੈੱਟ_error ();
	  ਰੋਕੋ;
	}
#ਜੇ_ਅੰਤ
	SUPER::process_op (op, env, param);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_blendcs:
	OPSET::process_blend (env, param);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_vsindexcs:
#ਜੇ 0
	ਜੇ (unlikely (env.arg_ਢੇਰ.peek ().blending ()))
	{
	  env.ਸੈੱਟ_error ();
	  ਰੋਕੋ;
	}
#ਜੇ_ਅੰਤ
	OPSET::process_vsindex (env, param);
	ਰੋਕੋ;

      ਮੂਲ:
	SUPER::process_op (op, env, param);
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  ਸਥਿਰ ਖਾਲੀ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ਅਚਲ hb_array_t<ਅਚਲ ELEM> blends,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ n, ਬਿਨਾਨਿਸ਼ਾਨੀ i)
  {
    ਜੇ (env.have_coords ())
      arg.ਸੈੱਟ_int (round (arg.ਮੰਜ਼ਿਲ_real () + env.blend_deltas (blends)));
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_ਸਦਿਸ਼_t<ਸੰਖਿਆ_t> inner = std::move (arg.deltas);
      arg.ਸੈੱਟ_blends (n, i, blends);
      ਜੇ (likely (inner.ਲੰਮਾਈ == arg.deltas.ਲੰਮਾਈ))
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < inner.ਲੰਮਾਈ; j++)
	  arg.deltas.arrayZ[j].ਸੈੱਟ_real (arg.deltas.arrayZ[j].ਮੰਜ਼ਿਲ_real () +
					 inner.arrayZ[j].ਮੰਜ਼ਿਲ_real ());
      ਨਹੀਂਤਾਂ
	env.ਸੈੱਟ_error ();
    }
    ਨਹੀਂਤਾਂ
      arg.ਸੈੱਟ_blends (n, i, blends);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  ਸਥਿਰ ਖਾਲੀ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ਅਚਲ hb_array_t<ਅਚਲ ELEM> blends,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ n, ਬਿਨਾਨਿਸ਼ਾਨੀ i)
  {
    arg.ਸੈੱਟ_real (arg.ਮੰਜ਼ਿਲ_real () + env.blend_deltas (blends));
  }

  ਸਥਿਰ ਖਾਲੀ process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n, k;

    env.process_blend ();
    k = env.get_region_ਗਿਣਤੀ ();
    n = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
    /* copy the blend values into blend array of the default values */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = env.arg_ਢੇਰ.get_ਗਿਣਤੀ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕੁੱਲ;
    ਜੇ (unlikely (hb_unsigned_mul_overflows (k + 1, n, &ਕੁੱਲ) || ਕੁੱਲ > ਗਿਣਤੀ_2))
    {
      env.ਸੈੱਟ_error ();
      ਵਾਪਸ;
    }
    ਜੇ (unlikely (!env.spend_budget (HB_BUDGET_1, ਕੁੱਲ - n)))
    {
      env.ਸੈੱਟ_error ();
      ਵਾਪਸ;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = ਗਿਣਤੀ_2 - ਕੁੱਲ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < n; i++)
    {
      ਅਚਲ hb_array_t<ਅਚਲ ELEM> blends = env.arg_ਢੇਰ.sub_array (ਸ਼ੁਰੂ + n + (i * k), k);
      process_arg_blend (env, env.arg_ਢੇਰ.arrayZ[ਸ਼ੁਰੂ + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ਢੇਰ.ਕੱਢੋ (k * n);
  }

  ਸਥਿਰ ਖਾਲੀ process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_ਦਲੀਲਾਂ ();
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, ਰਸਤਾ_3>  SUPER;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ OPSET, ਕਿਸਮ_ਨਾਂ PARAM, ਕਿਸਮ_ਨਾਂ ELEM>
ਵਰਤੋਂ cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#ਜੇ_ਅੰਤ /* HB_CFF2_INTERP_CS_HH */
