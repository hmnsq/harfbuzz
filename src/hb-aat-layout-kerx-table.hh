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

#جيڪڏهن_وصف_نه_ٿيل HB_AAT_LAYOUT_KERX_جدول_HH
#وصف_ڪريو HB_AAT_LAYOUT_KERX_جدول_HH

#ضمن "hb-kern.hh"
#ضمن "hb-aat-layout-ankr-table.hh"
#ضمن "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#وصف_ڪريو HB_AAT_ٽيگ_kerx HB_ٽيگ('k','e','r','x')


نطاق AAT {

استعمال نطاق OT;


ساكن مضمن صحيح
kerxTupleKern (صحيح value,
	       بنانشان صحيح tuple_ڳڻپ,
	       ثابت فراغ *بنياد,
	       hb_aat_apply_context_t *c)
{
  إذا (likely (!tuple_ڳڻپ || !c)) أرجع value;

  بنانشان صحيح وٿي = value;
  ثابت FWORD *pv = &Struct_At_وٿي<FWORD> (بنياد, وٿي);
  إذا (unlikely (!c->sanitizer.check_array (pv, tuple_ڳڻپ))) أرجع 0;
  hb_barrier ();
  أرجع *pv;
}


بنية hb_glyph_pair_t
{
  hb_codepoint_t کاٻي;
  hb_codepoint_t ساڄي;
};

بنية KernPair
{
  صحيح get_kerning () ثابت { أرجع value; }

  صحيح cmp (ثابت hb_glyph_pair_t &o) ثابت
  {
    صحيح ret = کاٻي.cmp (o.کاٻي);
    إذا (ret) أرجع ret;
    أرجع ساڄي.cmp (o.ساڄي);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBGlyphID16	کاٻي;
  HBGlyphID16	ساڄي;
  FWORD		value;
  عام:
  DEFINE_سائيز_STATIC (6);
};

سانچو <قسم_نالو Kern_Sub_جدول_سرو>
بنية Kerx_Sub_جدول_Format0
{
  صحيح get_kerning (hb_codepoint_t کاٻي, hb_codepoint_t ساڄي,
		   hb_aat_apply_context_t *c = خالي_اشارو) ثابت
  {
    hb_glyph_pair_t pair = {کاٻي, ساڄي};
    صحيح v = جوڙا.bsearch (pair).get_kerning ();
    أرجع kerxTupleKern (v, سرو.tuple_ڳڻپ_2 (), هذا, c);
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرو.coverage & سرو.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرو.coverage & سرو.Cross_وهڪرو);
    machine.kern (c->font, c->بفر, c->plan->kern_نقاب);

    return_trace (سچ);
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &پهريون_سيٽ, سيٽ_t &ٻيو_سيٽ, بنانشان انگ_glyphs) ثابت
  {
    لكل (ثابت KernPair& pair : جوڙا)
    {
      پهريون_سيٽ.جوڙيو (pair.کاٻي);
      ٻيو_سيٽ.جوڙيو (pair.ساڄي);
    }
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format0 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format0 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t کاٻي, hb_codepoint_t ساڄي) ثابت
    {
      إذا (!(*c->پهريون_سيٽ)[کاٻي] || !(*c->ٻيو_سيٽ)[ساڄي]) أرجع 0;
      أرجع جدول.get_kerning (کاٻي, ساڄي, c);
    }
  };


  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (جوڙا.sanitize (c)));
  }

  محمي:
  Kern_Sub_جدول_سرو	سرو;
  Bin_ڳولا_Array_جو<KernPair, قسم_نالو Kern_Sub_جدول_سرو::Types::HBUINT>
			جوڙا;	/* Sorted kern records. */
  عام:
  DEFINE_سائيز_ARRAY (Kern_Sub_جدول_سرو::static_سائيز + 16, جوڙا);
};


سانچو <منطقي extended>
بنية Format1_داخلا;

سانچو <>
بنية Format1_داخلا<سچ>
{
  تعداد Flags
  {
    ڌڪو		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  بنية داخلا_معلومات
  {
    HBUINT16	kern_Action_اشاريو;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    عام:
    DEFINE_سائيز_STATIC (2);
  };

  ساكن منطقي initiateAction (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.flags & ڌڪو; }

  ساكن منطقي performAction (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.معلومات.kern_Action_اشاريو != 0xFFFF; }

  ساكن بنانشان صحيح kern_Action_اشاريو (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.معلومات.kern_Action_اشاريو; }
};
سانچو <>
بنية Format1_داخلا<خطأ>
{
  تعداد Flags
  {
    ڌڪو		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    وٿي_2		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  تعريفنوع فراغ داخلا_معلومات;

  ساكن منطقي initiateAction (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.flags & ڌڪو; }

  ساكن منطقي performAction (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.flags & وٿي_2; }

  ساكن بنانشان صحيح kern_Action_اشاريو (ثابت داخلا_4<داخلا_معلومات> &داخلا)
  { أرجع داخلا.flags & وٿي_2; }
};

سانچو <قسم_نالو Kern_Sub_جدول_سرو>
بنية Kerx_Sub_جدول_Format1
{
  تعريفنوع قسم_نالو Kern_Sub_جدول_سرو::Types Types;
  تعريفنوع قسم_نالو Types::HBUINT HBUINT;

  تعريفنوع Format1_داخلا<Types::extended> Format1_داخلا_T;
  تعريفنوع قسم_نالو Format1_داخلا_T::داخلا_معلومات داخلا_معلومات;

  تعداد Flags
  {
    DontAdvance	= Format1_داخلا_T::DontAdvance,
  };

  منطقي is_action_initiable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    أرجع Format1_داخلا_T::initiateAction (داخلا);
  }
  منطقي is_actionable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    أرجع Format1_داخلا_T::performAction (داخلا);
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
	اونهائي (0),
	cross_وهڪرو (جدول->سرو.coverage & جدول->سرو.Cross_وهڪرو) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, داخلا_معلومات, Flags> *driver,
		     ثابت داخلا_4<داخلا_معلومات> &داخلا)
    {
      بنانشان صحيح flags = داخلا.flags;

      إذا (flags & Format1_داخلا_T::Reset)
	اونهائي = 0;

      إذا (flags & Format1_داخلا_T::ڌڪو)
      {
	إذا (likely (اونهائي < ARRAY_ڊيگهائي (ڍير)))
	  ڍير[اونهائي++] = بفر->idx;
	وإلا
	  اونهائي = 0; /* Probably not what CoreText does, but better? */
      }

      إذا (Format1_داخلا_T::performAction (داخلا) && اونهائي)
      {
	بنانشان صحيح tuple_ڳڻپ_2 = hb_max (1u, جدول->سرو.tuple_ڳڻپ_2 ());

	بنانشان صحيح kern_idx = Format1_داخلا_T::kern_Action_اشاريو (داخلا);
	kern_idx = Types::byte_وٿي_منزل_اشاريو (kern_idx, &جدول->machine, kernAction.arrayZ);
	ثابت FWORD *actions = &kernAction[kern_idx];
	إذا (!c->sanitizer.check_array (actions, اونهائي, tuple_ڳڻپ_2))
	{
	  اونهائي = 0;
	  أرجع;
	}
	hb_barrier ();

	hb_نقاب_t kern_نقاب = c->plan->kern_نقاب;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	منطقي آخري = خطأ;
	طالما (!آخري && اونهائي)
	{
	  بنانشان صحيح idx = ڍير[--اونهائي];
	  صحيح v = *actions;
	  actions += tuple_ڳڻپ_2;
	  إذا (idx >= بفر->len) استمر;

	  /* "The end of the list is marked by an odd value..." */
	  آخري = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = بفر->pos[idx];

	  إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
	  {
	    إذا (cross_وهڪرو)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      إذا (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_وٿي_2 = 0;
	      }
	      وإلا إذا (o.attach_type())
	      {
		o.y_وٿي_2 = hb_saturate_جوڙيو (o.y_وٿي_2, c->font->em_scale_y (v));
		بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وإلا إذا (بفر->info[idx].نقاب & kern_نقاب)
	    {
	      خودڪار scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_جوڙيو (o.x_advance, scaled);
	      o.x_وٿي_2 = hb_saturate_جوڙيو (o.x_وٿي_2, scaled);
	    }
	  }
	  وإلا
	  {
	    إذا (cross_وهڪرو)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      إذا (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_وٿي_2 = 0;
	      }
	      وإلا إذا (o.attach_type())
	      {
		o.x_وٿي_2 = hb_saturate_جوڙيو (o.x_وٿي_2, c->font->em_scale_x (v));
		بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وإلا إذا (بفر->info[idx].نقاب & kern_نقاب)
	    {
	      خودڪار scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_جوڙيو (o.y_advance, scaled);
	      o.y_وٿي_2 = hb_saturate_جوڙيو (o.y_وٿي_2, scaled);
	    }
	  }
	}
      }
    }

    عام:
    hb_aat_apply_context_t *c;
    ثابت Kerx_Sub_جدول_Format1 *جدول;
    خاص:
    ثابت Unsized_Array_جو<FWORD> &kernAction;
    بنانشان صحيح ڍير[8];
    بنانشان صحيح اونهائي;
    منطقي cross_وهڪرو;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning &&
	!(سرو.coverage & سرو.Cross_وهڪرو))
      أرجع خطأ;

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, داخلا_معلومات, Flags> driver (machine, c->font->face);

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

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &پهريون_سيٽ, سيٽ_t &ٻيو_سيٽ, بنانشان انگ_glyphs) ثابت
  {
    machine.collect_initial_glyphs (پهريون_سيٽ, انگ_glyphs, *هذا);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محمي:
  Kern_Sub_جدول_سرو				سرو;
  State_جدول<Types, داخلا_معلومات>			machine;
  NNOffset_منزل<Unsized_Array_جو<FWORD>, HBUINT>	kernAction;
  عام:
  DEFINE_سائيز_STATIC (Kern_Sub_جدول_سرو::static_سائيز + (State_جدول<Types, داخلا_معلومات>::static_سائيز + HBUINT::static_سائيز));
};

سانچو <قسم_نالو Kern_Sub_جدول_سرو>
بنية Kerx_Sub_جدول_Format2
{
  تعريفنوع قسم_نالو Kern_Sub_جدول_سرو::Types Types;
  تعريفنوع قسم_نالو Types::HBUINT HBUINT;

  صحيح get_kerning (hb_codepoint_t کاٻي, hb_codepoint_t ساڄي,
		   hb_aat_apply_context_t *c) ثابت
  {
    بنانشان صحيح انگ_glyphs = c->sanitizer.get_انگ_glyphs ();
    بنانشان صحيح l = (هذا+کاٻو_Class_جدول).get_class (کاٻي, انگ_glyphs, 0);
    بنانشان صحيح r = (هذا+ساڄو_Class_جدول).get_class (ساڄي, انگ_glyphs, 0);

    ثابت Unsized_Array_جو<FWORD> &arrayZ = هذا+array;
    بنانشان صحيح kern_idx = l + r;
    kern_idx = Types::وٿي_منزل_اشاريو (kern_idx, هذا, arrayZ.arrayZ);
    ثابت FWORD *v = &arrayZ[kern_idx];
    إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
    hb_barrier ();

    أرجع kerxTupleKern (*v, سرو.tuple_ڳڻپ_2 (), هذا, c);
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرو.coverage & سرو.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرو.coverage & سرو.Cross_وهڪرو);
    machine.kern (c->font, c->بفر, c->plan->kern_نقاب);

    return_trace (سچ);
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &پهريون_سيٽ, سيٽ_t &ٻيو_سيٽ, بنانشان انگ_glyphs) ثابت
  {
    (هذا+کاٻو_Class_جدول).collect_glyphs (پهريون_سيٽ, انگ_glyphs);
    (هذا+ساڄو_Class_جدول).collect_glyphs (ٻيو_سيٽ, انگ_glyphs);
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format2 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format2 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t کاٻي, hb_codepoint_t ساڄي) ثابت
    {
      إذا (!(*c->پهريون_سيٽ)[کاٻي] || !(*c->ٻيو_سيٽ)[ساڄي]) أرجع 0;
      أرجع جدول.get_kerning (کاٻي, ساڄي, c);
    }
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  کاٻو_Class_جدول.sanitize (c, هذا) &&
			  ساڄو_Class_جدول.sanitize (c, هذا) &&
			  hb_barrier () &&
			  c->check_range (هذا, array)));
  }

  محمي:
  Kern_Sub_جدول_سرو	سرو;
  HBUINT		صف_ويڪر;	/* The width, in bytes, of a row in the table. */
  NNOffset_منزل<قسم_نالو Types::ClassTypeWide, HBUINT>
			کاٻو_Class_جدول;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffset_منزل<قسم_نالو Types::ClassTypeWide, HBUINT>
			ساڄو_Class_جدول;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffset_منزل<Unsized_Array_جو<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  عام:
  DEFINE_سائيز_STATIC (Kern_Sub_جدول_سرو::static_سائيز + 4 * حجم (HBUINT));
};

سانچو <قسم_نالو Kern_Sub_جدول_سرو>
بنية Kerx_Sub_جدول_Format4
{
  تعريفنوع ExtendedTypes Types;

  بنية داخلا_معلومات
  {
    HBUINT16	ankr_Action_اشاريو;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    عام:
    DEFINE_سائيز_STATIC (2);
  };

  تعداد Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  منطقي is_action_initiable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    أرجع (داخلا.flags & Mark);
  }
  منطقي is_actionable (ثابت داخلا_4<داخلا_معلومات> &داخلا) ثابت
  {
    أرجع داخلا.معلومات.ankr_Action_اشاريو != 0xFFFF;
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = سچ;
    تعداد Sub_جدول_Flags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      اڻ_استعمال_ٿيل		= 0x3F000000,	/* Unused - must be zero. */
      وٿي_2		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ثابت Kerx_Sub_جدول_Format4 *جدول_2,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	جدول (جدول_2),
	action_type ((جدول->flags & ActionType) >> 30),
	ankr_معلومات ((HBUINT16 *) ((ثابت حرف *) &جدول->machine + (جدول->flags & وٿي_2))),
	mark_سيٽ (خطأ),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_بفر_t *بفر,
		     State_جدول_Driver<Types, داخلا_معلومات, Flags> *driver,
		     ثابت داخلا_4<داخلا_معلومات> &داخلا)
    {
      إذا (mark_سيٽ && داخلا.معلومات.ankr_Action_اشاريو != 0xFFFF && بفر->idx < بفر->len)
      {
	hb_glyph_position_t &o = بفر->cur_pos();
	بدل (action_type)
	{
	  حالة 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *معلومات = &ankr_معلومات[داخلا.معلومات.ankr_Action_اشاريو * 2];
	    إذا (!c->sanitizer.check_array (معلومات, 2)) أرجع;
	    hb_barrier ();
	    بنانشان صحيح mark_Control_نقطو = *معلومات++;
	    بنانشان صحيح curr_Control_نقطو = *معلومات++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    إذا (!c->font->get_glyph_contour_نقطو_for_origin (c->بفر->info[mark].codepoint,
							      mark_Control_نقطو,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_نقطو_for_origin (c->بفر->cur ().codepoint,
							      curr_Control_نقطو,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      أرجع;

	    o.x_وٿي_2 = hb_saturate_sub (markX, currX);
	    o.y_وٿي_2 = hb_saturate_sub (markY, currY);
	  }
	  اكسر;

	  حالة 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *معلومات = &ankr_معلومات[داخلا.معلومات.ankr_Action_اشاريو * 2];
	    إذا (!c->sanitizer.check_array (معلومات, 2)) أرجع;
	    hb_barrier ();
	    بنانشان صحيح mark_Anchor_نقطو = *معلومات++;
	    بنانشان صحيح curr_Anchor_نقطو = *معلومات++;
	    ثابت Anchor &markAnchor = c->ankr_جدول->get_anchor (c->بفر->info[mark].codepoint,
								  mark_Anchor_نقطو,
								  c->sanitizer.get_انگ_glyphs ());
	    ثابت Anchor &currAnchor = c->ankr_جدول->get_anchor (c->بفر->cur ().codepoint,
								  curr_Anchor_نقطو,
								  c->sanitizer.get_انگ_glyphs ());

	    o.x_وٿي_2 = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_وٿي_2 = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  اكسر;

	  حالة 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ثابت FWORD *معلومات = (ثابت FWORD *) &ankr_معلومات[داخلا.معلومات.ankr_Action_اشاريو * 4];
	    إذا (!c->sanitizer.check_array (معلومات, 4)) أرجع;
	    hb_barrier ();
	    صحيح markX = *معلومات++;
	    صحيح markY = *معلومات++;
	    صحيح currX = *معلومات++;
	    صحيح currY = *معلومات++;

	    o.x_وٿي_2 = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_وٿي_2 = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  اكسر;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (صحيح) mark - (صحيح) بفر->idx;
	إذا (c->بفر_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      إذا (داخلا.flags & Mark)
      {
	mark_سيٽ = سچ;
	mark = بفر->idx;
      }
    }

    عام:
    hb_aat_apply_context_t *c;
    ثابت Kerx_Sub_جدول_Format4 *جدول;
    خاص:
    بنانشان صحيح action_type;
    ثابت HBUINT16 *ankr_معلومات;
    منطقي mark_سيٽ;
    بنانشان صحيح mark;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    State_جدول_Driver<Types, داخلا_معلومات, Flags> driver (machine, c->font->face);

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

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &پهريون_سيٽ, سيٽ_t &ٻيو_سيٽ, بنانشان انگ_glyphs) ثابت
  {
    machine.collect_initial_glyphs (پهريون_سيٽ, انگ_glyphs, *هذا);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محمي:
  Kern_Sub_جدول_سرو		سرو;
  State_جدول<Types, داخلا_معلومات>	machine;
  HBUINT32			flags;
  عام:
  DEFINE_سائيز_STATIC (Kern_Sub_جدول_سرو::static_سائيز + (State_جدول<Types, داخلا_معلومات>::static_سائيز + HBUINT32::static_سائيز));
};

سانچو <قسم_نالو Kern_Sub_جدول_سرو>
بنية Kerx_Sub_جدول_Format6
{
  تعداد Flags
  {
    قدرون_Are_Long	= 0x00000001,
  };

  منطقي is_long () ثابت { أرجع flags & قدرون_Are_Long; }

  صحيح get_kerning (hb_codepoint_t کاٻي, hb_codepoint_t ساڄي,
		   hb_aat_apply_context_t *c) ثابت
  {
    بنانشان صحيح انگ_glyphs = c->sanitizer.get_انگ_glyphs ();
    إذا (is_long ())
    {
      ثابت خودڪار &t = u.l;
      بنانشان صحيح l = (هذا+t.صف_اشاريو_جدول).get_قدر_or_null (کاٻي, انگ_glyphs);
      بنانشان صحيح r = (هذا+t.ڪالم_اشاريو_جدول).get_قدر_or_null (ساڄي, انگ_glyphs);
      بنانشان صحيح وٿي = l + r;
      إذا (unlikely (وٿي < l)) أرجع 0; /* Addition overflow. */
      إذا (unlikely (hb_unsigned_mul_overflows (وٿي, حجم (FWORD32)))) أرجع 0;
      ثابت FWORD32 *v = &Struct_At_وٿي<FWORD32> (&(هذا+t.array), وٿي * حجم (FWORD32));
      إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
      hb_barrier ();
      أرجع kerxTupleKern (*v, سرو.tuple_ڳڻپ_2 (), &(هذا+vector), c);
    }
    وإلا
    {
      ثابت خودڪار &t = u.s;
      بنانشان صحيح l = (هذا+t.صف_اشاريو_جدول).get_قدر_or_null (کاٻي, انگ_glyphs);
      بنانشان صحيح r = (هذا+t.ڪالم_اشاريو_جدول).get_قدر_or_null (ساڄي, انگ_glyphs);
      بنانشان صحيح وٿي = l + r;
      ثابت FWORD *v = &Struct_At_وٿي<FWORD> (&(هذا+t.array), وٿي * حجم (FWORD));
      إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
      hb_barrier ();
      أرجع kerxTupleKern (*v, سرو.tuple_ڳڻپ_2 (), &(هذا+vector), c);
    }
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (سرو.coverage & سرو.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, سرو.coverage & سرو.Cross_وهڪرو);
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
			     u.l.صف_اشاريو_جدول.sanitize (c, هذا) &&
			     u.l.ڪالم_اشاريو_جدول.sanitize (c, هذا) &&
			     c->check_range (هذا, u.l.array)
			   ) : (
			     u.s.صف_اشاريو_جدول.sanitize (c, هذا) &&
			     u.s.ڪالم_اشاريو_جدول.sanitize (c, هذا) &&
			     c->check_range (هذا, u.s.array)
			   )) &&
			  (سرو.tuple_ڳڻپ_2 () == 0 ||
			   c->check_range (هذا, vector))));
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &پهريون_سيٽ, سيٽ_t &ٻيو_سيٽ, بنانشان انگ_glyphs) ثابت
  {
    إذا (is_long ())
    {
      ثابت خودڪار &t = u.l;
      (هذا+t.صف_اشاريو_جدول).collect_glyphs (پهريون_سيٽ, انگ_glyphs);
      (هذا+t.ڪالم_اشاريو_جدول).collect_glyphs (ٻيو_سيٽ, انگ_glyphs);
    }
    وإلا
    {
      ثابت خودڪار &t = u.s;
      (هذا+t.صف_اشاريو_جدول).collect_glyphs (پهريون_سيٽ, انگ_glyphs);
      (هذا+t.ڪالم_اشاريو_جدول).collect_glyphs (ٻيو_سيٽ, انگ_glyphs);
    }
  }

  بنية accelerator_t
  {
    ثابت Kerx_Sub_جدول_Format6 &جدول;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت Kerx_Sub_جدول_Format6 &جدول_2,
		   hb_aat_apply_context_t *c_) :
		     جدول (جدول_2), c (c_) {}

    صحيح get_kerning (hb_codepoint_t کاٻي, hb_codepoint_t ساڄي) ثابت
    {
      إذا (!(*c->پهريون_سيٽ)[کاٻي] || !(*c->ٻيو_سيٽ)[ساڄي]) أرجع 0;
      أرجع جدول.get_kerning (کاٻي, ساڄي, c);
    }
  };

  محمي:
  Kern_Sub_جدول_سرو		سرو;
  HBUINT32			flags;
  HBUINT16			صف_ڳڻپ_2;
  HBUINT16			ڪالم_ڳڻپ;
  اتحاد U
  {
    بنية Long
    {
      NNOffset32_منزل<Lookup<HBUINT32>>		صف_اشاريو_جدول;
      NNOffset32_منزل<Lookup<HBUINT32>>		ڪالم_اشاريو_جدول;
      NNOffset32_منزل<Unsized_Array_جو<FWORD32>>	array;
    } l;
    بنية Short
    {
      NNOffset32_منزل<Lookup<HBUINT16>>		صف_اشاريو_جدول;
      NNOffset32_منزل<Lookup<HBUINT16>>		ڪالم_اشاريو_جدول;
      NNOffset32_منزل<Unsized_Array_جو<FWORD>>	array;
    } s;
  } u;
  NNOffset32_منزل<Unsized_Array_جو<FWORD>>	vector;
  عام:
  DEFINE_سائيز_STATIC (Kern_Sub_جدول_سرو::static_سائيز + 24);
};


بنية Kerx_Sub_جدول_سرو
{
  تعريفنوع ExtendedTypes Types;

  بنانشان   tuple_ڳڻپ_2 () ثابت { أرجع tuple_ڳڻپ; }
  منطقي     is_horizontal () ثابت { أرجع !(coverage & Vertical); }

  تعداد Coverage
  {
    Vertical	= 0x80000000u,	/* Set if table has vertical kerning values. */
    Cross_وهڪرو	= 0x40000000u,	/* Set if table has cross-stream kerning values. */
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
  HBUINT32	ڊيگهائي;
  HBUINT32	coverage;
  HBUINT32	tuple_ڳڻپ;
  عام:
  DEFINE_سائيز_STATIC (12);
};

بنية Kerx_Sub_جدول
{
  دوست بنية kerx;

  size_t get_سائيز () ثابت { أرجع u.سرو.ڊيگهائي; }
  بنانشان صحيح get_type () ثابت { أرجع u.سرو.coverage & u.سرو.SubtableType; }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بنانشان صحيح subtable_type = get_type ();
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

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &پهريون_سيٽ, سيٽ_t &ٻيو_سيٽ, بنانشان انگ_glyphs) ثابت
  {
    بنانشان صحيح subtable_type = get_type ();
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); u.format0.collect_glyphs (پهريون_سيٽ, ٻيو_سيٽ, انگ_glyphs); أرجع;
    حالة 1:	hb_barrier (); u.format1.collect_glyphs (پهريون_سيٽ, ٻيو_سيٽ, انگ_glyphs); أرجع;
    حالة 2:	hb_barrier (); u.format2.collect_glyphs (پهريون_سيٽ, ٻيو_سيٽ, انگ_glyphs); أرجع;
    حالة 4:	hb_barrier (); u.format4.collect_glyphs (پهريون_سيٽ, ٻيو_سيٽ, انگ_glyphs); أرجع;
    حالة 6:	hb_barrier (); u.format6.collect_glyphs (پهريون_سيٽ, ٻيو_سيٽ, انگ_glyphs); أرجع;
    افتراضي:	أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(u.سرو.sanitize (c) &&
	  hb_barrier () &&
	  u.سرو.ڊيگهائي >= u.سرو.static_سائيز &&
	  c->check_range (هذا, u.سرو.ڊيگهائي)))
      return_trace (خطأ);

    return_trace (dispatch (c));
  }

  عام:
  اتحاد {
  Kerx_Sub_جدول_سرو				سرو;
  Kerx_Sub_جدول_Format0<Kerx_Sub_جدول_سرو>	format0;
  Kerx_Sub_جدول_Format1<Kerx_Sub_جدول_سرو>	format1;
  Kerx_Sub_جدول_Format2<Kerx_Sub_جدول_سرو>	format2;
  Kerx_Sub_جدول_Format4<Kerx_Sub_جدول_سرو>	format4;
  Kerx_Sub_جدول_Format6<Kerx_Sub_جدول_سرو>	format6;
  } u;
  عام:
  DEFINE_سائيز_MIN (12);
};


/*
 * The 'kerx' Table
 */

بنية kern_subtable_accelerator_معلومات_t
{
  hb_ٻٽو_انگ_سيٽ_t پهريون_سيٽ;
  hb_ٻٽو_انگ_سيٽ_t ٻيو_سيٽ;
  بدلجندڙ hb_aat_class_cache_t class_cache;
};

بنية kern_accelerator_معلومات_t
{
  hb_ويڪٽر_t<kern_subtable_accelerator_معلومات_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

سانچو <قسم_نالو T>
بنية Kerx_جدول
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت T* thiz () ثابت { أرجع static_cast<ثابت T *> (هذا); }

  منطقي has_state_machine () ثابت
  {
    تعريفنوع قسم_نالو T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پهريون_Sub_جدول;
    بنانشان صحيح ڳڻپ = thiz()->جدول_ڳڻپ;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (st->get_type () == 1)
	أرجع سچ;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع خطأ;
  }

  منطقي has_cross_وهڪرو () ثابت
  {
    تعريفنوع قسم_نالو T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پهريون_Sub_جدول;
    بنانشان صحيح ڳڻپ = thiz()->جدول_ڳڻپ;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (st->u.سرو.coverage & st->u.سرو.Cross_وهڪرو)
	أرجع سچ;
      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع خطأ;
  }

  صحيح get_h_kerning (hb_codepoint_t کاٻي, hb_codepoint_t ساڄي) ثابت
  {
    تعريفنوع قسم_نالو T::Sub_جدول Sub_جدول;

    صحيح v = 0;
    ثابت Sub_جدول *st = &thiz()->پهريون_Sub_جدول;
    بنانشان صحيح ڳڻپ = thiz()->جدول_ڳڻپ;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا ((st->u.سرو.coverage & (st->u.سرو.Variation | st->u.سرو.Cross_وهڪرو)) ||
	  !st->u.سرو.is_horizontal ())
	استمر;
      v += st->get_kerning (کاٻي, ساڄي);
      st = &StructAfter<Sub_جدول> (*st);
    }
    أرجع v;
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c,
	      ثابت kern_accelerator_معلومات_t &accel_معلومات) ثابت
  {
    c->بفر->unsafe_منزل_concat ();

    c->تياري_بفر_glyph_سيٽ ();

    تعريفنوع قسم_نالو T::Sub_جدول Sub_جدول;

    منطقي ret = خطأ;
    منطقي ڏٺل_Cross_وهڪرو = خطأ;
    c->سيٽ_lookup_اشاريو (0);
    ثابت Sub_جدول *st = &thiz()->پهريون_Sub_جدول;
    بنانشان صحيح ڳڻپ = thiz()->جدول_ڳڻپ;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      منطقي reverse;

      خودڪار &subtable_accel = accel_معلومات.subtable_accels[i];

      إذا (!T::Types::extended && (st->u.سرو.coverage & st->u.سرو.Variation))
	اذهب skip;

      إذا (HB_DIRECTION_IS_HORIZONTAL (c->بفر->props.direction) != st->u.سرو.is_horizontal ())
	اذهب skip;

      c->پهريون_سيٽ = &subtable_accel.پهريون_سيٽ;
      c->ٻيو_سيٽ = &subtable_accel.ٻيو_سيٽ;
      c->machine_class_cache = &subtable_accel.class_cache;

      إذا (!c->بفر_intersects_machine ())
      {
	(فراغ) c->بفر->پيغام (c->font, "skipped subtable %u because no glyph matches", c->lookup_اشاريو);
	اذهب skip;
      }

      reverse = منطقي (st->u.سرو.coverage & st->u.سرو.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->بفر->props.direction);

      إذا (!c->بفر->پيغام (c->font, "start subtable %u", c->lookup_اشاريو))
	اذهب skip;

      إذا (!ڏٺل_Cross_وهڪرو &&
	  (st->u.سرو.coverage & st->u.سرو.Cross_وهڪرو))
      {
	/* Attach all glyphs into a chain. */
	ڏٺل_Cross_وهڪرو = سچ;
	hb_glyph_position_t *pos = c->بفر->pos;
	بنانشان صحيح ڳڻپ = c->بفر->len;
	لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->بفر->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      إذا (reverse != c->بفر_is_reversed)
        c->ابتو_بفر ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < ڳڻپ - 1 ? st : (ثابت Sub_جدول *) خالي_اشارو);
	ret |= st->dispatch (c);
      }

      (فراغ) c->بفر->پيغام (c->font, "end subtable %u", c->lookup_اشاريو);

    skip:
      st = &StructAfter<Sub_جدول> (*st);
      c->سيٽ_lookup_اشاريو (c->lookup_اشاريو + 1);
    }
    إذا (c->بفر_is_reversed)
      c->ابتو_بفر ();

    أرجع ret;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(thiz()->نسخو.sanitize (c) &&
		    hb_barrier () &&
		    (بنانشان) thiz()->نسخو >= (بنانشان) T::min_نسخو &&
		    thiz()->جدول_ڳڻپ.sanitize (c))))
      return_trace (خطأ);

    تعريفنوع قسم_نالو T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پهريون_Sub_جدول;
    بنانشان صحيح ڳڻپ = thiz()->جدول_ڳڻپ;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (unlikely (!st->u.سرو.sanitize (c)))
	return_trace (خطأ);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < ڳڻپ - 1 ? st : (ثابت Sub_جدول *) خالي_اشارو);

      إذا (unlikely (!st->sanitize (c)))
	return_trace (خطأ);

      st = &StructAfter<Sub_جدول> (*st);
    }

    بنانشان major_نسخو = thiz()->نسخو;
    إذا (حجم (thiz()->نسخو) == 4)
      major_نسخو = major_نسخو >> 16;
    إذا (major_نسخو >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) st;
      إذا (!coverage->sanitize (c, ڳڻپ))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }

  kern_accelerator_معلومات_t create_accelerator_معلومات (بنانشان انگ_glyphs) ثابت
  {
    kern_accelerator_معلومات_t accel_معلومات;

    تعريفنوع قسم_نالو T::Sub_جدول Sub_جدول;

    ثابت Sub_جدول *st = &thiz()->پهريون_Sub_جدول;
    بنانشان صحيح ڳڻپ = thiz()->جدول_ڳڻپ;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      خودڪار &subtable_accel = *accel_معلومات.subtable_accels.push ();
      إذا (unlikely (accel_معلومات.subtable_accels.اندريون_error ()))
	  أرجع accel_معلومات;

      st->collect_glyphs (subtable_accel.پهريون_سيٽ, subtable_accel.ٻيو_سيٽ, انگ_glyphs);
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
      هذا->accel_معلومات = هذا->جدول->create_accelerator_معلومات (face->get_انگ_glyphs ());
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
  دوست بنية Kerx_جدول<kerx>;

  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_AAT_ٽيگ_kerx;
  ساكن constexpr بنانشان min_نسخو = 2u;

  تعريفنوع Kerx_Sub_جدول_سرو Sub_جدول_سرو;
  تعريفنوع Sub_جدول_سرو::Types Types;
  تعريفنوع Kerx_Sub_جدول Sub_جدول;

  منطقي has_معلومات () ثابت { أرجع نسخو; }

  محمي:
  HBUINT16	نسخو;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	اڻ_استعمال_ٿيل_2;		/* Set to 0. */
  HBUINT32	جدول_ڳڻپ;	/* The number of subtables included in the extended kerning
				 * table. */
  Sub_جدول	پهريون_Sub_جدول;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  عام:
  DEFINE_سائيز_MIN (8);
};

بنية kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#جيڪڏهن_پورو /* HB_AAT_LAYOUT_KERX_TABLE_HH */
