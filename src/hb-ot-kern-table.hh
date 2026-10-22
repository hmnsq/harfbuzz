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

#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#define HB_OT_TAG_kern HB_TAG('k','e','r','n')


ନାମାକାଶ OT {


template <typename KernSubTableHeader>
ସଂରଚନା KernSubTableFormat3
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_kerning (hb_codepoint_t left, hb_codepoint_t ଡାହାଣ) ଅଚଳ
  {
    hb_array_t<ଅଚଳ FWORD> kern_ମୂଲ୍ୟ = kern_ମୂଲ୍ୟ_Z.as_array (kern_ମୂଲ୍ୟ_ଗଣନା);
    hb_array_t<ଅଚଳ HBUINT8> ବାମ_Class = StructAfter<ଅଚଳ UnsizedArrayOf<HBUINT8>> (kern_ମୂଲ୍ୟ).as_array (glyph_ଗଣନା_2);
    hb_array_t<ଅଚଳ HBUINT8> ଡାହାଣ_Class = StructAfter<ଅଚଳ UnsizedArrayOf<HBUINT8>> (ବାମ_Class).as_array (glyph_ଗଣନା_2);
    hb_array_t<ଅଚଳ HBUINT8> kernIndex = StructAfter<ଅଚଳ UnsizedArrayOf<HBUINT8>> (ଡାହାଣ_Class).as_array (ବାମ_Class_ଗଣନା * ଡାହାଣ_Class_ଗଣନା);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବାମ_C = ବାମ_Class[left];
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଡାହାଣ_C = ଡାହାଣ_Class[ଡାହାଣ];
    ଯଦି (unlikely (ବାମ_C >= ବାମ_Class_ଗଣନା || ଡାହାଣ_C >= ଡାହାଣ_Class_ଗଣନା))
      ଫେରନ୍ତୁ 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ବାମ_C * ଡାହାଣ_Class_ଗଣନା + ଡାହାଣ_C;
    ଫେରନ୍ତୁ kern_ମୂଲ୍ୟ[kernIndex[i]];
  }

  ତାର୍କିକ apply (AAT::hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    ଯଦି (!c->plan->requested_kerning)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (header.coverage & header.Backwards)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    hb_kern_machine_t<KernSubTableFormat3> machine (*ଏହି, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  c->check_range (kern_ମୂଲ୍ୟ_Z,
				  kern_ମୂଲ୍ୟ_ଗଣନା * ଆକାର (FWORD) +
				  glyph_ଗଣନା_2 * 2 +
				  ବାମ_Class_ଗଣନା * ଡାହାଣ_Class_ଗଣନା));
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &ବାମ_set, set_t &ଡାହାଣ_set, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ଯଦି (likely (glyph_ଗଣନା_2))
    {
      ବାମ_set.add_range (0, ଅଙ୍କ_glyphs - 1);
      ଡାହାଣ_set.add_range (0, ଅଙ୍କ_glyphs - 1);
    }
  }

  ସୁରକ୍ଷିତ:
  KernSubTableHeader
		header;
  HBUINT16	glyph_ଗଣନା_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_ମୂଲ୍ୟ_ଗଣନା;	/* The number of kerning values. */
  HBUINT8	ବାମ_Class_ଗଣନା;	/* The number of left-hand classes. */
  HBUINT8	ଡାହାଣ_Class_ଗଣନା;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  UnsizedArrayOf<FWORD>
		kern_ମୂଲ୍ୟ_Z;	/* The kerning values.
				 * Length kernValueCount. */
#ଯଦି 0
  UnsizedArrayOf<HBUINT8>
		ବାମ_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>
		ଡାହାଣ_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>kernIndex;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#endif
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 6, kern_ମୂଲ୍ୟ_Z);
};

template <typename KernSubTableHeader>
ସଂରଚନା KernSubTable
{
  size_t get_size () ଅଚଳ { ଫେରନ୍ତୁ u.header.length; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_type () ଅଚଳ { ଫେରନ୍ତୁ u.header.format; }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_kerning (hb_codepoint_t left, hb_codepoint_t ଡାହାଣ) ଅଚଳ
  {
    ବାଛ (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_kerning (left, ଡାହାଣ);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_type = get_type ();
    TRACE_DISPATCH (ଏହି, subtable_type);
    ବାଛ (subtable_type) {
    ସ୍ଥିତି 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ifndef HB_NO_AAT_SHAPE
    ସ୍ଥିତି 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#endif
    ସ୍ଥିତି 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ifndef HB_NO_AAT_SHAPE
    ସ୍ଥିତି 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:	return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &ବାମ_set, set_t &ଡାହାଣ_set, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_type = get_type ();
    ବାଛ (subtable_type) {
    ସ୍ଥିତି 0:	hb_barrier (); u.format0.collect_glyphs (ବାମ_set, ଡାହାଣ_set, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
#ifndef HB_NO_AAT_SHAPE
    ସ୍ଥିତି 1:	hb_barrier (); u.format1.collect_glyphs (ବାମ_set, ଡାହାଣ_set, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
#endif
    ସ୍ଥିତି 2:	hb_barrier (); u.format2.collect_glyphs (ବାମ_set, ଡାହାଣ_set, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
#ifndef HB_NO_AAT_SHAPE
    ସ୍ଥିତି 3:	hb_barrier (); u.format3.collect_glyphs (ବାମ_set, ଡାହାଣ_set, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:	ଫେରନ୍ତୁ;
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.length >= u.header.min_size &&
		    c->check_range (ଏହି, u.header.length)))) return_trace (ମିଥ୍ୟା);

    return_trace (dispatch (c));
  }

  ସର୍ବସାଧାରଣ:
  ସଂଘ {
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (KernSubTableHeader::static_size);
};


ସଂରଚନା KernOTSubTableHeader
{
  ସ୍ଥିର constexpr ତାର୍କିକ apple = ମିଥ୍ୟା;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା AAT::ObsoleteTypes Types;

  unsigned   tuple_ଗଣନା_2 () ଅଚଳ { ଫେରନ୍ତୁ 0; }
  ତାର୍କିକ     is_horizontal () ଅଚଳ { ଫେରନ୍ତୁ (coverage & Horizontal); }

  ଗଣନା Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    CrossStream	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  HBUINT16	versionZ;	/* Unused. */
  HBUINT16	length;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା KernOT : AAT::KerxTable<KernOT>
{
  friend ସଂରଚନା AAT::KerxTable<KernOT>;

  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  ସ୍ଥିର constexpr unsigned minVersion = 0u;

  ପ୍ରକାରବ୍ୟାଖ୍ୟା KernOTSubTableHeader SubTableHeader;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା SubTableHeader::Types Types;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା KernSubTable<SubTableHeader> SubTable;

  ସୁରକ୍ଷିତ:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_ଗଣନା;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (4);
};


ସଂରଚନା KernAATSubTableHeader
{
  ସ୍ଥିର constexpr ତାର୍କିକ apple = ସତ୍ୟ;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା AAT::ObsoleteTypes Types;

  unsigned   tuple_ଗଣନା_2 () ଅଚଳ { ଫେରନ୍ତୁ 0; }
  ତାର୍କିକ     is_horizontal () ଅଚଳ { ଫେରନ୍ତୁ !(coverage & Vertical); }

  ଗଣନା Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  HBUINT32	length;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tupleIndex;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା KernAAT : AAT::KerxTable<KernAAT>
{
  friend ସଂରଚନା AAT::KerxTable<KernAAT>;

  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  ସ୍ଥିର constexpr unsigned minVersion = 0x00010000u;

  ପ୍ରକାରବ୍ୟାଖ୍ୟା KernAATSubTableHeader SubTableHeader;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା SubTableHeader::Types Types;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା KernSubTable<SubTableHeader> SubTable;

  ସୁରକ୍ଷିତ:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_ଗଣନା;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (8);
};

ସଂରଚନା kern
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  ତାର୍କିକ     has_data () ଅଚଳ { ଫେରନ୍ତୁ u.version32.v; }
  unsigned get_type () ଅଚଳ { ଫେରନ୍ତୁ u.major.v; }

  ତାର୍କିକ has_state_machine () ଅଚଳ
  {
    ବାଛ (get_type ()) {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.ot.has_state_machine ();
#ifndef HB_NO_AAT_SHAPE
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.aat.has_state_machine ();
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  ତାର୍କିକ has_cross_stream () ଅଚଳ
  {
    ବାଛ (get_type ()) {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.ot.has_cross_stream ();
#ifndef HB_NO_AAT_SHAPE
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.aat.has_cross_stream ();
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_h_kerning (hb_codepoint_t left, hb_codepoint_t ଡାହାଣ) ଅଚଳ
  {
    ବାଛ (get_type ()) {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.ot.get_h_kerning (left, ଡାହାଣ);
#ifndef HB_NO_AAT_SHAPE
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.aat.get_h_kerning (left, ଡାହାଣ);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  ତାର୍କିକ apply (AAT::hb_aat_apply_context_t *c,
	      ଅଚଳ AAT::kern_accelerator_data_t &accel_data) ଅଚଳ
  { ଫେରନ୍ତୁ dispatch (c, accel_data); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_type = get_type ();
    TRACE_DISPATCH (ଏହି, subtable_type);
    ବାଛ (subtable_type) {
    ସ୍ଥିତି 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ifndef HB_NO_AAT_SHAPE
    ସ୍ଥିତି 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:	return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.version32.v.sanitize (c)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_data_t create_accelerator_data (unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ବାଛ (get_type ()) {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.ot.create_accelerator_data (ଅଙ୍କ_glyphs);
#ifndef HB_NO_AAT_SHAPE
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.aat.create_accelerator_data (ଅଙ୍କ_glyphs);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ AAT::kern_accelerator_data_t ();
    }
  }

  ସଂରଚନା accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ଏହି->table = sc.reference_table<kern> (face);
      ଏହି->accel_data = ଏହି->table->create_accelerator_data (face->get_ଅଙ୍କ_glyphs ());
    }
    ~accelerator_t ()
    {
      ଏହି->table.destroy ();
    }

    hb_blob_t *get_blob () ଅଚଳ { ଫେରନ୍ତୁ table.get_blob (); }

    ତାର୍କିକ apply (AAT::hb_aat_apply_context_t *c) ଅଚଳ
    {
      ଫେରନ୍ତୁ table->apply (c, accel_data);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_data_t accel_data;
    AAT::hb_aat_scratch_t scratch;
  };

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT32 v; }	version32;
  ସଂରଚନା { HBUINT16 v; }	major;
  KernOT		ot;
#ifndef HB_NO_AAT_SHAPE
  KernAAT		aat;
#endif
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (4, version32.v);
};

ସଂରଚନା kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_KERN_TABLE_HH */
