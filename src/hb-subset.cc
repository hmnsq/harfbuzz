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

#समावेश "hb.hh"

#समावेश "hb-open-type.hh"
#समावेश "hb-open-file.hh"

#समावेश "hb-subset.hh"
#समावेश "hb-subset-table.hh"
#समावेश "hb-subset-accelerator.hh"

#समावेश "hb-ot-cmap-table.hh"
#समावेश "hb-ot-var-cvar-table.hh"
#समावेश "hb-ot-head-table.hh"
#समावेश "hb-ot-stat-table.hh"
#समावेश "hb-ot-post-table-v2subset.hh"


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


hb_user_तथ्य_कुंजी_t hb_subset_accelerator_user_तथ्य_कुंजी = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
अटल hb_tag_t known_tables[] {
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
#अगर_अघोषित HB_NO_BEYOND_64K
  HB_TAG('G','L','Y','F'),
#पूर्ण_अगर
  HB_TAG('G','P','O','S'),
  HB_TAG('G','S','U','B'),
  HB_TAG('g','v','a','r'),
#अगर_अघोषित HB_NO_BEYOND_64K
  HB_TAG('G','V','A','R'),
#पूर्ण_अगर
  HB_TAG('h','d','m','x'),
  HB_TAG('h','e','a','d'),
  HB_TAG('h','h','e','a'),
#अगर_अघोषित HB_NO_BEYOND_64K
  HB_TAG('H','H','E','A'),
#पूर्ण_अगर
  HB_TAG('h','m','t','x'),
#अगर_अघोषित HB_NO_BEYOND_64K
  HB_TAG('H','M','T','X'),
#पूर्ण_अगर
  HB_TAG('H','V','A','R'),
  HB_TAG('J','S','T','F'),
  HB_TAG('k','e','r','n'),
  HB_TAG('l','o','c','a'),
#अगर_अघोषित HB_NO_BEYOND_64K
  HB_TAG('L','O','C','A'),
#पूर्ण_अगर
  HB_TAG('L','T','S','H'),
  HB_TAG('M','A','T','H'),
  HB_TAG('m','a','x','p'),
#अगर_अघोषित HB_NO_BEYOND_64K
  HB_TAG('M','A','X','P'),
#पूर्ण_अगर
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
#अगर_अघोषित HB_NO_BEYOND_64K
  HB_TAG('V','H','E','A'),
#पूर्ण_अगर
  HB_TAG('v','m','t','x'),
#अगर_अघोषित HB_NO_BEYOND_64K
  HB_TAG('V','M','T','X'),
#पूर्ण_अगर
  HB_TAG('V','O','R','G'),
  HB_TAG('V','A','R','C'),
  HB_TAG('V','V','A','R'),
  HB_TAG('n','a','m','e'),
  HB_TAG('O','S','/','2')
};

अटल bool _table_is_empty (स्थिर hb_face_t *face, hb_tag_t tag)
{
  hb_blob_t* blob = hb_face_reference_table (face, tag);
  bool परिणाम = (blob == hb_blob_get_empty ());
  hb_blob_destroy (blob);
  वापस परिणाम;
}

अटल अचिन्हित पूर्णांक
_get_table_tags (स्थिर hb_subset_plan_t* plan,
                 अचिन्हित पूर्णांक  start_offset,
                 अचिन्हित पूर्णांक *table_गिनती_2, /* IN/OUT */
                 hb_tag_t     *table_tags /* OUT */)
{
  अचिन्हित अंक_tables = hb_face_get_table_tags (plan->source, 0, nullptr, nullptr);
  अगर (अंक_tables)
    वापस hb_face_get_table_tags (plan->source, start_offset, table_गिनती_2, table_tags);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  स्वतः it =
      hb_concat (
          + hb_array (known_tables)
          | hb_filter ([&] (hb_tag_t tag) {
            वापस !_table_is_empty (plan->source, tag) && !plan->no_subset_tables.has (tag);
          })
          | hb_map ([] (hb_tag_t tag) -> hb_tag_t { वापस tag; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_tag_t tag) {
            वापस !_table_is_empty (plan->source, tag);
          }));

  it += start_offset;

  अचिन्हित अंक_written = 0;
  जबतक (bool (it) && अंक_written < *table_गिनती_2)
    table_tags[अंक_written++] = *it++;

  *table_गिनती_2 = अंक_written;
  वापस अंक_written;
}


अटल bool
_is_table_present (hb_face_t *source, hb_tag_t tag)
{

  अगर (!hb_face_get_table_tags (source, 0, nullptr, nullptr)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    वापस !_table_is_empty (source, tag);
  }

  hb_tag_t table_tags[32];
  अचिन्हित offset = 0, अंक_tables = ARRAY_लंबाई (table_tags);
  जबतक (((व्योम) hb_face_get_table_tags (source, offset, &अंक_tables, table_tags), अंक_tables))
  {
    क्रम (अचिन्हित i = 0; i < अंक_tables; ++i)
      अगर (table_tags[i] == tag)
	वापस true;
    offset += अंक_tables;
  }
  वापस false;
}

अटल bool
_should_drop_table (hb_subset_plan_t *plan, hb_tag_t tag)
{
  अगर (plan->drop_tables.has (tag))
    वापस true;

  चयन (tag)
  {
  हाल HB_TAG('c','v','a','r'): /* hint table, fallthrough */
    वापस plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  हाल HB_TAG('c','v','t',' '): /* hint table, fallthrough */
  हाल HB_TAG('f','p','g','m'): /* hint table, fallthrough */
  हाल HB_TAG('p','r','e','p'): /* hint table, fallthrough */
  हाल HB_TAG('h','d','m','x'): /* hint table, fallthrough */
  हाल HB_TAG('V','D','M','X'): /* hint table, fallthrough */
    वापस plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#अगर_घोषित HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  हाल HB_TAG('G','D','E','F'):
  हाल HB_TAG('G','P','O','S'):
  हाल HB_TAG('G','S','U','B'):
  हाल HB_TAG('m','o','r','x'):
  हाल HB_TAG('m','o','r','t'):
  हाल HB_TAG('k','e','r','x'):
  हाल HB_TAG('k','e','r','n'):
    वापस true;
#पूर्ण_अगर

  हाल HB_TAG('a','v','a','r'):
  हाल HB_TAG('f','v','a','r'):
  हाल HB_TAG('g','v','a','r'):
#अगर_अघोषित HB_NO_BEYOND_64K
  हाल HB_TAG('G','V','A','R'):
#पूर्ण_अगर
  हाल HB_TAG('H','V','A','R'):
  हाल HB_TAG('V','V','A','R'):
  हाल HB_TAG('M','V','A','R'):
    वापस plan->all_axes_pinned;

  शेष:
    वापस false;
  }
}

अटल bool
_dependencies_satisfied (hb_subset_plan_t *plan, hb_tag_t tag,
                         स्थिर hb_set_t &subsetted_tags,
                         स्थिर hb_set_t &pending_subset_tags)
{
  bool outline_pending = pending_subset_tags.has (HB_TAG('g','l','y','f'));
#अगर_अघोषित HB_NO_BEYOND_64K
  outline_pending |= pending_subset_tags.has (HB_TAG('G','L','Y','F'));
#पूर्ण_अगर

  चयन (tag)
  {
  हाल HB_TAG('h','m','t','x'):
  हाल HB_TAG('v','m','t','x'):
  हाल HB_TAG('m','a','x','p'):
#अगर_अघोषित HB_NO_BEYOND_64K
  हाल HB_TAG('H','M','T','X'):
  हाल HB_TAG('V','M','T','X'):
  हाल HB_TAG('M','A','X','P'):
#पूर्ण_अगर
  हाल HB_TAG('O','S','/','2'):
    वापस !plan->normalized_coords || !outline_pending;
  हाल HB_TAG('G','P','O','S'):
  हाल HB_TAG('G','S','U','B'):
    वापस plan->all_axes_pinned || !pending_subset_tags.has (HB_TAG('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  हाल HB_TAG('g','v','a','r'):
#अगर_अघोषित HB_NO_BEYOND_64K
  हाल HB_TAG('G','V','A','R'):
#पूर्ण_अगर
  हाल HB_TAG('c','v','a','r'):
  हाल HB_TAG('H','V','A','R'):
  हाल HB_TAG('V','V','A','R'):
  हाल HB_TAG('M','V','A','R'):
  हाल HB_TAG('G','D','E','F'):
  हाल HB_TAG('C','F','F','2'):
    वापस !plan->has_avar2 || !pending_subset_tags.has (HB_TAG('a','v','a','r'));
  शेष:
    वापस true;
  }
}

अटल bool
_subset_table (hb_subset_plan_t *plan,
	       hb_vector_t<अक्षर> &buf,
	       hb_tag_t tag)
{
  अगर (plan->no_subset_tables.has (tag)) {
    वापस _hb_subset_table_passthrough (plan, tag);
  }

  DEBUG_संदेश (SUBSET, nullptr, "subset %c%c%c%c", HB_UNTAG (tag));

  bool success;
  अगर (_hb_subset_table_layout (plan, buf, tag, &success) ||
      _hb_subset_table_var (plan, buf, tag, &success) ||
      _hb_subset_table_cff (plan, buf, tag, &success) ||
      _hb_subset_table_color (plan, buf, tag, &success) ||
      _hb_subset_table_other (plan, buf, tag, &success))
    वापस success;


  चयन (tag)
  {
  हाल HB_TAG('h','e','a','d'):
  {
    bool outline_table_handles_शीर्ष =
	_is_table_present (plan->source, HB_TAG('g','l','y','f')) &&
	!_should_drop_table (plan, HB_TAG('g','l','y','f'));
#अगर_अघोषित HB_NO_BEYOND_64K
    outline_table_handles_शीर्ष |=
	_is_table_present (plan->source, HB_TAG('G','L','Y','F')) &&
	!_should_drop_table (plan, HB_TAG('G','L','Y','F'));
#पूर्ण_अगर
    अगर (outline_table_handles_शीर्ष)
      वापस true; /* skip head, handled by the outline table */
    वापस _hb_subset_table<स्थिर OT::शीर्ष_2> (plan, buf);
  }

  हाल HB_TAG('S','T','A','T'):
    अगर (!plan->user_axes_location.is_empty ()) वापस _hb_subset_table<स्थिर OT::STAT> (plan, buf);
    अन्यथा वापस _hb_subset_table_passthrough (plan, tag);

  हाल HB_TAG('c','v','t',' '):
#अगर_अघोषित HB_NO_VAR
    अगर (_is_table_present (plan->source, HB_TAG('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      स्वतः &cvar = *plan->source->table.cvar;
      वापस OT::cvar::add_cvt_and_apply_deltas (plan, cvar.get_tuple_var_तथ्य (), &cvar);
    }
#पूर्ण_अगर
    वापस _hb_subset_table_passthrough (plan, tag);
  }

  अगर (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    वापस _hb_subset_table_passthrough (plan, tag);

  // Drop table
  वापस true;
}

अटल व्योम attach_accelerator_तथ्य_2 (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  अगर (!plan->inprogress_accelerator) वापस;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = nullptr;

  अगर (accel->in_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    वापस;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_table<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  अगर (!hb_face_set_user_तथ्य(face,
                             hb_subset_accelerator_t::user_तथ्य_कुंजी(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             true))
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
hb_subset_or_fail (hb_face_t *source, स्थिर hb_subset_निवेश_t *निवेश)
{
  अगर (unlikely (!निवेश || !source)) वापस nullptr;

  अगर (unlikely (!source->get_अंक_glyphs ()))
  {
    DEBUG_संदेश (SUBSET, nullptr, "No glyphs in source font.");
    वापस nullptr;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (source, निवेश);
  अगर (unlikely (!plan)) {
    वापस nullptr;
  }

  hb_face_t * परिणाम = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  वापस परिणाम;
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
  अगर (unlikely (!plan || plan->in_error ())) {
    वापस nullptr;
  }

  hb_tag_t table_tags[32];
  अचिन्हित offset = 0, अंक_tables = ARRAY_लंबाई (table_tags);

  hb_set_t subsetted_tags, pending_subset_tags;
  जबतक (((व्योम) _get_table_tags (plan, offset, &अंक_tables, table_tags), अंक_tables))
  {
    क्रम (अचिन्हित i = 0; i < अंक_tables; ++i)
    {
      hb_tag_t tag = table_tags[i];
      अगर (_should_drop_table (plan, tag)) जारी;
      pending_subset_tags.add (tag);
    }

    offset += अंक_tables;
  }

  bool success = true;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_vector_t<अक्षर> buf;
    buf.alloc (8192 - 16);

    जबतक (!pending_subset_tags.is_empty ())
    {
      अगर (subsetted_tags.in_error ()
	  || pending_subset_tags.in_error ()) {
	success = false;
	जाओ end;
      }

      bool made_changes = false;
      क्रम (hb_tag_t tag : pending_subset_tags)
      {
	अगर (!_dependencies_satisfied (plan, tag,
				      subsetted_tags,
				      pending_subset_tags))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  जारी;
	}

	pending_subset_tags.del (tag);
	subsetted_tags.add (tag);
	made_changes = true;

	success = _subset_table (plan, buf, tag);
	अगर (unlikely (!success)) जाओ end;
      }

      अगर (!made_changes)
      {
	DEBUG_संदेश (SUBSET, nullptr, "Table dependencies unable to be satisfied. Subset failed.");
	success = false;
	जाओ end;
      }
    }
  }

  अगर (success && plan->attach_accelerator_तथ्य) {
    attach_accelerator_तथ्य_2 (plan, plan->dest);
  }

end:
  वापस success ? hb_face_reference (plan->dest) : nullptr;
}
