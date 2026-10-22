/*
 * Copyright © 2010,2012  Google, Inc.
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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper.hh"


ਸਥਿਰ ਤਾਰਕਿਕ
compose_hebrew (ਅਚਲ hb_ot_shape_normalize_context_t *c,
		hb_codepoint_t  a,
		hb_codepoint_t  b,
		hb_codepoint_t *ab)
{
  /* Hebrew presentation-form shaping.
   * https://bugzilla.mozilla.org/show_bug.cgi?id=728866
   * Hebrew presentation forms with dagesh, for characters U+05D0..05EA;
   * Note that some letters do not have a dagesh presForm encoded.
   */
  ਸਥਿਰ ਅਚਲ hb_codepoint_t sDageshForms[0x05EAu - 0x05D0u + 1] = {
    0xFB30u, /* ALEF */
    0xFB31u, /* BET */
    0xFB32u, /* GIMEL */
    0xFB33u, /* DALET */
    0xFB34u, /* HE */
    0xFB35u, /* VAV */
    0xFB36u, /* ZAYIN */
    0x0000u, /* HET */
    0xFB38u, /* TET */
    0xFB39u, /* YOD */
    0xFB3Au, /* FINAL KAF */
    0xFB3Bu, /* KAF */
    0xFB3Cu, /* LAMED */
    0x0000u, /* FINAL MEM */
    0xFB3Eu, /* MEM */
    0x0000u, /* FINAL NUN */
    0xFB40u, /* NUN */
    0xFB41u, /* SAMEKH */
    0x0000u, /* AYIN */
    0xFB43u, /* FINAL PE */
    0xFB44u, /* PE */
    0x0000u, /* FINAL TSADI */
    0xFB46u, /* TSADI */
    0xFB47u, /* QOF */
    0xFB48u, /* RESH */
    0xFB49u, /* SHIN */
    0xFB4Au /* TAV */
  };

  ਤਾਰਕਿਕ ਮਿਲਿਆ = (ਤਾਰਕਿਕ) c->unicode->compose (a, b, ab);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPER_HEBREW_FALLBACK
  ਵਾਪਸ ਮਿਲਿਆ;
#ਜੇ_ਅੰਤ

  ਜੇ (!ਮਿਲਿਆ && (c->plan && !c->plan->has_gpos_mark))
  {
      /* Special-case Hebrew presentation forms that are excluded from
       * standard normalization, but wanted for old fonts. */
      ਚੋਣ (b) {
      ਹਾਲਤ 0x05B4u: /* HIRIQ */
	  ਜੇ (a == 0x05D9u) { /* YOD */
	      *ab = 0xFB1Du;
	      ਮਿਲਿਆ = ਸੱਚ;
	  }
	  ਰੋਕੋ;
      ਹਾਲਤ 0x05B7u: /* PATAH */
	  ਜੇ (a == 0x05F2u) { /* YIDDISH YOD YOD */
	      *ab = 0xFB1Fu;
	      ਮਿਲਿਆ = ਸੱਚ;
	  } ਨਹੀਂਤਾਂ ਜੇ (a == 0x05D0u) { /* ALEF */
	      *ab = 0xFB2Eu;
	      ਮਿਲਿਆ = ਸੱਚ;
	  }
	  ਰੋਕੋ;
      ਹਾਲਤ 0x05B8u: /* QAMATS */
	  ਜੇ (a == 0x05D0u) { /* ALEF */
	      *ab = 0xFB2Fu;
	      ਮਿਲਿਆ = ਸੱਚ;
	  }
	  ਰੋਕੋ;
      ਹਾਲਤ 0x05B9u: /* HOLAM */
	  ਜੇ (a == 0x05D5u) { /* VAV */
	      *ab = 0xFB4Bu;
	      ਮਿਲਿਆ = ਸੱਚ;
	  }
	  ਰੋਕੋ;
      ਹਾਲਤ 0x05BCu: /* DAGESH */
	  ਜੇ (a >= 0x05D0u && a <= 0x05EAu) {
	      *ab = sDageshForms[a - 0x05D0u];
	      ਮਿਲਿਆ = (*ab != 0);
	  } ਨਹੀਂਤਾਂ ਜੇ (a == 0xFB2Au) { /* SHIN WITH SHIN DOT */
	      *ab = 0xFB2Cu;
	      ਮਿਲਿਆ = ਸੱਚ;
	  } ਨਹੀਂਤਾਂ ਜੇ (a == 0xFB2Bu) { /* SHIN WITH SIN DOT */
	      *ab = 0xFB2Du;
	      ਮਿਲਿਆ = ਸੱਚ;
	  }
	  ਰੋਕੋ;
      ਹਾਲਤ 0x05BFu: /* RAFE */
	  ਚੋਣ (a) {
	  ਹਾਲਤ 0x05D1u: /* BET */
	      *ab = 0xFB4Cu;
	      ਮਿਲਿਆ = ਸੱਚ;
	      ਰੋਕੋ;
	  ਹਾਲਤ 0x05DBu: /* KAF */
	      *ab = 0xFB4Du;
	      ਮਿਲਿਆ = ਸੱਚ;
	      ਰੋਕੋ;
	  ਹਾਲਤ 0x05E4u: /* PE */
	      *ab = 0xFB4Eu;
	      ਮਿਲਿਆ = ਸੱਚ;
	      ਰੋਕੋ;
	  }
	  ਰੋਕੋ;
      ਹਾਲਤ 0x05C1u: /* SHIN DOT */
	  ਜੇ (a == 0x05E9u) { /* SHIN */
	      *ab = 0xFB2Au;
	      ਮਿਲਿਆ = ਸੱਚ;
	  } ਨਹੀਂਤਾਂ ਜੇ (a == 0xFB49u) { /* SHIN WITH DAGESH */
	      *ab = 0xFB2Cu;
	      ਮਿਲਿਆ = ਸੱਚ;
	  }
	  ਰੋਕੋ;
      ਹਾਲਤ 0x05C2u: /* SIN DOT */
	  ਜੇ (a == 0x05E9u) { /* SHIN */
	      *ab = 0xFB2Bu;
	      ਮਿਲਿਆ = ਸੱਚ;
	  } ਨਹੀਂਤਾਂ ਜੇ (a == 0xFB49u) { /* SHIN WITH DAGESH */
	      *ab = 0xFB2Du;
	      ਮਿਲਿਆ = ਸੱਚ;
	  }
	  ਰੋਕੋ;
      }
  }

  ਵਾਪਸ ਮਿਲਿਆ;
}

ਸਥਿਰ ਖਾਲੀ
reorder_marks_hebrew (ਅਚਲ hb_ot_shape_plan_t *plan HB_ਅਣਵਰਤਿਆ,
		      hb_ਬਫਰ_t              *ਬਫਰ,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ              ਸ਼ੁਰੂ,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ              ਸਿਰਾ)
{
  hb_glyph_info_t *info = ਬਫਰ->info;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਸ਼ੁਰੂ + 2; i < ਸਿਰਾ; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ c0 = info_cc (info[i - 2]);
    ਬਿਨਾਨਿਸ਼ਾਨੀ c1 = info_cc (info[i - 1]);
    ਬਿਨਾਨਿਸ਼ਾਨੀ c2 = info_cc (info[i - 0]);

    ਜੇ ((c0 == HB_MODIFIED_COMBINING_CLASS_CCC17 || c0 == HB_MODIFIED_COMBINING_CLASS_CCC18) /* patach or qamats */ &&
	(c1 == HB_MODIFIED_COMBINING_CLASS_CCC10 || c1 == HB_MODIFIED_COMBINING_CLASS_CCC14) /* sheva or hiriq */ &&
	(c2 == HB_MODIFIED_COMBINING_CLASS_CCC22 || c2 == HB_UNICODE_COMBINING_CLASS_ਹੇਠਾਂ) /* meteg or below */)
    {
      ਬਫਰ->ਮਿਲਾਓ_clusters (i - 1, i + 1);
      hb_ਅਦਲਾ_ਬਦਲੀ (info[i - 1], info[i]);
      ਰੋਕੋ;
    }
  }


}

ਅਚਲ hb_ot_shaper_t _hb_ot_shaper_hebrew =
{
  ਖਾਲੀ_ਸੂਚਕ, /* collect_features */
  ਖਾਲੀ_ਸੂਚਕ, /* override_features */
  ਖਾਲੀ_ਸੂਚਕ, /* data_create */
  ਖਾਲੀ_ਸੂਚਕ, /* data_destroy */
  ਖਾਲੀ_ਸੂਚਕ, /* preprocess_text */
  ਖਾਲੀ_ਸੂਚਕ, /* postprocess_glyphs */
  ਖਾਲੀ_ਸੂਚਕ, /* decompose */
  compose_hebrew,
  ਖਾਲੀ_ਸੂਚਕ, /* setup_masks */
  reorder_marks_hebrew,
  HB_ਟੈਗ ('h','e','b','r'), /* gpos_tag. https://github.com/harfbuzz/harfbuzz/issues/347#issuecomment-267838368 */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_DEFAULT,
  HB_OT_SHAPE_ZERO_ਚੌੜਾਈ_MARKS_BY_GDEF_LATE,
  ਸੱਚ, /* fallback_position */
};


#ਜੇ_ਅੰਤ
