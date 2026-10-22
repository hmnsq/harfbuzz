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
بیعلامت صحيح
hb_segment_properties_hash (ثابت hb_segment_properties_t *p)
{
  أرجع ((بیعلامت صحيح) p->direction * 31 +
	  (بیعلامت صحيح) p->script) * 31 +
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
hb_میانگیر_t::enlarge (بیعلامت صحيح اندازه)
{
  إذا (unlikely (اندازه > max_len))
  {
    successful = خطأ;
    أرجع خطأ;
  }

  إذا (unlikely (!successful))
    أرجع خطأ;

  بیعلامت صحيح new_allocated = allocated;
  hb_glyph_position_t *new_pos = اشاره_تهی;
  hb_glyph_info_t *new_info = اشاره_تهی;
  منطقي separate_بیرون = بیرون_info != info;

  إذا (unlikely (hb_unsigned_mul_overflows (اندازه, حجم (info[0]))))
    اذهب انجامشده;

  طالما (اندازه >= new_allocated)
    new_allocated += (new_allocated >> 1) + 32;

  بیعلامت new_بایتها;
  إذا (unlikely (hb_unsigned_mul_overflows (new_allocated, حجم (info[0]), &new_بایتها)))
    اذهب انجامشده;

  static_assert (حجم (info[0]) == حجم (pos[0]), "");
  new_pos = (hb_glyph_position_t *) hb_realloc (pos, new_بایتها);
  new_info = (hb_glyph_info_t *) hb_realloc (info, new_بایتها);

انجامشده:
  إذا (unlikely (!new_pos || !new_info))
    successful = خطأ;

  إذا (likely (new_pos))
    pos = new_pos;

  إذا (likely (new_info))
    info = new_info;

  بیرون_info = separate_بیرون ? (hb_glyph_info_t *) pos : info;
  إذا (likely (successful))
    allocated = new_allocated;

  أرجع likely (successful);
}

منطقي
hb_میانگیر_t::make_جا_for (بیعلامت صحيح رقم_درون,
			    بیعلامت صحيح رقم_بیرون)
{
  إذا (unlikely (!ensure (بیرون_len + رقم_بیرون))) أرجع خطأ;

  إذا (بیرون_info == info &&
      بیرون_len + رقم_بیرون > idx + رقم_درون)
  {
    أكد (have_output);

    بیرون_info = (hb_glyph_info_t *) pos;
    hb_memcpy (بیرون_info, info, بیرون_len * حجم (بیرون_info[0]));
  }

  أرجع درست;
}

منطقي
hb_میانگیر_t::لغزش_forward (بیعلامت صحيح شمار)
{
  أكد (have_output);
  إذا (unlikely (!ensure (len + شمار))) أرجع خطأ;

  max_ops -= len - idx;
  إذا (unlikely (max_ops < 0))
  {
    successful = خطأ;
    أرجع خطأ;
  }

  memmove (info + idx + شمار, info + idx, (len - idx) * حجم (info[0]));
  إذا (idx + شمار > len)
  {
    /* Under memory failure we might expose this area.  At least
     * clean it up.  Oh well...
     *
     * Ideally, we should at least set Default_Ignorable bits on
     * these, as well as consistent cluster values.  But the former
     * is layering violation... */
    hb_memset (info + len, 0, (idx + شمار - len) * حجم (info[0]));
  }
  len += شمار;
  idx += شمار;

  أرجع درست;
}

hb_میانگیر_t::scratch_میانگیر_t *
hb_میانگیر_t::get_scratch_میانگیر (بیعلامت صحيح *اندازه)
{
  have_output = خطأ;
  have_positions = خطأ;

  بیرون_len = 0;
  بیرون_info = info;

  أكد ((uintptr_t) pos % حجم (scratch_میانگیر_t) == 0);
  *اندازه = allocated * حجم (pos[0]) / حجم (scratch_میانگیر_t);
  أرجع (scratch_میانگیر_t *) (فراغ *) pos;
}



/* HarfBuzz-Internal API */

فراغ
hb_میانگیر_t::similar (ثابت hb_میانگیر_t &src)
{
  hb_unicode_funcs_destroy (unicode);
  unicode = hb_unicode_funcs_reference (src.unicode);
  flags = src.flags;
  cluster_level = src.cluster_level;
  replacement = src.replacement;
  invisible = src.invisible;
  not_یافت = src.not_یافت;
  not_یافت_variation_selector = src.not_یافت_variation_selector;
}

فراغ
hb_میانگیر_t::reset ()
{
  hb_unicode_funcs_destroy (unicode);
  unicode = hb_unicode_funcs_reference (hb_unicode_funcs_get_default ());
  flags = HB_میانگیر_FLAG_DEFAULT;
  cluster_level = HB_میانگیر_CLUSTER_LEVEL_DEFAULT;
  replacement = HB_میانگیر_REPLACEMENT_CODEPOINT_DEFAULT;
  invisible = 0;
  not_یافت = 0;
  not_یافت_variation_selector = HB_CODEPOINT_INVALID;

  clear ();
}

فراغ
hb_میانگیر_t::clear ()
{
  content_type = HB_میانگیر_CONTENT_TYPE_INVALID;
  hb_segment_properties_t default_props = HB_SEGMENT_PROPERTIES_DEFAULT;
  props = default_props;

  successful = درست;
  have_output = خطأ;
  have_positions = خطأ;

  idx = 0;
  len = 0;
  بیرون_len = 0;
  بیرون_info = info;

  hb_memset (context, 0, حجم context);
  hb_memset (context_len, 0, حجم context_len);

  deallocate_var_all ();
  serial = 0;
  random_state = 1;
  scratch_flags = HB_میانگیر_SCRATCH_FLAG_DEFAULT;
}

فراغ
hb_میانگیر_t::enter ()
{
  deallocate_var_all ();
  serial = 0;
  scratch_flags = HB_میانگیر_SCRATCH_FLAG_DEFAULT;
  بیعلامت mul;
  إذا (likely (!hb_unsigned_mul_overflows (len, HB_میانگیر_MAX_LEN_ضریب, &mul)))
  {
    max_len = hb_max (mul, (بیعلامت) HB_میانگیر_MAX_LEN_MIN);
  }
  إذا (likely (!hb_unsigned_mul_overflows (len, HB_میانگیر_MAX_OPS_ضریب, &mul)))
  {
    max_ops = hb_max (mul, (بیعلامت) HB_میانگیر_MAX_OPS_MIN);
  }
}
فراغ
hb_میانگیر_t::leave ()
{
  max_len = HB_میانگیر_MAX_LEN_DEFAULT;
  max_ops = HB_میانگیر_MAX_OPS_DEFAULT;
  deallocate_var_all ();
  serial = 0;
}


فراغ
hb_میانگیر_t::جمع_کن (hb_codepoint_t  codepoint,
		  بیعلامت صحيح    cluster)
{
  hb_glyph_info_t *glyph;

  إذا (unlikely (!ensure (len + 1))) أرجع;

  glyph = &info[len];

  hb_memset (glyph, 0, حجم (*glyph));
  glyph->codepoint = codepoint;
  glyph->ماسک = 0;
  glyph->cluster = cluster;

  len++;
}

فراغ
hb_میانگیر_t::جمع_کن_info (ثابت hb_glyph_info_t &glyph_info)
{
  إذا (unlikely (!ensure (len + 1))) أرجع;

  info[len] = glyph_info;

  len++;
}
فراغ
hb_میانگیر_t::جمع_کن_info_and_pos (ثابت hb_glyph_info_t &glyph_info,
			       ثابت hb_glyph_position_t &glyph_pos)
{
  إذا (unlikely (!ensure (len + 1))) أرجع;

  info[len] = glyph_info;
  أكد (have_positions);
  pos[len] = glyph_pos;

  len++;
}


فراغ
hb_میانگیر_t::clear_output ()
{
  have_output = درست;
  have_positions = خطأ;

  idx = 0;
  بیرون_len = 0;
  بیرون_info = info;
}

فراغ
hb_میانگیر_t::clear_positions ()
{
  have_output = خطأ;
  have_positions = درست;

  بیرون_len = 0;
  بیرون_info = info;

  hb_memset (pos, 0, حجم (pos[0]) * len);
}

منطقي
hb_میانگیر_t::متزامن ()
{
  منطقي ret = خطأ;

  أكد (have_output);

  أكد (idx <= len);

  إذا (unlikely (!successful || !بعدی_glyphs (len - idx)))
    اذهب reset;

  إذا (بیرون_info != info)
  {
    pos = (hb_glyph_position_t *) info;
    info = بیرون_info;
  }
  len = بیرون_len;
  ret = درست;

reset:
  have_output = خطأ;
  بیرون_len = 0;
  بیرون_info = info;
  idx = 0;

  أرجع ret;
}

صحيح
hb_میانگیر_t::sync_so_far ()
{
  منطقي had_output = have_output;
  بیعلامت بیرون_i = بیرون_len;
  بیعلامت i = idx;
  بیعلامت old_idx = idx;

  إذا (متزامن ())
    idx = بیرون_i;
  وإلا
    idx = i;

  إذا (had_output)
  {
    have_output = درست;
    بیرون_len = idx;
  }

  أكد (idx <= len);

  أرجع idx - old_idx;
}

منطقي
hb_میانگیر_t::move_مقصد (بیعلامت صحيح i)
{
  إذا (!have_output)
  {
    أكد (i <= len);
    idx = i;
    أرجع درست;
  }
  إذا (unlikely (!successful))
    أرجع خطأ;

  أكد (i <= بیرون_len + (len - idx));

  إذا (بیرون_len < i)
  {
    بیعلامت صحيح شمار = i - بیرون_len;
    إذا (unlikely (!make_جا_for (شمار, شمار))) أرجع خطأ;

    max_ops -= شمار;
    إذا (unlikely (max_ops < 0))
    {
      successful = خطأ;
      أرجع خطأ;
    }

    memmove (بیرون_info + بیرون_len, info + idx, شمار * حجم (بیرون_info[0]));
    idx += شمار;
    بیرون_len += شمار;
  }
  وإلا إذا (بیرون_len > i)
  {
    /* Tricky part: rewinding... */
    بیعلامت صحيح شمار = بیرون_len - i;

    /* This will blow in our face if memory allocation fails later
     * in this same lookup...
     *
     * We used to shift with extra 32 items.
     * But that would leave empty slots in the buffer in case of allocation
     * failures.  See comments in shift_forward().  This can cause O(N^2)
     * behavior more severely than adding 32 empty slots can... */
    إذا (unlikely (idx < شمار && !لغزش_forward (شمار - idx))) أرجع خطأ;

    أكد (idx >= شمار);

    max_ops -= شمار;
    إذا (unlikely (max_ops < 0))
    {
      successful = خطأ;
      أرجع خطأ;
    }

    idx -= شمار;
    بیرون_len -= شمار;
    memmove (info + idx, بیرون_info + بیرون_len, شمار * حجم (بیرون_info[0]));
  }

  أرجع درست;
}


فراغ
hb_میانگیر_t::مجموعه_masks (hb_ماسک_t    value,
			hb_ماسک_t    ماسک,
			بیعلامت صحيح cluster_شروع,
			بیعلامت صحيح cluster_انتها)
{
  إذا (!ماسک)
    أرجع;

  hb_ماسک_t not_ماسک = ~ماسک;
  value &= ماسک;

  max_ops -= len;
  إذا (unlikely (max_ops < 0))
    successful = خطأ;

  بیعلامت صحيح شمار = len;

  إذا (cluster_شروع == 0 && cluster_انتها == (بیعلامت صحيح) -1)
  {
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      info[i].ماسک = (info[i].ماسک & not_ماسک) | value;
    أرجع;
  }

  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    إذا (cluster_شروع <= info[i].cluster && info[i].cluster < cluster_انتها)
      info[i].ماسک = (info[i].ماسک & not_ماسک) | value;
}

فراغ
hb_میانگیر_t::ادغام_clusters_impl (بیعلامت صحيح شروع,
				  بیعلامت صحيح انتها)
{
  max_ops -= انتها - شروع;
  إذا (unlikely (max_ops < 0))
    successful = خطأ;

  بیعلامت صحيح cluster = info[شروع].cluster;

  لكل (بیعلامت صحيح i = شروع + 1; i < انتها; i++)
    cluster = hb_min (cluster, info[i].cluster);

  /* Extend end */
  إذا (cluster != info[انتها - 1].cluster)
    طالما (انتها < len && info[انتها - 1].cluster == info[انتها].cluster)
      انتها++;

  /* Extend start */
  إذا (cluster != info[شروع].cluster)
    طالما (idx < شروع && info[شروع - 1].cluster == info[شروع].cluster)
      شروع--;

  /* If we hit the start of buffer, continue in out-buffer. */
  إذا (idx == شروع && info[شروع].cluster != cluster)
    لكل (بیعلامت صحيح i = بیرون_len; i && بیرون_info[i - 1].cluster == info[شروع].cluster; i--)
      مجموعه_cluster (بیرون_info[i - 1], cluster);

  لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
    مجموعه_cluster (info[i], cluster);
}
فراغ
hb_میانگیر_t::ادغام_بیرون_clusters_impl (بیعلامت صحيح شروع,
				      بیعلامت صحيح انتها)
{
  max_ops -= انتها - شروع;
  إذا (unlikely (max_ops < 0))
    successful = خطأ;

  بیعلامت صحيح cluster = بیرون_info[شروع].cluster;

  لكل (بیعلامت صحيح i = شروع + 1; i < انتها; i++)
    cluster = hb_min (cluster, بیرون_info[i].cluster);

  /* Extend start */
  طالما (شروع && بیرون_info[شروع - 1].cluster == بیرون_info[شروع].cluster)
    شروع--;

  /* Extend end */
  طالما (انتها < بیرون_len && بیرون_info[انتها - 1].cluster == بیرون_info[انتها].cluster)
    انتها++;

  /* If we hit the end of out-buffer, continue in buffer. */
  إذا (انتها == بیرون_len)
    لكل (بیعلامت صحيح i = idx; i < len && info[i].cluster == بیرون_info[انتها - 1].cluster; i++)
      مجموعه_cluster (info[i], cluster);

  لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
    مجموعه_cluster (بیرون_info[i], cluster);
}
فراغ
hb_میانگیر_t::delete_glyph ()
{
  /* The logic here is duplicated in hb_ot_hide_default_ignorables(). */

  بیعلامت صحيح cluster = info[idx].cluster;
  إذا ((idx + 1 < len && cluster == info[idx + 1].cluster) ||
      (بیرون_len && cluster == بیرون_info[بیرون_len - 1].cluster))
  {
    /* Cluster survives; do nothing. */
    اذهب انجامشده;
  }

  إذا (بیرون_len)
  {
    /* Merge cluster backward. */
    إذا (cluster < بیرون_info[بیرون_len - 1].cluster)
    {
      بیعلامت صحيح ماسک = info[idx].ماسک;
      بیعلامت صحيح old_cluster = بیرون_info[بیرون_len - 1].cluster;
      لكل (بیعلامت i = بیرون_len; i && بیرون_info[i - 1].cluster == old_cluster; i--)
	مجموعه_cluster (بیرون_info[i - 1], cluster, ماسک);
    }
    اذهب انجامشده;
  }

  إذا (idx + 1 < len)
  {
    /* Merge cluster forward. */
    ادغام_clusters (idx, idx + 2);
    اذهب انجامشده;
  }

انجامشده:
  skip_glyph ();
}

فراغ
hb_میانگیر_t::delete_glyphs_inplace (منطقي (*filter) (ثابت hb_glyph_info_t *info))
{
  /* Merge clusters and delete filtered glyphs.
   * NOTE! We can't use out-buffer as we have positioning data. */
  بیعلامت صحيح j = 0;
  بیعلامت صحيح شمار = len;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    إذا (filter (&info[i]))
    {
      /* Merge clusters.
       * Same logic as delete_glyph(), but for in-place removal. */

      بیعلامت صحيح cluster = info[i].cluster;
      إذا (i + 1 < شمار && cluster == info[i + 1].cluster)
	استمر; /* Cluster survives; do nothing. */

      إذا (j)
      {
	/* Merge cluster backward. */
	إذا (cluster < info[j - 1].cluster)
	{
	  بیعلامت صحيح ماسک = info[i].ماسک;
	  بیعلامت صحيح old_cluster = info[j - 1].cluster;
	  لكل (بیعلامت k = j; k && info[k - 1].cluster == old_cluster; k--)
	    مجموعه_cluster (info[k - 1], cluster, ماسک);
	}
	استمر;
      }

      إذا (i + 1 < شمار)
	ادغام_clusters (i, i + 2); /* Merge cluster forward. */

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
hb_میانگیر_t::guess_segment_properties ()
{
  assert_unicode ();

  /* If script is set to INVALID, guess from buffer contents */
  إذا (props.script == HB_SCRIPT_INVALID) {
    لكل (بیعلامت صحيح i = 0; i < len; i++) {
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

DEFINE_NULL_INSTANCE (hb_میانگیر_t) =
{
  HB_OBJECT_سرآیند_STATIC,

  const_cast<hb_unicode_funcs_t *> (&_hb_Null_hb_unicode_funcs_t),
  HB_میانگیر_FLAG_DEFAULT,
  HB_میانگیر_CLUSTER_LEVEL_DEFAULT,
  HB_میانگیر_REPLACEMENT_CODEPOINT_DEFAULT,
  0, /* invisible */
  0, /* not_found */
  HB_CODEPOINT_INVALID, /* not_found_variation_selector */


  HB_میانگیر_CONTENT_TYPE_INVALID,
  HB_SEGMENT_PROPERTIES_DEFAULT,

  خطأ, /* successful */
  خطأ, /* have_output */
  درست  /* have_positions */

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
hb_میانگیر_t *
hb_میانگیر_create ()
{
  hb_میانگیر_t *میانگیر;

  إذا (!(میانگیر = hb_object_create<hb_میانگیر_t> ()))
    أرجع hb_میانگیر_get_خالی ();

  میانگیر->max_len = HB_میانگیر_MAX_LEN_DEFAULT;
  میانگیر->max_ops = HB_میانگیر_MAX_OPS_DEFAULT;

  میانگیر->reset ();

  أرجع میانگیر;
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
hb_میانگیر_t *
hb_میانگیر_create_similar (ثابت hb_میانگیر_t *src)
{
  hb_میانگیر_t *میانگیر = hb_میانگیر_create ();

  میانگیر->similar (*src);

  أرجع میانگیر;
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
hb_میانگیر_reset (hb_میانگیر_t *میانگیر)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->reset ();
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
hb_میانگیر_t *
hb_میانگیر_get_خالی ()
{
  أرجع const_cast<hb_میانگیر_t *> (&Null (hb_میانگیر_t));
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
hb_میانگیر_t *
hb_میانگیر_reference (hb_میانگیر_t *میانگیر)
{
  أرجع hb_object_reference (میانگیر);
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
hb_میانگیر_destroy (hb_میانگیر_t *میانگیر)
{
  إذا (!hb_object_destroy (میانگیر)) أرجع;

  hb_unicode_funcs_destroy (میانگیر->unicode);

  hb_free (میانگیر->info);
  hb_free (میانگیر->pos);
#اگر_تعریف_نشده HB_NO_میانگیر_پیام
  إذا (میانگیر->پیام_destroy)
    میانگیر->پیام_destroy (میانگیر->پیام_داده);
#پایان_اگر

  hb_free (میانگیر);
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
hb_میانگیر_مجموعه_user_داده (hb_میانگیر_t        *میانگیر,
			 hb_user_داده_کلید_t *key,
			 فراغ *              داده,
			 hb_destroy_func_t   destroy,
			 hb_bool_t           replace)
{
  أرجع hb_object_مجموعه_user_داده (میانگیر, key, داده, destroy, replace);
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
hb_میانگیر_get_user_داده (ثابت hb_میانگیر_t  *میانگیر,
			 hb_user_داده_کلید_t *key)
{
  أرجع hb_object_get_user_داده (میانگیر, key);
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
hb_میانگیر_مجموعه_content_type (hb_میانگیر_t              *میانگیر,
			    hb_میانگیر_content_type_t  content_type)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->content_type = content_type;
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
hb_میانگیر_content_type_t
hb_میانگیر_get_content_type (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->content_type;
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
hb_میانگیر_مجموعه_unicode_funcs (hb_میانگیر_t        *میانگیر,
			     hb_unicode_funcs_t *unicode_funcs)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  إذا (!unicode_funcs)
    unicode_funcs = hb_unicode_funcs_get_default ();

  hb_unicode_funcs_reference (unicode_funcs);
  hb_unicode_funcs_destroy (میانگیر->unicode);
  میانگیر->unicode = unicode_funcs;
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
hb_میانگیر_get_unicode_funcs (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->unicode;
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
hb_میانگیر_مجموعه_direction (hb_میانگیر_t    *میانگیر,
			 hb_direction_t  direction)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->props.direction = direction;
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
hb_میانگیر_get_direction (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->props.direction;
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
hb_میانگیر_مجموعه_script (hb_میانگیر_t *میانگیر,
		      hb_script_t  script)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->props.script = script;
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
hb_میانگیر_get_script (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->props.script;
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
hb_میانگیر_مجموعه_language (hb_میانگیر_t   *میانگیر,
			hb_language_t  language)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->props.language = language;
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
hb_میانگیر_get_language (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->props.language;
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
hb_میانگیر_مجموعه_segment_properties (hb_میانگیر_t *میانگیر,
				  ثابت hb_segment_properties_t *props)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->props = *props;
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
hb_میانگیر_get_segment_properties (ثابت hb_میانگیر_t *میانگیر,
				  hb_segment_properties_t *props)
{
  *props = میانگیر->props;
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
hb_میانگیر_مجموعه_flags (hb_میانگیر_t       *میانگیر,
		     hb_میانگیر_flags_t  flags)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->flags = flags;
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
hb_میانگیر_flags_t
hb_میانگیر_get_flags (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->flags;
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
hb_میانگیر_مجموعه_cluster_level (hb_میانگیر_t               *میانگیر,
			     hb_میانگیر_cluster_level_t  cluster_level)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->cluster_level = cluster_level;
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
hb_میانگیر_cluster_level_t
hb_میانگیر_get_cluster_level (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->cluster_level;
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
hb_میانگیر_مجموعه_replacement_codepoint (hb_میانگیر_t    *میانگیر,
				     hb_codepoint_t  replacement)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->replacement = replacement;
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
hb_میانگیر_get_replacement_codepoint (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->replacement;
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
hb_میانگیر_مجموعه_invisible_glyph (hb_میانگیر_t    *میانگیر,
			       hb_codepoint_t  invisible)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->invisible = invisible;
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
hb_میانگیر_get_invisible_glyph (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->invisible;
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
hb_میانگیر_مجموعه_not_یافت_glyph (hb_میانگیر_t    *میانگیر,
			       hb_codepoint_t  not_یافت)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->not_یافت = not_یافت;
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
hb_میانگیر_get_not_یافت_glyph (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->not_یافت;
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
hb_میانگیر_مجموعه_not_یافت_variation_selector_glyph (hb_میانگیر_t    *میانگیر,
						  hb_codepoint_t  not_یافت_variation_selector)
{
  میانگیر->not_یافت_variation_selector = not_یافت_variation_selector;
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
hb_میانگیر_get_not_یافت_variation_selector_glyph (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->not_یافت_variation_selector;
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
hb_میانگیر_مجموعه_random_state (hb_میانگیر_t    *میانگیر,
			    بیعلامت        state)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->random_state = state;
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
بیعلامت
hb_میانگیر_get_random_state (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->random_state;
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
hb_میانگیر_clear_contents (hb_میانگیر_t *میانگیر)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  میانگیر->clear ();
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
hb_میانگیر_pre_allocate (hb_میانگیر_t *میانگیر, بیعلامت صحيح اندازه)
{
  أرجع میانگیر->ensure (اندازه);
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
hb_میانگیر_تخصیص_successful (hb_میانگیر_t  *میانگیر)
{
  أرجع میانگیر->successful;
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
hb_میانگیر_جمع_کن (hb_میانگیر_t    *میانگیر,
	       hb_codepoint_t  codepoint,
	       بیعلامت صحيح    cluster)
{
  میانگیر->جمع_کن (codepoint, cluster);
  میانگیر->clear_context (1);
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
hb_میانگیر_مجموعه_درازی (hb_میانگیر_t  *میانگیر,
		      بیعلامت صحيح  درازی)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع درازی == 0;

  إذا (unlikely (!میانگیر->ensure (درازی)))
    أرجع خطأ;

  /* Wipe the new space */
  إذا (درازی > میانگیر->len) {
    hb_memset (میانگیر->info + میانگیر->len, 0, حجم (میانگیر->info[0]) * (درازی - میانگیر->len));
    إذا (میانگیر->have_positions)
      hb_memset (میانگیر->pos + میانگیر->len, 0, حجم (میانگیر->pos[0]) * (درازی - میانگیر->len));
  }

  میانگیر->len = درازی;

  إذا (!درازی)
  {
    میانگیر->content_type = HB_میانگیر_CONTENT_TYPE_INVALID;
    میانگیر->clear_context (0);
  }
  میانگیر->clear_context (1);

  أرجع درست;
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
بیعلامت صحيح
hb_میانگیر_get_درازی (ثابت hb_میانگیر_t *میانگیر)
{
  أرجع میانگیر->len;
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
hb_میانگیر_get_glyph_infos (hb_میانگیر_t  *میانگیر,
			   بیعلامت صحيح *درازی)
{
  إذا (درازی)
    *درازی = میانگیر->len;

  أرجع (hb_glyph_info_t *) میانگیر->info;
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
hb_میانگیر_get_glyph_positions (hb_میانگیر_t  *میانگیر,
			       بیعلامت صحيح *درازی)
{
  إذا (درازی)
    *درازی = میانگیر->len;

  إذا (!میانگیر->have_positions)
  {
    إذا (unlikely (میانگیر->پیام_عمق))
      أرجع اشاره_تهی;

    میانگیر->clear_positions ();
  }

  أرجع (hb_glyph_position_t *) میانگیر->pos;
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
hb_میانگیر_has_positions (hb_میانگیر_t  *میانگیر)
{
  أرجع میانگیر->have_positions;
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
hb_میانگیر_معکوس (hb_میانگیر_t *میانگیر)
{
  میانگیر->reverse ();
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
hb_میانگیر_معکوس_range (hb_میانگیر_t *میانگیر,
			 بیعلامت صحيح شروع, بیعلامت صحيح انتها)
{
  میانگیر->معکوس_range (شروع, انتها);
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
hb_میانگیر_معکوس_clusters (hb_میانگیر_t *میانگیر)
{
  میانگیر->معکوس_clusters ();
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
hb_میانگیر_guess_segment_properties (hb_میانگیر_t *میانگیر)
{
  میانگیر->guess_segment_properties ();
}

قالب <نام_نوع utf_t>
ساكن مضمن فراغ
hb_میانگیر_جمع_کن_utf (hb_میانگیر_t  *میانگیر,
		   ثابت نام_نوع utf_t::codepoint_t *متن,
		   صحيح           متن_درازی,
		   بیعلامت صحيح  قلم_جابه_جایی,
		   صحيح           قلم_درازی)
{
  تعريفنوع نام_نوع utf_t::codepoint_t T;
  ثابت hb_codepoint_t replacement = میانگیر->replacement;

  میانگیر->assert_unicode ();

  إذا (unlikely (hb_object_is_immutable (میانگیر)))
    أرجع;

  إذا (متن_درازی == -1)
    متن_درازی = utf_t::طول_رشته (متن);

  إذا (قلم_درازی == -1)
    قلم_درازی = متن_درازی - قلم_جابه_جایی;

  قلم_جابه_جایی = hb_min (قلم_جابه_جایی, (بیعلامت) متن_درازی);
  قلم_درازی = hb_clamp (قلم_درازی, 0, متن_درازی - (صحيح) قلم_جابه_جایی);

  إذا (unlikely (قلم_درازی < 0 ||
		قلم_درازی > بیشترین_صحیح / 8 ||
		!میانگیر->ensure (میانگیر->len + قلم_درازی * حجم (T) / 4)))
    أرجع;

  /* If buffer is empty and pre-context provided, install it.
   * This check is written this way, to make sure people can
   * provide pre-context in one add_utf() call, then provide
   * text in a follow-up call.  See:
   *
   * https://bugzilla.mozilla.org/show_bug.cgi?id=801410#c13
   */
  إذا (!میانگیر->len && قلم_جابه_جایی > 0)
  {
    /* Add pre-context */
    میانگیر->clear_context (0);
    ثابت T *prev = متن + قلم_جابه_جایی;
    ثابت T *شروع = متن;
    طالما (شروع < prev && میانگیر->context_len[0] < میانگیر->CONTEXT_درازی)
    {
      hb_codepoint_t u;
      prev = utf_t::prev (prev, شروع, &u, replacement);
      میانگیر->context[0][میانگیر->context_len[0]++] = u;
    }
  }

  ثابت T *next = متن + قلم_جابه_جایی;
  ثابت T *انتها = next + قلم_درازی;
  طالما (next < انتها)
  {
    hb_codepoint_t u;
    ثابت T *old_بعدی = next;
    next = utf_t::next (next, انتها, &u, replacement);
    میانگیر->جمع_کن (u, old_بعدی - (ثابت T *) متن);
  }

  /* Add post-context */
  میانگیر->clear_context (1);
  انتها = متن + متن_درازی;
  طالما (next < انتها && میانگیر->context_len[1] < میانگیر->CONTEXT_درازی)
  {
    hb_codepoint_t u;
    next = utf_t::next (next, انتها, &u, replacement);
    میانگیر->context[1][میانگیر->context_len[1]++] = u;
  }

  میانگیر->content_type = HB_میانگیر_CONTENT_TYPE_UNICODE;
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
hb_میانگیر_جمع_کن_utf8 (hb_میانگیر_t  *میانگیر,
		    ثابت حرف   *متن,
		    صحيح           متن_درازی,
		    بیعلامت صحيح  قلم_جابه_جایی,
		    صحيح           قلم_درازی)
{
  hb_میانگیر_جمع_کن_utf<hb_utf8_t> (میانگیر, (ثابت uint8_t *) متن, متن_درازی, قلم_جابه_جایی, قلم_درازی);
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
hb_میانگیر_جمع_کن_utf16 (hb_میانگیر_t    *میانگیر,
		     ثابت uint16_t *متن,
		     صحيح             متن_درازی,
		     بیعلامت صحيح    قلم_جابه_جایی,
		     صحيح             قلم_درازی)
{
  hb_میانگیر_جمع_کن_utf<hb_utf16_t> (میانگیر, متن, متن_درازی, قلم_جابه_جایی, قلم_درازی);
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
hb_میانگیر_جمع_کن_utf32 (hb_میانگیر_t    *میانگیر,
		     ثابت uint32_t *متن,
		     صحيح             متن_درازی,
		     بیعلامت صحيح    قلم_جابه_جایی,
		     صحيح             قلم_درازی)
{
  hb_میانگیر_جمع_کن_utf<hb_utf32_t> (میانگیر, متن, متن_درازی, قلم_جابه_جایی, قلم_درازی);
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
hb_میانگیر_جمع_کن_latin1 (hb_میانگیر_t   *میانگیر,
		      ثابت uint8_t *متن,
		      صحيح            متن_درازی,
		      بیعلامت صحيح   قلم_جابه_جایی,
		      صحيح            قلم_درازی)
{
  hb_میانگیر_جمع_کن_utf<hb_latin1_t> (میانگیر, متن, متن_درازی, قلم_جابه_جایی, قلم_درازی);
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
hb_میانگیر_جمع_کن_codepoints (hb_میانگیر_t          *میانگیر,
			  ثابت hb_codepoint_t *متن,
			  صحيح                   متن_درازی,
			  بیعلامت صحيح          قلم_جابه_جایی,
			  صحيح                   قلم_درازی)
{
  hb_میانگیر_جمع_کن_utf<hb_utf32_novalidate_t> (میانگیر, متن, متن_درازی, قلم_جابه_جایی, قلم_درازی);
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
hb_میانگیر_append (hb_میانگیر_t *میانگیر,
		  ثابت hb_میانگیر_t *منبع,
		  بیعلامت صحيح شروع,
		  بیعلامت صحيح انتها)
{
  أكد (!میانگیر->have_output && !منبع->have_output);
  أكد (میانگیر->have_positions == منبع->have_positions ||
	  !میانگیر->len || !منبع->len);
  أكد (میانگیر->content_type == منبع->content_type ||
	  !میانگیر->len || !منبع->len);

  إذا (انتها > منبع->len)
    انتها = منبع->len;
  إذا (شروع > انتها)
    شروع = انتها;
  إذا (شروع == انتها)
    أرجع;

  إذا (میانگیر->len + (انتها - شروع) < میانگیر->len) /* Overflows. */
  {
    میانگیر->successful = خطأ;
    أرجع;
  }

  بیعلامت صحيح orig_len = میانگیر->len;
  hb_میانگیر_مجموعه_درازی (میانگیر, میانگیر->len + (انتها - شروع));
  إذا (unlikely (!میانگیر->successful))
    أرجع;

  إذا (!orig_len)
    میانگیر->content_type = منبع->content_type;
  إذا (!میانگیر->have_positions && منبع->have_positions)
    میانگیر->clear_positions ();

  hb_segment_properties_overlay (&میانگیر->props, &منبع->props);

  hb_memcpy (میانگیر->info + orig_len, منبع->info + شروع, (انتها - شروع) * حجم (میانگیر->info[0]));
  إذا (میانگیر->have_positions)
    hb_memcpy (میانگیر->pos + orig_len, منبع->pos + شروع, (انتها - شروع) * حجم (میانگیر->pos[0]));

  إذا (منبع->content_type == HB_میانگیر_CONTENT_TYPE_UNICODE)
  {
    /* See similar logic in add_utf. */

    /* pre-context */
    إذا (!orig_len && شروع + منبع->context_len[0] > 0)
    {
      میانگیر->clear_context (0);
      طالما (شروع > 0 && میانگیر->context_len[0] < میانگیر->CONTEXT_درازی)
	میانگیر->context[0][میانگیر->context_len[0]++] = منبع->info[--شروع].codepoint;
      لكل (خودکار i = 0u; i < منبع->context_len[0] && میانگیر->context_len[0] < میانگیر->CONTEXT_درازی; i++)
	میانگیر->context[0][میانگیر->context_len[0]++] = منبع->context[0][i];
    }

    /* post-context */
    میانگیر->clear_context (1);
    طالما (انتها < منبع->len && میانگیر->context_len[1] < میانگیر->CONTEXT_درازی)
      میانگیر->context[1][میانگیر->context_len[1]++] = منبع->info[انتها++].codepoint;
    لكل (خودکار i = 0u; i < منبع->context_len[1] && میانگیر->context_len[1] < میانگیر->CONTEXT_درازی; i++)
      میانگیر->context[1][میانگیر->context_len[1]++] = منبع->context[1][i];
  }
}


ساكن صحيح
compare_info_codepoint (ثابت hb_glyph_info_t *pa,
			ثابت hb_glyph_info_t *pb)
{
  أرجع (صحيح) pb->codepoint - (صحيح) pa->codepoint;
}

ساكن مضمن فراغ
normalize_glyphs_cluster (hb_میانگیر_t *میانگیر,
			  بیعلامت صحيح شروع,
			  بیعلامت صحيح انتها,
			  منطقي backward)
{
  hb_glyph_position_t *pos = میانگیر->pos;

  /* Total cluster advance */
  hb_position_t مجموع_x_advance = 0, مجموع_y_advance = 0;
  لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
  {
    مجموع_x_advance = hb_saturate_جمع_کن (مجموع_x_advance, pos[i].x_advance);
    مجموع_y_advance = hb_saturate_جمع_کن (مجموع_y_advance, pos[i].y_advance);
  }

  hb_position_t x_advance = 0, y_advance = 0;
  لكل (بیعلامت صحيح i = شروع; i < انتها; i++)
  {
    pos[i].x_جابه_جایی_2 = hb_saturate_جمع_کن (pos[i].x_جابه_جایی_2, x_advance);
    pos[i].y_جابه_جایی_2 = hb_saturate_جمع_کن (pos[i].y_جابه_جایی_2, y_advance);

    x_advance = hb_saturate_جمع_کن (x_advance, pos[i].x_advance);
    y_advance = hb_saturate_جمع_کن (y_advance, pos[i].y_advance);

    pos[i].x_advance = 0;
    pos[i].y_advance = 0;
  }

  إذا (backward)
  {
    /* Transfer all cluster advance to the last glyph. */
    pos[انتها - 1].x_advance = مجموع_x_advance;
    pos[انتها - 1].y_advance = مجموع_y_advance;

    hb_stable_مرتب_سازی (میانگیر->info + شروع, انتها - شروع - 1, compare_info_codepoint, میانگیر->pos + شروع);
  } وإلا {
    /* Transfer all cluster advance to the first glyph. */
    pos[شروع].x_advance = hb_saturate_جمع_کن (pos[شروع].x_advance, مجموع_x_advance);
    pos[شروع].y_advance = hb_saturate_جمع_کن (pos[شروع].y_advance, مجموع_y_advance);
    لكل (بیعلامت صحيح i = شروع + 1; i < انتها; i++) {
      pos[i].x_جابه_جایی_2 = hb_saturate_sub (pos[i].x_جابه_جایی_2, مجموع_x_advance);
      pos[i].y_جابه_جایی_2 = hb_saturate_sub (pos[i].y_جابه_جایی_2, مجموع_y_advance);
    }
    hb_stable_مرتب_سازی (میانگیر->info + شروع + 1, انتها - شروع - 1, compare_info_codepoint, میانگیر->pos + شروع + 1);
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
hb_میانگیر_normalize_glyphs (hb_میانگیر_t *میانگیر)
{
  أكد (میانگیر->have_positions);

  میانگیر->assert_glyphs ();

  منطقي backward = HB_DIRECTION_IS_BACKWARD (میانگیر->props.direction);

  foreach_cluster (میانگیر, شروع, انتها)
    normalize_glyphs_cluster (میانگیر, شروع, انتها, backward);
}

فراغ
hb_میانگیر_t::sort (بیعلامت صحيح شروع, بیعلامت صحيح انتها, صحيح(*compar)(ثابت hb_glyph_info_t *, ثابت hb_glyph_info_t *))
{
  أكد (!have_positions);
  لكل (بیعلامت صحيح i = شروع + 1; i < انتها; i++)
  {
    بیعلامت صحيح j = i;
    طالما (j > شروع && compar (&info[j - 1], &info[i]) > 0)
      j--;
    إذا (i == j)
      استمر;
    /* Move item i to occupy place for item j, shift what's in between. */
    ادغام_clusters (j, i + 1);
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
hb_میانگیر_diff_flags_t
hb_میانگیر_diff (hb_میانگیر_t *میانگیر,
		hb_میانگیر_t *reference,
		hb_codepoint_t dottedcircle_glyph,
		بیعلامت صحيح position_fuzz)
{
  إذا (میانگیر->content_type != reference->content_type && میانگیر->len && reference->len)
    أرجع HB_میانگیر_DIFF_FLAG_CONTENT_TYPE_MISMATCH;

  hb_میانگیر_diff_flags_t نتیجه = HB_میانگیر_DIFF_FLAG_EQUAL;
  منطقي شامل_است = dottedcircle_glyph != (hb_codepoint_t) -1;

  بیعلامت صحيح شمار = reference->len;

  إذا (میانگیر->len != شمار)
  {
    /*
     * we can't compare glyph-by-glyph, but we do want to know if there
     * are .notdef or dottedcircle glyphs present in the reference buffer
     */
    ثابت hb_glyph_info_t *info = reference->info;
    بیعلامت صحيح i;
    لكل (i = 0; i < شمار; i++)
    {
      إذا (شامل_است && info[i].codepoint == dottedcircle_glyph)
	نتیجه |= HB_میانگیر_DIFF_FLAG_DOTTED_CIRCLE_PRESENT;
      إذا (شامل_است && info[i].codepoint == 0)
	نتیجه |= HB_میانگیر_DIFF_FLAG_NOTDEF_PRESENT;
    }
    نتیجه |= HB_میانگیر_DIFF_FLAG_درازی_MISMATCH;
    أرجع hb_میانگیر_diff_flags_t (نتیجه);
  }

  إذا (!شمار)
    أرجع hb_میانگیر_diff_flags_t (نتیجه);

  ثابت hb_glyph_info_t *buf_info = میانگیر->info;
  ثابت hb_glyph_info_t *ref_info = reference->info;
  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    إذا (buf_info->codepoint != ref_info->codepoint)
      نتیجه |= HB_میانگیر_DIFF_FLAG_CODEPOINT_MISMATCH;
    إذا (buf_info->cluster != ref_info->cluster)
      نتیجه |= HB_میانگیر_DIFF_FLAG_CLUSTER_MISMATCH;
    إذا ((buf_info->ماسک ^ ref_info->ماسک) & HB_GLYPH_FLAG_DEFINED)
      نتیجه |= HB_میانگیر_DIFF_FLAG_GLYPH_FLAGS_MISMATCH;
    إذا (شامل_است && ref_info->codepoint == dottedcircle_glyph)
      نتیجه |= HB_میانگیر_DIFF_FLAG_DOTTED_CIRCLE_PRESENT;
    إذا (شامل_است && ref_info->codepoint == 0)
      نتیجه |= HB_میانگیر_DIFF_FLAG_NOTDEF_PRESENT;
    buf_info++;
    ref_info++;
  }

  إذا (میانگیر->content_type == HB_میانگیر_CONTENT_TYPE_GLYPHS)
  {
    خودکار position_diff = [] (hb_position_t a, hb_position_t b) -> uint64_t
    {
      int64_t diff = (int64_t) a - b;
      أرجع diff < 0 ? -diff : diff;
    };
    أكد (میانگیر->have_positions);
    ثابت hb_glyph_position_t *buf_pos = میانگیر->pos;
    ثابت hb_glyph_position_t *ref_pos = reference->pos;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (position_diff (buf_pos->x_advance, ref_pos->x_advance) > position_fuzz ||
	  position_diff (buf_pos->y_advance, ref_pos->y_advance) > position_fuzz ||
	  position_diff (buf_pos->x_جابه_جایی_2, ref_pos->x_جابه_جایی_2) > position_fuzz ||
	  position_diff (buf_pos->y_جابه_جایی_2, ref_pos->y_جابه_جایی_2) > position_fuzz)
      {
	نتیجه |= HB_میانگیر_DIFF_FLAG_POSITION_MISMATCH;
	اكسر;
      }
      buf_pos++;
      ref_pos++;
    }
  }

  أرجع نتیجه;
}


/*
 * Debugging.
 */

فراغ
hb_میانگیر_t::تغییریافته ()
{
#اگر_تعریف_شده HB_NO_میانگیر_پیام
  أرجع;
#وإلا
  إذا (!پیام_عمق)
    أرجع;

  إذا (تغییریافته_func)
    تغییریافته_func (هذا, تغییریافته_داده);
  وإلا
    update_digest ();
#پایان_اگر
}

#اگر_تعریف_نشده HB_NO_میانگیر_پیام
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
hb_میانگیر_مجموعه_پیام_func (hb_میانگیر_t *میانگیر,
			    hb_میانگیر_پیام_func_t func,
			    فراغ *user_داده, hb_destroy_func_t destroy)
{
  إذا (unlikely (hb_object_is_immutable (میانگیر)) ||
      unlikely (میانگیر->پیام_عمق))
  {
    إذا (destroy)
      destroy (user_داده);
    أرجع;
  }

  إذا (میانگیر->پیام_destroy)
    میانگیر->پیام_destroy (میانگیر->پیام_داده);

  إذا (func) {
    میانگیر->پیام_func = func;
    میانگیر->پیام_داده = user_داده;
    میانگیر->پیام_destroy = destroy;
  } وإلا {
    میانگیر->پیام_func = اشاره_تهی;
    میانگیر->پیام_داده = اشاره_تهی;
    میانگیر->پیام_destroy = اشاره_تهی;
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
hb_میانگیر_تغییریافته (hb_میانگیر_t *میانگیر)
{
  میانگیر->تغییریافته ();
}

منطقي
hb_میانگیر_t::پیام_impl (hb_font_t *font, ثابت حرف *fmt, va_list ap)
{
  أكد (!have_output || (بیرون_info == info && بیرون_len == idx));

  پیام_عمق++;

  حرف buf[100];
  vsnprintf (buf, حجم (buf), fmt, ap);
  منطقي ret = (منطقي) هذا->پیام_func (هذا, font, buf, هذا->پیام_داده);

  پیام_عمق--;

  أرجع ret;
}
#پایان_اگر
