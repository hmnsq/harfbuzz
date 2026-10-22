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

#شامل "hb.hh"
#شامل "hb-cff-interp-cs-common.hh"

فضاینام CFF {

using فضاینام OT;

ساختار blend_arg_t : عدد_t
{
  تهی set_int (صحیح v) { reset_blends (); عدد_t::set_int (v); }
  تهی set_fixed (int32_t v) { reset_blends (); عدد_t::set_fixed (v); }
  تهی set_real (دوگانه v) { reset_blends (); عدد_t::set_real (v); }

  تهی set_blends (unsigned صحیح رقم_مقادیر, unsigned صحیح مقدار_Index,
		   hb_array_t<ثابت blend_arg_t> blends_)
  {
    رقم_مقادیر_2 = رقم_مقادیر;
    مقدار_Index_2 = مقدار_Index;
    unsigned رقم_Blends = blends_.length;
    اگر (unlikely (!deltas.resize_exact (رقم_Blends)))
      بازگشت;
    برای (unsigned صحیح i = 0; i < رقم_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  منطقی blending () ثابت { بازگشت deltas.length > 0; }
  تهی reset_blends ()
  {
    رقم_مقادیر_2 = مقدار_Index_2 = 0;
    deltas.clear ();
  }

  unsigned صحیح رقم_مقادیر_2;
  unsigned صحیح مقدار_Index_2;
  hb_vector_t<عدد_t> deltas;
};

تعریفنوع biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

template <typename ELEM>
ساختار cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  template <typename ACC>
  cff2_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, unsigned صحیح fd,
			ثابت صحیح *coords_=nullptr, unsigned صحیح رقم_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_شمار_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    رقم_coords = رقم_coords_2;
    varStore = acc.varStore;
    do_blend = رقم_coords && varStore->size;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<اعشاری> *acquire_scalars_vector () ثابت
  {
    hb_vector_t<اعشاری> *scalars = cached_scalars_vector->get_acquire ();

    اگر (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<اعشاری> *) hb_calloc (1, اندازه (hb_vector_t<اعشاری>));
      اگر (unlikely (!scalars))
	بازگشت nullptr;
      scalars->init ();
    }

    بازگشت scalars;
  }

  تهی release_scalars_vector (hb_vector_t<اعشاری> *scalars) ثابت
  {
    اگر (!scalars)
      بازگشت;

    scalars->clear ();

    اگر (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    اگر (این->str_ref.avail ())
      بازگشت SUPER::fetch_op ();

    /* make up return or endchar op */
    اگر (این->call_پشته.is_empty ())
      بازگشت OpCode_endchar;
    وگرنه
      بازگشت OpCode_return;
  }

  ثابت ELEM& eval_arg (unsigned صحیح i)
  {
    بازگشت SUPER::arg_پشته[i];
  }

  ثابت ELEM& برداشت_arg ()
  {
    بازگشت SUPER::arg_پشته.برداشت ();
  }

  تهی process_blend ()
  {
    اگر (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      اگر (unlikely (!scalars))
	SUPER::set_error ();
      وگرنه
      {
	region_شمار_2 = varStore->varStore.get_region_index_شمار (get_ivs ());
	اگر (do_blend)
	{
	  اگر (unlikely (!scalars->resize_exact (region_شمار_2)))
	    SUPER::set_error ();
	  وگرنه
	    varStore->varStore.get_region_scalars (get_ivs (), coords, رقم_coords,
						   &(*scalars)[0], region_شمار_2);
	}
      }
      seen_blend = درست;
    }
  }

  تهی process_vsindex ()
  {
    unsigned صحیح  index = SUPER::arg_پشته.برداشت_uint ();
    اگر (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    وگرنه
    {
      set_ivs (index);
    }
    seen_vsindex_ = درست;
  }

  unsigned صحیح get_region_شمار () ثابت { بازگشت region_شمار_2; }
  تهی	 set_region_شمار (unsigned صحیح region_شمار_3) { region_شمار_2 = region_شمار_3; }
  unsigned صحیح get_ivs () ثابت { بازگشت ivs; }
  تهی	 set_ivs (unsigned صحیح ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  unsigned صحیح get_orig_ivs () ثابت { بازگشت orig_ivs; }
  منطقی	 seen_vsindex () ثابت { بازگشت seen_vsindex_; }

  دوگانه blend_deltas (hb_array_t<ثابت ELEM> deltas) ثابت
  {
    دوگانه v = 0;
    اگر (do_blend)
    {
      اگر (likely (scalars && scalars->length == deltas.length))
      {
        unsigned شمار = scalars->length;
	برای (unsigned i = 0; i < شمار; i++)
	  v += (دوگانه) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    بازگشت v;
  }

  منطقی have_coords () ثابت { بازگشت رقم_coords; }

  محافظتشده:
  ثابت صحیح     *coords;
  unsigned صحیح  رقم_coords;
  ثابت	 CFF2ItemVariationStore *varStore;
  unsigned صحیح  region_شمار_2;
  unsigned صحیح  ivs;
  unsigned صحیح  orig_ivs;
  hb_vector_t<اعشاری>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<اعشاری> *> *cached_scalars_vector = nullptr;
  منطقی	  do_blend;
  منطقی	  seen_vsindex_ = نادرست;
  منطقی	  seen_blend = نادرست;

  تعریفنوع cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
template <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
ساختار cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  ایستا تهی process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    انتخاب (op) {
      حالت OpCode_callsubr:
      حالت OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#اگر 0
	اگر (unlikely (env.arg_پشته.peek ().blending ()))
	{
	  env.set_error ();
	  بشکن;
	}
#endif
	SUPER::process_op (op, env, param);
	بشکن;

      حالت OpCode_blendcs:
	OPSET::process_blend (env, param);
	بشکن;

      حالت OpCode_vsindexcs:
#اگر 0
	اگر (unlikely (env.arg_پشته.peek ().blending ()))
	{
	  env.set_error ();
	  بشکن;
	}
#endif
	OPSET::process_vsindex (env, param);
	بشکن;

      پیشفرض:
	SUPER::process_op (op, env, param);
    }
  }

  template <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  ایستا تهی process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 unsigned n, unsigned i)
  {
    اگر (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    وگرنه اگر (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<عدد_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      اگر (likely (inner.length == arg.deltas.length))
	برای (unsigned j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      وگرنه
	env.set_error ();
    }
    وگرنه
      arg.set_blends (n, i, blends);
  }
  template <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  ایستا تهی process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 unsigned n, unsigned i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  ایستا تهی process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    unsigned صحیح n, k;

    env.process_blend ();
    k = env.get_region_شمار ();
    n = env.arg_پشته.برداشت_uint ();
    /* copy the blend values into blend array of the default values */
    unsigned صحیح شمار = env.arg_پشته.get_شمار ();
    unsigned صحیح total;
    اگر (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > شمار))
    {
      env.set_error ();
      بازگشت;
    }
    اگر (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      بازگشت;
    }
    unsigned صحیح start = شمار - total;
    برای (unsigned صحیح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت ELEM> blends = env.arg_پشته.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_پشته.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_پشته.برداشت (k * n);
  }

  ایستا تهی process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  خصوصی:
  تعریفنوع cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
