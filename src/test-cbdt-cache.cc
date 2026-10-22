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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_malloc cbdt_test_malloc
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_calloc cbdt_test_calloc
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_realloc cbdt_test_realloc
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_free cbdt_test_free

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/CBDT/CBDT.hh"

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ fail_after;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਵੰਡ_ਗਿਣਤੀ;

ਸਥਿਰ ਤਾਰਕਿਕ
fail_ਵੰਡ ()
{
  ਵੰਡ_ਗਿਣਤੀ++;
  ਵਾਪਸ fail_after && !--fail_after;
}

ਬਾਹਰੀ "C" ਖਾਲੀ *cbdt_test_malloc (size_t ਮਾਪ)
{ ਵਾਪਸ fail_ਵੰਡ () ? ਖਾਲੀ_ਸੂਚਕ : ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਮਾਪ); }
ਬਾਹਰੀ "C" ਖਾਲੀ *cbdt_test_calloc (size_t ਗਿਣਤੀ_2, size_t ਮਾਪ)
{ ਵਾਪਸ fail_ਵੰਡ () ? ਖਾਲੀ_ਸੂਚਕ : ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਗਿਣਤੀ_2, ਮਾਪ); }
ਬਾਹਰੀ "C" ਖਾਲੀ *cbdt_test_realloc (ਖਾਲੀ *ptr, size_t ਮਾਪ)
{ ਵਾਪਸ fail_ਵੰਡ () ? ਖਾਲੀ_ਸੂਚਕ : realloc (ptr, ਮਾਪ); }
ਬਾਹਰੀ "C" ਖਾਲੀ cbdt_test_free (ਖਾਲੀ *ptr)
{ ਮੁਕਤ (ptr); }

ਸਥਿਰ hb_blob_t *
subset_strike (ਅਚਲ OT::Bitmap_ਮਾਪ_ਸਾਰਣੀ &strike,
	       ਅਚਲ OT::CBLC *cblc,
	       hb_subset_plan_t *plan,
	       hb_ਬਾਈਟਾਂ_t cbdt,
	       hb_ਸਦਿਸ਼_t<ਅੱਖਰ> *ਅੰਕੜੇ,
	       OT::cbdt_dedup_context_t *dedup)
{
  ਅੱਖਰ ਬਫਰ[4096];
  hb_serialize_context_t serializer (ਬਫਰ, ਆਕਾਰ (ਬਫਰ));
  serializer.ਸ਼ੁਰੂ_serialize<OT::Bitmap_ਮਾਪ_ਸਾਰਣੀ> ();
  hb_subset_context_t context (ਖਾਲੀ_ਸੂਚਕ, plan, &serializer, HB_OT_ਟੈਗ_CBLC);
  hb_always_assert (strike.subset (&context, cblc, cbdt.arrayZ, cbdt.ਲੰਮਾਈ,
				  ਅੰਕੜੇ, dedup));
  serializer.ਸਿਰਾ_serialize ();
  hb_always_assert (!serializer.ਵਿੱਚ_error ());
  ਵਾਪਸ serializer.copy_blob ();
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
test_required_allocations (ਅਚਲ OT::Bitmap_ਮਾਪ_ਸਾਰਣੀ &strike,
			   ਅਚਲ OT::CBLC *cblc,
			   hb_subset_plan_t *plan,
			   hb_ਬਾਈਟਾਂ_t cbdt,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ failure)
{
  ਅੱਖਰ ਬਫਰ[4096];
  hb_serialize_context_t serializer (ਬਫਰ, ਆਕਾਰ (ਬਫਰ));
  serializer.ਸ਼ੁਰੂ_serialize<OT::Bitmap_ਮਾਪ_ਸਾਰਣੀ> ();
  hb_subset_context_t context (ਖਾਲੀ_ਸੂਚਕ, plan, &serializer, HB_OT_ਟੈਗ_CBLC);
  hb_ਸਦਿਸ਼_t<ਅੱਖਰ> ਅੰਕੜੇ;
  fail_after = failure;
  ਵੰਡ_ਗਿਣਤੀ = 0;
  ਤਾਰਕਿਕ success = strike.subset (&context, cblc, cbdt.arrayZ, cbdt.ਲੰਮਾਈ,
			       &ਅੰਕੜੇ, ਖਾਲੀ_ਸੂਚਕ);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਵੰਡ_ਗਿਣਤੀ;
  hb_always_assert (!fail_after);
  /* Required image/offset allocations must fail serialization instead of
   * being mistaken for a strike that can simply be discarded. */
  hb_always_assert (success || serializer.ਵਿੱਚ_error ());
  serializer.ਸਿਰਾ_serialize ();
  ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  hb_always_assert (argc == 2);
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0);
  hb_blob_destroy (blob);
  hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼ = hb_subset_ਨਿਵੇਸ਼_create_or_fail ();
  hb_always_assert (ਨਿਵੇਸ਼);
  hb_ਸੈੱਟ_t *unicodes = hb_subset_ਨਿਵੇਸ਼_unicode_ਸੈੱਟ (ਨਿਵੇਸ਼);
  hb_ਸੈੱਟ_ਜੋੜੋ (unicodes, 0x38);
  hb_ਸੈੱਟ_ਜੋੜੋ (unicodes, 0xAE);
  hb_ਸੈੱਟ_ਜੋੜੋ (unicodes, 0x2049);
  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (face, ਨਿਵੇਸ਼);
  hb_always_assert (plan);

  hb_blob_ptr_t<OT::CBLC> cblc =
      hb_sanitize_context_t ().reference_ਸਾਰਣੀ<OT::CBLC> (face);
  hb_blob_ptr_t<OT::CBDT> cbdt =
      hb_sanitize_context_t ().reference_ਸਾਰਣੀ<OT::CBDT> (face);
  hb_always_assert (cblc.get_ਲੰਮਾਈ () >= 8 + 2 * OT::Bitmap_ਮਾਪ_ਸਾਰਣੀ::static_ਮਾਪ);
  ਅਚਲ ਸਵੈਚਾਲਿਤ *strikes = reinterpret_cast<ਅਚਲ OT::Bitmap_ਮਾਪ_ਸਾਰਣੀ *> (
      reinterpret_cast<ਅਚਲ ਅੱਖਰ *> (cblc.get ()) + 8);

  ਬਿਨਾਨਿਸ਼ਾਨੀ required_allocations = test_required_allocations (
      strikes[0], cblc.get (), plan, cbdt.get_blob ()->as_ਬਾਈਟਾਂ (), 0);
  hb_always_assert (required_allocations);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ failure = 1; failure <= required_allocations; failure++)
    test_required_allocations (strikes[0], cblc.get (), plan,
			       cbdt.get_blob ()->as_ਬਾਈਟਾਂ (), failure);

  hb_ਸਦਿਸ਼_t<ਅੱਖਰ> expected_ਅੰਕੜੇ;
  hb_blob_t *expected[2];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 2; i++)
    expected[i] = subset_strike (strikes[i], cblc.get (), plan, cbdt.get_blob ()->as_ਬਾਈਟਾਂ (),
				&expected_ਅੰਕੜੇ, ਖਾਲੀ_ਸੂਚਕ);

  /* Fail the entries vector, lengths vector, and map allocations in turn.
   * A disabled cache must preserve both strikes and every image byte. */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ failure = 1; failure <= 3; failure++)
  {
    OT::cbdt_dedup_context_t dedup;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> lengths {1};
    fail_after = failure;
    hb_always_assert (!dedup.ਜੋੜੋ (0, 0, lengths));
    hb_always_assert (!fail_after);
    hb_always_assert (dedup.entries.ਵਿੱਚ_error () || dedup.lengths.ਵਿੱਚ_error () ||
		      dedup.map.ਵਿੱਚ_error ());

    hb_ਸਦਿਸ਼_t<ਅੱਖਰ> actual_ਅੰਕੜੇ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 2; i++)
    {
      hb_blob_t *actual = subset_strike (strikes[i], cblc.get (), plan,
					cbdt.get_blob ()->as_ਬਾਈਟਾਂ (), &actual_ਅੰਕੜੇ, &dedup);
      hb_always_assert (hb_blob_get_ਲੰਮਾਈ (actual) == hb_blob_get_ਲੰਮਾਈ (expected[i]));
      hb_always_assert (!hb_memcmp (hb_blob_get_ਅੰਕੜੇ (actual, ਖਾਲੀ_ਸੂਚਕ),
				   hb_blob_get_ਅੰਕੜੇ (expected[i], ਖਾਲੀ_ਸੂਚਕ),
				   hb_blob_get_ਲੰਮਾਈ (actual)));
      hb_blob_destroy (actual);
    }
    hb_always_assert (actual_ਅੰਕੜੇ.as_ਬਾਈਟਾਂ () == expected_ਅੰਕੜੇ.as_ਬਾਈਟਾਂ ());
  }

  ਲਈ (hb_blob_t *expected_blob : expected)
    hb_blob_destroy (expected_blob);
  cbdt.destroy ();
  cblc.destroy ();
  hb_subset_plan_destroy (plan);
  hb_subset_ਨਿਵੇਸ਼_destroy (ਨਿਵੇਸ਼);
  hb_face_destroy (face);
  ਵਾਪਸ 0;
}
