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
#جيڪڏهن_وصف_نه_ٿيل HB_CFF2_INTERP_CS_HH
#وصف_ڪريو HB_CFF2_INTERP_CS_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

استعمال نطاق OT;

بنية blend_arg_t : عدد_t
{
  فراغ سيٽ_int (صحيح v) { reset_blends (); عدد_t::سيٽ_int (v); }
  فراغ سيٽ_fixed (int32_t v) { reset_blends (); عدد_t::سيٽ_fixed (v); }
  فراغ سيٽ_real (مزدوج v) { reset_blends (); عدد_t::سيٽ_real (v); }

  فراغ سيٽ_blends (بنانشان صحيح انگ_قدرون, بنانشان صحيح قدر_اشاريو,
		   hb_array_t<ثابت blend_arg_t> blends_)
  {
    انگ_قدرون_2 = انگ_قدرون;
    قدر_اشاريو_2 = قدر_اشاريو;
    بنانشان انگ_Blends = blends_.ڊيگهائي;
    إذا (unlikely (!deltas.resize_exact (انگ_Blends)))
      أرجع;
    لكل (بنانشان صحيح i = 0; i < انگ_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  منطقي blending () ثابت { أرجع deltas.ڊيگهائي > 0; }
  فراغ reset_blends ()
  {
    انگ_قدرون_2 = قدر_اشاريو_2 = 0;
    deltas.clear ();
  }

  بنانشان صحيح انگ_قدرون_2;
  بنانشان صحيح قدر_اشاريو_2;
  hb_ويڪٽر_t<عدد_t> deltas;
};

تعريفنوع biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

سانچو <قسم_نالو ELEM>
بنية cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  سانچو <قسم_نالو ACC>
  cff2_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, بنانشان صحيح fd,
			ثابت صحيح *coords_=خالي_اشارو, بنانشان صحيح انگ_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.ذاتي_Dicts[fd].localSubrs),
      region_ڳڻپ_2 (0), cached_scalars_ويڪٽر (&acc.cached_scalars_ويڪٽر)
  {
    coords = coords_;
    انگ_coords = انگ_coords_2;
    varStore = acc.varStore;
    do_blend = انگ_coords && varStore->سائيز;
    orig_ivs = acc.ذاتي_Dicts[fd].ivs;
    سيٽ_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_ويڪٽر (scalars);
  }

  hb_ويڪٽر_t<عائم> *acquire_scalars_ويڪٽر () ثابت
  {
    hb_ويڪٽر_t<عائم> *scalars = cached_scalars_ويڪٽر->get_acquire ();

    إذا (!scalars || !cached_scalars_ويڪٽر->cmpexch (scalars, خالي_اشارو))
    {
      scalars = (hb_ويڪٽر_t<عائم> *) hb_calloc (1, حجم (hb_ويڪٽر_t<عائم>));
      إذا (unlikely (!scalars))
	أرجع خالي_اشارو;
      scalars->init ();
    }

    أرجع scalars;
  }

  فراغ release_scalars_ويڪٽر (hb_ويڪٽر_t<عائم> *scalars) ثابت
  {
    إذا (!scalars)
      أرجع;

    scalars->clear ();

    إذا (!cached_scalars_ويڪٽر->cmpexch (خالي_اشارو, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = خالي_اشارو;
  }

  op_ڪوڊ_t fetch_op ()
  {
    إذا (هذا->str_ref.avail ())
      أرجع SUPER::fetch_op ();

    /* make up return or endchar op */
    إذا (هذا->call_ڍير.is_empty ())
      أرجع Op_ڪوڊ_endchar;
    وإلا
      أرجع Op_ڪوڊ_return;
  }

  ثابت ELEM& eval_arg (بنانشان صحيح i)
  {
    أرجع SUPER::arg_ڍير[i];
  }

  ثابت ELEM& ڪڍو_arg ()
  {
    أرجع SUPER::arg_ڍير.ڪڍو ();
  }

  فراغ process_blend ()
  {
    إذا (!ڏٺل_blend)
    {
      scalars = acquire_scalars_ويڪٽر ();
      إذا (unlikely (!scalars))
	SUPER::سيٽ_error ();
      وإلا
      {
	region_ڳڻپ_2 = varStore->varStore.get_region_اشاريو_ڳڻپ (get_ivs ());
	إذا (do_blend)
	{
	  إذا (unlikely (!scalars->resize_exact (region_ڳڻپ_2)))
	    SUPER::سيٽ_error ();
	  وإلا
	    varStore->varStore.get_region_scalars (get_ivs (), coords, انگ_coords,
						   &(*scalars)[0], region_ڳڻپ_2);
	}
      }
      ڏٺل_blend = سچ;
    }
  }

  فراغ process_vsindex ()
  {
    بنانشان صحيح  اشاريو_2 = SUPER::arg_ڍير.ڪڍو_uint ();
    إذا (unlikely (ڏٺل_vsindex () || ڏٺل_blend))
    {
     SUPER::سيٽ_error ();
    }
    وإلا
    {
      سيٽ_ivs (اشاريو_2);
    }
    ڏٺل_vsindex_2 = سچ;
  }

  بنانشان صحيح get_region_ڳڻپ () ثابت { أرجع region_ڳڻپ_2; }
  فراغ	 سيٽ_region_ڳڻپ (بنانشان صحيح region_ڳڻپ_3) { region_ڳڻپ_2 = region_ڳڻپ_3; }
  بنانشان صحيح get_ivs () ثابت { أرجع ivs; }
  فراغ	 سيٽ_ivs (بنانشان صحيح ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  بنانشان صحيح get_orig_ivs () ثابت { أرجع orig_ivs; }
  منطقي	 ڏٺل_vsindex () ثابت { أرجع ڏٺل_vsindex_2; }

  مزدوج blend_deltas (hb_array_t<ثابت ELEM> deltas) ثابت
  {
    مزدوج v = 0;
    إذا (do_blend)
    {
      إذا (likely (scalars && scalars->ڊيگهائي == deltas.ڊيگهائي))
      {
        بنانشان ڳڻپ = scalars->ڊيگهائي;
	لكل (بنانشان i = 0; i < ڳڻپ; i++)
	  v += (مزدوج) scalars->arrayZ[i] * deltas.arrayZ[i].منزل_real ();
      }
    }
    أرجع v;
  }

  منطقي have_coords () ثابت { أرجع انگ_coords; }

  محمي:
  ثابت صحيح     *coords;
  بنانشان صحيح  انگ_coords;
  ثابت	 CFF2_شي_Variation_Store *varStore;
  بنانشان صحيح  region_ڳڻپ_2;
  بنانشان صحيح  ivs;
  بنانشان صحيح  orig_ivs;
  hb_ويڪٽر_t<عائم>  *scalars = خالي_اشارو;
  hb_atomic_t<hb_ويڪٽر_t<عائم> *> *cached_scalars_ويڪٽر = خالي_اشارو;
  منطقي	  do_blend;
  منطقي	  ڏٺل_vsindex_2 = خطأ;
  منطقي	  ڏٺل_blend = خطأ;

  تعريفنوع cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
سانچو <قسم_نالو OPSET, قسم_نالو PARAM, قسم_نالو ELEM, قسم_نالو رستو_3=رستو_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
بنية cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, رستو_3>
{
  ساكن فراغ process_op (op_ڪوڊ_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    بدل (op) {
      حالة Op_ڪوڊ_callsubr:
      حالة Op_ڪوڊ_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#إذا 0
	إذا (unlikely (env.arg_ڍير.peek ().blending ()))
	{
	  env.سيٽ_error ();
	  اكسر;
	}
#جيڪڏهن_پورو
	SUPER::process_op (op, env, param);
	اكسر;

      حالة Op_ڪوڊ_blendcs:
	OPSET::process_blend (env, param);
	اكسر;

      حالة Op_ڪوڊ_vsindexcs:
#إذا 0
	إذا (unlikely (env.arg_ڍير.peek ().blending ()))
	{
	  env.سيٽ_error ();
	  اكسر;
	}
#جيڪڏهن_پورو
	OPSET::process_vsindex (env, param);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
    }
  }

  سانچو <قسم_نالو T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  ساكن فراغ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 بنانشان n, بنانشان i)
  {
    إذا (env.have_coords ())
      arg.سيٽ_int (round (arg.منزل_real () + env.blend_deltas (blends)));
    وإلا إذا (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_ويڪٽر_t<عدد_t> inner = std::move (arg.deltas);
      arg.سيٽ_blends (n, i, blends);
      إذا (likely (inner.ڊيگهائي == arg.deltas.ڊيگهائي))
	لكل (بنانشان j = 0; j < inner.ڊيگهائي; j++)
	  arg.deltas.arrayZ[j].سيٽ_real (arg.deltas.arrayZ[j].منزل_real () +
					 inner.arrayZ[j].منزل_real ());
      وإلا
	env.سيٽ_error ();
    }
    وإلا
      arg.سيٽ_blends (n, i, blends);
  }
  سانچو <قسم_نالو T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  ساكن فراغ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 بنانشان n, بنانشان i)
  {
    arg.سيٽ_real (arg.منزل_real () + env.blend_deltas (blends));
  }

  ساكن فراغ process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    بنانشان صحيح n, k;

    env.process_blend ();
    k = env.get_region_ڳڻپ ();
    n = env.arg_ڍير.ڪڍو_uint ();
    /* copy the blend values into blend array of the default values */
    بنانشان صحيح ڳڻپ = env.arg_ڍير.get_ڳڻپ ();
    بنانشان صحيح ڪل;
    إذا (unlikely (hb_unsigned_mul_overflows (k + 1, n, &ڪل) || ڪل > ڳڻپ))
    {
      env.سيٽ_error ();
      أرجع;
    }
    إذا (unlikely (!env.spend_budget (HB_BUDGET_1, ڪل - n)))
    {
      env.سيٽ_error ();
      أرجع;
    }
    بنانشان صحيح شروعات = ڳڻپ - ڪل;
    لكل (بنانشان صحيح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت ELEM> blends = env.arg_ڍير.sub_array (شروعات + n + (i * k), k);
      process_arg_blend (env, env.arg_ڍير.arrayZ[شروعات + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ڍير.ڪڍو (k * n);
  }

  ساكن فراغ process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_دليلون ();
  }

  خاص:
  تعريفنوع cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, رستو_3>  SUPER;
};

سانچو <قسم_نالو OPSET, قسم_نالو PARAM, قسم_نالو ELEM>
استعمال cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#جيڪڏهن_پورو /* HB_CFF2_INTERP_CS_HH */
