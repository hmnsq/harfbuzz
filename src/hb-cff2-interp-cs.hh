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

संरचना blend_arg_t : संख्या_t
{
  शून्य set_int (पूर्णाङ्क v) { reset_blends (); संख्या_t::set_int (v); }
  शून्य set_fixed (int32_t v) { reset_blends (); संख्या_t::set_fixed (v); }
  शून्य set_real (द्विगुण v) { reset_blends (); संख्या_t::set_real (v); }

  शून्य set_blends (अचिन्हित पूर्णाङ्क अंक_मूल्यावली, अचिन्हित पूर्णाङ्क मूल्य_Index,
		   hb_array_t<ध्रुव blend_arg_t> blends_)
  {
    अंक_मूल्यावली_2 = अंक_मूल्यावली;
    मूल्य_Index_2 = मूल्य_Index;
    अचिन्हित अंक_Blends = blends_.length;
    यदि (unlikely (!deltas.resize_exact (अंक_Blends)))
      प्रत्यागम;
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < अंक_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  तार्किक blending () ध्रुव { प्रत्यागम deltas.length > 0; }
  शून्य reset_blends ()
  {
    अंक_मूल्यावली_2 = मूल्य_Index_2 = 0;
    deltas.clear ();
  }

  अचिन्हित पूर्णाङ्क अंक_मूल्यावली_2;
  अचिन्हित पूर्णाङ्क मूल्य_Index_2;
  hb_vector_t<संख्या_t> deltas;
};

प्रकारनिर्देश biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

ढाँचा <typename ELEM>
संरचना cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  ढाँचा <typename ACC>
  cff2_cs_interp_env_t (ध्रुव hb_ubytes_t &str, ACC &acc, अचिन्हित पूर्णाङ्क fd,
			ध्रुव पूर्णाङ्क *coords_=nullptr, अचिन्हित पूर्णाङ्क अंक_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_गिनती_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    अंक_coords = अंक_coords_2;
    varStore = acc.varStore;
    do_blend = अंक_coords && varStore->size;
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

    यदि (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<भिन्न> *) hb_calloc (1, परिमाण (hb_vector_t<भिन्न>));
      यदि (unlikely (!scalars))
	प्रत्यागम nullptr;
      scalars->init ();
    }

    प्रत्यागम scalars;
  }

  शून्य release_scalars_vector (hb_vector_t<भिन्न> *scalars) ध्रुव
  {
    यदि (!scalars)
      प्रत्यागम;

    scalars->clear ();

    यदि (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    यदि (एतत्->str_ref.avail ())
      प्रत्यागम SUPER::fetch_op ();

    /* make up return or endchar op */
    यदि (एतत्->call_ढेर.is_empty ())
      प्रत्यागम OpCode_endchar;
    अन्यथा
      प्रत्यागम OpCode_return;
  }

  ध्रुव ELEM& eval_arg (अचिन्हित पूर्णाङ्क i)
  {
    प्रत्यागम SUPER::arg_ढेर[i];
  }

  ध्रुव ELEM& निकालो_arg ()
  {
    प्रत्यागम SUPER::arg_ढेर.निकालो ();
  }

  शून्य process_blend ()
  {
    यदि (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      यदि (unlikely (!scalars))
	SUPER::set_error ();
      अन्यथा
      {
	region_गिनती_2 = varStore->varStore.get_region_index_गिनती (get_ivs ());
	यदि (do_blend)
	{
	  यदि (unlikely (!scalars->resize_exact (region_गिनती_2)))
	    SUPER::set_error ();
	  अन्यथा
	    varStore->varStore.get_region_scalars (get_ivs (), coords, अंक_coords,
						   &(*scalars)[0], region_गिनती_2);
	}
      }
      seen_blend = सत्य;
    }
  }

  शून्य process_vsindex ()
  {
    अचिन्हित पूर्णाङ्क  index = SUPER::arg_ढेर.निकालो_uint ();
    यदि (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    अन्यथा
    {
      set_ivs (index);
    }
    seen_vsindex_ = सत्य;
  }

  अचिन्हित पूर्णाङ्क get_region_गिनती () ध्रुव { प्रत्यागम region_गिनती_2; }
  शून्य	 set_region_गिनती (अचिन्हित पूर्णाङ्क region_गिनती_3) { region_गिनती_2 = region_गिनती_3; }
  अचिन्हित पूर्णाङ्क get_ivs () ध्रुव { प्रत्यागम ivs; }
  शून्य	 set_ivs (अचिन्हित पूर्णाङ्क ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  अचिन्हित पूर्णाङ्क get_orig_ivs () ध्रुव { प्रत्यागम orig_ivs; }
  तार्किक	 seen_vsindex () ध्रुव { प्रत्यागम seen_vsindex_; }

  द्विगुण blend_deltas (hb_array_t<ध्रुव ELEM> deltas) ध्रुव
  {
    द्विगुण v = 0;
    यदि (do_blend)
    {
      यदि (likely (scalars && scalars->length == deltas.length))
      {
        अचिन्हित गिनती = scalars->length;
	प्रति (अचिन्हित i = 0; i < गिनती; i++)
	  v += (द्विगुण) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    प्रत्यागम v;
  }

  तार्किक have_coords () ध्रुव { प्रत्यागम अंक_coords; }

  रक्षित:
  ध्रुव पूर्णाङ्क     *coords;
  अचिन्हित पूर्णाङ्क  अंक_coords;
  ध्रुव	 CFF2ItemVariationStore *varStore;
  अचिन्हित पूर्णाङ्क  region_गिनती_2;
  अचिन्हित पूर्णाङ्क  ivs;
  अचिन्हित पूर्णाङ्क  orig_ivs;
  hb_vector_t<भिन्न>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<भिन्न> *> *cached_scalars_vector = nullptr;
  तार्किक	  do_blend;
  तार्किक	  seen_vsindex_ = असत्य;
  तार्किक	  seen_blend = असत्य;

  प्रकारनिर्देश cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
ढाँचा <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
संरचना cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  स्थिर शून्य process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    विकल्प (op) {
      पक्ष OpCode_callsubr:
      पक्ष OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#यदि 0
	यदि (unlikely (env.arg_ढेर.peek ().blending ()))
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
#यदि 0
	यदि (unlikely (env.arg_ढेर.peek ().blending ()))
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

  ढाँचा <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  स्थिर शून्य process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ध्रुव hb_array_t<ध्रुव ELEM> blends,
				 अचिन्हित n, अचिन्हित i)
  {
    यदि (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    अन्यथा यदि (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<संख्या_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      यदि (likely (inner.length == arg.deltas.length))
	प्रति (अचिन्हित j = 0; j < inner.length; j++)
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
  स्थिर शून्य process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ध्रुव hb_array_t<ध्रुव ELEM> blends,
				 अचिन्हित n, अचिन्हित i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  स्थिर शून्य process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    अचिन्हित पूर्णाङ्क n, k;

    env.process_blend ();
    k = env.get_region_गिनती ();
    n = env.arg_ढेर.निकालो_uint ();
    /* copy the blend values into blend array of the default values */
    अचिन्हित पूर्णाङ्क गिनती = env.arg_ढेर.get_गिनती ();
    अचिन्हित पूर्णाङ्क total;
    यदि (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > गिनती))
    {
      env.set_error ();
      प्रत्यागम;
    }
    यदि (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      प्रत्यागम;
    }
    अचिन्हित पूर्णाङ्क start = गिनती - total;
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < n; i++)
    {
      ध्रुव hb_array_t<ध्रुव ELEM> blends = env.arg_ढेर.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_ढेर.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ढेर.निकालो (k * n);
  }

  स्थिर शून्य process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  गुप्त:
  प्रकारनिर्देश cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

ढाँचा <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
