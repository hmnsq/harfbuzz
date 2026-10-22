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

#تعریف_کن hb_malloc cbdt_test_malloc
#تعریف_کن hb_calloc cbdt_test_calloc
#تعریف_کن hb_realloc cbdt_test_realloc
#تعریف_کن hb_free cbdt_test_free

#ضمن "hb-subset.hh"
#ضمن "OT/Color/CBDT/CBDT.hh"

ساكن بیعلامت fail_after;
ساكن بیعلامت تخصیص_شمار;

ساكن منطقي
fail_تخصیص ()
{
  تخصیص_شمار++;
  أرجع fail_after && !--fail_after;
}

خارجي "C" فراغ *cbdt_test_malloc (size_t اندازه)
{ أرجع fail_تخصیص () ? اشاره_تهی : حافظه_بده (اندازه); }
خارجي "C" فراغ *cbdt_test_calloc (size_t شمار, size_t اندازه)
{ أرجع fail_تخصیص () ? اشاره_تهی : حافظه_صفر_بده (شمار, اندازه); }
خارجي "C" فراغ *cbdt_test_realloc (فراغ *ptr, size_t اندازه)
{ أرجع fail_تخصیص () ? اشاره_تهی : realloc (ptr, اندازه); }
خارجي "C" فراغ cbdt_test_free (فراغ *ptr)
{ حرر (ptr); }

ساكن hb_blob_t *
subset_strike (ثابت OT::Bitmap_اندازه_جدول &strike,
	       ثابت OT::CBLC *cblc,
	       hb_subset_plan_t *plan,
	       hb_بایتها_t cbdt,
	       hb_وکتور_t<حرف> *داده,
	       OT::cbdt_dedup_context_t *dedup)
{
  حرف میانگیر[4096];
  hb_serialize_context_t serializer (میانگیر, حجم (میانگیر));
  serializer.شروع_serialize<OT::Bitmap_اندازه_جدول> ();
  hb_subset_context_t context (اشاره_تهی, plan, &serializer, HB_OT_برچسب_CBLC);
  hb_always_assert (strike.subset (&context, cblc, cbdt.arrayZ, cbdt.درازی,
				  داده, dedup));
  serializer.انتها_serialize ();
  hb_always_assert (!serializer.درون_error ());
  أرجع serializer.copy_blob ();
}

ساكن بیعلامت
test_required_allocations (ثابت OT::Bitmap_اندازه_جدول &strike,
			   ثابت OT::CBLC *cblc,
			   hb_subset_plan_t *plan,
			   hb_بایتها_t cbdt,
			   بیعلامت failure)
{
  حرف میانگیر[4096];
  hb_serialize_context_t serializer (میانگیر, حجم (میانگیر));
  serializer.شروع_serialize<OT::Bitmap_اندازه_جدول> ();
  hb_subset_context_t context (اشاره_تهی, plan, &serializer, HB_OT_برچسب_CBLC);
  hb_وکتور_t<حرف> داده;
  fail_after = failure;
  تخصیص_شمار = 0;
  منطقي success = strike.subset (&context, cblc, cbdt.arrayZ, cbdt.درازی,
			       &داده, اشاره_تهی);
  بیعلامت شمار = تخصیص_شمار;
  hb_always_assert (!fail_after);
  /* Required image/offset allocations must fail serialization instead of
   * being mistaken for a strike that can simply be discarded. */
  hb_always_assert (success || serializer.درون_error ());
  serializer.انتها_serialize ();
  أرجع شمار;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  hb_always_assert (argc == 2);
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0);
  hb_blob_destroy (blob);
  hb_subset_درون_داد_t *درون_داد = hb_subset_درون_داد_create_or_fail ();
  hb_always_assert (درون_داد);
  hb_مجموعه_t *unicodes = hb_subset_درون_داد_unicode_مجموعه (درون_داد);
  hb_مجموعه_جمع_کن (unicodes, 0x38);
  hb_مجموعه_جمع_کن (unicodes, 0xAE);
  hb_مجموعه_جمع_کن (unicodes, 0x2049);
  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (face, درون_داد);
  hb_always_assert (plan);

  hb_blob_ptr_t<OT::CBLC> cblc =
      hb_sanitize_context_t ().reference_جدول<OT::CBLC> (face);
  hb_blob_ptr_t<OT::CBDT> cbdt =
      hb_sanitize_context_t ().reference_جدول<OT::CBDT> (face);
  hb_always_assert (cblc.get_درازی () >= 8 + 2 * OT::Bitmap_اندازه_جدول::static_اندازه);
  ثابت خودکار *strikes = reinterpret_cast<ثابت OT::Bitmap_اندازه_جدول *> (
      reinterpret_cast<ثابت حرف *> (cblc.get ()) + 8);

  بیعلامت required_allocations = test_required_allocations (
      strikes[0], cblc.get (), plan, cbdt.get_blob ()->as_بایتها (), 0);
  hb_always_assert (required_allocations);
  لكل (بیعلامت failure = 1; failure <= required_allocations; failure++)
    test_required_allocations (strikes[0], cblc.get (), plan,
			       cbdt.get_blob ()->as_بایتها (), failure);

  hb_وکتور_t<حرف> expected_داده;
  hb_blob_t *expected[2];
  لكل (بیعلامت i = 0; i < 2; i++)
    expected[i] = subset_strike (strikes[i], cblc.get (), plan, cbdt.get_blob ()->as_بایتها (),
				&expected_داده, اشاره_تهی);

  /* Fail the entries vector, lengths vector, and map allocations in turn.
   * A disabled cache must preserve both strikes and every image byte. */
  لكل (بیعلامت failure = 1; failure <= 3; failure++)
  {
    OT::cbdt_dedup_context_t dedup;
    hb_وکتور_t<بیعلامت> lengths {1};
    fail_after = failure;
    hb_always_assert (!dedup.جمع_کن (0, 0, lengths));
    hb_always_assert (!fail_after);
    hb_always_assert (dedup.entries.درون_error () || dedup.lengths.درون_error () ||
		      dedup.map.درون_error ());

    hb_وکتور_t<حرف> actual_داده;
    لكل (بیعلامت i = 0; i < 2; i++)
    {
      hb_blob_t *actual = subset_strike (strikes[i], cblc.get (), plan,
					cbdt.get_blob ()->as_بایتها (), &actual_داده, &dedup);
      hb_always_assert (hb_blob_get_درازی (actual) == hb_blob_get_درازی (expected[i]));
      hb_always_assert (!hb_memcmp (hb_blob_get_داده (actual, اشاره_تهی),
				   hb_blob_get_داده (expected[i], اشاره_تهی),
				   hb_blob_get_درازی (actual)));
      hb_blob_destroy (actual);
    }
    hb_always_assert (actual_داده.as_بایتها () == expected_داده.as_بایتها ());
  }

  لكل (hb_blob_t *expected_blob : expected)
    hb_blob_destroy (expected_blob);
  cbdt.destroy ();
  cblc.destroy ();
  hb_subset_plan_destroy (plan);
  hb_subset_درون_داد_destroy (درون_داد);
  hb_face_destroy (face);
  أرجع 0;
}
