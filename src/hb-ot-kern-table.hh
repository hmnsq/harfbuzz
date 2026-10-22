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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_KERN_جدول_HH
#وصف_ڪريو HB_OT_KERN_جدول_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#وصف_ڪريو HB_OT_ٽيگ_kern HB_ٽيگ('k','e','r','n')


نطاق OT {


سانچو <قسم_نالو Kern_Sub_جدول_سرو>
بنية Kern_Sub_جدول_Format3
{
  صحيح get_kerning (hb_codepoint_t کاٻي, hb_codepoint_t ساڄي) ثابت
  {
    hb_array_t<ثابت FWORD> kern_قدر = kern_قدر_Z.as_array (kern_قدر_ڳڻپ);
    hb_array_t<ثابت HBUINT8> کاٻو_Class = StructAfter<ثابت Unsized_Array_جو<HBUINT8>> (kern_قدر).as_array (glyph_ڳڻپ_2);
    hb_array_t<ثابت HBUINT8> ساڄو_Class = StructAfter<ثابت Unsized_Array_جو<HBUINT8>> (کاٻو_Class).as_array (glyph_ڳڻپ_2);
    hb_array_t<ثابت HBUINT8> kern_اشاريو = StructAfter<ثابت Unsized_Array_جو<HBUINT8>> (ساڄو_Class).as_array (کاٻو_Class_ڳڻپ * ساڄو_Class_ڳڻپ);

    بنانشان صحيح کاٻو_C = کاٻو_Class[کاٻي];
    بنانشان صحيح ساڄو_C = ساڄو_Class[ساڄي];
    إذا (unlikely (کاٻو_C >= کاٻو_Class_ڳڻپ || ساڄو_C >= ساڄو_Class_ڳڻپ))
      أرجع 0;
    بنانشان صحيح i = کاٻو_C * ساڄو_Class_ڳڻپ + ساڄو_C;
    أرجع kern_قدر[kern_اشاريو[i]];
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      أرجع خطأ;

    إذا (سرو.coverage & سرو.Backwards)
      أرجع خطأ;

    hb_kern_machine_t<Kern_Sub_جدول_Format3> machine (*هذا, سرو.coverage & سرو.Cross_وهڪرو);
    machine.kern (c->font, c->بفر, c->plan->kern_نقاب);

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (kern_قدر_Z,
				  kern_قدر_ڳڻپ * حجم (FWORD) +
				  glyph_ڳڻپ_2 * 2 +
				  کاٻو_Class_ڳڻپ * ساڄو_Class_ڳڻپ));
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &کاٻو_سيٽ, سيٽ_t &ساڄو_سيٽ, بنانشان انگ_glyphs) ثابت
  {
    إذا (likely (glyph_ڳڻپ_2))
    {
      کاٻو_سيٽ.جوڙيو_range (0, انگ_glyphs - 1);
      ساڄو_سيٽ.جوڙيو_range (0, انگ_glyphs - 1);
    }
  }

  محمي:
  Kern_Sub_جدول_سرو
		سرو;
  HBUINT16	glyph_ڳڻپ_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_قدر_ڳڻپ;	/* The number of kerning values. */
  HBUINT8	کاٻو_Class_ڳڻپ;	/* The number of left-hand classes. */
  HBUINT8	ساڄو_Class_ڳڻپ;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  Unsized_Array_جو<FWORD>
		kern_قدر_Z;	/* The kerning values.
				 * Length kernValueCount. */
#إذا 0
  Unsized_Array_جو<HBUINT8>
		کاٻو_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  Unsized_Array_جو<HBUINT8>
		ساڄو_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  Unsized_Array_جو<HBUINT8>kern_اشاريو;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#جيڪڏهن_پورو
  عام:
  DEFINE_سائيز_ARRAY (Kern_Sub_جدول_سرو::static_سائيز + 6, kern_قدر_Z);
};

سانچو <قسم_نالو Kern_Sub_جدول_سرو>
بنية Kern_Sub_جدول
{
  size_t get_سائيز () ثابت { أرجع u.سرو.ڊيگهائي; }
  بنانشان صحيح get_type () ثابت { أرجع u.سرو.format; }

  صحيح get_kerning (hb_codepoint_t کاٻي, hb_codepoint_t ساڄي) ثابت
  {
    بدل (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    حالة 0: hb_barrier (); أرجع u.format0.get_kerning (کاٻي, ساڄي);
    افتراضي:أرجع 0;
    }
  }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بنانشان صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#جيڪڏهن_پورو
    حالة 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
    حالة 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#جيڪڏهن_پورو
    افتراضي:	return_trace (c->default_return_قدر ());
    }
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &کاٻو_سيٽ, سيٽ_t &ساڄو_سيٽ, بنانشان انگ_glyphs) ثابت
  {
    بنانشان صحيح subtable_type = get_type ();
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); u.format0.collect_glyphs (کاٻو_سيٽ, ساڄو_سيٽ, انگ_glyphs); أرجع;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); u.format1.collect_glyphs (کاٻو_سيٽ, ساڄو_سيٽ, انگ_glyphs); أرجع;
#جيڪڏهن_پورو
    حالة 2:	hb_barrier (); u.format2.collect_glyphs (کاٻو_سيٽ, ساڄو_سيٽ, انگ_glyphs); أرجع;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
    حالة 3:	hb_barrier (); u.format3.collect_glyphs (کاٻو_سيٽ, ساڄو_سيٽ, انگ_glyphs); أرجع;
#جيڪڏهن_پورو
    افتراضي:	أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(u.سرو.sanitize (c) &&
		    hb_barrier () &&
		    u.سرو.ڊيگهائي >= u.سرو.min_سائيز &&
		    c->check_range (هذا, u.سرو.ڊيگهائي)))) return_trace (خطأ);

    return_trace (dispatch (c));
  }

  عام:
  اتحاد {
  Kern_Sub_جدول_سرو				سرو;
  AAT::Kerx_Sub_جدول_Format0<Kern_Sub_جدول_سرو>	format0;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  AAT::Kerx_Sub_جدول_Format1<Kern_Sub_جدول_سرو>	format1;
#جيڪڏهن_پورو
  AAT::Kerx_Sub_جدول_Format2<Kern_Sub_جدول_سرو>	format2;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  Kern_Sub_جدول_Format3<Kern_Sub_جدول_سرو>	format3;
#جيڪڏهن_پورو
  } u;
  عام:
  DEFINE_سائيز_MIN (Kern_Sub_جدول_سرو::static_سائيز);
};


بنية Kern_OTSub_جدول_سرو
{
  ساكن constexpr منطقي apple = خطأ;
  تعريفنوع AAT::ObsoleteTypes Types;

  بنانشان   tuple_ڳڻپ_2 () ثابت { أرجع 0; }
  منطقي     is_horizontal () ثابت { أرجع (coverage & Horizontal); }

  تعداد Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    Cross_وهڪرو	= 0x04u,
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
  HBUINT16	نسخو_Z;	/* Unused. */
  HBUINT16	ڊيگهائي;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  عام:
  DEFINE_سائيز_STATIC (6);
};

بنية KernOT : AAT::Kerx_جدول<KernOT>
{
  دوست بنية AAT::Kerx_جدول<KernOT>;

  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_kern;
  ساكن constexpr بنانشان min_نسخو = 0u;

  تعريفنوع Kern_OTSub_جدول_سرو Sub_جدول_سرو;
  تعريفنوع Sub_جدول_سرو::Types Types;
  تعريفنوع Kern_Sub_جدول<Sub_جدول_سرو> Sub_جدول;

  محمي:
  HBUINT16	نسخو;	/* Version--0x0000u */
  HBUINT16	جدول_ڳڻپ;	/* Number of subtables in the kerning table. */
  Sub_جدول	پهريون_Sub_جدول;	/* Subtables. */
  عام:
  DEFINE_سائيز_MIN (4);
};


بنية Kern_AATSub_جدول_سرو
{
  ساكن constexpr منطقي apple = سچ;
  تعريفنوع AAT::ObsoleteTypes Types;

  بنانشان   tuple_ڳڻپ_2 () ثابت { أرجع 0; }
  منطقي     is_horizontal () ثابت { أرجع !(coverage & Vertical); }

  تعداد Coverage
  {
    Vertical	= 0x80u,
    Cross_وهڪرو	= 0x40u,
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
  HBUINT32	ڊيگهائي;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tuple_اشاريو;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية KernAAT : AAT::Kerx_جدول<KernAAT>
{
  دوست بنية AAT::Kerx_جدول<KernAAT>;

  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_kern;
  ساكن constexpr بنانشان min_نسخو = 0x00010000u;

  تعريفنوع Kern_AATSub_جدول_سرو Sub_جدول_سرو;
  تعريفنوع Sub_جدول_سرو::Types Types;
  تعريفنوع Kern_Sub_جدول<Sub_جدول_سرو> Sub_جدول;

  محمي:
  HBUINT32	نسخو;	/* Version--0x00010000u */
  HBUINT32	جدول_ڳڻپ;	/* Number of subtables in the kerning table. */
  Sub_جدول	پهريون_Sub_جدول;	/* Subtables. */
  عام:
  DEFINE_سائيز_MIN (8);
};

بنية kern
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_kern;

  منطقي     has_معلومات () ثابت { أرجع u.version32.v; }
  بنانشان get_type () ثابت { أرجع u.major.v; }

  منطقي has_state_machine () ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.has_state_machine ();
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.has_state_machine ();
#جيڪڏهن_پورو
    افتراضي:أرجع خطأ;
    }
  }

  منطقي has_cross_وهڪرو () ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.has_cross_وهڪرو ();
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.has_cross_وهڪرو ();
#جيڪڏهن_پورو
    افتراضي:أرجع خطأ;
    }
  }

  صحيح get_h_kerning (hb_codepoint_t کاٻي, hb_codepoint_t ساڄي) ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.get_h_kerning (کاٻي, ساڄي);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.get_h_kerning (کاٻي, ساڄي);
#جيڪڏهن_پورو
    افتراضي:أرجع 0;
    }
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c,
	      ثابت AAT::kern_accelerator_معلومات_t &accel_معلومات) ثابت
  { أرجع dispatch (c, accel_معلومات); }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بنانشان صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#جيڪڏهن_پورو
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

  AAT::kern_accelerator_معلومات_t create_accelerator_معلومات (بنانشان انگ_glyphs) ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.create_accelerator_معلومات (انگ_glyphs);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.create_accelerator_معلومات (انگ_glyphs);
#جيڪڏهن_پورو
    افتراضي:أرجع AAT::kern_accelerator_معلومات_t ();
    }
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->جدول = sc.reference_جدول<kern> (face);
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

    hb_blob_ptr_t<kern> جدول;
    AAT::kern_accelerator_معلومات_t accel_معلومات;
    AAT::hb_aat_scratch_t scratch;
  };

  محمي:
  اتحاد {
  بنية { HBUINT32 v; }	version32;
  بنية { HBUINT16 v; }	major;
  KernOT		ot;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  KernAAT		aat;
#جيڪڏهن_پورو
  } u;
  عام:
  DEFINE_سائيز_UNION (4, version32.v);
};

بنية kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_KERN_TABLE_HH */
