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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-cff-interp-cs-common.hh"

নামাকাশ CFF {

using নামাকাশ OT;

সংরচনা blend_arg_t : সংখ্যা_t
{
  শূন্য set_int (পূর্ণাঙ্ক v) { reset_blends (); সংখ্যা_t::set_int (v); }
  শূন্য set_fixed (int32_t v) { reset_blends (); সংখ্যা_t::set_fixed (v); }
  শূন্য set_real (দ্বিগুণ v) { reset_blends (); সংখ্যা_t::set_real (v); }

  শূন্য set_blends (unsigned পূর্ণাঙ্ক অঙ্ক_মানসমূহ, unsigned পূর্ণাঙ্ক মান_Index,
		   hb_array_t<ধ্রুব blend_arg_t> blends_)
  {
    অঙ্ক_মানসমূহ_2 = অঙ্ক_মানসমূহ;
    মান_Index_2 = মান_Index;
    unsigned অঙ্ক_Blends = blends_.length;
    যদি (unlikely (!deltas.resize_exact (অঙ্ক_Blends)))
      প্রত্যাগম;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  তার্কিক blending () ধ্রুব { প্রত্যাগম deltas.length > 0; }
  শূন্য reset_blends ()
  {
    অঙ্ক_মানসমূহ_2 = মান_Index_2 = 0;
    deltas.clear ();
  }

  unsigned পূর্ণাঙ্ক অঙ্ক_মানসমূহ_2;
  unsigned পূর্ণাঙ্ক মান_Index_2;
  hb_vector_t<সংখ্যা_t> deltas;
};

প্রকারনির্দেশ biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

template <typename ELEM>
সংরচনা cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  template <typename ACC>
  cff2_cs_interp_env_t (ধ্রুব hb_ubytes_t &str, ACC &acc, unsigned পূর্ণাঙ্ক fd,
			ধ্রুব পূর্ণাঙ্ক *coords_=nullptr, unsigned পূর্ণাঙ্ক অঙ্ক_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_গুনতি_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    অঙ্ক_coords = অঙ্ক_coords_2;
    varStore = acc.varStore;
    do_blend = অঙ্ক_coords && varStore->size;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<ভিন্ন> *acquire_scalars_vector () ধ্রুব
  {
    hb_vector_t<ভিন্ন> *scalars = cached_scalars_vector->get_acquire ();

    যদি (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<ভিন্ন> *) hb_calloc (1, পরিমাণ (hb_vector_t<ভিন্ন>));
      যদি (unlikely (!scalars))
	প্রত্যাগম nullptr;
      scalars->init ();
    }

    প্রত্যাগম scalars;
  }

  শূন্য release_scalars_vector (hb_vector_t<ভিন্ন> *scalars) ধ্রুব
  {
    যদি (!scalars)
      প্রত্যাগম;

    scalars->clear ();

    যদি (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    যদি (এতত্->str_ref.avail ())
      প্রত্যাগম SUPER::fetch_op ();

    /* make up return or endchar op */
    যদি (এতত্->call_স্তূপ.is_empty ())
      প্রত্যাগম OpCode_endchar;
    অন্যথা
      প্রত্যাগম OpCode_return;
  }

  ধ্রুব ELEM& eval_arg (unsigned পূর্ণাঙ্ক i)
  {
    প্রত্যাগম SUPER::arg_স্তূপ[i];
  }

  ধ্রুব ELEM& বের_করো_arg ()
  {
    প্রত্যাগম SUPER::arg_স্তূপ.বের_করো ();
  }

  শূন্য process_blend ()
  {
    যদি (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      যদি (unlikely (!scalars))
	SUPER::set_error ();
      অন্যথা
      {
	region_গুনতি_2 = varStore->varStore.get_region_index_গুনতি (get_ivs ());
	যদি (do_blend)
	{
	  যদি (unlikely (!scalars->resize_exact (region_গুনতি_2)))
	    SUPER::set_error ();
	  অন্যথা
	    varStore->varStore.get_region_scalars (get_ivs (), coords, অঙ্ক_coords,
						   &(*scalars)[0], region_গুনতি_2);
	}
      }
      seen_blend = সত্য;
    }
  }

  শূন্য process_vsindex ()
  {
    unsigned পূর্ণাঙ্ক  index = SUPER::arg_স্তূপ.বের_করো_uint ();
    যদি (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    অন্যথা
    {
      set_ivs (index);
    }
    seen_vsindex_ = সত্য;
  }

  unsigned পূর্ণাঙ্ক get_region_গুনতি () ধ্রুব { প্রত্যাগম region_গুনতি_2; }
  শূন্য	 set_region_গুনতি (unsigned পূর্ণাঙ্ক region_গুনতি_3) { region_গুনতি_2 = region_গুনতি_3; }
  unsigned পূর্ণাঙ্ক get_ivs () ধ্রুব { প্রত্যাগম ivs; }
  শূন্য	 set_ivs (unsigned পূর্ণাঙ্ক ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  unsigned পূর্ণাঙ্ক get_orig_ivs () ধ্রুব { প্রত্যাগম orig_ivs; }
  তার্কিক	 seen_vsindex () ধ্রুব { প্রত্যাগম seen_vsindex_; }

  দ্বিগুণ blend_deltas (hb_array_t<ধ্রুব ELEM> deltas) ধ্রুব
  {
    দ্বিগুণ v = 0;
    যদি (do_blend)
    {
      যদি (likely (scalars && scalars->length == deltas.length))
      {
        unsigned গুনতি = scalars->length;
	প্রতি (unsigned i = 0; i < গুনতি; i++)
	  v += (দ্বিগুণ) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    প্রত্যাগম v;
  }

  তার্কিক have_coords () ধ্রুব { প্রত্যাগম অঙ্ক_coords; }

  রক্ষিত:
  ধ্রুব পূর্ণাঙ্ক     *coords;
  unsigned পূর্ণাঙ্ক  অঙ্ক_coords;
  ধ্রুব	 CFF2ItemVariationStore *varStore;
  unsigned পূর্ণাঙ্ক  region_গুনতি_2;
  unsigned পূর্ণাঙ্ক  ivs;
  unsigned পূর্ণাঙ্ক  orig_ivs;
  hb_vector_t<ভিন্ন>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<ভিন্ন> *> *cached_scalars_vector = nullptr;
  তার্কিক	  do_blend;
  তার্কিক	  seen_vsindex_ = অসত্য;
  তার্কিক	  seen_blend = অসত্য;

  প্রকারনির্দেশ cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
template <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
সংরচনা cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  স্থির শূন্য process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    বিকল্প (op) {
      পক্ষ OpCode_callsubr:
      পক্ষ OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#যদি 0
	যদি (unlikely (env.arg_স্তূপ.peek ().blending ()))
	{
	  env.set_error ();
	  ভঙ্গ;
	}
#endif
	SUPER::process_op (op, env, param);
	ভঙ্গ;

      পক্ষ OpCode_blendcs:
	OPSET::process_blend (env, param);
	ভঙ্গ;

      পক্ষ OpCode_vsindexcs:
#যদি 0
	যদি (unlikely (env.arg_স্তূপ.peek ().blending ()))
	{
	  env.set_error ();
	  ভঙ্গ;
	}
#endif
	OPSET::process_vsindex (env, param);
	ভঙ্গ;

      স্বভাব:
	SUPER::process_op (op, env, param);
    }
  }

  template <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  স্থির শূন্য process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ধ্রুব hb_array_t<ধ্রুব ELEM> blends,
				 unsigned n, unsigned i)
  {
    যদি (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    অন্যথা যদি (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<সংখ্যা_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      যদি (likely (inner.length == arg.deltas.length))
	প্রতি (unsigned j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      অন্যথা
	env.set_error ();
    }
    অন্যথা
      arg.set_blends (n, i, blends);
  }
  template <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  স্থির শূন্য process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ধ্রুব hb_array_t<ধ্রুব ELEM> blends,
				 unsigned n, unsigned i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  স্থির শূন্য process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    unsigned পূর্ণাঙ্ক n, k;

    env.process_blend ();
    k = env.get_region_গুনতি ();
    n = env.arg_স্তূপ.বের_করো_uint ();
    /* copy the blend values into blend array of the default values */
    unsigned পূর্ণাঙ্ক গুনতি = env.arg_স্তূপ.get_গুনতি ();
    unsigned পূর্ণাঙ্ক total;
    যদি (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > গুনতি))
    {
      env.set_error ();
      প্রত্যাগম;
    }
    যদি (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      প্রত্যাগম;
    }
    unsigned পূর্ণাঙ্ক start = গুনতি - total;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < n; i++)
    {
      ধ্রুব hb_array_t<ধ্রুব ELEM> blends = env.arg_স্তূপ.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_স্তূপ.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_স্তূপ.বের_করো (k * n);
  }

  স্থির শূন্য process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  গুপ্ত:
  প্রকারনির্দেশ cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
