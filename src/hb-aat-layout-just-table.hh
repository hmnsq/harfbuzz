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

#جيڪڏهن_وصف_نه_ٿيل HB_AAT_LAYOUT_JUST_جدول_HH
#وصف_ڪريو HB_AAT_LAYOUT_JUST_جدول_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-open-type.hh"

#ضمن "hb-aat-layout-morx-table.hh"

/*
 * just -- Justification
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6just.html
 */
#وصف_ڪريو HB_AAT_ٽيگ_just HB_ٽيگ('j','u','s','t')


نطاق AAT {

استعمال نطاق OT;


بنية Action_Subrecord_سرو
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	actionClass;	/* The JustClass value associated with this
				 * ActionSubrecord. */
  HBUINT16	actionType;	/* The type of postcompensation action. */
  HBUINT16	action_ڊيگهائي;	/* Length of this ActionSubrecord record, which
				 * must be a multiple of 4. */
  عام:
  DEFINE_سائيز_STATIC (6);
};

بنية DecompositionAction
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  Action_Subrecord_سرو
		سرو;
  F16DOT16	هيٺاهون_حد_بندي;	/* If the distance factor is less than this value,
				 * then the ligature is decomposed. */
  F16DOT16	مٿانهون_حد_بندي;	/* If the distance factor is greater than this value,
				 * then the ligature is decomposed. */
  HBUINT16	ترتيب;		/* Numerical order in which this ligature will
				 * be decomposed; you may want infrequent ligatures
				 * to decompose before more frequent ones. The ligatures
				 * on the line of text will decompose in increasing
				 * value of this field. */
  Array16_جو<HBUINT16>
		decomposedglyphs;
				/* Number of 16-bit glyph indexes that follow;
				 * the ligature will be decomposed into these glyphs.
				 *
				 * Array of decomposed glyphs. */
  عام:
  DEFINE_سائيز_ARRAY (18, decomposedglyphs);
};

بنية Unconditional_جوڙيو_Glyph_Action
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  Action_Subrecord_سرو
		سرو;
  HBGlyphID16	جوڙيو_Glyph;	/* Glyph that should be added if the distance factor
				 * is growing. */

  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية Conditional_جوڙيو_Glyph_Action
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  Action_Subrecord_سرو
		سرو;
  F16DOT16	subst_حد; /* Distance growth factor (in ems) at which
				 * this glyph is replaced and the growth factor
				 * recalculated. */
  HBGlyphID16	جوڙيو_Glyph;	/* Glyph to be added as kashida. If this value is
				 * 0xFFFF, no extra glyph will be added. Note that
				 * generally when a glyph is added, justification
				 * will need to be redone. */
  HBGlyphID16	substGlyph;	/* Glyph to be substituted for this glyph if the
				 * growth factor equals or exceeds the value of
				 * substThreshold. */
  عام:
  DEFINE_سائيز_STATIC (14);
};

بنية DuctileGlyphAction
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  Action_Subrecord_سرو
		سرو;
  HBUINT32	variationAxis;	/* The 4-byte tag identifying the ductile axis.
				 * This would normally be 0x64756374 ('duct'),
				 * but you may use any axis the font contains. */
  F16DOT16	minimum_حد_بندي;	/* The lowest value for the ductility axis that
				 * still yields an acceptable appearance. Normally
				 * this will be 1.0. */
  F16DOT16	no_Stretch_قدر; /* This is the default value that corresponds to
				 * no change in appearance. Normally, this will
				 * be 1.0. */
  F16DOT16	وڌ_کان_وڌ_حد_بندي;	/* The highest value for the ductility axis that
				 * still yields an acceptable appearance. */
  عام:
  DEFINE_سائيز_STATIC (22);
};

بنية Repeated_جوڙيو_Glyph_Action
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  Action_Subrecord_سرو
		سرو;
  HBUINT16	flags;		/* Currently unused; set to 0. */
  HBGlyphID16	glyph;		/* Glyph that should be added if the distance factor
				 * is growing. */
  عام:
  DEFINE_سائيز_STATIC (10);
};

بنية ActionSubrecord
{
  بنانشان صحيح get_ڊيگهائي () ثابت { أرجع u.سرو.action_ڊيگهائي; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (u.سرو.actionType)
    {
    حالة 0: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    حالة 1: hb_barrier ();  return_trace (u.unconditional_جوڙيو_Glyph_Action.sanitize (c));
    حالة 2: hb_barrier ();  return_trace (u.conditional_جوڙيو_Glyph_Action.sanitize (c));
    // case 3: hb_barrier (); return_trace (u.stretchGlyphAction.sanitize (c));
    حالة 4: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    حالة 5: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    افتراضي: return_trace (سچ);
    }
  }

  محمي:
  اتحاد	{
  Action_Subrecord_سرو		سرو;
  DecompositionAction		decompositionAction;
  Unconditional_جوڙيو_Glyph_Action	unconditional_جوڙيو_Glyph_Action;
  Conditional_جوڙيو_Glyph_Action	conditional_جوڙيو_Glyph_Action;
  /* StretchGlyphAction stretchGlyphAction; -- Not supported by CoreText */
  DuctileGlyphAction		ductileGlyphAction;
  Repeated_جوڙيو_Glyph_Action	repeated_جوڙيو_Glyph_Action;
  } u;				/* Data. The format of this data depends on
				 * the value of the actionType field. */
  عام:
  DEFINE_سائيز_UNION (6, سرو);
};

بنية PostcompensationActionChain
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بنانشان صحيح وٿي = min_سائيز;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      ثابت ActionSubrecord& subrecord = Struct_At_وٿي<ActionSubrecord> (هذا, وٿي);
      إذا (unlikely (!subrecord.sanitize (c))) return_trace (خطأ);
      وٿي += subrecord.get_ڊيگهائي ();
    }

    return_trace (سچ);
  }

  محمي:
  HBUINT32	ڳڻپ;

  عام:
  DEFINE_سائيز_STATIC (4);
};

بنية Just_ويڪر_Delta_داخلا
{
  تعداد Flags
  {
    Reserved1		=0xE000,/* Reserved. You should set these bits to zero. */
    UnlimiteGap		=0x1000,/* The glyph can take unlimited gap. When this
				 * glyph participates in the justification process,
				 * it and any other glyphs on the line having this
				 * bit set absorb all the remaining gap. */
    Reserved2		=0x0FF0,/* Reserved. You should set these bits to zero. */
    ترجيح_2		=0x000F /* The justification priority of the glyph. */
  };

  تعداد ترجيح_2
  {
    Kashida		= 0,	/* Kashida priority. This is the highest priority
				 * during justification. */
    Whitespace		= 1,	/* Whitespace priority. Any whitespace glyphs (as
				 * identified in the glyph properties table) will
				 * get this priority. */
    InterCharacter	= 2,	/* Inter-character priority. Give this to any
				 * remaining glyphs. */
    Null_ترجيح	= 3	/* Null priority. You should set this priority for
				 * glyphs that only participate in justification
				 * after the above priorities. Normally all glyphs
				 * have one of the previous three values. If you
				 * don't want a glyph to participate in justification,
				 * and you don't want to set its factors to zero,
				 * you may instead assign it to the null priority. */
  };

  محمي:
  F16DOT16	before_Grow_حد_بندي;/* The ratio by which the advance width of the
				 * glyph is permitted to grow on the left or top side. */
  F16DOT16	before_Shrink_حد_بندي;
				/* The ratio by which the advance width of the
				 * glyph is permitted to shrink on the left or top side. */
  F16DOT16	after_Grow_حد_بندي;	/* The ratio by which the advance width of the glyph
				 * is permitted to shrink on the left or top side. */
  F16DOT16	after_Shrink_حد_بندي;
				/* The ratio by which the advance width of the glyph
				 * is at most permitted to shrink on the right or
				 * bottom side. */
  HBUINT16	growFlags;	/* Flags controlling the grow case. */
  HBUINT16	shrinkFlags;	/* Flags controlling the shrink case. */

  عام:
  DEFINE_سائيز_STATIC (20);
};

بنية ويڪر_Delta_Pair
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT32	justClass;	/* The justification category associated
				 * with the wdRecord field. Only 7 bits of
				 * this field are used. (The other bits are
				 * used as padding to guarantee longword
				 * alignment of the following record). */
  Just_ويڪر_Delta_داخلا
		wd_رڪارڊ;	/* The actual width delta record. */

  عام:
  DEFINE_سائيز_STATIC (24);
};

تعريفنوع OT::Array32_جو<ويڪر_Delta_Pair> ويڪر_Delta_Cluster;

بنية JustificationCategory
{
  تعريفنوع فراغ داخلا_معلومات;

  تعداد Flags
  {
    سيٽ_Mark		=0x8000,/* If set, make the current glyph the marked
				 * glyph. */
    DontAdvance		=0x4000,/* If set, don't advance to the next glyph before
				 * going to the new state. */
    MarkCategory	=0x3F80,/* The justification category for the marked
				 * glyph if nonzero. */
    CurrentCategory	=0x007F /* The justification category for the current
				 * glyph if nonzero. */
  };

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  morph_سرو.sanitize (c) &&
			  st_سرو.sanitize (c)));
  }

  محمي:
  ChainSubtable<ObsoleteTypes>
		morph_سرو;	/* Metamorphosis-style subtable header. */
  State_جدول<ObsoleteTypes, داخلا_معلومات>
		st_سرو;	/* The justification insertion state table header */
  عام:
  DEFINE_سائيز_STATIC (30);
};

بنية Justification_سرو
{
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  just_Class_جدول.sanitize (c, بنياد, بنياد) &&
			  wdc_جدول.sanitize (c, بنياد) &&
			  pc_جدول.sanitize (c, بنياد) &&
			  lookup_جدول.sanitize (c, بنياد)));
  }

  محمي:
  Offset16_منزل<JustificationCategory>
		just_Class_جدول;	/* Offset to the justification category state table. */
  Offset16_منزل<ويڪر_Delta_Cluster>
		wdc_جدول;	/* Offset from start of justification table to start
				 * of the subtable containing the width delta factors
				 * for the glyphs in your font.
				 *
				 * The width delta clusters table. */
  Offset16_منزل<PostcompensationActionChain>
		pc_جدول;	/* Offset from start of justification table to start
				 * of postcompensation subtable (set to zero if none).
				 *
				 * The postcompensation subtable, if present in the font. */
  Lookup<Offset16_منزل<ويڪر_Delta_Cluster>>
		lookup_جدول;	/* Lookup table associating glyphs with width delta
				 * clusters. See the description of Width Delta Clusters
				 * table for details on how to interpret the lookup values. */

  عام:
  DEFINE_سائيز_MIN (8);
};

بنية just
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_AAT_ٽيگ_just;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخو.major == 1 &&
			  horiz_معلومات.sanitize (c, هذا, هذا) &&
			  vert_معلومات.sanitize (c, هذا, هذا)));
  }

  محمي:
  Fixed_نسخو<>نسخو;	/* Version of the justification table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the justification table (set to 0). */
  Offset16_منزل<Justification_سرو>
		horiz_معلومات;	/* Byte offset from the start of the justification table
				 * to the header for tables that contain justification
				 * information for horizontal text.
				 * If you are not including this information,
				 * store 0. */
  Offset16_منزل<Justification_سرو>
		vert_معلومات;	/* ditto, vertical */

  عام:
  DEFINE_سائيز_STATIC (10);
};

} /* namespace AAT */


#جيڪڏهن_پورو /* HB_AAT_LAYOUT_JUST_TABLE_HH */
