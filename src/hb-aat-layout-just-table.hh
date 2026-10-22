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

#که_نه_تعریف_شوی HB_AAT_LAYOUT_JUST_جدول_HH
#تعریف_کړه HB_AAT_LAYOUT_JUST_جدول_HH

#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-open-type.hh"

#ضمن "hb-aat-layout-morx-table.hh"

/*
 * just -- Justification
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6just.html
 */
#تعریف_کړه HB_AAT_نښان_just HB_نښان('j','u','s','t')


نطاق AAT {

کارول نطاق OT;


بنية Action_Subrecord_سرلیک
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	actionClass;	/* The JustClass value associated with this
				 * ActionSubrecord. */
  HBUINT16	actionType;	/* The type of postcompensation action. */
  HBUINT16	action_اوږدتیا;	/* Length of this ActionSubrecord record, which
				 * must be a multiple of 4. */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية DecompositionAction
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  Action_Subrecord_سرلیک
		سرلیک;
  F16DOT16	ښکتنی_حد;	/* If the distance factor is less than this value,
				 * then the ligature is decomposed. */
  F16DOT16	پاسنی_حد;	/* If the distance factor is greater than this value,
				 * then the ligature is decomposed. */
  HBUINT16	ترتیب;		/* Numerical order in which this ligature will
				 * be decomposed; you may want infrequent ligatures
				 * to decompose before more frequent ones. The ligatures
				 * on the line of text will decompose in increasing
				 * value of this field. */
  Array16_د<HBUINT16>
		decomposedglyphs;
				/* Number of 16-bit glyph indexes that follow;
				 * the ligature will be decomposed into these glyphs.
				 *
				 * Array of decomposed glyphs. */
  عام:
  DEFINE_اندازه_ARRAY (18, decomposedglyphs);
};

بنية Unconditional_جمع_کړه_Glyph_Action
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  Action_Subrecord_سرلیک
		سرلیک;
  HBGlyphID16	جمع_کړه_Glyph;	/* Glyph that should be added if the distance factor
				 * is growing. */

  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Conditional_جمع_کړه_Glyph_Action
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  Action_Subrecord_سرلیک
		سرلیک;
  F16DOT16	subst_پوله; /* Distance growth factor (in ems) at which
				 * this glyph is replaced and the growth factor
				 * recalculated. */
  HBGlyphID16	جمع_کړه_Glyph;	/* Glyph to be added as kashida. If this value is
				 * 0xFFFF, no extra glyph will be added. Note that
				 * generally when a glyph is added, justification
				 * will need to be redone. */
  HBGlyphID16	substGlyph;	/* Glyph to be substituted for this glyph if the
				 * growth factor equals or exceeds the value of
				 * substThreshold. */
  عام:
  DEFINE_اندازه_STATIC (14);
};

بنية DuctileGlyphAction
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  Action_Subrecord_سرلیک
		سرلیک;
  HBUINT32	variationAxis;	/* The 4-byte tag identifying the ductile axis.
				 * This would normally be 0x64756374 ('duct'),
				 * but you may use any axis the font contains. */
  F16DOT16	minimum_حد;	/* The lowest value for the ductility axis that
				 * still yields an acceptable appearance. Normally
				 * this will be 1.0. */
  F16DOT16	no_Stretch_ارزښت; /* This is the default value that corresponds to
				 * no change in appearance. Normally, this will
				 * be 1.0. */
  F16DOT16	اعظمي_حد;	/* The highest value for the ductility axis that
				 * still yields an acceptable appearance. */
  عام:
  DEFINE_اندازه_STATIC (22);
};

بنية Repeated_جمع_کړه_Glyph_Action
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  Action_Subrecord_سرلیک
		سرلیک;
  HBUINT16	flags;		/* Currently unused; set to 0. */
  HBGlyphID16	glyph;		/* Glyph that should be added if the distance factor
				 * is growing. */
  عام:
  DEFINE_اندازه_STATIC (10);
};

بنية ActionSubrecord
{
  بینښه صحيح get_اوږدتیا () ثابت { أرجع u.سرلیک.action_اوږدتیا; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (u.سرلیک.actionType)
    {
    حالة 0: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    حالة 1: hb_barrier ();  return_trace (u.unconditional_جمع_کړه_Glyph_Action.sanitize (c));
    حالة 2: hb_barrier ();  return_trace (u.conditional_جمع_کړه_Glyph_Action.sanitize (c));
    // case 3: hb_barrier (); return_trace (u.stretchGlyphAction.sanitize (c));
    حالة 4: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    حالة 5: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    افتراضي: return_trace (ریښتیا);
    }
  }

  محمي:
  اتحاد	{
  Action_Subrecord_سرلیک		سرلیک;
  DecompositionAction		decompositionAction;
  Unconditional_جمع_کړه_Glyph_Action	unconditional_جمع_کړه_Glyph_Action;
  Conditional_جمع_کړه_Glyph_Action	conditional_جمع_کړه_Glyph_Action;
  /* StretchGlyphAction stretchGlyphAction; -- Not supported by CoreText */
  DuctileGlyphAction		ductileGlyphAction;
  Repeated_جمع_کړه_Glyph_Action	repeated_جمع_کړه_Glyph_Action;
  } u;				/* Data. The format of this data depends on
				 * the value of the actionType field. */
  عام:
  DEFINE_اندازه_UNION (6, سرلیک);
};

بنية PostcompensationActionChain
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بینښه صحيح دننه_واټن = min_اندازه;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      ثابت ActionSubrecord& subrecord = Struct_At_دننه_واټن<ActionSubrecord> (هذا, دننه_واټن);
      إذا (unlikely (!subrecord.sanitize (c))) return_trace (خطأ);
      دننه_واټن += subrecord.get_اوږدتیا ();
    }

    return_trace (ریښتیا);
  }

  محمي:
  HBUINT32	شمېر;

  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية Just_پلنوالی_Delta_ثبت
{
  تعداد Flags
  {
    Reserved1		=0xE000,/* Reserved. You should set these bits to zero. */
    UnlimiteGap		=0x1000,/* The glyph can take unlimited gap. When this
				 * glyph participates in the justification process,
				 * it and any other glyphs on the line having this
				 * bit set absorb all the remaining gap. */
    Reserved2		=0x0FF0,/* Reserved. You should set these bits to zero. */
    لومړیتوب_2		=0x000F /* The justification priority of the glyph. */
  };

  تعداد لومړیتوب_2
  {
    Kashida		= 0,	/* Kashida priority. This is the highest priority
				 * during justification. */
    Whitespace		= 1,	/* Whitespace priority. Any whitespace glyphs (as
				 * identified in the glyph properties table) will
				 * get this priority. */
    InterCharacter	= 2,	/* Inter-character priority. Give this to any
				 * remaining glyphs. */
    Null_لومړیتوب	= 3	/* Null priority. You should set this priority for
				 * glyphs that only participate in justification
				 * after the above priorities. Normally all glyphs
				 * have one of the previous three values. If you
				 * don't want a glyph to participate in justification,
				 * and you don't want to set its factors to zero,
				 * you may instead assign it to the null priority. */
  };

  محمي:
  F16DOT16	before_Grow_حد;/* The ratio by which the advance width of the
				 * glyph is permitted to grow on the left or top side. */
  F16DOT16	before_Shrink_حد;
				/* The ratio by which the advance width of the
				 * glyph is permitted to shrink on the left or top side. */
  F16DOT16	after_Grow_حد;	/* The ratio by which the advance width of the glyph
				 * is permitted to shrink on the left or top side. */
  F16DOT16	after_Shrink_حد;
				/* The ratio by which the advance width of the glyph
				 * is at most permitted to shrink on the right or
				 * bottom side. */
  HBUINT16	growFlags;	/* Flags controlling the grow case. */
  HBUINT16	shrinkFlags;	/* Flags controlling the shrink case. */

  عام:
  DEFINE_اندازه_STATIC (20);
};

بنية پلنوالی_Delta_Pair
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
  Just_پلنوالی_Delta_ثبت
		wd_ریکارډ;	/* The actual width delta record. */

  عام:
  DEFINE_اندازه_STATIC (24);
};

تعريفنوع OT::Array32_د<پلنوالی_Delta_Pair> پلنوالی_Delta_Cluster;

بنية JustificationCategory
{
  تعريفنوع فراغ ثبت_معلومات;

  تعداد Flags
  {
    سېټ_Mark		=0x8000,/* If set, make the current glyph the marked
				 * glyph. */
    DontAdvance		=0x4000,/* If set, don't advance to the next glyph before
				 * going to the new state. */
    MarkCategory	=0x3F80,/* The justification category for the marked
				 * glyph if nonzero. */
    CurrentCategory	=0x007F /* The justification category for the current
				 * glyph if nonzero. */
  };

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  morph_سرلیک.sanitize (c) &&
			  st_سرلیک.sanitize (c)));
  }

  محمي:
  ChainSubtable<ObsoleteTypes>
		morph_سرلیک;	/* Metamorphosis-style subtable header. */
  State_جدول<ObsoleteTypes, ثبت_معلومات>
		st_سرلیک;	/* The justification insertion state table header */
  عام:
  DEFINE_اندازه_STATIC (30);
};

بنية Justification_سرلیک
{
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  just_Class_جدول.sanitize (c, بنسټ, بنسټ) &&
			  wdc_جدول.sanitize (c, بنسټ) &&
			  pc_جدول.sanitize (c, بنسټ) &&
			  lookup_جدول.sanitize (c, بنسټ)));
  }

  محمي:
  Offset16_منزل<JustificationCategory>
		just_Class_جدول;	/* Offset to the justification category state table. */
  Offset16_منزل<پلنوالی_Delta_Cluster>
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
  Lookup<Offset16_منزل<پلنوالی_Delta_Cluster>>
		lookup_جدول;	/* Lookup table associating glyphs with width delta
				 * clusters. See the description of Width Delta Clusters
				 * table for details on how to interpret the lookup values. */

  عام:
  DEFINE_اندازه_MIN (8);
};

بنية just
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_AAT_نښان_just;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخه.major == 1 &&
			  horiz_معلومات.sanitize (c, هذا, هذا) &&
			  vert_معلومات.sanitize (c, هذا, هذا)));
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the justification table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the justification table (set to 0). */
  Offset16_منزل<Justification_سرلیک>
		horiz_معلومات;	/* Byte offset from the start of the justification table
				 * to the header for tables that contain justification
				 * information for horizontal text.
				 * If you are not including this information,
				 * store 0. */
  Offset16_منزل<Justification_سرلیک>
		vert_معلومات;	/* ditto, vertical */

  عام:
  DEFINE_اندازه_STATIC (10);
};

} /* namespace AAT */


#د_که_پای /* HB_AAT_LAYOUT_JUST_TABLE_HH */
