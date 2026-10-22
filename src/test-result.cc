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
ساكن صحيح زنده_instances = 0;

تعداد error_کد_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

قالب <نام_نوع T>
استفاده نتیجه = hb_نتیجه_t<T, error_کد_t>;

بنية resource_t
{
  صحيح id;

  صریح resource_t (صحيح id_) : id (id_) { زنده_instances++; }
  resource_t (ثابت resource_t& o) : id (o.id) { زنده_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; زنده_instances++; }
  ~resource_t () { زنده_instances--; }

  resource_t& عملگر = (ثابت resource_t& o)
  {
    id = o.id;
    أرجع *هذا;
  }
  resource_t& عملگر = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    أرجع *هذا;
  }

  منطقي عملگر == (ثابت resource_t& o) ثابت { أرجع id == o.id; }
};

ساكن نتیجه<صحيح> returns_ok (صحيح v)
{
  أرجع Ok(v);
}

ساكن نتیجه<صحيح> returns_err (error_کد_t e)
{
  أرجع Err(e);
}

ساكن نتیجه<صحيح> try_callee (منطقي fail, صحيح v, error_کد_t e = ERR_A)
{
  إذا (fail) أرجع Err(e);
  أرجع Ok(v);
}

ساكن نتیجه<فراغ> try_callee_void (منطقي fail,  error_کد_t e)
{
  إذا (fail) أرجع e;
  أرجع Ok();
}

ساكن نتیجه<صحيح> try_caller (منطقي fail1, منطقي fail2)
{
  TRY_تخصیص_بده (صحيح شاخص_2, try_callee (fail1, 10, ERR_A));
  TRY_تخصیص_بده (صحيح index2, try_callee (fail2, 20, ERR_B));
  أرجع Ok(شاخص_2 + index2);
}

ساكن نتیجه<فراغ> try_caller_void (منطقي fail)
{
  TRY (try_callee_void (fail, ERR_C));
  أرجع Ok();
}

ساكن نتیجه<عائم> try_caller_type_change (منطقي fail)
{
  TRY_تخصیص_بده (صحيح شاخص_2, try_callee (fail, 100, ERR_A));
  أرجع Ok((عائم) شاخص_2 * 1.5f);
}

ساكن نتیجه<صحيح> try_تخصیص_بده_existing (منطقي fail)
{
  صحيح val = 1;
  TRY_تخصیص_بده (val, try_callee (fail, 42, ERR_A));
  أرجع Ok(val);
}

ساكن فراغ test_ok_basic ()
{
  نتیجه<صحيح> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((منطقي) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.مقدار_or (0) == 42);
  hb_always_assert (r.مقدار_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

ساكن فراغ test_err_basic ()
{
  نتیجه<صحيح> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.مقدار_or (99) == 99);
  hb_always_assert (r.مقدار_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

ساكن فراغ test_explicit_ok_err ()
{
  نتیجه<بیعلامت> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  نتیجه<بیعلامت> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

ساكن فراغ test_اشارهگرها ()
{
  صحيح x = 123;
  نتیجه<صحيح*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

ساكن فراغ test_void ()
{
  نتیجه<فراغ> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((منطقي) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  نتیجه<فراغ> r2 = Err(ERR_A);
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
  hb_always_assert (زنده_instances == 0);

  {
    نتیجه<resource_t> r = resource_t (1);
    hb_always_assert (زنده_instances == 1);
    hb_always_assert (r.value ().id == 1);
  }
  hb_always_assert (زنده_instances == 0);

  {
    نتیجه<resource_t> r1 = resource_t (2);
    hb_always_assert (زنده_instances == 1);
    نتیجه<resource_t> r2 = r1;
    hb_always_assert (زنده_instances == 2);
    نتیجه<resource_t> r3 = std::move (r1);
    hb_always_assert (زنده_instances == 3);
  }
  hb_always_assert (زنده_instances == 0);

  {
    نتیجه<resource_t> r = resource_t (3);
    hb_always_assert (زنده_instances == 1);
    r = Err(ERR_A);
    hb_always_assert (زنده_instances == 0);
    hb_always_assert (r.is_err ());
  }
  hb_always_assert (زنده_instances == 0);
}

ساكن فراغ test_try_macro ()
{
  // Test success path
  نتیجه<صحيح> r_success = try_caller (خطأ, خطأ);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  نتیجه<صحيح> r_fail1 = try_caller (درست, خطأ);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  نتیجه<صحيح> r_fail2 = try_caller (خطأ, درست);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  نتیجه<فراغ> v_ok = try_caller_void (خطأ);
  hb_always_assert (v_ok.is_ok ());

  نتیجه<فراغ> v_err = try_caller_void (درست);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  نتیجه<عائم> f_ok = try_caller_type_change (خطأ);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  نتیجه<عائم> f_err = try_caller_type_change (درست);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  نتیجه<صحيح> a_ok = try_تخصیص_بده_existing (خطأ);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  نتیجه<صحيح> a_err = try_تخصیص_بده_existing (درست);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

قالب <نام_نوع T, نام_نوع U, نام_نوع = فراغ>
بنية is_equality_comparable : std::false_type {};

قالب <نام_نوع T, نام_نوع U>
بنية is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

ساكن فراغ test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<نتیجه<صحيح>, صحيح>::value, "");
  static_assert (!is_equality_comparable<نتیجه<صحيح>, int64_t>::value, "");
  static_assert (!is_equality_comparable<صحيح, نتیجه<صحيح>>::value, "");
  static_assert (!is_equality_comparable<نتیجه<صحيح>, error_کد_t>::value, "");
  static_assert (!is_equality_comparable<error_کد_t, نتیجه<صحيح>>::value, "");
  static_assert (!is_equality_comparable<نتیجه<فراغ>, error_کد_t>::value, "");
  static_assert (is_equality_comparable<نتیجه<صحيح>, نتیجه<صحيح>>::value, "");
  static_assert (is_equality_comparable<نتیجه<صحيح>, hb_نتیجه_ok_t<صحيح>>::value, "");
  static_assert (is_equality_comparable<نتیجه<صحيح>, hb_نتیجه_err_t<error_کد_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_نتیجه_t<int64_t, int32_t>, صحيح>::value, "");
  static_assert (!std::is_constructible<hb_نتیجه_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_نتیجه_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_نتیجه_t<int64_t, int32_t>, hb_نتیجه_ok_t<صحيح>>::value, "");
  static_assert (std::is_constructible<hb_نتیجه_t<int64_t, int32_t>, hb_نتیجه_err_t<صحيح>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_نتیجه_t<صحيح, صحيح>, صحيح>::value, "");
  static_assert (std::is_constructible<hb_نتیجه_t<صحيح, صحيح>, hb_نتیجه_ok_t<صحيح>>::value, "");
  static_assert (std::is_constructible<hb_نتیجه_t<صحيح, صحيح>, hb_نتیجه_err_t<صحيح>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_نتیجه_t<صحيح, error_کد_t>, صحيح>::value, "");
  static_assert (!std::is_constructible<hb_نتیجه_t<صحيح, error_کد_t>, error_کد_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_نتیجه_t<resource_t, error_کد_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_نتیجه_t<resource_t, error_کد_t>, error_کد_t>::value, "");

  hb_نتیجه_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_نتیجه_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_نتیجه_t<صحيح, صحيح> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_نتیجه_t<صحيح, صحيح> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

بنية any_مقدار_t
{
  قالب <نام_نوع X>
  any_مقدار_t (X&&) {}
};

ساكن فراغ test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_نتیجه_t<any_مقدار_t, error_کد_t>, hb_نتیجه_err_t<ثابت حرف*>>::value, "");

  hb_نتیجه_t<any_مقدار_t, error_کد_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_نتیجه_t<any_مقدار_t, error_کد_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

بنية move_only_err_t
{
  صحيح کد;

  صریح move_only_err_t (صحيح c) : کد (c) {}
  move_only_err_t (ثابت move_only_err_t&) = حذف;
  move_only_err_t (move_only_err_t&& o) : کد (o.کد) { o.کد = -1; }
  move_only_err_t& عملگر = (ثابت move_only_err_t&) = حذف;
  move_only_err_t& عملگر = (move_only_err_t&& o) { کد = o.کد; o.کد = -1; أرجع *هذا; }
  ~move_only_err_t () = افتراضي;

  منطقي عملگر == (ثابت move_only_err_t& o) ثابت { أرجع کد == o.کد; }
};

بنية move_only_val_t
{
  صحيح val;

  صریح move_only_val_t (صحيح v) : val (v) {}
  move_only_val_t (ثابت move_only_val_t&) = حذف;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& عملگر = (ثابت move_only_val_t&) = حذف;
  move_only_val_t& عملگر = (move_only_val_t&& o) { val = o.val; o.val = -1; أرجع *هذا; }
  ~move_only_val_t () = افتراضي;

  منطقي عملگر == (ثابت move_only_val_t& o) ثابت { أرجع val == o.val; }
};

ساكن فراغ test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_نتیجه_t<صحيح, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().کد == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.کد == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_نتیجه_t<صحيح, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().کد == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_نتیجه_t<فراغ, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().کد == 789);

    hb_نتیجه_t<فراغ, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().کد == 999);
  }

  // Test value_or with move-only value types
  {
    hb_نتیجه_t<move_only_val_t, error_کد_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).مقدار_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_نتیجه_t<move_only_val_t, error_کد_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).مقدار_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

بنية copy_move_counter_t
{
  ساكن صحيح copy_شمار;
  ساكن صحيح move_شمار;

  ساكن فراغ reset ()
  {
    copy_شمار = 0;
    move_شمار = 0;
  }

  صحيح val;

  منطقي درون_error() ثابت {
    أرجع val == -1;
  }

  صریح copy_move_counter_t (صحيح v) : val (v) {}
  copy_move_counter_t (ثابت copy_move_counter_t& o) : val (o.val) { copy_شمار++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_شمار++; }
  ~copy_move_counter_t () = افتراضي;
};

صحيح copy_move_counter_t::copy_شمار = 0;
صحيح copy_move_counter_t::move_شمار = 0;

ساكن نتیجه<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t بیرون (42);
  أرجع Ok (std::move (بیرون));
}

ساكن نتیجه<copy_move_counter_t> return_tracked_from_local_with_برچسب ()
{
  copy_move_counter_t بیرون (42);
  أرجع Ok(بیرون);
}

ساكن hb_نتیجه_t<صحيح, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t بیرون (42);
  أرجع Err (std::move (بیرون));
}

ساكن hb_نتیجه_t<صحيح, copy_move_counter_t> return_tracked_error_from_local_with_برچسب ()
{
  copy_move_counter_t بیرون (42);
  أرجع Err(بیرون);
}

ساكن نتیجه<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t بیرون (42);
  أرجع Ok (std::move (بیرون));
}

ساكن فراغ test_return_حرکتها_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    نتیجه<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_شمار == 0);
    hb_always_assert (copy_move_counter_t::move_شمار >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    نتیجه<copy_move_counter_t> r = return_tracked_from_local_with_برچسب ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_شمار == 0);
    hb_always_assert (copy_move_counter_t::move_شمار >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_نتیجه_t<صحيح, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_شمار == 0);
    hb_always_assert (copy_move_counter_t::move_شمار >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_نتیجه_t<صحيح, copy_move_counter_t> r = return_tracked_error_from_local_with_برچسب ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_شمار == 0);
    hb_always_assert (copy_move_counter_t::move_شمار >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    نتیجه<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

بنية درون_error_t {
  منطقي successful;

  درون_error_t(منطقي s) : successful(s) {}

  منطقي درون_error() ثابت {
    أرجع !successful;
  }
};

ساكن فراغ test_from () {
  درون_error_t has_error(خطأ);
  درون_error_t no_error(درست);
  نتیجه<فراغ> r = نتیجه<فراغ>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = نتیجه<فراغ>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

بنية default_constructible_t
{
  صحيح val;

  default_constructible_t () : val (123) {}
  صریح default_constructible_t (صحيح v) : val (v) {}
};

بنية move_only_default_t
{
  صحيح val;
  move_only_default_t () : val (77) {}
  صریح move_only_default_t (صحيح v) : val (v) {}
  move_only_default_t (ثابت move_only_default_t&) = حذف;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& عملگر = (ثابت move_only_default_t&) = حذف;
  move_only_default_t& عملگر = (move_only_default_t&& o) { val = o.val; o.val = -1; أرجع *هذا; }
  ~move_only_default_t () = افتراضي;
};

ساكن فراغ test_مقدار_or_default ()
{
  // Test const &
  {
    ثابت نتیجه<صحيح> r_ok = Ok(42);
    hb_always_assert (r_ok.مقدار_or_default () == 42);

    ثابت نتیجه<صحيح> r_err = Err(ERR_A);
    hb_always_assert (r_err.مقدار_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    نتیجه<صحيح> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).مقدار_or_default () == 42);

    نتیجه<صحيح> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).مقدار_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    نتیجه<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.مقدار_or_default ().val == 456);

    نتیجه<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.مقدار_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).مقدار_or_default ().val == 456);
    hb_always_assert (std::move (r_err).مقدار_or_default ().val == 123);
  }

  // Test pointers
  {
    صحيح x = 10;
    نتیجه<صحيح*> r_ok = &x;
    hb_always_assert (r_ok.مقدار_or_default () == &x);

    نتیجه<صحيح*> r_err = ERR_A;
    hb_always_assert (r_err.مقدار_or_default () == اشاره_تهی);
  }

  // Test move-only types
  {
    نتیجه<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).مقدار_or_default ().val == 88);

    نتیجه<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).مقدار_or_default ().val == 77);
  }
}

ساكن فراغ test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    نتیجه<copy_move_counter_t> r_ok = نتیجه<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_شمار == 0);
    hb_always_assert (copy_move_counter_t::move_شمار == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    نتیجه<copy_move_counter_t> r_fail = نتیجه<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_شمار == 0);
    hb_always_assert (copy_move_counter_t::move_شمار == 0);
  }
}

صحيح اصلی ()
{
  test_ok_basic ();
  test_err_basic ();
  test_explicit_ok_err ();
  test_اشارهگرها ();
  test_void ();
  test_resource_cleanup ();
  test_try_macro ();
  test_same_or_convertible_types ();
  test_error_interception ();
  test_move_only_types ();
  test_return_حرکتها_from_local ();
  test_from();
  test_مقدار_or_default ();
  test_from_move ();

  أرجع 0;
}
