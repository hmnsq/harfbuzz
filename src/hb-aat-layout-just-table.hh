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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_LAYOUT_JUST_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_LAYOUT_JUST_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-morx-table.hh"

/*
 * just -- Justification
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6just.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_ਟੈਗ_just HB_ਟੈਗ('j','u','s','t')


ਨਾਮਾਕਾਸ਼ AAT {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;


ਬਣਤਰ Action_Subrecord_ਸਿਰਲੇਖ
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  HBUINT16	actionClass;	/* The JustClass value associated with this
				 * ActionSubrecord. */
  HBUINT16	actionType;	/* The type of postcompensation action. */
  HBUINT16	action_ਲੰਮਾਈ;	/* Length of this ActionSubrecord record, which
				 * must be a multiple of 4. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ DecompositionAction
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  Action_Subrecord_ਸਿਰਲੇਖ
		ਸਿਰਲੇਖ;
  F16DOT16	ਹੇਠਲਾ_ਹੱਦਬੰਦੀ;	/* If the distance factor is less than this value,
				 * then the ligature is decomposed. */
  F16DOT16	ਉੱਤਲਾ_ਹੱਦਬੰਦੀ;	/* If the distance factor is greater than this value,
				 * then the ligature is decomposed. */
  HBUINT16	ਕ੍ਰਮ;		/* Numerical order in which this ligature will
				 * be decomposed; you may want infrequent ligatures
				 * to decompose before more frequent ones. The ligatures
				 * on the line of text will decompose in increasing
				 * value of this field. */
  Array16_ਦਾ<HBUINT16>
		decomposedglyphs;
				/* Number of 16-bit glyph indexes that follow;
				 * the ligature will be decomposed into these glyphs.
				 *
				 * Array of decomposed glyphs. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (18, decomposedglyphs);
};

ਬਣਤਰ Unconditional_ਜੋੜੋ_Glyph_Action
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Action_Subrecord_ਸਿਰਲੇਖ
		ਸਿਰਲੇਖ;
  HBGlyphID16	ਜੋੜੋ_Glyph;	/* Glyph that should be added if the distance factor
				 * is growing. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ Conditional_ਜੋੜੋ_Glyph_Action
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Action_Subrecord_ਸਿਰਲੇਖ
		ਸਿਰਲੇਖ;
  F16DOT16	subst_ਹੱਦ; /* Distance growth factor (in ems) at which
				 * this glyph is replaced and the growth factor
				 * recalculated. */
  HBGlyphID16	ਜੋੜੋ_Glyph;	/* Glyph to be added as kashida. If this value is
				 * 0xFFFF, no extra glyph will be added. Note that
				 * generally when a glyph is added, justification
				 * will need to be redone. */
  HBGlyphID16	substGlyph;	/* Glyph to be substituted for this glyph if the
				 * growth factor equals or exceeds the value of
				 * substThreshold. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (14);
};

ਬਣਤਰ DuctileGlyphAction
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Action_Subrecord_ਸਿਰਲੇਖ
		ਸਿਰਲੇਖ;
  HBUINT32	variationAxis;	/* The 4-byte tag identifying the ductile axis.
				 * This would normally be 0x64756374 ('duct'),
				 * but you may use any axis the font contains. */
  F16DOT16	minimum_ਹੱਦਬੰਦੀ;	/* The lowest value for the ductility axis that
				 * still yields an acceptable appearance. Normally
				 * this will be 1.0. */
  F16DOT16	no_Stretch_ਮੁੱਲ; /* This is the default value that corresponds to
				 * no change in appearance. Normally, this will
				 * be 1.0. */
  F16DOT16	ਵੱਧ_ਤੋਂ_ਵੱਧ_ਹੱਦਬੰਦੀ;	/* The highest value for the ductility axis that
				 * still yields an acceptable appearance. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (22);
};

ਬਣਤਰ Repeated_ਜੋੜੋ_Glyph_Action
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Action_Subrecord_ਸਿਰਲੇਖ
		ਸਿਰਲੇਖ;
  HBUINT16	flags;		/* Currently unused; set to 0. */
  HBGlyphID16	glyph;		/* Glyph that should be added if the distance factor
				 * is growing. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (10);
};

ਬਣਤਰ ActionSubrecord
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਲੰਮਾਈ () ਅਚਲ { ਵਾਪਸ u.ਸਿਰਲੇਖ.action_ਲੰਮਾਈ; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ)))
      return_trace (ਝੂਠ);
    hb_barrier ();

    ਚੋਣ (u.ਸਿਰਲੇਖ.actionType)
    {
    ਹਾਲਤ 0: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    ਹਾਲਤ 1: hb_barrier ();  return_trace (u.unconditional_ਜੋੜੋ_Glyph_Action.sanitize (c));
    ਹਾਲਤ 2: hb_barrier ();  return_trace (u.conditional_ਜੋੜੋ_Glyph_Action.sanitize (c));
    // case 3: hb_barrier (); return_trace (u.stretchGlyphAction.sanitize (c));
    ਹਾਲਤ 4: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    ਹਾਲਤ 5: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    ਮੂਲ: return_trace (ਸੱਚ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ	{
  Action_Subrecord_ਸਿਰਲੇਖ		ਸਿਰਲੇਖ;
  DecompositionAction		decompositionAction;
  Unconditional_ਜੋੜੋ_Glyph_Action	unconditional_ਜੋੜੋ_Glyph_Action;
  Conditional_ਜੋੜੋ_Glyph_Action	conditional_ਜੋੜੋ_Glyph_Action;
  /* StretchGlyphAction stretchGlyphAction; -- Not supported by CoreText */
  DuctileGlyphAction		ductileGlyphAction;
  Repeated_ਜੋੜੋ_Glyph_Action	repeated_ਜੋੜੋ_Glyph_Action;
  } u;				/* Data. The format of this data depends on
				 * the value of the actionType field. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (6, ਸਿਰਲੇਖ);
};

ਬਣਤਰ PostcompensationActionChain
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ)))
      return_trace (ਝੂਠ);
    hb_barrier ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = min_ਮਾਪ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਅਚਲ ActionSubrecord& subrecord = Struct_At_ਵਿੱਥ<ActionSubrecord> (ਇਹ, ਵਿੱਥ);
      ਜੇ (unlikely (!subrecord.sanitize (c))) return_trace (ਝੂਠ);
      ਵਿੱਥ += subrecord.get_ਲੰਮਾਈ ();
    }

    return_trace (ਸੱਚ);
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT32	ਗਿਣਤੀ_2;

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ Just_ਚੌੜਾਈ_Delta_ਇੰਦਰਾਜ
{
  ਗਿਣਤੀ Flags
  {
    Reserved1		=0xE000,/* Reserved. You should set these bits to zero. */
    UnlimiteGap		=0x1000,/* The glyph can take unlimited gap. When this
				 * glyph participates in the justification process,
				 * it and any other glyphs on the line having this
				 * bit set absorb all the remaining gap. */
    Reserved2		=0x0FF0,/* Reserved. You should set these bits to zero. */
    ਤਰਜੀਹ_2		=0x000F /* The justification priority of the glyph. */
  };

  ਗਿਣਤੀ ਤਰਜੀਹ_2
  {
    Kashida		= 0,	/* Kashida priority. This is the highest priority
				 * during justification. */
    Whitespace		= 1,	/* Whitespace priority. Any whitespace glyphs (as
				 * identified in the glyph properties table) will
				 * get this priority. */
    InterCharacter	= 2,	/* Inter-character priority. Give this to any
				 * remaining glyphs. */
    Null_ਤਰਜੀਹ	= 3	/* Null priority. You should set this priority for
				 * glyphs that only participate in justification
				 * after the above priorities. Normally all glyphs
				 * have one of the previous three values. If you
				 * don't want a glyph to participate in justification,
				 * and you don't want to set its factors to zero,
				 * you may instead assign it to the null priority. */
  };

  ਸੁਰੱਖਿਅਤ:
  F16DOT16	before_Grow_ਹੱਦਬੰਦੀ;/* The ratio by which the advance width of the
				 * glyph is permitted to grow on the left or top side. */
  F16DOT16	before_Shrink_ਹੱਦਬੰਦੀ;
				/* The ratio by which the advance width of the
				 * glyph is permitted to shrink on the left or top side. */
  F16DOT16	after_Grow_ਹੱਦਬੰਦੀ;	/* The ratio by which the advance width of the glyph
				 * is permitted to shrink on the left or top side. */
  F16DOT16	after_Shrink_ਹੱਦਬੰਦੀ;
				/* The ratio by which the advance width of the glyph
				 * is at most permitted to shrink on the right or
				 * bottom side. */
  HBUINT16	growFlags;	/* Flags controlling the grow case. */
  HBUINT16	shrinkFlags;	/* Flags controlling the shrink case. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (20);
};

ਬਣਤਰ ਚੌੜਾਈ_Delta_Pair
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT32	justClass;	/* The justification category associated
				 * with the wdRecord field. Only 7 bits of
				 * this field are used. (The other bits are
				 * used as padding to guarantee longword
				 * alignment of the following record). */
  Just_ਚੌੜਾਈ_Delta_ਇੰਦਰਾਜ
		wd_ਰਿਕਾਰਡ;	/* The actual width delta record. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (24);
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ OT::Array32_ਦਾ<ਚੌੜਾਈ_Delta_Pair> ਚੌੜਾਈ_Delta_Cluster;

ਬਣਤਰ JustificationCategory
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ ਇੰਦਰਾਜ_ਅੰਕੜੇ;

  ਗਿਣਤੀ Flags
  {
    ਸੈੱਟ_Mark		=0x8000,/* If set, make the current glyph the marked
				 * glyph. */
    DontAdvance		=0x4000,/* If set, don't advance to the next glyph before
				 * going to the new state. */
    MarkCategory	=0x3F80,/* The justification category for the marked
				 * glyph if nonzero. */
    CurrentCategory	=0x007F /* The justification category for the current
				 * glyph if nonzero. */
  };

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  morph_ਸਿਰਲੇਖ.sanitize (c) &&
			  st_ਸਿਰਲੇਖ.sanitize (c)));
  }

  ਸੁਰੱਖਿਅਤ:
  ChainSubtable<ObsoleteTypes>
		morph_ਸਿਰਲੇਖ;	/* Metamorphosis-style subtable header. */
  State_ਸਾਰਣੀ<ObsoleteTypes, ਇੰਦਰਾਜ_ਅੰਕੜੇ>
		st_ਸਿਰਲੇਖ;	/* The justification insertion state table header */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (30);
};

ਬਣਤਰ Justification_ਸਿਰਲੇਖ
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  just_Class_ਸਾਰਣੀ.sanitize (c, ਅਧਾਰ, ਅਧਾਰ) &&
			  wdc_ਸਾਰਣੀ.sanitize (c, ਅਧਾਰ) &&
			  pc_ਸਾਰਣੀ.sanitize (c, ਅਧਾਰ) &&
			  lookup_ਸਾਰਣੀ.sanitize (c, ਅਧਾਰ)));
  }

  ਸੁਰੱਖਿਅਤ:
  Offset16_ਮੰਜ਼ਿਲ<JustificationCategory>
		just_Class_ਸਾਰਣੀ;	/* Offset to the justification category state table. */
  Offset16_ਮੰਜ਼ਿਲ<ਚੌੜਾਈ_Delta_Cluster>
		wdc_ਸਾਰਣੀ;	/* Offset from start of justification table to start
				 * of the subtable containing the width delta factors
				 * for the glyphs in your font.
				 *
				 * The width delta clusters table. */
  Offset16_ਮੰਜ਼ਿਲ<PostcompensationActionChain>
		pc_ਸਾਰਣੀ;	/* Offset from start of justification table to start
				 * of postcompensation subtable (set to zero if none).
				 *
				 * The postcompensation subtable, if present in the font. */
  Lookup<Offset16_ਮੰਜ਼ਿਲ<ਚੌੜਾਈ_Delta_Cluster>>
		lookup_ਸਾਰਣੀ;	/* Lookup table associating glyphs with width delta
				 * clusters. See the description of Width Delta Clusters
				 * table for details on how to interpret the lookup values. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8);
};

ਬਣਤਰ just
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_AAT_ਟੈਗ_just;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);

    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  ਸੰਸਕਰਣ.major == 1 &&
			  horiz_ਅੰਕੜੇ.sanitize (c, ਇਹ, ਇਹ) &&
			  vert_ਅੰਕੜੇ.sanitize (c, ਇਹ, ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the justification table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the justification table (set to 0). */
  Offset16_ਮੰਜ਼ਿਲ<Justification_ਸਿਰਲੇਖ>
		horiz_ਅੰਕੜੇ;	/* Byte offset from the start of the justification table
				 * to the header for tables that contain justification
				 * information for horizontal text.
				 * If you are not including this information,
				 * store 0. */
  Offset16_ਮੰਜ਼ਿਲ<Justification_ਸਿਰਲੇਖ>
		vert_ਅੰਕੜੇ;	/* ditto, vertical */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (10);
};

} /* namespace AAT */


#ਜੇ_ਅੰਤ /* HB_AAT_LAYOUT_JUST_TABLE_HH */
