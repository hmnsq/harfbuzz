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

#جے_بنا_تعریف HB_OT_KERN_جدول_HH
#تعریف_کرو HB_OT_KERN_جدول_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#تعریف_کرو HB_OT_ٹیگ_kern HB_ٹیگ('k','e','r','n')


نطاق OT {


سانچا <قسم_ناں Kern_Sub_جدول_سرخی>
بنية Kern_Sub_جدول_Format3
{
  صحيح get_kerning (hb_codepoint_t کھبے, hb_codepoint_t سجے) ثابت
  {
    hb_array_t<ثابت FWORD> kern_قدر = kern_قدر_Z.as_array (kern_قدر_گنتی);
    hb_array_t<ثابت HBUINT8> بایاں_Class = StructAfter<ثابت Unsized_Array_دا<HBUINT8>> (kern_قدر).as_array (glyph_گنتی_2);
    hb_array_t<ثابت HBUINT8> دایاں_Class = StructAfter<ثابت Unsized_Array_دا<HBUINT8>> (بایاں_Class).as_array (glyph_گنتی_2);
    hb_array_t<ثابت HBUINT8> kern_اشاریہ = StructAfter<ثابت Unsized_Array_دا<HBUINT8>> (دایاں_Class).as_array (بایاں_Class_گنتی * دایاں_Class_گنتی);

    بنانشانی صحيح بایاں_C = بایاں_Class[کھبے];
    بنانشانی صحيح دایاں_C = دایاں_Class[سجے];
    إذا (unlikely (بایاں_C >= بایاں_Class_گنتی || دایاں_C >= دایاں_Class_گنتی))
      أرجع 0;
    بنانشانی صحيح i = بایاں_C * دایاں_Class_گنتی + دایاں_C;
    أرجع kern_قدر[kern_اشاریہ[i]];
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      أرجع خطأ;

    إذا (سرخی.coverage & سرخی.Backwards)
      أرجع خطأ;

    hb_kern_machine_t<Kern_Sub_جدول_Format3> machine (*هذا, سرخی.coverage & سرخی.Cross_دھارا);
    machine.kern (c->font, c->بفر, c->plan->kern_نقاب);

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (kern_قدر_Z,
				  kern_قدر_گنتی * حجم (FWORD) +
				  glyph_گنتی_2 * 2 +
				  بایاں_Class_گنتی * دایاں_Class_گنتی));
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &بایاں_سیٹ, سیٹ_t &دایاں_سیٹ, بنانشانی عدد_glyphs) ثابت
  {
    إذا (likely (glyph_گنتی_2))
    {
      بایاں_سیٹ.جوڑو_range (0, عدد_glyphs - 1);
      دایاں_سیٹ.جوڑو_range (0, عدد_glyphs - 1);
    }
  }

  محمي:
  Kern_Sub_جدول_سرخی
		سرخی;
  HBUINT16	glyph_گنتی_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_قدر_گنتی;	/* The number of kerning values. */
  HBUINT8	بایاں_Class_گنتی;	/* The number of left-hand classes. */
  HBUINT8	دایاں_Class_گنتی;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  Unsized_Array_دا<FWORD>
		kern_قدر_Z;	/* The kerning values.
				 * Length kernValueCount. */
#إذا 0
  Unsized_Array_دا<HBUINT8>
		بایاں_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  Unsized_Array_دا<HBUINT8>
		دایاں_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  Unsized_Array_دا<HBUINT8>kern_اشاریہ;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#جے_انت
  عام:
  DEFINE_ناپ_ARRAY (Kern_Sub_جدول_سرخی::static_ناپ + 6, kern_قدر_Z);
};

سانچا <قسم_ناں Kern_Sub_جدول_سرخی>
بنية Kern_Sub_جدول
{
  size_t get_ناپ () ثابت { أرجع u.سرخی.لمائی; }
  بنانشانی صحيح get_type () ثابت { أرجع u.سرخی.format; }

  صحيح get_kerning (hb_codepoint_t کھبے, hb_codepoint_t سجے) ثابت
  {
    بدل (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    حالة 0: hb_barrier (); أرجع u.format0.get_kerning (کھبے, سجے);
    افتراضي:أرجع 0;
    }
  }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بنانشانی صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#جے_بنا_تعریف HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#جے_انت
    حالة 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#جے_بنا_تعریف HB_NO_AAT_SHAPE
    حالة 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#جے_انت
    افتراضي:	return_trace (c->default_return_قدر ());
    }
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &بایاں_سیٹ, سیٹ_t &دایاں_سیٹ, بنانشانی عدد_glyphs) ثابت
  {
    بنانشانی صحيح subtable_type = get_type ();
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); u.format0.collect_glyphs (بایاں_سیٹ, دایاں_سیٹ, عدد_glyphs); أرجع;
#جے_بنا_تعریف HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); u.format1.collect_glyphs (بایاں_سیٹ, دایاں_سیٹ, عدد_glyphs); أرجع;
#جے_انت
    حالة 2:	hb_barrier (); u.format2.collect_glyphs (بایاں_سیٹ, دایاں_سیٹ, عدد_glyphs); أرجع;
#جے_بنا_تعریف HB_NO_AAT_SHAPE
    حالة 3:	hb_barrier (); u.format3.collect_glyphs (بایاں_سیٹ, دایاں_سیٹ, عدد_glyphs); أرجع;
#جے_انت
    افتراضي:	أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(u.سرخی.sanitize (c) &&
		    hb_barrier () &&
		    u.سرخی.لمائی >= u.سرخی.min_ناپ &&
		    c->check_range (هذا, u.سرخی.لمائی)))) return_trace (خطأ);

    return_trace (dispatch (c));
  }

  عام:
  اتحاد {
  Kern_Sub_جدول_سرخی				سرخی;
  AAT::Kerx_Sub_جدول_Format0<Kern_Sub_جدول_سرخی>	format0;
#جے_بنا_تعریف HB_NO_AAT_SHAPE
  AAT::Kerx_Sub_جدول_Format1<Kern_Sub_جدول_سرخی>	format1;
#جے_انت
  AAT::Kerx_Sub_جدول_Format2<Kern_Sub_جدول_سرخی>	format2;
#جے_بنا_تعریف HB_NO_AAT_SHAPE
  Kern_Sub_جدول_Format3<Kern_Sub_جدول_سرخی>	format3;
#جے_انت
  } u;
  عام:
  DEFINE_ناپ_MIN (Kern_Sub_جدول_سرخی::static_ناپ);
};


بنية Kern_OTSub_جدول_سرخی
{
  ساكن constexpr منطقي apple = خطأ;
  تعريفنوع AAT::ObsoleteTypes Types;

  بنانشانی   tuple_گنتی_2 () ثابت { أرجع 0; }
  منطقي     is_horizontal () ثابت { أرجع (coverage & Horizontal); }

  تعداد Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    Cross_دھارا	= 0x04u,
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
  HBUINT16	نسخہ_Z;	/* Unused. */
  HBUINT16	لمائی;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  عام:
  DEFINE_ناپ_STATIC (6);
};

بنية KernOT : AAT::Kerx_جدول<KernOT>
{
  یار بنية AAT::Kerx_جدول<KernOT>;

  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_kern;
  ساكن constexpr بنانشانی min_نسخہ = 0u;

  تعريفنوع Kern_OTSub_جدول_سرخی Sub_جدول_سرخی;
  تعريفنوع Sub_جدول_سرخی::Types Types;
  تعريفنوع Kern_Sub_جدول<Sub_جدول_سرخی> Sub_جدول;

  محمي:
  HBUINT16	نسخہ;	/* Version--0x0000u */
  HBUINT16	جدول_گنتی;	/* Number of subtables in the kerning table. */
  Sub_جدول	پہلا_Sub_جدول;	/* Subtables. */
  عام:
  DEFINE_ناپ_MIN (4);
};


بنية Kern_AATSub_جدول_سرخی
{
  ساكن constexpr منطقي apple = سچ;
  تعريفنوع AAT::ObsoleteTypes Types;

  بنانشانی   tuple_گنتی_2 () ثابت { أرجع 0; }
  منطقي     is_horizontal () ثابت { أرجع !(coverage & Vertical); }

  تعداد Coverage
  {
    Vertical	= 0x80u,
    Cross_دھارا	= 0x40u,
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
  HBUINT32	لمائی;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tuple_اشاریہ;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية KernAAT : AAT::Kerx_جدول<KernAAT>
{
  یار بنية AAT::Kerx_جدول<KernAAT>;

  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_kern;
  ساكن constexpr بنانشانی min_نسخہ = 0x00010000u;

  تعريفنوع Kern_AATSub_جدول_سرخی Sub_جدول_سرخی;
  تعريفنوع Sub_جدول_سرخی::Types Types;
  تعريفنوع Kern_Sub_جدول<Sub_جدول_سرخی> Sub_جدول;

  محمي:
  HBUINT32	نسخہ;	/* Version--0x00010000u */
  HBUINT32	جدول_گنتی;	/* Number of subtables in the kerning table. */
  Sub_جدول	پہلا_Sub_جدول;	/* Subtables. */
  عام:
  DEFINE_ناپ_MIN (8);
};

بنية kern
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_kern;

  منطقي     has_انکڑے () ثابت { أرجع u.version32.v; }
  بنانشانی get_type () ثابت { أرجع u.major.v; }

  منطقي has_state_machine () ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.has_state_machine ();
#جے_بنا_تعریف HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.has_state_machine ();
#جے_انت
    افتراضي:أرجع خطأ;
    }
  }

  منطقي has_cross_دھارا () ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.has_cross_دھارا ();
#جے_بنا_تعریف HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.has_cross_دھارا ();
#جے_انت
    افتراضي:أرجع خطأ;
    }
  }

  صحيح get_h_kerning (hb_codepoint_t کھبے, hb_codepoint_t سجے) ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.get_h_kerning (کھبے, سجے);
#جے_بنا_تعریف HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.get_h_kerning (کھبے, سجے);
#جے_انت
    افتراضي:أرجع 0;
    }
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c,
	      ثابت AAT::kern_accelerator_انکڑے_t &accel_انکڑے) ثابت
  { أرجع dispatch (c, accel_انکڑے); }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بنانشانی صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#جے_بنا_تعریف HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#جے_انت
    افتراضي:	return_trace (c->default_return_قدر ());
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.version32.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_انکڑے_t create_accelerator_انکڑے (بنانشانی عدد_glyphs) ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.create_accelerator_انکڑے (عدد_glyphs);
#جے_بنا_تعریف HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.create_accelerator_انکڑے (عدد_glyphs);
#جے_انت
    افتراضي:أرجع AAT::kern_accelerator_انکڑے_t ();
    }
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->جدول = sc.reference_جدول<kern> (face);
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

    hb_blob_ptr_t<kern> جدول;
    AAT::kern_accelerator_انکڑے_t accel_انکڑے;
    AAT::hb_aat_scratch_t scratch;
  };

  محمي:
  اتحاد {
  بنية { HBUINT32 v; }	version32;
  بنية { HBUINT16 v; }	major;
  KernOT		ot;
#جے_بنا_تعریف HB_NO_AAT_SHAPE
  KernAAT		aat;
#جے_انت
  } u;
  عام:
  DEFINE_ناپ_UNION (4, version32.v);
};

بنية kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#جے_انت /* HB_OT_KERN_TABLE_HH */
