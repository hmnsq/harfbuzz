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
  पूर्णांक get_kerning (hb_codepoint_t left, hb_codepoint_t उजवा) स्थिरांक
  {
    hb_array_t<स्थिरांक FWORD> kern_मूल्य = kern_मूल्य_Z.as_array (kern_मूल्य_मोजणी);
    hb_array_t<स्थिरांक HBUINT8> डावा_Class = StructAfter<स्थिरांक UnsizedArrayOf<HBUINT8>> (kern_मूल्य).as_array (glyph_मोजणी_2);
    hb_array_t<स्थिरांक HBUINT8> उजवा_Class = StructAfter<स्थिरांक UnsizedArrayOf<HBUINT8>> (डावा_Class).as_array (glyph_मोजणी_2);
    hb_array_t<स्थिरांक HBUINT8> kernIndex = StructAfter<स्थिरांक UnsizedArrayOf<HBUINT8>> (उजवा_Class).as_array (डावा_Class_मोजणी * उजवा_Class_मोजणी);

    अचिन्हित पूर्णांक डावा_C = डावा_Class[left];
    अचिन्हित पूर्णांक उजवा_C = उजवा_Class[उजवा];
    जर (unlikely (डावा_C >= डावा_Class_मोजणी || उजवा_C >= उजवा_Class_मोजणी))
      परत 0;
    अचिन्हित पूर्णांक i = डावा_C * उजवा_Class_मोजणी + उजवा_C;
    परत kern_मूल्य[kernIndex[i]];
  }

  तार्किक apply (AAT::hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    जर (!c->plan->requested_kerning)
      परत असत्य;

    जर (header.coverage & header.Backwards)
      परत असत्य;

    hb_kern_machine_t<KernSubTableFormat3> machine (*हे, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  c->check_range (kern_मूल्य_Z,
				  kern_मूल्य_मोजणी * आकार (FWORD) +
				  glyph_मोजणी_2 * 2 +
				  डावा_Class_मोजणी * उजवा_Class_मोजणी));
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &डावा_set, set_t &उजवा_set, अचिन्हित अंक_glyphs) स्थिरांक
  {
    जर (likely (glyph_मोजणी_2))
    {
      डावा_set.add_range (0, अंक_glyphs - 1);
      उजवा_set.add_range (0, अंक_glyphs - 1);
    }
  }

  संरक्षित:
  KernSubTableHeader
		header;
  HBUINT16	glyph_मोजणी_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_मूल्य_मोजणी;	/* The number of kerning values. */
  HBUINT8	डावा_Class_मोजणी;	/* The number of left-hand classes. */
  HBUINT8	उजवा_Class_मोजणी;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  UnsizedArrayOf<FWORD>
		kern_मूल्य_Z;	/* The kerning values.
				 * Length kernValueCount. */
#जर 0
  UnsizedArrayOf<HBUINT8>
		डावा_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>
		उजवा_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>kernIndex;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#endif
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 6, kern_मूल्य_Z);
};

ढाँचा <typename KernSubTableHeader>
संरचना KernSubTable
{
  माप_प्रकार get_size () स्थिरांक { परत u.header.length; }
  अचिन्हित पूर्णांक get_type () स्थिरांक { परत u.header.format; }

  पूर्णांक get_kerning (hb_codepoint_t left, hb_codepoint_t उजवा) स्थिरांक
  {
    निवड (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    स्थिती 0: hb_barrier (); परत u.format0.get_kerning (left, उजवा);
    पूर्वनिर्धारित:परत 0;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    TRACE_DISPATCH (हे, subtable_type);
    निवड (subtable_type) {
    स्थिती 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ifndef HB_NO_AAT_SHAPE
    स्थिती 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#endif
    स्थिती 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ifndef HB_NO_AAT_SHAPE
    स्थिती 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#endif
    पूर्वनिर्धारित:	return_trace (c->default_return_मूल्य ());
    }
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &डावा_set, set_t &उजवा_set, अचिन्हित अंक_glyphs) स्थिरांक
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    निवड (subtable_type) {
    स्थिती 0:	hb_barrier (); u.format0.collect_glyphs (डावा_set, उजवा_set, अंक_glyphs); परत;
#ifndef HB_NO_AAT_SHAPE
    स्थिती 1:	hb_barrier (); u.format1.collect_glyphs (डावा_set, उजवा_set, अंक_glyphs); परत;
#endif
    स्थिती 2:	hb_barrier (); u.format2.collect_glyphs (डावा_set, उजवा_set, अंक_glyphs); परत;
#ifndef HB_NO_AAT_SHAPE
    स्थिती 3:	hb_barrier (); u.format3.collect_glyphs (डावा_set, उजवा_set, अंक_glyphs); परत;
#endif
    पूर्वनिर्धारित:	परत;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.length >= u.header.min_size &&
		    c->check_range (हे, u.header.length)))) return_trace (असत्य);

    return_trace (dispatch (c));
  }

  सार्वजनिक:
  संघ {
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
  प्रकारव्याख्या AAT::ObsoleteTypes Types;

  अचिन्हित   tuple_मोजणी_2 () स्थिरांक { परत 0; }
  तार्किक     is_horizontal () स्थिरांक { परत (coverage & Horizontal); }

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

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
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

  प्रकारव्याख्या KernOTSubTableHeader SubTableHeader;
  प्रकारव्याख्या SubTableHeader::Types Types;
  प्रकारव्याख्या KernSubTable<SubTableHeader> SubTable;

  संरक्षित:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_मोजणी;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  सार्वजनिक:
  DEFINE_SIZE_MIN (4);
};


संरचना KernAATSubTableHeader
{
  स्थिर constexpr तार्किक apple = सत्य;
  प्रकारव्याख्या AAT::ObsoleteTypes Types;

  अचिन्हित   tuple_मोजणी_2 () स्थिरांक { परत 0; }
  तार्किक     is_horizontal () स्थिरांक { परत !(coverage & Vertical); }

  गणना Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
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

  प्रकारव्याख्या KernAATSubTableHeader SubTableHeader;
  प्रकारव्याख्या SubTableHeader::Types Types;
  प्रकारव्याख्या KernSubTable<SubTableHeader> SubTable;

  संरक्षित:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_मोजणी;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

संरचना kern
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  तार्किक     has_data () स्थिरांक { परत u.version32.v; }
  अचिन्हित get_type () स्थिरांक { परत u.major.v; }

  तार्किक has_state_machine () स्थिरांक
  {
    निवड (get_type ()) {
    स्थिती 0: hb_barrier (); परत u.ot.has_state_machine ();
#ifndef HB_NO_AAT_SHAPE
    स्थिती 1: hb_barrier (); परत u.aat.has_state_machine ();
#endif
    पूर्वनिर्धारित:परत असत्य;
    }
  }

  तार्किक has_cross_stream () स्थिरांक
  {
    निवड (get_type ()) {
    स्थिती 0: hb_barrier (); परत u.ot.has_cross_stream ();
#ifndef HB_NO_AAT_SHAPE
    स्थिती 1: hb_barrier (); परत u.aat.has_cross_stream ();
#endif
    पूर्वनिर्धारित:परत असत्य;
    }
  }

  पूर्णांक get_h_kerning (hb_codepoint_t left, hb_codepoint_t उजवा) स्थिरांक
  {
    निवड (get_type ()) {
    स्थिती 0: hb_barrier (); परत u.ot.get_h_kerning (left, उजवा);
#ifndef HB_NO_AAT_SHAPE
    स्थिती 1: hb_barrier (); परत u.aat.get_h_kerning (left, उजवा);
#endif
    पूर्वनिर्धारित:परत 0;
    }
  }

  तार्किक apply (AAT::hb_aat_apply_context_t *c,
	      स्थिरांक AAT::kern_accelerator_data_t &accel_data) स्थिरांक
  { परत dispatch (c, accel_data); }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    TRACE_DISPATCH (हे, subtable_type);
    निवड (subtable_type) {
    स्थिती 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ifndef HB_NO_AAT_SHAPE
    स्थिती 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#endif
    पूर्वनिर्धारित:	return_trace (c->default_return_मूल्य ());
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!u.version32.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_data_t create_accelerator_data (अचिन्हित अंक_glyphs) स्थिरांक
  {
    निवड (get_type ()) {
    स्थिती 0: hb_barrier (); परत u.ot.create_accelerator_data (अंक_glyphs);
#ifndef HB_NO_AAT_SHAPE
    स्थिती 1: hb_barrier (); परत u.aat.create_accelerator_data (अंक_glyphs);
#endif
    पूर्वनिर्धारित:परत AAT::kern_accelerator_data_t ();
    }
  }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      हे->table = sc.reference_table<kern> (face);
      हे->accel_data = हे->table->create_accelerator_data (face->get_अंक_glyphs ());
    }
    ~accelerator_t ()
    {
      हे->table.destroy ();
    }

    hb_blob_t *get_blob () स्थिरांक { परत table.get_blob (); }

    तार्किक apply (AAT::hb_aat_apply_context_t *c) स्थिरांक
    {
      परत table->apply (c, accel_data);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_data_t accel_data;
    AAT::hb_aat_scratch_t scratch;
  };

  संरक्षित:
  संघ {
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
