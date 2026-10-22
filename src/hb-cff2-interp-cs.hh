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
#اگر_تعریف_نشده HB_CFF2_INTERP_CS_HH
#تعریف_کن HB_CFF2_INTERP_CS_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

استفاده نطاق OT;

بنية blend_arg_t : عدد_t
{
  فراغ مجموعه_int (صحيح v) { reset_blends (); عدد_t::مجموعه_int (v); }
  فراغ مجموعه_fixed (int32_t v) { reset_blends (); عدد_t::مجموعه_fixed (v); }
  فراغ مجموعه_real (مزدوج v) { reset_blends (); عدد_t::مجموعه_real (v); }

  فراغ مجموعه_blends (بیعلامت صحيح رقم_مقادیر, بیعلامت صحيح مقدار_شاخص,
		   hb_array_t<ثابت blend_arg_t> blends_)
  {
    رقم_مقادیر_2 = رقم_مقادیر;
    مقدار_شاخص_2 = مقدار_شاخص;
    بیعلامت رقم_Blends = blends_.درازی;
    إذا (unlikely (!deltas.resize_exact (رقم_Blends)))
      أرجع;
    لكل (بیعلامت صحيح i = 0; i < رقم_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  منطقي blending () ثابت { أرجع deltas.درازی > 0; }
  فراغ reset_blends ()
  {
    رقم_مقادیر_2 = مقدار_شاخص_2 = 0;
    deltas.clear ();
  }

  بیعلامت صحيح رقم_مقادیر_2;
  بیعلامت صحيح مقدار_شاخص_2;
  hb_وکتور_t<عدد_t> deltas;
};

تعريفنوع biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

قالب <نام_نوع ELEM>
بنية cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  قالب <نام_نوع ACC>
  cff2_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, بیعلامت صحيح fd,
			ثابت صحيح *coords_=اشاره_تهی, بیعلامت صحيح رقم_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.خصوصی_Dicts[fd].localSubrs),
      region_شمار_2 (0), cached_scalars_وکتور (&acc.cached_scalars_وکتور)
  {
    coords = coords_;
    رقم_coords = رقم_coords_2;
    varStore = acc.varStore;
    do_blend = رقم_coords && varStore->اندازه;
    orig_ivs = acc.خصوصی_Dicts[fd].ivs;
    مجموعه_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_وکتور (scalars);
  }

  hb_وکتور_t<عائم> *acquire_scalars_وکتور () ثابت
  {
    hb_وکتور_t<عائم> *scalars = cached_scalars_وکتور->get_acquire ();

    إذا (!scalars || !cached_scalars_وکتور->cmpexch (scalars, اشاره_تهی))
    {
      scalars = (hb_وکتور_t<عائم> *) hb_calloc (1, حجم (hb_وکتور_t<عائم>));
      إذا (unlikely (!scalars))
	أرجع اشاره_تهی;
      scalars->init ();
    }

    أرجع scalars;
  }

  فراغ release_scalars_وکتور (hb_وکتور_t<عائم> *scalars) ثابت
  {
    إذا (!scalars)
      أرجع;

    scalars->clear ();

    إذا (!cached_scalars_وکتور->cmpexch (اشاره_تهی, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = اشاره_تهی;
  }

  op_کد_t fetch_op ()
  {
    إذا (هذا->str_ref.avail ())
      أرجع SUPER::fetch_op ();

    /* make up return or endchar op */
    إذا (هذا->call_پشته.is_empty ())
      أرجع Op_کد_endchar;
    وإلا
      أرجع Op_کد_return;
  }

  ثابت ELEM& eval_arg (بیعلامت صحيح i)
  {
    أرجع SUPER::arg_پشته[i];
  }

  ثابت ELEM& برداشت_arg ()
  {
    أرجع SUPER::arg_پشته.برداشت ();
  }

  فراغ process_blend ()
  {
    إذا (!دیده_blend)
    {
      scalars = acquire_scalars_وکتور ();
      إذا (unlikely (!scalars))
	SUPER::مجموعه_error ();
      وإلا
      {
	region_شمار_2 = varStore->varStore.get_region_شاخص_شمار (get_ivs ());
	إذا (do_blend)
	{
	  إذا (unlikely (!scalars->resize_exact (region_شمار_2)))
	    SUPER::مجموعه_error ();
	  وإلا
	    varStore->varStore.get_region_scalars (get_ivs (), coords, رقم_coords,
						   &(*scalars)[0], region_شمار_2);
	}
      }
      دیده_blend = درست;
    }
  }

  فراغ process_vsindex ()
  {
    بیعلامت صحيح  شاخص_2 = SUPER::arg_پشته.برداشت_uint ();
    إذا (unlikely (دیده_vsindex () || دیده_blend))
    {
     SUPER::مجموعه_error ();
    }
    وإلا
    {
      مجموعه_ivs (شاخص_2);
    }
    دیده_vsindex_2 = درست;
  }

  بیعلامت صحيح get_region_شمار () ثابت { أرجع region_شمار_2; }
  فراغ	 مجموعه_region_شمار (بیعلامت صحيح region_شمار_3) { region_شمار_2 = region_شمار_3; }
  بیعلامت صحيح get_ivs () ثابت { أرجع ivs; }
  فراغ	 مجموعه_ivs (بیعلامت صحيح ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  بیعلامت صحيح get_orig_ivs () ثابت { أرجع orig_ivs; }
  منطقي	 دیده_vsindex () ثابت { أرجع دیده_vsindex_2; }

  مزدوج blend_deltas (hb_array_t<ثابت ELEM> deltas) ثابت
  {
    مزدوج v = 0;
    إذا (do_blend)
    {
      إذا (likely (scalars && scalars->درازی == deltas.درازی))
      {
        بیعلامت شمار = scalars->درازی;
	لكل (بیعلامت i = 0; i < شمار; i++)
	  v += (مزدوج) scalars->arrayZ[i] * deltas.arrayZ[i].مقصد_real ();
      }
    }
    أرجع v;
  }

  منطقي have_coords () ثابت { أرجع رقم_coords; }

  محمي:
  ثابت صحيح     *coords;
  بیعلامت صحيح  رقم_coords;
  ثابت	 CFF2_قلم_Variation_Store *varStore;
  بیعلامت صحيح  region_شمار_2;
  بیعلامت صحيح  ivs;
  بیعلامت صحيح  orig_ivs;
  hb_وکتور_t<عائم>  *scalars = اشاره_تهی;
  hb_atomic_t<hb_وکتور_t<عائم> *> *cached_scalars_وکتور = اشاره_تهی;
  منطقي	  do_blend;
  منطقي	  دیده_vsindex_2 = خطأ;
  منطقي	  دیده_blend = خطأ;

  تعريفنوع cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
قالب <نام_نوع OPSET, نام_نوع PARAM, نام_نوع ELEM, نام_نوع مسیر_3=مسیر_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
بنية cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, مسیر_3>
{
  ساكن فراغ process_op (op_کد_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    بدل (op) {
      حالة Op_کد_callsubr:
      حالة Op_کد_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#إذا 0
	إذا (unlikely (env.arg_پشته.peek ().blending ()))
	{
	  env.مجموعه_error ();
	  اكسر;
	}
#پایان_اگر
	SUPER::process_op (op, env, param);
	اكسر;

      حالة Op_کد_blendcs:
	OPSET::process_blend (env, param);
	اكسر;

      حالة Op_کد_vsindexcs:
#إذا 0
	إذا (unlikely (env.arg_پشته.peek ().blending ()))
	{
	  env.مجموعه_error ();
	  اكسر;
	}
#پایان_اگر
	OPSET::process_vsindex (env, param);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
    }
  }

  قالب <نام_نوع T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  ساكن فراغ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 بیعلامت n, بیعلامت i)
  {
    إذا (env.have_coords ())
      arg.مجموعه_int (round (arg.مقصد_real () + env.blend_deltas (blends)));
    وإلا إذا (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_وکتور_t<عدد_t> inner = std::move (arg.deltas);
      arg.مجموعه_blends (n, i, blends);
      إذا (likely (inner.درازی == arg.deltas.درازی))
	لكل (بیعلامت j = 0; j < inner.درازی; j++)
	  arg.deltas.arrayZ[j].مجموعه_real (arg.deltas.arrayZ[j].مقصد_real () +
					 inner.arrayZ[j].مقصد_real ());
      وإلا
	env.مجموعه_error ();
    }
    وإلا
      arg.مجموعه_blends (n, i, blends);
  }
  قالب <نام_نوع T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  ساكن فراغ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 بیعلامت n, بیعلامت i)
  {
    arg.مجموعه_real (arg.مقصد_real () + env.blend_deltas (blends));
  }

  ساكن فراغ process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    بیعلامت صحيح n, k;

    env.process_blend ();
    k = env.get_region_شمار ();
    n = env.arg_پشته.برداشت_uint ();
    /* copy the blend values into blend array of the default values */
    بیعلامت صحيح شمار = env.arg_پشته.get_شمار ();
    بیعلامت صحيح مجموع;
    إذا (unlikely (hb_unsigned_mul_overflows (k + 1, n, &مجموع) || مجموع > شمار))
    {
      env.مجموعه_error ();
      أرجع;
    }
    إذا (unlikely (!env.spend_budget (HB_BUDGET_1, مجموع - n)))
    {
      env.مجموعه_error ();
      أرجع;
    }
    بیعلامت صحيح شروع = شمار - مجموع;
    لكل (بیعلامت صحيح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت ELEM> blends = env.arg_پشته.sub_array (شروع + n + (i * k), k);
      process_arg_blend (env, env.arg_پشته.arrayZ[شروع + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_پشته.برداشت (k * n);
  }

  ساكن فراغ process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_آرگومانها ();
  }

  خاص:
  تعريفنوع cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, مسیر_3>  SUPER;
};

قالب <نام_نوع OPSET, نام_نوع PARAM, نام_نوع ELEM>
استفاده cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#پایان_اگر /* HB_CFF2_INTERP_CS_HH */
