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

#अन्तर्भाव "hb-aat-layout-common.hh"
#अन्तर्भाव "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#define HB_OT_TAG_kern HB_TAG('k','e','r','n')


नामाकाश OT {


ढाँचा <typename KernSubTableHeader>
संरचना KernSubTableFormat3
{
  पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दायाँ) अचर
  {
    hb_array_t<अचर FWORD> kern_मान = kern_मान_Z.as_array (kern_मान_गन्ती);
    hb_array_t<अचर HBUINT8> बायाँ_Class = StructAfter<अचर UnsizedArrayOf<HBUINT8>> (kern_मान).as_array (glyph_गन्ती_2);
    hb_array_t<अचर HBUINT8> दायाँ_Class = StructAfter<अचर UnsizedArrayOf<HBUINT8>> (बायाँ_Class).as_array (glyph_गन्ती_2);
    hb_array_t<अचर HBUINT8> kernIndex = StructAfter<अचर UnsizedArrayOf<HBUINT8>> (दायाँ_Class).as_array (बायाँ_Class_गन्ती * दायाँ_Class_गन्ती);

    अचिन्हित पूर्णाङ्क बायाँ_C = बायाँ_Class[left];
    अचिन्हित पूर्णाङ्क दायाँ_C = दायाँ_Class[दायाँ];
    यदि (unlikely (बायाँ_C >= बायाँ_Class_गन्ती || दायाँ_C >= दायाँ_Class_गन्ती))
      फर्काऊ 0;
    अचिन्हित पूर्णाङ्क i = बायाँ_C * दायाँ_Class_गन्ती + दायाँ_C;
    फर्काऊ kern_मान[kernIndex[i]];
  }

  तार्किक apply (AAT::hb_aat_apply_context_t *c) अचर
  {
    TRACE_APPLY (एतत्);

    यदि (!c->plan->requested_kerning)
      फर्काऊ असत्य;

    यदि (header.coverage & header.Backwards)
      फर्काऊ असत्य;

    hb_kern_machine_t<KernSubTableFormat3> machine (*एतत्, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  c->check_range (kern_मान_Z,
				  kern_मान_गन्ती * परिमाण (FWORD) +
				  glyph_गन्ती_2 * 2 +
				  बायाँ_Class_गन्ती * दायाँ_Class_गन्ती));
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &बायाँ_set, set_t &दायाँ_set, अचिन्हित अङ्क_glyphs) अचर
  {
    यदि (likely (glyph_गन्ती_2))
    {
      बायाँ_set.add_range (0, अङ्क_glyphs - 1);
      दायाँ_set.add_range (0, अङ्क_glyphs - 1);
    }
  }

  रक्षित:
  KernSubTableHeader
		header;
  HBUINT16	glyph_गन्ती_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_मान_गन्ती;	/* The number of kerning values. */
  HBUINT8	बायाँ_Class_गन्ती;	/* The number of left-hand classes. */
  HBUINT8	दायाँ_Class_गन्ती;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  UnsizedArrayOf<FWORD>
		kern_मान_Z;	/* The kerning values.
				 * Length kernValueCount. */
#यदि 0
  UnsizedArrayOf<HBUINT8>
		बायाँ_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>
		दायाँ_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>kernIndex;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#endif
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 6, kern_मान_Z);
};

ढाँचा <typename KernSubTableHeader>
संरचना KernSubTable
{
  माप_प्रकार get_size () अचर { फर्काऊ u.header.length; }
  अचिन्हित पूर्णाङ्क get_type () अचर { फर्काऊ u.header.format; }

  पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दायाँ) अचर
  {
    विकल्प (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    पक्ष 0: hb_barrier (); फर्काऊ u.format0.get_kerning (left, दायाँ);
    स्वभाव:फर्काऊ 0;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) अचर
  {
    अचिन्हित पूर्णाङ्क subtable_type = get_type ();
    TRACE_DISPATCH (एतत्, subtable_type);
    विकल्प (subtable_type) {
    पक्ष 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ifndef HB_NO_AAT_SHAPE
    पक्ष 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#endif
    पक्ष 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ifndef HB_NO_AAT_SHAPE
    पक्ष 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#endif
    स्वभाव:	return_trace (c->default_return_मान ());
    }
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &बायाँ_set, set_t &दायाँ_set, अचिन्हित अङ्क_glyphs) अचर
  {
    अचिन्हित पूर्णाङ्क subtable_type = get_type ();
    विकल्प (subtable_type) {
    पक्ष 0:	hb_barrier (); u.format0.collect_glyphs (बायाँ_set, दायाँ_set, अङ्क_glyphs); फर्काऊ;
#ifndef HB_NO_AAT_SHAPE
    पक्ष 1:	hb_barrier (); u.format1.collect_glyphs (बायाँ_set, दायाँ_set, अङ्क_glyphs); फर्काऊ;
#endif
    पक्ष 2:	hb_barrier (); u.format2.collect_glyphs (बायाँ_set, दायाँ_set, अङ्क_glyphs); फर्काऊ;
#ifndef HB_NO_AAT_SHAPE
    पक्ष 3:	hb_barrier (); u.format3.collect_glyphs (बायाँ_set, दायाँ_set, अङ्क_glyphs); फर्काऊ;
#endif
    स्वभाव:	फर्काऊ;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.length >= u.header.min_size &&
		    c->check_range (एतत्, u.header.length)))) return_trace (असत्य);

    return_trace (dispatch (c));
  }

  सार्वजनिक:
  सङ्घ {
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
  सार्वजनिक:
  DEFINE_SIZE_MIN (KernSubTableHeader::static_size);
};


संरचना KernOTSubTableHeader
{
  स्थिर constexpr तार्किक apple = असत्य;
  प्रकारनिर्देश AAT::ObsoleteTypes Types;

  अचिन्हित   tuple_गन्ती_2 () अचर { फर्काऊ 0; }
  तार्किक     is_horizontal () अचर { फर्काऊ (coverage & Horizontal); }

  गणना Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    CrossStream	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBUINT16	versionZ;	/* Unused. */
  HBUINT16	length;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना KernOT : AAT::KerxTable<KernOT>
{
  मित्र संरचना AAT::KerxTable<KernOT>;

  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  स्थिर constexpr अचिन्हित minVersion = 0u;

  प्रकारनिर्देश KernOTSubTableHeader SubTableHeader;
  प्रकारनिर्देश SubTableHeader::Types Types;
  प्रकारनिर्देश KernSubTable<SubTableHeader> SubTable;

  रक्षित:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_गन्ती;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  सार्वजनिक:
  DEFINE_SIZE_MIN (4);
};


संरचना KernAATSubTableHeader
{
  स्थिर constexpr तार्किक apple = सत्य;
  प्रकारनिर्देश AAT::ObsoleteTypes Types;

  अचिन्हित   tuple_गन्ती_2 () अचर { फर्काऊ 0; }
  तार्किक     is_horizontal () अचर { फर्काऊ !(coverage & Vertical); }

  गणना Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBUINT32	length;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tupleIndex;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना KernAAT : AAT::KerxTable<KernAAT>
{
  मित्र संरचना AAT::KerxTable<KernAAT>;

  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  स्थिर constexpr अचिन्हित minVersion = 0x00010000u;

  प्रकारनिर्देश KernAATSubTableHeader SubTableHeader;
  प्रकारनिर्देश SubTableHeader::Types Types;
  प्रकारनिर्देश KernSubTable<SubTableHeader> SubTable;

  रक्षित:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_गन्ती;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

संरचना kern
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  तार्किक     has_data () अचर { फर्काऊ u.version32.v; }
  अचिन्हित get_type () अचर { फर्काऊ u.major.v; }

  तार्किक has_state_machine () अचर
  {
    विकल्प (get_type ()) {
    पक्ष 0: hb_barrier (); फर्काऊ u.ot.has_state_machine ();
#ifndef HB_NO_AAT_SHAPE
    पक्ष 1: hb_barrier (); फर्काऊ u.aat.has_state_machine ();
#endif
    स्वभाव:फर्काऊ असत्य;
    }
  }

  तार्किक has_cross_stream () अचर
  {
    विकल्प (get_type ()) {
    पक्ष 0: hb_barrier (); फर्काऊ u.ot.has_cross_stream ();
#ifndef HB_NO_AAT_SHAPE
    पक्ष 1: hb_barrier (); फर्काऊ u.aat.has_cross_stream ();
#endif
    स्वभाव:फर्काऊ असत्य;
    }
  }

  पूर्णाङ्क get_h_kerning (hb_codepoint_t left, hb_codepoint_t दायाँ) अचर
  {
    विकल्प (get_type ()) {
    पक्ष 0: hb_barrier (); फर्काऊ u.ot.get_h_kerning (left, दायाँ);
#ifndef HB_NO_AAT_SHAPE
    पक्ष 1: hb_barrier (); फर्काऊ u.aat.get_h_kerning (left, दायाँ);
#endif
    स्वभाव:फर्काऊ 0;
    }
  }

  तार्किक apply (AAT::hb_aat_apply_context_t *c,
	      अचर AAT::kern_accelerator_data_t &accel_data) अचर
  { फर्काऊ dispatch (c, accel_data); }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) अचर
  {
    अचिन्हित पूर्णाङ्क subtable_type = get_type ();
    TRACE_DISPATCH (एतत्, subtable_type);
    विकल्प (subtable_type) {
    पक्ष 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ifndef HB_NO_AAT_SHAPE
    पक्ष 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#endif
    स्वभाव:	return_trace (c->default_return_मान ());
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!u.version32.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_data_t create_accelerator_data (अचिन्हित अङ्क_glyphs) अचर
  {
    विकल्प (get_type ()) {
    पक्ष 0: hb_barrier (); फर्काऊ u.ot.create_accelerator_data (अङ्क_glyphs);
#ifndef HB_NO_AAT_SHAPE
    पक्ष 1: hb_barrier (); फर्काऊ u.aat.create_accelerator_data (अङ्क_glyphs);
#endif
    स्वभाव:फर्काऊ AAT::kern_accelerator_data_t ();
    }
  }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      एतत्->table = sc.reference_table<kern> (face);
      एतत्->accel_data = एतत्->table->create_accelerator_data (face->get_अङ्क_glyphs ());
    }
    ~accelerator_t ()
    {
      एतत्->table.destroy ();
    }

    hb_blob_t *get_blob () अचर { फर्काऊ table.get_blob (); }

    तार्किक apply (AAT::hb_aat_apply_context_t *c) अचर
    {
      फर्काऊ table->apply (c, accel_data);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_data_t accel_data;
    AAT::hb_aat_scratch_t scratch;
  };

  रक्षित:
  सङ्घ {
  संरचना { HBUINT32 v; }	version32;
  संरचना { HBUINT16 v; }	major;
  KernOT		ot;
#ifndef HB_NO_AAT_SHAPE
  KernAAT		aat;
#endif
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (4, version32.v);
};

संरचना kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#endif /* HB_OT_KERN_TABLE_HH */
