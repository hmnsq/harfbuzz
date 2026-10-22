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
ساكن صحيح جيئرو_instances = 0;

تعداد error_ڪوڊ_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

سانچو <قسم_نالو T>
استعمال نتيجو = hb_نتيجو_t<T, error_ڪوڊ_t>;

بنية resource_t
{
  صحيح id;

  واضح resource_t (صحيح id_) : id (id_) { جيئرو_instances++; }
  resource_t (ثابت resource_t& o) : id (o.id) { جيئرو_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; جيئرو_instances++; }
  ~resource_t () { جيئرو_instances--; }

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

ساكن نتيجو<صحيح> returns_ok (صحيح v)
{
  أرجع Ok(v);
}

ساكن نتيجو<صحيح> returns_err (error_ڪوڊ_t e)
{
  أرجع Err(e);
}

ساكن نتيجو<صحيح> try_callee (منطقي fail, صحيح v, error_ڪوڊ_t e = ERR_A)
{
  إذا (fail) أرجع Err(e);
  أرجع Ok(v);
}

ساكن نتيجو<فراغ> try_callee_void (منطقي fail,  error_ڪوڊ_t e)
{
  إذا (fail) أرجع e;
  أرجع Ok();
}

ساكن نتيجو<صحيح> try_caller (منطقي fail1, منطقي fail2)
{
  TRY_سونپيو (صحيح اشاريو_2, try_callee (fail1, 10, ERR_A));
  TRY_سونپيو (صحيح index2, try_callee (fail2, 20, ERR_B));
  أرجع Ok(اشاريو_2 + index2);
}

ساكن نتيجو<فراغ> try_caller_void (منطقي fail)
{
  TRY (try_callee_void (fail, ERR_C));
  أرجع Ok();
}

ساكن نتيجو<عائم> try_caller_type_change (منطقي fail)
{
  TRY_سونپيو (صحيح اشاريو_2, try_callee (fail, 100, ERR_A));
  أرجع Ok((عائم) اشاريو_2 * 1.5f);
}

ساكن نتيجو<صحيح> try_سونپيو_existing (منطقي fail)
{
  صحيح val = 1;
  TRY_سونپيو (val, try_callee (fail, 42, ERR_A));
  أرجع Ok(val);
}

ساكن فراغ test_ok_basic ()
{
  نتيجو<صحيح> r = returns_ok (42);
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
  نتيجو<صحيح> r = returns_err (ERR_A);
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
  نتيجو<بنانشان> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  نتيجو<بنانشان> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

ساكن فراغ test_اشارا ()
{
  صحيح x = 123;
  نتيجو<صحيح*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

ساكن فراغ test_void ()
{
  نتيجو<فراغ> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((منطقي) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  نتيجو<فراغ> r2 = Err(ERR_A);
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
  hb_always_assert (جيئرو_instances == 0);

  {
    نتيجو<resource_t> r = resource_t (1);
    hb_always_assert (جيئرو_instances == 1);
    hb_always_assert (r.value ().id == 1);
  }
  hb_always_assert (جيئرو_instances == 0);

  {
    نتيجو<resource_t> r1 = resource_t (2);
    hb_always_assert (جيئرو_instances == 1);
    نتيجو<resource_t> r2 = r1;
    hb_always_assert (جيئرو_instances == 2);
    نتيجو<resource_t> r3 = std::move (r1);
    hb_always_assert (جيئرو_instances == 3);
  }
  hb_always_assert (جيئرو_instances == 0);

  {
    نتيجو<resource_t> r = resource_t (3);
    hb_always_assert (جيئرو_instances == 1);
    r = Err(ERR_A);
    hb_always_assert (جيئرو_instances == 0);
    hb_always_assert (r.is_err ());
  }
  hb_always_assert (جيئرو_instances == 0);
}

ساكن فراغ test_try_macro ()
{
  // Test success path
  نتيجو<صحيح> r_success = try_caller (خطأ, خطأ);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  نتيجو<صحيح> r_fail1 = try_caller (سچ, خطأ);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  نتيجو<صحيح> r_fail2 = try_caller (خطأ, سچ);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  نتيجو<فراغ> v_ok = try_caller_void (خطأ);
  hb_always_assert (v_ok.is_ok ());

  نتيجو<فراغ> v_err = try_caller_void (سچ);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  نتيجو<عائم> f_ok = try_caller_type_change (خطأ);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  نتيجو<عائم> f_err = try_caller_type_change (سچ);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  نتيجو<صحيح> a_ok = try_سونپيو_existing (خطأ);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  نتيجو<صحيح> a_err = try_سونپيو_existing (سچ);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

سانچو <قسم_نالو T, قسم_نالو U, قسم_نالو = فراغ>
بنية is_equality_comparable : std::false_type {};

سانچو <قسم_نالو T, قسم_نالو U>
بنية is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

ساكن فراغ test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<نتيجو<صحيح>, صحيح>::value, "");
  static_assert (!is_equality_comparable<نتيجو<صحيح>, int64_t>::value, "");
  static_assert (!is_equality_comparable<صحيح, نتيجو<صحيح>>::value, "");
  static_assert (!is_equality_comparable<نتيجو<صحيح>, error_ڪوڊ_t>::value, "");
  static_assert (!is_equality_comparable<error_ڪوڊ_t, نتيجو<صحيح>>::value, "");
  static_assert (!is_equality_comparable<نتيجو<فراغ>, error_ڪوڊ_t>::value, "");
  static_assert (is_equality_comparable<نتيجو<صحيح>, نتيجو<صحيح>>::value, "");
  static_assert (is_equality_comparable<نتيجو<صحيح>, hb_نتيجو_ok_t<صحيح>>::value, "");
  static_assert (is_equality_comparable<نتيجو<صحيح>, hb_نتيجو_err_t<error_ڪوڊ_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_نتيجو_t<int64_t, int32_t>, صحيح>::value, "");
  static_assert (!std::is_constructible<hb_نتيجو_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_نتيجو_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_نتيجو_t<int64_t, int32_t>, hb_نتيجو_ok_t<صحيح>>::value, "");
  static_assert (std::is_constructible<hb_نتيجو_t<int64_t, int32_t>, hb_نتيجو_err_t<صحيح>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_نتيجو_t<صحيح, صحيح>, صحيح>::value, "");
  static_assert (std::is_constructible<hb_نتيجو_t<صحيح, صحيح>, hb_نتيجو_ok_t<صحيح>>::value, "");
  static_assert (std::is_constructible<hb_نتيجو_t<صحيح, صحيح>, hb_نتيجو_err_t<صحيح>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_نتيجو_t<صحيح, error_ڪوڊ_t>, صحيح>::value, "");
  static_assert (!std::is_constructible<hb_نتيجو_t<صحيح, error_ڪوڊ_t>, error_ڪوڊ_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_نتيجو_t<resource_t, error_ڪوڊ_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_نتيجو_t<resource_t, error_ڪوڊ_t>, error_ڪوڊ_t>::value, "");

  hb_نتيجو_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_نتيجو_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_نتيجو_t<صحيح, صحيح> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_نتيجو_t<صحيح, صحيح> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

بنية any_قدر_t
{
  سانچو <قسم_نالو X>
  any_قدر_t (X&&) {}
};

ساكن فراغ test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_نتيجو_t<any_قدر_t, error_ڪوڊ_t>, hb_نتيجو_err_t<ثابت حرف*>>::value, "");

  hb_نتيجو_t<any_قدر_t, error_ڪوڊ_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_نتيجو_t<any_قدر_t, error_ڪوڊ_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

بنية move_only_err_t
{
  صحيح ڪوڊ;

  واضح move_only_err_t (صحيح c) : ڪوڊ (c) {}
  move_only_err_t (ثابت move_only_err_t&) = ميٽيو;
  move_only_err_t (move_only_err_t&& o) : ڪوڊ (o.ڪوڊ) { o.ڪوڊ = -1; }
  move_only_err_t& عامل = (ثابت move_only_err_t&) = ميٽيو;
  move_only_err_t& عامل = (move_only_err_t&& o) { ڪوڊ = o.ڪوڊ; o.ڪوڊ = -1; أرجع *هذا; }
  ~move_only_err_t () = افتراضي;

  منطقي عامل == (ثابت move_only_err_t& o) ثابت { أرجع ڪوڊ == o.ڪوڊ; }
};

بنية move_only_val_t
{
  صحيح val;

  واضح move_only_val_t (صحيح v) : val (v) {}
  move_only_val_t (ثابت move_only_val_t&) = ميٽيو;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& عامل = (ثابت move_only_val_t&) = ميٽيو;
  move_only_val_t& عامل = (move_only_val_t&& o) { val = o.val; o.val = -1; أرجع *هذا; }
  ~move_only_val_t () = افتراضي;

  منطقي عامل == (ثابت move_only_val_t& o) ثابت { أرجع val == o.val; }
};

ساكن فراغ test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_نتيجو_t<صحيح, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().ڪوڊ == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.ڪوڊ == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_نتيجو_t<صحيح, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().ڪوڊ == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_نتيجو_t<فراغ, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().ڪوڊ == 789);

    hb_نتيجو_t<فراغ, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().ڪوڊ == 999);
  }

  // Test value_or with move-only value types
  {
    hb_نتيجو_t<move_only_val_t, error_ڪوڊ_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).قدر_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_نتيجو_t<move_only_val_t, error_ڪوڊ_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).قدر_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

بنية copy_move_counter_t
{
  ساكن صحيح copy_ڳڻپ;
  ساكن صحيح move_ڳڻپ;

  ساكن فراغ reset ()
  {
    copy_ڳڻپ = 0;
    move_ڳڻپ = 0;
  }

  صحيح val;

  منطقي اندريون_error() ثابت {
    أرجع val == -1;
  }

  واضح copy_move_counter_t (صحيح v) : val (v) {}
  copy_move_counter_t (ثابت copy_move_counter_t& o) : val (o.val) { copy_ڳڻپ++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_ڳڻپ++; }
  ~copy_move_counter_t () = افتراضي;
};

صحيح copy_move_counter_t::copy_ڳڻپ = 0;
صحيح copy_move_counter_t::move_ڳڻپ = 0;

ساكن نتيجو<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t ٻاهريون (42);
  أرجع Ok (std::move (ٻاهريون));
}

ساكن نتيجو<copy_move_counter_t> return_tracked_from_local_with_ٽيگ ()
{
  copy_move_counter_t ٻاهريون (42);
  أرجع Ok(ٻاهريون);
}

ساكن hb_نتيجو_t<صحيح, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t ٻاهريون (42);
  أرجع Err (std::move (ٻاهريون));
}

ساكن hb_نتيجو_t<صحيح, copy_move_counter_t> return_tracked_error_from_local_with_ٽيگ ()
{
  copy_move_counter_t ٻاهريون (42);
  أرجع Err(ٻاهريون);
}

ساكن نتيجو<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t ٻاهريون (42);
  أرجع Ok (std::move (ٻاهريون));
}

ساكن فراغ test_return_چالون_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    نتيجو<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ڳڻپ == 0);
    hb_always_assert (copy_move_counter_t::move_ڳڻپ >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    نتيجو<copy_move_counter_t> r = return_tracked_from_local_with_ٽيگ ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ڳڻپ == 0);
    hb_always_assert (copy_move_counter_t::move_ڳڻپ >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_نتيجو_t<صحيح, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ڳڻپ == 0);
    hb_always_assert (copy_move_counter_t::move_ڳڻپ >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_نتيجو_t<صحيح, copy_move_counter_t> r = return_tracked_error_from_local_with_ٽيگ ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ڳڻپ == 0);
    hb_always_assert (copy_move_counter_t::move_ڳڻپ >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    نتيجو<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

بنية اندريون_error_t {
  منطقي successful;

  اندريون_error_t(منطقي s) : successful(s) {}

  منطقي اندريون_error() ثابت {
    أرجع !successful;
  }
};

ساكن فراغ test_from () {
  اندريون_error_t has_error(خطأ);
  اندريون_error_t no_error(سچ);
  نتيجو<فراغ> r = نتيجو<فراغ>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = نتيجو<فراغ>::from(no_error, ERR_B);
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
  move_only_default_t (ثابت move_only_default_t&) = ميٽيو;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& عامل = (ثابت move_only_default_t&) = ميٽيو;
  move_only_default_t& عامل = (move_only_default_t&& o) { val = o.val; o.val = -1; أرجع *هذا; }
  ~move_only_default_t () = افتراضي;
};

ساكن فراغ test_قدر_or_default ()
{
  // Test const &
  {
    ثابت نتيجو<صحيح> r_ok = Ok(42);
    hb_always_assert (r_ok.قدر_or_default () == 42);

    ثابت نتيجو<صحيح> r_err = Err(ERR_A);
    hb_always_assert (r_err.قدر_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    نتيجو<صحيح> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).قدر_or_default () == 42);

    نتيجو<صحيح> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).قدر_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    نتيجو<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.قدر_or_default ().val == 456);

    نتيجو<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.قدر_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).قدر_or_default ().val == 456);
    hb_always_assert (std::move (r_err).قدر_or_default ().val == 123);
  }

  // Test pointers
  {
    صحيح x = 10;
    نتيجو<صحيح*> r_ok = &x;
    hb_always_assert (r_ok.قدر_or_default () == &x);

    نتيجو<صحيح*> r_err = ERR_A;
    hb_always_assert (r_err.قدر_or_default () == خالي_اشارو);
  }

  // Test move-only types
  {
    نتيجو<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).قدر_or_default ().val == 88);

    نتيجو<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).قدر_or_default ().val == 77);
  }
}

ساكن فراغ test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    نتيجو<copy_move_counter_t> r_ok = نتيجو<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_ڳڻپ == 0);
    hb_always_assert (copy_move_counter_t::move_ڳڻپ == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    نتيجو<copy_move_counter_t> r_fail = نتيجو<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_ڳڻپ == 0);
    hb_always_assert (copy_move_counter_t::move_ڳڻپ == 0);
  }
}

صحيح مکيه ()
{
  test_ok_basic ();
  test_err_basic ();
  test_explicit_ok_err ();
  test_اشارا ();
  test_void ();
  test_resource_cleanup ();
  test_try_macro ();
  test_same_or_convertible_types ();
  test_error_interception ();
  test_move_only_types ();
  test_return_چالون_from_local ();
  test_from();
  test_قدر_or_default ();
  test_from_move ();

  أرجع 0;
}
