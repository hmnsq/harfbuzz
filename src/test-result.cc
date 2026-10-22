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
ساكن صحيح live_instances = 0;

تعداد error_code_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

template <typename T>
using result = hb_result_t<T, error_code_t>;

بنية resource_t
{
  صحيح id;

  explicit resource_t (صحيح id_) : id (id_) { live_instances++; }
  resource_t (ثابت resource_t& o) : id (o.id) { live_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; live_instances++; }
  ~resource_t () { live_instances--; }

  resource_t& operator = (ثابت resource_t& o)
  {
    id = o.id;
    ܦܢܐ *هذا;
  }
  resource_t& operator = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    ܦܢܐ *هذا;
  }

  منطقي operator == (ثابت resource_t& o) ثابت { ܦܢܐ id == o.id; }
};

ساكن result<صحيح> returns_ok (صحيح v)
{
  ܦܢܐ Ok(v);
}

ساكن result<صحيح> returns_err (error_code_t e)
{
  ܦܢܐ Err(e);
}

ساكن result<صحيح> try_callee (منطقي fail, صحيح v, error_code_t e = ERR_A)
{
  ܐܢ (fail) ܦܢܐ Err(e);
  ܦܢܐ Ok(v);
}

ساكن result<فراغ> try_callee_void (منطقي fail,  error_code_t e)
{
  ܐܢ (fail) ܦܢܐ e;
  ܦܢܐ Ok();
}

ساكن result<صحيح> try_caller (منطقي fail1, منطقي fail2)
{
  TRY_ASSIGN (صحيح index, try_callee (fail1, 10, ERR_A));
  TRY_ASSIGN (صحيح index2, try_callee (fail2, 20, ERR_B));
  ܦܢܐ Ok(index + index2);
}

ساكن result<فراغ> try_caller_void (منطقي fail)
{
  TRY (try_callee_void (fail, ERR_C));
  ܦܢܐ Ok();
}

ساكن result<عائم> try_caller_type_change (منطقي fail)
{
  TRY_ASSIGN (صحيح index, try_callee (fail, 100, ERR_A));
  ܦܢܐ Ok((عائم) index * 1.5f);
}

ساكن result<صحيح> try_assign_existing (منطقي fail)
{
  صحيح val = 1;
  TRY_ASSIGN (val, try_callee (fail, 42, ERR_A));
  ܦܢܐ Ok(val);
}

ساكن فراغ test_ok_basic ()
{
  result<صحيح> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((منطقي) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.value_or (0) == 42);
  hb_always_assert (r.value_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

ساكن فراغ test_err_basic ()
{
  result<صحيح> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.value_or (99) == 99);
  hb_always_assert (r.value_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

ساكن فراغ test_explicit_ok_err ()
{
  result<unsigned> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  result<unsigned> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

ساكن فراغ test_pointers ()
{
  صحيح x = 123;
  result<صحيح*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

ساكن فراغ test_void ()
{
  result<فراغ> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((منطقي) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  result<فراغ> r2 = Err(ERR_A);
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
  hb_always_assert (live_instances == 0);

  {
    result<resource_t> r = resource_t (1);
    hb_always_assert (live_instances == 1);
    hb_always_assert (r.value ().id == 1);
  }
  hb_always_assert (live_instances == 0);

  {
    result<resource_t> r1 = resource_t (2);
    hb_always_assert (live_instances == 1);
    result<resource_t> r2 = r1;
    hb_always_assert (live_instances == 2);
    result<resource_t> r3 = std::move (r1);
    hb_always_assert (live_instances == 3);
  }
  hb_always_assert (live_instances == 0);

  {
    result<resource_t> r = resource_t (3);
    hb_always_assert (live_instances == 1);
    r = Err(ERR_A);
    hb_always_assert (live_instances == 0);
    hb_always_assert (r.is_err ());
  }
  hb_always_assert (live_instances == 0);
}

ساكن فراغ test_try_macro ()
{
  // Test success path
  result<صحيح> r_success = try_caller (ܕܓܠܐ, ܕܓܠܐ);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  result<صحيح> r_fail1 = try_caller (ܫܪܪܐ, ܕܓܠܐ);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  result<صحيح> r_fail2 = try_caller (ܕܓܠܐ, ܫܪܪܐ);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  result<فراغ> v_ok = try_caller_void (ܕܓܠܐ);
  hb_always_assert (v_ok.is_ok ());

  result<فراغ> v_err = try_caller_void (ܫܪܪܐ);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  result<عائم> f_ok = try_caller_type_change (ܕܓܠܐ);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  result<عائم> f_err = try_caller_type_change (ܫܪܪܐ);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  result<صحيح> a_ok = try_assign_existing (ܕܓܠܐ);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  result<صحيح> a_err = try_assign_existing (ܫܪܪܐ);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

template <typename T, typename U, typename = فراغ>
بنية is_equality_comparable : std::false_type {};

template <typename T, typename U>
بنية is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

ساكن فراغ test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<result<صحيح>, صحيح>::value, "");
  static_assert (!is_equality_comparable<result<صحيح>, int64_t>::value, "");
  static_assert (!is_equality_comparable<صحيح, result<صحيح>>::value, "");
  static_assert (!is_equality_comparable<result<صحيح>, error_code_t>::value, "");
  static_assert (!is_equality_comparable<error_code_t, result<صحيح>>::value, "");
  static_assert (!is_equality_comparable<result<فراغ>, error_code_t>::value, "");
  static_assert (is_equality_comparable<result<صحيح>, result<صحيح>>::value, "");
  static_assert (is_equality_comparable<result<صحيح>, hb_result_ok_t<صحيح>>::value, "");
  static_assert (is_equality_comparable<result<صحيح>, hb_result_err_t<error_code_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, صحيح>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_ok_t<صحيح>>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_err_t<صحيح>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<صحيح, صحيح>, صحيح>::value, "");
  static_assert (std::is_constructible<hb_result_t<صحيح, صحيح>, hb_result_ok_t<صحيح>>::value, "");
  static_assert (std::is_constructible<hb_result_t<صحيح, صحيح>, hb_result_err_t<صحيح>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_result_t<صحيح, error_code_t>, صحيح>::value, "");
  static_assert (!std::is_constructible<hb_result_t<صحيح, error_code_t>, error_code_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, error_code_t>::value, "");

  hb_result_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_result_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_result_t<صحيح, صحيح> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_result_t<صحيح, صحيح> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

بنية any_value_t
{
  template <typename X>
  any_value_t (X&&) {}
};

ساكن فراغ test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_result_t<any_value_t, error_code_t>, hb_result_err_t<ثابت حرف*>>::value, "");

  hb_result_t<any_value_t, error_code_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_result_t<any_value_t, error_code_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

بنية move_only_err_t
{
  صحيح code;

  explicit move_only_err_t (صحيح c) : code (c) {}
  move_only_err_t (ثابت move_only_err_t&) = delete;
  move_only_err_t (move_only_err_t&& o) : code (o.code) { o.code = -1; }
  move_only_err_t& operator = (ثابت move_only_err_t&) = delete;
  move_only_err_t& operator = (move_only_err_t&& o) { code = o.code; o.code = -1; ܦܢܐ *هذا; }
  ~move_only_err_t () = افتراضي;

  منطقي operator == (ثابت move_only_err_t& o) ثابت { ܦܢܐ code == o.code; }
};

بنية move_only_val_t
{
  صحيح val;

  explicit move_only_val_t (صحيح v) : val (v) {}
  move_only_val_t (ثابت move_only_val_t&) = delete;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& operator = (ثابت move_only_val_t&) = delete;
  move_only_val_t& operator = (move_only_val_t&& o) { val = o.val; o.val = -1; ܦܢܐ *هذا; }
  ~move_only_val_t () = افتراضي;

  منطقي operator == (ثابت move_only_val_t& o) ثابت { ܦܢܐ val == o.val; }
};

ساكن فراغ test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_result_t<صحيح, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.code == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_result_t<صحيح, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_result_t<فراغ, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().code == 789);

    hb_result_t<فراغ, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().code == 999);
  }

  // Test value_or with move-only value types
  {
    hb_result_t<move_only_val_t, error_code_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).value_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_result_t<move_only_val_t, error_code_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).value_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

بنية copy_move_counter_t
{
  ساكن صحيح copy_count;
  ساكن صحيح move_count;

  ساكن فراغ reset ()
  {
    copy_count = 0;
    move_count = 0;
  }

  صحيح val;

  منطقي in_error() ثابت {
    ܦܢܐ val == -1;
  }

  explicit copy_move_counter_t (صحيح v) : val (v) {}
  copy_move_counter_t (ثابت copy_move_counter_t& o) : val (o.val) { copy_count++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_count++; }
  ~copy_move_counter_t () = افتراضي;
};

صحيح copy_move_counter_t::copy_count = 0;
صحيح copy_move_counter_t::move_count = 0;

ساكن result<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t out (42);
  ܦܢܐ Ok (std::move (out));
}

ساكن result<copy_move_counter_t> return_tracked_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  ܦܢܐ Ok(out);
}

ساكن hb_result_t<صحيح, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t out (42);
  ܦܢܐ Err (std::move (out));
}

ساكن hb_result_t<صحيح, copy_move_counter_t> return_tracked_error_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  ܦܢܐ Err(out);
}

ساكن result<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t out (42);
  ܦܢܐ Ok (std::move (out));
}

ساكن فراغ test_return_moves_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local_with_tag ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<صحيح, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<صحيح, copy_move_counter_t> r = return_tracked_error_from_local_with_tag ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    result<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

بنية in_error_t {
  منطقي successful;

  in_error_t(منطقي s) : successful(s) {}

  منطقي in_error() ثابت {
    ܦܢܐ !successful;
  }
};

ساكن فراغ test_from () {
  in_error_t has_error(ܕܓܠܐ);
  in_error_t no_error(ܫܪܪܐ);
  result<فراغ> r = result<فراغ>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = result<فراغ>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

بنية default_constructible_t
{
  صحيح val;

  default_constructible_t () : val (123) {}
  explicit default_constructible_t (صحيح v) : val (v) {}
};

بنية move_only_default_t
{
  صحيح val;
  move_only_default_t () : val (77) {}
  explicit move_only_default_t (صحيح v) : val (v) {}
  move_only_default_t (ثابت move_only_default_t&) = delete;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& operator = (ثابت move_only_default_t&) = delete;
  move_only_default_t& operator = (move_only_default_t&& o) { val = o.val; o.val = -1; ܦܢܐ *هذا; }
  ~move_only_default_t () = افتراضي;
};

ساكن فراغ test_value_or_default ()
{
  // Test const &
  {
    ثابت result<صحيح> r_ok = Ok(42);
    hb_always_assert (r_ok.value_or_default () == 42);

    ثابت result<صحيح> r_err = Err(ERR_A);
    hb_always_assert (r_err.value_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    result<صحيح> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).value_or_default () == 42);

    result<صحيح> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).value_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    result<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.value_or_default ().val == 456);

    result<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.value_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).value_or_default ().val == 456);
    hb_always_assert (std::move (r_err).value_or_default ().val == 123);
  }

  // Test pointers
  {
    صحيح x = 10;
    result<صحيح*> r_ok = &x;
    hb_always_assert (r_ok.value_or_default () == &x);

    result<صحيح*> r_err = ERR_A;
    hb_always_assert (r_err.value_or_default () == nullptr);
  }

  // Test move-only types
  {
    result<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).value_or_default ().val == 88);

    result<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).value_or_default ().val == 77);
  }
}

ساكن فراغ test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    result<copy_move_counter_t> r_ok = result<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    result<copy_move_counter_t> r_fail = result<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_count == 0);
    hb_always_assert (copy_move_counter_t::move_count == 0);
  }
}

صحيح main ()
{
  test_ok_basic ();
  test_err_basic ();
  test_explicit_ok_err ();
  test_pointers ();
  test_void ();
  test_resource_cleanup ();
  test_try_macro ();
  test_same_or_convertible_types ();
  test_error_interception ();
  test_move_only_types ();
  test_return_moves_from_local ();
  test_from();
  test_value_or_default ();
  test_from_move ();

  ܦܢܐ 0;
}
