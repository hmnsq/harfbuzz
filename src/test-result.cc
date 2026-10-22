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

#ಅನ್ತರ್ಭಾವ "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ live_instances = 0;

ಗಣನಾ error_code_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

template <typename T>
using result = hb_result_t<T, error_code_t>;

ಸಂರಚನಾ resource_t
{
  ಪೂರ್ಣಾಙ್ಕ id;

  explicit resource_t (ಪೂರ್ಣಾಙ್ಕ id_) : id (id_) { live_instances++; }
  resource_t (ಧ್ರುವ resource_t& o) : id (o.id) { live_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; live_instances++; }
  ~resource_t () { live_instances--; }

  resource_t& operator = (ಧ್ರುವ resource_t& o)
  {
    id = o.id;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }
  resource_t& operator = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ resource_t& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ id == o.id; }
};

ಸ್ಥಿರ result<ಪೂರ್ಣಾಙ್ಕ> returns_ok (ಪೂರ್ಣಾಙ್ಕ v)
{
  ಪ್ರತ್ಯಾಗಮ Ok(v);
}

ಸ್ಥಿರ result<ಪೂರ್ಣಾಙ್ಕ> returns_err (error_code_t e)
{
  ಪ್ರತ್ಯಾಗಮ Err(e);
}

ಸ್ಥಿರ result<ಪೂರ್ಣಾಙ್ಕ> try_callee (ತಾರ್ಕಿಕ fail, ಪೂರ್ಣಾಙ್ಕ v, error_code_t e = ERR_A)
{
  ಯದಿ (fail) ಪ್ರತ್ಯಾಗಮ Err(e);
  ಪ್ರತ್ಯಾಗಮ Ok(v);
}

ಸ್ಥಿರ result<ಶೂನ್ಯ> try_callee_void (ತಾರ್ಕಿಕ fail,  error_code_t e)
{
  ಯದಿ (fail) ಪ್ರತ್ಯಾಗಮ e;
  ಪ್ರತ್ಯಾಗಮ Ok();
}

ಸ್ಥಿರ result<ಪೂರ್ಣಾಙ್ಕ> try_caller (ತಾರ್ಕಿಕ fail1, ತಾರ್ಕಿಕ fail2)
{
  TRY_ASSIGN (ಪೂರ್ಣಾಙ್ಕ index, try_callee (fail1, 10, ERR_A));
  TRY_ASSIGN (ಪೂರ್ಣಾಙ್ಕ index2, try_callee (fail2, 20, ERR_B));
  ಪ್ರತ್ಯಾಗಮ Ok(index + index2);
}

ಸ್ಥಿರ result<ಶೂನ್ಯ> try_caller_void (ತಾರ್ಕಿಕ fail)
{
  TRY (try_callee_void (fail, ERR_C));
  ಪ್ರತ್ಯಾಗಮ Ok();
}

ಸ್ಥಿರ result<ಭಿನ್ನ> try_caller_type_change (ತಾರ್ಕಿಕ fail)
{
  TRY_ASSIGN (ಪೂರ್ಣಾಙ್ಕ index, try_callee (fail, 100, ERR_A));
  ಪ್ರತ್ಯಾಗಮ Ok((ಭಿನ್ನ) index * 1.5f);
}

ಸ್ಥಿರ result<ಪೂರ್ಣಾಙ್ಕ> try_assign_existing (ತಾರ್ಕಿಕ fail)
{
  ಪೂರ್ಣಾಙ್ಕ val = 1;
  TRY_ASSIGN (val, try_callee (fail, 42, ERR_A));
  ಪ್ರತ್ಯಾಗಮ Ok(val);
}

ಸ್ಥಿರ ಶೂನ್ಯ test_ok_basic ()
{
  result<ಪೂರ್ಣಾಙ್ಕ> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((ತಾರ್ಕಿಕ) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.ಮೌಲ್ಯ_or (0) == 42);
  hb_always_assert (r.ಮೌಲ್ಯ_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

ಸ್ಥಿರ ಶೂನ್ಯ test_err_basic ()
{
  result<ಪೂರ್ಣಾಙ್ಕ> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.ಮೌಲ್ಯ_or (99) == 99);
  hb_always_assert (r.ಮೌಲ್ಯ_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

ಸ್ಥಿರ ಶೂನ್ಯ test_explicit_ok_err ()
{
  result<unsigned> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  result<unsigned> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

ಸ್ಥಿರ ಶೂನ್ಯ test_pointers ()
{
  ಪೂರ್ಣಾಙ್ಕ x = 123;
  result<ಪೂರ್ಣಾಙ್ಕ*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

ಸ್ಥಿರ ಶೂನ್ಯ test_void ()
{
  result<ಶೂನ್ಯ> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((ತಾರ್ಕಿಕ) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  result<ಶೂನ್ಯ> r2 = Err(ERR_A);
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

ಸ್ಥಿರ ಶೂನ್ಯ test_resource_cleanup ()
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

ಸ್ಥಿರ ಶೂನ್ಯ test_try_macro ()
{
  // Test success path
  result<ಪೂರ್ಣಾಙ್ಕ> r_success = try_caller (ಅಸತ್ಯ, ಅಸತ್ಯ);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  result<ಪೂರ್ಣಾಙ್ಕ> r_fail1 = try_caller (ಸತ್ಯ, ಅಸತ್ಯ);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  result<ಪೂರ್ಣಾಙ್ಕ> r_fail2 = try_caller (ಅಸತ್ಯ, ಸತ್ಯ);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  result<ಶೂನ್ಯ> v_ok = try_caller_void (ಅಸತ್ಯ);
  hb_always_assert (v_ok.is_ok ());

  result<ಶೂನ್ಯ> v_err = try_caller_void (ಸತ್ಯ);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  result<ಭಿನ್ನ> f_ok = try_caller_type_change (ಅಸತ್ಯ);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  result<ಭಿನ್ನ> f_err = try_caller_type_change (ಸತ್ಯ);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  result<ಪೂರ್ಣಾಙ್ಕ> a_ok = try_assign_existing (ಅಸತ್ಯ);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  result<ಪೂರ್ಣಾಙ್ಕ> a_err = try_assign_existing (ಸತ್ಯ);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

template <typename T, typename U, typename = ಶೂನ್ಯ>
ಸಂರಚನಾ is_equality_comparable : std::false_type {};

template <typename T, typename U>
ಸಂರಚನಾ is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

ಸ್ಥಿರ ಶೂನ್ಯ test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<result<ಪೂರ್ಣಾಙ್ಕ>, ಪೂರ್ಣಾಙ್ಕ>::value, "");
  static_assert (!is_equality_comparable<result<ಪೂರ್ಣಾಙ್ಕ>, int64_t>::value, "");
  static_assert (!is_equality_comparable<ಪೂರ್ಣಾಙ್ಕ, result<ಪೂರ್ಣಾಙ್ಕ>>::value, "");
  static_assert (!is_equality_comparable<result<ಪೂರ್ಣಾಙ್ಕ>, error_code_t>::value, "");
  static_assert (!is_equality_comparable<error_code_t, result<ಪೂರ್ಣಾಙ್ಕ>>::value, "");
  static_assert (!is_equality_comparable<result<ಶೂನ್ಯ>, error_code_t>::value, "");
  static_assert (is_equality_comparable<result<ಪೂರ್ಣಾಙ್ಕ>, result<ಪೂರ್ಣಾಙ್ಕ>>::value, "");
  static_assert (is_equality_comparable<result<ಪೂರ್ಣಾಙ್ಕ>, hb_result_ok_t<ಪೂರ್ಣಾಙ್ಕ>>::value, "");
  static_assert (is_equality_comparable<result<ಪೂರ್ಣಾಙ್ಕ>, hb_result_err_t<error_code_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, ಪೂರ್ಣಾಙ್ಕ>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_ok_t<ಪೂರ್ಣಾಙ್ಕ>>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_err_t<ಪೂರ್ಣಾಙ್ಕ>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ>, ಪೂರ್ಣಾಙ್ಕ>::value, "");
  static_assert (std::is_constructible<hb_result_t<ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ>, hb_result_ok_t<ಪೂರ್ಣಾಙ್ಕ>>::value, "");
  static_assert (std::is_constructible<hb_result_t<ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ>, hb_result_err_t<ಪೂರ್ಣಾಙ್ಕ>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_result_t<ಪೂರ್ಣಾಙ್ಕ, error_code_t>, ಪೂರ್ಣಾಙ್ಕ>::value, "");
  static_assert (!std::is_constructible<hb_result_t<ಪೂರ್ಣಾಙ್ಕ, error_code_t>, error_code_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, error_code_t>::value, "");

  hb_result_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_result_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_result_t<ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_result_t<ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

ಸಂರಚನಾ any_ಮೌಲ್ಯ_t
{
  template <typename X>
  any_ಮೌಲ್ಯ_t (X&&) {}
};

ಸ್ಥಿರ ಶೂನ್ಯ test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_result_t<any_ಮೌಲ್ಯ_t, error_code_t>, hb_result_err_t<ಧ್ರುವ ವರ್ಣ*>>::value, "");

  hb_result_t<any_ಮೌಲ್ಯ_t, error_code_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_result_t<any_ಮೌಲ್ಯ_t, error_code_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

ಸಂರಚನಾ move_only_err_t
{
  ಪೂರ್ಣಾಙ್ಕ code;

  explicit move_only_err_t (ಪೂರ್ಣಾಙ್ಕ c) : code (c) {}
  move_only_err_t (ಧ್ರುವ move_only_err_t&) = delete;
  move_only_err_t (move_only_err_t&& o) : code (o.code) { o.code = -1; }
  move_only_err_t& operator = (ಧ್ರುವ move_only_err_t&) = delete;
  move_only_err_t& operator = (move_only_err_t&& o) { code = o.code; o.code = -1; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ~move_only_err_t () = ಸ್ವಭಾವ;

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ move_only_err_t& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ code == o.code; }
};

ಸಂರಚನಾ move_only_val_t
{
  ಪೂರ್ಣಾಙ್ಕ val;

  explicit move_only_val_t (ಪೂರ್ಣಾಙ್ಕ v) : val (v) {}
  move_only_val_t (ಧ್ರುವ move_only_val_t&) = delete;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& operator = (ಧ್ರುವ move_only_val_t&) = delete;
  move_only_val_t& operator = (move_only_val_t&& o) { val = o.val; o.val = -1; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ~move_only_val_t () = ಸ್ವಭಾವ;

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ move_only_val_t& o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ val == o.val; }
};

ಸ್ಥಿರ ಶೂನ್ಯ test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_result_t<ಪೂರ್ಣಾಙ್ಕ, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.code == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_result_t<ಪೂರ್ಣಾಙ್ಕ, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_result_t<ಶೂನ್ಯ, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().code == 789);

    hb_result_t<ಶೂನ್ಯ, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().code == 999);
  }

  // Test value_or with move-only value types
  {
    hb_result_t<move_only_val_t, error_code_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).ಮೌಲ್ಯ_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_result_t<move_only_val_t, error_code_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).ಮೌಲ್ಯ_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

ಸಂರಚನಾ copy_move_counter_t
{
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ copy_ಎಣಿಕೆ;
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ move_ಎಣಿಕೆ;

  ಸ್ಥಿರ ಶೂನ್ಯ reset ()
  {
    copy_ಎಣಿಕೆ = 0;
    move_ಎಣಿಕೆ = 0;
  }

  ಪೂರ್ಣಾಙ್ಕ val;

  ತಾರ್ಕಿಕ in_error() ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ val == -1;
  }

  explicit copy_move_counter_t (ಪೂರ್ಣಾಙ್ಕ v) : val (v) {}
  copy_move_counter_t (ಧ್ರುವ copy_move_counter_t& o) : val (o.val) { copy_ಎಣಿಕೆ++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_ಎಣಿಕೆ++; }
  ~copy_move_counter_t () = ಸ್ವಭಾವ;
};

ಪೂರ್ಣಾಙ್ಕ copy_move_counter_t::copy_ಎಣಿಕೆ = 0;
ಪೂರ್ಣಾಙ್ಕ copy_move_counter_t::move_ಎಣಿಕೆ = 0;

ಸ್ಥಿರ result<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t out (42);
  ಪ್ರತ್ಯಾಗಮ Ok (std::move (out));
}

ಸ್ಥಿರ result<copy_move_counter_t> return_tracked_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  ಪ್ರತ್ಯಾಗಮ Ok(out);
}

ಸ್ಥಿರ hb_result_t<ಪೂರ್ಣಾಙ್ಕ, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t out (42);
  ಪ್ರತ್ಯಾಗಮ Err (std::move (out));
}

ಸ್ಥಿರ hb_result_t<ಪೂರ್ಣಾಙ್ಕ, copy_move_counter_t> return_tracked_error_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  ಪ್ರತ್ಯಾಗಮ Err(out);
}

ಸ್ಥಿರ result<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t out (42);
  ಪ್ರತ್ಯಾಗಮ Ok (std::move (out));
}

ಸ್ಥಿರ ಶೂನ್ಯ test_return_moves_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ಎಣಿಕೆ == 0);
    hb_always_assert (copy_move_counter_t::move_ಎಣಿಕೆ >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local_with_tag ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ಎಣಿಕೆ == 0);
    hb_always_assert (copy_move_counter_t::move_ಎಣಿಕೆ >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<ಪೂರ್ಣಾಙ್ಕ, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ಎಣಿಕೆ == 0);
    hb_always_assert (copy_move_counter_t::move_ಎಣಿಕೆ >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<ಪೂರ್ಣಾಙ್ಕ, copy_move_counter_t> r = return_tracked_error_from_local_with_tag ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ಎಣಿಕೆ == 0);
    hb_always_assert (copy_move_counter_t::move_ಎಣಿಕೆ >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    result<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

ಸಂರಚನಾ in_error_t {
  ತಾರ್ಕಿಕ successful;

  in_error_t(ತಾರ್ಕಿಕ s) : successful(s) {}

  ತಾರ್ಕಿಕ in_error() ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ !successful;
  }
};

ಸ್ಥಿರ ಶೂನ್ಯ test_from () {
  in_error_t has_error(ಅಸತ್ಯ);
  in_error_t no_error(ಸತ್ಯ);
  result<ಶೂನ್ಯ> r = result<ಶೂನ್ಯ>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = result<ಶೂನ್ಯ>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

ಸಂರಚನಾ default_constructible_t
{
  ಪೂರ್ಣಾಙ್ಕ val;

  default_constructible_t () : val (123) {}
  explicit default_constructible_t (ಪೂರ್ಣಾಙ್ಕ v) : val (v) {}
};

ಸಂರಚನಾ move_only_default_t
{
  ಪೂರ್ಣಾಙ್ಕ val;
  move_only_default_t () : val (77) {}
  explicit move_only_default_t (ಪೂರ್ಣಾಙ್ಕ v) : val (v) {}
  move_only_default_t (ಧ್ರುವ move_only_default_t&) = delete;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& operator = (ಧ್ರುವ move_only_default_t&) = delete;
  move_only_default_t& operator = (move_only_default_t&& o) { val = o.val; o.val = -1; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  ~move_only_default_t () = ಸ್ವಭಾವ;
};

ಸ್ಥಿರ ಶೂನ್ಯ test_ಮೌಲ್ಯ_or_default ()
{
  // Test const &
  {
    ಧ್ರುವ result<ಪೂರ್ಣಾಙ್ಕ> r_ok = Ok(42);
    hb_always_assert (r_ok.ಮೌಲ್ಯ_or_default () == 42);

    ಧ್ರುವ result<ಪೂರ್ಣಾಙ್ಕ> r_err = Err(ERR_A);
    hb_always_assert (r_err.ಮೌಲ್ಯ_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    result<ಪೂರ್ಣಾಙ್ಕ> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).ಮೌಲ್ಯ_or_default () == 42);

    result<ಪೂರ್ಣಾಙ್ಕ> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).ಮೌಲ್ಯ_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    result<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.ಮೌಲ್ಯ_or_default ().val == 456);

    result<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.ಮೌಲ್ಯ_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).ಮೌಲ್ಯ_or_default ().val == 456);
    hb_always_assert (std::move (r_err).ಮೌಲ್ಯ_or_default ().val == 123);
  }

  // Test pointers
  {
    ಪೂರ್ಣಾಙ್ಕ x = 10;
    result<ಪೂರ್ಣಾಙ್ಕ*> r_ok = &x;
    hb_always_assert (r_ok.ಮೌಲ್ಯ_or_default () == &x);

    result<ಪೂರ್ಣಾಙ್ಕ*> r_err = ERR_A;
    hb_always_assert (r_err.ಮೌಲ್ಯ_or_default () == nullptr);
  }

  // Test move-only types
  {
    result<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).ಮೌಲ್ಯ_or_default ().val == 88);

    result<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).ಮೌಲ್ಯ_or_default ().val == 77);
  }
}

ಸ್ಥಿರ ಶೂನ್ಯ test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    result<copy_move_counter_t> r_ok = result<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_ಎಣಿಕೆ == 0);
    hb_always_assert (copy_move_counter_t::move_ಎಣಿಕೆ == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    result<copy_move_counter_t> r_fail = result<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_ಎಣಿಕೆ == 0);
    hb_always_assert (copy_move_counter_t::move_ಎಣಿಕೆ == 0);
  }
}

ಪೂರ್ಣಾಙ್ಕ main ()
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
  test_ಮೌಲ್ಯ_or_default ();
  test_from_move ();

  ಪ್ರತ್ಯಾಗಮ 0;
}
