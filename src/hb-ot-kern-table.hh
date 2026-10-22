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

#నిర్వచితం_కాకపోతే HB_OT_KERN_పట్టిక_HH
#నిర్వచించు HB_OT_KERN_పట్టిక_HH

#అన్తర్భావ "hb-aat-layout-common.hh"
#అన్తర్భావ "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#నిర్వచించు HB_OT_ట్యాగ్_kern HB_ట్యాగ్('k','e','r','n')


నామాకాశ OT {


మూస <రకం_పేరు Kern_Sub_పట్టిక_శీర్షిక>
నిర్మాణం Kern_Sub_పట్టిక_Format3
{
  పూర్ణాంకం get_kerning (hb_codepoint_t ఎడమ, hb_codepoint_t కుడి) స్థిరరాశి
  {
    hb_array_t<స్థిరరాశి FWORD> kern_విలువ = kern_విలువ_Z.as_array (kern_విలువ_లెక్క);
    hb_array_t<స్థిరరాశి HBUINT8> ఎడమ_Class = StructAfter<స్థిరరాశి Unsized_Array_యొక్క<HBUINT8>> (kern_విలువ).as_array (glyph_లెక్క_2);
    hb_array_t<స్థిరరాశి HBUINT8> కుడి_Class = StructAfter<స్థిరరాశి Unsized_Array_యొక్క<HBUINT8>> (ఎడమ_Class).as_array (glyph_లెక్క_2);
    hb_array_t<స్థిరరాశి HBUINT8> kern_సూచీ = StructAfter<స్థిరరాశి Unsized_Array_యొక్క<HBUINT8>> (కుడి_Class).as_array (ఎడమ_Class_లెక్క * కుడి_Class_లెక్క);

    గుర్తులేని పూర్ణాంకం ఎడమ_C = ఎడమ_Class[ఎడమ];
    గుర్తులేని పూర్ణాంకం కుడి_C = కుడి_Class[కుడి];
    ఒకవేళ (unlikely (ఎడమ_C >= ఎడమ_Class_లెక్క || కుడి_C >= కుడి_Class_లెక్క))
      తిరిగి 0;
    గుర్తులేని పూర్ణాంకం i = ఎడమ_C * కుడి_Class_లెక్క + కుడి_C;
    తిరిగి kern_విలువ[kern_సూచీ[i]];
  }

  తార్కిక apply (AAT::hb_aat_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    ఒకవేళ (!c->plan->requested_kerning)
      తిరిగి అబద్ధం;

    ఒకవేళ (శీర్షిక.coverage & శీర్షిక.Backwards)
      తిరిగి అబద్ధం;

    hb_kern_machine_t<Kern_Sub_పట్టిక_Format3> machine (*ఇది, శీర్షిక.coverage & శీర్షిక.Cross_ప్రవాహం);
    machine.kern (c->font, c->బఫరు, c->plan->kern_ముసుగు);

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  c->check_range (kern_విలువ_Z,
				  kern_విలువ_లెక్క * పరిమాణం (FWORD) +
				  glyph_లెక్క_2 * 2 +
				  ఎడమ_Class_లెక్క * కుడి_Class_లెక్క));
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &ఎడమ_సమితి, సమితి_t &కుడి_సమితి, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    ఒకవేళ (likely (glyph_లెక్క_2))
    {
      ఎడమ_సమితి.కూడు_range (0, అంకె_glyphs - 1);
      కుడి_సమితి.కూడు_range (0, అంకె_glyphs - 1);
    }
  }

  రక్షిత:
  Kern_Sub_పట్టిక_శీర్షిక
		శీర్షిక;
  HBUINT16	glyph_లెక్క_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_విలువ_లెక్క;	/* The number of kerning values. */
  HBUINT8	ఎడమ_Class_లెక్క;	/* The number of left-hand classes. */
  HBUINT8	కుడి_Class_లెక్క;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  Unsized_Array_యొక్క<FWORD>
		kern_విలువ_Z;	/* The kerning values.
				 * Length kernValueCount. */
#ఒకవేళ 0
  Unsized_Array_యొక్క<HBUINT8>
		ఎడమ_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  Unsized_Array_యొక్క<HBUINT8>
		కుడి_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  Unsized_Array_యొక్క<HBUINT8>kern_సూచీ;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#అయితే_ముగింపు
  బహిరంగ:
  DEFINE_కొలత_ARRAY (Kern_Sub_పట్టిక_శీర్షిక::static_కొలత + 6, kern_విలువ_Z);
};

మూస <రకం_పేరు Kern_Sub_పట్టిక_శీర్షిక>
నిర్మాణం Kern_Sub_పట్టిక
{
  size_t get_కొలత () స్థిరరాశి { తిరిగి u.శీర్షిక.నిడివి; }
  గుర్తులేని పూర్ణాంకం get_type () స్థిరరాశి { తిరిగి u.శీర్షిక.format; }

  పూర్ణాంకం get_kerning (hb_codepoint_t ఎడమ, hb_codepoint_t కుడి) స్థిరరాశి
  {
    ఎంపిక (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_kerning (ఎడమ, కుడి);
    అప్రమేయ:తిరిగి 0;
    }
  }

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం subtable_type = get_type ();
    TRACE_DISPATCH (ఇది, subtable_type);
    ఎంపిక (subtable_type) {
    స్థితి 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
    స్థితి 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#అయితే_ముగింపు
    స్థితి 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
    స్థితి 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#అయితే_ముగింపు
    అప్రమేయ:	return_trace (c->default_return_విలువ ());
    }
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_glyphs (సమితి_t &ఎడమ_సమితి, సమితి_t &కుడి_సమితి, గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం subtable_type = get_type ();
    ఎంపిక (subtable_type) {
    స్థితి 0:	hb_barrier (); u.format0.collect_glyphs (ఎడమ_సమితి, కుడి_సమితి, అంకె_glyphs); తిరిగి;
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
    స్థితి 1:	hb_barrier (); u.format1.collect_glyphs (ఎడమ_సమితి, కుడి_సమితి, అంకె_glyphs); తిరిగి;
#అయితే_ముగింపు
    స్థితి 2:	hb_barrier (); u.format2.collect_glyphs (ఎడమ_సమితి, కుడి_సమితి, అంకె_glyphs); తిరిగి;
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
    స్థితి 3:	hb_barrier (); u.format3.collect_glyphs (ఎడమ_సమితి, కుడి_సమితి, అంకె_glyphs); తిరిగి;
#అయితే_ముగింపు
    అప్రమేయ:	తిరిగి;
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!(u.శీర్షిక.sanitize (c) &&
		    hb_barrier () &&
		    u.శీర్షిక.నిడివి >= u.శీర్షిక.min_కొలత &&
		    c->check_range (ఇది, u.శీర్షిక.నిడివి)))) return_trace (అబద్ధం);

    return_trace (dispatch (c));
  }

  బహిరంగ:
  సంఘం {
  Kern_Sub_పట్టిక_శీర్షిక				శీర్షిక;
  AAT::Kerx_Sub_పట్టిక_Format0<Kern_Sub_పట్టిక_శీర్షిక>	format0;
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  AAT::Kerx_Sub_పట్టిక_Format1<Kern_Sub_పట్టిక_శీర్షిక>	format1;
#అయితే_ముగింపు
  AAT::Kerx_Sub_పట్టిక_Format2<Kern_Sub_పట్టిక_శీర్షిక>	format2;
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  Kern_Sub_పట్టిక_Format3<Kern_Sub_పట్టిక_శీర్షిక>	format3;
#అయితే_ముగింపు
  } u;
  బహిరంగ:
  DEFINE_కొలత_MIN (Kern_Sub_పట్టిక_శీర్షిక::static_కొలత);
};


నిర్మాణం Kern_OTSub_పట్టిక_శీర్షిక
{
  స్థిర constexpr తార్కిక apple = అబద్ధం;
  రకనిర్వచనం AAT::ObsoleteTypes Types;

  గుర్తులేని   tuple_లెక్క_2 () స్థిరరాశి { తిరిగి 0; }
  తార్కిక     is_horizontal () స్థిరరాశి { తిరిగి (coverage & Horizontal); }

  గణన Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    Cross_ప్రవాహం	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBUINT16	సంస్కరణ_Z;	/* Unused. */
  HBUINT16	నిడివి;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం KernOT : AAT::Kerx_పట్టిక<KernOT>
{
  మిత్ర నిర్మాణం AAT::Kerx_పట్టిక<KernOT>;

  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_kern;
  స్థిర constexpr గుర్తులేని min_సంస్కరణ = 0u;

  రకనిర్వచనం Kern_OTSub_పట్టిక_శీర్షిక Sub_పట్టిక_శీర్షిక;
  రకనిర్వచనం Sub_పట్టిక_శీర్షిక::Types Types;
  రకనిర్వచనం Kern_Sub_పట్టిక<Sub_పట్టిక_శీర్షిక> Sub_పట్టిక;

  రక్షిత:
  HBUINT16	సంస్కరణ;	/* Version--0x0000u */
  HBUINT16	పట్టిక_లెక్క;	/* Number of subtables in the kerning table. */
  Sub_పట్టిక	మొదటి_Sub_పట్టిక;	/* Subtables. */
  బహిరంగ:
  DEFINE_కొలత_MIN (4);
};


నిర్మాణం Kern_AATSub_పట్టిక_శీర్షిక
{
  స్థిర constexpr తార్కిక apple = నిజం;
  రకనిర్వచనం AAT::ObsoleteTypes Types;

  గుర్తులేని   tuple_లెక్క_2 () స్థిరరాశి { తిరిగి 0; }
  తార్కిక     is_horizontal () స్థిరరాశి { తిరిగి !(coverage & Vertical); }

  గణన Coverage
  {
    Vertical	= 0x80u,
    Cross_ప్రవాహం	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  బహిరంగ:
  HBUINT32	నిడివి;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tuple_సూచీ;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం KernAAT : AAT::Kerx_పట్టిక<KernAAT>
{
  మిత్ర నిర్మాణం AAT::Kerx_పట్టిక<KernAAT>;

  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_kern;
  స్థిర constexpr గుర్తులేని min_సంస్కరణ = 0x00010000u;

  రకనిర్వచనం Kern_AATSub_పట్టిక_శీర్షిక Sub_పట్టిక_శీర్షిక;
  రకనిర్వచనం Sub_పట్టిక_శీర్షిక::Types Types;
  రకనిర్వచనం Kern_Sub_పట్టిక<Sub_పట్టిక_శీర్షిక> Sub_పట్టిక;

  రక్షిత:
  HBUINT32	సంస్కరణ;	/* Version--0x00010000u */
  HBUINT32	పట్టిక_లెక్క;	/* Number of subtables in the kerning table. */
  Sub_పట్టిక	మొదటి_Sub_పట్టిక;	/* Subtables. */
  బహిరంగ:
  DEFINE_కొలత_MIN (8);
};

నిర్మాణం kern
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_kern;

  తార్కిక     has_దత్తాంశం () స్థిరరాశి { తిరిగి u.version32.v; }
  గుర్తులేని get_type () స్థిరరాశి { తిరిగి u.major.v; }

  తార్కిక has_state_machine () స్థిరరాశి
  {
    ఎంపిక (get_type ()) {
    స్థితి 0: hb_barrier (); తిరిగి u.ot.has_state_machine ();
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
    స్థితి 1: hb_barrier (); తిరిగి u.aat.has_state_machine ();
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }

  తార్కిక has_cross_ప్రవాహం () స్థిరరాశి
  {
    ఎంపిక (get_type ()) {
    స్థితి 0: hb_barrier (); తిరిగి u.ot.has_cross_ప్రవాహం ();
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
    స్థితి 1: hb_barrier (); తిరిగి u.aat.has_cross_ప్రవాహం ();
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }

  పూర్ణాంకం get_h_kerning (hb_codepoint_t ఎడమ, hb_codepoint_t కుడి) స్థిరరాశి
  {
    ఎంపిక (get_type ()) {
    స్థితి 0: hb_barrier (); తిరిగి u.ot.get_h_kerning (ఎడమ, కుడి);
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
    స్థితి 1: hb_barrier (); తిరిగి u.aat.get_h_kerning (ఎడమ, కుడి);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి 0;
    }
  }

  తార్కిక apply (AAT::hb_aat_apply_context_t *c,
	      స్థిరరాశి AAT::kern_accelerator_దత్తాంశం_t &accel_దత్తాంశం) స్థిరరాశి
  { తిరిగి dispatch (c, accel_దత్తాంశం); }

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం subtable_type = get_type ();
    TRACE_DISPATCH (ఇది, subtable_type);
    ఎంపిక (subtable_type) {
    స్థితి 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
    స్థితి 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#అయితే_ముగింపు
    అప్రమేయ:	return_trace (c->default_return_విలువ ());
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.version32.v.sanitize (c)) return_trace (అబద్ధం);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_దత్తాంశం_t create_accelerator_దత్తాంశం (గుర్తులేని అంకె_glyphs) స్థిరరాశి
  {
    ఎంపిక (get_type ()) {
    స్థితి 0: hb_barrier (); తిరిగి u.ot.create_accelerator_దత్తాంశం (అంకె_glyphs);
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
    స్థితి 1: hb_barrier (); తిరిగి u.aat.create_accelerator_దత్తాంశం (అంకె_glyphs);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి AAT::kern_accelerator_దత్తాంశం_t ();
    }
  }

  నిర్మాణం accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ఇది->పట్టిక = sc.reference_పట్టిక<kern> (face);
      ఇది->accel_దత్తాంశం = ఇది->పట్టిక->create_accelerator_దత్తాంశం (face->get_అంకె_glyphs ());
    }
    ~accelerator_t ()
    {
      ఇది->పట్టిక.destroy ();
    }

    hb_blob_t *get_blob () స్థిరరాశి { తిరిగి పట్టిక.get_blob (); }

    తార్కిక apply (AAT::hb_aat_apply_context_t *c) స్థిరరాశి
    {
      తిరిగి పట్టిక->apply (c, accel_దత్తాంశం);
    }

    hb_blob_ptr_t<kern> పట్టిక;
    AAT::kern_accelerator_దత్తాంశం_t accel_దత్తాంశం;
    AAT::hb_aat_scratch_t scratch;
  };

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT32 v; }	version32;
  నిర్మాణం { HBUINT16 v; }	major;
  KernOT		ot;
#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE
  KernAAT		aat;
#అయితే_ముగింపు
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (4, version32.v);
};

నిర్మాణం kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_KERN_TABLE_HH */
