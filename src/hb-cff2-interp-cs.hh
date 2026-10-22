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
#ifndef HB_CFF2_INTERP_CS_HH
#define HB_CFF2_INTERP_CS_HH

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-cff-interp-cs-common.hh"

નામાકાશ CFF {

using નામાકાશ OT;

સંરચના blend_arg_t : સંખ્યા_t
{
  શૂન્ય set_int (પૂર્ણાંક v) { reset_blends (); સંખ્યા_t::set_int (v); }
  શૂન્ય set_fixed (int32_t v) { reset_blends (); સંખ્યા_t::set_fixed (v); }
  શૂન્ય set_real (દ્વિગુણ v) { reset_blends (); સંખ્યા_t::set_real (v); }

  શૂન્ય set_blends (unsigned પૂર્ણાંક અંક_કિંમતો, unsigned પૂર્ણાંક કિંમત_Index,
		   hb_array_t<અચલ blend_arg_t> blends_)
  {
    અંક_કિંમતો_2 = અંક_કિંમતો;
    કિંમત_Index_2 = કિંમત_Index;
    unsigned અંક_Blends = blends_.length;
    જો (unlikely (!deltas.resize_exact (અંક_Blends)))
      પાછા;
    માટે (unsigned પૂર્ણાંક i = 0; i < અંક_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  તાર્કિક blending () અચલ { પાછા deltas.length > 0; }
  શૂન્ય reset_blends ()
  {
    અંક_કિંમતો_2 = કિંમત_Index_2 = 0;
    deltas.clear ();
  }

  unsigned પૂર્ણાંક અંક_કિંમતો_2;
  unsigned પૂર્ણાંક કિંમત_Index_2;
  hb_vector_t<સંખ્યા_t> deltas;
};

પ્રકારવ્યાખ્યા biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

template <typename ELEM>
સંરચના cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  template <typename ACC>
  cff2_cs_interp_env_t (અચલ hb_ubytes_t &str, ACC &acc, unsigned પૂર્ણાંક fd,
			અચલ પૂર્ણાંક *coords_=nullptr, unsigned પૂર્ણાંક અંક_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_ગણતરી_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    અંક_coords = અંક_coords_2;
    varStore = acc.varStore;
    do_blend = અંક_coords && varStore->size;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<દશાંશ> *acquire_scalars_vector () અચલ
  {
    hb_vector_t<દશાંશ> *scalars = cached_scalars_vector->get_acquire ();

    જો (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<દશાંશ> *) hb_calloc (1, કદ (hb_vector_t<દશાંશ>));
      જો (unlikely (!scalars))
	પાછા nullptr;
      scalars->init ();
    }

    પાછા scalars;
  }

  શૂન્ય release_scalars_vector (hb_vector_t<દશાંશ> *scalars) અચલ
  {
    જો (!scalars)
      પાછા;

    scalars->clear ();

    જો (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    જો (આ->str_ref.avail ())
      પાછા SUPER::fetch_op ();

    /* make up return or endchar op */
    જો (આ->call_ઢગલો.is_empty ())
      પાછા OpCode_endchar;
    નહીંતર
      પાછા OpCode_return;
  }

  અચલ ELEM& eval_arg (unsigned પૂર્ણાંક i)
  {
    પાછા SUPER::arg_ઢગલો[i];
  }

  અચલ ELEM& કાઢો_arg ()
  {
    પાછા SUPER::arg_ઢગલો.કાઢો ();
  }

  શૂન્ય process_blend ()
  {
    જો (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      જો (unlikely (!scalars))
	SUPER::set_error ();
      નહીંતર
      {
	region_ગણતરી_2 = varStore->varStore.get_region_index_ગણતરી (get_ivs ());
	જો (do_blend)
	{
	  જો (unlikely (!scalars->resize_exact (region_ગણતરી_2)))
	    SUPER::set_error ();
	  નહીંતર
	    varStore->varStore.get_region_scalars (get_ivs (), coords, અંક_coords,
						   &(*scalars)[0], region_ગણતરી_2);
	}
      }
      seen_blend = સાચું;
    }
  }

  શૂન્ય process_vsindex ()
  {
    unsigned પૂર્ણાંક  index = SUPER::arg_ઢગલો.કાઢો_uint ();
    જો (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    નહીંતર
    {
      set_ivs (index);
    }
    seen_vsindex_ = સાચું;
  }

  unsigned પૂર્ણાંક get_region_ગણતરી () અચલ { પાછા region_ગણતરી_2; }
  શૂન્ય	 set_region_ગણતરી (unsigned પૂર્ણાંક region_ગણતરી_3) { region_ગણતરી_2 = region_ગણતરી_3; }
  unsigned પૂર્ણાંક get_ivs () અચલ { પાછા ivs; }
  શૂન્ય	 set_ivs (unsigned પૂર્ણાંક ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  unsigned પૂર્ણાંક get_orig_ivs () અચલ { પાછા orig_ivs; }
  તાર્કિક	 seen_vsindex () અચલ { પાછા seen_vsindex_; }

  દ્વિગુણ blend_deltas (hb_array_t<અચલ ELEM> deltas) અચલ
  {
    દ્વિગુણ v = 0;
    જો (do_blend)
    {
      જો (likely (scalars && scalars->length == deltas.length))
      {
        unsigned ગણતરી = scalars->length;
	માટે (unsigned i = 0; i < ગણતરી; i++)
	  v += (દ્વિગુણ) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    પાછા v;
  }

  તાર્કિક have_coords () અચલ { પાછા અંક_coords; }

  સુરક્ષિત:
  અચલ પૂર્ણાંક     *coords;
  unsigned પૂર્ણાંક  અંક_coords;
  અચલ	 CFF2ItemVariationStore *varStore;
  unsigned પૂર્ણાંક  region_ગણતરી_2;
  unsigned પૂર્ણાંક  ivs;
  unsigned પૂર્ણાંક  orig_ivs;
  hb_vector_t<દશાંશ>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<દશાંશ> *> *cached_scalars_vector = nullptr;
  તાર્કિક	  do_blend;
  તાર્કિક	  seen_vsindex_ = ખોટું;
  તાર્કિક	  seen_blend = ખોટું;

  પ્રકારવ્યાખ્યા cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
template <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
સંરચના cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  સ્થિર શૂન્ય process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    પસંદગી (op) {
      સ્થિતિ OpCode_callsubr:
      સ્થિતિ OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#જો 0
	જો (unlikely (env.arg_ઢગલો.peek ().blending ()))
	{
	  env.set_error ();
	  તોડો;
	}
#endif
	SUPER::process_op (op, env, param);
	તોડો;

      સ્થિતિ OpCode_blendcs:
	OPSET::process_blend (env, param);
	તોડો;

      સ્થિતિ OpCode_vsindexcs:
#જો 0
	જો (unlikely (env.arg_ઢગલો.peek ().blending ()))
	{
	  env.set_error ();
	  તોડો;
	}
#endif
	OPSET::process_vsindex (env, param);
	તોડો;

      મૂળભૂત:
	SUPER::process_op (op, env, param);
    }
  }

  template <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  સ્થિર શૂન્ય process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 અચલ hb_array_t<અચલ ELEM> blends,
				 unsigned n, unsigned i)
  {
    જો (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    નહીંતર જો (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<સંખ્યા_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      જો (likely (inner.length == arg.deltas.length))
	માટે (unsigned j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      નહીંતર
	env.set_error ();
    }
    નહીંતર
      arg.set_blends (n, i, blends);
  }
  template <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  સ્થિર શૂન્ય process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 અચલ hb_array_t<અચલ ELEM> blends,
				 unsigned n, unsigned i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  સ્થિર શૂન્ય process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    unsigned પૂર્ણાંક n, k;

    env.process_blend ();
    k = env.get_region_ગણતરી ();
    n = env.arg_ઢગલો.કાઢો_uint ();
    /* copy the blend values into blend array of the default values */
    unsigned પૂર્ણાંક ગણતરી = env.arg_ઢગલો.get_ગણતરી ();
    unsigned પૂર્ણાંક total;
    જો (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > ગણતરી))
    {
      env.set_error ();
      પાછા;
    }
    જો (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      પાછા;
    }
    unsigned પૂર્ણાંક start = ગણતરી - total;
    માટે (unsigned પૂર્ણાંક i = 0; i < n; i++)
    {
      અચલ hb_array_t<અચલ ELEM> blends = env.arg_ઢગલો.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_ઢગલો.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ઢગલો.કાઢો (k * n);
  }

  સ્થિર શૂન્ય process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
