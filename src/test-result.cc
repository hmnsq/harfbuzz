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

#شامل "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
ساکن صحیح زندہ_instances = 0;

شمار error_رمز_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

سانچہ <قسم_نام T>
استعمال نتیجہ = hb_نتیجہ_t<T, error_رمز_t>;

ساخت resource_t
{
  صحیح id;

  صریح resource_t (صحیح id_) : id (id_) { زندہ_instances++; }
  resource_t (ثابت resource_t& o) : id (o.id) { زندہ_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; زندہ_instances++; }
  ~resource_t () { زندہ_instances--; }

  resource_t& عامل = (ثابت resource_t& o)
  {
    id = o.id;
    واپس *یہ;
  }
  resource_t& عامل = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    واپس *یہ;
  }

  منطقی عامل == (ثابت resource_t& o) ثابت { واپس id == o.id; }
};

ساکن نتیجہ<صحیح> returns_ok (صحیح v)
{
  واپس Ok(v);
}

ساکن نتیجہ<صحیح> returns_err (error_رمز_t e)
{
  واپس Err(e);
}

ساکن نتیجہ<صحیح> try_callee (منطقی fail, صحیح v, error_رمز_t e = ERR_A)
{
  اگر (fail) واپس Err(e);
  واپس Ok(v);
}

ساکن نتیجہ<خالی> try_callee_void (منطقی fail,  error_رمز_t e)
{
  اگر (fail) واپس e;
  واپس Ok();
}

ساکن نتیجہ<صحیح> try_caller (منطقی fail1, منطقی fail2)
{
  TRY_تفویض (صحیح اشاریہ_2, try_callee (fail1, 10, ERR_A));
  TRY_تفویض (صحیح index2, try_callee (fail2, 20, ERR_B));
  واپس Ok(اشاریہ_2 + index2);
}

ساکن نتیجہ<خالی> try_caller_void (منطقی fail)
{
  TRY (try_callee_void (fail, ERR_C));
  واپس Ok();
}

ساکن نتیجہ<اعشاری> try_caller_type_change (منطقی fail)
{
  TRY_تفویض (صحیح اشاریہ_2, try_callee (fail, 100, ERR_A));
  واپس Ok((اعشاری) اشاریہ_2 * 1.5f);
}

ساکن نتیجہ<صحیح> try_تفویض_existing (منطقی fail)
{
  صحیح val = 1;
  TRY_تفویض (val, try_callee (fail, 42, ERR_A));
  واپس Ok(val);
}

ساکن خالی test_ok_basic ()
{
  نتیجہ<صحیح> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((منطقی) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.قدر_or (0) == 42);
  hb_always_assert (r.قدر_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

ساکن خالی test_err_basic ()
{
  نتیجہ<صحیح> r = returns_err (ERR_A);
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

ساکن خالی test_explicit_ok_err ()
{
  نتیجہ<غیرعلامتی> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  نتیجہ<غیرعلامتی> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

ساکن خالی test_اشارے ()
{
  صحیح x = 123;
  نتیجہ<صحیح*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

ساکن خالی test_void ()
{
  نتیجہ<خالی> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((منطقی) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  نتیجہ<خالی> r2 = Err(ERR_A);
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

ساکن خالی test_resource_cleanup ()
{
  hb_always_assert (زندہ_instances == 0);

  {
    نتیجہ<resource_t> r = resource_t (1);
    hb_always_assert (زندہ_instances == 1);
    hb_always_assert (r.value ().id == 1);
  }
  hb_always_assert (زندہ_instances == 0);

  {
    نتیجہ<resource_t> r1 = resource_t (2);
    hb_always_assert (زندہ_instances == 1);
    نتیجہ<resource_t> r2 = r1;
    hb_always_assert (زندہ_instances == 2);
    نتیجہ<resource_t> r3 = std::move (r1);
    hb_always_assert (زندہ_instances == 3);
  }
  hb_always_assert (زندہ_instances == 0);

  {
    نتیجہ<resource_t> r = resource_t (3);
    hb_always_assert (زندہ_instances == 1);
    r = Err(ERR_A);
    hb_always_assert (زندہ_instances == 0);
    hb_always_assert (r.is_err ());
  }
  hb_always_assert (زندہ_instances == 0);
}

ساکن خالی test_try_macro ()
{
  // Test success path
  نتیجہ<صحیح> r_success = try_caller (جھوٹ, جھوٹ);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  نتیجہ<صحیح> r_fail1 = try_caller (سچ, جھوٹ);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  نتیجہ<صحیح> r_fail2 = try_caller (جھوٹ, سچ);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  نتیجہ<خالی> v_ok = try_caller_void (جھوٹ);
  hb_always_assert (v_ok.is_ok ());

  نتیجہ<خالی> v_err = try_caller_void (سچ);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  نتیجہ<اعشاری> f_ok = try_caller_type_change (جھوٹ);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  نتیجہ<اعشاری> f_err = try_caller_type_change (سچ);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  نتیجہ<صحیح> a_ok = try_تفویض_existing (جھوٹ);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  نتیجہ<صحیح> a_err = try_تفویض_existing (سچ);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

سانچہ <قسم_نام T, قسم_نام U, قسم_نام = خالی>
ساخت is_equality_comparable : std::false_type {};

سانچہ <قسم_نام T, قسم_نام U>
ساخت is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

ساکن خالی test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<نتیجہ<صحیح>, صحیح>::value, "");
  static_assert (!is_equality_comparable<نتیجہ<صحیح>, int64_t>::value, "");
  static_assert (!is_equality_comparable<صحیح, نتیجہ<صحیح>>::value, "");
  static_assert (!is_equality_comparable<نتیجہ<صحیح>, error_رمز_t>::value, "");
  static_assert (!is_equality_comparable<error_رمز_t, نتیجہ<صحیح>>::value, "");
  static_assert (!is_equality_comparable<نتیجہ<خالی>, error_رمز_t>::value, "");
  static_assert (is_equality_comparable<نتیجہ<صحیح>, نتیجہ<صحیح>>::value, "");
  static_assert (is_equality_comparable<نتیجہ<صحیح>, hb_نتیجہ_ok_t<صحیح>>::value, "");
  static_assert (is_equality_comparable<نتیجہ<صحیح>, hb_نتیجہ_err_t<error_رمز_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_نتیجہ_t<int64_t, int32_t>, صحیح>::value, "");
  static_assert (!std::is_constructible<hb_نتیجہ_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_نتیجہ_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_نتیجہ_t<int64_t, int32_t>, hb_نتیجہ_ok_t<صحیح>>::value, "");
  static_assert (std::is_constructible<hb_نتیجہ_t<int64_t, int32_t>, hb_نتیجہ_err_t<صحیح>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_نتیجہ_t<صحیح, صحیح>, صحیح>::value, "");
  static_assert (std::is_constructible<hb_نتیجہ_t<صحیح, صحیح>, hb_نتیجہ_ok_t<صحیح>>::value, "");
  static_assert (std::is_constructible<hb_نتیجہ_t<صحیح, صحیح>, hb_نتیجہ_err_t<صحیح>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_نتیجہ_t<صحیح, error_رمز_t>, صحیح>::value, "");
  static_assert (!std::is_constructible<hb_نتیجہ_t<صحیح, error_رمز_t>, error_رمز_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_نتیجہ_t<resource_t, error_رمز_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_نتیجہ_t<resource_t, error_رمز_t>, error_رمز_t>::value, "");

  hb_نتیجہ_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_نتیجہ_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_نتیجہ_t<صحیح, صحیح> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_نتیجہ_t<صحیح, صحیح> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

ساخت any_قدر_t
{
  سانچہ <قسم_نام X>
  any_قدر_t (X&&) {}
};

ساکن خالی test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_نتیجہ_t<any_قدر_t, error_رمز_t>, hb_نتیجہ_err_t<ثابت حرف*>>::value, "");

  hb_نتیجہ_t<any_قدر_t, error_رمز_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_نتیجہ_t<any_قدر_t, error_رمز_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

ساخت move_only_err_t
{
  صحیح رمز;

  صریح move_only_err_t (صحیح c) : رمز (c) {}
  move_only_err_t (ثابت move_only_err_t&) = حذف;
  move_only_err_t (move_only_err_t&& o) : رمز (o.رمز) { o.رمز = -1; }
  move_only_err_t& عامل = (ثابت move_only_err_t&) = حذف;
  move_only_err_t& عامل = (move_only_err_t&& o) { رمز = o.رمز; o.رمز = -1; واپس *یہ; }
  ~move_only_err_t () = طےشدہ;

  منطقی عامل == (ثابت move_only_err_t& o) ثابت { واپس رمز == o.رمز; }
};

ساخت move_only_val_t
{
  صحیح val;

  صریح move_only_val_t (صحیح v) : val (v) {}
  move_only_val_t (ثابت move_only_val_t&) = حذف;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& عامل = (ثابت move_only_val_t&) = حذف;
  move_only_val_t& عامل = (move_only_val_t&& o) { val = o.val; o.val = -1; واپس *یہ; }
  ~move_only_val_t () = طےشدہ;

  منطقی عامل == (ثابت move_only_val_t& o) ثابت { واپس val == o.val; }
};

ساکن خالی test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_نتیجہ_t<صحیح, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().رمز == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.رمز == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_نتیجہ_t<صحیح, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().رمز == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_نتیجہ_t<خالی, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().رمز == 789);

    hb_نتیجہ_t<خالی, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().رمز == 999);
  }

  // Test value_or with move-only value types
  {
    hb_نتیجہ_t<move_only_val_t, error_رمز_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).قدر_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_نتیجہ_t<move_only_val_t, error_رمز_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).قدر_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

ساخت copy_move_شمار_t
{
  ساکن صحیح copy_گنتی;
  ساکن صحیح move_گنتی;

  ساکن خالی reset ()
  {
    copy_گنتی = 0;
    move_گنتی = 0;
  }

  صحیح val;

  منطقی اندر_error() ثابت {
    واپس val == -1;
  }

  صریح copy_move_شمار_t (صحیح v) : val (v) {}
  copy_move_شمار_t (ثابت copy_move_شمار_t& o) : val (o.val) { copy_گنتی++; }
  copy_move_شمار_t (copy_move_شمار_t&& o) : val (o.val) { o.val = -1; move_گنتی++; }
  ~copy_move_شمار_t () = طےشدہ;
};

صحیح copy_move_شمار_t::copy_گنتی = 0;
صحیح copy_move_شمار_t::move_گنتی = 0;

ساکن نتیجہ<copy_move_شمار_t> return_tracked_from_local ()
{
  copy_move_شمار_t برآمد (42);
  واپس Ok (std::move (برآمد));
}

ساکن نتیجہ<copy_move_شمار_t> return_tracked_from_local_with_لیبل ()
{
  copy_move_شمار_t برآمد (42);
  واپس Ok(برآمد);
}

ساکن hb_نتیجہ_t<صحیح, copy_move_شمار_t> return_tracked_error_from_local ()
{
  copy_move_شمار_t برآمد (42);
  واپس Err (std::move (برآمد));
}

ساکن hb_نتیجہ_t<صحیح, copy_move_شمار_t> return_tracked_error_from_local_with_لیبل ()
{
  copy_move_شمار_t برآمد (42);
  واپس Err(برآمد);
}

ساکن نتیجہ<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t برآمد (42);
  واپس Ok (std::move (برآمد));
}

ساکن خالی test_return_چالیں_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_شمار_t::reset ();
    نتیجہ<copy_move_شمار_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_شمار_t::copy_گنتی == 0);
    hb_always_assert (copy_move_شمار_t::move_گنتی >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_شمار_t::reset ();
    نتیجہ<copy_move_شمار_t> r = return_tracked_from_local_with_لیبل ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_شمار_t::copy_گنتی == 0);
    hb_always_assert (copy_move_شمار_t::move_گنتی >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_شمار_t::reset ();
    hb_نتیجہ_t<صحیح, copy_move_شمار_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_شمار_t::copy_گنتی == 0);
    hb_always_assert (copy_move_شمار_t::move_گنتی >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_شمار_t::reset ();
    hb_نتیجہ_t<صحیح, copy_move_شمار_t> r = return_tracked_error_from_local_with_لیبل ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_شمار_t::copy_گنتی == 0);
    hb_always_assert (copy_move_شمار_t::move_گنتی >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    نتیجہ<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

ساخت اندر_error_t {
  منطقی successful;

  اندر_error_t(منطقی s) : successful(s) {}

  منطقی اندر_error() ثابت {
    واپس !successful;
  }
};

ساکن خالی test_from () {
  اندر_error_t has_error(جھوٹ);
  اندر_error_t no_error(سچ);
  نتیجہ<خالی> r = نتیجہ<خالی>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = نتیجہ<خالی>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

ساخت default_constructible_t
{
  صحیح val;

  default_constructible_t () : val (123) {}
  صریح default_constructible_t (صحیح v) : val (v) {}
};

ساخت move_only_default_t
{
  صحیح val;
  move_only_default_t () : val (77) {}
  صریح move_only_default_t (صحیح v) : val (v) {}
  move_only_default_t (ثابت move_only_default_t&) = حذف;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& عامل = (ثابت move_only_default_t&) = حذف;
  move_only_default_t& عامل = (move_only_default_t&& o) { val = o.val; o.val = -1; واپس *یہ; }
  ~move_only_default_t () = طےشدہ;
};

ساکن خالی test_قدر_or_default ()
{
  // Test const &
  {
    ثابت نتیجہ<صحیح> r_ok = Ok(42);
    hb_always_assert (r_ok.قدر_or_default () == 42);

    ثابت نتیجہ<صحیح> r_err = Err(ERR_A);
    hb_always_assert (r_err.قدر_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    نتیجہ<صحیح> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).قدر_or_default () == 42);

    نتیجہ<صحیح> r_err = Err(ERR_A);
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
    صحیح x = 10;
    نتیجہ<صحیح*> r_ok = &x;
    hb_always_assert (r_ok.قدر_or_default () == &x);

    نتیجہ<صحیح*> r_err = ERR_A;
    hb_always_assert (r_err.قدر_or_default () == خالی_اشاریہ);
  }

  // Test move-only types
  {
    نتیجہ<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).قدر_or_default ().val == 88);

    نتیجہ<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).قدر_or_default ().val == 77);
  }
}

ساکن خالی test_from_move()
{
  {
    copy_move_شمار_t::reset();
    copy_move_شمار_t ok (24);
    نتیجہ<copy_move_شمار_t> r_ok = نتیجہ<copy_move_شمار_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_شمار_t::copy_گنتی == 0);
    hb_always_assert (copy_move_شمار_t::move_گنتی == 1);
  }

  {
    copy_move_شمار_t::reset();
    copy_move_شمار_t fail (-1);
    نتیجہ<copy_move_شمار_t> r_fail = نتیجہ<copy_move_شمار_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_شمار_t::copy_گنتی == 0);
    hb_always_assert (copy_move_شمار_t::move_گنتی == 0);
  }
}

صحیح مین ()
{
  test_ok_basic ();
  test_err_basic ();
  test_explicit_ok_err ();
  test_اشارے ();
  test_void ();
  test_resource_cleanup ();
  test_try_macro ();
  test_same_or_convertible_types ();
  test_error_interception ();
  test_move_only_types ();
  test_return_چالیں_from_local ();
  test_from();
  test_قدر_or_default ();
  test_from_move ();

  واپس 0;
}
