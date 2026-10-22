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

#include "hb.hh"
#include "hb-cff-interp-cs-common.hh"

namespace CFF {

using namespace OT;

sa_nracanaa blend_arg_t : number_t
{
  void set_int (puur_naa_nka v) { reset_blends (); number_t::set_int (v); }
  void set_fixed (int32_t v) { reset_blends (); number_t::set_fixed (v); }
  void set_real (dviguu_na v) { reset_blends (); number_t::set_real (v); }

  void set_blends (unsigned puur_naa_nka numValues_, unsigned puur_naa_nka valueIndex_,
		   hb_array_t<const blend_arg_t> blends_)
  {
    numValues = numValues_;
    valueIndex = valueIndex_;
    unsigned numBlends = blends_.length;
    ᱡᱩᱫᱤ (unlikely (!deltas.resize_exact (numBlends)))
      ᱨᱩᱣᱟᱹᱲ;
    for (unsigned puur_naa_nka i = 0; i < numBlends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  bool blending () const { ᱨᱩᱣᱟᱹᱲ deltas.length > 0; }
  void reset_blends ()
  {
    numValues = valueIndex = 0;
    deltas.clear ();
  }

  unsigned puur_naa_nka numValues;
  unsigned puur_naa_nka valueIndex;
  hb_vector_t<number_t> deltas;
};

typedef biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

template <typename ELEM>
sa_nracanaa cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  template <typename ACC>
  cff2_cs_interp_env_t (const hb_ubytes_t &str, ACC &acc, unsigned puur_naa_nka fd,
			const puur_naa_nka *coords_=nullptr, unsigned puur_naa_nka num_coords_=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs),
      region_count (0), cached_scalars_vector (&acc.cached_scalars_vector)
  {
    coords = coords_;
    num_coords = num_coords_;
    varStore = acc.varStore;
    do_blend = num_coords && varStore->size;
    orig_ivs = acc.privateDicts[fd].ivs;
    set_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_vector (scalars);
  }

  hb_vector_t<float> *acquire_scalars_vector () const
  {
    hb_vector_t<float> *scalars = cached_scalars_vector->get_acquire ();

    ᱡᱩᱫᱤ (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<float> *) hb_calloc (1, parimaa_na (hb_vector_t<float>));
      ᱡᱩᱫᱤ (unlikely (!scalars))
	ᱨᱩᱣᱟᱹᱲ nullptr;
      scalars->init ();
    }

    ᱨᱩᱣᱟᱹᱲ scalars;
  }

  void release_scalars_vector (hb_vector_t<float> *scalars) const
  {
    ᱡᱩᱫᱤ (!scalars)
      ᱨᱩᱣᱟᱹᱲ;

    scalars->clear ();

    ᱡᱩᱫᱤ (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    ᱡᱩᱫᱤ (this->str_ref.avail ())
      ᱨᱩᱣᱟᱹᱲ SUPER::fetch_op ();

    /* make up return or endchar op */
    ᱡᱩᱫᱤ (this->callStack.is_empty ())
      ᱨᱩᱣᱟᱹᱲ OpCode_endchar;
    ᱵᱟᱝ
      ᱨᱩᱣᱟᱹᱲ OpCode_return;
  }

  const ELEM& eval_arg (unsigned puur_naa_nka i)
  {
    ᱨᱩᱣᱟᱹᱲ SUPER::argStack[i];
  }

  const ELEM& pop_arg ()
  {
    ᱨᱩᱣᱟᱹᱲ SUPER::argStack.pop ();
  }

  void process_blend ()
  {
    ᱡᱩᱫᱤ (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      ᱡᱩᱫᱤ (unlikely (!scalars))
	SUPER::set_error ();
      ᱵᱟᱝ
      {
	region_count = varStore->varStore.get_region_index_count (get_ivs ());
	ᱡᱩᱫᱤ (do_blend)
	{
	  ᱡᱩᱫᱤ (unlikely (!scalars->resize_exact (region_count)))
	    SUPER::set_error ();
	  ᱵᱟᱝ
	    varStore->varStore.get_region_scalars (get_ivs (), coords, num_coords,
						   &(*scalars)[0], region_count);
	}
      }
      seen_blend = true;
    }
  }

  void process_vsindex ()
  {
    unsigned puur_naa_nka  index = SUPER::argStack.pop_uint ();
    ᱡᱩᱫᱤ (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    ᱵᱟᱝ
    {
      set_ivs (index);
    }
    seen_vsindex_ = true;
  }

  unsigned puur_naa_nka get_region_count () const { ᱨᱩᱣᱟᱹᱲ region_count; }
  void	 set_region_count (unsigned puur_naa_nka region_count_) { region_count = region_count_; }
  unsigned puur_naa_nka get_ivs () const { ᱨᱩᱣᱟᱹᱲ ivs; }
  void	 set_ivs (unsigned puur_naa_nka ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  unsigned puur_naa_nka get_orig_ivs () const { ᱨᱩᱣᱟᱹᱲ orig_ivs; }
  bool	 seen_vsindex () const { ᱨᱩᱣᱟᱹᱲ seen_vsindex_; }

  dviguu_na blend_deltas (hb_array_t<const ELEM> deltas) const
  {
    dviguu_na v = 0;
    ᱡᱩᱫᱤ (do_blend)
    {
      ᱡᱩᱫᱤ (likely (scalars && scalars->length == deltas.length))
      {
        unsigned count = scalars->length;
	for (unsigned i = 0; i < count; i++)
	  v += (dviguu_na) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    ᱨᱩᱣᱟᱹᱲ v;
  }

  bool have_coords () const { ᱨᱩᱣᱟᱹᱲ num_coords; }

  rak_sita:
  const puur_naa_nka     *coords;
  unsigned puur_naa_nka  num_coords;
  const	 CFF2ItemVariationStore *varStore;
  unsigned puur_naa_nka  region_count;
  unsigned puur_naa_nka  ivs;
  unsigned puur_naa_nka  orig_ivs;
  hb_vector_t<float>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<float> *> *cached_scalars_vector = nullptr;
  bool	  do_blend;
  bool	  seen_vsindex_ = false;
  bool	  seen_blend = false;

  typedef cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
template <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
sa_nracanaa cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  static void process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    switch (op) {
      pak_sa OpCode_callsubr:
      pak_sa OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#ᱡᱩᱫᱤ 0
	ᱡᱩᱫᱤ (unlikely (env.argStack.peek ().blending ()))
	{
	  env.set_error ();
	  bha_nga;
	}
#endif
	SUPER::process_op (op, env, param);
	bha_nga;

      pak_sa OpCode_blendcs:
	OPSET::process_blend (env, param);
	bha_nga;

      pak_sa OpCode_vsindexcs:
#ᱡᱩᱫᱤ 0
	ᱡᱩᱫᱤ (unlikely (env.argStack.peek ().blending ()))
	{
	  env.set_error ();
	  bha_nga;
	}
#endif
	OPSET::process_vsindex (env, param);
	bha_nga;

      default:
	SUPER::process_op (op, env, param);
    }
  }

  template <typename T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  static void process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 const hb_array_t<const ELEM> blends,
				 unsigned n, unsigned i)
  {
    ᱡᱩᱫᱤ (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    ᱵᱟᱝ ᱡᱩᱫᱤ (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<number_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      ᱡᱩᱫᱤ (likely (inner.length == arg.deltas.length))
	for (unsigned j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      ᱵᱟᱝ
	env.set_error ();
    }
    ᱵᱟᱝ
      arg.set_blends (n, i, blends);
  }
  template <typename T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  static void process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 const hb_array_t<const ELEM> blends,
				 unsigned n, unsigned i)
  {
    arg.set_real (arg.to_real () + env.blend_deltas (blends));
  }

  static void process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    unsigned puur_naa_nka n, k;

    env.process_blend ();
    k = env.get_region_count ();
    n = env.argStack.pop_uint ();
    /* copy the blend values into blend array of the default values */
    unsigned puur_naa_nka count = env.argStack.get_count ();
    unsigned puur_naa_nka total;
    ᱡᱩᱫᱤ (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > count))
    {
      env.set_error ();
      ᱨᱩᱣᱟᱹᱲ;
    }
    ᱡᱩᱫᱤ (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      ᱨᱩᱣᱟᱹᱲ;
    }
    unsigned puur_naa_nka start = count - total;
    for (unsigned puur_naa_nka i = 0; i < n; i++)
    {
      const hb_array_t<const ELEM> blends = env.argStack.sub_array (start + n + (i * k), k);
      process_arg_blend (env, env.argStack.arrayZ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.argStack.pop (k * n);
  }

  static void process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_args ();
  }

  private:
  typedef cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM, typename ELEM>
using cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF2_INTERP_CS_HH */
