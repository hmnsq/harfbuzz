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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-open-file.hh"

#അന്തര്ഭാവ "hb-subset.hh"
#അന്തര്ഭാവ "hb-subset-table.hh"
#അന്തര്ഭാവ "hb-subset-accelerator.hh"

#അന്തര്ഭാവ "hb-ot-cmap-table.hh"
#അന്തര്ഭാവ "hb-ot-var-cvar-table.hh"
#അന്തര്ഭാവ "hb-ot-head-table.hh"
#അന്തര്ഭാവ "hb-ot-stat-table.hh"
#അന്തര്ഭാവ "hb-ot-post-table-v2subset.hh"


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


hb_user_data_താക്കോൽ_t hb_subset_accelerator_user_data_താക്കോൽ = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
സ്ഥിതം hb_tag_t known_tables[] {
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

സ്ഥിതം തർക്കം _table_is_empty (സ്ഥിരം hb_face_t *face, hb_tag_t tag)
{
  hb_blob_t* blob = hb_face_reference_table (face, tag);
  തർക്കം result = (blob == hb_blob_get_empty ());
  hb_blob_destroy (blob);
  തിരികെ result;
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
_get_table_tags (സ്ഥിരം hb_subset_plan_t* plan,
                 unsigned പൂർണ്ണസംഖ്യ  start_offset,
                 unsigned പൂർണ്ണസംഖ്യ *table_എണ്ണം_2, /* IN/OUT */
                 hb_tag_t     *table_tags /* OUT */)
{
  unsigned അക്കം_tables = hb_face_get_table_tags (plan->source, 0, nullptr, nullptr);
  എങ്കിൽ (അക്കം_tables)
    തിരികെ hb_face_get_table_tags (plan->source, start_offset, table_എണ്ണം_2, table_tags);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  auto it =
      hb_concat (
          + hb_array (known_tables)
          | hb_filter ([&] (hb_tag_t tag) {
            തിരികെ !_table_is_empty (plan->source, tag) && !plan->no_subset_tables.has (tag);
          })
          | hb_map ([] (hb_tag_t tag) -> hb_tag_t { തിരികെ tag; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_tag_t tag) {
            തിരികെ !_table_is_empty (plan->source, tag);
          }));

  it += start_offset;

  unsigned അക്കം_written = 0;
  ആകുമ്പോൾ (തർക്കം (it) && അക്കം_written < *table_എണ്ണം_2)
    table_tags[അക്കം_written++] = *it++;

  *table_എണ്ണം_2 = അക്കം_written;
  തിരികെ അക്കം_written;
}


സ്ഥിതം തർക്കം
_is_table_present (hb_face_t *source, hb_tag_t tag)
{

  എങ്കിൽ (!hb_face_get_table_tags (source, 0, nullptr, nullptr)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    തിരികെ !_table_is_empty (source, tag);
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, അക്കം_tables = ARRAY_LENGTH (table_tags);
  ആകുമ്പോൾ (((ശൂന്യം) hb_face_get_table_tags (source, offset, &അക്കം_tables, table_tags), അക്കം_tables))
  {
    വേണ്ടി (unsigned i = 0; i < അക്കം_tables; ++i)
      എങ്കിൽ (table_tags[i] == tag)
	തിരികെ സത്യം;
    offset += അക്കം_tables;
  }
  തിരികെ അസത്യം;
}

സ്ഥിതം തർക്കം
_should_drop_table (hb_subset_plan_t *plan, hb_tag_t tag)
{
  എങ്കിൽ (plan->drop_tables.has (tag))
    തിരികെ സത്യം;

  തിരഞ്ഞെടുപ്പ് (tag)
  {
  അവസ്ഥ HB_TAG('c','v','a','r'): /* hint table, fallthrough */
    തിരികെ plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  അവസ്ഥ HB_TAG('c','v','t',' '): /* hint table, fallthrough */
  അവസ്ഥ HB_TAG('f','p','g','m'): /* hint table, fallthrough */
  അവസ്ഥ HB_TAG('p','r','e','p'): /* hint table, fallthrough */
  അവസ്ഥ HB_TAG('h','d','m','x'): /* hint table, fallthrough */
  അവസ്ഥ HB_TAG('V','D','M','X'): /* hint table, fallthrough */
    തിരികെ plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#ifdef HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  അവസ്ഥ HB_TAG('G','D','E','F'):
  അവസ്ഥ HB_TAG('G','P','O','S'):
  അവസ്ഥ HB_TAG('G','S','U','B'):
  അവസ്ഥ HB_TAG('m','o','r','x'):
  അവസ്ഥ HB_TAG('m','o','r','t'):
  അവസ്ഥ HB_TAG('k','e','r','x'):
  അവസ്ഥ HB_TAG('k','e','r','n'):
    തിരികെ സത്യം;
#endif

  അവസ്ഥ HB_TAG('a','v','a','r'):
  അവസ്ഥ HB_TAG('f','v','a','r'):
  അവസ്ഥ HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  അവസ്ഥ HB_TAG('G','V','A','R'):
#endif
  അവസ്ഥ HB_TAG('H','V','A','R'):
  അവസ്ഥ HB_TAG('V','V','A','R'):
  അവസ്ഥ HB_TAG('M','V','A','R'):
    തിരികെ plan->all_axes_pinned;

  സ്ഥിരസ്ഥിതി:
    തിരികെ അസത്യം;
  }
}

സ്ഥിതം തർക്കം
_dependencies_satisfied (hb_subset_plan_t *plan, hb_tag_t tag,
                         സ്ഥിരം hb_set_t &subsetted_tags,
                         സ്ഥിരം hb_set_t &pending_subset_tags)
{
  തർക്കം outline_pending = pending_subset_tags.has (HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
  outline_pending |= pending_subset_tags.has (HB_TAG('G','L','Y','F'));
#endif

  തിരഞ്ഞെടുപ്പ് (tag)
  {
  അവസ്ഥ HB_TAG('h','m','t','x'):
  അവസ്ഥ HB_TAG('v','m','t','x'):
  അവസ്ഥ HB_TAG('m','a','x','p'):
#ifndef HB_NO_BEYOND_64K
  അവസ്ഥ HB_TAG('H','M','T','X'):
  അവസ്ഥ HB_TAG('V','M','T','X'):
  അവസ്ഥ HB_TAG('M','A','X','P'):
#endif
  അവസ്ഥ HB_TAG('O','S','/','2'):
    തിരികെ !plan->normalized_coords || !outline_pending;
  അവസ്ഥ HB_TAG('G','P','O','S'):
  അവസ്ഥ HB_TAG('G','S','U','B'):
    തിരികെ plan->all_axes_pinned || !pending_subset_tags.has (HB_TAG('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  അവസ്ഥ HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  അവസ്ഥ HB_TAG('G','V','A','R'):
#endif
  അവസ്ഥ HB_TAG('c','v','a','r'):
  അവസ്ഥ HB_TAG('H','V','A','R'):
  അവസ്ഥ HB_TAG('V','V','A','R'):
  അവസ്ഥ HB_TAG('M','V','A','R'):
  അവസ്ഥ HB_TAG('G','D','E','F'):
  അവസ്ഥ HB_TAG('C','F','F','2'):
    തിരികെ !plan->has_avar2 || !pending_subset_tags.has (HB_TAG('a','v','a','r'));
  സ്ഥിരസ്ഥിതി:
    തിരികെ സത്യം;
  }
}

സ്ഥിതം തർക്കം
_subset_table (hb_subset_plan_t *plan,
	       hb_vector_t<അക്ഷരം> &buf,
	       hb_tag_t tag)
{
  എങ്കിൽ (plan->no_subset_tables.has (tag)) {
    തിരികെ _hb_subset_table_passthrough (plan, tag);
  }

  DEBUG_സന്ദേശം (SUBSET, nullptr, "subset %c%c%c%c", HB_UNTAG (tag));

  തർക്കം success;
  എങ്കിൽ (_hb_subset_table_layout (plan, buf, tag, &success) ||
      _hb_subset_table_var (plan, buf, tag, &success) ||
      _hb_subset_table_cff (plan, buf, tag, &success) ||
      _hb_subset_table_color (plan, buf, tag, &success) ||
      _hb_subset_table_other (plan, buf, tag, &success))
    തിരികെ success;


  തിരഞ്ഞെടുപ്പ് (tag)
  {
  അവസ്ഥ HB_TAG('h','e','a','d'):
  {
    തർക്കം outline_table_handles_head =
	_is_table_present (plan->source, HB_TAG('g','l','y','f')) &&
	!_should_drop_table (plan, HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
    outline_table_handles_head |=
	_is_table_present (plan->source, HB_TAG('G','L','Y','F')) &&
	!_should_drop_table (plan, HB_TAG('G','L','Y','F'));
#endif
    എങ്കിൽ (outline_table_handles_head)
      തിരികെ സത്യം; /* skip head, handled by the outline table */
    തിരികെ _hb_subset_table<സ്ഥിരം OT::head> (plan, buf);
  }

  അവസ്ഥ HB_TAG('S','T','A','T'):
    എങ്കിൽ (!plan->user_axes_location.is_empty ()) തിരികെ _hb_subset_table<സ്ഥിരം OT::STAT> (plan, buf);
    അല്ലെങ്കിൽ തിരികെ _hb_subset_table_passthrough (plan, tag);

  അവസ്ഥ HB_TAG('c','v','t',' '):
#ifndef HB_NO_VAR
    എങ്കിൽ (_is_table_present (plan->source, HB_TAG('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      auto &cvar = *plan->source->table.cvar;
      തിരികെ OT::cvar::add_cvt_and_apply_deltas (plan, cvar.get_tuple_var_data (), &cvar);
    }
#endif
    തിരികെ _hb_subset_table_passthrough (plan, tag);
  }

  എങ്കിൽ (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    തിരികെ _hb_subset_table_passthrough (plan, tag);

  // Drop table
  തിരികെ സത്യം;
}

സ്ഥിതം ശൂന്യം _attach_accelerator_data (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  എങ്കിൽ (!plan->inprogress_accelerator) തിരികെ;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = nullptr;

  എങ്കിൽ (accel->in_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    തിരികെ;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_table<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  എങ്കിൽ (!hb_face_set_user_data(face,
                             hb_subset_accelerator_t::user_data_താക്കോൽ(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             സത്യം))
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
hb_subset_or_fail (hb_face_t *source, സ്ഥിരം hb_subset_നിവേശം_t *നിവേശം)
{
  എങ്കിൽ (unlikely (!നിവേശം || !source)) തിരികെ nullptr;

  എങ്കിൽ (unlikely (!source->get_അക്കം_glyphs ()))
  {
    DEBUG_സന്ദേശം (SUBSET, nullptr, "No glyphs in source font.");
    തിരികെ nullptr;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (source, നിവേശം);
  എങ്കിൽ (unlikely (!plan)) {
    തിരികെ nullptr;
  }

  hb_face_t * result = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  തിരികെ result;
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
  എങ്കിൽ (unlikely (!plan || plan->in_error ())) {
    തിരികെ nullptr;
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, അക്കം_tables = ARRAY_LENGTH (table_tags);

  hb_set_t subsetted_tags, pending_subset_tags;
  ആകുമ്പോൾ (((ശൂന്യം) _get_table_tags (plan, offset, &അക്കം_tables, table_tags), അക്കം_tables))
  {
    വേണ്ടി (unsigned i = 0; i < അക്കം_tables; ++i)
    {
      hb_tag_t tag = table_tags[i];
      എങ്കിൽ (_should_drop_table (plan, tag)) തുടരുക;
      pending_subset_tags.add (tag);
    }

    offset += അക്കം_tables;
  }

  തർക്കം success = സത്യം;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_vector_t<അക്ഷരം> buf;
    buf.alloc (8192 - 16);

    ആകുമ്പോൾ (!pending_subset_tags.is_empty ())
    {
      എങ്കിൽ (subsetted_tags.in_error ()
	  || pending_subset_tags.in_error ()) {
	success = അസത്യം;
	പോകുക end;
      }

      തർക്കം made_changes = അസത്യം;
      വേണ്ടി (hb_tag_t tag : pending_subset_tags)
      {
	എങ്കിൽ (!_dependencies_satisfied (plan, tag,
				      subsetted_tags,
				      pending_subset_tags))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  തുടരുക;
	}

	pending_subset_tags.del (tag);
	subsetted_tags.add (tag);
	made_changes = സത്യം;

	success = _subset_table (plan, buf, tag);
	എങ്കിൽ (unlikely (!success)) പോകുക end;
      }

      എങ്കിൽ (!made_changes)
      {
	DEBUG_സന്ദേശം (SUBSET, nullptr, "Table dependencies unable to be satisfied. Subset failed.");
	success = അസത്യം;
	പോകുക end;
      }
    }
  }

  എങ്കിൽ (success && plan->attach_accelerator_data) {
    _attach_accelerator_data (plan, plan->dest);
  }

end:
  തിരികെ success ? hb_face_reference (plan->dest) : nullptr;
}
