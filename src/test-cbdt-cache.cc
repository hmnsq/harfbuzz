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

#నిర్వచించు hb_malloc cbdt_test_malloc
#నిర్వచించు hb_calloc cbdt_test_calloc
#నిర్వచించు hb_realloc cbdt_test_realloc
#నిర్వచించు hb_free cbdt_test_free

#అన్తర్భావ "hb-subset.hh"
#అన్తర్భావ "OT/Color/CBDT/CBDT.hh"

స్థిర గుర్తులేని fail_after;
స్థిర గుర్తులేని కేటాయింపు_లెక్క;

స్థిర తార్కిక
fail_కేటాయింపు ()
{
  కేటాయింపు_లెక్క++;
  తిరిగి fail_after && !--fail_after;
}

బాహ్య "C" శూన్య *cbdt_test_malloc (size_t కొలత)
{ తిరిగి fail_కేటాయింపు () ? శూన్యసూచిక : జ్ఞాపకం_ఇవ్వు (కొలత); }
బాహ్య "C" శూన్య *cbdt_test_calloc (size_t లెక్క, size_t కొలత)
{ తిరిగి fail_కేటాయింపు () ? శూన్యసూచిక : సున్నా_జ్ఞాపకం_ఇవ్వు (లెక్క, కొలత); }
బాహ్య "C" శూన్య *cbdt_test_realloc (శూన్య *ptr, size_t కొలత)
{ తిరిగి fail_కేటాయింపు () ? శూన్యసూచిక : realloc (ptr, కొలత); }
బాహ్య "C" శూన్య cbdt_test_free (శూన్య *ptr)
{ విడుదల (ptr); }

స్థిర hb_blob_t *
subset_strike (స్థిరరాశి OT::Bitmap_కొలత_పట్టిక &strike,
	       స్థిరరాశి OT::CBLC *cblc,
	       hb_subset_plan_t *plan,
	       hb_బైట్లు_t cbdt,
	       hb_సదిశ_t<అక్షరం> *దత్తాంశం,
	       OT::cbdt_dedup_context_t *dedup)
{
  అక్షరం బఫరు[4096];
  hb_serialize_context_t serializer (బఫరు, పరిమాణం (బఫరు));
  serializer.ప్రారంభం_serialize<OT::Bitmap_కొలత_పట్టిక> ();
  hb_subset_context_t context (శూన్యసూచిక, plan, &serializer, HB_OT_ట్యాగ్_CBLC);
  hb_always_assert (strike.subset (&context, cblc, cbdt.arrayZ, cbdt.నిడివి,
				  దత్తాంశం, dedup));
  serializer.చివర_serialize ();
  hb_always_assert (!serializer.లోపల_error ());
  తిరిగి serializer.copy_blob ();
}

స్థిర గుర్తులేని
test_required_allocations (స్థిరరాశి OT::Bitmap_కొలత_పట్టిక &strike,
			   స్థిరరాశి OT::CBLC *cblc,
			   hb_subset_plan_t *plan,
			   hb_బైట్లు_t cbdt,
			   గుర్తులేని failure)
{
  అక్షరం బఫరు[4096];
  hb_serialize_context_t serializer (బఫరు, పరిమాణం (బఫరు));
  serializer.ప్రారంభం_serialize<OT::Bitmap_కొలత_పట్టిక> ();
  hb_subset_context_t context (శూన్యసూచిక, plan, &serializer, HB_OT_ట్యాగ్_CBLC);
  hb_సదిశ_t<అక్షరం> దత్తాంశం;
  fail_after = failure;
  కేటాయింపు_లెక్క = 0;
  తార్కిక success = strike.subset (&context, cblc, cbdt.arrayZ, cbdt.నిడివి,
			       &దత్తాంశం, శూన్యసూచిక);
  గుర్తులేని లెక్క = కేటాయింపు_లెక్క;
  hb_always_assert (!fail_after);
  /* Required image/offset allocations must fail serialization instead of
   * being mistaken for a strike that can simply be discarded. */
  hb_always_assert (success || serializer.లోపల_error ());
  serializer.చివర_serialize ();
  తిరిగి లెక్క;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  hb_always_assert (argc == 2);
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0);
  hb_blob_destroy (blob);
  hb_subset_ఆదానం_t *ఆదానం = hb_subset_ఆదానం_create_or_fail ();
  hb_always_assert (ఆదానం);
  hb_సమితి_t *unicodes = hb_subset_ఆదానం_unicode_సమితి (ఆదానం);
  hb_సమితి_కూడు (unicodes, 0x38);
  hb_సమితి_కూడు (unicodes, 0xAE);
  hb_సమితి_కూడు (unicodes, 0x2049);
  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (face, ఆదానం);
  hb_always_assert (plan);

  hb_blob_ptr_t<OT::CBLC> cblc =
      hb_sanitize_context_t ().reference_పట్టిక<OT::CBLC> (face);
  hb_blob_ptr_t<OT::CBDT> cbdt =
      hb_sanitize_context_t ().reference_పట్టిక<OT::CBDT> (face);
  hb_always_assert (cblc.get_నిడివి () >= 8 + 2 * OT::Bitmap_కొలత_పట్టిక::static_కొలత);
  స్థిరరాశి స్వయంచాలక *strikes = reinterpret_cast<స్థిరరాశి OT::Bitmap_కొలత_పట్టిక *> (
      reinterpret_cast<స్థిరరాశి అక్షరం *> (cblc.get ()) + 8);

  గుర్తులేని required_allocations = test_required_allocations (
      strikes[0], cblc.get (), plan, cbdt.get_blob ()->as_బైట్లు (), 0);
  hb_always_assert (required_allocations);
  కొరకు (గుర్తులేని failure = 1; failure <= required_allocations; failure++)
    test_required_allocations (strikes[0], cblc.get (), plan,
			       cbdt.get_blob ()->as_బైట్లు (), failure);

  hb_సదిశ_t<అక్షరం> expected_దత్తాంశం;
  hb_blob_t *expected[2];
  కొరకు (గుర్తులేని i = 0; i < 2; i++)
    expected[i] = subset_strike (strikes[i], cblc.get (), plan, cbdt.get_blob ()->as_బైట్లు (),
				&expected_దత్తాంశం, శూన్యసూచిక);

  /* Fail the entries vector, lengths vector, and map allocations in turn.
   * A disabled cache must preserve both strikes and every image byte. */
  కొరకు (గుర్తులేని failure = 1; failure <= 3; failure++)
  {
    OT::cbdt_dedup_context_t dedup;
    hb_సదిశ_t<గుర్తులేని> lengths {1};
    fail_after = failure;
    hb_always_assert (!dedup.కూడు (0, 0, lengths));
    hb_always_assert (!fail_after);
    hb_always_assert (dedup.entries.లోపల_error () || dedup.lengths.లోపల_error () ||
		      dedup.map.లోపల_error ());

    hb_సదిశ_t<అక్షరం> actual_దత్తాంశం;
    కొరకు (గుర్తులేని i = 0; i < 2; i++)
    {
      hb_blob_t *actual = subset_strike (strikes[i], cblc.get (), plan,
					cbdt.get_blob ()->as_బైట్లు (), &actual_దత్తాంశం, &dedup);
      hb_always_assert (hb_blob_get_నిడివి (actual) == hb_blob_get_నిడివి (expected[i]));
      hb_always_assert (!hb_memcmp (hb_blob_get_దత్తాంశం (actual, శూన్యసూచిక),
				   hb_blob_get_దత్తాంశం (expected[i], శూన్యసూచిక),
				   hb_blob_get_నిడివి (actual)));
      hb_blob_destroy (actual);
    }
    hb_always_assert (actual_దత్తాంశం.as_బైట్లు () == expected_దత్తాంశం.as_బైట్లు ());
  }

  కొరకు (hb_blob_t *expected_blob : expected)
    hb_blob_destroy (expected_blob);
  cbdt.destroy ();
  cblc.destroy ();
  hb_subset_plan_destroy (plan);
  hb_subset_ఆదానం_destroy (ఆదానం);
  hb_face_destroy (face);
  తిరిగి 0;
}
