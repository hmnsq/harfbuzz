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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-cff-interp-cs-common.hh"

नामाकाश CFF {

using नामाकाश OT;

संरचना blend_arg_t : सङ्ख्या_t
{
  शून्य set_int (पूर्णाङ्क v) { reset_blends (); सङ्ख्या_t::set_int (v); }
  शून्य set_fixed (int32_t v) { reset_blends (); सङ्ख्या_t::set_fixed (v); }
  शून्य set_real (द्विगुण v) { reset_blends (); सङ्ख्या_t::set_real (v); }

  शून्य set_blends (unsigned पूर्णाङ्क अङ्कः_मानानि, unsigned पूर्णाङ्क मानम्_Index,
		   hb_array_t<ध्रुव blend_arg_t> blends_)
  {
    अङ्कः_मानानि_2 = अङ्कः_मानानि;
    मानम्_Index_2 = मानम्_Index;
    unsigned अङ्कः_Blends = blends_.length;
    सचे (unlikely (!deltas.resize_exact (अङ्कः_Blends)))
      पटिनिवत्त;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < अङ्कः_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  तार्किक blending () ध्रुव { पटिनिवत्त deltas.length > 0; }
  शून्य reset_blends ()
  {
    अङ्कः_मानानि_2 = मानम्_Index_2 = 0;
    deltas.clear ();
  }

  unsigned पूर्णाङ्क अङ्कः_मानानि_2;
  unsigned पूर्णाङ्क मानम्_Index_2;
  hb_vector_t<सङ्ख्या_t> deltas;
};

प्रकारनिर्देश biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

template <typename ELEM>
संरचना cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  template <typename ACC>
  cff2_cs_interp_env_t (ध्रुव hb_ubytes_t &str, ACC &acc, unsigned पूर्णाङ्क fd,
			ध्रुव पूर्णाङ्क *coords_=nullptr, unsigned पूर्णाङ्क अङ्कः_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_गणनम्_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    अङ्कः_coords = अङ्कः_coords_2;
    varStore = acc.varStore;
    do_blend = अङ्कः_coords && varStore->size;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<भिन्न> *acquire_scalars_vector () ध्रुव
  {
    hb_vector_t<भिन्न> *scalars = cached_scalars_vector->get_acquire ();

    सचे (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<भिन्न> *) hb_calloc (1, परिमाण (hb_vector_t<भिन्न>));
      सचे (unlikely (!scalars))
	पटिनिवत्त nullptr;
      scalars->init ();
    }

    पटिनिवत्त scalars;
  }

  शून्य release_scalars_vector (hb_vector_t<भिन्न> *scalars) ध्रुव
  {
    सचे (!scalars)
      पटिनिवत्त;

    scalars->clear ();

    सचे (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    सचे (एतत्->str_ref.avail ())
      पटिनिवत्त SUPER::fetch_op ();

    /* make up return or endchar op */
    सचे (एतत्->call_राशिः.is_empty ())
      पटिनिवत्त OpCode_endchar;
    अञ्ञथा
      पटिनिवत्त OpCode_return;
  }

  ध्रुव ELEM& eval_arg (unsigned पूर्णाङ्क i)
  {
    पटिनिवत्त SUPER::arg_राशिः[i];
  }

  ध्रुव ELEM& निष्कासय_arg ()
  {
    पटिनिवत्त SUPER::arg_राशिः.निष्कासय ();
  }

  शून्य process_blend ()
  {
    सचे (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      सचे (unlikely (!scalars))
	SUPER::set_error ();
      अञ्ञथा
      {
	region_गणनम्_2 = varStore->varStore.get_region_index_गणनम् (get_ivs ());
	सचे (do_blend)
	{
	  सचे (unlikely (!scalars->resize_exact (region_गणनम्_2)))
	    SUPER::set_error ();
	  अञ्ञथा
	    varStore->varStore.get_region_scalars (get_ivs (), coords, अङ्कः_coords,
						   &(*scalars)[0], region_गणनम्_2);
	}
      }
      seen_blend = सच्च;
    }
  }

  शून्य process_vsindex ()
  {
    unsigned पूर्णाङ्क  index = SUPER::arg_राशिः.निष्कासय_uint ();
    सचे (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    अञ्ञथा
    {
      set_ivs (index);
    }
    seen_vsindex_ = सच्च;
  }

  unsigned पूर्णाङ्क get_region_गणनम् () ध्रुव { पटिनिवत्त region_गणनम्_2; }
  शून्य	 set_region_गणनम् (unsigned पूर्णाङ्क region_गणनम्_3) { region_गणनम्_2 = region_गणनम्_3; }
  unsigned पूर्णाङ्क get_ivs () ध्रुव { पटिनिवत्त ivs; }
  शून्य	 set_ivs (unsigned पूर्णाङ्क ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  unsigned पूर्णाङ्क get_orig_ivs () ध्रुव { पटिनिवत्त orig_ivs; }
  तार्किक	 seen_vsindex () ध्रुव { पटिनिवत्त seen_vsindex_; }

  द्विगुण blend_deltas (hb_array_t<ध्रुव ELEM> deltas) ध्रुव
  {
    द्विगुण v = 0;
    सचे (do_blend)
    {
      सचे (likely (scalars && scalars->length == deltas.length))
      {
        unsigned गणनम् = scalars->length;
	अत्थाय (unsigned i = 0; i < गणनम्; i++)
	  v += (द्विगुण) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    पटिनिवत्त v;
  }

  तार्किक have_coords () ध्रुव { पटिनिवत्त अङ्कः_coords; }

  रक्षित:
  ध्रुव पूर्णाङ्क     *coords;
  unsigned पूर्णाङ्क  अङ्कः_coords;
  ध्रुव	 CFF2ItemVariationStore *varStore;
  unsigned पूर्णाङ्क  region_गणनम्_2;
  unsigned पूर्णाङ्क  ivs;
  unsigned पूर्णाङ्क  orig_ivs;
  hb_vector_t<भिन्न>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<भिन्न> *> *cached_scalars_vector = nullptr;
  तार्किक	  do_blend;
  तार्किक	  seen_vsindex_ = मिच्छा;
  तार्किक	  seen_blend = मिच्छा;

  प्रकारनिर्देश cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
template <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
संरचना cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  स्थिर शून्य process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    विकल्प (op) {
      पक्ष OpCode_callsubr:
      पक्ष OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#सचे 0
	सचे (unlikely (env.arg_राशिः.peek ().blending ()))
	{
	  env.set_error ();
	  भङ्ग;
	}
#endif
	SUPER::process_op (op, env, param);
	भङ्ग;

      पक्ष OpCode_blendcs:
	OPSET::process_blend (env, param);
	भङ्ग;

      पक्ष OpCode_vsindexcs:
#सचे 0
	सचे (unlikely (env.arg_राशिः.peek ().blending ()))
	{
	  env.set_error ();
	  भङ्ग;
	}
#endif
	OPSET::process_vsindex (env, param);
	भङ्ग;

      स्वभाव:
	SUPER::process_op (op, env, param);
    }
  }

  template <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  स्थिर शून्य process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ध्रुव hb_array_t<ध्रुव ELEM> blends,
				 unsigned n, unsigned i)
  {
    सचे (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    अञ्ञथा सचे (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<सङ्ख्या_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      सचे (likely (inner.length == arg.deltas.length))
	अत्थाय (unsigned j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      अञ्ञथा
	env.set_error ();
    }
    अञ्ञथा
      arg.set_blends (n, i, blends);
  }
  template <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  स्थिर शून्य process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ध्रुव hb_array_t<ध्रुव ELEM> blends,
				 unsigned n, unsigned i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  स्थिर शून्य process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    unsigned पूर्णाङ्क n, k;

    env.process_blend ();
    k = env.get_region_गणनम् ();
    n = env.arg_राशिः.निष्कासय_uint ();
    /* copy the blend values into blend array of the default values */
    unsigned पूर्णाङ्क गणनम् = env.arg_राशिः.get_गणनम् ();
    unsigned पूर्णाङ्क total;
    सचे (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > गणनम्))
    {
      env.set_error ();
      पटिनिवत्त;
    }
    सचे (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      पटिनिवत्त;
    }
    unsigned पूर्णाङ्क start = गणनम् - total;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < n; i++)
    {
      ध्रुव hb_array_t<ध्रुव ELEM> blends = env.arg_राशिः.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_राशिः.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_राशिः.निष्कासय (k * n);
  }

  स्थिर शून्य process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  गुप्त:
  प्रकारनिर्देश cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
