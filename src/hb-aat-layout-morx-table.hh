/*
 * Copyright © 2017  Google, Inc.
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

#నిర్వచితం_కాకపోతే HB_AAT_LAYOUT_MORX_పట్టిక_HH
#నిర్వచించు HB_AAT_LAYOUT_MORX_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-aat-layout-common.hh"
#అన్తర్భావ "hb-ot-layout.hh"
#అన్తర్భావ "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#నిర్వచించు HB_AAT_ట్యాగ్_morx HB_ట్యాగ్('m','o','r','x')
#నిర్వచించు HB_AAT_ట్యాగ్_mort HB_ట్యాగ్('m','o','r','t')


నామాకాశ AAT {

ఉపయోగించి నామాకాశ OT;

మూస <రకం_పేరు Types>
నిర్మాణం RearrangementSubtable
{
  రకనిర్వచనం రకం_పేరు Types::HBUINT HBUINT;

  రకనిర్వచనం శూన్య నమోదు_దత్తాంశం;

  గణన Flags
  {
    Mark_మొదటి		= 0x8000,	/* If set, make the current glyph the first
					 * glyph to be rearranged. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. This means
					 * that the glyph index doesn't change, even
					 * if the glyph at that index has changed. */
    Mark_చివరి		= 0x2000,	/* If set, make the current glyph the last
					 * glyph to be rearranged. */
    Reserved		= 0x1FF0,	/* These bits are reserved and should be set to 0. */
    Verb		= 0x000F,	/* The type of rearrangement specified. */
  };

  తార్కిక is_action_initiable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    తిరిగి (నమోదు_2.flags & Mark_మొదటి);
  }
  తార్కిక is_actionable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    తిరిగి (నమోదు_2.flags & Verb);
  }

  నిర్మాణం driver_context_t
  {
    స్థిర constexpr తార్కిక in_place = నిజం;

    driver_context_t (స్థిరరాశి RearrangementSubtable *పట్టిక_2) :
	ret (అబద్ధం),
	పట్టిక (పట్టిక_2),
	ప్రారంభం (0), చివర (0) {}

    HB_AAT_TRANSITION_INLINE
    శూన్య transition (hb_బఫరు_t *బఫరు,
		     State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> *driver,
		     స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
    {
      గుర్తులేని పూర్ణాంకం flags = నమోదు_2.flags;

      ఒకవేళ (flags & Mark_మొదటి)
	ప్రారంభం = బఫరు->idx;

      ఒకవేళ (flags & Mark_చివరి)
	చివర = hb_min (బఫరు->idx + 1, బఫరు->len);

      ఒకవేళ ((flags & Verb) && ప్రారంభం < చివర)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	స్థిరరాశి గుర్తులేని అక్షరం map[16] =
	{
	  0x00,	/* 0	no change */
	  0x10,	/* 1	Ax => xA */
	  0x01,	/* 2	xD => Dx */
	  0x11,	/* 3	AxD => DxA */
	  0x20,	/* 4	ABx => xAB */
	  0x30,	/* 5	ABx => xBA */
	  0x02,	/* 6	xCD => CDx */
	  0x03,	/* 7	xCD => DCx */
	  0x12,	/* 8	AxCD => CDxA */
	  0x13,	/* 9	AxCD => DCxA */
	  0x21,	/* 10	ABxD => DxAB */
	  0x31,	/* 11	ABxD => DxBA */
	  0x22,	/* 12	ABxCD => CDxAB */
	  0x32,	/* 13	ABxCD => CDxBA */
	  0x23,	/* 14	ABxCD => DCxAB */
	  0x33,	/* 15	ABxCD => DCxBA */
	};

	గుర్తులేని పూర్ణాంకం m = map[flags & Verb];
	గుర్తులేని పూర్ణాంకం l = hb_min (2u, m >> 4);
	గుర్తులేని పూర్ణాంకం r = hb_min (2u, m & 0x0F);
	తార్కిక తిరగేయి_l = 3 == (m >> 4);
	తార్కిక తిరగేయి_r = 3 == (m & 0x0F);

	ఒకవేళ (చివర - ప్రారంభం >= l + r && చివర-ప్రారంభం <= HB_MAX_CONTEXT_నిడివి)
	{
	  బఫరు->విలీనం_clusters (ప్రారంభం, hb_min (బఫరు->idx + 1, బఫరు->len));
	  బఫరు->విలీనం_clusters (ప్రారంభం, చివర);

	  hb_glyph_info_t *info = బఫరు->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + ప్రారంభం, l * పరిమాణం (buf[0]));
	  hb_memcpy (buf + 2, info + చివర - r, r * పరిమాణం (buf[0]));

	  ఒకవేళ (l != r)
	    memmove (info + ప్రారంభం + r, info + ప్రారంభం + l, (చివర - ప్రారంభం - l - r) * పరిమాణం (buf[0]));

	  hb_memcpy (info + ప్రారంభం, buf + 2, r * పరిమాణం (buf[0]));
	  hb_memcpy (info + చివర - l, buf, l * పరిమాణం (buf[0]));
	  ఒకవేళ (తిరగేయి_l)
	  {
	    buf[0] = info[చివర - 1];
	    info[చివర - 1] = info[చివర - 2];
	    info[చివర - 2] = buf[0];
	  }
	  ఒకవేళ (తిరగేయి_r)
	  {
	    buf[0] = info[ప్రారంభం];
	    info[ప్రారంభం] = info[ప్రారంభం + 1];
	    info[ప్రారంభం + 1] = buf[0];
	  }
	}
      }
    }

    బహిరంగ:
    తార్కిక ret;
    స్థిరరాశి RearrangementSubtable *పట్టిక;
    అంతరంగ:
    గుర్తులేని పూర్ణాంకం ప్రారంభం;
    గుర్తులేని పూర్ణాంకం చివర;
  };

  తార్కిక apply (hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    driver_context_t dc (ఇది);

    State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (machine.sanitize (c));
  }

  బహిరంగ:
  State_పట్టిక<Types, నమోదు_దత్తాంశం>	machine;
  బహిరంగ:
  DEFINE_కొలత_STATIC ((State_పట్టిక<Types, నమోదు_దత్తాంశం>::static_కొలత));
};

మూస <రకం_పేరు Types>
నిర్మాణం ContextualSubtable
{
  రకనిర్వచనం రకం_పేరు Types::HBUINT HBUINT;

  నిర్మాణం నమోదు_దత్తాంశం
  {
    రకనిర్వచనం రకం_పేరు std::conditional<Types::extended, HBUINT16, HBINT16>::type స్థానభ్రంశం_Type;

    స్థానభ్రంశం_Type	mark_సూచీ_2;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    స్థానభ్రంశం_Type	current_సూచీ;	/* Index/offset of the substitution table for the
				 * current glyph. */
    బహిరంగ:
    DEFINE_కొలత_STATIC (4);
  };

  గణన Flags
  {
    సమితి_Mark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  తార్కిక is_action_initiable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    తిరిగి (నమోదు_2.flags & సమితి_Mark);
  }
  తార్కిక is_actionable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    ఒకవేళ (Types::extended)
      తిరిగి నమోదు_2.దత్తాంశం.mark_సూచీ_2 != 0xFFFF || నమోదు_2.దత్తాంశం.current_సూచీ != 0xFFFF;
    లేకపోతే
      తిరిగి నమోదు_2.దత్తాంశం.mark_సూచీ_2 || నమోదు_2.దత్తాంశం.current_సూచీ;
  }

  నిర్మాణం driver_context_t
  {
    స్థిర constexpr తార్కిక in_place = నిజం;

    driver_context_t (స్థిరరాశి ContextualSubtable *పట్టిక_2,
			     hb_aat_apply_context_t *c_) :
	ret (అబద్ధం),
	c (c_),
	పట్టిక (పట్టిక_2),
	mark_సమితి (అబద్ధం),
	mark (0),
	subs (పట్టిక+పట్టిక->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    శూన్య transition (hb_బఫరు_t *బఫరు,
		     State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> *driver,
		     స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      ఒకవేళ (బఫరు->idx == బఫరు->len && !mark_సమితి)
	తిరిగి;

      స్థిరరాశి HBGlyphID16 *replacement;

      replacement = శూన్యసూచిక;
      ఒకవేళ (Types::extended)
      {
	ఒకవేళ (నమోదు_2.దత్తాంశం.mark_సూచీ_2 != 0xFFFF)
	{
	  స్థిరరాశి Lookup<HBGlyphID16> &lookup = subs[నమోదు_2.దత్తాంశం.mark_సూచీ_2];
	  replacement = lookup.get_విలువ (బఫరు->info[mark].codepoint, driver->అంకె_glyphs);
	}
      }
      లేకపోతే
      {
	ఒకవేళ (నమోదు_2.దత్తాంశం.mark_సూచీ_2)
	{
	  పూర్ణాంకం స్థానభ్రంశం = (పూర్ణాంకం) నమోదు_2.దత్తాంశం.mark_సూచీ_2 + బఫరు->info[mark].codepoint;
	  స్థిరరాశి Unsized_Array_యొక్క<HBGlyphID16> &subs_old = (స్థిరరాశి Unsized_Array_యొక్క<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::మాట_స్థానభ్రంశం_గమ్యం_సూచీ (స్థానభ్రంశం, పట్టిక, subs_old.arrayZ)];
	  ఒకవేళ (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = శూన్యసూచిక;
	}
      }
      ఒకవేళ (replacement)
      {
	బఫరు->unsafe_గమ్యం_break (mark, hb_min (బఫరు->idx + 1, బఫరు->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = నిజం;
      }

      replacement = శూన్యసూచిక;
      గుర్తులేని పూర్ణాంకం idx = hb_min (బఫరు->idx, బఫరు->len - 1);
      ఒకవేళ (Types::extended)
      {
	ఒకవేళ (నమోదు_2.దత్తాంశం.current_సూచీ != 0xFFFF)
	{
	  స్థిరరాశి Lookup<HBGlyphID16> &lookup = subs[నమోదు_2.దత్తాంశం.current_సూచీ];
	  replacement = lookup.get_విలువ (బఫరు->info[idx].codepoint, driver->అంకె_glyphs);
	}
      }
      లేకపోతే
      {
	ఒకవేళ (నమోదు_2.దత్తాంశం.current_సూచీ)
	{
	  పూర్ణాంకం స్థానభ్రంశం = (పూర్ణాంకం) నమోదు_2.దత్తాంశం.current_సూచీ + బఫరు->info[idx].codepoint;
	  స్థిరరాశి Unsized_Array_యొక్క<HBGlyphID16> &subs_old = (స్థిరరాశి Unsized_Array_యొక్క<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::మాట_స్థానభ్రంశం_గమ్యం_సూచీ (స్థానభ్రంశం, పట్టిక, subs_old.arrayZ)];
	  ఒకవేళ (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = శూన్యసూచిక;
	}
      }
      ఒకవేళ (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = నిజం;
      }

      ఒకవేళ (నమోదు_2.flags & సమితి_Mark)
      {
	mark_సమితి = నిజం;
	mark = బఫరు->idx;
      }
    }

    బహిరంగ:
    తార్కిక ret;
    hb_aat_apply_context_t *c;
    స్థిరరాశి ContextualSubtable *పట్టిక;
    అంతరంగ:
    తార్కిక mark_సమితి;
    గుర్తులేని పూర్ణాంకం mark;
    స్థిరరాశి Unsized_List_యొక్క_Offset16_గమ్యం<Lookup<HBGlyphID16>, HBUINT, శూన్య, అబద్ధం> &subs;
  };

  తార్కిక apply (hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    driver_context_t dc (ఇది, c);

    State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);

    గుర్తులేని పూర్ణాంకం అంకె_entries = 0;
    ఒకవేళ (unlikely (!machine.sanitize (c, &అంకె_entries))) return_trace (అబద్ధం);
    hb_barrier ();

    ఒకవేళ (!Types::extended)
      return_trace (substitutionTables.sanitize (c, ఇది, 0));

    గుర్తులేని పూర్ణాంకం అంకె_lookups = 0;

    స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> *entries = machine.get_entries ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_entries; i++)
    {
      స్థిరరాశి నమోదు_దత్తాంశం &దత్తాంశం = entries[i].దత్తాంశం;

      ఒకవేళ (దత్తాంశం.mark_సూచీ_2 != 0xFFFF)
	అంకె_lookups = hb_max (అంకె_lookups, 1u + దత్తాంశం.mark_సూచీ_2);
      ఒకవేళ (దత్తాంశం.current_సూచీ != 0xFFFF)
	అంకె_lookups = hb_max (అంకె_lookups, 1u + దత్తాంశం.current_సూచీ);
    }

    return_trace (substitutionTables.sanitize (c, ఇది, అంకె_lookups));
  }

  బహిరంగ:
  State_పట్టిక<Types, నమోదు_దత్తాంశం>
		machine;
  రక్షిత:
  NNOffset_గమ్యం<Unsized_List_యొక్క_Offset16_గమ్యం<Lookup<HBGlyphID16>, HBUINT, శూన్య, అబద్ధం>, HBUINT>
		substitutionTables;
  బహిరంగ:
  DEFINE_కొలత_STATIC ((State_పట్టిక<Types, నమోదు_దత్తాంశం>::static_కొలత + HBUINT::static_కొలత));
};


మూస <తార్కిక extended>
నిర్మాణం Ligature_నమోదు;

మూస <>
నిర్మాణం Ligature_నమోదు<నిజం>
{

  నిర్మాణం నమోదు_దత్తాంశం
  {
    HBUINT16	lig_Action_సూచీ;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    బహిరంగ:
    DEFINE_కొలత_STATIC (2);
  };

  గణన Flags
  {
    సమితి_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  స్థిర తార్కిక initiateAction (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.flags & సమితి_Component; }

  స్థిర తార్కిక performAction (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.flags & PerformAction; }

  స్థిర గుర్తులేని పూర్ణాంకం lig_Action_సూచీ (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.దత్తాంశం.lig_Action_సూచీ; }
};
మూస <>
నిర్మాణం Ligature_నమోదు<అబద్ధం>
{
  రకనిర్వచనం శూన్య నమోదు_దత్తాంశం;

  గణన Flags
  {
    సమితి_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    స్థానభ్రంశం_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  స్థిర తార్కిక initiateAction (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.flags & సమితి_Component; }

  స్థిర తార్కిక performAction (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.flags & స్థానభ్రంశం_2; }

  స్థిర గుర్తులేని పూర్ణాంకం lig_Action_సూచీ (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
  { తిరిగి నమోదు_2.flags & స్థానభ్రంశం_2; }
};


మూస <రకం_పేరు Types>
నిర్మాణం LigatureSubtable
{
  రకనిర్వచనం రకం_పేరు Types::HBUINT HBUINT;

  రకనిర్వచనం Ligature_నమోదు<Types::extended> Ligature_నమోదు_T;
  రకనిర్వచనం రకం_పేరు Ligature_నమోదు_T::నమోదు_దత్తాంశం నమోదు_దత్తాంశం;

  గణన Flags
  {
    DontAdvance	= Ligature_నమోదు_T::DontAdvance,
  };

  తార్కిక is_action_initiable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    తిరిగి Ligature_నమోదు_T::initiateAction (నమోదు_2);
  }
  తార్కిక is_actionable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    తిరిగి Ligature_నమోదు_T::performAction (నమోదు_2);
  }

  నిర్మాణం driver_context_t
  {
    స్థిర constexpr తార్కిక in_place = అబద్ధం;
    గణన LigActionFlags
    {
      Lig_Action_చివరి	= 0x80000000,	/* This is the last action in the list. This also
					 * implies storage. */
      LigActionStore	= 0x40000000,	/* Store the ligature at the current cumulated index
					 * in the ligature table in place of the marked
					 * (i.e. currently-popped) glyph. */
      Lig_Action_స్థానభ్రంశం	= 0x3FFFFFFF,	/* A 30-bit value which is sign-extended to 32-bits
					 * and added to the glyph ID, resulting in an index
					 * into the component table. */
    };

    driver_context_t (స్థిరరాశి LigatureSubtable *పట్టిక_2,
		      hb_aat_apply_context_t *c_) :
	ret (అబద్ధం),
	c (c_),
	పట్టిక (పట్టిక_2),
	ligAction (పట్టిక+పట్టిక->ligAction),
	component (పట్టిక+పట్టిక->component),
	ligature (పట్టిక+పట్టిక->ligature),
	సరిపోలిక_నిడివి (0) {}

    HB_AAT_TRANSITION_INLINE
    శూన్య transition (hb_బఫరు_t *బఫరు,
		     State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> *driver,
		     స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
    {
      DEBUG_సందేశం (APPLY, శూన్యసూచిక, "Ligature transition at %u", బఫరు->idx);
      ఒకవేళ (నమోదు_2.flags & Ligature_నమోదు_T::సమితి_Component)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	ఒకవేళ (unlikely (సరిపోలిక_నిడివి && సరిపోలిక_positions[(సరిపోలిక_నిడివి - 1u) % ARRAY_నిడివి (సరిపోలిక_positions)] == బఫరు->బహిర్గతం_len))
	  సరిపోలిక_నిడివి--;

	సరిపోలిక_positions[సరిపోలిక_నిడివి++ % ARRAY_నిడివి (సరిపోలిక_positions)] = బఫరు->బహిర్గతం_len;
	DEBUG_సందేశం (APPLY, శూన్యసూచిక, "Set component at %u", బఫరు->బహిర్గతం_len);
      }

      ఒకవేళ (Ligature_నమోదు_T::performAction (నమోదు_2))
      {
	DEBUG_సందేశం (APPLY, శూన్యసూచిక, "Perform action with %u", సరిపోలిక_నిడివి);
	గుర్తులేని పూర్ణాంకం చివర = బఫరు->బహిర్గతం_len;

	ఒకవేళ (unlikely (!సరిపోలిక_నిడివి))
	  తిరిగి;

	ఒకవేళ (బఫరు->idx >= బఫరు->len)
	  తిరిగి; /* TODO Work on previous instead? */

	గుర్తులేని పూర్ణాంకం cursor = సరిపోలిక_నిడివి;

	గుర్తులేని పూర్ణాంకం action_idx = Ligature_నమోదు_T::lig_Action_సూచీ (నమోదు_2);
	action_idx = Types::స్థానభ్రంశం_గమ్యం_సూచీ (action_idx, పట్టిక, ligAction.arrayZ);
	స్థిరరాశి HBUINT32 *action_దత్తాంశం = &ligAction[action_idx];

	గుర్తులేని పూర్ణాంకం ligature_idx = 0;
	గుర్తులేని పూర్ణాంకం action;
	చేయి
	{
	  ఒకవేళ (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_సందేశం (APPLY, శూన్యసూచిక, "Stack underflow");
	    సరిపోలిక_నిడివి = 0;
	    ఆపు;
	  }

	  DEBUG_సందేశం (APPLY, శూన్యసూచిక, "Moving to stack position %u", cursor - 1);
	  ఒకవేళ (unlikely (!బఫరు->move_గమ్యం (సరిపోలిక_positions[--cursor % ARRAY_నిడివి (సరిపోలిక_positions)]))) తిరిగి;

	  ఒకవేళ (unlikely (!action_దత్తాంశం->sanitize (&c->sanitizer))) ఆపు;
	  hb_barrier ();
	  action = *action_దత్తాంశం;

	  uint32_t uoffset = action & Lig_Action_స్థానభ్రంశం;
	  ఒకవేళ (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t స్థానభ్రంశం = (int32_t) uoffset;
	  గుర్తులేని పూర్ణాంకం component_idx = బఫరు->cur().codepoint + స్థానభ్రంశం;
	  component_idx = Types::మాట_స్థానభ్రంశం_గమ్యం_సూచీ (component_idx, పట్టిక, component.arrayZ);
	  స్థిరరాశి HBUINT16 &component_దత్తాంశం = component[component_idx];
	  ఒకవేళ (unlikely (!component_దత్తాంశం.sanitize (&c->sanitizer))) ఆపు;
	  hb_barrier ();
	  ligature_idx += component_దత్తాంశం;

	  DEBUG_సందేశం (APPLY, శూన్యసూచిక, "Action store %d last %d",
		     తార్కిక (action & LigActionStore),
		     తార్కిక (action & Lig_Action_చివరి));
	  ఒకవేళ (action & (LigActionStore | Lig_Action_చివరి))
	  {
	    ligature_idx = Types::స్థానభ్రంశం_గమ్యం_సూచీ (ligature_idx, పట్టిక, ligature.arrayZ);
	    స్థిరరాశి HBGlyphID16 &ligature_దత్తాంశం = ligature[ligature_idx];
	    ఒకవేళ (unlikely (!ligature_దత్తాంశం.sanitize (&c->sanitizer))) ఆపు;
	    hb_barrier ();
	    hb_codepoint_t lig = ligature_దత్తాంశం;

	    DEBUG_సందేశం (APPLY, శూన్యసూచిక, "Produced ligature %u", lig);
	    ఒకవేళ (unlikely (!c->replace_glyph (lig))) తిరిగి;

	    గుర్తులేని పూర్ణాంకం lig_చివర = సరిపోలిక_positions[(సరిపోలిక_నిడివి - 1u) % ARRAY_నిడివి (సరిపోలిక_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    అయితే (సరిపోలిక_నిడివి - 1u > cursor)
	    {
	      DEBUG_సందేశం (APPLY, శూన్యసూచిక, "Skipping ligature component");
	      ఒకవేళ (unlikely (!బఫరు->move_గమ్యం (సరిపోలిక_positions[--సరిపోలిక_నిడివి % ARRAY_నిడివి (సరిపోలిక_positions)]))) తిరిగి;
	      ఒకవేళ (!c->delete_glyph ()) తిరిగి;
	    }

	    ఒకవేళ (unlikely (!బఫరు->move_గమ్యం (lig_చివర))) తిరిగి;
	    బఫరు->విలీనం_బహిర్గతం_clusters (సరిపోలిక_positions[cursor % ARRAY_నిడివి (సరిపోలిక_positions)], బఫరు->బహిర్గతం_len);
	  }

	  action_దత్తాంశం++;
	}
	అయితే (!(action & Lig_Action_చివరి));
	ఒకవేళ (unlikely (!బఫరు->move_గమ్యం (చివర))) తిరిగి;
      }
    }

    బహిరంగ:
    తార్కిక ret;
    hb_aat_apply_context_t *c;
    స్థిరరాశి LigatureSubtable *పట్టిక;
    అంతరంగ:
    స్థిరరాశి Unsized_Array_యొక్క<HBUINT32> &ligAction;
    స్థిరరాశి Unsized_Array_యొక్క<HBUINT16> &component;
    స్థిరరాశి Unsized_Array_యొక్క<HBGlyphID16> &ligature;
    గుర్తులేని పూర్ణాంకం సరిపోలిక_నిడివి;
    గుర్తులేని పూర్ణాంకం సరిపోలిక_positions[HB_MAX_CONTEXT_నిడివి];
  };

  తార్కిక apply (hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    driver_context_t dc (ఇది, c);

    State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (ఇది) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  బహిరంగ:
  State_పట్టిక<Types, నమోదు_దత్తాంశం>
		machine;
  రక్షిత:
  NNOffset_గమ్యం<Unsized_Array_యొక్క<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffset_గమ్యం<Unsized_Array_యొక్క<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffset_గమ్యం<Unsized_Array_యొక్క<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  బహిరంగ:
  DEFINE_కొలత_STATIC ((State_పట్టిక<Types, నమోదు_దత్తాంశం>::static_కొలత + 3 * HBUINT::static_కొలత));
};

మూస <రకం_పేరు Types>
నిర్మాణం NoncontextualSubtable
{
  తార్కిక apply (hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    తార్కిక ret = అబద్ధం;
    గుర్తులేని పూర్ణాంకం అంకె_glyphs = c->face->get_అంకె_glyphs ();

    hb_glyph_info_t *info = c->బఫరు->info;
    గుర్తులేని పూర్ణాంకం లెక్క = c->బఫరు->len;
    // If there's only one range, we already checked the flag.
    స్వయంచాలక *చివరి_range = c->range_flags && (c->range_flags->నిడివి > 1) ? &(*c->range_flags)[0] : శూన్యసూచిక;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      ఒకవేళ (unlikely (చివరి_range))
      {
	స్వయంచాలక *range = చివరి_range;
	{
	  గుర్తులేని cluster = info[i].cluster;
	  అయితే (cluster < range->cluster_మొదటి)
	    range--;
	  అయితే (cluster > range->cluster_చివరి)
	    range++;

	  చివరి_range = range;
	}
	ఒకవేళ (!(range->flags & c->subtable_flags))
	  కొనసాగు;
      }

      స్థిరరాశి HBGlyphID16 *replacement = substitute.get_విలువ (info[i].codepoint, అంకె_glyphs);
      ఒకవేళ (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = నిజం;
      }
    }

    return_trace (ret);
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_initial_glyphs (సమితి_t &glyphs, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    substitute.collect_glyphs (glyphs, అంకె_glyphs);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (substitute.sanitize (c));
  }

  రక్షిత:
  Lookup<HBGlyphID16>	substitute;
  బహిరంగ:
  DEFINE_కొలత_MIN (2);
};

మూస <రకం_పేరు Types>
నిర్మాణం InsertionSubtable
{
  రకనిర్వచనం రకం_పేరు Types::HBUINT HBUINT;

  నిర్మాణం నమోదు_దత్తాంశం
  {
    HBUINT16	current_చొప్పించు_సూచీ;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_చొప్పించు_సూచీ;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    బహిరంగ:
    DEFINE_కొలత_STATIC (4);
  };

  గణన Flags
  {
    సమితి_Mark		= 0x8000,     /* If set, mark the current glyph. */
    DontAdvance		= 0x4000,     /* If set, don't advance to the next glyph before
				       * going to the new state.  This does not mean
				       * that the glyph pointed to is the same one as
				       * before. If you've made insertions immediately
				       * downstream of the current glyph, the next glyph
				       * processed would in fact be the first one
				       * inserted. */
    CurrentIsKashidaLike= 0x2000,     /* If set, and the currentInsertList is nonzero,
				       * then the specified glyph list will be inserted
				       * as a kashida-like insertion, either before or
				       * after the current glyph (depending on the state
				       * of the currentInsertBefore flag). If clear, and
				       * the currentInsertList is nonzero, then the
				       * specified glyph list will be inserted as a
				       * split-vowel-like insertion, either before or
				       * after the current glyph (depending on the state
				       * of the currentInsertBefore flag). */
    MarkedIsKashidaLike= 0x1000,      /* If set, and the markedInsertList is nonzero,
				       * then the specified glyph list will be inserted
				       * as a kashida-like insertion, either before or
				       * after the marked glyph (depending on the state
				       * of the markedInsertBefore flag). If clear, and
				       * the markedInsertList is nonzero, then the
				       * specified glyph list will be inserted as a
				       * split-vowel-like insertion, either before or
				       * after the marked glyph (depending on the state
				       * of the markedInsertBefore flag). */
    Current_చొప్పించు_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_చొప్పించు_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_చొప్పించు_లెక్క= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_చొప్పించు_లెక్క= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  తార్కిక is_action_initiable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    తిరిగి (నమోదు_2.flags & సమితి_Mark);
  }
  తార్కిక is_actionable (స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2) స్థిరరాశి
  {
    తిరిగి (నమోదు_2.flags & (Current_చొప్పించు_లెక్క | Marked_చొప్పించు_లెక్క)) &&
	   (నమోదు_2.దత్తాంశం.current_చొప్పించు_సూచీ != 0xFFFF ||నమోదు_2.దత్తాంశం.marked_చొప్పించు_సూచీ != 0xFFFF);
  }

  నిర్మాణం driver_context_t
  {
    స్థిర constexpr తార్కిక in_place = అబద్ధం;

    driver_context_t (స్థిరరాశి InsertionSubtable *పట్టిక_2,
		      hb_aat_apply_context_t *c_) :
	ret (అబద్ధం),
	c (c_),
	పట్టిక (పట్టిక_2),
	mark (0),
	insertionAction (పట్టిక+పట్టిక->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    శూన్య transition (hb_బఫరు_t *బఫరు,
		     State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> *driver,
		     స్థిరరాశి నమోదు_3<నమోదు_దత్తాంశం> &నమోదు_2)
    {
      గుర్తులేని పూర్ణాంకం flags = నమోదు_2.flags;

      గుర్తులేని mark_loc = బఫరు->బహిర్గతం_len;

      ఒకవేళ (నమోదు_2.దత్తాంశం.marked_చొప్పించు_సూచీ != 0xFFFF)
      {
	గుర్తులేని పూర్ణాంకం లెక్క = (flags & Marked_చొప్పించు_లెక్క);
	ఒకవేళ (unlikely ((బఫరు->max_ops -= లెక్క) <= 0)) తిరిగి;
	గుర్తులేని పూర్ణాంకం ప్రారంభం = నమోదు_2.దత్తాంశం.marked_చొప్పించు_సూచీ;
	స్థిరరాశి HBGlyphID16 *glyphs = &insertionAction[ప్రారంభం];
	ఒకవేళ (unlikely (!c->sanitizer.check_array (glyphs, లెక్క))) లెక్క = 0;
	hb_barrier ();

	తార్కిక before = flags & Marked_చొప్పించు_Before;

	గుర్తులేని పూర్ణాంకం చివర = బఫరు->బహిర్గతం_len;
	ఒకవేళ (unlikely (!బఫరు->move_గమ్యం (mark))) తిరిగి;

	ఒకవేళ (బఫరు->idx < బఫరు->len && !before)
	  ఒకవేళ (unlikely (!బఫరు->copy_glyph ())) తిరిగి;
	/* TODO We ignore KashidaLike setting. */
	ఒకవేళ (unlikely (!c->output_glyphs (లెక్క, glyphs))) తిరిగి;
	ret = నిజం;
	ఒకవేళ (బఫరు->idx < బఫరు->len && !before)
	  బఫరు->skip_glyph ();

	ఒకవేళ (unlikely (!బఫరు->move_గమ్యం (చివర + లెక్క))) తిరిగి;

	బఫరు->unsafe_గమ్యం_break_from_outbuffer (mark, hb_min (బఫరు->idx + 1, బఫరు->len));
      }

      ఒకవేళ (flags & సమితి_Mark)
	mark = mark_loc;

      ఒకవేళ (నమోదు_2.దత్తాంశం.current_చొప్పించు_సూచీ != 0xFFFF)
      {
	గుర్తులేని పూర్ణాంకం లెక్క = (flags & Current_చొప్పించు_లెక్క) >> 5;
	ఒకవేళ (unlikely ((బఫరు->max_ops -= లెక్క) <= 0)) తిరిగి;
	గుర్తులేని పూర్ణాంకం ప్రారంభం = నమోదు_2.దత్తాంశం.current_చొప్పించు_సూచీ;
	స్థిరరాశి HBGlyphID16 *glyphs = &insertionAction[ప్రారంభం];
	ఒకవేళ (unlikely (!c->sanitizer.check_array (glyphs, లెక్క))) లెక్క = 0;
	hb_barrier ();

	తార్కిక before = flags & Current_చొప్పించు_Before;

	గుర్తులేని పూర్ణాంకం చివర = బఫరు->బహిర్గతం_len;

	ఒకవేళ (బఫరు->idx < బఫరు->len && !before)
	  ఒకవేళ (unlikely (!బఫరు->copy_glyph ())) తిరిగి;
	/* TODO We ignore KashidaLike setting. */
	ఒకవేళ (unlikely (!c->output_glyphs (లెక్క, glyphs))) తిరిగి;
	ret = నిజం;
	ఒకవేళ (బఫరు->idx < బఫరు->len && !before)
	  బఫరు->skip_glyph ();

	/* Humm. Not sure where to move to.  There's this wording under
	 * DontAdvance flag:
	 *
	 * "If set, don't update the glyph index before going to the new state.
	 * This does not mean that the glyph pointed to is the same one as
	 * before. If you've made insertions immediately downstream of the
	 * current glyph, the next glyph processed would in fact be the first
	 * one inserted."
	 *
	 * This suggests that if DontAdvance is NOT set, we should move to
	 * end+count.  If it *was*, then move to end, such that newly inserted
	 * glyphs are now visible.
	 *
	 * https://github.com/harfbuzz/harfbuzz/issues/1224#issuecomment-427691417
	 */
	ఒకవేళ (unlikely (!బఫరు->move_గమ్యం ((flags & DontAdvance) ? చివర : చివర + లెక్క))) తిరిగి;
      }
    }

    బహిరంగ:
    తార్కిక ret;
    hb_aat_apply_context_t *c;
    స్థిరరాశి InsertionSubtable *పట్టిక;
    అంతరంగ:
    గుర్తులేని పూర్ణాంకం mark;
    స్థిరరాశి Unsized_Array_యొక్క<HBGlyphID16> &insertionAction;
  };

  తార్కిక apply (hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    driver_context_t dc (ఇది, c);

    State_పట్టిక_Driver<Types, నమోదు_దత్తాంశం, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (ఇది) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  బహిరంగ:
  State_పట్టిక<Types, నమోదు_దత్తాంశం>
		machine;
  రక్షిత:
  NNOffset_గమ్యం<Unsized_Array_యొక్క<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  బహిరంగ:
  DEFINE_కొలత_STATIC ((State_పట్టిక<Types, నమోదు_దత్తాంశం>::static_కొలత + HBUINT::static_కొలత));
};


నిర్మాణం Feature
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};


నిర్మాణం hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  నిర్మాణం hb_applicable_t
  {
    మిత్ర నిర్మాణం hb_accelerate_subtables_context_t;
    మిత్ర నిర్మాణం hb_aat_layout_lookup_accelerator_t;

    బహిరంగ:
    hb_బిట్టు_సమితి_t glyph_సమితి;
    మార్చదగిన hb_aat_class_cache_t class_cache;

    మూస <రకం_పేరు T>
    స్వయంచాలక init_ (స్థిరరాశి T &obj_, గుర్తులేని అంకె_glyphs, hb_ప్రాధాన్యత<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_సమితి, అంకె_glyphs, obj_)
    )

    మూస <రకం_పేరు T>
    శూన్య init_ (స్థిరరాశి T &obj_, గుర్తులేని అంకె_glyphs, hb_ప్రాధాన్యత<0>)
    {
      obj_.collect_initial_glyphs (glyph_సమితి, అంకె_glyphs);
    }

    మూస <రకం_పేరు T>
    శూన్య init (స్థిరరాశి T &obj_, గుర్తులేని అంకె_glyphs)
    {
      glyph_సమితి.init ();
      init_ (obj_, అంకె_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    శూన్య
    fini ()
    {
      glyph_సమితి.fini ();
    }
  };

  /* Dispatch interface. */
  మూస <రకం_పేరు T>
  return_t dispatch (స్థిరరాశి T &obj)
  {
    hb_applicable_t *నమోదు_2 = &array[i++];

    నమోదు_2->init (obj, అంకె_glyphs);

    తిరిగి hb_ఖాళీలు_t ();
  }
  స్థిర return_t default_return_విలువ () { తిరిగి hb_ఖాళీలు_t (); }

  తార్కిక stop_sublookup_iteration (return_t r) స్థిరరాశి { తిరిగి అబద్ధం; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, గుర్తులేని అంకె_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), అంకె_glyphs (అంకె_glyphs_2) {}

  hb_applicable_t *array;
  గుర్తులేని అంకె_glyphs;
  గుర్తులేని i = 0;
};

నిర్మాణం hb_aat_layout_chain_accelerator_t
{
  మూస <రకం_పేరు TChain>
  స్థిర hb_aat_layout_chain_accelerator_t *create (స్థిరరాశి TChain &chain, గుర్తులేని అంకె_glyphs)
  {
    గుర్తులేని లెక్క = chain.get_subtable_లెక్క ();

    గుర్తులేని లబ్ధం;
    ఒకవేళ (unlikely (hb_unsigned_mul_overflows (లెక్క,
        పరిమాణం (hb_accelerate_subtables_context_t::hb_applicable_t), &లబ్ధం)))
      తిరిగి శూన్యసూచిక;

    గుర్తులేని కొలత;
    ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (పరిమాణం (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * పరిమాణం (hb_accelerate_subtables_context_t::hb_applicable_t), లబ్ధం, &కొలత))) {
      తిరిగి శూన్యసూచిక;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    స్వయంచాలక *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, కొలత);
    ఒకవేళ (unlikely (!thiz))
      తిరిగి శూన్యసూచిక;

    thiz->లెక్క = లెక్క;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, అంకె_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    తిరిగి thiz;
  }

  శూన్య destroy ()
  {
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      subtables[i].fini ();
  }

  గుర్తులేని లెక్క;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

మూస <రకం_పేరు Types>
నిర్మాణం ChainSubtable
{
  రకనిర్వచనం రకం_పేరు Types::HBUINT HBUINT;

  మూస <రకం_పేరు T>
  మిత్ర నిర్మాణం Chain;

  size_t get_కొలత () స్థిరరాశి     { తిరిగి నిడివి; }
  గుర్తులేని పూర్ణాంకం get_type () స్థిరరాశి     { తిరిగి coverage & 0xFF; }
  గుర్తులేని పూర్ణాంకం get_coverage () స్థిరరాశి { తిరిగి coverage >> (పరిమాణం (HBUINT) * 8 - 8); }

  గణన Coverage
  {
    Vertical		= 0x80,	/* If set, this subtable will only be applied
				 * to vertical text. If clear, this subtable
				 * will only be applied to horizontal text. */
    Backwards		= 0x40,	/* If set, this subtable will process glyphs
				 * in descending order. If clear, it will
				 * process the glyphs in ascending order. */
    AllDirections	= 0x20,	/* If set, this subtable will be applied to
				 * both horizontal and vertical text (i.e.
				 * the state of bit 0x80000000 is ignored). */
    Logical		= 0x10,	/* If set, this subtable will process glyphs
				 * in logical order (or reverse logical order,
				 * depending on the value of bit 0x80000000). */
  };
  గణన Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం subtable_type = get_type ();
    TRACE_DISPATCH (ఇది, subtable_type);
    ఎంపిక (subtable_type) {
    స్థితి Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    స్థితి Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    స్థితి Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    స్థితి Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    స్థితి Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    అప్రమేయ:			return_trace (c->default_return_విలువ ());
    }
  }

  తార్కిక apply (hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!(నిడివి.sanitize (c) &&
	  hb_barrier () &&
	  నిడివి >= min_కొలత &&
	  c->check_range (ఇది, నిడివి)))
      return_trace (అబద్ధం);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  రక్షిత:
  HBUINT	నిడివి;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  సంఘం {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  బహిరంగ:
  DEFINE_కొలత_MIN (2 * పరిమాణం (HBUINT) + 4);
};

మూస <రకం_పేరు Types>
నిర్మాణం Chain
{
  రకనిర్వచనం రకం_పేరు Types::HBUINT HBUINT;

  గుర్తులేని get_subtable_లెక్క () స్థిరరాశి { తిరిగి subtable_లెక్క_2; }

  hb_ముసుగు_t compile_flags (స్థిరరాశి hb_aat_map_builder_t *map) స్థిరరాశి
  {
    hb_ముసుగు_t flags = defaultFlags;
    {
      గుర్తులేని పూర్ణాంకం లెక్క = feature_లెక్క;
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      {
	స్థిరరాశి Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (గుర్తులేని పూర్ణాంకం) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (గుర్తులేని పూర్ణాంకం) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, అబద్ధం, 0 };
	ఒకవేళ (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	లేకపోతే ఒకవేళ (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_క్రింది_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_క్రింది_CASE_SMALL_CAPS;
	  వెళ్ళు retry;
	}
#నిర్వచితం_కాకపోతే HB_NO_AAT
	లేకపోతే ఒకవేళ (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_ట్యాగ్_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->పట్టిక.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#అయితే_ముగింపు
      }
    }
    తిరిగి flags;
  }

  శూన్య apply (hb_aat_apply_context_t *c,
	      స్థిరరాశి hb_aat_layout_chain_accelerator_t *accel) స్థిరరాశి
  {
    స్థిరరాశి ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_లెక్క));
    గుర్తులేని పూర్ణాంకం లెక్క = subtable_లెక్క_2;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      తార్కిక reverse;

      స్వయంచాలక coverage = subtable->get_coverage ();

      hb_ముసుగు_t subtable_flags = subtable->subFeatureFlags;
      ఒకవేళ (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (స్థిరరాశి hb_aat_map_t::range_flags_t _) -> తార్కిక { తిరిగి subtable_flags & (_.flags); })))
	వెళ్ళు skip;

      ఒకవేళ (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->బఫరు->props.direction) !=
	  తార్కిక (coverage & ChainSubtable<Types>::Vertical))
	వెళ్ళు skip;

      c->subtable_flags = subtable_flags;
      c->మొదటి_సమితి = accel ? &accel->subtables[i].glyph_సమితి : &Null(hb_బిట్టు_సమితి_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : శూన్యసూచిక;

      ఒకవేళ (!c->బఫరు_intersects_machine ())
      {
	(శూన్య) c->బఫరు->సందేశం (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_సూచీ);
	వెళ్ళు skip;
      }

      /* Buffer contents is always in logical direction.  Determine if
       * we need to reverse before applying this subtable.  We reverse
       * back after if we did reverse indeed.
       *
       * Quoting the spac:
       * """
       * Bits 28 and 30 of the coverage field control the order in which
       * glyphs are processed when the subtable is run by the layout engine.
       * Bit 28 is used to indicate if the glyph processing direction is
       * the same as logical order or layout order. Bit 30 is used to
       * indicate whether glyphs are processed forwards or backwards within
       * that order.

		Bit 30	Bit 28	Interpretation for Horizontal Text
		0	0	The subtable is processed in layout order
				(the same order as the glyphs, which is
				always left-to-right).
		1	0	The subtable is processed in reverse layout order
				(the order opposite that of the glyphs, which is
				always right-to-left).
		0	1	The subtable is processed in logical order
				(the same order as the characters, which may be
				left-to-right or right-to-left).
		1	1	The subtable is processed in reverse logical order
				(the order opposite that of the characters, which
				may be right-to-left or left-to-right).
       */
      reverse = coverage & ChainSubtable<Types>::Logical ?
		తార్కిక (coverage & ChainSubtable<Types>::Backwards) :
		తార్కిక (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->బఫరు->props.direction);

      ఒకవేళ (!c->బఫరు->సందేశం (c->font, "start chainsubtable %u", c->lookup_సూచీ))
	వెళ్ళు skip;

      ఒకవేళ (reverse != c->బఫరు_is_reversed)
        c->తిరగేయి_బఫరు ();

      subtable->apply (c);

      (శూన్య) c->బఫరు->సందేశం (c->font, "end chainsubtable %u", c->lookup_సూచీ);

      ఒకవేళ (unlikely (!c->బఫరు->successful)) ఆపు;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->సమితి_lookup_సూచీ (c->lookup_సూచీ + 1);
    }
    ఒకవేళ (c->బఫరు_is_reversed)
      c->తిరగేయి_బఫరు ();
  }

  size_t get_కొలత () స్థిరరాశి { తిరిగి నిడివి; }

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    స్థిరరాశి ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_లెక్క));
    గుర్తులేని పూర్ణాంకం లెక్క = subtable_లెక్క_2;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      రకం_పేరు context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      ఒకవేళ (c->stop_sublookup_iteration (ret))
	తిరిగి ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    తిరిగి c->default_return_విలువ ();
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం సంస్కరణ) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!(నిడివి.sanitize (c) &&
	  hb_barrier () &&
	  నిడివి >= min_కొలత &&
	  c->check_range (ఇది, నిడివి)))
      return_trace (అబద్ధం);

    ఒకవేళ (!c->check_array (featureZ.arrayZ, feature_లెక్క))
      return_trace (అబద్ధం);

    స్థిరరాశి ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_లెక్క));
    గుర్తులేని పూర్ణాంకం లెక్క = subtable_లెక్క_2;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (!subtable->sanitize (c))
	return_trace (అబద్ధం);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    ఒకవేళ (సంస్కరణ >= 3)
    {
      స్థిరరాశి SubtableGlyphCoverage *coverage = (స్థిరరాశి SubtableGlyphCoverage *) subtable;
      ఒకవేళ (!coverage->sanitize (c, లెక్క))
        return_trace (అబద్ధం);
    }

    return_trace (నిజం);
  }

  రక్షిత:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	నిడివి;		/* Total byte count, including this header. */
  HBUINT	feature_లెక్క;	/* Number of feature subtable entries. */
  HBUINT	subtable_లెక్క_2;	/* The number of subtables in the chain. */

  Unsized_Array_యొక్క<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  బహిరంగ:
  DEFINE_కొలత_MIN (8 + 2 * పరిమాణం (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

మూస <రకం_పేరు T, రకం_పేరు Types, hb_ట్యాగ్_t ట్యాగ్_3>
నిర్మాణం mortmorx
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = ట్యాగ్_3;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ != 0; }

  నిర్మాణం accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ఇది->పట్టిక = sc.reference_పట్టిక<T> (face);

      ఒకవేళ (unlikely (ఇది->పట్టిక->is_blocklisted (ఇది->పట్టిక.get_blob (), face)))
      {
        hb_blob_destroy (ఇది->పట్టిక.get_blob ());
        ఇది->పట్టిక = hb_blob_get_ఖాళీలు ();
      }

      ఇది->chain_లెక్క = పట్టిక->get_chain_లెక్క ();

      ఇది->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (ఇది->chain_లెక్క, పరిమాణం (*accels));
      ఒకవేళ (unlikely (!ఇది->accels))
      {
	ఇది->chain_లెక్క = 0;
	ఇది->పట్టిక.destroy ();
	ఇది->పట్టిక = hb_blob_get_ఖాళీలు ();
      }
    }
    ~accelerator_t ()
    {
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ఇది->chain_లెక్క; i++)
      {
	ఒకవేళ (ఇది->accels[i])
	  ఇది->accels[i]->destroy ();
	hb_free (ఇది->accels[i]);
      }
      hb_free (ఇది->accels);
      ఇది->పట్టిక.destroy ();
    }

    hb_blob_t *get_blob () స్థిరరాశి { తిరిగి పట్టిక.get_blob (); }

    మూస <రకం_పేరు Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (గుర్తులేని chain_సూచీ, స్థిరరాశి Chain &chain, గుర్తులేని అంకె_glyphs) స్థిరరాశి
    {
      ఒకవేళ (unlikely (chain_సూచీ >= chain_లెక్క)) తిరిగి శూన్యసూచిక;

    retry:
      స్వయంచాలక *accel = accels[chain_సూచీ].get_acquire ();
      ఒకవేళ (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, అంకె_glyphs);
	ఒకవేళ (unlikely (!accel))
	  తిరిగి శూన్యసూచిక;

	ఒకవేళ (unlikely (!accels[chain_సూచీ].cmpexch (శూన్యసూచిక, accel)))
	{
	  hb_free (accel);
	  వెళ్ళు retry;
	}
      }

      తిరిగి accel;
    }

    hb_blob_ptr_t<T> పట్టిక;
    గుర్తులేని పూర్ణాంకం chain_లెక్క;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  శూన్య compile_flags (స్థిరరాశి hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) స్థిరరాశి
  {
    స్థిరరాశి Chain<Types> *chain = &మొదటి_Chain;
    గుర్తులేని పూర్ణాంకం లెక్క = chain_లెక్క_2;
    ఒకవేళ (unlikely (!map->chain_flags.resize (లెక్క)))
      తిరిగి;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_మొదటి,
							     mapper->range_చివరి});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  గుర్తులేని get_chain_లెక్క () స్థిరరాశి
  {
    తిరిగి chain_లెక్క_2;
  }
  శూన్య apply (hb_aat_apply_context_t *c,
	      స్థిరరాశి hb_aat_map_t &map,
	      స్థిరరాశి accelerator_t &accel) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!c->బఫరు->successful)) తిరిగి;

    c->బఫరు->unsafe_గమ్యం_concat ();

    c->అమరిక_బఫరు_glyph_సమితి ();

    c->సమితి_lookup_సూచీ (0);
    స్థిరరాశి Chain<Types> *chain = &మొదటి_Chain;
    గుర్తులేని పూర్ణాంకం లెక్క = chain_లెక్క_2;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      స్వయంచాలక *chain_accel = accel.get_accel (i, *chain, c->face->get_అంకె_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      ఒకవేళ (unlikely (!c->బఫరు->successful)) తిరిగి;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!(సంస్కరణ.sanitize (c) &&
	  hb_barrier () &&
	  సంస్కరణ &&
	  chain_లెక్క_2.sanitize (c)))
      return_trace (అబద్ధం);

    స్థిరరాశి Chain<Types> *chain = &మొదటి_Chain;
    గుర్తులేని పూర్ణాంకం లెక్క = chain_లెక్క_2;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (!chain->sanitize (c, సంస్కరణ))
	return_trace (అబద్ధం);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (నిజం);
  }

  రక్షిత:
  HBUINT16	సంస్కరణ;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	వాడని_2;		/* Set to 0. */
  HBUINT32	chain_లెక్క_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	మొదటి_Chain;	/* Chains. */

  బహిరంగ:
  DEFINE_కొలత_MIN (8);
};

నిర్మాణం morx : mortmorx<morx, ExtendedTypes, HB_AAT_ట్యాగ్_morx>
{
  HB_అంతర్గత తార్కిక is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) స్థిరరాశి;
};

నిర్మాణం mort : mortmorx<mort, ObsoleteTypes, HB_AAT_ట్యాగ్_mort>
{
  HB_అంతర్గత తార్కిక is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) స్థిరరాశి;
};

నిర్మాణం morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
నిర్మాణం mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#అయితే_ముగింపు /* HB_AAT_LAYOUT_MORX_TABLE_HH */
