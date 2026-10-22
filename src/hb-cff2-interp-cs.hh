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

struct blend_arg_t : number_t
{
  void set_int (số_nguyên v) { reset_blends (); number_t::set_int (v); }
  void set_fixed (int32_t v) { reset_blends (); number_t::set_fixed (v); }
  void set_real (double v) { reset_blends (); number_t::set_real (v); }

  void set_blends (unsigned số_nguyên numValues_, unsigned số_nguyên valueIndex_,
		   hb_array_t<const blend_arg_t> blends_)
  {
    numValues = numValues_;
    valueIndex = valueIndex_;
    unsigned numBlends = blends_.length;
    nếu (unlikely (!deltas.resize_exact (numBlends)))
      trả_về;
    cho (unsigned số_nguyên i = 0; i < numBlends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  bool blending () const { trả_về deltas.length > 0; }
  void reset_blends ()
  {
    numValues = valueIndex = 0;
    deltas.clear ();
  }

  unsigned số_nguyên numValues;
  unsigned số_nguyên valueIndex;
  hb_vector_t<number_t> deltas;
};

typedef biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

template <typename ELEM>
struct cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  template <typename ACC>
  cff2_cs_interp_env_t (const hb_ubytes_t &str, ACC &acc, unsigned số_nguyên fd,
			const số_nguyên *coords_=nullptr, unsigned số_nguyên num_coords_=0)
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

    nếu (!scalars || !cached_scalars_vector->cmpexch (scalars, nullptr))
    {
      scalars = (hb_vector_t<float> *) hb_calloc (1, sizeof (hb_vector_t<float>));
      nếu (unlikely (!scalars))
	trả_về nullptr;
      scalars->init ();
    }

    trả_về scalars;
  }

  void release_scalars_vector (hb_vector_t<float> *scalars) const
  {
    nếu (!scalars)
      trả_về;

    scalars->clear ();

    nếu (!cached_scalars_vector->cmpexch (nullptr, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = nullptr;
  }

  op_code_t fetch_op ()
  {
    nếu (this->str_ref.avail ())
      trả_về SUPER::fetch_op ();

    /* make up return or endchar op */
    nếu (this->callStack.is_empty ())
      trả_về OpCode_endchar;
    không_thì
      trả_về OpCode_return;
  }

  const ELEM& eval_arg (unsigned số_nguyên i)
  {
    trả_về SUPER::argStack[i];
  }

  const ELEM& pop_arg ()
  {
    trả_về SUPER::argStack.pop ();
  }

  void process_blend ()
  {
    nếu (!seen_blend)
    {
      scalars = acquire_scalars_vector ();
      nếu (unlikely (!scalars))
	SUPER::set_error ();
      không_thì
      {
	region_count = varStore->varStore.get_region_index_count (get_ivs ());
	nếu (do_blend)
	{
	  nếu (unlikely (!scalars->resize_exact (region_count)))
	    SUPER::set_error ();
	  không_thì
	    varStore->varStore.get_region_scalars (get_ivs (), coords, num_coords,
						   &(*scalars)[0], region_count);
	}
      }
      seen_blend = true;
    }
  }

  void process_vsindex ()
  {
    unsigned số_nguyên  index = SUPER::argStack.pop_uint ();
    nếu (unlikely (seen_vsindex () || seen_blend))
    {
     SUPER::set_error ();
    }
    không_thì
    {
      set_ivs (index);
    }
    seen_vsindex_ = true;
  }

  unsigned số_nguyên get_region_count () const { trả_về region_count; }
  void	 set_region_count (unsigned số_nguyên region_count_) { region_count = region_count_; }
  unsigned số_nguyên get_ivs () const { trả_về ivs; }
  void	 set_ivs (unsigned số_nguyên ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  unsigned số_nguyên get_orig_ivs () const { trả_về orig_ivs; }
  bool	 seen_vsindex () const { trả_về seen_vsindex_; }

  double blend_deltas (hb_array_t<const ELEM> deltas) const
  {
    double v = 0;
    nếu (do_blend)
    {
      nếu (likely (scalars && scalars->length == deltas.length))
      {
        unsigned count = scalars->length;
	cho (unsigned i = 0; i < count; i++)
	  v += (double) scalars->arrayZ[i] * deltas.arrayZ[i].to_real ();
      }
    }
    trả_về v;
  }

  bool have_coords () const { trả_về num_coords; }

  protected:
  const số_nguyên     *coords;
  unsigned số_nguyên  num_coords;
  const	 CFF2ItemVariationStore *varStore;
  unsigned số_nguyên  region_count;
  unsigned số_nguyên  ivs;
  unsigned số_nguyên  orig_ivs;
  hb_vector_t<float>  *scalars = nullptr;
  hb_atomic_t<hb_vector_t<float> *> *cached_scalars_vector = nullptr;
  bool	  do_blend;
  bool	  seen_vsindex_ = false;
  bool	  seen_blend = false;

  typedef cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
template <typename OPSET, typename PARAM, typename ELEM, typename PATH=path_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
struct cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, PATH>
{
  static void process_op (op_code_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    switch (op) {
      case OpCode_callsubr:
      case OpCode_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#nếu 0
	nếu (unlikely (env.argStack.peek ().blending ()))
	{
	  env.set_error ();
	  break;
	}
#endif
	SUPER::process_op (op, env, param);
	break;

      case OpCode_blendcs:
	OPSET::process_blend (env, param);
	break;

      case OpCode_vsindexcs:
#nếu 0
	nếu (unlikely (env.argStack.peek ().blending ()))
	{
	  env.set_error ();
	  break;
	}
#endif
	OPSET::process_vsindex (env, param);
	break;

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
    nếu (env.have_coords ())
      arg.set_int (round (arg.to_real () + env.blend_deltas (blends)));
    không_thì nếu (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_vector_t<number_t> inner = std::move (arg.deltas);
      arg.set_blends (n, i, blends);
      nếu (likely (inner.length == arg.deltas.length))
	cho (unsigned j = 0; j < inner.length; j++)
	  arg.deltas.arrayZ[j].set_real (arg.deltas.arrayZ[j].to_real () +
					 inner.arrayZ[j].to_real ());
      không_thì
	env.set_error ();
    }
    không_thì
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
    unsigned số_nguyên n, k;

    env.process_blend ();
    k = env.get_region_count ();
    n = env.argStack.pop_uint ();
    /* copy the blend values into blend array of the default values */
    unsigned số_nguyên count = env.argStack.get_count ();
    unsigned số_nguyên total;
    nếu (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > count))
    {
      env.set_error ();
      trả_về;
    }
    nếu (unlikely (!env.spend_budget (HB_BUDGET_1, total - n)))
    {
      env.set_error ();
      trả_về;
    }
    unsigned số_nguyên start = count - total;
    cho (unsigned số_nguyên i = 0; i < n; i++)
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
