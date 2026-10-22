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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-cff-interp-cs-common.hh"

नामाकाश CFF {

using नामाकाश OT;

संरचना blend_arg_t : सङ्ख्या_t
{
  शून्य set_int (पूर्णाङ्क v) { reset_blends (); सङ्ख्या_t::set_int (v); }
  शून्य set_fixed (int32_t v) { reset_blends (); सङ्ख्या_t::set_fixed (v); }
  शून्य set_real (द्विगुण v) { reset_blends (); सङ्ख्या_t::set_real (v); }

  शून्य set_blends (अचिन्हित पूर्णाङ्क अङ्क_मानहरू, अचिन्हित पूर्णाङ्क मान_Index,
		   hb_array_t<अचर blend_arg_t> blends_)
  {
    अङ्क_मानहरू_2 = अङ्क_मानहरू;
    मान_Index_2 = मान_Index;
    अचिन्हित अङ्क_Blends = blends_.length;
    यदि (unlikely (!deltas.resize_exact (अङ्क_Blends)))
      फर्काऊ;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < अङ्क_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  तार्किक blending () अचर { फर्काऊ deltas.length > 0; }
  शून्य reset_blends ()
  {
    अङ्क_मानहरू_2 = मान_Index_2 = 0;
    deltas.clear ();
  }

  अचिन्हित पूर्णाङ्क अङ्क_मानहरू_2;
  अचिन्हित पूर्णाङ्क मान_Index_2;
  hb_vector_t<सङ्ख्या_t> deltas;
};

प्रकारनिर्देश biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

ढाँचा <typename ELEM>
संरचना cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  ढाँचा <typename ACC>
  cff2_cs_interp_env_t (अचर hb_ubytes_t &str, ACC &acc, अचिन्हित पूर्णाङ्क fd,
			अचर पूर्णाङ्क *coords_=nullptr, अचिन्हित पूर्णाङ्क अङ्क_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_गन्ती_2 (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    अङ्क_coords = अङ्क_coords_2;
    varStore = acc.varStore;
    do_blend = अङ्क_coords && varStore->size;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<भिन्न> *acquire_scalars_vector () अचर
  {
    hb_vector_t<भिन्न> *scalars = cached_scalars_vector->get_acquire ();

    यदि (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<भिन्न> *) hb_calloc (1, परिमाण (hb_vector_t<भिन्न>));
      यदि (unlikely (!scalars))
	फर्काऊ nullptr;
      scalars->init ();
    }

    फर्काऊ scalars;
  }

  शून्य release_scalars_vector (hb_vector_t<भिन्न> *scalars) अचर
  {
    यदि (!scalars)
      फर्काऊ;

    scalars->clear ();

    यदि (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    यदि (एतत्->str_ref.avail ())
      फर्काऊ SUPER::fetch_op ();

    /* make up return or endchar op */
    यदि (एतत्->call_थाक.is_empty ())
      फर्काऊ OpCode_endchar;
    अन्यथा
      फर्काऊ OpCode_return;
  }

  अचर ELEM& eval_arg (अचिन्हित पूर्णाङ्क i)
  {
    फर्काऊ SUPER::arg_थाक[i];
  }

  अचर ELEM& झिक_arg ()
  {
    फर्काऊ SUPER::arg_थाक.झिक ();
  }

  शून्य process_blend ()
  {
    यदि (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      यदि (unlikely (!scalars))
	SUPER::set_error ();
      अन्यथा
      {
	region_गन्ती_2 = varStore->varStore.get_region_index_गन्ती (get_ivs ());
	यदि (do_blend)
	{
	  यदि (unlikely (!scalars->resize_exact (region_गन्ती_2)))
	    SUPER::set_error ();
	  अन्यथा
	    varStore->varStore.get_region_scalars (get_ivs (), coords, अङ्क_coords,
						   &(*scalars)[0], region_गन्ती_2);
	}
      }
      seen_blend = सत्य;
    }
  }

  शून्य process_vsindex ()
  {
    अचिन्हित पूर्णाङ्क  index = SUPER::arg_थाक.झिक_uint ();
    यदि (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    अन्यथा
    {
      set_ivs (index);
    }
    seen_vsindex_ = सत्य;
  }

  अचिन्हित पूर्णाङ्क get_region_गन्ती () अचर { फर्काऊ region_गन्ती_2; }
  शून्य	 set_region_गन्ती (अचिन्हित पूर्णाङ्क region_गन्ती_3) { region_गन्ती_2 = region_गन्ती_3; }
  अचिन्हित पूर्णाङ्क get_ivs () अचर { फर्काऊ ivs; }
  शून्य	 set_ivs (अचिन्हित पूर्णाङ्क ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  अचिन्हित पूर्णाङ्क get_orig_ivs () अचर { फर्काऊ orig_ivs; }
  तार्किक	 seen_vsindex () अचर { फर्काऊ seen_vsindex_; }

  द्विगुण blend_deltas (hb_array_t<अचर ELEM> deltas) अचर
  {
    द्विगुण v = 0;
    यदि (do_blend)
    {
      यदि (likely (scalars && scalars->length == deltas.length))
      {
        अचिन्हित गन्ती = scalars->length;
	लागि (अचिन्हित i = 0; i < गन्ती; i++)
	  v += (द्विगुण) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    फर्काऊ v;
  }

  तार्किक have_coords () अचर { फर्काऊ अङ्क_coords; }

  रक्षित:
  अचर पूर्णाङ्क     *coords;
  अचिन्हित पूर्णाङ्क  अङ्क_coords;
  अचर	 CFF2ItemVariationStore *varStore;
  अचिन्हित पूर्णाङ्क  region_गन्ती_2;
  अचिन्हित पूर्णाङ्क  ivs;
  अचिन्हित पूर्णाङ्क  orig_ivs;
  hb_vector_t<भिन्न>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<भिन्न> *> *cached_scalars_vector = nullptr;
  तार्किक	  do_blend;
  तार्किक	  seen_vsindex_ = असत्य;
  तार्किक	  seen_blend = असत्य;

  प्रकारनिर्देश cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
ढाँचा <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
संरचना cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  स्थिर शून्य process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    विकल्प (op) {
      पक्ष OpCode_callsubr:
      पक्ष OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#यदि 0
	यदि (unlikely (env.arg_थाक.peek ().blending ()))
	{
	  env.set_error ();
	  रोक;
	}
#endif
	SUPER::process_op (op, env, param);
	रोक;

      पक्ष OpCode_blendcs:
	OPSET::process_blend (env, param);
	रोक;

      पक्ष OpCode_vsindexcs:
#यदि 0
	यदि (unlikely (env.arg_थाक.peek ().blending ()))
	{
	  env.set_error ();
	  रोक;
	}
#endif
	OPSET::process_vsindex (env, param);
	रोक;

      स्वभाव:
	SUPER::process_op (op, env, param);
    }
  }

  ढाँचा <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  स्थिर शून्य process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 अचर hb_array_t<अचर ELEM> blends,
				 अचिन्हित n, अचिन्हित i)
  {
    यदि (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    अन्यथा यदि (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<सङ्ख्या_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      यदि (likely (inner.length == arg.deltas.length))
	लागि (अचिन्हित j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      अन्यथा
	env.set_error ();
    }
    अन्यथा
      arg.set_blends (n, i, blends);
  }
  ढाँचा <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  स्थिर शून्य process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 अचर hb_array_t<अचर ELEM> blends,
				 अचिन्हित n, अचिन्हित i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  स्थिर शून्य process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    अचिन्हित पूर्णाङ्क n, k;

    env.process_blend ();
    k = env.get_region_गन्ती ();
    n = env.arg_थाक.झिक_uint ();
    /* copy the blend values into blend array of the default values */
    अचिन्हित पूर्णाङ्क गन्ती = env.arg_थाक.get_गन्ती ();
    अचिन्हित पूर्णाङ्क total;
    यदि (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > गन्ती))
    {
      env.set_error ();
      फर्काऊ;
    }
    यदि (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      फर्काऊ;
    }
    अचिन्हित पूर्णाङ्क start = गन्ती - total;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < n; i++)
    {
      अचर hb_array_t<अचर ELEM> blends = env.arg_थाक.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.arg_थाक.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_थाक.झिक (k * n);
  }

  स्थिर शून्य process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  गुप्त:
  प्रकारनिर्देश cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

ढाँचा <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
