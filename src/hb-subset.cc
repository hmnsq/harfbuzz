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


hb_user_معلومات_ڪنجي_t hb_subset_accelerator_user_معلومات_ڪنجي = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
ساكن hb_ٽيگ_t معلوم_tables[] {
  HB_ٽيگ('a','v','a','r'),
  HB_ٽيگ('B','A','S','E'),
  HB_ٽيگ('C','B','D','T'),
  HB_ٽيگ('C','B','L','C'),
  HB_ٽيگ('C','F','F',' '),
  HB_ٽيگ('C','F','F','2'),
  HB_ٽيگ('c','m','a','p'),
  HB_ٽيگ('C','O','L','R'),
  HB_ٽيگ('C','P','A','L'),
  HB_ٽيگ('c','v','a','r'),
  HB_ٽيگ('c','v','t',' '),
  HB_ٽيگ('D','S','I','G'),
  HB_ٽيگ('E','B','D','T'),
  HB_ٽيگ('E','B','L','C'),
  HB_ٽيگ('E','B','S','C'),
  HB_ٽيگ('f','p','g','m'),
  HB_ٽيگ('f','v','a','r'),
  HB_ٽيگ('g','a','s','p'),
  HB_ٽيگ('G','D','E','F'),
  HB_ٽيگ('g','l','y','f'),
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  HB_ٽيگ('G','L','Y','F'),
#جيڪڏهن_پورو
  HB_ٽيگ('G','P','O','S'),
  HB_ٽيگ('G','S','U','B'),
  HB_ٽيگ('g','v','a','r'),
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  HB_ٽيگ('G','V','A','R'),
#جيڪڏهن_پورو
  HB_ٽيگ('h','d','m','x'),
  HB_ٽيگ('h','e','a','d'),
  HB_ٽيگ('h','h','e','a'),
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  HB_ٽيگ('H','H','E','A'),
#جيڪڏهن_پورو
  HB_ٽيگ('h','m','t','x'),
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  HB_ٽيگ('H','M','T','X'),
#جيڪڏهن_پورو
  HB_ٽيگ('H','V','A','R'),
  HB_ٽيگ('J','S','T','F'),
  HB_ٽيگ('k','e','r','n'),
  HB_ٽيگ('l','o','c','a'),
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  HB_ٽيگ('L','O','C','A'),
#جيڪڏهن_پورو
  HB_ٽيگ('L','T','S','H'),
  HB_ٽيگ('M','A','T','H'),
  HB_ٽيگ('m','a','x','p'),
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  HB_ٽيگ('M','A','X','P'),
#جيڪڏهن_پورو
  HB_ٽيگ('M','E','R','G'),
  HB_ٽيگ('m','e','t','a'),
  HB_ٽيگ('M','V','A','R'),
  HB_ٽيگ('P','C','L','T'),
  HB_ٽيگ('p','o','s','t'),
  HB_ٽيگ('p','r','e','p'),
  HB_ٽيگ('s','b','i','x'),
  HB_ٽيگ('S','T','A','T'),
  HB_ٽيگ('S','V','G',' '),
  HB_ٽيگ('V','D','M','X'),
  HB_ٽيگ('v','h','e','a'),
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  HB_ٽيگ('V','H','E','A'),
#جيڪڏهن_پورو
  HB_ٽيگ('v','m','t','x'),
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  HB_ٽيگ('V','M','T','X'),
#جيڪڏهن_پورو
  HB_ٽيگ('V','O','R','G'),
  HB_ٽيگ('V','A','R','C'),
  HB_ٽيگ('V','V','A','R'),
  HB_ٽيگ('n','a','m','e'),
  HB_ٽيگ('O','S','/','2')
};

ساكن منطقي جدول_is_خالي_جايون (ثابت hb_face_t *face, hb_ٽيگ_t ٽيگ_2)
{
  hb_blob_t* blob = hb_face_reference_جدول (face, ٽيگ_2);
  منطقي نتيجو = (blob == hb_blob_get_خالي_جايون ());
  hb_blob_destroy (blob);
  أرجع نتيجو;
}

ساكن بنانشان صحيح
get_جدول_ٽيگ_فهرست_2 (ثابت hb_subset_plan_t* plan,
                 بنانشان صحيح  شروعات_وٿي,
                 بنانشان صحيح *جدول_ڳڻپ_2, /* IN/OUT */
                 hb_ٽيگ_t     *جدول_ٽيگ_فهرست /*ٻاهريون*/)
{
  بنانشان انگ_tables = hb_face_get_جدول_ٽيگ_فهرست (plan->ذريعو, 0, خالي_اشارو, خالي_اشارو);
  إذا (انگ_tables)
    أرجع hb_face_get_جدول_ٽيگ_فهرست (plan->ذريعو, شروعات_وٿي, جدول_ڳڻپ_2, جدول_ٽيگ_فهرست);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  خودڪار it =
      hb_concat (
          + hb_array (معلوم_tables)
          | hb_filter ([&] (hb_ٽيگ_t ٽيگ_2) {
            أرجع !جدول_is_خالي_جايون (plan->ذريعو, ٽيگ_2) && !plan->no_subset_tables.has (ٽيگ_2);
          })
          | hb_map ([] (hb_ٽيگ_t ٽيگ_2) -> hb_ٽيگ_t { أرجع ٽيگ_2; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_ٽيگ_t ٽيگ_2) {
            أرجع !جدول_is_خالي_جايون (plan->ذريعو, ٽيگ_2);
          }));

  it += شروعات_وٿي;

  بنانشان انگ_written = 0;
  طالما (منطقي (it) && انگ_written < *جدول_ڳڻپ_2)
    جدول_ٽيگ_فهرست[انگ_written++] = *it++;

  *جدول_ڳڻپ_2 = انگ_written;
  أرجع انگ_written;
}


ساكن منطقي
is_جدول_present (hb_face_t *ذريعو, hb_ٽيگ_t ٽيگ_2)
{

  إذا (!hb_face_get_جدول_ٽيگ_فهرست (ذريعو, 0, خالي_اشارو, خالي_اشارو)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    أرجع !جدول_is_خالي_جايون (ذريعو, ٽيگ_2);
  }

  hb_ٽيگ_t جدول_ٽيگ_فهرست[32];
  بنانشان وٿي = 0, انگ_tables = ARRAY_ڊيگهائي (جدول_ٽيگ_فهرست);
  طالما (((فراغ) hb_face_get_جدول_ٽيگ_فهرست (ذريعو, وٿي, &انگ_tables, جدول_ٽيگ_فهرست), انگ_tables))
  {
    لكل (بنانشان i = 0; i < انگ_tables; ++i)
      إذا (جدول_ٽيگ_فهرست[i] == ٽيگ_2)
	أرجع سچ;
    وٿي += انگ_tables;
  }
  أرجع خطأ;
}

ساكن منطقي
should_drop_جدول (hb_subset_plan_t *plan, hb_ٽيگ_t ٽيگ_2)
{
  إذا (plan->drop_tables.has (ٽيگ_2))
    أرجع سچ;

  بدل (ٽيگ_2)
  {
  حالة HB_ٽيگ('c','v','a','r'): /* hint table, fallthrough */
    أرجع plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  حالة HB_ٽيگ('c','v','t',' '): /* hint table, fallthrough */
  حالة HB_ٽيگ('f','p','g','m'): /* hint table, fallthrough */
  حالة HB_ٽيگ('p','r','e','p'): /* hint table, fallthrough */
  حالة HB_ٽيگ('h','d','m','x'): /* hint table, fallthrough */
  حالة HB_ٽيگ('V','D','M','X'): /* hint table, fallthrough */
    أرجع plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#جيڪڏهن_وصف_ٿيل HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  حالة HB_ٽيگ('G','D','E','F'):
  حالة HB_ٽيگ('G','P','O','S'):
  حالة HB_ٽيگ('G','S','U','B'):
  حالة HB_ٽيگ('m','o','r','x'):
  حالة HB_ٽيگ('m','o','r','t'):
  حالة HB_ٽيگ('k','e','r','x'):
  حالة HB_ٽيگ('k','e','r','n'):
    أرجع سچ;
#جيڪڏهن_پورو

  حالة HB_ٽيگ('a','v','a','r'):
  حالة HB_ٽيگ('f','v','a','r'):
  حالة HB_ٽيگ('g','v','a','r'):
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  حالة HB_ٽيگ('G','V','A','R'):
#جيڪڏهن_پورو
  حالة HB_ٽيگ('H','V','A','R'):
  حالة HB_ٽيگ('V','V','A','R'):
  حالة HB_ٽيگ('M','V','A','R'):
    أرجع plan->all_axes_pinned;

  افتراضي:
    أرجع خطأ;
  }
}

ساكن منطقي
_dependencies_satisfied (hb_subset_plan_t *plan, hb_ٽيگ_t ٽيگ_2,
                         ثابت hb_سيٽ_t &subsetted_ٽيگ_فهرست,
                         ثابت hb_سيٽ_t &pending_subset_ٽيگ_فهرست)
{
  منطقي outline_pending = pending_subset_ٽيگ_فهرست.has (HB_ٽيگ('g','l','y','f'));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  outline_pending |= pending_subset_ٽيگ_فهرست.has (HB_ٽيگ('G','L','Y','F'));
#جيڪڏهن_پورو

  بدل (ٽيگ_2)
  {
  حالة HB_ٽيگ('h','m','t','x'):
  حالة HB_ٽيگ('v','m','t','x'):
  حالة HB_ٽيگ('m','a','x','p'):
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  حالة HB_ٽيگ('H','M','T','X'):
  حالة HB_ٽيگ('V','M','T','X'):
  حالة HB_ٽيگ('M','A','X','P'):
#جيڪڏهن_پورو
  حالة HB_ٽيگ('O','S','/','2'):
    أرجع !plan->normalized_coords || !outline_pending;
  حالة HB_ٽيگ('G','P','O','S'):
  حالة HB_ٽيگ('G','S','U','B'):
    أرجع plan->all_axes_pinned || !pending_subset_ٽيگ_فهرست.has (HB_ٽيگ('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  حالة HB_ٽيگ('g','v','a','r'):
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  حالة HB_ٽيگ('G','V','A','R'):
#جيڪڏهن_پورو
  حالة HB_ٽيگ('c','v','a','r'):
  حالة HB_ٽيگ('H','V','A','R'):
  حالة HB_ٽيگ('V','V','A','R'):
  حالة HB_ٽيگ('M','V','A','R'):
  حالة HB_ٽيگ('G','D','E','F'):
  حالة HB_ٽيگ('C','F','F','2'):
    أرجع !plan->has_avar2 || !pending_subset_ٽيگ_فهرست.has (HB_ٽيگ('a','v','a','r'));
  افتراضي:
    أرجع سچ;
  }
}

ساكن منطقي
subset_جدول (hb_subset_plan_t *plan,
	       hb_ويڪٽر_t<حرف> &buf,
	       hb_ٽيگ_t ٽيگ_2)
{
  إذا (plan->no_subset_tables.has (ٽيگ_2)) {
    أرجع hb_subset_جدول_passthrough (plan, ٽيگ_2);
  }

  DEBUG_نياپو (SUBSET, خالي_اشارو, "subset %c%c%c%c", HB_UNTAG (ٽيگ_2));

  منطقي success;
  إذا (hb_subset_جدول_layout (plan, buf, ٽيگ_2, &success) ||
      hb_subset_جدول_var (plan, buf, ٽيگ_2, &success) ||
      hb_subset_جدول_cff (plan, buf, ٽيگ_2, &success) ||
      hb_subset_جدول_color (plan, buf, ٽيگ_2, &success) ||
      hb_subset_جدول_other (plan, buf, ٽيگ_2, &success))
    أرجع success;


  بدل (ٽيگ_2)
  {
  حالة HB_ٽيگ('h','e','a','d'):
  {
    منطقي outline_جدول_handles_سر =
	is_جدول_present (plan->ذريعو, HB_ٽيگ('g','l','y','f')) &&
	!should_drop_جدول (plan, HB_ٽيگ('g','l','y','f'));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    outline_جدول_handles_سر |=
	is_جدول_present (plan->ذريعو, HB_ٽيگ('G','L','Y','F')) &&
	!should_drop_جدول (plan, HB_ٽيگ('G','L','Y','F'));
#جيڪڏهن_پورو
    إذا (outline_جدول_handles_سر)
      أرجع سچ; /* skip head, handled by the outline table */
    أرجع hb_subset_جدول<ثابت OT::سر_2> (plan, buf);
  }

  حالة HB_ٽيگ('S','T','A','T'):
    إذا (!plan->user_axes_location.is_empty ()) أرجع hb_subset_جدول<ثابت OT::STAT> (plan, buf);
    وإلا أرجع hb_subset_جدول_passthrough (plan, ٽيگ_2);

  حالة HB_ٽيگ('c','v','t',' '):
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (is_جدول_present (plan->ذريعو, HB_ٽيگ('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      خودڪار &cvar = *plan->ذريعو->جدول.cvar;
      أرجع OT::cvar::جوڙيو_cvt_and_apply_deltas (plan, cvar.get_tuple_var_معلومات (), &cvar);
    }
#جيڪڏهن_پورو
    أرجع hb_subset_جدول_passthrough (plan, ٽيگ_2);
  }

  إذا (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    أرجع hb_subset_جدول_passthrough (plan, ٽيگ_2);

  // Drop table
  أرجع سچ;
}

ساكن فراغ attach_accelerator_معلومات_2 (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  إذا (!plan->inprogress_accelerator) أرجع;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = خالي_اشارو;

  إذا (accel->اندريون_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    أرجع;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_جدول<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  إذا (!hb_face_سيٽ_user_معلومات(face,
                             hb_subset_accelerator_t::user_معلومات_ڪنجي(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             سچ))
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
hb_subset_or_fail (hb_face_t *ذريعو, ثابت hb_subset_داخلا_t *داخلا_2)
{
  إذا (unlikely (!داخلا_2 || !ذريعو)) أرجع خالي_اشارو;

  إذا (unlikely (!ذريعو->get_انگ_glyphs ()))
  {
    DEBUG_نياپو (SUBSET, خالي_اشارو, "No glyphs in source font.");
    أرجع خالي_اشارو;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (ذريعو, داخلا_2);
  إذا (unlikely (!plan)) {
    أرجع خالي_اشارو;
  }

  hb_face_t * نتيجو = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  أرجع نتيجو;
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
  إذا (unlikely (!plan || plan->اندريون_error ())) {
    أرجع خالي_اشارو;
  }

  hb_ٽيگ_t جدول_ٽيگ_فهرست[32];
  بنانشان وٿي = 0, انگ_tables = ARRAY_ڊيگهائي (جدول_ٽيگ_فهرست);

  hb_سيٽ_t subsetted_ٽيگ_فهرست, pending_subset_ٽيگ_فهرست;
  طالما (((فراغ) get_جدول_ٽيگ_فهرست_2 (plan, وٿي, &انگ_tables, جدول_ٽيگ_فهرست), انگ_tables))
  {
    لكل (بنانشان i = 0; i < انگ_tables; ++i)
    {
      hb_ٽيگ_t ٽيگ_2 = جدول_ٽيگ_فهرست[i];
      إذا (should_drop_جدول (plan, ٽيگ_2)) استمر;
      pending_subset_ٽيگ_فهرست.جوڙيو (ٽيگ_2);
    }

    وٿي += انگ_tables;
  }

  منطقي success = سچ;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_ويڪٽر_t<حرف> buf;
    buf.alloc (8192 - 16);

    طالما (!pending_subset_ٽيگ_فهرست.is_empty ())
    {
      إذا (subsetted_ٽيگ_فهرست.اندريون_error ()
	  || pending_subset_ٽيگ_فهرست.اندريون_error ()) {
	success = خطأ;
	اذهب پڇاڙي;
      }

      منطقي made_changes = خطأ;
      لكل (hb_ٽيگ_t ٽيگ_2 : pending_subset_ٽيگ_فهرست)
      {
	إذا (!_dependencies_satisfied (plan, ٽيگ_2,
				      subsetted_ٽيگ_فهرست,
				      pending_subset_ٽيگ_فهرست))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  استمر;
	}

	pending_subset_ٽيگ_فهرست.del (ٽيگ_2);
	subsetted_ٽيگ_فهرست.جوڙيو (ٽيگ_2);
	made_changes = سچ;

	success = subset_جدول (plan, buf, ٽيگ_2);
	إذا (unlikely (!success)) اذهب پڇاڙي;
      }

      إذا (!made_changes)
      {
	DEBUG_نياپو (SUBSET, خالي_اشارو, "Table dependencies unable to be satisfied. Subset failed.");
	success = خطأ;
	اذهب پڇاڙي;
      }
    }
  }

  إذا (success && plan->attach_accelerator_معلومات) {
    attach_accelerator_معلومات_2 (plan, plan->dest);
  }

پڇاڙي:
  أرجع success ? hb_face_reference (plan->dest) : خالي_اشارو;
}
