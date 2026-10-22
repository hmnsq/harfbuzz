/*
 * Copyright © 2018  Ebrahim Byagowi
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_AAT_LAYOUT_KERX_పట్టిక_HH
#నిర్వచించు HB_AAT_LAYOUT_KERX_పట్టిక_HH

#అన్తర్భావ "hb-kern.hh"
#అన్తర్భావ "hb-aat-layout-ankr-table.hh"
#అన్తర్భావ "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#నిర్వచించు HB_AAT_ట్యాగ్_kerx HB_ట్యాగ్('k','e','r','x')


నామాకాశ AAT {

ఉపయోగించి నామాకాశ OT;


స్థిర అంతఃవరుస పూర్ణాంకం
kerxTupleKern (పూర్ణాంకం value,
	       గుర్తులేని పూర్ణాంకం tuple_లెక్క,
	       స్థిరరాశి శూన్య *పునాది,
	       hb_aat_apply_context_t *c)
{
  ఒకవేళ (likely (!tuple_లెక్క || !c)) తిరిగి value;

  గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = value;
  స్థిరరాశి FWORD *pv = &Struct_At_స్థానభ్రంశం<FWORD> (పునాది, స్థానభ్రంశం);
  ఒకవేళ (unlikely (!c->sanitizer.check_array (pv, tuple_లెక్క))) తిరిగి 0;
  hb_barrier ();
  తిరిగి *pv;
}


నిర్మాణం hb_glyph_pair_t
{
  hb_codepoint_t ఎడమ;
  hb_codepoint_t కుడి;
};

నిర్మాణం KernPair
{
  పూర్ణాంకం get_kerning () స్థిరరాశి { తిరిగి value; }

  పూర్ణాంకం cmp (స్థిరరాశి hb_glyph_pair_t &o) స్థిరరాశి
  {
    పూర్ణాంకం ret = ఎడమ.cmp (o.ఎడమ);
    ఒకవేళ (ret) తిరిగి ret;
    తిరిగి కుడి.cmp (o.కుడి);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBGlyphID16	ఎడమ;
  HBGlyphID16	కుడి;
  FWORD		value;
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

మూస <రకం_పేరు Kern_Sub_పట్టిక_శీర్షిక>
నిర్మాణం Kerx_Sub_పట్టిక_Format0
{
  పూర్ణాంకం get_kerning (hb_codepoint_t ఎడమ, hb_codepoint_t కుడి,
		   hb_aat_apply_context_t *c = శూన్యసూచిక) స్థిరరాశి
  {
    hb_glyph_pair_t pair = {ఎడమ, కుడి};
    పూర్ణాంకం v = జతలు.bsearch (pair).get_kerning ();
    తిరిగి kerxTupleKern (v, శీర్షిక.tuple_లెక్క_2 (), ఇది, c);
  }

  తార్కిక apply (hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    ఒకవేళ (!c->plan->requested_kerning)
      return_trace (అబద్ధం);

    ఒకవేళ (శీర్షిక.coverage & శీర్షిక.Backwards)
      return_trace (అబద్ధం);

    accelerator_t accel (*ఇది, c);
    hb_kern_machine_t<accelerator_t> machine (accel, శీర్షిక.coverage & శీర్షిక.Cross_ప్రవాహం);
    machine.kern (c->font, c->బఫరు, c->plan->kern_ముసుగు);

    return_trace (నిజం);
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &మొదటి_సమితి, సమితి_t &రెండవ_సమితి, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి KernPair& pair : జతలు)
    {
      మొదటి_సమితి.కూడు (pair.ఎడమ);
      రెండవ_సమితి.కూడు (pair.కుడి);
    }
  }

  నిర్మాణం accelerator_t
  {
    స్థిరరాశి Kerx_Sub_పట్టిక_Format0 &పట్టిక;
    hb_aat_apply_context_t *c;

    accelerator_t (స్థిరరాశి Kerx_Sub_పట్టిక_Format0 &పట్టిక_2,
		   hb_aat_apply_context_t *c_) :
		     పట్టిక (పట్టిక_2), c (c_) {}

    పూర్ణాంకం get_kerning (hb_codepoint_t ఎడమ, hb_codepoint_t కుడి) స్థిరరాశి
    {
      ఒకవేళ (!(*c->మొదటి_సమితి)[ఎడమ] || !(*c->రెండవ_సమితి)[కుడి]) తిరిగి 0;
      తిరిగి పట్టిక.get_kerning (ఎడమ, కుడి, c);
    }
  };


  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (జతలు.sanitize (c)));
  }

  రక్షిత:
  Kern_Sub_పట్టిక_శీర్షిక	శీర్షిక;
  Bin_శోధన_Array_యొక్క<KernPair, రకం_పేరు Kern_Sub_పట్టిక_శీర్షిక::Types::HBUINT>
			జతలు;	/* Sorted kern records. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (Kern_Sub_పట్టిక_శీర్షిక::static_కొలత + 16, జతలు);
};


మూస <తార్కిక extended>
నిర్మాణం Format1_నమోదు;

మూస <>
నిర్మాణం Format1_నమోదు<నిజం>
{
  గణన Flags
  {
    నెట్టు		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  నిర్మాణం నమోదు_దత్తాంశం
  {
    HBUINT16	kern_Action_సూచీ;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    బహిరంగ:
    DEFINE_కొలత_STATIC (2);
  };

  స్థిర తార్కిక initiateAction (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.flags & నెట్టు; }

  స్థిర తార్కిక performAction (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.దత్తాంశం.kern_Action_సూచీ != 0xFFFF; }

  స్థిర గుర్తులేని పూర్ణాంకం kern_Action_సూచీ (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.దత్తాంశం.kern_Action_సూచీ; }
};
మూస <>
నిర్మాణం Format1_నమోదు<అబద్ధం>
{
  గణన Flags
  {
    నెట్టు		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    స్థానభ్రంశం_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  రకనిర్వచనం శూన్య నమోదు_దత్తాంశం;

  స్థిర తార్కిక initiateAction (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.flags & నెట్టు; }

  స్థిర తార్కిక performAction (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.flags & స్థానభ్రంశం_2; }

  స్థిర గుర్తులేని పూర్ణాంకం kern_Action_సూచీ (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.flags & స్థానభ్రంశం_2; }
};

మూస <రకం_పేరు Kern_Sub_పట్టిక_శీర్షిక>
నిర్మాణం Kerx_Sub_పట్టిక_Format1
{
  రకనిర్వచనం రకం_పేరు Kern_Sub_పట్టిక_శీర్షిక::Types Types;
  రకనిర్వచనం రకం_పేరు Types::HBUINT HBUINT;

  రకనిర్వచనం Format1_నమోదు<Types::extended> Format1_నమోదు_T;
  రకనిర్వచనం రకం_పేరు Format1_నమోదు_T::నమోదు_దత్తాంశం నమోదు_దత్తాంశం;

  గణన Flags
  {
    DontAdvance	= Format1_నమోదు_T::DontAdvance,
  };

  తార్కిక is_action_initiable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    తిరిగి Format1_నమోదు_T::initiateAction (నమోదు_2);
  }
  తార్కిక is_actionable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    తిరిగి Format1_నమోదు_T::performAction (నమోదు_2);
  }

  నిర్మాణం driver_context_t
  {
    స్థిర constexpr తార్కిక in_place = నిజం;

    driver_context_t (స్థిరరాశి Kerx_Sub_పట్టిక_Format1 *పట్టిక_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	పట్టిక (పట్టిక_2),
	/* Apparently the offset kernAction is from the beginning of the state-machine,
	 * similar to offsets in morx table, NOT from beginning of this table, like
	 * other subtables in kerx.  Discovered via testing. */
	kernAction (&పట్టిక->machine + పట్టిక->kernAction),
	లోతు (0),
	cross_ప్రవాహం (పట్టిక->శీర్షిక.coverage & పట్టిక->శీర్షిక.Cross_ప్రవాహం) {}

    HB_AAT_TRANSITION_INLINE
    శూన్య transition (hb_బఫరు_t *బఫరు,
		     State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> *driver,
		     స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
    {
      గుర్తులేని పూర్ణాంకం flags = నమోదు_2.flags;

      ఒకవేళ (flags & Format1_నమోదు_T::Reset)
	లోతు = 0;

      ఒకవేళ (flags & Format1_నమోదు_T::నెట్టు)
      {
	ఒకవేళ (likely (లోతు < ARRAY_నిడివి (దొంతర)))
	  దొంతర[లోతు++] = బఫరు->idx;
	లేకపోతే
	  లోతు = 0; /* Probably not what CoreText does, but better? */
      }

      ఒకవేళ (Format1_నమోదు_T::performAction (నమోదు_2) && లోతు)
      {
	గుర్తులేని పూర్ణాంకం tuple_లెక్క_2 = hb_max (1u, పట్టిక->శీర్షిక.tuple_లెక్క_2 ());

	గుర్తులేని పూర్ణాంకం kern_idx = Format1_నమోదు_T::kern_Action_సూచీ (నమోదు_2);
	kern_idx = Types::byte_స్థానభ్రంశం_గమ్యం_సూచీ (kern_idx, &పట్టిక->machine, kernAction.arrayZ);
	స్థిరరాశి FWORD *actions = &kernAction[kern_idx];
	ఒకవేళ (!c->sanitizer.check_array (actions, లోతు, tuple_లెక్క_2))
	{
	  లోతు = 0;
	  తిరిగి;
	}
	hb_barrier ();

	hb_ముసుగు_t kern_ముసుగు = c->plan->kern_ముసుగు;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	తార్కిక చివరి = అబద్ధం;
	అయితే (!చివరి && లోతు)
	{
	  గుర్తులేని పూర్ణాంకం idx = దొంతర[--లోతు];
	  పూర్ణాంకం v = *actions;
	  actions += tuple_లెక్క_2;
	  ఒకవేళ (idx >= బఫరు->len) కొనసాగు;

	  /* "The end of the list is marked by an odd value..." */
	  చివరి = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = బఫరు->pos[idx];

	  ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (బఫరు->props.direction))
	  {
	    ఒకవేళ (cross_ప్రవాహం)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      ఒకవేళ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_స్థానభ్రంశం_2 = 0;
	      }
	      లేకపోతే ఒకవేళ (o.attach_type())
	      {
		o.y_స్థానభ్రంశం_2 = hb_saturate_కూడు (o.y_స్థానభ్రంశం_2, c->font->em_scale_y (v));
		బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    లేకపోతే ఒకవేళ (బఫరు->info[idx].ముసుగు & kern_ముసుగు)
	    {
	      స్వయంచాలక scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_కూడు (o.x_advance, scaled);
	      o.x_స్థానభ్రంశం_2 = hb_saturate_కూడు (o.x_స్థానభ్రంశం_2, scaled);
	    }
	  }
	  లేకపోతే
	  {
	    ఒకవేళ (cross_ప్రవాహం)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      ఒకవేళ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_స్థానభ్రంశం_2 = 0;
	      }
	      లేకపోతే ఒకవేళ (o.attach_type())
	      {
		o.x_స్థానభ్రంశం_2 = hb_saturate_కూడు (o.x_స్థానభ్రంశం_2, c->font->em_scale_x (v));
		బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    లేకపోతే ఒకవేళ (బఫరు->info[idx].ముసుగు & kern_ముసుగు)
	    {
	      స్వయంచాలక scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_కూడు (o.y_advance, scaled);
	      o.y_స్థానభ్రంశం_2 = hb_saturate_కూడు (o.y_స్థానభ్రంశం_2, scaled);
	    }
	  }
	}
      }
    }

    బహిరంగ:
    hb_aat_apply_context_t *c;
    స్థిరరాశి Kerx_Sub_పట్టిక_Format1 *పట్టిక;
    అంతరంగ:
    స్థిరరాశి Unsized_Array_యొక్క<FWORD> &kernAction;
    గుర్తులేని పూర్ణాంకం దొంతర[8];
    గుర్తులేని పూర్ణాంకం లోతు;
    తార్కిక cross_ప్రవాహం;
  };

  తార్కిక apply (hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    ఒకవేళ (!c->plan->requested_kerning &&
	!(శీర్షిక.coverage & శీర్షిక.Cross_ప్రవాహం))
      తిరిగి అబద్ధం;

    driver_context_t dc (ఇది, c);

    State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (ఇది) &&
			  machine.sanitize (c)));
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &మొదటి_సమితి, సమితి_t &రెండవ_సమితి, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    machine.collect_initial_glyphs (మొదటి_సమితి, అంకె_glyphs, *ఇది);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  రక్షిత:
  Kern_Sub_పట్టిక_శీర్షిక				శీర్షిక;
  State_పట్టిక<Types, నమోదు_దత్తాంశం>			machine;
  NNOffset_గమ్యం<Unsized_Array_యొక్క<FWORD>, HBUINT>	kernAction;
  బహిరంగ:
  DEFINE_కొలత_STATIC (Kern_Sub_పట్టిక_శీర్షిక::static_కొలత + (State_పట్టిక<Types, నమోదు_దత్తాంశం>::static_కొలత + HBUINT::static_కొలత));
};

మూస <రకం_పేరు Kern_Sub_పట్టిక_శీర్షిక>
నిర్మాణం Kerx_Sub_పట్టిక_Format2
{
  రకనిర్వచనం రకం_పేరు Kern_Sub_పట్టిక_శీర్షిక::Types Types;
  రకనిర్వచనం రకం_పేరు Types::HBUINT HBUINT;

  పూర్ణాంకం get_kerning (hb_codepoint_t ఎడమ, hb_codepoint_t కుడి,
		   hb_aat_apply_context_t *c) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం అంకె_glyphs = c->sanitizer.get_అంకె_glyphs ();
    గుర్తులేని పూర్ణాంకం l = (ఇది+ఎడమ_Class_పట్టిక).get_class (ఎడమ, అంకె_glyphs, 0);
    గుర్తులేని పూర్ణాంకం r = (ఇది+కుడి_Class_పట్టిక).get_class (కుడి, అంకె_glyphs, 0);

    స్థిరరాశి Unsized_Array_యొక్క<FWORD> &arrayZ = ఇది+array;
    గుర్తులేని పూర్ణాంకం kern_idx = l + r;
    kern_idx = Types::స్థానభ్రంశం_గమ్యం_సూచీ (kern_idx, ఇది, arrayZ.arrayZ);
    స్థిరరాశి FWORD *v = &arrayZ[kern_idx];
    ఒకవేళ (unlikely (!v->sanitize (&c->sanitizer))) తిరిగి 0;
    hb_barrier ();

    తిరిగి kerxTupleKern (*v, శీర్షిక.tuple_లెక్క_2 (), ఇది, c);
  }

  తార్కిక apply (hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    ఒకవేళ (!c->plan->requested_kerning)
      return_trace (అబద్ధం);

    ఒకవేళ (శీర్షిక.coverage & శీర్షిక.Backwards)
      return_trace (అబద్ధం);

    accelerator_t accel (*ఇది, c);
    hb_kern_machine_t<accelerator_t> machine (accel, శీర్షిక.coverage & శీర్షిక.Cross_ప్రవాహం);
    machine.kern (c->font, c->బఫరు, c->plan->kern_ముసుగు);

    return_trace (నిజం);
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &మొదటి_సమితి, సమితి_t &రెండవ_సమితి, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    (ఇది+ఎడమ_Class_పట్టిక).collect_glyphs (మొదటి_సమితి, అంకె_glyphs);
    (ఇది+కుడి_Class_పట్టిక).collect_glyphs (రెండవ_సమితి, అంకె_glyphs);
  }

  నిర్మాణం accelerator_t
  {
    స్థిరరాశి Kerx_Sub_పట్టిక_Format2 &పట్టిక;
    hb_aat_apply_context_t *c;

    accelerator_t (స్థిరరాశి Kerx_Sub_పట్టిక_Format2 &పట్టిక_2,
		   hb_aat_apply_context_t *c_) :
		     పట్టిక (పట్టిక_2), c (c_) {}

    పూర్ణాంకం get_kerning (hb_codepoint_t ఎడమ, hb_codepoint_t కుడి) స్థిరరాశి
    {
      ఒకవేళ (!(*c->మొదటి_సమితి)[ఎడమ] || !(*c->రెండవ_సమితి)[కుడి]) తిరిగి 0;
      తిరిగి పట్టిక.get_kerning (ఎడమ, కుడి, c);
    }
  };

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  ఎడమ_Class_పట్టిక.sanitize (c, ఇది) &&
			  కుడి_Class_పట్టిక.sanitize (c, ఇది) &&
			  hb_barrier () &&
			  c->check_range (ఇది, array)));
  }

  రక్షిత:
  Kern_Sub_పట్టిక_శీర్షిక	శీర్షిక;
  HBUINT		అడ్డువరుస_వెడల్పు;	/* The width, in bytes, of a row in the table. */
  NNOffset_గమ్యం<రకం_పేరు Types::ClassTypeWide, HBUINT>
			ఎడమ_Class_పట్టిక;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffset_గమ్యం<రకం_పేరు Types::ClassTypeWide, HBUINT>
			కుడి_Class_పట్టిక;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffset_గమ్యం<Unsized_Array_యొక్క<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (Kern_Sub_పట్టిక_శీర్షిక::static_కొలత + 4 * పరిమాణం (HBUINT));
};

మూస <రకం_పేరు Kern_Sub_పట్టిక_శీర్షిక>
నిర్మాణం Kerx_Sub_పట్టిక_Format4
{
  రకనిర్వచనం ExtendedTypes Types;

  నిర్మాణం నమోదు_దత్తాంశం
  {
    HBUINT16	ankr_Action_సూచీ;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    బహిరంగ:
    DEFINE_కొలత_STATIC (2);
  };

  గణన Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  తార్కిక is_action_initiable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    తిరిగి (నమోదు_2.flags & Mark);
  }
  తార్కిక is_actionable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    తిరిగి నమోదు_2.దత్తాంశం.ankr_Action_సూచీ != 0xFFFF;
  }

  నిర్మాణం driver_context_t
  {
    స్థిర constexpr తార్కిక in_place = నిజం;
    గణన Sub_పట్టిక_Flags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      వాడని		= 0x3F000000,	/* Unused - must be zero. */
      స్థానభ్రంశం_2		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (స్థిరరాశి Kerx_Sub_పట్టిక_Format4 *పట్టిక_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	పట్టిక (పట్టిక_2),
	action_type ((పట్టిక->flags & ActionType) >> 30),
	ankr_దత్తాంశం ((HBUINT16 *) ((స్థిరరాశి అక్షరం *) &పట్టిక->machine + (పట్టిక->flags & స్థానభ్రంశం_2))),
	mark_సమితి (అబద్ధం),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    శూన్య transition (hb_బఫరు_t *బఫరు,
		     State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> *driver,
		     స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
    {
      ఒకవేళ (mark_సమితి && నమోదు_2.దత్తాంశం.ankr_Action_సూచీ != 0xFFFF && బఫరు->idx < బఫరు->len)
      {
	hb_glyph_position_t &o = బఫరు->cur_pos();
	ఎంపిక (action_type)
	{
	  స్థితి 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    స్థిరరాశి HBUINT16 *దత్తాంశం = &ankr_దత్తాంశం[నమోదు_2.దత్తాంశం.ankr_Action_సూచీ * 2];
	    ఒకవేళ (!c->sanitizer.check_array (దత్తాంశం, 2)) తిరిగి;
	    hb_barrier ();
	    గుర్తులేని పూర్ణాంకం mark_Control_బిందువు = *దత్తాంశం++;
	    గుర్తులేని పూర్ణాంకం curr_Control_బిందువు = *దత్తాంశం++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    ఒకవేళ (!c->font->get_glyph_contour_బిందువు_for_origin (c->బఫరు->info[mark].codepoint,
							      mark_Control_బిందువు,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_బిందువు_for_origin (c->బఫరు->cur ().codepoint,
							      curr_Control_బిందువు,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      తిరిగి;

	    o.x_స్థానభ్రంశం_2 = hb_saturate_sub (markX, currX);
	    o.y_స్థానభ్రంశం_2 = hb_saturate_sub (markY, currY);
	  }
	  ఆపు;

	  స్థితి 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    స్థిరరాశి HBUINT16 *దత్తాంశం = &ankr_దత్తాంశం[నమోదు_2.దత్తాంశం.ankr_Action_సూచీ * 2];
	    ఒకవేళ (!c->sanitizer.check_array (దత్తాంశం, 2)) తిరిగి;
	    hb_barrier ();
	    గుర్తులేని పూర్ణాంకం mark_Anchor_బిందువు = *దత్తాంశం++;
	    గుర్తులేని పూర్ణాంకం curr_Anchor_బిందువు = *దత్తాంశం++;
	    స్థిరరాశి Anchor &markAnchor = c->ankr_పట్టిక->get_anchor (c->బఫరు->info[mark].codepoint,
								  mark_Anchor_బిందువు,
								  c->sanitizer.get_అంకె_glyphs ());
	    స్థిరరాశి Anchor &currAnchor = c->ankr_పట్టిక->get_anchor (c->బఫరు->cur ().codepoint,
								  curr_Anchor_బిందువు,
								  c->sanitizer.get_అంకె_glyphs ());

	    o.x_స్థానభ్రంశం_2 = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_స్థానభ్రంశం_2 = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  ఆపు;

	  స్థితి 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    స్థిరరాశి FWORD *దత్తాంశం = (స్థిరరాశి FWORD *) &ankr_దత్తాంశం[నమోదు_2.దత్తాంశం.ankr_Action_సూచీ * 4];
	    ఒకవేళ (!c->sanitizer.check_array (దత్తాంశం, 4)) తిరిగి;
	    hb_barrier ();
	    పూర్ణాంకం markX = *దత్తాంశం++;
	    పూర్ణాంకం markY = *దత్తాంశం++;
	    పూర్ణాంకం currX = *దత్తాంశం++;
	    పూర్ణాంకం currY = *దత్తాంశం++;

	    o.x_స్థానభ్రంశం_2 = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_స్థానభ్రంశం_2 = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  ఆపు;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (పూర్ణాంకం) mark - (పూర్ణాంకం) బఫరు->idx;
	ఒకవేళ (c->బఫరు_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      ఒకవేళ (నమోదు_2.flags & Mark)
      {
	mark_సమితి = నిజం;
	mark = బఫరు->idx;
      }
    }

    బహిరంగ:
    hb_aat_apply_context_t *c;
    స్థిరరాశి Kerx_Sub_పట్టిక_Format4 *పట్టిక;
    అంతరంగ:
    గుర్తులేని పూర్ణాంకం action_type;
    స్థిరరాశి HBUINT16 *ankr_దత్తాంశం;
    తార్కిక mark_సమితి;
    గుర్తులేని పూర్ణాంకం mark;
  };

  తార్కిక apply (hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    driver_context_t dc (ఇది, c);

    State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (ఇది) &&
			  machine.sanitize (c)));
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &మొదటి_సమితి, సమితి_t &రెండవ_సమితి, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    machine.collect_initial_glyphs (మొదటి_సమితి, అంకె_glyphs, *ఇది);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  రక్షిత:
  Kern_Sub_పట్టిక_శీర్షిక		శీర్షిక;
  State_పట్టిక<Types, నమోదు_దత్తాంశం>	machine;
  HBUINT32			flags;
  బహిరంగ:
  DEFINE_కొలత_STATIC (Kern_Sub_పట్టిక_శీర్షిక::static_కొలత + (State_పట్టిక<Types, నమోదు_దత్తాంశం>::static_కొలత + HBUINT32::static_కొలత));
};

మూస <రకం_పేరు Kern_Sub_పట్టిక_శీర్షిక>
నిర్మాణం Kerx_Sub_పట్టిక_Format6
{
  గణన Flags
  {
    విలువలు_Are_Long	= 0x00000001,
  };

  తార్కిక is_long () స్థిరరాశి { తిరిగి flags & విలువలు_Are_Long; }

  పూర్ణాంకం get_kerning (hb_codepoint_t ఎడమ, hb_codepoint_t కుడి,
		   hb_aat_apply_context_t *c) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం అంకె_glyphs = c->sanitizer.get_అంకె_glyphs ();
    ఒకవేళ (is_long ())
    {
      స్థిరరాశి స్వయంచాలక &t = u.l;
      గుర్తులేని పూర్ణాంకం l = (ఇది+t.అడ్డువరుస_సూచీ_పట్టిక).get_విలువ_or_null (ఎడమ, అంకె_glyphs);
      గుర్తులేని పూర్ణాంకం r = (ఇది+t.స్తంభం_సూచీ_పట్టిక).get_విలువ_or_null (కుడి, అంకె_glyphs);
      గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = l + r;
      ఒకవేళ (unlikely (స్థానభ్రంశం < l)) తిరిగి 0; /* Addition overflow. */
      ఒకవేళ (unlikely (hb_unsigned_mul_overflows (స్థానభ్రంశం, పరిమాణం (FWORD32)))) తిరిగి 0;
      స్థిరరాశి FWORD32 *v = &Struct_At_స్థానభ్రంశం<FWORD32> (&(ఇది+t.array), స్థానభ్రంశం * పరిమాణం (FWORD32));
      ఒకవేళ (unlikely (!v->sanitize (&c->sanitizer))) తిరిగి 0;
      hb_barrier ();
      తిరిగి kerxTupleKern (*v, శీర్షిక.tuple_లెక్క_2 (), &(ఇది+vector), c);
    }
    లేకపోతే
    {
      స్థిరరాశి స్వయంచాలక &t = u.s;
      గుర్తులేని పూర్ణాంకం l = (ఇది+t.అడ్డువరుస_సూచీ_పట్టిక).get_విలువ_or_null (ఎడమ, అంకె_glyphs);
      గుర్తులేని పూర్ణాంకం r = (ఇది+t.స్తంభం_సూచీ_పట్టిక).get_విలువ_or_null (కుడి, అంకె_glyphs);
      గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = l + r;
      స్థిరరాశి FWORD *v = &Struct_At_స్థానభ్రంశం<FWORD> (&(ఇది+t.array), స్థానభ్రంశం * పరిమాణం (FWORD));
      ఒకవేళ (unlikely (!v->sanitize (&c->sanitizer))) తిరిగి 0;
      hb_barrier ();
      తిరిగి kerxTupleKern (*v, శీర్షిక.tuple_లెక్క_2 (), &(ఇది+vector), c);
    }
  }

  తార్కిక apply (hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    ఒకవేళ (!c->plan->requested_kerning)
      return_trace (అబద్ధం);

    ఒకవేళ (శీర్షిక.coverage & శీర్షిక.Backwards)
      return_trace (అబద్ధం);

    accelerator_t accel (*ఇది, c);
    hb_kern_machine_t<accelerator_t> machine (accel, శీర్షిక.coverage & శీర్షిక.Cross_ప్రవాహం);
    machine.kern (c->font, c->బఫరు, c->plan->kern_ముసుగు);

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.అడ్డువరుస_సూచీ_పట్టిక.sanitize (c, ఇది) &&
			     u.l.స్తంభం_సూచీ_పట్టిక.sanitize (c, ఇది) &&
			     c->check_range (ఇది, u.l.array)
			   ) : (
			     u.s.అడ్డువరుస_సూచీ_పట్టిక.sanitize (c, ఇది) &&
			     u.s.స్తంభం_సూచీ_పట్టిక.sanitize (c, ఇది) &&
			     c->check_range (ఇది, u.s.array)
			   )) &&
			  (శీర్షిక.tuple_లెక్క_2 () == 0 ||
			   c->check_range (ఇది, vector))));
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &మొదటి_సమితి, సమితి_t &రెండవ_సమితి, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    ఒకవేళ (is_long ())
    {
      స్థిరరాశి స్వయంచాలక &t = u.l;
      (ఇది+t.అడ్డువరుస_సూచీ_పట్టిక).collect_glyphs (మొదటి_సమితి, అంకె_glyphs);
      (ఇది+t.స్తంభం_సూచీ_పట్టిక).collect_glyphs (రెండవ_సమితి, అంకె_glyphs);
    }
    లేకపోతే
    {
      స్థిరరాశి స్వయంచాలక &t = u.s;
      (ఇది+t.అడ్డువరుస_సూచీ_పట్టిక).collect_glyphs (మొదటి_సమితి, అంకె_glyphs);
      (ఇది+t.స్తంభం_సూచీ_పట్టిక).collect_glyphs (రెండవ_సమితి, అంకె_glyphs);
    }
  }

  నిర్మాణం accelerator_t
  {
    స్థిరరాశి Kerx_Sub_పట్టిక_Format6 &పట్టిక;
    hb_aat_apply_context_t *c;

    accelerator_t (స్థిరరాశి Kerx_Sub_పట్టిక_Format6 &పట్టిక_2,
		   hb_aat_apply_context_t *c_) :
		     పట్టిక (పట్టిక_2), c (c_) {}

    పూర్ణాంకం get_kerning (hb_codepoint_t ఎడమ, hb_codepoint_t కుడి) స్థిరరాశి
    {
      ఒకవేళ (!(*c->మొదటి_సమితి)[ఎడమ] || !(*c->రెండవ_సమితి)[కుడి]) తిరిగి 0;
      తిరిగి పట్టిక.get_kerning (ఎడమ, కుడి, c);
    }
  };

  రక్షిత:
  Kern_Sub_పట్టిక_శీర్షిక		శీర్షిక;
  HBUINT32			flags;
  HBUINT16			అడ్డువరుస_లెక్క_2;
  HBUINT16			స్తంభం_లెక్క;
  సంఘం U
  {
    నిర్మాణం Long
    {
      NNOffset32_గమ్యం<Lookup<HBUINT32>>		అడ్డువరుస_సూచీ_పట్టిక;
      NNOffset32_గమ్యం<Lookup<HBUINT32>>		స్తంభం_సూచీ_పట్టిక;
      NNOffset32_గమ్యం<Unsized_Array_యొక్క<FWORD32>>	array;
    } l;
    నిర్మాణం Short
    {
      NNOffset32_గమ్యం<Lookup<HBUINT16>>		అడ్డువరుస_సూచీ_పట్టిక;
      NNOffset32_గమ్యం<Lookup<HBUINT16>>		స్తంభం_సూచీ_పట్టిక;
      NNOffset32_గమ్యం<Unsized_Array_యొక్క<FWORD>>	array;
    } s;
  } u;
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<FWORD>>	vector;
  బహిరంగ:
  DEFINE_కొలత_STATIC (Kern_Sub_పట్టిక_శీర్షిక::static_కొలత + 24);
};


నిర్మాణం Kerx_Sub_పట్టిక_శీర్షిక
{
  రకనిర్వచనం ExtendedTypes Types;

  గుర్తులేని   tuple_లెక్క_2 () స్థిరరాశి { తిరిగి tuple_లెక్క; }
  తార్కిక     is_horizontal () స్థిరరాశి { తిరిగి !(coverage & Vertical); }

  గణన Coverage
  {
    Vertical	= 0x80000000u,	/* Set if table has vertical kerning values. */
    Cross_ప్రవాహం	= 0x40000000u,	/* Set if table has cross-stream kerning values. */
    Variation	= 0x20000000u,	/* Set if table has variation kerning values. */
    Backwards	= 0x10000000u,	/* If clear, process the glyphs forwards, that
				 * is, from first to last in the glyph stream.
				 * If we, process them from last to first.
				 * This flag only applies to state-table based
				 * 'kerx' subtables (types 1 and 4). */
    Reserved	= 0x0FFFFF00u,	/* Reserved, set to zero. */
    SubtableType= 0x000000FFu,	/* Subtable type. */
  };

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBUINT32	నిడివి;
  HBUINT32	coverage;
  HBUINT32	tuple_లెక్క;
  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};

నిర్మాణం Kerx_Sub_పట్టిక
{
  మిత్ర నిర్మాణం kerx;

  size_t get_కొలత () స్థిరరాశి { తిరిగి u.శీర్షిక.నిడివి; }
  గుర్తులేని పూర్ణాంకం get_type () స్థిరరాశి { తిరిగి u.శీర్షిక.coverage & u.శీర్షిక.SubtableType; }

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం subtable_type = get_type ();
    TRACE_DISPATCH (ఇది, subtable_type);
    ఎంపిక (subtable_type) {
    స్థితి 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    స్థితి 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    స్థితి 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    స్థితి 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    స్థితి 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    అప్రమేయ:	return_trace (c->default_return_విలువ ());
    }
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &మొదటి_సమితి, సమితి_t &రెండవ_సమితి, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం subtable_type = get_type ();
    ఎంపిక (subtable_type) {
    స్థితి 0:	hb_barrier (); u.format0.collect_glyphs (మొదటి_సమితి, రెండవ_సమితి, అంకె_glyphs); తిరిగి;
    స్థితి 1:	hb_barrier (); u.format1.collect_glyphs (మొదటి_సమితి, రెండవ_సమితి, అంకె_glyphs); తిరిగి;
    స్థితి 2:	hb_barrier (); u.format2.collect_glyphs (మొదటి_సమితి, రెండవ_సమితి, అంకె_glyphs); తిరిగి;
    స్థితి 4:	hb_barrier (); u.format4.collect_glyphs (మొదటి_సమితి, రెండవ_సమితి, అంకె_glyphs); తిరిగి;
    స్థితి 6:	hb_barrier (); u.format6.collect_glyphs (మొదటి_సమితి, రెండవ_సమితి, అంకె_glyphs); తిరిగి;
    అప్రమేయ:	తిరిగి;
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!(u.శీర్షిక.sanitize (c) &&
	  hb_barrier () &&
	  u.శీర్షిక.నిడివి >= u.శీర్షిక.static_కొలత &&
	  c->check_range (ఇది, u.శీర్షిక.నిడివి)))
      return_trace (అబద్ధం);

    return_trace (dispatch (c));
  }

  బహిరంగ:
  సంఘం {
  Kerx_Sub_పట్టిక_శీర్షిక				శీర్షిక;
  Kerx_Sub_పట్టిక_Format0<Kerx_Sub_పట్టిక_శీర్షిక>	format0;
  Kerx_Sub_పట్టిక_Format1<Kerx_Sub_పట్టిక_శీర్షిక>	format1;
  Kerx_Sub_పట్టిక_Format2<Kerx_Sub_పట్టిక_శీర్షిక>	format2;
  Kerx_Sub_పట్టిక_Format4<Kerx_Sub_పట్టిక_శీర్షిక>	format4;
  Kerx_Sub_పట్టిక_Format6<Kerx_Sub_పట్టిక_శీర్షిక>	format6;
  } u;
  బహిరంగ:
  DEFINE_కొలత_MIN (12);
};


/*
 * The 'kerx' Table
 */

నిర్మాణం kern_subtable_accelerator_దత్తాంశం_t
{
  hb_బిట్టు_సమితి_t మొదటి_సమితి;
  hb_బిట్టు_సమితి_t రెండవ_సమితి;
  మార్చదగిన hb_aat_class_cache_t class_cache;
};

నిర్మాణం kern_accelerator_దత్తాంశం_t
{
  hb_సదిశ_t<kern_subtable_accelerator_దత్తాంశం_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

మూస <రకం_పేరు T>
నిర్మాణం Kerx_పట్టిక
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  స్థిరరాశి T* thiz () స్థిరరాశి { తిరిగి static_cast<స్థిరరాశి T *> (ఇది); }

  తార్కిక has_state_machine () స్థిరరాశి
  {
    రకనిర్వచనం రకం_పేరు T::Sub_పట్టిక Sub_పట్టిక;

    స్థిరరాశి Sub_పట్టిక *st = &thiz()->మొదటి_Sub_పట్టిక;
    గుర్తులేని పూర్ణాంకం లెక్క = thiz()->పట్టిక_లెక్క;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (st->get_type () == 1)
	తిరిగి నిజం;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<Sub_పట్టిక> (*st);
    }
    తిరిగి అబద్ధం;
  }

  తార్కిక has_cross_ప్రవాహం () స్థిరరాశి
  {
    రకనిర్వచనం రకం_పేరు T::Sub_పట్టిక Sub_పట్టిక;

    స్థిరరాశి Sub_పట్టిక *st = &thiz()->మొదటి_Sub_పట్టిక;
    గుర్తులేని పూర్ణాంకం లెక్క = thiz()->పట్టిక_లెక్క;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (st->u.శీర్షిక.coverage & st->u.శీర్షిక.Cross_ప్రవాహం)
	తిరిగి నిజం;
      st = &StructAfter<Sub_పట్టిక> (*st);
    }
    తిరిగి అబద్ధం;
  }

  పూర్ణాంకం get_h_kerning (hb_codepoint_t ఎడమ, hb_codepoint_t కుడి) స్థిరరాశి
  {
    రకనిర్వచనం రకం_పేరు T::Sub_పట్టిక Sub_పట్టిక;

    పూర్ణాంకం v = 0;
    స్థిరరాశి Sub_పట్టిక *st = &thiz()->మొదటి_Sub_పట్టిక;
    గుర్తులేని పూర్ణాంకం లెక్క = thiz()->పట్టిక_లెక్క;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ ((st->u.శీర్షిక.coverage & (st->u.శీర్షిక.Variation | st->u.శీర్షిక.Cross_ప్రవాహం)) ||
	  !st->u.శీర్షిక.is_horizontal ())
	కొనసాగు;
      v += st->get_kerning (ఎడమ, కుడి);
      st = &StructAfter<Sub_పట్టిక> (*st);
    }
    తిరిగి v;
  }

  తార్కిక apply (AAT::hb_aat_apply_context_t *c,
	      స్థిరరాశి kern_accelerator_దత్తాంశం_t &accel_దత్తాంశం) స్థిరరాశి
  {
    c->బఫరు->unsafe_గమ్యం_concat ();

    c->అమరిక_బఫరు_glyph_సమితి ();

    రకనిర్వచనం రకం_పేరు T::Sub_పట్టిక Sub_పట్టిక;

    తార్కిక ret = అబద్ధం;
    తార్కిక చూసిన_Cross_ప్రవాహం = అబద్ధం;
    c->సమితి_lookup_సూచీ (0);
    స్థిరరాశి Sub_పట్టిక *st = &thiz()->మొదటి_Sub_పట్టిక;
    గుర్తులేని పూర్ణాంకం లెక్క = thiz()->పట్టిక_లెక్క;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      తార్కిక reverse;

      స్వయంచాలక &subtable_accel = accel_దత్తాంశం.subtable_accels[i];

      ఒకవేళ (!T::Types::extended && (st->u.శీర్షిక.coverage & st->u.శీర్షిక.Variation))
	వెళ్ళు skip;

      ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (c->బఫరు->props.direction) != st->u.శీర్షిక.is_horizontal ())
	వెళ్ళు skip;

      c->మొదటి_సమితి = &subtable_accel.మొదటి_సమితి;
      c->రెండవ_సమితి = &subtable_accel.రెండవ_సమితి;
      c->machine_class_cache = &subtable_accel.class_cache;

      ఒకవేళ (!c->బఫరు_intersects_machine ())
      {
	(శూన్య) c->బఫరు->సందేశం (c->font, "skipped subtable %u because no glyph matches", c->lookup_సూచీ);
	వెళ్ళు skip;
      }

      reverse = తార్కిక (st->u.శీర్షిక.coverage & st->u.శీర్షిక.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->బఫరు->props.direction);

      ఒకవేళ (!c->బఫరు->సందేశం (c->font, "start subtable %u", c->lookup_సూచీ))
	వెళ్ళు skip;

      ఒకవేళ (!చూసిన_Cross_ప్రవాహం &&
	  (st->u.శీర్షిక.coverage & st->u.శీర్షిక.Cross_ప్రవాహం))
      {
	/* Attach all glyphs into a chain. */
	చూసిన_Cross_ప్రవాహం = నిజం;
	hb_glyph_position_t *pos = c->బఫరు->pos;
	గుర్తులేని పూర్ణాంకం లెక్క = c->బఫరు->len;
	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->బఫరు->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      ఒకవేళ (reverse != c->బఫరు_is_reversed)
        c->తిరగేయి_బఫరు ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < లెక్క - 1 ? st : (స్థిరరాశి Sub_పట్టిక *) శూన్యసూచిక);
	ret |= st->dispatch (c);
      }

      (శూన్య) c->బఫరు->సందేశం (c->font, "end subtable %u", c->lookup_సూచీ);

    skip:
      st = &StructAfter<Sub_పట్టిక> (*st);
      c->సమితి_lookup_సూచీ (c->lookup_సూచీ + 1);
    }
    ఒకవేళ (c->బఫరు_is_reversed)
      c->తిరగేయి_బఫరు ();

    తిరిగి ret;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!(thiz()->సంస్కరణ.sanitize (c) &&
		    hb_barrier () &&
		    (గుర్తులేని) thiz()->సంస్కరణ >= (గుర్తులేని) T::min_సంస్కరణ &&
		    thiz()->పట్టిక_లెక్క.sanitize (c))))
      return_trace (అబద్ధం);

    రకనిర్వచనం రకం_పేరు T::Sub_పట్టిక Sub_పట్టిక;

    స్థిరరాశి Sub_పట్టిక *st = &thiz()->మొదటి_Sub_పట్టిక;
    గుర్తులేని పూర్ణాంకం లెక్క = thiz()->పట్టిక_లెక్క;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (unlikely (!st->u.శీర్షిక.sanitize (c)))
	return_trace (అబద్ధం);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < లెక్క - 1 ? st : (స్థిరరాశి Sub_పట్టిక *) శూన్యసూచిక);

      ఒకవేళ (unlikely (!st->sanitize (c)))
	return_trace (అబద్ధం);

      st = &StructAfter<Sub_పట్టిక> (*st);
    }

    గుర్తులేని major_సంస్కరణ = thiz()->సంస్కరణ;
    ఒకవేళ (పరిమాణం (thiz()->సంస్కరణ) == 4)
      major_సంస్కరణ = major_సంస్కరణ >> 16;
    ఒకవేళ (major_సంస్కరణ >= 3)
    {
      స్థిరరాశి SubtableGlyphCoverage *coverage = (స్థిరరాశి SubtableGlyphCoverage *) st;
      ఒకవేళ (!coverage->sanitize (c, లెక్క))
        return_trace (అబద్ధం);
    }

    return_trace (నిజం);
  }

  kern_accelerator_దత్తాంశం_t create_accelerator_దత్తాంశం (గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    kern_accelerator_దత్తాంశం_t accel_దత్తాంశం;

    రకనిర్వచనం రకం_పేరు T::Sub_పట్టిక Sub_పట్టిక;

    స్థిరరాశి Sub_పట్టిక *st = &thiz()->మొదటి_Sub_పట్టిక;
    గుర్తులేని పూర్ణాంకం లెక్క = thiz()->పట్టిక_లెక్క;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      స్వయంచాలక &subtable_accel = *accel_దత్తాంశం.subtable_accels.push ();
      ఒకవేళ (unlikely (accel_దత్తాంశం.subtable_accels.లోపల_error ()))
	  తిరిగి accel_దత్తాంశం;

      st->collect_glyphs (subtable_accel.మొదటి_సమితి, subtable_accel.రెండవ_సమితి, అంకె_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<Sub_పట్టిక> (*st);
    }

    తిరిగి accel_దత్తాంశం;
  }

  నిర్మాణం accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ఇది->పట్టిక = sc.reference_పట్టిక<T> (face);
      ఇది->accel_దత్తాంశం = ఇది->పట్టిక->create_accelerator_దత్తాంశం (face->get_అంకె_glyphs ());
    }
    ~accelerator_t ()
    {
      ఇది->పట్టిక.destroy ();
    }

    hb_blob_t *get_blob () స్థిరరాశి { తిరిగి పట్టిక.get_blob (); }

    తార్కిక apply (AAT::hb_aat_apply_context_t *c) స్థిరరాశి
    {
      తిరిగి పట్టిక->apply (c, accel_దత్తాంశం);
    }

    hb_blob_ptr_t<T> పట్టిక;
    kern_accelerator_దత్తాంశం_t accel_దత్తాంశం;
    hb_aat_scratch_t scratch;
  };
};

నిర్మాణం kerx : Kerx_పట్టిక<kerx>
{
  మిత్ర నిర్మాణం Kerx_పట్టిక<kerx>;

  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_AAT_ట్యాగ్_kerx;
  స్థిర constexpr గుర్తులేని min_సంస్కరణ = 2u;

  రకనిర్వచనం Kerx_Sub_పట్టిక_శీర్షిక Sub_పట్టిక_శీర్షిక;
  రకనిర్వచనం Sub_పట్టిక_శీర్షిక::Types Types;
  రకనిర్వచనం Kerx_Sub_పట్టిక Sub_పట్టిక;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ; }

  రక్షిత:
  HBUINT16	సంస్కరణ;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	వాడని_2;		/* Set to 0. */
  HBUINT32	పట్టిక_లెక్క;	/* The number of subtables included in the extended kerning
				 * table. */
  Sub_పట్టిక	మొదటి_Sub_పట్టిక;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  బహిరంగ:
  DEFINE_కొలత_MIN (8);
};

నిర్మాణం kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#అయితే_ముగింపు /* HB_AAT_LAYOUT_KERX_TABLE_HH */
