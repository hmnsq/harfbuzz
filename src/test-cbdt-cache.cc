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

#وصف_ڪريو hb_malloc cbdt_test_malloc
#وصف_ڪريو hb_calloc cbdt_test_calloc
#وصف_ڪريو hb_realloc cbdt_test_realloc
#وصف_ڪريو hb_free cbdt_test_free

#ضمن "hb-subset.hh"
#ضمن "OT/Color/CBDT/CBDT.hh"

ساكن بنانشان fail_after;
ساكن بنانشان ورهاست_ڳڻپ;

ساكن منطقي
fail_ورهاست ()
{
  ورهاست_ڳڻپ++;
  أرجع fail_after && !--fail_after;
}

خارجي "C" فراغ *cbdt_test_malloc (size_t سائيز)
{ أرجع fail_ورهاست () ? خالي_اشارو : يادگيري_ڏيو (سائيز); }
خارجي "C" فراغ *cbdt_test_calloc (size_t ڳڻپ, size_t سائيز)
{ أرجع fail_ورهاست () ? خالي_اشارو : صفر_يادگيري_ڏيو (ڳڻپ, سائيز); }
خارجي "C" فراغ *cbdt_test_realloc (فراغ *ptr, size_t سائيز)
{ أرجع fail_ورهاست () ? خالي_اشارو : realloc (ptr, سائيز); }
خارجي "C" فراغ cbdt_test_free (فراغ *ptr)
{ حرر (ptr); }

ساكن hb_blob_t *
subset_strike (ثابت OT::Bitmap_سائيز_جدول &strike,
	       ثابت OT::CBLC *cblc,
	       hb_subset_plan_t *plan,
	       hb_بائيٽون_t cbdt,
	       hb_ويڪٽر_t<حرف> *معلومات,
	       OT::cbdt_dedup_context_t *dedup)
{
  حرف بفر[4096];
  hb_serialize_context_t serializer (بفر, حجم (بفر));
  serializer.شروعات_serialize<OT::Bitmap_سائيز_جدول> ();
  hb_subset_context_t context (خالي_اشارو, plan, &serializer, HB_OT_ٽيگ_CBLC);
  hb_always_assert (strike.subset (&context, cblc, cbdt.arrayZ, cbdt.ڊيگهائي,
				  معلومات, dedup));
  serializer.پڇاڙي_serialize ();
  hb_always_assert (!serializer.اندريون_error ());
  أرجع serializer.copy_blob ();
}

ساكن بنانشان
test_required_allocations (ثابت OT::Bitmap_سائيز_جدول &strike,
			   ثابت OT::CBLC *cblc,
			   hb_subset_plan_t *plan,
			   hb_بائيٽون_t cbdt,
			   بنانشان failure)
{
  حرف بفر[4096];
  hb_serialize_context_t serializer (بفر, حجم (بفر));
  serializer.شروعات_serialize<OT::Bitmap_سائيز_جدول> ();
  hb_subset_context_t context (خالي_اشارو, plan, &serializer, HB_OT_ٽيگ_CBLC);
  hb_ويڪٽر_t<حرف> معلومات;
  fail_after = failure;
  ورهاست_ڳڻپ = 0;
  منطقي success = strike.subset (&context, cblc, cbdt.arrayZ, cbdt.ڊيگهائي,
			       &معلومات, خالي_اشارو);
  بنانشان ڳڻپ = ورهاست_ڳڻپ;
  hb_always_assert (!fail_after);
  /* Required image/offset allocations must fail serialization instead of
   * being mistaken for a strike that can simply be discarded. */
  hb_always_assert (success || serializer.اندريون_error ());
  serializer.پڇاڙي_serialize ();
  أرجع ڳڻپ;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  hb_always_assert (argc == 2);
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0);
  hb_blob_destroy (blob);
  hb_subset_داخلا_t *داخلا_2 = hb_subset_داخلا_create_or_fail ();
  hb_always_assert (داخلا_2);
  hb_سيٽ_t *unicodes = hb_subset_داخلا_unicode_سيٽ (داخلا_2);
  hb_سيٽ_جوڙيو (unicodes, 0x38);
  hb_سيٽ_جوڙيو (unicodes, 0xAE);
  hb_سيٽ_جوڙيو (unicodes, 0x2049);
  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (face, داخلا_2);
  hb_always_assert (plan);

  hb_blob_ptr_t<OT::CBLC> cblc =
      hb_sanitize_context_t ().reference_جدول<OT::CBLC> (face);
  hb_blob_ptr_t<OT::CBDT> cbdt =
      hb_sanitize_context_t ().reference_جدول<OT::CBDT> (face);
  hb_always_assert (cblc.get_ڊيگهائي () >= 8 + 2 * OT::Bitmap_سائيز_جدول::static_سائيز);
  ثابت خودڪار *strikes = reinterpret_cast<ثابت OT::Bitmap_سائيز_جدول *> (
      reinterpret_cast<ثابت حرف *> (cblc.get ()) + 8);

  بنانشان required_allocations = test_required_allocations (
      strikes[0], cblc.get (), plan, cbdt.get_blob ()->as_بائيٽون (), 0);
  hb_always_assert (required_allocations);
  لكل (بنانشان failure = 1; failure <= required_allocations; failure++)
    test_required_allocations (strikes[0], cblc.get (), plan,
			       cbdt.get_blob ()->as_بائيٽون (), failure);

  hb_ويڪٽر_t<حرف> expected_معلومات;
  hb_blob_t *expected[2];
  لكل (بنانشان i = 0; i < 2; i++)
    expected[i] = subset_strike (strikes[i], cblc.get (), plan, cbdt.get_blob ()->as_بائيٽون (),
				&expected_معلومات, خالي_اشارو);

  /* Fail the entries vector, lengths vector, and map allocations in turn.
   * A disabled cache must preserve both strikes and every image byte. */
  لكل (بنانشان failure = 1; failure <= 3; failure++)
  {
    OT::cbdt_dedup_context_t dedup;
    hb_ويڪٽر_t<بنانشان> lengths {1};
    fail_after = failure;
    hb_always_assert (!dedup.جوڙيو (0, 0, lengths));
    hb_always_assert (!fail_after);
    hb_always_assert (dedup.entries.اندريون_error () || dedup.lengths.اندريون_error () ||
		      dedup.map.اندريون_error ());

    hb_ويڪٽر_t<حرف> actual_معلومات;
    لكل (بنانشان i = 0; i < 2; i++)
    {
      hb_blob_t *actual = subset_strike (strikes[i], cblc.get (), plan,
					cbdt.get_blob ()->as_بائيٽون (), &actual_معلومات, &dedup);
      hb_always_assert (hb_blob_get_ڊيگهائي (actual) == hb_blob_get_ڊيگهائي (expected[i]));
      hb_always_assert (!hb_memcmp (hb_blob_get_معلومات (actual, خالي_اشارو),
				   hb_blob_get_معلومات (expected[i], خالي_اشارو),
				   hb_blob_get_ڊيگهائي (actual)));
      hb_blob_destroy (actual);
    }
    hb_always_assert (actual_معلومات.as_بائيٽون () == expected_معلومات.as_بائيٽون ());
  }

  لكل (hb_blob_t *expected_blob : expected)
    hb_blob_destroy (expected_blob);
  cbdt.destroy ();
  cblc.destroy ();
  hb_subset_plan_destroy (plan);
  hb_subset_داخلا_destroy (داخلا_2);
  hb_face_destroy (face);
  أرجع 0;
}
