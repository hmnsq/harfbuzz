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

#اگر_غیر_تعریف_شدہ HB_AAT_LAYOUT_KERX_جدول_HH
#تعریف_کرو HB_AAT_LAYOUT_KERX_جدول_HH

#شامل "hb-kern.hh"
#شامل "hb-aat-layout-ankr-table.hh"
#شامل "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#تعریف_کرو HB_AAT_لیبل_kerx HB_لیبل('k','e','r','x')


نامخلا AAT {

استعمال نامخلا OT;


ساکن اندرونی صحیح
kerxTupleKern (صحیح value,
	       غیرعلامتی صحیح tuple_گنتی,
	       ثابت خالی *بنیاد,
	       hb_aat_apply_context_t *c)
{
  اگر (likely (!tuple_گنتی || !c)) واپس value;

  غیرعلامتی صحیح انحراف = value;
  ثابت FWORD *pv = &Struct_At_انحراف<FWORD> (بنیاد, انحراف);
  اگر (unlikely (!c->sanitizer.check_array (pv, tuple_گنتی))) واپس 0;
  hb_barrier ();
  واپس *pv;
}


ساخت hb_glyph_pair_t
{
  hb_codepoint_t بائیں;
  hb_codepoint_t دائیں;
};

ساخت KernPair
{
  صحیح get_kerning () ثابت { واپس value; }

  صحیح cmp (ثابت hb_glyph_pair_t &o) ثابت
  {
    صحیح ret = بائیں.cmp (o.بائیں);
    اگر (ret) واپس ret;
    واپس دائیں.cmp (o.دائیں);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBGlyphID16	بائیں;
  HBGlyphID16	دائیں;
  FWORD		value;
  عوامی:
  DEFINE_سائز_STATIC (6);
};

سانچہ <قسم_نام Kern_Sub_جدول_سرخی>
ساخت Kerx_Sub_جدول_Format0
{
  صحیح get_kerning (hb_codepoint_t بائیں, hb_codepoint_t دائیں,
		   hb_aat_apply_context_t *c = خالی_اشاریہ) ثابت
  {
    hb_glyph_pair_t pair = {بائیں, دائیں};
    صحیح v = جوڑے.bsearch (pair).get_kerning ();
    واپس kerxTupleKern (v, سرخی.tuple_گنتی_2 (), یہ, c);
  }

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    اگر (!c->plan->requested_kerning)
      return_trace (جھوٹ);

    اگر (سرخی.coverage & سرخی.Backwards)
      return_trace (جھوٹ);

    accelerator_t accel (*یہ, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرخی.coverage & سرخی.Cross_دھارا);
    machine.kern (c->font, c->بفر, c->plan->kern_نقاب);

    return_trace (سچ);
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوسرا_سیٹ, غیرعلامتی عدد_glyphs) ثابت
  {
    برائے (ثابت KernPair& pair : جوڑے)
    {
      پہلا_سیٹ.جوڑو (pair.بائیں);
      دوسرا_سیٹ.جوڑو (pair.دائیں);
    }
  }

  ساخت accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format0 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format0 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحیح get_kerning (hb_codepoint_t بائیں, hb_codepoint_t دائیں) ثابت
    {
      اگر (!(*c->پہلا_سیٹ)[بائیں] || !(*c->دوسرا_سیٹ)[دائیں]) واپس 0;
      واپس جدول.get_kerning (بائیں, دائیں, c);
    }
  };


  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (جوڑے.sanitize (c)));
  }

  محفوظ:
  Kern_Sub_جدول_سرخی	سرخی;
  Bin_تلاش_Array_کا<KernPair, قسم_نام Kern_Sub_جدول_سرخی::Types::HBUINT>
			جوڑے;	/* Sorted kern records. */
  عوامی:
  DEFINE_سائز_ARRAY (Kern_Sub_جدول_سرخی::static_سائز + 16, جوڑے);
};


سانچہ <منطقی extended>
ساخت Format1_اندراج;

سانچہ <>
ساخت Format1_اندراج<سچ>
{
  شمار Flags
  {
    ڈالو		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  ساخت اندراج_ڈیٹا
  {
    HBUINT16	kern_Action_اشاریہ;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    عوامی:
    DEFINE_سائز_STATIC (2);
  };

  ساکن منطقی initiateAction (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.flags & ڈالو; }

  ساکن منطقی performAction (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.ڈیٹا.kern_Action_اشاریہ != 0xFFFF; }

  ساکن غیرعلامتی صحیح kern_Action_اشاریہ (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.ڈیٹا.kern_Action_اشاریہ; }
};
سانچہ <>
ساخت Format1_اندراج<جھوٹ>
{
  شمار Flags
  {
    ڈالو		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    انحراف_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  قسمتعریف خالی اندراج_ڈیٹا;

  ساکن منطقی initiateAction (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.flags & ڈالو; }

  ساکن منطقی performAction (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.flags & انحراف_2; }

  ساکن غیرعلامتی صحیح kern_Action_اشاریہ (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
  { واپس اندراج_2.flags & انحراف_2; }
};

سانچہ <قسم_نام Kern_Sub_جدول_سرخی>
ساخت Kerx_Sub_جدول_Format1
{
  قسمتعریف قسم_نام Kern_Sub_جدول_سرخی::Types Types;
  قسمتعریف قسم_نام Types::HBUINT HBUINT;

  قسمتعریف Format1_اندراج<Types::extended> Format1_اندراج_T;
  قسمتعریف قسم_نام Format1_اندراج_T::اندراج_ڈیٹا اندراج_ڈیٹا;

  شمار Flags
  {
    DontAdvance	= Format1_اندراج_T::DontAdvance,
  };

  منطقی is_action_initiable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    واپس Format1_اندراج_T::initiateAction (اندراج_2);
  }
  منطقی is_actionable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    واپس Format1_اندراج_T::performAction (اندراج_2);
  }

  ساخت driver_context_t
  {
    ساکن constexpr منطقی in_place = سچ;

    driver_context_t (ثابت Kerx_Sub_جدول_Format1 *جدول_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	جدول (جدول_2),
	/* Apparently the offset kernAction is from the beginning of the state-machine,
	 * similar to offsets in morx table, NOT from beginning of this table, like
	 * other subtables in kerx.  Discovered via testing. */
	kernAction (&جدول->machine + جدول->kernAction),
	گہرائی (0),
	cross_دھارا (جدول->سرخی.coverage & جدول->سرخی.Cross_دھارا) {}

    HB_AAT_TRANSITION_INLINE
    خالی transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> *driver,
		     ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
    {
      غیرعلامتی صحیح flags = اندراج_2.flags;

      اگر (flags & Format1_اندراج_T::Reset)
	گہرائی = 0;

      اگر (flags & Format1_اندراج_T::ڈالو)
      {
	اگر (likely (گہرائی < ARRAY_لمبائی (ڈھیر)))
	  ڈھیر[گہرائی++] = بفر->idx;
	ورنہ
	  گہرائی = 0; /* Probably not what CoreText does, but better? */
      }

      اگر (Format1_اندراج_T::performAction (اندراج_2) && گہرائی)
      {
	غیرعلامتی صحیح tuple_گنتی_2 = hb_max (1u, جدول->سرخی.tuple_گنتی_2 ());

	غیرعلامتی صحیح kern_idx = Format1_اندراج_T::kern_Action_اشاریہ (اندراج_2);
	kern_idx = Types::byte_انحراف_مقصد_اشاریہ (kern_idx, &جدول->machine, kernAction.arrayZ);
	ثابت FWORD *actions = &kernAction[kern_idx];
	اگر (!c->sanitizer.check_array (actions, گہرائی, tuple_گنتی_2))
	{
	  گہرائی = 0;
	  واپس;
	}
	hb_barrier ();

	hb_نقاب_t kern_نقاب = c->plan->kern_نقاب;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	منطقی آخری = جھوٹ;
	جب (!آخری && گہرائی)
	{
	  غیرعلامتی صحیح idx = ڈھیر[--گہرائی];
	  صحیح v = *actions;
	  actions += tuple_گنتی_2;
	  اگر (idx >= بفر->len) جاری;

	  /* "The end of the list is marked by an odd value..." */
	  آخری = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = بفر->pos[idx];

	  اگر (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
	  {
	    اگر (cross_دھارا)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      اگر (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_انحراف_2 = 0;
	      }
	      ورنہ اگر (o.attach_type())
	      {
		o.y_انحراف_2 = hb_saturate_جوڑو (o.y_انحراف_2, c->font->em_scale_y (v));
		بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    ورنہ اگر (بفر->info[idx].نقاب & kern_نقاب)
	    {
	      خودکار scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_جوڑو (o.x_advance, scaled);
	      o.x_انحراف_2 = hb_saturate_جوڑو (o.x_انحراف_2, scaled);
	    }
	  }
	  ورنہ
	  {
	    اگر (cross_دھارا)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      اگر (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_انحراف_2 = 0;
	      }
	      ورنہ اگر (o.attach_type())
	      {
		o.x_انحراف_2 = hb_saturate_جوڑو (o.x_انحراف_2, c->font->em_scale_x (v));
		بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    ورنہ اگر (بفر->info[idx].نقاب & kern_نقاب)
	    {
	      خودکار scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_جوڑو (o.y_advance, scaled);
	      o.y_انحراف_2 = hb_saturate_جوڑو (o.y_انحراف_2, scaled);
	    }
	  }
	}
      }
    }

    عوامی:
    hb_aat_apply_context_t *c;
    ثابت Kerx_Sub_جدول_Format1 *جدول;
    نجی:
    ثابت Unsized_Array_کا<FWORD> &kernAction;
    غیرعلامتی صحیح ڈھیر[8];
    غیرعلامتی صحیح گہرائی;
    منطقی cross_دھارا;
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    اگر (!c->plan->requested_kerning &&
	!(سرخی.coverage & سرخی.Cross_دھارا))
      واپس جھوٹ;

    driver_context_t dc (یہ, c);

    State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (یہ) &&
			  machine.sanitize (c)));
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوسرا_سیٹ, غیرعلامتی عدد_glyphs) ثابت
  {
    machine.collect_initial_glyphs (پہلا_سیٹ, عدد_glyphs, *یہ);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محفوظ:
  Kern_Sub_جدول_سرخی				سرخی;
  State_جدول<Types, اندراج_ڈیٹا>			machine;
  NNOffset_مقصد<Unsized_Array_کا<FWORD>, HBUINT>	kernAction;
  عوامی:
  DEFINE_سائز_STATIC (Kern_Sub_جدول_سرخی::static_سائز + (State_جدول<Types, اندراج_ڈیٹا>::static_سائز + HBUINT::static_سائز));
};

سانچہ <قسم_نام Kern_Sub_جدول_سرخی>
ساخت Kerx_Sub_جدول_Format2
{
  قسمتعریف قسم_نام Kern_Sub_جدول_سرخی::Types Types;
  قسمتعریف قسم_نام Types::HBUINT HBUINT;

  صحیح get_kerning (hb_codepoint_t بائیں, hb_codepoint_t دائیں,
		   hb_aat_apply_context_t *c) ثابت
  {
    غیرعلامتی صحیح عدد_glyphs = c->sanitizer.get_عدد_glyphs ();
    غیرعلامتی صحیح l = (یہ+بایاں_Class_جدول).get_class (بائیں, عدد_glyphs, 0);
    غیرعلامتی صحیح r = (یہ+دایاں_Class_جدول).get_class (دائیں, عدد_glyphs, 0);

    ثابت Unsized_Array_کا<FWORD> &arrayZ = یہ+array;
    غیرعلامتی صحیح kern_idx = l + r;
    kern_idx = Types::انحراف_مقصد_اشاریہ (kern_idx, یہ, arrayZ.arrayZ);
    ثابت FWORD *v = &arrayZ[kern_idx];
    اگر (unlikely (!v->sanitize (&c->sanitizer))) واپس 0;
    hb_barrier ();

    واپس kerxTupleKern (*v, سرخی.tuple_گنتی_2 (), یہ, c);
  }

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    اگر (!c->plan->requested_kerning)
      return_trace (جھوٹ);

    اگر (سرخی.coverage & سرخی.Backwards)
      return_trace (جھوٹ);

    accelerator_t accel (*یہ, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرخی.coverage & سرخی.Cross_دھارا);
    machine.kern (c->font, c->بفر, c->plan->kern_نقاب);

    return_trace (سچ);
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوسرا_سیٹ, غیرعلامتی عدد_glyphs) ثابت
  {
    (یہ+بایاں_Class_جدول).collect_glyphs (پہلا_سیٹ, عدد_glyphs);
    (یہ+دایاں_Class_جدول).collect_glyphs (دوسرا_سیٹ, عدد_glyphs);
  }

  ساخت accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format2 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format2 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحیح get_kerning (hb_codepoint_t بائیں, hb_codepoint_t دائیں) ثابت
    {
      اگر (!(*c->پہلا_سیٹ)[بائیں] || !(*c->دوسرا_سیٹ)[دائیں]) واپس 0;
      واپس جدول.get_kerning (بائیں, دائیں, c);
    }
  };

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  بایاں_Class_جدول.sanitize (c, یہ) &&
			  دایاں_Class_جدول.sanitize (c, یہ) &&
			  hb_barrier () &&
			  c->check_range (یہ, array)));
  }

  محفوظ:
  Kern_Sub_جدول_سرخی	سرخی;
  HBUINT		صف_چوڑائی;	/* The width, in bytes, of a row in the table. */
  NNOffset_مقصد<قسم_نام Types::ClassTypeWide, HBUINT>
			بایاں_Class_جدول;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffset_مقصد<قسم_نام Types::ClassTypeWide, HBUINT>
			دایاں_Class_جدول;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffset_مقصد<Unsized_Array_کا<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  عوامی:
  DEFINE_سائز_STATIC (Kern_Sub_جدول_سرخی::static_سائز + 4 * حجم (HBUINT));
};

سانچہ <قسم_نام Kern_Sub_جدول_سرخی>
ساخت Kerx_Sub_جدول_Format4
{
  قسمتعریف ExtendedTypes Types;

  ساخت اندراج_ڈیٹا
  {
    HBUINT16	ankr_Action_اشاریہ;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    عوامی:
    DEFINE_سائز_STATIC (2);
  };

  شمار Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  منطقی is_action_initiable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    واپس (اندراج_2.flags & Mark);
  }
  منطقی is_actionable (ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2) ثابت
  {
    واپس اندراج_2.ڈیٹا.ankr_Action_اشاریہ != 0xFFFF;
  }

  ساخت driver_context_t
  {
    ساکن constexpr منطقی in_place = سچ;
    شمار Sub_جدول_Flags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      غیر_مستعمل		= 0x3F000000,	/* Unused - must be zero. */
      انحراف_2		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ثابت Kerx_Sub_جدول_Format4 *جدول_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	جدول (جدول_2),
	action_type ((جدول->flags & ActionType) >> 30),
	ankr_ڈیٹا ((HBUINT16 *) ((ثابت حرف *) &جدول->machine + (جدول->flags & انحراف_2))),
	mark_سیٹ (جھوٹ),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    خالی transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> *driver,
		     ثابت اندراج_3<اندراج_ڈیٹا> &اندراج_2)
    {
      اگر (mark_سیٹ && اندراج_2.ڈیٹا.ankr_Action_اشاریہ != 0xFFFF && بفر->idx < بفر->len)
      {
	hb_glyph_position_t &o = بفر->cur_pos();
	انتخاب (action_type)
	{
	  حالت 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *ڈیٹا = &ankr_ڈیٹا[اندراج_2.ڈیٹا.ankr_Action_اشاریہ * 2];
	    اگر (!c->sanitizer.check_array (ڈیٹا, 2)) واپس;
	    hb_barrier ();
	    غیرعلامتی صحیح mark_Control_نقطہ = *ڈیٹا++;
	    غیرعلامتی صحیح curr_Control_نقطہ = *ڈیٹا++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    اگر (!c->font->get_glyph_contour_نقطہ_for_origin (c->بفر->info[mark].codepoint,
							      mark_Control_نقطہ,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_نقطہ_for_origin (c->بفر->cur ().codepoint,
							      curr_Control_نقطہ,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      واپس;

	    o.x_انحراف_2 = hb_saturate_sub (markX, currX);
	    o.y_انحراف_2 = hb_saturate_sub (markY, currY);
	  }
	  توڑو;

	  حالت 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *ڈیٹا = &ankr_ڈیٹا[اندراج_2.ڈیٹا.ankr_Action_اشاریہ * 2];
	    اگر (!c->sanitizer.check_array (ڈیٹا, 2)) واپس;
	    hb_barrier ();
	    غیرعلامتی صحیح mark_Anchor_نقطہ = *ڈیٹا++;
	    غیرعلامتی صحیح curr_Anchor_نقطہ = *ڈیٹا++;
	    ثابت Anchor &markAnchor = c->ankr_جدول->get_anchor (c->بفر->info[mark].codepoint,
								  mark_Anchor_نقطہ,
								  c->sanitizer.get_عدد_glyphs ());
	    ثابت Anchor &currAnchor = c->ankr_جدول->get_anchor (c->بفر->cur ().codepoint,
								  curr_Anchor_نقطہ,
								  c->sanitizer.get_عدد_glyphs ());

	    o.x_انحراف_2 = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_انحراف_2 = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  توڑو;

	  حالت 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ثابت FWORD *ڈیٹا = (ثابت FWORD *) &ankr_ڈیٹا[اندراج_2.ڈیٹا.ankr_Action_اشاریہ * 4];
	    اگر (!c->sanitizer.check_array (ڈیٹا, 4)) واپس;
	    hb_barrier ();
	    صحیح markX = *ڈیٹا++;
	    صحیح markY = *ڈیٹا++;
	    صحیح currX = *ڈیٹا++;
	    صحیح currY = *ڈیٹا++;

	    o.x_انحراف_2 = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_انحراف_2 = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  توڑو;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (صحیح) mark - (صحیح) بفر->idx;
	اگر (c->بفر_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      اگر (اندراج_2.flags & Mark)
      {
	mark_سیٹ = سچ;
	mark = بفر->idx;
      }
    }

    عوامی:
    hb_aat_apply_context_t *c;
    ثابت Kerx_Sub_جدول_Format4 *جدول;
    نجی:
    غیرعلامتی صحیح action_type;
    ثابت HBUINT16 *ankr_ڈیٹا;
    منطقی mark_سیٹ;
    غیرعلامتی صحیح mark;
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    driver_context_t dc (یہ, c);

    State_جدول_Driver<Types, اندراج_ڈیٹا, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (یہ) &&
			  machine.sanitize (c)));
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوسرا_سیٹ, غیرعلامتی عدد_glyphs) ثابت
  {
    machine.collect_initial_glyphs (پہلا_سیٹ, عدد_glyphs, *یہ);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محفوظ:
  Kern_Sub_جدول_سرخی		سرخی;
  State_جدول<Types, اندراج_ڈیٹا>	machine;
  HBUINT32			flags;
  عوامی:
  DEFINE_سائز_STATIC (Kern_Sub_جدول_سرخی::static_سائز + (State_جدول<Types, اندراج_ڈیٹا>::static_سائز + HBUINT32::static_سائز));
};

سانچہ <قسم_نام Kern_Sub_جدول_سرخی>
ساخت Kerx_Sub_جدول_Format6
{
  شمار Flags
  {
    اقدار_Are_Long	= 0x00000001,
  };

  منطقی is_long () ثابت { واپس flags & اقدار_Are_Long; }

  صحیح get_kerning (hb_codepoint_t بائیں, hb_codepoint_t دائیں,
		   hb_aat_apply_context_t *c) ثابت
  {
    غیرعلامتی صحیح عدد_glyphs = c->sanitizer.get_عدد_glyphs ();
    اگر (is_long ())
    {
      ثابت خودکار &t = u.l;
      غیرعلامتی صحیح l = (یہ+t.صف_اشاریہ_جدول).get_قدر_or_null (بائیں, عدد_glyphs);
      غیرعلامتی صحیح r = (یہ+t.عمود_اشاریہ_جدول).get_قدر_or_null (دائیں, عدد_glyphs);
      غیرعلامتی صحیح انحراف = l + r;
      اگر (unlikely (انحراف < l)) واپس 0; /* Addition overflow. */
      اگر (unlikely (hb_unsigned_mul_overflows (انحراف, حجم (FWORD32)))) واپس 0;
      ثابت FWORD32 *v = &Struct_At_انحراف<FWORD32> (&(یہ+t.array), انحراف * حجم (FWORD32));
      اگر (unlikely (!v->sanitize (&c->sanitizer))) واپس 0;
      hb_barrier ();
      واپس kerxTupleKern (*v, سرخی.tuple_گنتی_2 (), &(یہ+vector), c);
    }
    ورنہ
    {
      ثابت خودکار &t = u.s;
      غیرعلامتی صحیح l = (یہ+t.صف_اشاریہ_جدول).get_قدر_or_null (بائیں, عدد_glyphs);
      غیرعلامتی صحیح r = (یہ+t.عمود_اشاریہ_جدول).get_قدر_or_null (دائیں, عدد_glyphs);
      غیرعلامتی صحیح انحراف = l + r;
      ثابت FWORD *v = &Struct_At_انحراف<FWORD> (&(یہ+t.array), انحراف * حجم (FWORD));
      اگر (unlikely (!v->sanitize (&c->sanitizer))) واپس 0;
      hb_barrier ();
      واپس kerxTupleKern (*v, سرخی.tuple_گنتی_2 (), &(یہ+vector), c);
    }
  }

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    اگر (!c->plan->requested_kerning)
      return_trace (جھوٹ);

    اگر (سرخی.coverage & سرخی.Backwards)
      return_trace (جھوٹ);

    accelerator_t accel (*یہ, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرخی.coverage & سرخی.Cross_دھارا);
    machine.kern (c->font, c->بفر, c->plan->kern_نقاب);

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.صف_اشاریہ_جدول.sanitize (c, یہ) &&
			     u.l.عمود_اشاریہ_جدول.sanitize (c, یہ) &&
			     c->check_range (یہ, u.l.array)
			   ) : (
			     u.s.صف_اشاریہ_جدول.sanitize (c, یہ) &&
			     u.s.عمود_اشاریہ_جدول.sanitize (c, یہ) &&
			     c->check_range (یہ, u.s.array)
			   )) &&
			  (سرخی.tuple_گنتی_2 () == 0 ||
			   c->check_range (یہ, vector))));
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوسرا_سیٹ, غیرعلامتی عدد_glyphs) ثابت
  {
    اگر (is_long ())
    {
      ثابت خودکار &t = u.l;
      (یہ+t.صف_اشاریہ_جدول).collect_glyphs (پہلا_سیٹ, عدد_glyphs);
      (یہ+t.عمود_اشاریہ_جدول).collect_glyphs (دوسرا_سیٹ, عدد_glyphs);
    }
    ورنہ
    {
      ثابت خودکار &t = u.s;
      (یہ+t.صف_اشاریہ_جدول).collect_glyphs (پہلا_سیٹ, عدد_glyphs);
      (یہ+t.عمود_اشاریہ_جدول).collect_glyphs (دوسرا_سیٹ, عدد_glyphs);
    }
  }

  ساخت accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format6 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format6 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحیح get_kerning (hb_codepoint_t بائیں, hb_codepoint_t دائیں) ثابت
    {
      اگر (!(*c->پہلا_سیٹ)[بائیں] || !(*c->دوسرا_سیٹ)[دائیں]) واپس 0;
      واپس جدول.get_kerning (بائیں, دائیں, c);
    }
  };

  محفوظ:
  Kern_Sub_جدول_سرخی		سرخی;
  HBUINT32			flags;
  HBUINT16			صف_گنتی_2;
  HBUINT16			عمود_گنتی;
  اتحاد U
  {
    ساخت Long
    {
      NNOffset32_مقصد<Lookup<HBUINT32>>		صف_اشاریہ_جدول;
      NNOffset32_مقصد<Lookup<HBUINT32>>		عمود_اشاریہ_جدول;
      NNOffset32_مقصد<Unsized_Array_کا<FWORD32>>	array;
    } l;
    ساخت Short
    {
      NNOffset32_مقصد<Lookup<HBUINT16>>		صف_اشاریہ_جدول;
      NNOffset32_مقصد<Lookup<HBUINT16>>		عمود_اشاریہ_جدول;
      NNOffset32_مقصد<Unsized_Array_کا<FWORD>>	array;
    } s;
  } u;
  NNOffset32_مقصد<Unsized_Array_کا<FWORD>>	vector;
  عوامی:
  DEFINE_سائز_STATIC (Kern_Sub_جدول_سرخی::static_سائز + 24);
};


ساخت Kerx_Sub_جدول_سرخی
{
  قسمتعریف ExtendedTypes Types;

  غیرعلامتی   tuple_گنتی_2 () ثابت { واپس tuple_گنتی; }
  منطقی     is_horizontal () ثابت { واپس !(coverage & Vertical); }

  شمار Coverage
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

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBUINT32	لمبائی;
  HBUINT32	coverage;
  HBUINT32	tuple_گنتی;
  عوامی:
  DEFINE_سائز_STATIC (12);
};

ساخت Kerx_Sub_جدول
{
  دوست ساخت kerx;

  size_t get_سائز () ثابت { واپس u.سرخی.لمبائی; }
  غیرعلامتی صحیح get_type () ثابت { واپس u.سرخی.coverage & u.سرخی.SubtableType; }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    غیرعلامتی صحیح subtable_type = get_type ();
    TRACE_DISPATCH (یہ, subtable_type);
    انتخاب (subtable_type) {
    حالت 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    حالت 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالت 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    طےشدہ:	return_trace (c->default_return_قدر ());
    }
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &پہلا_سیٹ, سیٹ_t &دوسرا_سیٹ, غیرعلامتی عدد_glyphs) ثابت
  {
    غیرعلامتی صحیح subtable_type = get_type ();
    انتخاب (subtable_type) {
    حالت 0:	hb_barrier (); u.format0.collect_glyphs (پہلا_سیٹ, دوسرا_سیٹ, عدد_glyphs); واپس;
    حالت 1:	hb_barrier (); u.format1.collect_glyphs (پہلا_سیٹ, دوسرا_سیٹ, عدد_glyphs); واپس;
    حالت 2:	hb_barrier (); u.format2.collect_glyphs (پہلا_سیٹ, دوسرا_سیٹ, عدد_glyphs); واپس;
    حالت 4:	hb_barrier (); u.format4.collect_glyphs (پہلا_سیٹ, دوسرا_سیٹ, عدد_glyphs); واپس;
    حالت 6:	hb_barrier (); u.format6.collect_glyphs (پہلا_سیٹ, دوسرا_سیٹ, عدد_glyphs); واپس;
    طےشدہ:	واپس;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!(u.سرخی.sanitize (c) &&
	  hb_barrier () &&
	  u.سرخی.لمبائی >= u.سرخی.static_سائز &&
	  c->check_range (یہ, u.سرخی.لمبائی)))
      return_trace (جھوٹ);

    return_trace (dispatch (c));
  }

  عوامی:
  اتحاد {
  Kerx_Sub_جدول_سرخی				سرخی;
  Kerx_Sub_جدول_Format0<Kerx_Sub_جدول_سرخی>	format0;
  Kerx_Sub_جدول_Format1<Kerx_Sub_جدول_سرخی>	format1;
  Kerx_Sub_جدول_Format2<Kerx_Sub_جدول_سرخی>	format2;
  Kerx_Sub_جدول_Format4<Kerx_Sub_جدول_سرخی>	format4;
  Kerx_Sub_جدول_Format6<Kerx_Sub_جدول_سرخی>	format6;
  } u;
  عوامی:
  DEFINE_سائز_MIN (12);
};


/*
 * The 'kerx' Table
 */

ساخت kern_subtable_accelerator_ڈیٹا_t
{
  hb_ثنائی_ہندسہ_سیٹ_t پہلا_سیٹ;
  hb_ثنائی_ہندسہ_سیٹ_t دوسرا_سیٹ;
  تغیرپذیر hb_aat_class_cache_t class_cache;
};

ساخت kern_accelerator_ڈیٹا_t
{
  hb_سمتیہ_t<kern_subtable_accelerator_ڈیٹا_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

سانچہ <قسم_نام T>
ساخت Kerx_جدول
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت T* thiz () ثابت { واپس static_cast<ثابت T *> (یہ); }

  منطقی has_state_machine () ثابت
  {
    قسمتعریف قسم_نام T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    غیرعلامتی صحیح گنتی = thiz()->جدول_گنتی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (st->get_type () == 1)
	واپس سچ;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<Sub_جدول> (*st);
    }
    واپس جھوٹ;
  }

  منطقی has_cross_دھارا () ثابت
  {
    قسمتعریف قسم_نام T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    غیرعلامتی صحیح گنتی = thiz()->جدول_گنتی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (st->u.سرخی.coverage & st->u.سرخی.Cross_دھارا)
	واپس سچ;
      st = &StructAfter<Sub_جدول> (*st);
    }
    واپس جھوٹ;
  }

  صحیح get_h_kerning (hb_codepoint_t بائیں, hb_codepoint_t دائیں) ثابت
  {
    قسمتعریف قسم_نام T::Sub_جدول Sub_جدول;

    صحیح v = 0;
    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    غیرعلامتی صحیح گنتی = thiz()->جدول_گنتی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر ((st->u.سرخی.coverage & (st->u.سرخی.Variation | st->u.سرخی.Cross_دھارا)) ||
	  !st->u.سرخی.is_horizontal ())
	جاری;
      v += st->get_kerning (بائیں, دائیں);
      st = &StructAfter<Sub_جدول> (*st);
    }
    واپس v;
  }

  منطقی apply (AAT::hb_aat_apply_context_t *c,
	      ثابت kern_accelerator_ڈیٹا_t &accel_ڈیٹا) ثابت
  {
    c->بفر->unsafe_مقصد_concat ();

    c->تیاری_بفر_glyph_سیٹ ();

    قسمتعریف قسم_نام T::Sub_جدول Sub_جدول;

    منطقی ret = جھوٹ;
    منطقی نظر_آیا_Cross_دھارا = جھوٹ;
    c->سیٹ_lookup_اشاریہ (0);
    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    غیرعلامتی صحیح گنتی = thiz()->جدول_گنتی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      منطقی reverse;

      خودکار &subtable_accel = accel_ڈیٹا.subtable_accels[i];

      اگر (!T::Types::extended && (st->u.سرخی.coverage & st->u.سرخی.Variation))
	جاؤ skip;

      اگر (HB_DIRECTION_IS_HORIZONTAL (c->بفر->props.direction) != st->u.سرخی.is_horizontal ())
	جاؤ skip;

      c->پہلا_سیٹ = &subtable_accel.پہلا_سیٹ;
      c->دوسرا_سیٹ = &subtable_accel.دوسرا_سیٹ;
      c->machine_class_cache = &subtable_accel.class_cache;

      اگر (!c->بفر_intersects_machine ())
      {
	(خالی) c->بفر->پیغام (c->font, "skipped subtable %u because no glyph matches", c->lookup_اشاریہ);
	جاؤ skip;
      }

      reverse = منطقی (st->u.سرخی.coverage & st->u.سرخی.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->بفر->props.direction);

      اگر (!c->بفر->پیغام (c->font, "start subtable %u", c->lookup_اشاریہ))
	جاؤ skip;

      اگر (!نظر_آیا_Cross_دھارا &&
	  (st->u.سرخی.coverage & st->u.سرخی.Cross_دھارا))
      {
	/* Attach all glyphs into a chain. */
	نظر_آیا_Cross_دھارا = سچ;
	hb_glyph_position_t *pos = c->بفر->pos;
	غیرعلامتی صحیح گنتی = c->بفر->len;
	برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->بفر->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      اگر (reverse != c->بفر_is_reversed)
        c->الٹ_بفر ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < گنتی - 1 ? st : (ثابت Sub_جدول *) خالی_اشاریہ);
	ret |= st->dispatch (c);
      }

      (خالی) c->بفر->پیغام (c->font, "end subtable %u", c->lookup_اشاریہ);

    skip:
      st = &StructAfter<Sub_جدول> (*st);
      c->سیٹ_lookup_اشاریہ (c->lookup_اشاریہ + 1);
    }
    اگر (c->بفر_is_reversed)
      c->الٹ_بفر ();

    واپس ret;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!(thiz()->نسخہ.sanitize (c) &&
		    hb_barrier () &&
		    (غیرعلامتی) thiz()->نسخہ >= (غیرعلامتی) T::min_نسخہ &&
		    thiz()->جدول_گنتی.sanitize (c))))
      return_trace (جھوٹ);

    قسمتعریف قسم_نام T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    غیرعلامتی صحیح گنتی = thiz()->جدول_گنتی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (unlikely (!st->u.سرخی.sanitize (c)))
	return_trace (جھوٹ);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < گنتی - 1 ? st : (ثابت Sub_جدول *) خالی_اشاریہ);

      اگر (unlikely (!st->sanitize (c)))
	return_trace (جھوٹ);

      st = &StructAfter<Sub_جدول> (*st);
    }

    غیرعلامتی major_نسخہ = thiz()->نسخہ;
    اگر (حجم (thiz()->نسخہ) == 4)
      major_نسخہ = major_نسخہ >> 16;
    اگر (major_نسخہ >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) st;
      اگر (!coverage->sanitize (c, گنتی))
        return_trace (جھوٹ);
    }

    return_trace (سچ);
  }

  kern_accelerator_ڈیٹا_t create_accelerator_ڈیٹا (غیرعلامتی عدد_glyphs) ثابت
  {
    kern_accelerator_ڈیٹا_t accel_ڈیٹا;

    قسمتعریف قسم_نام T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پہلا_Sub_جدول;
    غیرعلامتی صحیح گنتی = thiz()->جدول_گنتی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      خودکار &subtable_accel = *accel_ڈیٹا.subtable_accels.push ();
      اگر (unlikely (accel_ڈیٹا.subtable_accels.اندر_error ()))
	  واپس accel_ڈیٹا;

      st->collect_glyphs (subtable_accel.پہلا_سیٹ, subtable_accel.دوسرا_سیٹ, عدد_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<Sub_جدول> (*st);
    }

    واپس accel_ڈیٹا;
  }

  ساخت accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      یہ->جدول = sc.reference_جدول<T> (face);
      یہ->accel_ڈیٹا = یہ->جدول->create_accelerator_ڈیٹا (face->get_عدد_glyphs ());
    }
    ~accelerator_t ()
    {
      یہ->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { واپس جدول.get_blob (); }

    منطقی apply (AAT::hb_aat_apply_context_t *c) ثابت
    {
      واپس جدول->apply (c, accel_ڈیٹا);
    }

    hb_blob_ptr_t<T> جدول;
    kern_accelerator_ڈیٹا_t accel_ڈیٹا;
    hb_aat_scratch_t scratch;
  };
};

ساخت kerx : Kerx_جدول<kerx>
{
  دوست ساخت Kerx_جدول<kerx>;

  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_AAT_لیبل_kerx;
  ساکن constexpr غیرعلامتی min_نسخہ = 2u;

  قسمتعریف Kerx_Sub_جدول_سرخی Sub_جدول_سرخی;
  قسمتعریف Sub_جدول_سرخی::Types Types;
  قسمتعریف Kerx_Sub_جدول Sub_جدول;

  منطقی has_ڈیٹا () ثابت { واپس نسخہ; }

  محفوظ:
  HBUINT16	نسخہ;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	غیر_مستعمل_2;		/* Set to 0. */
  HBUINT32	جدول_گنتی;	/* The number of subtables included in the extended kerning
				 * table. */
  Sub_جدول	پہلا_Sub_جدول;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  عوامی:
  DEFINE_سائز_MIN (8);
};

ساخت kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#اگر_ختم /* HB_AAT_LAYOUT_KERX_TABLE_HH */
