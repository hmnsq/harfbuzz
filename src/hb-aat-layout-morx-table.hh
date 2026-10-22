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

#جيڪڏهن_وصف_نه_ٿيل HB_AAT_LAYOUT_MORX_جدول_HH
#وصف_ڪريو HB_AAT_LAYOUT_MORX_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#وصف_ڪريو HB_AAT_ٽيگ_morx HB_ٽيگ('m','o','r','x')
#وصف_ڪريو HB_AAT_ٽيگ_mort HB_ٽيگ('m','o','r','t')


نطاق AAT {

استعمال نطاق OT;

سانچو <قسم_نالو Types>
بنية RearrangementSubtable
{
  تعريفنوع قسم_نالو Types::HBUINT HBUINT;

  تعريفنوع فراغ داخلا_معلومات;

  تعداد Flags
  {
    Mark_پهريون		= 0x8000,	/* If set, make the current glyph the first
					 * glyph to be rearranged. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. This means
					 * that the glyph index doesn't change, even
					 * if the glyph at that index has changed. */
    Mark_آخري		= 0x2000,	/* If set, make the current glyph the last
					 * glyph to be rearranged. */
    Reserved		= 0x1FF0,	/* These bits are reserved and should be set to 0. */
    Verb		= 0x000F,	/* The type of rearrangement specified. */
  };

  منطقي is_action_initiable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    أرجع (داخلا.flags & Mark_پهريون);
  }
  منطقي is_actionable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    أرجع (داخلا.flags & Verb);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = سچ;

    driver_context_t (ثابت RearrangementSubtable *جدول_2) :
	ret (خطأ),
	جدول (جدول_2),
	شروعات (0), پڇاڙي (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, داخلا_معلومات, Flags> *driver,
		     ثابت داخلا_4<داخلا_معلومات> &داخلا)
    {
      بنانشان صحيح flags = داخلا.flags;

      إذا (flags & Mark_پهريون)
	شروعات = بفر->idx;

      إذا (flags & Mark_آخري)
	پڇاڙي = hb_min (بفر->idx + 1, بفر->len);

      إذا ((flags & Verb) && شروعات < پڇاڙي)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ثابت بنانشان حرف map[16] =
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

	بنانشان صحيح m = map[flags & Verb];
	بنانشان صحيح l = hb_min (2u, m >> 4);
	بنانشان صحيح r = hb_min (2u, m & 0x0F);
	منطقي ابتو_l = 3 == (m >> 4);
	منطقي ابتو_r = 3 == (m & 0x0F);

	إذا (پڇاڙي - شروعات >= l + r && پڇاڙي-شروعات <= HB_MAX_CONTEXT_ڊيگهائي)
	{
	  بفر->ملايو_clusters (شروعات, hb_min (بفر->idx + 1, بفر->len));
	  بفر->ملايو_clusters (شروعات, پڇاڙي);

	  hb_glyph_info_t *info = بفر->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + شروعات, l * حجم (buf[0]));
	  hb_memcpy (buf + 2, info + پڇاڙي - r, r * حجم (buf[0]));

	  إذا (l != r)
	    memmove (info + شروعات + r, info + شروعات + l, (پڇاڙي - شروعات - l - r) * حجم (buf[0]));

	  hb_memcpy (info + شروعات, buf + 2, r * حجم (buf[0]));
	  hb_memcpy (info + پڇاڙي - l, buf, l * حجم (buf[0]));
	  إذا (ابتو_l)
	  {
	    buf[0] = info[پڇاڙي - 1];
	    info[پڇاڙي - 1] = info[پڇاڙي - 2];
	    info[پڇاڙي - 2] = buf[0];
	  }
	  إذا (ابتو_r)
	  {
	    buf[0] = info[شروعات];
	    info[شروعات] = info[شروعات + 1];
	    info[شروعات + 1] = buf[0];
	  }
	}
      }
    }

    عام:
    منطقي ret;
    ثابت RearrangementSubtable *جدول;
    خاص:
    بنانشان صحيح شروعات;
    بنانشان صحيح پڇاڙي;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا);

    State_جدول_Driver<Types, داخلا_معلومات, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (machine.sanitize (c));
  }

  عام:
  State_جدول<Types, داخلا_معلومات>	machine;
  عام:
  DEFINE_سائيز_STATIC ((State_جدول<Types, داخلا_معلومات>::static_سائيز));
};

سانچو <قسم_نالو Types>
بنية ContextualSubtable
{
  تعريفنوع قسم_نالو Types::HBUINT HBUINT;

  بنية داخلا_معلومات
  {
    تعريفنوع قسم_نالو std::conditional<Types::extended, HBUINT16, HBINT16>::type وٿي_Type;

    وٿي_Type	mark_اشاريو_2;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    وٿي_Type	current_اشاريو;	/* Index/offset of the substitution table for the
				 * current glyph. */
    عام:
    DEFINE_سائيز_STATIC (4);
  };

  تعداد Flags
  {
    سيٽ_Mark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  منطقي is_action_initiable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    أرجع (داخلا.flags & سيٽ_Mark);
  }
  منطقي is_actionable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    إذا (Types::extended)
      أرجع داخلا.معلومات.mark_اشاريو_2 != 0xFFFF || داخلا.معلومات.current_اشاريو != 0xFFFF;
    وإلا
      أرجع داخلا.معلومات.mark_اشاريو_2 || داخلا.معلومات.current_اشاريو;
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = سچ;

    driver_context_t (ثابت ContextualSubtable *جدول_2,
			     hb_aat_apply_context_t *c_) :
	ret (خطأ),
	c (c_),
	جدول (جدول_2),
	mark_سيٽ (خطأ),
	mark (0),
	subs (جدول+جدول->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, داخلا_معلومات, Flags> *driver,
		     ثابت داخلا_4<داخلا_معلومات> &داخلا)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      إذا (بفر->idx == بفر->len && !mark_سيٽ)
	أرجع;

      ثابت HBGlyphID16 *replacement;

      replacement = خالي_اشارو;
      إذا (Types::extended)
      {
	إذا (داخلا.معلومات.mark_اشاريو_2 != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[داخلا.معلومات.mark_اشاريو_2];
	  replacement = lookup.get_قدر (بفر->info[mark].codepoint, driver->انگ_glyphs);
	}
      }
      وإلا
      {
	إذا (داخلا.معلومات.mark_اشاريو_2)
	{
	  صحيح وٿي = (صحيح) داخلا.معلومات.mark_اشاريو_2 + بفر->info[mark].codepoint;
	  ثابت Unsized_Array_جو<HBGlyphID16> &subs_old = (ثابت Unsized_Array_جو<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::لفظ_وٿي_منزل_اشاريو (وٿي, جدول, subs_old.arrayZ)];
	  إذا (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = خالي_اشارو;
	}
      }
      إذا (replacement)
      {
	بفر->unsafe_منزل_break (mark, hb_min (بفر->idx + 1, بفر->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = سچ;
      }

      replacement = خالي_اشارو;
      بنانشان صحيح idx = hb_min (بفر->idx, بفر->len - 1);
      إذا (Types::extended)
      {
	إذا (داخلا.معلومات.current_اشاريو != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[داخلا.معلومات.current_اشاريو];
	  replacement = lookup.get_قدر (بفر->info[idx].codepoint, driver->انگ_glyphs);
	}
      }
      وإلا
      {
	إذا (داخلا.معلومات.current_اشاريو)
	{
	  صحيح وٿي = (صحيح) داخلا.معلومات.current_اشاريو + بفر->info[idx].codepoint;
	  ثابت Unsized_Array_جو<HBGlyphID16> &subs_old = (ثابت Unsized_Array_جو<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::لفظ_وٿي_منزل_اشاريو (وٿي, جدول, subs_old.arrayZ)];
	  إذا (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = خالي_اشارو;
	}
      }
      إذا (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = سچ;
      }

      إذا (داخلا.flags & سيٽ_Mark)
      {
	mark_سيٽ = سچ;
	mark = بفر->idx;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت ContextualSubtable *جدول;
    خاص:
    منطقي mark_سيٽ;
    بنانشان صحيح mark;
    ثابت Unsized_List_جو_Offset16_منزل<Lookup<HBGlyphID16>, HBUINT, فراغ, خطأ> &subs;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, داخلا_معلومات, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    بنانشان صحيح انگ_entries = 0;
    إذا (unlikely (!machine.sanitize (c, &انگ_entries))) return_trace (خطأ);
    hb_barrier ();

    إذا (!Types::extended)
      return_trace (substitutionTables.sanitize (c, هذا, 0));

    بنانشان صحيح انگ_lookups = 0;

    ثابت داخلا_4<داخلا_معلومات> *entries = machine.get_entries ();
    لكل (بنانشان صحيح i = 0; i < انگ_entries; i++)
    {
      ثابت داخلا_معلومات &معلومات = entries[i].معلومات;

      إذا (معلومات.mark_اشاريو_2 != 0xFFFF)
	انگ_lookups = hb_max (انگ_lookups, 1u + معلومات.mark_اشاريو_2);
      إذا (معلومات.current_اشاريو != 0xFFFF)
	انگ_lookups = hb_max (انگ_lookups, 1u + معلومات.current_اشاريو);
    }

    return_trace (substitutionTables.sanitize (c, هذا, انگ_lookups));
  }

  عام:
  State_جدول<Types, داخلا_معلومات>
		machine;
  محمي:
  NNOffset_منزل<Unsized_List_جو_Offset16_منزل<Lookup<HBGlyphID16>, HBUINT, فراغ, خطأ>, HBUINT>
		substitutionTables;
  عام:
  DEFINE_سائيز_STATIC ((State_جدول<Types, داخلا_معلومات>::static_سائيز + HBUINT::static_سائيز));
};


سانچو <منطقي extended>
بنية Ligature_داخلا;

سانچو <>
بنية Ligature_داخلا<سچ>
{

  بنية داخلا_معلومات
  {
    HBUINT16	lig_Action_اشاريو;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    عام:
    DEFINE_سائيز_STATIC (2);
  };

  تعداد Flags
  {
    سيٽ_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  ساكن منطقي initiateAction (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.flags & سيٽ_Component; }

  ساكن منطقي performAction (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.flags & PerformAction; }

  ساكن بنانشان صحيح lig_Action_اشاريو (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.معلومات.lig_Action_اشاريو; }
};
سانچو <>
بنية Ligature_داخلا<خطأ>
{
  تعريفنوع فراغ داخلا_معلومات;

  تعداد Flags
  {
    سيٽ_Component	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    وٿي_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  ساكن منطقي initiateAction (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.flags & سيٽ_Component; }

  ساكن منطقي performAction (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.flags & وٿي_2; }

  ساكن بنانشان صحيح lig_Action_اشاريو (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.flags & وٿي_2; }
};


سانچو <قسم_نالو Types>
بنية LigatureSubtable
{
  تعريفنوع قسم_نالو Types::HBUINT HBUINT;

  تعريفنوع Ligature_داخلا<Types::extended> Ligature_داخلا_T;
  تعريفنوع قسم_نالو Ligature_داخلا_T::داخلا_معلومات داخلا_معلومات;

  تعداد Flags
  {
    DontAdvance	= Ligature_داخلا_T::DontAdvance,
  };

  منطقي is_action_initiable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    أرجع Ligature_داخلا_T::initiateAction (داخلا);
  }
  منطقي is_actionable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    أرجع Ligature_داخلا_T::performAction (داخلا);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = خطأ;
    تعداد LigActionFlags
    {
      Lig_Action_آخري	= 0x80000000,	/* This is the last action in the list. This also
					 * implies storage. */
      LigActionStore	= 0x40000000,	/* Store the ligature at the current cumulated index
					 * in the ligature table in place of the marked
					 * (i.e. currently-popped) glyph. */
      Lig_Action_وٿي	= 0x3FFFFFFF,	/* A 30-bit value which is sign-extended to 32-bits
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
	ميلاپ_ڊيگهائي (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, داخلا_معلومات, Flags> *driver,
		     ثابت داخلا_4<داخلا_معلومات> &داخلا)
    {
      DEBUG_نياپو (APPLY, خالي_اشارو, "Ligature transition at %u", بفر->idx);
      إذا (داخلا.flags & Ligature_داخلا_T::سيٽ_Component)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	إذا (unlikely (ميلاپ_ڊيگهائي && ميلاپ_positions[(ميلاپ_ڊيگهائي - 1u) % ARRAY_ڊيگهائي (ميلاپ_positions)] == بفر->ٻاهريون_len))
	  ميلاپ_ڊيگهائي--;

	ميلاپ_positions[ميلاپ_ڊيگهائي++ % ARRAY_ڊيگهائي (ميلاپ_positions)] = بفر->ٻاهريون_len;
	DEBUG_نياپو (APPLY, خالي_اشارو, "Set component at %u", بفر->ٻاهريون_len);
      }

      إذا (Ligature_داخلا_T::performAction (داخلا))
      {
	DEBUG_نياپو (APPLY, خالي_اشارو, "Perform action with %u", ميلاپ_ڊيگهائي);
	بنانشان صحيح پڇاڙي = بفر->ٻاهريون_len;

	إذا (unlikely (!ميلاپ_ڊيگهائي))
	  أرجع;

	إذا (بفر->idx >= بفر->len)
	  أرجع; /* TODO Work on previous instead? */

	بنانشان صحيح cursor = ميلاپ_ڊيگهائي;

	بنانشان صحيح action_idx = Ligature_داخلا_T::lig_Action_اشاريو (داخلا);
	action_idx = Types::وٿي_منزل_اشاريو (action_idx, جدول, ligAction.arrayZ);
	ثابت HBUINT32 *action_معلومات = &ligAction[action_idx];

	بنانشان صحيح ligature_idx = 0;
	بنانشان صحيح action;
	افعل
	{
	  إذا (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_نياپو (APPLY, خالي_اشارو, "Stack underflow");
	    ميلاپ_ڊيگهائي = 0;
	    اكسر;
	  }

	  DEBUG_نياپو (APPLY, خالي_اشارو, "Moving to stack position %u", cursor - 1);
	  إذا (unlikely (!بفر->move_منزل (ميلاپ_positions[--cursor % ARRAY_ڊيگهائي (ميلاپ_positions)]))) أرجع;

	  إذا (unlikely (!action_معلومات->sanitize (&c->sanitizer))) اكسر;
	  hb_barrier ();
	  action = *action_معلومات;

	  uint32_t uoffset = action & Lig_Action_وٿي;
	  إذا (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t وٿي = (int32_t) uoffset;
	  بنانشان صحيح component_idx = بفر->cur().codepoint + وٿي;
	  component_idx = Types::لفظ_وٿي_منزل_اشاريو (component_idx, جدول, component.arrayZ);
	  ثابت HBUINT16 &component_معلومات = component[component_idx];
	  إذا (unlikely (!component_معلومات.sanitize (&c->sanitizer))) اكسر;
	  hb_barrier ();
	  ligature_idx += component_معلومات;

	  DEBUG_نياپو (APPLY, خالي_اشارو, "Action store %d last %d",
		     منطقي (action & LigActionStore),
		     منطقي (action & Lig_Action_آخري));
	  إذا (action & (LigActionStore | Lig_Action_آخري))
	  {
	    ligature_idx = Types::وٿي_منزل_اشاريو (ligature_idx, جدول, ligature.arrayZ);
	    ثابت HBGlyphID16 &ligature_معلومات = ligature[ligature_idx];
	    إذا (unlikely (!ligature_معلومات.sanitize (&c->sanitizer))) اكسر;
	    hb_barrier ();
	    hb_codepoint_t lig = ligature_معلومات;

	    DEBUG_نياپو (APPLY, خالي_اشارو, "Produced ligature %u", lig);
	    إذا (unlikely (!c->replace_glyph (lig))) أرجع;

	    بنانشان صحيح lig_پڇاڙي = ميلاپ_positions[(ميلاپ_ڊيگهائي - 1u) % ARRAY_ڊيگهائي (ميلاپ_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    طالما (ميلاپ_ڊيگهائي - 1u > cursor)
	    {
	      DEBUG_نياپو (APPLY, خالي_اشارو, "Skipping ligature component");
	      إذا (unlikely (!بفر->move_منزل (ميلاپ_positions[--ميلاپ_ڊيگهائي % ARRAY_ڊيگهائي (ميلاپ_positions)]))) أرجع;
	      إذا (!c->delete_glyph ()) أرجع;
	    }

	    إذا (unlikely (!بفر->move_منزل (lig_پڇاڙي))) أرجع;
	    بفر->ملايو_ٻاهريون_clusters (ميلاپ_positions[cursor % ARRAY_ڊيگهائي (ميلاپ_positions)], بفر->ٻاهريون_len);
	  }

	  action_معلومات++;
	}
	طالما (!(action & Lig_Action_آخري));
	إذا (unlikely (!بفر->move_منزل (پڇاڙي))) أرجع;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت LigatureSubtable *جدول;
    خاص:
    ثابت Unsized_Array_جو<HBUINT32> &ligAction;
    ثابت Unsized_Array_جو<HBUINT16> &component;
    ثابت Unsized_Array_جو<HBGlyphID16> &ligature;
    بنانشان صحيح ميلاپ_ڊيگهائي;
    بنانشان صحيح ميلاپ_positions[HB_MAX_CONTEXT_ڊيگهائي];
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, داخلا_معلومات, Flags> driver (machine, c->face);

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
  State_جدول<Types, داخلا_معلومات>
		machine;
  محمي:
  NNOffset_منزل<Unsized_Array_جو<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffset_منزل<Unsized_Array_جو<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffset_منزل<Unsized_Array_جو<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  عام:
  DEFINE_سائيز_STATIC ((State_جدول<Types, داخلا_معلومات>::static_سائيز + 3 * HBUINT::static_سائيز));
};

سانچو <قسم_نالو Types>
بنية NoncontextualSubtable
{
  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    منطقي ret = خطأ;
    بنانشان صحيح انگ_glyphs = c->face->get_انگ_glyphs ();

    hb_glyph_info_t *info = c->بفر->info;
    بنانشان صحيح ڳڻپ = c->بفر->len;
    // If there's only one range, we already checked the flag.
    خودڪار *آخري_range = c->range_flags && (c->range_flags->ڊيگهائي > 1) ? &(*c->range_flags)[0] : خالي_اشارو;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      إذا (unlikely (آخري_range))
      {
	خودڪار *range = آخري_range;
	{
	  بنانشان cluster = info[i].cluster;
	  طالما (cluster < range->cluster_پهريون)
	    range--;
	  طالما (cluster > range->cluster_آخري)
	    range++;

	  آخري_range = range;
	}
	إذا (!(range->flags & c->subtable_flags))
	  استمر;
      }

      ثابت HBGlyphID16 *replacement = substitute.get_قدر (info[i].codepoint, انگ_glyphs);
      إذا (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = سچ;
      }
    }

    return_trace (ret);
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_initial_glyphs (سيٽ_t &glyphs, بنانشان انگ_glyphs) ثابت
  {
    substitute.collect_glyphs (glyphs, انگ_glyphs);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (substitute.sanitize (c));
  }

  محمي:
  Lookup<HBGlyphID16>	substitute;
  عام:
  DEFINE_سائيز_MIN (2);
};

سانچو <قسم_نالو Types>
بنية InsertionSubtable
{
  تعريفنوع قسم_نالو Types::HBUINT HBUINT;

  بنية داخلا_معلومات
  {
    HBUINT16	current_وجهو_اشاريو;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_وجهو_اشاريو;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    عام:
    DEFINE_سائيز_STATIC (4);
  };

  تعداد Flags
  {
    سيٽ_Mark		= 0x8000,     /* If set, mark the current glyph. */
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
    Current_وجهو_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_وجهو_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_وجهو_ڳڻپ= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_وجهو_ڳڻپ= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  منطقي is_action_initiable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    أرجع (داخلا.flags & سيٽ_Mark);
  }
  منطقي is_actionable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    أرجع (داخلا.flags & (Current_وجهو_ڳڻپ | Marked_وجهو_ڳڻپ)) &&
	   (داخلا.معلومات.current_وجهو_اشاريو != 0xFFFF ||داخلا.معلومات.marked_وجهو_اشاريو != 0xFFFF);
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
		     State_جدول_Driver<Types, داخلا_معلومات, Flags> *driver,
		     ثابت داخلا_4<داخلا_معلومات> &داخلا)
    {
      بنانشان صحيح flags = داخلا.flags;

      بنانشان mark_loc = بفر->ٻاهريون_len;

      إذا (داخلا.معلومات.marked_وجهو_اشاريو != 0xFFFF)
      {
	بنانشان صحيح ڳڻپ = (flags & Marked_وجهو_ڳڻپ);
	إذا (unlikely ((بفر->max_ops -= ڳڻپ) <= 0)) أرجع;
	بنانشان صحيح شروعات = داخلا.معلومات.marked_وجهو_اشاريو;
	ثابت HBGlyphID16 *glyphs = &insertionAction[شروعات];
	إذا (unlikely (!c->sanitizer.check_array (glyphs, ڳڻپ))) ڳڻپ = 0;
	hb_barrier ();

	منطقي before = flags & Marked_وجهو_Before;

	بنانشان صحيح پڇاڙي = بفر->ٻاهريون_len;
	إذا (unlikely (!بفر->move_منزل (mark))) أرجع;

	إذا (بفر->idx < بفر->len && !before)
	  إذا (unlikely (!بفر->copy_glyph ())) أرجع;
	/* TODO We ignore KashidaLike setting. */
	إذا (unlikely (!c->output_glyphs (ڳڻپ, glyphs))) أرجع;
	ret = سچ;
	إذا (بفر->idx < بفر->len && !before)
	  بفر->skip_glyph ();

	إذا (unlikely (!بفر->move_منزل (پڇاڙي + ڳڻپ))) أرجع;

	بفر->unsafe_منزل_break_from_outbuffer (mark, hb_min (بفر->idx + 1, بفر->len));
      }

      إذا (flags & سيٽ_Mark)
	mark = mark_loc;

      إذا (داخلا.معلومات.current_وجهو_اشاريو != 0xFFFF)
      {
	بنانشان صحيح ڳڻپ = (flags & Current_وجهو_ڳڻپ) >> 5;
	إذا (unlikely ((بفر->max_ops -= ڳڻپ) <= 0)) أرجع;
	بنانشان صحيح شروعات = داخلا.معلومات.current_وجهو_اشاريو;
	ثابت HBGlyphID16 *glyphs = &insertionAction[شروعات];
	إذا (unlikely (!c->sanitizer.check_array (glyphs, ڳڻپ))) ڳڻپ = 0;
	hb_barrier ();

	منطقي before = flags & Current_وجهو_Before;

	بنانشان صحيح پڇاڙي = بفر->ٻاهريون_len;

	إذا (بفر->idx < بفر->len && !before)
	  إذا (unlikely (!بفر->copy_glyph ())) أرجع;
	/* TODO We ignore KashidaLike setting. */
	إذا (unlikely (!c->output_glyphs (ڳڻپ, glyphs))) أرجع;
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
	إذا (unlikely (!بفر->move_منزل ((flags & DontAdvance) ? پڇاڙي : پڇاڙي + ڳڻپ))) أرجع;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت InsertionSubtable *جدول;
    خاص:
    بنانشان صحيح mark;
    ثابت Unsized_Array_جو<HBGlyphID16> &insertionAction;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, داخلا_معلومات, Flags> driver (machine, c->face);

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
  State_جدول<Types, داخلا_معلومات>
		machine;
  محمي:
  NNOffset_منزل<Unsized_Array_جو<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  عام:
  DEFINE_سائيز_STATIC ((State_جدول<Types, داخلا_معلومات>::static_سائيز + HBUINT::static_سائيز));
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
  DEFINE_سائيز_STATIC (12);
};


بنية hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  بنية hb_applicable_t
  {
    دوست بنية hb_accelerate_subtables_context_t;
    دوست بنية hb_aat_layout_lookup_accelerator_t;

    عام:
    hb_ٻٽو_انگ_سيٽ_t glyph_سيٽ;
    بدلجندڙ hb_aat_class_cache_t class_cache;

    سانچو <قسم_نالو T>
    خودڪار init_ (ثابت T &obj_, بنانشان انگ_glyphs, hb_ترجيح<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_سيٽ, انگ_glyphs, obj_)
    )

    سانچو <قسم_نالو T>
    فراغ init_ (ثابت T &obj_, بنانشان انگ_glyphs, hb_ترجيح<0>)
    {
      obj_.collect_initial_glyphs (glyph_سيٽ, انگ_glyphs);
    }

    سانچو <قسم_نالو T>
    فراغ init (ثابت T &obj_, بنانشان انگ_glyphs)
    {
      glyph_سيٽ.init ();
      init_ (obj_, انگ_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    فراغ
    fini ()
    {
      glyph_سيٽ.fini ();
    }
  };

  /* Dispatch interface. */
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj)
  {
    hb_applicable_t *داخلا = &array[i++];

    داخلا->init (obj, انگ_glyphs);

    أرجع hb_خالي_جايون_t ();
  }
  ساكن return_t default_return_قدر () { أرجع hb_خالي_جايون_t (); }

  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع خطأ; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, بنانشان انگ_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), انگ_glyphs (انگ_glyphs_2) {}

  hb_applicable_t *array;
  بنانشان انگ_glyphs;
  بنانشان i = 0;
};

بنية hb_aat_layout_chain_accelerator_t
{
  سانچو <قسم_نالو TChain>
  ساكن hb_aat_layout_chain_accelerator_t *create (ثابت TChain &chain, بنانشان انگ_glyphs)
  {
    بنانشان ڳڻپ = chain.get_subtable_ڳڻپ ();

    بنانشان حاصل_ضرب;
    إذا (unlikely (hb_unsigned_mul_overflows (ڳڻپ,
        حجم (hb_accelerate_subtables_context_t::hb_applicable_t), &حاصل_ضرب)))
      أرجع خالي_اشارو;

    بنانشان سائيز;
    إذا (unlikely (hb_unsigned_جوڙيو_overflows (حجم (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t), حاصل_ضرب, &سائيز))) {
      أرجع خالي_اشارو;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    خودڪار *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, سائيز);
    إذا (unlikely (!thiz))
      أرجع خالي_اشارو;

    thiz->ڳڻپ = ڳڻپ;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, انگ_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    أرجع thiz;
  }

  فراغ destroy ()
  {
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      subtables[i].fini ();
  }

  بنانشان ڳڻپ;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

سانچو <قسم_نالو Types>
بنية ChainSubtable
{
  تعريفنوع قسم_نالو Types::HBUINT HBUINT;

  سانچو <قسم_نالو T>
  دوست بنية Chain;

  size_t get_سائيز () ثابت     { أرجع ڊيگهائي; }
  بنانشان صحيح get_type () ثابت     { أرجع coverage & 0xFF; }
  بنانشان صحيح get_coverage () ثابت { أرجع coverage >> (حجم (HBUINT) * 8 - 8); }

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

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بنانشان صحيح subtable_type = get_type ();
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
    إذا (!(ڊيگهائي.sanitize (c) &&
	  hb_barrier () &&
	  ڊيگهائي >= min_سائيز &&
	  c->check_range (هذا, ڊيگهائي)))
      return_trace (خطأ);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  محمي:
  HBUINT	ڊيگهائي;		/* Total subtable length, including this header. */
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
  DEFINE_سائيز_MIN (2 * حجم (HBUINT) + 4);
};

سانچو <قسم_نالو Types>
بنية Chain
{
  تعريفنوع قسم_نالو Types::HBUINT HBUINT;

  بنانشان get_subtable_ڳڻپ () ثابت { أرجع subtable_ڳڻپ_2; }

  hb_نقاب_t compile_flags (ثابت hb_aat_map_builder_t *map) ثابت
  {
    hb_نقاب_t flags = defaultFlags;
    {
      بنانشان صحيح ڳڻپ = feature_ڳڻپ;
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
      {
	ثابت Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (بنانشان صحيح) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (بنانشان صحيح) feature.featureSetting;
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
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_هيٺاهون_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_هيٺاهون_CASE_SMALL_CAPS;
	  اذهب retry;
	}
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT
	وإلا إذا (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_ٽيگ_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->جدول.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#جيڪڏهن_پورو
      }
    }
    أرجع flags;
  }

  فراغ apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_layout_chain_accelerator_t *accel) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ڳڻپ));
    بنانشان صحيح ڳڻپ = subtable_ڳڻپ_2;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      منطقي reverse;

      خودڪار coverage = subtable->get_coverage ();

      hb_نقاب_t subtable_flags = subtable->subFeatureFlags;
      إذا (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ثابت hb_aat_map_t::range_flags_t _) -> منطقي { أرجع subtable_flags & (_.flags); })))
	اذهب skip;

      إذا (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->بفر->props.direction) !=
	  منطقي (coverage & ChainSubtable<Types>::Vertical))
	اذهب skip;

      c->subtable_flags = subtable_flags;
      c->پهريون_سيٽ = accel ? &accel->subtables[i].glyph_سيٽ : &Null(hb_ٻٽو_انگ_سيٽ_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : خالي_اشارو;

      إذا (!c->بفر_intersects_machine ())
      {
	(فراغ) c->بفر->پيغام (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_اشاريو);
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

      إذا (!c->بفر->پيغام (c->font, "start chainsubtable %u", c->lookup_اشاريو))
	اذهب skip;

      إذا (reverse != c->بفر_is_reversed)
        c->ابتو_بفر ();

      subtable->apply (c);

      (فراغ) c->بفر->پيغام (c->font, "end chainsubtable %u", c->lookup_اشاريو);

      إذا (unlikely (!c->بفر->successful)) اكسر;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->سيٽ_lookup_اشاريو (c->lookup_اشاريو + 1);
    }
    إذا (c->بفر_is_reversed)
      c->ابتو_بفر ();
  }

  size_t get_سائيز () ثابت { أرجع ڊيگهائي; }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ڳڻپ));
    بنانشان صحيح ڳڻپ = subtable_ڳڻپ_2;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      قسم_نالو context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      إذا (c->stop_sublookup_iteration (ret))
	أرجع ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    أرجع c->default_return_قدر ();
  }

  منطقي sanitize (hb_sanitize_context_t *c, بنانشان صحيح نسخو) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(ڊيگهائي.sanitize (c) &&
	  hb_barrier () &&
	  ڊيگهائي >= min_سائيز &&
	  c->check_range (هذا, ڊيگهائي)))
      return_trace (خطأ);

    إذا (!c->check_array (featureZ.arrayZ, feature_ڳڻپ))
      return_trace (خطأ);

    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ڳڻپ));
    بنانشان صحيح ڳڻپ = subtable_ڳڻپ_2;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (!subtable->sanitize (c))
	return_trace (خطأ);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    إذا (نسخو >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) subtable;
      إذا (!coverage->sanitize (c, ڳڻپ))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }

  محمي:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	ڊيگهائي;		/* Total byte count, including this header. */
  HBUINT	feature_ڳڻپ;	/* Number of feature subtable entries. */
  HBUINT	subtable_ڳڻپ_2;	/* The number of subtables in the chain. */

  Unsized_Array_جو<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  عام:
  DEFINE_سائيز_MIN (8 + 2 * حجم (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

سانچو <قسم_نالو T, قسم_نالو Types, hb_ٽيگ_t ٽيگ_3>
بنية mortmorx
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = ٽيگ_3;

  منطقي has_معلومات () ثابت { أرجع نسخو != 0; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->جدول = sc.reference_جدول<T> (face);

      إذا (unlikely (هذا->جدول->is_blocklisted (هذا->جدول.get_blob (), face)))
      {
        hb_blob_destroy (هذا->جدول.get_blob ());
        هذا->جدول = hb_blob_get_خالي_جايون ();
      }

      هذا->chain_ڳڻپ = جدول->get_chain_ڳڻپ ();

      هذا->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (هذا->chain_ڳڻپ, حجم (*accels));
      إذا (unlikely (!هذا->accels))
      {
	هذا->chain_ڳڻپ = 0;
	هذا->جدول.destroy ();
	هذا->جدول = hb_blob_get_خالي_جايون ();
      }
    }
    ~accelerator_t ()
    {
      لكل (بنانشان صحيح i = 0; i < هذا->chain_ڳڻپ; i++)
      {
	إذا (هذا->accels[i])
	  هذا->accels[i]->destroy ();
	hb_free (هذا->accels[i]);
      }
      hb_free (هذا->accels);
      هذا->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع جدول.get_blob (); }

    سانچو <قسم_نالو Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (بنانشان chain_اشاريو, ثابت Chain &chain, بنانشان انگ_glyphs) ثابت
    {
      إذا (unlikely (chain_اشاريو >= chain_ڳڻپ)) أرجع خالي_اشارو;

    retry:
      خودڪار *accel = accels[chain_اشاريو].get_acquire ();
      إذا (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, انگ_glyphs);
	إذا (unlikely (!accel))
	  أرجع خالي_اشارو;

	إذا (unlikely (!accels[chain_اشاريو].cmpexch (خالي_اشارو, accel)))
	{
	  hb_free (accel);
	  اذهب retry;
	}
      }

      أرجع accel;
    }

    hb_blob_ptr_t<T> جدول;
    بنانشان صحيح chain_ڳڻپ;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  فراغ compile_flags (ثابت hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ثابت
  {
    ثابت Chain<Types> *chain = &پهريون_Chain;
    بنانشان صحيح ڳڻپ = chain_ڳڻپ_2;
    إذا (unlikely (!map->chain_flags.resize (ڳڻپ)))
      أرجع;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_پهريون,
							     mapper->range_آخري});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  بنانشان get_chain_ڳڻپ () ثابت
  {
    أرجع chain_ڳڻپ_2;
  }
  فراغ apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_map_t &map,
	      ثابت accelerator_t &accel) ثابت
  {
    إذا (unlikely (!c->بفر->successful)) أرجع;

    c->بفر->unsafe_منزل_concat ();

    c->تياري_بفر_glyph_سيٽ ();

    c->سيٽ_lookup_اشاريو (0);
    ثابت Chain<Types> *chain = &پهريون_Chain;
    بنانشان صحيح ڳڻپ = chain_ڳڻپ_2;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      خودڪار *chain_accel = accel.get_accel (i, *chain, c->face->get_انگ_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      إذا (unlikely (!c->بفر->successful)) أرجع;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(نسخو.sanitize (c) &&
	  hb_barrier () &&
	  نسخو &&
	  chain_ڳڻپ_2.sanitize (c)))
      return_trace (خطأ);

    ثابت Chain<Types> *chain = &پهريون_Chain;
    بنانشان صحيح ڳڻپ = chain_ڳڻپ_2;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (!chain->sanitize (c, نسخو))
	return_trace (خطأ);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (سچ);
  }

  محمي:
  HBUINT16	نسخو;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	اڻ_استعمال_ٿيل_2;		/* Set to 0. */
  HBUINT32	chain_ڳڻپ_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	پهريون_Chain;	/* Chains. */

  عام:
  DEFINE_سائيز_MIN (8);
};

بنية morx : mortmorx<morx, ExtendedTypes, HB_AAT_ٽيگ_morx>
{
  HB_اندروني منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

بنية mort : mortmorx<mort, ObsoleteTypes, HB_AAT_ٽيگ_mort>
{
  HB_اندروني منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

بنية morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
بنية mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#جيڪڏهن_پورو /* HB_AAT_LAYOUT_MORX_TABLE_HH */
