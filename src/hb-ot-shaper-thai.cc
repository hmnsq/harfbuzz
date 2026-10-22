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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE

#شامل "hb-ot-shaper.hh"


/* Thai / Lao shaper */


/* PUA shaping */


شمار thai_consonant_type_t
{
  NC,
  AC,
  RC,
  DC,
  NOT_CONSONANT,
  عدد_CONSONANT_TYPES = NOT_CONSONANT
};

ساکن thai_consonant_type_t
get_consonant_type (hb_codepoint_t u)
{
  اگر (u == 0x0E1Bu || u == 0x0E1Du || u == 0x0E1Fu/* || u == 0x0E2Cu*/)
    واپس AC;
  اگر (u == 0x0E0Du || u == 0x0E10u)
    واپس RC;
  اگر (u == 0x0E0Eu || u == 0x0E0Fu)
    واپس DC;
  اگر (hb_اندر_range<hb_codepoint_t> (u, 0x0E01u, 0x0E2Eu))
    واپس NC;
  واپس NOT_CONSONANT;
}


شمار thai_mark_type_t
{
  AV,
  BV,
  T,
  NOT_MARK,
  عدد_MARK_TYPES = NOT_MARK
};

ساکن thai_mark_type_t
get_mark_type (hb_codepoint_t u)
{
  اگر (u == 0x0E31u || hb_اندر_range<hb_codepoint_t> (u, 0x0E34u, 0x0E37u) ||
      u == 0x0E47u || hb_اندر_range<hb_codepoint_t> (u, 0x0E4Du, 0x0E4Eu))
    واپس AV;
  اگر (hb_اندر_range<hb_codepoint_t> (u, 0x0E38u, 0x0E3Au))
    واپس BV;
  اگر (hb_اندر_range<hb_codepoint_t> (u, 0x0E48u, 0x0E4Cu))
    واپس T;
  واپس NOT_MARK;
}


شمار thai_action_t
{
  NOP,
  SD,  /* Shift combining-mark down */
  SL,  /* Shift combining-mark left */
  SDL, /* Shift combining-mark down-left */
  RD   /* Remove descender from base */
};

ساکن hb_codepoint_t
thai_pua_shape (hb_codepoint_t u, thai_action_t action, hb_font_t *font)
{
  ساخت thai_pua_mapping_t {
    uint16_t u;
    uint16_t win_pua;
    uint16_t mac_pua;
  } ثابت *pua_mappings = خالی_اشاریہ;
  ساکن ثابت thai_pua_mapping_t SD_mappings[] = {
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
  ساکن ثابت thai_pua_mapping_t SDL_mappings[] = {
    {0x0E48u, 0xF705u, 0xF88Cu}, /* MAI EK */
    {0x0E49u, 0xF706u, 0xF88Fu}, /* MAI THO */
    {0x0E4Au, 0xF707u, 0xF892u}, /* MAI TRI */
    {0x0E4Bu, 0xF708u, 0xF895u}, /* MAI CHATTAWA */
    {0x0E4Cu, 0xF709u, 0xF898u}, /* THANTHAKHAT */
    {0x0000u, 0x0000u, 0x0000u}
  };
  ساکن ثابت thai_pua_mapping_t SL_mappings[] = {
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
  ساکن ثابت thai_pua_mapping_t RD_mappings[] = {
    {0x0E0Du, 0xF70Fu, 0xF89Au}, /* YO YING */
    {0x0E10u, 0xF700u, 0xF89Eu}, /* THO THAN */
    {0x0000u, 0x0000u, 0x0000u}
  };

  انتخاب (action) {
    حالت NOP: واپس u;
    حالت SD:  pua_mappings = SD_mappings; توڑو;
    حالت SDL: pua_mappings = SDL_mappings; توڑو;
    حالت SL:  pua_mappings = SL_mappings; توڑو;
    حالت RD:  pua_mappings = RD_mappings; توڑو;
  }
  برائے (; pua_mappings->u; pua_mappings++)
    اگر (pua_mappings->u == u)
    {
      hb_codepoint_t glyph;
      اگر (hb_font_get_glyph (font, pua_mappings->win_pua, 0, &glyph))
	واپس pua_mappings->win_pua;
      اگر (hb_font_get_glyph (font, pua_mappings->mac_pua, 0, &glyph))
	واپس pua_mappings->mac_pua;
      توڑو;
    }
  واپس u;
}


ساکن ثابت شمار thai_above_state_t
{     /* Cluster above looks like: */
  T0, /*  ⣤                      */
  T1, /*     ⣼                   */
  T2, /*        ⣾                */
  T3, /*           ⣿             */
  عدد_ABOVE_حالتیں
} thai_above_آغاز_state[عدد_CONSONANT_TYPES + 1/* For NOT_CONSONANT */] =
{
  T0, /* NC */
  T1, /* AC */
  T0, /* RC */
  T0, /* DC */
  T3, /* NOT_CONSONANT */
};

ساکن ثابت ساخت thai_above_state_machine_کنارہ_t {
  thai_action_t action;
  thai_above_state_t اگلا_state;
} thai_above_state_machine[عدد_ABOVE_حالتیں][عدد_MARK_TYPES] =
{        /*AV*/    /*BV*/    /*T*/
/*T0*/ {{NOP,T3}, {NOP,T0}, {SD, T3}},
/*T1*/ {{SL, T2}, {NOP,T1}, {SDL,T2}},
/*T2*/ {{NOP,T3}, {NOP,T2}, {SL, T3}},
/*T3*/ {{NOP,T3}, {NOP,T3}, {NOP,T3}},
};


ساکن ثابت شمار thai_نیچے_state_t
{
  B0, /* No descender */
  B1, /* Removable descender */
  B2, /* Strict descender */
  عدد_نیچے_حالتیں
} thai_نیچے_آغاز_state[عدد_CONSONANT_TYPES + 1/* For NOT_CONSONANT */] =
{
  B0, /* NC */
  B0, /* AC */
  B1, /* RC */
  B2, /* DC */
  B2, /* NOT_CONSONANT */
};

ساکن ثابت ساخت thai_نیچے_state_machine_کنارہ_t {
  thai_action_t action;
  thai_نیچے_state_t اگلا_state;
} thai_نیچے_state_machine[عدد_نیچے_حالتیں][عدد_MARK_TYPES] =
{        /*AV*/    /*BV*/    /*T*/
/*B0*/ {{NOP,B0}, {NOP,B2}, {NOP, B0}},
/*B1*/ {{NOP,B1}, {RD, B2}, {NOP, B1}},
/*B2*/ {{NOP,B2}, {SD, B2}, {NOP, B2}},
};


ساکن خالی
do_thai_pua_shaping (ثابت hb_ot_shape_plan_t *plan HB_غیر_مستعمل,
		     hb_بفر_t              *بفر,
		     hb_font_t                *font)
{
#اگر_تعریف_شدہ HB_NO_OT_SHAPER_THAI_FALLBACK
  واپس;
#اگر_ختم

  thai_above_state_t above_state = thai_above_آغاز_state[NOT_CONSONANT];
  thai_نیچے_state_t نیچے_state = thai_نیچے_آغاز_state[NOT_CONSONANT];
  غیرعلامتی صحیح بنیاد = 0;

  hb_glyph_info_t *info = بفر->info;
  غیرعلامتی صحیح گنتی = بفر->len;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
  {
    thai_mark_type_t mt = get_mark_type (info[i].codepoint);

    اگر (mt == NOT_MARK) {
      thai_consonant_type_t ct = get_consonant_type (info[i].codepoint);
      above_state = thai_above_آغاز_state[ct];
      نیچے_state = thai_نیچے_آغاز_state[ct];
      بنیاد = i;
      جاری;
    }

    ثابت thai_above_state_machine_کنارہ_t &above_کنارہ = thai_above_state_machine[above_state][mt];
    ثابت thai_نیچے_state_machine_کنارہ_t &نیچے_کنارہ = thai_نیچے_state_machine[نیچے_state][mt];
    above_state = above_کنارہ.اگلا_state;
    نیچے_state = نیچے_کنارہ.اگلا_state;

    /* At least one of the above/below actions is NOP. */
    thai_action_t action = above_کنارہ.action != NOP ? above_کنارہ.action : نیچے_کنارہ.action;

    بفر->unsafe_مقصد_break (بنیاد, i);
    اگر (action == RD)
      info[بنیاد].codepoint = thai_pua_shape (info[بنیاد].codepoint, action, font);
    ورنہ
      info[i].codepoint = thai_pua_shape (info[i].codepoint, action, font);
  }
}


ساکن خالی
preprocess_متن_thai (ثابت hb_ot_shape_plan_t *plan,
		      hb_بفر_t              *بفر,
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
#تعریف_کرو IS_SARA_AM(x) (((x) & ~0x0080u) == 0x0E33u)
#تعریف_کرو NIKHAHIT_FROM_SARA_AM(x) ((x) - 0x0E33u + 0x0E4Du)
#تعریف_کرو SARA_AA_FROM_SARA_AM(x) ((x) - 1)
#تعریف_کرو IS_ABOVE_بنیاد_MARK(x) (hb_اندر_ranges<hb_codepoint_t> ((x) & ~0x0080u, 0x0E34u, 0x0E37u, 0x0E47u, 0x0E4Eu, 0x0E31u, 0x0E31u, 0x0E3Bu, 0x0E3Bu))

  بفر->clear_output ();
  غیرعلامتی صحیح گنتی = بفر->len;
  برائے (بفر->idx = 0; بفر->idx < گنتی /* No need for: && buffer->successful */;)
  {
    hb_codepoint_t u = بفر->cur().codepoint;
    اگر (likely (!IS_SARA_AM (u)))
    {
      اگر (unlikely (!بفر->اگلا_glyph ())) توڑو;
      جاری;
    }

    /* Is SARA AM. Decompose and reorder. */
    (خالی) بفر->output_glyph (NIKHAHIT_FROM_SARA_AM (u));
    hb_glyph_info_سیٹ_continuation (&بفر->prev(), بفر);
    اگر (unlikely (!بفر->replace_glyph (SARA_AA_FROM_SARA_AM (u)))) توڑو;

    /* Make Nikhahit be recognized as a ccc=0 mark when zeroing widths. */
    غیرعلامتی صحیح انتہا = بفر->برآمد_len;
    hb_glyph_info_سیٹ_general_category (&بفر->برآمد_info[انتہا - 2], HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK);

    /* Ok, let's see... */
    غیرعلامتی صحیح آغاز = انتہا - 2;
    جب (آغاز > 0 && IS_ABOVE_بنیاد_MARK (بفر->برآمد_info[آغاز - 1].codepoint))
      آغاز--;

    اگر (آغاز + 2 < انتہا)
    {
      /* Move Nikhahit (end-2) to the beginning */
      بفر->ملاؤ_برآمد_clusters (آغاز, انتہا);
      hb_glyph_info_t t = بفر->برآمد_info[انتہا - 2];
      memmove (بفر->برآمد_info + آغاز + 1,
	       بفر->برآمد_info + آغاز,
	       حجم (بفر->برآمد_info[0]) * (انتہا - آغاز - 2));
      بفر->برآمد_info[آغاز] = t;
    }

    /* Since we decomposed, and NIKHAHIT is combining, merge clusters with the
     * previous cluster. */
    اگر (آغاز)
      بفر->ملاؤ_برآمد_grapheme_clusters (آغاز - 1, انتہا);
  }
  بفر->ہمآہنگ ();

  /* If font has Thai GSUB, we are done. */
  اگر (plan->props.script == HB_SCRIPT_THAI && !plan->map.ملا_script[0])
    do_thai_pua_shaping (plan, بفر, font);
}

ثابت hb_ot_shaper_t _hb_ot_shaper_thai =
{
  خالی_اشاریہ, /* collect_features */
  خالی_اشاریہ, /* override_features */
  خالی_اشاریہ, /* data_create */
  خالی_اشاریہ, /* data_destroy */
  preprocess_متن_thai,
  خالی_اشاریہ, /* postprocess_glyphs */
  خالی_اشاریہ, /* decompose */
  خالی_اشاریہ, /* compose */
  خالی_اشاریہ, /* setup_masks */
  خالی_اشاریہ, /* reorder_marks */
  HB_لیبل_NONE, /* gpos_tag */
  HB_OT_SHAPE_NORMALIZATION_منوال_DEFAULT,
  HB_OT_SHAPE_ZERO_چوڑائی_MARKS_BY_GDEF_LATE,
  جھوٹ,/* fallback_position */
};


#اگر_ختم
