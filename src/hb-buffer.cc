/*
 * Copyright © 1998-2004  David Turner and Werner Lemberg
 * Copyright © 2004,2007,2009,2010  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
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
 * Red Hat Author(s): Owen Taylor, Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-utf.hh"


/**
 * SECTION: hb-buffer
 * @title: hb-buffer
 * @short_description: Input and output buffers
 * @include: hb.h
 *
 * Buffers serve a dual role in HarfBuzz; before shaping, they hold
 * the input characters that are passed to hb_shape(), and after
 * shaping they hold the output glyphs.
 *
 * The input buffer is a sequence of Unicode codepoints, with
 * associated attributes such as direction and script.  The output
 * buffer is a sequence of glyphs, with associated attributes such
 * as position and cluster.
 **/


/**
 * hb_segment_properties_equal:
 * @a: first #hb_segment_properties_t to compare.
 * @b: second #hb_segment_properties_t to compare.
 *
 * Checks the equality of two #hb_segment_properties_t's.
 *
 * Return value:
 * `true` if all properties of @a equal those of @b, `false` otherwise.
 *
 * Since: 0.9.7
 **/
hb_bool_t
hb_segment_properties_equal (ਅਚਲ hb_segment_properties_t *a,
			     ਅਚਲ hb_segment_properties_t *b)
{
  ਵਾਪਸ a->direction == b->direction &&
	 a->script    == b->script    &&
	 a->language  == b->language  &&
	 a->reserved1 == b->reserved1 &&
	 a->reserved2 == b->reserved2;

}

/**
 * hb_segment_properties_hash:
 * @p: #hb_segment_properties_t to hash.
 *
 * Creates a hash representing @p.
 *
 * Return value:
 * A hash of @p.
 *
 * Since: 0.9.7
 **/
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_segment_properties_hash (ਅਚਲ hb_segment_properties_t *p)
{
  ਵਾਪਸ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) p->direction * 31 +
	  (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) p->script) * 31 +
	 (intptr_t) (p->language);
}

/**
 * hb_segment_properties_overlay:
 * @p: #hb_segment_properties_t to fill in.
 * @src: #hb_segment_properties_t to fill in from.
 *
 * Fills in missing fields of @p from @src in a considered manner.
 *
 * First, if @p does not have direction set, direction is copied from @src.
 *
 * Next, if @p and @src have the same direction (which can be unset), if @p
 * does not have script set, script is copied from @src.
 *
 * Finally, if @p and @src have the same direction and script (which either
 * can be unset), if @p does not have language set, language is copied from
 * @src.
 *
 * Since: 3.3.0
 **/
ਖਾਲੀ
hb_segment_properties_overlay (hb_segment_properties_t *p,
			       ਅਚਲ hb_segment_properties_t *src)
{
  ਜੇ (unlikely (!p || !src))
    ਵਾਪਸ;

  ਜੇ (!p->direction)
    p->direction = src->direction;

  ਜੇ (p->direction != src->direction)
    ਵਾਪਸ;

  ਜੇ (!p->script)
    p->script = src->script;

  ਜੇ (p->script != src->script)
    ਵਾਪਸ;

  ਜੇ (!p->language)
    p->language = src->language;
}

/* Here is how the buffer works internally:
 *
 * There are two info pointers: info and out_info.  They always have
 * the same allocated size, but different lengths.
 *
 * As an optimization, both info and out_info may point to the
 * same piece of memory, which is owned by info.  This remains the
 * case as long as out_len doesn't exceed i at any time.
 * In that case, sync() is mostly no-op and the glyph operations
 * operate mostly in-place.
 *
 * As soon as out_info gets longer than info, out_info is moved over
 * to an alternate buffer (which we reuse the pos buffer for), and its
 * current contents (out_len entries) are copied to the new place.
 *
 * This should all remain transparent to the user.  sync() then
 * switches info over to out_info and does housekeeping.
 */



/* Internal API */

ਤਾਰਕਿਕ
hb_ਬਫਰ_t::enlarge (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
{
  ਜੇ (unlikely (ਮਾਪ > max_len))
  {
    successful = ਝੂਠ;
    ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (unlikely (!successful))
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_allocated = allocated;
  hb_glyph_position_t *new_pos = ਖਾਲੀ_ਸੂਚਕ;
  hb_glyph_info_t *new_info = ਖਾਲੀ_ਸੂਚਕ;
  ਤਾਰਕਿਕ separate_ਬਾਹਰਲਾ = ਬਾਹਰਲਾ_info != info;

  ਜੇ (unlikely (hb_unsigned_mul_overflows (ਮਾਪ, ਆਕਾਰ (info[0]))))
    ਜਾਓ ਪੂਰਾ;

  ਜਦੋਂ (ਮਾਪ >= new_allocated)
    new_allocated += (new_allocated >> 1) + 32;

  ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਬਾਈਟਾਂ;
  ਜੇ (unlikely (hb_unsigned_mul_overflows (new_allocated, ਆਕਾਰ (info[0]), &new_ਬਾਈਟਾਂ)))
    ਜਾਓ ਪੂਰਾ;

  static_assert (ਆਕਾਰ (info[0]) == ਆਕਾਰ (pos[0]), "");
  new_pos = (hb_glyph_position_t *) hb_realloc (pos, new_ਬਾਈਟਾਂ);
  new_info = (hb_glyph_info_t *) hb_realloc (info, new_ਬਾਈਟਾਂ);

ਪੂਰਾ:
  ਜੇ (unlikely (!new_pos || !new_info))
    successful = ਝੂਠ;

  ਜੇ (likely (new_pos))
    pos = new_pos;

  ਜੇ (likely (new_info))
    info = new_info;

  ਬਾਹਰਲਾ_info = separate_ਬਾਹਰਲਾ ? (hb_glyph_info_t *) pos : info;
  ਜੇ (likely (successful))
    allocated = new_allocated;

  ਵਾਪਸ likely (successful);
}

ਤਾਰਕਿਕ
hb_ਬਫਰ_t::make_ਗੁੰਜਾਇਸ਼_for (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ਵਿੱਚ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_ਬਾਹਰਲਾ)
{
  ਜੇ (unlikely (!ensure (ਬਾਹਰਲਾ_len + ਅੰਕ_ਬਾਹਰਲਾ))) ਵਾਪਸ ਝੂਠ;

  ਜੇ (ਬਾਹਰਲਾ_info == info &&
      ਬਾਹਰਲਾ_len + ਅੰਕ_ਬਾਹਰਲਾ > idx + ਅੰਕ_ਵਿੱਚ)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (have_output);

    ਬਾਹਰਲਾ_info = (hb_glyph_info_t *) pos;
    hb_memcpy (ਬਾਹਰਲਾ_info, info, ਬਾਹਰਲਾ_len * ਆਕਾਰ (ਬਾਹਰਲਾ_info[0]));
  }

  ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ
hb_ਬਫਰ_t::ਖਿਸਕਾਓ_forward (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2)
{
  ਪ੍ਰਤਿਜ੍ਞਾ (have_output);
  ਜੇ (unlikely (!ensure (len + ਗਿਣਤੀ_2))) ਵਾਪਸ ਝੂਠ;

  max_ops -= len - idx;
  ਜੇ (unlikely (max_ops < 0))
  {
    successful = ਝੂਠ;
    ਵਾਪਸ ਝੂਠ;
  }

  memmove (info + idx + ਗਿਣਤੀ_2, info + idx, (len - idx) * ਆਕਾਰ (info[0]));
  ਜੇ (idx + ਗਿਣਤੀ_2 > len)
  {
    /* Under memory failure we might expose this area.  At least
     * clean it up.  Oh well...
     *
     * Ideally, we should at least set Default_Ignorable bits on
     * these, as well as consistent cluster values.  But the former
     * is layering violation... */
    hb_memset (info + len, 0, (idx + ਗਿਣਤੀ_2 - len) * ਆਕਾਰ (info[0]));
  }
  len += ਗਿਣਤੀ_2;
  idx += ਗਿਣਤੀ_2;

  ਵਾਪਸ ਸੱਚ;
}

hb_ਬਫਰ_t::scratch_ਬਫਰ_t *
hb_ਬਫਰ_t::get_scratch_ਬਫਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਮਾਪ)
{
  have_output = ਝੂਠ;
  have_positions = ਝੂਠ;

  ਬਾਹਰਲਾ_len = 0;
  ਬਾਹਰਲਾ_info = info;

  ਪ੍ਰਤਿਜ੍ਞਾ ((uintptr_t) pos % ਆਕਾਰ (scratch_ਬਫਰ_t) == 0);
  *ਮਾਪ = allocated * ਆਕਾਰ (pos[0]) / ਆਕਾਰ (scratch_ਬਫਰ_t);
  ਵਾਪਸ (scratch_ਬਫਰ_t *) (ਖਾਲੀ *) pos;
}



/* HarfBuzz-Internal API */

ਖਾਲੀ
hb_ਬਫਰ_t::similar (ਅਚਲ hb_ਬਫਰ_t &src)
{
  hb_unicode_funcs_destroy (unicode);
  unicode = hb_unicode_funcs_reference (src.unicode);
  flags = src.flags;
  cluster_level = src.cluster_level;
  replacement = src.replacement;
  invisible = src.invisible;
  not_ਮਿਲਿਆ = src.not_ਮਿਲਿਆ;
  not_ਮਿਲਿਆ_variation_selector = src.not_ਮਿਲਿਆ_variation_selector;
}

ਖਾਲੀ
hb_ਬਫਰ_t::reset ()
{
  hb_unicode_funcs_destroy (unicode);
  unicode = hb_unicode_funcs_reference (hb_unicode_funcs_get_default ());
  flags = HB_ਬਫਰ_FLAG_DEFAULT;
  cluster_level = HB_ਬਫਰ_CLUSTER_LEVEL_DEFAULT;
  replacement = HB_ਬਫਰ_REPLACEMENT_CODEPOINT_DEFAULT;
  invisible = 0;
  not_ਮਿਲਿਆ = 0;
  not_ਮਿਲਿਆ_variation_selector = HB_CODEPOINT_INVALID;

  clear ();
}

ਖਾਲੀ
hb_ਬਫਰ_t::clear ()
{
  content_type = HB_ਬਫਰ_CONTENT_TYPE_INVALID;
  hb_segment_properties_t default_props = HB_SEGMENT_PROPERTIES_DEFAULT;
  props = default_props;

  successful = ਸੱਚ;
  have_output = ਝੂਠ;
  have_positions = ਝੂਠ;

  idx = 0;
  len = 0;
  ਬਾਹਰਲਾ_len = 0;
  ਬਾਹਰਲਾ_info = info;

  hb_memset (context, 0, ਆਕਾਰ context);
  hb_memset (context_len, 0, ਆਕਾਰ context_len);

  deallocate_var_all ();
  serial = 0;
  random_state = 1;
  scratch_flags = HB_ਬਫਰ_SCRATCH_FLAG_DEFAULT;
}

ਖਾਲੀ
hb_ਬਫਰ_t::enter ()
{
  deallocate_var_all ();
  serial = 0;
  scratch_flags = HB_ਬਫਰ_SCRATCH_FLAG_DEFAULT;
  ਬਿਨਾਨਿਸ਼ਾਨੀ mul;
  ਜੇ (likely (!hb_unsigned_mul_overflows (len, HB_ਬਫਰ_MAX_LEN_ਗੁਣਕ, &mul)))
  {
    max_len = hb_max (mul, (ਬਿਨਾਨਿਸ਼ਾਨੀ) HB_ਬਫਰ_MAX_LEN_MIN);
  }
  ਜੇ (likely (!hb_unsigned_mul_overflows (len, HB_ਬਫਰ_MAX_OPS_ਗੁਣਕ, &mul)))
  {
    max_ops = hb_max (mul, (ਬਿਨਾਨਿਸ਼ਾਨੀ) HB_ਬਫਰ_MAX_OPS_MIN);
  }
}
ਖਾਲੀ
hb_ਬਫਰ_t::leave ()
{
  max_len = HB_ਬਫਰ_MAX_LEN_DEFAULT;
  max_ops = HB_ਬਫਰ_MAX_OPS_DEFAULT;
  deallocate_var_all ();
  serial = 0;
}


ਖਾਲੀ
hb_ਬਫਰ_t::ਜੋੜੋ (hb_codepoint_t  codepoint,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    cluster)
{
  hb_glyph_info_t *glyph;

  ਜੇ (unlikely (!ensure (len + 1))) ਵਾਪਸ;

  glyph = &info[len];

  hb_memset (glyph, 0, ਆਕਾਰ (*glyph));
  glyph->codepoint = codepoint;
  glyph->ਨਕਾਬ = 0;
  glyph->cluster = cluster;

  len++;
}

ਖਾਲੀ
hb_ਬਫਰ_t::ਜੋੜੋ_info (ਅਚਲ hb_glyph_info_t &glyph_info)
{
  ਜੇ (unlikely (!ensure (len + 1))) ਵਾਪਸ;

  info[len] = glyph_info;

  len++;
}
ਖਾਲੀ
hb_ਬਫਰ_t::ਜੋੜੋ_info_and_pos (ਅਚਲ hb_glyph_info_t &glyph_info,
			       ਅਚਲ hb_glyph_position_t &glyph_pos)
{
  ਜੇ (unlikely (!ensure (len + 1))) ਵਾਪਸ;

  info[len] = glyph_info;
  ਪ੍ਰਤਿਜ੍ਞਾ (have_positions);
  pos[len] = glyph_pos;

  len++;
}


ਖਾਲੀ
hb_ਬਫਰ_t::clear_output ()
{
  have_output = ਸੱਚ;
  have_positions = ਝੂਠ;

  idx = 0;
  ਬਾਹਰਲਾ_len = 0;
  ਬਾਹਰਲਾ_info = info;
}

ਖਾਲੀ
hb_ਬਫਰ_t::clear_positions ()
{
  have_output = ਝੂਠ;
  have_positions = ਸੱਚ;

  ਬਾਹਰਲਾ_len = 0;
  ਬਾਹਰਲਾ_info = info;

  hb_memset (pos, 0, ਆਕਾਰ (pos[0]) * len);
}

ਤਾਰਕਿਕ
hb_ਬਫਰ_t::ਸਮਕਾਲੀ ()
{
  ਤਾਰਕਿਕ ret = ਝੂਠ;

  ਪ੍ਰਤਿਜ੍ਞਾ (have_output);

  ਪ੍ਰਤਿਜ੍ਞਾ (idx <= len);

  ਜੇ (unlikely (!successful || !ਅਗਲਾ_glyphs (len - idx)))
    ਜਾਓ reset;

  ਜੇ (ਬਾਹਰਲਾ_info != info)
  {
    pos = (hb_glyph_position_t *) info;
    info = ਬਾਹਰਲਾ_info;
  }
  len = ਬਾਹਰਲਾ_len;
  ret = ਸੱਚ;

reset:
  have_output = ਝੂਠ;
  ਬਾਹਰਲਾ_len = 0;
  ਬਾਹਰਲਾ_info = info;
  idx = 0;

  ਵਾਪਸ ret;
}

ਪੂਰਨ
hb_ਬਫਰ_t::sync_so_far ()
{
  ਤਾਰਕਿਕ had_output = have_output;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਹਰਲਾ_i = ਬਾਹਰਲਾ_len;
  ਬਿਨਾਨਿਸ਼ਾਨੀ i = idx;
  ਬਿਨਾਨਿਸ਼ਾਨੀ old_idx = idx;

  ਜੇ (ਸਮਕਾਲੀ ())
    idx = ਬਾਹਰਲਾ_i;
  ਨਹੀਂਤਾਂ
    idx = i;

  ਜੇ (had_output)
  {
    have_output = ਸੱਚ;
    ਬਾਹਰਲਾ_len = idx;
  }

  ਪ੍ਰਤਿਜ੍ਞਾ (idx <= len);

  ਵਾਪਸ idx - old_idx;
}

ਤਾਰਕਿਕ
hb_ਬਫਰ_t::move_ਮੰਜ਼ਿਲ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
{
  ਜੇ (!have_output)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (i <= len);
    idx = i;
    ਵਾਪਸ ਸੱਚ;
  }
  ਜੇ (unlikely (!successful))
    ਵਾਪਸ ਝੂਠ;

  ਪ੍ਰਤਿਜ੍ਞਾ (i <= ਬਾਹਰਲਾ_len + (len - idx));

  ਜੇ (ਬਾਹਰਲਾ_len < i)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = i - ਬਾਹਰਲਾ_len;
    ਜੇ (unlikely (!make_ਗੁੰਜਾਇਸ਼_for (ਗਿਣਤੀ_2, ਗਿਣਤੀ_2))) ਵਾਪਸ ਝੂਠ;

    max_ops -= ਗਿਣਤੀ_2;
    ਜੇ (unlikely (max_ops < 0))
    {
      successful = ਝੂਠ;
      ਵਾਪਸ ਝੂਠ;
    }

    memmove (ਬਾਹਰਲਾ_info + ਬਾਹਰਲਾ_len, info + idx, ਗਿਣਤੀ_2 * ਆਕਾਰ (ਬਾਹਰਲਾ_info[0]));
    idx += ਗਿਣਤੀ_2;
    ਬਾਹਰਲਾ_len += ਗਿਣਤੀ_2;
  }
  ਨਹੀਂਤਾਂ ਜੇ (ਬਾਹਰਲਾ_len > i)
  {
    /* Tricky part: rewinding... */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਾਹਰਲਾ_len - i;

    /* This will blow in our face if memory allocation fails later
     * in this same lookup...
     *
     * We used to shift with extra 32 items.
     * But that would leave empty slots in the buffer in case of allocation
     * failures.  See comments in shift_forward().  This can cause O(N^2)
     * behavior more severely than adding 32 empty slots can... */
    ਜੇ (unlikely (idx < ਗਿਣਤੀ_2 && !ਖਿਸਕਾਓ_forward (ਗਿਣਤੀ_2 - idx))) ਵਾਪਸ ਝੂਠ;

    ਪ੍ਰਤਿਜ੍ਞਾ (idx >= ਗਿਣਤੀ_2);

    max_ops -= ਗਿਣਤੀ_2;
    ਜੇ (unlikely (max_ops < 0))
    {
      successful = ਝੂਠ;
      ਵਾਪਸ ਝੂਠ;
    }

    idx -= ਗਿਣਤੀ_2;
    ਬਾਹਰਲਾ_len -= ਗਿਣਤੀ_2;
    memmove (info + idx, ਬਾਹਰਲਾ_info + ਬਾਹਰਲਾ_len, ਗਿਣਤੀ_2 * ਆਕਾਰ (ਬਾਹਰਲਾ_info[0]));
  }

  ਵਾਪਸ ਸੱਚ;
}


ਖਾਲੀ
hb_ਬਫਰ_t::ਸੈੱਟ_masks (hb_ਨਕਾਬ_t    value,
			hb_ਨਕਾਬ_t    ਨਕਾਬ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster_ਸ਼ੁਰੂ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster_ਸਿਰਾ)
{
  ਜੇ (!ਨਕਾਬ)
    ਵਾਪਸ;

  hb_ਨਕਾਬ_t not_ਨਕਾਬ = ~ਨਕਾਬ;
  value &= ਨਕਾਬ;

  max_ops -= len;
  ਜੇ (unlikely (max_ops < 0))
    successful = ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = len;

  ਜੇ (cluster_ਸ਼ੁਰੂ == 0 && cluster_ਸਿਰਾ == (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      info[i].ਨਕਾਬ = (info[i].ਨਕਾਬ & not_ਨਕਾਬ) | value;
    ਵਾਪਸ;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਜੇ (cluster_ਸ਼ੁਰੂ <= info[i].cluster && info[i].cluster < cluster_ਸਿਰਾ)
      info[i].ਨਕਾਬ = (info[i].ਨਕਾਬ & not_ਨਕਾਬ) | value;
}

ਖਾਲੀ
hb_ਬਫਰ_t::ਮਿਲਾਓ_clusters_impl (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  max_ops -= ਸਿਰਾ - ਸ਼ੁਰੂ;
  ਜੇ (unlikely (max_ops < 0))
    successful = ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = info[ਸ਼ੁਰੂ].cluster;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ + 1; i < ਸਿਰਾ; i++)
    cluster = hb_min (cluster, info[i].cluster);

  /* Extend end */
  ਜੇ (cluster != info[ਸਿਰਾ - 1].cluster)
    ਜਦੋਂ (ਸਿਰਾ < len && info[ਸਿਰਾ - 1].cluster == info[ਸਿਰਾ].cluster)
      ਸਿਰਾ++;

  /* Extend start */
  ਜੇ (cluster != info[ਸ਼ੁਰੂ].cluster)
    ਜਦੋਂ (idx < ਸ਼ੁਰੂ && info[ਸ਼ੁਰੂ - 1].cluster == info[ਸ਼ੁਰੂ].cluster)
      ਸ਼ੁਰੂ--;

  /* If we hit the start of buffer, continue in out-buffer. */
  ਜੇ (idx == ਸ਼ੁਰੂ && info[ਸ਼ੁਰੂ].cluster != cluster)
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਬਾਹਰਲਾ_len; i && ਬਾਹਰਲਾ_info[i - 1].cluster == info[ਸ਼ੁਰੂ].cluster; i--)
      ਸੈੱਟ_cluster (ਬਾਹਰਲਾ_info[i - 1], cluster);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
    ਸੈੱਟ_cluster (info[i], cluster);
}
ਖਾਲੀ
hb_ਬਫਰ_t::ਮਿਲਾਓ_ਬਾਹਰਲਾ_clusters_impl (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  max_ops -= ਸਿਰਾ - ਸ਼ੁਰੂ;
  ਜੇ (unlikely (max_ops < 0))
    successful = ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = ਬਾਹਰਲਾ_info[ਸ਼ੁਰੂ].cluster;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ + 1; i < ਸਿਰਾ; i++)
    cluster = hb_min (cluster, ਬਾਹਰਲਾ_info[i].cluster);

  /* Extend start */
  ਜਦੋਂ (ਸ਼ੁਰੂ && ਬਾਹਰਲਾ_info[ਸ਼ੁਰੂ - 1].cluster == ਬਾਹਰਲਾ_info[ਸ਼ੁਰੂ].cluster)
    ਸ਼ੁਰੂ--;

  /* Extend end */
  ਜਦੋਂ (ਸਿਰਾ < ਬਾਹਰਲਾ_len && ਬਾਹਰਲਾ_info[ਸਿਰਾ - 1].cluster == ਬਾਹਰਲਾ_info[ਸਿਰਾ].cluster)
    ਸਿਰਾ++;

  /* If we hit the end of out-buffer, continue in buffer. */
  ਜੇ (ਸਿਰਾ == ਬਾਹਰਲਾ_len)
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = idx; i < len && info[i].cluster == ਬਾਹਰਲਾ_info[ਸਿਰਾ - 1].cluster; i++)
      ਸੈੱਟ_cluster (info[i], cluster);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
    ਸੈੱਟ_cluster (ਬਾਹਰਲਾ_info[i], cluster);
}
ਖਾਲੀ
hb_ਬਫਰ_t::delete_glyph ()
{
  /* The logic here is duplicated in hb_ot_hide_default_ignorables(). */

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = info[idx].cluster;
  ਜੇ ((idx + 1 < len && cluster == info[idx + 1].cluster) ||
      (ਬਾਹਰਲਾ_len && cluster == ਬਾਹਰਲਾ_info[ਬਾਹਰਲਾ_len - 1].cluster))
  {
    /* Cluster survives; do nothing. */
    ਜਾਓ ਪੂਰਾ;
  }

  ਜੇ (ਬਾਹਰਲਾ_len)
  {
    /* Merge cluster backward. */
    ਜੇ (cluster < ਬਾਹਰਲਾ_info[ਬਾਹਰਲਾ_len - 1].cluster)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ = info[idx].ਨਕਾਬ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_cluster = ਬਾਹਰਲਾ_info[ਬਾਹਰਲਾ_len - 1].cluster;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਬਾਹਰਲਾ_len; i && ਬਾਹਰਲਾ_info[i - 1].cluster == old_cluster; i--)
	ਸੈੱਟ_cluster (ਬਾਹਰਲਾ_info[i - 1], cluster, ਨਕਾਬ);
    }
    ਜਾਓ ਪੂਰਾ;
  }

  ਜੇ (idx + 1 < len)
  {
    /* Merge cluster forward. */
    ਮਿਲਾਓ_clusters (idx, idx + 2);
    ਜਾਓ ਪੂਰਾ;
  }

ਪੂਰਾ:
  skip_glyph ();
}

ਖਾਲੀ
hb_ਬਫਰ_t::delete_glyphs_inplace (ਤਾਰਕਿਕ (*filter) (ਅਚਲ hb_glyph_info_t *info))
{
  /* Merge clusters and delete filtered glyphs.
   * NOTE! We can't use out-buffer as we have positioning data. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = len;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਜੇ (filter (&info[i]))
    {
      /* Merge clusters.
       * Same logic as delete_glyph(), but for in-place removal. */

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = info[i].cluster;
      ਜੇ (i + 1 < ਗਿਣਤੀ_2 && cluster == info[i + 1].cluster)
	ਜਾਰੀ; /* Cluster survives; do nothing. */

      ਜੇ (j)
      {
	/* Merge cluster backward. */
	ਜੇ (cluster < info[j - 1].cluster)
	{
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ = info[i].ਨਕਾਬ;
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_cluster = info[j - 1].cluster;
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ k = j; k && info[k - 1].cluster == old_cluster; k--)
	    ਸੈੱਟ_cluster (info[k - 1], cluster, ਨਕਾਬ);
	}
	ਜਾਰੀ;
      }

      ਜੇ (i + 1 < ਗਿਣਤੀ_2)
	ਮਿਲਾਓ_clusters (i, i + 2); /* Merge cluster forward. */

      ਜਾਰੀ;
    }

    ਜੇ (j != i)
    {
      info[j] = info[i];
      pos[j] = pos[i];
    }
    j++;
  }
  len = j;
}

ਖਾਲੀ
hb_ਬਫਰ_t::guess_segment_properties ()
{
  assert_unicode ();

  /* If script is set to INVALID, guess from buffer contents */
  ਜੇ (props.script == HB_SCRIPT_INVALID) {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < len; i++) {
      hb_script_t script = unicode->script (info[i].codepoint);
      ਜੇ (likely (script != HB_SCRIPT_COMMON &&
		  script != HB_SCRIPT_INHERITED &&
		  script != HB_SCRIPT_UNKNOWN)) {
	props.script = script;
	ਰੋਕੋ;
      }
    }
  }

  /* If direction is set to INVALID, guess from script */
  ਜੇ (props.direction == HB_DIRECTION_INVALID) {
    props.direction = hb_script_get_horizontal_direction (props.script);
    ਜੇ (props.direction == HB_DIRECTION_INVALID)
      props.direction = HB_DIRECTION_LTR;
  }

  /* If language is not set, use default language from locale */
  ਜੇ (props.language == HB_LANGUAGE_INVALID) {
    /* TODO get_default_for_script? using $LANGUAGE */
    props.language = hb_language_get_default ();
  }
}


/* Public API */

DEFINE_NULL_INSTANCE (hb_ਬਫਰ_t) =
{
  HB_OBJECT_ਸਿਰਲੇਖ_STATIC,

  const_cast<hb_unicode_funcs_t *> (&_hb_Null_hb_unicode_funcs_t),
  HB_ਬਫਰ_FLAG_DEFAULT,
  HB_ਬਫਰ_CLUSTER_LEVEL_DEFAULT,
  HB_ਬਫਰ_REPLACEMENT_CODEPOINT_DEFAULT,
  0, /* invisible */
  0, /* not_found */
  HB_CODEPOINT_INVALID, /* not_found_variation_selector */


  HB_ਬਫਰ_CONTENT_TYPE_INVALID,
  HB_SEGMENT_PROPERTIES_DEFAULT,

  ਝੂਠ, /* successful */
  ਝੂਠ, /* have_output */
  ਸੱਚ  /* have_positions */

  /* Zero is good enough for everything else. */
};


/**
 * hb_buffer_create:
 *
 * Creates a new #hb_buffer_t with all properties to defaults.
 *
 * Return value: (transfer full):
 * A newly allocated #hb_buffer_t with a reference count of 1. The initial
 * reference count should be released with hb_buffer_destroy() when you are done
 * using the #hb_buffer_t. This function never returns `NULL`. If memory cannot
 * be allocated, a special #hb_buffer_t object will be returned on which
 * hb_buffer_allocation_successful() returns `false`.
 *
 * Since: 0.9.2
 **/
hb_ਬਫਰ_t *
hb_ਬਫਰ_create ()
{
  hb_ਬਫਰ_t *ਬਫਰ;

  ਜੇ (!(ਬਫਰ = hb_object_create<hb_ਬਫਰ_t> ()))
    ਵਾਪਸ hb_ਬਫਰ_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  ਬਫਰ->max_len = HB_ਬਫਰ_MAX_LEN_DEFAULT;
  ਬਫਰ->max_ops = HB_ਬਫਰ_MAX_OPS_DEFAULT;

  ਬਫਰ->reset ();

  ਵਾਪਸ ਬਫਰ;
}

/**
 * hb_buffer_create_similar:
 * @src: An #hb_buffer_t
 *
 * Creates a new #hb_buffer_t, similar to hb_buffer_create(). The only
 * difference is that the buffer is configured similarly to @src.
 *
 * Return value: (transfer full):
 * A newly allocated #hb_buffer_t, similar to hb_buffer_create().
 *
 * Since: 3.3.0
 **/
hb_ਬਫਰ_t *
hb_ਬਫਰ_create_similar (ਅਚਲ hb_ਬਫਰ_t *src)
{
  hb_ਬਫਰ_t *ਬਫਰ = hb_ਬਫਰ_create ();

  ਬਫਰ->similar (*src);

  ਵਾਪਸ ਬਫਰ;
}

/**
 * hb_buffer_reset:
 * @buffer: An #hb_buffer_t
 *
 * Clears the buffer contents and segment properties, and restores the
 * default Unicode functions and buffer settings.
 *
 * The message callback, its associated data, and any attached user data are
 * preserved. To remove the message callback, call
 * hb_buffer_set_message_func() with a `NULL` function.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_reset (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->reset ();
}

/**
 * hb_buffer_get_empty:
 *
 * Fetches an empty #hb_buffer_t.
 *
 * Return value: (transfer full): The empty buffer
 *
 * Since: 0.9.2
 **/
hb_ਬਫਰ_t *
hb_ਬਫਰ_get_ਖਾਲੀ_ਥਾਵਾਂ ()
{
  ਵਾਪਸ const_cast<hb_ਬਫਰ_t *> (&Null (hb_ਬਫਰ_t));
}

/**
 * hb_buffer_reference: (skip)
 * @buffer: An #hb_buffer_t
 *
 * Increases the reference count on @buffer by one. This prevents @buffer from
 * being destroyed until a matching call to hb_buffer_destroy() is made.
 *
 * Return value: (transfer full):
 * The referenced #hb_buffer_t.
 *
 * Since: 0.9.2
 **/
hb_ਬਫਰ_t *
hb_ਬਫਰ_reference (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ hb_object_reference (ਬਫਰ);
}

/**
 * hb_buffer_destroy: (skip)
 * @buffer: An #hb_buffer_t
 *
 * Deallocate the @buffer.
 * Decreases the reference count on @buffer by one. If the result is zero, then
 * @buffer and all associated resources are freed. See hb_buffer_reference().
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_destroy (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਜੇ (!hb_object_destroy (ਬਫਰ)) ਵਾਪਸ;

  hb_unicode_funcs_destroy (ਬਫਰ->unicode);

  hb_free (ਬਫਰ->info);
  hb_free (ਬਫਰ->pos);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_ਸੁਨੇਹਾ
  ਜੇ (ਬਫਰ->ਸੁਨੇਹਾ_destroy)
    ਬਫਰ->ਸੁਨੇਹਾ_destroy (ਬਫਰ->ਸੁਨੇਹਾ_ਅੰਕੜੇ);
#ਜੇ_ਅੰਤ

  hb_free (ਬਫਰ);
}

/**
 * hb_buffer_set_user_data: (skip)
 * @buffer: An #hb_buffer_t
 * @key: The user-data key
 * @data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified buffer.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_ਬਫਰ_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_ਬਫਰ_t        *ਬਫਰ,
			 hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
			 ਖਾਲੀ *              ਅੰਕੜੇ,
			 hb_destroy_func_t   destroy,
			 hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (ਬਫਰ, key, ਅੰਕੜੇ, destroy, replace);
}

/**
 * hb_buffer_get_user_data: (skip)
 * @buffer: An #hb_buffer_t
 * @key: The user-data key to query
 *
 * Fetches the user data associated with the specified key,
 * attached to the specified buffer.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ *
hb_ਬਫਰ_get_user_ਅੰਕੜੇ (ਅਚਲ hb_ਬਫਰ_t  *ਬਫਰ,
			 hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (ਬਫਰ, key);
}


/**
 * hb_buffer_set_content_type:
 * @buffer: An #hb_buffer_t
 * @content_type: The type of buffer contents to set
 *
 * Sets the type of @buffer contents. Buffers are either empty, contain
 * characters (before shaping), or contain glyphs (the result of shaping).
 *
 * You rarely need to call this function, since a number of other
 * functions transition the content type for you. Namely:
 *
 * - A newly created buffer starts with content type
 *   %HB_BUFFER_CONTENT_TYPE_INVALID. Calling hb_buffer_reset(),
 *   hb_buffer_clear_contents(), as well as calling hb_buffer_set_length()
 *   with an argument of zero all set the buffer content type to invalid
 *   as well.
 *
 * - Calling hb_buffer_add_utf8(), hb_buffer_add_utf16(),
 *   hb_buffer_add_utf32(), hb_buffer_add_codepoints() and
 *   hb_buffer_add_latin1() expect that buffer is either empty and
 *   have a content type of invalid, or that buffer content type is
 *   %HB_BUFFER_CONTENT_TYPE_UNICODE, and they also set the content
 *   type to Unicode if they added anything to an empty buffer.
 *
 * - Finally hb_shape() and hb_shape_full() expect that the buffer
 *   is either empty and have content type of invalid, or that buffer
 *   content type is %HB_BUFFER_CONTENT_TYPE_UNICODE, and upon
 *   success they set the buffer content type to
 *   %HB_BUFFER_CONTENT_TYPE_GLYPHS.
 *
 * The above transitions are designed such that one can use a buffer
 * in a loop of "reset : add-text : shape" without needing to ever
 * modify the content type manually.
 *
 * Since: 0.9.5
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_content_type (hb_ਬਫਰ_t              *ਬਫਰ,
			    hb_ਬਫਰ_content_type_t  content_type)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->content_type = content_type;
}

/**
 * hb_buffer_get_content_type:
 * @buffer: An #hb_buffer_t
 *
 * Fetches the type of @buffer contents. Buffers are either empty, contain
 * characters (before shaping), or contain glyphs (the result of shaping).
 *
 * Return value:
 * The type of @buffer contents
 *
 * Since: 0.9.5
 **/
hb_ਬਫਰ_content_type_t
hb_ਬਫਰ_get_content_type (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->content_type;
}


/**
 * hb_buffer_set_unicode_funcs:
 * @buffer: An #hb_buffer_t
 * @unicode_funcs: The Unicode-functions structure
 *
 * Sets the Unicode-functions structure of a buffer to
 * @unicode_funcs.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_unicode_funcs (hb_ਬਫਰ_t        *ਬਫਰ,
			     hb_unicode_funcs_t *unicode_funcs)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਜੇ (!unicode_funcs)
    unicode_funcs = hb_unicode_funcs_get_default ();

  hb_unicode_funcs_reference (unicode_funcs);
  hb_unicode_funcs_destroy (ਬਫਰ->unicode);
  ਬਫਰ->unicode = unicode_funcs;
}

/**
 * hb_buffer_get_unicode_funcs:
 * @buffer: An #hb_buffer_t
 *
 * Fetches the Unicode-functions structure of a buffer.
 *
 * Return value: The Unicode-functions structure
 *
 * Since: 0.9.2
 **/
hb_unicode_funcs_t *
hb_ਬਫਰ_get_unicode_funcs (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->unicode;
}

/**
 * hb_buffer_set_direction:
 * @buffer: An #hb_buffer_t
 * @direction: the #hb_direction_t of the @buffer
 *
 * Set the text flow direction of the buffer. No shaping can happen without
 * setting @buffer direction, and it controls the visual direction for the
 * output glyphs; for RTL direction the glyphs will be reversed. Many layout
 * features depend on the proper setting of the direction, for example,
 * reversing RTL text before shaping, then shaping with LTR direction is not
 * the same as keeping the text in logical order and shaping with RTL
 * direction.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_direction (hb_ਬਫਰ_t    *ਬਫਰ,
			 hb_direction_t  direction)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->props.direction = direction;
}

/**
 * hb_buffer_get_direction:
 * @buffer: An #hb_buffer_t
 *
 * See hb_buffer_set_direction()
 *
 * Return value:
 * The direction of the @buffer.
 *
 * Since: 0.9.2
 **/
hb_direction_t
hb_ਬਫਰ_get_direction (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->props.direction;
}

/**
 * hb_buffer_set_script:
 * @buffer: An #hb_buffer_t
 * @script: An #hb_script_t to set.
 *
 * Sets the script of @buffer to @script.
 *
 * Script is crucial for choosing the proper shaping behaviour for scripts that
 * require it (e.g. Arabic) and the which OpenType features defined in the font
 * to be applied.
 *
 * You can pass one of the predefined #hb_script_t values, or use
 * hb_script_from_string() or hb_script_from_iso15924_tag() to get the
 * corresponding script from an ISO 15924 script tag.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_script (hb_ਬਫਰ_t *ਬਫਰ,
		      hb_script_t  script)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->props.script = script;
}

/**
 * hb_buffer_get_script:
 * @buffer: An #hb_buffer_t
 *
 * Fetches the script of @buffer.
 *
 * Return value:
 * The #hb_script_t of the @buffer
 *
 * Since: 0.9.2
 **/
hb_script_t
hb_ਬਫਰ_get_script (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->props.script;
}

/**
 * hb_buffer_set_language:
 * @buffer: An #hb_buffer_t
 * @language: An hb_language_t to set
 *
 * Sets the language of @buffer to @language.
 *
 * Languages are crucial for selecting which OpenType feature to apply to the
 * buffer which can result in applying language-specific behaviour. Languages
 * are orthogonal to the scripts, and though they are related, they are
 * different concepts and should not be confused with each other.
 *
 * Use hb_language_from_string() to convert from BCP 47 language tags to
 * #hb_language_t.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_language (hb_ਬਫਰ_t   *ਬਫਰ,
			hb_language_t  language)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->props.language = language;
}

/**
 * hb_buffer_get_language:
 * @buffer: An #hb_buffer_t
 *
 * See hb_buffer_set_language().
 *
 * Return value: (transfer none):
 * The #hb_language_t of the buffer. Must not be freed by the caller.
 *
 * Since: 0.9.2
 **/
hb_language_t
hb_ਬਫਰ_get_language (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->props.language;
}

/**
 * hb_buffer_set_segment_properties:
 * @buffer: An #hb_buffer_t
 * @props: An #hb_segment_properties_t to use
 *
 * Sets the segment properties of the buffer, a shortcut for calling
 * hb_buffer_set_direction(), hb_buffer_set_script() and
 * hb_buffer_set_language() individually.
 *
 * Since: 0.9.7
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_segment_properties (hb_ਬਫਰ_t *ਬਫਰ,
				  ਅਚਲ hb_segment_properties_t *props)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->props = *props;
}

/**
 * hb_buffer_get_segment_properties:
 * @buffer: An #hb_buffer_t
 * @props: (out): The output #hb_segment_properties_t
 *
 * Sets @props to the #hb_segment_properties_t of @buffer.
 *
 * Since: 0.9.7
 **/
ਖਾਲੀ
hb_ਬਫਰ_get_segment_properties (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ,
				  hb_segment_properties_t *props)
{
  *props = ਬਫਰ->props;
}


/**
 * hb_buffer_set_flags:
 * @buffer: An #hb_buffer_t
 * @flags: The buffer flags to set
 *
 * Sets @buffer flags to @flags. See #hb_buffer_flags_t.
 *
 * Since: 0.9.7
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_flags (hb_ਬਫਰ_t       *ਬਫਰ,
		     hb_ਬਫਰ_flags_t  flags)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->flags = flags;
}

/**
 * hb_buffer_get_flags:
 * @buffer: An #hb_buffer_t
 *
 * Fetches the #hb_buffer_flags_t of @buffer.
 *
 * Return value:
 * The @buffer flags
 *
 * Since: 0.9.7
 **/
hb_ਬਫਰ_flags_t
hb_ਬਫਰ_get_flags (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->flags;
}

/**
 * hb_buffer_set_cluster_level:
 * @buffer: An #hb_buffer_t
 * @cluster_level: The cluster level to set on the buffer
 *
 * Sets the cluster level of a buffer. The #hb_buffer_cluster_level_t
 * dictates one aspect of how HarfBuzz will treat non-base characters
 * during shaping.
 *
 * Since: 0.9.42
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_cluster_level (hb_ਬਫਰ_t               *ਬਫਰ,
			     hb_ਬਫਰ_cluster_level_t  cluster_level)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->cluster_level = cluster_level;
}

/**
 * hb_buffer_get_cluster_level:
 * @buffer: An #hb_buffer_t
 *
 * Fetches the cluster level of a buffer. The #hb_buffer_cluster_level_t
 * dictates one aspect of how HarfBuzz will treat non-base characters
 * during shaping.
 *
 * Return value: The cluster level of @buffer
 *
 * Since: 0.9.42
 **/
hb_ਬਫਰ_cluster_level_t
hb_ਬਫਰ_get_cluster_level (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->cluster_level;
}


/**
 * hb_buffer_set_replacement_codepoint:
 * @buffer: An #hb_buffer_t
 * @replacement: the replacement #hb_codepoint_t
 *
 * Sets the #hb_codepoint_t that replaces invalid entries for a given encoding
 * when adding text to @buffer.
 *
 * Default is #HB_BUFFER_REPLACEMENT_CODEPOINT_DEFAULT.
 *
 * Since: 0.9.31
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_replacement_codepoint (hb_ਬਫਰ_t    *ਬਫਰ,
				     hb_codepoint_t  replacement)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->replacement = replacement;
}

/**
 * hb_buffer_get_replacement_codepoint:
 * @buffer: An #hb_buffer_t
 *
 * Fetches the #hb_codepoint_t that replaces invalid entries for a given encoding
 * when adding text to @buffer.
 *
 * Return value:
 * The @buffer replacement #hb_codepoint_t
 *
 * Since: 0.9.31
 **/
hb_codepoint_t
hb_ਬਫਰ_get_replacement_codepoint (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->replacement;
}


/**
 * hb_buffer_set_invisible_glyph:
 * @buffer: An #hb_buffer_t
 * @invisible: the invisible #hb_codepoint_t
 *
 * Sets the #hb_codepoint_t that replaces invisible characters in
 * the shaping result.  If set to zero (default), the glyph for the
 * U+0020 SPACE character is used.  Otherwise, this value is used
 * verbatim.
 *
 * Since: 2.0.0
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_invisible_glyph (hb_ਬਫਰ_t    *ਬਫਰ,
			       hb_codepoint_t  invisible)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->invisible = invisible;
}

/**
 * hb_buffer_get_invisible_glyph:
 * @buffer: An #hb_buffer_t
 *
 * See hb_buffer_set_invisible_glyph().
 *
 * Return value:
 * The @buffer invisible #hb_codepoint_t
 *
 * Since: 2.0.0
 **/
hb_codepoint_t
hb_ਬਫਰ_get_invisible_glyph (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->invisible;
}

/**
 * hb_buffer_set_not_found_glyph:
 * @buffer: An #hb_buffer_t
 * @not_found: the not-found #hb_codepoint_t
 *
 * Sets the #hb_codepoint_t that replaces characters not found in
 * the font during shaping.
 *
 * The not-found glyph defaults to zero, sometimes known as the
 * ".notdef" glyph.  This API allows for differentiating the two.
 *
 * Since: 3.1.0
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_not_ਮਿਲਿਆ_glyph (hb_ਬਫਰ_t    *ਬਫਰ,
			       hb_codepoint_t  not_ਮਿਲਿਆ)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->not_ਮਿਲਿਆ = not_ਮਿਲਿਆ;
}

/**
 * hb_buffer_get_not_found_glyph:
 * @buffer: An #hb_buffer_t
 *
 * See hb_buffer_set_not_found_glyph().
 *
 * Return value:
 * The @buffer not-found #hb_codepoint_t
 *
 * Since: 3.1.0
 **/
hb_codepoint_t
hb_ਬਫਰ_get_not_ਮਿਲਿਆ_glyph (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->not_ਮਿਲਿਆ;
}

/**
 * hb_buffer_set_not_found_variation_selector_glyph:
 * @buffer: An #hb_buffer_t
 * @not_found_variation_selector: the not-found-variation-selector #hb_codepoint_t
 *
 * Sets the #hb_codepoint_t that replaces variation-selector characters not resolved
 * in the font during shaping.
 *
 * The not-found-variation-selector glyph defaults to #HB_CODEPOINT_INVALID,
 * in which case an unresolved variation-selector will be removed from the glyph
 * string during shaping. This API allows for changing that and retaining a glyph,
 * such that the situation can be detected by the client and handled accordingly
 * (e.g. by using a different font).
 *
 * Since: 10.0.0
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_not_ਮਿਲਿਆ_variation_selector_glyph (hb_ਬਫਰ_t    *ਬਫਰ,
						  hb_codepoint_t  not_ਮਿਲਿਆ_variation_selector)
{
  ਬਫਰ->not_ਮਿਲਿਆ_variation_selector = not_ਮਿਲਿਆ_variation_selector;
}

/**
 * hb_buffer_get_not_found_variation_selector_glyph:
 * @buffer: An #hb_buffer_t
 *
 * See hb_buffer_set_not_found_variation_selector_glyph().
 *
 * Return value:
 * The @buffer not-found-variation-selector #hb_codepoint_t
 *
 * Since: 10.0.0
 **/
hb_codepoint_t
hb_ਬਫਰ_get_not_ਮਿਲਿਆ_variation_selector_glyph (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->not_ਮਿਲਿਆ_variation_selector;
}

/**
 * hb_buffer_set_random_state:
 * @buffer: An #hb_buffer_t
 * @state: the new random state
 *
 * Sets the random state of the buffer. The state changes
 * every time a glyph uses randomness (eg. the `rand`
 * OpenType feature). This function together with
 * hb_buffer_get_random_state() allow for transferring
 * the current random state to a subsequent buffer, to
 * get better randomness distribution.
 *
 * Defaults to 1 and when buffer contents are cleared.
 * A value of 0 disables randomness during shaping.
 *
 * Since: 8.4.0
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_random_state (hb_ਬਫਰ_t    *ਬਫਰ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ        state)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->random_state = state;
}

/**
 * hb_buffer_get_random_state:
 * @buffer: An #hb_buffer_t
 *
 * See hb_buffer_set_random_state().
 *
 * Return value:
 * The @buffer random state
 *
 * Since: 8.4.0
 **/
ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_ਬਫਰ_get_random_state (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->random_state;
}

/**
 * hb_buffer_clear_contents:
 * @buffer: An #hb_buffer_t
 *
 * Clears the buffer contents and segment properties (direction, script,
 * language), and resets the random state to its default value of 1.
 *
 * Unlike hb_buffer_reset(), this function preserves the Unicode functions,
 * flags, cluster level, replacement code point, and invisible and not-found
 * glyphs. The message callback and attached user data are also preserved.
 *
 * Since: 0.9.11
 **/
ਖਾਲੀ
hb_ਬਫਰ_clear_contents (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਬਫਰ->clear ();
}

/**
 * hb_buffer_pre_allocate:
 * @buffer: An #hb_buffer_t
 * @size: Number of items to pre allocate.
 *
 * Pre allocates memory for @buffer to fit at least @size number of items.
 *
 * Return value:
 * `true` if @buffer memory allocation succeeded, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_ਬਫਰ_pre_allocate (hb_ਬਫਰ_t *ਬਫਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
{
  ਵਾਪਸ ਬਫਰ->ensure (ਮਾਪ);
}

/**
 * hb_buffer_allocation_successful:
 * @buffer: An #hb_buffer_t
 *
 * Check if allocating memory for the buffer succeeded.
 *
 * Return value:
 * `true` if @buffer memory allocation succeeded, `false` otherwise.
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_ਬਫਰ_ਵੰਡ_successful (hb_ਬਫਰ_t  *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->successful;
}

/**
 * hb_buffer_add:
 * @buffer: An #hb_buffer_t
 * @codepoint: A Unicode code point.
 * @cluster: The cluster value of @codepoint.
 *
 * Appends a character with the Unicode value of @codepoint to @buffer, and
 * gives it the initial cluster value of @cluster. Clusters can be any thing
 * the client wants, they are usually used to refer to the index of the
 * character in the input text stream and are output in
 * #hb_glyph_info_t.cluster field.
 *
 * This function does not check the validity of @codepoint, it is up to the
 * caller to ensure it is a valid Unicode code point.
 *
 * Since: 0.9.7
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਜੋੜੋ (hb_ਬਫਰ_t    *ਬਫਰ,
	       hb_codepoint_t  codepoint,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    cluster)
{
  ਬਫਰ->ਜੋੜੋ (codepoint, cluster);
  ਬਫਰ->clear_context (1);
}

/**
 * hb_buffer_set_length:
 * @buffer: An #hb_buffer_t
 * @length: The new length of @buffer
 *
 * Similar to hb_buffer_pre_allocate(), but clears any new items added at the
 * end.
 *
 * Return value:
 * `true` if @buffer memory allocation succeeded, `false` otherwise.
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_ਬਫਰ_ਸੈੱਟ_ਲੰਮਾਈ (hb_ਬਫਰ_t  *ਬਫਰ,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਲੰਮਾਈ)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ ਲੰਮਾਈ == 0;

  ਜੇ (unlikely (!ਬਫਰ->ensure (ਲੰਮਾਈ)))
    ਵਾਪਸ ਝੂਠ;

  /* Wipe the new space */
  ਜੇ (ਲੰਮਾਈ > ਬਫਰ->len) {
    hb_memset (ਬਫਰ->info + ਬਫਰ->len, 0, ਆਕਾਰ (ਬਫਰ->info[0]) * (ਲੰਮਾਈ - ਬਫਰ->len));
    ਜੇ (ਬਫਰ->have_positions)
      hb_memset (ਬਫਰ->pos + ਬਫਰ->len, 0, ਆਕਾਰ (ਬਫਰ->pos[0]) * (ਲੰਮਾਈ - ਬਫਰ->len));
  }

  ਬਫਰ->len = ਲੰਮਾਈ;

  ਜੇ (!ਲੰਮਾਈ)
  {
    ਬਫਰ->content_type = HB_ਬਫਰ_CONTENT_TYPE_INVALID;
    ਬਫਰ->clear_context (0);
  }
  ਬਫਰ->clear_context (1);

  ਵਾਪਸ ਸੱਚ;
}

/**
 * hb_buffer_get_length:
 * @buffer: An #hb_buffer_t
 *
 * Returns the number of items in the buffer.
 *
 * Return value:
 * The @buffer length.
 * The value valid as long as buffer has not been modified.
 *
 * Since: 0.9.2
 **/
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ਬਫਰ_get_ਲੰਮਾਈ (ਅਚਲ hb_ਬਫਰ_t *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->len;
}

/**
 * hb_buffer_get_glyph_infos:
 * @buffer: An #hb_buffer_t
 * @length: (out): The output-array length.
 *
 * Returns @buffer glyph information array.  Returned pointer
 * is valid as long as @buffer contents are not modified.
 *
 * Return value: (transfer none) (array length=length):
 * The @buffer glyph information array.
 * The value valid as long as buffer has not been modified.
 *
 * Since: 0.9.2
 **/
hb_glyph_info_t *
hb_ਬਫਰ_get_glyph_infos (hb_ਬਫਰ_t  *ਬਫਰ,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਲੰਮਾਈ)
{
  ਜੇ (ਲੰਮਾਈ)
    *ਲੰਮਾਈ = ਬਫਰ->len;

  ਵਾਪਸ (hb_glyph_info_t *) ਬਫਰ->info;
}

/**
 * hb_buffer_get_glyph_positions:
 * @buffer: An #hb_buffer_t
 * @length: (out): The output length
 *
 * Returns @buffer glyph position array.  Returned pointer
 * is valid as long as @buffer contents are not modified.
 *
 * If buffer did not have positions before, the positions will be
 * initialized to zeros, unless this function is called from
 * within a buffer message callback (see hb_buffer_set_message_func()),
 * in which case `NULL` is returned.
 *
 * Return value: (transfer none) (array length=length):
 * The @buffer glyph position array.
 * The value valid as long as buffer has not been modified.
 *
 * Since: 0.9.2
 **/
hb_glyph_position_t *
hb_ਬਫਰ_get_glyph_positions (hb_ਬਫਰ_t  *ਬਫਰ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਲੰਮਾਈ)
{
  ਜੇ (ਲੰਮਾਈ)
    *ਲੰਮਾਈ = ਬਫਰ->len;

  ਜੇ (!ਬਫਰ->have_positions)
  {
    ਜੇ (unlikely (ਬਫਰ->ਸੁਨੇਹਾ_ਡੂੰਘਾਈ))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਬਫਰ->clear_positions ();
  }

  ਵਾਪਸ (hb_glyph_position_t *) ਬਫਰ->pos;
}

/**
 * hb_buffer_has_positions:
 * @buffer: an #hb_buffer_t.
 *
 * Returns whether @buffer has glyph position data.
 * A buffer gains position data when hb_buffer_get_glyph_positions() is called on it,
 * and cleared of position data when hb_buffer_clear_contents() is called.
 *
 * Return value:
 * `true` if the @buffer has position array, `false` otherwise.
 *
 * Since: 2.7.3
 **/
HB_EXTERN hb_bool_t
hb_ਬਫਰ_has_positions (hb_ਬਫਰ_t  *ਬਫਰ)
{
  ਵਾਪਸ ਬਫਰ->have_positions;
}

/**
 * hb_glyph_info_get_glyph_flags:
 * @info: a #hb_glyph_info_t
 *
 * Returns glyph flags encoded within a #hb_glyph_info_t.
 *
 * Return value:
 * The #hb_glyph_flags_t encoded within @info
 *
 * Since: 1.5.0
 **/
hb_glyph_flags_t
(hb_glyph_info_get_glyph_flags) (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ hb_glyph_info_get_glyph_flags (info);
}

/**
 * hb_buffer_reverse:
 * @buffer: An #hb_buffer_t
 *
 * Reverses buffer contents.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਉਲਟ (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਬਫਰ->reverse ();
}

/**
 * hb_buffer_reverse_range:
 * @buffer: An #hb_buffer_t
 * @start: start index
 * @end: end index
 *
 * Reverses buffer contents between @start and @end.
 *
 * Since: 0.9.41
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਉਲਟ_range (hb_ਬਫਰ_t *ਬਫਰ,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  ਬਫਰ->ਉਲਟ_range (ਸ਼ੁਰੂ, ਸਿਰਾ);
}

/**
 * hb_buffer_reverse_clusters:
 * @buffer: An #hb_buffer_t
 *
 * Reverses buffer clusters.  That is, the buffer contents are
 * reversed, then each cluster (consecutive items having the
 * same cluster number) are reversed again.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਉਲਟ_clusters (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਬਫਰ->ਉਲਟ_clusters ();
}

/**
 * hb_buffer_guess_segment_properties:
 * @buffer: An #hb_buffer_t
 *
 * Sets unset buffer segment properties based on buffer Unicode
 * contents.  If buffer is not empty, it must have content type
 * #HB_BUFFER_CONTENT_TYPE_UNICODE.
 *
 * If buffer script is not set (ie. is #HB_SCRIPT_INVALID), it
 * will be set to the Unicode script of the first character in
 * the buffer that has a script other than #HB_SCRIPT_COMMON,
 * #HB_SCRIPT_INHERITED, and #HB_SCRIPT_UNKNOWN.
 *
 * Next, if buffer direction is not set (ie. is #HB_DIRECTION_INVALID),
 * it will be set to the natural horizontal direction of the
 * buffer script as returned by hb_script_get_horizontal_direction().
 * If hb_script_get_horizontal_direction() returns #HB_DIRECTION_INVALID,
 * then #HB_DIRECTION_LTR is used.
 *
 * Finally, if buffer language is not set (ie. is #HB_LANGUAGE_INVALID),
 * it will be set to the process's default language as returned by
 * hb_language_get_default().  This may change in the future by
 * taking buffer script into consideration when choosing a language.
 * Note that hb_language_get_default() is NOT threadsafe the first time
 * it is called.  See documentation for that function for details.
 *
 * Since: 0.9.7
 **/
ਖਾਲੀ
hb_ਬਫਰ_guess_segment_properties (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਬਫਰ->guess_segment_properties ();
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ utf_t>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਬਫਰ_ਜੋੜੋ_utf (hb_ਬਫਰ_t  *ਬਫਰ,
		   ਅਚਲ ਕਿਸਮ_ਨਾਂ utf_t::codepoint_t *ਪਾਠ,
		   ਪੂਰਨ           ਪਾਠ_ਲੰਮਾਈ,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਵਸਤੂ_ਵਿੱਥ,
		   ਪੂਰਨ           ਵਸਤੂ_ਲੰਮਾਈ)
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ utf_t::codepoint_t T;
  ਅਚਲ hb_codepoint_t replacement = ਬਫਰ->replacement;

  ਬਫਰ->assert_unicode ();

  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)))
    ਵਾਪਸ;

  ਜੇ (ਪਾਠ_ਲੰਮਾਈ == -1)
    ਪਾਠ_ਲੰਮਾਈ = utf_t::ਲੜੀ_ਲੰਬਾਈ (ਪਾਠ);

  ਜੇ (ਵਸਤੂ_ਲੰਮਾਈ == -1)
    ਵਸਤੂ_ਲੰਮਾਈ = ਪਾਠ_ਲੰਮਾਈ - ਵਸਤੂ_ਵਿੱਥ;

  ਵਸਤੂ_ਵਿੱਥ = hb_min (ਵਸਤੂ_ਵਿੱਥ, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਪਾਠ_ਲੰਮਾਈ);
  ਵਸਤੂ_ਲੰਮਾਈ = hb_clamp (ਵਸਤੂ_ਲੰਮਾਈ, 0, ਪਾਠ_ਲੰਮਾਈ - (ਪੂਰਨ) ਵਸਤੂ_ਵਿੱਥ);

  ਜੇ (unlikely (ਵਸਤੂ_ਲੰਮਾਈ < 0 ||
		ਵਸਤੂ_ਲੰਮਾਈ > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ / 8 ||
		!ਬਫਰ->ensure (ਬਫਰ->len + ਵਸਤੂ_ਲੰਮਾਈ * ਆਕਾਰ (T) / 4)))
    ਵਾਪਸ;

  /* If buffer is empty and pre-context provided, install it.
   * This check is written this way, to make sure people can
   * provide pre-context in one add_utf() call, then provide
   * text in a follow-up call.  See:
   *
   * https://bugzilla.mozilla.org/show_bug.cgi?id=801410#c13
   */
  ਜੇ (!ਬਫਰ->len && ਵਸਤੂ_ਵਿੱਥ > 0)
  {
    /* Add pre-context */
    ਬਫਰ->clear_context (0);
    ਅਚਲ T *prev = ਪਾਠ + ਵਸਤੂ_ਵਿੱਥ;
    ਅਚਲ T *ਸ਼ੁਰੂ = ਪਾਠ;
    ਜਦੋਂ (ਸ਼ੁਰੂ < prev && ਬਫਰ->context_len[0] < ਬਫਰ->CONTEXT_ਲੰਮਾਈ)
    {
      hb_codepoint_t u;
      prev = utf_t::prev (prev, ਸ਼ੁਰੂ, &u, replacement);
      ਬਫਰ->context[0][ਬਫਰ->context_len[0]++] = u;
    }
  }

  ਅਚਲ T *next = ਪਾਠ + ਵਸਤੂ_ਵਿੱਥ;
  ਅਚਲ T *ਸਿਰਾ = next + ਵਸਤੂ_ਲੰਮਾਈ;
  ਜਦੋਂ (next < ਸਿਰਾ)
  {
    hb_codepoint_t u;
    ਅਚਲ T *old_ਅਗਲਾ = next;
    next = utf_t::next (next, ਸਿਰਾ, &u, replacement);
    ਬਫਰ->ਜੋੜੋ (u, old_ਅਗਲਾ - (ਅਚਲ T *) ਪਾਠ);
  }

  /* Add post-context */
  ਬਫਰ->clear_context (1);
  ਸਿਰਾ = ਪਾਠ + ਪਾਠ_ਲੰਮਾਈ;
  ਜਦੋਂ (next < ਸਿਰਾ && ਬਫਰ->context_len[1] < ਬਫਰ->CONTEXT_ਲੰਮਾਈ)
  {
    hb_codepoint_t u;
    next = utf_t::next (next, ਸਿਰਾ, &u, replacement);
    ਬਫਰ->context[1][ਬਫਰ->context_len[1]++] = u;
  }

  ਬਫਰ->content_type = HB_ਬਫਰ_CONTENT_TYPE_UNICODE;
}

/**
 * hb_buffer_add_utf8:
 * @buffer: An #hb_buffer_t
 * @text: (array length=text_length) (element-type uint8_t): An array of UTF-8
 *               characters to append.
 * @text_length: The length of the @text, or -1 if it is `NULL` terminated.
 * @item_offset: The offset of the first character to add to the @buffer.
 * @item_length: The number of characters to add to the @buffer, or -1 for the
 *               end of @text (assuming it is `NULL` terminated).
 *
 * See hb_buffer_add_codepoints().
 *
 * Replaces invalid UTF-8 characters with the @buffer replacement code point,
 * see hb_buffer_set_replacement_codepoint().
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਜੋੜੋ_utf8 (hb_ਬਫਰ_t  *ਬਫਰ,
		    ਅਚਲ ਅੱਖਰ   *ਪਾਠ,
		    ਪੂਰਨ           ਪਾਠ_ਲੰਮਾਈ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਵਸਤੂ_ਵਿੱਥ,
		    ਪੂਰਨ           ਵਸਤੂ_ਲੰਮਾਈ)
{
  hb_ਬਫਰ_ਜੋੜੋ_utf<hb_utf8_t> (ਬਫਰ, (ਅਚਲ uint8_t *) ਪਾਠ, ਪਾਠ_ਲੰਮਾਈ, ਵਸਤੂ_ਵਿੱਥ, ਵਸਤੂ_ਲੰਮਾਈ);
}

/**
 * hb_buffer_add_utf16:
 * @buffer: An #hb_buffer_t
 * @text: (array length=text_length): An array of UTF-16 characters to append
 * @text_length: The length of the @text, or -1 if it is `NULL` terminated
 * @item_offset: The offset of the first character to add to the @buffer
 * @item_length: The number of characters to add to the @buffer, or -1 for the
 *               end of @text (assuming it is `NULL` terminated)
 *
 * See hb_buffer_add_codepoints().
 *
 * Replaces invalid UTF-16 characters with the @buffer replacement code point,
 * see hb_buffer_set_replacement_codepoint().
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਜੋੜੋ_utf16 (hb_ਬਫਰ_t    *ਬਫਰ,
		     ਅਚਲ uint16_t *ਪਾਠ,
		     ਪੂਰਨ             ਪਾਠ_ਲੰਮਾਈ,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਵਸਤੂ_ਵਿੱਥ,
		     ਪੂਰਨ             ਵਸਤੂ_ਲੰਮਾਈ)
{
  hb_ਬਫਰ_ਜੋੜੋ_utf<hb_utf16_t> (ਬਫਰ, ਪਾਠ, ਪਾਠ_ਲੰਮਾਈ, ਵਸਤੂ_ਵਿੱਥ, ਵਸਤੂ_ਲੰਮਾਈ);
}

/**
 * hb_buffer_add_utf32:
 * @buffer: An #hb_buffer_t
 * @text: (array length=text_length): An array of UTF-32 characters to append
 * @text_length: The length of the @text, or -1 if it is `NULL` terminated
 * @item_offset: The offset of the first character to add to the @buffer
 * @item_length: The number of characters to add to the @buffer, or -1 for the
 *               end of @text (assuming it is `NULL` terminated)
 *
 * See hb_buffer_add_codepoints().
 *
 * Replaces invalid UTF-32 characters with the @buffer replacement code point,
 * see hb_buffer_set_replacement_codepoint().
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਜੋੜੋ_utf32 (hb_ਬਫਰ_t    *ਬਫਰ,
		     ਅਚਲ uint32_t *ਪਾਠ,
		     ਪੂਰਨ             ਪਾਠ_ਲੰਮਾਈ,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਵਸਤੂ_ਵਿੱਥ,
		     ਪੂਰਨ             ਵਸਤੂ_ਲੰਮਾਈ)
{
  hb_ਬਫਰ_ਜੋੜੋ_utf<hb_utf32_t> (ਬਫਰ, ਪਾਠ, ਪਾਠ_ਲੰਮਾਈ, ਵਸਤੂ_ਵਿੱਥ, ਵਸਤੂ_ਲੰਮਾਈ);
}

/**
 * hb_buffer_add_latin1:
 * @buffer: An #hb_buffer_t
 * @text: (array length=text_length) (element-type uint8_t): an array of UTF-8
 *               characters to append
 * @text_length: the length of the @text, or -1 if it is `NULL` terminated
 * @item_offset: the offset of the first character to add to the @buffer
 * @item_length: the number of characters to add to the @buffer, or -1 for the
 *               end of @text (assuming it is `NULL` terminated)
 *
 * Similar to hb_buffer_add_codepoints(), but allows only access to first 256
 * Unicode code points that can fit in 8-bit strings.
 *
 * <note>Has nothing to do with non-Unicode Latin-1 encoding.</note>
 *
 * Since: 0.9.39
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਜੋੜੋ_latin1 (hb_ਬਫਰ_t   *ਬਫਰ,
		      ਅਚਲ uint8_t *ਪਾਠ,
		      ਪੂਰਨ            ਪਾਠ_ਲੰਮਾਈ,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   ਵਸਤੂ_ਵਿੱਥ,
		      ਪੂਰਨ            ਵਸਤੂ_ਲੰਮਾਈ)
{
  hb_ਬਫਰ_ਜੋੜੋ_utf<hb_latin1_t> (ਬਫਰ, ਪਾਠ, ਪਾਠ_ਲੰਮਾਈ, ਵਸਤੂ_ਵਿੱਥ, ਵਸਤੂ_ਲੰਮਾਈ);
}

/**
 * hb_buffer_add_codepoints:
 * @buffer: a #hb_buffer_t to append characters to.
 * @text: (array length=text_length): an array of Unicode code points to append.
 * @text_length: the length of the @text, or -1 if it is `NULL` terminated.
 * @item_offset: the offset of the first code point to add to the @buffer.
 * @item_length: the number of code points to add to the @buffer, or -1 for the
 *               end of @text (assuming it is `NULL` terminated).
 *
 * Appends characters from @text array to @buffer. The @item_offset is the
 * position of the first character from @text that will be appended, and
 * @item_length is the number of character. When shaping part of a larger text
 * (e.g. a run of text from a paragraph), instead of passing just the substring
 * corresponding to the run, it is preferable to pass the whole
 * paragraph and specify the run start and length as @item_offset and
 * @item_length, respectively, to give HarfBuzz the full context to be able,
 * for example, to do cross-run Arabic shaping or properly handle combining
 * marks at stat of run.
 *
 * This function does not check the validity of @text, it is up to the caller
 * to ensure it contains a valid Unicode scalar values.  In contrast,
 * hb_buffer_add_utf32() can be used that takes similar input but performs
 * sanity-check on the input.
 *
 * Since: 0.9.31
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਜੋੜੋ_codepoints (hb_ਬਫਰ_t          *ਬਫਰ,
			  ਅਚਲ hb_codepoint_t *ਪਾਠ,
			  ਪੂਰਨ                   ਪਾਠ_ਲੰਮਾਈ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          ਵਸਤੂ_ਵਿੱਥ,
			  ਪੂਰਨ                   ਵਸਤੂ_ਲੰਮਾਈ)
{
  hb_ਬਫਰ_ਜੋੜੋ_utf<hb_utf32_novalidate_t> (ਬਫਰ, ਪਾਠ, ਪਾਠ_ਲੰਮਾਈ, ਵਸਤੂ_ਵਿੱਥ, ਵਸਤੂ_ਲੰਮਾਈ);
}


/**
 * hb_buffer_append:
 * @buffer: An #hb_buffer_t
 * @source: source #hb_buffer_t
 * @start: start index into source buffer to copy.  Use 0 to copy from start of buffer.
 * @end: end index into source buffer to copy.  Use @UINT_MAX (or ((unsigned int) -1)) to copy to end of buffer.
 *
 * Append (part of) contents of another buffer to this buffer.
 *
 * Since: 1.5.0
 **/
HB_EXTERN ਖਾਲੀ
hb_ਬਫਰ_append (hb_ਬਫਰ_t *ਬਫਰ,
		  ਅਚਲ hb_ਬਫਰ_t *ਸਰੋਤ,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
{
  ਪ੍ਰਤਿਜ੍ਞਾ (!ਬਫਰ->have_output && !ਸਰੋਤ->have_output);
  ਪ੍ਰਤਿਜ੍ਞਾ (ਬਫਰ->have_positions == ਸਰੋਤ->have_positions ||
	  !ਬਫਰ->len || !ਸਰੋਤ->len);
  ਪ੍ਰਤਿਜ੍ਞਾ (ਬਫਰ->content_type == ਸਰੋਤ->content_type ||
	  !ਬਫਰ->len || !ਸਰੋਤ->len);

  ਜੇ (ਸਿਰਾ > ਸਰੋਤ->len)
    ਸਿਰਾ = ਸਰੋਤ->len;
  ਜੇ (ਸ਼ੁਰੂ > ਸਿਰਾ)
    ਸ਼ੁਰੂ = ਸਿਰਾ;
  ਜੇ (ਸ਼ੁਰੂ == ਸਿਰਾ)
    ਵਾਪਸ;

  ਜੇ (ਬਫਰ->len + (ਸਿਰਾ - ਸ਼ੁਰੂ) < ਬਫਰ->len) /* Overflows. */
  {
    ਬਫਰ->successful = ਝੂਠ;
    ਵਾਪਸ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ orig_len = ਬਫਰ->len;
  hb_ਬਫਰ_ਸੈੱਟ_ਲੰਮਾਈ (ਬਫਰ, ਬਫਰ->len + (ਸਿਰਾ - ਸ਼ੁਰੂ));
  ਜੇ (unlikely (!ਬਫਰ->successful))
    ਵਾਪਸ;

  ਜੇ (!orig_len)
    ਬਫਰ->content_type = ਸਰੋਤ->content_type;
  ਜੇ (!ਬਫਰ->have_positions && ਸਰੋਤ->have_positions)
    ਬਫਰ->clear_positions ();

  hb_segment_properties_overlay (&ਬਫਰ->props, &ਸਰੋਤ->props);

  hb_memcpy (ਬਫਰ->info + orig_len, ਸਰੋਤ->info + ਸ਼ੁਰੂ, (ਸਿਰਾ - ਸ਼ੁਰੂ) * ਆਕਾਰ (ਬਫਰ->info[0]));
  ਜੇ (ਬਫਰ->have_positions)
    hb_memcpy (ਬਫਰ->pos + orig_len, ਸਰੋਤ->pos + ਸ਼ੁਰੂ, (ਸਿਰਾ - ਸ਼ੁਰੂ) * ਆਕਾਰ (ਬਫਰ->pos[0]));

  ਜੇ (ਸਰੋਤ->content_type == HB_ਬਫਰ_CONTENT_TYPE_UNICODE)
  {
    /* See similar logic in add_utf. */

    /* pre-context */
    ਜੇ (!orig_len && ਸ਼ੁਰੂ + ਸਰੋਤ->context_len[0] > 0)
    {
      ਬਫਰ->clear_context (0);
      ਜਦੋਂ (ਸ਼ੁਰੂ > 0 && ਬਫਰ->context_len[0] < ਬਫਰ->CONTEXT_ਲੰਮਾਈ)
	ਬਫਰ->context[0][ਬਫਰ->context_len[0]++] = ਸਰੋਤ->info[--ਸ਼ੁਰੂ].codepoint;
      ਲਈ (ਸਵੈਚਾਲਿਤ i = 0u; i < ਸਰੋਤ->context_len[0] && ਬਫਰ->context_len[0] < ਬਫਰ->CONTEXT_ਲੰਮਾਈ; i++)
	ਬਫਰ->context[0][ਬਫਰ->context_len[0]++] = ਸਰੋਤ->context[0][i];
    }

    /* post-context */
    ਬਫਰ->clear_context (1);
    ਜਦੋਂ (ਸਿਰਾ < ਸਰੋਤ->len && ਬਫਰ->context_len[1] < ਬਫਰ->CONTEXT_ਲੰਮਾਈ)
      ਬਫਰ->context[1][ਬਫਰ->context_len[1]++] = ਸਰੋਤ->info[ਸਿਰਾ++].codepoint;
    ਲਈ (ਸਵੈਚਾਲਿਤ i = 0u; i < ਸਰੋਤ->context_len[1] && ਬਫਰ->context_len[1] < ਬਫਰ->CONTEXT_ਲੰਮਾਈ; i++)
      ਬਫਰ->context[1][ਬਫਰ->context_len[1]++] = ਸਰੋਤ->context[1][i];
  }
}


ਸਥਿਰ ਪੂਰਨ
compare_info_codepoint (ਅਚਲ hb_glyph_info_t *pa,
			ਅਚਲ hb_glyph_info_t *pb)
{
  ਵਾਪਸ (ਪੂਰਨ) pb->codepoint - (ਪੂਰਨ) pa->codepoint;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
normalize_glyphs_cluster (hb_ਬਫਰ_t *ਬਫਰ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ,
			  ਤਾਰਕਿਕ backward)
{
  hb_glyph_position_t *pos = ਬਫਰ->pos;

  /* Total cluster advance */
  hb_position_t ਕੁੱਲ_x_advance = 0, ਕੁੱਲ_y_advance = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
  {
    ਕੁੱਲ_x_advance = hb_saturate_ਜੋੜੋ (ਕੁੱਲ_x_advance, pos[i].x_advance);
    ਕੁੱਲ_y_advance = hb_saturate_ਜੋੜੋ (ਕੁੱਲ_y_advance, pos[i].y_advance);
  }

  hb_position_t x_advance = 0, y_advance = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
  {
    pos[i].x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[i].x_ਵਿੱਥ_2, x_advance);
    pos[i].y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[i].y_ਵਿੱਥ_2, y_advance);

    x_advance = hb_saturate_ਜੋੜੋ (x_advance, pos[i].x_advance);
    y_advance = hb_saturate_ਜੋੜੋ (y_advance, pos[i].y_advance);

    pos[i].x_advance = 0;
    pos[i].y_advance = 0;
  }

  ਜੇ (backward)
  {
    /* Transfer all cluster advance to the last glyph. */
    pos[ਸਿਰਾ - 1].x_advance = ਕੁੱਲ_x_advance;
    pos[ਸਿਰਾ - 1].y_advance = ਕੁੱਲ_y_advance;

    hb_stable_ਛਾਂਟੋ (ਬਫਰ->info + ਸ਼ੁਰੂ, ਸਿਰਾ - ਸ਼ੁਰੂ - 1, compare_info_codepoint, ਬਫਰ->pos + ਸ਼ੁਰੂ);
  } ਨਹੀਂਤਾਂ {
    /* Transfer all cluster advance to the first glyph. */
    pos[ਸ਼ੁਰੂ].x_advance = hb_saturate_ਜੋੜੋ (pos[ਸ਼ੁਰੂ].x_advance, ਕੁੱਲ_x_advance);
    pos[ਸ਼ੁਰੂ].y_advance = hb_saturate_ਜੋੜੋ (pos[ਸ਼ੁਰੂ].y_advance, ਕੁੱਲ_y_advance);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ + 1; i < ਸਿਰਾ; i++) {
      pos[i].x_ਵਿੱਥ_2 = hb_saturate_sub (pos[i].x_ਵਿੱਥ_2, ਕੁੱਲ_x_advance);
      pos[i].y_ਵਿੱਥ_2 = hb_saturate_sub (pos[i].y_ਵਿੱਥ_2, ਕੁੱਲ_y_advance);
    }
    hb_stable_ਛਾਂਟੋ (ਬਫਰ->info + ਸ਼ੁਰੂ + 1, ਸਿਰਾ - ਸ਼ੁਰੂ - 1, compare_info_codepoint, ਬਫਰ->pos + ਸ਼ੁਰੂ + 1);
  }
}

/**
 * hb_buffer_normalize_glyphs:
 * @buffer: An #hb_buffer_t
 *
 * Reorders a glyph buffer to have canonical in-cluster glyph order / position.
 * The resulting clusters should behave identical to pre-reordering clusters.
 *
 * <note>This has nothing to do with Unicode normalization.</note>
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_ਬਫਰ_normalize_glyphs (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਪ੍ਰਤਿਜ੍ਞਾ (ਬਫਰ->have_positions);

  ਬਫਰ->assert_glyphs ();

  ਤਾਰਕਿਕ backward = HB_DIRECTION_IS_BACKWARD (ਬਫਰ->props.direction);

  foreach_cluster (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ)
    normalize_glyphs_cluster (ਬਫਰ, ਸ਼ੁਰੂ, ਸਿਰਾ, backward);
}

ਖਾਲੀ
hb_ਬਫਰ_t::sort (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ, ਪੂਰਨ(*compar)(ਅਚਲ hb_glyph_info_t *, ਅਚਲ hb_glyph_info_t *))
{
  ਪ੍ਰਤਿਜ੍ਞਾ (!have_positions);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਸ਼ੁਰੂ + 1; i < ਸਿਰਾ; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = i;
    ਜਦੋਂ (j > ਸ਼ੁਰੂ && compar (&info[j - 1], &info[i]) > 0)
      j--;
    ਜੇ (i == j)
      ਜਾਰੀ;
    /* Move item i to occupy place for item j, shift what's in between. */
    ਮਿਲਾਓ_clusters (j, i + 1);
    {
      hb_glyph_info_t t = info[i];
      memmove (&info[j + 1], &info[j], (i - j) * ਆਕਾਰ (hb_glyph_info_t));
      info[j] = t;
    }
  }
}


/*
 * Comparing buffers.
 */

/**
 * hb_buffer_diff:
 * @buffer: a buffer.
 * @reference: other buffer to compare to.
 * @dottedcircle_glyph: glyph id of U+25CC DOTTED CIRCLE, or (hb_codepoint_t) -1.
 * @position_fuzz: allowed absolute difference in position values.
 *
 * If dottedcircle_glyph is (hb_codepoint_t) -1 then #HB_BUFFER_DIFF_FLAG_DOTTED_CIRCLE_PRESENT
 * and #HB_BUFFER_DIFF_FLAG_NOTDEF_PRESENT are never returned.  This should be used by most
 * callers if just comparing two buffers is needed.
 *
 * Since: 1.5.0
 **/
hb_ਬਫਰ_diff_flags_t
hb_ਬਫਰ_diff (hb_ਬਫਰ_t *ਬਫਰ,
		hb_ਬਫਰ_t *reference,
		hb_codepoint_t dottedcircle_glyph,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ position_fuzz)
{
  ਜੇ (ਬਫਰ->content_type != reference->content_type && ਬਫਰ->len && reference->len)
    ਵਾਪਸ HB_ਬਫਰ_DIFF_FLAG_CONTENT_TYPE_MISMATCH;

  hb_ਬਫਰ_diff_flags_t ਨਤੀਜਾ = HB_ਬਫਰ_DIFF_FLAG_EQUAL;
  ਤਾਰਕਿਕ ਸ਼ਾਮਲ_ਹੈ = dottedcircle_glyph != (hb_codepoint_t) -1;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = reference->len;

  ਜੇ (ਬਫਰ->len != ਗਿਣਤੀ_2)
  {
    /*
     * we can't compare glyph-by-glyph, but we do want to know if there
     * are .notdef or dottedcircle glyphs present in the reference buffer
     */
    ਅਚਲ hb_glyph_info_t *info = reference->info;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
    ਲਈ (i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (ਸ਼ਾਮਲ_ਹੈ && info[i].codepoint == dottedcircle_glyph)
	ਨਤੀਜਾ |= HB_ਬਫਰ_DIFF_FLAG_DOTTED_CIRCLE_PRESENT;
      ਜੇ (ਸ਼ਾਮਲ_ਹੈ && info[i].codepoint == 0)
	ਨਤੀਜਾ |= HB_ਬਫਰ_DIFF_FLAG_NOTDEF_PRESENT;
    }
    ਨਤੀਜਾ |= HB_ਬਫਰ_DIFF_FLAG_ਲੰਮਾਈ_MISMATCH;
    ਵਾਪਸ hb_ਬਫਰ_diff_flags_t (ਨਤੀਜਾ);
  }

  ਜੇ (!ਗਿਣਤੀ_2)
    ਵਾਪਸ hb_ਬਫਰ_diff_flags_t (ਨਤੀਜਾ);

  ਅਚਲ hb_glyph_info_t *buf_info = ਬਫਰ->info;
  ਅਚਲ hb_glyph_info_t *ref_info = reference->info;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਜੇ (buf_info->codepoint != ref_info->codepoint)
      ਨਤੀਜਾ |= HB_ਬਫਰ_DIFF_FLAG_CODEPOINT_MISMATCH;
    ਜੇ (buf_info->cluster != ref_info->cluster)
      ਨਤੀਜਾ |= HB_ਬਫਰ_DIFF_FLAG_CLUSTER_MISMATCH;
    ਜੇ ((buf_info->ਨਕਾਬ ^ ref_info->ਨਕਾਬ) & HB_GLYPH_FLAG_DEFINED)
      ਨਤੀਜਾ |= HB_ਬਫਰ_DIFF_FLAG_GLYPH_FLAGS_MISMATCH;
    ਜੇ (ਸ਼ਾਮਲ_ਹੈ && ref_info->codepoint == dottedcircle_glyph)
      ਨਤੀਜਾ |= HB_ਬਫਰ_DIFF_FLAG_DOTTED_CIRCLE_PRESENT;
    ਜੇ (ਸ਼ਾਮਲ_ਹੈ && ref_info->codepoint == 0)
      ਨਤੀਜਾ |= HB_ਬਫਰ_DIFF_FLAG_NOTDEF_PRESENT;
    buf_info++;
    ref_info++;
  }

  ਜੇ (ਬਫਰ->content_type == HB_ਬਫਰ_CONTENT_TYPE_GLYPHS)
  {
    ਸਵੈਚਾਲਿਤ position_diff = [] (hb_position_t a, hb_position_t b) -> uint64_t
    {
      int64_t diff = (int64_t) a - b;
      ਵਾਪਸ diff < 0 ? -diff : diff;
    };
    ਪ੍ਰਤਿਜ੍ਞਾ (ਬਫਰ->have_positions);
    ਅਚਲ hb_glyph_position_t *buf_pos = ਬਫਰ->pos;
    ਅਚਲ hb_glyph_position_t *ref_pos = reference->pos;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (position_diff (buf_pos->x_advance, ref_pos->x_advance) > position_fuzz ||
	  position_diff (buf_pos->y_advance, ref_pos->y_advance) > position_fuzz ||
	  position_diff (buf_pos->x_ਵਿੱਥ_2, ref_pos->x_ਵਿੱਥ_2) > position_fuzz ||
	  position_diff (buf_pos->y_ਵਿੱਥ_2, ref_pos->y_ਵਿੱਥ_2) > position_fuzz)
      {
	ਨਤੀਜਾ |= HB_ਬਫਰ_DIFF_FLAG_POSITION_MISMATCH;
	ਰੋਕੋ;
      }
      buf_pos++;
      ref_pos++;
    }
  }

  ਵਾਪਸ ਨਤੀਜਾ;
}


/*
 * Debugging.
 */

ਖਾਲੀ
hb_ਬਫਰ_t::ਬਦਲਿਆ ()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_ਸੁਨੇਹਾ
  ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
  ਜੇ (!ਸੁਨੇਹਾ_ਡੂੰਘਾਈ)
    ਵਾਪਸ;

  ਜੇ (ਬਦਲਿਆ_func)
    ਬਦਲਿਆ_func (ਇਹ, ਬਦਲਿਆ_ਅੰਕੜੇ);
  ਨਹੀਂਤਾਂ
    update_digest ();
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_ਬਫਰ_ਸੁਨੇਹਾ
/**
 * hb_buffer_set_message_func:
 * @buffer: An #hb_buffer_t
 * @func: (closure user_data) (destroy destroy) (scope notified): Callback function
 * @user_data: (nullable): Data to pass to @func
 * @destroy: (nullable): The function to call when @user_data is not needed anymore
 *
 * Sets the implementation function for #hb_buffer_message_func_t.
 * The callback and its associated data are preserved by hb_buffer_reset()
 * and hb_buffer_clear_contents(). The @destroy function is called when the
 * callback is replaced or removed, or when @buffer is destroyed.
 *
 * Since: 1.1.3
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਸੈੱਟ_ਸੁਨੇਹਾ_func (hb_ਬਫਰ_t *ਬਫਰ,
			    hb_ਬਫਰ_ਸੁਨੇਹਾ_func_t func,
			    ਖਾਲੀ *user_ਅੰਕੜੇ, hb_destroy_func_t destroy)
{
  ਜੇ (unlikely (hb_object_is_immutable (ਬਫਰ)) ||
      unlikely (ਬਫਰ->ਸੁਨੇਹਾ_ਡੂੰਘਾਈ))
  {
    ਜੇ (destroy)
      destroy (user_ਅੰਕੜੇ);
    ਵਾਪਸ;
  }

  ਜੇ (ਬਫਰ->ਸੁਨੇਹਾ_destroy)
    ਬਫਰ->ਸੁਨੇਹਾ_destroy (ਬਫਰ->ਸੁਨੇਹਾ_ਅੰਕੜੇ);

  ਜੇ (func) {
    ਬਫਰ->ਸੁਨੇਹਾ_func = func;
    ਬਫਰ->ਸੁਨੇਹਾ_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
    ਬਫਰ->ਸੁਨੇਹਾ_destroy = destroy;
  } ਨਹੀਂਤਾਂ {
    ਬਫਰ->ਸੁਨੇਹਾ_func = ਖਾਲੀ_ਸੂਚਕ;
    ਬਫਰ->ਸੁਨੇਹਾ_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
    ਬਫਰ->ਸੁਨੇਹਾ_destroy = ਖਾਲੀ_ਸੂਚਕ;
  }
}
/**
 * hb_buffer_changed:
 * @buffer: An #hb_buffer_t
 *
 * Called by a message callback after modifying buffer glyph indices,
 * to update internal caches.
 *
 * If not called from inside a message callback, does nothing.
 *
 * Since: 13.0.0
 **/
ਖਾਲੀ
hb_ਬਫਰ_ਬਦਲਿਆ (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਬਫਰ->ਬਦਲਿਆ ();
}

ਤਾਰਕਿਕ
hb_ਬਫਰ_t::ਸੁਨੇਹਾ_impl (hb_font_t *font, ਅਚਲ ਅੱਖਰ *fmt, va_list ap)
{
  ਪ੍ਰਤਿਜ੍ਞਾ (!have_output || (ਬਾਹਰਲਾ_info == info && ਬਾਹਰਲਾ_len == idx));

  ਸੁਨੇਹਾ_ਡੂੰਘਾਈ++;

  ਅੱਖਰ buf[100];
  vsnprintf (buf, ਆਕਾਰ (buf), fmt, ap);
  ਤਾਰਕਿਕ ret = (ਤਾਰਕਿਕ) ਇਹ->ਸੁਨੇਹਾ_func (ਇਹ, font, buf, ਇਹ->ਸੁਨੇਹਾ_ਅੰਕੜੇ);

  ਸੁਨੇਹਾ_ਡੂੰਘਾਈ--;

  ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ
