/*
 * Copyright © 2018  Ebrahim Byagowi
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
 */

#నిర్వచితం_కాకపోతే HB_AAT_LAYOUT_JUST_పట్టిక_HH
#నిర్వచించు HB_AAT_LAYOUT_JUST_పట్టిక_HH

#అన్తర్భావ "hb-aat-layout-common.hh"
#అన్తర్భావ "hb-ot-layout.hh"
#అన్తర్భావ "hb-open-type.hh"

#అన్తర్భావ "hb-aat-layout-morx-table.hh"

/*
 * just -- Justification
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6just.html
 */
#నిర్వచించు HB_AAT_ట్యాగ్_just HB_ట్యాగ్('j','u','s','t')


నామాకాశ AAT {

ఉపయోగించి నామాకాశ OT;


నిర్మాణం Action_Subrecord_శీర్షిక
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  HBUINT16	actionClass;	/* The JustClass value associated with this
				 * ActionSubrecord. */
  HBUINT16	actionType;	/* The type of postcompensation action. */
  HBUINT16	action_నిడివి;	/* Length of this ActionSubrecord record, which
				 * must be a multiple of 4. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం DecompositionAction
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  Action_Subrecord_శీర్షిక
		శీర్షిక;
  F16DOT16	క్రింది_హద్దు;	/* If the distance factor is less than this value,
				 * then the ligature is decomposed. */
  F16DOT16	పై_హద్దు;	/* If the distance factor is greater than this value,
				 * then the ligature is decomposed. */
  HBUINT16	క్రమం;		/* Numerical order in which this ligature will
				 * be decomposed; you may want infrequent ligatures
				 * to decompose before more frequent ones. The ligatures
				 * on the line of text will decompose in increasing
				 * value of this field. */
  Array16_యొక్క<HBUINT16>
		decomposedglyphs;
				/* Number of 16-bit glyph indexes that follow;
				 * the ligature will be decomposed into these glyphs.
				 *
				 * Array of decomposed glyphs. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (18, decomposedglyphs);
};

నిర్మాణం Unconditional_కూడు_Glyph_Action
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  Action_Subrecord_శీర్షిక
		శీర్షిక;
  HBGlyphID16	కూడు_Glyph;	/* Glyph that should be added if the distance factor
				 * is growing. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం Conditional_కూడు_Glyph_Action
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  Action_Subrecord_శీర్షిక
		శీర్షిక;
  F16DOT16	subst_పరిమితి; /* Distance growth factor (in ems) at which
				 * this glyph is replaced and the growth factor
				 * recalculated. */
  HBGlyphID16	కూడు_Glyph;	/* Glyph to be added as kashida. If this value is
				 * 0xFFFF, no extra glyph will be added. Note that
				 * generally when a glyph is added, justification
				 * will need to be redone. */
  HBGlyphID16	substGlyph;	/* Glyph to be substituted for this glyph if the
				 * growth factor equals or exceeds the value of
				 * substThreshold. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (14);
};

నిర్మాణం DuctileGlyphAction
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  Action_Subrecord_శీర్షిక
		శీర్షిక;
  HBUINT32	variationAxis;	/* The 4-byte tag identifying the ductile axis.
				 * This would normally be 0x64756374 ('duct'),
				 * but you may use any axis the font contains. */
  F16DOT16	minimum_హద్దు;	/* The lowest value for the ductility axis that
				 * still yields an acceptable appearance. Normally
				 * this will be 1.0. */
  F16DOT16	no_Stretch_విలువ; /* This is the default value that corresponds to
				 * no change in appearance. Normally, this will
				 * be 1.0. */
  F16DOT16	గరిష్ఠ_హద్దు;	/* The highest value for the ductility axis that
				 * still yields an acceptable appearance. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (22);
};

నిర్మాణం Repeated_కూడు_Glyph_Action
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  Action_Subrecord_శీర్షిక
		శీర్షిక;
  HBUINT16	flags;		/* Currently unused; set to 0. */
  HBGlyphID16	glyph;		/* Glyph that should be added if the distance factor
				 * is growing. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (10);
};

నిర్మాణం ActionSubrecord
{
  గుర్తులేని పూర్ణాంకం get_నిడివి () స్థిరరాశి { తిరిగి u.శీర్షిక.action_నిడివి; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది)))
      return_trace (అబద్ధం);
    hb_barrier ();

    ఎంపిక (u.శీర్షిక.actionType)
    {
    స్థితి 0: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    స్థితి 1: hb_barrier ();  return_trace (u.unconditional_కూడు_Glyph_Action.sanitize (c));
    స్థితి 2: hb_barrier ();  return_trace (u.conditional_కూడు_Glyph_Action.sanitize (c));
    // case 3: hb_barrier (); return_trace (u.stretchGlyphAction.sanitize (c));
    స్థితి 4: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    స్థితి 5: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    అప్రమేయ: return_trace (నిజం);
    }
  }

  రక్షిత:
  సంఘం	{
  Action_Subrecord_శీర్షిక		శీర్షిక;
  DecompositionAction		decompositionAction;
  Unconditional_కూడు_Glyph_Action	unconditional_కూడు_Glyph_Action;
  Conditional_కూడు_Glyph_Action	conditional_కూడు_Glyph_Action;
  /* StretchGlyphAction stretchGlyphAction; -- Not supported by CoreText */
  DuctileGlyphAction		ductileGlyphAction;
  Repeated_కూడు_Glyph_Action	repeated_కూడు_Glyph_Action;
  } u;				/* Data. The format of this data depends on
				 * the value of the actionType field. */
  బహిరంగ:
  DEFINE_కొలత_UNION (6, శీర్షిక);
};

నిర్మాణం PostcompensationActionChain
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది)))
      return_trace (అబద్ధం);
    hb_barrier ();

    గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = min_కొలత;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      స్థిరరాశి ActionSubrecord& subrecord = Struct_At_స్థానభ్రంశం<ActionSubrecord> (ఇది, స్థానభ్రంశం);
      ఒకవేళ (unlikely (!subrecord.sanitize (c))) return_trace (అబద్ధం);
      స్థానభ్రంశం += subrecord.get_నిడివి ();
    }

    return_trace (నిజం);
  }

  రక్షిత:
  HBUINT32	లెక్క;

  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం Just_వెడల్పు_Delta_నమోదు
{
  గణన Flags
  {
    Reserved1		=0xE000,/* Reserved. You should set these bits to zero. */
    UnlimiteGap		=0x1000,/* The glyph can take unlimited gap. When this
				 * glyph participates in the justification process,
				 * it and any other glyphs on the line having this
				 * bit set absorb all the remaining gap. */
    Reserved2		=0x0FF0,/* Reserved. You should set these bits to zero. */
    ప్రాధాన్యత_2		=0x000F /* The justification priority of the glyph. */
  };

  గణన ప్రాధాన్యత_2
  {
    Kashida		= 0,	/* Kashida priority. This is the highest priority
				 * during justification. */
    Whitespace		= 1,	/* Whitespace priority. Any whitespace glyphs (as
				 * identified in the glyph properties table) will
				 * get this priority. */
    InterCharacter	= 2,	/* Inter-character priority. Give this to any
				 * remaining glyphs. */
    Null_ప్రాధాన్యత	= 3	/* Null priority. You should set this priority for
				 * glyphs that only participate in justification
				 * after the above priorities. Normally all glyphs
				 * have one of the previous three values. If you
				 * don't want a glyph to participate in justification,
				 * and you don't want to set its factors to zero,
				 * you may instead assign it to the null priority. */
  };

  రక్షిత:
  F16DOT16	before_Grow_హద్దు;/* The ratio by which the advance width of the
				 * glyph is permitted to grow on the left or top side. */
  F16DOT16	before_Shrink_హద్దు;
				/* The ratio by which the advance width of the
				 * glyph is permitted to shrink on the left or top side. */
  F16DOT16	after_Grow_హద్దు;	/* The ratio by which the advance width of the glyph
				 * is permitted to shrink on the left or top side. */
  F16DOT16	after_Shrink_హద్దు;
				/* The ratio by which the advance width of the glyph
				 * is at most permitted to shrink on the right or
				 * bottom side. */
  HBUINT16	growFlags;	/* Flags controlling the grow case. */
  HBUINT16	shrinkFlags;	/* Flags controlling the shrink case. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (20);
};

నిర్మాణం వెడల్పు_Delta_Pair
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT32	justClass;	/* The justification category associated
				 * with the wdRecord field. Only 7 bits of
				 * this field are used. (The other bits are
				 * used as padding to guarantee longword
				 * alignment of the following record). */
  Just_వెడల్పు_Delta_నమోదు
		wd_రికార్డు;	/* The actual width delta record. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (24);
};

రకనిర్వచనం OT::Array32_యొక్క<వెడల్పు_Delta_Pair> వెడల్పు_Delta_Cluster;

నిర్మాణం JustificationCategory
{
  రకనిర్వచనం శూన్య నమోదు_దత్తాంశం;

  గణన Flags
  {
    సమితి_Mark		=0x8000,/* If set, make the current glyph the marked
				 * glyph. */
    DontAdvance		=0x4000,/* If set, don't advance to the next glyph before
				 * going to the new state. */
    MarkCategory	=0x3F80,/* The justification category for the marked
				 * glyph if nonzero. */
    CurrentCategory	=0x007F /* The justification category for the current
				 * glyph if nonzero. */
  };

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  morph_శీర్షిక.sanitize (c) &&
			  st_శీర్షిక.sanitize (c)));
  }

  రక్షిత:
  ChainSubtable<ObsoleteTypes>
		morph_శీర్షిక;	/* Metamorphosis-style subtable header. */
  State_పట్టిక<ObsoleteTypes, నమోదు_దత్తాంశం>
		st_శీర్షిక;	/* The justification insertion state table header */
  బహిరంగ:
  DEFINE_కొలత_STATIC (30);
};

నిర్మాణం Justification_శీర్షిక
{
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  just_Class_పట్టిక.sanitize (c, పునాది, పునాది) &&
			  wdc_పట్టిక.sanitize (c, పునాది) &&
			  pc_పట్టిక.sanitize (c, పునాది) &&
			  lookup_పట్టిక.sanitize (c, పునాది)));
  }

  రక్షిత:
  Offset16_గమ్యం<JustificationCategory>
		just_Class_పట్టిక;	/* Offset to the justification category state table. */
  Offset16_గమ్యం<వెడల్పు_Delta_Cluster>
		wdc_పట్టిక;	/* Offset from start of justification table to start
				 * of the subtable containing the width delta factors
				 * for the glyphs in your font.
				 *
				 * The width delta clusters table. */
  Offset16_గమ్యం<PostcompensationActionChain>
		pc_పట్టిక;	/* Offset from start of justification table to start
				 * of postcompensation subtable (set to zero if none).
				 *
				 * The postcompensation subtable, if present in the font. */
  Lookup<Offset16_గమ్యం<వెడల్పు_Delta_Cluster>>
		lookup_పట్టిక;	/* Lookup table associating glyphs with width delta
				 * clusters. See the description of Width Delta Clusters
				 * table for details on how to interpret the lookup values. */

  బహిరంగ:
  DEFINE_కొలత_MIN (8);
};

నిర్మాణం just
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_AAT_ట్యాగ్_just;

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);

    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  సంస్కరణ.major == 1 &&
			  horiz_దత్తాంశం.sanitize (c, ఇది, ఇది) &&
			  vert_దత్తాంశం.sanitize (c, ఇది, ఇది)));
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the justification table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the justification table (set to 0). */
  Offset16_గమ్యం<Justification_శీర్షిక>
		horiz_దత్తాంశం;	/* Byte offset from the start of the justification table
				 * to the header for tables that contain justification
				 * information for horizontal text.
				 * If you are not including this information,
				 * store 0. */
  Offset16_గమ్యం<Justification_శీర్షిక>
		vert_దత్తాంశం;	/* ditto, vertical */

  బహిరంగ:
  DEFINE_కొలత_STATIC (10);
};

} /* namespace AAT */


#అయితే_ముగింపు /* HB_AAT_LAYOUT_JUST_TABLE_HH */
