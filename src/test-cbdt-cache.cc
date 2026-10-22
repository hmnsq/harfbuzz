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

#تعریف_کړه hb_malloc cbdt_test_malloc
#تعریف_کړه hb_calloc cbdt_test_calloc
#تعریف_کړه hb_realloc cbdt_test_realloc
#تعریف_کړه hb_free cbdt_test_free

#ضمن "hb-subset.hh"
#ضمن "OT/Color/CBDT/CBDT.hh"

ساكن بینښه fail_after;
ساكن بینښه ویش_شمېر;

ساكن منطقي
fail_ویش ()
{
  ویش_شمېر++;
  أرجع fail_after && !--fail_after;
}

خارجي "C" فراغ *cbdt_test_malloc (size_t اندازه)
{ أرجع fail_ویش () ? تش_اشاره : حافظه_ورکړه (اندازه); }
خارجي "C" فراغ *cbdt_test_calloc (size_t شمېر, size_t اندازه)
{ أرجع fail_ویش () ? تش_اشاره : حافظه_صفر_ورکړه (شمېر, اندازه); }
خارجي "C" فراغ *cbdt_test_realloc (فراغ *ptr, size_t اندازه)
{ أرجع fail_ویش () ? تش_اشاره : realloc (ptr, اندازه); }
خارجي "C" فراغ cbdt_test_free (فراغ *ptr)
{ حرر (ptr); }

ساكن hb_blob_t *
subset_strike (ثابت OT::Bitmap_اندازه_جدول &strike,
	       ثابت OT::CBLC *cblc,
	       hb_subset_plan_t *plan,
	       hb_بایټونه_t cbdt,
	       hb_ویکټور_t<حرف> *معلومات,
	       OT::cbdt_dedup_context_t *dedup)
{
  حرف بفر[4096];
  hb_serialize_context_t serializer (بفر, حجم (بفر));
  serializer.پیل_serialize<OT::Bitmap_اندازه_جدول> ();
  hb_subset_context_t context (تش_اشاره, plan, &serializer, HB_OT_نښان_CBLC);
  hb_always_assert (strike.subset (&context, cblc, cbdt.arrayZ, cbdt.اوږدتیا,
				  معلومات, dedup));
  serializer.پای_ټکی_serialize ();
  hb_always_assert (!serializer.دننه_error ());
  أرجع serializer.copy_blob ();
}

ساكن بینښه
test_required_allocations (ثابت OT::Bitmap_اندازه_جدول &strike,
			   ثابت OT::CBLC *cblc,
			   hb_subset_plan_t *plan,
			   hb_بایټونه_t cbdt,
			   بینښه failure)
{
  حرف بفر[4096];
  hb_serialize_context_t serializer (بفر, حجم (بفر));
  serializer.پیل_serialize<OT::Bitmap_اندازه_جدول> ();
  hb_subset_context_t context (تش_اشاره, plan, &serializer, HB_OT_نښان_CBLC);
  hb_ویکټور_t<حرف> معلومات;
  fail_after = failure;
  ویش_شمېر = 0;
  منطقي success = strike.subset (&context, cblc, cbdt.arrayZ, cbdt.اوږدتیا,
			       &معلومات, تش_اشاره);
  بینښه شمېر = ویش_شمېر;
  hb_always_assert (!fail_after);
  /* Required image/offset allocations must fail serialization instead of
   * being mistaken for a strike that can simply be discarded. */
  hb_always_assert (success || serializer.دننه_error ());
  serializer.پای_ټکی_serialize ();
  أرجع شمېر;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  hb_always_assert (argc == 2);
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0);
  hb_blob_destroy (blob);
  hb_subset_ننوت_t *ننوت = hb_subset_ننوت_create_or_fail ();
  hb_always_assert (ننوت);
  hb_سېټ_t *unicodes = hb_subset_ننوت_unicode_سېټ (ننوت);
  hb_سېټ_جمع_کړه (unicodes, 0x38);
  hb_سېټ_جمع_کړه (unicodes, 0xAE);
  hb_سېټ_جمع_کړه (unicodes, 0x2049);
  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (face, ننوت);
  hb_always_assert (plan);

  hb_blob_ptr_t<OT::CBLC> cblc =
      hb_sanitize_context_t ().reference_جدول<OT::CBLC> (face);
  hb_blob_ptr_t<OT::CBDT> cbdt =
      hb_sanitize_context_t ().reference_جدول<OT::CBDT> (face);
  hb_always_assert (cblc.get_اوږدتیا () >= 8 + 2 * OT::Bitmap_اندازه_جدول::static_اندازه);
  ثابت خپلکاری *strikes = reinterpret_cast<ثابت OT::Bitmap_اندازه_جدول *> (
      reinterpret_cast<ثابت حرف *> (cblc.get ()) + 8);

  بینښه required_allocations = test_required_allocations (
      strikes[0], cblc.get (), plan, cbdt.get_blob ()->as_بایټونه (), 0);
  hb_always_assert (required_allocations);
  لكل (بینښه failure = 1; failure <= required_allocations; failure++)
    test_required_allocations (strikes[0], cblc.get (), plan,
			       cbdt.get_blob ()->as_بایټونه (), failure);

  hb_ویکټور_t<حرف> expected_معلومات;
  hb_blob_t *expected[2];
  لكل (بینښه i = 0; i < 2; i++)
    expected[i] = subset_strike (strikes[i], cblc.get (), plan, cbdt.get_blob ()->as_بایټونه (),
				&expected_معلومات, تش_اشاره);

  /* Fail the entries vector, lengths vector, and map allocations in turn.
   * A disabled cache must preserve both strikes and every image byte. */
  لكل (بینښه failure = 1; failure <= 3; failure++)
  {
    OT::cbdt_dedup_context_t dedup;
    hb_ویکټور_t<بینښه> lengths {1};
    fail_after = failure;
    hb_always_assert (!dedup.جمع_کړه (0, 0, lengths));
    hb_always_assert (!fail_after);
    hb_always_assert (dedup.entries.دننه_error () || dedup.lengths.دننه_error () ||
		      dedup.map.دننه_error ());

    hb_ویکټور_t<حرف> actual_معلومات;
    لكل (بینښه i = 0; i < 2; i++)
    {
      hb_blob_t *actual = subset_strike (strikes[i], cblc.get (), plan,
					cbdt.get_blob ()->as_بایټونه (), &actual_معلومات, &dedup);
      hb_always_assert (hb_blob_get_اوږدتیا (actual) == hb_blob_get_اوږدتیا (expected[i]));
      hb_always_assert (!hb_memcmp (hb_blob_get_معلومات (actual, تش_اشاره),
				   hb_blob_get_معلومات (expected[i], تش_اشاره),
				   hb_blob_get_اوږدتیا (actual)));
      hb_blob_destroy (actual);
    }
    hb_always_assert (actual_معلومات.as_بایټونه () == expected_معلومات.as_بایټونه ());
  }

  لكل (hb_blob_t *expected_blob : expected)
    hb_blob_destroy (expected_blob);
  cbdt.destroy ();
  cblc.destroy ();
  hb_subset_plan_destroy (plan);
  hb_subset_ننوت_destroy (ننوت);
  hb_face_destroy (face);
  أرجع 0;
}
