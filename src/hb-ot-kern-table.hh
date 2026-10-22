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

#അന്തര്ഭാവ "hb-aat-layout-common.hh"
#അന്തര്ഭാവ "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#define HB_OT_TAG_kern HB_TAG('k','e','r','n')


നാമാകാശ OT {


template <typename KernSubTableHeader>
ഘടന KernSubTableFormat3
{
  പൂർണ്ണസംഖ്യ get_kerning (hb_codepoint_t left, hb_codepoint_t വലത്) സ്ഥിരം
  {
    hb_array_t<സ്ഥിരം FWORD> kern_മൂല്യം = kern_മൂല്യം_Z.as_array (kern_മൂല്യം_എണ്ണം);
    hb_array_t<സ്ഥിരം HBUINT8> ഇടത്_Class = StructAfter<സ്ഥിരം UnsizedArrayOf<HBUINT8>> (kern_മൂല്യം).as_array (glyph_എണ്ണം_2);
    hb_array_t<സ്ഥിരം HBUINT8> വലത്_Class = StructAfter<സ്ഥിരം UnsizedArrayOf<HBUINT8>> (ഇടത്_Class).as_array (glyph_എണ്ണം_2);
    hb_array_t<സ്ഥിരം HBUINT8> kernIndex = StructAfter<സ്ഥിരം UnsizedArrayOf<HBUINT8>> (വലത്_Class).as_array (ഇടത്_Class_എണ്ണം * വലത്_Class_എണ്ണം);

    unsigned പൂർണ്ണസംഖ്യ ഇടത്_C = ഇടത്_Class[left];
    unsigned പൂർണ്ണസംഖ്യ വലത്_C = വലത്_Class[വലത്];
    എങ്കിൽ (unlikely (ഇടത്_C >= ഇടത്_Class_എണ്ണം || വലത്_C >= വലത്_Class_എണ്ണം))
      തിരികെ 0;
    unsigned പൂർണ്ണസംഖ്യ i = ഇടത്_C * വലത്_Class_എണ്ണം + വലത്_C;
    തിരികെ kern_മൂല്യം[kernIndex[i]];
  }

  തർക്കം apply (AAT::hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    എങ്കിൽ (!c->plan->requested_kerning)
      തിരികെ അസത്യം;

    എങ്കിൽ (header.coverage & header.Backwards)
      തിരികെ അസത്യം;

    hb_kern_machine_t<KernSubTableFormat3> machine (*ഇത്, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  c->check_range (kern_മൂല്യം_Z,
				  kern_മൂല്യം_എണ്ണം * വലുപ്പം (FWORD) +
				  glyph_എണ്ണം_2 * 2 +
				  ഇടത്_Class_എണ്ണം * വലത്_Class_എണ്ണം));
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &ഇടത്_set, set_t &വലത്_set, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    എങ്കിൽ (likely (glyph_എണ്ണം_2))
    {
      ഇടത്_set.add_range (0, അക്കം_glyphs - 1);
      വലത്_set.add_range (0, അക്കം_glyphs - 1);
    }
  }

  സംരക്ഷിത:
  KernSubTableHeader
		header;
  HBUINT16	glyph_എണ്ണം_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_മൂല്യം_എണ്ണം;	/* The number of kerning values. */
  HBUINT8	ഇടത്_Class_എണ്ണം;	/* The number of left-hand classes. */
  HBUINT8	വലത്_Class_എണ്ണം;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  UnsizedArrayOf<FWORD>
		kern_മൂല്യം_Z;	/* The kerning values.
				 * Length kernValueCount. */
#എങ്കിൽ 0
  UnsizedArrayOf<HBUINT8>
		ഇടത്_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>
		വലത്_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>kernIndex;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#endif
  പൊതു:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 6, kern_മൂല്യം_Z);
};

template <typename KernSubTableHeader>
ഘടന KernSubTable
{
  size_t get_size () സ്ഥിരം { തിരികെ u.header.length; }
  unsigned പൂർണ്ണസംഖ്യ get_type () സ്ഥിരം { തിരികെ u.header.format; }

  പൂർണ്ണസംഖ്യ get_kerning (hb_codepoint_t left, hb_codepoint_t വലത്) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_kerning (left, വലത്);
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ subtable_type = get_type ();
    TRACE_DISPATCH (ഇത്, subtable_type);
    തിരഞ്ഞെടുപ്പ് (subtable_type) {
    അവസ്ഥ 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ifndef HB_NO_AAT_SHAPE
    അവസ്ഥ 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#endif
    അവസ്ഥ 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ifndef HB_NO_AAT_SHAPE
    അവസ്ഥ 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#endif
    സ്ഥിരസ്ഥിതി:	return_trace (c->default_return_മൂല്യം ());
    }
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &ഇടത്_set, set_t &വലത്_set, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ subtable_type = get_type ();
    തിരഞ്ഞെടുപ്പ് (subtable_type) {
    അവസ്ഥ 0:	hb_barrier (); u.format0.collect_glyphs (ഇടത്_set, വലത്_set, അക്കം_glyphs); തിരികെ;
#ifndef HB_NO_AAT_SHAPE
    അവസ്ഥ 1:	hb_barrier (); u.format1.collect_glyphs (ഇടത്_set, വലത്_set, അക്കം_glyphs); തിരികെ;
#endif
    അവസ്ഥ 2:	hb_barrier (); u.format2.collect_glyphs (ഇടത്_set, വലത്_set, അക്കം_glyphs); തിരികെ;
#ifndef HB_NO_AAT_SHAPE
    അവസ്ഥ 3:	hb_barrier (); u.format3.collect_glyphs (ഇടത്_set, വലത്_set, അക്കം_glyphs); തിരികെ;
#endif
    സ്ഥിരസ്ഥിതി:	തിരികെ;
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.length >= u.header.min_size &&
		    c->check_range (ഇത്, u.header.length)))) return_trace (അസത്യം);

    return_trace (dispatch (c));
  }

  പൊതു:
  സംഘം {
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
  പൊതു:
  DEFINE_SIZE_MIN (KernSubTableHeader::static_size);
};


ഘടന KernOTSubTableHeader
{
  സ്ഥിതം constexpr തർക്കം apple = അസത്യം;
  തരനിർവചനം AAT::ObsoleteTypes Types;

  unsigned   tuple_എണ്ണം_2 () സ്ഥിരം { തിരികെ 0; }
  തർക്കം     is_horizontal () സ്ഥിരം { തിരികെ (coverage & Horizontal); }

  എണ്ണൽ Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    CrossStream	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  HBUINT16	versionZ;	/* Unused. */
  HBUINT16	length;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

ഘടന KernOT : AAT::KerxTable<KernOT>
{
  friend ഘടന AAT::KerxTable<KernOT>;

  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  സ്ഥിതം constexpr unsigned minVersion = 0u;

  തരനിർവചനം KernOTSubTableHeader SubTableHeader;
  തരനിർവചനം SubTableHeader::Types Types;
  തരനിർവചനം KernSubTable<SubTableHeader> SubTable;

  സംരക്ഷിത:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_എണ്ണം;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  പൊതു:
  DEFINE_SIZE_MIN (4);
};


ഘടന KernAATSubTableHeader
{
  സ്ഥിതം constexpr തർക്കം apple = സത്യം;
  തരനിർവചനം AAT::ObsoleteTypes Types;

  unsigned   tuple_എണ്ണം_2 () സ്ഥിരം { തിരികെ 0; }
  തർക്കം     is_horizontal () സ്ഥിരം { തിരികെ !(coverage & Vertical); }

  എണ്ണൽ Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  HBUINT32	length;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tupleIndex;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന KernAAT : AAT::KerxTable<KernAAT>
{
  friend ഘടന AAT::KerxTable<KernAAT>;

  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  സ്ഥിതം constexpr unsigned minVersion = 0x00010000u;

  തരനിർവചനം KernAATSubTableHeader SubTableHeader;
  തരനിർവചനം SubTableHeader::Types Types;
  തരനിർവചനം KernSubTable<SubTableHeader> SubTable;

  സംരക്ഷിത:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_എണ്ണം;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  പൊതു:
  DEFINE_SIZE_MIN (8);
};

ഘടന kern
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  തർക്കം     has_data () സ്ഥിരം { തിരികെ u.version32.v; }
  unsigned get_type () സ്ഥിരം { തിരികെ u.major.v; }

  തർക്കം has_state_machine () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (get_type ()) {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.ot.has_state_machine ();
#ifndef HB_NO_AAT_SHAPE
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.aat.has_state_machine ();
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }

  തർക്കം has_cross_stream () സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (get_type ()) {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.ot.has_cross_stream ();
#ifndef HB_NO_AAT_SHAPE
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.aat.has_cross_stream ();
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ അസത്യം;
    }
  }

  പൂർണ്ണസംഖ്യ get_h_kerning (hb_codepoint_t left, hb_codepoint_t വലത്) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (get_type ()) {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.ot.get_h_kerning (left, വലത്);
#ifndef HB_NO_AAT_SHAPE
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.aat.get_h_kerning (left, വലത്);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  തർക്കം apply (AAT::hb_aat_apply_context_t *c,
	      സ്ഥിരം AAT::kern_accelerator_data_t &accel_data) സ്ഥിരം
  { തിരികെ dispatch (c, accel_data); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ subtable_type = get_type ();
    TRACE_DISPATCH (ഇത്, subtable_type);
    തിരഞ്ഞെടുപ്പ് (subtable_type) {
    അവസ്ഥ 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ifndef HB_NO_AAT_SHAPE
    അവസ്ഥ 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#endif
    സ്ഥിരസ്ഥിതി:	return_trace (c->default_return_മൂല്യം ());
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.version32.v.sanitize (c)) return_trace (അസത്യം);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_data_t create_accelerator_data (unsigned അക്കം_glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (get_type ()) {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.ot.create_accelerator_data (അക്കം_glyphs);
#ifndef HB_NO_AAT_SHAPE
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.aat.create_accelerator_data (അക്കം_glyphs);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ AAT::kern_accelerator_data_t ();
    }
  }

  ഘടന accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ഇത്->table = sc.reference_table<kern> (face);
      ഇത്->accel_data = ഇത്->table->create_accelerator_data (face->get_അക്കം_glyphs ());
    }
    ~accelerator_t ()
    {
      ഇത്->table.destroy ();
    }

    hb_blob_t *get_blob () സ്ഥിരം { തിരികെ table.get_blob (); }

    തർക്കം apply (AAT::hb_aat_apply_context_t *c) സ്ഥിരം
    {
      തിരികെ table->apply (c, accel_data);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_data_t accel_data;
    AAT::hb_aat_scratch_t scratch;
  };

  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT32 v; }	version32;
  ഘടന { HBUINT16 v; }	major;
  KernOT		ot;
#ifndef HB_NO_AAT_SHAPE
  KernAAT		aat;
#endif
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (4, version32.v);
};

ഘടന kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_KERN_TABLE_HH */
