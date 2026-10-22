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

#جے_بنا_تعریف HB_AAT_LAYOUT_KERX_جدول_HH
#تعریف_کرو HB_AAT_LAYOUT_KERX_جدول_HH

#ضمن "hb-kern.hh"
#ضمن "hb-aat-layout-ankr-table.hh"
#ضمن "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#تعریف_کرو HB_AAT_ٹیگ_kerx HB_ٹیگ('k','e','r','x')


نطاق AAT {

ورتوں نطاق OT;


ساكن مضمن صحيح
kerxTupleKern (صحيح value,
	       بنانشانی صحيح tuple_گنتی,
	       ثابت فراغ *ادھار,
	       hb_aat_apply_context_t *c)
{
  إذا (likely (!tuple_گنتی || !c)) أرجع value;

  بنانشانی صحيح وتھ = value;
  ثابت FWORD *pv = &Struct_At_وتھ<FWORD> (ادھار, وتھ);
  إذا (unlikely (!c->sanitizer.check_array (pv, tuple_گنتی))) أرجع 0;
  hb_barrier ();
  أرجع *pv;
}


بنية hb_glyph_pair_t
{
  hb_codepoint_t کھبے;
  hb_codepoint_t سجے;
};

بنية KernPair
{
  صحيح get_kerning () ثابت { أرجع value; }

  صحيح cmp (ثابت hb_glyph_pair_t &o) ثابت
  {
    صحيح ret = کھبے.cmp (o.کھبے);
    إذا (ret) أرجع ret;
    أرجع سجے.cmp (o.سجے);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBGlyphID16	کھبے;
  HBGlyphID16	سجے;
  FWORD		value;
  عام:
  DEFINE_ناپ_STATIC (6);
};

سانچا <قسم_ناں Kern_Sub_جدول_سرخی>
بنية Kerx_Sub_جدول_Format0
{
  صحيح get_kerning (hb_codepoint_t کھبے, hb_codepoint_t سجے,
		   hb_aat_apply_context_t *c = خالی_سوچک) ثابت
  {
    hb_glyph_pair_t pair = {کھبے, سجے};
    صحيح v = جوڑے.bsearch (pair).get_kerning ();
    أرجع kerxTupleKern (v, سرخی.tuple_گنتی_2 (), هذا, c);
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرخی.coverage & سرخی.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرخی.coverage & سرخی.Cross_دھارا);
    machine.kern (c->font, c->بفر, c->plan->kern_نقاب);

    return_trace (سچ);
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوجا_سیٹ, بنانشانی عدد_glyphs) ثابت
  {
    لكل (ثابت KernPair& pair : جوڑے)
    {
      پہلا_سیٹ.جوڑو (pair.کھبے);
      دوجا_سیٹ.جوڑو (pair.سجے);
    }
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format0 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format0 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t کھبے, hb_codepoint_t سجے) ثابت
    {
      إذا (!(*c->پہلا_سیٹ)[کھبے] || !(*c->دوجا_سیٹ)[سجے]) أرجع 0;
      أرجع جدول.get_kerning (کھبے, سجے, c);
    }
  };


  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (جوڑے.sanitize (c)));
  }

  محمي:
  Kern_Sub_جدول_سرخی	سرخی;
  Bin_تلاش_Array_دا<KernPair, قسم_ناں Kern_Sub_جدول_سرخی::Types::HBUINT>
			جوڑے;	/* Sorted kern records. */
  عام:
  DEFINE_ناپ_ARRAY (Kern_Sub_جدول_سرخی::static_ناپ + 16, جوڑے);
};


سانچا <منطقي extended>
بنية Format1_اندراج;

سانچا <>
بنية Format1_اندراج<سچ>
{
  تعداد Flags
  {
    ڈالو		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  بنية اندراج_انکڑے
  {
    HBUINT16	kern_Action_اشاریہ;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    عام:
    DEFINE_ناپ_STATIC (2);
  };

  ساكن منطقي initiateAction (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.flags & ڈالو; }

  ساكن منطقي performAction (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.انکڑے.kern_Action_اشاریہ != 0xFFFF; }

  ساكن بنانشانی صحيح kern_Action_اشاریہ (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.انکڑے.kern_Action_اشاریہ; }
};
سانچا <>
بنية Format1_اندراج<خطأ>
{
  تعداد Flags
  {
    ڈالو		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    وتھ_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  تعريفنوع فراغ اندراج_انکڑے;

  ساكن منطقي initiateAction (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.flags & ڈالو; }

  ساكن منطقي performAction (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.flags & وتھ_2; }

  ساكن بنانشانی صحيح kern_Action_اشاریہ (ثابت اندراج_2<اندراج_انکڑے> &اندراج)
  { أرجع اندراج.flags & وتھ_2; }
};

سانچا <قسم_ناں Kern_Sub_جدول_سرخی>
بنية Kerx_Sub_جدول_Format1
{
  تعريفنوع قسم_ناں Kern_Sub_جدول_سرخی::Types Types;
  تعريفنوع قسم_ناں Types::HBUINT HBUINT;

  تعريفنوع Format1_اندراج<Types::extended> Format1_اندراج_T;
  تعريفنوع قسم_ناں Format1_اندراج_T::اندراج_انکڑے اندراج_انکڑے;

  تعداد Flags
  {
    DontAdvance	= Format1_اندراج_T::DontAdvance,
  };

  منطقي is_action_initiable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    أرجع Format1_اندراج_T::initiateAction (اندراج);
  }
  منطقي is_actionable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    أرجع Format1_اندراج_T::performAction (اندراج);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = سچ;

    driver_context_t (ثابت Kerx_Sub_جدول_Format1 *جدول_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	جدول (جدول_2),
	/* Apparently the offset kernAction is from the beginning of the state-machine,
	 * similar to offsets in morx table, NOT from beginning of this table, like
	 * other subtables in kerx.  Discovered via testing. */
	kernAction (&جدول->machine + جدول->kernAction),
	ڈونگھائی (0),
	cross_دھارا (جدول->سرخی.coverage & جدول->سرخی.Cross_دھارا) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, اندراج_انکڑے, Flags> *driver,
		     ثابت اندراج_2<اندراج_انکڑے> &اندراج)
    {
      بنانشانی صحيح flags = اندراج.flags;

      إذا (flags & Format1_اندراج_T::Reset)
	ڈونگھائی = 0;

      إذا (flags & Format1_اندراج_T::ڈالو)
      {
	إذا (likely (ڈونگھائی < ARRAY_لمائی (ڈھیر)))
	  ڈھیر[ڈونگھائی++] = بفر->idx;
	وإلا
	  ڈونگھائی = 0; /* Probably not what CoreText does, but better? */
      }

      إذا (Format1_اندراج_T::performAction (اندراج) && ڈونگھائی)
      {
	بنانشانی صحيح tuple_گنتی_2 = hb_max (1u, جدول->سرخی.tuple_گنتی_2 ());

	بنانشانی صحيح kern_idx = Format1_اندراج_T::kern_Action_اشاریہ (اندراج);
	kern_idx = Types::byte_وتھ_منزل_اشاریہ (kern_idx, &جدول->machine, kernAction.arrayZ);
	ثابت FWORD *actions = &kernAction[kern_idx];
	إذا (!c->sanitizer.check_array (actions, ڈونگھائی, tuple_گنتی_2))
	{
	  ڈونگھائی = 0;
	  أرجع;
	}
	hb_barrier ();

	hb_نقاب_t kern_نقاب = c->plan->kern_نقاب;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	منطقي آخری = خطأ;
	طالما (!آخری && ڈونگھائی)
	{
	  بنانشانی صحيح idx = ڈھیر[--ڈونگھائی];
	  صحيح v = *actions;
	  actions += tuple_گنتی_2;
	  إذا (idx >= بفر->len) استمر;

	  /* "The end of the list is marked by an odd value..." */
	  آخری = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = بفر->pos[idx];

	  إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
	  {
	    إذا (cross_دھارا)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      إذا (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_وتھ_2 = 0;
	      }
	      وإلا إذا (o.attach_type())
	      {
		o.y_وتھ_2 = hb_saturate_جوڑو (o.y_وتھ_2, c->font->em_scale_y (v));
		بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وإلا إذا (بفر->info[idx].نقاب & kern_نقاب)
	    {
	      خودکار scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_جوڑو (o.x_advance, scaled);
	      o.x_وتھ_2 = hb_saturate_جوڑو (o.x_وتھ_2, scaled);
	    }
	  }
	  وإلا
	  {
	    إذا (cross_دھارا)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      إذا (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_وتھ_2 = 0;
	      }
	      وإلا إذا (o.attach_type())
	      {
		o.x_وتھ_2 = hb_saturate_جوڑو (o.x_وتھ_2, c->font->em_scale_x (v));
		بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وإلا إذا (بفر->info[idx].نقاب & kern_نقاب)
	    {
	      خودکار scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_جوڑو (o.y_advance, scaled);
	      o.y_وتھ_2 = hb_saturate_جوڑو (o.y_وتھ_2, scaled);
	    }
	  }
	}
      }
    }

    عام:
    hb_aat_apply_context_t *c;
    ثابت Kerx_Sub_جدول_Format1 *جدول;
    خاص:
    ثابت Unsized_Array_دا<FWORD> &kernAction;
    بنانشانی صحيح ڈھیر[8];
    بنانشانی صحيح ڈونگھائی;
    منطقي cross_دھارا;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning &&
	!(سرخی.coverage & سرخی.Cross_دھارا))
      أرجع خطأ;

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, اندراج_انکڑے, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (هذا) &&
			  machine.sanitize (c)));
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوجا_سیٹ, بنانشانی عدد_glyphs) ثابت
  {
    machine.collect_initial_glyphs (پہلا_سیٹ, عدد_glyphs, *هذا);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محمي:
  Kern_Sub_جدول_سرخی				سرخی;
  State_جدول<Types, اندراج_انکڑے>			machine;
  NNOffset_منزل<Unsized_Array_دا<FWORD>, HBUINT>	kernAction;
  عام:
  DEFINE_ناپ_STATIC (Kern_Sub_جدول_سرخی::static_ناپ + (State_جدول<Types, اندراج_انکڑے>::static_ناپ + HBUINT::static_ناپ));
};

سانچا <قسم_ناں Kern_Sub_جدول_سرخی>
بنية Kerx_Sub_جدول_Format2
{
  تعريفنوع قسم_ناں Kern_Sub_جدول_سرخی::Types Types;
  تعريفنوع قسم_ناں Types::HBUINT HBUINT;

  صحيح get_kerning (hb_codepoint_t کھبے, hb_codepoint_t سجے,
		   hb_aat_apply_context_t *c) ثابت
  {
    بنانشانی صحيح عدد_glyphs = c->sanitizer.get_عدد_glyphs ();
    بنانشانی صحيح l = (هذا+بایاں_Class_جدول).get_class (کھبے, عدد_glyphs, 0);
    بنانشانی صحيح r = (هذا+دایاں_Class_جدول).get_class (سجے, عدد_glyphs, 0);

    ثابت Unsized_Array_دا<FWORD> &arrayZ = هذا+array;
    بنانشانی صحيح kern_idx = l + r;
    kern_idx = Types::وتھ_منزل_اشاریہ (kern_idx, هذا, arrayZ.arrayZ);
    ثابت FWORD *v = &arrayZ[kern_idx];
    إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
    hb_barrier ();

    أرجع kerxTupleKern (*v, سرخی.tuple_گنتی_2 (), هذا, c);
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرخی.coverage & سرخی.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرخی.coverage & سرخی.Cross_دھارا);
    machine.kern (c->font, c->بفر, c->plan->kern_نقاب);

    return_trace (سچ);
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوجا_سیٹ, بنانشانی عدد_glyphs) ثابت
  {
    (هذا+بایاں_Class_جدول).collect_glyphs (پہلا_سیٹ, عدد_glyphs);
    (هذا+دایاں_Class_جدول).collect_glyphs (دوجا_سیٹ, عدد_glyphs);
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format2 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format2 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t کھبے, hb_codepoint_t سجے) ثابت
    {
      إذا (!(*c->پہلا_سیٹ)[کھبے] || !(*c->دوجا_سیٹ)[سجے]) أرجع 0;
      أرجع جدول.get_kerning (کھبے, سجے, c);
    }
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  بایاں_Class_جدول.sanitize (c, هذا) &&
			  دایاں_Class_جدول.sanitize (c, هذا) &&
			  hb_barrier () &&
			  c->check_range (هذا, array)));
  }

  محمي:
  Kern_Sub_جدول_سرخی	سرخی;
  HBUINT		صف_چوڑائی;	/* The width, in bytes, of a row in the table. */
  NNOffset_منزل<قسم_ناں Types::ClassTypeWide, HBUINT>
			بایاں_Class_جدول;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffset_منزل<قسم_ناں Types::ClassTypeWide, HBUINT>
			دایاں_Class_جدول;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffset_منزل<Unsized_Array_دا<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  عام:
  DEFINE_ناپ_STATIC (Kern_Sub_جدول_سرخی::static_ناپ + 4 * حجم (HBUINT));
};

سانچا <قسم_ناں Kern_Sub_جدول_سرخی>
بنية Kerx_Sub_جدول_Format4
{
  تعريفنوع ExtendedTypes Types;

  بنية اندراج_انکڑے
  {
    HBUINT16	ankr_Action_اشاریہ;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    عام:
    DEFINE_ناپ_STATIC (2);
  };

  تعداد Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  منطقي is_action_initiable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    أرجع (اندراج.flags & Mark);
  }
  منطقي is_actionable (ثابت اندراج_2<اندراج_انکڑے> &اندراج) ثابت
  {
    أرجع اندراج.انکڑے.ankr_Action_اشاریہ != 0xFFFF;
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = سچ;
    تعداد Sub_جدول_Flags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      بن_ورتیا		= 0x3F000000,	/* Unused - must be zero. */
      وتھ_2		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ثابت Kerx_Sub_جدول_Format4 *جدول_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	جدول (جدول_2),
	action_type ((جدول->flags & ActionType) >> 30),
	ankr_انکڑے ((HBUINT16 *) ((ثابت حرف *) &جدول->machine + (جدول->flags & وتھ_2))),
	mark_سیٹ (خطأ),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, اندراج_انکڑے, Flags> *driver,
		     ثابت اندراج_2<اندراج_انکڑے> &اندراج)
    {
      إذا (mark_سیٹ && اندراج.انکڑے.ankr_Action_اشاریہ != 0xFFFF && بفر->idx < بفر->len)
      {
	hb_glyph_position_t &o = بفر->cur_pos();
	بدل (action_type)
	{
	  حالة 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *انکڑے = &ankr_انکڑے[اندراج.انکڑے.ankr_Action_اشاریہ * 2];
	    إذا (!c->sanitizer.check_array (انکڑے, 2)) أرجع;
	    hb_barrier ();
	    بنانشانی صحيح mark_Control_نقطہ = *انکڑے++;
	    بنانشانی صحيح curr_Control_نقطہ = *انکڑے++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    إذا (!c->font->get_glyph_contour_نقطہ_for_origin (c->بفر->info[mark].codepoint,
							      mark_Control_نقطہ,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_نقطہ_for_origin (c->بفر->cur ().codepoint,
							      curr_Control_نقطہ,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      أرجع;

	    o.x_وتھ_2 = hb_saturate_sub (markX, currX);
	    o.y_وتھ_2 = hb_saturate_sub (markY, currY);
	  }
	  اكسر;

	  حالة 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *انکڑے = &ankr_انکڑے[اندراج.انکڑے.ankr_Action_اشاریہ * 2];
	    إذا (!c->sanitizer.check_array (انکڑے, 2)) أرجع;
	    hb_barrier ();
	    بنانشانی صحيح mark_Anchor_نقطہ = *انکڑے++;
	    بنانشانی صحيح curr_Anchor_نقطہ = *انکڑے++;
	    ثابت Anchor &markAnchor = c->ankr_جدول->get_anchor (c->بفر->info[mark].codepoint,
								  mark_Anchor_نقطہ,
								  c->sanitizer.get_عدد_glyphs ());
	    ثابت Anchor &currAnchor = c->ankr_جدول->get_anchor (c->بفر->cur ().codepoint,
								  curr_Anchor_نقطہ,
								  c->sanitizer.get_عدد_glyphs ());

	    o.x_وتھ_2 = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_وتھ_2 = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  اكسر;

	  حالة 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ثابت FWORD *انکڑے = (ثابت FWORD *) &ankr_انکڑے[اندراج.انکڑے.ankr_Action_اشاریہ * 4];
	    إذا (!c->sanitizer.check_array (انکڑے, 4)) أرجع;
	    hb_barrier ();
	    صحيح markX = *انکڑے++;
	    صحيح markY = *انکڑے++;
	    صحيح currX = *انکڑے++;
	    صحيح currY = *انکڑے++;

	    o.x_وتھ_2 = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_وتھ_2 = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  اكسر;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (صحيح) mark - (صحيح) بفر->idx;
	إذا (c->بفر_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      إذا (اندراج.flags & Mark)
      {
	mark_سیٹ = سچ;
	mark = بفر->idx;
      }
    }

    عام:
    hb_aat_apply_context_t *c;
    ثابت Kerx_Sub_جدول_Format4 *جدول;
    خاص:
    بنانشانی صحيح action_type;
    ثابت HBUINT16 *ankr_انکڑے;
    منطقي mark_سیٹ;
    بنانشانی صحيح mark;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, اندراج_انکڑے, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (هذا) &&
			  machine.sanitize (c)));
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوجا_سیٹ, بنانشانی عدد_glyphs) ثابت
  {
    machine.collect_initial_glyphs (پہلا_سیٹ, عدد_glyphs, *هذا);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محمي:
  Kern_Sub_جدول_سرخی		سرخی;
  State_جدول<Types, اندراج_انکڑے>	machine;
  HBUINT32			flags;
  عام:
  DEFINE_ناپ_STATIC (Kern_Sub_جدول_سرخی::static_ناپ + (State_جدول<Types, اندراج_انکڑے>::static_ناپ + HBUINT32::static_ناپ));
};

سانچا <قسم_ناں Kern_Sub_جدول_سرخی>
بنية Kerx_Sub_جدول_Format6
{
  تعداد Flags
  {
    اقدار_Are_Long	= 0x00000001,
  };

  منطقي is_long () ثابت { أرجع flags & اقدار_Are_Long; }

  صحيح get_kerning (hb_codepoint_t کھبے, hb_codepoint_t سجے,
		   hb_aat_apply_context_t *c) ثابت
  {
    بنانشانی صحيح عدد_glyphs = c->sanitizer.get_عدد_glyphs ();
    إذا (is_long ())
    {
      ثابت خودکار &t = u.l;
      بنانشانی صحيح l = (هذا+t.صف_اشاریہ_جدول).get_قدر_or_null (کھبے, عدد_glyphs);
      بنانشانی صحيح r = (هذا+t.کالم_اشاریہ_جدول).get_قدر_or_null (سجے, عدد_glyphs);
      بنانشانی صحيح وتھ = l + r;
      إذا (unlikely (وتھ < l)) أرجع 0; /* Addition overflow. */
      إذا (unlikely (hb_unsigned_mul_overflows (وتھ, حجم (FWORD32)))) أرجع 0;
      ثابت FWORD32 *v = &Struct_At_وتھ<FWORD32> (&(هذا+t.array), وتھ * حجم (FWORD32));
      إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
      hb_barrier ();
      أرجع kerxTupleKern (*v, سرخی.tuple_گنتی_2 (), &(هذا+vector), c);
    }
    وإلا
    {
      ثابت خودکار &t = u.s;
      بنانشانی صحيح l = (هذا+t.صف_اشاریہ_جدول).get_قدر_or_null (کھبے, عدد_glyphs);
      بنانشانی صحيح r = (هذا+t.کالم_اشاریہ_جدول).get_قدر_or_null (سجے, عدد_glyphs);
      بنانشانی صحيح وتھ = l + r;
      ثابت FWORD *v = &Struct_At_وتھ<FWORD> (&(هذا+t.array), وتھ * حجم (FWORD));
      إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
      hb_barrier ();
      أرجع kerxTupleKern (*v, سرخی.tuple_گنتی_2 (), &(هذا+vector), c);
    }
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرخی.coverage & سرخی.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرخی.coverage & سرخی.Cross_دھارا);
    machine.kern (c->font, c->بفر, c->plan->kern_نقاب);

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.صف_اشاریہ_جدول.sanitize (c, هذا) &&
			     u.l.کالم_اشاریہ_جدول.sanitize (c, هذا) &&
			     c->check_range (هذا, u.l.array)
			   ) : (
			     u.s.صف_اشاریہ_جدول.sanitize (c, هذا) &&
			     u.s.کالم_اشاریہ_جدول.sanitize (c, هذا) &&
			     c->check_range (هذا, u.s.array)
			   )) &&
			  (سرخی.tuple_گنتی_2 () == 0 ||
			   c->check_range (هذا, vector))));
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوجا_سیٹ, بنانشانی عدد_glyphs) ثابت
  {
    إذا (is_long ())
    {
      ثابت خودکار &t = u.l;
      (هذا+t.صف_اشاریہ_جدول).collect_glyphs (پہلا_سیٹ, عدد_glyphs);
      (هذا+t.کالم_اشاریہ_جدول).collect_glyphs (دوجا_سیٹ, عدد_glyphs);
    }
    وإلا
    {
      ثابت خودکار &t = u.s;
      (هذا+t.صف_اشاریہ_جدول).collect_glyphs (پہلا_سیٹ, عدد_glyphs);
      (هذا+t.کالم_اشاریہ_جدول).collect_glyphs (دوجا_سیٹ, عدد_glyphs);
    }
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format6 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format6 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t کھبے, hb_codepoint_t سجے) ثابت
    {
      إذا (!(*c->پہلا_سیٹ)[کھبے] || !(*c->دوجا_سیٹ)[سجے]) أرجع 0;
      أرجع جدول.get_kerning (کھبے, سجے, c);
    }
  };

  محمي:
  Kern_Sub_جدول_سرخی		سرخی;
  HBUINT32			flags;
  HBUINT16			صف_گنتی_2;
  HBUINT16			کالم_گنتی;
  اتحاد U
  {
    بنية Long
    {
      NNOffset32_منزل<Lookup<HBUINT32>>		صف_اشاریہ_جدول;
      NNOffset32_منزل<Lookup<HBUINT32>>		کالم_اشاریہ_جدول;
      NNOffset32_منزل<Unsized_Array_دا<FWORD32>>	array;
    } l;
    بنية Short
    {
      NNOffset32_منزل<Lookup<HBUINT16>>		صف_اشاریہ_جدول;
      NNOffset32_منزل<Lookup<HBUINT16>>		کالم_اشاریہ_جدول;
      NNOffset32_منزل<Unsized_Array_دا<FWORD>>	array;
    } s;
  } u;
  NNOffset32_منزل<Unsized_Array_دا<FWORD>>	vector;
  عام:
  DEFINE_ناپ_STATIC (Kern_Sub_جدول_سرخی::static_ناپ + 24);
};


بنية Kerx_Sub_جدول_سرخی
{
  تعريفنوع ExtendedTypes Types;

  بنانشانی   tuple_گنتی_2 () ثابت { أرجع tuple_گنتی; }
  منطقي     is_horizontal () ثابت { أرجع !(coverage & Vertical); }

  تعداد Coverage
  {
    Vertical	= 0x80000000u,	/* Set if table has vertical kerning values. */
    Cross_دھارا	= 0x40000000u,	/* Set if table has cross-stream kerning values. */
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
  HBUINT32	لمائی;
  HBUINT32	coverage;
  HBUINT32	tuple_گنتی;
  عام:
  DEFINE_ناپ_STATIC (12);
};

بنية Kerx_Sub_جدول
{
  یار بنية kerx;

  size_t get_ناپ () ثابت { أرجع u.سرخی.لمائی; }
  بنانشانی صحيح get_type () ثابت { أرجع u.سرخی.coverage & u.سرخی.SubtableType; }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بنانشانی صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    افتراضي:	return_trace (c->default_return_قدر ());
    }
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوجا_سیٹ, بنانشانی عدد_glyphs) ثابت
  {
    بنانشانی صحيح subtable_type = get_type ();
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); u.format0.collect_glyphs (پہلا_سیٹ, دوجا_سیٹ, عدد_glyphs); أرجع;
    حالة 1:	hb_barrier (); u.format1.collect_glyphs (پہلا_سیٹ, دوجا_سیٹ, عدد_glyphs); أرجع;
    حالة 2:	hb_barrier (); u.format2.collect_glyphs (پہلا_سیٹ, دوجا_سیٹ, عدد_glyphs); أرجع;
    حالة 4:	hb_barrier (); u.format4.collect_glyphs (پہلا_سیٹ, دوجا_سیٹ, عدد_glyphs); أرجع;
    حالة 6:	hb_barrier (); u.format6.collect_glyphs (پہلا_سیٹ, دوجا_سیٹ, عدد_glyphs); أرجع;
    افتراضي:	أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(u.سرخی.sanitize (c) &&
	  hb_barrier () &&
	  u.سرخی.لمائی >= u.سرخی.static_ناپ &&
	  c->check_range (هذا, u.سرخی.لمائی)))
      return_trace (خطأ);

    return_trace (dispatch (c));
  }

  عام:
  اتحاد {
  Kerx_Sub_جدول_سرخی				سرخی;
  Kerx_Sub_جدول_Format0<Kerx_Sub_جدول_سرخی>	format0;
  Kerx_Sub_جدول_Format1<Kerx_Sub_جدول_سرخی>	format1;
  Kerx_Sub_جدول_Format2<Kerx_Sub_جدول_سرخی>	format2;
  Kerx_Sub_جدول_Format4<Kerx_Sub_جدول_سرخی>	format4;
  Kerx_Sub_جدول_Format6<Kerx_Sub_جدول_سرخی>	format6;
  } u;
  عام:
  DEFINE_ناپ_MIN (12);
};


/*
 * The 'kerx' Table
 */

بنية kern_subtable_accelerator_انکڑے_t
{
  hb_ثنائی_ہندسہ_سیٹ_t پہلا_سیٹ;
  hb_ثنائی_ہندسہ_سیٹ_t دوجا_سیٹ;
  بدلنجوگ hb_aat_class_cache_t class_cache;
};

بنية kern_accelerator_انکڑے_t
{
  hb_سمتیہ_t<kern_subtable_accelerator_انکڑے_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

سانچا <قسم_ناں T>
بنية Kerx_جدول
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت T* thiz () ثابت { أرجع static_cast<ثابت T *> (هذا); }

  منطقي has_state_machine () ثابت
  {
    تعريفنوع قسم_ناں T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    بنانشانی صحيح گنتی = thiz()->جدول_گنتی;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا (st->get_type () == 1)
	أرجع سچ;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع خطأ;
  }

  منطقي has_cross_دھارا () ثابت
  {
    تعريفنوع قسم_ناں T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    بنانشانی صحيح گنتی = thiz()->جدول_گنتی;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا (st->u.سرخی.coverage & st->u.سرخی.Cross_دھارا)
	أرجع سچ;
      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع خطأ;
  }

  صحيح get_h_kerning (hb_codepoint_t کھبے, hb_codepoint_t سجے) ثابت
  {
    تعريفنوع قسم_ناں T::Sub_جدول Sub_جدول;

    صحيح v = 0;
    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    بنانشانی صحيح گنتی = thiz()->جدول_گنتی;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا ((st->u.سرخی.coverage & (st->u.سرخی.Variation | st->u.سرخی.Cross_دھارا)) ||
	  !st->u.سرخی.is_horizontal ())
	استمر;
      v += st->get_kerning (کھبے, سجے);
      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع v;
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c,
	      ثابت kern_accelerator_انکڑے_t &accel_انکڑے) ثابت
  {
    c->بفر->unsafe_منزل_concat ();

    c->تیاری_بفر_glyph_سیٹ ();

    تعريفنوع قسم_ناں T::Sub_جدول Sub_جدول;

    منطقي ret = خطأ;
    منطقي تکیا_Cross_دھارا = خطأ;
    c->سیٹ_lookup_اشاریہ (0);
    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    بنانشانی صحيح گنتی = thiz()->جدول_گنتی;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      منطقي reverse;

      خودکار &subtable_accel = accel_انکڑے.subtable_accels[i];

      إذا (!T::Types::extended && (st->u.سرخی.coverage & st->u.سرخی.Variation))
	اذهب skip;

      إذا (HB_DIRECTION_IS_HORIZONTAL (c->بفر->props.direction) != st->u.سرخی.is_horizontal ())
	اذهب skip;

      c->پہلا_سیٹ = &subtable_accel.پہلا_سیٹ;
      c->دوجا_سیٹ = &subtable_accel.دوجا_سیٹ;
      c->machine_class_cache = &subtable_accel.class_cache;

      إذا (!c->بفر_intersects_machine ())
      {
	(فراغ) c->بفر->سنیہا (c->font, "skipped subtable %u because no glyph matches", c->lookup_اشاریہ);
	اذهب skip;
      }

      reverse = منطقي (st->u.سرخی.coverage & st->u.سرخی.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->بفر->props.direction);

      إذا (!c->بفر->سنیہا (c->font, "start subtable %u", c->lookup_اشاریہ))
	اذهب skip;

      إذا (!تکیا_Cross_دھارا &&
	  (st->u.سرخی.coverage & st->u.سرخی.Cross_دھارا))
      {
	/* Attach all glyphs into a chain. */
	تکیا_Cross_دھارا = سچ;
	hb_glyph_position_t *pos = c->بفر->pos;
	بنانشانی صحيح گنتی = c->بفر->len;
	لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->بفر->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      إذا (reverse != c->بفر_is_reversed)
        c->الٹ_بفر ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < گنتی - 1 ? st : (ثابت Sub_جدول *) خالی_سوچک);
	ret |= st->dispatch (c);
      }

      (فراغ) c->بفر->سنیہا (c->font, "end subtable %u", c->lookup_اشاریہ);

    skip:
      st = &StructAfter<Sub_جدول> (*st);
      c->سیٹ_lookup_اشاریہ (c->lookup_اشاریہ + 1);
    }
    إذا (c->بفر_is_reversed)
      c->الٹ_بفر ();

    أرجع ret;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(thiz()->نسخہ.sanitize (c) &&
		    hb_barrier () &&
		    (بنانشانی) thiz()->نسخہ >= (بنانشانی) T::min_نسخہ &&
		    thiz()->جدول_گنتی.sanitize (c))))
      return_trace (خطأ);

    تعريفنوع قسم_ناں T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    بنانشانی صحيح گنتی = thiz()->جدول_گنتی;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا (unlikely (!st->u.سرخی.sanitize (c)))
	return_trace (خطأ);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < گنتی - 1 ? st : (ثابت Sub_جدول *) خالی_سوچک);

      إذا (unlikely (!st->sanitize (c)))
	return_trace (خطأ);

      st = &StructAfter<Sub_جدول> (*st);
    }

    بنانشانی major_نسخہ = thiz()->نسخہ;
    إذا (حجم (thiz()->نسخہ) == 4)
      major_نسخہ = major_نسخہ >> 16;
    إذا (major_نسخہ >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) st;
      إذا (!coverage->sanitize (c, گنتی))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }

  kern_accelerator_انکڑے_t create_accelerator_انکڑے (بنانشانی عدد_glyphs) ثابت
  {
    kern_accelerator_انکڑے_t accel_انکڑے;

    تعريفنوع قسم_ناں T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    بنانشانی صحيح گنتی = thiz()->جدول_گنتی;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      خودکار &subtable_accel = *accel_انکڑے.subtable_accels.push ();
      إذا (unlikely (accel_انکڑے.subtable_accels.وچ_error ()))
	  أرجع accel_انکڑے;

      st->collect_glyphs (subtable_accel.پہلا_سیٹ, subtable_accel.دوجا_سیٹ, عدد_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<Sub_جدول> (*st);
    }

    أرجع accel_انکڑے;
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->جدول = sc.reference_جدول<T> (face);
      هذا->accel_انکڑے = هذا->جدول->create_accelerator_انکڑے (face->get_عدد_glyphs ());
    }
    ~accelerator_t ()
    {
      هذا->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع جدول.get_blob (); }

    منطقي apply (AAT::hb_aat_apply_context_t *c) ثابت
    {
      أرجع جدول->apply (c, accel_انکڑے);
    }

    hb_blob_ptr_t<T> جدول;
    kern_accelerator_انکڑے_t accel_انکڑے;
    hb_aat_scratch_t scratch;
  };
};

بنية kerx : Kerx_جدول<kerx>
{
  یار بنية Kerx_جدول<kerx>;

  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_AAT_ٹیگ_kerx;
  ساكن constexpr بنانشانی min_نسخہ = 2u;

  تعريفنوع Kerx_Sub_جدول_سرخی Sub_جدول_سرخی;
  تعريفنوع Sub_جدول_سرخی::Types Types;
  تعريفنوع Kerx_Sub_جدول Sub_جدول;

  منطقي has_انکڑے () ثابت { أرجع نسخہ; }

  محمي:
  HBUINT16	نسخہ;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	بن_ورتیا_2;		/* Set to 0. */
  HBUINT32	جدول_گنتی;	/* The number of subtables included in the extended kerning
				 * table. */
  Sub_جدول	پہلا_Sub_جدول;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  عام:
  DEFINE_ناپ_MIN (8);
};

بنية kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#جے_انت /* HB_AAT_LAYOUT_KERX_TABLE_HH */
