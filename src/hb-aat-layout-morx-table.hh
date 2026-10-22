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

#اگر_غیر_تعریف_شدہ HB_AAT_LAYOUT_MORX_جدول_HH
#تعریف_کرو HB_AAT_LAYOUT_MORX_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-aat-layout-common.hh"
#شامل "hb-ot-layout.hh"
#شامل "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#تعریف_کرو HB_AAT_لیبل_morx HB_لیبل('m','o','r','x')
#تعریف_کرو HB_AAT_لیبل_mort HB_لیبل('m','o','r','t')


نامخلا AAT {

استعمال نامخلا OT;

سانچہ <قسم_نام Types>
ساخت RearrangementSubtable
{
  قسمتعریف قسم_نام Types::HBUINT HBUINT;

  قسمتعریف خالی اندراج_ڈیٹا;

  شمار Flags
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

  منطقی is_action_initiable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    واپس (اندراج_2.flags & Mark_پہلا);
  }
  منطقی is_actionable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    واپس (اندراج_2.flags & Verb);
  }

  ساخت driver_context_t
  {
    ساکن constexpr منطقی in_place = سچ;

    driver_context_t (ثابت RearrangementSubtable *جدول_2) :
	ret (جھوٹ),
	جدول (جدول_2),
	آغاز (0), انتہا (0) {}

    HB_AAT_TRANSITION_INLINE
    خالی transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> *driver,
		     ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
    {
      غیرعلامتی صحیح flags = اندراج_2.flags;

      اگر (flags & Mark_پہلا)
	آغاز = بفر->idx;

      اگر (flags & Mark_آخری)
	انتہا = hb_min (بفر->idx + 1, بفر->len);

      اگر ((flags & Verb) && آغاز < انتہا)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ثابت غیرعلامتی حرف map[16] =
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

	غیرعلامتی صحیح m = map[flags & Verb];
	غیرعلامتی صحیح l = hb_min (2u, m >> 4);
	غیرعلامتی صحیح r = hb_min (2u, m & 0x0F);
	منطقی الٹ_l = 3 == (m >> 4);
	منطقی الٹ_r = 3 == (m & 0x0F);

	اگر (انتہا - آغاز >= l + r && انتہا-آغاز <= HB_MAX_CONTEXT_لمبائی)
	{
	  بفر->ملاؤ_clusters (آغاز, hb_min (بفر->idx + 1, بفر->len));
	  بفر->ملاؤ_clusters (آغاز, انتہا);

	  hb_glyph_info_t *info = بفر->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + آغاز, l * حجم (buf[0]));
	  hb_memcpy (buf + 2, info + انتہا - r, r * حجم (buf[0]));

	  اگر (l != r)
	    memmove (info + آغاز + r, info + آغاز + l, (انتہا - آغاز - l - r) * حجم (buf[0]));

	  hb_memcpy (info + آغاز, buf + 2, r * حجم (buf[0]));
	  hb_memcpy (info + انتہا - l, buf, l * حجم (buf[0]));
	  اگر (الٹ_l)
	  {
	    buf[0] = info[انتہا - 1];
	    info[انتہا - 1] = info[انتہا - 2];
	    info[انتہا - 2] = buf[0];
	  }
	  اگر (الٹ_r)
	  {
	    buf[0] = info[آغاز];
	    info[آغاز] = info[آغاز + 1];
	    info[آغاز + 1] = buf[0];
	  }
	}
      }
    }

    عوامی:
    منطقی ret;
    ثابت RearrangementSubtable *جدول;
    نجی:
    غیرعلامتی صحیح آغاز;
    غیرعلامتی صحیح انتہا;
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    driver_context_t dc (یہ);

    State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (machine.sanitize (c));
  }

  عوامی:
  State_جدول<Types, اندراج_ڈیٹا>	machine;
  عوامی:
  DEFINE_سائز_STATIC ((State_جدول<Types, اندراج_ڈیٹا>::static_سائز));
};

سانچہ <قسم_نام Types>
ساخت ContextualSubtable
{
  قسمتعریف قسم_نام Types::HBUINT HBUINT;

  ساخت اندراج_ڈیٹا
  {
    قسمتعریف قسم_نام std::conditional<Types::extended, HBUINT16, HBINT16>::type انحراف_Type;

    انحراف_Type	mark_اشاریہ_2;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    انحراف_Type	current_اشاریہ;	/* Index/offset of the substitution table for the
				 * current glyph. */
    عوامی:
    DEFINE_سائز_STATIC (4);
  };

  شمار Flags
  {
    سیٹ_Mark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  منطقی is_action_initiable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    واپس (اندراج_2.flags & سیٹ_Mark);
  }
  منطقی is_actionable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    اگر (Types::extended)
      واپس اندراج_2.ڈیٹا.mark_اشاریہ_2 != 0xFFFF || اندراج_2.ڈیٹا.current_اشاریہ != 0xFFFF;
    ورنہ
      واپس اندراج_2.ڈیٹا.mark_اشاریہ_2 || اندراج_2.ڈیٹا.current_اشاریہ;
  }

  ساخت driver_context_t
  {
    ساکن constexpr منطقی in_place = سچ;

    driver_context_t (ثابت ContextualSubtable *جدول_2,
			     hb_aat_apply_context_t *c_) :
	ret (جھوٹ),
	c (c_),
	جدول (جدول_2),
	mark_سیٹ (جھوٹ),
	mark (0),
	subs (جدول+جدول->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    خالی transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> *driver,
		     ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      اگر (بفر->idx == بفر->len && !mark_سیٹ)
	واپس;

      ثابت HBGlyphID16 *replacement;

      replacement = خالی_اشاریہ;
      اگر (Types::extended)
      {
	اگر (اندراج_2.ڈیٹا.mark_اشاریہ_2 != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[اندراج_2.ڈیٹا.mark_اشاریہ_2];
	  replacement = lookup.get_قدر (بفر->info[mark].codepoint, driver->عدد_glyphs);
	}
      }
      ورنہ
      {
	اگر (اندراج_2.ڈیٹا.mark_اشاریہ_2)
	{
	  صحیح انحراف = (صحیح) اندراج_2.ڈیٹا.mark_اشاریہ_2 + بفر->info[mark].codepoint;
	  ثابت Unsized_Array_کا<HBGlyphID16> &subs_old = (ثابت Unsized_Array_کا<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::لفظ_انحراف_مقصد_اشاریہ (انحراف, جدول, subs_old.arrayZ)];
	  اگر (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = خالی_اشاریہ;
	}
      }
      اگر (replacement)
      {
	بفر->unsafe_مقصد_break (mark, hb_min (بفر->idx + 1, بفر->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = سچ;
      }

      replacement = خالی_اشاریہ;
      غیرعلامتی صحیح idx = hb_min (بفر->idx, بفر->len - 1);
      اگر (Types::extended)
      {
	اگر (اندراج_2.ڈیٹا.current_اشاریہ != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[اندراج_2.ڈیٹا.current_اشاریہ];
	  replacement = lookup.get_قدر (بفر->info[idx].codepoint, driver->عدد_glyphs);
	}
      }
      ورنہ
      {
	اگر (اندراج_2.ڈیٹا.current_اشاریہ)
	{
	  صحیح انحراف = (صحیح) اندراج_2.ڈیٹا.current_اشاریہ + بفر->info[idx].codepoint;
	  ثابت Unsized_Array_کا<HBGlyphID16> &subs_old = (ثابت Unsized_Array_کا<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::لفظ_انحراف_مقصد_اشاریہ (انحراف, جدول, subs_old.arrayZ)];
	  اگر (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = خالی_اشاریہ;
	}
      }
      اگر (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = سچ;
      }

      اگر (اندراج_2.flags & سیٹ_Mark)
      {
	mark_سیٹ = سچ;
	mark = بفر->idx;
      }
    }

    عوامی:
    منطقی ret;
    hb_aat_apply_context_t *c;
    ثابت ContextualSubtable *جدول;
    نجی:
    منطقی mark_سیٹ;
    غیرعلامتی صحیح mark;
    ثابت Unsized_List_کا_Offset16_مقصد<Lookup<HBGlyphID16>, HBUINT, خالی, جھوٹ> &subs;
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    driver_context_t dc (یہ, c);

    State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);

    غیرعلامتی صحیح عدد_entries = 0;
    اگر (unlikely (!machine.sanitize (c, &عدد_entries))) return_trace (جھوٹ);
    hb_barrier ();

    اگر (!Types::extended)
      return_trace (substitutionTables.sanitize (c, یہ, 0));

    غیرعلامتی صحیح عدد_lookups = 0;

    ثابت اندراج_3<اندراج_ڈیٹا> *entries = machine.get_entries ();
    برائے (غیرعلامتی صحیح i = 0; i < عدد_entries; i++)
    {
      ثابت اندراج_ڈیٹا &ڈیٹا = entries[i].ڈیٹا;

      اگر (ڈیٹا.mark_اشاریہ_2 != 0xFFFF)
	عدد_lookups = hb_max (عدد_lookups, 1u + ڈیٹا.mark_اشاریہ_2);
      اگر (ڈیٹا.current_اشاریہ != 0xFFFF)
	عدد_lookups = hb_max (عدد_lookups, 1u + ڈیٹا.current_اشاریہ);
    }

    return_trace (substitutionTables.sanitize (c, یہ, عدد_lookups));
  }

  عوامی:
  State_جدول<Types, اندراج_ڈیٹا>
		machine;
  محفوظ:
  NNOffset_مقصد<Unsized_List_کا_Offset16_مقصد<Lookup<HBGlyphID16>, HBUINT, خالی, جھوٹ>, HBUINT>
		substitutionTables;
  عوامی:
  DEFINE_سائز_STATIC ((State_جدول<Types, اندراج_ڈیٹا>::static_سائز + HBUINT::static_سائز));
};


سانچہ <منطقی extended>
ساخت Ligature_اندراج;

سانچہ <>
ساخت Ligature_اندراج<سچ>
{

  ساخت اندراج_ڈیٹا
  {
    HBUINT16	lig_Action_اشاریہ;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    عوامی:
    DEFINE_سائز_STATIC (2);
  };

  شمار Flags
  {
    سیٹ_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  ساکن منطقی initiateAction (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.flags & سیٹ_Component; }

  ساکن منطقی performAction (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.flags & PerformAction; }

  ساکن غیرعلامتی صحیح lig_Action_اشاریہ (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.ڈیٹا.lig_Action_اشاریہ; }
};
سانچہ <>
ساخت Ligature_اندراج<جھوٹ>
{
  قسمتعریف خالی اندراج_ڈیٹا;

  شمار Flags
  {
    سیٹ_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    انحراف_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  ساکن منطقی initiateAction (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.flags & سیٹ_Component; }

  ساکن منطقی performAction (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.flags & انحراف_2; }

  ساکن غیرعلامتی صحیح lig_Action_اشاریہ (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.flags & انحراف_2; }
};


سانچہ <قسم_نام Types>
ساخت LigatureSubtable
{
  قسمتعریف قسم_نام Types::HBUINT HBUINT;

  قسمتعریف Ligature_اندراج<Types::extended> Ligature_اندراج_T;
  قسمتعریف قسم_نام Ligature_اندراج_T::اندراج_ڈیٹا اندراج_ڈیٹا;

  شمار Flags
  {
    DontAdvance	= Ligature_اندراج_T::DontAdvance,
  };

  منطقی is_action_initiable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    واپس Ligature_اندراج_T::initiateAction (اندراج_2);
  }
  منطقی is_actionable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    واپس Ligature_اندراج_T::performAction (اندراج_2);
  }

  ساخت driver_context_t
  {
    ساکن constexpr منطقی in_place = جھوٹ;
    شمار LigActionFlags
    {
      Lig_Action_آخری	= 0x80000000,	/* This is the last action in the list. This also
					 * implies storage. */
      LigActionStore	= 0x40000000,	/* Store the ligature at the current cumulated index
					 * in the ligature table in place of the marked
					 * (i.e. currently-popped) glyph. */
      Lig_Action_انحراف	= 0x3FFFFFFF,	/* A 30-bit value which is sign-extended to 32-bits
					 * and added to the glyph ID, resulting in an index
					 * into the component table. */
    };

    driver_context_t (ثابت LigatureSubtable *جدول_2,
		      hb_aat_apply_context_t *c_) :
	ret (جھوٹ),
	c (c_),
	جدول (جدول_2),
	ligAction (جدول+جدول->ligAction),
	component (جدول+جدول->component),
	ligature (جدول+جدول->ligature),
	مطابقت_لمبائی (0) {}

    HB_AAT_TRANSITION_INLINE
    خالی transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> *driver,
		     ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
    {
      DEBUG_پیغام (APPLY, خالی_اشاریہ, "Ligature transition at %u", بفر->idx);
      اگر (اندراج_2.flags & Ligature_اندراج_T::سیٹ_Component)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	اگر (unlikely (مطابقت_لمبائی && مطابقت_positions[(مطابقت_لمبائی - 1u) % ARRAY_لمبائی (مطابقت_positions)] == بفر->برآمد_len))
	  مطابقت_لمبائی--;

	مطابقت_positions[مطابقت_لمبائی++ % ARRAY_لمبائی (مطابقت_positions)] = بفر->برآمد_len;
	DEBUG_پیغام (APPLY, خالی_اشاریہ, "Set component at %u", بفر->برآمد_len);
      }

      اگر (Ligature_اندراج_T::performAction (اندراج_2))
      {
	DEBUG_پیغام (APPLY, خالی_اشاریہ, "Perform action with %u", مطابقت_لمبائی);
	غیرعلامتی صحیح انتہا = بفر->برآمد_len;

	اگر (unlikely (!مطابقت_لمبائی))
	  واپس;

	اگر (بفر->idx >= بفر->len)
	  واپس; /* TODO Work on previous instead? */

	غیرعلامتی صحیح cursor = مطابقت_لمبائی;

	غیرعلامتی صحیح action_idx = Ligature_اندراج_T::lig_Action_اشاریہ (اندراج_2);
	action_idx = Types::انحراف_مقصد_اشاریہ (action_idx, جدول, ligAction.arrayZ);
	ثابت HBUINT32 *action_ڈیٹا = &ligAction[action_idx];

	غیرعلامتی صحیح ligature_idx = 0;
	غیرعلامتی صحیح action;
	کرو
	{
	  اگر (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_پیغام (APPLY, خالی_اشاریہ, "Stack underflow");
	    مطابقت_لمبائی = 0;
	    توڑو;
	  }

	  DEBUG_پیغام (APPLY, خالی_اشاریہ, "Moving to stack position %u", cursor - 1);
	  اگر (unlikely (!بفر->move_مقصد (مطابقت_positions[--cursor % ARRAY_لمبائی (مطابقت_positions)]))) واپس;

	  اگر (unlikely (!action_ڈیٹا->sanitize (&c->sanitizer))) توڑو;
	  hb_barrier ();
	  action = *action_ڈیٹا;

	  uint32_t uoffset = action & Lig_Action_انحراف;
	  اگر (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t انحراف = (int32_t) uoffset;
	  غیرعلامتی صحیح component_idx = بفر->cur().codepoint + انحراف;
	  component_idx = Types::لفظ_انحراف_مقصد_اشاریہ (component_idx, جدول, component.arrayZ);
	  ثابت HBUINT16 &component_ڈیٹا = component[component_idx];
	  اگر (unlikely (!component_ڈیٹا.sanitize (&c->sanitizer))) توڑو;
	  hb_barrier ();
	  ligature_idx += component_ڈیٹا;

	  DEBUG_پیغام (APPLY, خالی_اشاریہ, "Action store %d last %d",
		     منطقی (action & LigActionStore),
		     منطقی (action & Lig_Action_آخری));
	  اگر (action & (LigActionStore | Lig_Action_آخری))
	  {
	    ligature_idx = Types::انحراف_مقصد_اشاریہ (ligature_idx, جدول, ligature.arrayZ);
	    ثابت HBGlyphID16 &ligature_ڈیٹا = ligature[ligature_idx];
	    اگر (unlikely (!ligature_ڈیٹا.sanitize (&c->sanitizer))) توڑو;
	    hb_barrier ();
	    hb_codepoint_t lig = ligature_ڈیٹا;

	    DEBUG_پیغام (APPLY, خالی_اشاریہ, "Produced ligature %u", lig);
	    اگر (unlikely (!c->replace_glyph (lig))) واپس;

	    غیرعلامتی صحیح lig_انتہا = مطابقت_positions[(مطابقت_لمبائی - 1u) % ARRAY_لمبائی (مطابقت_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    جب (مطابقت_لمبائی - 1u > cursor)
	    {
	      DEBUG_پیغام (APPLY, خالی_اشاریہ, "Skipping ligature component");
	      اگر (unlikely (!بفر->move_مقصد (مطابقت_positions[--مطابقت_لمبائی % ARRAY_لمبائی (مطابقت_positions)]))) واپس;
	      اگر (!c->delete_glyph ()) واپس;
	    }

	    اگر (unlikely (!بفر->move_مقصد (lig_انتہا))) واپس;
	    بفر->ملاؤ_برآمد_clusters (مطابقت_positions[cursor % ARRAY_لمبائی (مطابقت_positions)], بفر->برآمد_len);
	  }

	  action_ڈیٹا++;
	}
	جب (!(action & Lig_Action_آخری));
	اگر (unlikely (!بفر->move_مقصد (انتہا))) واپس;
      }
    }

    عوامی:
    منطقی ret;
    hb_aat_apply_context_t *c;
    ثابت LigatureSubtable *جدول;
    نجی:
    ثابت Unsized_Array_کا<HBUINT32> &ligAction;
    ثابت Unsized_Array_کا<HBUINT16> &component;
    ثابت Unsized_Array_کا<HBGlyphID16> &ligature;
    غیرعلامتی صحیح مطابقت_لمبائی;
    غیرعلامتی صحیح مطابقت_positions[HB_MAX_CONTEXT_لمبائی];
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    driver_context_t dc (یہ, c);

    State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (یہ) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  عوامی:
  State_جدول<Types, اندراج_ڈیٹا>
		machine;
  محفوظ:
  NNOffset_مقصد<Unsized_Array_کا<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffset_مقصد<Unsized_Array_کا<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffset_مقصد<Unsized_Array_کا<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  عوامی:
  DEFINE_سائز_STATIC ((State_جدول<Types, اندراج_ڈیٹا>::static_سائز + 3 * HBUINT::static_سائز));
};

سانچہ <قسم_نام Types>
ساخت NoncontextualSubtable
{
  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    منطقی ret = جھوٹ;
    غیرعلامتی صحیح عدد_glyphs = c->face->get_عدد_glyphs ();

    hb_glyph_info_t *info = c->بفر->info;
    غیرعلامتی صحیح گنتی = c->بفر->len;
    // If there's only one range, we already checked the flag.
    خودکار *آخری_range = c->range_flags && (c->range_flags->لمبائی > 1) ? &(*c->range_flags)[0] : خالی_اشاریہ;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      اگر (unlikely (آخری_range))
      {
	خودکار *range = آخری_range;
	{
	  غیرعلامتی cluster = info[i].cluster;
	  جب (cluster < range->cluster_پہلا)
	    range--;
	  جب (cluster > range->cluster_آخری)
	    range++;

	  آخری_range = range;
	}
	اگر (!(range->flags & c->subtable_flags))
	  جاری;
      }

      ثابت HBGlyphID16 *replacement = substitute.get_قدر (info[i].codepoint, عدد_glyphs);
      اگر (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = سچ;
      }
    }

    return_trace (ret);
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_initial_glyphs (سیٹ_t &glyphs, غیرعلامتی عدد_glyphs) ثابت
  {
    substitute.collect_glyphs (glyphs, عدد_glyphs);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (substitute.sanitize (c));
  }

  محفوظ:
  Lookup<HBGlyphID16>	substitute;
  عوامی:
  DEFINE_سائز_MIN (2);
};

سانچہ <قسم_نام Types>
ساخت InsertionSubtable
{
  قسمتعریف قسم_نام Types::HBUINT HBUINT;

  ساخت اندراج_ڈیٹا
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
    عوامی:
    DEFINE_سائز_STATIC (4);
  };

  شمار Flags
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

  منطقی is_action_initiable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    واپس (اندراج_2.flags & سیٹ_Mark);
  }
  منطقی is_actionable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    واپس (اندراج_2.flags & (Current_داخل_کرو_گنتی | Marked_داخل_کرو_گنتی)) &&
	   (اندراج_2.ڈیٹا.current_داخل_کرو_اشاریہ != 0xFFFF ||اندراج_2.ڈیٹا.marked_داخل_کرو_اشاریہ != 0xFFFF);
  }

  ساخت driver_context_t
  {
    ساکن constexpr منطقی in_place = جھوٹ;

    driver_context_t (ثابت InsertionSubtable *جدول_2,
		      hb_aat_apply_context_t *c_) :
	ret (جھوٹ),
	c (c_),
	جدول (جدول_2),
	mark (0),
	insertionAction (جدول+جدول->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    خالی transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> *driver,
		     ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
    {
      غیرعلامتی صحیح flags = اندراج_2.flags;

      غیرعلامتی mark_loc = بفر->برآمد_len;

      اگر (اندراج_2.ڈیٹا.marked_داخل_کرو_اشاریہ != 0xFFFF)
      {
	غیرعلامتی صحیح گنتی = (flags & Marked_داخل_کرو_گنتی);
	اگر (unlikely ((بفر->max_ops -= گنتی) <= 0)) واپس;
	غیرعلامتی صحیح آغاز = اندراج_2.ڈیٹا.marked_داخل_کرو_اشاریہ;
	ثابت HBGlyphID16 *glyphs = &insertionAction[آغاز];
	اگر (unlikely (!c->sanitizer.check_array (glyphs, گنتی))) گنتی = 0;
	hb_barrier ();

	منطقی before = flags & Marked_داخل_کرو_Before;

	غیرعلامتی صحیح انتہا = بفر->برآمد_len;
	اگر (unlikely (!بفر->move_مقصد (mark))) واپس;

	اگر (بفر->idx < بفر->len && !before)
	  اگر (unlikely (!بفر->copy_glyph ())) واپس;
	/* TODO We ignore KashidaLike setting. */
	اگر (unlikely (!c->output_glyphs (گنتی, glyphs))) واپس;
	ret = سچ;
	اگر (بفر->idx < بفر->len && !before)
	  بفر->skip_glyph ();

	اگر (unlikely (!بفر->move_مقصد (انتہا + گنتی))) واپس;

	بفر->unsafe_مقصد_break_from_outbuffer (mark, hb_min (بفر->idx + 1, بفر->len));
      }

      اگر (flags & سیٹ_Mark)
	mark = mark_loc;

      اگر (اندراج_2.ڈیٹا.current_داخل_کرو_اشاریہ != 0xFFFF)
      {
	غیرعلامتی صحیح گنتی = (flags & Current_داخل_کرو_گنتی) >> 5;
	اگر (unlikely ((بفر->max_ops -= گنتی) <= 0)) واپس;
	غیرعلامتی صحیح آغاز = اندراج_2.ڈیٹا.current_داخل_کرو_اشاریہ;
	ثابت HBGlyphID16 *glyphs = &insertionAction[آغاز];
	اگر (unlikely (!c->sanitizer.check_array (glyphs, گنتی))) گنتی = 0;
	hb_barrier ();

	منطقی before = flags & Current_داخل_کرو_Before;

	غیرعلامتی صحیح انتہا = بفر->برآمد_len;

	اگر (بفر->idx < بفر->len && !before)
	  اگر (unlikely (!بفر->copy_glyph ())) واپس;
	/* TODO We ignore KashidaLike setting. */
	اگر (unlikely (!c->output_glyphs (گنتی, glyphs))) واپس;
	ret = سچ;
	اگر (بفر->idx < بفر->len && !before)
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
	اگر (unlikely (!بفر->move_مقصد ((flags & DontAdvance) ? انتہا : انتہا + گنتی))) واپس;
      }
    }

    عوامی:
    منطقی ret;
    hb_aat_apply_context_t *c;
    ثابت InsertionSubtable *جدول;
    نجی:
    غیرعلامتی صحیح mark;
    ثابت Unsized_Array_کا<HBGlyphID16> &insertionAction;
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    driver_context_t dc (یہ, c);

    State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (یہ) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  عوامی:
  State_جدول<Types, اندراج_ڈیٹا>
		machine;
  محفوظ:
  NNOffset_مقصد<Unsized_Array_کا<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  عوامی:
  DEFINE_سائز_STATIC ((State_جدول<Types, اندراج_ڈیٹا>::static_سائز + HBUINT::static_سائز));
};


ساخت Feature
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  عوامی:
  DEFINE_سائز_STATIC (12);
};


ساخت hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  ساخت hb_applicable_t
  {
    دوست ساخت hb_accelerate_subtables_context_t;
    دوست ساخت hb_aat_layout_lookup_accelerator_t;

    عوامی:
    hb_ثنائی_ہندسہ_سیٹ_t glyph_سیٹ;
    تغیرپذیر hb_aat_class_cache_t class_cache;

    سانچہ <قسم_نام T>
    خودکار init_ (ثابت T &obj_, غیرعلامتی عدد_glyphs, hb_ترجیح<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_سیٹ, عدد_glyphs, obj_)
    )

    سانچہ <قسم_نام T>
    خالی init_ (ثابت T &obj_, غیرعلامتی عدد_glyphs, hb_ترجیح<0>)
    {
      obj_.collect_initial_glyphs (glyph_سیٹ, عدد_glyphs);
    }

    سانچہ <قسم_نام T>
    خالی init (ثابت T &obj_, غیرعلامتی عدد_glyphs)
    {
      glyph_سیٹ.init ();
      init_ (obj_, عدد_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    خالی
    fini ()
    {
      glyph_سیٹ.fini ();
    }
  };

  /* Dispatch interface. */
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj)
  {
    hb_applicable_t *اندراج_2 = &array[i++];

    اندراج_2->init (obj, عدد_glyphs);

    واپس hb_خالی_خانے_t ();
  }
  ساکن return_t default_return_قدر () { واپس hb_خالی_خانے_t (); }

  منطقی stop_sublookup_iteration (return_t r) ثابت { واپس جھوٹ; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, غیرعلامتی عدد_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), عدد_glyphs (عدد_glyphs_2) {}

  hb_applicable_t *array;
  غیرعلامتی عدد_glyphs;
  غیرعلامتی i = 0;
};

ساخت hb_aat_layout_chain_accelerator_t
{
  سانچہ <قسم_نام TChain>
  ساکن hb_aat_layout_chain_accelerator_t *create (ثابت TChain &chain, غیرعلامتی عدد_glyphs)
  {
    غیرعلامتی گنتی = chain.get_subtable_گنتی ();

    غیرعلامتی حاصل_ضرب;
    اگر (unlikely (hb_unsigned_mul_overflows (گنتی,
        حجم (hb_accelerate_subtables_context_t::hb_applicable_t), &حاصل_ضرب)))
      واپس خالی_اشاریہ;

    غیرعلامتی سائز;
    اگر (unlikely (hb_unsigned_جوڑو_overflows (حجم (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t), حاصل_ضرب, &سائز))) {
      واپس خالی_اشاریہ;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    خودکار *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, سائز);
    اگر (unlikely (!thiz))
      واپس خالی_اشاریہ;

    thiz->گنتی = گنتی;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, عدد_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    واپس thiz;
  }

  خالی destroy ()
  {
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      subtables[i].fini ();
  }

  غیرعلامتی گنتی;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

سانچہ <قسم_نام Types>
ساخت ChainSubtable
{
  قسمتعریف قسم_نام Types::HBUINT HBUINT;

  سانچہ <قسم_نام T>
  دوست ساخت Chain;

  size_t get_سائز () ثابت     { واپس لمبائی; }
  غیرعلامتی صحیح get_type () ثابت     { واپس coverage & 0xFF; }
  غیرعلامتی صحیح get_coverage () ثابت { واپس coverage >> (حجم (HBUINT) * 8 - 8); }

  شمار Coverage
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
  شمار Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    غیرعلامتی صحیح subtable_type = get_type ();
    TRACE_DISPATCH (یہ, subtable_type);
    انتخاب (subtable_type) {
    حالت Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    حالت Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    حالت Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    حالت Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    حالت Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    طےشدہ:			return_trace (c->default_return_قدر ());
    }
  }

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!(لمبائی.sanitize (c) &&
	  hb_barrier () &&
	  لمبائی >= min_سائز &&
	  c->check_range (یہ, لمبائی)))
      return_trace (جھوٹ);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  محفوظ:
  HBUINT	لمبائی;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  اتحاد {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  عوامی:
  DEFINE_سائز_MIN (2 * حجم (HBUINT) + 4);
};

سانچہ <قسم_نام Types>
ساخت Chain
{
  قسمتعریف قسم_نام Types::HBUINT HBUINT;

  غیرعلامتی get_subtable_گنتی () ثابت { واپس subtable_گنتی_2; }

  hb_نقاب_t compile_flags (ثابت hb_aat_map_builder_t *map) ثابت
  {
    hb_نقاب_t flags = defaultFlags;
    {
      غیرعلامتی صحیح گنتی = feature_گنتی;
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
      {
	ثابت Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (غیرعلامتی صحیح) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (غیرعلامتی صحیح) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, جھوٹ, 0 };
	اگر (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	ورنہ اگر (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_زیریں_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_زیریں_CASE_SMALL_CAPS;
	  جاؤ retry;
	}
#اگر_غیر_تعریف_شدہ HB_NO_AAT
	ورنہ اگر (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_لیبل_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->جدول.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#اگر_ختم
      }
    }
    واپس flags;
  }

  خالی apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_layout_chain_accelerator_t *accel) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_گنتی));
    غیرعلامتی صحیح گنتی = subtable_گنتی_2;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      منطقی reverse;

      خودکار coverage = subtable->get_coverage ();

      hb_نقاب_t subtable_flags = subtable->subFeatureFlags;
      اگر (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ثابت hb_aat_map_t::range_flags_t _) -> منطقی { واپس subtable_flags & (_.flags); })))
	جاؤ skip;

      اگر (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->بفر->props.direction) !=
	  منطقی (coverage & ChainSubtable<Types>::Vertical))
	جاؤ skip;

      c->subtable_flags = subtable_flags;
      c->پہلا_سیٹ = accel ? &accel->subtables[i].glyph_سیٹ : &Null(hb_ثنائی_ہندسہ_سیٹ_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : خالی_اشاریہ;

      اگر (!c->بفر_intersects_machine ())
      {
	(خالی) c->بفر->پیغام (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_اشاریہ);
	جاؤ skip;
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
		منطقی (coverage & ChainSubtable<Types>::Backwards) :
		منطقی (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->بفر->props.direction);

      اگر (!c->بفر->پیغام (c->font, "start chainsubtable %u", c->lookup_اشاریہ))
	جاؤ skip;

      اگر (reverse != c->بفر_is_reversed)
        c->الٹ_بفر ();

      subtable->apply (c);

      (خالی) c->بفر->پیغام (c->font, "end chainsubtable %u", c->lookup_اشاریہ);

      اگر (unlikely (!c->بفر->successful)) توڑو;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->سیٹ_lookup_اشاریہ (c->lookup_اشاریہ + 1);
    }
    اگر (c->بفر_is_reversed)
      c->الٹ_بفر ();
  }

  size_t get_سائز () ثابت { واپس لمبائی; }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_گنتی));
    غیرعلامتی صحیح گنتی = subtable_گنتی_2;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      قسم_نام context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      اگر (c->stop_sublookup_iteration (ret))
	واپس ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    واپس c->default_return_قدر ();
  }

  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح نسخہ) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!(لمبائی.sanitize (c) &&
	  hb_barrier () &&
	  لمبائی >= min_سائز &&
	  c->check_range (یہ, لمبائی)))
      return_trace (جھوٹ);

    اگر (!c->check_array (featureZ.arrayZ, feature_گنتی))
      return_trace (جھوٹ);

    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_گنتی));
    غیرعلامتی صحیح گنتی = subtable_گنتی_2;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (!subtable->sanitize (c))
	return_trace (جھوٹ);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    اگر (نسخہ >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) subtable;
      اگر (!coverage->sanitize (c, گنتی))
        return_trace (جھوٹ);
    }

    return_trace (سچ);
  }

  محفوظ:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	لمبائی;		/* Total byte count, including this header. */
  HBUINT	feature_گنتی;	/* Number of feature subtable entries. */
  HBUINT	subtable_گنتی_2;	/* The number of subtables in the chain. */

  Unsized_Array_کا<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  عوامی:
  DEFINE_سائز_MIN (8 + 2 * حجم (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

سانچہ <قسم_نام T, قسم_نام Types, hb_لیبل_t لیبل_3>
ساخت mortmorx
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = لیبل_3;

  منطقی has_ڈیٹا () ثابت { واپس نسخہ != 0; }

  ساخت accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      یہ->جدول = sc.reference_جدول<T> (face);

      اگر (unlikely (یہ->جدول->is_blocklisted (یہ->جدول.get_blob (), face)))
      {
        hb_blob_destroy (یہ->جدول.get_blob ());
        یہ->جدول = hb_blob_get_خالی_خانے ();
      }

      یہ->chain_گنتی = جدول->get_chain_گنتی ();

      یہ->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (یہ->chain_گنتی, حجم (*accels));
      اگر (unlikely (!یہ->accels))
      {
	یہ->chain_گنتی = 0;
	یہ->جدول.destroy ();
	یہ->جدول = hb_blob_get_خالی_خانے ();
      }
    }
    ~accelerator_t ()
    {
      برائے (غیرعلامتی صحیح i = 0; i < یہ->chain_گنتی; i++)
      {
	اگر (یہ->accels[i])
	  یہ->accels[i]->destroy ();
	hb_free (یہ->accels[i]);
      }
      hb_free (یہ->accels);
      یہ->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { واپس جدول.get_blob (); }

    سانچہ <قسم_نام Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (غیرعلامتی chain_اشاریہ, ثابت Chain &chain, غیرعلامتی عدد_glyphs) ثابت
    {
      اگر (unlikely (chain_اشاریہ >= chain_گنتی)) واپس خالی_اشاریہ;

    retry:
      خودکار *accel = accels[chain_اشاریہ].get_acquire ();
      اگر (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, عدد_glyphs);
	اگر (unlikely (!accel))
	  واپس خالی_اشاریہ;

	اگر (unlikely (!accels[chain_اشاریہ].cmpexch (خالی_اشاریہ, accel)))
	{
	  hb_free (accel);
	  جاؤ retry;
	}
      }

      واپس accel;
    }

    hb_blob_ptr_t<T> جدول;
    غیرعلامتی صحیح chain_گنتی;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  خالی compile_flags (ثابت hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ثابت
  {
    ثابت Chain<Types> *chain = &پہلا_Chain;
    غیرعلامتی صحیح گنتی = chain_گنتی_2;
    اگر (unlikely (!map->chain_flags.resize (گنتی)))
      واپس;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_پہلا,
							     mapper->range_آخری});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  غیرعلامتی get_chain_گنتی () ثابت
  {
    واپس chain_گنتی_2;
  }
  خالی apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_map_t &map,
	      ثابت accelerator_t &accel) ثابت
  {
    اگر (unlikely (!c->بفر->successful)) واپس;

    c->بفر->unsafe_مقصد_concat ();

    c->تیاری_بفر_glyph_سیٹ ();

    c->سیٹ_lookup_اشاریہ (0);
    ثابت Chain<Types> *chain = &پہلا_Chain;
    غیرعلامتی صحیح گنتی = chain_گنتی_2;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      خودکار *chain_accel = accel.get_accel (i, *chain, c->face->get_عدد_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      اگر (unlikely (!c->بفر->successful)) واپس;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!(نسخہ.sanitize (c) &&
	  hb_barrier () &&
	  نسخہ &&
	  chain_گنتی_2.sanitize (c)))
      return_trace (جھوٹ);

    ثابت Chain<Types> *chain = &پہلا_Chain;
    غیرعلامتی صحیح گنتی = chain_گنتی_2;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (!chain->sanitize (c, نسخہ))
	return_trace (جھوٹ);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (سچ);
  }

  محفوظ:
  HBUINT16	نسخہ;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	غیر_مستعمل_2;		/* Set to 0. */
  HBUINT32	chain_گنتی_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	پہلا_Chain;	/* Chains. */

  عوامی:
  DEFINE_سائز_MIN (8);
};

ساخت morx : mortmorx<morx, ExtendedTypes, HB_AAT_لیبل_morx>
{
  HB_اندرونی منطقی is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

ساخت mort : mortmorx<mort, ObsoleteTypes, HB_AAT_لیبل_mort>
{
  HB_اندرونی منطقی is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

ساخت morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
ساخت mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#اگر_ختم /* HB_AAT_LAYOUT_MORX_TABLE_HH */
