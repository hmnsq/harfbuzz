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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cff-interp-cs-common.hh"

ନାମାକାଶ CFF {

using ନାମାକାଶ OT;

ସଂରଚନା blend_arg_t : ସଂଖ୍ୟା_t
{
  ଶୂନ୍ୟ set_int (ପୂର୍ଣ୍ଣସଂଖ୍ୟା v) { reset_blends (); ସଂଖ୍ୟା_t::set_int (v); }
  ଶୂନ୍ୟ set_fixed (int32_t v) { reset_blends (); ସଂଖ୍ୟା_t::set_fixed (v); }
  ଶୂନ୍ୟ set_real (ଦ୍ୱିଗୁଣ v) { reset_blends (); ସଂଖ୍ୟା_t::set_real (v); }

  ଶୂନ୍ୟ set_blends (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ_Index,
		   hb_array_t<ଅଚଳ blend_arg_t> blends_)
  {
    ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2 = ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ;
    ମୂଲ୍ୟ_Index_2 = ମୂଲ୍ୟ_Index;
    unsigned ଅଙ୍କ_Blends = blends_.length;
    ଯଦି (unlikely (!deltas.resize_exact (ଅଙ୍କ_Blends)))
      ଫେରନ୍ତୁ;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  ତାର୍କିକ blending () ଅଚଳ { ଫେରନ୍ତୁ deltas.length > 0; }
  ଶୂନ୍ୟ reset_blends ()
  {
    ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2 = ମୂଲ୍ୟ_Index_2 = 0;
    deltas.clear ();
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ_Index_2;
  hb_vector_t<ସଂଖ୍ୟା_t> deltas;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

template <typename ELEM>
ସଂରଚନା cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  template <typename ACC>
  cff2_cs_interp_env_t (ଅଚଳ hb_ubytes_t &str, ACC &acc, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd,
			ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords_=nullptr, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_ଗଣନା_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    ଅଙ୍କ_coords = ଅଙ୍କ_coords_2;
    varStore = acc.varStore;
    do_blend = ଅଙ୍କ_coords && varStore->size;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<ଦଶମିକ> *acquire_scalars_vector () ଅଚଳ
  {
    hb_vector_t<ଦଶମିକ> *scalars = cached_scalars_vector->get_acquire ();

    ଯଦି (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<ଦଶମିକ> *) hb_calloc (1, ଆକାର (hb_vector_t<ଦଶମିକ>));
      ଯଦି (unlikely (!scalars))
	ଫେରନ୍ତୁ nullptr;
      scalars->init ();
    }

    ଫେରନ୍ତୁ scalars;
  }

  ଶୂନ୍ୟ release_scalars_vector (hb_vector_t<ଦଶମିକ> *scalars) ଅଚଳ
  {
    ଯଦି (!scalars)
      ଫେରନ୍ତୁ;

    scalars->clear ();

    ଯଦି (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    ଯଦି (ଏହି->str_ref.avail ())
      ଫେରନ୍ତୁ SUPER::fetch_op ();

    /* make up return or endchar op */
    ଯଦି (ଏହି->call_ଗଦା.is_empty ())
      ଫେରନ୍ତୁ OpCode_endchar;
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ OpCode_return;
  }

  ଅଚଳ ELEM& eval_arg (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
  {
    ଫେରନ୍ତୁ SUPER::arg_ଗଦା[i];
  }

  ଅଚଳ ELEM& କାଢ଼_arg ()
  {
    ଫେରନ୍ତୁ SUPER::arg_ଗଦା.କାଢ଼ ();
  }

  ଶୂନ୍ୟ process_blend ()
  {
    ଯଦି (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      ଯଦି (unlikely (!scalars))
	SUPER::set_error ();
      ଅନ୍ୟଥା
      {
	region_ଗଣନା_2 = varStore->varStore.get_region_index_ଗଣନା (get_ivs ());
	ଯଦି (do_blend)
	{
	  ଯଦି (unlikely (!scalars->resize_exact (region_ଗଣନା_2)))
	    SUPER::set_error ();
	  ଅନ୍ୟଥା
	    varStore->varStore.get_region_scalars (get_ivs (), coords, ଅଙ୍କ_coords,
						   &(*scalars)[0], region_ଗଣନା_2);
	}
      }
      seen_blend = ସତ୍ୟ;
    }
  }

  ଶୂନ୍ୟ process_vsindex ()
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  index = SUPER::arg_ଗଦା.କାଢ଼_uint ();
    ଯଦି (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    ଅନ୍ୟଥା
    {
      set_ivs (index);
    }
    seen_vsindex_ = ସତ୍ୟ;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_region_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ region_ଗଣନା_2; }
  ଶୂନ୍ୟ	 set_region_ଗଣନା (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା region_ଗଣନା_3) { region_ଗଣନା_2 = region_ଗଣନା_3; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ivs () ଅଚଳ { ଫେରନ୍ତୁ ivs; }
  ଶୂନ୍ୟ	 set_ivs (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_orig_ivs () ଅଚଳ { ଫେରନ୍ତୁ orig_ivs; }
  ତାର୍କିକ	 seen_vsindex () ଅଚଳ { ଫେରନ୍ତୁ seen_vsindex_; }

  ଦ୍ୱିଗୁଣ blend_deltas (hb_array_t<ଅଚଳ ELEM> deltas) ଅଚଳ
  {
    ଦ୍ୱିଗୁଣ v = 0;
    ଯଦି (do_blend)
    {
      ଯଦି (likely (scalars && scalars->length == deltas.length))
      {
        unsigned ଗଣନା_2 = scalars->length;
	ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
	  v += (ଦ୍ୱିଗୁଣ) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    ଫେରନ୍ତୁ v;
  }

  ତାର୍କିକ have_coords () ଅଚଳ { ଫେରନ୍ତୁ ଅଙ୍କ_coords; }

  ସୁରକ୍ଷିତ:
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା     *coords;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  ଅଙ୍କ_coords;
  ଅଚଳ	 CFF2ItemVariationStore *varStore;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  region_ଗଣନା_2;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  ivs;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  orig_ivs;
  hb_vector_t<ଦଶମିକ>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<ଦଶମିକ> *> *cached_scalars_vector = nullptr;
  ତାର୍କିକ	  do_blend;
  ତାର୍କିକ	  seen_vsindex_ = ମିଥ୍ୟା;
  ତାର୍କିକ	  seen_blend = ମିଥ୍ୟା;

  ପ୍ରକାରବ୍ୟାଖ୍ୟା cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
template <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
ସଂରଚନା cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    ବାଛ (op) {
      ସ୍ଥିତି OpCode_callsubr:
      ସ୍ଥିତି OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#ଯଦି 0
	ଯଦି (unlikely (env.arg_ଗଦା.peek ().blending ()))
	{
	  env.set_error ();
	  ଭାଙ୍ଗ;
	}
#endif
	SUPER::process_op (op, env, param);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_blendcs:
	OPSET::process_blend (env, param);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_vsindexcs:
#ଯଦି 0
	ଯଦି (unlikely (env.arg_ଗଦା.peek ().blending ()))
	{
	  env.set_error ();
	  ଭାଙ୍ଗ;
	}
#endif
	OPSET::process_vsindex (env, param);
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	SUPER::process_op (op, env, param);
    }
  }

  template <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  ସ୍ଥିର ଶୂନ୍ୟ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ଅଚଳ hb_array_t<ଅଚଳ ELEM> blends,
				 unsigned n, unsigned i)
  {
    ଯଦି (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    ଅନ୍ୟଥା ଯଦି (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<ସଂଖ୍ୟା_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      ଯଦି (likely (inner.length == arg.deltas.length))
	ପାଇଁ (unsigned j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      ଅନ୍ୟଥା
	env.set_error ();
    }
    ଅନ୍ୟଥା
      arg.set_blends (n, i, blends);
  }
  template <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  ସ୍ଥିର ଶୂନ୍ୟ process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 ଅଚଳ hb_array_t<ଅଚଳ ELEM> blends,
				 unsigned n, unsigned i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  ସ୍ଥିର ଶୂନ୍ୟ process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, k;

    env.process_blend ();
    k = env.get_region_ଗଣନା ();
    n = env.arg_ଗଦା.କାଢ଼_uint ();
    /* copy the blend values into blend array of the default values */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = env.arg_ଗଦା.get_ଗଣନା ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା total;
    ଯଦି (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > ଗଣନା_2))
    {
      env.set_error ();
      ଫେରନ୍ତୁ;
    }
    ଯଦି (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      ଫେରନ୍ତୁ;
    }
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = ଗଣନା_2 - total;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
    {
      ଅଚଳ hb_array_t<ଅଚଳ ELEM> blends = env.arg_ଗଦା.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_ଗଦା.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ଗଦା.କାଢ଼ (k * n);
  }

  ସ୍ଥିର ଶୂନ୍ୟ process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
