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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-open-file.hh"

#ଅନ୍ତର୍ଭାଵ "hb-subset.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-accelerator.hh"

#ଅନ୍ତର୍ଭାଵ "hb-ot-cmap-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-cvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-head-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-stat-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-post-table-v2subset.hh"


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


hb_user_data_ଚାବି_t hb_subset_accelerator_user_data_ଚାବି = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
ସ୍ଥିର hb_tag_t known_tables[] {
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

ସ୍ଥିର ତାର୍କିକ _table_is_empty (ଅଚଳ hb_face_t *face, hb_tag_t tag)
{
  hb_blob_t* blob = hb_face_reference_table (face, tag);
  ତାର୍କିକ result = (blob == hb_blob_get_empty ());
  hb_blob_destroy (blob);
  ଫେରନ୍ତୁ result;
}

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
_get_table_tags (ଅଚଳ hb_subset_plan_t* plan,
                 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  start_offset,
                 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *table_ଗଣନା_2, /* IN/OUT */
                 hb_tag_t     *table_tags /* OUT */)
{
  unsigned ଅଙ୍କ_tables = hb_face_get_table_tags (plan->source, 0, nullptr, nullptr);
  ଯଦି (ଅଙ୍କ_tables)
    ଫେରନ୍ତୁ hb_face_get_table_tags (plan->source, start_offset, table_ଗଣନା_2, table_tags);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  auto it =
      hb_concat (
          + hb_array (known_tables)
          | hb_filter ([&] (hb_tag_t tag) {
            ଫେରନ୍ତୁ !_table_is_empty (plan->source, tag) && !plan->no_subset_tables.has (tag);
          })
          | hb_map ([] (hb_tag_t tag) -> hb_tag_t { ଫେରନ୍ତୁ tag; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_tag_t tag) {
            ଫେରନ୍ତୁ !_table_is_empty (plan->source, tag);
          }));

  it += start_offset;

  unsigned ଅଙ୍କ_written = 0;
  ଯେତେବେଳେ (ତାର୍କିକ (it) && ଅଙ୍କ_written < *table_ଗଣନା_2)
    table_tags[ଅଙ୍କ_written++] = *it++;

  *table_ଗଣନା_2 = ଅଙ୍କ_written;
  ଫେରନ୍ତୁ ଅଙ୍କ_written;
}


ସ୍ଥିର ତାର୍କିକ
_is_table_present (hb_face_t *source, hb_tag_t tag)
{

  ଯଦି (!hb_face_get_table_tags (source, 0, nullptr, nullptr)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    ଫେରନ୍ତୁ !_table_is_empty (source, tag);
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, ଅଙ୍କ_tables = ARRAY_LENGTH (table_tags);
  ଯେତେବେଳେ (((ଶୂନ୍ୟ) hb_face_get_table_tags (source, offset, &ଅଙ୍କ_tables, table_tags), ଅଙ୍କ_tables))
  {
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_tables; ++i)
      ଯଦି (table_tags[i] == tag)
	ଫେରନ୍ତୁ ସତ୍ୟ;
    offset += ଅଙ୍କ_tables;
  }
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ସ୍ଥିର ତାର୍କିକ
_should_drop_table (hb_subset_plan_t *plan, hb_tag_t tag)
{
  ଯଦି (plan->drop_tables.has (tag))
    ଫେରନ୍ତୁ ସତ୍ୟ;

  ବାଛ (tag)
  {
  ସ୍ଥିତି HB_TAG('c','v','a','r'): /* hint table, fallthrough */
    ଫେରନ୍ତୁ plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  ସ୍ଥିତି HB_TAG('c','v','t',' '): /* hint table, fallthrough */
  ସ୍ଥିତି HB_TAG('f','p','g','m'): /* hint table, fallthrough */
  ସ୍ଥିତି HB_TAG('p','r','e','p'): /* hint table, fallthrough */
  ସ୍ଥିତି HB_TAG('h','d','m','x'): /* hint table, fallthrough */
  ସ୍ଥିତି HB_TAG('V','D','M','X'): /* hint table, fallthrough */
    ଫେରନ୍ତୁ plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#ifdef HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  ସ୍ଥିତି HB_TAG('G','D','E','F'):
  ସ୍ଥିତି HB_TAG('G','P','O','S'):
  ସ୍ଥିତି HB_TAG('G','S','U','B'):
  ସ୍ଥିତି HB_TAG('m','o','r','x'):
  ସ୍ଥିତି HB_TAG('m','o','r','t'):
  ସ୍ଥିତି HB_TAG('k','e','r','x'):
  ସ୍ଥିତି HB_TAG('k','e','r','n'):
    ଫେରନ୍ତୁ ସତ୍ୟ;
#endif

  ସ୍ଥିତି HB_TAG('a','v','a','r'):
  ସ୍ଥିତି HB_TAG('f','v','a','r'):
  ସ୍ଥିତି HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  ସ୍ଥିତି HB_TAG('G','V','A','R'):
#endif
  ସ୍ଥିତି HB_TAG('H','V','A','R'):
  ସ୍ଥିତି HB_TAG('V','V','A','R'):
  ସ୍ଥିତି HB_TAG('M','V','A','R'):
    ଫେରନ୍ତୁ plan->all_axes_pinned;

  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
}

ସ୍ଥିର ତାର୍କିକ
_dependencies_satisfied (hb_subset_plan_t *plan, hb_tag_t tag,
                         ଅଚଳ hb_set_t &subsetted_tags,
                         ଅଚଳ hb_set_t &pending_subset_tags)
{
  ତାର୍କିକ outline_pending = pending_subset_tags.has (HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
  outline_pending |= pending_subset_tags.has (HB_TAG('G','L','Y','F'));
#endif

  ବାଛ (tag)
  {
  ସ୍ଥିତି HB_TAG('h','m','t','x'):
  ସ୍ଥିତି HB_TAG('v','m','t','x'):
  ସ୍ଥିତି HB_TAG('m','a','x','p'):
#ifndef HB_NO_BEYOND_64K
  ସ୍ଥିତି HB_TAG('H','M','T','X'):
  ସ୍ଥିତି HB_TAG('V','M','T','X'):
  ସ୍ଥିତି HB_TAG('M','A','X','P'):
#endif
  ସ୍ଥିତି HB_TAG('O','S','/','2'):
    ଫେରନ୍ତୁ !plan->normalized_coords || !outline_pending;
  ସ୍ଥିତି HB_TAG('G','P','O','S'):
  ସ୍ଥିତି HB_TAG('G','S','U','B'):
    ଫେରନ୍ତୁ plan->all_axes_pinned || !pending_subset_tags.has (HB_TAG('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  ସ୍ଥିତି HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  ସ୍ଥିତି HB_TAG('G','V','A','R'):
#endif
  ସ୍ଥିତି HB_TAG('c','v','a','r'):
  ସ୍ଥିତି HB_TAG('H','V','A','R'):
  ସ୍ଥିତି HB_TAG('V','V','A','R'):
  ସ୍ଥିତି HB_TAG('M','V','A','R'):
  ସ୍ଥିତି HB_TAG('G','D','E','F'):
  ସ୍ଥିତି HB_TAG('C','F','F','2'):
    ଫେରନ୍ତୁ !plan->has_avar2 || !pending_subset_tags.has (HB_TAG('a','v','a','r'));
  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
}

ସ୍ଥିର ତାର୍କିକ
_subset_table (hb_subset_plan_t *plan,
	       hb_vector_t<ଅକ୍ଷର> &buf,
	       hb_tag_t tag)
{
  ଯଦି (plan->no_subset_tables.has (tag)) {
    ଫେରନ୍ତୁ _hb_subset_table_passthrough (plan, tag);
  }

  DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "subset %c%c%c%c", HB_UNTAG (tag));

  ତାର୍କିକ success;
  ଯଦି (_hb_subset_table_layout (plan, buf, tag, &success) ||
      _hb_subset_table_var (plan, buf, tag, &success) ||
      _hb_subset_table_cff (plan, buf, tag, &success) ||
      _hb_subset_table_color (plan, buf, tag, &success) ||
      _hb_subset_table_other (plan, buf, tag, &success))
    ଫେରନ୍ତୁ success;


  ବାଛ (tag)
  {
  ସ୍ଥିତି HB_TAG('h','e','a','d'):
  {
    ତାର୍କିକ outline_table_handles_head =
	_is_table_present (plan->source, HB_TAG('g','l','y','f')) &&
	!_should_drop_table (plan, HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
    outline_table_handles_head |=
	_is_table_present (plan->source, HB_TAG('G','L','Y','F')) &&
	!_should_drop_table (plan, HB_TAG('G','L','Y','F'));
#endif
    ଯଦି (outline_table_handles_head)
      ଫେରନ୍ତୁ ସତ୍ୟ; /* skip head, handled by the outline table */
    ଫେରନ୍ତୁ _hb_subset_table<ଅଚଳ OT::head> (plan, buf);
  }

  ସ୍ଥିତି HB_TAG('S','T','A','T'):
    ଯଦି (!plan->user_axes_location.is_empty ()) ଫେରନ୍ତୁ _hb_subset_table<ଅଚଳ OT::STAT> (plan, buf);
    ଅନ୍ୟଥା ଫେରନ୍ତୁ _hb_subset_table_passthrough (plan, tag);

  ସ୍ଥିତି HB_TAG('c','v','t',' '):
#ifndef HB_NO_VAR
    ଯଦି (_is_table_present (plan->source, HB_TAG('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      auto &cvar = *plan->source->table.cvar;
      ଫେରନ୍ତୁ OT::cvar::add_cvt_and_apply_deltas (plan, cvar.get_tuple_var_data (), &cvar);
    }
#endif
    ଫେରନ୍ତୁ _hb_subset_table_passthrough (plan, tag);
  }

  ଯଦି (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    ଫେରନ୍ତୁ _hb_subset_table_passthrough (plan, tag);

  // Drop table
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ଶୂନ୍ୟ _attach_accelerator_data (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  ଯଦି (!plan->inprogress_accelerator) ଫେରନ୍ତୁ;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = nullptr;

  ଯଦି (accel->in_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    ଫେରନ୍ତୁ;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_table<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  ଯଦି (!hb_face_set_user_data(face,
                             hb_subset_accelerator_t::user_data_ଚାବି(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             ସତ୍ୟ))
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
hb_subset_or_fail (hb_face_t *source, ଅଚଳ hb_subset_ନିବେଶ_t *ନିବେଶ)
{
  ଯଦି (unlikely (!ନିବେଶ || !source)) ଫେରନ୍ତୁ nullptr;

  ଯଦି (unlikely (!source->get_ଅଙ୍କ_glyphs ()))
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "No glyphs in source font.");
    ଫେରନ୍ତୁ nullptr;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (source, ନିବେଶ);
  ଯଦି (unlikely (!plan)) {
    ଫେରନ୍ତୁ nullptr;
  }

  hb_face_t * result = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  ଫେରନ୍ତୁ result;
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
  ଯଦି (unlikely (!plan || plan->in_error ())) {
    ଫେରନ୍ତୁ nullptr;
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, ଅଙ୍କ_tables = ARRAY_LENGTH (table_tags);

  hb_set_t subsetted_tags, pending_subset_tags;
  ଯେତେବେଳେ (((ଶୂନ୍ୟ) _get_table_tags (plan, offset, &ଅଙ୍କ_tables, table_tags), ଅଙ୍କ_tables))
  {
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_tables; ++i)
    {
      hb_tag_t tag = table_tags[i];
      ଯଦି (_should_drop_table (plan, tag)) ଜାରି;
      pending_subset_tags.add (tag);
    }

    offset += ଅଙ୍କ_tables;
  }

  ତାର୍କିକ success = ସତ୍ୟ;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_vector_t<ଅକ୍ଷର> buf;
    buf.alloc (8192 - 16);

    ଯେତେବେଳେ (!pending_subset_tags.is_empty ())
    {
      ଯଦି (subsetted_tags.in_error ()
	  || pending_subset_tags.in_error ()) {
	success = ମିଥ୍ୟା;
	ଯାଅ end;
      }

      ତାର୍କିକ made_changes = ମିଥ୍ୟା;
      ପାଇଁ (hb_tag_t tag : pending_subset_tags)
      {
	ଯଦି (!_dependencies_satisfied (plan, tag,
				      subsetted_tags,
				      pending_subset_tags))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  ଜାରି;
	}

	pending_subset_tags.del (tag);
	subsetted_tags.add (tag);
	made_changes = ସତ୍ୟ;

	success = _subset_table (plan, buf, tag);
	ଯଦି (unlikely (!success)) ଯାଅ end;
      }

      ଯଦି (!made_changes)
      {
	DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr, "Table dependencies unable to be satisfied. Subset failed.");
	success = ମିଥ୍ୟା;
	ଯାଅ end;
      }
    }
  }

  ଯଦି (success && plan->attach_accelerator_data) {
    _attach_accelerator_data (plan, plan->dest);
  }

end:
  ଫେରନ୍ତୁ success ? hb_face_reference (plan->dest) : nullptr;
}
