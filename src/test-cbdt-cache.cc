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

#define hb_malloc cbdt_test_malloc
#define hb_calloc cbdt_test_calloc
#define hb_realloc cbdt_test_realloc
#define hb_free cbdt_test_free

#অন্তর্ভাব "hb-subset.hh"
#অন্তর্ভাব "OT/Color/CBDT/CBDT.hh"

স্থির unsigned fail_after;
স্থির unsigned allocation_গুনতি;

স্থির যৌক্তিক
fail_allocation ()
{
  allocation_গুনতি++;
  ফেরত fail_after && !--fail_after;
}

বাহ্যিক "C" শূন্য *cbdt_test_malloc (size_t আকার_2)
{ ফেরত fail_allocation () ? nullptr : malloc (আকার_2); }
বাহ্যিক "C" শূন্য *cbdt_test_calloc (size_t গুনতি, size_t আকার_2)
{ ফেরত fail_allocation () ? nullptr : calloc (গুনতি, আকার_2); }
বাহ্যিক "C" শূন্য *cbdt_test_realloc (শূন্য *ptr, size_t আকার_2)
{ ফেরত fail_allocation () ? nullptr : realloc (ptr, আকার_2); }
বাহ্যিক "C" শূন্য cbdt_test_free (শূন্য *ptr)
{ মুক্ত (ptr); }

স্থির hb_blob_t *
subset_strike (ধ্রুবক OT::Bitmap_আকার_Table &strike,
	       ধ্রুবক OT::CBLC *cblc,
	       hb_subset_plan_t *plan,
	       hb_bytes_t cbdt,
	       hb_vector_t<অক্ষর> *তথ্য,
	       OT::cbdt_dedup_context_t *dedup)
{
  অক্ষর বাফার[4096];
  hb_serialize_context_t serializer (বাফার, আকার (বাফার));
  serializer.start_serialize<OT::Bitmap_আকার_Table> ();
  hb_subset_context_t context (nullptr, plan, &serializer, HB_OT_TAG_CBLC);
  hb_always_assert (strike.subset (&context, cblc, cbdt.arrayZ, cbdt.দৈর্ঘ্য,
				  তথ্য, dedup));
  serializer.end_serialize ();
  hb_always_assert (!serializer.in_error ());
  ফেরত serializer.copy_blob ();
}

স্থির unsigned
test_required_allocations (ধ্রুবক OT::Bitmap_আকার_Table &strike,
			   ধ্রুবক OT::CBLC *cblc,
			   hb_subset_plan_t *plan,
			   hb_bytes_t cbdt,
			   unsigned failure)
{
  অক্ষর বাফার[4096];
  hb_serialize_context_t serializer (বাফার, আকার (বাফার));
  serializer.start_serialize<OT::Bitmap_আকার_Table> ();
  hb_subset_context_t context (nullptr, plan, &serializer, HB_OT_TAG_CBLC);
  hb_vector_t<অক্ষর> তথ্য;
  fail_after = failure;
  allocation_গুনতি = 0;
  যৌক্তিক success = strike.subset (&context, cblc, cbdt.arrayZ, cbdt.দৈর্ঘ্য,
			       &তথ্য, nullptr);
  unsigned গুনতি = allocation_গুনতি;
  hb_always_assert (!fail_after);
  /* Required image/offset allocations must fail serialization instead of
   * being mistaken for a strike that can simply be discarded. */
  hb_always_assert (success || serializer.in_error ());
  serializer.end_serialize ();
  ফেরত গুনতি;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  hb_always_assert (argc == 2);
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0);
  hb_blob_destroy (blob);
  hb_subset_প্রবেশ_t *প্রবেশ = hb_subset_প্রবেশ_create_or_fail ();
  hb_always_assert (প্রবেশ);
  hb_set_t *unicodes = hb_subset_প্রবেশ_unicode_set (প্রবেশ);
  hb_set_add (unicodes, 0x38);
  hb_set_add (unicodes, 0xAE);
  hb_set_add (unicodes, 0x2049);
  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (face, প্রবেশ);
  hb_always_assert (plan);

  hb_blob_ptr_t<OT::CBLC> cblc =
      hb_sanitize_context_t ().reference_table<OT::CBLC> (face);
  hb_blob_ptr_t<OT::CBDT> cbdt =
      hb_sanitize_context_t ().reference_table<OT::CBDT> (face);
  hb_always_assert (cblc.get_দৈর্ঘ্য () >= 8 + 2 * OT::Bitmap_আকার_Table::static_আকার);
  ধ্রুবক auto *strikes = reinterpret_cast<ধ্রুবক OT::Bitmap_আকার_Table *> (
      reinterpret_cast<ধ্রুবক অক্ষর *> (cblc.get ()) + 8);

  unsigned required_allocations = test_required_allocations (
      strikes[0], cblc.get (), plan, cbdt.get_blob ()->as_bytes (), 0);
  hb_always_assert (required_allocations);
  জন্য (unsigned failure = 1; failure <= required_allocations; failure++)
    test_required_allocations (strikes[0], cblc.get (), plan,
			       cbdt.get_blob ()->as_bytes (), failure);

  hb_vector_t<অক্ষর> expected_তথ্য;
  hb_blob_t *expected[2];
  জন্য (unsigned i = 0; i < 2; i++)
    expected[i] = subset_strike (strikes[i], cblc.get (), plan, cbdt.get_blob ()->as_bytes (),
				&expected_তথ্য, nullptr);

  /* Fail the entries vector, lengths vector, and map allocations in turn.
   * A disabled cache must preserve both strikes and every image byte. */
  জন্য (unsigned failure = 1; failure <= 3; failure++)
  {
    OT::cbdt_dedup_context_t dedup;
    hb_vector_t<unsigned> lengths {1};
    fail_after = failure;
    hb_always_assert (!dedup.add (0, 0, lengths));
    hb_always_assert (!fail_after);
    hb_always_assert (dedup.entries.in_error () || dedup.lengths.in_error () ||
		      dedup.map.in_error ());

    hb_vector_t<অক্ষর> actual_তথ্য;
    জন্য (unsigned i = 0; i < 2; i++)
    {
      hb_blob_t *actual = subset_strike (strikes[i], cblc.get (), plan,
					cbdt.get_blob ()->as_bytes (), &actual_তথ্য, &dedup);
      hb_always_assert (hb_blob_get_দৈর্ঘ্য (actual) == hb_blob_get_দৈর্ঘ্য (expected[i]));
      hb_always_assert (!hb_memcmp (hb_blob_get_তথ্য (actual, nullptr),
				   hb_blob_get_তথ্য (expected[i], nullptr),
				   hb_blob_get_দৈর্ঘ্য (actual)));
      hb_blob_destroy (actual);
    }
    hb_always_assert (actual_তথ্য.as_bytes () == expected_তথ্য.as_bytes ());
  }

  জন্য (hb_blob_t *expected_blob : expected)
    hb_blob_destroy (expected_blob);
  cbdt.destroy ();
  cblc.destroy ();
  hb_subset_plan_destroy (plan);
  hb_subset_প্রবেশ_destroy (প্রবেশ);
  hb_face_destroy (face);
  ফেরত 0;
}
