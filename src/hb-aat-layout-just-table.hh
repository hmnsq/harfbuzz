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

#اگر_غیر_تعریف_شدہ HB_AAT_LAYOUT_JUST_جدول_HH
#تعریف_کرو HB_AAT_LAYOUT_JUST_جدول_HH

#شامل "hb-aat-layout-common.hh"
#شامل "hb-ot-layout.hh"
#شامل "hb-open-type.hh"

#شامل "hb-aat-layout-morx-table.hh"

/*
 * just -- Justification
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6just.html
 */
#تعریف_کرو HB_AAT_لیبل_just HB_لیبل('j','u','s','t')


نامخلا AAT {

استعمال نامخلا OT;


ساخت Action_Subrecord_سرخی
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  HBUINT16	actionClass;	/* The JustClass value associated with this
				 * ActionSubrecord. */
  HBUINT16	actionType;	/* The type of postcompensation action. */
  HBUINT16	action_لمبائی;	/* Length of this ActionSubrecord record, which
				 * must be a multiple of 4. */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت DecompositionAction
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  Action_Subrecord_سرخی
		سرخی;
  F16DOT16	زیریں_حد_بندی;	/* If the distance factor is less than this value,
				 * then the ligature is decomposed. */
  F16DOT16	بالائی_حد_بندی;	/* If the distance factor is greater than this value,
				 * then the ligature is decomposed. */
  HBUINT16	ترتیب;		/* Numerical order in which this ligature will
				 * be decomposed; you may want infrequent ligatures
				 * to decompose before more frequent ones. The ligatures
				 * on the line of text will decompose in increasing
				 * value of this field. */
  Array16_کا<HBUINT16>
		decomposedglyphs;
				/* Number of 16-bit glyph indexes that follow;
				 * the ligature will be decomposed into these glyphs.
				 *
				 * Array of decomposed glyphs. */
  عوامی:
  DEFINE_سائز_ARRAY (18, decomposedglyphs);
};

ساخت Unconditional_جوڑو_Glyph_Action
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  Action_Subrecord_سرخی
		سرخی;
  HBGlyphID16	جوڑو_Glyph;	/* Glyph that should be added if the distance factor
				 * is growing. */

  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت Conditional_جوڑو_Glyph_Action
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  Action_Subrecord_سرخی
		سرخی;
  F16DOT16	subst_حد; /* Distance growth factor (in ems) at which
				 * this glyph is replaced and the growth factor
				 * recalculated. */
  HBGlyphID16	جوڑو_Glyph;	/* Glyph to be added as kashida. If this value is
				 * 0xFFFF, no extra glyph will be added. Note that
				 * generally when a glyph is added, justification
				 * will need to be redone. */
  HBGlyphID16	substGlyph;	/* Glyph to be substituted for this glyph if the
				 * growth factor equals or exceeds the value of
				 * substThreshold. */
  عوامی:
  DEFINE_سائز_STATIC (14);
};

ساخت DuctileGlyphAction
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  Action_Subrecord_سرخی
		سرخی;
  HBUINT32	variationAxis;	/* The 4-byte tag identifying the ductile axis.
				 * This would normally be 0x64756374 ('duct'),
				 * but you may use any axis the font contains. */
  F16DOT16	minimum_حد_بندی;	/* The lowest value for the ductility axis that
				 * still yields an acceptable appearance. Normally
				 * this will be 1.0. */
  F16DOT16	no_Stretch_قدر; /* This is the default value that corresponds to
				 * no change in appearance. Normally, this will
				 * be 1.0. */
  F16DOT16	زیادہ_سے_زیادہ_حد_بندی;	/* The highest value for the ductility axis that
				 * still yields an acceptable appearance. */
  عوامی:
  DEFINE_سائز_STATIC (22);
};

ساخت Repeated_جوڑو_Glyph_Action
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  Action_Subrecord_سرخی
		سرخی;
  HBUINT16	flags;		/* Currently unused; set to 0. */
  HBGlyphID16	glyph;		/* Glyph that should be added if the distance factor
				 * is growing. */
  عوامی:
  DEFINE_سائز_STATIC (10);
};

ساخت ActionSubrecord
{
  غیرعلامتی صحیح get_لمبائی () ثابت { واپس u.سرخی.action_لمبائی; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ)))
      return_trace (جھوٹ);
    hb_barrier ();

    انتخاب (u.سرخی.actionType)
    {
    حالت 0: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    حالت 1: hb_barrier ();  return_trace (u.unconditional_جوڑو_Glyph_Action.sanitize (c));
    حالت 2: hb_barrier ();  return_trace (u.conditional_جوڑو_Glyph_Action.sanitize (c));
    // case 3: hb_barrier (); return_trace (u.stretchGlyphAction.sanitize (c));
    حالت 4: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    حالت 5: hb_barrier ();  return_trace (u.decompositionAction.sanitize (c));
    طےشدہ: return_trace (سچ);
    }
  }

  محفوظ:
  اتحاد	{
  Action_Subrecord_سرخی		سرخی;
  DecompositionAction		decompositionAction;
  Unconditional_جوڑو_Glyph_Action	unconditional_جوڑو_Glyph_Action;
  Conditional_جوڑو_Glyph_Action	conditional_جوڑو_Glyph_Action;
  /* StretchGlyphAction stretchGlyphAction; -- Not supported by CoreText */
  DuctileGlyphAction		ductileGlyphAction;
  Repeated_جوڑو_Glyph_Action	repeated_جوڑو_Glyph_Action;
  } u;				/* Data. The format of this data depends on
				 * the value of the actionType field. */
  عوامی:
  DEFINE_سائز_UNION (6, سرخی);
};

ساخت PostcompensationActionChain
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ)))
      return_trace (جھوٹ);
    hb_barrier ();

    غیرعلامتی صحیح انحراف = min_سائز;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      ثابت ActionSubrecord& subrecord = Struct_At_انحراف<ActionSubrecord> (یہ, انحراف);
      اگر (unlikely (!subrecord.sanitize (c))) return_trace (جھوٹ);
      انحراف += subrecord.get_لمبائی ();
    }

    return_trace (سچ);
  }

  محفوظ:
  HBUINT32	گنتی;

  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت Just_چوڑائی_Delta_اندراج
{
  شمار Flags
  {
    Reserved1		=0xE000,/* Reserved. You should set these bits to zero. */
    UnlimiteGap		=0x1000,/* The glyph can take unlimited gap. When this
				 * glyph participates in the justification process,
				 * it and any other glyphs on the line having this
				 * bit set absorb all the remaining gap. */
    Reserved2		=0x0FF0,/* Reserved. You should set these bits to zero. */
    ترجیح_2		=0x000F /* The justification priority of the glyph. */
  };

  شمار ترجیح_2
  {
    Kashida		= 0,	/* Kashida priority. This is the highest priority
				 * during justification. */
    Whitespace		= 1,	/* Whitespace priority. Any whitespace glyphs (as
				 * identified in the glyph properties table) will
				 * get this priority. */
    InterCharacter	= 2,	/* Inter-character priority. Give this to any
				 * remaining glyphs. */
    Null_ترجیح	= 3	/* Null priority. You should set this priority for
				 * glyphs that only participate in justification
				 * after the above priorities. Normally all glyphs
				 * have one of the previous three values. If you
				 * don't want a glyph to participate in justification,
				 * and you don't want to set its factors to zero,
				 * you may instead assign it to the null priority. */
  };

  محفوظ:
  F16DOT16	before_Grow_حد_بندی;/* The ratio by which the advance width of the
				 * glyph is permitted to grow on the left or top side. */
  F16DOT16	before_Shrink_حد_بندی;
				/* The ratio by which the advance width of the
				 * glyph is permitted to shrink on the left or top side. */
  F16DOT16	after_Grow_حد_بندی;	/* The ratio by which the advance width of the glyph
				 * is permitted to shrink on the left or top side. */
  F16DOT16	after_Shrink_حد_بندی;
				/* The ratio by which the advance width of the glyph
				 * is at most permitted to shrink on the right or
				 * bottom side. */
  HBUINT16	growFlags;	/* Flags controlling the grow case. */
  HBUINT16	shrinkFlags;	/* Flags controlling the shrink case. */

  عوامی:
  DEFINE_سائز_STATIC (20);
};

ساخت چوڑائی_Delta_Pair
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  HBUINT32	justClass;	/* The justification category associated
				 * with the wdRecord field. Only 7 bits of
				 * this field are used. (The other bits are
				 * used as padding to guarantee longword
				 * alignment of the following record). */
  Just_چوڑائی_Delta_اندراج
		wd_ریکارڈ;	/* The actual width delta record. */

  عوامی:
  DEFINE_سائز_STATIC (24);
};

قسمتعریف OT::Array32_کا<چوڑائی_Delta_Pair> چوڑائی_Delta_Cluster;

ساخت JustificationCategory
{
  قسمتعریف خالی اندراج_ڈیٹا;

  شمار Flags
  {
    سیٹ_Mark		=0x8000,/* If set, make the current glyph the marked
				 * glyph. */
    DontAdvance		=0x4000,/* If set, don't advance to the next glyph before
				 * going to the new state. */
    MarkCategory	=0x3F80,/* The justification category for the marked
				 * glyph if nonzero. */
    CurrentCategory	=0x007F /* The justification category for the current
				 * glyph if nonzero. */
  };

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  morph_سرخی.sanitize (c) &&
			  st_سرخی.sanitize (c)));
  }

  محفوظ:
  ChainSubtable<ObsoleteTypes>
		morph_سرخی;	/* Metamorphosis-style subtable header. */
  State_جدول<ObsoleteTypes, اندراج_ڈیٹا>
		st_سرخی;	/* The justification insertion state table header */
  عوامی:
  DEFINE_سائز_STATIC (30);
};

ساخت Justification_سرخی
{
  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  just_Class_جدول.sanitize (c, بنیاد, بنیاد) &&
			  wdc_جدول.sanitize (c, بنیاد) &&
			  pc_جدول.sanitize (c, بنیاد) &&
			  lookup_جدول.sanitize (c, بنیاد)));
  }

  محفوظ:
  Offset16_مقصد<JustificationCategory>
		just_Class_جدول;	/* Offset to the justification category state table. */
  Offset16_مقصد<چوڑائی_Delta_Cluster>
		wdc_جدول;	/* Offset from start of justification table to start
				 * of the subtable containing the width delta factors
				 * for the glyphs in your font.
				 *
				 * The width delta clusters table. */
  Offset16_مقصد<PostcompensationActionChain>
		pc_جدول;	/* Offset from start of justification table to start
				 * of postcompensation subtable (set to zero if none).
				 *
				 * The postcompensation subtable, if present in the font. */
  Lookup<Offset16_مقصد<چوڑائی_Delta_Cluster>>
		lookup_جدول;	/* Lookup table associating glyphs with width delta
				 * clusters. See the description of Width Delta Clusters
				 * table for details on how to interpret the lookup values. */

  عوامی:
  DEFINE_سائز_MIN (8);
};

ساخت just
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_AAT_لیبل_just;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);

    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  نسخہ.major == 1 &&
			  horiz_ڈیٹا.sanitize (c, یہ, یہ) &&
			  vert_ڈیٹا.sanitize (c, یہ, یہ)));
  }

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version of the justification table
				 * (0x00010000u for version 1.0). */
  HBUINT16	format;		/* Format of the justification table (set to 0). */
  Offset16_مقصد<Justification_سرخی>
		horiz_ڈیٹا;	/* Byte offset from the start of the justification table
				 * to the header for tables that contain justification
				 * information for horizontal text.
				 * If you are not including this information,
				 * store 0. */
  Offset16_مقصد<Justification_سرخی>
		vert_ڈیٹا;	/* ditto, vertical */

  عوامی:
  DEFINE_سائز_STATIC (10);
};

} /* namespace AAT */


#اگر_ختم /* HB_AAT_LAYOUT_JUST_TABLE_HH */
