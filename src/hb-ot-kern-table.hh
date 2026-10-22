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

#ಅನ್ತರ್ಭಾವ "hb-aat-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#define HB_OT_TAG_kern HB_TAG('k','e','r','n')


ನಾಮಾಕಾಶ OT {


template <typename KernSubTableHeader>
ಸಂರಚನಾ KernSubTableFormat3
{
  ಪೂರ್ಣಾಙ್ಕ get_kerning (hb_codepoint_t left, hb_codepoint_t ಬಲ) ಧ್ರುವ
  {
    hb_array_t<ಧ್ರುವ FWORD> kern_ಮೌಲ್ಯ = kern_ಮೌಲ್ಯ_Z.as_array (kern_ಮೌಲ್ಯ_ಎಣಿಕೆ);
    hb_array_t<ಧ್ರುವ HBUINT8> ಎಡ_Class = StructAfter<ಧ್ರುವ UnsizedArrayOf<HBUINT8>> (kern_ಮೌಲ್ಯ).as_array (glyph_ಎಣಿಕೆ_2);
    hb_array_t<ಧ್ರುವ HBUINT8> ಬಲ_Class = StructAfter<ಧ್ರುವ UnsizedArrayOf<HBUINT8>> (ಎಡ_Class).as_array (glyph_ಎಣಿಕೆ_2);
    hb_array_t<ಧ್ರುವ HBUINT8> kernIndex = StructAfter<ಧ್ರುವ UnsizedArrayOf<HBUINT8>> (ಬಲ_Class).as_array (ಎಡ_Class_ಎಣಿಕೆ * ಬಲ_Class_ಎಣಿಕೆ);

    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಡ_C = ಎಡ_Class[left];
    unsigned ಪೂರ್ಣಾಙ್ಕ ಬಲ_C = ಬಲ_Class[ಬಲ];
    ಯದಿ (unlikely (ಎಡ_C >= ಎಡ_Class_ಎಣಿಕೆ || ಬಲ_C >= ಬಲ_Class_ಎಣಿಕೆ))
      ಪ್ರತ್ಯಾಗಮ 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ i = ಎಡ_C * ಬಲ_Class_ಎಣಿಕೆ + ಬಲ_C;
    ಪ್ರತ್ಯಾಗಮ kern_ಮೌಲ್ಯ[kernIndex[i]];
  }

  ತಾರ್ಕಿಕ apply (AAT::hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    ಯದಿ (!c->plan->requested_kerning)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (header.coverage & header.Backwards)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    hb_kern_machine_t<KernSubTableFormat3> machine (*ಏತತ್, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  c->check_range (kern_ಮೌಲ್ಯ_Z,
				  kern_ಮೌಲ್ಯ_ಎಣಿಕೆ * ಪರಿಮಾಣ (FWORD) +
				  glyph_ಎಣಿಕೆ_2 * 2 +
				  ಎಡ_Class_ಎಣಿಕೆ * ಬಲ_Class_ಎಣಿಕೆ));
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &ಎಡ_set, set_t &ಬಲ_set, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಯದಿ (likely (glyph_ಎಣಿಕೆ_2))
    {
      ಎಡ_set.add_range (0, ಅಂಕೆ_glyphs - 1);
      ಬಲ_set.add_range (0, ಅಂಕೆ_glyphs - 1);
    }
  }

  ರಕ್ಷಿತ:
  KernSubTableHeader
		header;
  HBUINT16	glyph_ಎಣಿಕೆ_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_ಮೌಲ್ಯ_ಎಣಿಕೆ;	/* The number of kerning values. */
  HBUINT8	ಎಡ_Class_ಎಣಿಕೆ;	/* The number of left-hand classes. */
  HBUINT8	ಬಲ_Class_ಎಣಿಕೆ;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  UnsizedArrayOf<FWORD>
		kern_ಮೌಲ್ಯ_Z;	/* The kerning values.
				 * Length kernValueCount. */
#ಯದಿ 0
  UnsizedArrayOf<HBUINT8>
		ಎಡ_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>
		ಬಲ_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>kernIndex;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#endif
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 6, kern_ಮೌಲ್ಯ_Z);
};

template <typename KernSubTableHeader>
ಸಂರಚನಾ KernSubTable
{
  size_t get_size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ u.header.length; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_type () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ u.header.format; }

  ಪೂರ್ಣಾಙ್ಕ get_kerning (hb_codepoint_t left, hb_codepoint_t ಬಲ) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_kerning (left, ಬಲ);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ subtable_type = get_type ();
    TRACE_DISPATCH (ಏತತ್, subtable_type);
    ವಿಕಲ್ಪ (subtable_type) {
    ಪಕ್ಷ 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ifndef HB_NO_AAT_SHAPE
    ಪಕ್ಷ 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#endif
    ಪಕ್ಷ 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ifndef HB_NO_AAT_SHAPE
    ಪಕ್ಷ 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#endif
    ಸ್ವಭಾವ:	return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &ಎಡ_set, set_t &ಬಲ_set, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ subtable_type = get_type ();
    ವಿಕಲ್ಪ (subtable_type) {
    ಪಕ್ಷ 0:	hb_barrier (); u.format0.collect_glyphs (ಎಡ_set, ಬಲ_set, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
#ifndef HB_NO_AAT_SHAPE
    ಪಕ್ಷ 1:	hb_barrier (); u.format1.collect_glyphs (ಎಡ_set, ಬಲ_set, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
#endif
    ಪಕ್ಷ 2:	hb_barrier (); u.format2.collect_glyphs (ಎಡ_set, ಬಲ_set, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
#ifndef HB_NO_AAT_SHAPE
    ಪಕ್ಷ 3:	hb_barrier (); u.format3.collect_glyphs (ಎಡ_set, ಬಲ_set, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
#endif
    ಸ್ವಭಾವ:	ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.length >= u.header.min_size &&
		    c->check_range (ಏತತ್, u.header.length)))) return_trace (ಅಸತ್ಯ);

    return_trace (dispatch (c));
  }

  ಸಾರ್ವಜನಿಕ:
  ಸಙ್ಘ {
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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (KernSubTableHeader::static_size);
};


ಸಂರಚನಾ KernOTSubTableHeader
{
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ apple = ಅಸತ್ಯ;
  ಪ್ರಕಾರನಿರ್ದೇಶ AAT::ObsoleteTypes Types;

  unsigned   tuple_ಎಣಿಕೆ_2 () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ 0; }
  ತಾರ್ಕಿಕ     is_horizontal () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (coverage & Horizontal); }

  ಗಣನಾ Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    CrossStream	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT16	versionZ;	/* Unused. */
  HBUINT16	length;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ KernOT : AAT::KerxTable<KernOT>
{
  friend ಸಂರಚನಾ AAT::KerxTable<KernOT>;

  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  ಸ್ಥಿರ constexpr unsigned minVersion = 0u;

  ಪ್ರಕಾರನಿರ್ದೇಶ KernOTSubTableHeader SubTableHeader;
  ಪ್ರಕಾರನಿರ್ದೇಶ SubTableHeader::Types Types;
  ಪ್ರಕಾರನಿರ್ದೇಶ KernSubTable<SubTableHeader> SubTable;

  ರಕ್ಷಿತ:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_ಎಣಿಕೆ;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (4);
};


ಸಂರಚನಾ KernAATSubTableHeader
{
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ apple = ಸತ್ಯ;
  ಪ್ರಕಾರನಿರ್ದೇಶ AAT::ObsoleteTypes Types;

  unsigned   tuple_ಎಣಿಕೆ_2 () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ 0; }
  ತಾರ್ಕಿಕ     is_horizontal () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !(coverage & Vertical); }

  ಗಣನಾ Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT32	length;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tupleIndex;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ KernAAT : AAT::KerxTable<KernAAT>
{
  friend ಸಂರಚನಾ AAT::KerxTable<KernAAT>;

  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  ಸ್ಥಿರ constexpr unsigned minVersion = 0x00010000u;

  ಪ್ರಕಾರನಿರ್ದೇಶ KernAATSubTableHeader SubTableHeader;
  ಪ್ರಕಾರನಿರ್ದೇಶ SubTableHeader::Types Types;
  ಪ್ರಕಾರನಿರ್ದೇಶ KernSubTable<SubTableHeader> SubTable;

  ರಕ್ಷಿತ:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_ಎಣಿಕೆ;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (8);
};

ಸಂರಚನಾ kern
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  ತಾರ್ಕಿಕ     has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ u.version32.v; }
  unsigned get_type () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ u.major.v; }

  ತಾರ್ಕಿಕ has_state_machine () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (get_type ()) {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.ot.has_state_machine ();
#ifndef HB_NO_AAT_SHAPE
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.aat.has_state_machine ();
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  ತಾರ್ಕಿಕ has_cross_stream () ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (get_type ()) {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.ot.has_cross_stream ();
#ifndef HB_NO_AAT_SHAPE
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.aat.has_cross_stream ();
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  ಪೂರ್ಣಾಙ್ಕ get_h_kerning (hb_codepoint_t left, hb_codepoint_t ಬಲ) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (get_type ()) {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.ot.get_h_kerning (left, ಬಲ);
#ifndef HB_NO_AAT_SHAPE
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.aat.get_h_kerning (left, ಬಲ);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  ತಾರ್ಕಿಕ apply (AAT::hb_aat_apply_context_t *c,
	      ಧ್ರುವ AAT::kern_accelerator_data_t &accel_data) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ dispatch (c, accel_data); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ subtable_type = get_type ();
    TRACE_DISPATCH (ಏತತ್, subtable_type);
    ವಿಕಲ್ಪ (subtable_type) {
    ಪಕ್ಷ 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ifndef HB_NO_AAT_SHAPE
    ಪಕ್ಷ 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#endif
    ಸ್ವಭಾವ:	return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.version32.v.sanitize (c)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_data_t create_accelerator_data (unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (get_type ()) {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.ot.create_accelerator_data (ಅಂಕೆ_glyphs);
#ifndef HB_NO_AAT_SHAPE
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.aat.create_accelerator_data (ಅಂಕೆ_glyphs);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ AAT::kern_accelerator_data_t ();
    }
  }

  ಸಂರಚನಾ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ಏತತ್->table = sc.reference_table<kern> (face);
      ಏತತ್->accel_data = ಏತತ್->table->create_accelerator_data (face->get_ಅಂಕೆ_glyphs ());
    }
    ~accelerator_t ()
    {
      ಏತತ್->table.destroy ();
    }

    hb_blob_t *get_blob () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ table.get_blob (); }

    ತಾರ್ಕಿಕ apply (AAT::hb_aat_apply_context_t *c) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ table->apply (c, accel_data);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_data_t accel_data;
    AAT::hb_aat_scratch_t scratch;
  };

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT32 v; }	version32;
  ಸಂರಚನಾ { HBUINT16 v; }	major;
  KernOT		ot;
#ifndef HB_NO_AAT_SHAPE
  KernAAT		aat;
#endif
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (4, version32.v);
};

ಸಂರಚನಾ kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_KERN_TABLE_HH */
