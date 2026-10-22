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

#అన్తర్భావ "hb-buffer.hh"
#అన్తర్భావ "hb-utf.hh"


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
hb_segment_properties_equal (స్థిరరాశి hb_segment_properties_t *a,
			     స్థిరరాశి hb_segment_properties_t *b)
{
  తిరిగి a->direction == b->direction &&
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
గుర్తులేని పూర్ణాంకం
hb_segment_properties_hash (స్థిరరాశి hb_segment_properties_t *p)
{
  తిరిగి ((గుర్తులేని పూర్ణాంకం) p->direction * 31 +
	  (గుర్తులేని పూర్ణాంకం) p->script) * 31 +
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
శూన్య
hb_segment_properties_overlay (hb_segment_properties_t *p,
			       స్థిరరాశి hb_segment_properties_t *src)
{
  ఒకవేళ (unlikely (!p || !src))
    తిరిగి;

  ఒకవేళ (!p->direction)
    p->direction = src->direction;

  ఒకవేళ (p->direction != src->direction)
    తిరిగి;

  ఒకవేళ (!p->script)
    p->script = src->script;

  ఒకవేళ (p->script != src->script)
    తిరిగి;

  ఒకవేళ (!p->language)
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

తార్కిక
hb_బఫరు_t::enlarge (గుర్తులేని పూర్ణాంకం కొలత)
{
  ఒకవేళ (unlikely (కొలత > max_len))
  {
    successful = అబద్ధం;
    తిరిగి అబద్ధం;
  }

  ఒకవేళ (unlikely (!successful))
    తిరిగి అబద్ధం;

  గుర్తులేని పూర్ణాంకం new_allocated = allocated;
  hb_glyph_position_t *new_pos = శూన్యసూచిక;
  hb_glyph_info_t *new_info = శూన్యసూచిక;
  తార్కిక separate_బహిర్గతం = బహిర్గతం_info != info;

  ఒకవేళ (unlikely (hb_unsigned_mul_overflows (కొలత, పరిమాణం (info[0]))))
    వెళ్ళు పూర్తి;

  అయితే (కొలత >= new_allocated)
    new_allocated += (new_allocated >> 1) + 32;

  గుర్తులేని new_బైట్లు;
  ఒకవేళ (unlikely (hb_unsigned_mul_overflows (new_allocated, పరిమాణం (info[0]), &new_బైట్లు)))
    వెళ్ళు పూర్తి;

  static_assert (పరిమాణం (info[0]) == పరిమాణం (pos[0]), "");
  new_pos = (hb_glyph_position_t *) hb_realloc (pos, new_బైట్లు);
  new_info = (hb_glyph_info_t *) hb_realloc (info, new_బైట్లు);

పూర్తి:
  ఒకవేళ (unlikely (!new_pos || !new_info))
    successful = అబద్ధం;

  ఒకవేళ (likely (new_pos))
    pos = new_pos;

  ఒకవేళ (likely (new_info))
    info = new_info;

  బహిర్గతం_info = separate_బహిర్గతం ? (hb_glyph_info_t *) pos : info;
  ఒకవేళ (likely (successful))
    allocated = new_allocated;

  తిరిగి likely (successful);
}

తార్కిక
hb_బఫరు_t::make_స్థలం_for (గుర్తులేని పూర్ణాంకం అంకె_లోపల,
			    గుర్తులేని పూర్ణాంకం అంకె_బహిర్గతం)
{
  ఒకవేళ (unlikely (!ensure (బహిర్గతం_len + అంకె_బహిర్గతం))) తిరిగి అబద్ధం;

  ఒకవేళ (బహిర్గతం_info == info &&
      బహిర్గతం_len + అంకె_బహిర్గతం > idx + అంకె_లోపల)
  {
    ప్రతిజ్ఞా (have_output);

    బహిర్గతం_info = (hb_glyph_info_t *) pos;
    hb_memcpy (బహిర్గతం_info, info, బహిర్గతం_len * పరిమాణం (బహిర్గతం_info[0]));
  }

  తిరిగి నిజం;
}

తార్కిక
hb_బఫరు_t::జరుపు_forward (గుర్తులేని పూర్ణాంకం లెక్క)
{
  ప్రతిజ్ఞా (have_output);
  ఒకవేళ (unlikely (!ensure (len + లెక్క))) తిరిగి అబద్ధం;

  max_ops -= len - idx;
  ఒకవేళ (unlikely (max_ops < 0))
  {
    successful = అబద్ధం;
    తిరిగి అబద్ధం;
  }

  memmove (info + idx + లెక్క, info + idx, (len - idx) * పరిమాణం (info[0]));
  ఒకవేళ (idx + లెక్క > len)
  {
    /* Under memory failure we might expose this area.  At least
     * clean it up.  Oh well...
     *
     * Ideally, we should at least set Default_Ignorable bits on
     * these, as well as consistent cluster values.  But the former
     * is layering violation... */
    hb_memset (info + len, 0, (idx + లెక్క - len) * పరిమాణం (info[0]));
  }
  len += లెక్క;
  idx += లెక్క;

  తిరిగి నిజం;
}

hb_బఫరు_t::scratch_బఫరు_t *
hb_బఫరు_t::get_scratch_బఫరు (గుర్తులేని పూర్ణాంకం *కొలత)
{
  have_output = అబద్ధం;
  have_positions = అబద్ధం;

  బహిర్గతం_len = 0;
  బహిర్గతం_info = info;

  ప్రతిజ్ఞా ((uintptr_t) pos % పరిమాణం (scratch_బఫరు_t) == 0);
  *కొలత = allocated * పరిమాణం (pos[0]) / పరిమాణం (scratch_బఫరు_t);
  తిరిగి (scratch_బఫరు_t *) (శూన్య *) pos;
}



/* HarfBuzz-Internal API */

శూన్య
hb_బఫరు_t::similar (స్థిరరాశి hb_బఫరు_t &src)
{
  hb_unicode_funcs_destroy (unicode);
  unicode = hb_unicode_funcs_reference (src.unicode);
  flags = src.flags;
  cluster_level = src.cluster_level;
  replacement = src.replacement;
  invisible = src.invisible;
  not_దొరికింది = src.not_దొరికింది;
  not_దొరికింది_variation_selector = src.not_దొరికింది_variation_selector;
}

శూన్య
hb_బఫరు_t::reset ()
{
  hb_unicode_funcs_destroy (unicode);
  unicode = hb_unicode_funcs_reference (hb_unicode_funcs_get_default ());
  flags = HB_బఫరు_FLAG_DEFAULT;
  cluster_level = HB_బఫరు_CLUSTER_LEVEL_DEFAULT;
  replacement = HB_బఫరు_REPLACEMENT_CODEPOINT_DEFAULT;
  invisible = 0;
  not_దొరికింది = 0;
  not_దొరికింది_variation_selector = HB_CODEPOINT_INVALID;

  clear ();
}

శూన్య
hb_బఫరు_t::clear ()
{
  content_type = HB_బఫరు_CONTENT_TYPE_INVALID;
  hb_segment_properties_t default_props = HB_SEGMENT_PROPERTIES_DEFAULT;
  props = default_props;

  successful = నిజం;
  have_output = అబద్ధం;
  have_positions = అబద్ధం;

  idx = 0;
  len = 0;
  బహిర్గతం_len = 0;
  బహిర్గతం_info = info;

  hb_memset (context, 0, పరిమాణం context);
  hb_memset (context_len, 0, పరిమాణం context_len);

  deallocate_var_all ();
  serial = 0;
  random_state = 1;
  scratch_flags = HB_బఫరు_SCRATCH_FLAG_DEFAULT;
}

శూన్య
hb_బఫరు_t::enter ()
{
  deallocate_var_all ();
  serial = 0;
  scratch_flags = HB_బఫరు_SCRATCH_FLAG_DEFAULT;
  గుర్తులేని mul;
  ఒకవేళ (likely (!hb_unsigned_mul_overflows (len, HB_బఫరు_MAX_LEN_గుణకం, &mul)))
  {
    max_len = hb_max (mul, (గుర్తులేని) HB_బఫరు_MAX_LEN_MIN);
  }
  ఒకవేళ (likely (!hb_unsigned_mul_overflows (len, HB_బఫరు_MAX_OPS_గుణకం, &mul)))
  {
    max_ops = hb_max (mul, (గుర్తులేని) HB_బఫరు_MAX_OPS_MIN);
  }
}
శూన్య
hb_బఫరు_t::leave ()
{
  max_len = HB_బఫరు_MAX_LEN_DEFAULT;
  max_ops = HB_బఫరు_MAX_OPS_DEFAULT;
  deallocate_var_all ();
  serial = 0;
}


శూన్య
hb_బఫరు_t::కూడు (hb_codepoint_t  codepoint,
		  గుర్తులేని పూర్ణాంకం    cluster)
{
  hb_glyph_info_t *glyph;

  ఒకవేళ (unlikely (!ensure (len + 1))) తిరిగి;

  glyph = &info[len];

  hb_memset (glyph, 0, పరిమాణం (*glyph));
  glyph->codepoint = codepoint;
  glyph->ముసుగు = 0;
  glyph->cluster = cluster;

  len++;
}

శూన్య
hb_బఫరు_t::కూడు_info (స్థిరరాశి hb_glyph_info_t &glyph_info)
{
  ఒకవేళ (unlikely (!ensure (len + 1))) తిరిగి;

  info[len] = glyph_info;

  len++;
}
శూన్య
hb_బఫరు_t::కూడు_info_and_pos (స్థిరరాశి hb_glyph_info_t &glyph_info,
			       స్థిరరాశి hb_glyph_position_t &glyph_pos)
{
  ఒకవేళ (unlikely (!ensure (len + 1))) తిరిగి;

  info[len] = glyph_info;
  ప్రతిజ్ఞా (have_positions);
  pos[len] = glyph_pos;

  len++;
}


శూన్య
hb_బఫరు_t::clear_output ()
{
  have_output = నిజం;
  have_positions = అబద్ధం;

  idx = 0;
  బహిర్గతం_len = 0;
  బహిర్గతం_info = info;
}

శూన్య
hb_బఫరు_t::clear_positions ()
{
  have_output = అబద్ధం;
  have_positions = నిజం;

  బహిర్గతం_len = 0;
  బహిర్గతం_info = info;

  hb_memset (pos, 0, పరిమాణం (pos[0]) * len);
}

తార్కిక
hb_బఫరు_t::సమకాలీకరణ ()
{
  తార్కిక ret = అబద్ధం;

  ప్రతిజ్ఞా (have_output);

  ప్రతిజ్ఞా (idx <= len);

  ఒకవేళ (unlikely (!successful || !తదుపరి_glyphs (len - idx)))
    వెళ్ళు reset;

  ఒకవేళ (బహిర్గతం_info != info)
  {
    pos = (hb_glyph_position_t *) info;
    info = బహిర్గతం_info;
  }
  len = బహిర్గతం_len;
  ret = నిజం;

reset:
  have_output = అబద్ధం;
  బహిర్గతం_len = 0;
  బహిర్గతం_info = info;
  idx = 0;

  తిరిగి ret;
}

పూర్ణాంకం
hb_బఫరు_t::sync_so_far ()
{
  తార్కిక had_output = have_output;
  గుర్తులేని బహిర్గతం_i = బహిర్గతం_len;
  గుర్తులేని i = idx;
  గుర్తులేని old_idx = idx;

  ఒకవేళ (సమకాలీకరణ ())
    idx = బహిర్గతం_i;
  లేకపోతే
    idx = i;

  ఒకవేళ (had_output)
  {
    have_output = నిజం;
    బహిర్గతం_len = idx;
  }

  ప్రతిజ్ఞా (idx <= len);

  తిరిగి idx - old_idx;
}

తార్కిక
hb_బఫరు_t::move_గమ్యం (గుర్తులేని పూర్ణాంకం i)
{
  ఒకవేళ (!have_output)
  {
    ప్రతిజ్ఞా (i <= len);
    idx = i;
    తిరిగి నిజం;
  }
  ఒకవేళ (unlikely (!successful))
    తిరిగి అబద్ధం;

  ప్రతిజ్ఞా (i <= బహిర్గతం_len + (len - idx));

  ఒకవేళ (బహిర్గతం_len < i)
  {
    గుర్తులేని పూర్ణాంకం లెక్క = i - బహిర్గతం_len;
    ఒకవేళ (unlikely (!make_స్థలం_for (లెక్క, లెక్క))) తిరిగి అబద్ధం;

    max_ops -= లెక్క;
    ఒకవేళ (unlikely (max_ops < 0))
    {
      successful = అబద్ధం;
      తిరిగి అబద్ధం;
    }

    memmove (బహిర్గతం_info + బహిర్గతం_len, info + idx, లెక్క * పరిమాణం (బహిర్గతం_info[0]));
    idx += లెక్క;
    బహిర్గతం_len += లెక్క;
  }
  లేకపోతే ఒకవేళ (బహిర్గతం_len > i)
  {
    /* Tricky part: rewinding... */
    గుర్తులేని పూర్ణాంకం లెక్క = బహిర్గతం_len - i;

    /* This will blow in our face if memory allocation fails later
     * in this same lookup...
     *
     * We used to shift with extra 32 items.
     * But that would leave empty slots in the buffer in case of allocation
     * failures.  See comments in shift_forward().  This can cause O(N^2)
     * behavior more severely than adding 32 empty slots can... */
    ఒకవేళ (unlikely (idx < లెక్క && !జరుపు_forward (లెక్క - idx))) తిరిగి అబద్ధం;

    ప్రతిజ్ఞా (idx >= లెక్క);

    max_ops -= లెక్క;
    ఒకవేళ (unlikely (max_ops < 0))
    {
      successful = అబద్ధం;
      తిరిగి అబద్ధం;
    }

    idx -= లెక్క;
    బహిర్గతం_len -= లెక్క;
    memmove (info + idx, బహిర్గతం_info + బహిర్గతం_len, లెక్క * పరిమాణం (బహిర్గతం_info[0]));
  }

  తిరిగి నిజం;
}


శూన్య
hb_బఫరు_t::సమితి_masks (hb_ముసుగు_t    value,
			hb_ముసుగు_t    ముసుగు,
			గుర్తులేని పూర్ణాంకం cluster_ప్రారంభం,
			గుర్తులేని పూర్ణాంకం cluster_చివర)
{
  ఒకవేళ (!ముసుగు)
    తిరిగి;

  hb_ముసుగు_t not_ముసుగు = ~ముసుగు;
  value &= ముసుగు;

  max_ops -= len;
  ఒకవేళ (unlikely (max_ops < 0))
    successful = అబద్ధం;

  గుర్తులేని పూర్ణాంకం లెక్క = len;

  ఒకవేళ (cluster_ప్రారంభం == 0 && cluster_చివర == (గుర్తులేని పూర్ణాంకం) -1)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      info[i].ముసుగు = (info[i].ముసుగు & not_ముసుగు) | value;
    తిరిగి;
  }

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    ఒకవేళ (cluster_ప్రారంభం <= info[i].cluster && info[i].cluster < cluster_చివర)
      info[i].ముసుగు = (info[i].ముసుగు & not_ముసుగు) | value;
}

శూన్య
hb_బఫరు_t::విలీనం_clusters_impl (గుర్తులేని పూర్ణాంకం ప్రారంభం,
				  గుర్తులేని పూర్ణాంకం చివర)
{
  max_ops -= చివర - ప్రారంభం;
  ఒకవేళ (unlikely (max_ops < 0))
    successful = అబద్ధం;

  గుర్తులేని పూర్ణాంకం cluster = info[ప్రారంభం].cluster;

  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం + 1; i < చివర; i++)
    cluster = hb_min (cluster, info[i].cluster);

  /* Extend end */
  ఒకవేళ (cluster != info[చివర - 1].cluster)
    అయితే (చివర < len && info[చివర - 1].cluster == info[చివర].cluster)
      చివర++;

  /* Extend start */
  ఒకవేళ (cluster != info[ప్రారంభం].cluster)
    అయితే (idx < ప్రారంభం && info[ప్రారంభం - 1].cluster == info[ప్రారంభం].cluster)
      ప్రారంభం--;

  /* If we hit the start of buffer, continue in out-buffer. */
  ఒకవేళ (idx == ప్రారంభం && info[ప్రారంభం].cluster != cluster)
    కొరకు (గుర్తులేని పూర్ణాంకం i = బహిర్గతం_len; i && బహిర్గతం_info[i - 1].cluster == info[ప్రారంభం].cluster; i--)
      సమితి_cluster (బహిర్గతం_info[i - 1], cluster);

  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
    సమితి_cluster (info[i], cluster);
}
శూన్య
hb_బఫరు_t::విలీనం_బహిర్గతం_clusters_impl (గుర్తులేని పూర్ణాంకం ప్రారంభం,
				      గుర్తులేని పూర్ణాంకం చివర)
{
  max_ops -= చివర - ప్రారంభం;
  ఒకవేళ (unlikely (max_ops < 0))
    successful = అబద్ధం;

  గుర్తులేని పూర్ణాంకం cluster = బహిర్గతం_info[ప్రారంభం].cluster;

  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం + 1; i < చివర; i++)
    cluster = hb_min (cluster, బహిర్గతం_info[i].cluster);

  /* Extend start */
  అయితే (ప్రారంభం && బహిర్గతం_info[ప్రారంభం - 1].cluster == బహిర్గతం_info[ప్రారంభం].cluster)
    ప్రారంభం--;

  /* Extend end */
  అయితే (చివర < బహిర్గతం_len && బహిర్గతం_info[చివర - 1].cluster == బహిర్గతం_info[చివర].cluster)
    చివర++;

  /* If we hit the end of out-buffer, continue in buffer. */
  ఒకవేళ (చివర == బహిర్గతం_len)
    కొరకు (గుర్తులేని పూర్ణాంకం i = idx; i < len && info[i].cluster == బహిర్గతం_info[చివర - 1].cluster; i++)
      సమితి_cluster (info[i], cluster);

  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
    సమితి_cluster (బహిర్గతం_info[i], cluster);
}
శూన్య
hb_బఫరు_t::delete_glyph ()
{
  /* The logic here is duplicated in hb_ot_hide_default_ignorables(). */

  గుర్తులేని పూర్ణాంకం cluster = info[idx].cluster;
  ఒకవేళ ((idx + 1 < len && cluster == info[idx + 1].cluster) ||
      (బహిర్గతం_len && cluster == బహిర్గతం_info[బహిర్గతం_len - 1].cluster))
  {
    /* Cluster survives; do nothing. */
    వెళ్ళు పూర్తి;
  }

  ఒకవేళ (బహిర్గతం_len)
  {
    /* Merge cluster backward. */
    ఒకవేళ (cluster < బహిర్గతం_info[బహిర్గతం_len - 1].cluster)
    {
      గుర్తులేని పూర్ణాంకం ముసుగు = info[idx].ముసుగు;
      గుర్తులేని పూర్ణాంకం old_cluster = బహిర్గతం_info[బహిర్గతం_len - 1].cluster;
      కొరకు (గుర్తులేని i = బహిర్గతం_len; i && బహిర్గతం_info[i - 1].cluster == old_cluster; i--)
	సమితి_cluster (బహిర్గతం_info[i - 1], cluster, ముసుగు);
    }
    వెళ్ళు పూర్తి;
  }

  ఒకవేళ (idx + 1 < len)
  {
    /* Merge cluster forward. */
    విలీనం_clusters (idx, idx + 2);
    వెళ్ళు పూర్తి;
  }

పూర్తి:
  skip_glyph ();
}

శూన్య
hb_బఫరు_t::delete_glyphs_inplace (తార్కిక (*filter) (స్థిరరాశి hb_glyph_info_t *info))
{
  /* Merge clusters and delete filtered glyphs.
   * NOTE! We can't use out-buffer as we have positioning data. */
  గుర్తులేని పూర్ణాంకం j = 0;
  గుర్తులేని పూర్ణాంకం లెక్క = len;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    ఒకవేళ (filter (&info[i]))
    {
      /* Merge clusters.
       * Same logic as delete_glyph(), but for in-place removal. */

      గుర్తులేని పూర్ణాంకం cluster = info[i].cluster;
      ఒకవేళ (i + 1 < లెక్క && cluster == info[i + 1].cluster)
	కొనసాగు; /* Cluster survives; do nothing. */

      ఒకవేళ (j)
      {
	/* Merge cluster backward. */
	ఒకవేళ (cluster < info[j - 1].cluster)
	{
	  గుర్తులేని పూర్ణాంకం ముసుగు = info[i].ముసుగు;
	  గుర్తులేని పూర్ణాంకం old_cluster = info[j - 1].cluster;
	  కొరకు (గుర్తులేని k = j; k && info[k - 1].cluster == old_cluster; k--)
	    సమితి_cluster (info[k - 1], cluster, ముసుగు);
	}
	కొనసాగు;
      }

      ఒకవేళ (i + 1 < లెక్క)
	విలీనం_clusters (i, i + 2); /* Merge cluster forward. */

      కొనసాగు;
    }

    ఒకవేళ (j != i)
    {
      info[j] = info[i];
      pos[j] = pos[i];
    }
    j++;
  }
  len = j;
}

శూన్య
hb_బఫరు_t::guess_segment_properties ()
{
  assert_unicode ();

  /* If script is set to INVALID, guess from buffer contents */
  ఒకవేళ (props.script == HB_SCRIPT_INVALID) {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < len; i++) {
      hb_script_t script = unicode->script (info[i].codepoint);
      ఒకవేళ (likely (script != HB_SCRIPT_COMMON &&
		  script != HB_SCRIPT_INHERITED &&
		  script != HB_SCRIPT_UNKNOWN)) {
	props.script = script;
	ఆపు;
      }
    }
  }

  /* If direction is set to INVALID, guess from script */
  ఒకవేళ (props.direction == HB_DIRECTION_INVALID) {
    props.direction = hb_script_get_horizontal_direction (props.script);
    ఒకవేళ (props.direction == HB_DIRECTION_INVALID)
      props.direction = HB_DIRECTION_LTR;
  }

  /* If language is not set, use default language from locale */
  ఒకవేళ (props.language == HB_LANGUAGE_INVALID) {
    /* TODO get_default_for_script? using $LANGUAGE */
    props.language = hb_language_get_default ();
  }
}


/* Public API */

DEFINE_NULL_INSTANCE (hb_బఫరు_t) =
{
  HB_OBJECT_శీర్షిక_STATIC,

  const_cast<hb_unicode_funcs_t *> (&_hb_Null_hb_unicode_funcs_t),
  HB_బఫరు_FLAG_DEFAULT,
  HB_బఫరు_CLUSTER_LEVEL_DEFAULT,
  HB_బఫరు_REPLACEMENT_CODEPOINT_DEFAULT,
  0, /* invisible */
  0, /* not_found */
  HB_CODEPOINT_INVALID, /* not_found_variation_selector */


  HB_బఫరు_CONTENT_TYPE_INVALID,
  HB_SEGMENT_PROPERTIES_DEFAULT,

  అబద్ధం, /* successful */
  అబద్ధం, /* have_output */
  నిజం  /* have_positions */

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
hb_బఫరు_t *
hb_బఫరు_create ()
{
  hb_బఫరు_t *బఫరు;

  ఒకవేళ (!(బఫరు = hb_object_create<hb_బఫరు_t> ()))
    తిరిగి hb_బఫరు_get_ఖాళీలు ();

  బఫరు->max_len = HB_బఫరు_MAX_LEN_DEFAULT;
  బఫరు->max_ops = HB_బఫరు_MAX_OPS_DEFAULT;

  బఫరు->reset ();

  తిరిగి బఫరు;
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
hb_బఫరు_t *
hb_బఫరు_create_similar (స్థిరరాశి hb_బఫరు_t *src)
{
  hb_బఫరు_t *బఫరు = hb_బఫరు_create ();

  బఫరు->similar (*src);

  తిరిగి బఫరు;
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
శూన్య
hb_బఫరు_reset (hb_బఫరు_t *బఫరు)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->reset ();
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
hb_బఫరు_t *
hb_బఫరు_get_ఖాళీలు ()
{
  తిరిగి const_cast<hb_బఫరు_t *> (&Null (hb_బఫరు_t));
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
hb_బఫరు_t *
hb_బఫరు_reference (hb_బఫరు_t *బఫరు)
{
  తిరిగి hb_object_reference (బఫరు);
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
శూన్య
hb_బఫరు_destroy (hb_బఫరు_t *బఫరు)
{
  ఒకవేళ (!hb_object_destroy (బఫరు)) తిరిగి;

  hb_unicode_funcs_destroy (బఫరు->unicode);

  hb_free (బఫరు->info);
  hb_free (బఫరు->pos);
#నిర్వచితం_కాకపోతే HB_NO_బఫరు_సందేశం
  ఒకవేళ (బఫరు->సందేశం_destroy)
    బఫరు->సందేశం_destroy (బఫరు->సందేశం_దత్తాంశం);
#అయితే_ముగింపు

  hb_free (బఫరు);
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
hb_బఫరు_సమితి_user_దత్తాంశం (hb_బఫరు_t        *బఫరు,
			 hb_user_దత్తాంశం_కీలకం_t *key,
			 శూన్య *              దత్తాంశం,
			 hb_destroy_func_t   destroy,
			 hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (బఫరు, key, దత్తాంశం, destroy, replace);
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
శూన్య *
hb_బఫరు_get_user_దత్తాంశం (స్థిరరాశి hb_బఫరు_t  *బఫరు,
			 hb_user_దత్తాంశం_కీలకం_t *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (బఫరు, key);
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
శూన్య
hb_బఫరు_సమితి_content_type (hb_బఫరు_t              *బఫరు,
			    hb_బఫరు_content_type_t  content_type)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->content_type = content_type;
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
hb_బఫరు_content_type_t
hb_బఫరు_get_content_type (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->content_type;
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
శూన్య
hb_బఫరు_సమితి_unicode_funcs (hb_బఫరు_t        *బఫరు,
			     hb_unicode_funcs_t *unicode_funcs)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  ఒకవేళ (!unicode_funcs)
    unicode_funcs = hb_unicode_funcs_get_default ();

  hb_unicode_funcs_reference (unicode_funcs);
  hb_unicode_funcs_destroy (బఫరు->unicode);
  బఫరు->unicode = unicode_funcs;
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
hb_బఫరు_get_unicode_funcs (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->unicode;
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
శూన్య
hb_బఫరు_సమితి_direction (hb_బఫరు_t    *బఫరు,
			 hb_direction_t  direction)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->props.direction = direction;
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
hb_బఫరు_get_direction (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->props.direction;
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
శూన్య
hb_బఫరు_సమితి_script (hb_బఫరు_t *బఫరు,
		      hb_script_t  script)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->props.script = script;
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
hb_బఫరు_get_script (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->props.script;
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
శూన్య
hb_బఫరు_సమితి_language (hb_బఫరు_t   *బఫరు,
			hb_language_t  language)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->props.language = language;
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
hb_బఫరు_get_language (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->props.language;
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
శూన్య
hb_బఫరు_సమితి_segment_properties (hb_బఫరు_t *బఫరు,
				  స్థిరరాశి hb_segment_properties_t *props)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->props = *props;
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
శూన్య
hb_బఫరు_get_segment_properties (స్థిరరాశి hb_బఫరు_t *బఫరు,
				  hb_segment_properties_t *props)
{
  *props = బఫరు->props;
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
శూన్య
hb_బఫరు_సమితి_flags (hb_బఫరు_t       *బఫరు,
		     hb_బఫరు_flags_t  flags)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->flags = flags;
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
hb_బఫరు_flags_t
hb_బఫరు_get_flags (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->flags;
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
శూన్య
hb_బఫరు_సమితి_cluster_level (hb_బఫరు_t               *బఫరు,
			     hb_బఫరు_cluster_level_t  cluster_level)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->cluster_level = cluster_level;
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
hb_బఫరు_cluster_level_t
hb_బఫరు_get_cluster_level (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->cluster_level;
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
శూన్య
hb_బఫరు_సమితి_replacement_codepoint (hb_బఫరు_t    *బఫరు,
				     hb_codepoint_t  replacement)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->replacement = replacement;
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
hb_బఫరు_get_replacement_codepoint (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->replacement;
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
శూన్య
hb_బఫరు_సమితి_invisible_glyph (hb_బఫరు_t    *బఫరు,
			       hb_codepoint_t  invisible)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->invisible = invisible;
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
hb_బఫరు_get_invisible_glyph (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->invisible;
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
శూన్య
hb_బఫరు_సమితి_not_దొరికింది_glyph (hb_బఫరు_t    *బఫరు,
			       hb_codepoint_t  not_దొరికింది)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->not_దొరికింది = not_దొరికింది;
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
hb_బఫరు_get_not_దొరికింది_glyph (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->not_దొరికింది;
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
శూన్య
hb_బఫరు_సమితి_not_దొరికింది_variation_selector_glyph (hb_బఫరు_t    *బఫరు,
						  hb_codepoint_t  not_దొరికింది_variation_selector)
{
  బఫరు->not_దొరికింది_variation_selector = not_దొరికింది_variation_selector;
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
hb_బఫరు_get_not_దొరికింది_variation_selector_glyph (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->not_దొరికింది_variation_selector;
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
శూన్య
hb_బఫరు_సమితి_random_state (hb_బఫరు_t    *బఫరు,
			    గుర్తులేని        state)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->random_state = state;
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
గుర్తులేని
hb_బఫరు_get_random_state (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->random_state;
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
శూన్య
hb_బఫరు_clear_contents (hb_బఫరు_t *బఫరు)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  బఫరు->clear ();
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
hb_బఫరు_pre_allocate (hb_బఫరు_t *బఫరు, గుర్తులేని పూర్ణాంకం కొలత)
{
  తిరిగి బఫరు->ensure (కొలత);
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
hb_బఫరు_కేటాయింపు_successful (hb_బఫరు_t  *బఫరు)
{
  తిరిగి బఫరు->successful;
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
శూన్య
hb_బఫరు_కూడు (hb_బఫరు_t    *బఫరు,
	       hb_codepoint_t  codepoint,
	       గుర్తులేని పూర్ణాంకం    cluster)
{
  బఫరు->కూడు (codepoint, cluster);
  బఫరు->clear_context (1);
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
hb_బఫరు_సమితి_నిడివి (hb_బఫరు_t  *బఫరు,
		      గుర్తులేని పూర్ణాంకం  నిడివి)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి నిడివి == 0;

  ఒకవేళ (unlikely (!బఫరు->ensure (నిడివి)))
    తిరిగి అబద్ధం;

  /* Wipe the new space */
  ఒకవేళ (నిడివి > బఫరు->len) {
    hb_memset (బఫరు->info + బఫరు->len, 0, పరిమాణం (బఫరు->info[0]) * (నిడివి - బఫరు->len));
    ఒకవేళ (బఫరు->have_positions)
      hb_memset (బఫరు->pos + బఫరు->len, 0, పరిమాణం (బఫరు->pos[0]) * (నిడివి - బఫరు->len));
  }

  బఫరు->len = నిడివి;

  ఒకవేళ (!నిడివి)
  {
    బఫరు->content_type = HB_బఫరు_CONTENT_TYPE_INVALID;
    బఫరు->clear_context (0);
  }
  బఫరు->clear_context (1);

  తిరిగి నిజం;
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
గుర్తులేని పూర్ణాంకం
hb_బఫరు_get_నిడివి (స్థిరరాశి hb_బఫరు_t *బఫరు)
{
  తిరిగి బఫరు->len;
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
hb_బఫరు_get_glyph_infos (hb_బఫరు_t  *బఫరు,
			   గుర్తులేని పూర్ణాంకం *నిడివి)
{
  ఒకవేళ (నిడివి)
    *నిడివి = బఫరు->len;

  తిరిగి (hb_glyph_info_t *) బఫరు->info;
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
hb_బఫరు_get_glyph_positions (hb_బఫరు_t  *బఫరు,
			       గుర్తులేని పూర్ణాంకం *నిడివి)
{
  ఒకవేళ (నిడివి)
    *నిడివి = బఫరు->len;

  ఒకవేళ (!బఫరు->have_positions)
  {
    ఒకవేళ (unlikely (బఫరు->సందేశం_లోతు))
      తిరిగి శూన్యసూచిక;

    బఫరు->clear_positions ();
  }

  తిరిగి (hb_glyph_position_t *) బఫరు->pos;
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
hb_బఫరు_has_positions (hb_బఫరు_t  *బఫరు)
{
  తిరిగి బఫరు->have_positions;
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
(hb_glyph_info_get_glyph_flags) (స్థిరరాశి hb_glyph_info_t *info)
{
  తిరిగి hb_glyph_info_get_glyph_flags (info);
}

/**
 * hb_buffer_reverse:
 * @buffer: An #hb_buffer_t
 *
 * Reverses buffer contents.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_బఫరు_తిరగేయి (hb_బఫరు_t *బఫరు)
{
  బఫరు->reverse ();
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
శూన్య
hb_బఫరు_తిరగేయి_range (hb_బఫరు_t *బఫరు,
			 గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర)
{
  బఫరు->తిరగేయి_range (ప్రారంభం, చివర);
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
శూన్య
hb_బఫరు_తిరగేయి_clusters (hb_బఫరు_t *బఫరు)
{
  బఫరు->తిరగేయి_clusters ();
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
శూన్య
hb_బఫరు_guess_segment_properties (hb_బఫరు_t *బఫరు)
{
  బఫరు->guess_segment_properties ();
}

మూస <రకం_పేరు utf_t>
స్థిర అంతఃవరుస శూన్య
hb_బఫరు_కూడు_utf (hb_బఫరు_t  *బఫరు,
		   స్థిరరాశి రకం_పేరు utf_t::codepoint_t *పాఠ్యం,
		   పూర్ణాంకం           పాఠ్యం_నిడివి,
		   గుర్తులేని పూర్ణాంకం  అంశం_స్థానభ్రంశం,
		   పూర్ణాంకం           అంశం_నిడివి)
{
  రకనిర్వచనం రకం_పేరు utf_t::codepoint_t T;
  స్థిరరాశి hb_codepoint_t replacement = బఫరు->replacement;

  బఫరు->assert_unicode ();

  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)))
    తిరిగి;

  ఒకవేళ (పాఠ్యం_నిడివి == -1)
    పాఠ్యం_నిడివి = utf_t::అక్షరశ్రేణి_పొడవు (పాఠ్యం);

  ఒకవేళ (అంశం_నిడివి == -1)
    అంశం_నిడివి = పాఠ్యం_నిడివి - అంశం_స్థానభ్రంశం;

  అంశం_స్థానభ్రంశం = hb_min (అంశం_స్థానభ్రంశం, (గుర్తులేని) పాఠ్యం_నిడివి);
  అంశం_నిడివి = hb_clamp (అంశం_నిడివి, 0, పాఠ్యం_నిడివి - (పూర్ణాంకం) అంశం_స్థానభ్రంశం);

  ఒకవేళ (unlikely (అంశం_నిడివి < 0 ||
		అంశం_నిడివి > గరిష్ఠ_పూర్ణాంకం / 8 ||
		!బఫరు->ensure (బఫరు->len + అంశం_నిడివి * పరిమాణం (T) / 4)))
    తిరిగి;

  /* If buffer is empty and pre-context provided, install it.
   * This check is written this way, to make sure people can
   * provide pre-context in one add_utf() call, then provide
   * text in a follow-up call.  See:
   *
   * https://bugzilla.mozilla.org/show_bug.cgi?id=801410#c13
   */
  ఒకవేళ (!బఫరు->len && అంశం_స్థానభ్రంశం > 0)
  {
    /* Add pre-context */
    బఫరు->clear_context (0);
    స్థిరరాశి T *prev = పాఠ్యం + అంశం_స్థానభ్రంశం;
    స్థిరరాశి T *ప్రారంభం = పాఠ్యం;
    అయితే (ప్రారంభం < prev && బఫరు->context_len[0] < బఫరు->CONTEXT_నిడివి)
    {
      hb_codepoint_t u;
      prev = utf_t::prev (prev, ప్రారంభం, &u, replacement);
      బఫరు->context[0][బఫరు->context_len[0]++] = u;
    }
  }

  స్థిరరాశి T *next = పాఠ్యం + అంశం_స్థానభ్రంశం;
  స్థిరరాశి T *చివర = next + అంశం_నిడివి;
  అయితే (next < చివర)
  {
    hb_codepoint_t u;
    స్థిరరాశి T *old_తదుపరి = next;
    next = utf_t::next (next, చివర, &u, replacement);
    బఫరు->కూడు (u, old_తదుపరి - (స్థిరరాశి T *) పాఠ్యం);
  }

  /* Add post-context */
  బఫరు->clear_context (1);
  చివర = పాఠ్యం + పాఠ్యం_నిడివి;
  అయితే (next < చివర && బఫరు->context_len[1] < బఫరు->CONTEXT_నిడివి)
  {
    hb_codepoint_t u;
    next = utf_t::next (next, చివర, &u, replacement);
    బఫరు->context[1][బఫరు->context_len[1]++] = u;
  }

  బఫరు->content_type = HB_బఫరు_CONTENT_TYPE_UNICODE;
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
శూన్య
hb_బఫరు_కూడు_utf8 (hb_బఫరు_t  *బఫరు,
		    స్థిరరాశి అక్షరం   *పాఠ్యం,
		    పూర్ణాంకం           పాఠ్యం_నిడివి,
		    గుర్తులేని పూర్ణాంకం  అంశం_స్థానభ్రంశం,
		    పూర్ణాంకం           అంశం_నిడివి)
{
  hb_బఫరు_కూడు_utf<hb_utf8_t> (బఫరు, (స్థిరరాశి uint8_t *) పాఠ్యం, పాఠ్యం_నిడివి, అంశం_స్థానభ్రంశం, అంశం_నిడివి);
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
శూన్య
hb_బఫరు_కూడు_utf16 (hb_బఫరు_t    *బఫరు,
		     స్థిరరాశి uint16_t *పాఠ్యం,
		     పూర్ణాంకం             పాఠ్యం_నిడివి,
		     గుర్తులేని పూర్ణాంకం    అంశం_స్థానభ్రంశం,
		     పూర్ణాంకం             అంశం_నిడివి)
{
  hb_బఫరు_కూడు_utf<hb_utf16_t> (బఫరు, పాఠ్యం, పాఠ్యం_నిడివి, అంశం_స్థానభ్రంశం, అంశం_నిడివి);
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
శూన్య
hb_బఫరు_కూడు_utf32 (hb_బఫరు_t    *బఫరు,
		     స్థిరరాశి uint32_t *పాఠ్యం,
		     పూర్ణాంకం             పాఠ్యం_నిడివి,
		     గుర్తులేని పూర్ణాంకం    అంశం_స్థానభ్రంశం,
		     పూర్ణాంకం             అంశం_నిడివి)
{
  hb_బఫరు_కూడు_utf<hb_utf32_t> (బఫరు, పాఠ్యం, పాఠ్యం_నిడివి, అంశం_స్థానభ్రంశం, అంశం_నిడివి);
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
శూన్య
hb_బఫరు_కూడు_latin1 (hb_బఫరు_t   *బఫరు,
		      స్థిరరాశి uint8_t *పాఠ్యం,
		      పూర్ణాంకం            పాఠ్యం_నిడివి,
		      గుర్తులేని పూర్ణాంకం   అంశం_స్థానభ్రంశం,
		      పూర్ణాంకం            అంశం_నిడివి)
{
  hb_బఫరు_కూడు_utf<hb_latin1_t> (బఫరు, పాఠ్యం, పాఠ్యం_నిడివి, అంశం_స్థానభ్రంశం, అంశం_నిడివి);
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
శూన్య
hb_బఫరు_కూడు_codepoints (hb_బఫరు_t          *బఫరు,
			  స్థిరరాశి hb_codepoint_t *పాఠ్యం,
			  పూర్ణాంకం                   పాఠ్యం_నిడివి,
			  గుర్తులేని పూర్ణాంకం          అంశం_స్థానభ్రంశం,
			  పూర్ణాంకం                   అంశం_నిడివి)
{
  hb_బఫరు_కూడు_utf<hb_utf32_novalidate_t> (బఫరు, పాఠ్యం, పాఠ్యం_నిడివి, అంశం_స్థానభ్రంశం, అంశం_నిడివి);
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
HB_EXTERN శూన్య
hb_బఫరు_append (hb_బఫరు_t *బఫరు,
		  స్థిరరాశి hb_బఫరు_t *ఆధారం,
		  గుర్తులేని పూర్ణాంకం ప్రారంభం,
		  గుర్తులేని పూర్ణాంకం చివర)
{
  ప్రతిజ్ఞా (!బఫరు->have_output && !ఆధారం->have_output);
  ప్రతిజ్ఞా (బఫరు->have_positions == ఆధారం->have_positions ||
	  !బఫరు->len || !ఆధారం->len);
  ప్రతిజ్ఞా (బఫరు->content_type == ఆధారం->content_type ||
	  !బఫరు->len || !ఆధారం->len);

  ఒకవేళ (చివర > ఆధారం->len)
    చివర = ఆధారం->len;
  ఒకవేళ (ప్రారంభం > చివర)
    ప్రారంభం = చివర;
  ఒకవేళ (ప్రారంభం == చివర)
    తిరిగి;

  ఒకవేళ (బఫరు->len + (చివర - ప్రారంభం) < బఫరు->len) /* Overflows. */
  {
    బఫరు->successful = అబద్ధం;
    తిరిగి;
  }

  గుర్తులేని పూర్ణాంకం orig_len = బఫరు->len;
  hb_బఫరు_సమితి_నిడివి (బఫరు, బఫరు->len + (చివర - ప్రారంభం));
  ఒకవేళ (unlikely (!బఫరు->successful))
    తిరిగి;

  ఒకవేళ (!orig_len)
    బఫరు->content_type = ఆధారం->content_type;
  ఒకవేళ (!బఫరు->have_positions && ఆధారం->have_positions)
    బఫరు->clear_positions ();

  hb_segment_properties_overlay (&బఫరు->props, &ఆధారం->props);

  hb_memcpy (బఫరు->info + orig_len, ఆధారం->info + ప్రారంభం, (చివర - ప్రారంభం) * పరిమాణం (బఫరు->info[0]));
  ఒకవేళ (బఫరు->have_positions)
    hb_memcpy (బఫరు->pos + orig_len, ఆధారం->pos + ప్రారంభం, (చివర - ప్రారంభం) * పరిమాణం (బఫరు->pos[0]));

  ఒకవేళ (ఆధారం->content_type == HB_బఫరు_CONTENT_TYPE_UNICODE)
  {
    /* See similar logic in add_utf. */

    /* pre-context */
    ఒకవేళ (!orig_len && ప్రారంభం + ఆధారం->context_len[0] > 0)
    {
      బఫరు->clear_context (0);
      అయితే (ప్రారంభం > 0 && బఫరు->context_len[0] < బఫరు->CONTEXT_నిడివి)
	బఫరు->context[0][బఫరు->context_len[0]++] = ఆధారం->info[--ప్రారంభం].codepoint;
      కొరకు (స్వయంచాలక i = 0u; i < ఆధారం->context_len[0] && బఫరు->context_len[0] < బఫరు->CONTEXT_నిడివి; i++)
	బఫరు->context[0][బఫరు->context_len[0]++] = ఆధారం->context[0][i];
    }

    /* post-context */
    బఫరు->clear_context (1);
    అయితే (చివర < ఆధారం->len && బఫరు->context_len[1] < బఫరు->CONTEXT_నిడివి)
      బఫరు->context[1][బఫరు->context_len[1]++] = ఆధారం->info[చివర++].codepoint;
    కొరకు (స్వయంచాలక i = 0u; i < ఆధారం->context_len[1] && బఫరు->context_len[1] < బఫరు->CONTEXT_నిడివి; i++)
      బఫరు->context[1][బఫరు->context_len[1]++] = ఆధారం->context[1][i];
  }
}


స్థిర పూర్ణాంకం
compare_info_codepoint (స్థిరరాశి hb_glyph_info_t *pa,
			స్థిరరాశి hb_glyph_info_t *pb)
{
  తిరిగి (పూర్ణాంకం) pb->codepoint - (పూర్ణాంకం) pa->codepoint;
}

స్థిర అంతఃవరుస శూన్య
normalize_glyphs_cluster (hb_బఫరు_t *బఫరు,
			  గుర్తులేని పూర్ణాంకం ప్రారంభం,
			  గుర్తులేని పూర్ణాంకం చివర,
			  తార్కిక backward)
{
  hb_glyph_position_t *pos = బఫరు->pos;

  /* Total cluster advance */
  hb_position_t మొత్తం_x_advance = 0, మొత్తం_y_advance = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
  {
    మొత్తం_x_advance = hb_saturate_కూడు (మొత్తం_x_advance, pos[i].x_advance);
    మొత్తం_y_advance = hb_saturate_కూడు (మొత్తం_y_advance, pos[i].y_advance);
  }

  hb_position_t x_advance = 0, y_advance = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం; i < చివర; i++)
  {
    pos[i].x_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[i].x_స్థానభ్రంశం_2, x_advance);
    pos[i].y_స్థానభ్రంశం_2 = hb_saturate_కూడు (pos[i].y_స్థానభ్రంశం_2, y_advance);

    x_advance = hb_saturate_కూడు (x_advance, pos[i].x_advance);
    y_advance = hb_saturate_కూడు (y_advance, pos[i].y_advance);

    pos[i].x_advance = 0;
    pos[i].y_advance = 0;
  }

  ఒకవేళ (backward)
  {
    /* Transfer all cluster advance to the last glyph. */
    pos[చివర - 1].x_advance = మొత్తం_x_advance;
    pos[చివర - 1].y_advance = మొత్తం_y_advance;

    hb_stable_క్రమీకరించు (బఫరు->info + ప్రారంభం, చివర - ప్రారంభం - 1, compare_info_codepoint, బఫరు->pos + ప్రారంభం);
  } లేకపోతే {
    /* Transfer all cluster advance to the first glyph. */
    pos[ప్రారంభం].x_advance = hb_saturate_కూడు (pos[ప్రారంభం].x_advance, మొత్తం_x_advance);
    pos[ప్రారంభం].y_advance = hb_saturate_కూడు (pos[ప్రారంభం].y_advance, మొత్తం_y_advance);
    కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం + 1; i < చివర; i++) {
      pos[i].x_స్థానభ్రంశం_2 = hb_saturate_sub (pos[i].x_స్థానభ్రంశం_2, మొత్తం_x_advance);
      pos[i].y_స్థానభ్రంశం_2 = hb_saturate_sub (pos[i].y_స్థానభ్రంశం_2, మొత్తం_y_advance);
    }
    hb_stable_క్రమీకరించు (బఫరు->info + ప్రారంభం + 1, చివర - ప్రారంభం - 1, compare_info_codepoint, బఫరు->pos + ప్రారంభం + 1);
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
శూన్య
hb_బఫరు_normalize_glyphs (hb_బఫరు_t *బఫరు)
{
  ప్రతిజ్ఞా (బఫరు->have_positions);

  బఫరు->assert_glyphs ();

  తార్కిక backward = HB_DIRECTION_IS_BACKWARD (బఫరు->props.direction);

  foreach_cluster (బఫరు, ప్రారంభం, చివర)
    normalize_glyphs_cluster (బఫరు, ప్రారంభం, చివర, backward);
}

శూన్య
hb_బఫరు_t::sort (గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని పూర్ణాంకం చివర, పూర్ణాంకం(*compar)(స్థిరరాశి hb_glyph_info_t *, స్థిరరాశి hb_glyph_info_t *))
{
  ప్రతిజ్ఞా (!have_positions);
  కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం + 1; i < చివర; i++)
  {
    గుర్తులేని పూర్ణాంకం j = i;
    అయితే (j > ప్రారంభం && compar (&info[j - 1], &info[i]) > 0)
      j--;
    ఒకవేళ (i == j)
      కొనసాగు;
    /* Move item i to occupy place for item j, shift what's in between. */
    విలీనం_clusters (j, i + 1);
    {
      hb_glyph_info_t t = info[i];
      memmove (&info[j + 1], &info[j], (i - j) * పరిమాణం (hb_glyph_info_t));
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
hb_బఫరు_diff_flags_t
hb_బఫరు_diff (hb_బఫరు_t *బఫరు,
		hb_బఫరు_t *reference,
		hb_codepoint_t dottedcircle_glyph,
		గుర్తులేని పూర్ణాంకం position_fuzz)
{
  ఒకవేళ (బఫరు->content_type != reference->content_type && బఫరు->len && reference->len)
    తిరిగి HB_బఫరు_DIFF_FLAG_CONTENT_TYPE_MISMATCH;

  hb_బఫరు_diff_flags_t ఫలితం = HB_బఫరు_DIFF_FLAG_EQUAL;
  తార్కిక కలిగియుంది = dottedcircle_glyph != (hb_codepoint_t) -1;

  గుర్తులేని పూర్ణాంకం లెక్క = reference->len;

  ఒకవేళ (బఫరు->len != లెక్క)
  {
    /*
     * we can't compare glyph-by-glyph, but we do want to know if there
     * are .notdef or dottedcircle glyphs present in the reference buffer
     */
    స్థిరరాశి hb_glyph_info_t *info = reference->info;
    గుర్తులేని పూర్ణాంకం i;
    కొరకు (i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (కలిగియుంది && info[i].codepoint == dottedcircle_glyph)
	ఫలితం |= HB_బఫరు_DIFF_FLAG_DOTTED_CIRCLE_PRESENT;
      ఒకవేళ (కలిగియుంది && info[i].codepoint == 0)
	ఫలితం |= HB_బఫరు_DIFF_FLAG_NOTDEF_PRESENT;
    }
    ఫలితం |= HB_బఫరు_DIFF_FLAG_నిడివి_MISMATCH;
    తిరిగి hb_బఫరు_diff_flags_t (ఫలితం);
  }

  ఒకవేళ (!లెక్క)
    తిరిగి hb_బఫరు_diff_flags_t (ఫలితం);

  స్థిరరాశి hb_glyph_info_t *buf_info = బఫరు->info;
  స్థిరరాశి hb_glyph_info_t *ref_info = reference->info;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    ఒకవేళ (buf_info->codepoint != ref_info->codepoint)
      ఫలితం |= HB_బఫరు_DIFF_FLAG_CODEPOINT_MISMATCH;
    ఒకవేళ (buf_info->cluster != ref_info->cluster)
      ఫలితం |= HB_బఫరు_DIFF_FLAG_CLUSTER_MISMATCH;
    ఒకవేళ ((buf_info->ముసుగు ^ ref_info->ముసుగు) & HB_GLYPH_FLAG_DEFINED)
      ఫలితం |= HB_బఫరు_DIFF_FLAG_GLYPH_FLAGS_MISMATCH;
    ఒకవేళ (కలిగియుంది && ref_info->codepoint == dottedcircle_glyph)
      ఫలితం |= HB_బఫరు_DIFF_FLAG_DOTTED_CIRCLE_PRESENT;
    ఒకవేళ (కలిగియుంది && ref_info->codepoint == 0)
      ఫలితం |= HB_బఫరు_DIFF_FLAG_NOTDEF_PRESENT;
    buf_info++;
    ref_info++;
  }

  ఒకవేళ (బఫరు->content_type == HB_బఫరు_CONTENT_TYPE_GLYPHS)
  {
    స్వయంచాలక position_diff = [] (hb_position_t a, hb_position_t b) -> uint64_t
    {
      int64_t diff = (int64_t) a - b;
      తిరిగి diff < 0 ? -diff : diff;
    };
    ప్రతిజ్ఞా (బఫరు->have_positions);
    స్థిరరాశి hb_glyph_position_t *buf_pos = బఫరు->pos;
    స్థిరరాశి hb_glyph_position_t *ref_pos = reference->pos;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (position_diff (buf_pos->x_advance, ref_pos->x_advance) > position_fuzz ||
	  position_diff (buf_pos->y_advance, ref_pos->y_advance) > position_fuzz ||
	  position_diff (buf_pos->x_స్థానభ్రంశం_2, ref_pos->x_స్థానభ్రంశం_2) > position_fuzz ||
	  position_diff (buf_pos->y_స్థానభ్రంశం_2, ref_pos->y_స్థానభ్రంశం_2) > position_fuzz)
      {
	ఫలితం |= HB_బఫరు_DIFF_FLAG_POSITION_MISMATCH;
	ఆపు;
      }
      buf_pos++;
      ref_pos++;
    }
  }

  తిరిగి ఫలితం;
}


/*
 * Debugging.
 */

శూన్య
hb_బఫరు_t::మారింది ()
{
#నిర్వచితమైతే HB_NO_బఫరు_సందేశం
  తిరిగి;
#లేకపోతే
  ఒకవేళ (!సందేశం_లోతు)
    తిరిగి;

  ఒకవేళ (మారింది_func)
    మారింది_func (ఇది, మారింది_దత్తాంశం);
  లేకపోతే
    update_digest ();
#అయితే_ముగింపు
}

#నిర్వచితం_కాకపోతే HB_NO_బఫరు_సందేశం
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
శూన్య
hb_బఫరు_సమితి_సందేశం_func (hb_బఫరు_t *బఫరు,
			    hb_బఫరు_సందేశం_func_t func,
			    శూన్య *user_దత్తాంశం, hb_destroy_func_t destroy)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (బఫరు)) ||
      unlikely (బఫరు->సందేశం_లోతు))
  {
    ఒకవేళ (destroy)
      destroy (user_దత్తాంశం);
    తిరిగి;
  }

  ఒకవేళ (బఫరు->సందేశం_destroy)
    బఫరు->సందేశం_destroy (బఫరు->సందేశం_దత్తాంశం);

  ఒకవేళ (func) {
    బఫరు->సందేశం_func = func;
    బఫరు->సందేశం_దత్తాంశం = user_దత్తాంశం;
    బఫరు->సందేశం_destroy = destroy;
  } లేకపోతే {
    బఫరు->సందేశం_func = శూన్యసూచిక;
    బఫరు->సందేశం_దత్తాంశం = శూన్యసూచిక;
    బఫరు->సందేశం_destroy = శూన్యసూచిక;
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
శూన్య
hb_బఫరు_మారింది (hb_బఫరు_t *బఫరు)
{
  బఫరు->మారింది ();
}

తార్కిక
hb_బఫరు_t::సందేశం_impl (hb_font_t *font, స్థిరరాశి అక్షరం *fmt, va_list ap)
{
  ప్రతిజ్ఞా (!have_output || (బహిర్గతం_info == info && బహిర్గతం_len == idx));

  సందేశం_లోతు++;

  అక్షరం buf[100];
  vsnprintf (buf, పరిమాణం (buf), fmt, ap);
  తార్కిక ret = (తార్కిక) ఇది->సందేశం_func (ఇది, font, buf, ఇది->సందేశం_దత్తాంశం);

  సందేశం_లోతు--;

  తిరిగి ret;
}
#అయితే_ముగింపు
