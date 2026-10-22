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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-open-file.hh"

#अन्तर्भाव "hb-subset.hh"
#अन्तर्भाव "hb-subset-table.hh"
#अन्तर्भाव "hb-subset-accelerator.hh"

#अन्तर्भाव "hb-ot-cmap-table.hh"
#अन्तर्भाव "hb-ot-var-cvar-table.hh"
#अन्तर्भाव "hb-ot-head-table.hh"
#अन्तर्भाव "hb-ot-stat-table.hh"
#अन्तर्भाव "hb-ot-post-table-v2subset.hh"


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


hb_user_data_किल्ली_t hb_subset_accelerator_user_data_किल्ली = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
स्थिर hb_tag_t known_tables[] {
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

स्थिर तार्किक _table_is_empty (स्थिरांक hb_face_t *face, hb_tag_t tag)
{
  hb_blob_t* blob = hb_face_reference_table (face, tag);
  तार्किक result = (blob == hb_blob_get_empty ());
  hb_blob_destroy (blob);
  परत result;
}

स्थिर अचिन्हित पूर्णांक
_get_table_tags (स्थिरांक hb_subset_plan_t* plan,
                 अचिन्हित पूर्णांक  start_offset,
                 अचिन्हित पूर्णांक *table_मोजणी_2, /* IN/OUT */
                 hb_tag_t     *table_tags /* OUT */)
{
  अचिन्हित अंक_tables = hb_face_get_table_tags (plan->source, 0, nullptr, nullptr);
  जर (अंक_tables)
    परत hb_face_get_table_tags (plan->source, start_offset, table_मोजणी_2, table_tags);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  स्वतः it =
      hb_concat (
          + hb_array (known_tables)
          | hb_filter ([&] (hb_tag_t tag) {
            परत !_table_is_empty (plan->source, tag) && !plan->no_subset_tables.has (tag);
          })
          | hb_map ([] (hb_tag_t tag) -> hb_tag_t { परत tag; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_tag_t tag) {
            परत !_table_is_empty (plan->source, tag);
          }));

  it += start_offset;

  अचिन्हित अंक_written = 0;
  पर्यंत (तार्किक (it) && अंक_written < *table_मोजणी_2)
    table_tags[अंक_written++] = *it++;

  *table_मोजणी_2 = अंक_written;
  परत अंक_written;
}


स्थिर तार्किक
_is_table_present (hb_face_t *source, hb_tag_t tag)
{

  जर (!hb_face_get_table_tags (source, 0, nullptr, nullptr)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    परत !_table_is_empty (source, tag);
  }

  hb_tag_t table_tags[32];
  अचिन्हित offset = 0, अंक_tables = ARRAY_LENGTH (table_tags);
  पर्यंत (((शून्य) hb_face_get_table_tags (source, offset, &अंक_tables, table_tags), अंक_tables))
  {
    साठी (अचिन्हित i = 0; i < अंक_tables; ++i)
      जर (table_tags[i] == tag)
	परत सत्य;
    offset += अंक_tables;
  }
  परत असत्य;
}

स्थिर तार्किक
_should_drop_table (hb_subset_plan_t *plan, hb_tag_t tag)
{
  जर (plan->drop_tables.has (tag))
    परत सत्य;

  निवड (tag)
  {
  स्थिती HB_TAG('c','v','a','r'): /* hint table, fallthrough */
    परत plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  स्थिती HB_TAG('c','v','t',' '): /* hint table, fallthrough */
  स्थिती HB_TAG('f','p','g','m'): /* hint table, fallthrough */
  स्थिती HB_TAG('p','r','e','p'): /* hint table, fallthrough */
  स्थिती HB_TAG('h','d','m','x'): /* hint table, fallthrough */
  स्थिती HB_TAG('V','D','M','X'): /* hint table, fallthrough */
    परत plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#ifdef HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  स्थिती HB_TAG('G','D','E','F'):
  स्थिती HB_TAG('G','P','O','S'):
  स्थिती HB_TAG('G','S','U','B'):
  स्थिती HB_TAG('m','o','r','x'):
  स्थिती HB_TAG('m','o','r','t'):
  स्थिती HB_TAG('k','e','r','x'):
  स्थिती HB_TAG('k','e','r','n'):
    परत सत्य;
#endif

  स्थिती HB_TAG('a','v','a','r'):
  स्थिती HB_TAG('f','v','a','r'):
  स्थिती HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  स्थिती HB_TAG('G','V','A','R'):
#endif
  स्थिती HB_TAG('H','V','A','R'):
  स्थिती HB_TAG('V','V','A','R'):
  स्थिती HB_TAG('M','V','A','R'):
    परत plan->all_axes_pinned;

  पूर्वनिर्धारित:
    परत असत्य;
  }
}

स्थिर तार्किक
_dependencies_satisfied (hb_subset_plan_t *plan, hb_tag_t tag,
                         स्थिरांक hb_set_t &subsetted_tags,
                         स्थिरांक hb_set_t &pending_subset_tags)
{
  तार्किक outline_pending = pending_subset_tags.has (HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
  outline_pending |= pending_subset_tags.has (HB_TAG('G','L','Y','F'));
#endif

  निवड (tag)
  {
  स्थिती HB_TAG('h','m','t','x'):
  स्थिती HB_TAG('v','m','t','x'):
  स्थिती HB_TAG('m','a','x','p'):
#ifndef HB_NO_BEYOND_64K
  स्थिती HB_TAG('H','M','T','X'):
  स्थिती HB_TAG('V','M','T','X'):
  स्थिती HB_TAG('M','A','X','P'):
#endif
  स्थिती HB_TAG('O','S','/','2'):
    परत !plan->normalized_coords || !outline_pending;
  स्थिती HB_TAG('G','P','O','S'):
  स्थिती HB_TAG('G','S','U','B'):
    परत plan->all_axes_pinned || !pending_subset_tags.has (HB_TAG('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  स्थिती HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  स्थिती HB_TAG('G','V','A','R'):
#endif
  स्थिती HB_TAG('c','v','a','r'):
  स्थिती HB_TAG('H','V','A','R'):
  स्थिती HB_TAG('V','V','A','R'):
  स्थिती HB_TAG('M','V','A','R'):
  स्थिती HB_TAG('G','D','E','F'):
  स्थिती HB_TAG('C','F','F','2'):
    परत !plan->has_avar2 || !pending_subset_tags.has (HB_TAG('a','v','a','r'));
  पूर्वनिर्धारित:
    परत सत्य;
  }
}

स्थिर तार्किक
_subset_table (hb_subset_plan_t *plan,
	       hb_vector_t<अक्षर> &buf,
	       hb_tag_t tag)
{
  जर (plan->no_subset_tables.has (tag)) {
    परत _hb_subset_table_passthrough (plan, tag);
  }

  DEBUG_संदेश (SUBSET, nullptr, "subset %c%c%c%c", HB_UNTAG (tag));

  तार्किक success;
  जर (_hb_subset_table_layout (plan, buf, tag, &success) ||
      _hb_subset_table_var (plan, buf, tag, &success) ||
      _hb_subset_table_cff (plan, buf, tag, &success) ||
      _hb_subset_table_color (plan, buf, tag, &success) ||
      _hb_subset_table_other (plan, buf, tag, &success))
    परत success;


  निवड (tag)
  {
  स्थिती HB_TAG('h','e','a','d'):
  {
    तार्किक outline_table_handles_head =
	_is_table_present (plan->source, HB_TAG('g','l','y','f')) &&
	!_should_drop_table (plan, HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
    outline_table_handles_head |=
	_is_table_present (plan->source, HB_TAG('G','L','Y','F')) &&
	!_should_drop_table (plan, HB_TAG('G','L','Y','F'));
#endif
    जर (outline_table_handles_head)
      परत सत्य; /* skip head, handled by the outline table */
    परत _hb_subset_table<स्थिरांक OT::head> (plan, buf);
  }

  स्थिती HB_TAG('S','T','A','T'):
    जर (!plan->user_axes_location.is_empty ()) परत _hb_subset_table<स्थिरांक OT::STAT> (plan, buf);
    अन्यथा परत _hb_subset_table_passthrough (plan, tag);

  स्थिती HB_TAG('c','v','t',' '):
#ifndef HB_NO_VAR
    जर (_is_table_present (plan->source, HB_TAG('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      स्वतः &cvar = *plan->source->table.cvar;
      परत OT::cvar::add_cvt_and_apply_deltas (plan, cvar.get_tuple_var_data (), &cvar);
    }
#endif
    परत _hb_subset_table_passthrough (plan, tag);
  }

  जर (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    परत _hb_subset_table_passthrough (plan, tag);

  // Drop table
  परत सत्य;
}

स्थिर शून्य _attach_accelerator_data (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  जर (!plan->inprogress_accelerator) परत;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = nullptr;

  जर (accel->in_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    परत;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_table<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  जर (!hb_face_set_user_data(face,
                             hb_subset_accelerator_t::user_data_किल्ली(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             सत्य))
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
hb_subset_or_fail (hb_face_t *source, स्थिरांक hb_subset_आदान_t *आदान)
{
  जर (unlikely (!आदान || !source)) परत nullptr;

  जर (unlikely (!source->get_अंक_glyphs ()))
  {
    DEBUG_संदेश (SUBSET, nullptr, "No glyphs in source font.");
    परत nullptr;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (source, आदान);
  जर (unlikely (!plan)) {
    परत nullptr;
  }

  hb_face_t * result = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  परत result;
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
  जर (unlikely (!plan || plan->in_error ())) {
    परत nullptr;
  }

  hb_tag_t table_tags[32];
  अचिन्हित offset = 0, अंक_tables = ARRAY_LENGTH (table_tags);

  hb_set_t subsetted_tags, pending_subset_tags;
  पर्यंत (((शून्य) _get_table_tags (plan, offset, &अंक_tables, table_tags), अंक_tables))
  {
    साठी (अचिन्हित i = 0; i < अंक_tables; ++i)
    {
      hb_tag_t tag = table_tags[i];
      जर (_should_drop_table (plan, tag)) सुरूठेवा;
      pending_subset_tags.add (tag);
    }

    offset += अंक_tables;
  }

  तार्किक success = सत्य;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_vector_t<अक्षर> buf;
    buf.alloc (8192 - 16);

    पर्यंत (!pending_subset_tags.is_empty ())
    {
      जर (subsetted_tags.in_error ()
	  || pending_subset_tags.in_error ()) {
	success = असत्य;
	जा end;
      }

      तार्किक made_changes = असत्य;
      साठी (hb_tag_t tag : pending_subset_tags)
      {
	जर (!_dependencies_satisfied (plan, tag,
				      subsetted_tags,
				      pending_subset_tags))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  सुरूठेवा;
	}

	pending_subset_tags.del (tag);
	subsetted_tags.add (tag);
	made_changes = सत्य;

	success = _subset_table (plan, buf, tag);
	जर (unlikely (!success)) जा end;
      }

      जर (!made_changes)
      {
	DEBUG_संदेश (SUBSET, nullptr, "Table dependencies unable to be satisfied. Subset failed.");
	success = असत्य;
	जा end;
      }
    }
  }

  जर (success && plan->attach_accelerator_data) {
    _attach_accelerator_data (plan, plan->dest);
  }

end:
  परत success ? hb_face_reference (plan->dest) : nullptr;
}
