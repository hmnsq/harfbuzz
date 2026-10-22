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

#അന്തര്ഭാവ "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
സ്ഥിതം പൂർണ്ണസംഖ്യ live_instances = 0;

എണ്ണൽ error_code_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

template <typename T>
using result = hb_result_t<T, error_code_t>;

ഘടന resource_t
{
  പൂർണ്ണസംഖ്യ id;

  explicit resource_t (പൂർണ്ണസംഖ്യ id_) : id (id_) { live_instances++; }
  resource_t (സ്ഥിരം resource_t& o) : id (o.id) { live_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; live_instances++; }
  ~resource_t () { live_instances--; }

  resource_t& operator = (സ്ഥിരം resource_t& o)
  {
    id = o.id;
    തിരികെ *ഇത്;
  }
  resource_t& operator = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    തിരികെ *ഇത്;
  }

  തർക്കം operator == (സ്ഥിരം resource_t& o) സ്ഥിരം { തിരികെ id == o.id; }
};

സ്ഥിതം result<പൂർണ്ണസംഖ്യ> returns_ok (പൂർണ്ണസംഖ്യ v)
{
  തിരികെ Ok(v);
}

സ്ഥിതം result<പൂർണ്ണസംഖ്യ> returns_err (error_code_t e)
{
  തിരികെ Err(e);
}

സ്ഥിതം result<പൂർണ്ണസംഖ്യ> try_callee (തർക്കം fail, പൂർണ്ണസംഖ്യ v, error_code_t e = ERR_A)
{
  എങ്കിൽ (fail) തിരികെ Err(e);
  തിരികെ Ok(v);
}

സ്ഥിതം result<ശൂന്യം> try_callee_void (തർക്കം fail,  error_code_t e)
{
  എങ്കിൽ (fail) തിരികെ e;
  തിരികെ Ok();
}

സ്ഥിതം result<പൂർണ്ണസംഖ്യ> try_caller (തർക്കം fail1, തർക്കം fail2)
{
  TRY_ASSIGN (പൂർണ്ണസംഖ്യ index, try_callee (fail1, 10, ERR_A));
  TRY_ASSIGN (പൂർണ്ണസംഖ്യ index2, try_callee (fail2, 20, ERR_B));
  തിരികെ Ok(index + index2);
}

സ്ഥിതം result<ശൂന്യം> try_caller_void (തർക്കം fail)
{
  TRY (try_callee_void (fail, ERR_C));
  തിരികെ Ok();
}

സ്ഥിതം result<ദശാംശം> try_caller_type_change (തർക്കം fail)
{
  TRY_ASSIGN (പൂർണ്ണസംഖ്യ index, try_callee (fail, 100, ERR_A));
  തിരികെ Ok((ദശാംശം) index * 1.5f);
}

സ്ഥിതം result<പൂർണ്ണസംഖ്യ> try_assign_existing (തർക്കം fail)
{
  പൂർണ്ണസംഖ്യ val = 1;
  TRY_ASSIGN (val, try_callee (fail, 42, ERR_A));
  തിരികെ Ok(val);
}

സ്ഥിതം ശൂന്യം test_ok_basic ()
{
  result<പൂർണ്ണസംഖ്യ> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((തർക്കം) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.മൂല്യം_or (0) == 42);
  hb_always_assert (r.മൂല്യം_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

സ്ഥിതം ശൂന്യം test_err_basic ()
{
  result<പൂർണ്ണസംഖ്യ> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.മൂല്യം_or (99) == 99);
  hb_always_assert (r.മൂല്യം_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

സ്ഥിതം ശൂന്യം test_explicit_ok_err ()
{
  result<unsigned> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  result<unsigned> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

സ്ഥിതം ശൂന്യം test_pointers ()
{
  പൂർണ്ണസംഖ്യ x = 123;
  result<പൂർണ്ണസംഖ്യ*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

സ്ഥിതം ശൂന്യം test_void ()
{
  result<ശൂന്യം> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((തർക്കം) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  result<ശൂന്യം> r2 = Err(ERR_A);
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

സ്ഥിതം ശൂന്യം test_resource_cleanup ()
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

സ്ഥിതം ശൂന്യം test_try_macro ()
{
  // Test success path
  result<പൂർണ്ണസംഖ്യ> r_success = try_caller (അസത്യം, അസത്യം);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  result<പൂർണ്ണസംഖ്യ> r_fail1 = try_caller (സത്യം, അസത്യം);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  result<പൂർണ്ണസംഖ്യ> r_fail2 = try_caller (അസത്യം, സത്യം);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  result<ശൂന്യം> v_ok = try_caller_void (അസത്യം);
  hb_always_assert (v_ok.is_ok ());

  result<ശൂന്യം> v_err = try_caller_void (സത്യം);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  result<ദശാംശം> f_ok = try_caller_type_change (അസത്യം);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  result<ദശാംശം> f_err = try_caller_type_change (സത്യം);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  result<പൂർണ്ണസംഖ്യ> a_ok = try_assign_existing (അസത്യം);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  result<പൂർണ്ണസംഖ്യ> a_err = try_assign_existing (സത്യം);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

template <typename T, typename U, typename = ശൂന്യം>
ഘടന is_equality_comparable : std::false_type {};

template <typename T, typename U>
ഘടന is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

സ്ഥിതം ശൂന്യം test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<result<പൂർണ്ണസംഖ്യ>, പൂർണ്ണസംഖ്യ>::value, "");
  static_assert (!is_equality_comparable<result<പൂർണ്ണസംഖ്യ>, int64_t>::value, "");
  static_assert (!is_equality_comparable<പൂർണ്ണസംഖ്യ, result<പൂർണ്ണസംഖ്യ>>::value, "");
  static_assert (!is_equality_comparable<result<പൂർണ്ണസംഖ്യ>, error_code_t>::value, "");
  static_assert (!is_equality_comparable<error_code_t, result<പൂർണ്ണസംഖ്യ>>::value, "");
  static_assert (!is_equality_comparable<result<ശൂന്യം>, error_code_t>::value, "");
  static_assert (is_equality_comparable<result<പൂർണ്ണസംഖ്യ>, result<പൂർണ്ണസംഖ്യ>>::value, "");
  static_assert (is_equality_comparable<result<പൂർണ്ണസംഖ്യ>, hb_result_ok_t<പൂർണ്ണസംഖ്യ>>::value, "");
  static_assert (is_equality_comparable<result<പൂർണ്ണസംഖ്യ>, hb_result_err_t<error_code_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, പൂർണ്ണസംഖ്യ>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_ok_t<പൂർണ്ണസംഖ്യ>>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_err_t<പൂർണ്ണസംഖ്യ>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ>, പൂർണ്ണസംഖ്യ>::value, "");
  static_assert (std::is_constructible<hb_result_t<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ>, hb_result_ok_t<പൂർണ്ണസംഖ്യ>>::value, "");
  static_assert (std::is_constructible<hb_result_t<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ>, hb_result_err_t<പൂർണ്ണസംഖ്യ>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_result_t<പൂർണ്ണസംഖ്യ, error_code_t>, പൂർണ്ണസംഖ്യ>::value, "");
  static_assert (!std::is_constructible<hb_result_t<പൂർണ്ണസംഖ്യ, error_code_t>, error_code_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, error_code_t>::value, "");

  hb_result_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_result_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_result_t<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_result_t<പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

ഘടന any_മൂല്യം_t
{
  template <typename X>
  any_മൂല്യം_t (X&&) {}
};

സ്ഥിതം ശൂന്യം test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_result_t<any_മൂല്യം_t, error_code_t>, hb_result_err_t<സ്ഥിരം അക്ഷരം*>>::value, "");

  hb_result_t<any_മൂല്യം_t, error_code_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_result_t<any_മൂല്യം_t, error_code_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

ഘടന move_only_err_t
{
  പൂർണ്ണസംഖ്യ code;

  explicit move_only_err_t (പൂർണ്ണസംഖ്യ c) : code (c) {}
  move_only_err_t (സ്ഥിരം move_only_err_t&) = delete;
  move_only_err_t (move_only_err_t&& o) : code (o.code) { o.code = -1; }
  move_only_err_t& operator = (സ്ഥിരം move_only_err_t&) = delete;
  move_only_err_t& operator = (move_only_err_t&& o) { code = o.code; o.code = -1; തിരികെ *ഇത്; }
  ~move_only_err_t () = സ്ഥിരസ്ഥിതി;

  തർക്കം operator == (സ്ഥിരം move_only_err_t& o) സ്ഥിരം { തിരികെ code == o.code; }
};

ഘടന move_only_val_t
{
  പൂർണ്ണസംഖ്യ val;

  explicit move_only_val_t (പൂർണ്ണസംഖ്യ v) : val (v) {}
  move_only_val_t (സ്ഥിരം move_only_val_t&) = delete;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& operator = (സ്ഥിരം move_only_val_t&) = delete;
  move_only_val_t& operator = (move_only_val_t&& o) { val = o.val; o.val = -1; തിരികെ *ഇത്; }
  ~move_only_val_t () = സ്ഥിരസ്ഥിതി;

  തർക്കം operator == (സ്ഥിരം move_only_val_t& o) സ്ഥിരം { തിരികെ val == o.val; }
};

സ്ഥിതം ശൂന്യം test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_result_t<പൂർണ്ണസംഖ്യ, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.code == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_result_t<പൂർണ്ണസംഖ്യ, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_result_t<ശൂന്യം, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().code == 789);

    hb_result_t<ശൂന്യം, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().code == 999);
  }

  // Test value_or with move-only value types
  {
    hb_result_t<move_only_val_t, error_code_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).മൂല്യം_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_result_t<move_only_val_t, error_code_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).മൂല്യം_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

ഘടന copy_move_counter_t
{
  സ്ഥിതം പൂർണ്ണസംഖ്യ copy_എണ്ണം;
  സ്ഥിതം പൂർണ്ണസംഖ്യ move_എണ്ണം;

  സ്ഥിതം ശൂന്യം reset ()
  {
    copy_എണ്ണം = 0;
    move_എണ്ണം = 0;
  }

  പൂർണ്ണസംഖ്യ val;

  തർക്കം in_error() സ്ഥിരം {
    തിരികെ val == -1;
  }

  explicit copy_move_counter_t (പൂർണ്ണസംഖ്യ v) : val (v) {}
  copy_move_counter_t (സ്ഥിരം copy_move_counter_t& o) : val (o.val) { copy_എണ്ണം++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_എണ്ണം++; }
  ~copy_move_counter_t () = സ്ഥിരസ്ഥിതി;
};

പൂർണ്ണസംഖ്യ copy_move_counter_t::copy_എണ്ണം = 0;
പൂർണ്ണസംഖ്യ copy_move_counter_t::move_എണ്ണം = 0;

സ്ഥിതം result<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t out (42);
  തിരികെ Ok (std::move (out));
}

സ്ഥിതം result<copy_move_counter_t> return_tracked_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  തിരികെ Ok(out);
}

സ്ഥിതം hb_result_t<പൂർണ്ണസംഖ്യ, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t out (42);
  തിരികെ Err (std::move (out));
}

സ്ഥിതം hb_result_t<പൂർണ്ണസംഖ്യ, copy_move_counter_t> return_tracked_error_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  തിരികെ Err(out);
}

സ്ഥിതം result<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t out (42);
  തിരികെ Ok (std::move (out));
}

സ്ഥിതം ശൂന്യം test_return_moves_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_എണ്ണം == 0);
    hb_always_assert (copy_move_counter_t::move_എണ്ണം >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local_with_tag ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_എണ്ണം == 0);
    hb_always_assert (copy_move_counter_t::move_എണ്ണം >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<പൂർണ്ണസംഖ്യ, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_എണ്ണം == 0);
    hb_always_assert (copy_move_counter_t::move_എണ്ണം >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<പൂർണ്ണസംഖ്യ, copy_move_counter_t> r = return_tracked_error_from_local_with_tag ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_എണ്ണം == 0);
    hb_always_assert (copy_move_counter_t::move_എണ്ണം >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    result<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

ഘടന in_error_t {
  തർക്കം successful;

  in_error_t(തർക്കം s) : successful(s) {}

  തർക്കം in_error() സ്ഥിരം {
    തിരികെ !successful;
  }
};

സ്ഥിതം ശൂന്യം test_from () {
  in_error_t has_error(അസത്യം);
  in_error_t no_error(സത്യം);
  result<ശൂന്യം> r = result<ശൂന്യം>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = result<ശൂന്യം>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

ഘടന default_constructible_t
{
  പൂർണ്ണസംഖ്യ val;

  default_constructible_t () : val (123) {}
  explicit default_constructible_t (പൂർണ്ണസംഖ്യ v) : val (v) {}
};

ഘടന move_only_default_t
{
  പൂർണ്ണസംഖ്യ val;
  move_only_default_t () : val (77) {}
  explicit move_only_default_t (പൂർണ്ണസംഖ്യ v) : val (v) {}
  move_only_default_t (സ്ഥിരം move_only_default_t&) = delete;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& operator = (സ്ഥിരം move_only_default_t&) = delete;
  move_only_default_t& operator = (move_only_default_t&& o) { val = o.val; o.val = -1; തിരികെ *ഇത്; }
  ~move_only_default_t () = സ്ഥിരസ്ഥിതി;
};

സ്ഥിതം ശൂന്യം test_മൂല്യം_or_default ()
{
  // Test const &
  {
    സ്ഥിരം result<പൂർണ്ണസംഖ്യ> r_ok = Ok(42);
    hb_always_assert (r_ok.മൂല്യം_or_default () == 42);

    സ്ഥിരം result<പൂർണ്ണസംഖ്യ> r_err = Err(ERR_A);
    hb_always_assert (r_err.മൂല്യം_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    result<പൂർണ്ണസംഖ്യ> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).മൂല്യം_or_default () == 42);

    result<പൂർണ്ണസംഖ്യ> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).മൂല്യം_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    result<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.മൂല്യം_or_default ().val == 456);

    result<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.മൂല്യം_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).മൂല്യം_or_default ().val == 456);
    hb_always_assert (std::move (r_err).മൂല്യം_or_default ().val == 123);
  }

  // Test pointers
  {
    പൂർണ്ണസംഖ്യ x = 10;
    result<പൂർണ്ണസംഖ്യ*> r_ok = &x;
    hb_always_assert (r_ok.മൂല്യം_or_default () == &x);

    result<പൂർണ്ണസംഖ്യ*> r_err = ERR_A;
    hb_always_assert (r_err.മൂല്യം_or_default () == nullptr);
  }

  // Test move-only types
  {
    result<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).മൂല്യം_or_default ().val == 88);

    result<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).മൂല്യം_or_default ().val == 77);
  }
}

സ്ഥിതം ശൂന്യം test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    result<copy_move_counter_t> r_ok = result<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_എണ്ണം == 0);
    hb_always_assert (copy_move_counter_t::move_എണ്ണം == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    result<copy_move_counter_t> r_fail = result<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_എണ്ണം == 0);
    hb_always_assert (copy_move_counter_t::move_എണ്ണം == 0);
  }
}

പൂർണ്ണസംഖ്യ പ്രധാന ()
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
  test_മൂല്യം_or_default ();
  test_from_move ();

  തിരികെ 0;
}
