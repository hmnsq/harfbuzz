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

#که_نه_تعریف_شوی HB_AAT_LAYOUT_MORX_جدول_HH
#تعریف_کړه HB_AAT_LAYOUT_MORX_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#تعریف_کړه HB_AAT_نښان_morx HB_نښان('m','o','r','x')
#تعریف_کړه HB_AAT_نښان_mort HB_نښان('m','o','r','t')


نطاق AAT {

کارول نطاق OT;

کينډۍ <ډول_نوم Types>
بنية RearrangementSubtable
{
  تعريفنوع ډول_نوم Types::HBUINT HBUINT;

  تعريفنوع فراغ ثبت_معلومات;

  تعداد Flags
  {
    Mark_لومړی		= 0x8000,	/* If set, make the current glyph the first
					 * glyph to be rearranged. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. This means
					 * that the glyph index doesn't change, even
					 * if the glyph at that index has changed. */
    Mark_وروستی		= 0x2000,	/* If set, make the current glyph the last
					 * glyph to be rearranged. */
    Reserved		= 0x1FF0,	/* These bits are reserved and should be set to 0. */
    Verb		= 0x000F,	/* The type of rearrangement specified. */
  };

  منطقي is_action_initiable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    أرجع (ثبت.flags & Mark_لومړی);
  }
  منطقي is_actionable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    أرجع (ثبت.flags & Verb);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = ریښتیا;

    driver_context_t (ثابت RearrangementSubtable *جدول_2) :
	ret (خطأ),
	جدول (جدول_2),
	پیل (0), پای_ټکی (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, ثبت_معلومات, Flags> *driver,
		     ثابت ثبت_2<ثبت_معلومات> &ثبت)
    {
      بینښه صحيح flags = ثبت.flags;

      إذا (flags & Mark_لومړی)
	پیل = بفر->idx;

      إذا (flags & Mark_وروستی)
	پای_ټکی = hb_min (بفر->idx + 1, بفر->len);

      إذا ((flags & Verb) && پیل < پای_ټکی)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ثابت بینښه حرف map[16] =
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

	بینښه صحيح m = map[flags & Verb];
	بینښه صحيح l = hb_min (2u, m >> 4);
	بینښه صحيح r = hb_min (2u, m & 0x0F);
	منطقي سرچپه_l = 3 == (m >> 4);
	منطقي سرچپه_r = 3 == (m & 0x0F);

	إذا (پای_ټکی - پیل >= l + r && پای_ټکی-پیل <= HB_MAX_CONTEXT_اوږدتیا)
	{
	  بفر->یوځای_clusters (پیل, hb_min (بفر->idx + 1, بفر->len));
	  بفر->یوځای_clusters (پیل, پای_ټکی);

	  hb_glyph_info_t *info = بفر->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + پیل, l * حجم (buf[0]));
	  hb_memcpy (buf + 2, info + پای_ټکی - r, r * حجم (buf[0]));

	  إذا (l != r)
	    memmove (info + پیل + r, info + پیل + l, (پای_ټکی - پیل - l - r) * حجم (buf[0]));

	  hb_memcpy (info + پیل, buf + 2, r * حجم (buf[0]));
	  hb_memcpy (info + پای_ټکی - l, buf, l * حجم (buf[0]));
	  إذا (سرچپه_l)
	  {
	    buf[0] = info[پای_ټکی - 1];
	    info[پای_ټکی - 1] = info[پای_ټکی - 2];
	    info[پای_ټکی - 2] = buf[0];
	  }
	  إذا (سرچپه_r)
	  {
	    buf[0] = info[پیل];
	    info[پیل] = info[پیل + 1];
	    info[پیل + 1] = buf[0];
	  }
	}
      }
    }

    عام:
    منطقي ret;
    ثابت RearrangementSubtable *جدول;
    خاص:
    بینښه صحيح پیل;
    بینښه صحيح پای_ټکی;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا);

    State_جدول_Driver<Types, ثبت_معلومات, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (machine.sanitize (c));
  }

  عام:
  State_جدول<Types, ثبت_معلومات>	machine;
  عام:
  DEFINE_اندازه_STATIC ((State_جدول<Types, ثبت_معلومات>::static_اندازه));
};

کينډۍ <ډول_نوم Types>
بنية ContextualSubtable
{
  تعريفنوع ډول_نوم Types::HBUINT HBUINT;

  بنية ثبت_معلومات
  {
    تعريفنوع ډول_نوم std::conditional<Types::extended, HBUINT16, HBINT16>::type دننه_واټن_Type;

    دننه_واټن_Type	mark_شاخص_2;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    دننه_واټن_Type	current_شاخص;	/* Index/offset of the substitution table for the
				 * current glyph. */
    عام:
    DEFINE_اندازه_STATIC (4);
  };

  تعداد Flags
  {
    سېټ_Mark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  منطقي is_action_initiable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    أرجع (ثبت.flags & سېټ_Mark);
  }
  منطقي is_actionable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    إذا (Types::extended)
      أرجع ثبت.معلومات.mark_شاخص_2 != 0xFFFF || ثبت.معلومات.current_شاخص != 0xFFFF;
    وإلا
      أرجع ثبت.معلومات.mark_شاخص_2 || ثبت.معلومات.current_شاخص;
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = ریښتیا;

    driver_context_t (ثابت ContextualSubtable *جدول_2,
			     hb_aat_apply_context_t *c_) :
	ret (خطأ),
	c (c_),
	جدول (جدول_2),
	mark_سېټ (خطأ),
	mark (0),
	subs (جدول+جدول->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, ثبت_معلومات, Flags> *driver,
		     ثابت ثبت_2<ثبت_معلومات> &ثبت)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      إذا (بفر->idx == بفر->len && !mark_سېټ)
	أرجع;

      ثابت HBGlyphID16 *replacement;

      replacement = تش_اشاره;
      إذا (Types::extended)
      {
	إذا (ثبت.معلومات.mark_شاخص_2 != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[ثبت.معلومات.mark_شاخص_2];
	  replacement = lookup.get_ارزښت (بفر->info[mark].codepoint, driver->عدد_glyphs);
	}
      }
      وإلا
      {
	إذا (ثبت.معلومات.mark_شاخص_2)
	{
	  صحيح دننه_واټن = (صحيح) ثبت.معلومات.mark_شاخص_2 + بفر->info[mark].codepoint;
	  ثابت Unsized_Array_د<HBGlyphID16> &subs_old = (ثابت Unsized_Array_د<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::کلمه_دننه_واټن_منزل_شاخص (دننه_واټن, جدول, subs_old.arrayZ)];
	  إذا (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = تش_اشاره;
	}
      }
      إذا (replacement)
      {
	بفر->unsafe_منزل_break (mark, hb_min (بفر->idx + 1, بفر->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = ریښتیا;
      }

      replacement = تش_اشاره;
      بینښه صحيح idx = hb_min (بفر->idx, بفر->len - 1);
      إذا (Types::extended)
      {
	إذا (ثبت.معلومات.current_شاخص != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[ثبت.معلومات.current_شاخص];
	  replacement = lookup.get_ارزښت (بفر->info[idx].codepoint, driver->عدد_glyphs);
	}
      }
      وإلا
      {
	إذا (ثبت.معلومات.current_شاخص)
	{
	  صحيح دننه_واټن = (صحيح) ثبت.معلومات.current_شاخص + بفر->info[idx].codepoint;
	  ثابت Unsized_Array_د<HBGlyphID16> &subs_old = (ثابت Unsized_Array_د<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::کلمه_دننه_واټن_منزل_شاخص (دننه_واټن, جدول, subs_old.arrayZ)];
	  إذا (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = تش_اشاره;
	}
      }
      إذا (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = ریښتیا;
      }

      إذا (ثبت.flags & سېټ_Mark)
      {
	mark_سېټ = ریښتیا;
	mark = بفر->idx;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت ContextualSubtable *جدول;
    خاص:
    منطقي mark_سېټ;
    بینښه صحيح mark;
    ثابت Unsized_List_د_Offset16_منزل<Lookup<HBGlyphID16>, HBUINT, فراغ, خطأ> &subs;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, ثبت_معلومات, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    بینښه صحيح عدد_entries = 0;
    إذا (unlikely (!machine.sanitize (c, &عدد_entries))) return_trace (خطأ);
    hb_barrier ();

    إذا (!Types::extended)
      return_trace (substitutionTables.sanitize (c, هذا, 0));

    بینښه صحيح عدد_lookups = 0;

    ثابت ثبت_2<ثبت_معلومات> *entries = machine.get_entries ();
    لكل (بینښه صحيح i = 0; i < عدد_entries; i++)
    {
      ثابت ثبت_معلومات &معلومات = entries[i].معلومات;

      إذا (معلومات.mark_شاخص_2 != 0xFFFF)
	عدد_lookups = hb_max (عدد_lookups, 1u + معلومات.mark_شاخص_2);
      إذا (معلومات.current_شاخص != 0xFFFF)
	عدد_lookups = hb_max (عدد_lookups, 1u + معلومات.current_شاخص);
    }

    return_trace (substitutionTables.sanitize (c, هذا, عدد_lookups));
  }

  عام:
  State_جدول<Types, ثبت_معلومات>
		machine;
  محمي:
  NNOffset_منزل<Unsized_List_د_Offset16_منزل<Lookup<HBGlyphID16>, HBUINT, فراغ, خطأ>, HBUINT>
		substitutionTables;
  عام:
  DEFINE_اندازه_STATIC ((State_جدول<Types, ثبت_معلومات>::static_اندازه + HBUINT::static_اندازه));
};


کينډۍ <منطقي extended>
بنية Ligature_ثبت;

کينډۍ <>
بنية Ligature_ثبت<ریښتیا>
{

  بنية ثبت_معلومات
  {
    HBUINT16	lig_Action_شاخص;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    عام:
    DEFINE_اندازه_STATIC (2);
  };

  تعداد Flags
  {
    سېټ_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  ساكن منطقي initiateAction (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.flags & سېټ_Component; }

  ساكن منطقي performAction (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.flags & PerformAction; }

  ساكن بینښه صحيح lig_Action_شاخص (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.معلومات.lig_Action_شاخص; }
};
کينډۍ <>
بنية Ligature_ثبت<خطأ>
{
  تعريفنوع فراغ ثبت_معلومات;

  تعداد Flags
  {
    سېټ_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    دننه_واټن_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  ساكن منطقي initiateAction (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.flags & سېټ_Component; }

  ساكن منطقي performAction (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.flags & دننه_واټن_2; }

  ساكن بینښه صحيح lig_Action_شاخص (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.flags & دننه_واټن_2; }
};


کينډۍ <ډول_نوم Types>
بنية LigatureSubtable
{
  تعريفنوع ډول_نوم Types::HBUINT HBUINT;

  تعريفنوع Ligature_ثبت<Types::extended> Ligature_ثبت_T;
  تعريفنوع ډول_نوم Ligature_ثبت_T::ثبت_معلومات ثبت_معلومات;

  تعداد Flags
  {
    DontAdvance	= Ligature_ثبت_T::DontAdvance,
  };

  منطقي is_action_initiable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    أرجع Ligature_ثبت_T::initiateAction (ثبت);
  }
  منطقي is_actionable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    أرجع Ligature_ثبت_T::performAction (ثبت);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = خطأ;
    تعداد LigActionFlags
    {
      Lig_Action_وروستی	= 0x80000000,	/* This is the last action in the list. This also
					 * implies storage. */
      LigActionStore	= 0x40000000,	/* Store the ligature at the current cumulated index
					 * in the ligature table in place of the marked
					 * (i.e. currently-popped) glyph. */
      Lig_Action_دننه_واټن	= 0x3FFFFFFF,	/* A 30-bit value which is sign-extended to 32-bits
					 * and added to the glyph ID, resulting in an index
					 * into the component table. */
    };

    driver_context_t (ثابت LigatureSubtable *جدول_2,
		      hb_aat_apply_context_t *c_) :
	ret (خطأ),
	c (c_),
	جدول (جدول_2),
	ligAction (جدول+جدول->ligAction),
	component (جدول+جدول->component),
	ligature (جدول+جدول->ligature),
	سمون_اوږدتیا (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, ثبت_معلومات, Flags> *driver,
		     ثابت ثبت_2<ثبت_معلومات> &ثبت)
    {
      DEBUG_پیغام (APPLY, تش_اشاره, "Ligature transition at %u", بفر->idx);
      إذا (ثبت.flags & Ligature_ثبت_T::سېټ_Component)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	إذا (unlikely (سمون_اوږدتیا && سمون_positions[(سمون_اوږدتیا - 1u) % ARRAY_اوږدتیا (سمون_positions)] == بفر->بهر_len))
	  سمون_اوږدتیا--;

	سمون_positions[سمون_اوږدتیا++ % ARRAY_اوږدتیا (سمون_positions)] = بفر->بهر_len;
	DEBUG_پیغام (APPLY, تش_اشاره, "Set component at %u", بفر->بهر_len);
      }

      إذا (Ligature_ثبت_T::performAction (ثبت))
      {
	DEBUG_پیغام (APPLY, تش_اشاره, "Perform action with %u", سمون_اوږدتیا);
	بینښه صحيح پای_ټکی = بفر->بهر_len;

	إذا (unlikely (!سمون_اوږدتیا))
	  أرجع;

	إذا (بفر->idx >= بفر->len)
	  أرجع; /* TODO Work on previous instead? */

	بینښه صحيح cursor = سمون_اوږدتیا;

	بینښه صحيح action_idx = Ligature_ثبت_T::lig_Action_شاخص (ثبت);
	action_idx = Types::دننه_واټن_منزل_شاخص (action_idx, جدول, ligAction.arrayZ);
	ثابت HBUINT32 *action_معلومات = &ligAction[action_idx];

	بینښه صحيح ligature_idx = 0;
	بینښه صحيح action;
	افعل
	{
	  إذا (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_پیغام (APPLY, تش_اشاره, "Stack underflow");
	    سمون_اوږدتیا = 0;
	    اكسر;
	  }

	  DEBUG_پیغام (APPLY, تش_اشاره, "Moving to stack position %u", cursor - 1);
	  إذا (unlikely (!بفر->move_منزل (سمون_positions[--cursor % ARRAY_اوږدتیا (سمون_positions)]))) أرجع;

	  إذا (unlikely (!action_معلومات->sanitize (&c->sanitizer))) اكسر;
	  hb_barrier ();
	  action = *action_معلومات;

	  uint32_t uoffset = action & Lig_Action_دننه_واټن;
	  إذا (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t دننه_واټن = (int32_t) uoffset;
	  بینښه صحيح component_idx = بفر->cur().codepoint + دننه_واټن;
	  component_idx = Types::کلمه_دننه_واټن_منزل_شاخص (component_idx, جدول, component.arrayZ);
	  ثابت HBUINT16 &component_معلومات = component[component_idx];
	  إذا (unlikely (!component_معلومات.sanitize (&c->sanitizer))) اكسر;
	  hb_barrier ();
	  ligature_idx += component_معلومات;

	  DEBUG_پیغام (APPLY, تش_اشاره, "Action store %d last %d",
		     منطقي (action & LigActionStore),
		     منطقي (action & Lig_Action_وروستی));
	  إذا (action & (LigActionStore | Lig_Action_وروستی))
	  {
	    ligature_idx = Types::دننه_واټن_منزل_شاخص (ligature_idx, جدول, ligature.arrayZ);
	    ثابت HBGlyphID16 &ligature_معلومات = ligature[ligature_idx];
	    إذا (unlikely (!ligature_معلومات.sanitize (&c->sanitizer))) اكسر;
	    hb_barrier ();
	    hb_codepoint_t lig = ligature_معلومات;

	    DEBUG_پیغام (APPLY, تش_اشاره, "Produced ligature %u", lig);
	    إذا (unlikely (!c->replace_glyph (lig))) أرجع;

	    بینښه صحيح lig_پای_ټکی = سمون_positions[(سمون_اوږدتیا - 1u) % ARRAY_اوږدتیا (سمون_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    طالما (سمون_اوږدتیا - 1u > cursor)
	    {
	      DEBUG_پیغام (APPLY, تش_اشاره, "Skipping ligature component");
	      إذا (unlikely (!بفر->move_منزل (سمون_positions[--سمون_اوږدتیا % ARRAY_اوږدتیا (سمون_positions)]))) أرجع;
	      إذا (!c->delete_glyph ()) أرجع;
	    }

	    إذا (unlikely (!بفر->move_منزل (lig_پای_ټکی))) أرجع;
	    بفر->یوځای_بهر_clusters (سمون_positions[cursor % ARRAY_اوږدتیا (سمون_positions)], بفر->بهر_len);
	  }

	  action_معلومات++;
	}
	طالما (!(action & Lig_Action_وروستی));
	إذا (unlikely (!بفر->move_منزل (پای_ټکی))) أرجع;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت LigatureSubtable *جدول;
    خاص:
    ثابت Unsized_Array_د<HBUINT32> &ligAction;
    ثابت Unsized_Array_د<HBUINT16> &component;
    ثابت Unsized_Array_د<HBGlyphID16> &ligature;
    بینښه صحيح سمون_اوږدتیا;
    بینښه صحيح سمون_positions[HB_MAX_CONTEXT_اوږدتیا];
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, ثبت_معلومات, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (هذا) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  عام:
  State_جدول<Types, ثبت_معلومات>
		machine;
  محمي:
  NNOffset_منزل<Unsized_Array_د<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffset_منزل<Unsized_Array_د<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffset_منزل<Unsized_Array_د<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  عام:
  DEFINE_اندازه_STATIC ((State_جدول<Types, ثبت_معلومات>::static_اندازه + 3 * HBUINT::static_اندازه));
};

کينډۍ <ډول_نوم Types>
بنية NoncontextualSubtable
{
  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    منطقي ret = خطأ;
    بینښه صحيح عدد_glyphs = c->face->get_عدد_glyphs ();

    hb_glyph_info_t *info = c->بفر->info;
    بینښه صحيح شمېر = c->بفر->len;
    // If there's only one range, we already checked the flag.
    خپلکاری *وروستی_range = c->range_flags && (c->range_flags->اوږدتیا > 1) ? &(*c->range_flags)[0] : تش_اشاره;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      إذا (unlikely (وروستی_range))
      {
	خپلکاری *range = وروستی_range;
	{
	  بینښه cluster = info[i].cluster;
	  طالما (cluster < range->cluster_لومړی)
	    range--;
	  طالما (cluster > range->cluster_وروستی)
	    range++;

	  وروستی_range = range;
	}
	إذا (!(range->flags & c->subtable_flags))
	  استمر;
      }

      ثابت HBGlyphID16 *replacement = substitute.get_ارزښت (info[i].codepoint, عدد_glyphs);
      إذا (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = ریښتیا;
      }
    }

    return_trace (ret);
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_initial_glyphs (سېټ_t &glyphs, بینښه عدد_glyphs) ثابت
  {
    substitute.collect_glyphs (glyphs, عدد_glyphs);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (substitute.sanitize (c));
  }

  محمي:
  Lookup<HBGlyphID16>	substitute;
  عام:
  DEFINE_اندازه_MIN (2);
};

کينډۍ <ډول_نوم Types>
بنية InsertionSubtable
{
  تعريفنوع ډول_نوم Types::HBUINT HBUINT;

  بنية ثبت_معلومات
  {
    HBUINT16	current_ورننباسه_شاخص;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_ورننباسه_شاخص;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    عام:
    DEFINE_اندازه_STATIC (4);
  };

  تعداد Flags
  {
    سېټ_Mark		= 0x8000,     /* If set, mark the current glyph. */
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
    Current_ورننباسه_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_ورننباسه_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_ورننباسه_شمېر= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_ورننباسه_شمېر= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  منطقي is_action_initiable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    أرجع (ثبت.flags & سېټ_Mark);
  }
  منطقي is_actionable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    أرجع (ثبت.flags & (Current_ورننباسه_شمېر | Marked_ورننباسه_شمېر)) &&
	   (ثبت.معلومات.current_ورننباسه_شاخص != 0xFFFF ||ثبت.معلومات.marked_ورننباسه_شاخص != 0xFFFF);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = خطأ;

    driver_context_t (ثابت InsertionSubtable *جدول_2,
		      hb_aat_apply_context_t *c_) :
	ret (خطأ),
	c (c_),
	جدول (جدول_2),
	mark (0),
	insertionAction (جدول+جدول->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, ثبت_معلومات, Flags> *driver,
		     ثابت ثبت_2<ثبت_معلومات> &ثبت)
    {
      بینښه صحيح flags = ثبت.flags;

      بینښه mark_loc = بفر->بهر_len;

      إذا (ثبت.معلومات.marked_ورننباسه_شاخص != 0xFFFF)
      {
	بینښه صحيح شمېر = (flags & Marked_ورننباسه_شمېر);
	إذا (unlikely ((بفر->max_ops -= شمېر) <= 0)) أرجع;
	بینښه صحيح پیل = ثبت.معلومات.marked_ورننباسه_شاخص;
	ثابت HBGlyphID16 *glyphs = &insertionAction[پیل];
	إذا (unlikely (!c->sanitizer.check_array (glyphs, شمېر))) شمېر = 0;
	hb_barrier ();

	منطقي before = flags & Marked_ورننباسه_Before;

	بینښه صحيح پای_ټکی = بفر->بهر_len;
	إذا (unlikely (!بفر->move_منزل (mark))) أرجع;

	إذا (بفر->idx < بفر->len && !before)
	  إذا (unlikely (!بفر->copy_glyph ())) أرجع;
	/* TODO We ignore KashidaLike setting. */
	إذا (unlikely (!c->output_glyphs (شمېر, glyphs))) أرجع;
	ret = ریښتیا;
	إذا (بفر->idx < بفر->len && !before)
	  بفر->skip_glyph ();

	إذا (unlikely (!بفر->move_منزل (پای_ټکی + شمېر))) أرجع;

	بفر->unsafe_منزل_break_from_outbuffer (mark, hb_min (بفر->idx + 1, بفر->len));
      }

      إذا (flags & سېټ_Mark)
	mark = mark_loc;

      إذا (ثبت.معلومات.current_ورننباسه_شاخص != 0xFFFF)
      {
	بینښه صحيح شمېر = (flags & Current_ورننباسه_شمېر) >> 5;
	إذا (unlikely ((بفر->max_ops -= شمېر) <= 0)) أرجع;
	بینښه صحيح پیل = ثبت.معلومات.current_ورننباسه_شاخص;
	ثابت HBGlyphID16 *glyphs = &insertionAction[پیل];
	إذا (unlikely (!c->sanitizer.check_array (glyphs, شمېر))) شمېر = 0;
	hb_barrier ();

	منطقي before = flags & Current_ورننباسه_Before;

	بینښه صحيح پای_ټکی = بفر->بهر_len;

	إذا (بفر->idx < بفر->len && !before)
	  إذا (unlikely (!بفر->copy_glyph ())) أرجع;
	/* TODO We ignore KashidaLike setting. */
	إذا (unlikely (!c->output_glyphs (شمېر, glyphs))) أرجع;
	ret = ریښتیا;
	إذا (بفر->idx < بفر->len && !before)
	  بفر->skip_glyph ();

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
	إذا (unlikely (!بفر->move_منزل ((flags & DontAdvance) ? پای_ټکی : پای_ټکی + شمېر))) أرجع;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت InsertionSubtable *جدول;
    خاص:
    بینښه صحيح mark;
    ثابت Unsized_Array_د<HBGlyphID16> &insertionAction;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, ثبت_معلومات, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (هذا) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  عام:
  State_جدول<Types, ثبت_معلومات>
		machine;
  محمي:
  NNOffset_منزل<Unsized_Array_د<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  عام:
  DEFINE_اندازه_STATIC ((State_جدول<Types, ثبت_معلومات>::static_اندازه + HBUINT::static_اندازه));
};


بنية Feature
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  عام:
  DEFINE_اندازه_STATIC (12);
};


بنية hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  بنية hb_applicable_t
  {
    ملګری بنية hb_accelerate_subtables_context_t;
    ملګری بنية hb_aat_layout_lookup_accelerator_t;

    عام:
    hb_بټ_سېټ_t glyph_سېټ;
    بدلیدونکی hb_aat_class_cache_t class_cache;

    کينډۍ <ډول_نوم T>
    خپلکاری init_ (ثابت T &obj_, بینښه عدد_glyphs, hb_لومړیتوب<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_سېټ, عدد_glyphs, obj_)
    )

    کينډۍ <ډول_نوم T>
    فراغ init_ (ثابت T &obj_, بینښه عدد_glyphs, hb_لومړیتوب<0>)
    {
      obj_.collect_initial_glyphs (glyph_سېټ, عدد_glyphs);
    }

    کينډۍ <ډول_نوم T>
    فراغ init (ثابت T &obj_, بینښه عدد_glyphs)
    {
      glyph_سېټ.init ();
      init_ (obj_, عدد_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    فراغ
    fini ()
    {
      glyph_سېټ.fini ();
    }
  };

  /* Dispatch interface. */
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj)
  {
    hb_applicable_t *ثبت = &array[i++];

    ثبت->init (obj, عدد_glyphs);

    أرجع hb_تشه_t ();
  }
  ساكن return_t default_return_ارزښت () { أرجع hb_تشه_t (); }

  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع خطأ; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, بینښه عدد_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), عدد_glyphs (عدد_glyphs_2) {}

  hb_applicable_t *array;
  بینښه عدد_glyphs;
  بینښه i = 0;
};

بنية hb_aat_layout_chain_accelerator_t
{
  کينډۍ <ډول_نوم TChain>
  ساكن hb_aat_layout_chain_accelerator_t *create (ثابت TChain &chain, بینښه عدد_glyphs)
  {
    بینښه شمېر = chain.get_subtable_شمېر ();

    بینښه حاصل_ضرب;
    إذا (unlikely (hb_unsigned_mul_overflows (شمېر,
        حجم (hb_accelerate_subtables_context_t::hb_applicable_t), &حاصل_ضرب)))
      أرجع تش_اشاره;

    بینښه اندازه;
    إذا (unlikely (hb_unsigned_جمع_کړه_overflows (حجم (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t), حاصل_ضرب, &اندازه))) {
      أرجع تش_اشاره;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    خپلکاری *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, اندازه);
    إذا (unlikely (!thiz))
      أرجع تش_اشاره;

    thiz->شمېر = شمېر;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, عدد_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    أرجع thiz;
  }

  فراغ destroy ()
  {
    لكل (بینښه i = 0; i < شمېر; i++)
      subtables[i].fini ();
  }

  بینښه شمېر;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

کينډۍ <ډول_نوم Types>
بنية ChainSubtable
{
  تعريفنوع ډول_نوم Types::HBUINT HBUINT;

  کينډۍ <ډول_نوم T>
  ملګری بنية Chain;

  size_t get_اندازه () ثابت     { أرجع اوږدتیا; }
  بینښه صحيح get_type () ثابت     { أرجع coverage & 0xFF; }
  بینښه صحيح get_coverage () ثابت { أرجع coverage >> (حجم (HBUINT) * 8 - 8); }

  تعداد Coverage
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
  تعداد Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بینښه صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    حالة Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    حالة Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    حالة Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    حالة Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    افتراضي:			return_trace (c->default_return_ارزښت ());
    }
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(اوږدتیا.sanitize (c) &&
	  hb_barrier () &&
	  اوږدتیا >= min_اندازه &&
	  c->check_range (هذا, اوږدتیا)))
      return_trace (خطأ);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  محمي:
  HBUINT	اوږدتیا;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  اتحاد {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  عام:
  DEFINE_اندازه_MIN (2 * حجم (HBUINT) + 4);
};

کينډۍ <ډول_نوم Types>
بنية Chain
{
  تعريفنوع ډول_نوم Types::HBUINT HBUINT;

  بینښه get_subtable_شمېر () ثابت { أرجع subtable_شمېر_2; }

  hb_ماسک_t compile_flags (ثابت hb_aat_map_builder_t *map) ثابت
  {
    hb_ماسک_t flags = defaultFlags;
    {
      بینښه صحيح شمېر = feature_شمېر;
      لكل (بینښه i = 0; i < شمېر; i++)
      {
	ثابت Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (بینښه صحيح) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (بینښه صحيح) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, خطأ, 0 };
	إذا (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	وإلا إذا (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_ښکتنی_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_ښکتنی_CASE_SMALL_CAPS;
	  اذهب retry;
	}
#که_نه_تعریف_شوی HB_NO_AAT
	وإلا إذا (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_نښان_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->جدول.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#د_که_پای
      }
    }
    أرجع flags;
  }

  فراغ apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_layout_chain_accelerator_t *accel) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_شمېر));
    بینښه صحيح شمېر = subtable_شمېر_2;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      منطقي reverse;

      خپلکاری coverage = subtable->get_coverage ();

      hb_ماسک_t subtable_flags = subtable->subFeatureFlags;
      إذا (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ثابت hb_aat_map_t::range_flags_t _) -> منطقي { أرجع subtable_flags & (_.flags); })))
	اذهب skip;

      إذا (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->بفر->props.direction) !=
	  منطقي (coverage & ChainSubtable<Types>::Vertical))
	اذهب skip;

      c->subtable_flags = subtable_flags;
      c->لومړی_سېټ = accel ? &accel->subtables[i].glyph_سېټ : &Null(hb_بټ_سېټ_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : تش_اشاره;

      إذا (!c->بفر_intersects_machine ())
      {
	(فراغ) c->بفر->پیغام (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_شاخص);
	اذهب skip;
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
		منطقي (coverage & ChainSubtable<Types>::Backwards) :
		منطقي (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->بفر->props.direction);

      إذا (!c->بفر->پیغام (c->font, "start chainsubtable %u", c->lookup_شاخص))
	اذهب skip;

      إذا (reverse != c->بفر_is_reversed)
        c->سرچپه_بفر ();

      subtable->apply (c);

      (فراغ) c->بفر->پیغام (c->font, "end chainsubtable %u", c->lookup_شاخص);

      إذا (unlikely (!c->بفر->successful)) اكسر;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->سېټ_lookup_شاخص (c->lookup_شاخص + 1);
    }
    إذا (c->بفر_is_reversed)
      c->سرچپه_بفر ();
  }

  size_t get_اندازه () ثابت { أرجع اوږدتیا; }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_شمېر));
    بینښه صحيح شمېر = subtable_شمېر_2;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      ډول_نوم context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      إذا (c->stop_sublookup_iteration (ret))
	أرجع ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    أرجع c->default_return_ارزښت ();
  }

  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح نسخه) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(اوږدتیا.sanitize (c) &&
	  hb_barrier () &&
	  اوږدتیا >= min_اندازه &&
	  c->check_range (هذا, اوږدتیا)))
      return_trace (خطأ);

    إذا (!c->check_array (featureZ.arrayZ, feature_شمېر))
      return_trace (خطأ);

    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_شمېر));
    بینښه صحيح شمېر = subtable_شمېر_2;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (!subtable->sanitize (c))
	return_trace (خطأ);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    إذا (نسخه >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) subtable;
      إذا (!coverage->sanitize (c, شمېر))
        return_trace (خطأ);
    }

    return_trace (ریښتیا);
  }

  محمي:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	اوږدتیا;		/* Total byte count, including this header. */
  HBUINT	feature_شمېر;	/* Number of feature subtable entries. */
  HBUINT	subtable_شمېر_2;	/* The number of subtables in the chain. */

  Unsized_Array_د<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  عام:
  DEFINE_اندازه_MIN (8 + 2 * حجم (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

کينډۍ <ډول_نوم T, ډول_نوم Types, hb_نښان_t نښان_3>
بنية mortmorx
{
  ساكن constexpr hb_نښان_t جدول_نښان = نښان_3;

  منطقي has_معلومات () ثابت { أرجع نسخه != 0; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->جدول = sc.reference_جدول<T> (face);

      إذا (unlikely (هذا->جدول->is_blocklisted (هذا->جدول.get_blob (), face)))
      {
        hb_blob_destroy (هذا->جدول.get_blob ());
        هذا->جدول = hb_blob_get_تشه ();
      }

      هذا->chain_شمېر = جدول->get_chain_شمېر ();

      هذا->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (هذا->chain_شمېر, حجم (*accels));
      إذا (unlikely (!هذا->accels))
      {
	هذا->chain_شمېر = 0;
	هذا->جدول.destroy ();
	هذا->جدول = hb_blob_get_تشه ();
      }
    }
    ~accelerator_t ()
    {
      لكل (بینښه صحيح i = 0; i < هذا->chain_شمېر; i++)
      {
	إذا (هذا->accels[i])
	  هذا->accels[i]->destroy ();
	hb_free (هذا->accels[i]);
      }
      hb_free (هذا->accels);
      هذا->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع جدول.get_blob (); }

    کينډۍ <ډول_نوم Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (بینښه chain_شاخص, ثابت Chain &chain, بینښه عدد_glyphs) ثابت
    {
      إذا (unlikely (chain_شاخص >= chain_شمېر)) أرجع تش_اشاره;

    retry:
      خپلکاری *accel = accels[chain_شاخص].get_acquire ();
      إذا (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, عدد_glyphs);
	إذا (unlikely (!accel))
	  أرجع تش_اشاره;

	إذا (unlikely (!accels[chain_شاخص].cmpexch (تش_اشاره, accel)))
	{
	  hb_free (accel);
	  اذهب retry;
	}
      }

      أرجع accel;
    }

    hb_blob_ptr_t<T> جدول;
    بینښه صحيح chain_شمېر;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  فراغ compile_flags (ثابت hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ثابت
  {
    ثابت Chain<Types> *chain = &لومړی_Chain;
    بینښه صحيح شمېر = chain_شمېر_2;
    إذا (unlikely (!map->chain_flags.resize (شمېر)))
      أرجع;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_لومړی,
							     mapper->range_وروستی});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  بینښه get_chain_شمېر () ثابت
  {
    أرجع chain_شمېر_2;
  }
  فراغ apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_map_t &map,
	      ثابت accelerator_t &accel) ثابت
  {
    إذا (unlikely (!c->بفر->successful)) أرجع;

    c->بفر->unsafe_منزل_concat ();

    c->تیاری_بفر_glyph_سېټ ();

    c->سېټ_lookup_شاخص (0);
    ثابت Chain<Types> *chain = &لومړی_Chain;
    بینښه صحيح شمېر = chain_شمېر_2;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      خپلکاری *chain_accel = accel.get_accel (i, *chain, c->face->get_عدد_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      إذا (unlikely (!c->بفر->successful)) أرجع;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(نسخه.sanitize (c) &&
	  hb_barrier () &&
	  نسخه &&
	  chain_شمېر_2.sanitize (c)))
      return_trace (خطأ);

    ثابت Chain<Types> *chain = &لومړی_Chain;
    بینښه صحيح شمېر = chain_شمېر_2;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (!chain->sanitize (c, نسخه))
	return_trace (خطأ);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (ریښتیا);
  }

  محمي:
  HBUINT16	نسخه;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	ناکارول_شوی_2;		/* Set to 0. */
  HBUINT32	chain_شمېر_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	لومړی_Chain;	/* Chains. */

  عام:
  DEFINE_اندازه_MIN (8);
};

بنية morx : mortmorx<morx, ExtendedTypes, HB_AAT_نښان_morx>
{
  HB_دننی منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

بنية mort : mortmorx<mort, ObsoleteTypes, HB_AAT_نښان_mort>
{
  HB_دننی منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

بنية morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
بنية mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#د_که_پای /* HB_AAT_LAYOUT_MORX_TABLE_HH */
