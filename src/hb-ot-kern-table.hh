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

#অন্তর্ভাব "hb-aat-layout-common.hh"
#অন্তর্ভাব "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#define HB_OT_TAG_kern HB_TAG('k','e','r','n')


নামাকাশ OT {


template <typename KernSubTableHeader>
কাঠামো KernSubTableFormat3
{
  পূর্ণসংখ্যা get_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুবক
  {
    hb_array_t<ধ্রুবক FWORD> kern_মান = kern_মান_Z.as_array (kern_মান_গুনতি);
    hb_array_t<ধ্রুবক HBUINT8> বাম_Class = StructAfter<ধ্রুবক UnsizedArrayOf<HBUINT8>> (kern_মান).as_array (glyph_গুনতি_2);
    hb_array_t<ধ্রুবক HBUINT8> ডান_Class = StructAfter<ধ্রুবক UnsizedArrayOf<HBUINT8>> (বাম_Class).as_array (glyph_গুনতি_2);
    hb_array_t<ধ্রুবক HBUINT8> kern_সূচক = StructAfter<ধ্রুবক UnsizedArrayOf<HBUINT8>> (ডান_Class).as_array (বাম_Class_গুনতি * ডান_Class_গুনতি);

    unsigned পূর্ণসংখ্যা বাম_C = বাম_Class[left];
    unsigned পূর্ণসংখ্যা ডান_C = ডান_Class[ডান];
    যদি (unlikely (বাম_C >= বাম_Class_গুনতি || ডান_C >= ডান_Class_গুনতি))
      ফেরত 0;
    unsigned পূর্ণসংখ্যা i = বাম_C * ডান_Class_গুনতি + ডান_C;
    ফেরত kern_মান[kern_সূচক[i]];
  }

  যৌক্তিক apply (AAT::hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    যদি (!c->plan->requested_kerning)
      ফেরত মিথ্যা;

    যদি (header.coverage & header.Backwards)
      ফেরত মিথ্যা;

    hb_kern_machine_t<KernSubTableFormat3> machine (*এই, header.coverage & header.CrossStream);
    machine.kern (c->font, c->বাফার, c->plan->kern_mask);

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  c->check_range (kern_মান_Z,
				  kern_মান_গুনতি * আকার (FWORD) +
				  glyph_গুনতি_2 * 2 +
				  বাম_Class_গুনতি * ডান_Class_গুনতি));
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &বাম_set, set_t &ডান_set, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    যদি (likely (glyph_গুনতি_2))
    {
      বাম_set.add_range (0, অঙ্ক_glyphs - 1);
      ডান_set.add_range (0, অঙ্ক_glyphs - 1);
    }
  }

  সুরক্ষিত:
  KernSubTableHeader
		header;
  HBUINT16	glyph_গুনতি_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_মান_গুনতি;	/* The number of kerning values. */
  HBUINT8	বাম_Class_গুনতি;	/* The number of left-hand classes. */
  HBUINT8	ডান_Class_গুনতি;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  UnsizedArrayOf<FWORD>
		kern_মান_Z;	/* The kerning values.
				 * Length kernValueCount. */
#যদি 0
  UnsizedArrayOf<HBUINT8>
		বাম_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>
		ডান_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>kern_সূচক;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#endif
  সর্বজনীন:
  DEFINE_আকার_ARRAY (KernSubTableHeader::static_আকার + 6, kern_মান_Z);
};

template <typename KernSubTableHeader>
কাঠামো KernSubTable
{
  size_t get_আকার () ধ্রুবক { ফেরত u.header.দৈর্ঘ্য; }
  unsigned পূর্ণসংখ্যা get_type () ধ্রুবক { ফেরত u.header.format; }

  পূর্ণসংখ্যা get_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুবক
  {
    নির্বাচন (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    ক্ষেত্র 0: hb_barrier (); ফেরত u.format0.get_kerning (left, ডান);
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা subtable_type = get_type ();
    TRACE_DISPATCH (এই, subtable_type);
    নির্বাচন (subtable_type) {
    ক্ষেত্র 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ifndef HB_NO_AAT_SHAPE
    ক্ষেত্র 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#endif
    ক্ষেত্র 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ifndef HB_NO_AAT_SHAPE
    ক্ষেত্র 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#endif
    পূর্বনির্ধারিত:	return_trace (c->default_return_মান ());
    }
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &বাম_set, set_t &ডান_set, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা subtable_type = get_type ();
    নির্বাচন (subtable_type) {
    ক্ষেত্র 0:	hb_barrier (); u.format0.collect_glyphs (বাম_set, ডান_set, অঙ্ক_glyphs); ফেরত;
#ifndef HB_NO_AAT_SHAPE
    ক্ষেত্র 1:	hb_barrier (); u.format1.collect_glyphs (বাম_set, ডান_set, অঙ্ক_glyphs); ফেরত;
#endif
    ক্ষেত্র 2:	hb_barrier (); u.format2.collect_glyphs (বাম_set, ডান_set, অঙ্ক_glyphs); ফেরত;
#ifndef HB_NO_AAT_SHAPE
    ক্ষেত্র 3:	hb_barrier (); u.format3.collect_glyphs (বাম_set, ডান_set, অঙ্ক_glyphs); ফেরত;
#endif
    পূর্বনির্ধারিত:	ফেরত;
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.দৈর্ঘ্য >= u.header.min_আকার &&
		    c->check_range (এই, u.header.দৈর্ঘ্য)))) return_trace (মিথ্যা);

    return_trace (dispatch (c));
  }

  সর্বজনীন:
  সংঘ {
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
  সর্বজনীন:
  DEFINE_আকার_MIN (KernSubTableHeader::static_আকার);
};


কাঠামো KernOTSubTableHeader
{
  স্থির constexpr যৌক্তিক apple = মিথ্যা;
  প্রকারলেখ AAT::ObsoleteTypes Types;

  unsigned   tuple_গুনতি_2 () ধ্রুবক { ফেরত 0; }
  যৌক্তিক     is_horizontal () ধ্রুবক { ফেরত (coverage & Horizontal); }

  গণনা Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    CrossStream	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBUINT16	versionZ;	/* Unused. */
  HBUINT16	দৈর্ঘ্য;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো KernOT : AAT::KerxTable<KernOT>
{
  friend কাঠামো AAT::KerxTable<KernOT>;

  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  স্থির constexpr unsigned minVersion = 0u;

  প্রকারলেখ KernOTSubTableHeader SubTableHeader;
  প্রকারলেখ SubTableHeader::Types Types;
  প্রকারলেখ KernSubTable<SubTableHeader> SubTable;

  সুরক্ষিত:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_গুনতি;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  সর্বজনীন:
  DEFINE_আকার_MIN (4);
};


কাঠামো KernAATSubTableHeader
{
  স্থির constexpr যৌক্তিক apple = সত্য;
  প্রকারলেখ AAT::ObsoleteTypes Types;

  unsigned   tuple_গুনতি_2 () ধ্রুবক { ফেরত 0; }
  যৌক্তিক     is_horizontal () ধ্রুবক { ফেরত !(coverage & Vertical); }

  গণনা Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBUINT32	দৈর্ঘ্য;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tuple_সূচক;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো KernAAT : AAT::KerxTable<KernAAT>
{
  friend কাঠামো AAT::KerxTable<KernAAT>;

  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  স্থির constexpr unsigned minVersion = 0x00010000u;

  প্রকারলেখ KernAATSubTableHeader SubTableHeader;
  প্রকারলেখ SubTableHeader::Types Types;
  প্রকারলেখ KernSubTable<SubTableHeader> SubTable;

  সুরক্ষিত:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_গুনতি;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  সর্বজনীন:
  DEFINE_আকার_MIN (8);
};

কাঠামো kern
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  যৌক্তিক     has_তথ্য () ধ্রুবক { ফেরত u.version32.v; }
  unsigned get_type () ধ্রুবক { ফেরত u.major.v; }

  যৌক্তিক has_state_machine () ধ্রুবক
  {
    নির্বাচন (get_type ()) {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.ot.has_state_machine ();
#ifndef HB_NO_AAT_SHAPE
    ক্ষেত্র 1: hb_barrier (); ফেরত u.aat.has_state_machine ();
#endif
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }

  যৌক্তিক has_cross_stream () ধ্রুবক
  {
    নির্বাচন (get_type ()) {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.ot.has_cross_stream ();
#ifndef HB_NO_AAT_SHAPE
    ক্ষেত্র 1: hb_barrier (); ফেরত u.aat.has_cross_stream ();
#endif
    পূর্বনির্ধারিত:ফেরত মিথ্যা;
    }
  }

  পূর্ণসংখ্যা get_h_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুবক
  {
    নির্বাচন (get_type ()) {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.ot.get_h_kerning (left, ডান);
#ifndef HB_NO_AAT_SHAPE
    ক্ষেত্র 1: hb_barrier (); ফেরত u.aat.get_h_kerning (left, ডান);
#endif
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  যৌক্তিক apply (AAT::hb_aat_apply_context_t *c,
	      ধ্রুবক AAT::kern_accelerator_তথ্য_t &accel_তথ্য) ধ্রুবক
  { ফেরত dispatch (c, accel_তথ্য); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা subtable_type = get_type ();
    TRACE_DISPATCH (এই, subtable_type);
    নির্বাচন (subtable_type) {
    ক্ষেত্র 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ifndef HB_NO_AAT_SHAPE
    ক্ষেত্র 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#endif
    পূর্বনির্ধারিত:	return_trace (c->default_return_মান ());
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.version32.v.sanitize (c)) return_trace (মিথ্যা);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_তথ্য_t create_accelerator_তথ্য (unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    নির্বাচন (get_type ()) {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.ot.create_accelerator_তথ্য (অঙ্ক_glyphs);
#ifndef HB_NO_AAT_SHAPE
    ক্ষেত্র 1: hb_barrier (); ফেরত u.aat.create_accelerator_তথ্য (অঙ্ক_glyphs);
#endif
    পূর্বনির্ধারিত:ফেরত AAT::kern_accelerator_তথ্য_t ();
    }
  }

  কাঠামো accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      এই->table = sc.reference_table<kern> (face);
      এই->accel_তথ্য = এই->table->create_accelerator_তথ্য (face->get_অঙ্ক_glyphs ());
    }
    ~accelerator_t ()
    {
      এই->table.destroy ();
    }

    hb_blob_t *get_blob () ধ্রুবক { ফেরত table.get_blob (); }

    যৌক্তিক apply (AAT::hb_aat_apply_context_t *c) ধ্রুবক
    {
      ফেরত table->apply (c, accel_তথ্য);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_তথ্য_t accel_তথ্য;
    AAT::hb_aat_scratch_t scratch;
  };

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT32 v; }	version32;
  কাঠামো { HBUINT16 v; }	major;
  KernOT		ot;
#ifndef HB_NO_AAT_SHAPE
  KernAAT		aat;
#endif
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (4, version32.v);
};

কাঠামো kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_KERN_TABLE_HH */
