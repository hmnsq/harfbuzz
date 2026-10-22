/*
 * Copyright © 2010,2012  Google, Inc.
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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE

#అన్తర్భావ "hb-ot-shaper.hh"


/* Thai / Lao shaper */


/* PUA shaping */


గణన thai_consonant_type_t
{
  NC,
  AC,
  RC,
  DC,
  NOT_CONSONANT,
  అంకె_CONSONANT_TYPES = NOT_CONSONANT
};

స్థిర thai_consonant_type_t
get_consonant_type (hb_codepoint_t u)
{
  ఒకవేళ (u == 0x0E1Bu || u == 0x0E1Du || u == 0x0E1Fu/* || u == 0x0E2Cu*/)
    తిరిగి AC;
  ఒకవేళ (u == 0x0E0Du || u == 0x0E10u)
    తిరిగి RC;
  ఒకవేళ (u == 0x0E0Eu || u == 0x0E0Fu)
    తిరిగి DC;
  ఒకవేళ (hb_లోపల_range<hb_codepoint_t> (u, 0x0E01u, 0x0E2Eu))
    తిరిగి NC;
  తిరిగి NOT_CONSONANT;
}


గణన thai_mark_type_t
{
  AV,
  BV,
  T,
  NOT_MARK,
  అంకె_MARK_TYPES = NOT_MARK
};

స్థిర thai_mark_type_t
get_mark_type (hb_codepoint_t u)
{
  ఒకవేళ (u == 0x0E31u || hb_లోపల_range<hb_codepoint_t> (u, 0x0E34u, 0x0E37u) ||
      u == 0x0E47u || hb_లోపల_range<hb_codepoint_t> (u, 0x0E4Du, 0x0E4Eu))
    తిరిగి AV;
  ఒకవేళ (hb_లోపల_range<hb_codepoint_t> (u, 0x0E38u, 0x0E3Au))
    తిరిగి BV;
  ఒకవేళ (hb_లోపల_range<hb_codepoint_t> (u, 0x0E48u, 0x0E4Cu))
    తిరిగి T;
  తిరిగి NOT_MARK;
}


గణన thai_action_t
{
  NOP,
  SD,  /* Shift combining-mark down */
  SL,  /* Shift combining-mark left */
  SDL, /* Shift combining-mark down-left */
  RD   /* Remove descender from base */
};

స్థిర hb_codepoint_t
thai_pua_shape (hb_codepoint_t u, thai_action_t action, hb_font_t *font)
{
  నిర్మాణం thai_pua_mapping_t {
    uint16_t u;
    uint16_t win_pua;
    uint16_t mac_pua;
  } స్థిరరాశి *pua_mappings = శూన్యసూచిక;
  స్థిర స్థిరరాశి thai_pua_mapping_t SD_mappings[] = {
    {0x0E48u, 0xF70Au, 0xF88Bu}, /* MAI EK */
    {0x0E49u, 0xF70Bu, 0xF88Eu}, /* MAI THO */
    {0x0E4Au, 0xF70Cu, 0xF891u}, /* MAI TRI */
    {0x0E4Bu, 0xF70Du, 0xF894u}, /* MAI CHATTAWA */
    {0x0E4Cu, 0xF70Eu, 0xF897u}, /* THANTHAKHAT */
    {0x0E38u, 0xF718u, 0xF89Bu}, /* SARA U */
    {0x0E39u, 0xF719u, 0xF89Cu}, /* SARA UU */
    {0x0E3Au, 0xF71Au, 0xF89Du}, /* PHINTHU */
    {0x0000u, 0x0000u, 0x0000u}
  };
  స్థిర స్థిరరాశి thai_pua_mapping_t SDL_mappings[] = {
    {0x0E48u, 0xF705u, 0xF88Cu}, /* MAI EK */
    {0x0E49u, 0xF706u, 0xF88Fu}, /* MAI THO */
    {0x0E4Au, 0xF707u, 0xF892u}, /* MAI TRI */
    {0x0E4Bu, 0xF708u, 0xF895u}, /* MAI CHATTAWA */
    {0x0E4Cu, 0xF709u, 0xF898u}, /* THANTHAKHAT */
    {0x0000u, 0x0000u, 0x0000u}
  };
  స్థిర స్థిరరాశి thai_pua_mapping_t SL_mappings[] = {
    {0x0E48u, 0xF713u, 0xF88Au}, /* MAI EK */
    {0x0E49u, 0xF714u, 0xF88Du}, /* MAI THO */
    {0x0E4Au, 0xF715u, 0xF890u}, /* MAI TRI */
    {0x0E4Bu, 0xF716u, 0xF893u}, /* MAI CHATTAWA */
    {0x0E4Cu, 0xF717u, 0xF896u}, /* THANTHAKHAT */
    {0x0E31u, 0xF710u, 0xF884u}, /* MAI HAN-AKAT */
    {0x0E34u, 0xF701u, 0xF885u}, /* SARA I */
    {0x0E35u, 0xF702u, 0xF886u}, /* SARA II */
    {0x0E36u, 0xF703u, 0xF887u}, /* SARA UE */
    {0x0E37u, 0xF704u, 0xF888u}, /* SARA UEE */
    {0x0E47u, 0xF712u, 0xF889u}, /* MAITAIKHU */
    {0x0E4Du, 0xF711u, 0xF899u}, /* NIKHAHIT */
    {0x0000u, 0x0000u, 0x0000u}
  };
  స్థిర స్థిరరాశి thai_pua_mapping_t RD_mappings[] = {
    {0x0E0Du, 0xF70Fu, 0xF89Au}, /* YO YING */
    {0x0E10u, 0xF700u, 0xF89Eu}, /* THO THAN */
    {0x0000u, 0x0000u, 0x0000u}
  };

  ఎంపిక (action) {
    స్థితి NOP: తిరిగి u;
    స్థితి SD:  pua_mappings = SD_mappings; ఆపు;
    స్థితి SDL: pua_mappings = SDL_mappings; ఆపు;
    స్థితి SL:  pua_mappings = SL_mappings; ఆపు;
    స్థితి RD:  pua_mappings = RD_mappings; ఆపు;
  }
  కొరకు (; pua_mappings->u; pua_mappings++)
    ఒకవేళ (pua_mappings->u == u)
    {
      hb_codepoint_t glyph;
      ఒకవేళ (hb_font_get_glyph (font, pua_mappings->win_pua, 0, &glyph))
	తిరిగి pua_mappings->win_pua;
      ఒకవేళ (hb_font_get_glyph (font, pua_mappings->mac_pua, 0, &glyph))
	తిరిగి pua_mappings->mac_pua;
      ఆపు;
    }
  తిరిగి u;
}


స్థిర స్థిరరాశి గణన thai_above_state_t
{     /* Cluster above looks like: */
  T0, /*  ⣤                      */
  T1, /*     ⣼                   */
  T2, /*        ⣾                */
  T3, /*           ⣿             */
  అంకె_ABOVE_స్థితులు
} thai_above_ప్రారంభం_state[అంకె_CONSONANT_TYPES + 1/* For NOT_CONSONANT */] =
{
  T0, /* NC */
  T1, /* AC */
  T0, /* RC */
  T0, /* DC */
  T3, /* NOT_CONSONANT */
};

స్థిర స్థిరరాశి నిర్మాణం thai_above_state_machine_అంచు_t {
  thai_action_t action;
  thai_above_state_t తదుపరి_state;
} thai_above_state_machine[అంకె_ABOVE_స్థితులు][అంకె_MARK_TYPES] =
{        /*AV*/    /*BV*/    /*T*/
/*T0*/ {{NOP,T3}, {NOP,T0}, {SD, T3}},
/*T1*/ {{SL, T2}, {NOP,T1}, {SDL,T2}},
/*T2*/ {{NOP,T3}, {NOP,T2}, {SL, T3}},
/*T3*/ {{NOP,T3}, {NOP,T3}, {NOP,T3}},
};


స్థిర స్థిరరాశి గణన thai_క్రింద_state_t
{
  B0, /* No descender */
  B1, /* Removable descender */
  B2, /* Strict descender */
  అంకె_క్రింద_స్థితులు
} thai_క్రింద_ప్రారంభం_state[అంకె_CONSONANT_TYPES + 1/* For NOT_CONSONANT */] =
{
  B0, /* NC */
  B0, /* AC */
  B1, /* RC */
  B2, /* DC */
  B2, /* NOT_CONSONANT */
};

స్థిర స్థిరరాశి నిర్మాణం thai_క్రింద_state_machine_అంచు_t {
  thai_action_t action;
  thai_క్రింద_state_t తదుపరి_state;
} thai_క్రింద_state_machine[అంకె_క్రింద_స్థితులు][అంకె_MARK_TYPES] =
{        /*AV*/    /*BV*/    /*T*/
/*B0*/ {{NOP,B0}, {NOP,B2}, {NOP, B0}},
/*B1*/ {{NOP,B1}, {RD, B2}, {NOP, B1}},
/*B2*/ {{NOP,B2}, {SD, B2}, {NOP, B2}},
};


స్థిర శూన్య
do_thai_pua_shaping (స్థిరరాశి hb_ot_shape_plan_t *plan HB_వాడని,
		     hb_బఫరు_t              *బఫరు,
		     hb_font_t                *font)
{
#నిర్వచితమైతే HB_NO_OT_SHAPER_THAI_FALLBACK
  తిరిగి;
#అయితే_ముగింపు

  thai_above_state_t above_state = thai_above_ప్రారంభం_state[NOT_CONSONANT];
  thai_క్రింద_state_t క్రింద_state = thai_క్రింద_ప్రారంభం_state[NOT_CONSONANT];
  గుర్తులేని పూర్ణాంకం పునాది = 0;

  hb_glyph_info_t *info = బఫరు->info;
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    thai_mark_type_t mt = get_mark_type (info[i].codepoint);

    ఒకవేళ (mt == NOT_MARK) {
      thai_consonant_type_t ct = get_consonant_type (info[i].codepoint);
      above_state = thai_above_ప్రారంభం_state[ct];
      క్రింద_state = thai_క్రింద_ప్రారంభం_state[ct];
      పునాది = i;
      కొనసాగు;
    }

    స్థిరరాశి thai_above_state_machine_అంచు_t &above_అంచు = thai_above_state_machine[above_state][mt];
    స్థిరరాశి thai_క్రింద_state_machine_అంచు_t &క్రింద_అంచు = thai_క్రింద_state_machine[క్రింద_state][mt];
    above_state = above_అంచు.తదుపరి_state;
    క్రింద_state = క్రింద_అంచు.తదుపరి_state;

    /* At least one of the above/below actions is NOP. */
    thai_action_t action = above_అంచు.action != NOP ? above_అంచు.action : క్రింద_అంచు.action;

    బఫరు->unsafe_గమ్యం_break (పునాది, i);
    ఒకవేళ (action == RD)
      info[పునాది].codepoint = thai_pua_shape (info[పునాది].codepoint, action, font);
    లేకపోతే
      info[i].codepoint = thai_pua_shape (info[i].codepoint, action, font);
  }
}


స్థిర శూన్య
preprocess_పాఠ్యం_thai (స్థిరరాశి hb_ot_shape_plan_t *plan,
		      hb_బఫరు_t              *బఫరు,
		      hb_font_t                *font)
{
  /* This function implements the shaping logic documented here:
   *
   *   https://linux.thai.net/~thep/th-otf/shaping.html
   *
   * The first shaping rule listed there is needed even if the font has Thai
   * OpenType tables.  The rest do fallback positioning based on PUA codepoints.
   * We implement that only if there exist no Thai GSUB in the font.
   */

  /* The following is NOT specified in the MS OT Thai spec, however, it seems
   * to be what Uniscribe and other engines implement.  According to Eric Muller:
   *
   * When you have a SARA AM, decompose it in NIKHAHIT + SARA AA, *and* move the
   * NIKHAHIT backwards over any above-base marks.
   *
   * <0E14, 0E4B, 0E33> -> <0E14, 0E4D, 0E4B, 0E32>
   *
   * This reordering is legit only when the NIKHAHIT comes from a SARA AM, not
   * when it's there to start with. The string <0E14, 0E4B, 0E4D> is probably
   * not what a user wanted, but the rendering is nevertheless nikhahit above
   * chattawa.
   *
   * Same for Lao.
   *
   * Note:
   *
   * Uniscribe also does some below-marks reordering.  Namely, it positions U+0E3A
   * after U+0E38 and U+0E39.  We do that by modifying the ccc for U+0E3A.
   * See unicode->modified_combining_class ().  Lao does NOT have a U+0E3A
   * equivalent.
   */


  /*
   * Here are the characters of significance:
   *
   *			Thai	Lao
   * SARA AM:		U+0E33	U+0EB3
   * SARA AA:		U+0E32	U+0EB2
   * Nikhahit:		U+0E4D	U+0ECD
   *
   * Testing shows that Uniscribe reorder the following marks:
   * Thai:	<0E31,0E34..0E37,     0E47..0E4E>
   * Lao:	<0EB1,0EB4..0EB7,0EBB,0EC8..0ECD>
   *
   * Note how the Lao versions are the same as Thai + 0x80.
   */

  /* We only get one script at a time, so a script-agnostic implementation
   * is adequate here. */
#నిర్వచించు IS_SARA_AM(x) (((x) & ~0x0080u) == 0x0E33u)
#నిర్వచించు NIKHAHIT_FROM_SARA_AM(x) ((x) - 0x0E33u + 0x0E4Du)
#నిర్వచించు SARA_AA_FROM_SARA_AM(x) ((x) - 1)
#నిర్వచించు IS_ABOVE_పునాది_MARK(x) (hb_లోపల_ranges<hb_codepoint_t> ((x) & ~0x0080u, 0x0E34u, 0x0E37u, 0x0E47u, 0x0E4Eu, 0x0E31u, 0x0E31u, 0x0E3Bu, 0x0E3Bu))

  బఫరు->clear_output ();
  గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
  కొరకు (బఫరు->idx = 0; బఫరు->idx < లెక్క /* No need for: && buffer->successful */;)
  {
    hb_codepoint_t u = బఫరు->cur().codepoint;
    ఒకవేళ (likely (!IS_SARA_AM (u)))
    {
      ఒకవేళ (unlikely (!బఫరు->తదుపరి_glyph ())) ఆపు;
      కొనసాగు;
    }

    /* Is SARA AM. Decompose and reorder. */
    (శూన్య) బఫరు->output_glyph (NIKHAHIT_FROM_SARA_AM (u));
    hb_glyph_info_సమితి_continuation (&బఫరు->prev(), బఫరు);
    ఒకవేళ (unlikely (!బఫరు->replace_glyph (SARA_AA_FROM_SARA_AM (u)))) ఆపు;

    /* Make Nikhahit be recognized as a ccc=0 mark when zeroing widths. */
    గుర్తులేని పూర్ణాంకం చివర = బఫరు->బహిర్గతం_len;
    hb_glyph_info_సమితి_general_category (&బఫరు->బహిర్గతం_info[చివర - 2], HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK);

    /* Ok, let's see... */
    గుర్తులేని పూర్ణాంకం ప్రారంభం = చివర - 2;
    అయితే (ప్రారంభం > 0 && IS_ABOVE_పునాది_MARK (బఫరు->బహిర్గతం_info[ప్రారంభం - 1].codepoint))
      ప్రారంభం--;

    ఒకవేళ (ప్రారంభం + 2 < చివర)
    {
      /* Move Nikhahit (end-2) to the beginning */
      బఫరు->విలీనం_బహిర్గతం_clusters (ప్రారంభం, చివర);
      hb_glyph_info_t t = బఫరు->బహిర్గతం_info[చివర - 2];
      memmove (బఫరు->బహిర్గతం_info + ప్రారంభం + 1,
	       బఫరు->బహిర్గతం_info + ప్రారంభం,
	       పరిమాణం (బఫరు->బహిర్గతం_info[0]) * (చివర - ప్రారంభం - 2));
      బఫరు->బహిర్గతం_info[ప్రారంభం] = t;
    }

    /* Since we decomposed, and NIKHAHIT is combining, merge clusters with the
     * previous cluster. */
    ఒకవేళ (ప్రారంభం)
      బఫరు->విలీనం_బహిర్గతం_grapheme_clusters (ప్రారంభం - 1, చివర);
  }
  బఫరు->సమకాలీకరణ ();

  /* If font has Thai GSUB, we are done. */
  ఒకవేళ (plan->props.script == HB_SCRIPT_THAI && !plan->map.దొరికింది_script[0])
    do_thai_pua_shaping (plan, బఫరు, font);
}

స్థిరరాశి hb_ot_shaper_t _hb_ot_shaper_thai =
{
  శూన్యసూచిక, /* collect_features */
  శూన్యసూచిక, /* override_features */
  శూన్యసూచిక, /* data_create */
  శూన్యసూచిక, /* data_destroy */
  preprocess_పాఠ్యం_thai,
  శూన్యసూచిక, /* postprocess_glyphs */
  శూన్యసూచిక, /* decompose */
  శూన్యసూచిక, /* compose */
  శూన్యసూచిక, /* setup_masks */
  శూన్యసూచిక, /* reorder_marks */
  HB_ట్యాగ్_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_DEFAULT,
  HB_OT_SHAPE_ZERO_వెడల్పు_MARKS_BY_GDEF_LATE,
  అబద్ధం,/* fallback_position */
};


#అయితే_ముగింపు
