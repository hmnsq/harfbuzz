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

#اگر_تعریف_نشده HB_AAT_LAYOUT_KERX_جدول_HH
#تعریف_کن HB_AAT_LAYOUT_KERX_جدول_HH

#ضمن "hb-kern.hh"
#ضمن "hb-aat-layout-ankr-table.hh"
#ضمن "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#تعریف_کن HB_AAT_برچسب_kerx HB_برچسب('k','e','r','x')


نطاق AAT {

استفاده نطاق OT;


ساكن مضمن صحيح
kerxTupleKern (صحيح value,
	       بیعلامت صحيح tuple_شمار,
	       ثابت فراغ *پایه,
	       hb_aat_apply_context_t *c)
{
  إذا (likely (!tuple_شمار || !c)) أرجع value;

  بیعلامت صحيح جابه_جایی = value;
  ثابت FWORD *pv = &Struct_At_جابه_جایی<FWORD> (پایه, جابه_جایی);
  إذا (unlikely (!c->sanitizer.check_array (pv, tuple_شمار))) أرجع 0;
  hb_barrier ();
  أرجع *pv;
}


بنية hb_glyph_pair_t
{
  hb_codepoint_t چپ;
  hb_codepoint_t راست;
};

بنية KernPair
{
  صحيح get_kerning () ثابت { أرجع value; }

  صحيح cmp (ثابت hb_glyph_pair_t &o) ثابت
  {
    صحيح ret = چپ.cmp (o.چپ);
    إذا (ret) أرجع ret;
    أرجع راست.cmp (o.راست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBGlyphID16	چپ;
  HBGlyphID16	راست;
  FWORD		value;
  عام:
  DEFINE_اندازه_STATIC (6);
};

قالب <نام_نوع Kern_Sub_جدول_سرآیند>
بنية Kerx_Sub_جدول_Format0
{
  صحيح get_kerning (hb_codepoint_t چپ, hb_codepoint_t راست,
		   hb_aat_apply_context_t *c = اشاره_تهی) ثابت
  {
    hb_glyph_pair_t pair = {چپ, راست};
    صحيح v = جفتها.bsearch (pair).get_kerning ();
    أرجع kerxTupleKern (v, سرآیند.tuple_شمار_2 (), هذا, c);
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرآیند.coverage & سرآیند.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرآیند.coverage & سرآیند.Cross_جریان);
    machine.kern (c->font, c->میانگیر, c->plan->kern_ماسک);

    return_trace (درست);
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &اولی_مجموعه, مجموعه_t &دومی_مجموعه, بیعلامت رقم_glyphs) ثابت
  {
    لكل (ثابت KernPair& pair : جفتها)
    {
      اولی_مجموعه.جمع_کن (pair.چپ);
      دومی_مجموعه.جمع_کن (pair.راست);
    }
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format0 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format0 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t چپ, hb_codepoint_t راست) ثابت
    {
      إذا (!(*c->اولی_مجموعه)[چپ] || !(*c->دومی_مجموعه)[راست]) أرجع 0;
      أرجع جدول.get_kerning (چپ, راست, c);
    }
  };


  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (جفتها.sanitize (c)));
  }

  محمي:
  Kern_Sub_جدول_سرآیند	سرآیند;
  Bin_جستجو_Array_از<KernPair, نام_نوع Kern_Sub_جدول_سرآیند::Types::HBUINT>
			جفتها;	/* Sorted kern records. */
  عام:
  DEFINE_اندازه_ARRAY (Kern_Sub_جدول_سرآیند::static_اندازه + 16, جفتها);
};


قالب <منطقي extended>
بنية Format1_مدخل;

قالب <>
بنية Format1_مدخل<درست>
{
  تعداد Flags
  {
    افزودن		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  بنية مدخل_داده
  {
    HBUINT16	kern_Action_شاخص;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    عام:
    DEFINE_اندازه_STATIC (2);
  };

  ساكن منطقي initiateAction (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.flags & افزودن; }

  ساكن منطقي performAction (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.داده.kern_Action_شاخص != 0xFFFF; }

  ساكن بیعلامت صحيح kern_Action_شاخص (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.داده.kern_Action_شاخص; }
};
قالب <>
بنية Format1_مدخل<خطأ>
{
  تعداد Flags
  {
    افزودن		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    جابه_جایی_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  تعريفنوع فراغ مدخل_داده;

  ساكن منطقي initiateAction (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.flags & افزودن; }

  ساكن منطقي performAction (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.flags & جابه_جایی_2; }

  ساكن بیعلامت صحيح kern_Action_شاخص (ثابت مدخل_2<مدخل_داده> &مدخل)
  { أرجع مدخل.flags & جابه_جایی_2; }
};

قالب <نام_نوع Kern_Sub_جدول_سرآیند>
بنية Kerx_Sub_جدول_Format1
{
  تعريفنوع نام_نوع Kern_Sub_جدول_سرآیند::Types Types;
  تعريفنوع نام_نوع Types::HBUINT HBUINT;

  تعريفنوع Format1_مدخل<Types::extended> Format1_مدخل_T;
  تعريفنوع نام_نوع Format1_مدخل_T::مدخل_داده مدخل_داده;

  تعداد Flags
  {
    DontAdvance	= Format1_مدخل_T::DontAdvance,
  };

  منطقي is_action_initiable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    أرجع Format1_مدخل_T::initiateAction (مدخل);
  }
  منطقي is_actionable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    أرجع Format1_مدخل_T::performAction (مدخل);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = درست;

    driver_context_t (ثابت Kerx_Sub_جدول_Format1 *جدول_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	جدول (جدول_2),
	/* Apparently the offset kernAction is from the beginning of the state-machine,
	 * similar to offsets in morx table, NOT from beginning of this table, like
	 * other subtables in kerx.  Discovered via testing. */
	kernAction (&جدول->machine + جدول->kernAction),
	عمق (0),
	cross_جریان (جدول->سرآیند.coverage & جدول->سرآیند.Cross_جریان) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_میانگیر_t *میانگیر,
		     State_جدول_Driver<Types, مدخل_داده, Flags> *driver,
		     ثابت مدخل_2<مدخل_داده> &مدخل)
    {
      بیعلامت صحيح flags = مدخل.flags;

      إذا (flags & Format1_مدخل_T::Reset)
	عمق = 0;

      إذا (flags & Format1_مدخل_T::افزودن)
      {
	إذا (likely (عمق < ARRAY_درازی (پشته)))
	  پشته[عمق++] = میانگیر->idx;
	وإلا
	  عمق = 0; /* Probably not what CoreText does, but better? */
      }

      إذا (Format1_مدخل_T::performAction (مدخل) && عمق)
      {
	بیعلامت صحيح tuple_شمار_2 = hb_max (1u, جدول->سرآیند.tuple_شمار_2 ());

	بیعلامت صحيح kern_idx = Format1_مدخل_T::kern_Action_شاخص (مدخل);
	kern_idx = Types::byte_جابه_جایی_مقصد_شاخص (kern_idx, &جدول->machine, kernAction.arrayZ);
	ثابت FWORD *actions = &kernAction[kern_idx];
	إذا (!c->sanitizer.check_array (actions, عمق, tuple_شمار_2))
	{
	  عمق = 0;
	  أرجع;
	}
	hb_barrier ();

	hb_ماسک_t kern_ماسک = c->plan->kern_ماسک;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	منطقي آخرین = خطأ;
	طالما (!آخرین && عمق)
	{
	  بیعلامت صحيح idx = پشته[--عمق];
	  صحيح v = *actions;
	  actions += tuple_شمار_2;
	  إذا (idx >= میانگیر->len) استمر;

	  /* "The end of the list is marked by an odd value..." */
	  آخرین = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = میانگیر->pos[idx];

	  إذا (HB_DIRECTION_IS_HORIZONTAL (میانگیر->props.direction))
	  {
	    إذا (cross_جریان)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      إذا (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_جابه_جایی_2 = 0;
	      }
	      وإلا إذا (o.attach_type())
	      {
		o.y_جابه_جایی_2 = hb_saturate_جمع_کن (o.y_جابه_جایی_2, c->font->em_scale_y (v));
		میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وإلا إذا (میانگیر->info[idx].ماسک & kern_ماسک)
	    {
	      خودکار scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_جمع_کن (o.x_advance, scaled);
	      o.x_جابه_جایی_2 = hb_saturate_جمع_کن (o.x_جابه_جایی_2, scaled);
	    }
	  }
	  وإلا
	  {
	    إذا (cross_جریان)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      إذا (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_جابه_جایی_2 = 0;
	      }
	      وإلا إذا (o.attach_type())
	      {
		o.x_جابه_جایی_2 = hb_saturate_جمع_کن (o.x_جابه_جایی_2, c->font->em_scale_x (v));
		میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وإلا إذا (میانگیر->info[idx].ماسک & kern_ماسک)
	    {
	      خودکار scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_جمع_کن (o.y_advance, scaled);
	      o.y_جابه_جایی_2 = hb_saturate_جمع_کن (o.y_جابه_جایی_2, scaled);
	    }
	  }
	}
      }
    }

    عام:
    hb_aat_apply_context_t *c;
    ثابت Kerx_Sub_جدول_Format1 *جدول;
    خاص:
    ثابت Unsized_Array_از<FWORD> &kernAction;
    بیعلامت صحيح پشته[8];
    بیعلامت صحيح عمق;
    منطقي cross_جریان;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning &&
	!(سرآیند.coverage & سرآیند.Cross_جریان))
      أرجع خطأ;

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, مدخل_داده, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (هذا) &&
			  machine.sanitize (c)));
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &اولی_مجموعه, مجموعه_t &دومی_مجموعه, بیعلامت رقم_glyphs) ثابت
  {
    machine.collect_initial_glyphs (اولی_مجموعه, رقم_glyphs, *هذا);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محمي:
  Kern_Sub_جدول_سرآیند				سرآیند;
  State_جدول<Types, مدخل_داده>			machine;
  NNOffset_مقصد<Unsized_Array_از<FWORD>, HBUINT>	kernAction;
  عام:
  DEFINE_اندازه_STATIC (Kern_Sub_جدول_سرآیند::static_اندازه + (State_جدول<Types, مدخل_داده>::static_اندازه + HBUINT::static_اندازه));
};

قالب <نام_نوع Kern_Sub_جدول_سرآیند>
بنية Kerx_Sub_جدول_Format2
{
  تعريفنوع نام_نوع Kern_Sub_جدول_سرآیند::Types Types;
  تعريفنوع نام_نوع Types::HBUINT HBUINT;

  صحيح get_kerning (hb_codepoint_t چپ, hb_codepoint_t راست,
		   hb_aat_apply_context_t *c) ثابت
  {
    بیعلامت صحيح رقم_glyphs = c->sanitizer.get_رقم_glyphs ();
    بیعلامت صحيح l = (هذا+چپ_Class_جدول).get_class (چپ, رقم_glyphs, 0);
    بیعلامت صحيح r = (هذا+راست_Class_جدول).get_class (راست, رقم_glyphs, 0);

    ثابت Unsized_Array_از<FWORD> &arrayZ = هذا+array;
    بیعلامت صحيح kern_idx = l + r;
    kern_idx = Types::جابه_جایی_مقصد_شاخص (kern_idx, هذا, arrayZ.arrayZ);
    ثابت FWORD *v = &arrayZ[kern_idx];
    إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
    hb_barrier ();

    أرجع kerxTupleKern (*v, سرآیند.tuple_شمار_2 (), هذا, c);
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرآیند.coverage & سرآیند.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرآیند.coverage & سرآیند.Cross_جریان);
    machine.kern (c->font, c->میانگیر, c->plan->kern_ماسک);

    return_trace (درست);
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &اولی_مجموعه, مجموعه_t &دومی_مجموعه, بیعلامت رقم_glyphs) ثابت
  {
    (هذا+چپ_Class_جدول).collect_glyphs (اولی_مجموعه, رقم_glyphs);
    (هذا+راست_Class_جدول).collect_glyphs (دومی_مجموعه, رقم_glyphs);
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format2 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format2 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t چپ, hb_codepoint_t راست) ثابت
    {
      إذا (!(*c->اولی_مجموعه)[چپ] || !(*c->دومی_مجموعه)[راست]) أرجع 0;
      أرجع جدول.get_kerning (چپ, راست, c);
    }
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  چپ_Class_جدول.sanitize (c, هذا) &&
			  راست_Class_جدول.sanitize (c, هذا) &&
			  hb_barrier () &&
			  c->check_range (هذا, array)));
  }

  محمي:
  Kern_Sub_جدول_سرآیند	سرآیند;
  HBUINT		ردیف_عرض;	/* The width, in bytes, of a row in the table. */
  NNOffset_مقصد<نام_نوع Types::ClassTypeWide, HBUINT>
			چپ_Class_جدول;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffset_مقصد<نام_نوع Types::ClassTypeWide, HBUINT>
			راست_Class_جدول;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffset_مقصد<Unsized_Array_از<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  عام:
  DEFINE_اندازه_STATIC (Kern_Sub_جدول_سرآیند::static_اندازه + 4 * حجم (HBUINT));
};

قالب <نام_نوع Kern_Sub_جدول_سرآیند>
بنية Kerx_Sub_جدول_Format4
{
  تعريفنوع ExtendedTypes Types;

  بنية مدخل_داده
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

  منطقي is_action_initiable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    أرجع (مدخل.flags & Mark);
  }
  منطقي is_actionable (ثابت مدخل_2<مدخل_داده> &مدخل) ثابت
  {
    أرجع مدخل.داده.ankr_Action_شاخص != 0xFFFF;
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = درست;
    تعداد Sub_جدول_Flags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      استفادهنشده		= 0x3F000000,	/* Unused - must be zero. */
      جابه_جایی_2		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ثابت Kerx_Sub_جدول_Format4 *جدول_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	جدول (جدول_2),
	action_type ((جدول->flags & ActionType) >> 30),
	ankr_داده ((HBUINT16 *) ((ثابت حرف *) &جدول->machine + (جدول->flags & جابه_جایی_2))),
	mark_مجموعه (خطأ),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_میانگیر_t *میانگیر,
		     State_جدول_Driver<Types, مدخل_داده, Flags> *driver,
		     ثابت مدخل_2<مدخل_داده> &مدخل)
    {
      إذا (mark_مجموعه && مدخل.داده.ankr_Action_شاخص != 0xFFFF && میانگیر->idx < میانگیر->len)
      {
	hb_glyph_position_t &o = میانگیر->cur_pos();
	بدل (action_type)
	{
	  حالة 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *داده = &ankr_داده[مدخل.داده.ankr_Action_شاخص * 2];
	    إذا (!c->sanitizer.check_array (داده, 2)) أرجع;
	    hb_barrier ();
	    بیعلامت صحيح mark_Control_نقطه = *داده++;
	    بیعلامت صحيح curr_Control_نقطه = *داده++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    إذا (!c->font->get_glyph_contour_نقطه_for_origin (c->میانگیر->info[mark].codepoint,
							      mark_Control_نقطه,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_نقطه_for_origin (c->میانگیر->cur ().codepoint,
							      curr_Control_نقطه,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      أرجع;

	    o.x_جابه_جایی_2 = hb_saturate_sub (markX, currX);
	    o.y_جابه_جایی_2 = hb_saturate_sub (markY, currY);
	  }
	  اكسر;

	  حالة 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *داده = &ankr_داده[مدخل.داده.ankr_Action_شاخص * 2];
	    إذا (!c->sanitizer.check_array (داده, 2)) أرجع;
	    hb_barrier ();
	    بیعلامت صحيح mark_Anchor_نقطه = *داده++;
	    بیعلامت صحيح curr_Anchor_نقطه = *داده++;
	    ثابت Anchor &markAnchor = c->ankr_جدول->get_anchor (c->میانگیر->info[mark].codepoint,
								  mark_Anchor_نقطه,
								  c->sanitizer.get_رقم_glyphs ());
	    ثابت Anchor &currAnchor = c->ankr_جدول->get_anchor (c->میانگیر->cur ().codepoint,
								  curr_Anchor_نقطه,
								  c->sanitizer.get_رقم_glyphs ());

	    o.x_جابه_جایی_2 = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_جابه_جایی_2 = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  اكسر;

	  حالة 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ثابت FWORD *داده = (ثابت FWORD *) &ankr_داده[مدخل.داده.ankr_Action_شاخص * 4];
	    إذا (!c->sanitizer.check_array (داده, 4)) أرجع;
	    hb_barrier ();
	    صحيح markX = *داده++;
	    صحيح markY = *داده++;
	    صحيح currX = *داده++;
	    صحيح currY = *داده++;

	    o.x_جابه_جایی_2 = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_جابه_جایی_2 = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  اكسر;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (صحيح) mark - (صحيح) میانگیر->idx;
	إذا (c->میانگیر_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      إذا (مدخل.flags & Mark)
      {
	mark_مجموعه = درست;
	mark = میانگیر->idx;
      }
    }

    عام:
    hb_aat_apply_context_t *c;
    ثابت Kerx_Sub_جدول_Format4 *جدول;
    خاص:
    بیعلامت صحيح action_type;
    ثابت HBUINT16 *ankr_داده;
    منطقي mark_مجموعه;
    بیعلامت صحيح mark;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, مدخل_داده, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (هذا) &&
			  machine.sanitize (c)));
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &اولی_مجموعه, مجموعه_t &دومی_مجموعه, بیعلامت رقم_glyphs) ثابت
  {
    machine.collect_initial_glyphs (اولی_مجموعه, رقم_glyphs, *هذا);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محمي:
  Kern_Sub_جدول_سرآیند		سرآیند;
  State_جدول<Types, مدخل_داده>	machine;
  HBUINT32			flags;
  عام:
  DEFINE_اندازه_STATIC (Kern_Sub_جدول_سرآیند::static_اندازه + (State_جدول<Types, مدخل_داده>::static_اندازه + HBUINT32::static_اندازه));
};

قالب <نام_نوع Kern_Sub_جدول_سرآیند>
بنية Kerx_Sub_جدول_Format6
{
  تعداد Flags
  {
    مقادیر_Are_Long	= 0x00000001,
  };

  منطقي is_long () ثابت { أرجع flags & مقادیر_Are_Long; }

  صحيح get_kerning (hb_codepoint_t چپ, hb_codepoint_t راست,
		   hb_aat_apply_context_t *c) ثابت
  {
    بیعلامت صحيح رقم_glyphs = c->sanitizer.get_رقم_glyphs ();
    إذا (is_long ())
    {
      ثابت خودکار &t = u.l;
      بیعلامت صحيح l = (هذا+t.ردیف_شاخص_جدول).get_مقدار_or_null (چپ, رقم_glyphs);
      بیعلامت صحيح r = (هذا+t.ستونی_شاخص_جدول).get_مقدار_or_null (راست, رقم_glyphs);
      بیعلامت صحيح جابه_جایی = l + r;
      إذا (unlikely (جابه_جایی < l)) أرجع 0; /* Addition overflow. */
      إذا (unlikely (hb_unsigned_mul_overflows (جابه_جایی, حجم (FWORD32)))) أرجع 0;
      ثابت FWORD32 *v = &Struct_At_جابه_جایی<FWORD32> (&(هذا+t.array), جابه_جایی * حجم (FWORD32));
      إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
      hb_barrier ();
      أرجع kerxTupleKern (*v, سرآیند.tuple_شمار_2 (), &(هذا+vector), c);
    }
    وإلا
    {
      ثابت خودکار &t = u.s;
      بیعلامت صحيح l = (هذا+t.ردیف_شاخص_جدول).get_مقدار_or_null (چپ, رقم_glyphs);
      بیعلامت صحيح r = (هذا+t.ستونی_شاخص_جدول).get_مقدار_or_null (راست, رقم_glyphs);
      بیعلامت صحيح جابه_جایی = l + r;
      ثابت FWORD *v = &Struct_At_جابه_جایی<FWORD> (&(هذا+t.array), جابه_جایی * حجم (FWORD));
      إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
      hb_barrier ();
      أرجع kerxTupleKern (*v, سرآیند.tuple_شمار_2 (), &(هذا+vector), c);
    }
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرآیند.coverage & سرآیند.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرآیند.coverage & سرآیند.Cross_جریان);
    machine.kern (c->font, c->میانگیر, c->plan->kern_ماسک);

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.ردیف_شاخص_جدول.sanitize (c, هذا) &&
			     u.l.ستونی_شاخص_جدول.sanitize (c, هذا) &&
			     c->check_range (هذا, u.l.array)
			   ) : (
			     u.s.ردیف_شاخص_جدول.sanitize (c, هذا) &&
			     u.s.ستونی_شاخص_جدول.sanitize (c, هذا) &&
			     c->check_range (هذا, u.s.array)
			   )) &&
			  (سرآیند.tuple_شمار_2 () == 0 ||
			   c->check_range (هذا, vector))));
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &اولی_مجموعه, مجموعه_t &دومی_مجموعه, بیعلامت رقم_glyphs) ثابت
  {
    إذا (is_long ())
    {
      ثابت خودکار &t = u.l;
      (هذا+t.ردیف_شاخص_جدول).collect_glyphs (اولی_مجموعه, رقم_glyphs);
      (هذا+t.ستونی_شاخص_جدول).collect_glyphs (دومی_مجموعه, رقم_glyphs);
    }
    وإلا
    {
      ثابت خودکار &t = u.s;
      (هذا+t.ردیف_شاخص_جدول).collect_glyphs (اولی_مجموعه, رقم_glyphs);
      (هذا+t.ستونی_شاخص_جدول).collect_glyphs (دومی_مجموعه, رقم_glyphs);
    }
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format6 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format6 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t چپ, hb_codepoint_t راست) ثابت
    {
      إذا (!(*c->اولی_مجموعه)[چپ] || !(*c->دومی_مجموعه)[راست]) أرجع 0;
      أرجع جدول.get_kerning (چپ, راست, c);
    }
  };

  محمي:
  Kern_Sub_جدول_سرآیند		سرآیند;
  HBUINT32			flags;
  HBUINT16			ردیف_شمار_2;
  HBUINT16			ستونی_شمار;
  اتحاد U
  {
    بنية Long
    {
      NNOffset32_مقصد<Lookup<HBUINT32>>		ردیف_شاخص_جدول;
      NNOffset32_مقصد<Lookup<HBUINT32>>		ستونی_شاخص_جدول;
      NNOffset32_مقصد<Unsized_Array_از<FWORD32>>	array;
    } l;
    بنية Short
    {
      NNOffset32_مقصد<Lookup<HBUINT16>>		ردیف_شاخص_جدول;
      NNOffset32_مقصد<Lookup<HBUINT16>>		ستونی_شاخص_جدول;
      NNOffset32_مقصد<Unsized_Array_از<FWORD>>	array;
    } s;
  } u;
  NNOffset32_مقصد<Unsized_Array_از<FWORD>>	vector;
  عام:
  DEFINE_اندازه_STATIC (Kern_Sub_جدول_سرآیند::static_اندازه + 24);
};


بنية Kerx_Sub_جدول_سرآیند
{
  تعريفنوع ExtendedTypes Types;

  بیعلامت   tuple_شمار_2 () ثابت { أرجع tuple_شمار; }
  منطقي     is_horizontal () ثابت { أرجع !(coverage & Vertical); }

  تعداد Coverage
  {
    Vertical	= 0x80000000u,	/* Set if table has vertical kerning values. */
    Cross_جریان	= 0x40000000u,	/* Set if table has cross-stream kerning values. */
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
  HBUINT32	درازی;
  HBUINT32	coverage;
  HBUINT32	tuple_شمار;
  عام:
  DEFINE_اندازه_STATIC (12);
};

بنية Kerx_Sub_جدول
{
  دوست بنية kerx;

  size_t get_اندازه () ثابت { أرجع u.سرآیند.درازی; }
  بیعلامت صحيح get_type () ثابت { أرجع u.سرآیند.coverage & u.سرآیند.SubtableType; }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بیعلامت صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    افتراضي:	return_trace (c->default_return_مقدار ());
    }
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &اولی_مجموعه, مجموعه_t &دومی_مجموعه, بیعلامت رقم_glyphs) ثابت
  {
    بیعلامت صحيح subtable_type = get_type ();
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); u.format0.collect_glyphs (اولی_مجموعه, دومی_مجموعه, رقم_glyphs); أرجع;
    حالة 1:	hb_barrier (); u.format1.collect_glyphs (اولی_مجموعه, دومی_مجموعه, رقم_glyphs); أرجع;
    حالة 2:	hb_barrier (); u.format2.collect_glyphs (اولی_مجموعه, دومی_مجموعه, رقم_glyphs); أرجع;
    حالة 4:	hb_barrier (); u.format4.collect_glyphs (اولی_مجموعه, دومی_مجموعه, رقم_glyphs); أرجع;
    حالة 6:	hb_barrier (); u.format6.collect_glyphs (اولی_مجموعه, دومی_مجموعه, رقم_glyphs); أرجع;
    افتراضي:	أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(u.سرآیند.sanitize (c) &&
	  hb_barrier () &&
	  u.سرآیند.درازی >= u.سرآیند.static_اندازه &&
	  c->check_range (هذا, u.سرآیند.درازی)))
      return_trace (خطأ);

    return_trace (dispatch (c));
  }

  عام:
  اتحاد {
  Kerx_Sub_جدول_سرآیند				سرآیند;
  Kerx_Sub_جدول_Format0<Kerx_Sub_جدول_سرآیند>	format0;
  Kerx_Sub_جدول_Format1<Kerx_Sub_جدول_سرآیند>	format1;
  Kerx_Sub_جدول_Format2<Kerx_Sub_جدول_سرآیند>	format2;
  Kerx_Sub_جدول_Format4<Kerx_Sub_جدول_سرآیند>	format4;
  Kerx_Sub_جدول_Format6<Kerx_Sub_جدول_سرآیند>	format6;
  } u;
  عام:
  DEFINE_اندازه_MIN (12);
};


/*
 * The 'kerx' Table
 */

بنية kern_subtable_accelerator_داده_t
{
  hb_بیت_مجموعه_t اولی_مجموعه;
  hb_بیت_مجموعه_t دومی_مجموعه;
  تغییرپذیر hb_aat_class_cache_t class_cache;
};

بنية kern_accelerator_داده_t
{
  hb_وکتور_t<kern_subtable_accelerator_داده_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

قالب <نام_نوع T>
بنية Kerx_جدول
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت T* thiz () ثابت { أرجع static_cast<ثابت T *> (هذا); }

  منطقي has_state_machine () ثابت
  {
    تعريفنوع نام_نوع T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->اولی_Sub_جدول;
    بیعلامت صحيح شمار = thiz()->جدول_شمار;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (st->get_type () == 1)
	أرجع درست;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع خطأ;
  }

  منطقي has_cross_جریان () ثابت
  {
    تعريفنوع نام_نوع T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->اولی_Sub_جدول;
    بیعلامت صحيح شمار = thiz()->جدول_شمار;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (st->u.سرآیند.coverage & st->u.سرآیند.Cross_جریان)
	أرجع درست;
      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع خطأ;
  }

  صحيح get_h_kerning (hb_codepoint_t چپ, hb_codepoint_t راست) ثابت
  {
    تعريفنوع نام_نوع T::Sub_جدول Sub_جدول;

    صحيح v = 0;
    ثابت Sub_جدول *st = &thiz()->اولی_Sub_جدول;
    بیعلامت صحيح شمار = thiz()->جدول_شمار;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا ((st->u.سرآیند.coverage & (st->u.سرآیند.Variation | st->u.سرآیند.Cross_جریان)) ||
	  !st->u.سرآیند.is_horizontal ())
	استمر;
      v += st->get_kerning (چپ, راست);
      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع v;
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c,
	      ثابت kern_accelerator_داده_t &accel_داده) ثابت
  {
    c->میانگیر->unsafe_مقصد_concat ();

    c->راهاندازی_میانگیر_glyph_مجموعه ();

    تعريفنوع نام_نوع T::Sub_جدول Sub_جدول;

    منطقي ret = خطأ;
    منطقي دیده_Cross_جریان = خطأ;
    c->مجموعه_lookup_شاخص (0);
    ثابت Sub_جدول *st = &thiz()->اولی_Sub_جدول;
    بیعلامت صحيح شمار = thiz()->جدول_شمار;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      منطقي reverse;

      خودکار &subtable_accel = accel_داده.subtable_accels[i];

      إذا (!T::Types::extended && (st->u.سرآیند.coverage & st->u.سرآیند.Variation))
	اذهب skip;

      إذا (HB_DIRECTION_IS_HORIZONTAL (c->میانگیر->props.direction) != st->u.سرآیند.is_horizontal ())
	اذهب skip;

      c->اولی_مجموعه = &subtable_accel.اولی_مجموعه;
      c->دومی_مجموعه = &subtable_accel.دومی_مجموعه;
      c->machine_class_cache = &subtable_accel.class_cache;

      إذا (!c->میانگیر_intersects_machine ())
      {
	(فراغ) c->میانگیر->پیام (c->font, "skipped subtable %u because no glyph matches", c->lookup_شاخص);
	اذهب skip;
      }

      reverse = منطقي (st->u.سرآیند.coverage & st->u.سرآیند.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->میانگیر->props.direction);

      إذا (!c->میانگیر->پیام (c->font, "start subtable %u", c->lookup_شاخص))
	اذهب skip;

      إذا (!دیده_Cross_جریان &&
	  (st->u.سرآیند.coverage & st->u.سرآیند.Cross_جریان))
      {
	/* Attach all glyphs into a chain. */
	دیده_Cross_جریان = درست;
	hb_glyph_position_t *pos = c->میانگیر->pos;
	بیعلامت صحيح شمار = c->میانگیر->len;
	لكل (بیعلامت صحيح i = 0; i < شمار; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->میانگیر->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      إذا (reverse != c->میانگیر_is_reversed)
        c->معکوس_میانگیر ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < شمار - 1 ? st : (ثابت Sub_جدول *) اشاره_تهی);
	ret |= st->dispatch (c);
      }

      (فراغ) c->میانگیر->پیام (c->font, "end subtable %u", c->lookup_شاخص);

    skip:
      st = &StructAfter<Sub_جدول> (*st);
      c->مجموعه_lookup_شاخص (c->lookup_شاخص + 1);
    }
    إذا (c->میانگیر_is_reversed)
      c->معکوس_میانگیر ();

    أرجع ret;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(thiz()->نسخه.sanitize (c) &&
		    hb_barrier () &&
		    (بیعلامت) thiz()->نسخه >= (بیعلامت) T::min_نسخه &&
		    thiz()->جدول_شمار.sanitize (c))))
      return_trace (خطأ);

    تعريفنوع نام_نوع T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->اولی_Sub_جدول;
    بیعلامت صحيح شمار = thiz()->جدول_شمار;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (unlikely (!st->u.سرآیند.sanitize (c)))
	return_trace (خطأ);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < شمار - 1 ? st : (ثابت Sub_جدول *) اشاره_تهی);

      إذا (unlikely (!st->sanitize (c)))
	return_trace (خطأ);

      st = &StructAfter<Sub_جدول> (*st);
    }

    بیعلامت major_نسخه = thiz()->نسخه;
    إذا (حجم (thiz()->نسخه) == 4)
      major_نسخه = major_نسخه >> 16;
    إذا (major_نسخه >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) st;
      إذا (!coverage->sanitize (c, شمار))
        return_trace (خطأ);
    }

    return_trace (درست);
  }

  kern_accelerator_داده_t create_accelerator_داده (بیعلامت رقم_glyphs) ثابت
  {
    kern_accelerator_داده_t accel_داده;

    تعريفنوع نام_نوع T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->اولی_Sub_جدول;
    بیعلامت صحيح شمار = thiz()->جدول_شمار;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      خودکار &subtable_accel = *accel_داده.subtable_accels.push ();
      إذا (unlikely (accel_داده.subtable_accels.درون_error ()))
	  أرجع accel_داده;

      st->collect_glyphs (subtable_accel.اولی_مجموعه, subtable_accel.دومی_مجموعه, رقم_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<Sub_جدول> (*st);
    }

    أرجع accel_داده;
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->جدول = sc.reference_جدول<T> (face);
      هذا->accel_داده = هذا->جدول->create_accelerator_داده (face->get_رقم_glyphs ());
    }
    ~accelerator_t ()
    {
      هذا->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع جدول.get_blob (); }

    منطقي apply (AAT::hb_aat_apply_context_t *c) ثابت
    {
      أرجع جدول->apply (c, accel_داده);
    }

    hb_blob_ptr_t<T> جدول;
    kern_accelerator_داده_t accel_داده;
    hb_aat_scratch_t scratch;
  };
};

بنية kerx : Kerx_جدول<kerx>
{
  دوست بنية Kerx_جدول<kerx>;

  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_AAT_برچسب_kerx;
  ساكن constexpr بیعلامت min_نسخه = 2u;

  تعريفنوع Kerx_Sub_جدول_سرآیند Sub_جدول_سرآیند;
  تعريفنوع Sub_جدول_سرآیند::Types Types;
  تعريفنوع Kerx_Sub_جدول Sub_جدول;

  منطقي has_داده () ثابت { أرجع نسخه; }

  محمي:
  HBUINT16	نسخه;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	استفادهنشده_2;		/* Set to 0. */
  HBUINT32	جدول_شمار;	/* The number of subtables included in the extended kerning
				 * table. */
  Sub_جدول	اولی_Sub_جدول;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  عام:
  DEFINE_اندازه_MIN (8);
};

بنية kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#پایان_اگر /* HB_AAT_LAYOUT_KERX_TABLE_HH */
