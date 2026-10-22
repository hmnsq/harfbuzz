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
#నిర్వచితం_కాకపోతే HB_CFF2_INTERP_CS_HH
#నిర్వచించు HB_CFF2_INTERP_CS_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-cff-interp-cs-common.hh"

నామాకాశ CFF {

ఉపయోగించి నామాకాశ OT;

నిర్మాణం blend_arg_t : సంఖ్య_t
{
  శూన్య సమితి_int (పూర్ణాంకం v) { reset_blends (); సంఖ్య_t::సమితి_int (v); }
  శూన్య సమితి_fixed (int32_t v) { reset_blends (); సంఖ్య_t::సమితి_fixed (v); }
  శూన్య సమితి_real (ద్విగుణ v) { reset_blends (); సంఖ్య_t::సమితి_real (v); }

  శూన్య సమితి_blends (గుర్తులేని పూర్ణాంకం అంకె_విలువలు, గుర్తులేని పూర్ణాంకం విలువ_సూచీ,
		   hb_array_t<స్థిరరాశి blend_arg_t> blends_)
  {
    అంకె_విలువలు_2 = అంకె_విలువలు;
    విలువ_సూచీ_2 = విలువ_సూచీ;
    గుర్తులేని అంకె_Blends = blends_.నిడివి;
    ఒకవేళ (unlikely (!deltas.resize_exact (అంకె_Blends)))
      తిరిగి;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_Blends; i++)
      deltas.arrayZ[i] = blends_.arrayZ[i];
  }

  తార్కిక blending () స్థిరరాశి { తిరిగి deltas.నిడివి > 0; }
  శూన్య reset_blends ()
  {
    అంకె_విలువలు_2 = విలువ_సూచీ_2 = 0;
    deltas.clear ();
  }

  గుర్తులేని పూర్ణాంకం అంకె_విలువలు_2;
  గుర్తులేని పూర్ణాంకం విలువ_సూచీ_2;
  hb_సదిశ_t<సంఖ్య_t> deltas;
};

రకనిర్వచనం biased_subrs_t<CFF2Subrs>   cff2_biased_subrs_t;

మూస <రకం_పేరు ELEM>
నిర్మాణం cff2_cs_interp_env_t : cs_interp_env_t<ELEM, CFF2Subrs>
{
  మూస <రకం_పేరు ACC>
  cff2_cs_interp_env_t (స్థిరరాశి hb_ubytes_t &str, ACC &acc, గుర్తులేని పూర్ణాంకం fd,
			స్థిరరాశి పూర్ణాంకం *coords_=శూన్యసూచిక, గుర్తులేని పూర్ణాంకం అంకె_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.వ్యక్తిగత_Dicts[fd].localSubrs),
      region_లెక్క_2 (0), cached_scalars_సదిశ (&acc.cached_scalars_సదిశ)
  {
    coords = coords_;
    అంకె_coords = అంకె_coords_2;
    varStore = acc.varStore;
    do_blend = అంకె_coords && varStore->కొలత;
    orig_ivs = acc.వ్యక్తిగత_Dicts[fd].ivs;
    సమితి_ivs (orig_ivs);
  }

  ~cff2_cs_interp_env_t ()
  {
    release_scalars_సదిశ (scalars);
  }

  hb_సదిశ_t<దశాంశం> *acquire_scalars_సదిశ () స్థిరరాశి
  {
    hb_సదిశ_t<దశాంశం> *scalars = cached_scalars_సదిశ->get_acquire ();

    ఒకవేళ (!scalars || !cached_scalars_సదిశ->cmpexch (scalars, శూన్యసూచిక))
    {
      scalars = (hb_సదిశ_t<దశాంశం> *) hb_calloc (1, పరిమాణం (hb_సదిశ_t<దశాంశం>));
      ఒకవేళ (unlikely (!scalars))
	తిరిగి శూన్యసూచిక;
      scalars->init ();
    }

    తిరిగి scalars;
  }

  శూన్య release_scalars_సదిశ (hb_సదిశ_t<దశాంశం> *scalars) స్థిరరాశి
  {
    ఒకవేళ (!scalars)
      తిరిగి;

    scalars->clear ();

    ఒకవేళ (!cached_scalars_సదిశ->cmpexch (శూన్యసూచిక, scalars))
    {
      scalars->fini ();
      hb_free (scalars);
    }
    scalars = శూన్యసూచిక;
  }

  op_సంకేతం_t fetch_op ()
  {
    ఒకవేళ (ఇది->str_ref.avail ())
      తిరిగి SUPER::fetch_op ();

    /* make up return or endchar op */
    ఒకవేళ (ఇది->call_దొంతర.is_empty ())
      తిరిగి Op_సంకేతం_endchar;
    లేకపోతే
      తిరిగి Op_సంకేతం_return;
  }

  స్థిరరాశి ELEM& eval_arg (గుర్తులేని పూర్ణాంకం i)
  {
    తిరిగి SUPER::arg_దొంతర[i];
  }

  స్థిరరాశి ELEM& తీయి_arg ()
  {
    తిరిగి SUPER::arg_దొంతర.తీయి ();
  }

  శూన్య process_blend ()
  {
    ఒకవేళ (!చూసిన_blend)
    {
      scalars = acquire_scalars_సదిశ ();
      ఒకవేళ (unlikely (!scalars))
	SUPER::సమితి_error ();
      లేకపోతే
      {
	region_లెక్క_2 = varStore->varStore.get_region_సూచీ_లెక్క (get_ivs ());
	ఒకవేళ (do_blend)
	{
	  ఒకవేళ (unlikely (!scalars->resize_exact (region_లెక్క_2)))
	    SUPER::సమితి_error ();
	  లేకపోతే
	    varStore->varStore.get_region_scalars (get_ivs (), coords, అంకె_coords,
						   &(*scalars)[0], region_లెక్క_2);
	}
      }
      చూసిన_blend = నిజం;
    }
  }

  శూన్య process_vsindex ()
  {
    గుర్తులేని పూర్ణాంకం  సూచీ_2 = SUPER::arg_దొంతర.తీయి_uint ();
    ఒకవేళ (unlikely (చూసిన_vsindex () || చూసిన_blend))
    {
     SUPER::సమితి_error ();
    }
    లేకపోతే
    {
      సమితి_ivs (సూచీ_2);
    }
    చూసిన_vsindex_2 = నిజం;
  }

  గుర్తులేని పూర్ణాంకం get_region_లెక్క () స్థిరరాశి { తిరిగి region_లెక్క_2; }
  శూన్య	 సమితి_region_లెక్క (గుర్తులేని పూర్ణాంకం region_లెక్క_3) { region_లెక్క_2 = region_లెక్క_3; }
  గుర్తులేని పూర్ణాంకం get_ivs () స్థిరరాశి { తిరిగి ivs; }
  శూన్య	 సమితి_ivs (గుర్తులేని పూర్ణాంకం ivs_) { ivs = ivs_; }
  /* The FD's private-dict ivs, before any vsindex op in the charstring. */
  గుర్తులేని పూర్ణాంకం get_orig_ivs () స్థిరరాశి { తిరిగి orig_ivs; }
  తార్కిక	 చూసిన_vsindex () స్థిరరాశి { తిరిగి చూసిన_vsindex_2; }

  ద్విగుణ blend_deltas (hb_array_t<స్థిరరాశి ELEM> deltas) స్థిరరాశి
  {
    ద్విగుణ v = 0;
    ఒకవేళ (do_blend)
    {
      ఒకవేళ (likely (scalars && scalars->నిడివి == deltas.నిడివి))
      {
        గుర్తులేని లెక్క = scalars->నిడివి;
	కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
	  v += (ద్విగుణ) scalars->arrayZ[i] * deltas.arrayZ[i].గమ్యం_real ();
      }
    }
    తిరిగి v;
  }

  తార్కిక have_coords () స్థిరరాశి { తిరిగి అంకె_coords; }

  రక్షిత:
  స్థిరరాశి పూర్ణాంకం     *coords;
  గుర్తులేని పూర్ణాంకం  అంకె_coords;
  స్థిరరాశి	 CFF2_అంశం_Variation_Store *varStore;
  గుర్తులేని పూర్ణాంకం  region_లెక్క_2;
  గుర్తులేని పూర్ణాంకం  ivs;
  గుర్తులేని పూర్ణాంకం  orig_ivs;
  hb_సదిశ_t<దశాంశం>  *scalars = శూన్యసూచిక;
  hb_atomic_t<hb_సదిశ_t<దశాంశం> *> *cached_scalars_సదిశ = శూన్యసూచిక;
  తార్కిక	  do_blend;
  తార్కిక	  చూసిన_vsindex_2 = అబద్ధం;
  తార్కిక	  చూసిన_blend = అబద్ధం;

  రకనిర్వచనం cs_interp_env_t<ELEM, CFF2Subrs> SUPER;
};
మూస <రకం_పేరు OPSET, రకం_పేరు PARAM, రకం_పేరు ELEM, రకం_పేరు మార్గం_3=మార్గం_procs_null_t<cff2_cs_interp_env_t<ELEM>, PARAM>>
నిర్మాణం cff2_cs_opset_t : cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, మార్గం_3>
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_callsubr:
      స్థితి Op_సంకేతం_callgsubr:
	/* a subroutine number shouldn't be a blended value */
#ఒకవేళ 0
	ఒకవేళ (unlikely (env.arg_దొంతర.peek ().blending ()))
	{
	  env.సమితి_error ();
	  ఆపు;
	}
#అయితే_ముగింపు
	SUPER::process_op (op, env, param);
	ఆపు;

      స్థితి Op_సంకేతం_blendcs:
	OPSET::process_blend (env, param);
	ఆపు;

      స్థితి Op_సంకేతం_vsindexcs:
#ఒకవేళ 0
	ఒకవేళ (unlikely (env.arg_దొంతర.peek ().blending ()))
	{
	  env.సమితి_error ();
	  ఆపు;
	}
#అయితే_ముగింపు
	OPSET::process_vsindex (env, param);
	ఆపు;

      అప్రమేయ:
	SUPER::process_op (op, env, param);
    }
  }

  మూస <రకం_పేరు T = ELEM,
	    hb_enable_if (hb_is_same (T, blend_arg_t))>
  స్థిర శూన్య process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 స్థిరరాశి hb_array_t<స్థిరరాశి ELEM> blends,
				 గుర్తులేని n, గుర్తులేని i)
  {
    ఒకవేళ (env.have_coords ())
      arg.సమితి_int (round (arg.గమ్యం_real () + env.blend_deltas (blends)));
    లేకపోతే ఒకవేళ (unlikely (arg.blending ()))
    {
      /* A blend result used as an operand of a later blend: the value is
       * default + inner deltas + outer deltas, so accumulate the deltas. */
      hb_సదిశ_t<సంఖ్య_t> inner = std::move (arg.deltas);
      arg.సమితి_blends (n, i, blends);
      ఒకవేళ (likely (inner.నిడివి == arg.deltas.నిడివి))
	కొరకు (గుర్తులేని j = 0; j < inner.నిడివి; j++)
	  arg.deltas.arrayZ[j].సమితి_real (arg.deltas.arrayZ[j].గమ్యం_real () +
					 inner.arrayZ[j].గమ్యం_real ());
      లేకపోతే
	env.సమితి_error ();
    }
    లేకపోతే
      arg.సమితి_blends (n, i, blends);
  }
  మూస <రకం_పేరు T = ELEM,
	    hb_enable_if (!hb_is_same (T, blend_arg_t))>
  స్థిర శూన్య process_arg_blend (cff2_cs_interp_env_t<ELEM> &env,
				 ELEM &arg,
				 స్థిరరాశి hb_array_t<స్థిరరాశి ELEM> blends,
				 గుర్తులేని n, గుర్తులేని i)
  {
    arg.సమితి_real (arg.గమ్యం_real () + env.blend_deltas (blends));
  }

  స్థిర శూన్య process_blend (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    గుర్తులేని పూర్ణాంకం n, k;

    env.process_blend ();
    k = env.get_region_లెక్క ();
    n = env.arg_దొంతర.తీయి_uint ();
    /* copy the blend values into blend array of the default values */
    గుర్తులేని పూర్ణాంకం లెక్క = env.arg_దొంతర.get_లెక్క ();
    గుర్తులేని పూర్ణాంకం మొత్తం;
    ఒకవేళ (unlikely (hb_unsigned_mul_overflows (k + 1, n, &మొత్తం) || మొత్తం > లెక్క))
    {
      env.సమితి_error ();
      తిరిగి;
    }
    ఒకవేళ (unlikely (!env.spend_budget (HB_BUDGET_1, మొత్తం - n)))
    {
      env.సమితి_error ();
      తిరిగి;
    }
    గుర్తులేని పూర్ణాంకం ప్రారంభం = లెక్క - మొత్తం;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < n; i++)
    {
      స్థిరరాశి hb_array_t<స్థిరరాశి ELEM> blends = env.arg_దొంతర.sub_array (ప్రారంభం + n + (i * k), k);
      process_arg_blend (env, env.arg_దొంతర.arrayZ[ప్రారంభం + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_దొంతర.తీయి (k * n);
  }

  స్థిర శూన్య process_vsindex (cff2_cs_interp_env_t<ELEM> &env, PARAM& param)
  {
    env.process_vsindex ();
    env.clear_వాదనలు ();
  }

  అంతరంగ:
  రకనిర్వచనం cs_opset_t<ELEM, OPSET, cff2_cs_interp_env_t<ELEM>, PARAM, మార్గం_3>  SUPER;
};

మూస <రకం_పేరు OPSET, రకం_పేరు PARAM, రకం_పేరు ELEM>
ఉపయోగించి cff2_cs_interpreter_t = cs_interpreter_t<cff2_cs_interp_env_t<ELEM>, OPSET, PARAM>;

} /* namespace CFF */

#అయితే_ముగింపు /* HB_CFF2_INTERP_CS_HH */
