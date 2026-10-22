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

#شامل "hb-subset.hh"
#شامل "OT/Color/CBDT/CBDT.hh"

ساکن غیرعلامتی fail_after;
ساکن غیرعلامتی تخصیص_گنتی;

ساکن منطقی
fail_تخصیص ()
{
  تخصیص_گنتی++;
  واپس fail_after && !--fail_after;
}

خارجی "C" خالی *cbdt_test_malloc (size_t سائز)
{ واپس fail_تخصیص () ? خالی_اشاریہ : حافظہ_دو (سائز); }
خارجی "C" خالی *cbdt_test_calloc (size_t گنتی, size_t سائز)
{ واپس fail_تخصیص () ? خالی_اشاریہ : صفر_حافظہ_دو (گنتی, سائز); }
خارجی "C" خالی *cbdt_test_realloc (خالی *ptr, size_t سائز)
{ واپس fail_تخصیص () ? خالی_اشاریہ : realloc (ptr, سائز); }
خارجی "C" خالی cbdt_test_free (خالی *ptr)
{ آزاد (ptr); }

ساکن hb_blob_t *
subset_strike (ثابت OT::Bitmap_سائز_جدول &strike,
	       ثابت OT::CBLC *cblc,
	       hb_subset_plan_t *plan,
	       hb_بائٹیں_t cbdt,
	       hb_سمتیہ_t<حرف> *ڈیٹا,
	       OT::cbdt_dedup_context_t *dedup)
{
  حرف بفر[4096];
  hb_serialize_context_t serializer (بفر, حجم (بفر));
  serializer.آغاز_serialize<OT::Bitmap_سائز_جدول> ();
  hb_subset_context_t context (خالی_اشاریہ, plan, &serializer, HB_OT_لیبل_CBLC);
  hb_always_assert (strike.subset (&context, cblc, cbdt.arrayZ, cbdt.لمبائی,
				  ڈیٹا, dedup));
  serializer.انتہا_serialize ();
  hb_always_assert (!serializer.اندر_error ());
  واپس serializer.copy_blob ();
}

ساکن غیرعلامتی
test_required_allocations (ثابت OT::Bitmap_سائز_جدول &strike,
			   ثابت OT::CBLC *cblc,
			   hb_subset_plan_t *plan,
			   hb_بائٹیں_t cbdt,
			   غیرعلامتی failure)
{
  حرف بفر[4096];
  hb_serialize_context_t serializer (بفر, حجم (بفر));
  serializer.آغاز_serialize<OT::Bitmap_سائز_جدول> ();
  hb_subset_context_t context (خالی_اشاریہ, plan, &serializer, HB_OT_لیبل_CBLC);
  hb_سمتیہ_t<حرف> ڈیٹا;
  fail_after = failure;
  تخصیص_گنتی = 0;
  منطقی success = strike.subset (&context, cblc, cbdt.arrayZ, cbdt.لمبائی,
			       &ڈیٹا, خالی_اشاریہ);
  غیرعلامتی گنتی = تخصیص_گنتی;
  hb_always_assert (!fail_after);
  /* Required image/offset allocations must fail serialization instead of
   * being mistaken for a strike that can simply be discarded. */
  hb_always_assert (success || serializer.اندر_error ());
  serializer.انتہا_serialize ();
  واپس گنتی;
}

صحیح
مین (صحیح argc, حرف **argv)
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
  hb_always_assert (cblc.get_لمبائی () >= 8 + 2 * OT::Bitmap_سائز_جدول::static_سائز);
  ثابت خودکار *strikes = reinterpret_cast<ثابت OT::Bitmap_سائز_جدول *> (
      reinterpret_cast<ثابت حرف *> (cblc.get ()) + 8);

  غیرعلامتی required_allocations = test_required_allocations (
      strikes[0], cblc.get (), plan, cbdt.get_blob ()->as_بائٹیں (), 0);
  hb_always_assert (required_allocations);
  برائے (غیرعلامتی failure = 1; failure <= required_allocations; failure++)
    test_required_allocations (strikes[0], cblc.get (), plan,
			       cbdt.get_blob ()->as_بائٹیں (), failure);

  hb_سمتیہ_t<حرف> expected_ڈیٹا;
  hb_blob_t *expected[2];
  برائے (غیرعلامتی i = 0; i < 2; i++)
    expected[i] = subset_strike (strikes[i], cblc.get (), plan, cbdt.get_blob ()->as_بائٹیں (),
				&expected_ڈیٹا, خالی_اشاریہ);

  /* Fail the entries vector, lengths vector, and map allocations in turn.
   * A disabled cache must preserve both strikes and every image byte. */
  برائے (غیرعلامتی failure = 1; failure <= 3; failure++)
  {
    OT::cbdt_dedup_context_t dedup;
    hb_سمتیہ_t<غیرعلامتی> lengths {1};
    fail_after = failure;
    hb_always_assert (!dedup.جوڑو (0, 0, lengths));
    hb_always_assert (!fail_after);
    hb_always_assert (dedup.entries.اندر_error () || dedup.lengths.اندر_error () ||
		      dedup.map.اندر_error ());

    hb_سمتیہ_t<حرف> actual_ڈیٹا;
    برائے (غیرعلامتی i = 0; i < 2; i++)
    {
      hb_blob_t *actual = subset_strike (strikes[i], cblc.get (), plan,
					cbdt.get_blob ()->as_بائٹیں (), &actual_ڈیٹا, &dedup);
      hb_always_assert (hb_blob_get_لمبائی (actual) == hb_blob_get_لمبائی (expected[i]));
      hb_always_assert (!hb_memcmp (hb_blob_get_ڈیٹا (actual, خالی_اشاریہ),
				   hb_blob_get_ڈیٹا (expected[i], خالی_اشاریہ),
				   hb_blob_get_لمبائی (actual)));
      hb_blob_destroy (actual);
    }
    hb_always_assert (actual_ڈیٹا.as_بائٹیں () == expected_ڈیٹا.as_بائٹیں ());
  }

  برائے (hb_blob_t *expected_blob : expected)
    hb_blob_destroy (expected_blob);
  cbdt.destroy ();
  cblc.destroy ();
  hb_subset_plan_destroy (plan);
  hb_subset_داخلہ_destroy (داخلہ);
  hb_face_destroy (face);
  واپس 0;
}
