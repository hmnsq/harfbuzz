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

#अगर_अघोषित HB_OT_KERN_TABLE_HH
#घोषणा HB_OT_KERN_TABLE_HH

#समावेश "hb-aat-layout-common.hh"
#समावेश "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#घोषणा HB_OT_TAG_kern HB_TAG('k','e','r','n')


namespace OT {


ढाँचा <typename KernSubTableHeader>
काष्ठा KernSubTableFormat3
{
  पूर्णांक get_kerning (hb_codepoint_t बाँए, hb_codepoint_t दाँए) स्थिर
  {
    hb_array_t<स्थिर FWORD> kern_मूल्य = kern_मूल्य_Z.as_array (kern_मूल्य_गिनती);
    hb_array_t<स्थिर HBUINT8> बायाँ_Class = StructAfter<स्थिर UnsizedArrayOf<HBUINT8>> (kern_मूल्य).as_array (glyph_गिनती_2);
    hb_array_t<स्थिर HBUINT8> दायाँ_Class = StructAfter<स्थिर UnsizedArrayOf<HBUINT8>> (बायाँ_Class).as_array (glyph_गिनती_2);
    hb_array_t<स्थिर HBUINT8> kern_सूचकांक = StructAfter<स्थिर UnsizedArrayOf<HBUINT8>> (दायाँ_Class).as_array (बायाँ_Class_गिनती * दायाँ_Class_गिनती);

    अचिन्हित पूर्णांक बायाँ_C = बायाँ_Class[बाँए];
    अचिन्हित पूर्णांक दायाँ_C = दायाँ_Class[दाँए];
    अगर (unlikely (बायाँ_C >= बायाँ_Class_गिनती || दायाँ_C >= दायाँ_Class_गिनती))
      वापस 0;
    अचिन्हित पूर्णांक i = बायाँ_C * दायाँ_Class_गिनती + दायाँ_C;
    वापस kern_मूल्य[kern_सूचकांक[i]];
  }

  bool apply (AAT::hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    अगर (!c->plan->requested_kerning)
      वापस false;

    अगर (header.coverage & header.Backwards)
      वापस false;

    hb_kern_machine_t<KernSubTableFormat3> machine (*यह, header.coverage & header.CrossStream);
    machine.kern (c->font, c->बफर, c->plan->kern_mask);

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  c->check_range (kern_मूल्य_Z,
				  kern_मूल्य_गिनती * माप (FWORD) +
				  glyph_गिनती_2 * 2 +
				  बायाँ_Class_गिनती * दायाँ_Class_गिनती));
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &बायाँ_set, set_t &दायाँ_set, अचिन्हित अंक_glyphs) स्थिर
  {
    अगर (likely (glyph_गिनती_2))
    {
      बायाँ_set.add_range (0, अंक_glyphs - 1);
      दायाँ_set.add_range (0, अंक_glyphs - 1);
    }
  }

  रक्षित:
  KernSubTableHeader
		header;
  HBUINT16	glyph_गिनती_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_मूल्य_गिनती;	/* The number of kerning values. */
  HBUINT8	बायाँ_Class_गिनती;	/* The number of left-hand classes. */
  HBUINT8	दायाँ_Class_गिनती;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  UnsizedArrayOf<FWORD>
		kern_मूल्य_Z;	/* The kerning values.
				 * Length kernValueCount. */
#अगर 0
  UnsizedArrayOf<HBUINT8>
		बायाँ_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>
		दायाँ_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  UnsizedArrayOf<HBUINT8>kern_सूचकांक;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#पूर्ण_अगर
  खुला:
  DEFINE_आकार_ARRAY (KernSubTableHeader::static_आकार + 6, kern_मूल्य_Z);
};

ढाँचा <typename KernSubTableHeader>
काष्ठा KernSubTable
{
  माप_प्रकार get_आकार () स्थिर { वापस u.header.लंबाई; }
  अचिन्हित पूर्णांक get_type () स्थिर { वापस u.header.format; }

  पूर्णांक get_kerning (hb_codepoint_t बाँए, hb_codepoint_t दाँए) स्थिर
  {
    चयन (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    हाल 0: hb_barrier (); वापस u.format0.get_kerning (बाँए, दाँए);
    शेष:वापस 0;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    TRACE_DISPATCH (यह, subtable_type);
    चयन (subtable_type) {
    हाल 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#अगर_अघोषित HB_NO_AAT_SHAPE
    हाल 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#पूर्ण_अगर
    हाल 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#अगर_अघोषित HB_NO_AAT_SHAPE
    हाल 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#पूर्ण_अगर
    शेष:	return_trace (c->default_return_मूल्य ());
    }
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &बायाँ_set, set_t &दायाँ_set, अचिन्हित अंक_glyphs) स्थिर
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    चयन (subtable_type) {
    हाल 0:	hb_barrier (); u.format0.collect_glyphs (बायाँ_set, दायाँ_set, अंक_glyphs); वापस;
#अगर_अघोषित HB_NO_AAT_SHAPE
    हाल 1:	hb_barrier (); u.format1.collect_glyphs (बायाँ_set, दायाँ_set, अंक_glyphs); वापस;
#पूर्ण_अगर
    हाल 2:	hb_barrier (); u.format2.collect_glyphs (बायाँ_set, दायाँ_set, अंक_glyphs); वापस;
#अगर_अघोषित HB_NO_AAT_SHAPE
    हाल 3:	hb_barrier (); u.format3.collect_glyphs (बायाँ_set, दायाँ_set, अंक_glyphs); वापस;
#पूर्ण_अगर
    शेष:	वापस;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!(u.header.sanitize (c) &&
		    hb_barrier () &&
		    u.header.लंबाई >= u.header.min_आकार &&
		    c->check_range (यह, u.header.लंबाई)))) return_trace (false);

    return_trace (dispatch (c));
  }

  खुला:
  जोड़ {
  KernSubTableHeader				header;
  AAT::KerxSubTableFormat0<KernSubTableHeader>	format0;
#अगर_अघोषित HB_NO_AAT_SHAPE
  AAT::KerxSubTableFormat1<KernSubTableHeader>	format1;
#पूर्ण_अगर
  AAT::KerxSubTableFormat2<KernSubTableHeader>	format2;
#अगर_अघोषित HB_NO_AAT_SHAPE
  KernSubTableFormat3<KernSubTableHeader>	format3;
#पूर्ण_अगर
  } u;
  खुला:
  DEFINE_आकार_MIN (KernSubTableHeader::static_आकार);
};


काष्ठा KernOTSubTableHeader
{
  अटल constexpr bool apple = false;
  प्रकार AAT::ObsoleteTypes Types;

  अचिन्हित   tuple_गिनती_2 () स्थिर { वापस 0; }
  bool     is_horizontal () स्थिर { वापस (coverage & Horizontal); }

  क्रमागत Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    CrossStream	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBUINT16	versionZ;	/* Unused. */
  HBUINT16	लंबाई;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा KernOT : AAT::KerxTable<KernOT>
{
  मित्र काष्ठा AAT::KerxTable<KernOT>;

  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  अटल constexpr अचिन्हित minVersion = 0u;

  प्रकार KernOTSubTableHeader SubTableHeader;
  प्रकार SubTableHeader::Types Types;
  प्रकार KernSubTable<SubTableHeader> SubTable;

  रक्षित:
  HBUINT16	version;	/* Version--0x0000u */
  HBUINT16	table_गिनती;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  खुला:
  DEFINE_आकार_MIN (4);
};


काष्ठा KernAATSubTableHeader
{
  अटल constexpr bool apple = true;
  प्रकार AAT::ObsoleteTypes Types;

  अचिन्हित   tuple_गिनती_2 () स्थिर { वापस 0; }
  bool     is_horizontal () स्थिर { वापस !(coverage & Vertical); }

  क्रमागत Coverage
  {
    Vertical	= 0x80u,
    CrossStream	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBUINT32	लंबाई;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tuple_सूचकांक;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा KernAAT : AAT::KerxTable<KernAAT>
{
  मित्र काष्ठा AAT::KerxTable<KernAAT>;

  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_kern;
  अटल constexpr अचिन्हित minVersion = 0x00010000u;

  प्रकार KernAATSubTableHeader SubTableHeader;
  प्रकार SubTableHeader::Types Types;
  प्रकार KernSubTable<SubTableHeader> SubTable;

  रक्षित:
  HBUINT32	version;	/* Version--0x00010000u */
  HBUINT32	table_गिनती;	/* Number of subtables in the kerning table. */
  SubTable	firstSubTable;	/* Subtables. */
  खुला:
  DEFINE_आकार_MIN (8);
};

काष्ठा kern
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_kern;

  bool     has_तथ्य () स्थिर { वापस u.version32.v; }
  अचिन्हित get_type () स्थिर { वापस u.major.v; }

  bool has_state_machine () स्थिर
  {
    चयन (get_type ()) {
    हाल 0: hb_barrier (); वापस u.ot.has_state_machine ();
#अगर_अघोषित HB_NO_AAT_SHAPE
    हाल 1: hb_barrier (); वापस u.aat.has_state_machine ();
#पूर्ण_अगर
    शेष:वापस false;
    }
  }

  bool has_cross_stream () स्थिर
  {
    चयन (get_type ()) {
    हाल 0: hb_barrier (); वापस u.ot.has_cross_stream ();
#अगर_अघोषित HB_NO_AAT_SHAPE
    हाल 1: hb_barrier (); वापस u.aat.has_cross_stream ();
#पूर्ण_अगर
    शेष:वापस false;
    }
  }

  पूर्णांक get_h_kerning (hb_codepoint_t बाँए, hb_codepoint_t दाँए) स्थिर
  {
    चयन (get_type ()) {
    हाल 0: hb_barrier (); वापस u.ot.get_h_kerning (बाँए, दाँए);
#अगर_अघोषित HB_NO_AAT_SHAPE
    हाल 1: hb_barrier (); वापस u.aat.get_h_kerning (बाँए, दाँए);
#पूर्ण_अगर
    शेष:वापस 0;
    }
  }

  bool apply (AAT::hb_aat_apply_context_t *c,
	      स्थिर AAT::kern_accelerator_तथ्य_t &accel_तथ्य) स्थिर
  { वापस dispatch (c, accel_तथ्य); }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    TRACE_DISPATCH (यह, subtable_type);
    चयन (subtable_type) {
    हाल 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#अगर_अघोषित HB_NO_AAT_SHAPE
    हाल 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#पूर्ण_अगर
    शेष:	return_trace (c->default_return_मूल्य ());
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.version32.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_तथ्य_t create_accelerator_तथ्य (अचिन्हित अंक_glyphs) स्थिर
  {
    चयन (get_type ()) {
    हाल 0: hb_barrier (); वापस u.ot.create_accelerator_तथ्य (अंक_glyphs);
#अगर_अघोषित HB_NO_AAT_SHAPE
    हाल 1: hb_barrier (); वापस u.aat.create_accelerator_तथ्य (अंक_glyphs);
#पूर्ण_अगर
    शेष:वापस AAT::kern_accelerator_तथ्य_t ();
    }
  }

  काष्ठा accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      यह->table = sc.reference_table<kern> (face);
      यह->accel_तथ्य = यह->table->create_accelerator_तथ्य (face->get_अंक_glyphs ());
    }
    ~accelerator_t ()
    {
      यह->table.destroy ();
    }

    hb_blob_t *get_blob () स्थिर { वापस table.get_blob (); }

    bool apply (AAT::hb_aat_apply_context_t *c) स्थिर
    {
      वापस table->apply (c, accel_तथ्य);
    }

    hb_blob_ptr_t<kern> table;
    AAT::kern_accelerator_तथ्य_t accel_तथ्य;
    AAT::hb_aat_scratch_t scratch;
  };

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT32 v; }	version32;
  काष्ठा { HBUINT16 v; }	major;
  KernOT		ot;
#अगर_अघोषित HB_NO_AAT_SHAPE
  KernAAT		aat;
#पूर्ण_अगर
  } u;
  खुला:
  DEFINE_आकार_UNION (4, version32.v);
};

काष्ठा kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_KERN_TABLE_HH */
