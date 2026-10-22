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

#ضمن "hb.hh"

#ضمن "hb-open-type.hh"
#ضمن "hb-open-file.hh"

#ضمن "hb-subset.hh"
#ضمن "hb-subset-table.hh"
#ضمن "hb-subset-accelerator.hh"

#ضمن "hb-ot-cmap-table.hh"
#ضمن "hb-ot-var-cvar-table.hh"
#ضمن "hb-ot-head-table.hh"
#ضمن "hb-ot-stat-table.hh"
#ضمن "hb-ot-post-table-v2subset.hh"


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


hb_user_data_key_t _hb_subset_accelerator_user_data_key = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
ساكن hb_tag_t known_tables[] {
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

ساكن منطقي _table_is_empty (ثابت hb_face_t *face, hb_tag_t tag)
{
  hb_blob_t* blob = hb_face_reference_table (face, tag);
  منطقي result = (blob == hb_blob_get_empty ());
  hb_blob_destroy (blob);
  החזר result;
}

ساكن unsigned صحيح
_get_table_tags (ثابت hb_subset_plan_t* plan,
                 unsigned صحيح  start_offset,
                 unsigned صحيح *table_count, /* IN/OUT */
                 hb_tag_t     *table_tags /* OUT */)
{
  unsigned num_tables = hb_face_get_table_tags (plan->source, 0, nullptr, nullptr);
  אם (num_tables)
    החזר hb_face_get_table_tags (plan->source, start_offset, table_count, table_tags);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  auto it =
      hb_concat (
          + hb_array (known_tables)
          | hb_filter ([&] (hb_tag_t tag) {
            החזר !_table_is_empty (plan->source, tag) && !plan->no_subset_tables.has (tag);
          })
          | hb_map ([] (hb_tag_t tag) -> hb_tag_t { החזר tag; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_tag_t tag) {
            החזר !_table_is_empty (plan->source, tag);
          }));

  it += start_offset;

  unsigned num_written = 0;
  כלעוד (منطقي (it) && num_written < *table_count)
    table_tags[num_written++] = *it++;

  *table_count = num_written;
  החזר num_written;
}


ساكن منطقي
_is_table_present (hb_face_t *source, hb_tag_t tag)
{

  אם (!hb_face_get_table_tags (source, 0, nullptr, nullptr)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    החזר !_table_is_empty (source, tag);
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, num_tables = ARRAY_LENGTH (table_tags);
  כלעוד (((فراغ) hb_face_get_table_tags (source, offset, &num_tables, table_tags), num_tables))
  {
    עבור (unsigned i = 0; i < num_tables; ++i)
      אם (table_tags[i] == tag)
	החזר אמת;
    offset += num_tables;
  }
  החזר שקר;
}

ساكن منطقي
_should_drop_table (hb_subset_plan_t *plan, hb_tag_t tag)
{
  אם (plan->drop_tables.has (tag))
    החזר אמת;

  بدل (tag)
  {
  حالة HB_TAG('c','v','a','r'): /* hint table, fallthrough */
    החזר plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  حالة HB_TAG('c','v','t',' '): /* hint table, fallthrough */
  حالة HB_TAG('f','p','g','m'): /* hint table, fallthrough */
  حالة HB_TAG('p','r','e','p'): /* hint table, fallthrough */
  حالة HB_TAG('h','d','m','x'): /* hint table, fallthrough */
  حالة HB_TAG('V','D','M','X'): /* hint table, fallthrough */
    החזר plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#ifdef HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  حالة HB_TAG('G','D','E','F'):
  حالة HB_TAG('G','P','O','S'):
  حالة HB_TAG('G','S','U','B'):
  حالة HB_TAG('m','o','r','x'):
  حالة HB_TAG('m','o','r','t'):
  حالة HB_TAG('k','e','r','x'):
  حالة HB_TAG('k','e','r','n'):
    החזר אמת;
#endif

  حالة HB_TAG('a','v','a','r'):
  حالة HB_TAG('f','v','a','r'):
  حالة HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  حالة HB_TAG('G','V','A','R'):
#endif
  حالة HB_TAG('H','V','A','R'):
  حالة HB_TAG('V','V','A','R'):
  حالة HB_TAG('M','V','A','R'):
    החזר plan->all_axes_pinned;

  افتراضي:
    החזר שקר;
  }
}

ساكن منطقي
_dependencies_satisfied (hb_subset_plan_t *plan, hb_tag_t tag,
                         ثابت hb_set_t &subsetted_tags,
                         ثابت hb_set_t &pending_subset_tags)
{
  منطقي outline_pending = pending_subset_tags.has (HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
  outline_pending |= pending_subset_tags.has (HB_TAG('G','L','Y','F'));
#endif

  بدل (tag)
  {
  حالة HB_TAG('h','m','t','x'):
  حالة HB_TAG('v','m','t','x'):
  حالة HB_TAG('m','a','x','p'):
#ifndef HB_NO_BEYOND_64K
  حالة HB_TAG('H','M','T','X'):
  حالة HB_TAG('V','M','T','X'):
  حالة HB_TAG('M','A','X','P'):
#endif
  حالة HB_TAG('O','S','/','2'):
    החזר !plan->normalized_coords || !outline_pending;
  حالة HB_TAG('G','P','O','S'):
  حالة HB_TAG('G','S','U','B'):
    החזר plan->all_axes_pinned || !pending_subset_tags.has (HB_TAG('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  حالة HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  حالة HB_TAG('G','V','A','R'):
#endif
  حالة HB_TAG('c','v','a','r'):
  حالة HB_TAG('H','V','A','R'):
  حالة HB_TAG('V','V','A','R'):
  حالة HB_TAG('M','V','A','R'):
  حالة HB_TAG('G','D','E','F'):
  حالة HB_TAG('C','F','F','2'):
    החזר !plan->has_avar2 || !pending_subset_tags.has (HB_TAG('a','v','a','r'));
  افتراضي:
    החזר אמת;
  }
}

ساكن منطقي
_subset_table (hb_subset_plan_t *plan,
	       hb_vector_t<حرف> &buf,
	       hb_tag_t tag)
{
  אם (plan->no_subset_tables.has (tag)) {
    החזר _hb_subset_table_passthrough (plan, tag);
  }

  DEBUG_MSG (SUBSET, nullptr, "subset %c%c%c%c", HB_UNTAG (tag));

  منطقي success;
  אם (_hb_subset_table_layout (plan, buf, tag, &success) ||
      _hb_subset_table_var (plan, buf, tag, &success) ||
      _hb_subset_table_cff (plan, buf, tag, &success) ||
      _hb_subset_table_color (plan, buf, tag, &success) ||
      _hb_subset_table_other (plan, buf, tag, &success))
    החזר success;


  بدل (tag)
  {
  حالة HB_TAG('h','e','a','d'):
  {
    منطقي outline_table_handles_head =
	_is_table_present (plan->source, HB_TAG('g','l','y','f')) &&
	!_should_drop_table (plan, HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
    outline_table_handles_head |=
	_is_table_present (plan->source, HB_TAG('G','L','Y','F')) &&
	!_should_drop_table (plan, HB_TAG('G','L','Y','F'));
#endif
    אם (outline_table_handles_head)
      החזר אמת; /* skip head, handled by the outline table */
    החזר _hb_subset_table<ثابت OT::head> (plan, buf);
  }

  حالة HB_TAG('S','T','A','T'):
    אם (!plan->user_axes_location.is_empty ()) החזר _hb_subset_table<ثابت OT::STAT> (plan, buf);
    אחרת החזר _hb_subset_table_passthrough (plan, tag);

  حالة HB_TAG('c','v','t',' '):
#ifndef HB_NO_VAR
    אם (_is_table_present (plan->source, HB_TAG('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      auto &cvar = *plan->source->table.cvar;
      החזר OT::cvar::add_cvt_and_apply_deltas (plan, cvar.get_tuple_var_data (), &cvar);
    }
#endif
    החזר _hb_subset_table_passthrough (plan, tag);
  }

  אם (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    החזר _hb_subset_table_passthrough (plan, tag);

  // Drop table
  החזר אמת;
}

ساكن فراغ _attach_accelerator_data (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  אם (!plan->inprogress_accelerator) החזר;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = nullptr;

  אם (accel->in_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    החזר;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_table<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  אם (!hb_face_set_user_data(face,
                             hb_subset_accelerator_t::user_data_key(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             אמת))
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
hb_subset_or_fail (hb_face_t *source, ثابت hb_subset_input_t *input)
{
  אם (unlikely (!input || !source)) החזר nullptr;

  אם (unlikely (!source->get_num_glyphs ()))
  {
    DEBUG_MSG (SUBSET, nullptr, "No glyphs in source font.");
    החזר nullptr;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (source, input);
  אם (unlikely (!plan)) {
    החזר nullptr;
  }

  hb_face_t * result = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  החזר result;
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
  אם (unlikely (!plan || plan->in_error ())) {
    החזר nullptr;
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, num_tables = ARRAY_LENGTH (table_tags);

  hb_set_t subsetted_tags, pending_subset_tags;
  כלעוד (((فراغ) _get_table_tags (plan, offset, &num_tables, table_tags), num_tables))
  {
    עבור (unsigned i = 0; i < num_tables; ++i)
    {
      hb_tag_t tag = table_tags[i];
      אם (_should_drop_table (plan, tag)) استمر;
      pending_subset_tags.add (tag);
    }

    offset += num_tables;
  }

  منطقي success = אמת;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_vector_t<حرف> buf;
    buf.alloc (8192 - 16);

    כלעוד (!pending_subset_tags.is_empty ())
    {
      אם (subsetted_tags.in_error ()
	  || pending_subset_tags.in_error ()) {
	success = שקר;
	اذهب end;
      }

      منطقي made_changes = שקר;
      עבור (hb_tag_t tag : pending_subset_tags)
      {
	אם (!_dependencies_satisfied (plan, tag,
				      subsetted_tags,
				      pending_subset_tags))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  استمر;
	}

	pending_subset_tags.del (tag);
	subsetted_tags.add (tag);
	made_changes = אמת;

	success = _subset_table (plan, buf, tag);
	אם (unlikely (!success)) اذهب end;
      }

      אם (!made_changes)
      {
	DEBUG_MSG (SUBSET, nullptr, "Table dependencies unable to be satisfied. Subset failed.");
	success = שקר;
	اذهب end;
      }
    }
  }

  אם (success && plan->attach_accelerator_data) {
    _attach_accelerator_data (plan, plan->dest);
  }

end:
  החזר success ? hb_face_reference (plan->dest) : nullptr;
}
