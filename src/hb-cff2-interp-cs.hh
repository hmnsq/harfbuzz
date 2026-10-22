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

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

using نطاق OT;

بنية blend_arg_t : سان_t
{
  فراغ set_int (صحيح v) { reset_blends (); سان_t::set_int (v); }
  فراغ set_fixed (int32_t v) { reset_blends (); سان_t::set_fixed (v); }
  فراغ set_real (مزدوج v) { reset_blends (); سان_t::set_real (v); }

  فراغ set_blends (unsigned صحيح رەقەم_قىممەتلەر, unsigned صحيح قىممەت_Index,
		   hb_array_t<ثابت blend_arg_t> blends_)
  {
    رەقەم_قىممەتلەر_2 = رەقەم_قىممەتلەر;
    قىممەت_Index_2 = قىممەت_Index;
    unsigned رەقەم_Blends = blends_.length;
    إذا (unlikely (!deltas.resize_exact (رەقەم_Blends)))
      أرجع;
    لكل (unsigned صحيح i = 0; i < رەقەم_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  منطقي blending () ثابت { أرجع deltas.length > 0; }
  فراغ reset_blends ()
  {
    رەقەم_قىممەتلەر_2 = قىممەت_Index_2 = 0;
    deltas.clear ();
  }

  unsigned صحيح رەقەم_قىممەتلەر_2;
  unsigned صحيح قىممەت_Index_2;
  hb_vector_t<سان_t> deltas;
};

تعريفنوع biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

template <typename ELEM>
بنية cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  template <typename ACC>
  cff2_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, unsigned صحيح fd,
			ثابت صحيح *coords_=nullptr, unsigned صحيح رەقەم_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_سانى_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    رەقەم_coords = رەقەم_coords_2;
    varStore = acc.varStore;
    do_blend = رەقەم_coords && varStore->size;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<عائم> *acquire_scalars_vector () ثابت
  {
    hb_vector_t<عائم> *scalars = cached_scalars_vector->get_acquire ();

    إذا (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<عائم> *) hb_calloc (1, حجم (hb_vector_t<عائم>));
      إذا (unlikely (!scalars))
	أرجع nullptr;
      scalars->init ();
    }

    أرجع scalars;
  }

  فراغ release_scalars_vector (hb_vector_t<عائم> *scalars) ثابت
  {
    إذا (!scalars)
      أرجع;

    scalars->clear ();

    إذا (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    إذا (هذا->str_ref.avail ())
      أرجع SUPER::fetch_op ();

    /* make up return or endchar op */
    إذا (هذا->call_دۆۋە.is_empty ())
      أرجع OpCode_endchar;
    وإلا
      أرجع OpCode_return;
  }

  ثابت ELEM& eval_arg (unsigned صحيح i)
  {
    أرجع SUPER::arg_دۆۋە[i];
  }

  ثابت ELEM& چىقىرىش_arg ()
  {
    أرجع SUPER::arg_دۆۋە.چىقىرىش ();
  }

  فراغ process_blend ()
  {
    إذا (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      إذا (unlikely (!scalars))
	SUPER::set_error ();
      وإلا
      {
	region_سانى_2 = varStore->varStore.get_region_index_سانى (get_ivs ());
	إذا (do_blend)
	{
	  إذا (unlikely (!scalars->resize_exact (region_سانى_2)))
	    SUPER::set_error ();
	  وإلا
	    varStore->varStore.get_region_scalars (get_ivs (), coords, رەقەم_coords,
						   &(*scalars)[0], region_سانى_2);
	}
      }
      seen_blend = true;
    }
  }

  فراغ process_vsindex ()
  {
    unsigned صحيح  index = SUPER::arg_دۆۋە.چىقىرىش_uint ();
    إذا (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    وإلا
    {
      set_ivs (index);
    }
    seen_vsindex_ = true;
  }

  unsigned صحيح get_region_سانى () ثابت { أرجع region_سانى_2; }
  فراغ	 set_region_سانى (unsigned صحيح region_سانى_3) { region_سانى_2 = region_سانى_3; }
  unsigned صحيح get_ivs () ثابت { أرجع ivs; }
  فراغ	 set_ivs (unsigned صحيح ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  unsigned صحيح get_orig_ivs () ثابت { أرجع orig_ivs; }
  منطقي	 seen_vsindex () ثابت { أرجع seen_vsindex_; }

  مزدوج blend_deltas (hb_array_t<ثابت ELEM> deltas) ثابت
  {
    مزدوج v = 0;
    إذا (do_blend)
    {
      إذا (likely (scalars && scalars->length == deltas.length))
      {
        unsigned سانى = scalars->length;
	لكل (unsigned i = 0; i < سانى; i++)
	  v += (مزدوج) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    أرجع v;
  }

  منطقي have_coords () ثابت { أرجع رەقەم_coords; }

  محمي:
  ثابت صحيح     *coords;
  unsigned صحيح  رەقەم_coords;
  ثابت	 CFF2ItemVariationStore *varStore;
  unsigned صحيح  region_سانى_2;
  unsigned صحيح  ivs;
  unsigned صحيح  orig_ivs;
  hb_vector_t<عائم>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<عائم> *> *cached_scalars_vector = nullptr;
  منطقي	  do_blend;
  منطقي	  seen_vsindex_ = خطأ;
  منطقي	  seen_blend = خطأ;

  تعريفنوع cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
template <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
بنية cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  ساكن فراغ process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    بدل (op) {
      حالة OpCode_callsubr:
      حالة OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#إذا 0
	إذا (unlikely (env.arg_دۆۋە.peek ().blending ()))
	{
	  env.set_error ();
	  اكسر;
	}
#endif
	SUPER::process_op (op, env, param);
	اكسر;

      حالة OpCode_blendcs:
	OPSET::process_blend (env, param);
	اكسر;

      حالة OpCode_vsindexcs:
#إذا 0
	إذا (unlikely (env.arg_دۆۋە.peek ().blending ()))
	{
	  env.set_error ();
	  اكسر;
	}
#endif
	OPSET::process_vsindex (env, param);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
    }
  }

  template <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  ساكن فراغ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 unsigned n, unsigned i)
  {
    إذا (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    وإلا إذا (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<سان_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      إذا (likely (inner.length == arg.deltas.length))
	لكل (unsigned j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      وإلا
	env.set_error ();
    }
    وإلا
      arg.set_blends (n, i, blends);
  }
  template <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  ساكن فراغ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ثابت hb_array_t<ثابت ELEM> blends,
				 unsigned n, unsigned i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  ساكن فراغ process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    unsigned صحيح n, k;

    env.process_blend ();
    k = env.get_region_سانى ();
    n = env.arg_دۆۋە.چىقىرىش_uint ();
    /* copy the blend values into blend array of the default values */
    unsigned صحيح سانى = env.arg_دۆۋە.get_سانى ();
    unsigned صحيح total;
    إذا (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > سانى))
    {
      env.set_error ();
      أرجع;
    }
    إذا (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      أرجع;
    }
    unsigned صحيح start = سانى - total;
    لكل (unsigned صحيح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت ELEM> blends = env.arg_دۆۋە.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_دۆۋە.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_دۆۋە.چىقىرىش (k * n);
  }

  ساكن فراغ process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  خاص:
  تعريفنوع cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
