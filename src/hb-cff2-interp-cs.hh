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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-cff-interp-cs-common.hh"

നാമാകാശ CFF {

using നാമാകാശ OT;

ഘടന blend_arg_t : സംഖ്യ_t
{
  ശൂന്യം set_int (പൂർണ്ണസംഖ്യ v) { reset_blends (); സംഖ്യ_t::set_int (v); }
  ശൂന്യം set_fixed (int32_t v) { reset_blends (); സംഖ്യ_t::set_fixed (v); }
  ശൂന്യം set_real (ഇരട്ട v) { reset_blends (); സംഖ്യ_t::set_real (v); }

  ശൂന്യം set_blends (unsigned പൂർണ്ണസംഖ്യ അക്കം_മൂല്യങ്ങൾ, unsigned പൂർണ്ണസംഖ്യ മൂല്യം_Index,
		   hb_array_t<സ്ഥിരം blend_arg_t> blends_)
  {
    അക്കം_മൂല്യങ്ങൾ_2 = അക്കം_മൂല്യങ്ങൾ;
    മൂല്യം_Index_2 = മൂല്യം_Index;
    unsigned അക്കം_Blends = blends_.length;
    എങ്കിൽ (unlikely (!deltas.resize_exact (അക്കം_Blends)))
      തിരികെ;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  തർക്കം blending () സ്ഥിരം { തിരികെ deltas.length > 0; }
  ശൂന്യം reset_blends ()
  {
    അക്കം_മൂല്യങ്ങൾ_2 = മൂല്യം_Index_2 = 0;
    deltas.clear ();
  }

  unsigned പൂർണ്ണസംഖ്യ അക്കം_മൂല്യങ്ങൾ_2;
  unsigned പൂർണ്ണസംഖ്യ മൂല്യം_Index_2;
  hb_vector_t<സംഖ്യ_t> deltas;
};

തരനിർവചനം biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

template <typename ELEM>
ഘടന cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  template <typename ACC>
  cff2_cs_interp_env_t (സ്ഥിരം hb_ubytes_t &str, ACC &acc, unsigned പൂർണ്ണസംഖ്യ fd,
			സ്ഥിരം പൂർണ്ണസംഖ്യ *coords_=nullptr, unsigned പൂർണ്ണസംഖ്യ അക്കം_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_എണ്ണം_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    അക്കം_coords = അക്കം_coords_2;
    varStore = acc.varStore;
    do_blend = അക്കം_coords && varStore->size;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<ദശാംശം> *acquire_scalars_vector () സ്ഥിരം
  {
    hb_vector_t<ദശാംശം> *scalars = cached_scalars_vector->get_acquire ();

    എങ്കിൽ (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<ദശാംശം> *) hb_calloc (1, വലുപ്പം (hb_vector_t<ദശാംശം>));
      എങ്കിൽ (unlikely (!scalars))
	തിരികെ nullptr;
      scalars->init ();
    }

    തിരികെ scalars;
  }

  ശൂന്യം release_scalars_vector (hb_vector_t<ദശാംശം> *scalars) സ്ഥിരം
  {
    എങ്കിൽ (!scalars)
      തിരികെ;

    scalars->clear ();

    എങ്കിൽ (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    എങ്കിൽ (ഇത്->str_ref.avail ())
      തിരികെ SUPER::fetch_op ();

    /* make up return or endchar op */
    എങ്കിൽ (ഇത്->call_അട്ടി.is_empty ())
      തിരികെ OpCode_endchar;
    അല്ലെങ്കിൽ
      തിരികെ OpCode_return;
  }

  സ്ഥിരം ELEM& eval_arg (unsigned പൂർണ്ണസംഖ്യ i)
  {
    തിരികെ SUPER::arg_അട്ടി[i];
  }

  സ്ഥിരം ELEM& എടുക്കുക_arg ()
  {
    തിരികെ SUPER::arg_അട്ടി.എടുക്കുക ();
  }

  ശൂന്യം process_blend ()
  {
    എങ്കിൽ (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      എങ്കിൽ (unlikely (!scalars))
	SUPER::set_error ();
      അല്ലെങ്കിൽ
      {
	region_എണ്ണം_2 = varStore->varStore.get_region_index_എണ്ണം (get_ivs ());
	എങ്കിൽ (do_blend)
	{
	  എങ്കിൽ (unlikely (!scalars->resize_exact (region_എണ്ണം_2)))
	    SUPER::set_error ();
	  അല്ലെങ്കിൽ
	    varStore->varStore.get_region_scalars (get_ivs (), coords, അക്കം_coords,
						   &(*scalars)[0], region_എണ്ണം_2);
	}
      }
      seen_blend = സത്യം;
    }
  }

  ശൂന്യം process_vsindex ()
  {
    unsigned പൂർണ്ണസംഖ്യ  index = SUPER::arg_അട്ടി.എടുക്കുക_uint ();
    എങ്കിൽ (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    അല്ലെങ്കിൽ
    {
      set_ivs (index);
    }
    seen_vsindex_ = സത്യം;
  }

  unsigned പൂർണ്ണസംഖ്യ get_region_എണ്ണം () സ്ഥിരം { തിരികെ region_എണ്ണം_2; }
  ശൂന്യം	 set_region_എണ്ണം (unsigned പൂർണ്ണസംഖ്യ region_എണ്ണം_3) { region_എണ്ണം_2 = region_എണ്ണം_3; }
  unsigned പൂർണ്ണസംഖ്യ get_ivs () സ്ഥിരം { തിരികെ ivs; }
  ശൂന്യം	 set_ivs (unsigned പൂർണ്ണസംഖ്യ ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  unsigned പൂർണ്ണസംഖ്യ get_orig_ivs () സ്ഥിരം { തിരികെ orig_ivs; }
  തർക്കം	 seen_vsindex () സ്ഥിരം { തിരികെ seen_vsindex_; }

  ഇരട്ട blend_deltas (hb_array_t<സ്ഥിരം ELEM> deltas) സ്ഥിരം
  {
    ഇരട്ട v = 0;
    എങ്കിൽ (do_blend)
    {
      എങ്കിൽ (likely (scalars && scalars->length == deltas.length))
      {
        unsigned എണ്ണം = scalars->length;
	വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
	  v += (ഇരട്ട) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    തിരികെ v;
  }

  തർക്കം have_coords () സ്ഥിരം { തിരികെ അക്കം_coords; }

  സംരക്ഷിത:
  സ്ഥിരം പൂർണ്ണസംഖ്യ     *coords;
  unsigned പൂർണ്ണസംഖ്യ  അക്കം_coords;
  സ്ഥിരം	 CFF2ItemVariationStore *varStore;
  unsigned പൂർണ്ണസംഖ്യ  region_എണ്ണം_2;
  unsigned പൂർണ്ണസംഖ്യ  ivs;
  unsigned പൂർണ്ണസംഖ്യ  orig_ivs;
  hb_vector_t<ദശാംശം>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<ദശാംശം> *> *cached_scalars_vector = nullptr;
  തർക്കം	  do_blend;
  തർക്കം	  seen_vsindex_ = അസത്യം;
  തർക്കം	  seen_blend = അസത്യം;

  തരനിർവചനം cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
template <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
ഘടന cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_callsubr:
      അവസ്ഥ OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#എങ്കിൽ 0
	എങ്കിൽ (unlikely (env.arg_അട്ടി.peek ().blending ()))
	{
	  env.set_error ();
	  നിർത്തുക;
	}
#endif
	SUPER::process_op (op, env, param);
	നിർത്തുക;

      അവസ്ഥ OpCode_blendcs:
	OPSET::process_blend (env, param);
	നിർത്തുക;

      അവസ്ഥ OpCode_vsindexcs:
#എങ്കിൽ 0
	എങ്കിൽ (unlikely (env.arg_അട്ടി.peek ().blending ()))
	{
	  env.set_error ();
	  നിർത്തുക;
	}
#endif
	OPSET::process_vsindex (env, param);
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	SUPER::process_op (op, env, param);
    }
  }

  template <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  സ്ഥിതം ശൂന്യം process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 സ്ഥിരം hb_array_t<സ്ഥിരം ELEM> blends,
				 unsigned n, unsigned i)
  {
    എങ്കിൽ (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<സംഖ്യ_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      എങ്കിൽ (likely (inner.length == arg.deltas.length))
	വേണ്ടി (unsigned j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      അല്ലെങ്കിൽ
	env.set_error ();
    }
    അല്ലെങ്കിൽ
      arg.set_blends (n, i, blends);
  }
  template <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  സ്ഥിതം ശൂന്യം process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 സ്ഥിരം hb_array_t<സ്ഥിരം ELEM> blends,
				 unsigned n, unsigned i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  സ്ഥിതം ശൂന്യം process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    unsigned പൂർണ്ണസംഖ്യ n, k;

    env.process_blend ();
    k = env.get_region_എണ്ണം ();
    n = env.arg_അട്ടി.എടുക്കുക_uint ();
    /* copy the blend values into blend array of the default values */
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = env.arg_അട്ടി.get_എണ്ണം ();
    unsigned പൂർണ്ണസംഖ്യ total;
    എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > എണ്ണം))
    {
      env.set_error ();
      തിരികെ;
    }
    എങ്കിൽ (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      തിരികെ;
    }
    unsigned പൂർണ്ണസംഖ്യ start = എണ്ണം - total;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
    {
      സ്ഥിരം hb_array_t<സ്ഥിരം ELEM> blends = env.arg_അട്ടി.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_അട്ടി.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_അട്ടി.എടുക്കുക (k * n);
  }

  സ്ഥിതം ശൂന്യം process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  സ്വകാര്യ:
  തരനിർവചനം cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
