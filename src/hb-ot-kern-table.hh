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

#அந்தர்பாவ "hb-aat-layout-common.hh"
#அந்தர்பாவ "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#define HB_OT_TAG_kern HB_TAG('k','e','r','n')


நாமாகாஶ OT {


template <typename KernSubTableHeader>
கட்டமைப்பு KernSubTableFormat3
{
  முழு get_kerning (hb_codepoint_t left, hb_codepoint_t வலது) மாறிலி
  {
    hb_array_t<மாறிலி FWORD> kern_மதிப்பு = kern_மதிப்பு_Z.as_array (kern_மதிப்பு_மொத்தம்);
    hb_array_t<மாறிலி HBUINT8> இடது_Class = StructAfter<மாறிலி UnsizedArrayOf<HBUINT8>> (kern_மதிப்பு).as_array (glyph_மொத்தம்_2);
    hb_array_t<மாறிலி HBUINT8> வலது_Class = StructAfter<மாறிலி UnsizedArrayOf<HBUINT8>> (இடது_Class).as_array (glyph_மொத்தம்_2);
    hb_array_t<மாறிலி HBUINT8> kernIndex = StructAfter<மாறிலி UnsizedArrayOf<HBUINT8>> (வலது_Class).as_array (இடது_Class_மொத்தம் * வலது_Class_மொத்தம்);

    unsigned முழு இடது_C = இடது_Class[left];
    unsigned முழு வலது_C = வலது_Class[வலது];
    எனில் (unlikely (இடது_C >= இடது_Class_மொத்தம் || வலது_C >= வலது_Class_மொத்தம்))
      திரும்பு 0;
    unsigned முழு i = இடது_C * வலது_Class_மொத்தம் + வலது_C;
    திரும்பு kern_மதிப்பு[kernIndex[i]];
  }

  தருக்க apply (AAT::hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    எனில் (!c->plan->requested_kerning)
      திரும்பு பொய்;

    எனில் (header.coverage & header.Backwards)
      திரும்பு பொய்;

    hb_kern_machine_t<KernSubTableFormat3> machine (*இது, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  c->check_range (kern_மதிப்பு_Z,
				  kern_மதிப்பு_மொத்தம் * அளவு (FWORD) +
				  glyph_மொத்தம்_2 * 2 +
				  இடது_Class_மொத்தம் * வலது_Class_மொத்தம்));
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &இடது_set, set_t &வலது_set, unsigned இலக்கம்_glyphs) மாறிலி
  {
    எனில் (likely (glyph_மொத்தம்_2))
    {
      இடது_set.add_range (0, இலக்கம்_glyphs - 1);
      வலது_set.add_range (0, இலக்கம்_glyphs - 1);
    }
  }

  பாதுகாக்கப்பட்ட:
  KernSubTableHeader
		header;
  HBUINT16	glyph_மொத்தம்_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_மதிப்பு_மொத்தம்;	/* The number of kerning values. */
  HBUINT8	இடது_Class_மொத்தம்;	/* The number of left-hand classes. */
  HBUINT8	வலது_Class_மொத்தம்;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  UnsizedArrayOf<FWORD>
		kern_மதிப்பு_Z;	/* The kerning values.
				 * Length kernValueCount. */
#எனில் 0
  UnsizedArrayOf<HBUINT8>
		இடது_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>
		வலது_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>kernIndex;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#endif
  பொது:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 6, kern_மதிப்பு_Z);
};

template <typename KernSubTableHeader>
கட்டமைப்பு KernSubTable
{
  size_t get_size () மாறிலி { திரும்பு u.header.length; }
  unsigned முழு get_type () மாறிலி { திரும்பு u.header.format; }

  முழு get_kerning (hb_codepoint_t left, hb_codepoint_t வலது) மாறிலி
  {
    தேர்வு (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_kerning (left, வலது);
    இயல்பு:திரும்பு 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    unsigned முழு subtable_type = get_type ();
    TRACE_DISPATCH (இது, subtable_type);
    தேர்வு (subtable_type) {
    நிலை 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ifndef HB_NO_AAT_SHAPE
    நிலை 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#endif
    நிலை 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ifndef HB_NO_AAT_SHAPE
    நிலை 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#endif
    இயல்பு:	return_trace (c->default_return_மதிப்பு ());
    }
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &இடது_set, set_t &வலது_set, unsigned இலக்கம்_glyphs) மாறிலி
  {
    unsigned முழு subtable_type = get_type ();
    தேர்வு (subtable_type) {
    நிலை 0:	hb_barrier (); u.format0.collect_glyphs (இடது_set, வலது_set, இலக்கம்_glyphs); திரும்பு;
#ifndef HB_NO_AAT_SHAPE
    நிலை 1:	hb_barrier (); u.format1.collect_glyphs (இடது_set, வலது_set, இலக்கம்_glyphs); திரும்பு;
#endif
    நிலை 2:	hb_barrier (); u.format2.collect_glyphs (இடது_set, வலது_set, இலக்கம்_glyphs); திரும்பு;
#ifndef HB_NO_AAT_SHAPE
    நிலை 3:	hb_barrier (); u.format3.collect_glyphs (இடது_set, வலது_set, இலக்கம்_glyphs); திரும்பு;
#endif
    இயல்பு:	திரும்பு;
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.length >= u.header.min_size &&
		    c->check_range (இது, u.header.length)))) return_trace (பொய்);

    return_trace (dispatch (c));
  }

  பொது:
  ஒன்றிப்பு {
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
  பொது:
  DEFINE_SIZE_MIN (KernSubTableHeader::static_size);
};


கட்டமைப்பு KernOTSubTableHeader
{
  நிலையான constexpr தருக்க apple = பொய்;
  வகைவரை AAT::ObsoleteTypes Types;

  unsigned   tuple_மொத்தம்_2 () மாறிலி { திரும்பு 0; }
  தருக்க     is_horizontal () மாறிலி { திரும்பு (coverage & Horizontal); }

  எண்ணிக்கை Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    CrossStream	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  HBUINT16	versionZ;	/* Unused. */
  HBUINT16	length;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு KernOT : AAT::KerxTable<KernOT>
{
  friend கட்டமைப்பு AAT::KerxTable<KernOT>;

  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  நிலையான constexpr unsigned minVersion = 0u;

  வகைவரை KernOTSubTableHeader SubTableHeader;
  வகைவரை SubTableHeader::Types Types;
  வகைவரை KernSubTable<SubTableHeader> SubTable;

  பாதுகாக்கப்பட்ட:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_மொத்தம்;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  பொது:
  DEFINE_SIZE_MIN (4);
};


கட்டமைப்பு KernAATSubTableHeader
{
  நிலையான constexpr தருக்க apple = உண்மை;
  வகைவரை AAT::ObsoleteTypes Types;

  unsigned   tuple_மொத்தம்_2 () மாறிலி { திரும்பு 0; }
  தருக்க     is_horizontal () மாறிலி { திரும்பு !(coverage & Vertical); }

  எண்ணிக்கை Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  HBUINT32	length;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tupleIndex;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு KernAAT : AAT::KerxTable<KernAAT>
{
  friend கட்டமைப்பு AAT::KerxTable<KernAAT>;

  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  நிலையான constexpr unsigned minVersion = 0x00010000u;

  வகைவரை KernAATSubTableHeader SubTableHeader;
  வகைவரை SubTableHeader::Types Types;
  வகைவரை KernSubTable<SubTableHeader> SubTable;

  பாதுகாக்கப்பட்ட:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_மொத்தம்;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  பொது:
  DEFINE_SIZE_MIN (8);
};

கட்டமைப்பு kern
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  தருக்க     has_data () மாறிலி { திரும்பு u.version32.v; }
  unsigned get_type () மாறிலி { திரும்பு u.major.v; }

  தருக்க has_state_machine () மாறிலி
  {
    தேர்வு (get_type ()) {
    நிலை 0: hb_barrier (); திரும்பு u.ot.has_state_machine ();
#ifndef HB_NO_AAT_SHAPE
    நிலை 1: hb_barrier (); திரும்பு u.aat.has_state_machine ();
#endif
    இயல்பு:திரும்பு பொய்;
    }
  }

  தருக்க has_cross_stream () மாறிலி
  {
    தேர்வு (get_type ()) {
    நிலை 0: hb_barrier (); திரும்பு u.ot.has_cross_stream ();
#ifndef HB_NO_AAT_SHAPE
    நிலை 1: hb_barrier (); திரும்பு u.aat.has_cross_stream ();
#endif
    இயல்பு:திரும்பு பொய்;
    }
  }

  முழு get_h_kerning (hb_codepoint_t left, hb_codepoint_t வலது) மாறிலி
  {
    தேர்வு (get_type ()) {
    நிலை 0: hb_barrier (); திரும்பு u.ot.get_h_kerning (left, வலது);
#ifndef HB_NO_AAT_SHAPE
    நிலை 1: hb_barrier (); திரும்பு u.aat.get_h_kerning (left, வலது);
#endif
    இயல்பு:திரும்பு 0;
    }
  }

  தருக்க apply (AAT::hb_aat_apply_context_t *c,
	      மாறிலி AAT::kern_accelerator_data_t &accel_data) மாறிலி
  { திரும்பு dispatch (c, accel_data); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    unsigned முழு subtable_type = get_type ();
    TRACE_DISPATCH (இது, subtable_type);
    தேர்வு (subtable_type) {
    நிலை 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ifndef HB_NO_AAT_SHAPE
    நிலை 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#endif
    இயல்பு:	return_trace (c->default_return_மதிப்பு ());
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.version32.v.sanitize (c)) return_trace (பொய்);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_data_t create_accelerator_data (unsigned இலக்கம்_glyphs) மாறிலி
  {
    தேர்வு (get_type ()) {
    நிலை 0: hb_barrier (); திரும்பு u.ot.create_accelerator_data (இலக்கம்_glyphs);
#ifndef HB_NO_AAT_SHAPE
    நிலை 1: hb_barrier (); திரும்பு u.aat.create_accelerator_data (இலக்கம்_glyphs);
#endif
    இயல்பு:திரும்பு AAT::kern_accelerator_data_t ();
    }
  }

  கட்டமைப்பு accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      இது->table = sc.reference_table<kern> (face);
      இது->accel_data = இது->table->create_accelerator_data (face->get_இலக்கம்_glyphs ());
    }
    ~accelerator_t ()
    {
      இது->table.destroy ();
    }

    hb_blob_t *get_blob () மாறிலி { திரும்பு table.get_blob (); }

    தருக்க apply (AAT::hb_aat_apply_context_t *c) மாறிலி
    {
      திரும்பு table->apply (c, accel_data);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_data_t accel_data;
    AAT::hb_aat_scratch_t scratch;
  };

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT32 v; }	version32;
  கட்டமைப்பு { HBUINT16 v; }	major;
  KernOT		ot;
#ifndef HB_NO_AAT_SHAPE
  KernAAT		aat;
#endif
  } u;
  பொது:
  DEFINE_SIZE_UNION (4, version32.v);
};

கட்டமைப்பு kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_KERN_TABLE_HH */
