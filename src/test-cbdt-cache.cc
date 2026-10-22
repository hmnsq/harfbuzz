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

#અન્તર્ભાવ "hb-subset.hh"
#અન્તર્ભાવ "OT/Color/CBDT/CBDT.hh"

સ્થિર unsigned fail_after;
સ્થિર unsigned allocation_ગણતરી;

સ્થિર તાર્કિક
fail_allocation ()
{
  allocation_ગણતરી++;
  પાછા fail_after && !--fail_after;
}

બાહ્ય "C" શૂન્ય *cbdt_test_malloc (size_t size)
{ પાછા fail_allocation () ? nullptr : malloc (size); }
બાહ્ય "C" શૂન્ય *cbdt_test_calloc (size_t ગણતરી, size_t size)
{ પાછા fail_allocation () ? nullptr : calloc (ગણતરી, size); }
બાહ્ય "C" શૂન્ય *cbdt_test_realloc (શૂન્ય *ptr, size_t size)
{ પાછા fail_allocation () ? nullptr : realloc (ptr, size); }
બાહ્ય "C" શૂન્ય cbdt_test_free (શૂન્ય *ptr)
{ મુક્ત (ptr); }

સ્થિર hb_blob_t *
subset_strike (અચલ OT::BitmapSizeTable &strike,
	       અચલ OT::CBLC *cblc,
	       hb_subset_plan_t *plan,
	       hb_bytes_t cbdt,
	       hb_vector_t<અક્ષર> *data,
	       OT::cbdt_dedup_context_t *dedup)
{
  અક્ષર buffer[4096];
  hb_serialize_context_t serializer (buffer, કદ (buffer));
  serializer.start_serialize<OT::BitmapSizeTable> ();
  hb_subset_context_t context (nullptr, plan, &serializer, HB_OT_TAG_CBLC);
  hb_always_assert (strike.subset (&context, cblc, cbdt.arrayZ, cbdt.length,
				  data, dedup));
  serializer.end_serialize ();
  hb_always_assert (!serializer.in_error ());
  પાછા serializer.copy_blob ();
}

સ્થિર unsigned
test_required_allocations (અચલ OT::BitmapSizeTable &strike,
			   અચલ OT::CBLC *cblc,
			   hb_subset_plan_t *plan,
			   hb_bytes_t cbdt,
			   unsigned failure)
{
  અક્ષર buffer[4096];
  hb_serialize_context_t serializer (buffer, કદ (buffer));
  serializer.start_serialize<OT::BitmapSizeTable> ();
  hb_subset_context_t context (nullptr, plan, &serializer, HB_OT_TAG_CBLC);
  hb_vector_t<અક્ષર> data;
  fail_after = failure;
  allocation_ગણતરી = 0;
  તાર્કિક success = strike.subset (&context, cblc, cbdt.arrayZ, cbdt.length,
			       &data, nullptr);
  unsigned ગણતરી = allocation_ગણતરી;
  hb_always_assert (!fail_after);
  /* Required image/offset allocations must fail serialization instead of
   * being mistaken for a strike that can simply be discarded. */
  hb_always_assert (success || serializer.in_error ());
  serializer.end_serialize ();
  પાછા ગણતરી;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  hb_always_assert (argc == 2);
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0);
  hb_blob_destroy (blob);
  hb_subset_આગત_t *આગત = hb_subset_આગત_create_or_fail ();
  hb_always_assert (આગત);
  hb_set_t *unicodes = hb_subset_આગત_unicode_set (આગત);
  hb_set_add (unicodes, 0x38);
  hb_set_add (unicodes, 0xAE);
  hb_set_add (unicodes, 0x2049);
  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (face, આગત);
  hb_always_assert (plan);

  hb_blob_ptr_t<OT::CBLC> cblc =
      hb_sanitize_context_t ().reference_table<OT::CBLC> (face);
  hb_blob_ptr_t<OT::CBDT> cbdt =
      hb_sanitize_context_t ().reference_table<OT::CBDT> (face);
  hb_always_assert (cblc.get_length () >= 8 + 2 * OT::BitmapSizeTable::static_size);
  અચલ auto *strikes = reinterpret_cast<અચલ OT::BitmapSizeTable *> (
      reinterpret_cast<અચલ અક્ષર *> (cblc.get ()) + 8);

  unsigned required_allocations = test_required_allocations (
      strikes[0], cblc.get (), plan, cbdt.get_blob ()->as_bytes (), 0);
  hb_always_assert (required_allocations);
  માટે (unsigned failure = 1; failure <= required_allocations; failure++)
    test_required_allocations (strikes[0], cblc.get (), plan,
			       cbdt.get_blob ()->as_bytes (), failure);

  hb_vector_t<અક્ષર> expected_data;
  hb_blob_t *expected[2];
  માટે (unsigned i = 0; i < 2; i++)
    expected[i] = subset_strike (strikes[i], cblc.get (), plan, cbdt.get_blob ()->as_bytes (),
				&expected_data, nullptr);

  /* Fail the entries vector, lengths vector, and map allocations in turn.
   * A disabled cache must preserve both strikes and every image byte. */
  માટે (unsigned failure = 1; failure <= 3; failure++)
  {
    OT::cbdt_dedup_context_t dedup;
    hb_vector_t<unsigned> lengths {1};
    fail_after = failure;
    hb_always_assert (!dedup.add (0, 0, lengths));
    hb_always_assert (!fail_after);
    hb_always_assert (dedup.entries.in_error () || dedup.lengths.in_error () ||
		      dedup.map.in_error ());

    hb_vector_t<અક્ષર> actual_data;
    માટે (unsigned i = 0; i < 2; i++)
    {
      hb_blob_t *actual = subset_strike (strikes[i], cblc.get (), plan,
					cbdt.get_blob ()->as_bytes (), &actual_data, &dedup);
      hb_always_assert (hb_blob_get_length (actual) == hb_blob_get_length (expected[i]));
      hb_always_assert (!hb_memcmp (hb_blob_get_data (actual, nullptr),
				   hb_blob_get_data (expected[i], nullptr),
				   hb_blob_get_length (actual)));
      hb_blob_destroy (actual);
    }
    hb_always_assert (actual_data.as_bytes () == expected_data.as_bytes ());
  }

  માટે (hb_blob_t *expected_blob : expected)
    hb_blob_destroy (expected_blob);
  cbdt.destroy ();
  cblc.destroy ();
  hb_subset_plan_destroy (plan);
  hb_subset_આગત_destroy (આગત);
  hb_face_destroy (face);
  પાછા 0;
}
