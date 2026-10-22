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

#اگر_تعریف_نشده HB_OT_KERN_جدول_HH
#تعریف_کن HB_OT_KERN_جدول_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#تعریف_کن HB_OT_برچسب_kern HB_برچسب('k','e','r','n')


نطاق OT {


قالب <نام_نوع Kern_Sub_جدول_سرآیند>
بنية Kern_Sub_جدول_Format3
{
  صحيح get_kerning (hb_codepoint_t چپ, hb_codepoint_t راست) ثابت
  {
    hb_array_t<ثابت FWORD> kern_مقدار = kern_مقدار_Z.as_array (kern_مقدار_شمار);
    hb_array_t<ثابت HBUINT8> چپ_Class = StructAfter<ثابت Unsized_Array_از<HBUINT8>> (kern_مقدار).as_array (glyph_شمار_2);
    hb_array_t<ثابت HBUINT8> راست_Class = StructAfter<ثابت Unsized_Array_از<HBUINT8>> (چپ_Class).as_array (glyph_شمار_2);
    hb_array_t<ثابت HBUINT8> kern_شاخص = StructAfter<ثابت Unsized_Array_از<HBUINT8>> (راست_Class).as_array (چپ_Class_شمار * راست_Class_شمار);

    بیعلامت صحيح چپ_C = چپ_Class[چپ];
    بیعلامت صحيح راست_C = راست_Class[راست];
    إذا (unlikely (چپ_C >= چپ_Class_شمار || راست_C >= راست_Class_شمار))
      أرجع 0;
    بیعلامت صحيح i = چپ_C * راست_Class_شمار + راست_C;
    أرجع kern_مقدار[kern_شاخص[i]];
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      أرجع خطأ;

    إذا (سرآیند.coverage & سرآیند.Backwards)
      أرجع خطأ;

    hb_kern_machine_t<Kern_Sub_جدول_Format3> machine (*هذا, سرآیند.coverage & سرآیند.Cross_جریان);
    machine.kern (c->font, c->میانگیر, c->plan->kern_ماسک);

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (kern_مقدار_Z,
				  kern_مقدار_شمار * حجم (FWORD) +
				  glyph_شمار_2 * 2 +
				  چپ_Class_شمار * راست_Class_شمار));
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &چپ_مجموعه, مجموعه_t &راست_مجموعه, بیعلامت رقم_glyphs) ثابت
  {
    إذا (likely (glyph_شمار_2))
    {
      چپ_مجموعه.جمع_کن_range (0, رقم_glyphs - 1);
      راست_مجموعه.جمع_کن_range (0, رقم_glyphs - 1);
    }
  }

  محمي:
  Kern_Sub_جدول_سرآیند
		سرآیند;
  HBUINT16	glyph_شمار_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_مقدار_شمار;	/* The number of kerning values. */
  HBUINT8	چپ_Class_شمار;	/* The number of left-hand classes. */
  HBUINT8	راست_Class_شمار;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  Unsized_Array_از<FWORD>
		kern_مقدار_Z;	/* The kerning values.
				 * Length kernValueCount. */
#إذا 0
  Unsized_Array_از<HBUINT8>
		چپ_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  Unsized_Array_از<HBUINT8>
		راست_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  Unsized_Array_از<HBUINT8>kern_شاخص;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#پایان_اگر
  عام:
  DEFINE_اندازه_ARRAY (Kern_Sub_جدول_سرآیند::static_اندازه + 6, kern_مقدار_Z);
};

قالب <نام_نوع Kern_Sub_جدول_سرآیند>
بنية Kern_Sub_جدول
{
  size_t get_اندازه () ثابت { أرجع u.سرآیند.درازی; }
  بیعلامت صحيح get_type () ثابت { أرجع u.سرآیند.format; }

  صحيح get_kerning (hb_codepoint_t چپ, hb_codepoint_t راست) ثابت
  {
    بدل (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    حالة 0: hb_barrier (); أرجع u.format0.get_kerning (چپ, راست);
    افتراضي:أرجع 0;
    }
  }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بیعلامت صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#پایان_اگر
    حالة 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
    حالة 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#پایان_اگر
    افتراضي:	return_trace (c->default_return_مقدار ());
    }
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &چپ_مجموعه, مجموعه_t &راست_مجموعه, بیعلامت رقم_glyphs) ثابت
  {
    بیعلامت صحيح subtable_type = get_type ();
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); u.format0.collect_glyphs (چپ_مجموعه, راست_مجموعه, رقم_glyphs); أرجع;
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); u.format1.collect_glyphs (چپ_مجموعه, راست_مجموعه, رقم_glyphs); أرجع;
#پایان_اگر
    حالة 2:	hb_barrier (); u.format2.collect_glyphs (چپ_مجموعه, راست_مجموعه, رقم_glyphs); أرجع;
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
    حالة 3:	hb_barrier (); u.format3.collect_glyphs (چپ_مجموعه, راست_مجموعه, رقم_glyphs); أرجع;
#پایان_اگر
    افتراضي:	أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(u.سرآیند.sanitize (c) &&
		    hb_barrier () &&
		    u.سرآیند.درازی >= u.سرآیند.min_اندازه &&
		    c->check_range (هذا, u.سرآیند.درازی)))) return_trace (خطأ);

    return_trace (dispatch (c));
  }

  عام:
  اتحاد {
  Kern_Sub_جدول_سرآیند				سرآیند;
  AAT::Kerx_Sub_جدول_Format0<Kern_Sub_جدول_سرآیند>	format0;
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  AAT::Kerx_Sub_جدول_Format1<Kern_Sub_جدول_سرآیند>	format1;
#پایان_اگر
  AAT::Kerx_Sub_جدول_Format2<Kern_Sub_جدول_سرآیند>	format2;
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  Kern_Sub_جدول_Format3<Kern_Sub_جدول_سرآیند>	format3;
#پایان_اگر
  } u;
  عام:
  DEFINE_اندازه_MIN (Kern_Sub_جدول_سرآیند::static_اندازه);
};


بنية Kern_OTSub_جدول_سرآیند
{
  ساكن constexpr منطقي apple = خطأ;
  تعريفنوع AAT::ObsoleteTypes Types;

  بیعلامت   tuple_شمار_2 () ثابت { أرجع 0; }
  منطقي     is_horizontal () ثابت { أرجع (coverage & Horizontal); }

  تعداد Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    Cross_جریان	= 0x04u,
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
  HBUINT16	درازی;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية KernOT : AAT::Kerx_جدول<KernOT>
{
  دوست بنية AAT::Kerx_جدول<KernOT>;

  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_kern;
  ساكن constexpr بیعلامت min_نسخه = 0u;

  تعريفنوع Kern_OTSub_جدول_سرآیند Sub_جدول_سرآیند;
  تعريفنوع Sub_جدول_سرآیند::Types Types;
  تعريفنوع Kern_Sub_جدول<Sub_جدول_سرآیند> Sub_جدول;

  محمي:
  HBUINT16	نسخه;	/* Version--0x0000u */
  HBUINT16	جدول_شمار;	/* Number of subtables in the kerning table. */
  Sub_جدول	اولی_Sub_جدول;	/* Subtables. */
  عام:
  DEFINE_اندازه_MIN (4);
};


بنية Kern_AATSub_جدول_سرآیند
{
  ساكن constexpr منطقي apple = درست;
  تعريفنوع AAT::ObsoleteTypes Types;

  بیعلامت   tuple_شمار_2 () ثابت { أرجع 0; }
  منطقي     is_horizontal () ثابت { أرجع !(coverage & Vertical); }

  تعداد Coverage
  {
    Vertical	= 0x80u,
    Cross_جریان	= 0x40u,
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
  HBUINT32	درازی;		/* Length of the subtable (including this header). */
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
  دوست بنية AAT::Kerx_جدول<KernAAT>;

  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_kern;
  ساكن constexpr بیعلامت min_نسخه = 0x00010000u;

  تعريفنوع Kern_AATSub_جدول_سرآیند Sub_جدول_سرآیند;
  تعريفنوع Sub_جدول_سرآیند::Types Types;
  تعريفنوع Kern_Sub_جدول<Sub_جدول_سرآیند> Sub_جدول;

  محمي:
  HBUINT32	نسخه;	/* Version--0x00010000u */
  HBUINT32	جدول_شمار;	/* Number of subtables in the kerning table. */
  Sub_جدول	اولی_Sub_جدول;	/* Subtables. */
  عام:
  DEFINE_اندازه_MIN (8);
};

بنية kern
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_kern;

  منطقي     has_داده () ثابت { أرجع u.version32.v; }
  بیعلامت get_type () ثابت { أرجع u.major.v; }

  منطقي has_state_machine () ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.has_state_machine ();
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.has_state_machine ();
#پایان_اگر
    افتراضي:أرجع خطأ;
    }
  }

  منطقي has_cross_جریان () ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.has_cross_جریان ();
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.has_cross_جریان ();
#پایان_اگر
    افتراضي:أرجع خطأ;
    }
  }

  صحيح get_h_kerning (hb_codepoint_t چپ, hb_codepoint_t راست) ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.get_h_kerning (چپ, راست);
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.get_h_kerning (چپ, راست);
#پایان_اگر
    افتراضي:أرجع 0;
    }
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c,
	      ثابت AAT::kern_accelerator_داده_t &accel_داده) ثابت
  { أرجع dispatch (c, accel_داده); }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بیعلامت صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#پایان_اگر
    افتراضي:	return_trace (c->default_return_مقدار ());
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.version32.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_داده_t create_accelerator_داده (بیعلامت رقم_glyphs) ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.create_accelerator_داده (رقم_glyphs);
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.create_accelerator_داده (رقم_glyphs);
#پایان_اگر
    افتراضي:أرجع AAT::kern_accelerator_داده_t ();
    }
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->جدول = sc.reference_جدول<kern> (face);
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

    hb_blob_ptr_t<kern> جدول;
    AAT::kern_accelerator_داده_t accel_داده;
    AAT::hb_aat_scratch_t scratch;
  };

  محمي:
  اتحاد {
  بنية { HBUINT32 v; }	version32;
  بنية { HBUINT16 v; }	major;
  KernOT		ot;
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  KernAAT		aat;
#پایان_اگر
  } u;
  عام:
  DEFINE_اندازه_UNION (4, version32.v);
};

بنية kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#پایان_اگر /* HB_OT_KERN_TABLE_HH */
