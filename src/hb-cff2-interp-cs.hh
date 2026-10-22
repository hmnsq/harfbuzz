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
#अगर_अघोषित HB_CFF2_INTERP_CS_HH
#घोषणा HB_CFF2_INTERP_CS_HH

#समावेश "hb.hh"
#समावेश "hb-cff-interp-cs-common.hh"

namespace CFF {

using namespace OT;

काष्ठा blend_arg_t : संख्या_t
{
  व्योम set_int (पूर्णांक v) { reset_blends (); संख्या_t::set_int (v); }
  व्योम set_fixed (int32_t v) { reset_blends (); संख्या_t::set_fixed (v); }
  व्योम set_real (द्विगुन v) { reset_blends (); संख्या_t::set_real (v); }

  व्योम set_blends (अचिन्हित पूर्णांक अंक_मूल्यावली, अचिन्हित पूर्णांक मूल्य_सूचकांक,
		   hb_array_t<स्थिर blend_arg_t> blends_)
  {
    अंक_मूल्यावली_2 = अंक_मूल्यावली;
    मूल्य_सूचकांक_2 = मूल्य_सूचकांक;
    अचिन्हित अंक_Blends = blends_.लंबाई;
    अगर (unlikely (!deltas.resize_exact (अंक_Blends)))
      वापस;
    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  bool blending () स्थिर { वापस deltas.लंबाई > 0; }
  व्योम reset_blends ()
  {
    अंक_मूल्यावली_2 = मूल्य_सूचकांक_2 = 0;
    deltas.सफा ();
  }

  अचिन्हित पूर्णांक अंक_मूल्यावली_2;
  अचिन्हित पूर्णांक मूल्य_सूचकांक_2;
  hb_vector_t<संख्या_t> deltas;
};

प्रकार biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

ढाँचा <typename ELEM>
काष्ठा cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  ढाँचा <typename ACC>
  cff2_cs_interp_env_t (स्थिर hb_ubytes_t &str, ACC &acc, अचिन्हित पूर्णांक fd,
			स्थिर पूर्णांक *coords_=nullptr, अचिन्हित पूर्णांक अंक_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_गिनती_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    अंक_coords = अंक_coords_2;
    varStore = acc.varStore;
    do_blend = अंक_coords && varStore->आकार;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<भग्न> *acquire_scalars_vector () स्थिर
  {
    hb_vector_t<भग्न> *scalars = cached_scalars_vector->get_acquire ();

    अगर (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<भग्न> *) hb_calloc (1, माप (hb_vector_t<भग्न>));
      अगर (unlikely (!scalars))
	वापस nullptr;
      scalars->init ();
    }

    वापस scalars;
  }

  व्योम release_scalars_vector (hb_vector_t<भग्न> *scalars) स्थिर
  {
    अगर (!scalars)
      वापस;

    scalars->सफा ();

    अगर (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    अगर (यह->str_ref.avail ())
      वापस SUPER::fetch_op ();

    /* make up return or endchar op */
    अगर (यह->call_ढेर.is_empty ())
      वापस OpCode_endchar;
    अन्यथा
      वापस OpCode_return;
  }

  स्थिर ELEM& eval_arg (अचिन्हित पूर्णांक i)
  {
    वापस SUPER::arg_ढेर[i];
  }

  स्थिर ELEM& निकालो_arg ()
  {
    वापस SUPER::arg_ढेर.निकालो ();
  }

  व्योम process_blend ()
  {
    अगर (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      अगर (unlikely (!scalars))
	SUPER::set_error ();
      अन्यथा
      {
	region_गिनती_2 = varStore->varStore.get_region_सूचकांक_गिनती (get_ivs ());
	अगर (do_blend)
	{
	  अगर (unlikely (!scalars->resize_exact (region_गिनती_2)))
	    SUPER::set_error ();
	  अन्यथा
	    varStore->varStore.get_region_scalars (get_ivs (), coords, अंक_coords,
						   &(*scalars)[0], region_गिनती_2);
	}
      }
      seen_blend = true;
    }
  }

  व्योम process_vsindex ()
  {
    अचिन्हित पूर्णांक  सूचकांक_2 = SUPER::arg_ढेर.निकालो_uint ();
    अगर (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    अन्यथा
    {
      set_ivs (सूचकांक_2);
    }
    seen_vsindex_ = true;
  }

  अचिन्हित पूर्णांक get_region_गिनती () स्थिर { वापस region_गिनती_2; }
  व्योम	 set_region_गिनती (अचिन्हित पूर्णांक region_गिनती_3) { region_गिनती_2 = region_गिनती_3; }
  अचिन्हित पूर्णांक get_ivs () स्थिर { वापस ivs; }
  व्योम	 set_ivs (अचिन्हित पूर्णांक ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  अचिन्हित पूर्णांक get_orig_ivs () स्थिर { वापस orig_ivs; }
  bool	 seen_vsindex () स्थिर { वापस seen_vsindex_; }

  द्विगुन blend_deltas (hb_array_t<स्थिर ELEM> deltas) स्थिर
  {
    द्विगुन v = 0;
    अगर (do_blend)
    {
      अगर (likely (scalars && scalars->लंबाई == deltas.लंबाई))
      {
        अचिन्हित गिनती = scalars->लंबाई;
	क्रम (अचिन्हित i = 0; i < गिनती; i++)
	  v += (द्विगुन) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    वापस v;
  }

  bool have_coords () स्थिर { वापस अंक_coords; }

  रक्षित:
  स्थिर पूर्णांक     *coords;
  अचिन्हित पूर्णांक  अंक_coords;
  स्थिर	 CFF2ItemVariationStore *varStore;
  अचिन्हित पूर्णांक  region_गिनती_2;
  अचिन्हित पूर्णांक  ivs;
  अचिन्हित पूर्णांक  orig_ivs;
  hb_vector_t<भग्न>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<भग्न> *> *cached_scalars_vector = nullptr;
  bool	  do_blend;
  bool	  seen_vsindex_ = false;
  bool	  seen_blend = false;

  प्रकार cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
ढाँचा <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
काष्ठा cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  अटल व्योम process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    चयन (op) {
      हाल OpCode_callsubr:
      हाल OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#अगर 0
	अगर (unlikely (env.arg_ढेर.peek ().blending ()))
	{
	  env.set_error ();
	  अवरोध;
	}
#पूर्ण_अगर
	SUPER::process_op (op, env, param);
	अवरोध;

      हाल OpCode_blendcs:
	OPSET::process_blend (env, param);
	अवरोध;

      हाल OpCode_vsindexcs:
#अगर 0
	अगर (unlikely (env.arg_ढेर.peek ().blending ()))
	{
	  env.set_error ();
	  अवरोध;
	}
#पूर्ण_अगर
	OPSET::process_vsindex (env, param);
	अवरोध;

      शेष:
	SUPER::process_op (op, env, param);
    }
  }

  ढाँचा <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  अटल व्योम process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 स्थिर hb_array_t<स्थिर ELEM> blends,
				 अचिन्हित n, अचिन्हित i)
  {
    अगर (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    अन्यथा अगर (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<संख्या_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      अगर (likely (inner.लंबाई == arg.deltas.लंबाई))
	क्रम (अचिन्हित j = 0; j < inner.लंबाई; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      अन्यथा
	env.set_error ();
    }
    अन्यथा
      arg.set_blends (n, i, blends);
  }
  ढाँचा <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  अटल व्योम process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 स्थिर hb_array_t<स्थिर ELEM> blends,
				 अचिन्हित n, अचिन्हित i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  अटल व्योम process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    अचिन्हित पूर्णांक n, k;

    env.process_blend ();
    k = env.get_region_गिनती ();
    n = env.arg_ढेर.निकालो_uint ();
    /* copy the blend values into blend array of the default values */
    अचिन्हित पूर्णांक गिनती = env.arg_ढेर.get_गिनती ();
    अचिन्हित पूर्णांक योग;
    अगर (unlikely (hb_unsigned_mul_overflows (k + 1, n, &योग) || योग > गिनती))
    {
      env.set_error ();
      वापस;
    }
    अगर (unlikely (!env.spend_budget (HB_BUDGET_1, योग - n)))
    {
      env.set_error ();
      वापस;
    }
    अचिन्हित पूर्णांक start = गिनती - योग;
    क्रम (अचिन्हित पूर्णांक i = 0; i < n; i++)
    {
      स्थिर hb_array_t<स्थिर ELEM> blends = env.arg_ढेर.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_ढेर.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ढेर.निकालो (k * n);
  }

  अटल व्योम process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  निजी:
  प्रकार cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

ढाँचा <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#पूर्ण_अगर /* HB_CFF2_INTERP_CS_HH */
