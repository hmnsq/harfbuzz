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

#که_نه_تعریف_شوی HB_AAT_LAYOUT_KERX_جدول_HH
#تعریف_کړه HB_AAT_LAYOUT_KERX_جدول_HH

#ضمن "hb-kern.hh"
#ضمن "hb-aat-layout-ankr-table.hh"
#ضمن "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#تعریف_کړه HB_AAT_نښان_kerx HB_نښان('k','e','r','x')


نطاق AAT {

کارول نطاق OT;


ساكن مضمن صحيح
kerxTupleKern (صحيح value,
	       بینښه صحيح tuple_شمېر,
	       ثابت فراغ *بنسټ,
	       hb_aat_apply_context_t *c)
{
  إذا (likely (!tuple_شمېر || !c)) أرجع value;

  بینښه صحيح دننه_واټن = value;
  ثابت FWORD *pv = &Struct_At_دننه_واټن<FWORD> (بنسټ, دننه_واټن);
  إذا (unlikely (!c->sanitizer.check_array (pv, tuple_شمېر))) أرجع 0;
  hb_barrier ();
  أرجع *pv;
}


بنية hb_glyph_pair_t
{
  hb_codepoint_t کیڼ;
  hb_codepoint_t ښی;
};

بنية KernPair
{
  صحيح get_kerning () ثابت { أرجع value; }

  صحيح cmp (ثابت hb_glyph_pair_t &o) ثابت
  {
    صحيح ret = کیڼ.cmp (o.کیڼ);
    إذا (ret) أرجع ret;
    أرجع ښی.cmp (o.ښی);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBGlyphID16	کیڼ;
  HBGlyphID16	ښی;
  FWORD		value;
  عام:
  DEFINE_اندازه_STATIC (6);
};

کينډۍ <ډول_نوم Kern_Sub_جدول_سرلیک>
بنية Kerx_Sub_جدول_Format0
{
  صحيح get_kerning (hb_codepoint_t کیڼ, hb_codepoint_t ښی,
		   hb_aat_apply_context_t *c = تش_اشاره) ثابت
  {
    hb_glyph_pair_t pair = {کیڼ, ښی};
    صحيح v = جوړې.bsearch (pair).get_kerning ();
    أرجع kerxTupleKern (v, سرلیک.tuple_شمېر_2 (), هذا, c);
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرلیک.coverage & سرلیک.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرلیک.coverage & سرلیک.Cross_بهیر);
    machine.kern (c->font, c->بفر, c->plan->kern_ماسک);

    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &لومړی_سېټ, سېټ_t &دوهم_سېټ, بینښه عدد_glyphs) ثابت
  {
    لكل (ثابت KernPair& pair : جوړې)
    {
      لومړی_سېټ.جمع_کړه (pair.کیڼ);
      دوهم_سېټ.جمع_کړه (pair.ښی);
    }
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format0 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format0 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t کیڼ, hb_codepoint_t ښی) ثابت
    {
      إذا (!(*c->لومړی_سېټ)[کیڼ] || !(*c->دوهم_سېټ)[ښی]) أرجع 0;
      أرجع جدول.get_kerning (کیڼ, ښی, c);
    }
  };


  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (جوړې.sanitize (c)));
  }

  محمي:
  Kern_Sub_جدول_سرلیک	سرلیک;
  Bin_پلټنه_Array_د<KernPair, ډول_نوم Kern_Sub_جدول_سرلیک::Types::HBUINT>
			جوړې;	/* Sorted kern records. */
  عام:
  DEFINE_اندازه_ARRAY (Kern_Sub_جدول_سرلیک::static_اندازه + 16, جوړې);
};


کينډۍ <منطقي extended>
بنية Format1_ثبت;

کينډۍ <>
بنية Format1_ثبت<ریښتیا>
{
  تعداد Flags
  {
    ټېل		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  بنية ثبت_معلومات
  {
    HBUINT16	kern_Action_شاخص;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    عام:
    DEFINE_اندازه_STATIC (2);
  };

  ساكن منطقي initiateAction (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.flags & ټېل; }

  ساكن منطقي performAction (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.معلومات.kern_Action_شاخص != 0xFFFF; }

  ساكن بینښه صحيح kern_Action_شاخص (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.معلومات.kern_Action_شاخص; }
};
کينډۍ <>
بنية Format1_ثبت<خطأ>
{
  تعداد Flags
  {
    ټېل		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    دننه_واټن_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  تعريفنوع فراغ ثبت_معلومات;

  ساكن منطقي initiateAction (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.flags & ټېل; }

  ساكن منطقي performAction (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.flags & دننه_واټن_2; }

  ساكن بینښه صحيح kern_Action_شاخص (ثابت ثبت_2<ثبت_معلومات> &ثبت)
  { أرجع ثبت.flags & دننه_واټن_2; }
};

کينډۍ <ډول_نوم Kern_Sub_جدول_سرلیک>
بنية Kerx_Sub_جدول_Format1
{
  تعريفنوع ډول_نوم Kern_Sub_جدول_سرلیک::Types Types;
  تعريفنوع ډول_نوم Types::HBUINT HBUINT;

  تعريفنوع Format1_ثبت<Types::extended> Format1_ثبت_T;
  تعريفنوع ډول_نوم Format1_ثبت_T::ثبت_معلومات ثبت_معلومات;

  تعداد Flags
  {
    DontAdvance	= Format1_ثبت_T::DontAdvance,
  };

  منطقي is_action_initiable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    أرجع Format1_ثبت_T::initiateAction (ثبت);
  }
  منطقي is_actionable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    أرجع Format1_ثبت_T::performAction (ثبت);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = ریښتیا;

    driver_context_t (ثابت Kerx_Sub_جدول_Format1 *جدول_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	جدول (جدول_2),
	/* Apparently the offset kernAction is from the beginning of the state-machine,
	 * similar to offsets in morx table, NOT from beginning of this table, like
	 * other subtables in kerx.  Discovered via testing. */
	kernAction (&جدول->machine + جدول->kernAction),
	ژوروالی (0),
	cross_بهیر (جدول->سرلیک.coverage & جدول->سرلیک.Cross_بهیر) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, ثبت_معلومات, Flags> *driver,
		     ثابت ثبت_2<ثبت_معلومات> &ثبت)
    {
      بینښه صحيح flags = ثبت.flags;

      إذا (flags & Format1_ثبت_T::Reset)
	ژوروالی = 0;

      إذا (flags & Format1_ثبت_T::ټېل)
      {
	إذا (likely (ژوروالی < ARRAY_اوږدتیا (ډېری)))
	  ډېری[ژوروالی++] = بفر->idx;
	وإلا
	  ژوروالی = 0; /* Probably not what CoreText does, but better? */
      }

      إذا (Format1_ثبت_T::performAction (ثبت) && ژوروالی)
      {
	بینښه صحيح tuple_شمېر_2 = hb_max (1u, جدول->سرلیک.tuple_شمېر_2 ());

	بینښه صحيح kern_idx = Format1_ثبت_T::kern_Action_شاخص (ثبت);
	kern_idx = Types::byte_دننه_واټن_منزل_شاخص (kern_idx, &جدول->machine, kernAction.arrayZ);
	ثابت FWORD *actions = &kernAction[kern_idx];
	إذا (!c->sanitizer.check_array (actions, ژوروالی, tuple_شمېر_2))
	{
	  ژوروالی = 0;
	  أرجع;
	}
	hb_barrier ();

	hb_ماسک_t kern_ماسک = c->plan->kern_ماسک;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	منطقي وروستی = خطأ;
	طالما (!وروستی && ژوروالی)
	{
	  بینښه صحيح idx = ډېری[--ژوروالی];
	  صحيح v = *actions;
	  actions += tuple_شمېر_2;
	  إذا (idx >= بفر->len) استمر;

	  /* "The end of the list is marked by an odd value..." */
	  وروستی = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = بفر->pos[idx];

	  إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
	  {
	    إذا (cross_بهیر)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      إذا (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_دننه_واټن_2 = 0;
	      }
	      وإلا إذا (o.attach_type())
	      {
		o.y_دننه_واټن_2 = hb_saturate_جمع_کړه (o.y_دننه_واټن_2, c->font->em_scale_y (v));
		بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وإلا إذا (بفر->info[idx].ماسک & kern_ماسک)
	    {
	      خپلکاری scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_جمع_کړه (o.x_advance, scaled);
	      o.x_دننه_واټن_2 = hb_saturate_جمع_کړه (o.x_دننه_واټن_2, scaled);
	    }
	  }
	  وإلا
	  {
	    إذا (cross_بهیر)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      إذا (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_دننه_واټن_2 = 0;
	      }
	      وإلا إذا (o.attach_type())
	      {
		o.x_دننه_واټن_2 = hb_saturate_جمع_کړه (o.x_دننه_واټن_2, c->font->em_scale_x (v));
		بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وإلا إذا (بفر->info[idx].ماسک & kern_ماسک)
	    {
	      خپلکاری scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_جمع_کړه (o.y_advance, scaled);
	      o.y_دننه_واټن_2 = hb_saturate_جمع_کړه (o.y_دننه_واټن_2, scaled);
	    }
	  }
	}
      }
    }

    عام:
    hb_aat_apply_context_t *c;
    ثابت Kerx_Sub_جدول_Format1 *جدول;
    خاص:
    ثابت Unsized_Array_د<FWORD> &kernAction;
    بینښه صحيح ډېری[8];
    بینښه صحيح ژوروالی;
    منطقي cross_بهیر;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning &&
	!(سرلیک.coverage & سرلیک.Cross_بهیر))
      أرجع خطأ;

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, ثبت_معلومات, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (هذا) &&
			  machine.sanitize (c)));
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &لومړی_سېټ, سېټ_t &دوهم_سېټ, بینښه عدد_glyphs) ثابت
  {
    machine.collect_initial_glyphs (لومړی_سېټ, عدد_glyphs, *هذا);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محمي:
  Kern_Sub_جدول_سرلیک				سرلیک;
  State_جدول<Types, ثبت_معلومات>			machine;
  NNOffset_منزل<Unsized_Array_د<FWORD>, HBUINT>	kernAction;
  عام:
  DEFINE_اندازه_STATIC (Kern_Sub_جدول_سرلیک::static_اندازه + (State_جدول<Types, ثبت_معلومات>::static_اندازه + HBUINT::static_اندازه));
};

کينډۍ <ډول_نوم Kern_Sub_جدول_سرلیک>
بنية Kerx_Sub_جدول_Format2
{
  تعريفنوع ډول_نوم Kern_Sub_جدول_سرلیک::Types Types;
  تعريفنوع ډول_نوم Types::HBUINT HBUINT;

  صحيح get_kerning (hb_codepoint_t کیڼ, hb_codepoint_t ښی,
		   hb_aat_apply_context_t *c) ثابت
  {
    بینښه صحيح عدد_glyphs = c->sanitizer.get_عدد_glyphs ();
    بینښه صحيح l = (هذا+کیڼ_Class_جدول).get_class (کیڼ, عدد_glyphs, 0);
    بینښه صحيح r = (هذا+ښی_Class_جدول).get_class (ښی, عدد_glyphs, 0);

    ثابت Unsized_Array_د<FWORD> &arrayZ = هذا+array;
    بینښه صحيح kern_idx = l + r;
    kern_idx = Types::دننه_واټن_منزل_شاخص (kern_idx, هذا, arrayZ.arrayZ);
    ثابت FWORD *v = &arrayZ[kern_idx];
    إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
    hb_barrier ();

    أرجع kerxTupleKern (*v, سرلیک.tuple_شمېر_2 (), هذا, c);
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرلیک.coverage & سرلیک.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرلیک.coverage & سرلیک.Cross_بهیر);
    machine.kern (c->font, c->بفر, c->plan->kern_ماسک);

    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &لومړی_سېټ, سېټ_t &دوهم_سېټ, بینښه عدد_glyphs) ثابت
  {
    (هذا+کیڼ_Class_جدول).collect_glyphs (لومړی_سېټ, عدد_glyphs);
    (هذا+ښی_Class_جدول).collect_glyphs (دوهم_سېټ, عدد_glyphs);
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format2 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format2 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t کیڼ, hb_codepoint_t ښی) ثابت
    {
      إذا (!(*c->لومړی_سېټ)[کیڼ] || !(*c->دوهم_سېټ)[ښی]) أرجع 0;
      أرجع جدول.get_kerning (کیڼ, ښی, c);
    }
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  کیڼ_Class_جدول.sanitize (c, هذا) &&
			  ښی_Class_جدول.sanitize (c, هذا) &&
			  hb_barrier () &&
			  c->check_range (هذا, array)));
  }

  محمي:
  Kern_Sub_جدول_سرلیک	سرلیک;
  HBUINT		لیکه_پلنوالی;	/* The width, in bytes, of a row in the table. */
  NNOffset_منزل<ډول_نوم Types::ClassTypeWide, HBUINT>
			کیڼ_Class_جدول;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffset_منزل<ډول_نوم Types::ClassTypeWide, HBUINT>
			ښی_Class_جدول;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffset_منزل<Unsized_Array_د<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  عام:
  DEFINE_اندازه_STATIC (Kern_Sub_جدول_سرلیک::static_اندازه + 4 * حجم (HBUINT));
};

کينډۍ <ډول_نوم Kern_Sub_جدول_سرلیک>
بنية Kerx_Sub_جدول_Format4
{
  تعريفنوع ExtendedTypes Types;

  بنية ثبت_معلومات
  {
    HBUINT16	ankr_Action_شاخص;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    عام:
    DEFINE_اندازه_STATIC (2);
  };

  تعداد Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  منطقي is_action_initiable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    أرجع (ثبت.flags & Mark);
  }
  منطقي is_actionable (ثابت ثبت_2<ثبت_معلومات> &ثبت) ثابت
  {
    أرجع ثبت.معلومات.ankr_Action_شاخص != 0xFFFF;
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = ریښتیا;
    تعداد Sub_جدول_Flags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      ناکارول_شوی		= 0x3F000000,	/* Unused - must be zero. */
      دننه_واټن_2		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ثابت Kerx_Sub_جدول_Format4 *جدول_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	جدول (جدول_2),
	action_type ((جدول->flags & ActionType) >> 30),
	ankr_معلومات ((HBUINT16 *) ((ثابت حرف *) &جدول->machine + (جدول->flags & دننه_واټن_2))),
	mark_سېټ (خطأ),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, ثبت_معلومات, Flags> *driver,
		     ثابت ثبت_2<ثبت_معلومات> &ثبت)
    {
      إذا (mark_سېټ && ثبت.معلومات.ankr_Action_شاخص != 0xFFFF && بفر->idx < بفر->len)
      {
	hb_glyph_position_t &o = بفر->cur_pos();
	بدل (action_type)
	{
	  حالة 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *معلومات = &ankr_معلومات[ثبت.معلومات.ankr_Action_شاخص * 2];
	    إذا (!c->sanitizer.check_array (معلومات, 2)) أرجع;
	    hb_barrier ();
	    بینښه صحيح mark_Control_ټکی = *معلومات++;
	    بینښه صحيح curr_Control_ټکی = *معلومات++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    إذا (!c->font->get_glyph_contour_ټکی_for_origin (c->بفر->info[mark].codepoint,
							      mark_Control_ټکی,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_ټکی_for_origin (c->بفر->cur ().codepoint,
							      curr_Control_ټکی,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      أرجع;

	    o.x_دننه_واټن_2 = hb_saturate_sub (markX, currX);
	    o.y_دننه_واټن_2 = hb_saturate_sub (markY, currY);
	  }
	  اكسر;

	  حالة 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *معلومات = &ankr_معلومات[ثبت.معلومات.ankr_Action_شاخص * 2];
	    إذا (!c->sanitizer.check_array (معلومات, 2)) أرجع;
	    hb_barrier ();
	    بینښه صحيح mark_Anchor_ټکی = *معلومات++;
	    بینښه صحيح curr_Anchor_ټکی = *معلومات++;
	    ثابت Anchor &markAnchor = c->ankr_جدول->get_anchor (c->بفر->info[mark].codepoint,
								  mark_Anchor_ټکی,
								  c->sanitizer.get_عدد_glyphs ());
	    ثابت Anchor &currAnchor = c->ankr_جدول->get_anchor (c->بفر->cur ().codepoint,
								  curr_Anchor_ټکی,
								  c->sanitizer.get_عدد_glyphs ());

	    o.x_دننه_واټن_2 = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_دننه_واټن_2 = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  اكسر;

	  حالة 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ثابت FWORD *معلومات = (ثابت FWORD *) &ankr_معلومات[ثبت.معلومات.ankr_Action_شاخص * 4];
	    إذا (!c->sanitizer.check_array (معلومات, 4)) أرجع;
	    hb_barrier ();
	    صحيح markX = *معلومات++;
	    صحيح markY = *معلومات++;
	    صحيح currX = *معلومات++;
	    صحيح currY = *معلومات++;

	    o.x_دننه_واټن_2 = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_دننه_واټن_2 = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  اكسر;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (صحيح) mark - (صحيح) بفر->idx;
	إذا (c->بفر_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      إذا (ثبت.flags & Mark)
      {
	mark_سېټ = ریښتیا;
	mark = بفر->idx;
      }
    }

    عام:
    hb_aat_apply_context_t *c;
    ثابت Kerx_Sub_جدول_Format4 *جدول;
    خاص:
    بینښه صحيح action_type;
    ثابت HBUINT16 *ankr_معلومات;
    منطقي mark_سېټ;
    بینښه صحيح mark;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, ثبت_معلومات, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (هذا) &&
			  machine.sanitize (c)));
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &لومړی_سېټ, سېټ_t &دوهم_سېټ, بینښه عدد_glyphs) ثابت
  {
    machine.collect_initial_glyphs (لومړی_سېټ, عدد_glyphs, *هذا);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محمي:
  Kern_Sub_جدول_سرلیک		سرلیک;
  State_جدول<Types, ثبت_معلومات>	machine;
  HBUINT32			flags;
  عام:
  DEFINE_اندازه_STATIC (Kern_Sub_جدول_سرلیک::static_اندازه + (State_جدول<Types, ثبت_معلومات>::static_اندازه + HBUINT32::static_اندازه));
};

کينډۍ <ډول_نوم Kern_Sub_جدول_سرلیک>
بنية Kerx_Sub_جدول_Format6
{
  تعداد Flags
  {
    ارزښتونه_Are_Long	= 0x00000001,
  };

  منطقي is_long () ثابت { أرجع flags & ارزښتونه_Are_Long; }

  صحيح get_kerning (hb_codepoint_t کیڼ, hb_codepoint_t ښی,
		   hb_aat_apply_context_t *c) ثابت
  {
    بینښه صحيح عدد_glyphs = c->sanitizer.get_عدد_glyphs ();
    إذا (is_long ())
    {
      ثابت خپلکاری &t = u.l;
      بینښه صحيح l = (هذا+t.لیکه_شاخص_جدول).get_ارزښت_or_null (کیڼ, عدد_glyphs);
      بینښه صحيح r = (هذا+t.ستنه_شاخص_جدول).get_ارزښت_or_null (ښی, عدد_glyphs);
      بینښه صحيح دننه_واټن = l + r;
      إذا (unlikely (دننه_واټن < l)) أرجع 0; /* Addition overflow. */
      إذا (unlikely (hb_unsigned_mul_overflows (دننه_واټن, حجم (FWORD32)))) أرجع 0;
      ثابت FWORD32 *v = &Struct_At_دننه_واټن<FWORD32> (&(هذا+t.array), دننه_واټن * حجم (FWORD32));
      إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
      hb_barrier ();
      أرجع kerxTupleKern (*v, سرلیک.tuple_شمېر_2 (), &(هذا+vector), c);
    }
    وإلا
    {
      ثابت خپلکاری &t = u.s;
      بینښه صحيح l = (هذا+t.لیکه_شاخص_جدول).get_ارزښت_or_null (کیڼ, عدد_glyphs);
      بینښه صحيح r = (هذا+t.ستنه_شاخص_جدول).get_ارزښت_or_null (ښی, عدد_glyphs);
      بینښه صحيح دننه_واټن = l + r;
      ثابت FWORD *v = &Struct_At_دننه_واټن<FWORD> (&(هذا+t.array), دننه_واټن * حجم (FWORD));
      إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
      hb_barrier ();
      أرجع kerxTupleKern (*v, سرلیک.tuple_شمېر_2 (), &(هذا+vector), c);
    }
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرلیک.coverage & سرلیک.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرلیک.coverage & سرلیک.Cross_بهیر);
    machine.kern (c->font, c->بفر, c->plan->kern_ماسک);

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.لیکه_شاخص_جدول.sanitize (c, هذا) &&
			     u.l.ستنه_شاخص_جدول.sanitize (c, هذا) &&
			     c->check_range (هذا, u.l.array)
			   ) : (
			     u.s.لیکه_شاخص_جدول.sanitize (c, هذا) &&
			     u.s.ستنه_شاخص_جدول.sanitize (c, هذا) &&
			     c->check_range (هذا, u.s.array)
			   )) &&
			  (سرلیک.tuple_شمېر_2 () == 0 ||
			   c->check_range (هذا, vector))));
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &لومړی_سېټ, سېټ_t &دوهم_سېټ, بینښه عدد_glyphs) ثابت
  {
    إذا (is_long ())
    {
      ثابت خپلکاری &t = u.l;
      (هذا+t.لیکه_شاخص_جدول).collect_glyphs (لومړی_سېټ, عدد_glyphs);
      (هذا+t.ستنه_شاخص_جدول).collect_glyphs (دوهم_سېټ, عدد_glyphs);
    }
    وإلا
    {
      ثابت خپلکاری &t = u.s;
      (هذا+t.لیکه_شاخص_جدول).collect_glyphs (لومړی_سېټ, عدد_glyphs);
      (هذا+t.ستنه_شاخص_جدول).collect_glyphs (دوهم_سېټ, عدد_glyphs);
    }
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format6 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format6 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t کیڼ, hb_codepoint_t ښی) ثابت
    {
      إذا (!(*c->لومړی_سېټ)[کیڼ] || !(*c->دوهم_سېټ)[ښی]) أرجع 0;
      أرجع جدول.get_kerning (کیڼ, ښی, c);
    }
  };

  محمي:
  Kern_Sub_جدول_سرلیک		سرلیک;
  HBUINT32			flags;
  HBUINT16			لیکه_شمېر_2;
  HBUINT16			ستنه_شمېر;
  اتحاد U
  {
    بنية Long
    {
      NNOffset32_منزل<Lookup<HBUINT32>>		لیکه_شاخص_جدول;
      NNOffset32_منزل<Lookup<HBUINT32>>		ستنه_شاخص_جدول;
      NNOffset32_منزل<Unsized_Array_د<FWORD32>>	array;
    } l;
    بنية Short
    {
      NNOffset32_منزل<Lookup<HBUINT16>>		لیکه_شاخص_جدول;
      NNOffset32_منزل<Lookup<HBUINT16>>		ستنه_شاخص_جدول;
      NNOffset32_منزل<Unsized_Array_د<FWORD>>	array;
    } s;
  } u;
  NNOffset32_منزل<Unsized_Array_د<FWORD>>	vector;
  عام:
  DEFINE_اندازه_STATIC (Kern_Sub_جدول_سرلیک::static_اندازه + 24);
};


بنية Kerx_Sub_جدول_سرلیک
{
  تعريفنوع ExtendedTypes Types;

  بینښه   tuple_شمېر_2 () ثابت { أرجع tuple_شمېر; }
  منطقي     is_horizontal () ثابت { أرجع !(coverage & Vertical); }

  تعداد Coverage
  {
    Vertical	= 0x80000000u,	/* Set if table has vertical kerning values. */
    Cross_بهیر	= 0x40000000u,	/* Set if table has cross-stream kerning values. */
    Variation	= 0x20000000u,	/* Set if table has variation kerning values. */
    Backwards	= 0x10000000u,	/* If clear, process the glyphs forwards, that
				 * is, from first to last in the glyph stream.
				 * If we, process them from last to first.
				 * This flag only applies to state-table based
				 * 'kerx' subtables (types 1 and 4). */
    Reserved	= 0x0FFFFF00u,	/* Reserved, set to zero. */
    SubtableType= 0x000000FFu,	/* Subtable type. */
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT32	اوږدتیا;
  HBUINT32	coverage;
  HBUINT32	tuple_شمېر;
  عام:
  DEFINE_اندازه_STATIC (12);
};

بنية Kerx_Sub_جدول
{
  ملګری بنية kerx;

  size_t get_اندازه () ثابت { أرجع u.سرلیک.اوږدتیا; }
  بینښه صحيح get_type () ثابت { أرجع u.سرلیک.coverage & u.سرلیک.SubtableType; }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بینښه صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    افتراضي:	return_trace (c->default_return_ارزښت ());
    }
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &لومړی_سېټ, سېټ_t &دوهم_سېټ, بینښه عدد_glyphs) ثابت
  {
    بینښه صحيح subtable_type = get_type ();
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); u.format0.collect_glyphs (لومړی_سېټ, دوهم_سېټ, عدد_glyphs); أرجع;
    حالة 1:	hb_barrier (); u.format1.collect_glyphs (لومړی_سېټ, دوهم_سېټ, عدد_glyphs); أرجع;
    حالة 2:	hb_barrier (); u.format2.collect_glyphs (لومړی_سېټ, دوهم_سېټ, عدد_glyphs); أرجع;
    حالة 4:	hb_barrier (); u.format4.collect_glyphs (لومړی_سېټ, دوهم_سېټ, عدد_glyphs); أرجع;
    حالة 6:	hb_barrier (); u.format6.collect_glyphs (لومړی_سېټ, دوهم_سېټ, عدد_glyphs); أرجع;
    افتراضي:	أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(u.سرلیک.sanitize (c) &&
	  hb_barrier () &&
	  u.سرلیک.اوږدتیا >= u.سرلیک.static_اندازه &&
	  c->check_range (هذا, u.سرلیک.اوږدتیا)))
      return_trace (خطأ);

    return_trace (dispatch (c));
  }

  عام:
  اتحاد {
  Kerx_Sub_جدول_سرلیک				سرلیک;
  Kerx_Sub_جدول_Format0<Kerx_Sub_جدول_سرلیک>	format0;
  Kerx_Sub_جدول_Format1<Kerx_Sub_جدول_سرلیک>	format1;
  Kerx_Sub_جدول_Format2<Kerx_Sub_جدول_سرلیک>	format2;
  Kerx_Sub_جدول_Format4<Kerx_Sub_جدول_سرلیک>	format4;
  Kerx_Sub_جدول_Format6<Kerx_Sub_جدول_سرلیک>	format6;
  } u;
  عام:
  DEFINE_اندازه_MIN (12);
};


/*
 * The 'kerx' Table
 */

بنية kern_subtable_accelerator_معلومات_t
{
  hb_بټ_سېټ_t لومړی_سېټ;
  hb_بټ_سېټ_t دوهم_سېټ;
  بدلیدونکی hb_aat_class_cache_t class_cache;
};

بنية kern_accelerator_معلومات_t
{
  hb_ویکټور_t<kern_subtable_accelerator_معلومات_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

کينډۍ <ډول_نوم T>
بنية Kerx_جدول
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت T* thiz () ثابت { أرجع static_cast<ثابت T *> (هذا); }

  منطقي has_state_machine () ثابت
  {
    تعريفنوع ډول_نوم T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->لومړی_Sub_جدول;
    بینښه صحيح شمېر = thiz()->جدول_شمېر;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (st->get_type () == 1)
	أرجع ریښتیا;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع خطأ;
  }

  منطقي has_cross_بهیر () ثابت
  {
    تعريفنوع ډول_نوم T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->لومړی_Sub_جدول;
    بینښه صحيح شمېر = thiz()->جدول_شمېر;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (st->u.سرلیک.coverage & st->u.سرلیک.Cross_بهیر)
	أرجع ریښتیا;
      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع خطأ;
  }

  صحيح get_h_kerning (hb_codepoint_t کیڼ, hb_codepoint_t ښی) ثابت
  {
    تعريفنوع ډول_نوم T::Sub_جدول Sub_جدول;

    صحيح v = 0;
    ثابت Sub_جدول *st = &thiz()->لومړی_Sub_جدول;
    بینښه صحيح شمېر = thiz()->جدول_شمېر;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا ((st->u.سرلیک.coverage & (st->u.سرلیک.Variation | st->u.سرلیک.Cross_بهیر)) ||
	  !st->u.سرلیک.is_horizontal ())
	استمر;
      v += st->get_kerning (کیڼ, ښی);
      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع v;
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c,
	      ثابت kern_accelerator_معلومات_t &accel_معلومات) ثابت
  {
    c->بفر->unsafe_منزل_concat ();

    c->تیاری_بفر_glyph_سېټ ();

    تعريفنوع ډول_نوم T::Sub_جدول Sub_جدول;

    منطقي ret = خطأ;
    منطقي کتل_شوی_Cross_بهیر = خطأ;
    c->سېټ_lookup_شاخص (0);
    ثابت Sub_جدول *st = &thiz()->لومړی_Sub_جدول;
    بینښه صحيح شمېر = thiz()->جدول_شمېر;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      منطقي reverse;

      خپلکاری &subtable_accel = accel_معلومات.subtable_accels[i];

      إذا (!T::Types::extended && (st->u.سرلیک.coverage & st->u.سرلیک.Variation))
	اذهب skip;

      إذا (HB_DIRECTION_IS_HORIZONTAL (c->بفر->props.direction) != st->u.سرلیک.is_horizontal ())
	اذهب skip;

      c->لومړی_سېټ = &subtable_accel.لومړی_سېټ;
      c->دوهم_سېټ = &subtable_accel.دوهم_سېټ;
      c->machine_class_cache = &subtable_accel.class_cache;

      إذا (!c->بفر_intersects_machine ())
      {
	(فراغ) c->بفر->پیغام (c->font, "skipped subtable %u because no glyph matches", c->lookup_شاخص);
	اذهب skip;
      }

      reverse = منطقي (st->u.سرلیک.coverage & st->u.سرلیک.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->بفر->props.direction);

      إذا (!c->بفر->پیغام (c->font, "start subtable %u", c->lookup_شاخص))
	اذهب skip;

      إذا (!کتل_شوی_Cross_بهیر &&
	  (st->u.سرلیک.coverage & st->u.سرلیک.Cross_بهیر))
      {
	/* Attach all glyphs into a chain. */
	کتل_شوی_Cross_بهیر = ریښتیا;
	hb_glyph_position_t *pos = c->بفر->pos;
	بینښه صحيح شمېر = c->بفر->len;
	لكل (بینښه صحيح i = 0; i < شمېر; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->بفر->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      إذا (reverse != c->بفر_is_reversed)
        c->سرچپه_بفر ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < شمېر - 1 ? st : (ثابت Sub_جدول *) تش_اشاره);
	ret |= st->dispatch (c);
      }

      (فراغ) c->بفر->پیغام (c->font, "end subtable %u", c->lookup_شاخص);

    skip:
      st = &StructAfter<Sub_جدول> (*st);
      c->سېټ_lookup_شاخص (c->lookup_شاخص + 1);
    }
    إذا (c->بفر_is_reversed)
      c->سرچپه_بفر ();

    أرجع ret;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(thiz()->نسخه.sanitize (c) &&
		    hb_barrier () &&
		    (بینښه) thiz()->نسخه >= (بینښه) T::min_نسخه &&
		    thiz()->جدول_شمېر.sanitize (c))))
      return_trace (خطأ);

    تعريفنوع ډول_نوم T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->لومړی_Sub_جدول;
    بینښه صحيح شمېر = thiz()->جدول_شمېر;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (unlikely (!st->u.سرلیک.sanitize (c)))
	return_trace (خطأ);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < شمېر - 1 ? st : (ثابت Sub_جدول *) تش_اشاره);

      إذا (unlikely (!st->sanitize (c)))
	return_trace (خطأ);

      st = &StructAfter<Sub_جدول> (*st);
    }

    بینښه major_نسخه = thiz()->نسخه;
    إذا (حجم (thiz()->نسخه) == 4)
      major_نسخه = major_نسخه >> 16;
    إذا (major_نسخه >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) st;
      إذا (!coverage->sanitize (c, شمېر))
        return_trace (خطأ);
    }

    return_trace (ریښتیا);
  }

  kern_accelerator_معلومات_t create_accelerator_معلومات (بینښه عدد_glyphs) ثابت
  {
    kern_accelerator_معلومات_t accel_معلومات;

    تعريفنوع ډول_نوم T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->لومړی_Sub_جدول;
    بینښه صحيح شمېر = thiz()->جدول_شمېر;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      خپلکاری &subtable_accel = *accel_معلومات.subtable_accels.push ();
      إذا (unlikely (accel_معلومات.subtable_accels.دننه_error ()))
	  أرجع accel_معلومات;

      st->collect_glyphs (subtable_accel.لومړی_سېټ, subtable_accel.دوهم_سېټ, عدد_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<Sub_جدول> (*st);
    }

    أرجع accel_معلومات;
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->جدول = sc.reference_جدول<T> (face);
      هذا->accel_معلومات = هذا->جدول->create_accelerator_معلومات (face->get_عدد_glyphs ());
    }
    ~accelerator_t ()
    {
      هذا->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع جدول.get_blob (); }

    منطقي apply (AAT::hb_aat_apply_context_t *c) ثابت
    {
      أرجع جدول->apply (c, accel_معلومات);
    }

    hb_blob_ptr_t<T> جدول;
    kern_accelerator_معلومات_t accel_معلومات;
    hb_aat_scratch_t scratch;
  };
};

بنية kerx : Kerx_جدول<kerx>
{
  ملګری بنية Kerx_جدول<kerx>;

  ساكن constexpr hb_نښان_t جدول_نښان = HB_AAT_نښان_kerx;
  ساكن constexpr بینښه min_نسخه = 2u;

  تعريفنوع Kerx_Sub_جدول_سرلیک Sub_جدول_سرلیک;
  تعريفنوع Sub_جدول_سرلیک::Types Types;
  تعريفنوع Kerx_Sub_جدول Sub_جدول;

  منطقي has_معلومات () ثابت { أرجع نسخه; }

  محمي:
  HBUINT16	نسخه;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	ناکارول_شوی_2;		/* Set to 0. */
  HBUINT32	جدول_شمېر;	/* The number of subtables included in the extended kerning
				 * table. */
  Sub_جدول	لومړی_Sub_جدول;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  عام:
  DEFINE_اندازه_MIN (8);
};

بنية kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#د_که_پای /* HB_AAT_LAYOUT_KERX_TABLE_HH */
