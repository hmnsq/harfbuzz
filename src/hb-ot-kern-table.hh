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
সংরচনা KernSubTableFormat3
{
  পূর্ণাঙ্ক get_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুব
  {
    hb_array_t<ধ্রুব FWORD> kern_মান = kern_মান_Z.as_array (kern_মান_গুনতি);
    hb_array_t<ধ্রুব HBUINT8> বাম_Class = StructAfter<ধ্রুব UnsizedArrayOf<HBUINT8>> (kern_মান).as_array (glyph_গুনতি_2);
    hb_array_t<ধ্রুব HBUINT8> ডান_Class = StructAfter<ধ্রুব UnsizedArrayOf<HBUINT8>> (বাম_Class).as_array (glyph_গুনতি_2);
    hb_array_t<ধ্রুব HBUINT8> kernIndex = StructAfter<ধ্রুব UnsizedArrayOf<HBUINT8>> (ডান_Class).as_array (বাম_Class_গুনতি * ডান_Class_গুনতি);

    unsigned পূর্ণাঙ্ক বাম_C = বাম_Class[left];
    unsigned পূর্ণাঙ্ক ডান_C = ডান_Class[ডান];
    যদি (unlikely (বাম_C >= বাম_Class_গুনতি || ডান_C >= ডান_Class_গুনতি))
      প্রত্যাগম 0;
    unsigned পূর্ণাঙ্ক i = বাম_C * ডান_Class_গুনতি + ডান_C;
    প্রত্যাগম kern_মান[kernIndex[i]];
  }

  তার্কিক apply (AAT::hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    যদি (!c->plan->requested_kerning)
      প্রত্যাগম অসত্য;

    যদি (header.coverage & header.Backwards)
      প্রত্যাগম অসত্য;

    hb_kern_machine_t<KernSubTableFormat3> machine (*এতত্, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (সত্য);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  c->check_range (kern_মান_Z,
				  kern_মান_গুনতি * পরিমাণ (FWORD) +
				  glyph_গুনতি_2 * 2 +
				  বাম_Class_গুনতি * ডান_Class_গুনতি));
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &বাম_set, set_t &ডান_set, unsigned অঙ্ক_glyphs) ধ্রুব
  {
    যদি (likely (glyph_গুনতি_2))
    {
      বাম_set.add_range (0, অঙ্ক_glyphs - 1);
      ডান_set.add_range (0, অঙ্ক_glyphs - 1);
    }
  }

  রক্ষিত:
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
  UnsizedArrayOf<HBUINT8>kernIndex;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#endif
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 6, kern_মান_Z);
};

template <typename KernSubTableHeader>
সংরচনা KernSubTable
{
  size_t get_size () ধ্রুব { প্রত্যাগম u.header.length; }
  unsigned পূর্ণাঙ্ক get_type () ধ্রুব { প্রত্যাগম u.header.format; }

  পূর্ণাঙ্ক get_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুব
  {
    বিকল্প (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    পক্ষ 0: hb_barrier (); প্রত্যাগম u.format0.get_kerning (left, ডান);
    স্বভাব:প্রত্যাগম 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক subtable_type = get_type ();
    TRACE_DISPATCH (এতত্, subtable_type);
    বিকল্প (subtable_type) {
    পক্ষ 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ifndef HB_NO_AAT_SHAPE
    পক্ষ 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#endif
    পক্ষ 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ifndef HB_NO_AAT_SHAPE
    পক্ষ 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#endif
    স্বভাব:	return_trace (c->default_return_মান ());
    }
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &বাম_set, set_t &ডান_set, unsigned অঙ্ক_glyphs) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক subtable_type = get_type ();
    বিকল্প (subtable_type) {
    পক্ষ 0:	hb_barrier (); u.format0.collect_glyphs (বাম_set, ডান_set, অঙ্ক_glyphs); প্রত্যাগম;
#ifndef HB_NO_AAT_SHAPE
    পক্ষ 1:	hb_barrier (); u.format1.collect_glyphs (বাম_set, ডান_set, অঙ্ক_glyphs); প্রত্যাগম;
#endif
    পক্ষ 2:	hb_barrier (); u.format2.collect_glyphs (বাম_set, ডান_set, অঙ্ক_glyphs); প্রত্যাগম;
#ifndef HB_NO_AAT_SHAPE
    পক্ষ 3:	hb_barrier (); u.format3.collect_glyphs (বাম_set, ডান_set, অঙ্ক_glyphs); প্রত্যাগম;
#endif
    স্বভাব:	প্রত্যাগম;
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.length >= u.header.min_size &&
		    c->check_range (এতত্, u.header.length)))) return_trace (অসত্য);

    return_trace (dispatch (c));
  }

  সার্বজনিক:
  সঙ্ঘ {
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
  সার্বজনিক:
  DEFINE_SIZE_MIN (KernSubTableHeader::static_size);
};


সংরচনা KernOTSubTableHeader
{
  স্থির constexpr তার্কিক apple = অসত্য;
  প্রকারনির্দেশ AAT::ObsoleteTypes Types;

  unsigned   tuple_গুনতি_2 () ধ্রুব { প্রত্যাগম 0; }
  তার্কিক     is_horizontal () ধ্রুব { প্রত্যাগম (coverage & Horizontal); }

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

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্বজনিক:
  HBUINT16	versionZ;	/* Unused. */
  HBUINT16	length;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা KernOT : AAT::KerxTable<KernOT>
{
  friend সংরচনা AAT::KerxTable<KernOT>;

  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  স্থির constexpr unsigned minVersion = 0u;

  প্রকারনির্দেশ KernOTSubTableHeader SubTableHeader;
  প্রকারনির্দেশ SubTableHeader::Types Types;
  প্রকারনির্দেশ KernSubTable<SubTableHeader> SubTable;

  রক্ষিত:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_গুনতি;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  সার্বজনিক:
  DEFINE_SIZE_MIN (4);
};


সংরচনা KernAATSubTableHeader
{
  স্থির constexpr তার্কিক apple = সত্য;
  প্রকারনির্দেশ AAT::ObsoleteTypes Types;

  unsigned   tuple_গুনতি_2 () ধ্রুব { প্রত্যাগম 0; }
  তার্কিক     is_horizontal () ধ্রুব { প্রত্যাগম !(coverage & Vertical); }

  গণনা Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্বজনিক:
  HBUINT32	length;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tupleIndex;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা KernAAT : AAT::KerxTable<KernAAT>
{
  friend সংরচনা AAT::KerxTable<KernAAT>;

  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  স্থির constexpr unsigned minVersion = 0x00010000u;

  প্রকারনির্দেশ KernAATSubTableHeader SubTableHeader;
  প্রকারনির্দেশ SubTableHeader::Types Types;
  প্রকারনির্দেশ KernSubTable<SubTableHeader> SubTable;

  রক্ষিত:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_গুনতি;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  সার্বজনিক:
  DEFINE_SIZE_MIN (8);
};

সংরচনা kern
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  তার্কিক     has_data () ধ্রুব { প্রত্যাগম u.version32.v; }
  unsigned get_type () ধ্রুব { প্রত্যাগম u.major.v; }

  তার্কিক has_state_machine () ধ্রুব
  {
    বিকল্প (get_type ()) {
    পক্ষ 0: hb_barrier (); প্রত্যাগম u.ot.has_state_machine ();
#ifndef HB_NO_AAT_SHAPE
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.aat.has_state_machine ();
#endif
    স্বভাব:প্রত্যাগম অসত্য;
    }
  }

  তার্কিক has_cross_stream () ধ্রুব
  {
    বিকল্প (get_type ()) {
    পক্ষ 0: hb_barrier (); প্রত্যাগম u.ot.has_cross_stream ();
#ifndef HB_NO_AAT_SHAPE
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.aat.has_cross_stream ();
#endif
    স্বভাব:প্রত্যাগম অসত্য;
    }
  }

  পূর্ণাঙ্ক get_h_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুব
  {
    বিকল্প (get_type ()) {
    পক্ষ 0: hb_barrier (); প্রত্যাগম u.ot.get_h_kerning (left, ডান);
#ifndef HB_NO_AAT_SHAPE
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.aat.get_h_kerning (left, ডান);
#endif
    স্বভাব:প্রত্যাগম 0;
    }
  }

  তার্কিক apply (AAT::hb_aat_apply_context_t *c,
	      ধ্রুব AAT::kern_accelerator_data_t &accel_data) ধ্রুব
  { প্রত্যাগম dispatch (c, accel_data); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক subtable_type = get_type ();
    TRACE_DISPATCH (এতত্, subtable_type);
    বিকল্প (subtable_type) {
    পক্ষ 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ifndef HB_NO_AAT_SHAPE
    পক্ষ 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#endif
    স্বভাব:	return_trace (c->default_return_মান ());
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.version32.v.sanitize (c)) return_trace (অসত্য);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_data_t create_accelerator_data (unsigned অঙ্ক_glyphs) ধ্রুব
  {
    বিকল্প (get_type ()) {
    পক্ষ 0: hb_barrier (); প্রত্যাগম u.ot.create_accelerator_data (অঙ্ক_glyphs);
#ifndef HB_NO_AAT_SHAPE
    পক্ষ 1: hb_barrier (); প্রত্যাগম u.aat.create_accelerator_data (অঙ্ক_glyphs);
#endif
    স্বভাব:প্রত্যাগম AAT::kern_accelerator_data_t ();
    }
  }

  সংরচনা accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      এতত্->table = sc.reference_table<kern> (face);
      এতত্->accel_data = এতত্->table->create_accelerator_data (face->get_অঙ্ক_glyphs ());
    }
    ~accelerator_t ()
    {
      এতত্->table.destroy ();
    }

    hb_blob_t *get_blob () ধ্রুব { প্রত্যাগম table.get_blob (); }

    তার্কিক apply (AAT::hb_aat_apply_context_t *c) ধ্রুব
    {
      প্রত্যাগম table->apply (c, accel_data);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_data_t accel_data;
    AAT::hb_aat_scratch_t scratch;
  };

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT32 v; }	version32;
  সংরচনা { HBUINT16 v; }	major;
  KernOT		ot;
#ifndef HB_NO_AAT_SHAPE
  KernAAT		aat;
#endif
  } u;
  সার্বজনিক:
  DEFINE_SIZE_UNION (4, version32.v);
};

সংরচনা kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_KERN_TABLE_HH */
