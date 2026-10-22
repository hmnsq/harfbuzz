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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-cff-interp-cs-common.hh"

நாமாகாஶ CFF {

using நாமாகாஶ OT;

கட்டமைப்பு blend_arg_t : எண்_t
{
  வெறுமை set_int (முழு v) { reset_blends (); எண்_t::set_int (v); }
  வெறுமை set_fixed (int32_t v) { reset_blends (); எண்_t::set_fixed (v); }
  வெறுமை set_real (இரட்டை v) { reset_blends (); எண்_t::set_real (v); }

  வெறுமை set_blends (unsigned முழு இலக்கம்_மதிப்புகள், unsigned முழு மதிப்பு_Index,
		   hb_array_t<மாறிலி blend_arg_t> blends_)
  {
    இலக்கம்_மதிப்புகள்_2 = இலக்கம்_மதிப்புகள்;
    மதிப்பு_Index_2 = மதிப்பு_Index;
    unsigned இலக்கம்_Blends = blends_.length;
    எனில் (unlikely (!deltas.resize_exact (இலக்கம்_Blends)))
      திரும்பு;
    க்கு (unsigned முழு i = 0; i < இலக்கம்_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  தருக்க blending () மாறிலி { திரும்பு deltas.length > 0; }
  வெறுமை reset_blends ()
  {
    இலக்கம்_மதிப்புகள்_2 = மதிப்பு_Index_2 = 0;
    deltas.clear ();
  }

  unsigned முழு இலக்கம்_மதிப்புகள்_2;
  unsigned முழு மதிப்பு_Index_2;
  hb_vector_t<எண்_t> deltas;
};

வகைவரை biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

template <typename ELEM>
கட்டமைப்பு cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  template <typename ACC>
  cff2_cs_interp_env_t (மாறிலி hb_ubytes_t &str, ACC &acc, unsigned முழு fd,
			மாறிலி முழு *coords_=nullptr, unsigned முழு இலக்கம்_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_மொத்தம்_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    இலக்கம்_coords = இலக்கம்_coords_2;
    varStore = acc.varStore;
    do_blend = இலக்கம்_coords && varStore->size;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<தசம> *acquire_scalars_vector () மாறிலி
  {
    hb_vector_t<தசம> *scalars = cached_scalars_vector->get_acquire ();

    எனில் (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<தசம> *) hb_calloc (1, அளவு (hb_vector_t<தசம>));
      எனில் (unlikely (!scalars))
	திரும்பு nullptr;
      scalars->init ();
    }

    திரும்பு scalars;
  }

  வெறுமை release_scalars_vector (hb_vector_t<தசம> *scalars) மாறிலி
  {
    எனில் (!scalars)
      திரும்பு;

    scalars->clear ();

    எனில் (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    எனில் (இது->str_ref.avail ())
      திரும்பு SUPER::fetch_op ();

    /* make up return or endchar op */
    எனில் (இது->call_அடுக்கு.is_empty ())
      திரும்பு OpCode_endchar;
    இல்லையெனில்
      திரும்பு OpCode_return;
  }

  மாறிலி ELEM& eval_arg (unsigned முழு i)
  {
    திரும்பு SUPER::arg_அடுக்கு[i];
  }

  மாறிலி ELEM& எடு_arg ()
  {
    திரும்பு SUPER::arg_அடுக்கு.எடு ();
  }

  வெறுமை process_blend ()
  {
    எனில் (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      எனில் (unlikely (!scalars))
	SUPER::set_error ();
      இல்லையெனில்
      {
	region_மொத்தம்_2 = varStore->varStore.get_region_index_மொத்தம் (get_ivs ());
	எனில் (do_blend)
	{
	  எனில் (unlikely (!scalars->resize_exact (region_மொத்தம்_2)))
	    SUPER::set_error ();
	  இல்லையெனில்
	    varStore->varStore.get_region_scalars (get_ivs (), coords, இலக்கம்_coords,
						   &(*scalars)[0], region_மொத்தம்_2);
	}
      }
      seen_blend = உண்மை;
    }
  }

  வெறுமை process_vsindex ()
  {
    unsigned முழு  index = SUPER::arg_அடுக்கு.எடு_uint ();
    எனில் (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    இல்லையெனில்
    {
      set_ivs (index);
    }
    seen_vsindex_ = உண்மை;
  }

  unsigned முழு get_region_மொத்தம் () மாறிலி { திரும்பு region_மொத்தம்_2; }
  வெறுமை	 set_region_மொத்தம் (unsigned முழு region_மொத்தம்_3) { region_மொத்தம்_2 = region_மொத்தம்_3; }
  unsigned முழு get_ivs () மாறிலி { திரும்பு ivs; }
  வெறுமை	 set_ivs (unsigned முழு ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  unsigned முழு get_orig_ivs () மாறிலி { திரும்பு orig_ivs; }
  தருக்க	 seen_vsindex () மாறிலி { திரும்பு seen_vsindex_; }

  இரட்டை blend_deltas (hb_array_t<மாறிலி ELEM> deltas) மாறிலி
  {
    இரட்டை v = 0;
    எனில் (do_blend)
    {
      எனில் (likely (scalars && scalars->length == deltas.length))
      {
        unsigned மொத்தம் = scalars->length;
	க்கு (unsigned i = 0; i < மொத்தம்; i++)
	  v += (இரட்டை) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    திரும்பு v;
  }

  தருக்க have_coords () மாறிலி { திரும்பு இலக்கம்_coords; }

  பாதுகாக்கப்பட்ட:
  மாறிலி முழு     *coords;
  unsigned முழு  இலக்கம்_coords;
  மாறிலி	 CFF2ItemVariationStore *varStore;
  unsigned முழு  region_மொத்தம்_2;
  unsigned முழு  ivs;
  unsigned முழு  orig_ivs;
  hb_vector_t<தசம>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<தசம> *> *cached_scalars_vector = nullptr;
  தருக்க	  do_blend;
  தருக்க	  seen_vsindex_ = பொய்;
  தருக்க	  seen_blend = பொய்;

  வகைவரை cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
template <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
கட்டமைப்பு cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  நிலையான வெறுமை process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    தேர்வு (op) {
      நிலை OpCode_callsubr:
      நிலை OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#எனில் 0
	எனில் (unlikely (env.arg_அடுக்கு.peek ().blending ()))
	{
	  env.set_error ();
	  நிறுத்து;
	}
#endif
	SUPER::process_op (op, env, param);
	நிறுத்து;

      நிலை OpCode_blendcs:
	OPSET::process_blend (env, param);
	நிறுத்து;

      நிலை OpCode_vsindexcs:
#எனில் 0
	எனில் (unlikely (env.arg_அடுக்கு.peek ().blending ()))
	{
	  env.set_error ();
	  நிறுத்து;
	}
#endif
	OPSET::process_vsindex (env, param);
	நிறுத்து;

      இயல்பு:
	SUPER::process_op (op, env, param);
    }
  }

  template <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  நிலையான வெறுமை process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 மாறிலி hb_array_t<மாறிலி ELEM> blends,
				 unsigned n, unsigned i)
  {
    எனில் (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    இல்லையெனில் எனில் (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<எண்_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      எனில் (likely (inner.length == arg.deltas.length))
	க்கு (unsigned j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      இல்லையெனில்
	env.set_error ();
    }
    இல்லையெனில்
      arg.set_blends (n, i, blends);
  }
  template <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  நிலையான வெறுமை process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 மாறிலி hb_array_t<மாறிலி ELEM> blends,
				 unsigned n, unsigned i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  நிலையான வெறுமை process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    unsigned முழு n, k;

    env.process_blend ();
    k = env.get_region_மொத்தம் ();
    n = env.arg_அடுக்கு.எடு_uint ();
    /* copy the blend values into blend array of the default values */
    unsigned முழு மொத்தம் = env.arg_அடுக்கு.get_மொத்தம் ();
    unsigned முழு total;
    எனில் (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > மொத்தம்))
    {
      env.set_error ();
      திரும்பு;
    }
    எனில் (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      திரும்பு;
    }
    unsigned முழு start = மொத்தம் - total;
    க்கு (unsigned முழு i = 0; i < n; i++)
    {
      மாறிலி hb_array_t<மாறிலி ELEM> blends = env.arg_அடுக்கு.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_அடுக்கு.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_அடுக்கு.எடு (k * n);
  }

  நிலையான வெறுமை process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  தனிப்பட்ட:
  வகைவரை cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
