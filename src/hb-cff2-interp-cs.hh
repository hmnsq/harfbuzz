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
#جے_بنا_تعریف HB_CFF2_INTERP_CS_HH
#تعریف_کرو HB_CFF2_INTERP_CS_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

ورتوں نطاق OT;

بنية blend_arg_t : نمبر_t
{
  فراغ سیٹ_int (صحيح v) { reset_blends (); نمبر_t::سیٹ_int (v); }
  فراغ سیٹ_fixed (int32_t v) { reset_blends (); نمبر_t::سیٹ_fixed (v); }
  فراغ سیٹ_real (مزدوج v) { reset_blends (); نمبر_t::سیٹ_real (v); }

  فراغ سیٹ_blends (بنانشانی صحيح عدد_اقدار, بنانشانی صحيح قدر_اشاریہ,
		   hb_array_t<ثابت blend_arg_t> blends_)
  {
    عدد_اقدار_2 = عدد_اقدار;
    قدر_اشاریہ_2 = قدر_اشاریہ;
    بنانشانی عدد_Blends = blends_.لمائی;
    إذا (unlikely (!deltas.resize_exact (عدد_Blends)))
      أرجع;
    لكل (بنانشانی صحيح i = 0; i < عدد_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  منطقي blending () ثابت { أرجع deltas.لمائی > 0; }
  فراغ reset_blends ()
  {
    عدد_اقدار_2 = قدر_اشاریہ_2 = 0;
    deltas.clear ();
  }

  بنانشانی صحيح عدد_اقدار_2;
  بنانشانی صحيح قدر_اشاریہ_2;
  hb_سمتیہ_t<نمبر_t> deltas;
};

تعريفنوع biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

سانچا <قسم_ناں ELEM>
بنية cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  سانچا <قسم_ناں ACC>
  cff2_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, بنانشانی صحيح fd,
			ثابت صحيح *coords_=خالی_سوچک, بنانشانی صحيح عدد_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.نجی_Dicts[fd].localSubrs),
      region_گنتی_2 (0), cached_scalars_سمتیہ (&acc.cached_scalars_سمتیہ)
  {
    coords = coords_;
    عدد_coords = عدد_coords_2;
    varStore = acc.varStore;
    do_blend = عدد_coords && varStore->ناپ;
    orig_ivs = acc.نجی_Dicts[fd].ivs;
    سیٹ_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_سمتیہ (scalars);
  }

  hb_سمتیہ_t<عائم> *acquire_scalars_سمتیہ () ثابت
  {
    hb_سمتیہ_t<عائم> *scalars = cached_scalars_سمتیہ->get_acquire ();

    إذا (!scalars || !cached_scalars_سمتیہ->cmpexch (scalars, خالی_سوچک))
    {
      scalars = (hb_سمتیہ_t<عائم> *) hb_calloc (1, حجم (hb_سمتیہ_t<عائم>));
      إذا (unlikely (!scalars))
	أرجع خالی_سوچک;
      scalars->init ();
    }

    أرجع scalars;
  }

  فراغ release_scalars_سمتیہ (hb_سمتیہ_t<عائم> *scalars) ثابت
  {
    إذا (!scalars)
      أرجع;

    scalars->clear ();

    إذا (!cached_scalars_سمتیہ->cmpexch (خالی_سوچک, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = خالی_سوچک;
  }

  op_کوڈ_t fetch_op ()
  {
    إذا (هذا->str_ref.avail ())
      أرجع SUPER::fetch_op ();

    /* make up return or endchar op */
    إذا (هذا->call_ڈھیر.is_empty ())
      أرجع Op_کوڈ_endchar;
    وإلا
      أرجع Op_کوڈ_return;
  }

  ثابت ELEM& eval_arg (بنانشانی صحيح i)
  {
    أرجع SUPER::arg_ڈھیر[i];
  }

  ثابت ELEM& نکالو_arg ()
  {
    أرجع SUPER::arg_ڈھیر.نکالو ();
  }

  فراغ process_blend ()
  {
    إذا (!تکیا_blend)
    {
      scalars = acquire_scalars_سمتیہ ();
      إذا (unlikely (!scalars))
	SUPER::سیٹ_error ();
      وإلا
      {
	region_گنتی_2 = varStore->varStore.get_region_اشاریہ_گنتی (get_ivs ());
	إذا (do_blend)
	{
	  إذا (unlikely (!scalars->resize_exact (region_گنتی_2)))
	    SUPER::سیٹ_error ();
	  وإلا
	    varStore->varStore.get_region_scalars (get_ivs (), coords, عدد_coords,
						   &(*scalars)[0], region_گنتی_2);
	}
      }
      تکیا_blend = سچ;
    }
  }

  فراغ process_vsindex ()
  {
    بنانشانی صحيح  اشاریہ_2 = SUPER::arg_ڈھیر.نکالو_uint ();
    إذا (unlikely (تکیا_vsindex () || تکیا_blend))
    {
     SUPER::سیٹ_error ();
    }
    وإلا
    {
      سیٹ_ivs (اشاریہ_2);
    }
    تکیا_vsindex_2 = سچ;
  }

  بنانشانی صحيح get_region_گنتی () ثابت { أرجع region_گنتی_2; }
  فراغ	 سیٹ_region_گنتی (بنانشانی صحيح region_گنتی_3) { region_گنتی_2 = region_گنتی_3; }
  بنانشانی صحيح get_ivs () ثابت { أرجع ivs; }
  فراغ	 سیٹ_ivs (بنانشانی صحيح ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  بنانشانی صحيح get_orig_ivs () ثابت { أرجع orig_ivs; }
  منطقي	 تکیا_vsindex () ثابت { أرجع تکیا_vsindex_2; }

  مزدوج blend_deltas (hb_array_t<ثابت ELEM> deltas) ثابت
  {
    مزدوج v = 0;
    إذا (do_blend)
    {
      إذا (likely (scalars && scalars->لمائی == deltas.لمائی))
      {
        بنانشانی گنتی = scalars->لمائی;
	لكل (بنانشانی i = 0; i < گنتی; i++)
	  v += (مزدوج) scalars->arrayZ[i] * deltas.arrayZ[i].منزل_real ();
      }
    }
    أرجع v;
  }

  منطقي have_coords () ثابت { أرجع عدد_coords; }

  محمي:
  ثابت صحيح     *coords;
  بنانشانی صحيح  عدد_coords;
  ثابت	 CFF2_شے_Variation_Store *varStore;
  بنانشانی صحيح  region_گنتی_2;
  بنانشانی صحيح  ivs;
  بنانشانی صحيح  orig_ivs;
  hb_سمتیہ_t<عائم>  *scalars = خالی_سوچک;
  hb_atomic_t<hb_سمتیہ_t<عائم> *> *cached_scalars_سمتیہ = خالی_سوچک;
  منطقي	  do_blend;
  منطقي	  تکیا_vsindex_2 = خطأ;
  منطقي	  تکیا_blend = خطأ;

  تعريفنوع cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
سانچا <قسم_ناں OPSET, قسم_ناں PARAM, قسم_ناں ELEM, قسم_ناں رستہ_3=رستہ_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
بنية cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, رستہ_3>
{
  ساكن فراغ process_op (op_کوڈ_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    بدل (op) {
      حالة Op_کوڈ_callsubr:
      حالة Op_کوڈ_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#إذا 0
	إذا (unlikely (env.arg_ڈھیر.peek ().blending ()))
	{
	  env.سیٹ_error ();
	  اكسر;
	}
#جے_انت
	SUPER::process_op (op, env, param);
	اكسر;

      حالة Op_کوڈ_blendcs:
	OPSET::process_blend (env, param);
	اكسر;

      حالة Op_کوڈ_vsindexcs:
#إذا 0
	إذا (unlikely (env.arg_ڈھیر.peek ().blending ()))
	{
	  env.سیٹ_error ();
	  اكسر;
	}
#جے_انت
	OPSET::process_vsindex (env, param);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
    }
  }

  سانچا <قسم_ناں T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  ساكن فراغ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 بنانشانی n, بنانشانی i)
  {
    إذا (env.have_coords ())
      arg.سیٹ_int (round (arg.منزل_real () + env.blend_deltas (blends)));
    وإلا إذا (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_سمتیہ_t<نمبر_t> inner = std::move (arg.deltas);
      arg.سیٹ_blends (n, i, blends);
      إذا (likely (inner.لمائی == arg.deltas.لمائی))
	لكل (بنانشانی j = 0; j < inner.لمائی; j++)
	  arg.deltas.arrayZ[j].سیٹ_real (arg.deltas.arrayZ[j].منزل_real () +
					 inner.arrayZ[j].منزل_real ());
      وإلا
	env.سیٹ_error ();
    }
    وإلا
      arg.سیٹ_blends (n, i, blends);
  }
  سانچا <قسم_ناں T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  ساكن فراغ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 بنانشانی n, بنانشانی i)
  {
    arg.سیٹ_real (arg.منزل_real () + env.blend_deltas (blends));
  }

  ساكن فراغ process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    بنانشانی صحيح n, k;

    env.process_blend ();
    k = env.get_region_گنتی ();
    n = env.arg_ڈھیر.نکالو_uint ();
    /* copy the blend values into blend array of the default values */
    بنانشانی صحيح گنتی = env.arg_ڈھیر.get_گنتی ();
    بنانشانی صحيح کل;
    إذا (unlikely (hb_unsigned_mul_overflows (k + 1, n, &کل) || کل > گنتی))
    {
      env.سیٹ_error ();
      أرجع;
    }
    إذا (unlikely (!env.spend_budget (HB_BUDGET_1, کل - n)))
    {
      env.سیٹ_error ();
      أرجع;
    }
    بنانشانی صحيح شروع = گنتی - کل;
    لكل (بنانشانی صحيح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت ELEM> blends = env.arg_ڈھیر.sub_array (شروع + n + (i * k), k);
      process_arg_blend (env, env.arg_ڈھیر.arrayZ[شروع + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ڈھیر.نکالو (k * n);
  }

  ساكن فراغ process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_دلیلاں ();
  }

  خاص:
  تعريفنوع cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, رستہ_3>  SUPER;
};

سانچا <قسم_ناں OPSET, قسم_ناں PARAM, قسم_ناں ELEM>
ورتوں cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#جے_انت /* HB_CFF2_INTERP_CS_HH */
