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

#અન્તર્ભાવ "hb-aat-layout-common.hh"
#અન્તર્ભાવ "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#define HB_OT_TAG_kern HB_TAG('k','e','r','n')


નામાકાશ OT {


template <typename KernSubTableHeader>
સંરચના KernSubTableFormat3
{
  પૂર્ણાંક get_kerning (hb_codepoint_t left, hb_codepoint_t જમણું) અચલ
  {
    hb_array_t<અચલ FWORD> kern_કિંમત = kern_કિંમત_Z.as_array (kern_કિંમત_ગણતરી);
    hb_array_t<અચલ HBUINT8> ડાબું_Class = StructAfter<અચલ UnsizedArrayOf<HBUINT8>> (kern_કિંમત).as_array (glyph_ગણતરી_2);
    hb_array_t<અચલ HBUINT8> જમણું_Class = StructAfter<અચલ UnsizedArrayOf<HBUINT8>> (ડાબું_Class).as_array (glyph_ગણતરી_2);
    hb_array_t<અચલ HBUINT8> kernIndex = StructAfter<અચલ UnsizedArrayOf<HBUINT8>> (જમણું_Class).as_array (ડાબું_Class_ગણતરી * જમણું_Class_ગણતરી);

    unsigned પૂર્ણાંક ડાબું_C = ડાબું_Class[left];
    unsigned પૂર્ણાંક જમણું_C = જમણું_Class[જમણું];
    જો (unlikely (ડાબું_C >= ડાબું_Class_ગણતરી || જમણું_C >= જમણું_Class_ગણતરી))
      પાછા 0;
    unsigned પૂર્ણાંક i = ડાબું_C * જમણું_Class_ગણતરી + જમણું_C;
    પાછા kern_કિંમત[kernIndex[i]];
  }

  તાર્કિક apply (AAT::hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    જો (!c->plan->requested_kerning)
      પાછા ખોટું;

    જો (header.coverage & header.Backwards)
      પાછા ખોટું;

    hb_kern_machine_t<KernSubTableFormat3> machine (*આ, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  c->check_range (kern_કિંમત_Z,
				  kern_કિંમત_ગણતરી * કદ (FWORD) +
				  glyph_ગણતરી_2 * 2 +
				  ડાબું_Class_ગણતરી * જમણું_Class_ગણતરી));
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &ડાબું_set, set_t &જમણું_set, unsigned અંક_glyphs) અચલ
  {
    જો (likely (glyph_ગણતરી_2))
    {
      ડાબું_set.add_range (0, અંક_glyphs - 1);
      જમણું_set.add_range (0, અંક_glyphs - 1);
    }
  }

  સુરક્ષિત:
  KernSubTableHeader
		header;
  HBUINT16	glyph_ગણતરી_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_કિંમત_ગણતરી;	/* The number of kerning values. */
  HBUINT8	ડાબું_Class_ગણતરી;	/* The number of left-hand classes. */
  HBUINT8	જમણું_Class_ગણતરી;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  UnsizedArrayOf<FWORD>
		kern_કિંમત_Z;	/* The kerning values.
				 * Length kernValueCount. */
#જો 0
  UnsizedArrayOf<HBUINT8>
		ડાબું_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>
		જમણું_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>kernIndex;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#endif
  જાહેર:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 6, kern_કિંમત_Z);
};

template <typename KernSubTableHeader>
સંરચના KernSubTable
{
  size_t get_size () અચલ { પાછા u.header.length; }
  unsigned પૂર્ણાંક get_type () અચલ { પાછા u.header.format; }

  પૂર્ણાંક get_kerning (hb_codepoint_t left, hb_codepoint_t જમણું) અચલ
  {
    પસંદગી (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_kerning (left, જમણું);
    મૂળભૂત:પાછા 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    unsigned પૂર્ણાંક subtable_type = get_type ();
    TRACE_DISPATCH (આ, subtable_type);
    પસંદગી (subtable_type) {
    સ્થિતિ 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ifndef HB_NO_AAT_SHAPE
    સ્થિતિ 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#endif
    સ્થિતિ 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ifndef HB_NO_AAT_SHAPE
    સ્થિતિ 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#endif
    મૂળભૂત:	return_trace (c->default_return_કિંમત ());
    }
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &ડાબું_set, set_t &જમણું_set, unsigned અંક_glyphs) અચલ
  {
    unsigned પૂર્ણાંક subtable_type = get_type ();
    પસંદગી (subtable_type) {
    સ્થિતિ 0:	hb_barrier (); u.format0.collect_glyphs (ડાબું_set, જમણું_set, અંક_glyphs); પાછા;
#ifndef HB_NO_AAT_SHAPE
    સ્થિતિ 1:	hb_barrier (); u.format1.collect_glyphs (ડાબું_set, જમણું_set, અંક_glyphs); પાછા;
#endif
    સ્થિતિ 2:	hb_barrier (); u.format2.collect_glyphs (ડાબું_set, જમણું_set, અંક_glyphs); પાછા;
#ifndef HB_NO_AAT_SHAPE
    સ્થિતિ 3:	hb_barrier (); u.format3.collect_glyphs (ડાબું_set, જમણું_set, અંક_glyphs); પાછા;
#endif
    મૂળભૂત:	પાછા;
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.length >= u.header.min_size &&
		    c->check_range (આ, u.header.length)))) return_trace (ખોટું);

    return_trace (dispatch (c));
  }

  જાહેર:
  સંઘ {
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
  જાહેર:
  DEFINE_SIZE_MIN (KernSubTableHeader::static_size);
};


સંરચના KernOTSubTableHeader
{
  સ્થિર constexpr તાર્કિક apple = ખોટું;
  પ્રકારવ્યાખ્યા AAT::ObsoleteTypes Types;

  unsigned   tuple_ગણતરી_2 () અચલ { પાછા 0; }
  તાર્કિક     is_horizontal () અચલ { પાછા (coverage & Horizontal); }

  ગણના Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    CrossStream	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
  HBUINT16	versionZ;	/* Unused. */
  HBUINT16	length;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના KernOT : AAT::KerxTable<KernOT>
{
  friend સંરચના AAT::KerxTable<KernOT>;

  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  સ્થિર constexpr unsigned minVersion = 0u;

  પ્રકારવ્યાખ્યા KernOTSubTableHeader SubTableHeader;
  પ્રકારવ્યાખ્યા SubTableHeader::Types Types;
  પ્રકારવ્યાખ્યા KernSubTable<SubTableHeader> SubTable;

  સુરક્ષિત:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_ગણતરી;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  જાહેર:
  DEFINE_SIZE_MIN (4);
};


સંરચના KernAATSubTableHeader
{
  સ્થિર constexpr તાર્કિક apple = સાચું;
  પ્રકારવ્યાખ્યા AAT::ObsoleteTypes Types;

  unsigned   tuple_ગણતરી_2 () અચલ { પાછા 0; }
  તાર્કિક     is_horizontal () અચલ { પાછા !(coverage & Vertical); }

  ગણના Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
  HBUINT32	length;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tupleIndex;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના KernAAT : AAT::KerxTable<KernAAT>
{
  friend સંરચના AAT::KerxTable<KernAAT>;

  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  સ્થિર constexpr unsigned minVersion = 0x00010000u;

  પ્રકારવ્યાખ્યા KernAATSubTableHeader SubTableHeader;
  પ્રકારવ્યાખ્યા SubTableHeader::Types Types;
  પ્રકારવ્યાખ્યા KernSubTable<SubTableHeader> SubTable;

  સુરક્ષિત:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_ગણતરી;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  જાહેર:
  DEFINE_SIZE_MIN (8);
};

સંરચના kern
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  તાર્કિક     has_data () અચલ { પાછા u.version32.v; }
  unsigned get_type () અચલ { પાછા u.major.v; }

  તાર્કિક has_state_machine () અચલ
  {
    પસંદગી (get_type ()) {
    સ્થિતિ 0: hb_barrier (); પાછા u.ot.has_state_machine ();
#ifndef HB_NO_AAT_SHAPE
    સ્થિતિ 1: hb_barrier (); પાછા u.aat.has_state_machine ();
#endif
    મૂળભૂત:પાછા ખોટું;
    }
  }

  તાર્કિક has_cross_stream () અચલ
  {
    પસંદગી (get_type ()) {
    સ્થિતિ 0: hb_barrier (); પાછા u.ot.has_cross_stream ();
#ifndef HB_NO_AAT_SHAPE
    સ્થિતિ 1: hb_barrier (); પાછા u.aat.has_cross_stream ();
#endif
    મૂળભૂત:પાછા ખોટું;
    }
  }

  પૂર્ણાંક get_h_kerning (hb_codepoint_t left, hb_codepoint_t જમણું) અચલ
  {
    પસંદગી (get_type ()) {
    સ્થિતિ 0: hb_barrier (); પાછા u.ot.get_h_kerning (left, જમણું);
#ifndef HB_NO_AAT_SHAPE
    સ્થિતિ 1: hb_barrier (); પાછા u.aat.get_h_kerning (left, જમણું);
#endif
    મૂળભૂત:પાછા 0;
    }
  }

  તાર્કિક apply (AAT::hb_aat_apply_context_t *c,
	      અચલ AAT::kern_accelerator_data_t &accel_data) અચલ
  { પાછા dispatch (c, accel_data); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    unsigned પૂર્ણાંક subtable_type = get_type ();
    TRACE_DISPATCH (આ, subtable_type);
    પસંદગી (subtable_type) {
    સ્થિતિ 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ifndef HB_NO_AAT_SHAPE
    સ્થિતિ 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#endif
    મૂળભૂત:	return_trace (c->default_return_કિંમત ());
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.version32.v.sanitize (c)) return_trace (ખોટું);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_data_t create_accelerator_data (unsigned અંક_glyphs) અચલ
  {
    પસંદગી (get_type ()) {
    સ્થિતિ 0: hb_barrier (); પાછા u.ot.create_accelerator_data (અંક_glyphs);
#ifndef HB_NO_AAT_SHAPE
    સ્થિતિ 1: hb_barrier (); પાછા u.aat.create_accelerator_data (અંક_glyphs);
#endif
    મૂળભૂત:પાછા AAT::kern_accelerator_data_t ();
    }
  }

  સંરચના accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      આ->table = sc.reference_table<kern> (face);
      આ->accel_data = આ->table->create_accelerator_data (face->get_અંક_glyphs ());
    }
    ~accelerator_t ()
    {
      આ->table.destroy ();
    }

    hb_blob_t *get_blob () અચલ { પાછા table.get_blob (); }

    તાર્કિક apply (AAT::hb_aat_apply_context_t *c) અચલ
    {
      પાછા table->apply (c, accel_data);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_data_t accel_data;
    AAT::hb_aat_scratch_t scratch;
  };

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT32 v; }	version32;
  સંરચના { HBUINT16 v; }	major;
  KernOT		ot;
#ifndef HB_NO_AAT_SHAPE
  KernAAT		aat;
#endif
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (4, version32.v);
};

સંરચના kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_KERN_TABLE_HH */
