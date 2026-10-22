/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Garret Rieger, Rod Sheeter, Behdad Esfahbod
 */

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-open-file.hh"

#அந்தர்பாவ "hb-subset.hh"
#அந்தர்பாவ "hb-subset-table.hh"
#அந்தர்பாவ "hb-subset-accelerator.hh"

#அந்தர்பாவ "hb-ot-cmap-table.hh"
#அந்தர்பாவ "hb-ot-var-cvar-table.hh"
#அந்தர்பாவ "hb-ot-head-table.hh"
#அந்தர்பாவ "hb-ot-stat-table.hh"
#அந்தர்பாவ "hb-ot-post-table-v2subset.hh"


/**
 * SECTION:hb-subset
 * @title: hb-subset
 * @short_description: Subsets font files.
 * @include: hb-subset.h
 *
 * Subsetting reduces the codepoint coverage of font files and removes all data
 * that is no longer needed. A subset input describes the desired subset. The input is
 * provided along with a font to the subsetting operation. Output is a new font file
 * containing only the data specified in the input.
 *
 * Currently most outline and bitmap tables are supported: glyf, CFF, CFF2, sbix,
 * COLR, and CBDT/CBLC. This also includes fonts with variable outlines via OpenType
 * variations. Notably EBDT/EBLC and SVG are not supported. Layout subsetting is supported
 * only for OpenType Layout tables (GSUB, GPOS, GDEF). Notably subsetting of graphite or AAT tables
 * is not yet supported.
 *
 * Fonts with graphite or AAT tables may still be subsetted but will likely need to use the
 * retain glyph ids option and configure the subset to pass through the layout tables untouched.
 */


hb_user_data_சாவி_t hb_subset_accelerator_user_data_சாவி = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
நிலையான hb_tag_t known_tables[] {
  HB_TAG('a','v','a','r'),
  HB_TAG('B','A','S','E'),
  HB_TAG('C','B','D','T'),
  HB_TAG('C','B','L','C'),
  HB_TAG('C','F','F',' '),
  HB_TAG('C','F','F','2'),
  HB_TAG('c','m','a','p'),
  HB_TAG('C','O','L','R'),
  HB_TAG('C','P','A','L'),
  HB_TAG('c','v','a','r'),
  HB_TAG('c','v','t',' '),
  HB_TAG('D','S','I','G'),
  HB_TAG('E','B','D','T'),
  HB_TAG('E','B','L','C'),
  HB_TAG('E','B','S','C'),
  HB_TAG('f','p','g','m'),
  HB_TAG('f','v','a','r'),
  HB_TAG('g','a','s','p'),
  HB_TAG('G','D','E','F'),
  HB_TAG('g','l','y','f'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('G','L','Y','F'),
#endif
  HB_TAG('G','P','O','S'),
  HB_TAG('G','S','U','B'),
  HB_TAG('g','v','a','r'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('G','V','A','R'),
#endif
  HB_TAG('h','d','m','x'),
  HB_TAG('h','e','a','d'),
  HB_TAG('h','h','e','a'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('H','H','E','A'),
#endif
  HB_TAG('h','m','t','x'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('H','M','T','X'),
#endif
  HB_TAG('H','V','A','R'),
  HB_TAG('J','S','T','F'),
  HB_TAG('k','e','r','n'),
  HB_TAG('l','o','c','a'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('L','O','C','A'),
#endif
  HB_TAG('L','T','S','H'),
  HB_TAG('M','A','T','H'),
  HB_TAG('m','a','x','p'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('M','A','X','P'),
#endif
  HB_TAG('M','E','R','G'),
  HB_TAG('m','e','t','a'),
  HB_TAG('M','V','A','R'),
  HB_TAG('P','C','L','T'),
  HB_TAG('p','o','s','t'),
  HB_TAG('p','r','e','p'),
  HB_TAG('s','b','i','x'),
  HB_TAG('S','T','A','T'),
  HB_TAG('S','V','G',' '),
  HB_TAG('V','D','M','X'),
  HB_TAG('v','h','e','a'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('V','H','E','A'),
#endif
  HB_TAG('v','m','t','x'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('V','M','T','X'),
#endif
  HB_TAG('V','O','R','G'),
  HB_TAG('V','A','R','C'),
  HB_TAG('V','V','A','R'),
  HB_TAG('n','a','m','e'),
  HB_TAG('O','S','/','2')
};

நிலையான தருக்க _table_is_empty (மாறிலி hb_face_t *face, hb_tag_t tag)
{
  hb_blob_t* blob = hb_face_reference_table (face, tag);
  தருக்க result = (blob == hb_blob_get_empty ());
  hb_blob_destroy (blob);
  திரும்பு result;
}

நிலையான unsigned முழு
_get_table_tags (மாறிலி hb_subset_plan_t* plan,
                 unsigned முழு  start_offset,
                 unsigned முழு *table_மொத்தம்_2, /* IN/OUT */
                 hb_tag_t     *table_tags /* OUT */)
{
  unsigned இலக்கம்_tables = hb_face_get_table_tags (plan->source, 0, nullptr, nullptr);
  எனில் (இலக்கம்_tables)
    திரும்பு hb_face_get_table_tags (plan->source, start_offset, table_மொத்தம்_2, table_tags);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  auto it =
      hb_concat (
          + hb_array (known_tables)
          | hb_filter ([&] (hb_tag_t tag) {
            திரும்பு !_table_is_empty (plan->source, tag) && !plan->no_subset_tables.has (tag);
          })
          | hb_map ([] (hb_tag_t tag) -> hb_tag_t { திரும்பு tag; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_tag_t tag) {
            திரும்பு !_table_is_empty (plan->source, tag);
          }));

  it += start_offset;

  unsigned இலக்கம்_written = 0;
  போது (தருக்க (it) && இலக்கம்_written < *table_மொத்தம்_2)
    table_tags[இலக்கம்_written++] = *it++;

  *table_மொத்தம்_2 = இலக்கம்_written;
  திரும்பு இலக்கம்_written;
}


நிலையான தருக்க
_is_table_present (hb_face_t *source, hb_tag_t tag)
{

  எனில் (!hb_face_get_table_tags (source, 0, nullptr, nullptr)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    திரும்பு !_table_is_empty (source, tag);
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, இலக்கம்_tables = ARRAY_LENGTH (table_tags);
  போது (((வெறுமை) hb_face_get_table_tags (source, offset, &இலக்கம்_tables, table_tags), இலக்கம்_tables))
  {
    க்கு (unsigned i = 0; i < இலக்கம்_tables; ++i)
      எனில் (table_tags[i] == tag)
	திரும்பு உண்மை;
    offset += இலக்கம்_tables;
  }
  திரும்பு பொய்;
}

நிலையான தருக்க
_should_drop_table (hb_subset_plan_t *plan, hb_tag_t tag)
{
  எனில் (plan->drop_tables.has (tag))
    திரும்பு உண்மை;

  தேர்வு (tag)
  {
  நிலை HB_TAG('c','v','a','r'): /* hint table, fallthrough */
    திரும்பு plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  நிலை HB_TAG('c','v','t',' '): /* hint table, fallthrough */
  நிலை HB_TAG('f','p','g','m'): /* hint table, fallthrough */
  நிலை HB_TAG('p','r','e','p'): /* hint table, fallthrough */
  நிலை HB_TAG('h','d','m','x'): /* hint table, fallthrough */
  நிலை HB_TAG('V','D','M','X'): /* hint table, fallthrough */
    திரும்பு plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#ifdef HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  நிலை HB_TAG('G','D','E','F'):
  நிலை HB_TAG('G','P','O','S'):
  நிலை HB_TAG('G','S','U','B'):
  நிலை HB_TAG('m','o','r','x'):
  நிலை HB_TAG('m','o','r','t'):
  நிலை HB_TAG('k','e','r','x'):
  நிலை HB_TAG('k','e','r','n'):
    திரும்பு உண்மை;
#endif

  நிலை HB_TAG('a','v','a','r'):
  நிலை HB_TAG('f','v','a','r'):
  நிலை HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  நிலை HB_TAG('G','V','A','R'):
#endif
  நிலை HB_TAG('H','V','A','R'):
  நிலை HB_TAG('V','V','A','R'):
  நிலை HB_TAG('M','V','A','R'):
    திரும்பு plan->all_axes_pinned;

  இயல்பு:
    திரும்பு பொய்;
  }
}

நிலையான தருக்க
_dependencies_satisfied (hb_subset_plan_t *plan, hb_tag_t tag,
                         மாறிலி hb_set_t &subsetted_tags,
                         மாறிலி hb_set_t &pending_subset_tags)
{
  தருக்க outline_pending = pending_subset_tags.has (HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
  outline_pending |= pending_subset_tags.has (HB_TAG('G','L','Y','F'));
#endif

  தேர்வு (tag)
  {
  நிலை HB_TAG('h','m','t','x'):
  நிலை HB_TAG('v','m','t','x'):
  நிலை HB_TAG('m','a','x','p'):
#ifndef HB_NO_BEYOND_64K
  நிலை HB_TAG('H','M','T','X'):
  நிலை HB_TAG('V','M','T','X'):
  நிலை HB_TAG('M','A','X','P'):
#endif
  நிலை HB_TAG('O','S','/','2'):
    திரும்பு !plan->normalized_coords || !outline_pending;
  நிலை HB_TAG('G','P','O','S'):
  நிலை HB_TAG('G','S','U','B'):
    திரும்பு plan->all_axes_pinned || !pending_subset_tags.has (HB_TAG('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  நிலை HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  நிலை HB_TAG('G','V','A','R'):
#endif
  நிலை HB_TAG('c','v','a','r'):
  நிலை HB_TAG('H','V','A','R'):
  நிலை HB_TAG('V','V','A','R'):
  நிலை HB_TAG('M','V','A','R'):
  நிலை HB_TAG('G','D','E','F'):
  நிலை HB_TAG('C','F','F','2'):
    திரும்பு !plan->has_avar2 || !pending_subset_tags.has (HB_TAG('a','v','a','r'));
  இயல்பு:
    திரும்பு உண்மை;
  }
}

நிலையான தருக்க
_subset_table (hb_subset_plan_t *plan,
	       hb_vector_t<எழுத்து> &buf,
	       hb_tag_t tag)
{
  எனில் (plan->no_subset_tables.has (tag)) {
    திரும்பு _hb_subset_table_passthrough (plan, tag);
  }

  DEBUG_செய்தி (SUBSET, nullptr, "subset %c%c%c%c", HB_UNTAG (tag));

  தருக்க success;
  எனில் (_hb_subset_table_layout (plan, buf, tag, &success) ||
      _hb_subset_table_var (plan, buf, tag, &success) ||
      _hb_subset_table_cff (plan, buf, tag, &success) ||
      _hb_subset_table_color (plan, buf, tag, &success) ||
      _hb_subset_table_other (plan, buf, tag, &success))
    திரும்பு success;


  தேர்வு (tag)
  {
  நிலை HB_TAG('h','e','a','d'):
  {
    தருக்க outline_table_handles_head =
	_is_table_present (plan->source, HB_TAG('g','l','y','f')) &&
	!_should_drop_table (plan, HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
    outline_table_handles_head |=
	_is_table_present (plan->source, HB_TAG('G','L','Y','F')) &&
	!_should_drop_table (plan, HB_TAG('G','L','Y','F'));
#endif
    எனில் (outline_table_handles_head)
      திரும்பு உண்மை; /* skip head, handled by the outline table */
    திரும்பு _hb_subset_table<மாறிலி OT::head> (plan, buf);
  }

  நிலை HB_TAG('S','T','A','T'):
    எனில் (!plan->user_axes_location.is_empty ()) திரும்பு _hb_subset_table<மாறிலி OT::STAT> (plan, buf);
    இல்லையெனில் திரும்பு _hb_subset_table_passthrough (plan, tag);

  நிலை HB_TAG('c','v','t',' '):
#ifndef HB_NO_VAR
    எனில் (_is_table_present (plan->source, HB_TAG('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      auto &cvar = *plan->source->table.cvar;
      திரும்பு OT::cvar::add_cvt_and_apply_deltas (plan, cvar.get_tuple_var_data (), &cvar);
    }
#endif
    திரும்பு _hb_subset_table_passthrough (plan, tag);
  }

  எனில் (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    திரும்பு _hb_subset_table_passthrough (plan, tag);

  // Drop table
  திரும்பு உண்மை;
}

நிலையான வெறுமை _attach_accelerator_data (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  எனில் (!plan->inprogress_accelerator) திரும்பு;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = nullptr;

  எனில் (accel->in_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    திரும்பு;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_table<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  எனில் (!hb_face_set_user_data(face,
                             hb_subset_accelerator_t::user_data_சாவி(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             உண்மை))
    hb_subset_accelerator_t::destroy (accel);
}

/**
 * hb_subset_or_fail:
 * @source: font face data to be subset.
 * @input: input to use for the subsetting.
 *
 * Subsets a font according to provided input. Returns nullptr
 * if the subset operation fails or the face has no glyphs.
 *
 * Since: 2.9.0
 **/
hb_face_t *
hb_subset_or_fail (hb_face_t *source, மாறிலி hb_subset_உள்ளீடு_t *உள்ளீடு)
{
  எனில் (unlikely (!உள்ளீடு || !source)) திரும்பு nullptr;

  எனில் (unlikely (!source->get_இலக்கம்_glyphs ()))
  {
    DEBUG_செய்தி (SUBSET, nullptr, "No glyphs in source font.");
    திரும்பு nullptr;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (source, உள்ளீடு);
  எனில் (unlikely (!plan)) {
    திரும்பு nullptr;
  }

  hb_face_t * result = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  திரும்பு result;
}


/**
 * hb_subset_plan_execute_or_fail:
 * @plan: a subsetting plan.
 *
 * Executes the provided subsetting @plan.
 *
 * Return value:
 * on success returns a reference to generated font subset. If the subsetting operation fails
 * returns nullptr.
 *
 * Since: 4.0.0
 **/
hb_face_t *
hb_subset_plan_execute_or_fail (hb_subset_plan_t *plan)
{
  எனில் (unlikely (!plan || plan->in_error ())) {
    திரும்பு nullptr;
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, இலக்கம்_tables = ARRAY_LENGTH (table_tags);

  hb_set_t subsetted_tags, pending_subset_tags;
  போது (((வெறுமை) _get_table_tags (plan, offset, &இலக்கம்_tables, table_tags), இலக்கம்_tables))
  {
    க்கு (unsigned i = 0; i < இலக்கம்_tables; ++i)
    {
      hb_tag_t tag = table_tags[i];
      எனில் (_should_drop_table (plan, tag)) தொடர்;
      pending_subset_tags.add (tag);
    }

    offset += இலக்கம்_tables;
  }

  தருக்க success = உண்மை;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_vector_t<எழுத்து> buf;
    buf.alloc (8192 - 16);

    போது (!pending_subset_tags.is_empty ())
    {
      எனில் (subsetted_tags.in_error ()
	  || pending_subset_tags.in_error ()) {
	success = பொய்;
	செல் end;
      }

      தருக்க made_changes = பொய்;
      க்கு (hb_tag_t tag : pending_subset_tags)
      {
	எனில் (!_dependencies_satisfied (plan, tag,
				      subsetted_tags,
				      pending_subset_tags))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  தொடர்;
	}

	pending_subset_tags.del (tag);
	subsetted_tags.add (tag);
	made_changes = உண்மை;

	success = _subset_table (plan, buf, tag);
	எனில் (unlikely (!success)) செல் end;
      }

      எனில் (!made_changes)
      {
	DEBUG_செய்தி (SUBSET, nullptr, "Table dependencies unable to be satisfied. Subset failed.");
	success = பொய்;
	செல் end;
      }
    }
  }

  எனில் (success && plan->attach_accelerator_data) {
    _attach_accelerator_data (plan, plan->dest);
  }

end:
  திரும்பு success ? hb_face_reference (plan->dest) : nullptr;
}
