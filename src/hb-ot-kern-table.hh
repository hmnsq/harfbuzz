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

#که_نه_تعریف_شوی HB_OT_KERN_جدول_HH
#تعریف_کړه HB_OT_KERN_جدول_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#تعریف_کړه HB_OT_نښان_kern HB_نښان('k','e','r','n')


نطاق OT {


کينډۍ <ډول_نوم Kern_Sub_جدول_سرلیک>
بنية Kern_Sub_جدول_Format3
{
  صحيح get_kerning (hb_codepoint_t کیڼ, hb_codepoint_t ښی) ثابت
  {
    hb_array_t<ثابت FWORD> kern_ارزښت = kern_ارزښت_Z.as_array (kern_ارزښت_شمېر);
    hb_array_t<ثابت HBUINT8> کیڼ_Class = StructAfter<ثابت Unsized_Array_د<HBUINT8>> (kern_ارزښت).as_array (glyph_شمېر_2);
    hb_array_t<ثابت HBUINT8> ښی_Class = StructAfter<ثابت Unsized_Array_د<HBUINT8>> (کیڼ_Class).as_array (glyph_شمېر_2);
    hb_array_t<ثابت HBUINT8> kern_شاخص = StructAfter<ثابت Unsized_Array_د<HBUINT8>> (ښی_Class).as_array (کیڼ_Class_شمېر * ښی_Class_شمېر);

    بینښه صحيح کیڼ_C = کیڼ_Class[کیڼ];
    بینښه صحيح ښی_C = ښی_Class[ښی];
    إذا (unlikely (کیڼ_C >= کیڼ_Class_شمېر || ښی_C >= ښی_Class_شمېر))
      أرجع 0;
    بینښه صحيح i = کیڼ_C * ښی_Class_شمېر + ښی_C;
    أرجع kern_ارزښت[kern_شاخص[i]];
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      أرجع خطأ;

    إذا (سرلیک.coverage & سرلیک.Backwards)
      أرجع خطأ;

    hb_kern_machine_t<Kern_Sub_جدول_Format3> machine (*هذا, سرلیک.coverage & سرلیک.Cross_بهیر);
    machine.kern (c->font, c->بفر, c->plan->kern_ماسک);

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (kern_ارزښت_Z,
				  kern_ارزښت_شمېر * حجم (FWORD) +
				  glyph_شمېر_2 * 2 +
				  کیڼ_Class_شمېر * ښی_Class_شمېر));
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &کیڼ_سېټ, سېټ_t &ښی_سېټ, بینښه عدد_glyphs) ثابت
  {
    إذا (likely (glyph_شمېر_2))
    {
      کیڼ_سېټ.جمع_کړه_range (0, عدد_glyphs - 1);
      ښی_سېټ.جمع_کړه_range (0, عدد_glyphs - 1);
    }
  }

  محمي:
  Kern_Sub_جدول_سرلیک
		سرلیک;
  HBUINT16	glyph_شمېر_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_ارزښت_شمېر;	/* The number of kerning values. */
  HBUINT8	کیڼ_Class_شمېر;	/* The number of left-hand classes. */
  HBUINT8	ښی_Class_شمېر;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  Unsized_Array_د<FWORD>
		kern_ارزښت_Z;	/* The kerning values.
				 * Length kernValueCount. */
#إذا 0
  Unsized_Array_د<HBUINT8>
		کیڼ_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  Unsized_Array_د<HBUINT8>
		ښی_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  Unsized_Array_د<HBUINT8>kern_شاخص;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#د_که_پای
  عام:
  DEFINE_اندازه_ARRAY (Kern_Sub_جدول_سرلیک::static_اندازه + 6, kern_ارزښت_Z);
};

کينډۍ <ډول_نوم Kern_Sub_جدول_سرلیک>
بنية Kern_Sub_جدول
{
  size_t get_اندازه () ثابت { أرجع u.سرلیک.اوږدتیا; }
  بینښه صحيح get_type () ثابت { أرجع u.سرلیک.format; }

  صحيح get_kerning (hb_codepoint_t کیڼ, hb_codepoint_t ښی) ثابت
  {
    بدل (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    حالة 0: hb_barrier (); أرجع u.format0.get_kerning (کیڼ, ښی);
    افتراضي:أرجع 0;
    }
  }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بینښه صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#د_که_پای
    حالة 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
    حالة 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#د_که_پای
    افتراضي:	return_trace (c->default_return_ارزښت ());
    }
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &کیڼ_سېټ, سېټ_t &ښی_سېټ, بینښه عدد_glyphs) ثابت
  {
    بینښه صحيح subtable_type = get_type ();
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); u.format0.collect_glyphs (کیڼ_سېټ, ښی_سېټ, عدد_glyphs); أرجع;
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); u.format1.collect_glyphs (کیڼ_سېټ, ښی_سېټ, عدد_glyphs); أرجع;
#د_که_پای
    حالة 2:	hb_barrier (); u.format2.collect_glyphs (کیڼ_سېټ, ښی_سېټ, عدد_glyphs); أرجع;
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
    حالة 3:	hb_barrier (); u.format3.collect_glyphs (کیڼ_سېټ, ښی_سېټ, عدد_glyphs); أرجع;
#د_که_پای
    افتراضي:	أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(u.سرلیک.sanitize (c) &&
		    hb_barrier () &&
		    u.سرلیک.اوږدتیا >= u.سرلیک.min_اندازه &&
		    c->check_range (هذا, u.سرلیک.اوږدتیا)))) return_trace (خطأ);

    return_trace (dispatch (c));
  }

  عام:
  اتحاد {
  Kern_Sub_جدول_سرلیک				سرلیک;
  AAT::Kerx_Sub_جدول_Format0<Kern_Sub_جدول_سرلیک>	format0;
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  AAT::Kerx_Sub_جدول_Format1<Kern_Sub_جدول_سرلیک>	format1;
#د_که_پای
  AAT::Kerx_Sub_جدول_Format2<Kern_Sub_جدول_سرلیک>	format2;
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  Kern_Sub_جدول_Format3<Kern_Sub_جدول_سرلیک>	format3;
#د_که_پای
  } u;
  عام:
  DEFINE_اندازه_MIN (Kern_Sub_جدول_سرلیک::static_اندازه);
};


بنية Kern_OTSub_جدول_سرلیک
{
  ساكن constexpr منطقي apple = خطأ;
  تعريفنوع AAT::ObsoleteTypes Types;

  بینښه   tuple_شمېر_2 () ثابت { أرجع 0; }
  منطقي     is_horizontal () ثابت { أرجع (coverage & Horizontal); }

  تعداد Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    Cross_بهیر	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT16	نسخه_Z;	/* Unused. */
  HBUINT16	اوږدتیا;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية KernOT : AAT::Kerx_جدول<KernOT>
{
  ملګری بنية AAT::Kerx_جدول<KernOT>;

  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_kern;
  ساكن constexpr بینښه min_نسخه = 0u;

  تعريفنوع Kern_OTSub_جدول_سرلیک Sub_جدول_سرلیک;
  تعريفنوع Sub_جدول_سرلیک::Types Types;
  تعريفنوع Kern_Sub_جدول<Sub_جدول_سرلیک> Sub_جدول;

  محمي:
  HBUINT16	نسخه;	/* Version--0x0000u */
  HBUINT16	جدول_شمېر;	/* Number of subtables in the kerning table. */
  Sub_جدول	لومړی_Sub_جدول;	/* Subtables. */
  عام:
  DEFINE_اندازه_MIN (4);
};


بنية Kern_AATSub_جدول_سرلیک
{
  ساكن constexpr منطقي apple = ریښتیا;
  تعريفنوع AAT::ObsoleteTypes Types;

  بینښه   tuple_شمېر_2 () ثابت { أرجع 0; }
  منطقي     is_horizontal () ثابت { أرجع !(coverage & Vertical); }

  تعداد Coverage
  {
    Vertical	= 0x80u,
    Cross_بهیر	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT32	اوږدتیا;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tuple_شاخص;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية KernAAT : AAT::Kerx_جدول<KernAAT>
{
  ملګری بنية AAT::Kerx_جدول<KernAAT>;

  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_kern;
  ساكن constexpr بینښه min_نسخه = 0x00010000u;

  تعريفنوع Kern_AATSub_جدول_سرلیک Sub_جدول_سرلیک;
  تعريفنوع Sub_جدول_سرلیک::Types Types;
  تعريفنوع Kern_Sub_جدول<Sub_جدول_سرلیک> Sub_جدول;

  محمي:
  HBUINT32	نسخه;	/* Version--0x00010000u */
  HBUINT32	جدول_شمېر;	/* Number of subtables in the kerning table. */
  Sub_جدول	لومړی_Sub_جدول;	/* Subtables. */
  عام:
  DEFINE_اندازه_MIN (8);
};

بنية kern
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_kern;

  منطقي     has_معلومات () ثابت { أرجع u.version32.v; }
  بینښه get_type () ثابت { أرجع u.major.v; }

  منطقي has_state_machine () ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.has_state_machine ();
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.has_state_machine ();
#د_که_پای
    افتراضي:أرجع خطأ;
    }
  }

  منطقي has_cross_بهیر () ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.has_cross_بهیر ();
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.has_cross_بهیر ();
#د_که_پای
    افتراضي:أرجع خطأ;
    }
  }

  صحيح get_h_kerning (hb_codepoint_t کیڼ, hb_codepoint_t ښی) ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.get_h_kerning (کیڼ, ښی);
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.get_h_kerning (کیڼ, ښی);
#د_که_پای
    افتراضي:أرجع 0;
    }
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c,
	      ثابت AAT::kern_accelerator_معلومات_t &accel_معلومات) ثابت
  { أرجع dispatch (c, accel_معلومات); }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بینښه صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#د_که_پای
    افتراضي:	return_trace (c->default_return_ارزښت ());
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.version32.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_معلومات_t create_accelerator_معلومات (بینښه عدد_glyphs) ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.create_accelerator_معلومات (عدد_glyphs);
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.create_accelerator_معلومات (عدد_glyphs);
#د_که_پای
    افتراضي:أرجع AAT::kern_accelerator_معلومات_t ();
    }
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->جدول = sc.reference_جدول<kern> (face);
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

    hb_blob_ptr_t<kern> جدول;
    AAT::kern_accelerator_معلومات_t accel_معلومات;
    AAT::hb_aat_scratch_t scratch;
  };

  محمي:
  اتحاد {
  بنية { HBUINT32 v; }	version32;
  بنية { HBUINT16 v; }	major;
  KernOT		ot;
#که_نه_تعریف_شوی HB_NO_AAT_SHAPE
  KernAAT		aat;
#د_که_پای
  } u;
  عام:
  DEFINE_اندازه_UNION (4, version32.v);
};

بنية kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#د_که_پای /* HB_OT_KERN_TABLE_HH */
