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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_KERN_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_KERN_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-kerx-table.hh"


/*
 * kern -- Kerning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/kern
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kern.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_kern HB_ਟੈਗ('k','e','r','n')


ਨਾਮਾਕਾਸ਼ OT {


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>
ਬਣਤਰ Kern_Sub_ਸਾਰਣੀ_Format3
{
  ਪੂਰਨ get_kerning (hb_codepoint_t ਖੱਬੇ, hb_codepoint_t ਸੱਜੇ) ਅਚਲ
  {
    hb_array_t<ਅਚਲ FWORD> kern_ਮੁੱਲ = kern_ਮੁੱਲ_Z.as_array (kern_ਮੁੱਲ_ਗਿਣਤੀ);
    hb_array_t<ਅਚਲ HBUINT8> ਖੱਬਾ_Class = StructAfter<ਅਚਲ Unsized_Array_ਦਾ<HBUINT8>> (kern_ਮੁੱਲ).as_array (glyph_ਗਿਣਤੀ_2);
    hb_array_t<ਅਚਲ HBUINT8> ਸੱਜਾ_Class = StructAfter<ਅਚਲ Unsized_Array_ਦਾ<HBUINT8>> (ਖੱਬਾ_Class).as_array (glyph_ਗਿਣਤੀ_2);
    hb_array_t<ਅਚਲ HBUINT8> kern_ਸੂਚਕਾਂਕ = StructAfter<ਅਚਲ Unsized_Array_ਦਾ<HBUINT8>> (ਸੱਜਾ_Class).as_array (ਖੱਬਾ_Class_ਗਿਣਤੀ * ਸੱਜਾ_Class_ਗਿਣਤੀ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੱਬਾ_C = ਖੱਬਾ_Class[ਖੱਬੇ];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੱਜਾ_C = ਸੱਜਾ_Class[ਸੱਜੇ];
    ਜੇ (unlikely (ਖੱਬਾ_C >= ਖੱਬਾ_Class_ਗਿਣਤੀ || ਸੱਜਾ_C >= ਸੱਜਾ_Class_ਗਿਣਤੀ))
      ਵਾਪਸ 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਖੱਬਾ_C * ਸੱਜਾ_Class_ਗਿਣਤੀ + ਸੱਜਾ_C;
    ਵਾਪਸ kern_ਮੁੱਲ[kern_ਸੂਚਕਾਂਕ[i]];
  }

  ਤਾਰਕਿਕ apply (AAT::hb_aat_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    ਜੇ (!c->plan->requested_kerning)
      ਵਾਪਸ ਝੂਠ;

    ਜੇ (ਸਿਰਲੇਖ.coverage & ਸਿਰਲੇਖ.Backwards)
      ਵਾਪਸ ਝੂਠ;

    hb_kern_machine_t<Kern_Sub_ਸਾਰਣੀ_Format3> machine (*ਇਹ, ਸਿਰਲੇਖ.coverage & ਸਿਰਲੇਖ.Cross_ਧਾਰਾ);
    machine.kern (c->font, c->ਬਫਰ, c->plan->kern_ਨਕਾਬ);

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  c->check_range (kern_ਮੁੱਲ_Z,
				  kern_ਮੁੱਲ_ਗਿਣਤੀ * ਆਕਾਰ (FWORD) +
				  glyph_ਗਿਣਤੀ_2 * 2 +
				  ਖੱਬਾ_Class_ਗਿਣਤੀ * ਸੱਜਾ_Class_ਗਿਣਤੀ));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &ਖੱਬਾ_ਸੈੱਟ, ਸੈੱਟ_t &ਸੱਜਾ_ਸੈੱਟ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਜੇ (likely (glyph_ਗਿਣਤੀ_2))
    {
      ਖੱਬਾ_ਸੈੱਟ.ਜੋੜੋ_range (0, ਅੰਕ_glyphs - 1);
      ਸੱਜਾ_ਸੈੱਟ.ਜੋੜੋ_range (0, ਅੰਕ_glyphs - 1);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ
		ਸਿਰਲੇਖ;
  HBUINT16	glyph_ਗਿਣਤੀ_2;	/* The number of glyphs in this font. */
  HBUINT8	kern_ਮੁੱਲ_ਗਿਣਤੀ;	/* The number of kerning values. */
  HBUINT8	ਖੱਬਾ_Class_ਗਿਣਤੀ;	/* The number of left-hand classes. */
  HBUINT8	ਸੱਜਾ_Class_ਗਿਣਤੀ;/* The number of right-hand classes. */
  HBUINT8	flags;		/* Set to zero (reserved for future use). */
  Unsized_Array_ਦਾ<FWORD>
		kern_ਮੁੱਲ_Z;	/* The kerning values.
				 * Length kernValueCount. */
#ਜੇ 0
  Unsized_Array_ਦਾ<HBUINT8>
		ਖੱਬਾ_Class;	/* The left-hand classes.
				 * Length glyphCount. */
  Unsized_Array_ਦਾ<HBUINT8>
		ਸੱਜਾ_Class;	/* The right-hand classes.
				 * Length glyphCount. */
  Unsized_Array_ਦਾ<HBUINT8>kern_ਸੂਚਕਾਂਕ;
				/* The indices into the kernValue array.
				 * Length leftClassCount * rightClassCount */
#ਜੇ_ਅੰਤ
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::static_ਮਾਪ + 6, kern_ਮੁੱਲ_Z);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>
ਬਣਤਰ Kern_Sub_ਸਾਰਣੀ
{
  size_t get_ਮਾਪ () ਅਚਲ { ਵਾਪਸ u.ਸਿਰਲੇਖ.ਲੰਮਾਈ; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_type () ਅਚਲ { ਵਾਪਸ u.ਸਿਰਲੇਖ.format; }

  ਪੂਰਨ get_kerning (hb_codepoint_t ਖੱਬੇ, hb_codepoint_t ਸੱਜੇ) ਅਚਲ
  {
    ਚੋਣ (get_type ()) {
    /* This method hooks up to hb_font_t's get_h_kerning.  Only support Format0. */
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_kerning (ਖੱਬੇ, ਸੱਜੇ);
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_type = get_type ();
    TRACE_DISPATCH (ਇਹ, subtable_type);
    ਚੋਣ (subtable_type) {
    ਹਾਲਤ 0:	hb_barrier (); return_trace (c->dispatch (u.format0));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
    ਹਾਲਤ 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ਜੇ_ਅੰਤ
    ਹਾਲਤ 2:	hb_barrier (); return_trace (c->dispatch (u.format2));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
    ਹਾਲਤ 3:	hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ਜੇ_ਅੰਤ
    ਮੂਲ:	return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &ਖੱਬਾ_ਸੈੱਟ, ਸੈੱਟ_t &ਸੱਜਾ_ਸੈੱਟ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_type = get_type ();
    ਚੋਣ (subtable_type) {
    ਹਾਲਤ 0:	hb_barrier (); u.format0.collect_glyphs (ਖੱਬਾ_ਸੈੱਟ, ਸੱਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs); ਵਾਪਸ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
    ਹਾਲਤ 1:	hb_barrier (); u.format1.collect_glyphs (ਖੱਬਾ_ਸੈੱਟ, ਸੱਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs); ਵਾਪਸ;
#ਜੇ_ਅੰਤ
    ਹਾਲਤ 2:	hb_barrier (); u.format2.collect_glyphs (ਖੱਬਾ_ਸੈੱਟ, ਸੱਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs); ਵਾਪਸ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
    ਹਾਲਤ 3:	hb_barrier (); u.format3.collect_glyphs (ਖੱਬਾ_ਸੈੱਟ, ਸੱਜਾ_ਸੈੱਟ, ਅੰਕ_glyphs); ਵਾਪਸ;
#ਜੇ_ਅੰਤ
    ਮੂਲ:	ਵਾਪਸ;
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!(u.ਸਿਰਲੇਖ.sanitize (c) &&
		    hb_barrier () &&
		    u.ਸਿਰਲੇਖ.ਲੰਮਾਈ >= u.ਸਿਰਲੇਖ.min_ਮਾਪ &&
		    c->check_range (ਇਹ, u.ਸਿਰਲੇਖ.ਲੰਮਾਈ)))) return_trace (ਝੂਠ);

    return_trace (dispatch (c));
  }

  ਜਨਤਕ:
  ਸੰਘ {
  Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ				ਸਿਰਲੇਖ;
  AAT::Kerx_Sub_ਸਾਰਣੀ_Format0<Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>	format0;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  AAT::Kerx_Sub_ਸਾਰਣੀ_Format1<Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>	format1;
#ਜੇ_ਅੰਤ
  AAT::Kerx_Sub_ਸਾਰਣੀ_Format2<Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>	format2;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  Kern_Sub_ਸਾਰਣੀ_Format3<Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ>	format3;
#ਜੇ_ਅੰਤ
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (Kern_Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::static_ਮਾਪ);
};


ਬਣਤਰ Kern_OTSub_ਸਾਰਣੀ_ਸਿਰਲੇਖ
{
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ apple = ਝੂਠ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ AAT::ObsoleteTypes Types;

  ਬਿਨਾਨਿਸ਼ਾਨੀ   tuple_ਗਿਣਤੀ_2 () ਅਚਲ { ਵਾਪਸ 0; }
  ਤਾਰਕਿਕ     is_horizontal () ਅਚਲ { ਵਾਪਸ (coverage & Horizontal); }

  ਗਿਣਤੀ Coverage
  {
    Horizontal	= 0x01u,
    Minimum	= 0x02u,
    Cross_ਧਾਰਾ	= 0x04u,
    Override	= 0x08u,

    /* Not supported: */
    Backwards	= 0x00u,
    Variation	= 0x00u,
  };

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBUINT16	ਸੰਸਕਰਣ_Z;	/* Unused. */
  HBUINT16	ਲੰਮਾਈ;		/* Length of the subtable (including this header). */
  HBUINT8	format;		/* Subtable format. */
  HBUINT8	coverage;	/* Coverage bits. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ KernOT : AAT::Kerx_ਸਾਰਣੀ<KernOT>
{
  ਮਿੱਤਰ ਬਣਤਰ AAT::Kerx_ਸਾਰਣੀ<KernOT>;

  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_kern;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਸੰਸਕਰਣ = 0u;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Kern_OTSub_ਸਾਰਣੀ_ਸਿਰਲੇਖ Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::Types Types;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Kern_Sub_ਸਾਰਣੀ<Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ> Sub_ਸਾਰਣੀ;

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸੰਸਕਰਣ;	/* Version--0x0000u */
  HBUINT16	ਸਾਰਣੀ_ਗਿਣਤੀ;	/* Number of subtables in the kerning table. */
  Sub_ਸਾਰਣੀ	ਪਹਿਲਾ_Sub_ਸਾਰਣੀ;	/* Subtables. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (4);
};


ਬਣਤਰ Kern_AATSub_ਸਾਰਣੀ_ਸਿਰਲੇਖ
{
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ apple = ਸੱਚ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ AAT::ObsoleteTypes Types;

  ਬਿਨਾਨਿਸ਼ਾਨੀ   tuple_ਗਿਣਤੀ_2 () ਅਚਲ { ਵਾਪਸ 0; }
  ਤਾਰਕਿਕ     is_horizontal () ਅਚਲ { ਵਾਪਸ !(coverage & Vertical); }

  ਗਿਣਤੀ Coverage
  {
    Vertical	= 0x80u,
    Cross_ਧਾਰਾ	= 0x40u,
    Variation	= 0x20u,

    /* Not supported: */
    Backwards	= 0x00u,
  };

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBUINT32	ਲੰਮਾਈ;		/* Length of the subtable (including this header). */
  HBUINT8	coverage;	/* Coverage bits. */
  HBUINT8	format;		/* Subtable format. */
  HBUINT16	tuple_ਸੂਚਕਾਂਕ;	/* The tuple index (used for variations fonts).
				 * This value specifies which tuple this subtable covers.
				 * Note: We don't implement. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ KernAAT : AAT::Kerx_ਸਾਰਣੀ<KernAAT>
{
  ਮਿੱਤਰ ਬਣਤਰ AAT::Kerx_ਸਾਰਣੀ<KernAAT>;

  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_kern;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ min_ਸੰਸਕਰਣ = 0x00010000u;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Kern_AATSub_ਸਾਰਣੀ_ਸਿਰਲੇਖ Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ::Types Types;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Kern_Sub_ਸਾਰਣੀ<Sub_ਸਾਰਣੀ_ਸਿਰਲੇਖ> Sub_ਸਾਰਣੀ;

  ਸੁਰੱਖਿਅਤ:
  HBUINT32	ਸੰਸਕਰਣ;	/* Version--0x00010000u */
  HBUINT32	ਸਾਰਣੀ_ਗਿਣਤੀ;	/* Number of subtables in the kerning table. */
  Sub_ਸਾਰਣੀ	ਪਹਿਲਾ_Sub_ਸਾਰਣੀ;	/* Subtables. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8);
};

ਬਣਤਰ kern
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_kern;

  ਤਾਰਕਿਕ     has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ u.version32.v; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_type () ਅਚਲ { ਵਾਪਸ u.major.v; }

  ਤਾਰਕਿਕ has_state_machine () ਅਚਲ
  {
    ਚੋਣ (get_type ()) {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.ot.has_state_machine ();
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.aat.has_state_machine ();
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }

  ਤਾਰਕਿਕ has_cross_ਧਾਰਾ () ਅਚਲ
  {
    ਚੋਣ (get_type ()) {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.ot.has_cross_ਧਾਰਾ ();
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.aat.has_cross_ਧਾਰਾ ();
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ ਝੂਠ;
    }
  }

  ਪੂਰਨ get_h_kerning (hb_codepoint_t ਖੱਬੇ, hb_codepoint_t ਸੱਜੇ) ਅਚਲ
  {
    ਚੋਣ (get_type ()) {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.ot.get_h_kerning (ਖੱਬੇ, ਸੱਜੇ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.aat.get_h_kerning (ਖੱਬੇ, ਸੱਜੇ);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  ਤਾਰਕਿਕ apply (AAT::hb_aat_apply_context_t *c,
	      ਅਚਲ AAT::kern_accelerator_ਅੰਕੜੇ_t &accel_ਅੰਕੜੇ) ਅਚਲ
  { ਵਾਪਸ dispatch (c, accel_ਅੰਕੜੇ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subtable_type = get_type ();
    TRACE_DISPATCH (ਇਹ, subtable_type);
    ਚੋਣ (subtable_type) {
    ਹਾਲਤ 0:	hb_barrier (); return_trace (c->dispatch (u.ot, std::forward<Ts> (ds)...));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
    ਹਾਲਤ 1:	hb_barrier (); return_trace (c->dispatch (u.aat, std::forward<Ts> (ds)...));
#ਜੇ_ਅੰਤ
    ਮੂਲ:	return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.version32.v.sanitize (c)) return_trace (ਝੂਠ);
    hb_barrier ();
    return_trace (dispatch (c));
  }

  AAT::kern_accelerator_ਅੰਕੜੇ_t create_accelerator_ਅੰਕੜੇ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਚੋਣ (get_type ()) {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.ot.create_accelerator_ਅੰਕੜੇ (ਅੰਕ_glyphs);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.aat.create_accelerator_ਅੰਕੜੇ (ਅੰਕ_glyphs);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ AAT::kern_accelerator_ਅੰਕੜੇ_t ();
    }
  }

  ਬਣਤਰ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ਇਹ->ਸਾਰਣੀ = sc.reference_ਸਾਰਣੀ<kern> (face);
      ਇਹ->accel_ਅੰਕੜੇ = ਇਹ->ਸਾਰਣੀ->create_accelerator_ਅੰਕੜੇ (face->get_ਅੰਕ_glyphs ());
    }
    ~accelerator_t ()
    {
      ਇਹ->ਸਾਰਣੀ.destroy ();
    }

    hb_blob_t *get_blob () ਅਚਲ { ਵਾਪਸ ਸਾਰਣੀ.get_blob (); }

    ਤਾਰਕਿਕ apply (AAT::hb_aat_apply_context_t *c) ਅਚਲ
    {
      ਵਾਪਸ ਸਾਰਣੀ->apply (c, accel_ਅੰਕੜੇ);
    }

    hb_blob_ptr_t<kern> ਸਾਰਣੀ;
    AAT::kern_accelerator_ਅੰਕੜੇ_t accel_ਅੰਕੜੇ;
    AAT::hb_aat_scratch_t scratch;
  };

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT32 v; }	version32;
  ਬਣਤਰ { HBUINT16 v; }	major;
  KernOT		ot;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE
  KernAAT		aat;
#ਜੇ_ਅੰਤ
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (4, version32.v);
};

ਬਣਤਰ kern_accelerator_t : kern::accelerator_t {
  kern_accelerator_t (hb_face_t *face) : kern::accelerator_t (face) {}
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_KERN_TABLE_HH */
