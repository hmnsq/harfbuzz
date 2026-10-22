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

#جے_بنا_تعریف HB_AAT_LAYOUT_MORX_جدول_HH
#تعریف_کرو HB_AAT_LAYOUT_MORX_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#تعریف_کرو HB_AAT_ٹیگ_morx HB_ٹیگ('m','o','r','x')
#تعریف_کرو HB_AAT_ٹیگ_mort HB_ٹیگ('m','o','r','t')


نطاق AAT {

ورتوں نطاق OT;

سانچا <قسم_ناں Types>
بنية RearrangementSubtable
{
  تعريفنوع قسم_ناں Types::HBUINT HBUINT;

  تعريفنوع فراغ اندراج_انکڑے;

  تعداد Flags
  {
    Mark_پہلا		= 0x8000,	/* If set, make the current glyph the first
					 * glyph to be rearranged. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. This means
					 * that the glyph index doesn't change, even
					 * if the glyph at that index has changed. */
    Mark_آخری		= 0x2000,	/* If set, make the current glyph the last
					 * glyph to be rearranged. */
    Reserved		= 0x1FF0,	/* These bits are reserved and should be set to 0. */
    Verb		= 0x000F,	/* The type of rearrangement specified. */
  };

  منطقي is_action_initiable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    أرجع (اندراج.flags & Mark_پہلا);
  }
  منطقي is_actionable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    أرجع (اندراج.flags & Verb);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = سچ;

    driver_context_t (ثابت RearrangementSubtable *جدول_2) :
	ret (خطأ),
	جدول (جدول_2),
	شروع (0), سرا (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, اندراج_انکڑے, Flags> *driver,
		     ثابت اندراج_2<اندراج_انکڑے> &اندراج)
    {
      بنانشانی صحيح flags = اندراج.flags;

      إذا (flags & Mark_پہلا)
	شروع = بفر->idx;

      إذا (flags & Mark_آخری)
	سرا = hb_min (بفر->idx + 1, بفر->len);

      إذا ((flags & Verb) && شروع < سرا)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ثابت بنانشانی حرف map[16] =
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

	بنانشانی صحيح m = map[flags & Verb];
	بنانشانی صحيح l = hb_min (2u, m >> 4);
	بنانشانی صحيح r = hb_min (2u, m & 0x0F);
	منطقي الٹ_l = 3 == (m >> 4);
	منطقي الٹ_r = 3 == (m & 0x0F);

	إذا (سرا - شروع >= l + r && سرا-شروع <= HB_MAX_CONTEXT_لمائی)
	{
	  بفر->ملاؤ_clusters (شروع, hb_min (بفر->idx + 1, بفر->len));
	  بفر->ملاؤ_clusters (شروع, سرا);

	  hb_glyph_info_t *info = بفر->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + شروع, l * حجم (buf[0]));
	  hb_memcpy (buf + 2, info + سرا - r, r * حجم (buf[0]));

	  إذا (l != r)
	    memmove (info + شروع + r, info + شروع + l, (سرا - شروع - l - r) * حجم (buf[0]));

	  hb_memcpy (info + شروع, buf + 2, r * حجم (buf[0]));
	  hb_memcpy (info + سرا - l, buf, l * حجم (buf[0]));
	  إذا (الٹ_l)
	  {
	    buf[0] = info[سرا - 1];
	    info[سرا - 1] = info[سرا - 2];
	    info[سرا - 2] = buf[0];
	  }
	  إذا (الٹ_r)
	  {
	    buf[0] = info[شروع];
	    info[شروع] = info[شروع + 1];
	    info[شروع + 1] = buf[0];
	  }
	}
      }
    }

    عام:
    منطقي ret;
    ثابت RearrangementSubtable *جدول;
    خاص:
    بنانشانی صحيح شروع;
    بنانشانی صحيح سرا;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا);

    State_جدول_Driver<Types, اندراج_انکڑے, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (machine.sanitize (c));
  }

  عام:
  State_جدول<Types, اندراج_انکڑے>	machine;
  عام:
  DEFINE_ناپ_STATIC ((State_جدول<Types, اندراج_انکڑے>::static_ناپ));
};

سانچا <قسم_ناں Types>
بنية ContextualSubtable
{
  تعريفنوع قسم_ناں Types::HBUINT HBUINT;

  بنية اندراج_انکڑے
  {
    تعريفنوع قسم_ناں std::conditional<Types::extended, HBUINT16, HBINT16>::type وتھ_Type;

    وتھ_Type	mark_اشاریہ_2;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    وتھ_Type	current_اشاریہ;	/* Index/offset of the substitution table for the
				 * current glyph. */
    عام:
    DEFINE_ناپ_STATIC (4);
  };

  تعداد Flags
  {
    سیٹ_Mark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  منطقي is_action_initiable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    أرجع (اندراج.flags & سیٹ_Mark);
  }
  منطقي is_actionable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    إذا (Types::extended)
      أرجع اندراج.انکڑے.mark_اشاریہ_2 != 0xFFFF || اندراج.انکڑے.current_اشاریہ != 0xFFFF;
    وإلا
      أرجع اندراج.انکڑے.mark_اشاریہ_2 || اندراج.انکڑے.current_اشاریہ;
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = سچ;

    driver_context_t (ثابت ContextualSubtable *جدول_2,
			     hb_aat_apply_context_t *c_) :
	ret (خطأ),
	c (c_),
	جدول (جدول_2),
	mark_سیٹ (خطأ),
	mark (0),
	subs (جدول+جدول->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, اندراج_انکڑے, Flags> *driver,
		     ثابت اندراج_2<اندراج_انکڑے> &اندراج)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      إذا (بفر->idx == بفر->len && !mark_سیٹ)
	أرجع;

      ثابت HBGlyphID16 *replacement;

      replacement = خالی_سوچک;
      إذا (Types::extended)
      {
	إذا (اندراج.انکڑے.mark_اشاریہ_2 != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[اندراج.انکڑے.mark_اشاریہ_2];
	  replacement = lookup.get_قدر (بفر->info[mark].codepoint, driver->عدد_glyphs);
	}
      }
      وإلا
      {
	إذا (اندراج.انکڑے.mark_اشاریہ_2)
	{
	  صحيح وتھ = (صحيح) اندراج.انکڑے.mark_اشاریہ_2 + بفر->info[mark].codepoint;
	  ثابت Unsized_Array_دا<HBGlyphID16> &subs_old = (ثابت Unsized_Array_دا<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::لفظ_وتھ_منزل_اشاریہ (وتھ, جدول, subs_old.arrayZ)];
	  إذا (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = خالی_سوچک;
	}
      }
      إذا (replacement)
      {
	بفر->unsafe_منزل_break (mark, hb_min (بفر->idx + 1, بفر->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = سچ;
      }

      replacement = خالی_سوچک;
      بنانشانی صحيح idx = hb_min (بفر->idx, بفر->len - 1);
      إذا (Types::extended)
      {
	إذا (اندراج.انکڑے.current_اشاریہ != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[اندراج.انکڑے.current_اشاریہ];
	  replacement = lookup.get_قدر (بفر->info[idx].codepoint, driver->عدد_glyphs);
	}
      }
      وإلا
      {
	إذا (اندراج.انکڑے.current_اشاریہ)
	{
	  صحيح وتھ = (صحيح) اندراج.انکڑے.current_اشاریہ + بفر->info[idx].codepoint;
	  ثابت Unsized_Array_دا<HBGlyphID16> &subs_old = (ثابت Unsized_Array_دا<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::لفظ_وتھ_منزل_اشاریہ (وتھ, جدول, subs_old.arrayZ)];
	  إذا (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = خالی_سوچک;
	}
      }
      إذا (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = سچ;
      }

      إذا (اندراج.flags & سیٹ_Mark)
      {
	mark_سیٹ = سچ;
	mark = بفر->idx;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت ContextualSubtable *جدول;
    خاص:
    منطقي mark_سیٹ;
    بنانشانی صحيح mark;
    ثابت Unsized_List_دا_Offset16_منزل<Lookup<HBGlyphID16>, HBUINT, فراغ, خطأ> &subs;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, اندراج_انکڑے, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    بنانشانی صحيح عدد_entries = 0;
    إذا (unlikely (!machine.sanitize (c, &عدد_entries))) return_trace (خطأ);
    hb_barrier ();

    إذا (!Types::extended)
      return_trace (substitutionTables.sanitize (c, هذا, 0));

    بنانشانی صحيح عدد_lookups = 0;

    ثابت اندراج_2<اندراج_انکڑے> *entries = machine.get_entries ();
    لكل (بنانشانی صحيح i = 0; i < عدد_entries; i++)
    {
      ثابت اندراج_انکڑے &انکڑے = entries[i].انکڑے;

      إذا (انکڑے.mark_اشاریہ_2 != 0xFFFF)
	عدد_lookups = hb_max (عدد_lookups, 1u + انکڑے.mark_اشاریہ_2);
      إذا (انکڑے.current_اشاریہ != 0xFFFF)
	عدد_lookups = hb_max (عدد_lookups, 1u + انکڑے.current_اشاریہ);
    }

    return_trace (substitutionTables.sanitize (c, هذا, عدد_lookups));
  }

  عام:
  State_جدول<Types, اندراج_انکڑے>
		machine;
  محمي:
  NNOffset_منزل<Unsized_List_دا_Offset16_منزل<Lookup<HBGlyphID16>, HBUINT, فراغ, خطأ>, HBUINT>
		substitutionTables;
  عام:
  DEFINE_ناپ_STATIC ((State_جدول<Types, اندراج_انکڑے>::static_ناپ + HBUINT::static_ناپ));
};


سانچا <منطقي extended>
بنية Ligature_اندراج;

سانچا <>
بنية Ligature_اندراج<سچ>
{

  بنية اندراج_انکڑے
  {
    HBUINT16	lig_Action_اشاریہ;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    عام:
    DEFINE_ناپ_STATIC (2);
  };

  تعداد Flags
  {
    سیٹ_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  ساكن منطقي initiateAction (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.flags & سیٹ_Component; }

  ساكن منطقي performAction (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.flags & PerformAction; }

  ساكن بنانشانی صحيح lig_Action_اشاریہ (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.انکڑے.lig_Action_اشاریہ; }
};
سانچا <>
بنية Ligature_اندراج<خطأ>
{
  تعريفنوع فراغ اندراج_انکڑے;

  تعداد Flags
  {
    سیٹ_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    وتھ_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  ساكن منطقي initiateAction (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.flags & سیٹ_Component; }

  ساكن منطقي performAction (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.flags & وتھ_2; }

  ساكن بنانشانی صحيح lig_Action_اشاریہ (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.flags & وتھ_2; }
};


سانچا <قسم_ناں Types>
بنية LigatureSubtable
{
  تعريفنوع قسم_ناں Types::HBUINT HBUINT;

  تعريفنوع Ligature_اندراج<Types::extended> Ligature_اندراج_T;
  تعريفنوع قسم_ناں Ligature_اندراج_T::اندراج_انکڑے اندراج_انکڑے;

  تعداد Flags
  {
    DontAdvance	= Ligature_اندراج_T::DontAdvance,
  };

  منطقي is_action_initiable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    أرجع Ligature_اندراج_T::initiateAction (اندراج);
  }
  منطقي is_actionable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    أرجع Ligature_اندراج_T::performAction (اندراج);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = خطأ;
    تعداد LigActionFlags
    {
      Lig_Action_آخری	= 0x80000000,	/* This is the last action in the list. This also
					 * implies storage. */
      LigActionStore	= 0x40000000,	/* Store the ligature at the current cumulated index
					 * in the ligature table in place of the marked
					 * (i.e. currently-popped) glyph. */
      Lig_Action_وتھ	= 0x3FFFFFFF,	/* A 30-bit value which is sign-extended to 32-bits
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
	میل_لمائی (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, اندراج_انکڑے, Flags> *driver,
		     ثابت اندراج_2<اندراج_انکڑے> &اندراج)
    {
      DEBUG_پیغام (APPLY, خالی_سوچک, "Ligature transition at %u", بفر->idx);
      إذا (اندراج.flags & Ligature_اندراج_T::سیٹ_Component)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	إذا (unlikely (میل_لمائی && میل_positions[(میل_لمائی - 1u) % ARRAY_لمائی (میل_positions)] == بفر->باہرلا_len))
	  میل_لمائی--;

	میل_positions[میل_لمائی++ % ARRAY_لمائی (میل_positions)] = بفر->باہرلا_len;
	DEBUG_پیغام (APPLY, خالی_سوچک, "Set component at %u", بفر->باہرلا_len);
      }

      إذا (Ligature_اندراج_T::performAction (اندراج))
      {
	DEBUG_پیغام (APPLY, خالی_سوچک, "Perform action with %u", میل_لمائی);
	بنانشانی صحيح سرا = بفر->باہرلا_len;

	إذا (unlikely (!میل_لمائی))
	  أرجع;

	إذا (بفر->idx >= بفر->len)
	  أرجع; /* TODO Work on previous instead? */

	بنانشانی صحيح cursor = میل_لمائی;

	بنانشانی صحيح action_idx = Ligature_اندراج_T::lig_Action_اشاریہ (اندراج);
	action_idx = Types::وتھ_منزل_اشاریہ (action_idx, جدول, ligAction.arrayZ);
	ثابت HBUINT32 *action_انکڑے = &ligAction[action_idx];

	بنانشانی صحيح ligature_idx = 0;
	بنانشانی صحيح action;
	افعل
	{
	  إذا (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_پیغام (APPLY, خالی_سوچک, "Stack underflow");
	    میل_لمائی = 0;
	    اكسر;
	  }

	  DEBUG_پیغام (APPLY, خالی_سوچک, "Moving to stack position %u", cursor - 1);
	  إذا (unlikely (!بفر->move_منزل (میل_positions[--cursor % ARRAY_لمائی (میل_positions)]))) أرجع;

	  إذا (unlikely (!action_انکڑے->sanitize (&c->sanitizer))) اكسر;
	  hb_barrier ();
	  action = *action_انکڑے;

	  uint32_t uoffset = action & Lig_Action_وتھ;
	  إذا (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t وتھ = (int32_t) uoffset;
	  بنانشانی صحيح component_idx = بفر->cur().codepoint + وتھ;
	  component_idx = Types::لفظ_وتھ_منزل_اشاریہ (component_idx, جدول, component.arrayZ);
	  ثابت HBUINT16 &component_انکڑے = component[component_idx];
	  إذا (unlikely (!component_انکڑے.sanitize (&c->sanitizer))) اكسر;
	  hb_barrier ();
	  ligature_idx += component_انکڑے;

	  DEBUG_پیغام (APPLY, خالی_سوچک, "Action store %d last %d",
		     منطقي (action & LigActionStore),
		     منطقي (action & Lig_Action_آخری));
	  إذا (action & (LigActionStore | Lig_Action_آخری))
	  {
	    ligature_idx = Types::وتھ_منزل_اشاریہ (ligature_idx, جدول, ligature.arrayZ);
	    ثابت HBGlyphID16 &ligature_انکڑے = ligature[ligature_idx];
	    إذا (unlikely (!ligature_انکڑے.sanitize (&c->sanitizer))) اكسر;
	    hb_barrier ();
	    hb_codepoint_t lig = ligature_انکڑے;

	    DEBUG_پیغام (APPLY, خالی_سوچک, "Produced ligature %u", lig);
	    إذا (unlikely (!c->replace_glyph (lig))) أرجع;

	    بنانشانی صحيح lig_سرا = میل_positions[(میل_لمائی - 1u) % ARRAY_لمائی (میل_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    طالما (میل_لمائی - 1u > cursor)
	    {
	      DEBUG_پیغام (APPLY, خالی_سوچک, "Skipping ligature component");
	      إذا (unlikely (!بفر->move_منزل (میل_positions[--میل_لمائی % ARRAY_لمائی (میل_positions)]))) أرجع;
	      إذا (!c->delete_glyph ()) أرجع;
	    }

	    إذا (unlikely (!بفر->move_منزل (lig_سرا))) أرجع;
	    بفر->ملاؤ_باہرلا_clusters (میل_positions[cursor % ARRAY_لمائی (میل_positions)], بفر->باہرلا_len);
	  }

	  action_انکڑے++;
	}
	طالما (!(action & Lig_Action_آخری));
	إذا (unlikely (!بفر->move_منزل (سرا))) أرجع;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت LigatureSubtable *جدول;
    خاص:
    ثابت Unsized_Array_دا<HBUINT32> &ligAction;
    ثابت Unsized_Array_دا<HBUINT16> &component;
    ثابت Unsized_Array_دا<HBGlyphID16> &ligature;
    بنانشانی صحيح میل_لمائی;
    بنانشانی صحيح میل_positions[HB_MAX_CONTEXT_لمائی];
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, اندراج_انکڑے, Flags> driver (machine, c->face);

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
  State_جدول<Types, اندراج_انکڑے>
		machine;
  محمي:
  NNOffset_منزل<Unsized_Array_دا<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffset_منزل<Unsized_Array_دا<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffset_منزل<Unsized_Array_دا<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  عام:
  DEFINE_ناپ_STATIC ((State_جدول<Types, اندراج_انکڑے>::static_ناپ + 3 * HBUINT::static_ناپ));
};

سانچا <قسم_ناں Types>
بنية NoncontextualSubtable
{
  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    منطقي ret = خطأ;
    بنانشانی صحيح عدد_glyphs = c->face->get_عدد_glyphs ();

    hb_glyph_info_t *info = c->بفر->info;
    بنانشانی صحيح گنتی = c->بفر->len;
    // If there's only one range, we already checked the flag.
    خودکار *آخری_range = c->range_flags && (c->range_flags->لمائی > 1) ? &(*c->range_flags)[0] : خالی_سوچک;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      إذا (unlikely (آخری_range))
      {
	خودکار *range = آخری_range;
	{
	  بنانشانی cluster = info[i].cluster;
	  طالما (cluster < range->cluster_پہلا)
	    range--;
	  طالما (cluster > range->cluster_آخری)
	    range++;

	  آخری_range = range;
	}
	إذا (!(range->flags & c->subtable_flags))
	  استمر;
      }

      ثابت HBGlyphID16 *replacement = substitute.get_قدر (info[i].codepoint, عدد_glyphs);
      إذا (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = سچ;
      }
    }

    return_trace (ret);
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_initial_glyphs (سیٹ_t &glyphs, بنانشانی عدد_glyphs) ثابت
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
  DEFINE_ناپ_MIN (2);
};

سانچا <قسم_ناں Types>
بنية InsertionSubtable
{
  تعريفنوع قسم_ناں Types::HBUINT HBUINT;

  بنية اندراج_انکڑے
  {
    HBUINT16	current_داخل_کرو_اشاریہ;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_داخل_کرو_اشاریہ;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    عام:
    DEFINE_ناپ_STATIC (4);
  };

  تعداد Flags
  {
    سیٹ_Mark		= 0x8000,     /* If set, mark the current glyph. */
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
    Current_داخل_کرو_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_داخل_کرو_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_داخل_کرو_گنتی= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_داخل_کرو_گنتی= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  منطقي is_action_initiable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    أرجع (اندراج.flags & سیٹ_Mark);
  }
  منطقي is_actionable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    أرجع (اندراج.flags & (Current_داخل_کرو_گنتی | Marked_داخل_کرو_گنتی)) &&
	   (اندراج.انکڑے.current_داخل_کرو_اشاریہ != 0xFFFF ||اندراج.انکڑے.marked_داخل_کرو_اشاریہ != 0xFFFF);
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
		     State_جدول_Driver<Types, اندراج_انکڑے, Flags> *driver,
		     ثابت اندراج_2<اندراج_انکڑے> &اندراج)
    {
      بنانشانی صحيح flags = اندراج.flags;

      بنانشانی mark_loc = بفر->باہرلا_len;

      إذا (اندراج.انکڑے.marked_داخل_کرو_اشاریہ != 0xFFFF)
      {
	بنانشانی صحيح گنتی = (flags & Marked_داخل_کرو_گنتی);
	إذا (unlikely ((بفر->max_ops -= گنتی) <= 0)) أرجع;
	بنانشانی صحيح شروع = اندراج.انکڑے.marked_داخل_کرو_اشاریہ;
	ثابت HBGlyphID16 *glyphs = &insertionAction[شروع];
	إذا (unlikely (!c->sanitizer.check_array (glyphs, گنتی))) گنتی = 0;
	hb_barrier ();

	منطقي before = flags & Marked_داخل_کرو_Before;

	بنانشانی صحيح سرا = بفر->باہرلا_len;
	إذا (unlikely (!بفر->move_منزل (mark))) أرجع;

	إذا (بفر->idx < بفر->len && !before)
	  إذا (unlikely (!بفر->copy_glyph ())) أرجع;
	/* TODO We ignore KashidaLike setting. */
	إذا (unlikely (!c->output_glyphs (گنتی, glyphs))) أرجع;
	ret = سچ;
	إذا (بفر->idx < بفر->len && !before)
	  بفر->skip_glyph ();

	إذا (unlikely (!بفر->move_منزل (سرا + گنتی))) أرجع;

	بفر->unsafe_منزل_break_from_outbuffer (mark, hb_min (بفر->idx + 1, بفر->len));
      }

      إذا (flags & سیٹ_Mark)
	mark = mark_loc;

      إذا (اندراج.انکڑے.current_داخل_کرو_اشاریہ != 0xFFFF)
      {
	بنانشانی صحيح گنتی = (flags & Current_داخل_کرو_گنتی) >> 5;
	إذا (unlikely ((بفر->max_ops -= گنتی) <= 0)) أرجع;
	بنانشانی صحيح شروع = اندراج.انکڑے.current_داخل_کرو_اشاریہ;
	ثابت HBGlyphID16 *glyphs = &insertionAction[شروع];
	إذا (unlikely (!c->sanitizer.check_array (glyphs, گنتی))) گنتی = 0;
	hb_barrier ();

	منطقي before = flags & Current_داخل_کرو_Before;

	بنانشانی صحيح سرا = بفر->باہرلا_len;

	إذا (بفر->idx < بفر->len && !before)
	  إذا (unlikely (!بفر->copy_glyph ())) أرجع;
	/* TODO We ignore KashidaLike setting. */
	إذا (unlikely (!c->output_glyphs (گنتی, glyphs))) أرجع;
	ret = سچ;
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
	إذا (unlikely (!بفر->move_منزل ((flags & DontAdvance) ? سرا : سرا + گنتی))) أرجع;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت InsertionSubtable *جدول;
    خاص:
    بنانشانی صحيح mark;
    ثابت Unsized_Array_دا<HBGlyphID16> &insertionAction;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, اندراج_انکڑے, Flags> driver (machine, c->face);

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
  State_جدول<Types, اندراج_انکڑے>
		machine;
  محمي:
  NNOffset_منزل<Unsized_Array_دا<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  عام:
  DEFINE_ناپ_STATIC ((State_جدول<Types, اندراج_انکڑے>::static_ناپ + HBUINT::static_ناپ));
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
  DEFINE_ناپ_STATIC (12);
};


بنية hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  بنية hb_applicable_t
  {
    یار بنية hb_accelerate_subtables_context_t;
    یار بنية hb_aat_layout_lookup_accelerator_t;

    عام:
    hb_ثنائی_ہندسہ_سیٹ_t glyph_سیٹ;
    بدلنجوگ hb_aat_class_cache_t class_cache;

    سانچا <قسم_ناں T>
    خودکار init_ (ثابت T &obj_, بنانشانی عدد_glyphs, hb_ترجیح<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_سیٹ, عدد_glyphs, obj_)
    )

    سانچا <قسم_ناں T>
    فراغ init_ (ثابت T &obj_, بنانشانی عدد_glyphs, hb_ترجیح<0>)
    {
      obj_.collect_initial_glyphs (glyph_سیٹ, عدد_glyphs);
    }

    سانچا <قسم_ناں T>
    فراغ init (ثابت T &obj_, بنانشانی عدد_glyphs)
    {
      glyph_سیٹ.init ();
      init_ (obj_, عدد_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    فراغ
    fini ()
    {
      glyph_سیٹ.fini ();
    }
  };

  /* Dispatch interface. */
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj)
  {
    hb_applicable_t *اندراج = &array[i++];

    اندراج->init (obj, عدد_glyphs);

    أرجع hb_خالی_تھاواں_t ();
  }
  ساكن return_t default_return_قدر () { أرجع hb_خالی_تھاواں_t (); }

  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع خطأ; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, بنانشانی عدد_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), عدد_glyphs (عدد_glyphs_2) {}

  hb_applicable_t *array;
  بنانشانی عدد_glyphs;
  بنانشانی i = 0;
};

بنية hb_aat_layout_chain_accelerator_t
{
  سانچا <قسم_ناں TChain>
  ساكن hb_aat_layout_chain_accelerator_t *create (ثابت TChain &chain, بنانشانی عدد_glyphs)
  {
    بنانشانی گنتی = chain.get_subtable_گنتی ();

    بنانشانی ضرب_حاصل;
    إذا (unlikely (hb_unsigned_mul_overflows (گنتی,
        حجم (hb_accelerate_subtables_context_t::hb_applicable_t), &ضرب_حاصل)))
      أرجع خالی_سوچک;

    بنانشانی ناپ;
    إذا (unlikely (hb_unsigned_جوڑو_overflows (حجم (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t), ضرب_حاصل, &ناپ))) {
      أرجع خالی_سوچک;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    خودکار *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, ناپ);
    إذا (unlikely (!thiz))
      أرجع خالی_سوچک;

    thiz->گنتی = گنتی;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, عدد_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    أرجع thiz;
  }

  فراغ destroy ()
  {
    لكل (بنانشانی i = 0; i < گنتی; i++)
      subtables[i].fini ();
  }

  بنانشانی گنتی;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

سانچا <قسم_ناں Types>
بنية ChainSubtable
{
  تعريفنوع قسم_ناں Types::HBUINT HBUINT;

  سانچا <قسم_ناں T>
  یار بنية Chain;

  size_t get_ناپ () ثابت     { أرجع لمائی; }
  بنانشانی صحيح get_type () ثابت     { أرجع coverage & 0xFF; }
  بنانشانی صحيح get_coverage () ثابت { أرجع coverage >> (حجم (HBUINT) * 8 - 8); }

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

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بنانشانی صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    حالة Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    حالة Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    حالة Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    حالة Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    افتراضي:			return_trace (c->default_return_قدر ());
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
    إذا (!(لمائی.sanitize (c) &&
	  hb_barrier () &&
	  لمائی >= min_ناپ &&
	  c->check_range (هذا, لمائی)))
      return_trace (خطأ);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  محمي:
  HBUINT	لمائی;		/* Total subtable length, including this header. */
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
  DEFINE_ناپ_MIN (2 * حجم (HBUINT) + 4);
};

سانچا <قسم_ناں Types>
بنية Chain
{
  تعريفنوع قسم_ناں Types::HBUINT HBUINT;

  بنانشانی get_subtable_گنتی () ثابت { أرجع subtable_گنتی_2; }

  hb_نقاب_t compile_flags (ثابت hb_aat_map_builder_t *map) ثابت
  {
    hb_نقاب_t flags = defaultFlags;
    {
      بنانشانی صحيح گنتی = feature_گنتی;
      لكل (بنانشانی i = 0; i < گنتی; i++)
      {
	ثابت Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (بنانشانی صحيح) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (بنانشانی صحيح) feature.featureSetting;
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
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_ہیٹھلا_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_ہیٹھلا_CASE_SMALL_CAPS;
	  اذهب retry;
	}
#جے_بنا_تعریف HB_NO_AAT
	وإلا إذا (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_ٹیگ_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->جدول.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#جے_انت
      }
    }
    أرجع flags;
  }

  فراغ apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_layout_chain_accelerator_t *accel) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_گنتی));
    بنانشانی صحيح گنتی = subtable_گنتی_2;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      منطقي reverse;

      خودکار coverage = subtable->get_coverage ();

      hb_نقاب_t subtable_flags = subtable->subFeatureFlags;
      إذا (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ثابت hb_aat_map_t::range_flags_t _) -> منطقي { أرجع subtable_flags & (_.flags); })))
	اذهب skip;

      إذا (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->بفر->props.direction) !=
	  منطقي (coverage & ChainSubtable<Types>::Vertical))
	اذهب skip;

      c->subtable_flags = subtable_flags;
      c->پہلا_سیٹ = accel ? &accel->subtables[i].glyph_سیٹ : &Null(hb_ثنائی_ہندسہ_سیٹ_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : خالی_سوچک;

      إذا (!c->بفر_intersects_machine ())
      {
	(فراغ) c->بفر->سنیہا (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_اشاریہ);
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

      إذا (!c->بفر->سنیہا (c->font, "start chainsubtable %u", c->lookup_اشاریہ))
	اذهب skip;

      إذا (reverse != c->بفر_is_reversed)
        c->الٹ_بفر ();

      subtable->apply (c);

      (فراغ) c->بفر->سنیہا (c->font, "end chainsubtable %u", c->lookup_اشاریہ);

      إذا (unlikely (!c->بفر->successful)) اكسر;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->سیٹ_lookup_اشاریہ (c->lookup_اشاریہ + 1);
    }
    إذا (c->بفر_is_reversed)
      c->الٹ_بفر ();
  }

  size_t get_ناپ () ثابت { أرجع لمائی; }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_گنتی));
    بنانشانی صحيح گنتی = subtable_گنتی_2;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      قسم_ناں context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      إذا (c->stop_sublookup_iteration (ret))
	أرجع ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    أرجع c->default_return_قدر ();
  }

  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح نسخہ) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(لمائی.sanitize (c) &&
	  hb_barrier () &&
	  لمائی >= min_ناپ &&
	  c->check_range (هذا, لمائی)))
      return_trace (خطأ);

    إذا (!c->check_array (featureZ.arrayZ, feature_گنتی))
      return_trace (خطأ);

    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_گنتی));
    بنانشانی صحيح گنتی = subtable_گنتی_2;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا (!subtable->sanitize (c))
	return_trace (خطأ);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    إذا (نسخہ >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) subtable;
      إذا (!coverage->sanitize (c, گنتی))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }

  محمي:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	لمائی;		/* Total byte count, including this header. */
  HBUINT	feature_گنتی;	/* Number of feature subtable entries. */
  HBUINT	subtable_گنتی_2;	/* The number of subtables in the chain. */

  Unsized_Array_دا<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  عام:
  DEFINE_ناپ_MIN (8 + 2 * حجم (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

سانچا <قسم_ناں T, قسم_ناں Types, hb_ٹیگ_t ٹیگ_3>
بنية mortmorx
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = ٹیگ_3;

  منطقي has_انکڑے () ثابت { أرجع نسخہ != 0; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->جدول = sc.reference_جدول<T> (face);

      إذا (unlikely (هذا->جدول->is_blocklisted (هذا->جدول.get_blob (), face)))
      {
        hb_blob_destroy (هذا->جدول.get_blob ());
        هذا->جدول = hb_blob_get_خالی_تھاواں ();
      }

      هذا->chain_گنتی = جدول->get_chain_گنتی ();

      هذا->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (هذا->chain_گنتی, حجم (*accels));
      إذا (unlikely (!هذا->accels))
      {
	هذا->chain_گنتی = 0;
	هذا->جدول.destroy ();
	هذا->جدول = hb_blob_get_خالی_تھاواں ();
      }
    }
    ~accelerator_t ()
    {
      لكل (بنانشانی صحيح i = 0; i < هذا->chain_گنتی; i++)
      {
	إذا (هذا->accels[i])
	  هذا->accels[i]->destroy ();
	hb_free (هذا->accels[i]);
      }
      hb_free (هذا->accels);
      هذا->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع جدول.get_blob (); }

    سانچا <قسم_ناں Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (بنانشانی chain_اشاریہ, ثابت Chain &chain, بنانشانی عدد_glyphs) ثابت
    {
      إذا (unlikely (chain_اشاریہ >= chain_گنتی)) أرجع خالی_سوچک;

    retry:
      خودکار *accel = accels[chain_اشاریہ].get_acquire ();
      إذا (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, عدد_glyphs);
	إذا (unlikely (!accel))
	  أرجع خالی_سوچک;

	إذا (unlikely (!accels[chain_اشاریہ].cmpexch (خالی_سوچک, accel)))
	{
	  hb_free (accel);
	  اذهب retry;
	}
      }

      أرجع accel;
    }

    hb_blob_ptr_t<T> جدول;
    بنانشانی صحيح chain_گنتی;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  فراغ compile_flags (ثابت hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ثابت
  {
    ثابت Chain<Types> *chain = &پہلا_Chain;
    بنانشانی صحيح گنتی = chain_گنتی_2;
    إذا (unlikely (!map->chain_flags.resize (گنتی)))
      أرجع;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_پہلا,
							     mapper->range_آخری});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  بنانشانی get_chain_گنتی () ثابت
  {
    أرجع chain_گنتی_2;
  }
  فراغ apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_map_t &map,
	      ثابت accelerator_t &accel) ثابت
  {
    إذا (unlikely (!c->بفر->successful)) أرجع;

    c->بفر->unsafe_منزل_concat ();

    c->تیاری_بفر_glyph_سیٹ ();

    c->سیٹ_lookup_اشاریہ (0);
    ثابت Chain<Types> *chain = &پہلا_Chain;
    بنانشانی صحيح گنتی = chain_گنتی_2;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      خودکار *chain_accel = accel.get_accel (i, *chain, c->face->get_عدد_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      إذا (unlikely (!c->بفر->successful)) أرجع;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(نسخہ.sanitize (c) &&
	  hb_barrier () &&
	  نسخہ &&
	  chain_گنتی_2.sanitize (c)))
      return_trace (خطأ);

    ثابت Chain<Types> *chain = &پہلا_Chain;
    بنانشانی صحيح گنتی = chain_گنتی_2;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا (!chain->sanitize (c, نسخہ))
	return_trace (خطأ);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (سچ);
  }

  محمي:
  HBUINT16	نسخہ;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	بن_ورتیا_2;		/* Set to 0. */
  HBUINT32	chain_گنتی_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	پہلا_Chain;	/* Chains. */

  عام:
  DEFINE_ناپ_MIN (8);
};

بنية morx : mortmorx<morx, ExtendedTypes, HB_AAT_ٹیگ_morx>
{
  HB_اندرونی منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

بنية mort : mortmorx<mort, ObsoleteTypes, HB_AAT_ٹیگ_mort>
{
  HB_اندرونی منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

بنية morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
بنية mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#جے_انت /* HB_AAT_LAYOUT_MORX_TABLE_HH */
