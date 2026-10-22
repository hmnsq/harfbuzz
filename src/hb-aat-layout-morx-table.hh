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

#اگر_تعریف_نشده HB_AAT_LAYOUT_MORX_جدول_HH
#تعریف_کن HB_AAT_LAYOUT_MORX_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#تعریف_کن HB_AAT_برچسب_morx HB_برچسب('m','o','r','x')
#تعریف_کن HB_AAT_برچسب_mort HB_برچسب('m','o','r','t')


نطاق AAT {

استفاده نطاق OT;

قالب <نام_نوع Types>
بنية RearrangementSubtable
{
  تعريفنوع نام_نوع Types::HBUINT HBUINT;

  تعريفنوع فراغ مدخل_داده;

  تعداد Flags
  {
    Mark_اولی		= 0x8000,	/* If set, make the current glyph the first
					 * glyph to be rearranged. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. This means
					 * that the glyph index doesn't change, even
					 * if the glyph at that index has changed. */
    Mark_آخرین		= 0x2000,	/* If set, make the current glyph the last
					 * glyph to be rearranged. */
    Reserved		= 0x1FF0,	/* These bits are reserved and should be set to 0. */
    Verb		= 0x000F,	/* The type of rearrangement specified. */
  };

  منطقي is_action_initiable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    أرجع (مدخل.flags & Mark_اولی);
  }
  منطقي is_actionable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    أرجع (مدخل.flags & Verb);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = درست;

    driver_context_t (ثابت RearrangementSubtable *جدول_2) :
	ret (خطأ),
	جدول (جدول_2),
	شروع (0), انتها (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_میانگیر_t *میانگیر,
		     State_جدول_Driver<Types, مدخل_داده, Flags> *driver,
		     ثابت مدخل_2<مدخل_داده> &مدخل)
    {
      بیعلامت صحيح flags = مدخل.flags;

      إذا (flags & Mark_اولی)
	شروع = میانگیر->idx;

      إذا (flags & Mark_آخرین)
	انتها = hb_min (میانگیر->idx + 1, میانگیر->len);

      إذا ((flags & Verb) && شروع < انتها)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ثابت بیعلامت حرف map[16] =
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

	بیعلامت صحيح m = map[flags & Verb];
	بیعلامت صحيح l = hb_min (2u, m >> 4);
	بیعلامت صحيح r = hb_min (2u, m & 0x0F);
	منطقي معکوس_l = 3 == (m >> 4);
	منطقي معکوس_r = 3 == (m & 0x0F);

	إذا (انتها - شروع >= l + r && انتها-شروع <= HB_MAX_CONTEXT_درازی)
	{
	  میانگیر->ادغام_clusters (شروع, hb_min (میانگیر->idx + 1, میانگیر->len));
	  میانگیر->ادغام_clusters (شروع, انتها);

	  hb_glyph_info_t *info = میانگیر->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + شروع, l * حجم (buf[0]));
	  hb_memcpy (buf + 2, info + انتها - r, r * حجم (buf[0]));

	  إذا (l != r)
	    memmove (info + شروع + r, info + شروع + l, (انتها - شروع - l - r) * حجم (buf[0]));

	  hb_memcpy (info + شروع, buf + 2, r * حجم (buf[0]));
	  hb_memcpy (info + انتها - l, buf, l * حجم (buf[0]));
	  إذا (معکوس_l)
	  {
	    buf[0] = info[انتها - 1];
	    info[انتها - 1] = info[انتها - 2];
	    info[انتها - 2] = buf[0];
	  }
	  إذا (معکوس_r)
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
    بیعلامت صحيح شروع;
    بیعلامت صحيح انتها;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا);

    State_جدول_Driver<Types, مدخل_داده, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (machine.sanitize (c));
  }

  عام:
  State_جدول<Types, مدخل_داده>	machine;
  عام:
  DEFINE_اندازه_STATIC ((State_جدول<Types, مدخل_داده>::static_اندازه));
};

قالب <نام_نوع Types>
بنية ContextualSubtable
{
  تعريفنوع نام_نوع Types::HBUINT HBUINT;

  بنية مدخل_داده
  {
    تعريفنوع نام_نوع std::conditional<Types::extended, HBUINT16, HBINT16>::type جابه_جایی_Type;

    جابه_جایی_Type	mark_شاخص_2;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    جابه_جایی_Type	current_شاخص;	/* Index/offset of the substitution table for the
				 * current glyph. */
    عام:
    DEFINE_اندازه_STATIC (4);
  };

  تعداد Flags
  {
    مجموعه_Mark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  منطقي is_action_initiable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    أرجع (مدخل.flags & مجموعه_Mark);
  }
  منطقي is_actionable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    إذا (Types::extended)
      أرجع مدخل.داده.mark_شاخص_2 != 0xFFFF || مدخل.داده.current_شاخص != 0xFFFF;
    وإلا
      أرجع مدخل.داده.mark_شاخص_2 || مدخل.داده.current_شاخص;
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = درست;

    driver_context_t (ثابت ContextualSubtable *جدول_2,
			     hb_aat_apply_context_t *c_) :
	ret (خطأ),
	c (c_),
	جدول (جدول_2),
	mark_مجموعه (خطأ),
	mark (0),
	subs (جدول+جدول->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_میانگیر_t *میانگیر,
		     State_جدول_Driver<Types, مدخل_داده, Flags> *driver,
		     ثابت مدخل_2<مدخل_داده> &مدخل)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      إذا (میانگیر->idx == میانگیر->len && !mark_مجموعه)
	أرجع;

      ثابت HBGlyphID16 *replacement;

      replacement = اشاره_تهی;
      إذا (Types::extended)
      {
	إذا (مدخل.داده.mark_شاخص_2 != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[مدخل.داده.mark_شاخص_2];
	  replacement = lookup.get_مقدار (میانگیر->info[mark].codepoint, driver->رقم_glyphs);
	}
      }
      وإلا
      {
	إذا (مدخل.داده.mark_شاخص_2)
	{
	  صحيح جابه_جایی = (صحيح) مدخل.داده.mark_شاخص_2 + میانگیر->info[mark].codepoint;
	  ثابت Unsized_Array_از<HBGlyphID16> &subs_old = (ثابت Unsized_Array_از<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::کلمه_جابه_جایی_مقصد_شاخص (جابه_جایی, جدول, subs_old.arrayZ)];
	  إذا (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = اشاره_تهی;
	}
      }
      إذا (replacement)
      {
	میانگیر->unsafe_مقصد_break (mark, hb_min (میانگیر->idx + 1, میانگیر->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = درست;
      }

      replacement = اشاره_تهی;
      بیعلامت صحيح idx = hb_min (میانگیر->idx, میانگیر->len - 1);
      إذا (Types::extended)
      {
	إذا (مدخل.داده.current_شاخص != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[مدخل.داده.current_شاخص];
	  replacement = lookup.get_مقدار (میانگیر->info[idx].codepoint, driver->رقم_glyphs);
	}
      }
      وإلا
      {
	إذا (مدخل.داده.current_شاخص)
	{
	  صحيح جابه_جایی = (صحيح) مدخل.داده.current_شاخص + میانگیر->info[idx].codepoint;
	  ثابت Unsized_Array_از<HBGlyphID16> &subs_old = (ثابت Unsized_Array_از<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::کلمه_جابه_جایی_مقصد_شاخص (جابه_جایی, جدول, subs_old.arrayZ)];
	  إذا (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = اشاره_تهی;
	}
      }
      إذا (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = درست;
      }

      إذا (مدخل.flags & مجموعه_Mark)
      {
	mark_مجموعه = درست;
	mark = میانگیر->idx;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت ContextualSubtable *جدول;
    خاص:
    منطقي mark_مجموعه;
    بیعلامت صحيح mark;
    ثابت Unsized_List_از_Offset16_مقصد<Lookup<HBGlyphID16>, HBUINT, فراغ, خطأ> &subs;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, مدخل_داده, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    بیعلامت صحيح رقم_entries = 0;
    إذا (unlikely (!machine.sanitize (c, &رقم_entries))) return_trace (خطأ);
    hb_barrier ();

    إذا (!Types::extended)
      return_trace (substitutionTables.sanitize (c, هذا, 0));

    بیعلامت صحيح رقم_lookups = 0;

    ثابت مدخل_2<مدخل_داده> *entries = machine.get_entries ();
    لكل (بیعلامت صحيح i = 0; i < رقم_entries; i++)
    {
      ثابت مدخل_داده &داده = entries[i].داده;

      إذا (داده.mark_شاخص_2 != 0xFFFF)
	رقم_lookups = hb_max (رقم_lookups, 1u + داده.mark_شاخص_2);
      إذا (داده.current_شاخص != 0xFFFF)
	رقم_lookups = hb_max (رقم_lookups, 1u + داده.current_شاخص);
    }

    return_trace (substitutionTables.sanitize (c, هذا, رقم_lookups));
  }

  عام:
  State_جدول<Types, مدخل_داده>
		machine;
  محمي:
  NNOffset_مقصد<Unsized_List_از_Offset16_مقصد<Lookup<HBGlyphID16>, HBUINT, فراغ, خطأ>, HBUINT>
		substitutionTables;
  عام:
  DEFINE_اندازه_STATIC ((State_جدول<Types, مدخل_داده>::static_اندازه + HBUINT::static_اندازه));
};


قالب <منطقي extended>
بنية Ligature_مدخل;

قالب <>
بنية Ligature_مدخل<درست>
{

  بنية مدخل_داده
  {
    HBUINT16	lig_Action_شاخص;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    عام:
    DEFINE_اندازه_STATIC (2);
  };

  تعداد Flags
  {
    مجموعه_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  ساكن منطقي initiateAction (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.flags & مجموعه_Component; }

  ساكن منطقي performAction (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.flags & PerformAction; }

  ساكن بیعلامت صحيح lig_Action_شاخص (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.داده.lig_Action_شاخص; }
};
قالب <>
بنية Ligature_مدخل<خطأ>
{
  تعريفنوع فراغ مدخل_داده;

  تعداد Flags
  {
    مجموعه_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    جابه_جایی_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  ساكن منطقي initiateAction (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.flags & مجموعه_Component; }

  ساكن منطقي performAction (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.flags & جابه_جایی_2; }

  ساكن بیعلامت صحيح lig_Action_شاخص (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.flags & جابه_جایی_2; }
};


قالب <نام_نوع Types>
بنية LigatureSubtable
{
  تعريفنوع نام_نوع Types::HBUINT HBUINT;

  تعريفنوع Ligature_مدخل<Types::extended> Ligature_مدخل_T;
  تعريفنوع نام_نوع Ligature_مدخل_T::مدخل_داده مدخل_داده;

  تعداد Flags
  {
    DontAdvance	= Ligature_مدخل_T::DontAdvance,
  };

  منطقي is_action_initiable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    أرجع Ligature_مدخل_T::initiateAction (مدخل);
  }
  منطقي is_actionable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    أرجع Ligature_مدخل_T::performAction (مدخل);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = خطأ;
    تعداد LigActionFlags
    {
      Lig_Action_آخرین	= 0x80000000,	/* This is the last action in the list. This also
					 * implies storage. */
      LigActionStore	= 0x40000000,	/* Store the ligature at the current cumulated index
					 * in the ligature table in place of the marked
					 * (i.e. currently-popped) glyph. */
      Lig_Action_جابه_جایی	= 0x3FFFFFFF,	/* A 30-bit value which is sign-extended to 32-bits
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
	تطابق_درازی (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_میانگیر_t *میانگیر,
		     State_جدول_Driver<Types, مدخل_داده, Flags> *driver,
		     ثابت مدخل_2<مدخل_داده> &مدخل)
    {
      DEBUG_پیام (APPLY, اشاره_تهی, "Ligature transition at %u", میانگیر->idx);
      إذا (مدخل.flags & Ligature_مدخل_T::مجموعه_Component)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	إذا (unlikely (تطابق_درازی && تطابق_positions[(تطابق_درازی - 1u) % ARRAY_درازی (تطابق_positions)] == میانگیر->بیرون_len))
	  تطابق_درازی--;

	تطابق_positions[تطابق_درازی++ % ARRAY_درازی (تطابق_positions)] = میانگیر->بیرون_len;
	DEBUG_پیام (APPLY, اشاره_تهی, "Set component at %u", میانگیر->بیرون_len);
      }

      إذا (Ligature_مدخل_T::performAction (مدخل))
      {
	DEBUG_پیام (APPLY, اشاره_تهی, "Perform action with %u", تطابق_درازی);
	بیعلامت صحيح انتها = میانگیر->بیرون_len;

	إذا (unlikely (!تطابق_درازی))
	  أرجع;

	إذا (میانگیر->idx >= میانگیر->len)
	  أرجع; /* TODO Work on previous instead? */

	بیعلامت صحيح cursor = تطابق_درازی;

	بیعلامت صحيح action_idx = Ligature_مدخل_T::lig_Action_شاخص (مدخل);
	action_idx = Types::جابه_جایی_مقصد_شاخص (action_idx, جدول, ligAction.arrayZ);
	ثابت HBUINT32 *action_داده = &ligAction[action_idx];

	بیعلامت صحيح ligature_idx = 0;
	بیعلامت صحيح action;
	افعل
	{
	  إذا (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_پیام (APPLY, اشاره_تهی, "Stack underflow");
	    تطابق_درازی = 0;
	    اكسر;
	  }

	  DEBUG_پیام (APPLY, اشاره_تهی, "Moving to stack position %u", cursor - 1);
	  إذا (unlikely (!میانگیر->move_مقصد (تطابق_positions[--cursor % ARRAY_درازی (تطابق_positions)]))) أرجع;

	  إذا (unlikely (!action_داده->sanitize (&c->sanitizer))) اكسر;
	  hb_barrier ();
	  action = *action_داده;

	  uint32_t uoffset = action & Lig_Action_جابه_جایی;
	  إذا (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t جابه_جایی = (int32_t) uoffset;
	  بیعلامت صحيح component_idx = میانگیر->cur().codepoint + جابه_جایی;
	  component_idx = Types::کلمه_جابه_جایی_مقصد_شاخص (component_idx, جدول, component.arrayZ);
	  ثابت HBUINT16 &component_داده = component[component_idx];
	  إذا (unlikely (!component_داده.sanitize (&c->sanitizer))) اكسر;
	  hb_barrier ();
	  ligature_idx += component_داده;

	  DEBUG_پیام (APPLY, اشاره_تهی, "Action store %d last %d",
		     منطقي (action & LigActionStore),
		     منطقي (action & Lig_Action_آخرین));
	  إذا (action & (LigActionStore | Lig_Action_آخرین))
	  {
	    ligature_idx = Types::جابه_جایی_مقصد_شاخص (ligature_idx, جدول, ligature.arrayZ);
	    ثابت HBGlyphID16 &ligature_داده = ligature[ligature_idx];
	    إذا (unlikely (!ligature_داده.sanitize (&c->sanitizer))) اكسر;
	    hb_barrier ();
	    hb_codepoint_t lig = ligature_داده;

	    DEBUG_پیام (APPLY, اشاره_تهی, "Produced ligature %u", lig);
	    إذا (unlikely (!c->replace_glyph (lig))) أرجع;

	    بیعلامت صحيح lig_انتها = تطابق_positions[(تطابق_درازی - 1u) % ARRAY_درازی (تطابق_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    طالما (تطابق_درازی - 1u > cursor)
	    {
	      DEBUG_پیام (APPLY, اشاره_تهی, "Skipping ligature component");
	      إذا (unlikely (!میانگیر->move_مقصد (تطابق_positions[--تطابق_درازی % ARRAY_درازی (تطابق_positions)]))) أرجع;
	      إذا (!c->delete_glyph ()) أرجع;
	    }

	    إذا (unlikely (!میانگیر->move_مقصد (lig_انتها))) أرجع;
	    میانگیر->ادغام_بیرون_clusters (تطابق_positions[cursor % ARRAY_درازی (تطابق_positions)], میانگیر->بیرون_len);
	  }

	  action_داده++;
	}
	طالما (!(action & Lig_Action_آخرین));
	إذا (unlikely (!میانگیر->move_مقصد (انتها))) أرجع;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت LigatureSubtable *جدول;
    خاص:
    ثابت Unsized_Array_از<HBUINT32> &ligAction;
    ثابت Unsized_Array_از<HBUINT16> &component;
    ثابت Unsized_Array_از<HBGlyphID16> &ligature;
    بیعلامت صحيح تطابق_درازی;
    بیعلامت صحيح تطابق_positions[HB_MAX_CONTEXT_درازی];
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, مدخل_داده, Flags> driver (machine, c->face);

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
  State_جدول<Types, مدخل_داده>
		machine;
  محمي:
  NNOffset_مقصد<Unsized_Array_از<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffset_مقصد<Unsized_Array_از<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffset_مقصد<Unsized_Array_از<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  عام:
  DEFINE_اندازه_STATIC ((State_جدول<Types, مدخل_داده>::static_اندازه + 3 * HBUINT::static_اندازه));
};

قالب <نام_نوع Types>
بنية NoncontextualSubtable
{
  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    منطقي ret = خطأ;
    بیعلامت صحيح رقم_glyphs = c->face->get_رقم_glyphs ();

    hb_glyph_info_t *info = c->میانگیر->info;
    بیعلامت صحيح شمار = c->میانگیر->len;
    // If there's only one range, we already checked the flag.
    خودکار *آخرین_range = c->range_flags && (c->range_flags->درازی > 1) ? &(*c->range_flags)[0] : اشاره_تهی;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      إذا (unlikely (آخرین_range))
      {
	خودکار *range = آخرین_range;
	{
	  بیعلامت cluster = info[i].cluster;
	  طالما (cluster < range->cluster_اولی)
	    range--;
	  طالما (cluster > range->cluster_آخرین)
	    range++;

	  آخرین_range = range;
	}
	إذا (!(range->flags & c->subtable_flags))
	  استمر;
      }

      ثابت HBGlyphID16 *replacement = substitute.get_مقدار (info[i].codepoint, رقم_glyphs);
      إذا (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = درست;
      }
    }

    return_trace (ret);
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_initial_glyphs (مجموعه_t &glyphs, بیعلامت رقم_glyphs) ثابت
  {
    substitute.collect_glyphs (glyphs, رقم_glyphs);
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

قالب <نام_نوع Types>
بنية InsertionSubtable
{
  تعريفنوع نام_نوع Types::HBUINT HBUINT;

  بنية مدخل_داده
  {
    HBUINT16	current_درج_شاخص;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_درج_شاخص;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    عام:
    DEFINE_اندازه_STATIC (4);
  };

  تعداد Flags
  {
    مجموعه_Mark		= 0x8000,     /* If set, mark the current glyph. */
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
    Current_درج_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_درج_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_درج_شمار= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_درج_شمار= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  منطقي is_action_initiable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    أرجع (مدخل.flags & مجموعه_Mark);
  }
  منطقي is_actionable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    أرجع (مدخل.flags & (Current_درج_شمار | Marked_درج_شمار)) &&
	   (مدخل.داده.current_درج_شاخص != 0xFFFF ||مدخل.داده.marked_درج_شاخص != 0xFFFF);
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
    فراغ transition (hb_میانگیر_t *میانگیر,
		     State_جدول_Driver<Types, مدخل_داده, Flags> *driver,
		     ثابت مدخل_2<مدخل_داده> &مدخل)
    {
      بیعلامت صحيح flags = مدخل.flags;

      بیعلامت mark_loc = میانگیر->بیرون_len;

      إذا (مدخل.داده.marked_درج_شاخص != 0xFFFF)
      {
	بیعلامت صحيح شمار = (flags & Marked_درج_شمار);
	إذا (unlikely ((میانگیر->max_ops -= شمار) <= 0)) أرجع;
	بیعلامت صحيح شروع = مدخل.داده.marked_درج_شاخص;
	ثابت HBGlyphID16 *glyphs = &insertionAction[شروع];
	إذا (unlikely (!c->sanitizer.check_array (glyphs, شمار))) شمار = 0;
	hb_barrier ();

	منطقي before = flags & Marked_درج_Before;

	بیعلامت صحيح انتها = میانگیر->بیرون_len;
	إذا (unlikely (!میانگیر->move_مقصد (mark))) أرجع;

	إذا (میانگیر->idx < میانگیر->len && !before)
	  إذا (unlikely (!میانگیر->copy_glyph ())) أرجع;
	/* TODO We ignore KashidaLike setting. */
	إذا (unlikely (!c->output_glyphs (شمار, glyphs))) أرجع;
	ret = درست;
	إذا (میانگیر->idx < میانگیر->len && !before)
	  میانگیر->skip_glyph ();

	إذا (unlikely (!میانگیر->move_مقصد (انتها + شمار))) أرجع;

	میانگیر->unsafe_مقصد_break_from_outbuffer (mark, hb_min (میانگیر->idx + 1, میانگیر->len));
      }

      إذا (flags & مجموعه_Mark)
	mark = mark_loc;

      إذا (مدخل.داده.current_درج_شاخص != 0xFFFF)
      {
	بیعلامت صحيح شمار = (flags & Current_درج_شمار) >> 5;
	إذا (unlikely ((میانگیر->max_ops -= شمار) <= 0)) أرجع;
	بیعلامت صحيح شروع = مدخل.داده.current_درج_شاخص;
	ثابت HBGlyphID16 *glyphs = &insertionAction[شروع];
	إذا (unlikely (!c->sanitizer.check_array (glyphs, شمار))) شمار = 0;
	hb_barrier ();

	منطقي before = flags & Current_درج_Before;

	بیعلامت صحيح انتها = میانگیر->بیرون_len;

	إذا (میانگیر->idx < میانگیر->len && !before)
	  إذا (unlikely (!میانگیر->copy_glyph ())) أرجع;
	/* TODO We ignore KashidaLike setting. */
	إذا (unlikely (!c->output_glyphs (شمار, glyphs))) أرجع;
	ret = درست;
	إذا (میانگیر->idx < میانگیر->len && !before)
	  میانگیر->skip_glyph ();

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
	إذا (unlikely (!میانگیر->move_مقصد ((flags & DontAdvance) ? انتها : انتها + شمار))) أرجع;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت InsertionSubtable *جدول;
    خاص:
    بیعلامت صحيح mark;
    ثابت Unsized_Array_از<HBGlyphID16> &insertionAction;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, مدخل_داده, Flags> driver (machine, c->face);

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
  State_جدول<Types, مدخل_داده>
		machine;
  محمي:
  NNOffset_مقصد<Unsized_Array_از<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  عام:
  DEFINE_اندازه_STATIC ((State_جدول<Types, مدخل_داده>::static_اندازه + HBUINT::static_اندازه));
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
    دوست بنية hb_accelerate_subtables_context_t;
    دوست بنية hb_aat_layout_lookup_accelerator_t;

    عام:
    hb_بیت_مجموعه_t glyph_مجموعه;
    تغییرپذیر hb_aat_class_cache_t class_cache;

    قالب <نام_نوع T>
    خودکار init_ (ثابت T &obj_, بیعلامت رقم_glyphs, hb_اولویت<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_مجموعه, رقم_glyphs, obj_)
    )

    قالب <نام_نوع T>
    فراغ init_ (ثابت T &obj_, بیعلامت رقم_glyphs, hb_اولویت<0>)
    {
      obj_.collect_initial_glyphs (glyph_مجموعه, رقم_glyphs);
    }

    قالب <نام_نوع T>
    فراغ init (ثابت T &obj_, بیعلامت رقم_glyphs)
    {
      glyph_مجموعه.init ();
      init_ (obj_, رقم_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    فراغ
    fini ()
    {
      glyph_مجموعه.fini ();
    }
  };

  /* Dispatch interface. */
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj)
  {
    hb_applicable_t *مدخل = &array[i++];

    مدخل->init (obj, رقم_glyphs);

    أرجع hb_خالی_t ();
  }
  ساكن return_t default_return_مقدار () { أرجع hb_خالی_t (); }

  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع خطأ; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, بیعلامت رقم_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), رقم_glyphs (رقم_glyphs_2) {}

  hb_applicable_t *array;
  بیعلامت رقم_glyphs;
  بیعلامت i = 0;
};

بنية hb_aat_layout_chain_accelerator_t
{
  قالب <نام_نوع TChain>
  ساكن hb_aat_layout_chain_accelerator_t *create (ثابت TChain &chain, بیعلامت رقم_glyphs)
  {
    بیعلامت شمار = chain.get_subtable_شمار ();

    بیعلامت حاصلضرب;
    إذا (unlikely (hb_unsigned_mul_overflows (شمار,
        حجم (hb_accelerate_subtables_context_t::hb_applicable_t), &حاصلضرب)))
      أرجع اشاره_تهی;

    بیعلامت اندازه;
    إذا (unlikely (hb_unsigned_جمع_کن_overflows (حجم (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t), حاصلضرب, &اندازه))) {
      أرجع اشاره_تهی;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    خودکار *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, اندازه);
    إذا (unlikely (!thiz))
      أرجع اشاره_تهی;

    thiz->شمار = شمار;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, رقم_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    أرجع thiz;
  }

  فراغ destroy ()
  {
    لكل (بیعلامت i = 0; i < شمار; i++)
      subtables[i].fini ();
  }

  بیعلامت شمار;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

قالب <نام_نوع Types>
بنية ChainSubtable
{
  تعريفنوع نام_نوع Types::HBUINT HBUINT;

  قالب <نام_نوع T>
  دوست بنية Chain;

  size_t get_اندازه () ثابت     { أرجع درازی; }
  بیعلامت صحيح get_type () ثابت     { أرجع coverage & 0xFF; }
  بیعلامت صحيح get_coverage () ثابت { أرجع coverage >> (حجم (HBUINT) * 8 - 8); }

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

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بیعلامت صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    حالة Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    حالة Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    حالة Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    حالة Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    افتراضي:			return_trace (c->default_return_مقدار ());
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
    إذا (!(درازی.sanitize (c) &&
	  hb_barrier () &&
	  درازی >= min_اندازه &&
	  c->check_range (هذا, درازی)))
      return_trace (خطأ);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  محمي:
  HBUINT	درازی;		/* Total subtable length, including this header. */
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

قالب <نام_نوع Types>
بنية Chain
{
  تعريفنوع نام_نوع Types::HBUINT HBUINT;

  بیعلامت get_subtable_شمار () ثابت { أرجع subtable_شمار_2; }

  hb_ماسک_t compile_flags (ثابت hb_aat_map_builder_t *map) ثابت
  {
    hb_ماسک_t flags = defaultFlags;
    {
      بیعلامت صحيح شمار = feature_شمار;
      لكل (بیعلامت i = 0; i < شمار; i++)
      {
	ثابت Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (بیعلامت صحيح) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (بیعلامت صحيح) feature.featureSetting;
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
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_پایینی_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_پایینی_CASE_SMALL_CAPS;
	  اذهب retry;
	}
#اگر_تعریف_نشده HB_NO_AAT
	وإلا إذا (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_برچسب_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->جدول.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#پایان_اگر
      }
    }
    أرجع flags;
  }

  فراغ apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_layout_chain_accelerator_t *accel) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_شمار));
    بیعلامت صحيح شمار = subtable_شمار_2;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      منطقي reverse;

      خودکار coverage = subtable->get_coverage ();

      hb_ماسک_t subtable_flags = subtable->subFeatureFlags;
      إذا (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ثابت hb_aat_map_t::range_flags_t _) -> منطقي { أرجع subtable_flags & (_.flags); })))
	اذهب skip;

      إذا (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->میانگیر->props.direction) !=
	  منطقي (coverage & ChainSubtable<Types>::Vertical))
	اذهب skip;

      c->subtable_flags = subtable_flags;
      c->اولی_مجموعه = accel ? &accel->subtables[i].glyph_مجموعه : &Null(hb_بیت_مجموعه_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : اشاره_تهی;

      إذا (!c->میانگیر_intersects_machine ())
      {
	(فراغ) c->میانگیر->پیام (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_شاخص);
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
		HB_DIRECTION_IS_BACKWARD (c->میانگیر->props.direction);

      إذا (!c->میانگیر->پیام (c->font, "start chainsubtable %u", c->lookup_شاخص))
	اذهب skip;

      إذا (reverse != c->میانگیر_is_reversed)
        c->معکوس_میانگیر ();

      subtable->apply (c);

      (فراغ) c->میانگیر->پیام (c->font, "end chainsubtable %u", c->lookup_شاخص);

      إذا (unlikely (!c->میانگیر->successful)) اكسر;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->مجموعه_lookup_شاخص (c->lookup_شاخص + 1);
    }
    إذا (c->میانگیر_is_reversed)
      c->معکوس_میانگیر ();
  }

  size_t get_اندازه () ثابت { أرجع درازی; }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_شمار));
    بیعلامت صحيح شمار = subtable_شمار_2;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      نام_نوع context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      إذا (c->stop_sublookup_iteration (ret))
	أرجع ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    أرجع c->default_return_مقدار ();
  }

  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح نسخه) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(درازی.sanitize (c) &&
	  hb_barrier () &&
	  درازی >= min_اندازه &&
	  c->check_range (هذا, درازی)))
      return_trace (خطأ);

    إذا (!c->check_array (featureZ.arrayZ, feature_شمار))
      return_trace (خطأ);

    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_شمار));
    بیعلامت صحيح شمار = subtable_شمار_2;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (!subtable->sanitize (c))
	return_trace (خطأ);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    إذا (نسخه >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) subtable;
      إذا (!coverage->sanitize (c, شمار))
        return_trace (خطأ);
    }

    return_trace (درست);
  }

  محمي:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	درازی;		/* Total byte count, including this header. */
  HBUINT	feature_شمار;	/* Number of feature subtable entries. */
  HBUINT	subtable_شمار_2;	/* The number of subtables in the chain. */

  Unsized_Array_از<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  عام:
  DEFINE_اندازه_MIN (8 + 2 * حجم (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

قالب <نام_نوع T, نام_نوع Types, hb_برچسب_t برچسب_3>
بنية mortmorx
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = برچسب_3;

  منطقي has_داده () ثابت { أرجع نسخه != 0; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->جدول = sc.reference_جدول<T> (face);

      إذا (unlikely (هذا->جدول->is_blocklisted (هذا->جدول.get_blob (), face)))
      {
        hb_blob_destroy (هذا->جدول.get_blob ());
        هذا->جدول = hb_blob_get_خالی ();
      }

      هذا->chain_شمار = جدول->get_chain_شمار ();

      هذا->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (هذا->chain_شمار, حجم (*accels));
      إذا (unlikely (!هذا->accels))
      {
	هذا->chain_شمار = 0;
	هذا->جدول.destroy ();
	هذا->جدول = hb_blob_get_خالی ();
      }
    }
    ~accelerator_t ()
    {
      لكل (بیعلامت صحيح i = 0; i < هذا->chain_شمار; i++)
      {
	إذا (هذا->accels[i])
	  هذا->accels[i]->destroy ();
	hb_free (هذا->accels[i]);
      }
      hb_free (هذا->accels);
      هذا->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع جدول.get_blob (); }

    قالب <نام_نوع Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (بیعلامت chain_شاخص, ثابت Chain &chain, بیعلامت رقم_glyphs) ثابت
    {
      إذا (unlikely (chain_شاخص >= chain_شمار)) أرجع اشاره_تهی;

    retry:
      خودکار *accel = accels[chain_شاخص].get_acquire ();
      إذا (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, رقم_glyphs);
	إذا (unlikely (!accel))
	  أرجع اشاره_تهی;

	إذا (unlikely (!accels[chain_شاخص].cmpexch (اشاره_تهی, accel)))
	{
	  hb_free (accel);
	  اذهب retry;
	}
      }

      أرجع accel;
    }

    hb_blob_ptr_t<T> جدول;
    بیعلامت صحيح chain_شمار;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  فراغ compile_flags (ثابت hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ثابت
  {
    ثابت Chain<Types> *chain = &اولی_Chain;
    بیعلامت صحيح شمار = chain_شمار_2;
    إذا (unlikely (!map->chain_flags.resize (شمار)))
      أرجع;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_اولی,
							     mapper->range_آخرین});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  بیعلامت get_chain_شمار () ثابت
  {
    أرجع chain_شمار_2;
  }
  فراغ apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_map_t &map,
	      ثابت accelerator_t &accel) ثابت
  {
    إذا (unlikely (!c->میانگیر->successful)) أرجع;

    c->میانگیر->unsafe_مقصد_concat ();

    c->راهاندازی_میانگیر_glyph_مجموعه ();

    c->مجموعه_lookup_شاخص (0);
    ثابت Chain<Types> *chain = &اولی_Chain;
    بیعلامت صحيح شمار = chain_شمار_2;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      خودکار *chain_accel = accel.get_accel (i, *chain, c->face->get_رقم_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      إذا (unlikely (!c->میانگیر->successful)) أرجع;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(نسخه.sanitize (c) &&
	  hb_barrier () &&
	  نسخه &&
	  chain_شمار_2.sanitize (c)))
      return_trace (خطأ);

    ثابت Chain<Types> *chain = &اولی_Chain;
    بیعلامت صحيح شمار = chain_شمار_2;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (!chain->sanitize (c, نسخه))
	return_trace (خطأ);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (درست);
  }

  محمي:
  HBUINT16	نسخه;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	استفادهنشده_2;		/* Set to 0. */
  HBUINT32	chain_شمار_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	اولی_Chain;	/* Chains. */

  عام:
  DEFINE_اندازه_MIN (8);
};

بنية morx : mortmorx<morx, ExtendedTypes, HB_AAT_برچسب_morx>
{
  HB_داخلی منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

بنية mort : mortmorx<mort, ObsoleteTypes, HB_AAT_برچسب_mort>
{
  HB_داخلی منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

بنية morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
بنية mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#پایان_اگر /* HB_AAT_LAYOUT_MORX_TABLE_HH */
