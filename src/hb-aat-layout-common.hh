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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_LAYOUT_COMMON_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_LAYOUT_COMMON_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gdef-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cache.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-bit-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-bit-page.hh"


ਨਾਮਾਕਾਸ਼ OT {
ਬਣਤਰ GDEF;
};

ਨਾਮਾਕਾਸ਼ AAT {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;

ਬਣਤਰ ankr;

ਵਰਤੋਂ hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_TRANSITION_INLINE
#ਜੇ_ਅੰਤ

ਬਣਤਰ hb_aat_scratch_t
{
  hb_aat_scratch_t () = ਮੂਲ;
  hb_aat_scratch_t (ਅਚਲ hb_aat_scratch_t &) = ਮਿਟਾਓ;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    ਬਫਰ_glyph_ਸੈੱਟ.ਸੈੱਟ_relaxed (o.ਬਫਰ_glyph_ਸੈੱਟ.get_relaxed ());
    o.ਬਫਰ_glyph_ਸੈੱਟ.ਸੈੱਟ_relaxed (ਖਾਲੀ_ਸੂਚਕ);
  }
  hb_aat_scratch_t & ਸੰਚਾਲਕ = (hb_aat_scratch_t &&o)
  {
    ਬਫਰ_glyph_ਸੈੱਟ.ਸੈੱਟ_relaxed (o.ਬਫਰ_glyph_ਸੈੱਟ.get_relaxed ());
    o.ਬਫਰ_glyph_ਸੈੱਟ.ਸੈੱਟ_relaxed (ਖਾਲੀ_ਸੂਚਕ);
    ਵਾਪਸ *ਇਹ;
  }
  ~hb_aat_scratch_t ()
  {
    ਸਵੈਚਾਲਿਤ *s = ਬਫਰ_glyph_ਸੈੱਟ.get_relaxed ();
    ਜੇ (unlikely (!s))
      ਵਾਪਸ;
    s->fini ();
    hb_free (s);
  }

  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t *create_ਬਫਰ_glyph_ਸੈੱਟ () ਅਚਲ
  {
    hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t *s = ਬਫਰ_glyph_ਸੈੱਟ.get_acquire ();
    ਜੇ (s && ਬਫਰ_glyph_ਸੈੱਟ.cmpexch (s, ਖਾਲੀ_ਸੂਚਕ))
    {
      s->clear ();
      ਵਾਪਸ s;
    }

    s = (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t *) hb_calloc (1, ਆਕਾਰ (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t));
    ਜੇ (unlikely (!s))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    s->init ();

    ਵਾਪਸ s;
  }
  ਖਾਲੀ destroy_ਬਫਰ_glyph_ਸੈੱਟ (hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t *s) ਅਚਲ
  {
    ਜੇ (unlikely (!s))
      ਵਾਪਸ;
    ਜੇ (ਬਫਰ_glyph_ਸੈੱਟ.cmpexch (ਖਾਲੀ_ਸੂਚਕ, s))
      ਵਾਪਸ;
    s->fini ();
    hb_free (s);
  }

  ਬਦਲਣਯੋਗ hb_atomic_t<hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t *> ਬਫਰ_glyph_ਸੈੱਟ;
};

ਗਿਣਤੀ { DELETED_GLYPH = 0xFFFF };

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_SCRATCH_FLAG_AAT_HAS_DELETED HB_ਬਫਰ_SCRATCH_FLAG_SHAPER0

ਬਣਤਰ hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, ਤਾਰਕਿਕ, HB_DEBUG_APPLY>
{
  ਅਚਲ ਅੱਖਰ *get_name () { ਵਾਪਸ "APPLY"; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts>
  return_t dispatch (ਅਚਲ T &obj, Ts&&... ds)
  { ਵਾਪਸ obj.apply (ਇਹ, std::forward<Ts> (ds)...); }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ ਝੂਠ; }
  ਤਾਰਕਿਕ stop_sublookup_iteration (return_t r) ਅਚਲ { ਵਾਪਸ r; }

  ਅਚਲ hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_ਬਫਰ_t *ਬਫਰ;
  hb_sanitize_context_t sanitizer;
  ਅਚਲ ankr *ankr_ਸਾਰਣੀ;
  ਅਚਲ OT::GDEF &gdef;
  ਤਾਰਕਿਕ has_glyph_classes;
  ਅਚਲ hb_sorted_ਸਦਿਸ਼_t<hb_aat_map_t::range_flags_t> *range_flags = ਖਾਲੀ_ਸੂਚਕ;
  hb_ਨਕਾਬ_t subtable_flags = 0;
  ਤਾਰਕਿਕ ਬਫਰ_is_reversed = ਝੂਠ;
  // Caches
  ਤਾਰਕਿਕ using_ਬਫਰ_glyph_ਸੈੱਟ = ਝੂਠ;
  hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t *ਬਫਰ_glyph_ਸੈੱਟ = ਖਾਲੀ_ਸੂਚਕ;
  ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t *ਪਹਿਲਾ_ਸੈੱਟ = ਖਾਲੀ_ਸੂਚਕ;
  ਅਚਲ hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_t *ਦੂਜਾ_ਸੈੱਟ = ਖਾਲੀ_ਸੂਚਕ;
  hb_aat_class_cache_t *machine_class_cache = ਖਾਲੀ_ਸੂਚਕ;

  /* Unused. For debug tracing only. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ;

  HB_ਅੰਦਰੂਨੀ hb_aat_apply_context_t (ਅਚਲ hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_ਬਫਰ_t *ਬਫਰ_2,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_ਅੰਦਰੂਨੀ ~hb_aat_apply_context_t ();

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਸੈੱਟ_ankr_ਸਾਰਣੀ (ਅਚਲ AAT::ankr *ankr_ਸਾਰਣੀ_2);

  ਖਾਲੀ ਸੈੱਟ_lookup_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) { lookup_ਸੂਚਕਾਂਕ = i; }

  ਖਾਲੀ ਉਲਟ_ਬਫਰ ()
  {
    ਬਫਰ->reverse ();
    ਬਫਰ_is_reversed = !ਬਫਰ_is_reversed;
  }

  ਖਾਲੀ ਤਿਆਰੀ_ਬਫਰ_glyph_ਸੈੱਟ ()
  {
    using_ਬਫਰ_glyph_ਸੈੱਟ = ਬਫਰ->len >= 4 && ਬਫਰ_glyph_ਸੈੱਟ;

    ਜੇ (likely (using_ਬਫਰ_glyph_ਸੈੱਟ))
      ਬਫਰ->collect_codepoints (*ਬਫਰ_glyph_ਸੈੱਟ);
  }
  ਤਾਰਕਿਕ ਬਫਰ_intersects_machine () ਅਚਲ
  {
    ਜੇ (likely (using_ਬਫਰ_glyph_ਸੈੱਟ))
      ਵਾਪਸ ਬਫਰ_glyph_ਸੈੱਟ->intersects (*ਪਹਿਲਾ_ਸੈੱਟ);

    // Faster for shorter buffers.
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਬਫਰ->len; i++)
      ਜੇ (ਪਹਿਲਾ_ਸੈੱਟ->has (ਬਫਰ->info[i].codepoint))
	ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  HB_NODISCARD ਤਾਰਕਿਕ output_glyphs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				   ਅਚਲ T *glyphs)
  {
    ਜੇ (likely (using_ਬਫਰ_glyph_ਸੈੱਟ))
      ਬਫਰ_glyph_ਸੈੱਟ->ਜੋੜੋ_array (glyphs, ਗਿਣਤੀ_2);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਤਾਰਕਿਕ at_ਸਿਰਾ_ਦਾ_ਪਾਠ = ਬਫਰ->idx == ਬਫਰ->len;
      ਜੇ (at_ਸਿਰਾ_ਦਾ_ਪਾਠ)
	ਜੇ (unlikely (!ਬਫਰ->output_glyph (glyphs[i]))) ਵਾਪਸ ਝੂਠ;

      hb_glyph_info_t &info = at_ਸਿਰਾ_ਦਾ_ਪਾਠ ? ਬਫਰ->prev () : ਬਫਰ->cur ();
      ਜੇ (glyphs[i] == DELETED_GLYPH)
      {
        ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_AAT_HAS_DELETED;
	hb_glyph_info_ਸੈੱਟ_aat_deleted (&info);
      }
      ਨਹੀਂਤਾਂ
      {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT
	ਜੇ (has_glyph_classes)
	  hb_glyph_info_ਸੈੱਟ_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#ਜੇ_ਅੰਤ
      }
      ਜੇ (!at_ਸਿਰਾ_ਦਾ_ਪਾਠ)
	ਜੇ (unlikely (!ਬਫਰ->output_glyph (glyphs[i]))) ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  HB_NODISCARD ਤਾਰਕਿਕ replace_glyph (hb_codepoint_t glyph)
  {
    ਜੇ (glyph == DELETED_GLYPH)
    {
      ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_ਸੈੱਟ_aat_deleted (&ਬਫਰ->cur());
    }

    ਜੇ (likely (using_ਬਫਰ_glyph_ਸੈੱਟ))
      ਬਫਰ_glyph_ਸੈੱਟ->ਜੋੜੋ (glyph);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT
    ਜੇ (has_glyph_classes)
      hb_glyph_info_ਸੈੱਟ_glyph_props (&ਬਫਰ->cur(),
				      gdef.get_glyph_props (glyph));
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਬਫਰ->replace_glyph (glyph);
  }

  HB_NODISCARD ਤਾਰਕਿਕ delete_glyph ()
  {
    ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_AAT_HAS_DELETED;
    hb_glyph_info_ਸੈੱਟ_aat_deleted (&ਬਫਰ->cur());
    ਜੇ (likely (using_ਬਫਰ_glyph_ਸੈੱਟ))
      ਬਫਰ_glyph_ਸੈੱਟ->ਜੋੜੋ (DELETED_GLYPH);
    ਵਾਪਸ ਬਫਰ->replace_glyph (DELETED_GLYPH);
  }

  ਖਾਲੀ replace_glyph_inplace (ਬਿਨਾਨਿਸ਼ਾਨੀ i, hb_codepoint_t glyph)
  {
    ਬਫਰ->info[i].codepoint = glyph;
    ਜੇ (glyph == DELETED_GLYPH)
    {
      ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_ਸੈੱਟ_aat_deleted (&ਬਫਰ->info[i]);
    }
    ਜੇ (likely (using_ਬਫਰ_glyph_ਸੈੱਟ))
      ਬਫਰ_glyph_ਸੈੱਟ->ਜੋੜੋ (glyph);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT
    ਜੇ (has_glyph_classes)
      hb_glyph_info_ਸੈੱਟ_glyph_props (&ਬਫਰ->info[i],
				      gdef.get_glyph_props (glyph));
#ਜੇ_ਅੰਤ
  }
};


/*
 * Lookup Table
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਬਣਤਰ Lookup;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ LookupFormat0
{
  ਮਿੱਤਰ ਬਣਤਰ Lookup<T>;

  ਨਿਜੀ:
  ਅਚਲ T* get_ਮੁੱਲ (hb_codepoint_t glyph_id, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਜੇ (unlikely (glyph_id >= ਅੰਕ_glyphs)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ &arrayZ[glyph_id];
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    glyphs.ਜੋੜੋ_range (0, ਅੰਕ_glyphs - 1);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ filter_t>
  ਖਾਲੀ collect_glyphs_filtered (ਸੈੱਟ_t &glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs, ਅਚਲ filter_t &filter) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_glyphs; i++)
      ਜੇ (filter (arrayZ[i]))
	glyphs.ਜੋੜੋ (i);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (arrayZ.sanitize (c, c->get_ਅੰਕ_glyphs ()));
  }
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (arrayZ.sanitize (c, c->get_ਅੰਕ_glyphs (), ਅਧਾਰ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 0 */
  Unsized_Array_ਦਾ<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNBOUNDED (2);
};


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ LookupSegmentSingle
{
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ Termination_ਸ਼ਬਦ_ਗਿਣਤੀ = 2u;

  ਪੂਰਨ cmp (hb_codepoint_t g) ਅਚਲ
  { ਵਾਪਸ g < ਪਹਿਲਾ ? -1 : g <= ਆਖਰੀ ? 0 : +1 ; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &glyphs) ਅਚਲ
  {
    ਜੇ (ਪਹਿਲਾ == DELETED_GLYPH) ਵਾਪਸ;
    glyphs.ਜੋੜੋ_range (ਪਹਿਲਾ, ਆਖਰੀ);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ filter_t>
  ਖਾਲੀ collect_glyphs_filtered (ਸੈੱਟ_t &glyphs, ਅਚਲ filter_t &filter) ਅਚਲ
  {
    ਜੇ (ਪਹਿਲਾ == DELETED_GLYPH) ਵਾਪਸ;
    ਜੇ (!filter (value)) ਵਾਪਸ;
    glyphs.ਜੋੜੋ_range (ਪਹਿਲਾ, ਆਖਰੀ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && value.sanitize (c));
  }
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && value.sanitize (c, ਅਧਾਰ));
  }

  HBGlyphID16	ਆਖਰੀ;		/* Last GlyphID in this segment */
  HBGlyphID16	ਪਹਿਲਾ;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + T::static_ਮਾਪ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ LookupFormat2
{
  ਮਿੱਤਰ ਬਣਤਰ Lookup<T>;

  ਨਿਜੀ:
  ਅਚਲ T* get_ਮੁੱਲ (hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਅਚਲ LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    ਵਾਪਸ v ? &v->value : ਖਾਲੀ_ਸੂਚਕ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &glyphs) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = segments.get_ਲੰਮਾਈ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      segments[i].collect_glyphs (glyphs);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ filter_t>
  ਖਾਲੀ collect_glyphs_filtered (ਸੈੱਟ_t &glyphs, ਅਚਲ filter_t &filter) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = segments.get_ਲੰਮਾਈ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (segments.sanitize (c));
  }
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (segments.sanitize (c, ਅਧਾਰ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_ਖੋਜ_Array_ਦਾ<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (8, segments);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ LookupSegmentArray
{
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ Termination_ਸ਼ਬਦ_ਗਿਣਤੀ = 2u;

  ਅਚਲ T* get_ਮੁੱਲ (hb_codepoint_t glyph_id, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਵਾਪਸ ਪਹਿਲਾ <= glyph_id && glyph_id <= ਆਖਰੀ ? &(ਅਧਾਰ+ਮੁੱਲਾਂ_Z)[glyph_id - ਪਹਿਲਾ] : ਖਾਲੀ_ਸੂਚਕ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &glyphs) ਅਚਲ
  {
    ਜੇ (ਪਹਿਲਾ == DELETED_GLYPH) ਵਾਪਸ;
    glyphs.ਜੋੜੋ_range (ਪਹਿਲਾ, ਆਖਰੀ);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ filter_t>
  ਖਾਲੀ collect_glyphs_filtered (ਸੈੱਟ_t &glyphs, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, ਅਚਲ filter_t &filter) ਅਚਲ
  {
    ਜੇ (ਪਹਿਲਾ == DELETED_GLYPH) ਵਾਪਸ;
    ਅਚਲ ਸਵੈਚਾਲਿਤ &values = ਅਧਾਰ+ਮੁੱਲਾਂ_Z;
    ਲਈ (hb_codepoint_t i = ਪਹਿਲਾ; i <= ਆਖਰੀ; i++)
      ਜੇ (filter (values[i - ਪਹਿਲਾ]))
	glyphs.ਜੋੜੋ (i);
  }

  ਪੂਰਨ cmp (hb_codepoint_t g) ਅਚਲ
  { ਵਾਪਸ g < ਪਹਿਲਾ ? -1 : g <= ਆਖਰੀ ? 0 : +1; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  ਪਹਿਲਾ <= ਆਖਰੀ &&
		  ਮੁੱਲਾਂ_Z.sanitize (c, ਅਧਾਰ, ਆਖਰੀ - ਪਹਿਲਾ + 1));
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ...Ts>
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ, Ts&&... ds) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  ਪਹਿਲਾ <= ਆਖਰੀ &&
		  ਮੁੱਲਾਂ_Z.sanitize (c, ਅਧਾਰ, ਆਖਰੀ - ਪਹਿਲਾ + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	ਆਖਰੀ;		/* Last GlyphID in this segment */
  HBGlyphID16	ਪਹਿਲਾ;		/* First GlyphID in this segment */
  NNOffset16_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<T>>
		ਮੁੱਲਾਂ_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ LookupFormat4
{
  ਮਿੱਤਰ ਬਣਤਰ Lookup<T>;

  ਨਿਜੀ:
  ਅਚਲ T* get_ਮੁੱਲ (hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਅਚਲ LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    ਵਾਪਸ v ? v->get_ਮੁੱਲ (glyph_id, ਇਹ) : ਖਾਲੀ_ਸੂਚਕ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &glyphs) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = segments.get_ਲੰਮਾਈ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      segments[i].collect_glyphs (glyphs);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ filter_t>
  ਖਾਲੀ collect_glyphs_filtered (ਸੈੱਟ_t &glyphs, ਅਚਲ filter_t &filter) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = segments.get_ਲੰਮਾਈ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      segments[i].collect_glyphs_filtered (glyphs, ਇਹ, filter);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (segments.sanitize (c, ਇਹ));
  }
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (segments.sanitize (c, ਇਹ, ਅਧਾਰ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_ਖੋਜ_Array_ਦਾ<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (8, segments);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ LookupSingle
{
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ Termination_ਸ਼ਬਦ_ਗਿਣਤੀ = 1u;

  ਪੂਰਨ cmp (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ glyph.cmp (g); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &glyphs) ਅਚਲ
  {
    ਜੇ (glyph == DELETED_GLYPH) ਵਾਪਸ;
    glyphs.ਜੋੜੋ (glyph);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ filter_t>
  ਖਾਲੀ collect_glyphs_filtered (ਸੈੱਟ_t &glyphs, ਅਚਲ filter_t &filter) ਅਚਲ
  {
    ਜੇ (glyph == DELETED_GLYPH) ਵਾਪਸ;
    ਜੇ (!filter (value)) ਵਾਪਸ;
    glyphs.ਜੋੜੋ (glyph);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && value.sanitize (c));
  }
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && value.sanitize (c, ਅਧਾਰ));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (2 + T::static_ਮਾਪ);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ LookupFormat6
{
  ਮਿੱਤਰ ਬਣਤਰ Lookup<T>;

  ਨਿਜੀ:
  ਅਚਲ T* get_ਮੁੱਲ (hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਅਚਲ LookupSingle<T> *v = entries.bsearch (glyph_id);
    ਵਾਪਸ v ? &v->value : ਖਾਲੀ_ਸੂਚਕ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &glyphs) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = entries.get_ਲੰਮਾਈ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      entries[i].collect_glyphs (glyphs);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ filter_t>
  ਖਾਲੀ collect_glyphs_filtered (ਸੈੱਟ_t &glyphs, ਅਚਲ filter_t &filter) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = entries.get_ਲੰਮਾਈ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (entries.sanitize (c));
  }
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (entries.sanitize (c, ਅਧਾਰ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_ਖੋਜ_Array_ਦਾ<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (8, entries);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ LookupFormat8
{
  ਮਿੱਤਰ ਬਣਤਰ Lookup<T>;

  ਨਿਜੀ:
  ਅਚਲ T* get_ਮੁੱਲ (hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਵਾਪਸ ਪਹਿਲਾ_Glyph <= glyph_id && glyph_id - ਪਹਿਲਾ_Glyph < glyph_ਗਿਣਤੀ_2 ?
	   &ਮੁੱਲ_Array_Z[glyph_id - ਪਹਿਲਾ_Glyph] : ਖਾਲੀ_ਸੂਚਕ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &glyphs) ਅਚਲ
  {
    ਜੇ (unlikely (!glyph_ਗਿਣਤੀ_2)) ਵਾਪਸ;
    ਜੇ (ਪਹਿਲਾ_Glyph == DELETED_GLYPH) ਵਾਪਸ;
    glyphs.ਜੋੜੋ_range (ਪਹਿਲਾ_Glyph, ਪਹਿਲਾ_Glyph + glyph_ਗਿਣਤੀ_2 - 1);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ filter_t>
  ਖਾਲੀ collect_glyphs_filtered (ਸੈੱਟ_t &glyphs, ਅਚਲ filter_t &filter) ਅਚਲ
  {
    ਜੇ (unlikely (!glyph_ਗਿਣਤੀ_2)) ਵਾਪਸ;
    ਜੇ (ਪਹਿਲਾ_Glyph == DELETED_GLYPH) ਵਾਪਸ;
    ਅਚਲ T *p = ਮੁੱਲ_Array_Z.arrayZ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < glyph_ਗਿਣਤੀ_2; i++)
      ਜੇ (filter (p[i]))
	glyphs.ਜੋੜੋ (ਪਹਿਲਾ_Glyph + i);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && ਮੁੱਲ_Array_Z.sanitize (c, glyph_ਗਿਣਤੀ_2));
  }
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && ਮੁੱਲ_Array_Z.sanitize (c, glyph_ਗਿਣਤੀ_2, ਅਧਾਰ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	ਪਹਿਲਾ_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_ਗਿਣਤੀ_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_ਦਾ<T>
		ਮੁੱਲ_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (6, ਮੁੱਲ_Array_Z);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ LookupFormat10
{
  ਮਿੱਤਰ ਬਣਤਰ Lookup<T>;

  ਨਿਜੀ:
  ਅਚਲ ਕਿਸਮ_ਨਾਂ T::type get_ਮੁੱਲ_or_null (hb_codepoint_t glyph_id) ਅਚਲ
  {
    ਜੇ (!(ਪਹਿਲਾ_Glyph <= glyph_id && glyph_id - ਪਹਿਲਾ_Glyph < glyph_ਗਿਣਤੀ_2))
      ਵਾਪਸ Null (T);

    ਅਚਲ HBUINT8 *p = &ਮੁੱਲ_Array_Z[(glyph_id - ਪਹਿਲਾ_Glyph) * ਮੁੱਲ_ਮਾਪ];

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਮੁੱਲ_ਮਾਪ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      v = (v << 8) | *p++;

    ਵਾਪਸ v;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &glyphs) ਅਚਲ
  {
    ਜੇ (unlikely (!glyph_ਗਿਣਤੀ_2)) ਵਾਪਸ;
    ਜੇ (ਪਹਿਲਾ_Glyph == DELETED_GLYPH) ਵਾਪਸ;
    glyphs.ਜੋੜੋ_range (ਪਹਿਲਾ_Glyph, ਪਹਿਲਾ_Glyph + glyph_ਗਿਣਤੀ_2 - 1);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ filter_t>
  ਖਾਲੀ collect_glyphs_filtered (ਸੈੱਟ_t &glyphs, ਅਚਲ filter_t &filter) ਅਚਲ
  {
    ਜੇ (unlikely (!glyph_ਗਿਣਤੀ_2)) ਵਾਪਸ;
    ਜੇ (ਪਹਿਲਾ_Glyph == DELETED_GLYPH) ਵਾਪਸ;
    ਅਚਲ HBUINT8 *p = ਮੁੱਲ_Array_Z.arrayZ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < glyph_ਗਿਣਤੀ_2; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਮੁੱਲ_ਮਾਪ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < ਗਿਣਤੀ_2; j++)
	v = (v << 8) | *p++;
      ਜੇ (filter (v))
	glyphs.ਜੋੜੋ (ਪਹਿਲਾ_Glyph + i);
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  ਮੁੱਲ_ਮਾਪ <= 4 &&
		  ਮੁੱਲ_Array_Z.sanitize (c, glyph_ਗਿਣਤੀ_2 * ਮੁੱਲ_ਮਾਪ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	ਮੁੱਲ_ਮਾਪ;	/* Byte size of each value. */
  HBGlyphID16	ਪਹਿਲਾ_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_ਗਿਣਤੀ_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_ਦਾ<HBUINT8>
		ਮੁੱਲ_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (8, ਮੁੱਲ_Array_Z);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ Lookup
{
  ਅਚਲ T* get_ਮੁੱਲ (hb_codepoint_t glyph_id, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_ਮੁੱਲ (glyph_id, ਅੰਕ_glyphs);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_ਮੁੱਲ (glyph_id);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.get_ਮੁੱਲ (glyph_id);
    ਹਾਲਤ 6: hb_barrier (); ਵਾਪਸ u.format6.get_ਮੁੱਲ (glyph_id);
    ਹਾਲਤ 8: hb_barrier (); ਵਾਪਸ u.format8.get_ਮੁੱਲ (glyph_id);
    ਮੂਲ:ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
  }

  ਅਚਲ ਕਿਸਮ_ਨਾਂ T::type get_ਮੁੱਲ_or_null (hb_codepoint_t glyph_id, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
      /* Format 10 cannot return a pointer. */
      ਹਾਲਤ 10: hb_barrier (); ਵਾਪਸ u.format10.get_ਮੁੱਲ_or_null (glyph_id);
      ਮੂਲ:
      ਅਚਲ T *v = get_ਮੁੱਲ (glyph_id, ਅੰਕ_glyphs);
      ਵਾਪਸ v ? *v : Null (T);
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); u.format0.collect_glyphs (glyphs, ਅੰਕ_glyphs); ਵਾਪਸ;
    ਹਾਲਤ 2: hb_barrier (); u.format2.collect_glyphs (glyphs); ਵਾਪਸ;
    ਹਾਲਤ 4: hb_barrier (); u.format4.collect_glyphs (glyphs); ਵਾਪਸ;
    ਹਾਲਤ 6: hb_barrier (); u.format6.collect_glyphs (glyphs); ਵਾਪਸ;
    ਹਾਲਤ 8: hb_barrier (); u.format8.collect_glyphs (glyphs); ਵਾਪਸ;
    ਹਾਲਤ 10: hb_barrier (); u.format10.collect_glyphs (glyphs); ਵਾਪਸ;
    ਮੂਲ:ਵਾਪਸ;
    }
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ filter_t>
  ਖਾਲੀ collect_glyphs_filtered (ਸੈੱਟ_t &glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs, ਅਚਲ filter_t &filter) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, ਅੰਕ_glyphs, filter); ਵਾਪਸ;
    ਹਾਲਤ 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); ਵਾਪਸ;
    ਹਾਲਤ 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); ਵਾਪਸ;
    ਹਾਲਤ 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); ਵਾਪਸ;
    ਹਾਲਤ 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); ਵਾਪਸ;
    ਹਾਲਤ 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); ਵਾਪਸ;
    ਮੂਲ:ਵਾਪਸ;
    }
  }

  ਕਿਸਮ_ਨਾਂ T::type get_class (hb_codepoint_t glyph_id,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਾਹਰਲਾ_ਦਾ_Range) ਅਚਲ
  {
    ਅਚਲ T *v = get_ਮੁੱਲ (glyph_id, ਅੰਕ_glyphs);
    ਵਾਪਸ v ? *v : ਬਾਹਰਲਾ_ਦਾ_Range;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.format.v.sanitize (c)) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ਹਾਲਤ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    ਹਾਲਤ 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    ਹਾਲਤ 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    ਹਾਲਤ 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!u.format.v.sanitize (c)) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 0: hb_barrier (); return_trace (u.format0.sanitize (c, ਅਧਾਰ));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.sanitize (c, ਅਧਾਰ));
    ਹਾਲਤ 4: hb_barrier (); return_trace (u.format4.sanitize (c, ਅਧਾਰ));
    ਹਾਲਤ 6: hb_barrier (); return_trace (u.format6.sanitize (c, ਅਧਾਰ));
    ਹਾਲਤ 8: hb_barrier (); return_trace (u.format8.sanitize (c, ਅਧਾਰ));
    ਹਾਲਤ 10: return_trace (ਝੂਠ); /* We don't support format10 here currently. */
    ਮੂਲ:return_trace (ਸੱਚ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_ਬਾਈਟਾਂ_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ ਇੰਦਰਾਜ_2
{
  // This doesn't seem like it's ever called.
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_ਮਾਪ, "");

    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		ਅੰਕੜੇ;		/* Optional offsets to per-glyph tables. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + T::static_ਮਾਪ);
};

ਸਾਂਚਾ <>
ਬਣਤਰ ਇੰਦਰਾਜ_2<ਖਾਲੀ>
{
  // This does seem like it's ever called.
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਜਨਤਕ:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਗਿਣਤੀ Class
{
  CLASS_ਸਿਰਾ_ਦਾ_ਪਾਠ = 0,
  CLASS_ਬਾਹਰਲਾ_ਦਾ_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_ਸਿਰਾ_ਦਾ_ਸਤਰ = 3,
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types, ਕਿਸਮ_ਨਾਂ Extra>
ਬਣਤਰ State_ਸਾਰਣੀ
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::HBUINT HBUINT;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::HBUSHORT HBUSHORT;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::ClassTypeNarrow ClassType;

  ਗਿਣਤੀ State
  {
    STATE_ਸ਼ੁਰੂ_ਦਾ_ਪਾਠ = 0,
    STATE_ਸ਼ੁਰੂ_ਦਾ_ਸਤਰ = 1,
  };

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ ਸਾਰਣੀ_t>
  ਖਾਲੀ collect_initial_glyphs (ਸੈੱਟ_t &glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs, ਅਚਲ ਸਾਰਣੀ_t &ਸਾਰਣੀ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_classes = nClasses;

    ਜੇ (unlikely (ਅੰਕ_classes > hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t::ਬਿੱਟ_2))
    {
      (ਇਹ+class_ਸਾਰਣੀ).collect_glyphs (glyphs, ਅੰਕ_glyphs);
      ਵਾਪਸ;
    }

    // Collect all classes going out from the start state.
    hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t filter;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_classes; i++)
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ &ਇੰਦਰਾਜ = get_ਇੰਦਰਾਜ (STATE_ਸ਼ੁਰੂ_ਦਾ_ਪਾਠ, i);
      ਜੇ (new_state (ਇੰਦਰਾਜ.newState) == STATE_ਸ਼ੁਰੂ_ਦਾ_ਪਾਠ &&
	  !ਸਾਰਣੀ.is_action_initiable (ਇੰਦਰਾਜ) && !ਸਾਰਣੀ.is_actionable (ਇੰਦਰਾਜ))
	ਜਾਰੀ;

      filter.ਜੋੜੋ (i);
    }

    // And glyphs in those classes.

    ਜੇ (filter (CLASS_DELETED_GLYPH))
      glyphs.ਜੋੜੋ (DELETED_GLYPH);

    (ਇਹ+class_ਸਾਰਣੀ).collect_glyphs_filtered (glyphs, ਅੰਕ_glyphs, filter);
  }

  ਪੂਰਨ new_state (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ newState) ਅਚਲ
  { ਵਾਪਸ Types::extended ? newState : ((ਪੂਰਨ) newState - (ਪੂਰਨ) state_Array_ਸਾਰਣੀ) / (ਪੂਰਨ) nClasses; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_class (hb_codepoint_t glyph_id,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs,
			  hb_aat_class_cache_t *cache = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ klass;
    ਜੇ (cache && cache->get (glyph_id, &klass)) ਵਾਪਸ klass;
    ਜੇ (unlikely (glyph_id == DELETED_GLYPH)) ਵਾਪਸ CLASS_DELETED_GLYPH;
    klass = (ਇਹ+class_ਸਾਰਣੀ).get_class (glyph_id, ਅੰਕ_glyphs, CLASS_ਬਾਹਰਲਾ_ਦਾ_BOUNDS);
    ਜੇ (cache) cache->ਸੈੱਟ (glyph_id, klass);
    ਵਾਪਸ klass;
  }

  ਅਚਲ ਇੰਦਰਾਜ_2<Extra> *get_entries () ਅਚਲ
  { ਵਾਪਸ (ਇਹ+ਇੰਦਰਾਜ_ਸਾਰਣੀ).arrayZ; }

  ਅਚਲ ਇੰਦਰਾਜ_2<Extra> &get_ਇੰਦਰਾਜ (ਪੂਰਨ state, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ n_classes = nClasses;
    ਜੇ (unlikely (klass >= n_classes))
      klass = CLASS_ਬਾਹਰਲਾ_ਦਾ_BOUNDS;

    ਅਚਲ HBUSHORT *ਹਾਲਤਾਂ = (ਇਹ+state_Array_ਸਾਰਣੀ).arrayZ;
    ਅਚਲ ਇੰਦਰਾਜ_2<Extra> *entries = (ਇਹ+ਇੰਦਰਾਜ_ਸਾਰਣੀ).arrayZ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਇੰਦਰਾਜ = ਹਾਲਤਾਂ[state * n_classes + klass];
    DEBUG_ਸੁਨੇਹਾ (APPLY, ਖਾਲੀ_ਸੂਚਕ, "e%u", ਇੰਦਰਾਜ);

    ਵਾਪਸ entries[ਇੰਦਰਾਜ];
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c,
		 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਅੰਕ_entries_ਬਾਹਰਲਾ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!(c->check_struct (ਇਹ) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    class_ਸਾਰਣੀ.sanitize (c, ਇਹ)))) return_trace (ਝੂਠ);

    ਅਚਲ HBUSHORT *ਹਾਲਤਾਂ = (ਇਹ+state_Array_ਸਾਰਣੀ).arrayZ;
    ਅਚਲ ਇੰਦਰਾਜ_2<Extra> *entries = (ਇਹ+ਇੰਦਰਾਜ_ਸਾਰਣੀ).arrayZ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_classes = nClasses;
    ਜੇ (unlikely (hb_unsigned_mul_overflows (ਅੰਕ_classes, ਹਾਲਤਾਂ[0].static_ਮਾਪ)))
      return_trace (ਝੂਠ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪੰਕਤੀ_stride = ਅੰਕ_classes * ਹਾਲਤਾਂ[0].static_ਮਾਪ;

    /* Apple 'kern' table has this peculiarity:
     *
     * "Because the stateTableOffset in the state table header is (strictly
     * speaking) redundant, some 'kern' tables use it to record an initial
     * state where that should not be StartOfText. To determine if this is
     * done, calculate what the stateTableOffset should be. If it's different
     * from the actual stateTableOffset, use it as the initial state."
     *
     * We implement this by calling the initial state zero, but allow *negative*
     * states if the start state indeed was not the first state.  Since the code
     * is shared, this will also apply to 'mort' table.  The 'kerx' / 'morx'
     * tables are not affected since those address states by index, not offset.
     */

    ਪੂਰਨ min_state = 0;
    ਪੂਰਨ max_state = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_entries = 0;

    ਪੂਰਨ state_pos = 0;
    ਪੂਰਨ state_neg = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਇੰਦਰਾਜ = 0;
    ਜਦੋਂ (min_state < state_neg || state_pos <= max_state)
    {
      ਜੇ (min_state < state_neg)
      {
	/* Negative states. */
	ਜੇ (unlikely (hb_unsigned_mul_overflows (min_state, ਅੰਕ_classes)))
	  return_trace (ਝੂਠ);
	ਜੇ (unlikely (!c->check_range (&ਹਾਲਤਾਂ[min_state * ਅੰਕ_classes],
				       -min_state,
				       ਪੰਕਤੀ_stride)))
	  return_trace (ਝੂਠ);
	ਜੇ ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (ਝੂਠ);
	{ /* Sweep new states. */
	  ਅਚਲ HBUSHORT *stop = &ਹਾਲਤਾਂ[min_state * ਅੰਕ_classes];
	  ਜੇ (unlikely (stop > ਹਾਲਤਾਂ))
	    return_trace (ਝੂਠ);
	  ਲਈ (ਅਚਲ HBUSHORT *p = ਹਾਲਤਾਂ; stop < p; p--)
	    ਅੰਕ_entries = hb_max (ਅੰਕ_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      ਜੇ (state_pos <= max_state)
      {
	/* Positive states. */
	ਜੇ (unlikely (!c->check_range (ਹਾਲਤਾਂ,
				       max_state + 1,
				       ਪੰਕਤੀ_stride)))
	  return_trace (ਝੂਠ);
	ਜੇ ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (ਝੂਠ);
	{ /* Sweep new states. */
	  ਜੇ (unlikely (hb_unsigned_mul_overflows ((max_state + 1), ਅੰਕ_classes)))
	    return_trace (ਝੂਠ);
	  ਅਚਲ HBUSHORT *stop = &ਹਾਲਤਾਂ[(max_state + 1) * ਅੰਕ_classes];
	  ਜੇ (unlikely (stop < ਹਾਲਤਾਂ))
	    return_trace (ਝੂਠ);
	  ਲਈ (ਅਚਲ HBUSHORT *p = &ਹਾਲਤਾਂ[state_pos * ਅੰਕ_classes]; p < stop; p++)
	    ਅੰਕ_entries = hb_max (ਅੰਕ_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      ਜੇ (unlikely (!c->check_array (entries, ਅੰਕ_entries)))
	return_trace (ਝੂਠ);
      ਜੇ ((c->max_ops -= ਅੰਕ_entries - ਇੰਦਰਾਜ) <= 0)
	return_trace (ਝੂਠ);
      { /* Sweep new entries. */
	ਅਚਲ ਇੰਦਰਾਜ_2<Extra> *stop = &entries[ਅੰਕ_entries];
	ਲਈ (ਅਚਲ ਇੰਦਰਾਜ_2<Extra> *p = &entries[ਇੰਦਰਾਜ]; p < stop; p++)
	{
	  ਪੂਰਨ newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	ਇੰਦਰਾਜ = ਅੰਕ_entries;
      }
    }

    ਜੇ (ਅੰਕ_entries_ਬਾਹਰਲਾ)
      *ਅੰਕ_entries_ਬਾਹਰਲਾ = ਅੰਕ_entries;

    return_trace (ਸੱਚ);
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffset_ਮੰਜ਼ਿਲ<ClassType, HBUINT>
		class_ਸਾਰਣੀ;	/* Offset to the class table. */
  NNOffset_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBUSHORT>, HBUINT>
		state_Array_ਸਾਰਣੀ;/* Offset to the state array. */
  NNOffset_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<ਇੰਦਰਾਜ_2<Extra>>, HBUINT>
		ਇੰਦਰਾਜ_ਸਾਰਣੀ;	/* Offset to the entry array. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 * ਆਕਾਰ (HBUINT));
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ HBUCHAR>
ਬਣਤਰ Class_ਸਾਰਣੀ
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_class (hb_codepoint_t glyph_id, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਾਹਰਲਾ_ਦਾ_Range) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = glyph_id - ਪਹਿਲਾ_Glyph;
    ਵਾਪਸ i >= classArray.len ? ਬਾਹਰਲਾ_ਦਾ_Range : classArray.arrayZ[i];
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_class (hb_codepoint_t glyph_id,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs HB_ਅਣਵਰਤਿਆ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਾਹਰਲਾ_ਦਾ_Range) ਅਚਲ
  {
    ਵਾਪਸ get_class (glyph_id, ਬਾਹਰਲਾ_ਦਾ_Range);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_glyphs (ਸੈੱਟ_t &glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < classArray.len; i++)
      ਜੇ (classArray.arrayZ[i] != CLASS_ਬਾਹਰਲਾ_ਦਾ_BOUNDS)
	glyphs.ਜੋੜੋ (ਪਹਿਲਾ_Glyph + i);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t, ਕਿਸਮ_ਨਾਂ filter_t>
  ਖਾਲੀ collect_glyphs_filtered (ਸੈੱਟ_t &glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs, ਅਚਲ filter_t &filter) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < classArray.len; i++)
      ਜੇ (filter (classArray.arrayZ[i]))
	glyphs.ਜੋੜੋ (ਪਹਿਲਾ_Glyph + i);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && classArray.sanitize (c));
  }
  ਸੁਰੱਖਿਅਤ:
  HBGlyphID16		ਪਹਿਲਾ_Glyph;	/* First glyph index included in the trimmed array. */
  Array16_ਦਾ<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, classArray);
};

ਬਣਤਰ SubtableGlyphCoverage
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ subtable_ਗਿਣਤੀ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);

    ਜੇ (unlikely (!c->check_array (&subtableOffsets, subtable_ਗਿਣਤੀ)))
      return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਈਟਾਂ = (c->get_ਅੰਕ_glyphs () + CHAR_ਦੋਆਧਾਰੀ_ਅੰਕ - 1) / CHAR_ਦੋਆਧਾਰੀ_ਅੰਕ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < subtable_ਗਿਣਤੀ; i++)
    {
      uint32_t ਵਿੱਥ = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      ਜੇ (ਵਿੱਥ == 0 || ਵਿੱਥ == 0xFFFFFFFF)
        ਜਾਰੀ;
      ਜੇ (unlikely (!subtableOffsets[i].sanitize (c, ਇਹ, ਬਾਈਟਾਂ)))
        return_trace (ਝੂਠ);
    }

    return_trace (ਸੱਚ);
  }
  ਸੁਰੱਖਿਅਤ:
  Unsized_Array_ਦਾ<NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (0, subtableOffsets);
};

ਬਣਤਰ ObsoleteTypes
{
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ extended = ਝੂਠ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ HBUINT16 HBUINT;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ HBUINT8 HBUSHORT;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Class_ਸਾਰਣੀ<HBUINT8> ClassTypeNarrow;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Class_ਸਾਰਣੀ<HBUINT16> ClassTypeWide;

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਪੂਰਨ ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (int64_t ਵਿੱਥ,
			    ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
			    ਅਚਲ T *array)
  {
    int64_t array_ਵਿੱਥ = (ਅਚਲ ਅੱਖਰ *) array - (ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ;
    ਪੂਰਨ bad_ਸੂਚਕਾਂਕ = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ / T::static_ਮਾਪ;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    ਜੇ (unlikely (ਵਿੱਥ < array_ਵਿੱਥ))
      ਵਾਪਸ bad_ਸੂਚਕਾਂਕ;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t ਸੂਚਕਾਂਕ_2 = (ਵਿੱਥ - array_ਵਿੱਥ) / T::static_ਮਾਪ;
    ਜੇ (unlikely (ਸੂਚਕਾਂਕ_2 > bad_ਸੂਚਕਾਂਕ))
      ਵਾਪਸ bad_ਸੂਚਕਾਂਕ;
    ਵਾਪਸ ਸੂਚਕਾਂਕ_2;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਪੂਰਨ byte_ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (int64_t ਵਿੱਥ,
				ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
				ਅਚਲ T *array)
  {
    ਵਾਪਸ ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (ਵਿੱਥ, ਅਧਾਰ, array);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਪੂਰਨ ਸ਼ਬਦ_ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (int64_t ਵਿੱਥ,
				ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
				ਅਚਲ T *array)
  {
    ਵਾਪਸ ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (2 * ਵਿੱਥ, ਅਧਾਰ, array);
  }
};
ਬਣਤਰ ExtendedTypes
{
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ extended = ਸੱਚ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ HBUINT32 HBUINT;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ HBUINT16 HBUSHORT;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Lookup<HBUINT16> ClassTypeNarrow;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Lookup<HBUINT16> ClassTypeWide;

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ,
				     ਅਚਲ ਖਾਲੀ *ਅਧਾਰ HB_ਅਣਵਰਤਿਆ,
				     ਅਚਲ T *array HB_ਅਣਵਰਤਿਆ)
  {
    ਵਾਪਸ ਵਿੱਥ;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ byte_ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ,
					 ਅਚਲ ਖਾਲੀ *ਅਧਾਰ HB_ਅਣਵਰਤਿਆ,
					 ਅਚਲ T *array HB_ਅਣਵਰਤਿਆ)
  {
    ਵਾਪਸ ਵਿੱਥ / 2;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ਬਦ_ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ,
					 ਅਚਲ ਖਾਲੀ *ਅਧਾਰ HB_ਅਣਵਰਤਿਆ,
					 ਅਚਲ T *array HB_ਅਣਵਰਤਿਆ)
  {
    ਵਾਪਸ ਵਿੱਥ;
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types, ਕਿਸਮ_ਨਾਂ ਇੰਦਰਾਜ_ਅੰਕੜੇ, ਕਿਸਮ_ਨਾਂ Flags>
ਬਣਤਰ State_ਸਾਰਣੀ_Driver
{
  ਵਰਤੋਂ State_ਸਾਰਣੀ_T = State_ਸਾਰਣੀ<Types, ਇੰਦਰਾਜ_ਅੰਕੜੇ>;
  ਵਰਤੋਂ ਇੰਦਰਾਜ_T = ਇੰਦਰਾਜ_2<ਇੰਦਰਾਜ_ਅੰਕੜੇ>;

  State_ਸਾਰਣੀ_Driver (ਅਚਲ State_ਸਾਰਣੀ_T &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      ਅੰਕ_glyphs (face_->get_ਅੰਕ_glyphs ()) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t>
  ਖਾਲੀ drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_ਬਫਰ_t *ਬਫਰ = ac->ਬਫਰ;

    ਜੇ (!c->in_place)
      ਬਫਰ->clear_output ();

    ਪੂਰਨ state = State_ਸਾਰਣੀ_T::STATE_ਸ਼ੁਰੂ_ਦਾ_ਪਾਠ;
    // If there's only one range, we already checked the flag.
    ਸਵੈਚਾਲਿਤ *ਆਖਰੀ_range = ac->range_flags && (ac->range_flags->ਲੰਮਾਈ > 1) ? &(*ac->range_flags)[0] : ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ ਤਾਰਕਿਕ ਸ਼ੁਰੂ_state_ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_break_eot =
      !c->ਸਾਰਣੀ->is_actionable (machine.get_ਇੰਦਰਾਜ (State_ਸਾਰਣੀ_T::STATE_ਸ਼ੁਰੂ_ਦਾ_ਪਾਠ, CLASS_ਸਿਰਾ_ਦਾ_ਪਾਠ));
    ਲਈ (ਬਫਰ->idx = 0; ਬਫਰ->successful;)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ klass = likely (ਬਫਰ->idx < ਬਫਰ->len) ?
			   machine.get_class (ਬਫਰ->cur().codepoint, ਅੰਕ_glyphs, ac->machine_class_cache) :
			   (ਬਿਨਾਨਿਸ਼ਾਨੀ) CLASS_ਸਿਰਾ_ਦਾ_ਪਾਠ;
    resume:
      DEBUG_ਸੁਨੇਹਾ (APPLY, ਖਾਲੀ_ਸੂਚਕ, "c%u at %u", klass, ਬਫਰ->idx);
      ਅਚਲ ਇੰਦਰਾਜ_T &ਇੰਦਰਾਜ = machine.get_ਇੰਦਰਾਜ (state, klass);
      ਅਚਲ ਪੂਰਨ ਅਗਲਾ_state = machine.new_state (ਇੰਦਰਾਜ.newState);

      ਤਾਰਕਿਕ is_not_epsilon_transition = !(ਇੰਦਰਾਜ.flags & Flags::DontAdvance);
      ਤਾਰਕਿਕ is_not_actionable = !c->ਸਾਰਣੀ->is_actionable (ਇੰਦਰਾਜ);

      ਜੇ (unlikely (ਆਖਰੀ_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	ਸਵੈਚਾਲਿਤ *range = ਆਖਰੀ_range;
	ਜੇ (ਬਫਰ->idx < ਬਫਰ->len)
	{
	  ਬਿਨਾਨਿਸ਼ਾਨੀ cluster = ਬਫਰ->cur().cluster;
	  ਜਦੋਂ (cluster < range->cluster_ਪਹਿਲਾ)
	    range--;
	  ਜਦੋਂ (cluster > range->cluster_ਆਖਰੀ)
	    range++;


	  ਆਖਰੀ_range = range;
	}
	ਜੇ (!(range->flags & ac->subtable_flags))
	{
	  ਜੇ (ਬਫਰ->idx == ਬਫਰ->len)
	    ਰੋਕੋ;

	  state = State_ਸਾਰਣੀ_T::STATE_ਸ਼ੁਰੂ_ਦਾ_ਪਾਠ;
	  (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
	  ਜਾਰੀ;
	}
      }
      ਨਹੀਂਤਾਂ
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	ਤਾਰਕਿਕ is_null_transition = state == State_ਸਾਰਣੀ_T::STATE_ਸ਼ੁਰੂ_ਦਾ_ਪਾਠ &&
				  ਅਗਲਾ_state == State_ਸਾਰਣੀ_T::STATE_ਸ਼ੁਰੂ_ਦਾ_ਪਾਠ &&
				  ਸ਼ੁਰੂ_state_ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	ਜੇ (is_null_transition)
	{
	  ਬਿਨਾਨਿਸ਼ਾਨੀ old_klass = klass;
	  ਕਰੋ
	  {
	    c->transition (ਬਫਰ, ਇਹ, ਇੰਦਰਾਜ);

	    ਜੇ (ਬਫਰ->idx == ਬਫਰ->len || !ਬਫਰ->successful)
	      ਰੋਕੋ;

	    (ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();

	    klass = likely (ਬਫਰ->idx < ਬਫਰ->len) ?
		     machine.get_class (ਬਫਰ->cur().codepoint, ਅੰਕ_glyphs, ac->machine_class_cache) :
		     (ਬਿਨਾਨਿਸ਼ਾਨੀ) CLASS_ਸਿਰਾ_ਦਾ_ਪਾਠ;
	  } ਜਦੋਂ (klass == old_klass);

	  ਜੇ (ਬਫਰ->idx == ਬਫਰ->len || !ਬਫਰ->successful)
	    ਰੋਕੋ;

	  ਜਾਓ resume;
	}
      }

      /* Conditions under which it's guaranteed safe-to-break before current glyph:
       *
       * 1. There was no action in this transition; and
       *
       * 2. If we break before current glyph, the results will be the same. That
       *    is guaranteed if:
       *
       *    2a. We were already in start-of-text state; or
       *
       *    2b. We are epsilon-transitioning to start-of-text state; or
       *
       *    2c. Starting from start-of-text state seeing current glyph:
       *
       *        2c'. There won't be any actions; and
       *
       *        2c". We would end up in the same state that we were going to end up
       *             in now, including whether epsilon-transitioning.
       *
       *    and
       *
       * 3. If we break before current glyph, there won't be any end-of-text action
       *    after previous glyph.
       *
       * This triples the transitions we need to look up, but is worth returning
       * granular unsafe-to-break results. See eg.:
       *
       *   https://github.com/harfbuzz/harfbuzz/issues/2860
       */
      ਅਚਲ ਇੰਦਰਾਜ_T *wouldbe_ਇੰਦਰਾਜ;
      ਤਾਰਕਿਕ is_ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_break =
      (
          /* 1. */
          !c->ਸਾਰਣੀ->is_actionable (ਇੰਦਰਾਜ) &&

          /* 2. */
          // This one is meh, I know...
	  (
                 state == State_ਸਾਰਣੀ_T::STATE_ਸ਼ੁਰੂ_ਦਾ_ਪਾਠ
              || ((ਇੰਦਰਾਜ.flags & Flags::DontAdvance) && ਅਗਲਾ_state == State_ਸਾਰਣੀ_T::STATE_ਸ਼ੁਰੂ_ਦਾ_ਪਾਠ)
              || (
		    /* 2c. */
		    wouldbe_ਇੰਦਰਾਜ = &machine.get_ਇੰਦਰਾਜ(State_ਸਾਰਣੀ_T::STATE_ਸ਼ੁਰੂ_ਦਾ_ਪਾਠ, klass)
		    ,
		    /* 2c'. */
		    !c->ਸਾਰਣੀ->is_actionable (*wouldbe_ਇੰਦਰਾਜ) &&
		    /* 2c". */
		    (
		      ਅਗਲਾ_state == machine.new_state(wouldbe_ਇੰਦਰਾਜ->newState) &&
		      (ਇੰਦਰਾਜ.flags & Flags::DontAdvance) == (wouldbe_ਇੰਦਰਾਜ->flags & Flags::DontAdvance)
		    )
		 )
	  ) &&

          /* 3. */
          !c->ਸਾਰਣੀ->is_actionable (machine.get_ਇੰਦਰਾਜ (state, CLASS_ਸਿਰਾ_ਦਾ_ਪਾਠ))
      );

      ਜੇ (!is_ਸੁਰੱਖਿਅਤ_ਮੰਜ਼ਿਲ_break && ਬਫਰ->backtrack_len () && ਬਫਰ->idx < ਬਫਰ->len)
	ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break_from_outbuffer (ਬਫਰ->backtrack_len () - 1, ਬਫਰ->idx + 1);

      c->transition (ਬਫਰ, ਇਹ, ਇੰਦਰਾਜ);

      state = ਅਗਲਾ_state;
      DEBUG_ਸੁਨੇਹਾ (APPLY, ਖਾਲੀ_ਸੂਚਕ, "s%d", state);

      ਜੇ (ਬਫਰ->idx == ਬਫਰ->len)
	ਰੋਕੋ;

      ਜੇ (is_not_epsilon_transition || ਬਫਰ->max_ops-- <= 0)
	(ਖਾਲੀ) ਬਫਰ->ਅਗਲਾ_glyph ();
    }

    ਜੇ (!c->in_place)
      ਬਫਰ->ਸਮਕਾਲੀ ();
  }

  ਜਨਤਕ:
  ਅਚਲ State_ਸਾਰਣੀ_T &machine;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs;
};


} /* namespace AAT */


#ਜੇ_ਅੰਤ /* HB_AAT_LAYOUT_COMMON_HH */
