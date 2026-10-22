/*
 * Copyright © 2025  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
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
 *
 * Google Author(s): Garret Rieger
 */

#ضمن "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
ساكن صحيح جیوندا_instances = 0;

تعداد error_کوڈ_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

سانچا <قسم_ناں T>
ورتوں نتیجہ = hb_نتیجہ_t<T, error_کوڈ_t>;

بنية resource_t
{
  صحيح id;

  واضح resource_t (صحيح id_) : id (id_) { جیوندا_instances++; }
  resource_t (ثابت resource_t& o) : id (o.id) { جیوندا_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; جیوندا_instances++; }
  ~resource_t () { جیوندا_instances--; }

  resource_t& عامل = (ثابت resource_t& o)
  {
    id = o.id;
    أرجع *هذا;
  }
  resource_t& عامل = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    أرجع *هذا;
  }

  منطقي عامل == (ثابت resource_t& o) ثابت { أرجع id == o.id; }
};

ساكن نتیجہ<صحيح> returns_ok (صحيح v)
{
  أرجع Ok(v);
}

ساكن نتیجہ<صحيح> returns_err (error_کوڈ_t e)
{
  أرجع Err(e);
}

ساكن نتیجہ<صحيح> try_callee (منطقي fail, صحيح v, error_کوڈ_t e = ERR_A)
{
  إذا (fail) أرجع Err(e);
  أرجع Ok(v);
}

ساكن نتیجہ<فراغ> try_callee_void (منطقي fail,  error_کوڈ_t e)
{
  إذا (fail) أرجع e;
  أرجع Ok();
}

ساكن نتیجہ<صحيح> try_caller (منطقي fail1, منطقي fail2)
{
  TRY_سونپو (صحيح اشاریہ_2, try_callee (fail1, 10, ERR_A));
  TRY_سونپو (صحيح index2, try_callee (fail2, 20, ERR_B));
  أرجع Ok(اشاریہ_2 + index2);
}

ساكن نتیجہ<فراغ> try_caller_void (منطقي fail)
{
  TRY (try_callee_void (fail, ERR_C));
  أرجع Ok();
}

ساكن نتیجہ<عائم> try_caller_type_change (منطقي fail)
{
  TRY_سونپو (صحيح اشاریہ_2, try_callee (fail, 100, ERR_A));
  أرجع Ok((عائم) اشاریہ_2 * 1.5f);
}

ساكن نتیجہ<صحيح> try_سونپو_existing (منطقي fail)
{
  صحيح val = 1;
  TRY_سونپو (val, try_callee (fail, 42, ERR_A));
  أرجع Ok(val);
}

ساكن فراغ test_ok_basic ()
{
  نتیجہ<صحيح> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((منطقي) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.قدر_or (0) == 42);
  hb_always_assert (r.قدر_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

ساكن فراغ test_err_basic ()
{
  نتیجہ<صحيح> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.قدر_or (99) == 99);
  hb_always_assert (r.قدر_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

ساكن فراغ test_explicit_ok_err ()
{
  نتیجہ<بنانشانی> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  نتیجہ<بنانشانی> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

ساكن فراغ test_سوچکاں ()
{
  صحيح x = 123;
  نتیجہ<صحيح*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

ساكن فراغ test_void ()
{
  نتیجہ<فراغ> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((منطقي) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  نتیجہ<فراغ> r2 = Err(ERR_A);
  hb_always_assert (!r2.is_ok ());
  hb_always_assert (r2.is_err ());
  hb_always_assert (!r2);
  hb_always_assert (r2.error () == ERR_A);
  hb_always_assert (r2 == Err(ERR_A));
  hb_always_assert (r2 != Err(ERR_B));
  hb_always_assert (r2 != Ok());
  hb_always_assert (Err(ERR_A) == r2);
  hb_always_assert (Err(ERR_B) != r2);
  hb_always_assert (Ok() != r2);
}

ساكن فراغ test_resource_cleanup ()
{
  hb_always_assert (جیوندا_instances == 0);

  {
    نتیجہ<resource_t> r = resource_t (1);
    hb_always_assert (جیوندا_instances == 1);
    hb_always_assert (r.value ().id == 1);
  }
  hb_always_assert (جیوندا_instances == 0);

  {
    نتیجہ<resource_t> r1 = resource_t (2);
    hb_always_assert (جیوندا_instances == 1);
    نتیجہ<resource_t> r2 = r1;
    hb_always_assert (جیوندا_instances == 2);
    نتیجہ<resource_t> r3 = std::move (r1);
    hb_always_assert (جیوندا_instances == 3);
  }
  hb_always_assert (جیوندا_instances == 0);

  {
    نتیجہ<resource_t> r = resource_t (3);
    hb_always_assert (جیوندا_instances == 1);
    r = Err(ERR_A);
    hb_always_assert (جیوندا_instances == 0);
    hb_always_assert (r.is_err ());
  }
  hb_always_assert (جیوندا_instances == 0);
}

ساكن فراغ test_try_macro ()
{
  // Test success path
  نتیجہ<صحيح> r_success = try_caller (خطأ, خطأ);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  نتیجہ<صحيح> r_fail1 = try_caller (سچ, خطأ);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  نتیجہ<صحيح> r_fail2 = try_caller (خطأ, سچ);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  نتیجہ<فراغ> v_ok = try_caller_void (خطأ);
  hb_always_assert (v_ok.is_ok ());

  نتیجہ<فراغ> v_err = try_caller_void (سچ);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  نتیجہ<عائم> f_ok = try_caller_type_change (خطأ);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  نتیجہ<عائم> f_err = try_caller_type_change (سچ);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  نتیجہ<صحيح> a_ok = try_سونپو_existing (خطأ);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  نتیجہ<صحيح> a_err = try_سونپو_existing (سچ);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

سانچا <قسم_ناں T, قسم_ناں U, قسم_ناں = فراغ>
بنية is_equality_comparable : std::false_type {};

سانچا <قسم_ناں T, قسم_ناں U>
بنية is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

ساكن فراغ test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<نتیجہ<صحيح>, صحيح>::value, "");
  static_assert (!is_equality_comparable<نتیجہ<صحيح>, int64_t>::value, "");
  static_assert (!is_equality_comparable<صحيح, نتیجہ<صحيح>>::value, "");
  static_assert (!is_equality_comparable<نتیجہ<صحيح>, error_کوڈ_t>::value, "");
  static_assert (!is_equality_comparable<error_کوڈ_t, نتیجہ<صحيح>>::value, "");
  static_assert (!is_equality_comparable<نتیجہ<فراغ>, error_کوڈ_t>::value, "");
  static_assert (is_equality_comparable<نتیجہ<صحيح>, نتیجہ<صحيح>>::value, "");
  static_assert (is_equality_comparable<نتیجہ<صحيح>, hb_نتیجہ_ok_t<صحيح>>::value, "");
  static_assert (is_equality_comparable<نتیجہ<صحيح>, hb_نتیجہ_err_t<error_کوڈ_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_نتیجہ_t<int64_t, int32_t>, صحيح>::value, "");
  static_assert (!std::is_constructible<hb_نتیجہ_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_نتیجہ_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_نتیجہ_t<int64_t, int32_t>, hb_نتیجہ_ok_t<صحيح>>::value, "");
  static_assert (std::is_constructible<hb_نتیجہ_t<int64_t, int32_t>, hb_نتیجہ_err_t<صحيح>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_نتیجہ_t<صحيح, صحيح>, صحيح>::value, "");
  static_assert (std::is_constructible<hb_نتیجہ_t<صحيح, صحيح>, hb_نتیجہ_ok_t<صحيح>>::value, "");
  static_assert (std::is_constructible<hb_نتیجہ_t<صحيح, صحيح>, hb_نتیجہ_err_t<صحيح>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_نتیجہ_t<صحيح, error_کوڈ_t>, صحيح>::value, "");
  static_assert (!std::is_constructible<hb_نتیجہ_t<صحيح, error_کوڈ_t>, error_کوڈ_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_نتیجہ_t<resource_t, error_کوڈ_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_نتیجہ_t<resource_t, error_کوڈ_t>, error_کوڈ_t>::value, "");

  hb_نتیجہ_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_نتیجہ_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_نتیجہ_t<صحيح, صحيح> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_نتیجہ_t<صحيح, صحيح> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

بنية any_قدر_t
{
  سانچا <قسم_ناں X>
  any_قدر_t (X&&) {}
};

ساكن فراغ test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_نتیجہ_t<any_قدر_t, error_کوڈ_t>, hb_نتیجہ_err_t<ثابت حرف*>>::value, "");

  hb_نتیجہ_t<any_قدر_t, error_کوڈ_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_نتیجہ_t<any_قدر_t, error_کوڈ_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

بنية move_only_err_t
{
  صحيح کوڈ;

  واضح move_only_err_t (صحيح c) : کوڈ (c) {}
  move_only_err_t (ثابت move_only_err_t&) = مٹاؤ;
  move_only_err_t (move_only_err_t&& o) : کوڈ (o.کوڈ) { o.کوڈ = -1; }
  move_only_err_t& عامل = (ثابت move_only_err_t&) = مٹاؤ;
  move_only_err_t& عامل = (move_only_err_t&& o) { کوڈ = o.کوڈ; o.کوڈ = -1; أرجع *هذا; }
  ~move_only_err_t () = افتراضي;

  منطقي عامل == (ثابت move_only_err_t& o) ثابت { أرجع کوڈ == o.کوڈ; }
};

بنية move_only_val_t
{
  صحيح val;

  واضح move_only_val_t (صحيح v) : val (v) {}
  move_only_val_t (ثابت move_only_val_t&) = مٹاؤ;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& عامل = (ثابت move_only_val_t&) = مٹاؤ;
  move_only_val_t& عامل = (move_only_val_t&& o) { val = o.val; o.val = -1; أرجع *هذا; }
  ~move_only_val_t () = افتراضي;

  منطقي عامل == (ثابت move_only_val_t& o) ثابت { أرجع val == o.val; }
};

ساكن فراغ test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_نتیجہ_t<صحيح, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().کوڈ == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.کوڈ == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_نتیجہ_t<صحيح, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().کوڈ == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_نتیجہ_t<فراغ, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().کوڈ == 789);

    hb_نتیجہ_t<فراغ, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().کوڈ == 999);
  }

  // Test value_or with move-only value types
  {
    hb_نتیجہ_t<move_only_val_t, error_کوڈ_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).قدر_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_نتیجہ_t<move_only_val_t, error_کوڈ_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).قدر_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

بنية copy_move_counter_t
{
  ساكن صحيح copy_گنتی;
  ساكن صحيح move_گنتی;

  ساكن فراغ reset ()
  {
    copy_گنتی = 0;
    move_گنتی = 0;
  }

  صحيح val;

  منطقي وچ_error() ثابت {
    أرجع val == -1;
  }

  واضح copy_move_counter_t (صحيح v) : val (v) {}
  copy_move_counter_t (ثابت copy_move_counter_t& o) : val (o.val) { copy_گنتی++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_گنتی++; }
  ~copy_move_counter_t () = افتراضي;
};

صحيح copy_move_counter_t::copy_گنتی = 0;
صحيح copy_move_counter_t::move_گنتی = 0;

ساكن نتیجہ<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t باہرلا (42);
  أرجع Ok (std::move (باہرلا));
}

ساكن نتیجہ<copy_move_counter_t> return_tracked_from_local_with_ٹیگ ()
{
  copy_move_counter_t باہرلا (42);
  أرجع Ok(باہرلا);
}

ساكن hb_نتیجہ_t<صحيح, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t باہرلا (42);
  أرجع Err (std::move (باہرلا));
}

ساكن hb_نتیجہ_t<صحيح, copy_move_counter_t> return_tracked_error_from_local_with_ٹیگ ()
{
  copy_move_counter_t باہرلا (42);
  أرجع Err(باہرلا);
}

ساكن نتیجہ<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t باہرلا (42);
  أرجع Ok (std::move (باہرلا));
}

ساكن فراغ test_return_چالاں_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    نتیجہ<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_گنتی == 0);
    hb_always_assert (copy_move_counter_t::move_گنتی >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    نتیجہ<copy_move_counter_t> r = return_tracked_from_local_with_ٹیگ ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_گنتی == 0);
    hb_always_assert (copy_move_counter_t::move_گنتی >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_نتیجہ_t<صحيح, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_گنتی == 0);
    hb_always_assert (copy_move_counter_t::move_گنتی >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_نتیجہ_t<صحيح, copy_move_counter_t> r = return_tracked_error_from_local_with_ٹیگ ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_گنتی == 0);
    hb_always_assert (copy_move_counter_t::move_گنتی >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    نتیجہ<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

بنية وچ_error_t {
  منطقي successful;

  وچ_error_t(منطقي s) : successful(s) {}

  منطقي وچ_error() ثابت {
    أرجع !successful;
  }
};

ساكن فراغ test_from () {
  وچ_error_t has_error(خطأ);
  وچ_error_t no_error(سچ);
  نتیجہ<فراغ> r = نتیجہ<فراغ>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = نتیجہ<فراغ>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

بنية default_constructible_t
{
  صحيح val;

  default_constructible_t () : val (123) {}
  واضح default_constructible_t (صحيح v) : val (v) {}
};

بنية move_only_default_t
{
  صحيح val;
  move_only_default_t () : val (77) {}
  واضح move_only_default_t (صحيح v) : val (v) {}
  move_only_default_t (ثابت move_only_default_t&) = مٹاؤ;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& عامل = (ثابت move_only_default_t&) = مٹاؤ;
  move_only_default_t& عامل = (move_only_default_t&& o) { val = o.val; o.val = -1; أرجع *هذا; }
  ~move_only_default_t () = افتراضي;
};

ساكن فراغ test_قدر_or_default ()
{
  // Test const &
  {
    ثابت نتیجہ<صحيح> r_ok = Ok(42);
    hb_always_assert (r_ok.قدر_or_default () == 42);

    ثابت نتیجہ<صحيح> r_err = Err(ERR_A);
    hb_always_assert (r_err.قدر_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    نتیجہ<صحيح> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).قدر_or_default () == 42);

    نتیجہ<صحيح> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).قدر_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    نتیجہ<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.قدر_or_default ().val == 456);

    نتیجہ<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.قدر_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).قدر_or_default ().val == 456);
    hb_always_assert (std::move (r_err).قدر_or_default ().val == 123);
  }

  // Test pointers
  {
    صحيح x = 10;
    نتیجہ<صحيح*> r_ok = &x;
    hb_always_assert (r_ok.قدر_or_default () == &x);

    نتیجہ<صحيح*> r_err = ERR_A;
    hb_always_assert (r_err.قدر_or_default () == خالی_سوچک);
  }

  // Test move-only types
  {
    نتیجہ<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).قدر_or_default ().val == 88);

    نتیجہ<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).قدر_or_default ().val == 77);
  }
}

ساكن فراغ test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    نتیجہ<copy_move_counter_t> r_ok = نتیجہ<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_گنتی == 0);
    hb_always_assert (copy_move_counter_t::move_گنتی == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    نتیجہ<copy_move_counter_t> r_fail = نتیجہ<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_گنتی == 0);
    hb_always_assert (copy_move_counter_t::move_گنتی == 0);
  }
}

صحيح مکھ ()
{
  test_ok_basic ();
  test_err_basic ();
  test_explicit_ok_err ();
  test_سوچکاں ();
  test_void ();
  test_resource_cleanup ();
  test_try_macro ();
  test_same_or_convertible_types ();
  test_error_interception ();
  test_move_only_types ();
  test_return_چالاں_from_local ();
  test_from();
  test_قدر_or_default ();
  test_from_move ();

  أرجع 0;
}
