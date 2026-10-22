/*
 * Copyright © 2026  Behdad Esfahbod
 *
 * This is part of HarfBuzz, a text shaping library.
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
 */

#घोषणा hb_malloc cbdt_test_malloc
#घोषणा hb_calloc cbdt_test_calloc
#घोषणा hb_realloc cbdt_test_realloc
#घोषणा hb_free cbdt_test_free

#समावेश "hb-subset.hh"
#समावेश "OT/Color/CBDT/CBDT.hh"

अटल अचिन्हित fail_after;
अटल अचिन्हित allocation_गिनती;

अटल bool
fail_allocation ()
{
  allocation_गिनती++;
  वापस fail_after && !--fail_after;
}

बाह्य "C" व्योम *cbdt_test_malloc (माप_प्रकार आकार)
{ वापस fail_allocation () ? nullptr : दो_स्मृति (आकार); }
बाह्य "C" व्योम *cbdt_test_calloc (माप_प्रकार गिनती, माप_प्रकार आकार)
{ वापस fail_allocation () ? nullptr : सुस्मृति (गिनती, आकार); }
बाह्य "C" व्योम *cbdt_test_realloc (व्योम *ptr, माप_प्रकार आकार)
{ वापस fail_allocation () ? nullptr : पुनः_स्मृति (ptr, आकार); }
बाह्य "C" व्योम cbdt_test_free (व्योम *ptr)
{ मुक्त (ptr); }

अटल hb_blob_t *
subset_strike (स्थिर OT::Bitmap_आकार_Table &strike,
	       स्थिर OT::CBLC *cblc,
	       hb_subset_plan_t *plan,
	       hb_bytes_t cbdt,
	       hb_vector_t<अक्षर> *तथ्य,
	       OT::cbdt_dedup_context_t *dedup)
{
  अक्षर बफर[4096];
  hb_serialize_context_t serializer (बफर, माप (बफर));
  serializer.start_serialize<OT::Bitmap_आकार_Table> ();
  hb_subset_context_t context (nullptr, plan, &serializer, HB_OT_TAG_CBLC);
  hb_always_assert (strike.subset (&context, cblc, cbdt.arrayZ, cbdt.लंबाई,
				  तथ्य, dedup));
  serializer.end_serialize ();
  hb_always_assert (!serializer.in_error ());
  वापस serializer.copy_blob ();
}

अटल अचिन्हित
test_required_allocations (स्थिर OT::Bitmap_आकार_Table &strike,
			   स्थिर OT::CBLC *cblc,
			   hb_subset_plan_t *plan,
			   hb_bytes_t cbdt,
			   अचिन्हित failure)
{
  अक्षर बफर[4096];
  hb_serialize_context_t serializer (बफर, माप (बफर));
  serializer.start_serialize<OT::Bitmap_आकार_Table> ();
  hb_subset_context_t context (nullptr, plan, &serializer, HB_OT_TAG_CBLC);
  hb_vector_t<अक्षर> तथ्य;
  fail_after = failure;
  allocation_गिनती = 0;
  bool success = strike.subset (&context, cblc, cbdt.arrayZ, cbdt.लंबाई,
			       &तथ्य, nullptr);
  अचिन्हित गिनती = allocation_गिनती;
  hb_always_assert (!fail_after);
  /* Required image/offset allocations must fail serialization instead of
   * being mistaken for a strike that can simply be discarded. */
  hb_always_assert (success || serializer.in_error ());
  serializer.end_serialize ();
  वापस गिनती;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  hb_always_assert (argc == 2);
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0);
  hb_blob_destroy (blob);
  hb_subset_निवेश_t *निवेश = hb_subset_निवेश_create_or_fail ();
  hb_always_assert (निवेश);
  hb_set_t *unicodes = hb_subset_निवेश_unicode_set (निवेश);
  hb_set_add (unicodes, 0x38);
  hb_set_add (unicodes, 0xAE);
  hb_set_add (unicodes, 0x2049);
  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (face, निवेश);
  hb_always_assert (plan);

  hb_blob_ptr_t<OT::CBLC> cblc =
      hb_sanitize_context_t ().reference_table<OT::CBLC> (face);
  hb_blob_ptr_t<OT::CBDT> cbdt =
      hb_sanitize_context_t ().reference_table<OT::CBDT> (face);
  hb_always_assert (cblc.get_लंबाई () >= 8 + 2 * OT::Bitmap_आकार_Table::static_आकार);
  स्थिर स्वतः *strikes = reinterpret_cast<स्थिर OT::Bitmap_आकार_Table *> (
      reinterpret_cast<स्थिर अक्षर *> (cblc.लो ()) + 8);

  अचिन्हित required_allocations = test_required_allocations (
      strikes[0], cblc.लो (), plan, cbdt.get_blob ()->as_bytes (), 0);
  hb_always_assert (required_allocations);
  क्रम (अचिन्हित failure = 1; failure <= required_allocations; failure++)
    test_required_allocations (strikes[0], cblc.लो (), plan,
			       cbdt.get_blob ()->as_bytes (), failure);

  hb_vector_t<अक्षर> expected_तथ्य;
  hb_blob_t *expected[2];
  क्रम (अचिन्हित i = 0; i < 2; i++)
    expected[i] = subset_strike (strikes[i], cblc.लो (), plan, cbdt.get_blob ()->as_bytes (),
				&expected_तथ्य, nullptr);

  /* Fail the entries vector, lengths vector, and map allocations in turn.
   * A disabled cache must preserve both strikes and every image byte. */
  क्रम (अचिन्हित failure = 1; failure <= 3; failure++)
  {
    OT::cbdt_dedup_context_t dedup;
    hb_vector_t<अचिन्हित> lengths {1};
    fail_after = failure;
    hb_always_assert (!dedup.add (0, 0, lengths));
    hb_always_assert (!fail_after);
    hb_always_assert (dedup.entries.in_error () || dedup.lengths.in_error () ||
		      dedup.map.in_error ());

    hb_vector_t<अक्षर> actual_तथ्य;
    क्रम (अचिन्हित i = 0; i < 2; i++)
    {
      hb_blob_t *actual = subset_strike (strikes[i], cblc.लो (), plan,
					cbdt.get_blob ()->as_bytes (), &actual_तथ्य, &dedup);
      hb_always_assert (hb_blob_get_लंबाई (actual) == hb_blob_get_लंबाई (expected[i]));
      hb_always_assert (!hb_memcmp (hb_blob_get_तथ्य (actual, nullptr),
				   hb_blob_get_तथ्य (expected[i], nullptr),
				   hb_blob_get_लंबाई (actual)));
      hb_blob_destroy (actual);
    }
    hb_always_assert (actual_तथ्य.as_bytes () == expected_तथ्य.as_bytes ());
  }

  क्रम (hb_blob_t *expected_blob : expected)
    hb_blob_destroy (expected_blob);
  cbdt.destroy ();
  cblc.destroy ();
  hb_subset_plan_destroy (plan);
  hb_subset_निवेश_destroy (निवेश);
  hb_face_destroy (face);
  वापस 0;
}
