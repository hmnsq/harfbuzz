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

#اگر_غیر_تعریف_شدہ HB_OT_KERN_جدول_HH
#تعریف_کرو HB_OT_KERN_جدول_HH

#شامل "hb-aat-layout-common.hh"
#شامل "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#تعریف_کرو HB_OT_لیبل_kern HB_لیبل('k','e','r','n')


نامخلا OT {


سانچہ <قسم_نام Kern_Sub_جدول_سرخی>
ساخت Kern_Sub_جدول_Format3
{
  صحیح get_kerning (hb_codepoint_t بائیں, hb_codepoint_t دائیں) ثابت
  {
    hb_array_t<ثابت FWORD> kern_قدر = kern_قدر_Z.as_array (kern_قدر_گنتی);
    hb_array_t<ثابت HBUINT8> بایاں_Class = StructAfter<ثابت Unsized_Array_کا<HBUINT8>> (kern_قدر).as_array (glyph_گنتی_2);
    hb_array_t<ثابت HBUINT8> دایاں_Class = StructAfter<ثابت Unsized_Array_کا<HBUINT8>> (بایاں_Class).as_array (glyph_گنتی_2);
    hb_array_t<ثابت HBUINT8> kern_اشاریہ = StructAfter<ثابت Unsized_Array_کا<HBUINT8>> (دایاں_Class).as_array (بایاں_Class_گنتی * دایاں_Class_گنتی);

    غیرعلامتی صحیح بایاں_C = بایاں_Class[بائیں];
    غیرعلامتی صحیح دایاں_C = دایاں_Class[دائیں];
    اگر (unlikely (بایاں_C >= بایاں_Class_گنتی || دایاں_C >= دایاں_Class_گنتی))
      واپس 0;
    غیرعلامتی صحیح i = بایاں_C * دایاں_Class_گنتی + دایاں_C;
    واپس kern_قدر[kern_اشاریہ[i]];
  }

  منطقی apply (AAT::hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);

    اگر (!c->plan->requested_kerning)
      واپس جھوٹ;

    اگر (سرخی.coverage & سرخی.Backwards)
      واپس جھوٹ;

    hb_kern_machine_t<Kern_Sub_جدول_Format3> machine (*یہ, سرخی.coverage & سرخی.Cross_دھارا);
    machine.kern (c->font, c->بفر, c->plan->kern_نقاب);

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  c->check_range (kern_قدر_Z,
				  kern_قدر_گنتی * حجم (FWORD) +
				  glyph_گنتی_2 * 2 +
				  بایاں_Class_گنتی * دایاں_Class_گنتی));
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &بایاں_سیٹ, سیٹ_t &دایاں_سیٹ, غیرعلامتی عدد_glyphs) ثابت
  {
    اگر (likely (glyph_گنتی_2))
    {
      بایاں_سیٹ.جوڑو_range (0, عدد_glyphs - 1);
      دایاں_سیٹ.جوڑو_range (0, عدد_glyphs - 1);
    }
  }

  محفوظ:
  Kern_Sub_جدول_سرخی
		سرخی;
  HBUINT16	glyph_گنتی_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_قدر_گنتی;	/* The number of kerning values. */
  HBUINT8	بایاں_Class_گنتی;	/* The number of left-hand classes. */
  HBUINT8	دایاں_Class_گنتی;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  Unsized_Array_کا<FWORD>
		kern_قدر_Z;	/* The kerning values.
				 * Length kernValueCount. */
#اگر 0
  Unsized_Array_کا<HBUINT8>
		بایاں_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  Unsized_Array_کا<HBUINT8>
		دایاں_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  Unsized_Array_کا<HBUINT8>kern_اشاریہ;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#اگر_ختم
  عوامی:
  DEFINE_سائز_ARRAY (Kern_Sub_جدول_سرخی::static_سائز + 6, kern_قدر_Z);
};

سانچہ <قسم_نام Kern_Sub_جدول_سرخی>
ساخت Kern_Sub_جدول
{
  size_t get_سائز () ثابت { واپس u.سرخی.لمبائی; }
  غیرعلامتی صحیح get_type () ثابت { واپس u.سرخی.format; }

  صحیح get_kerning (hb_codepoint_t بائیں, hb_codepoint_t دائیں) ثابت
  {
    انتخاب (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    حالت 0: hb_barrier (); واپس u.format0.get_kerning (بائیں, دائیں);
    طےشدہ:واپس 0;
    }
  }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    غیرعلامتی صحیح subtable_type = get_type ();
    TRACE_DISPATCH (یہ, subtable_type);
    انتخاب (subtable_type) {
    حالت 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
    حالت 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#اگر_ختم
    حالت 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
    حالت 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#اگر_ختم
    طےشدہ:	return_trace (c->default_return_قدر ());
    }
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &بایاں_سیٹ, سیٹ_t &دایاں_سیٹ, غیرعلامتی عدد_glyphs) ثابت
  {
    غیرعلامتی صحیح subtable_type = get_type ();
    انتخاب (subtable_type) {
    حالت 0:	hb_barrier (); u.format0.collect_glyphs (بایاں_سیٹ, دایاں_سیٹ, عدد_glyphs); واپس;
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
    حالت 1:	hb_barrier (); u.format1.collect_glyphs (بایاں_سیٹ, دایاں_سیٹ, عدد_glyphs); واپس;
#اگر_ختم
    حالت 2:	hb_barrier (); u.format2.collect_glyphs (بایاں_سیٹ, دایاں_سیٹ, عدد_glyphs); واپس;
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
    حالت 3:	hb_barrier (); u.format3.collect_glyphs (بایاں_سیٹ, دایاں_سیٹ, عدد_glyphs); واپس;
#اگر_ختم
    طےشدہ:	واپس;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!(u.سرخی.sanitize (c) &&
		    hb_barrier () &&
		    u.سرخی.لمبائی >= u.سرخی.min_سائز &&
		    c->check_range (یہ, u.سرخی.لمبائی)))) return_trace (جھوٹ);

    return_trace (dispatch (c));
  }

  عوامی:
  اتحاد {
  Kern_Sub_جدول_سرخی				سرخی;
  AAT::Kerx_Sub_جدول_Format0<Kern_Sub_جدول_سرخی>	format0;
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  AAT::Kerx_Sub_جدول_Format1<Kern_Sub_جدول_سرخی>	format1;
#اگر_ختم
  AAT::Kerx_Sub_جدول_Format2<Kern_Sub_جدول_سرخی>	format2;
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  Kern_Sub_جدول_Format3<Kern_Sub_جدول_سرخی>	format3;
#اگر_ختم
  } u;
  عوامی:
  DEFINE_سائز_MIN (Kern_Sub_جدول_سرخی::static_سائز);
};


ساخت Kern_OTSub_جدول_سرخی
{
  ساکن constexpr منطقی apple = جھوٹ;
  قسمتعریف AAT::ObsoleteTypes Types;

  غیرعلامتی   tuple_گنتی_2 () ثابت { واپس 0; }
  منطقی     is_horizontal () ثابت { واپس (coverage & Horizontal); }

  شمار Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    Cross_دھارا	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBUINT16	نسخہ_Z;	/* Unused. */
  HBUINT16	لمبائی;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت KernOT : AAT::Kerx_جدول<KernOT>
{
  دوست ساخت AAT::Kerx_جدول<KernOT>;

  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_kern;
  ساکن constexpr غیرعلامتی min_نسخہ = 0u;

  قسمتعریف Kern_OTSub_جدول_سرخی Sub_جدول_سرخی;
  قسمتعریف Sub_جدول_سرخی::Types Types;
  قسمتعریف Kern_Sub_جدول<Sub_جدول_سرخی> Sub_جدول;

  محفوظ:
  HBUINT16	نسخہ;	/* Version--0x0000u */
  HBUINT16	جدول_گنتی;	/* Number of subtables in the kerning table. */
  Sub_جدول	پہلا_Sub_جدول;	/* Subtables. */
  عوامی:
  DEFINE_سائز_MIN (4);
};


ساخت Kern_AATSub_جدول_سرخی
{
  ساکن constexpr منطقی apple = سچ;
  قسمتعریف AAT::ObsoleteTypes Types;

  غیرعلامتی   tuple_گنتی_2 () ثابت { واپس 0; }
  منطقی     is_horizontal () ثابت { واپس !(coverage & Vertical); }

  شمار Coverage
  {
    Vertical	= 0x80u,
    Cross_دھارا	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBUINT32	لمبائی;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tuple_اشاریہ;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت KernAAT : AAT::Kerx_جدول<KernAAT>
{
  دوست ساخت AAT::Kerx_جدول<KernAAT>;

  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_kern;
  ساکن constexpr غیرعلامتی min_نسخہ = 0x00010000u;

  قسمتعریف Kern_AATSub_جدول_سرخی Sub_جدول_سرخی;
  قسمتعریف Sub_جدول_سرخی::Types Types;
  قسمتعریف Kern_Sub_جدول<Sub_جدول_سرخی> Sub_جدول;

  محفوظ:
  HBUINT32	نسخہ;	/* Version--0x00010000u */
  HBUINT32	جدول_گنتی;	/* Number of subtables in the kerning table. */
  Sub_جدول	پہلا_Sub_جدول;	/* Subtables. */
  عوامی:
  DEFINE_سائز_MIN (8);
};

ساخت kern
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_kern;

  منطقی     has_ڈیٹا () ثابت { واپس u.version32.v; }
  غیرعلامتی get_type () ثابت { واپس u.major.v; }

  منطقی has_state_machine () ثابت
  {
    انتخاب (get_type ()) {
    حالت 0: hb_barrier (); واپس u.ot.has_state_machine ();
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
    حالت 1: hb_barrier (); واپس u.aat.has_state_machine ();
#اگر_ختم
    طےشدہ:واپس جھوٹ;
    }
  }

  منطقی has_cross_دھارا () ثابت
  {
    انتخاب (get_type ()) {
    حالت 0: hb_barrier (); واپس u.ot.has_cross_دھارا ();
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
    حالت 1: hb_barrier (); واپس u.aat.has_cross_دھارا ();
#اگر_ختم
    طےشدہ:واپس جھوٹ;
    }
  }

  صحیح get_h_kerning (hb_codepoint_t بائیں, hb_codepoint_t دائیں) ثابت
  {
    انتخاب (get_type ()) {
    حالت 0: hb_barrier (); واپس u.ot.get_h_kerning (بائیں, دائیں);
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
    حالت 1: hb_barrier (); واپس u.aat.get_h_kerning (بائیں, دائیں);
#اگر_ختم
    طےشدہ:واپس 0;
    }
  }

  منطقی apply (AAT::hb_aat_apply_context_t *c,
	      ثابت AAT::kern_accelerator_ڈیٹا_t &accel_ڈیٹا) ثابت
  { واپس dispatch (c, accel_ڈیٹا); }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    غیرعلامتی صحیح subtable_type = get_type ();
    TRACE_DISPATCH (یہ, subtable_type);
    انتخاب (subtable_type) {
    حالت 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
    حالت 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#اگر_ختم
    طےشدہ:	return_trace (c->default_return_قدر ());
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.version32.v.sanitize (c)) return_trace (جھوٹ);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_ڈیٹا_t create_accelerator_ڈیٹا (غیرعلامتی عدد_glyphs) ثابت
  {
    انتخاب (get_type ()) {
    حالت 0: hb_barrier (); واپس u.ot.create_accelerator_ڈیٹا (عدد_glyphs);
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
    حالت 1: hb_barrier (); واپس u.aat.create_accelerator_ڈیٹا (عدد_glyphs);
#اگر_ختم
    طےشدہ:واپس AAT::kern_accelerator_ڈیٹا_t ();
    }
  }

  ساخت accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      یہ->جدول = sc.reference_جدول<kern> (face);
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

    hb_blob_ptr_t<kern> جدول;
    AAT::kern_accelerator_ڈیٹا_t accel_ڈیٹا;
    AAT::hb_aat_scratch_t scratch;
  };

  محفوظ:
  اتحاد {
  ساخت { HBUINT32 v; }	version32;
  ساخت { HBUINT16 v; }	major;
  KernOT		ot;
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  KernAAT		aat;
#اگر_ختم
  } u;
  عوامی:
  DEFINE_سائز_UNION (4, version32.v);
};

ساخت kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#اگر_ختم /* HB_OT_KERN_TABLE_HH */
