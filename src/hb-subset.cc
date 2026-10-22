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


hb_user_انکڑے_کلید_t hb_subset_accelerator_user_انکڑے_کلید = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
ساكن hb_ٹیگ_t معلوم_tables[] {
  HB_ٹیگ('a','v','a','r'),
  HB_ٹیگ('B','A','S','E'),
  HB_ٹیگ('C','B','D','T'),
  HB_ٹیگ('C','B','L','C'),
  HB_ٹیگ('C','F','F',' '),
  HB_ٹیگ('C','F','F','2'),
  HB_ٹیگ('c','m','a','p'),
  HB_ٹیگ('C','O','L','R'),
  HB_ٹیگ('C','P','A','L'),
  HB_ٹیگ('c','v','a','r'),
  HB_ٹیگ('c','v','t',' '),
  HB_ٹیگ('D','S','I','G'),
  HB_ٹیگ('E','B','D','T'),
  HB_ٹیگ('E','B','L','C'),
  HB_ٹیگ('E','B','S','C'),
  HB_ٹیگ('f','p','g','m'),
  HB_ٹیگ('f','v','a','r'),
  HB_ٹیگ('g','a','s','p'),
  HB_ٹیگ('G','D','E','F'),
  HB_ٹیگ('g','l','y','f'),
#جے_بنا_تعریف HB_NO_BEYOND_64K
  HB_ٹیگ('G','L','Y','F'),
#جے_انت
  HB_ٹیگ('G','P','O','S'),
  HB_ٹیگ('G','S','U','B'),
  HB_ٹیگ('g','v','a','r'),
#جے_بنا_تعریف HB_NO_BEYOND_64K
  HB_ٹیگ('G','V','A','R'),
#جے_انت
  HB_ٹیگ('h','d','m','x'),
  HB_ٹیگ('h','e','a','d'),
  HB_ٹیگ('h','h','e','a'),
#جے_بنا_تعریف HB_NO_BEYOND_64K
  HB_ٹیگ('H','H','E','A'),
#جے_انت
  HB_ٹیگ('h','m','t','x'),
#جے_بنا_تعریف HB_NO_BEYOND_64K
  HB_ٹیگ('H','M','T','X'),
#جے_انت
  HB_ٹیگ('H','V','A','R'),
  HB_ٹیگ('J','S','T','F'),
  HB_ٹیگ('k','e','r','n'),
  HB_ٹیگ('l','o','c','a'),
#جے_بنا_تعریف HB_NO_BEYOND_64K
  HB_ٹیگ('L','O','C','A'),
#جے_انت
  HB_ٹیگ('L','T','S','H'),
  HB_ٹیگ('M','A','T','H'),
  HB_ٹیگ('m','a','x','p'),
#جے_بنا_تعریف HB_NO_BEYOND_64K
  HB_ٹیگ('M','A','X','P'),
#جے_انت
  HB_ٹیگ('M','E','R','G'),
  HB_ٹیگ('m','e','t','a'),
  HB_ٹیگ('M','V','A','R'),
  HB_ٹیگ('P','C','L','T'),
  HB_ٹیگ('p','o','s','t'),
  HB_ٹیگ('p','r','e','p'),
  HB_ٹیگ('s','b','i','x'),
  HB_ٹیگ('S','T','A','T'),
  HB_ٹیگ('S','V','G',' '),
  HB_ٹیگ('V','D','M','X'),
  HB_ٹیگ('v','h','e','a'),
#جے_بنا_تعریف HB_NO_BEYOND_64K
  HB_ٹیگ('V','H','E','A'),
#جے_انت
  HB_ٹیگ('v','m','t','x'),
#جے_بنا_تعریف HB_NO_BEYOND_64K
  HB_ٹیگ('V','M','T','X'),
#جے_انت
  HB_ٹیگ('V','O','R','G'),
  HB_ٹیگ('V','A','R','C'),
  HB_ٹیگ('V','V','A','R'),
  HB_ٹیگ('n','a','m','e'),
  HB_ٹیگ('O','S','/','2')
};

ساكن منطقي جدول_is_خالی_تھاواں (ثابت hb_face_t *face, hb_ٹیگ_t ٹیگ_2)
{
  hb_blob_t* blob = hb_face_reference_جدول (face, ٹیگ_2);
  منطقي نتیجہ = (blob == hb_blob_get_خالی_تھاواں ());
  hb_blob_destroy (blob);
  أرجع نتیجہ;
}

ساكن بنانشانی صحيح
get_جدول_ٹیگ_سوچی_2 (ثابت hb_subset_plan_t* plan,
                 بنانشانی صحيح  شروع_وتھ,
                 بنانشانی صحيح *جدول_گنتی_2, /* IN/OUT */
                 hb_ٹیگ_t     *جدول_ٹیگ_سوچی /*باہرلا*/)
{
  بنانشانی عدد_tables = hb_face_get_جدول_ٹیگ_سوچی (plan->سروت, 0, خالی_سوچک, خالی_سوچک);
  إذا (عدد_tables)
    أرجع hb_face_get_جدول_ٹیگ_سوچی (plan->سروت, شروع_وتھ, جدول_گنتی_2, جدول_ٹیگ_سوچی);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  خودکار it =
      hb_concat (
          + hb_array (معلوم_tables)
          | hb_filter ([&] (hb_ٹیگ_t ٹیگ_2) {
            أرجع !جدول_is_خالی_تھاواں (plan->سروت, ٹیگ_2) && !plan->no_subset_tables.has (ٹیگ_2);
          })
          | hb_map ([] (hb_ٹیگ_t ٹیگ_2) -> hb_ٹیگ_t { أرجع ٹیگ_2; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_ٹیگ_t ٹیگ_2) {
            أرجع !جدول_is_خالی_تھاواں (plan->سروت, ٹیگ_2);
          }));

  it += شروع_وتھ;

  بنانشانی عدد_written = 0;
  طالما (منطقي (it) && عدد_written < *جدول_گنتی_2)
    جدول_ٹیگ_سوچی[عدد_written++] = *it++;

  *جدول_گنتی_2 = عدد_written;
  أرجع عدد_written;
}


ساكن منطقي
is_جدول_present (hb_face_t *سروت, hb_ٹیگ_t ٹیگ_2)
{

  إذا (!hb_face_get_جدول_ٹیگ_سوچی (سروت, 0, خالی_سوچک, خالی_سوچک)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    أرجع !جدول_is_خالی_تھاواں (سروت, ٹیگ_2);
  }

  hb_ٹیگ_t جدول_ٹیگ_سوچی[32];
  بنانشانی وتھ = 0, عدد_tables = ARRAY_لمائی (جدول_ٹیگ_سوچی);
  طالما (((فراغ) hb_face_get_جدول_ٹیگ_سوچی (سروت, وتھ, &عدد_tables, جدول_ٹیگ_سوچی), عدد_tables))
  {
    لكل (بنانشانی i = 0; i < عدد_tables; ++i)
      إذا (جدول_ٹیگ_سوچی[i] == ٹیگ_2)
	أرجع سچ;
    وتھ += عدد_tables;
  }
  أرجع خطأ;
}

ساكن منطقي
should_drop_جدول (hb_subset_plan_t *plan, hb_ٹیگ_t ٹیگ_2)
{
  إذا (plan->drop_tables.has (ٹیگ_2))
    أرجع سچ;

  بدل (ٹیگ_2)
  {
  حالة HB_ٹیگ('c','v','a','r'): /* hint table, fallthrough */
    أرجع plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  حالة HB_ٹیگ('c','v','t',' '): /* hint table, fallthrough */
  حالة HB_ٹیگ('f','p','g','m'): /* hint table, fallthrough */
  حالة HB_ٹیگ('p','r','e','p'): /* hint table, fallthrough */
  حالة HB_ٹیگ('h','d','m','x'): /* hint table, fallthrough */
  حالة HB_ٹیگ('V','D','M','X'): /* hint table, fallthrough */
    أرجع plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#جے_تعریف_شدہ HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  حالة HB_ٹیگ('G','D','E','F'):
  حالة HB_ٹیگ('G','P','O','S'):
  حالة HB_ٹیگ('G','S','U','B'):
  حالة HB_ٹیگ('m','o','r','x'):
  حالة HB_ٹیگ('m','o','r','t'):
  حالة HB_ٹیگ('k','e','r','x'):
  حالة HB_ٹیگ('k','e','r','n'):
    أرجع سچ;
#جے_انت

  حالة HB_ٹیگ('a','v','a','r'):
  حالة HB_ٹیگ('f','v','a','r'):
  حالة HB_ٹیگ('g','v','a','r'):
#جے_بنا_تعریف HB_NO_BEYOND_64K
  حالة HB_ٹیگ('G','V','A','R'):
#جے_انت
  حالة HB_ٹیگ('H','V','A','R'):
  حالة HB_ٹیگ('V','V','A','R'):
  حالة HB_ٹیگ('M','V','A','R'):
    أرجع plan->all_axes_pinned;

  افتراضي:
    أرجع خطأ;
  }
}

ساكن منطقي
_dependencies_satisfied (hb_subset_plan_t *plan, hb_ٹیگ_t ٹیگ_2,
                         ثابت hb_سیٹ_t &subsetted_ٹیگ_سوچی,
                         ثابت hb_سیٹ_t &pending_subset_ٹیگ_سوچی)
{
  منطقي outline_pending = pending_subset_ٹیگ_سوچی.has (HB_ٹیگ('g','l','y','f'));
#جے_بنا_تعریف HB_NO_BEYOND_64K
  outline_pending |= pending_subset_ٹیگ_سوچی.has (HB_ٹیگ('G','L','Y','F'));
#جے_انت

  بدل (ٹیگ_2)
  {
  حالة HB_ٹیگ('h','m','t','x'):
  حالة HB_ٹیگ('v','m','t','x'):
  حالة HB_ٹیگ('m','a','x','p'):
#جے_بنا_تعریف HB_NO_BEYOND_64K
  حالة HB_ٹیگ('H','M','T','X'):
  حالة HB_ٹیگ('V','M','T','X'):
  حالة HB_ٹیگ('M','A','X','P'):
#جے_انت
  حالة HB_ٹیگ('O','S','/','2'):
    أرجع !plan->normalized_coords || !outline_pending;
  حالة HB_ٹیگ('G','P','O','S'):
  حالة HB_ٹیگ('G','S','U','B'):
    أرجع plan->all_axes_pinned || !pending_subset_ٹیگ_سوچی.has (HB_ٹیگ('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  حالة HB_ٹیگ('g','v','a','r'):
#جے_بنا_تعریف HB_NO_BEYOND_64K
  حالة HB_ٹیگ('G','V','A','R'):
#جے_انت
  حالة HB_ٹیگ('c','v','a','r'):
  حالة HB_ٹیگ('H','V','A','R'):
  حالة HB_ٹیگ('V','V','A','R'):
  حالة HB_ٹیگ('M','V','A','R'):
  حالة HB_ٹیگ('G','D','E','F'):
  حالة HB_ٹیگ('C','F','F','2'):
    أرجع !plan->has_avar2 || !pending_subset_ٹیگ_سوچی.has (HB_ٹیگ('a','v','a','r'));
  افتراضي:
    أرجع سچ;
  }
}

ساكن منطقي
subset_جدول (hb_subset_plan_t *plan,
	       hb_سمتیہ_t<حرف> &buf,
	       hb_ٹیگ_t ٹیگ_2)
{
  إذا (plan->no_subset_tables.has (ٹیگ_2)) {
    أرجع hb_subset_جدول_passthrough (plan, ٹیگ_2);
  }

  DEBUG_پیغام (SUBSET, خالی_سوچک, "subset %c%c%c%c", HB_UNTAG (ٹیگ_2));

  منطقي success;
  إذا (hb_subset_جدول_layout (plan, buf, ٹیگ_2, &success) ||
      hb_subset_جدول_var (plan, buf, ٹیگ_2, &success) ||
      hb_subset_جدول_cff (plan, buf, ٹیگ_2, &success) ||
      hb_subset_جدول_color (plan, buf, ٹیگ_2, &success) ||
      hb_subset_جدول_other (plan, buf, ٹیگ_2, &success))
    أرجع success;


  بدل (ٹیگ_2)
  {
  حالة HB_ٹیگ('h','e','a','d'):
  {
    منطقي outline_جدول_handles_سر =
	is_جدول_present (plan->سروت, HB_ٹیگ('g','l','y','f')) &&
	!should_drop_جدول (plan, HB_ٹیگ('g','l','y','f'));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    outline_جدول_handles_سر |=
	is_جدول_present (plan->سروت, HB_ٹیگ('G','L','Y','F')) &&
	!should_drop_جدول (plan, HB_ٹیگ('G','L','Y','F'));
#جے_انت
    إذا (outline_جدول_handles_سر)
      أرجع سچ; /* skip head, handled by the outline table */
    أرجع hb_subset_جدول<ثابت OT::سر_2> (plan, buf);
  }

  حالة HB_ٹیگ('S','T','A','T'):
    إذا (!plan->user_axes_location.is_empty ()) أرجع hb_subset_جدول<ثابت OT::STAT> (plan, buf);
    وإلا أرجع hb_subset_جدول_passthrough (plan, ٹیگ_2);

  حالة HB_ٹیگ('c','v','t',' '):
#جے_بنا_تعریف HB_NO_VAR
    إذا (is_جدول_present (plan->سروت, HB_ٹیگ('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      خودکار &cvar = *plan->سروت->جدول.cvar;
      أرجع OT::cvar::جوڑو_cvt_and_apply_deltas (plan, cvar.get_tuple_var_انکڑے (), &cvar);
    }
#جے_انت
    أرجع hb_subset_جدول_passthrough (plan, ٹیگ_2);
  }

  إذا (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    أرجع hb_subset_جدول_passthrough (plan, ٹیگ_2);

  // Drop table
  أرجع سچ;
}

ساكن فراغ attach_accelerator_انکڑے_2 (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  إذا (!plan->inprogress_accelerator) أرجع;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = خالی_سوچک;

  إذا (accel->وچ_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    أرجع;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_جدول<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  إذا (!hb_face_سیٹ_user_انکڑے(face,
                             hb_subset_accelerator_t::user_انکڑے_کلید(),
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
hb_subset_or_fail (hb_face_t *سروت, ثابت hb_subset_داخلہ_t *داخلہ)
{
  إذا (unlikely (!داخلہ || !سروت)) أرجع خالی_سوچک;

  إذا (unlikely (!سروت->get_عدد_glyphs ()))
  {
    DEBUG_پیغام (SUBSET, خالی_سوچک, "No glyphs in source font.");
    أرجع خالی_سوچک;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (سروت, داخلہ);
  إذا (unlikely (!plan)) {
    أرجع خالی_سوچک;
  }

  hb_face_t * نتیجہ = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  أرجع نتیجہ;
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
  إذا (unlikely (!plan || plan->وچ_error ())) {
    أرجع خالی_سوچک;
  }

  hb_ٹیگ_t جدول_ٹیگ_سوچی[32];
  بنانشانی وتھ = 0, عدد_tables = ARRAY_لمائی (جدول_ٹیگ_سوچی);

  hb_سیٹ_t subsetted_ٹیگ_سوچی, pending_subset_ٹیگ_سوچی;
  طالما (((فراغ) get_جدول_ٹیگ_سوچی_2 (plan, وتھ, &عدد_tables, جدول_ٹیگ_سوچی), عدد_tables))
  {
    لكل (بنانشانی i = 0; i < عدد_tables; ++i)
    {
      hb_ٹیگ_t ٹیگ_2 = جدول_ٹیگ_سوچی[i];
      إذا (should_drop_جدول (plan, ٹیگ_2)) استمر;
      pending_subset_ٹیگ_سوچی.جوڑو (ٹیگ_2);
    }

    وتھ += عدد_tables;
  }

  منطقي success = سچ;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_سمتیہ_t<حرف> buf;
    buf.alloc (8192 - 16);

    طالما (!pending_subset_ٹیگ_سوچی.is_empty ())
    {
      إذا (subsetted_ٹیگ_سوچی.وچ_error ()
	  || pending_subset_ٹیگ_سوچی.وچ_error ()) {
	success = خطأ;
	اذهب سرا;
      }

      منطقي made_changes = خطأ;
      لكل (hb_ٹیگ_t ٹیگ_2 : pending_subset_ٹیگ_سوچی)
      {
	إذا (!_dependencies_satisfied (plan, ٹیگ_2,
				      subsetted_ٹیگ_سوچی,
				      pending_subset_ٹیگ_سوچی))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  استمر;
	}

	pending_subset_ٹیگ_سوچی.del (ٹیگ_2);
	subsetted_ٹیگ_سوچی.جوڑو (ٹیگ_2);
	made_changes = سچ;

	success = subset_جدول (plan, buf, ٹیگ_2);
	إذا (unlikely (!success)) اذهب سرا;
      }

      إذا (!made_changes)
      {
	DEBUG_پیغام (SUBSET, خالی_سوچک, "Table dependencies unable to be satisfied. Subset failed.");
	success = خطأ;
	اذهب سرا;
      }
    }
  }

  إذا (success && plan->attach_accelerator_انکڑے) {
    attach_accelerator_انکڑے_2 (plan, plan->dest);
  }

سرا:
  أرجع success ? hb_face_reference (plan->dest) : خالی_سوچک;
}
