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
#که_نه_تعریف_شوی HB_CFF2_INTERP_CS_HH
#تعریف_کړه HB_CFF2_INTERP_CS_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

کارول نطاق OT;

بنية blend_arg_t : شمېره_t
{
  فراغ سېټ_int (صحيح v) { reset_blends (); شمېره_t::سېټ_int (v); }
  فراغ سېټ_fixed (int32_t v) { reset_blends (); شمېره_t::سېټ_fixed (v); }
  فراغ سېټ_real (مزدوج v) { reset_blends (); شمېره_t::سېټ_real (v); }

  فراغ سېټ_blends (بینښه صحيح عدد_ارزښتونه, بینښه صحيح ارزښت_شاخص,
		   hb_array_t<ثابت blend_arg_t> blends_)
  {
    عدد_ارزښتونه_2 = عدد_ارزښتونه;
    ارزښت_شاخص_2 = ارزښت_شاخص;
    بینښه عدد_Blends = blends_.اوږدتیا;
    إذا (unlikely (!deltas.resize_exact (عدد_Blends)))
      أرجع;
    لكل (بینښه صحيح i = 0; i < عدد_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  منطقي blending () ثابت { أرجع deltas.اوږدتیا > 0; }
  فراغ reset_blends ()
  {
    عدد_ارزښتونه_2 = ارزښت_شاخص_2 = 0;
    deltas.clear ();
  }

  بینښه صحيح عدد_ارزښتونه_2;
  بینښه صحيح ارزښت_شاخص_2;
  hb_ویکټور_t<شمېره_t> deltas;
};

تعريفنوع biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

کينډۍ <ډول_نوم ELEM>
بنية cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  کينډۍ <ډول_نوم ACC>
  cff2_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, بینښه صحيح fd,
			ثابت صحيح *coords_=تش_اشاره, بینښه صحيح عدد_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.شخصي_Dicts[fd].localSubrs),
      region_شمېر_2 (0), cached_scalars_ویکټور (&acc.cached_scalars_ویکټور)
  {
    coords = coords_;
    عدد_coords = عدد_coords_2;
    varStore = acc.varStore;
    do_blend = عدد_coords && varStore->اندازه;
    orig_ivs = acc.شخصي_Dicts[fd].ivs;
    سېټ_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_ویکټور (scalars);
  }

  hb_ویکټور_t<عائم> *acquire_scalars_ویکټور () ثابت
  {
    hb_ویکټور_t<عائم> *scalars = cached_scalars_ویکټور->get_acquire ();

    إذا (!scalars || !cached_scalars_ویکټور->cmpexch (scalars, تش_اشاره))
    {
      scalars = (hb_ویکټور_t<عائم> *) hb_calloc (1, حجم (hb_ویکټور_t<عائم>));
      إذا (unlikely (!scalars))
	أرجع تش_اشاره;
      scalars->init ();
    }

    أرجع scalars;
  }

  فراغ release_scalars_ویکټور (hb_ویکټور_t<عائم> *scalars) ثابت
  {
    إذا (!scalars)
      أرجع;

    scalars->clear ();

    إذا (!cached_scalars_ویکټور->cmpexch (تش_اشاره, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = تش_اشاره;
  }

  op_کوډ_t fetch_op ()
  {
    إذا (هذا->str_ref.avail ())
      أرجع SUPER::fetch_op ();

    /* make up return or endchar op */
    إذا (هذا->call_ډېری.is_empty ())
      أرجع Op_کوډ_endchar;
    وإلا
      أرجع Op_کوډ_return;
  }

  ثابت ELEM& eval_arg (بینښه صحيح i)
  {
    أرجع SUPER::arg_ډېری[i];
  }

  ثابت ELEM& راوباسه_arg ()
  {
    أرجع SUPER::arg_ډېری.راوباسه ();
  }

  فراغ process_blend ()
  {
    إذا (!کتل_شوی_blend)
    {
      scalars = acquire_scalars_ویکټور ();
      إذا (unlikely (!scalars))
	SUPER::سېټ_error ();
      وإلا
      {
	region_شمېر_2 = varStore->varStore.get_region_شاخص_شمېر (get_ivs ());
	إذا (do_blend)
	{
	  إذا (unlikely (!scalars->resize_exact (region_شمېر_2)))
	    SUPER::سېټ_error ();
	  وإلا
	    varStore->varStore.get_region_scalars (get_ivs (), coords, عدد_coords,
						   &(*scalars)[0], region_شمېر_2);
	}
      }
      کتل_شوی_blend = ریښتیا;
    }
  }

  فراغ process_vsindex ()
  {
    بینښه صحيح  شاخص_2 = SUPER::arg_ډېری.راوباسه_uint ();
    إذا (unlikely (کتل_شوی_vsindex () || کتل_شوی_blend))
    {
     SUPER::سېټ_error ();
    }
    وإلا
    {
      سېټ_ivs (شاخص_2);
    }
    کتل_شوی_vsindex_2 = ریښتیا;
  }

  بینښه صحيح get_region_شمېر () ثابت { أرجع region_شمېر_2; }
  فراغ	 سېټ_region_شمېر (بینښه صحيح region_شمېر_3) { region_شمېر_2 = region_شمېر_3; }
  بینښه صحيح get_ivs () ثابت { أرجع ivs; }
  فراغ	 سېټ_ivs (بینښه صحيح ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  بینښه صحيح get_orig_ivs () ثابت { أرجع orig_ivs; }
  منطقي	 کتل_شوی_vsindex () ثابت { أرجع کتل_شوی_vsindex_2; }

  مزدوج blend_deltas (hb_array_t<ثابت ELEM> deltas) ثابت
  {
    مزدوج v = 0;
    إذا (do_blend)
    {
      إذا (likely (scalars && scalars->اوږدتیا == deltas.اوږدتیا))
      {
        بینښه شمېر = scalars->اوږدتیا;
	لكل (بینښه i = 0; i < شمېر; i++)
	  v += (مزدوج) scalars->arrayZ[i] * deltas.arrayZ[i].منزل_real ();
      }
    }
    أرجع v;
  }

  منطقي have_coords () ثابت { أرجع عدد_coords; }

  محمي:
  ثابت صحيح     *coords;
  بینښه صحيح  عدد_coords;
  ثابت	 CFF2_توکی_Variation_Store *varStore;
  بینښه صحيح  region_شمېر_2;
  بینښه صحيح  ivs;
  بینښه صحيح  orig_ivs;
  hb_ویکټور_t<عائم>  *scalars = تش_اشاره;
  hb_atomic_t<hb_ویکټور_t<عائم> *> *cached_scalars_ویکټور = تش_اشاره;
  منطقي	  do_blend;
  منطقي	  کتل_شوی_vsindex_2 = خطأ;
  منطقي	  کتل_شوی_blend = خطأ;

  تعريفنوع cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
کينډۍ <ډول_نوم OPSET, ډول_نوم PARAM, ډول_نوم ELEM, ډول_نوم لار_3=لار_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
بنية cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, لار_3>
{
  ساكن فراغ process_op (op_کوډ_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    بدل (op) {
      حالة Op_کوډ_callsubr:
      حالة Op_کوډ_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#إذا 0
	إذا (unlikely (env.arg_ډېری.peek ().blending ()))
	{
	  env.سېټ_error ();
	  اكسر;
	}
#د_که_پای
	SUPER::process_op (op, env, param);
	اكسر;

      حالة Op_کوډ_blendcs:
	OPSET::process_blend (env, param);
	اكسر;

      حالة Op_کوډ_vsindexcs:
#إذا 0
	إذا (unlikely (env.arg_ډېری.peek ().blending ()))
	{
	  env.سېټ_error ();
	  اكسر;
	}
#د_که_پای
	OPSET::process_vsindex (env, param);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
    }
  }

  کينډۍ <ډول_نوم T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  ساكن فراغ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 بینښه n, بینښه i)
  {
    إذا (env.have_coords ())
      arg.سېټ_int (round (arg.منزل_real () + env.blend_deltas (blends)));
    وإلا إذا (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_ویکټور_t<شمېره_t> inner = std::move (arg.deltas);
      arg.سېټ_blends (n, i, blends);
      إذا (likely (inner.اوږدتیا == arg.deltas.اوږدتیا))
	لكل (بینښه j = 0; j < inner.اوږدتیا; j++)
	  arg.deltas.arrayZ[j].سېټ_real (arg.deltas.arrayZ[j].منزل_real () +
					 inner.arrayZ[j].منزل_real ());
      وإلا
	env.سېټ_error ();
    }
    وإلا
      arg.سېټ_blends (n, i, blends);
  }
  کينډۍ <ډول_نوم T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  ساكن فراغ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 بینښه n, بینښه i)
  {
    arg.سېټ_real (arg.منزل_real () + env.blend_deltas (blends));
  }

  ساكن فراغ process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    بینښه صحيح n, k;

    env.process_blend ();
    k = env.get_region_شمېر ();
    n = env.arg_ډېری.راوباسه_uint ();
    /* copy the blend values into blend array of the default values */
    بینښه صحيح شمېر = env.arg_ډېری.get_شمېر ();
    بینښه صحيح ټول;
    إذا (unlikely (hb_unsigned_mul_overflows (k + 1, n, &ټول) || ټول > شمېر))
    {
      env.سېټ_error ();
      أرجع;
    }
    إذا (unlikely (!env.spend_budget (HB_BUDGET_1, ټول - n)))
    {
      env.سېټ_error ();
      أرجع;
    }
    بینښه صحيح پیل = شمېر - ټول;
    لكل (بینښه صحيح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت ELEM> blends = env.arg_ډېری.sub_array (پیل + n + (i * k), k);
      process_arg_blend (env, env.arg_ډېری.arrayZ[پیل + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ډېری.راوباسه (k * n);
  }

  ساكن فراغ process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_دلیلونه ();
  }

  خاص:
  تعريفنوع cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, لار_3>  SUPER;
};

کينډۍ <ډول_نوم OPSET, ډول_نوم PARAM, ډول_نوم ELEM>
کارول cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#د_که_پای /* HB_CFF2_INTERP_CS_HH */
