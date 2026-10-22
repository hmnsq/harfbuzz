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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-cff-interp-cs-common.hh"

ನಾಮಾಕಾಶ CFF {

using ನಾಮಾಕಾಶ OT;

ಸಂರಚನಾ blend_arg_t : ಸಂಖ್ಯೆ_t
{
  ಶೂನ್ಯ set_int (ಪೂರ್ಣಾಙ್ಕ v) { reset_blends (); ಸಂಖ್ಯೆ_t::set_int (v); }
  ಶೂನ್ಯ set_fixed (int32_t v) { reset_blends (); ಸಂಖ್ಯೆ_t::set_fixed (v); }
  ಶೂನ್ಯ set_real (ದ್ವಿಗುಣ v) { reset_blends (); ಸಂಖ್ಯೆ_t::set_real (v); }

  ಶೂನ್ಯ set_blends (unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_ಮೌಲ್ಯಗಳು, unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ_Index,
		   hb_array_t<ಧ್ರುವ blend_arg_t> blends_)
  {
    ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2 = ಅಂಕೆ_ಮೌಲ್ಯಗಳು;
    ಮೌಲ್ಯ_Index_2 = ಮೌಲ್ಯ_Index;
    unsigned ಅಂಕೆ_Blends = blends_.length;
    ಯದಿ (unlikely (!deltas.resize_exact (ಅಂಕೆ_Blends)))
      ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  ತಾರ್ಕಿಕ blending () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ deltas.length > 0; }
  ಶೂನ್ಯ reset_blends ()
  {
    ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2 = ಮೌಲ್ಯ_Index_2 = 0;
    deltas.clear ();
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ_Index_2;
  hb_vector_t<ಸಂಖ್ಯೆ_t> deltas;
};

ಪ್ರಕಾರನಿರ್ದೇಶ biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

template <typename ELEM>
ಸಂರಚನಾ cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  template <typename ACC>
  cff2_cs_interp_env_t (ಧ್ರುವ hb_ubytes_t &str, ACC &acc, unsigned ಪೂರ್ಣಾಙ್ಕ fd,
			ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords_=nullptr, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_ಎಣಿಕೆ_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    ಅಂಕೆ_coords = ಅಂಕೆ_coords_2;
    varStore = acc.varStore;
    do_blend = ಅಂಕೆ_coords && varStore->size;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<ಭಿನ್ನ> *acquire_scalars_vector () ಧ್ರುವ
  {
    hb_vector_t<ಭಿನ್ನ> *scalars = cached_scalars_vector->get_acquire ();

    ಯದಿ (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<ಭಿನ್ನ> *) hb_calloc (1, ಪರಿಮಾಣ (hb_vector_t<ಭಿನ್ನ>));
      ಯದಿ (unlikely (!scalars))
	ಪ್ರತ್ಯಾಗಮ nullptr;
      scalars->init ();
    }

    ಪ್ರತ್ಯಾಗಮ scalars;
  }

  ಶೂನ್ಯ release_scalars_vector (hb_vector_t<ಭಿನ್ನ> *scalars) ಧ್ರುವ
  {
    ಯದಿ (!scalars)
      ಪ್ರತ್ಯಾಗಮ;

    scalars->clear ();

    ಯದಿ (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    ಯದಿ (ಏತತ್->str_ref.avail ())
      ಪ್ರತ್ಯಾಗಮ SUPER::fetch_op ();

    /* make up return or endchar op */
    ಯದಿ (ಏತತ್->call_ರಾಶಿ.is_empty ())
      ಪ್ರತ್ಯಾಗಮ OpCode_endchar;
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ OpCode_return;
  }

  ಧ್ರುವ ELEM& eval_arg (unsigned ಪೂರ್ಣಾಙ್ಕ i)
  {
    ಪ್ರತ್ಯಾಗಮ SUPER::arg_ರಾಶಿ[i];
  }

  ಧ್ರುವ ELEM& ತೆಗೆ_arg ()
  {
    ಪ್ರತ್ಯಾಗಮ SUPER::arg_ರಾಶಿ.ತೆಗೆ ();
  }

  ಶೂನ್ಯ process_blend ()
  {
    ಯದಿ (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      ಯದಿ (unlikely (!scalars))
	SUPER::set_error ();
      ಅನ್ಯಥಾ
      {
	region_ಎಣಿಕೆ_2 = varStore->varStore.get_region_index_ಎಣಿಕೆ (get_ivs ());
	ಯದಿ (do_blend)
	{
	  ಯದಿ (unlikely (!scalars->resize_exact (region_ಎಣಿಕೆ_2)))
	    SUPER::set_error ();
	  ಅನ್ಯಥಾ
	    varStore->varStore.get_region_scalars (get_ivs (), coords, ಅಂಕೆ_coords,
						   &(*scalars)[0], region_ಎಣಿಕೆ_2);
	}
      }
      seen_blend = ಸತ್ಯ;
    }
  }

  ಶೂನ್ಯ process_vsindex ()
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ  index = SUPER::arg_ರಾಶಿ.ತೆಗೆ_uint ();
    ಯದಿ (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    ಅನ್ಯಥಾ
    {
      set_ivs (index);
    }
    seen_vsindex_ = ಸತ್ಯ;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_region_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ region_ಎಣಿಕೆ_2; }
  ಶೂನ್ಯ	 set_region_ಎಣಿಕೆ (unsigned ಪೂರ್ಣಾಙ್ಕ region_ಎಣಿಕೆ_3) { region_ಎಣಿಕೆ_2 = region_ಎಣಿಕೆ_3; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_ivs () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ivs; }
  ಶೂನ್ಯ	 set_ivs (unsigned ಪೂರ್ಣಾಙ್ಕ ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  unsigned ಪೂರ್ಣಾಙ್ಕ get_orig_ivs () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ orig_ivs; }
  ತಾರ್ಕಿಕ	 seen_vsindex () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ seen_vsindex_; }

  ದ್ವಿಗುಣ blend_deltas (hb_array_t<ಧ್ರುವ ELEM> deltas) ಧ್ರುವ
  {
    ದ್ವಿಗುಣ v = 0;
    ಯದಿ (do_blend)
    {
      ಯದಿ (likely (scalars && scalars->length == deltas.length))
      {
        unsigned ಎಣಿಕೆ = scalars->length;
	ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
	  v += (ದ್ವಿಗುಣ) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    ಪ್ರತ್ಯಾಗಮ v;
  }

  ತಾರ್ಕಿಕ have_coords () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_coords; }

  ರಕ್ಷಿತ:
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ     *coords;
  unsigned ಪೂರ್ಣಾಙ್ಕ  ಅಂಕೆ_coords;
  ಧ್ರುವ	 CFF2ItemVariationStore *varStore;
  unsigned ಪೂರ್ಣಾಙ್ಕ  region_ಎಣಿಕೆ_2;
  unsigned ಪೂರ್ಣಾಙ್ಕ  ivs;
  unsigned ಪೂರ್ಣಾಙ್ಕ  orig_ivs;
  hb_vector_t<ಭಿನ್ನ>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<ಭಿನ್ನ> *> *cached_scalars_vector = nullptr;
  ತಾರ್ಕಿಕ	  do_blend;
  ತಾರ್ಕಿಕ	  seen_vsindex_ = ಅಸತ್ಯ;
  ತಾರ್ಕಿಕ	  seen_blend = ಅಸತ್ಯ;

  ಪ್ರಕಾರನಿರ್ದೇಶ cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
template <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
ಸಂರಚನಾ cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ OpCode_callsubr:
      ಪಕ್ಷ OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#ಯದಿ 0
	ಯದಿ (unlikely (env.arg_ರಾಶಿ.peek ().blending ()))
	{
	  env.set_error ();
	  ಭಙ್ಗ;
	}
#endif
	SUPER::process_op (op, env, param);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_blendcs:
	OPSET::process_blend (env, param);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_vsindexcs:
#ಯದಿ 0
	ಯದಿ (unlikely (env.arg_ರಾಶಿ.peek ().blending ()))
	{
	  env.set_error ();
	  ಭಙ್ಗ;
	}
#endif
	OPSET::process_vsindex (env, param);
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	SUPER::process_op (op, env, param);
    }
  }

  template <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  ಸ್ಥಿರ ಶೂನ್ಯ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ಧ್ರುವ hb_array_t<ಧ್ರುವ ELEM> blends,
				 unsigned n, unsigned i)
  {
    ಯದಿ (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    ಅನ್ಯಥಾ ಯದಿ (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<ಸಂಖ್ಯೆ_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      ಯದಿ (likely (inner.length == arg.deltas.length))
	ಪ್ರತಿ (unsigned j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      ಅನ್ಯಥಾ
	env.set_error ();
    }
    ಅನ್ಯಥಾ
      arg.set_blends (n, i, blends);
  }
  template <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  ಸ್ಥಿರ ಶೂನ್ಯ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ಧ್ರುವ hb_array_t<ಧ್ರುವ ELEM> blends,
				 unsigned n, unsigned i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  ಸ್ಥಿರ ಶೂನ್ಯ process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ n, k;

    env.process_blend ();
    k = env.get_region_ಎಣಿಕೆ ();
    n = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
    /* copy the blend values into blend array of the default values */
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ();
    unsigned ಪೂರ್ಣಾಙ್ಕ total;
    ಯದಿ (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > ಎಣಿಕೆ))
    {
      env.set_error ();
      ಪ್ರತ್ಯಾಗಮ;
    }
    ಯದಿ (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      ಪ್ರತ್ಯಾಗಮ;
    }
    unsigned ಪೂರ್ಣಾಙ್ಕ start = ಎಣಿಕೆ - total;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
    {
      ಧ್ರುವ hb_array_t<ಧ್ರುವ ELEM> blends = env.arg_ರಾಶಿ.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_ರಾಶಿ.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ರಾಶಿ.ತೆಗೆ (k * n);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
