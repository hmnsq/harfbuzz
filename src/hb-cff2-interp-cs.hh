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
  शून्य set_int (पूर्णांक v) { reset_blends (); संख्या_t::set_int (v); }
  शून्य set_fixed (int32_t v) { reset_blends (); संख्या_t::set_fixed (v); }
  शून्य set_real (द्विगुण v) { reset_blends (); संख्या_t::set_real (v); }

  शून्य set_blends (अचिन्हित पूर्णांक अंक_मूल्ये, अचिन्हित पूर्णांक मूल्य_Index,
		   hb_array_t<स्थिरांक blend_arg_t> blends_)
  {
    अंक_मूल्ये_2 = अंक_मूल्ये;
    मूल्य_Index_2 = मूल्य_Index;
    अचिन्हित अंक_Blends = blends_.length;
    जर (unlikely (!deltas.resize_exact (अंक_Blends)))
      परत;
    साठी (अचिन्हित पूर्णांक i = 0; i < अंक_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  तार्किक blending () स्थिरांक { परत deltas.length > 0; }
  शून्य reset_blends ()
  {
    अंक_मूल्ये_2 = मूल्य_Index_2 = 0;
    deltas.clear ();
  }

  अचिन्हित पूर्णांक अंक_मूल्ये_2;
  अचिन्हित पूर्णांक मूल्य_Index_2;
  hb_vector_t<संख्या_t> deltas;
};

प्रकारव्याख्या biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

ढाँचा <typename ELEM>
संरचना cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  ढाँचा <typename ACC>
  cff2_cs_interp_env_t (स्थिरांक hb_ubytes_t &str, ACC &acc, अचिन्हित पूर्णांक fd,
			स्थिरांक पूर्णांक *coords_=nullptr, अचिन्हित पूर्णांक अंक_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_मोजणी_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
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

  hb_vector_t<दशांश> *acquire_scalars_vector () स्थिरांक
  {
    hb_vector_t<दशांश> *scalars = cached_scalars_vector->get_acquire ();

    जर (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<दशांश> *) hb_calloc (1, आकार (hb_vector_t<दशांश>));
      जर (unlikely (!scalars))
	परत nullptr;
      scalars->init ();
    }

    परत scalars;
  }

  शून्य release_scalars_vector (hb_vector_t<दशांश> *scalars) स्थिरांक
  {
    जर (!scalars)
      परत;

    scalars->clear ();

    जर (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    जर (हे->str_ref.avail ())
      परत SUPER::fetch_op ();

    /* make up return or endchar op */
    जर (हे->call_चळत.is_empty ())
      परत OpCode_endchar;
    अन्यथा
      परत OpCode_return;
  }

  स्थिरांक ELEM& eval_arg (अचिन्हित पूर्णांक i)
  {
    परत SUPER::arg_चळत[i];
  }

  स्थिरांक ELEM& काढा_arg ()
  {
    परत SUPER::arg_चळत.काढा ();
  }

  शून्य process_blend ()
  {
    जर (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      जर (unlikely (!scalars))
	SUPER::set_error ();
      अन्यथा
      {
	region_मोजणी_2 = varStore->varStore.get_region_index_मोजणी (get_ivs ());
	जर (do_blend)
	{
	  जर (unlikely (!scalars->resize_exact (region_मोजणी_2)))
	    SUPER::set_error ();
	  अन्यथा
	    varStore->varStore.get_region_scalars (get_ivs (), coords, अंक_coords,
						   &(*scalars)[0], region_मोजणी_2);
	}
      }
      seen_blend = सत्य;
    }
  }

  शून्य process_vsindex ()
  {
    अचिन्हित पूर्णांक  index = SUPER::arg_चळत.काढा_uint ();
    जर (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    अन्यथा
    {
      set_ivs (index);
    }
    seen_vsindex_ = सत्य;
  }

  अचिन्हित पूर्णांक get_region_मोजणी () स्थिरांक { परत region_मोजणी_2; }
  शून्य	 set_region_मोजणी (अचिन्हित पूर्णांक region_मोजणी_3) { region_मोजणी_2 = region_मोजणी_3; }
  अचिन्हित पूर्णांक get_ivs () स्थिरांक { परत ivs; }
  शून्य	 set_ivs (अचिन्हित पूर्णांक ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  अचिन्हित पूर्णांक get_orig_ivs () स्थिरांक { परत orig_ivs; }
  तार्किक	 seen_vsindex () स्थिरांक { परत seen_vsindex_; }

  द्विगुण blend_deltas (hb_array_t<स्थिरांक ELEM> deltas) स्थिरांक
  {
    द्विगुण v = 0;
    जर (do_blend)
    {
      जर (likely (scalars && scalars->length == deltas.length))
      {
        अचिन्हित मोजणी = scalars->length;
	साठी (अचिन्हित i = 0; i < मोजणी; i++)
	  v += (द्विगुण) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    परत v;
  }

  तार्किक have_coords () स्थिरांक { परत अंक_coords; }

  संरक्षित:
  स्थिरांक पूर्णांक     *coords;
  अचिन्हित पूर्णांक  अंक_coords;
  स्थिरांक	 CFF2ItemVariationStore *varStore;
  अचिन्हित पूर्णांक  region_मोजणी_2;
  अचिन्हित पूर्णांक  ivs;
  अचिन्हित पूर्णांक  orig_ivs;
  hb_vector_t<दशांश>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<दशांश> *> *cached_scalars_vector = nullptr;
  तार्किक	  do_blend;
  तार्किक	  seen_vsindex_ = असत्य;
  तार्किक	  seen_blend = असत्य;

  प्रकारव्याख्या cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
ढाँचा <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
संरचना cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  स्थिर शून्य process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    निवड (op) {
      स्थिती OpCode_callsubr:
      स्थिती OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#जर 0
	जर (unlikely (env.arg_चळत.peek ().blending ()))
	{
	  env.set_error ();
	  थांबा;
	}
#endif
	SUPER::process_op (op, env, param);
	थांबा;

      स्थिती OpCode_blendcs:
	OPSET::process_blend (env, param);
	थांबा;

      स्थिती OpCode_vsindexcs:
#जर 0
	जर (unlikely (env.arg_चळत.peek ().blending ()))
	{
	  env.set_error ();
	  थांबा;
	}
#endif
	OPSET::process_vsindex (env, param);
	थांबा;

      पूर्वनिर्धारित:
	SUPER::process_op (op, env, param);
    }
  }

  ढाँचा <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  स्थिर शून्य process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 स्थिरांक hb_array_t<स्थिरांक ELEM> blends,
				 अचिन्हित n, अचिन्हित i)
  {
    जर (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    अन्यथा जर (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<संख्या_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      जर (likely (inner.length == arg.deltas.length))
	साठी (अचिन्हित j = 0; j < inner.length; j++)
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
				 स्थिरांक hb_array_t<स्थिरांक ELEM> blends,
				 अचिन्हित n, अचिन्हित i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  स्थिर शून्य process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    अचिन्हित पूर्णांक n, k;

    env.process_blend ();
    k = env.get_region_मोजणी ();
    n = env.arg_चळत.काढा_uint ();
    /* copy the blend values into blend array of the default values */
    अचिन्हित पूर्णांक मोजणी = env.arg_चळत.get_मोजणी ();
    अचिन्हित पूर्णांक total;
    जर (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > मोजणी))
    {
      env.set_error ();
      परत;
    }
    जर (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      परत;
    }
    अचिन्हित पूर्णांक start = मोजणी - total;
    साठी (अचिन्हित पूर्णांक i = 0; i < n; i++)
    {
      स्थिरांक hb_array_t<स्थिरांक ELEM> blends = env.arg_चळत.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_चळत.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_चळत.काढा (k * n);
  }

  स्थिर शून्य process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  खाजगी:
  प्रकारव्याख्या cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

ढाँचा <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
