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

#ifndef HB_OT_KERN_TABLE_HH
#define HB_OT_KERN_TABLE_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#define HB_OT_TAG_kern HB_TAG('k','e','r','n')


نطاق OT {


template <typename KernSubTableHeader>
بنية KernSubTableFormat3
{
  صحيح get_kerning (hb_codepoint_t left, hb_codepoint_t ئوڭ) ثابت
  {
    hb_array_t<ثابت FWORD> kern_قىممەت = kern_قىممەت_Z.as_array (kern_قىممەت_سانى);
    hb_array_t<ثابت HBUINT8> سول_Class = StructAfter<ثابت UnsizedArrayOf<HBUINT8>> (kern_قىممەت).as_array (glyph_سانى_2);
    hb_array_t<ثابت HBUINT8> ئوڭ_Class = StructAfter<ثابت UnsizedArrayOf<HBUINT8>> (سول_Class).as_array (glyph_سانى_2);
    hb_array_t<ثابت HBUINT8> kernIndex = StructAfter<ثابت UnsizedArrayOf<HBUINT8>> (ئوڭ_Class).as_array (سول_Class_سانى * ئوڭ_Class_سانى);

    unsigned صحيح سول_C = سول_Class[left];
    unsigned صحيح ئوڭ_C = ئوڭ_Class[ئوڭ];
    إذا (unlikely (سول_C >= سول_Class_سانى || ئوڭ_C >= ئوڭ_Class_سانى))
      أرجع 0;
    unsigned صحيح i = سول_C * ئوڭ_Class_سانى + ئوڭ_C;
    أرجع kern_قىممەت[kernIndex[i]];
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      أرجع خطأ;

    إذا (header.coverage & header.Backwards)
      أرجع خطأ;

    hb_kern_machine_t<KernSubTableFormat3> machine (*هذا, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (kern_قىممەت_Z,
				  kern_قىممەت_سانى * حجم (FWORD) +
				  glyph_سانى_2 * 2 +
				  سول_Class_سانى * ئوڭ_Class_سانى));
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &سول_set, set_t &ئوڭ_set, unsigned رەقەم_glyphs) ثابت
  {
    إذا (likely (glyph_سانى_2))
    {
      سول_set.add_range (0, رەقەم_glyphs - 1);
      ئوڭ_set.add_range (0, رەقەم_glyphs - 1);
    }
  }

  محمي:
  KernSubTableHeader
		header;
  HBUINT16	glyph_سانى_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_قىممەت_سانى;	/* The number of kerning values. */
  HBUINT8	سول_Class_سانى;	/* The number of left-hand classes. */
  HBUINT8	ئوڭ_Class_سانى;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  UnsizedArrayOf<FWORD>
		kern_قىممەت_Z;	/* The kerning values.
				 * Length kernValueCount. */
#إذا 0
  UnsizedArrayOf<HBUINT8>
		سول_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>
		ئوڭ_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>kernIndex;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#endif
  عام:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 6, kern_قىممەت_Z);
};

template <typename KernSubTableHeader>
بنية KernSubTable
{
  size_t get_size () ثابت { أرجع u.header.length; }
  unsigned صحيح get_type () ثابت { أرجع u.header.format; }

  صحيح get_kerning (hb_codepoint_t left, hb_codepoint_t ئوڭ) ثابت
  {
    بدل (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    حالة 0: hb_barrier (); أرجع u.format0.get_kerning (left, ئوڭ);
    افتراضي:أرجع 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    unsigned صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ifndef HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#endif
    حالة 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ifndef HB_NO_AAT_SHAPE
    حالة 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#endif
    افتراضي:	return_trace (c->default_return_قىممەت ());
    }
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &سول_set, set_t &ئوڭ_set, unsigned رەقەم_glyphs) ثابت
  {
    unsigned صحيح subtable_type = get_type ();
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); u.format0.collect_glyphs (سول_set, ئوڭ_set, رەقەم_glyphs); أرجع;
#ifndef HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); u.format1.collect_glyphs (سول_set, ئوڭ_set, رەقەم_glyphs); أرجع;
#endif
    حالة 2:	hb_barrier (); u.format2.collect_glyphs (سول_set, ئوڭ_set, رەقەم_glyphs); أرجع;
#ifndef HB_NO_AAT_SHAPE
    حالة 3:	hb_barrier (); u.format3.collect_glyphs (سول_set, ئوڭ_set, رەقەم_glyphs); أرجع;
#endif
    افتراضي:	أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.length >= u.header.min_size &&
		    c->check_range (هذا, u.header.length)))) return_trace (خطأ);

    return_trace (dispatch (c));
  }

  عام:
  اتحاد {
  KernSubTableHeader				header;
  AAT::KerxSubTableFormat0<KernSubTableHeader>	format0;
#ifndef HB_NO_AAT_SHAPE
  AAT::KerxSubTableFormat1<KernSubTableHeader>	format1;
#endif
  AAT::KerxSubTableFormat2<KernSubTableHeader>	format2;
#ifndef HB_NO_AAT_SHAPE
  KernSubTableFormat3<KernSubTableHeader>	format3;
#endif
  } u;
  عام:
  DEFINE_SIZE_MIN (KernSubTableHeader::static_size);
};


بنية KernOTSubTableHeader
{
  ساكن constexpr منطقي apple = خطأ;
  تعريفنوع AAT::ObsoleteTypes Types;

  unsigned   tuple_سانى_2 () ثابت { أرجع 0; }
  منطقي     is_horizontal () ثابت { أرجع (coverage & Horizontal); }

  تعداد Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    CrossStream	= 0x04u,
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
  HBUINT16	versionZ;	/* Unused. */
  HBUINT16	length;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية KernOT : AAT::KerxTable<KernOT>
{
  friend بنية AAT::KerxTable<KernOT>;

  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  ساكن constexpr unsigned minVersion = 0u;

  تعريفنوع KernOTSubTableHeader SubTableHeader;
  تعريفنوع SubTableHeader::Types Types;
  تعريفنوع KernSubTable<SubTableHeader> SubTable;

  محمي:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_سانى;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  عام:
  DEFINE_SIZE_MIN (4);
};


بنية KernAATSubTableHeader
{
  ساكن constexpr منطقي apple = true;
  تعريفنوع AAT::ObsoleteTypes Types;

  unsigned   tuple_سانى_2 () ثابت { أرجع 0; }
  منطقي     is_horizontal () ثابت { أرجع !(coverage & Vertical); }

  تعداد Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
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
  HBUINT32	length;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tupleIndex;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية KernAAT : AAT::KerxTable<KernAAT>
{
  friend بنية AAT::KerxTable<KernAAT>;

  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  ساكن constexpr unsigned minVersion = 0x00010000u;

  تعريفنوع KernAATSubTableHeader SubTableHeader;
  تعريفنوع SubTableHeader::Types Types;
  تعريفنوع KernSubTable<SubTableHeader> SubTable;

  محمي:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_سانى;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  عام:
  DEFINE_SIZE_MIN (8);
};

بنية kern
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  منطقي     has_data () ثابت { أرجع u.version32.v; }
  unsigned get_type () ثابت { أرجع u.major.v; }

  منطقي has_state_machine () ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.has_state_machine ();
#ifndef HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.has_state_machine ();
#endif
    افتراضي:أرجع خطأ;
    }
  }

  منطقي has_cross_stream () ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.has_cross_stream ();
#ifndef HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.has_cross_stream ();
#endif
    افتراضي:أرجع خطأ;
    }
  }

  صحيح get_h_kerning (hb_codepoint_t left, hb_codepoint_t ئوڭ) ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.get_h_kerning (left, ئوڭ);
#ifndef HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.get_h_kerning (left, ئوڭ);
#endif
    افتراضي:أرجع 0;
    }
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c,
	      ثابت AAT::kern_accelerator_data_t &accel_data) ثابت
  { أرجع dispatch (c, accel_data); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    unsigned صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ifndef HB_NO_AAT_SHAPE
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#endif
    افتراضي:	return_trace (c->default_return_قىممەت ());
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.version32.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_data_t create_accelerator_data (unsigned رەقەم_glyphs) ثابت
  {
    بدل (get_type ()) {
    حالة 0: hb_barrier (); أرجع u.ot.create_accelerator_data (رەقەم_glyphs);
#ifndef HB_NO_AAT_SHAPE
    حالة 1: hb_barrier (); أرجع u.aat.create_accelerator_data (رەقەم_glyphs);
#endif
    افتراضي:أرجع AAT::kern_accelerator_data_t ();
    }
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->table = sc.reference_table<kern> (face);
      هذا->accel_data = هذا->table->create_accelerator_data (face->get_رەقەم_glyphs ());
    }
    ~accelerator_t ()
    {
      هذا->table.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع table.get_blob (); }

    منطقي apply (AAT::hb_aat_apply_context_t *c) ثابت
    {
      أرجع table->apply (c, accel_data);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_data_t accel_data;
    AAT::hb_aat_scratch_t scratch;
  };

  محمي:
  اتحاد {
  بنية { HBUINT32 v; }	version32;
  بنية { HBUINT16 v; }	major;
  KernOT		ot;
#ifndef HB_NO_AAT_SHAPE
  KernAAT		aat;
#endif
  } u;
  عام:
  DEFINE_SIZE_UNION (4, version32.v);
};

بنية kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_KERN_TABLE_HH */
