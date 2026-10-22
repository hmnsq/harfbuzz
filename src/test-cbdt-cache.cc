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

#تعریف_کرو hb_malloc cbdt_test_malloc
#تعریف_کرو hb_calloc cbdt_test_calloc
#تعریف_کرو hb_realloc cbdt_test_realloc
#تعریف_کرو hb_free cbdt_test_free

#ضمن "hb-subset.hh"
#ضمن "OT/Color/CBDT/CBDT.hh"

ساكن بنانشانی fail_after;
ساكن بنانشانی ونڈ_گنتی;

ساكن منطقي
fail_ونڈ ()
{
  ونڈ_گنتی++;
  أرجع fail_after && !--fail_after;
}

خارجي "C" فراغ *cbdt_test_malloc (size_t ناپ)
{ أرجع fail_ونڈ () ? خالی_سوچک : حافظہ_دیو (ناپ); }
خارجي "C" فراغ *cbdt_test_calloc (size_t گنتی, size_t ناپ)
{ أرجع fail_ونڈ () ? خالی_سوچک : صفر_حافظہ_دیو (گنتی, ناپ); }
خارجي "C" فراغ *cbdt_test_realloc (فراغ *ptr, size_t ناپ)
{ أرجع fail_ونڈ () ? خالی_سوچک : realloc (ptr, ناپ); }
خارجي "C" فراغ cbdt_test_free (فراغ *ptr)
{ حرر (ptr); }

ساكن hb_blob_t *
subset_strike (ثابت OT::Bitmap_ناپ_جدول &strike,
	       ثابت OT::CBLC *cblc,
	       hb_subset_plan_t *plan,
	       hb_بائٹاں_t cbdt,
	       hb_سمتیہ_t<حرف> *انکڑے,
	       OT::cbdt_dedup_context_t *dedup)
{
  حرف بفر[4096];
  hb_serialize_context_t serializer (بفر, حجم (بفر));
  serializer.شروع_serialize<OT::Bitmap_ناپ_جدول> ();
  hb_subset_context_t context (خالی_سوچک, plan, &serializer, HB_OT_ٹیگ_CBLC);
  hb_always_assert (strike.subset (&context, cblc, cbdt.arrayZ, cbdt.لمائی,
				  انکڑے, dedup));
  serializer.سرا_serialize ();
  hb_always_assert (!serializer.وچ_error ());
  أرجع serializer.copy_blob ();
}

ساكن بنانشانی
test_required_allocations (ثابت OT::Bitmap_ناپ_جدول &strike,
			   ثابت OT::CBLC *cblc,
			   hb_subset_plan_t *plan,
			   hb_بائٹاں_t cbdt,
			   بنانشانی failure)
{
  حرف بفر[4096];
  hb_serialize_context_t serializer (بفر, حجم (بفر));
  serializer.شروع_serialize<OT::Bitmap_ناپ_جدول> ();
  hb_subset_context_t context (خالی_سوچک, plan, &serializer, HB_OT_ٹیگ_CBLC);
  hb_سمتیہ_t<حرف> انکڑے;
  fail_after = failure;
  ونڈ_گنتی = 0;
  منطقي success = strike.subset (&context, cblc, cbdt.arrayZ, cbdt.لمائی,
			       &انکڑے, خالی_سوچک);
  بنانشانی گنتی = ونڈ_گنتی;
  hb_always_assert (!fail_after);
  /* Required image/offset allocations must fail serialization instead of
   * being mistaken for a strike that can simply be discarded. */
  hb_always_assert (success || serializer.وچ_error ());
  serializer.سرا_serialize ();
  أرجع گنتی;
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  hb_always_assert (argc == 2);
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0);
  hb_blob_destroy (blob);
  hb_subset_داخلہ_t *داخلہ = hb_subset_داخلہ_create_or_fail ();
  hb_always_assert (داخلہ);
  hb_سیٹ_t *unicodes = hb_subset_داخلہ_unicode_سیٹ (داخلہ);
  hb_سیٹ_جوڑو (unicodes, 0x38);
  hb_سیٹ_جوڑو (unicodes, 0xAE);
  hb_سیٹ_جوڑو (unicodes, 0x2049);
  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (face, داخلہ);
  hb_always_assert (plan);

  hb_blob_ptr_t<OT::CBLC> cblc =
      hb_sanitize_context_t ().reference_جدول<OT::CBLC> (face);
  hb_blob_ptr_t<OT::CBDT> cbdt =
      hb_sanitize_context_t ().reference_جدول<OT::CBDT> (face);
  hb_always_assert (cblc.get_لمائی () >= 8 + 2 * OT::Bitmap_ناپ_جدول::static_ناپ);
  ثابت خودکار *strikes = reinterpret_cast<ثابت OT::Bitmap_ناپ_جدول *> (
      reinterpret_cast<ثابت حرف *> (cblc.get ()) + 8);

  بنانشانی required_allocations = test_required_allocations (
      strikes[0], cblc.get (), plan, cbdt.get_blob ()->as_بائٹاں (), 0);
  hb_always_assert (required_allocations);
  لكل (بنانشانی failure = 1; failure <= required_allocations; failure++)
    test_required_allocations (strikes[0], cblc.get (), plan,
			       cbdt.get_blob ()->as_بائٹاں (), failure);

  hb_سمتیہ_t<حرف> expected_انکڑے;
  hb_blob_t *expected[2];
  لكل (بنانشانی i = 0; i < 2; i++)
    expected[i] = subset_strike (strikes[i], cblc.get (), plan, cbdt.get_blob ()->as_بائٹاں (),
				&expected_انکڑے, خالی_سوچک);

  /* Fail the entries vector, lengths vector, and map allocations in turn.
   * A disabled cache must preserve both strikes and every image byte. */
  لكل (بنانشانی failure = 1; failure <= 3; failure++)
  {
    OT::cbdt_dedup_context_t dedup;
    hb_سمتیہ_t<بنانشانی> lengths {1};
    fail_after = failure;
    hb_always_assert (!dedup.جوڑو (0, 0, lengths));
    hb_always_assert (!fail_after);
    hb_always_assert (dedup.entries.وچ_error () || dedup.lengths.وچ_error () ||
		      dedup.map.وچ_error ());

    hb_سمتیہ_t<حرف> actual_انکڑے;
    لكل (بنانشانی i = 0; i < 2; i++)
    {
      hb_blob_t *actual = subset_strike (strikes[i], cblc.get (), plan,
					cbdt.get_blob ()->as_بائٹاں (), &actual_انکڑے, &dedup);
      hb_always_assert (hb_blob_get_لمائی (actual) == hb_blob_get_لمائی (expected[i]));
      hb_always_assert (!hb_memcmp (hb_blob_get_انکڑے (actual, خالی_سوچک),
				   hb_blob_get_انکڑے (expected[i], خالی_سوچک),
				   hb_blob_get_لمائی (actual)));
      hb_blob_destroy (actual);
    }
    hb_always_assert (actual_انکڑے.as_بائٹاں () == expected_انکڑے.as_بائٹاں ());
  }

  لكل (hb_blob_t *expected_blob : expected)
    hb_blob_destroy (expected_blob);
  cbdt.destroy ();
  cblc.destroy ();
  hb_subset_plan_destroy (plan);
  hb_subset_داخلہ_destroy (داخلہ);
  hb_face_destroy (face);
  أرجع 0;
}
