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

#ضمن "hb-buffer.hh"
#ضمن "hb-utf.hh"


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
hb_segment_properties_equal (ثابت hb_segment_properties_t *a,
			     ثابت hb_segment_properties_t *b)
{
  أرجع a->direction == b->direction &&
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
بنانشان صحيح
hb_segment_properties_hash (ثابت hb_segment_properties_t *p)
{
  أرجع ((بنانشان صحيح) p->direction * 31 +
	  (بنانشان صحيح) p->script) * 31 +
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
فراغ
hb_segment_properties_overlay (hb_segment_properties_t *p,
			       ثابت hb_segment_properties_t *src)
{
  إذا (unlikely (!p || !src))
    أرجع;

  إذا (!p->direction)
    p->direction = src->direction;

  إذا (p->direction != src->direction)
    أرجع;

  إذا (!p->script)
    p->script = src->script;

  إذا (p->script != src->script)
    أرجع;

  إذا (!p->language)
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

منطقي
hb_بفر_t::enlarge (بنانشان صحيح سائيز)
{
  إذا (unlikely (سائيز > max_len))
  {
    successful = خطأ;
    أرجع خطأ;
  }

  إذا (unlikely (!successful))
    أرجع خطأ;

  بنانشان صحيح new_allocated = allocated;
  hb_glyph_position_t *new_pos = خالي_اشارو;
  hb_glyph_info_t *new_info = خالي_اشارو;
  منطقي separate_ٻاهريون = ٻاهريون_info != info;

  إذا (unlikely (hb_unsigned_mul_overflows (سائيز, حجم (info[0]))))
    اذهب پورو;

  طالما (سائيز >= new_allocated)
    new_allocated += (new_allocated >> 1) + 32;

  بنانشان new_بائيٽون;
  إذا (unlikely (hb_unsigned_mul_overflows (new_allocated, حجم (info[0]), &new_بائيٽون)))
    اذهب پورو;

  static_assert (حجم (info[0]) == حجم (pos[0]), "");
  new_pos = (hb_glyph_position_t *) hb_realloc (pos, new_بائيٽون);
  new_info = (hb_glyph_info_t *) hb_realloc (info, new_بائيٽون);

پورو:
  إذا (unlikely (!new_pos || !new_info))
    successful = خطأ;

  إذا (likely (new_pos))
    pos = new_pos;

  إذا (likely (new_info))
    info = new_info;

  ٻاهريون_info = separate_ٻاهريون ? (hb_glyph_info_t *) pos : info;
  إذا (likely (successful))
    allocated = new_allocated;

  أرجع likely (successful);
}

منطقي
hb_بفر_t::make_گنجائش_for (بنانشان صحيح انگ_اندريون,
			    بنانشان صحيح انگ_ٻاهريون)
{
  إذا (unlikely (!ensure (ٻاهريون_len + انگ_ٻاهريون))) أرجع خطأ;

  إذا (ٻاهريون_info == info &&
      ٻاهريون_len + انگ_ٻاهريون > idx + انگ_اندريون)
  {
    أكد (have_output);

    ٻاهريون_info = (hb_glyph_info_t *) pos;
    hb_memcpy (ٻاهريون_info, info, ٻاهريون_len * حجم (ٻاهريون_info[0]));
  }

  أرجع سچ;
}

منطقي
hb_بفر_t::سرڪايو_forward (بنانشان صحيح ڳڻپ)
{
  أكد (have_output);
  إذا (unlikely (!ensure (len + ڳڻپ))) أرجع خطأ;

  max_ops -= len - idx;
  إذا (unlikely (max_ops < 0))
  {
    successful = خطأ;
    أرجع خطأ;
  }

  memmove (info + idx + ڳڻپ, info + idx, (len - idx) * حجم (info[0]));
  إذا (idx + ڳڻپ > len)
  {
    /* Under memory failure we might expose this area.  At least
     * clean it up.  Oh well...
     *
     * Ideally, we should at least set Default_Ignorable bits on
     * these, as well as consistent cluster values.  But the former
     * is layering violation... */
    hb_memset (info + len, 0, (idx + ڳڻپ - len) * حجم (info[0]));
  }
  len += ڳڻپ;
  idx += ڳڻپ;

  أرجع سچ;
}

hb_بفر_t::scratch_بفر_t *
hb_بفر_t::get_scratch_بفر (بنانشان صحيح *سائيز)
{
  have_output = خطأ;
  have_positions = خطأ;

  ٻاهريون_len = 0;
  ٻاهريون_info = info;

  أكد ((uintptr_t) pos % حجم (scratch_بفر_t) == 0);
  *سائيز = allocated * حجم (pos[0]) / حجم (scratch_بفر_t);
  أرجع (scratch_بفر_t *) (فراغ *) pos;
}



/* HarfBuzz-Internal API */

فراغ
hb_بفر_t::similar (ثابت hb_بفر_t &src)
{
  hb_unicode_funcs_destroy (unicode);
  unicode = hb_unicode_funcs_reference (src.unicode);
  flags = src.flags;
  cluster_level = src.cluster_level;
  replacement = src.replacement;
  invisible = src.invisible;
  not_مليو = src.not_مليو;
  not_مليو_variation_selector = src.not_مليو_variation_selector;
}

فراغ
hb_بفر_t::reset ()
{
  hb_unicode_funcs_destroy (unicode);
  unicode = hb_unicode_funcs_reference (hb_unicode_funcs_get_default ());
  flags = HB_بفر_FLAG_DEFAULT;
  cluster_level = HB_بفر_CLUSTER_LEVEL_DEFAULT;
  replacement = HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT;
  invisible = 0;
  not_مليو = 0;
  not_مليو_variation_selector = HB_CODEPOINT_INVALID;

  clear ();
}

فراغ
hb_بفر_t::clear ()
{
  content_type = HB_بفر_CONTENT_TYPE_INVALID;
  hb_segment_properties_t default_props = HB_SEGMENT_PROPERTIES_DEFAULT;
  props = default_props;

  successful = سچ;
  have_output = خطأ;
  have_positions = خطأ;

  idx = 0;
  len = 0;
  ٻاهريون_len = 0;
  ٻاهريون_info = info;

  hb_memset (context, 0, حجم context);
  hb_memset (context_len, 0, حجم context_len);

  deallocate_var_all ();
  serial = 0;
  random_state = 1;
  scratch_flags = HB_بفر_SCRATCH_FLAG_DEFAULT;
}

فراغ
hb_بفر_t::enter ()
{
  deallocate_var_all ();
  serial = 0;
  scratch_flags = HB_بفر_SCRATCH_FLAG_DEFAULT;
  بنانشان mul;
  إذا (likely (!hb_unsigned_mul_overflows (len, HB_بفر_MAX_LEN_ضرب_ڪندڙ, &mul)))
  {
    max_len = hb_max (mul, (بنانشان) HB_بفر_MAX_LEN_MIN);
  }
  إذا (likely (!hb_unsigned_mul_overflows (len, HB_بفر_MAX_OPS_ضرب_ڪندڙ, &mul)))
  {
    max_ops = hb_max (mul, (بنانشان) HB_بفر_MAX_OPS_MIN);
  }
}
فراغ
hb_بفر_t::leave ()
{
  max_len = HB_بفر_MAX_LEN_DEFAULT;
  max_ops = HB_بفر_MAX_OPS_DEFAULT;
  deallocate_var_all ();
  serial = 0;
}


فراغ
hb_بفر_t::جوڙيو (hb_codepoint_t  codepoint,
		  بنانشان صحيح    cluster)
{
  hb_glyph_info_t *glyph;

  إذا (unlikely (!ensure (len + 1))) أرجع;

  glyph = &info[len];

  hb_memset (glyph, 0, حجم (*glyph));
  glyph->codepoint = codepoint;
  glyph->نقاب = 0;
  glyph->cluster = cluster;

  len++;
}

فراغ
hb_بفر_t::جوڙيو_info (ثابت hb_glyph_info_t &glyph_info)
{
  إذا (unlikely (!ensure (len + 1))) أرجع;

  info[len] = glyph_info;

  len++;
}
فراغ
hb_بفر_t::جوڙيو_info_and_pos (ثابت hb_glyph_info_t &glyph_info,
			       ثابت hb_glyph_position_t &glyph_pos)
{
  إذا (unlikely (!ensure (len + 1))) أرجع;

  info[len] = glyph_info;
  أكد (have_positions);
  pos[len] = glyph_pos;

  len++;
}


فراغ
hb_بفر_t::clear_output ()
{
  have_output = سچ;
  have_positions = خطأ;

  idx = 0;
  ٻاهريون_len = 0;
  ٻاهريون_info = info;
}

فراغ
hb_بفر_t::clear_positions ()
{
  have_output = خطأ;
  have_positions = سچ;

  ٻاهريون_len = 0;
  ٻاهريون_info = info;

  hb_memset (pos, 0, حجم (pos[0]) * len);
}

منطقي
hb_بفر_t::متزامن ()
{
  منطقي ret = خطأ;

  أكد (have_output);

  أكد (idx <= len);

  إذا (unlikely (!successful || !ايندڙ_glyphs (len - idx)))
    اذهب reset;

  إذا (ٻاهريون_info != info)
  {
    pos = (hb_glyph_position_t *) info;
    info = ٻاهريون_info;
  }
  len = ٻاهريون_len;
  ret = سچ;

reset:
  have_output = خطأ;
  ٻاهريون_len = 0;
  ٻاهريون_info = info;
  idx = 0;

  أرجع ret;
}

صحيح
hb_بفر_t::sync_so_far ()
{
  منطقي had_output = have_output;
  بنانشان ٻاهريون_i = ٻاهريون_len;
  بنانشان i = idx;
  بنانشان old_idx = idx;

  إذا (متزامن ())
    idx = ٻاهريون_i;
  وإلا
    idx = i;

  إذا (had_output)
  {
    have_output = سچ;
    ٻاهريون_len = idx;
  }

  أكد (idx <= len);

  أرجع idx - old_idx;
}

منطقي
hb_بفر_t::move_منزل (بنانشان صحيح i)
{
  إذا (!have_output)
  {
    أكد (i <= len);
    idx = i;
    أرجع سچ;
  }
  إذا (unlikely (!successful))
    أرجع خطأ;

  أكد (i <= ٻاهريون_len + (len - idx));

  إذا (ٻاهريون_len < i)
  {
    بنانشان صحيح ڳڻپ = i - ٻاهريون_len;
    إذا (unlikely (!make_گنجائش_for (ڳڻپ, ڳڻپ))) أرجع خطأ;

    max_ops -= ڳڻپ;
    إذا (unlikely (max_ops < 0))
    {
      successful = خطأ;
      أرجع خطأ;
    }

    memmove (ٻاهريون_info + ٻاهريون_len, info + idx, ڳڻپ * حجم (ٻاهريون_info[0]));
    idx += ڳڻپ;
    ٻاهريون_len += ڳڻپ;
  }
  وإلا إذا (ٻاهريون_len > i)
  {
    /* Tricky part: rewinding... */
    بنانشان صحيح ڳڻپ = ٻاهريون_len - i;

    /* This will blow in our face if memory allocation fails later
     * in this same lookup...
     *
     * We used to shift with extra 32 items.
     * But that would leave empty slots in the buffer in case of allocation
     * failures.  See comments in shift_forward().  This can cause O(N^2)
     * behavior more severely than adding 32 empty slots can... */
    إذا (unlikely (idx < ڳڻپ && !سرڪايو_forward (ڳڻپ - idx))) أرجع خطأ;

    أكد (idx >= ڳڻپ);

    max_ops -= ڳڻپ;
    إذا (unlikely (max_ops < 0))
    {
      successful = خطأ;
      أرجع خطأ;
    }

    idx -= ڳڻپ;
    ٻاهريون_len -= ڳڻپ;
    memmove (info + idx, ٻاهريون_info + ٻاهريون_len, ڳڻپ * حجم (ٻاهريون_info[0]));
  }

  أرجع سچ;
}


فراغ
hb_بفر_t::سيٽ_masks (hb_نقاب_t    value,
			hb_نقاب_t    نقاب,
			بنانشان صحيح cluster_شروعات,
			بنانشان صحيح cluster_پڇاڙي)
{
  إذا (!نقاب)
    أرجع;

  hb_نقاب_t not_نقاب = ~نقاب;
  value &= نقاب;

  max_ops -= len;
  إذا (unlikely (max_ops < 0))
    successful = خطأ;

  بنانشان صحيح ڳڻپ = len;

  إذا (cluster_شروعات == 0 && cluster_پڇاڙي == (بنانشان صحيح) -1)
  {
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      info[i].نقاب = (info[i].نقاب & not_نقاب) | value;
    أرجع;
  }

  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    إذا (cluster_شروعات <= info[i].cluster && info[i].cluster < cluster_پڇاڙي)
      info[i].نقاب = (info[i].نقاب & not_نقاب) | value;
}

فراغ
hb_بفر_t::ملايو_clusters_impl (بنانشان صحيح شروعات,
				  بنانشان صحيح پڇاڙي)
{
  max_ops -= پڇاڙي - شروعات;
  إذا (unlikely (max_ops < 0))
    successful = خطأ;

  بنانشان صحيح cluster = info[شروعات].cluster;

  لكل (بنانشان صحيح i = شروعات + 1; i < پڇاڙي; i++)
    cluster = hb_min (cluster, info[i].cluster);

  /* Extend end */
  إذا (cluster != info[پڇاڙي - 1].cluster)
    طالما (پڇاڙي < len && info[پڇاڙي - 1].cluster == info[پڇاڙي].cluster)
      پڇاڙي++;

  /* Extend start */
  إذا (cluster != info[شروعات].cluster)
    طالما (idx < شروعات && info[شروعات - 1].cluster == info[شروعات].cluster)
      شروعات--;

  /* If we hit the start of buffer, continue in out-buffer. */
  إذا (idx == شروعات && info[شروعات].cluster != cluster)
    لكل (بنانشان صحيح i = ٻاهريون_len; i && ٻاهريون_info[i - 1].cluster == info[شروعات].cluster; i--)
      سيٽ_cluster (ٻاهريون_info[i - 1], cluster);

  لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
    سيٽ_cluster (info[i], cluster);
}
فراغ
hb_بفر_t::ملايو_ٻاهريون_clusters_impl (بنانشان صحيح شروعات,
				      بنانشان صحيح پڇاڙي)
{
  max_ops -= پڇاڙي - شروعات;
  إذا (unlikely (max_ops < 0))
    successful = خطأ;

  بنانشان صحيح cluster = ٻاهريون_info[شروعات].cluster;

  لكل (بنانشان صحيح i = شروعات + 1; i < پڇاڙي; i++)
    cluster = hb_min (cluster, ٻاهريون_info[i].cluster);

  /* Extend start */
  طالما (شروعات && ٻاهريون_info[شروعات - 1].cluster == ٻاهريون_info[شروعات].cluster)
    شروعات--;

  /* Extend end */
  طالما (پڇاڙي < ٻاهريون_len && ٻاهريون_info[پڇاڙي - 1].cluster == ٻاهريون_info[پڇاڙي].cluster)
    پڇاڙي++;

  /* If we hit the end of out-buffer, continue in buffer. */
  إذا (پڇاڙي == ٻاهريون_len)
    لكل (بنانشان صحيح i = idx; i < len && info[i].cluster == ٻاهريون_info[پڇاڙي - 1].cluster; i++)
      سيٽ_cluster (info[i], cluster);

  لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
    سيٽ_cluster (ٻاهريون_info[i], cluster);
}
فراغ
hb_بفر_t::delete_glyph ()
{
  /* The logic here is duplicated in hb_ot_hide_default_ignorables(). */

  بنانشان صحيح cluster = info[idx].cluster;
  إذا ((idx + 1 < len && cluster == info[idx + 1].cluster) ||
      (ٻاهريون_len && cluster == ٻاهريون_info[ٻاهريون_len - 1].cluster))
  {
    /* Cluster survives; do nothing. */
    اذهب پورو;
  }

  إذا (ٻاهريون_len)
  {
    /* Merge cluster backward. */
    إذا (cluster < ٻاهريون_info[ٻاهريون_len - 1].cluster)
    {
      بنانشان صحيح نقاب = info[idx].نقاب;
      بنانشان صحيح old_cluster = ٻاهريون_info[ٻاهريون_len - 1].cluster;
      لكل (بنانشان i = ٻاهريون_len; i && ٻاهريون_info[i - 1].cluster == old_cluster; i--)
	سيٽ_cluster (ٻاهريون_info[i - 1], cluster, نقاب);
    }
    اذهب پورو;
  }

  إذا (idx + 1 < len)
  {
    /* Merge cluster forward. */
    ملايو_clusters (idx, idx + 2);
    اذهب پورو;
  }

پورو:
  skip_glyph ();
}

فراغ
hb_بفر_t::delete_glyphs_inplace (منطقي (*filter) (ثابت hb_glyph_info_t *info))
{
  /* Merge clusters and delete filtered glyphs.
   * NOTE! We can't use out-buffer as we have positioning data. */
  بنانشان صحيح j = 0;
  بنانشان صحيح ڳڻپ = len;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
  {
    إذا (filter (&info[i]))
    {
      /* Merge clusters.
       * Same logic as delete_glyph(), but for in-place removal. */

      بنانشان صحيح cluster = info[i].cluster;
      إذا (i + 1 < ڳڻپ && cluster == info[i + 1].cluster)
	استمر; /* Cluster survives; do nothing. */

      إذا (j)
      {
	/* Merge cluster backward. */
	إذا (cluster < info[j - 1].cluster)
	{
	  بنانشان صحيح نقاب = info[i].نقاب;
	  بنانشان صحيح old_cluster = info[j - 1].cluster;
	  لكل (بنانشان k = j; k && info[k - 1].cluster == old_cluster; k--)
	    سيٽ_cluster (info[k - 1], cluster, نقاب);
	}
	استمر;
      }

      إذا (i + 1 < ڳڻپ)
	ملايو_clusters (i, i + 2); /* Merge cluster forward. */

      استمر;
    }

    إذا (j != i)
    {
      info[j] = info[i];
      pos[j] = pos[i];
    }
    j++;
  }
  len = j;
}

فراغ
hb_بفر_t::guess_segment_properties ()
{
  assert_unicode ();

  /* If script is set to INVALID, guess from buffer contents */
  إذا (props.script == HB_SCRIPT_INVALID) {
    لكل (بنانشان صحيح i = 0; i < len; i++) {
      hb_script_t script = unicode->script (info[i].codepoint);
      إذا (likely (script != HB_SCRIPT_COMMON &&
		  script != HB_SCRIPT_INHERITED &&
		  script != HB_SCRIPT_UNKNOWN)) {
	props.script = script;
	اكسر;
      }
    }
  }

  /* If direction is set to INVALID, guess from script */
  إذا (props.direction == HB_DIRECTION_INVALID) {
    props.direction = hb_script_get_horizontal_direction (props.script);
    إذا (props.direction == HB_DIRECTION_INVALID)
      props.direction = HB_DIRECTION_LTR;
  }

  /* If language is not set, use default language from locale */
  إذا (props.language == HB_LANGUAGE_INVALID) {
    /* TODO get_default_for_script? using $LANGUAGE */
    props.language = hb_language_get_default ();
  }
}


/* Public API */

DEFINE_NULL_INSTANCE (hb_بفر_t) =
{
  HB_OBJECT_سرو_STATIC,

  const_cast<hb_unicode_funcs_t *> (&_hb_Null_hb_unicode_funcs_t),
  HB_بفر_FLAG_DEFAULT,
  HB_بفر_CLUSTER_LEVEL_DEFAULT,
  HB_بفر_REPLACEMENT_CODEPOINT_DEFAULT,
  0, /* invisible */
  0, /* not_found */
  HB_CODEPOINT_INVALID, /* not_found_variation_selector */


  HB_بفر_CONTENT_TYPE_INVALID,
  HB_SEGMENT_PROPERTIES_DEFAULT,

  خطأ, /* successful */
  خطأ, /* have_output */
  سچ  /* have_positions */

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
hb_بفر_t *
hb_بفر_create ()
{
  hb_بفر_t *بفر;

  إذا (!(بفر = hb_object_create<hb_بفر_t> ()))
    أرجع hb_بفر_get_خالي_جايون ();

  بفر->max_len = HB_بفر_MAX_LEN_DEFAULT;
  بفر->max_ops = HB_بفر_MAX_OPS_DEFAULT;

  بفر->reset ();

  أرجع بفر;
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
hb_بفر_t *
hb_بفر_create_similar (ثابت hb_بفر_t *src)
{
  hb_بفر_t *بفر = hb_بفر_create ();

  بفر->similar (*src);

  أرجع بفر;
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
فراغ
hb_بفر_reset (hb_بفر_t *بفر)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->reset ();
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
hb_بفر_t *
hb_بفر_get_خالي_جايون ()
{
  أرجع const_cast<hb_بفر_t *> (&Null (hb_بفر_t));
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
hb_بفر_t *
hb_بفر_reference (hb_بفر_t *بفر)
{
  أرجع hb_object_reference (بفر);
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
فراغ
hb_بفر_destroy (hb_بفر_t *بفر)
{
  إذا (!hb_object_destroy (بفر)) أرجع;

  hb_unicode_funcs_destroy (بفر->unicode);

  hb_free (بفر->info);
  hb_free (بفر->pos);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_بفر_پيغام
  إذا (بفر->پيغام_destroy)
    بفر->پيغام_destroy (بفر->پيغام_معلومات);
#جيڪڏهن_پورو

  hb_free (بفر);
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
hb_بفر_سيٽ_user_معلومات (hb_بفر_t        *بفر,
			 hb_user_معلومات_ڪنجي_t *key,
			 فراغ *              معلومات,
			 hb_destroy_func_t   destroy,
			 hb_bool_t           replace)
{
  أرجع hb_object_سيٽ_user_معلومات (بفر, key, معلومات, destroy, replace);
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
فراغ *
hb_بفر_get_user_معلومات (ثابت hb_بفر_t  *بفر,
			 hb_user_معلومات_ڪنجي_t *key)
{
  أرجع hb_object_get_user_معلومات (بفر, key);
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
فراغ
hb_بفر_سيٽ_content_type (hb_بفر_t              *بفر,
			    hb_بفر_content_type_t  content_type)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->content_type = content_type;
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
hb_بفر_content_type_t
hb_بفر_get_content_type (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->content_type;
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
فراغ
hb_بفر_سيٽ_unicode_funcs (hb_بفر_t        *بفر,
			     hb_unicode_funcs_t *unicode_funcs)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  إذا (!unicode_funcs)
    unicode_funcs = hb_unicode_funcs_get_default ();

  hb_unicode_funcs_reference (unicode_funcs);
  hb_unicode_funcs_destroy (بفر->unicode);
  بفر->unicode = unicode_funcs;
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
hb_بفر_get_unicode_funcs (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->unicode;
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
فراغ
hb_بفر_سيٽ_direction (hb_بفر_t    *بفر,
			 hb_direction_t  direction)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->props.direction = direction;
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
hb_بفر_get_direction (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->props.direction;
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
فراغ
hb_بفر_سيٽ_script (hb_بفر_t *بفر,
		      hb_script_t  script)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->props.script = script;
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
hb_بفر_get_script (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->props.script;
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
فراغ
hb_بفر_سيٽ_language (hb_بفر_t   *بفر,
			hb_language_t  language)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->props.language = language;
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
hb_بفر_get_language (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->props.language;
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
فراغ
hb_بفر_سيٽ_segment_properties (hb_بفر_t *بفر,
				  ثابت hb_segment_properties_t *props)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->props = *props;
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
فراغ
hb_بفر_get_segment_properties (ثابت hb_بفر_t *بفر,
				  hb_segment_properties_t *props)
{
  *props = بفر->props;
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
فراغ
hb_بفر_سيٽ_flags (hb_بفر_t       *بفر,
		     hb_بفر_flags_t  flags)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->flags = flags;
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
hb_بفر_flags_t
hb_بفر_get_flags (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->flags;
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
فراغ
hb_بفر_سيٽ_cluster_level (hb_بفر_t               *بفر,
			     hb_بفر_cluster_level_t  cluster_level)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->cluster_level = cluster_level;
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
hb_بفر_cluster_level_t
hb_بفر_get_cluster_level (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->cluster_level;
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
فراغ
hb_بفر_سيٽ_replacement_codepoint (hb_بفر_t    *بفر,
				     hb_codepoint_t  replacement)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->replacement = replacement;
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
hb_بفر_get_replacement_codepoint (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->replacement;
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
فراغ
hb_بفر_سيٽ_invisible_glyph (hb_بفر_t    *بفر,
			       hb_codepoint_t  invisible)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->invisible = invisible;
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
hb_بفر_get_invisible_glyph (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->invisible;
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
فراغ
hb_بفر_سيٽ_not_مليو_glyph (hb_بفر_t    *بفر,
			       hb_codepoint_t  not_مليو)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->not_مليو = not_مليو;
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
hb_بفر_get_not_مليو_glyph (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->not_مليو;
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
فراغ
hb_بفر_سيٽ_not_مليو_variation_selector_glyph (hb_بفر_t    *بفر,
						  hb_codepoint_t  not_مليو_variation_selector)
{
  بفر->not_مليو_variation_selector = not_مليو_variation_selector;
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
hb_بفر_get_not_مليو_variation_selector_glyph (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->not_مليو_variation_selector;
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
فراغ
hb_بفر_سيٽ_random_state (hb_بفر_t    *بفر,
			    بنانشان        state)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->random_state = state;
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
بنانشان
hb_بفر_get_random_state (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->random_state;
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
فراغ
hb_بفر_clear_contents (hb_بفر_t *بفر)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  بفر->clear ();
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
hb_بفر_pre_allocate (hb_بفر_t *بفر, بنانشان صحيح سائيز)
{
  أرجع بفر->ensure (سائيز);
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
hb_بفر_ورهاست_successful (hb_بفر_t  *بفر)
{
  أرجع بفر->successful;
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
فراغ
hb_بفر_جوڙيو (hb_بفر_t    *بفر,
	       hb_codepoint_t  codepoint,
	       بنانشان صحيح    cluster)
{
  بفر->جوڙيو (codepoint, cluster);
  بفر->clear_context (1);
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
hb_بفر_سيٽ_ڊيگهائي (hb_بفر_t  *بفر,
		      بنانشان صحيح  ڊيگهائي)
{
  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع ڊيگهائي == 0;

  إذا (unlikely (!بفر->ensure (ڊيگهائي)))
    أرجع خطأ;

  /* Wipe the new space */
  إذا (ڊيگهائي > بفر->len) {
    hb_memset (بفر->info + بفر->len, 0, حجم (بفر->info[0]) * (ڊيگهائي - بفر->len));
    إذا (بفر->have_positions)
      hb_memset (بفر->pos + بفر->len, 0, حجم (بفر->pos[0]) * (ڊيگهائي - بفر->len));
  }

  بفر->len = ڊيگهائي;

  إذا (!ڊيگهائي)
  {
    بفر->content_type = HB_بفر_CONTENT_TYPE_INVALID;
    بفر->clear_context (0);
  }
  بفر->clear_context (1);

  أرجع سچ;
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
بنانشان صحيح
hb_بفر_get_ڊيگهائي (ثابت hb_بفر_t *بفر)
{
  أرجع بفر->len;
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
hb_بفر_get_glyph_infos (hb_بفر_t  *بفر,
			   بنانشان صحيح *ڊيگهائي)
{
  إذا (ڊيگهائي)
    *ڊيگهائي = بفر->len;

  أرجع (hb_glyph_info_t *) بفر->info;
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
hb_بفر_get_glyph_positions (hb_بفر_t  *بفر,
			       بنانشان صحيح *ڊيگهائي)
{
  إذا (ڊيگهائي)
    *ڊيگهائي = بفر->len;

  إذا (!بفر->have_positions)
  {
    إذا (unlikely (بفر->پيغام_اونهائي))
      أرجع خالي_اشارو;

    بفر->clear_positions ();
  }

  أرجع (hb_glyph_position_t *) بفر->pos;
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
hb_بفر_has_positions (hb_بفر_t  *بفر)
{
  أرجع بفر->have_positions;
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
(hb_glyph_info_get_glyph_flags) (ثابت hb_glyph_info_t *info)
{
  أرجع hb_glyph_info_get_glyph_flags (info);
}

/**
 * hb_buffer_reverse:
 * @buffer: An #hb_buffer_t
 *
 * Reverses buffer contents.
 *
 * Since: 0.9.2
 **/
فراغ
hb_بفر_ابتو (hb_بفر_t *بفر)
{
  بفر->reverse ();
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
فراغ
hb_بفر_ابتو_range (hb_بفر_t *بفر,
			 بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي)
{
  بفر->ابتو_range (شروعات, پڇاڙي);
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
فراغ
hb_بفر_ابتو_clusters (hb_بفر_t *بفر)
{
  بفر->ابتو_clusters ();
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
فراغ
hb_بفر_guess_segment_properties (hb_بفر_t *بفر)
{
  بفر->guess_segment_properties ();
}

سانچو <قسم_نالو utf_t>
ساكن مضمن فراغ
hb_بفر_جوڙيو_utf (hb_بفر_t  *بفر,
		   ثابت قسم_نالو utf_t::codepoint_t *متن,
		   صحيح           متن_ڊيگهائي,
		   بنانشان صحيح  شي_وٿي,
		   صحيح           شي_ڊيگهائي)
{
  تعريفنوع قسم_نالو utf_t::codepoint_t T;
  ثابت hb_codepoint_t replacement = بفر->replacement;

  بفر->assert_unicode ();

  إذا (unlikely (hb_object_is_immutable (بفر)))
    أرجع;

  إذا (متن_ڊيگهائي == -1)
    متن_ڊيگهائي = utf_t::لڙي_ڊيگهه (متن);

  إذا (شي_ڊيگهائي == -1)
    شي_ڊيگهائي = متن_ڊيگهائي - شي_وٿي;

  شي_وٿي = hb_min (شي_وٿي, (بنانشان) متن_ڊيگهائي);
  شي_ڊيگهائي = hb_clamp (شي_ڊيگهائي, 0, متن_ڊيگهائي - (صحيح) شي_وٿي);

  إذا (unlikely (شي_ڊيگهائي < 0 ||
		شي_ڊيگهائي > وڌ_کان_وڌ_عدد / 8 ||
		!بفر->ensure (بفر->len + شي_ڊيگهائي * حجم (T) / 4)))
    أرجع;

  /* If buffer is empty and pre-context provided, install it.
   * This check is written this way, to make sure people can
   * provide pre-context in one add_utf() call, then provide
   * text in a follow-up call.  See:
   *
   * https://bugzilla.mozilla.org/show_bug.cgi?id=801410#c13
   */
  إذا (!بفر->len && شي_وٿي > 0)
  {
    /* Add pre-context */
    بفر->clear_context (0);
    ثابت T *prev = متن + شي_وٿي;
    ثابت T *شروعات = متن;
    طالما (شروعات < prev && بفر->context_len[0] < بفر->CONTEXT_ڊيگهائي)
    {
      hb_codepoint_t u;
      prev = utf_t::prev (prev, شروعات, &u, replacement);
      بفر->context[0][بفر->context_len[0]++] = u;
    }
  }

  ثابت T *next = متن + شي_وٿي;
  ثابت T *پڇاڙي = next + شي_ڊيگهائي;
  طالما (next < پڇاڙي)
  {
    hb_codepoint_t u;
    ثابت T *old_ايندڙ = next;
    next = utf_t::next (next, پڇاڙي, &u, replacement);
    بفر->جوڙيو (u, old_ايندڙ - (ثابت T *) متن);
  }

  /* Add post-context */
  بفر->clear_context (1);
  پڇاڙي = متن + متن_ڊيگهائي;
  طالما (next < پڇاڙي && بفر->context_len[1] < بفر->CONTEXT_ڊيگهائي)
  {
    hb_codepoint_t u;
    next = utf_t::next (next, پڇاڙي, &u, replacement);
    بفر->context[1][بفر->context_len[1]++] = u;
  }

  بفر->content_type = HB_بفر_CONTENT_TYPE_UNICODE;
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
فراغ
hb_بفر_جوڙيو_utf8 (hb_بفر_t  *بفر,
		    ثابت حرف   *متن,
		    صحيح           متن_ڊيگهائي,
		    بنانشان صحيح  شي_وٿي,
		    صحيح           شي_ڊيگهائي)
{
  hb_بفر_جوڙيو_utf<hb_utf8_t> (بفر, (ثابت uint8_t *) متن, متن_ڊيگهائي, شي_وٿي, شي_ڊيگهائي);
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
فراغ
hb_بفر_جوڙيو_utf16 (hb_بفر_t    *بفر,
		     ثابت uint16_t *متن,
		     صحيح             متن_ڊيگهائي,
		     بنانشان صحيح    شي_وٿي,
		     صحيح             شي_ڊيگهائي)
{
  hb_بفر_جوڙيو_utf<hb_utf16_t> (بفر, متن, متن_ڊيگهائي, شي_وٿي, شي_ڊيگهائي);
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
فراغ
hb_بفر_جوڙيو_utf32 (hb_بفر_t    *بفر,
		     ثابت uint32_t *متن,
		     صحيح             متن_ڊيگهائي,
		     بنانشان صحيح    شي_وٿي,
		     صحيح             شي_ڊيگهائي)
{
  hb_بفر_جوڙيو_utf<hb_utf32_t> (بفر, متن, متن_ڊيگهائي, شي_وٿي, شي_ڊيگهائي);
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
فراغ
hb_بفر_جوڙيو_latin1 (hb_بفر_t   *بفر,
		      ثابت uint8_t *متن,
		      صحيح            متن_ڊيگهائي,
		      بنانشان صحيح   شي_وٿي,
		      صحيح            شي_ڊيگهائي)
{
  hb_بفر_جوڙيو_utf<hb_latin1_t> (بفر, متن, متن_ڊيگهائي, شي_وٿي, شي_ڊيگهائي);
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
فراغ
hb_بفر_جوڙيو_codepoints (hb_بفر_t          *بفر,
			  ثابت hb_codepoint_t *متن,
			  صحيح                   متن_ڊيگهائي,
			  بنانشان صحيح          شي_وٿي,
			  صحيح                   شي_ڊيگهائي)
{
  hb_بفر_جوڙيو_utf<hb_utf32_novalidate_t> (بفر, متن, متن_ڊيگهائي, شي_وٿي, شي_ڊيگهائي);
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
HB_EXTERN فراغ
hb_بفر_append (hb_بفر_t *بفر,
		  ثابت hb_بفر_t *ذريعو,
		  بنانشان صحيح شروعات,
		  بنانشان صحيح پڇاڙي)
{
  أكد (!بفر->have_output && !ذريعو->have_output);
  أكد (بفر->have_positions == ذريعو->have_positions ||
	  !بفر->len || !ذريعو->len);
  أكد (بفر->content_type == ذريعو->content_type ||
	  !بفر->len || !ذريعو->len);

  إذا (پڇاڙي > ذريعو->len)
    پڇاڙي = ذريعو->len;
  إذا (شروعات > پڇاڙي)
    شروعات = پڇاڙي;
  إذا (شروعات == پڇاڙي)
    أرجع;

  إذا (بفر->len + (پڇاڙي - شروعات) < بفر->len) /* Overflows. */
  {
    بفر->successful = خطأ;
    أرجع;
  }

  بنانشان صحيح orig_len = بفر->len;
  hb_بفر_سيٽ_ڊيگهائي (بفر, بفر->len + (پڇاڙي - شروعات));
  إذا (unlikely (!بفر->successful))
    أرجع;

  إذا (!orig_len)
    بفر->content_type = ذريعو->content_type;
  إذا (!بفر->have_positions && ذريعو->have_positions)
    بفر->clear_positions ();

  hb_segment_properties_overlay (&بفر->props, &ذريعو->props);

  hb_memcpy (بفر->info + orig_len, ذريعو->info + شروعات, (پڇاڙي - شروعات) * حجم (بفر->info[0]));
  إذا (بفر->have_positions)
    hb_memcpy (بفر->pos + orig_len, ذريعو->pos + شروعات, (پڇاڙي - شروعات) * حجم (بفر->pos[0]));

  إذا (ذريعو->content_type == HB_بفر_CONTENT_TYPE_UNICODE)
  {
    /* See similar logic in add_utf. */

    /* pre-context */
    إذا (!orig_len && شروعات + ذريعو->context_len[0] > 0)
    {
      بفر->clear_context (0);
      طالما (شروعات > 0 && بفر->context_len[0] < بفر->CONTEXT_ڊيگهائي)
	بفر->context[0][بفر->context_len[0]++] = ذريعو->info[--شروعات].codepoint;
      لكل (خودڪار i = 0u; i < ذريعو->context_len[0] && بفر->context_len[0] < بفر->CONTEXT_ڊيگهائي; i++)
	بفر->context[0][بفر->context_len[0]++] = ذريعو->context[0][i];
    }

    /* post-context */
    بفر->clear_context (1);
    طالما (پڇاڙي < ذريعو->len && بفر->context_len[1] < بفر->CONTEXT_ڊيگهائي)
      بفر->context[1][بفر->context_len[1]++] = ذريعو->info[پڇاڙي++].codepoint;
    لكل (خودڪار i = 0u; i < ذريعو->context_len[1] && بفر->context_len[1] < بفر->CONTEXT_ڊيگهائي; i++)
      بفر->context[1][بفر->context_len[1]++] = ذريعو->context[1][i];
  }
}


ساكن صحيح
compare_info_codepoint (ثابت hb_glyph_info_t *pa,
			ثابت hb_glyph_info_t *pb)
{
  أرجع (صحيح) pb->codepoint - (صحيح) pa->codepoint;
}

ساكن مضمن فراغ
normalize_glyphs_cluster (hb_بفر_t *بفر,
			  بنانشان صحيح شروعات,
			  بنانشان صحيح پڇاڙي,
			  منطقي backward)
{
  hb_glyph_position_t *pos = بفر->pos;

  /* Total cluster advance */
  hb_position_t ڪل_x_advance = 0, ڪل_y_advance = 0;
  لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
  {
    ڪل_x_advance = hb_saturate_جوڙيو (ڪل_x_advance, pos[i].x_advance);
    ڪل_y_advance = hb_saturate_جوڙيو (ڪل_y_advance, pos[i].y_advance);
  }

  hb_position_t x_advance = 0, y_advance = 0;
  لكل (بنانشان صحيح i = شروعات; i < پڇاڙي; i++)
  {
    pos[i].x_وٿي_2 = hb_saturate_جوڙيو (pos[i].x_وٿي_2, x_advance);
    pos[i].y_وٿي_2 = hb_saturate_جوڙيو (pos[i].y_وٿي_2, y_advance);

    x_advance = hb_saturate_جوڙيو (x_advance, pos[i].x_advance);
    y_advance = hb_saturate_جوڙيو (y_advance, pos[i].y_advance);

    pos[i].x_advance = 0;
    pos[i].y_advance = 0;
  }

  إذا (backward)
  {
    /* Transfer all cluster advance to the last glyph. */
    pos[پڇاڙي - 1].x_advance = ڪل_x_advance;
    pos[پڇاڙي - 1].y_advance = ڪل_y_advance;

    hb_stable_ترتيب_ڏيو (بفر->info + شروعات, پڇاڙي - شروعات - 1, compare_info_codepoint, بفر->pos + شروعات);
  } وإلا {
    /* Transfer all cluster advance to the first glyph. */
    pos[شروعات].x_advance = hb_saturate_جوڙيو (pos[شروعات].x_advance, ڪل_x_advance);
    pos[شروعات].y_advance = hb_saturate_جوڙيو (pos[شروعات].y_advance, ڪل_y_advance);
    لكل (بنانشان صحيح i = شروعات + 1; i < پڇاڙي; i++) {
      pos[i].x_وٿي_2 = hb_saturate_sub (pos[i].x_وٿي_2, ڪل_x_advance);
      pos[i].y_وٿي_2 = hb_saturate_sub (pos[i].y_وٿي_2, ڪل_y_advance);
    }
    hb_stable_ترتيب_ڏيو (بفر->info + شروعات + 1, پڇاڙي - شروعات - 1, compare_info_codepoint, بفر->pos + شروعات + 1);
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
فراغ
hb_بفر_normalize_glyphs (hb_بفر_t *بفر)
{
  أكد (بفر->have_positions);

  بفر->assert_glyphs ();

  منطقي backward = HB_DIRECTION_IS_BACKWARD (بفر->props.direction);

  foreach_cluster (بفر, شروعات, پڇاڙي)
    normalize_glyphs_cluster (بفر, شروعات, پڇاڙي, backward);
}

فراغ
hb_بفر_t::sort (بنانشان صحيح شروعات, بنانشان صحيح پڇاڙي, صحيح(*compar)(ثابت hb_glyph_info_t *, ثابت hb_glyph_info_t *))
{
  أكد (!have_positions);
  لكل (بنانشان صحيح i = شروعات + 1; i < پڇاڙي; i++)
  {
    بنانشان صحيح j = i;
    طالما (j > شروعات && compar (&info[j - 1], &info[i]) > 0)
      j--;
    إذا (i == j)
      استمر;
    /* Move item i to occupy place for item j, shift what's in between. */
    ملايو_clusters (j, i + 1);
    {
      hb_glyph_info_t t = info[i];
      memmove (&info[j + 1], &info[j], (i - j) * حجم (hb_glyph_info_t));
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
hb_بفر_diff_flags_t
hb_بفر_diff (hb_بفر_t *بفر,
		hb_بفر_t *reference,
		hb_codepoint_t dottedcircle_glyph,
		بنانشان صحيح position_fuzz)
{
  إذا (بفر->content_type != reference->content_type && بفر->len && reference->len)
    أرجع HB_بفر_DIFF_FLAG_CONTENT_TYPE_MISMATCH;

  hb_بفر_diff_flags_t نتيجو = HB_بفر_DIFF_FLAG_EQUAL;
  منطقي شامل_آهي = dottedcircle_glyph != (hb_codepoint_t) -1;

  بنانشان صحيح ڳڻپ = reference->len;

  إذا (بفر->len != ڳڻپ)
  {
    /*
     * we can't compare glyph-by-glyph, but we do want to know if there
     * are .notdef or dottedcircle glyphs present in the reference buffer
     */
    ثابت hb_glyph_info_t *info = reference->info;
    بنانشان صحيح i;
    لكل (i = 0; i < ڳڻپ; i++)
    {
      إذا (شامل_آهي && info[i].codepoint == dottedcircle_glyph)
	نتيجو |= HB_بفر_DIFF_FLAG_DOTTED_CIRCLE_PRESENT;
      إذا (شامل_آهي && info[i].codepoint == 0)
	نتيجو |= HB_بفر_DIFF_FLAG_NOTDEF_PRESENT;
    }
    نتيجو |= HB_بفر_DIFF_FLAG_ڊيگهائي_MISMATCH;
    أرجع hb_بفر_diff_flags_t (نتيجو);
  }

  إذا (!ڳڻپ)
    أرجع hb_بفر_diff_flags_t (نتيجو);

  ثابت hb_glyph_info_t *buf_info = بفر->info;
  ثابت hb_glyph_info_t *ref_info = reference->info;
  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
  {
    إذا (buf_info->codepoint != ref_info->codepoint)
      نتيجو |= HB_بفر_DIFF_FLAG_CODEPOINT_MISMATCH;
    إذا (buf_info->cluster != ref_info->cluster)
      نتيجو |= HB_بفر_DIFF_FLAG_CLUSTER_MISMATCH;
    إذا ((buf_info->نقاب ^ ref_info->نقاب) & HB_GLYPH_FLAG_DEFINED)
      نتيجو |= HB_بفر_DIFF_FLAG_GLYPH_FLAGS_MISMATCH;
    إذا (شامل_آهي && ref_info->codepoint == dottedcircle_glyph)
      نتيجو |= HB_بفر_DIFF_FLAG_DOTTED_CIRCLE_PRESENT;
    إذا (شامل_آهي && ref_info->codepoint == 0)
      نتيجو |= HB_بفر_DIFF_FLAG_NOTDEF_PRESENT;
    buf_info++;
    ref_info++;
  }

  إذا (بفر->content_type == HB_بفر_CONTENT_TYPE_GLYPHS)
  {
    خودڪار position_diff = [] (hb_position_t a, hb_position_t b) -> uint64_t
    {
      int64_t diff = (int64_t) a - b;
      أرجع diff < 0 ? -diff : diff;
    };
    أكد (بفر->have_positions);
    ثابت hb_glyph_position_t *buf_pos = بفر->pos;
    ثابت hb_glyph_position_t *ref_pos = reference->pos;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (position_diff (buf_pos->x_advance, ref_pos->x_advance) > position_fuzz ||
	  position_diff (buf_pos->y_advance, ref_pos->y_advance) > position_fuzz ||
	  position_diff (buf_pos->x_وٿي_2, ref_pos->x_وٿي_2) > position_fuzz ||
	  position_diff (buf_pos->y_وٿي_2, ref_pos->y_وٿي_2) > position_fuzz)
      {
	نتيجو |= HB_بفر_DIFF_FLAG_POSITION_MISMATCH;
	اكسر;
      }
      buf_pos++;
      ref_pos++;
    }
  }

  أرجع نتيجو;
}


/*
 * Debugging.
 */

فراغ
hb_بفر_t::بدليو ()
{
#جيڪڏهن_وصف_ٿيل HB_NO_بفر_پيغام
  أرجع;
#وإلا
  إذا (!پيغام_اونهائي)
    أرجع;

  إذا (بدليو_func)
    بدليو_func (هذا, بدليو_معلومات);
  وإلا
    update_digest ();
#جيڪڏهن_پورو
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_بفر_پيغام
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
فراغ
hb_بفر_سيٽ_پيغام_func (hb_بفر_t *بفر,
			    hb_بفر_پيغام_func_t func,
			    فراغ *user_معلومات, hb_destroy_func_t destroy)
{
  إذا (unlikely (hb_object_is_immutable (بفر)) ||
      unlikely (بفر->پيغام_اونهائي))
  {
    إذا (destroy)
      destroy (user_معلومات);
    أرجع;
  }

  إذا (بفر->پيغام_destroy)
    بفر->پيغام_destroy (بفر->پيغام_معلومات);

  إذا (func) {
    بفر->پيغام_func = func;
    بفر->پيغام_معلومات = user_معلومات;
    بفر->پيغام_destroy = destroy;
  } وإلا {
    بفر->پيغام_func = خالي_اشارو;
    بفر->پيغام_معلومات = خالي_اشارو;
    بفر->پيغام_destroy = خالي_اشارو;
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
فراغ
hb_بفر_بدليو (hb_بفر_t *بفر)
{
  بفر->بدليو ();
}

منطقي
hb_بفر_t::پيغام_impl (hb_font_t *font, ثابت حرف *fmt, va_list ap)
{
  أكد (!have_output || (ٻاهريون_info == info && ٻاهريون_len == idx));

  پيغام_اونهائي++;

  حرف buf[100];
  vsnprintf (buf, حجم (buf), fmt, ap);
  منطقي ret = (منطقي) هذا->پيغام_func (هذا, font, buf, هذا->پيغام_معلومات);

  پيغام_اونهائي--;

  أرجع ret;
}
#جيڪڏهن_پورو
