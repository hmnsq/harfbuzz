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

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-open-file.hh"

#అన్తర్భావ "hb-subset.hh"
#అన్తర్భావ "hb-subset-table.hh"
#అన్తర్భావ "hb-subset-accelerator.hh"

#అన్తర్భావ "hb-ot-cmap-table.hh"
#అన్తర్భావ "hb-ot-var-cvar-table.hh"
#అన్తర్భావ "hb-ot-head-table.hh"
#అన్తర్భావ "hb-ot-stat-table.hh"
#అన్తర్భావ "hb-ot-post-table-v2subset.hh"


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


hb_user_దత్తాంశం_కీలకం_t hb_subset_accelerator_user_దత్తాంశం_కీలకం = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
స్థిర hb_ట్యాగ్_t తెలిసిన_tables[] {
  HB_ట్యాగ్('a','v','a','r'),
  HB_ట్యాగ్('B','A','S','E'),
  HB_ట్యాగ్('C','B','D','T'),
  HB_ట్యాగ్('C','B','L','C'),
  HB_ట్యాగ్('C','F','F',' '),
  HB_ట్యాగ్('C','F','F','2'),
  HB_ట్యాగ్('c','m','a','p'),
  HB_ట్యాగ్('C','O','L','R'),
  HB_ట్యాగ్('C','P','A','L'),
  HB_ట్యాగ్('c','v','a','r'),
  HB_ట్యాగ్('c','v','t',' '),
  HB_ట్యాగ్('D','S','I','G'),
  HB_ట్యాగ్('E','B','D','T'),
  HB_ట్యాగ్('E','B','L','C'),
  HB_ట్యాగ్('E','B','S','C'),
  HB_ట్యాగ్('f','p','g','m'),
  HB_ట్యాగ్('f','v','a','r'),
  HB_ట్యాగ్('g','a','s','p'),
  HB_ట్యాగ్('G','D','E','F'),
  HB_ట్యాగ్('g','l','y','f'),
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  HB_ట్యాగ్('G','L','Y','F'),
#అయితే_ముగింపు
  HB_ట్యాగ్('G','P','O','S'),
  HB_ట్యాగ్('G','S','U','B'),
  HB_ట్యాగ్('g','v','a','r'),
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  HB_ట్యాగ్('G','V','A','R'),
#అయితే_ముగింపు
  HB_ట్యాగ్('h','d','m','x'),
  HB_ట్యాగ్('h','e','a','d'),
  HB_ట్యాగ్('h','h','e','a'),
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  HB_ట్యాగ్('H','H','E','A'),
#అయితే_ముగింపు
  HB_ట్యాగ్('h','m','t','x'),
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  HB_ట్యాగ్('H','M','T','X'),
#అయితే_ముగింపు
  HB_ట్యాగ్('H','V','A','R'),
  HB_ట్యాగ్('J','S','T','F'),
  HB_ట్యాగ్('k','e','r','n'),
  HB_ట్యాగ్('l','o','c','a'),
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  HB_ట్యాగ్('L','O','C','A'),
#అయితే_ముగింపు
  HB_ట్యాగ్('L','T','S','H'),
  HB_ట్యాగ్('M','A','T','H'),
  HB_ట్యాగ్('m','a','x','p'),
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  HB_ట్యాగ్('M','A','X','P'),
#అయితే_ముగింపు
  HB_ట్యాగ్('M','E','R','G'),
  HB_ట్యాగ్('m','e','t','a'),
  HB_ట్యాగ్('M','V','A','R'),
  HB_ట్యాగ్('P','C','L','T'),
  HB_ట్యాగ్('p','o','s','t'),
  HB_ట్యాగ్('p','r','e','p'),
  HB_ట్యాగ్('s','b','i','x'),
  HB_ట్యాగ్('S','T','A','T'),
  HB_ట్యాగ్('S','V','G',' '),
  HB_ట్యాగ్('V','D','M','X'),
  HB_ట్యాగ్('v','h','e','a'),
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  HB_ట్యాగ్('V','H','E','A'),
#అయితే_ముగింపు
  HB_ట్యాగ్('v','m','t','x'),
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  HB_ట్యాగ్('V','M','T','X'),
#అయితే_ముగింపు
  HB_ట్యాగ్('V','O','R','G'),
  HB_ట్యాగ్('V','A','R','C'),
  HB_ట్యాగ్('V','V','A','R'),
  HB_ట్యాగ్('n','a','m','e'),
  HB_ట్యాగ్('O','S','/','2')
};

స్థిర తార్కిక పట్టిక_is_ఖాళీలు (స్థిరరాశి hb_face_t *face, hb_ట్యాగ్_t ట్యాగ్_2)
{
  hb_blob_t* blob = hb_face_reference_పట్టిక (face, ట్యాగ్_2);
  తార్కిక ఫలితం = (blob == hb_blob_get_ఖాళీలు ());
  hb_blob_destroy (blob);
  తిరిగి ఫలితం;
}

స్థిర గుర్తులేని పూర్ణాంకం
get_పట్టిక_ట్యాగులు_2 (స్థిరరాశి hb_subset_plan_t* plan,
                 గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
                 గుర్తులేని పూర్ణాంకం *పట్టిక_లెక్క_2, /* IN/OUT */
                 hb_ట్యాగ్_t     *పట్టిక_ట్యాగులు /*బహిర్గతం*/)
{
  గుర్తులేని అంకె_tables = hb_face_get_పట్టిక_ట్యాగులు (plan->ఆధారం, 0, శూన్యసూచిక, శూన్యసూచిక);
  ఒకవేళ (అంకె_tables)
    తిరిగి hb_face_get_పట్టిక_ట్యాగులు (plan->ఆధారం, ప్రారంభం_స్థానభ్రంశం, పట్టిక_లెక్క_2, పట్టిక_ట్యాగులు);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  స్వయంచాలక it =
      hb_concat (
          + hb_array (తెలిసిన_tables)
          | hb_filter ([&] (hb_ట్యాగ్_t ట్యాగ్_2) {
            తిరిగి !పట్టిక_is_ఖాళీలు (plan->ఆధారం, ట్యాగ్_2) && !plan->no_subset_tables.has (ట్యాగ్_2);
          })
          | hb_map ([] (hb_ట్యాగ్_t ట్యాగ్_2) -> hb_ట్యాగ్_t { తిరిగి ట్యాగ్_2; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_ట్యాగ్_t ట్యాగ్_2) {
            తిరిగి !పట్టిక_is_ఖాళీలు (plan->ఆధారం, ట్యాగ్_2);
          }));

  it += ప్రారంభం_స్థానభ్రంశం;

  గుర్తులేని అంకె_written = 0;
  అయితే (తార్కిక (it) && అంకె_written < *పట్టిక_లెక్క_2)
    పట్టిక_ట్యాగులు[అంకె_written++] = *it++;

  *పట్టిక_లెక్క_2 = అంకె_written;
  తిరిగి అంకె_written;
}


స్థిర తార్కిక
is_పట్టిక_present (hb_face_t *ఆధారం, hb_ట్యాగ్_t ట్యాగ్_2)
{

  ఒకవేళ (!hb_face_get_పట్టిక_ట్యాగులు (ఆధారం, 0, శూన్యసూచిక, శూన్యసూచిక)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    తిరిగి !పట్టిక_is_ఖాళీలు (ఆధారం, ట్యాగ్_2);
  }

  hb_ట్యాగ్_t పట్టిక_ట్యాగులు[32];
  గుర్తులేని స్థానభ్రంశం = 0, అంకె_tables = ARRAY_నిడివి (పట్టిక_ట్యాగులు);
  అయితే (((శూన్య) hb_face_get_పట్టిక_ట్యాగులు (ఆధారం, స్థానభ్రంశం, &అంకె_tables, పట్టిక_ట్యాగులు), అంకె_tables))
  {
    కొరకు (గుర్తులేని i = 0; i < అంకె_tables; ++i)
      ఒకవేళ (పట్టిక_ట్యాగులు[i] == ట్యాగ్_2)
	తిరిగి నిజం;
    స్థానభ్రంశం += అంకె_tables;
  }
  తిరిగి అబద్ధం;
}

స్థిర తార్కిక
should_drop_పట్టిక (hb_subset_plan_t *plan, hb_ట్యాగ్_t ట్యాగ్_2)
{
  ఒకవేళ (plan->drop_tables.has (ట్యాగ్_2))
    తిరిగి నిజం;

  ఎంపిక (ట్యాగ్_2)
  {
  స్థితి HB_ట్యాగ్('c','v','a','r'): /* hint table, fallthrough */
    తిరిగి plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  స్థితి HB_ట్యాగ్('c','v','t',' '): /* hint table, fallthrough */
  స్థితి HB_ట్యాగ్('f','p','g','m'): /* hint table, fallthrough */
  స్థితి HB_ట్యాగ్('p','r','e','p'): /* hint table, fallthrough */
  స్థితి HB_ట్యాగ్('h','d','m','x'): /* hint table, fallthrough */
  స్థితి HB_ట్యాగ్('V','D','M','X'): /* hint table, fallthrough */
    తిరిగి plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#నిర్వచితమైతే HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  స్థితి HB_ట్యాగ్('G','D','E','F'):
  స్థితి HB_ట్యాగ్('G','P','O','S'):
  స్థితి HB_ట్యాగ్('G','S','U','B'):
  స్థితి HB_ట్యాగ్('m','o','r','x'):
  స్థితి HB_ట్యాగ్('m','o','r','t'):
  స్థితి HB_ట్యాగ్('k','e','r','x'):
  స్థితి HB_ట్యాగ్('k','e','r','n'):
    తిరిగి నిజం;
#అయితే_ముగింపు

  స్థితి HB_ట్యాగ్('a','v','a','r'):
  స్థితి HB_ట్యాగ్('f','v','a','r'):
  స్థితి HB_ట్యాగ్('g','v','a','r'):
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  స్థితి HB_ట్యాగ్('G','V','A','R'):
#అయితే_ముగింపు
  స్థితి HB_ట్యాగ్('H','V','A','R'):
  స్థితి HB_ట్యాగ్('V','V','A','R'):
  స్థితి HB_ట్యాగ్('M','V','A','R'):
    తిరిగి plan->all_axes_pinned;

  అప్రమేయ:
    తిరిగి అబద్ధం;
  }
}

స్థిర తార్కిక
_dependencies_satisfied (hb_subset_plan_t *plan, hb_ట్యాగ్_t ట్యాగ్_2,
                         స్థిరరాశి hb_సమితి_t &subsetted_ట్యాగులు,
                         స్థిరరాశి hb_సమితి_t &pending_subset_ట్యాగులు)
{
  తార్కిక outline_pending = pending_subset_ట్యాగులు.has (HB_ట్యాగ్('g','l','y','f'));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  outline_pending |= pending_subset_ట్యాగులు.has (HB_ట్యాగ్('G','L','Y','F'));
#అయితే_ముగింపు

  ఎంపిక (ట్యాగ్_2)
  {
  స్థితి HB_ట్యాగ్('h','m','t','x'):
  స్థితి HB_ట్యాగ్('v','m','t','x'):
  స్థితి HB_ట్యాగ్('m','a','x','p'):
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  స్థితి HB_ట్యాగ్('H','M','T','X'):
  స్థితి HB_ట్యాగ్('V','M','T','X'):
  స్థితి HB_ట్యాగ్('M','A','X','P'):
#అయితే_ముగింపు
  స్థితి HB_ట్యాగ్('O','S','/','2'):
    తిరిగి !plan->normalized_coords || !outline_pending;
  స్థితి HB_ట్యాగ్('G','P','O','S'):
  స్థితి HB_ట్యాగ్('G','S','U','B'):
    తిరిగి plan->all_axes_pinned || !pending_subset_ట్యాగులు.has (HB_ట్యాగ్('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  స్థితి HB_ట్యాగ్('g','v','a','r'):
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  స్థితి HB_ట్యాగ్('G','V','A','R'):
#అయితే_ముగింపు
  స్థితి HB_ట్యాగ్('c','v','a','r'):
  స్థితి HB_ట్యాగ్('H','V','A','R'):
  స్థితి HB_ట్యాగ్('V','V','A','R'):
  స్థితి HB_ట్యాగ్('M','V','A','R'):
  స్థితి HB_ట్యాగ్('G','D','E','F'):
  స్థితి HB_ట్యాగ్('C','F','F','2'):
    తిరిగి !plan->has_avar2 || !pending_subset_ట్యాగులు.has (HB_ట్యాగ్('a','v','a','r'));
  అప్రమేయ:
    తిరిగి నిజం;
  }
}

స్థిర తార్కిక
subset_పట్టిక (hb_subset_plan_t *plan,
	       hb_సదిశ_t<అక్షరం> &buf,
	       hb_ట్యాగ్_t ట్యాగ్_2)
{
  ఒకవేళ (plan->no_subset_tables.has (ట్యాగ్_2)) {
    తిరిగి hb_subset_పట్టిక_passthrough (plan, ట్యాగ్_2);
  }

  DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "subset %c%c%c%c", HB_UNTAG (ట్యాగ్_2));

  తార్కిక success;
  ఒకవేళ (hb_subset_పట్టిక_layout (plan, buf, ట్యాగ్_2, &success) ||
      hb_subset_పట్టిక_var (plan, buf, ట్యాగ్_2, &success) ||
      hb_subset_పట్టిక_cff (plan, buf, ట్యాగ్_2, &success) ||
      hb_subset_పట్టిక_color (plan, buf, ట్యాగ్_2, &success) ||
      hb_subset_పట్టిక_other (plan, buf, ట్యాగ్_2, &success))
    తిరిగి success;


  ఎంపిక (ట్యాగ్_2)
  {
  స్థితి HB_ట్యాగ్('h','e','a','d'):
  {
    తార్కిక outline_పట్టిక_handles_తల =
	is_పట్టిక_present (plan->ఆధారం, HB_ట్యాగ్('g','l','y','f')) &&
	!should_drop_పట్టిక (plan, HB_ట్యాగ్('g','l','y','f'));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    outline_పట్టిక_handles_తల |=
	is_పట్టిక_present (plan->ఆధారం, HB_ట్యాగ్('G','L','Y','F')) &&
	!should_drop_పట్టిక (plan, HB_ట్యాగ్('G','L','Y','F'));
#అయితే_ముగింపు
    ఒకవేళ (outline_పట్టిక_handles_తల)
      తిరిగి నిజం; /* skip head, handled by the outline table */
    తిరిగి hb_subset_పట్టిక<స్థిరరాశి OT::తల_2> (plan, buf);
  }

  స్థితి HB_ట్యాగ్('S','T','A','T'):
    ఒకవేళ (!plan->user_axes_location.is_empty ()) తిరిగి hb_subset_పట్టిక<స్థిరరాశి OT::STAT> (plan, buf);
    లేకపోతే తిరిగి hb_subset_పట్టిక_passthrough (plan, ట్యాగ్_2);

  స్థితి HB_ట్యాగ్('c','v','t',' '):
#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (is_పట్టిక_present (plan->ఆధారం, HB_ట్యాగ్('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      స్వయంచాలక &cvar = *plan->ఆధారం->పట్టిక.cvar;
      తిరిగి OT::cvar::కూడు_cvt_and_apply_deltas (plan, cvar.get_tuple_var_దత్తాంశం (), &cvar);
    }
#అయితే_ముగింపు
    తిరిగి hb_subset_పట్టిక_passthrough (plan, ట్యాగ్_2);
  }

  ఒకవేళ (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    తిరిగి hb_subset_పట్టిక_passthrough (plan, ట్యాగ్_2);

  // Drop table
  తిరిగి నిజం;
}

స్థిర శూన్య attach_accelerator_దత్తాంశం_2 (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  ఒకవేళ (!plan->inprogress_accelerator) తిరిగి;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = శూన్యసూచిక;

  ఒకవేళ (accel->లోపల_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    తిరిగి;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_పట్టిక<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  ఒకవేళ (!hb_face_సమితి_user_దత్తాంశం(face,
                             hb_subset_accelerator_t::user_దత్తాంశం_కీలకం(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             నిజం))
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
hb_subset_or_fail (hb_face_t *ఆధారం, స్థిరరాశి hb_subset_ఆదానం_t *ఆదానం)
{
  ఒకవేళ (unlikely (!ఆదానం || !ఆధారం)) తిరిగి శూన్యసూచిక;

  ఒకవేళ (unlikely (!ఆధారం->get_అంకె_glyphs ()))
  {
    DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "No glyphs in source font.");
    తిరిగి శూన్యసూచిక;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (ఆధారం, ఆదానం);
  ఒకవేళ (unlikely (!plan)) {
    తిరిగి శూన్యసూచిక;
  }

  hb_face_t * ఫలితం = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  తిరిగి ఫలితం;
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
  ఒకవేళ (unlikely (!plan || plan->లోపల_error ())) {
    తిరిగి శూన్యసూచిక;
  }

  hb_ట్యాగ్_t పట్టిక_ట్యాగులు[32];
  గుర్తులేని స్థానభ్రంశం = 0, అంకె_tables = ARRAY_నిడివి (పట్టిక_ట్యాగులు);

  hb_సమితి_t subsetted_ట్యాగులు, pending_subset_ట్యాగులు;
  అయితే (((శూన్య) get_పట్టిక_ట్యాగులు_2 (plan, స్థానభ్రంశం, &అంకె_tables, పట్టిక_ట్యాగులు), అంకె_tables))
  {
    కొరకు (గుర్తులేని i = 0; i < అంకె_tables; ++i)
    {
      hb_ట్యాగ్_t ట్యాగ్_2 = పట్టిక_ట్యాగులు[i];
      ఒకవేళ (should_drop_పట్టిక (plan, ట్యాగ్_2)) కొనసాగు;
      pending_subset_ట్యాగులు.కూడు (ట్యాగ్_2);
    }

    స్థానభ్రంశం += అంకె_tables;
  }

  తార్కిక success = నిజం;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_సదిశ_t<అక్షరం> buf;
    buf.alloc (8192 - 16);

    అయితే (!pending_subset_ట్యాగులు.is_empty ())
    {
      ఒకవేళ (subsetted_ట్యాగులు.లోపల_error ()
	  || pending_subset_ట్యాగులు.లోపల_error ()) {
	success = అబద్ధం;
	వెళ్ళు చివర;
      }

      తార్కిక made_changes = అబద్ధం;
      కొరకు (hb_ట్యాగ్_t ట్యాగ్_2 : pending_subset_ట్యాగులు)
      {
	ఒకవేళ (!_dependencies_satisfied (plan, ట్యాగ్_2,
				      subsetted_ట్యాగులు,
				      pending_subset_ట్యాగులు))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  కొనసాగు;
	}

	pending_subset_ట్యాగులు.del (ట్యాగ్_2);
	subsetted_ట్యాగులు.కూడు (ట్యాగ్_2);
	made_changes = నిజం;

	success = subset_పట్టిక (plan, buf, ట్యాగ్_2);
	ఒకవేళ (unlikely (!success)) వెళ్ళు చివర;
      }

      ఒకవేళ (!made_changes)
      {
	DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "Table dependencies unable to be satisfied. Subset failed.");
	success = అబద్ధం;
	వెళ్ళు చివర;
      }
    }
  }

  ఒకవేళ (success && plan->attach_accelerator_దత్తాంశం) {
    attach_accelerator_దత్తాంశం_2 (plan, plan->dest);
  }

చివర:
  తిరిగి success ? hb_face_reference (plan->dest) : శూన్యసూచిక;
}
