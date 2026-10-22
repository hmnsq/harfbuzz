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

#ifndef HB_AAT_LAYOUT_JUST_TABLE_HH
#define HB_AAT_LAYOUT_JUST_TABLE_HH

#அந்தர்பாவ "hb-aat-layout-common.hh"
#அந்தர்பாவ "hb-ot-layout.hh"
#அந்தர்பாவ "hb-open-type.hh"

#அந்தர்பாவ "hb-aat-layout-morx-table.hh"

/*
 * just -- Justification
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6just.html
 */
#define HB_AAT_TAG_just HB_TAG('j','u','s','t')


நாமாகாஶ AAT {

using நாமாகாஶ OT;


கட்டமைப்பு ActionSubrecordHeader
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  HBUINT16	actionClass;	/* The JustClass value associated with this
				 * ActionSubrecord. */
  HBUINT16	actionType;	/* The type of postcompensation action. */
  HBUINT16	actionLength;	/* Length of this ActionSubrecord record, which
				 * must be a multiple of 4. */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு DecompositionAction
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  ActionSubrecordHeader
		header;
  F16DOT16	lowerLimit;	/* If the distance factor is less than this value,
				 * then the ligature is decomposed. */
  F16DOT16	upperLimit;	/* If the distance factor is greater than this value,
				 * then the ligature is decomposed. */
  HBUINT16	order;		/* Numerical order in which this ligature will
				 * be decomposed; you may want infrequent ligatures
				 * to decompose before more frequent ones. The ligatures
				 * on the line of text will decompose in increasing
				 * value of this field. */
  Array16Of<HBUINT16>
		decomposedglyphs;
				/* Number of 16-bit glyph indexes that follow;
				 * the ligature will be decomposed into these glyphs.
				 *
				 * Array of decomposed glyphs. */
  பொது:
  DEFINE_SIZE_ARRAY (18, decomposedglyphs);
};

கட்டமைப்பு UnconditionalAddGlyphAction
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  ActionSubrecordHeader
		header;
  HBGlyphID16	addGlyph;	/* Glyph that should be added if the distance factor
				 * is growing. */

  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு ConditionalAddGlyphAction
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  ActionSubrecordHeader
		header;
  F16DOT16	substThreshold; /* Distance growth factor (in ems) at which
				 * this glyph is replaced and the growth factor
				 * recalculated. */
  HBGlyphID16	addGlyph;	/* Glyph to be added as kashida. If this value is
				 * 0xFFFF, no extra glyph will be added. Note that
				 * generally when a glyph is added, justification
				 * will need to be redone. */
  HBGlyphID16	substGlyph;	/* Glyph to be substituted for this glyph if the
				 * growth factor equals or exceeds the value of
				 * substThreshold. */
  பொது:
  DEFINE_SIZE_STATIC (14);
};

கட்டமைப்பு DuctileGlyphAction
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  ActionSubrecordHeader
		header;
  HBUINT32	variationAxis;	/* The 4-byte tag identifying the ductile axis.
				 * This would normally be 0x64756374 ('duct'),
				 * but you may use any axis the font contains. */
  F16DOT16	minimumLimit;	/* The lowest value for the ductility axis that
				 * still yields an acceptable appearance. Normally
				 * this will be 1.0. */
  F16DOT16	no_Stretch_மதிப்பு; /* This is the default value that corresponds to
				 * no change in appearance. Normally, this will
				 * be 1.0. */
  F16DOT16	maximumLimit;	/* The highest value for the ductility axis that
				 * still yields an acceptable appearance. */
  பொது:
  DEFINE_SIZE_STATIC (22);
};

கட்டமைப்பு RepeatedAddGlyphAction
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  ActionSubrecordHeader
		header;
  HBUINT16	flags;		/* Currently unused; set to 0. */
  HBGlyphID16	glyph;		/* Glyph that should be added if the distance factor
				 * is growing. */
  பொது:
  DEFINE_SIZE_STATIC (10);
};

கட்டமைப்பு ActionSubrecord
{
  unsigned முழு get_length () மாறிலி { திரும்பு u.header.actionLength; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது)))
      return_trace (பொய்);
    hb_barrier ();

    தேர்வு (u.header.actionType)
    {
    நிலை 0: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    நிலை 1: hb_barrier ();  return_trace (u.unconditionalAddGlyphAction.sanitize (c));
    நிலை 2: hb_barrier ();  return_trace (u.conditionalAddGlyphAction.sanitize (c));
    // case 3: hb_barrier (); return_trace (u.stretchGlyphAction.sanitize (c));
    நிலை 4: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    நிலை 5: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    இயல்பு: return_trace (உண்மை);
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு	{
  ActionSubrecordHeader		header;
  DecompositionAction		decompositionAction;
  UnconditionalAddGlyphAction	unconditionalAddGlyphAction;
  ConditionalAddGlyphAction	conditionalAddGlyphAction;
  /* StretchGlyphAction stretchGlyphAction; -- Not supported by CoreText */
  DuctileGlyphAction		ductileGlyphAction;
  RepeatedAddGlyphAction	repeatedAddGlyphAction;
  } u;				/* Data. The format of this data depends on
				 * the value of the actionType field. */
  பொது:
  DEFINE_SIZE_UNION (6, header);
};

கட்டமைப்பு PostcompensationActionChain
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது)))
      return_trace (பொய்);
    hb_barrier ();

    unsigned முழு offset = min_size;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      மாறிலி ActionSubrecord& subrecord = StructAtOffset<ActionSubrecord> (இது, offset);
      எனில் (unlikely (!subrecord.sanitize (c))) return_trace (பொய்);
      offset += subrecord.get_length ();
    }

    return_trace (உண்மை);
  }

  பாதுகாக்கப்பட்ட:
  HBUINT32	மொத்தம்;

  பொது:
  DEFINE_SIZE_STATIC (4);
};

கட்டமைப்பு JustWidthDeltaEntry
{
  எண்ணிக்கை Flags
  {
    Reserved1		=0xE000,/* Reserved. You should set these bits to zero. */
    UnlimiteGap		=0x1000,/* The glyph can take unlimited gap. When this
				 * glyph participates in the justification process,
				 * it and any other glyphs on the line having this
				 * bit set absorb all the remaining gap. */
    Reserved2		=0x0FF0,/* Reserved. You should set these bits to zero. */
    Priority		=0x000F /* The justification priority of the glyph. */
  };

  எண்ணிக்கை Priority
  {
    Kashida		= 0,	/* Kashida priority. This is the highest priority
				 * during justification. */
    Whitespace		= 1,	/* Whitespace priority. Any whitespace glyphs (as
				 * identified in the glyph properties table) will
				 * get this priority. */
    InterCharacter	= 2,	/* Inter-character priority. Give this to any
				 * remaining glyphs. */
    NullPriority	= 3	/* Null priority. You should set this priority for
				 * glyphs that only participate in justification
				 * after the above priorities. Normally all glyphs
				 * have one of the previous three values. If you
				 * don't want a glyph to participate in justification,
				 * and you don't want to set its factors to zero,
				 * you may instead assign it to the null priority. */
  };

  பாதுகாக்கப்பட்ட:
  F16DOT16	beforeGrowLimit;/* The ratio by which the advance width of the
				 * glyph is permitted to grow on the left or top side. */
  F16DOT16	beforeShrinkLimit;
				/* The ratio by which the advance width of the
				 * glyph is permitted to shrink on the left or top side. */
  F16DOT16	afterGrowLimit;	/* The ratio by which the advance width of the glyph
				 * is permitted to shrink on the left or top side. */
  F16DOT16	afterShrinkLimit;
				/* The ratio by which the advance width of the glyph
				 * is at most permitted to shrink on the right or
				 * bottom side. */
  HBUINT16	growFlags;	/* Flags controlling the grow case. */
  HBUINT16	shrinkFlags;	/* Flags controlling the shrink case. */

  பொது:
  DEFINE_SIZE_STATIC (20);
};

கட்டமைப்பு WidthDeltaPair
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT32	justClass;	/* The justification category associated
				 * with the wdRecord field. Only 7 bits of
				 * this field are used. (The other bits are
				 * used as padding to guarantee longword
				 * alignment of the following record). */
  JustWidthDeltaEntry
		wdRecord;	/* The actual width delta record. */

  பொது:
  DEFINE_SIZE_STATIC (24);
};

வகைவரை OT::Array32Of<WidthDeltaPair> WidthDeltaCluster;

கட்டமைப்பு JustificationCategory
{
  வகைவரை வெறுமை EntryData;

  எண்ணிக்கை Flags
  {
    SetMark		=0x8000,/* If set, make the current glyph the marked
				 * glyph. */
    DontAdvance		=0x4000,/* If set, don't advance to the next glyph before
				 * going to the new state. */
    MarkCategory	=0x3F80,/* The justification category for the marked
				 * glyph if nonzero. */
    CurrentCategory	=0x007F /* The justification category for the current
				 * glyph if nonzero. */
  };

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  morphHeader.sanitize (c) &&
			  stHeader.sanitize (c)));
  }

  பாதுகாக்கப்பட்ட:
  ChainSubtable<ObsoleteTypes>
		morphHeader;	/* Metamorphosis-style subtable header. */
  StateTable<ObsoleteTypes, EntryData>
		stHeader;	/* The justification insertion state table header */
  பொது:
  DEFINE_SIZE_STATIC (30);
};

கட்டமைப்பு JustificationHeader
{
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  justClassTable.sanitize (c, base, base) &&
			  wdcTable.sanitize (c, base) &&
			  pcTable.sanitize (c, base) &&
			  lookupTable.sanitize (c, base)));
  }

  பாதுகாக்கப்பட்ட:
  Offset16To<JustificationCategory>
		justClassTable;	/* Offset to the justification category state table. */
  Offset16To<WidthDeltaCluster>
		wdcTable;	/* Offset from start of justification table to start
				 * of the subtable containing the width delta factors
				 * for the glyphs in your font.
				 *
				 * The width delta clusters table. */
  Offset16To<PostcompensationActionChain>
		pcTable;	/* Offset from start of justification table to start
				 * of postcompensation subtable (set to zero if none).
				 *
				 * The postcompensation subtable, if present in the font. */
  Lookup<Offset16To<WidthDeltaCluster>>
		lookupTable;	/* Lookup table associating glyphs with width delta
				 * clusters. See the description of Width Delta Clusters
				 * table for details on how to interpret the lookup values. */

  பொது:
  DEFINE_SIZE_MIN (8);
};

கட்டமைப்பு just
{
  நிலையான constexpr hb_tag_t tableTag = HB_AAT_TAG_just;

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);

    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  version.major == 1 &&
			  horizData.sanitize (c, இது, இது) &&
			  vertData.sanitize (c, இது, இது)));
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;	/* Version of the justification table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the justification table (set to 0). */
  Offset16To<JustificationHeader>
		horizData;	/* Byte offset from the start of the justification table
				 * to the header for tables that contain justification
				 * information for horizontal text.
				 * If you are not including this information,
				 * store 0. */
  Offset16To<JustificationHeader>
		vertData;	/* ditto, vertical */

  பொது:
  DEFINE_SIZE_STATIC (10);
};

} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_JUST_TABLE_HH */
