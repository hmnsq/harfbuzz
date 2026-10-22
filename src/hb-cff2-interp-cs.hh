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
#اگر_غیر_تعریف_شدہ HB_CFF2_INTERP_CS_HH
#تعریف_کرو HB_CFF2_INTERP_CS_HH

#شامل "hb.hh"
#شامل "hb-cff-interp-cs-common.hh"

نامخلا CFF {

استعمال نامخلا OT;

ساخت blend_arg_t : نمبر_t
{
  خالی سیٹ_int (صحیح v) { reset_blends (); نمبر_t::سیٹ_int (v); }
  خالی سیٹ_fixed (int32_t v) { reset_blends (); نمبر_t::سیٹ_fixed (v); }
  خالی سیٹ_real (دوہرا v) { reset_blends (); نمبر_t::سیٹ_real (v); }

  خالی سیٹ_blends (غیرعلامتی صحیح عدد_اقدار, غیرعلامتی صحیح قدر_اشاریہ,
		   hb_array_t<ثابت blend_arg_t> blends_)
  {
    عدد_اقدار_2 = عدد_اقدار;
    قدر_اشاریہ_2 = قدر_اشاریہ;
    غیرعلامتی عدد_Blends = blends_.لمبائی;
    اگر (unlikely (!deltas.resize_exact (عدد_Blends)))
      واپس;
    برائے (غیرعلامتی صحیح i = 0; i < عدد_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  منطقی blending () ثابت { واپس deltas.لمبائی > 0; }
  خالی reset_blends ()
  {
    عدد_اقدار_2 = قدر_اشاریہ_2 = 0;
    deltas.clear ();
  }

  غیرعلامتی صحیح عدد_اقدار_2;
  غیرعلامتی صحیح قدر_اشاریہ_2;
  hb_سمتیہ_t<نمبر_t> deltas;
};

قسمتعریف biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

سانچہ <قسم_نام ELEM>
ساخت cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  سانچہ <قسم_نام ACC>
  cff2_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, غیرعلامتی صحیح fd,
			ثابت صحیح *coords_=خالی_اشاریہ, غیرعلامتی صحیح عدد_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.نجی_Dicts[fd].localSubrs),
      region_گنتی_2 (0), cached_scalars_سمتیہ (&acc.cached_scalars_سمتیہ)
  {
    coords = coords_;
    عدد_coords = عدد_coords_2;
    varStore = acc.varStore;
    do_blend = عدد_coords && varStore->سائز;
    orig_ivs = acc.نجی_Dicts[fd].ivs;
    سیٹ_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_سمتیہ (scalars);
  }

  hb_سمتیہ_t<اعشاری> *acquire_scalars_سمتیہ () ثابت
  {
    hb_سمتیہ_t<اعشاری> *scalars = cached_scalars_سمتیہ->get_acquire ();

    اگر (!scalars || !cached_scalars_سمتیہ->cmpexch (scalars, خالی_اشاریہ))
    {
      scalars = (hb_سمتیہ_t<اعشاری> *) hb_calloc (1, حجم (hb_سمتیہ_t<اعشاری>));
      اگر (unlikely (!scalars))
	واپس خالی_اشاریہ;
      scalars->init ();
    }

    واپس scalars;
  }

  خالی release_scalars_سمتیہ (hb_سمتیہ_t<اعشاری> *scalars) ثابت
  {
    اگر (!scalars)
      واپس;

    scalars->clear ();

    اگر (!cached_scalars_سمتیہ->cmpexch (خالی_اشاریہ, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = خالی_اشاریہ;
  }

  op_رمز_t fetch_op ()
  {
    اگر (یہ->str_ref.avail ())
      واپس SUPER::fetch_op ();

    /* make up return or endchar op */
    اگر (یہ->call_ڈھیر.is_empty ())
      واپس Op_رمز_endchar;
    ورنہ
      واپس Op_رمز_return;
  }

  ثابت ELEM& eval_arg (غیرعلامتی صحیح i)
  {
    واپس SUPER::arg_ڈھیر[i];
  }

  ثابت ELEM& نکالو_arg ()
  {
    واپس SUPER::arg_ڈھیر.نکالو ();
  }

  خالی process_blend ()
  {
    اگر (!نظر_آیا_blend)
    {
      scalars = acquire_scalars_سمتیہ ();
      اگر (unlikely (!scalars))
	SUPER::سیٹ_error ();
      ورنہ
      {
	region_گنتی_2 = varStore->varStore.get_region_اشاریہ_گنتی (get_ivs ());
	اگر (do_blend)
	{
	  اگر (unlikely (!scalars->resize_exact (region_گنتی_2)))
	    SUPER::سیٹ_error ();
	  ورنہ
	    varStore->varStore.get_region_scalars (get_ivs (), coords, عدد_coords,
						   &(*scalars)[0], region_گنتی_2);
	}
      }
      نظر_آیا_blend = سچ;
    }
  }

  خالی process_vsindex ()
  {
    غیرعلامتی صحیح  اشاریہ_2 = SUPER::arg_ڈھیر.نکالو_uint ();
    اگر (unlikely (نظر_آیا_vsindex () || نظر_آیا_blend))
    {
     SUPER::سیٹ_error ();
    }
    ورنہ
    {
      سیٹ_ivs (اشاریہ_2);
    }
    نظر_آیا_vsindex_2 = سچ;
  }

  غیرعلامتی صحیح get_region_گنتی () ثابت { واپس region_گنتی_2; }
  خالی	 سیٹ_region_گنتی (غیرعلامتی صحیح region_گنتی_3) { region_گنتی_2 = region_گنتی_3; }
  غیرعلامتی صحیح get_ivs () ثابت { واپس ivs; }
  خالی	 سیٹ_ivs (غیرعلامتی صحیح ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  غیرعلامتی صحیح get_orig_ivs () ثابت { واپس orig_ivs; }
  منطقی	 نظر_آیا_vsindex () ثابت { واپس نظر_آیا_vsindex_2; }

  دوہرا blend_deltas (hb_array_t<ثابت ELEM> deltas) ثابت
  {
    دوہرا v = 0;
    اگر (do_blend)
    {
      اگر (likely (scalars && scalars->لمبائی == deltas.لمبائی))
      {
        غیرعلامتی گنتی = scalars->لمبائی;
	برائے (غیرعلامتی i = 0; i < گنتی; i++)
	  v += (دوہرا) scalars->arrayZ[i] * deltas.arrayZ[i].مقصد_real ();
      }
    }
    واپس v;
  }

  منطقی have_coords () ثابت { واپس عدد_coords; }

  محفوظ:
  ثابت صحیح     *coords;
  غیرعلامتی صحیح  عدد_coords;
  ثابت	 CFF2_شے_Variation_Store *varStore;
  غیرعلامتی صحیح  region_گنتی_2;
  غیرعلامتی صحیح  ivs;
  غیرعلامتی صحیح  orig_ivs;
  hb_سمتیہ_t<اعشاری>  *scalars = خالی_اشاریہ;
  hb_atomic_t<hb_سمتیہ_t<اعشاری> *> *cached_scalars_سمتیہ = خالی_اشاریہ;
  منطقی	  do_blend;
  منطقی	  نظر_آیا_vsindex_2 = جھوٹ;
  منطقی	  نظر_آیا_blend = جھوٹ;

  قسمتعریف cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
سانچہ <قسم_نام OPSET, قسم_نام PARAM, قسم_نام ELEM, قسم_نام راستہ_3=راستہ_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
ساخت cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, راستہ_3>
{
  ساکن خالی process_op (op_رمز_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    انتخاب (op) {
      حالت Op_رمز_callsubr:
      حالت Op_رمز_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#اگر 0
	اگر (unlikely (env.arg_ڈھیر.peek ().blending ()))
	{
	  env.سیٹ_error ();
	  توڑو;
	}
#اگر_ختم
	SUPER::process_op (op, env, param);
	توڑو;

      حالت Op_رمز_blendcs:
	OPSET::process_blend (env, param);
	توڑو;

      حالت Op_رمز_vsindexcs:
#اگر 0
	اگر (unlikely (env.arg_ڈھیر.peek ().blending ()))
	{
	  env.سیٹ_error ();
	  توڑو;
	}
#اگر_ختم
	OPSET::process_vsindex (env, param);
	توڑو;

      طےشدہ:
	SUPER::process_op (op, env, param);
    }
  }

  سانچہ <قسم_نام T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  ساکن خالی process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 غیرعلامتی n, غیرعلامتی i)
  {
    اگر (env.have_coords ())
      arg.سیٹ_int (round (arg.مقصد_real () + env.blend_deltas (blends)));
    ورنہ اگر (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_سمتیہ_t<نمبر_t> inner = std::move (arg.deltas);
      arg.سیٹ_blends (n, i, blends);
      اگر (likely (inner.لمبائی == arg.deltas.لمبائی))
	برائے (غیرعلامتی j = 0; j < inner.لمبائی; j++)
	  arg.deltas.arrayZ[j].سیٹ_real (arg.deltas.arrayZ[j].مقصد_real () +
					 inner.arrayZ[j].مقصد_real ());
      ورنہ
	env.سیٹ_error ();
    }
    ورنہ
      arg.سیٹ_blends (n, i, blends);
  }
  سانچہ <قسم_نام T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  ساکن خالی process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 غیرعلامتی n, غیرعلامتی i)
  {
    arg.سیٹ_real (arg.مقصد_real () + env.blend_deltas (blends));
  }

  ساکن خالی process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    غیرعلامتی صحیح n, k;

    env.process_blend ();
    k = env.get_region_گنتی ();
    n = env.arg_ڈھیر.نکالو_uint ();
    /* copy the blend values into blend array of the default values */
    غیرعلامتی صحیح گنتی = env.arg_ڈھیر.get_گنتی ();
    غیرعلامتی صحیح کل;
    اگر (unlikely (hb_unsigned_mul_overflows (k + 1, n, &کل) || کل > گنتی))
    {
      env.سیٹ_error ();
      واپس;
    }
    اگر (unlikely (!env.spend_budget (HB_BUDGET_1, کل - n)))
    {
      env.سیٹ_error ();
      واپس;
    }
    غیرعلامتی صحیح آغاز = گنتی - کل;
    برائے (غیرعلامتی صحیح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت ELEM> blends = env.arg_ڈھیر.sub_array (آغاز + n + (i * k), k);
      process_arg_blend (env, env.arg_ڈھیر.arrayZ[آغاز + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ڈھیر.نکالو (k * n);
  }

  ساکن خالی process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_دلائل ();
  }

  نجی:
  قسمتعریف cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, راستہ_3>  SUPER;
};

سانچہ <قسم_نام OPSET, قسم_نام PARAM, قسم_نام ELEM>
استعمال cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#اگر_ختم /* HB_CFF2_INTERP_CS_HH */
