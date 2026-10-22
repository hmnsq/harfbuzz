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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
ਸਥਿਰ ਪੂਰਨ ਜੀਵਤ_instances = 0;

ਗਿਣਤੀ error_ਕੋਡ_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਵਰਤੋਂ ਨਤੀਜਾ = hb_ਨਤੀਜਾ_t<T, error_ਕੋਡ_t>;

ਬਣਤਰ resource_t
{
  ਪੂਰਨ id;

  ਸਪਸ਼ਟ resource_t (ਪੂਰਨ id_) : id (id_) { ਜੀਵਤ_instances++; }
  resource_t (ਅਚਲ resource_t& o) : id (o.id) { ਜੀਵਤ_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; ਜੀਵਤ_instances++; }
  ~resource_t () { ਜੀਵਤ_instances--; }

  resource_t& ਸੰਚਾਲਕ = (ਅਚਲ resource_t& o)
  {
    id = o.id;
    ਵਾਪਸ *ਇਹ;
  }
  resource_t& ਸੰਚਾਲਕ = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    ਵਾਪਸ *ਇਹ;
  }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ resource_t& o) ਅਚਲ { ਵਾਪਸ id == o.id; }
};

ਸਥਿਰ ਨਤੀਜਾ<ਪੂਰਨ> returns_ok (ਪੂਰਨ v)
{
  ਵਾਪਸ Ok(v);
}

ਸਥਿਰ ਨਤੀਜਾ<ਪੂਰਨ> returns_err (error_ਕੋਡ_t e)
{
  ਵਾਪਸ Err(e);
}

ਸਥਿਰ ਨਤੀਜਾ<ਪੂਰਨ> try_callee (ਤਾਰਕਿਕ fail, ਪੂਰਨ v, error_ਕੋਡ_t e = ERR_A)
{
  ਜੇ (fail) ਵਾਪਸ Err(e);
  ਵਾਪਸ Ok(v);
}

ਸਥਿਰ ਨਤੀਜਾ<ਖਾਲੀ> try_callee_void (ਤਾਰਕਿਕ fail,  error_ਕੋਡ_t e)
{
  ਜੇ (fail) ਵਾਪਸ e;
  ਵਾਪਸ Ok();
}

ਸਥਿਰ ਨਤੀਜਾ<ਪੂਰਨ> try_caller (ਤਾਰਕਿਕ fail1, ਤਾਰਕਿਕ fail2)
{
  TRY_ਸੌਂਪੋ (ਪੂਰਨ ਸੂਚਕਾਂਕ_2, try_callee (fail1, 10, ERR_A));
  TRY_ਸੌਂਪੋ (ਪੂਰਨ index2, try_callee (fail2, 20, ERR_B));
  ਵਾਪਸ Ok(ਸੂਚਕਾਂਕ_2 + index2);
}

ਸਥਿਰ ਨਤੀਜਾ<ਖਾਲੀ> try_caller_void (ਤਾਰਕਿਕ fail)
{
  TRY (try_callee_void (fail, ERR_C));
  ਵਾਪਸ Ok();
}

ਸਥਿਰ ਨਤੀਜਾ<ਦਸ਼ਮਲਵ> try_caller_type_change (ਤਾਰਕਿਕ fail)
{
  TRY_ਸੌਂਪੋ (ਪੂਰਨ ਸੂਚਕਾਂਕ_2, try_callee (fail, 100, ERR_A));
  ਵਾਪਸ Ok((ਦਸ਼ਮਲਵ) ਸੂਚਕਾਂਕ_2 * 1.5f);
}

ਸਥਿਰ ਨਤੀਜਾ<ਪੂਰਨ> try_ਸੌਂਪੋ_existing (ਤਾਰਕਿਕ fail)
{
  ਪੂਰਨ val = 1;
  TRY_ਸੌਂਪੋ (val, try_callee (fail, 42, ERR_A));
  ਵਾਪਸ Ok(val);
}

ਸਥਿਰ ਖਾਲੀ test_ok_basic ()
{
  ਨਤੀਜਾ<ਪੂਰਨ> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((ਤਾਰਕਿਕ) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.ਮੁੱਲ_or (0) == 42);
  hb_always_assert (r.ਮੁੱਲ_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

ਸਥਿਰ ਖਾਲੀ test_err_basic ()
{
  ਨਤੀਜਾ<ਪੂਰਨ> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.ਮੁੱਲ_or (99) == 99);
  hb_always_assert (r.ਮੁੱਲ_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

ਸਥਿਰ ਖਾਲੀ test_explicit_ok_err ()
{
  ਨਤੀਜਾ<ਬਿਨਾਨਿਸ਼ਾਨੀ> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  ਨਤੀਜਾ<ਬਿਨਾਨਿਸ਼ਾਨੀ> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

ਸਥਿਰ ਖਾਲੀ test_ਸੂਚਕ_ਸਾਰੇ ()
{
  ਪੂਰਨ x = 123;
  ਨਤੀਜਾ<ਪੂਰਨ*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

ਸਥਿਰ ਖਾਲੀ test_void ()
{
  ਨਤੀਜਾ<ਖਾਲੀ> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((ਤਾਰਕਿਕ) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  ਨਤੀਜਾ<ਖਾਲੀ> r2 = Err(ERR_A);
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

ਸਥਿਰ ਖਾਲੀ test_resource_cleanup ()
{
  hb_always_assert (ਜੀਵਤ_instances == 0);

  {
    ਨਤੀਜਾ<resource_t> r = resource_t (1);
    hb_always_assert (ਜੀਵਤ_instances == 1);
    hb_always_assert (r.value ().id == 1);
  }
  hb_always_assert (ਜੀਵਤ_instances == 0);

  {
    ਨਤੀਜਾ<resource_t> r1 = resource_t (2);
    hb_always_assert (ਜੀਵਤ_instances == 1);
    ਨਤੀਜਾ<resource_t> r2 = r1;
    hb_always_assert (ਜੀਵਤ_instances == 2);
    ਨਤੀਜਾ<resource_t> r3 = std::move (r1);
    hb_always_assert (ਜੀਵਤ_instances == 3);
  }
  hb_always_assert (ਜੀਵਤ_instances == 0);

  {
    ਨਤੀਜਾ<resource_t> r = resource_t (3);
    hb_always_assert (ਜੀਵਤ_instances == 1);
    r = Err(ERR_A);
    hb_always_assert (ਜੀਵਤ_instances == 0);
    hb_always_assert (r.is_err ());
  }
  hb_always_assert (ਜੀਵਤ_instances == 0);
}

ਸਥਿਰ ਖਾਲੀ test_try_macro ()
{
  // Test success path
  ਨਤੀਜਾ<ਪੂਰਨ> r_success = try_caller (ਝੂਠ, ਝੂਠ);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  ਨਤੀਜਾ<ਪੂਰਨ> r_fail1 = try_caller (ਸੱਚ, ਝੂਠ);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  ਨਤੀਜਾ<ਪੂਰਨ> r_fail2 = try_caller (ਝੂਠ, ਸੱਚ);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  ਨਤੀਜਾ<ਖਾਲੀ> v_ok = try_caller_void (ਝੂਠ);
  hb_always_assert (v_ok.is_ok ());

  ਨਤੀਜਾ<ਖਾਲੀ> v_err = try_caller_void (ਸੱਚ);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  ਨਤੀਜਾ<ਦਸ਼ਮਲਵ> f_ok = try_caller_type_change (ਝੂਠ);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  ਨਤੀਜਾ<ਦਸ਼ਮਲਵ> f_err = try_caller_type_change (ਸੱਚ);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  ਨਤੀਜਾ<ਪੂਰਨ> a_ok = try_ਸੌਂਪੋ_existing (ਝੂਠ);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  ਨਤੀਜਾ<ਪੂਰਨ> a_err = try_ਸੌਂਪੋ_existing (ਸੱਚ);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ U, ਕਿਸਮ_ਨਾਂ = ਖਾਲੀ>
ਬਣਤਰ is_equality_comparable : std::false_type {};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ U>
ਬਣਤਰ is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

ਸਥਿਰ ਖਾਲੀ test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<ਨਤੀਜਾ<ਪੂਰਨ>, ਪੂਰਨ>::value, "");
  static_assert (!is_equality_comparable<ਨਤੀਜਾ<ਪੂਰਨ>, int64_t>::value, "");
  static_assert (!is_equality_comparable<ਪੂਰਨ, ਨਤੀਜਾ<ਪੂਰਨ>>::value, "");
  static_assert (!is_equality_comparable<ਨਤੀਜਾ<ਪੂਰਨ>, error_ਕੋਡ_t>::value, "");
  static_assert (!is_equality_comparable<error_ਕੋਡ_t, ਨਤੀਜਾ<ਪੂਰਨ>>::value, "");
  static_assert (!is_equality_comparable<ਨਤੀਜਾ<ਖਾਲੀ>, error_ਕੋਡ_t>::value, "");
  static_assert (is_equality_comparable<ਨਤੀਜਾ<ਪੂਰਨ>, ਨਤੀਜਾ<ਪੂਰਨ>>::value, "");
  static_assert (is_equality_comparable<ਨਤੀਜਾ<ਪੂਰਨ>, hb_ਨਤੀਜਾ_ok_t<ਪੂਰਨ>>::value, "");
  static_assert (is_equality_comparable<ਨਤੀਜਾ<ਪੂਰਨ>, hb_ਨਤੀਜਾ_err_t<error_ਕੋਡ_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_ਨਤੀਜਾ_t<int64_t, int32_t>, ਪੂਰਨ>::value, "");
  static_assert (!std::is_constructible<hb_ਨਤੀਜਾ_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_ਨਤੀਜਾ_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_ਨਤੀਜਾ_t<int64_t, int32_t>, hb_ਨਤੀਜਾ_ok_t<ਪੂਰਨ>>::value, "");
  static_assert (std::is_constructible<hb_ਨਤੀਜਾ_t<int64_t, int32_t>, hb_ਨਤੀਜਾ_err_t<ਪੂਰਨ>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_ਨਤੀਜਾ_t<ਪੂਰਨ, ਪੂਰਨ>, ਪੂਰਨ>::value, "");
  static_assert (std::is_constructible<hb_ਨਤੀਜਾ_t<ਪੂਰਨ, ਪੂਰਨ>, hb_ਨਤੀਜਾ_ok_t<ਪੂਰਨ>>::value, "");
  static_assert (std::is_constructible<hb_ਨਤੀਜਾ_t<ਪੂਰਨ, ਪੂਰਨ>, hb_ਨਤੀਜਾ_err_t<ਪੂਰਨ>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_ਨਤੀਜਾ_t<ਪੂਰਨ, error_ਕੋਡ_t>, ਪੂਰਨ>::value, "");
  static_assert (!std::is_constructible<hb_ਨਤੀਜਾ_t<ਪੂਰਨ, error_ਕੋਡ_t>, error_ਕੋਡ_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_ਨਤੀਜਾ_t<resource_t, error_ਕੋਡ_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_ਨਤੀਜਾ_t<resource_t, error_ਕੋਡ_t>, error_ਕੋਡ_t>::value, "");

  hb_ਨਤੀਜਾ_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_ਨਤੀਜਾ_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_ਨਤੀਜਾ_t<ਪੂਰਨ, ਪੂਰਨ> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_ਨਤੀਜਾ_t<ਪੂਰਨ, ਪੂਰਨ> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

ਬਣਤਰ any_ਮੁੱਲ_t
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ X>
  any_ਮੁੱਲ_t (X&&) {}
};

ਸਥਿਰ ਖਾਲੀ test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_ਨਤੀਜਾ_t<any_ਮੁੱਲ_t, error_ਕੋਡ_t>, hb_ਨਤੀਜਾ_err_t<ਅਚਲ ਅੱਖਰ*>>::value, "");

  hb_ਨਤੀਜਾ_t<any_ਮੁੱਲ_t, error_ਕੋਡ_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_ਨਤੀਜਾ_t<any_ਮੁੱਲ_t, error_ਕੋਡ_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

ਬਣਤਰ move_only_err_t
{
  ਪੂਰਨ ਕੋਡ;

  ਸਪਸ਼ਟ move_only_err_t (ਪੂਰਨ c) : ਕੋਡ (c) {}
  move_only_err_t (ਅਚਲ move_only_err_t&) = ਮਿਟਾਓ;
  move_only_err_t (move_only_err_t&& o) : ਕੋਡ (o.ਕੋਡ) { o.ਕੋਡ = -1; }
  move_only_err_t& ਸੰਚਾਲਕ = (ਅਚਲ move_only_err_t&) = ਮਿਟਾਓ;
  move_only_err_t& ਸੰਚਾਲਕ = (move_only_err_t&& o) { ਕੋਡ = o.ਕੋਡ; o.ਕੋਡ = -1; ਵਾਪਸ *ਇਹ; }
  ~move_only_err_t () = ਮੂਲ;

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ move_only_err_t& o) ਅਚਲ { ਵਾਪਸ ਕੋਡ == o.ਕੋਡ; }
};

ਬਣਤਰ move_only_val_t
{
  ਪੂਰਨ val;

  ਸਪਸ਼ਟ move_only_val_t (ਪੂਰਨ v) : val (v) {}
  move_only_val_t (ਅਚਲ move_only_val_t&) = ਮਿਟਾਓ;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& ਸੰਚਾਲਕ = (ਅਚਲ move_only_val_t&) = ਮਿਟਾਓ;
  move_only_val_t& ਸੰਚਾਲਕ = (move_only_val_t&& o) { val = o.val; o.val = -1; ਵਾਪਸ *ਇਹ; }
  ~move_only_val_t () = ਮੂਲ;

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ move_only_val_t& o) ਅਚਲ { ਵਾਪਸ val == o.val; }
};

ਸਥਿਰ ਖਾਲੀ test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_ਨਤੀਜਾ_t<ਪੂਰਨ, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().ਕੋਡ == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.ਕੋਡ == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_ਨਤੀਜਾ_t<ਪੂਰਨ, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().ਕੋਡ == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_ਨਤੀਜਾ_t<ਖਾਲੀ, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().ਕੋਡ == 789);

    hb_ਨਤੀਜਾ_t<ਖਾਲੀ, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().ਕੋਡ == 999);
  }

  // Test value_or with move-only value types
  {
    hb_ਨਤੀਜਾ_t<move_only_val_t, error_ਕੋਡ_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).ਮੁੱਲ_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_ਨਤੀਜਾ_t<move_only_val_t, error_ਕੋਡ_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).ਮੁੱਲ_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

ਬਣਤਰ copy_move_counter_t
{
  ਸਥਿਰ ਪੂਰਨ copy_ਗਿਣਤੀ;
  ਸਥਿਰ ਪੂਰਨ move_ਗਿਣਤੀ;

  ਸਥਿਰ ਖਾਲੀ reset ()
  {
    copy_ਗਿਣਤੀ = 0;
    move_ਗਿਣਤੀ = 0;
  }

  ਪੂਰਨ val;

  ਤਾਰਕਿਕ ਵਿੱਚ_error() ਅਚਲ {
    ਵਾਪਸ val == -1;
  }

  ਸਪਸ਼ਟ copy_move_counter_t (ਪੂਰਨ v) : val (v) {}
  copy_move_counter_t (ਅਚਲ copy_move_counter_t& o) : val (o.val) { copy_ਗਿਣਤੀ++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_ਗਿਣਤੀ++; }
  ~copy_move_counter_t () = ਮੂਲ;
};

ਪੂਰਨ copy_move_counter_t::copy_ਗਿਣਤੀ = 0;
ਪੂਰਨ copy_move_counter_t::move_ਗਿਣਤੀ = 0;

ਸਥਿਰ ਨਤੀਜਾ<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t ਬਾਹਰਲਾ (42);
  ਵਾਪਸ Ok (std::move (ਬਾਹਰਲਾ));
}

ਸਥਿਰ ਨਤੀਜਾ<copy_move_counter_t> return_tracked_from_local_with_ਟੈਗ ()
{
  copy_move_counter_t ਬਾਹਰਲਾ (42);
  ਵਾਪਸ Ok(ਬਾਹਰਲਾ);
}

ਸਥਿਰ hb_ਨਤੀਜਾ_t<ਪੂਰਨ, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t ਬਾਹਰਲਾ (42);
  ਵਾਪਸ Err (std::move (ਬਾਹਰਲਾ));
}

ਸਥਿਰ hb_ਨਤੀਜਾ_t<ਪੂਰਨ, copy_move_counter_t> return_tracked_error_from_local_with_ਟੈਗ ()
{
  copy_move_counter_t ਬਾਹਰਲਾ (42);
  ਵਾਪਸ Err(ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਨਤੀਜਾ<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t ਬਾਹਰਲਾ (42);
  ਵਾਪਸ Ok (std::move (ਬਾਹਰਲਾ));
}

ਸਥਿਰ ਖਾਲੀ test_return_ਚਾਲਾਂ_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    ਨਤੀਜਾ<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ਗਿਣਤੀ == 0);
    hb_always_assert (copy_move_counter_t::move_ਗਿਣਤੀ >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    ਨਤੀਜਾ<copy_move_counter_t> r = return_tracked_from_local_with_ਟੈਗ ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ਗਿਣਤੀ == 0);
    hb_always_assert (copy_move_counter_t::move_ਗਿਣਤੀ >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_ਨਤੀਜਾ_t<ਪੂਰਨ, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ਗਿਣਤੀ == 0);
    hb_always_assert (copy_move_counter_t::move_ਗਿਣਤੀ >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_ਨਤੀਜਾ_t<ਪੂਰਨ, copy_move_counter_t> r = return_tracked_error_from_local_with_ਟੈਗ ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ਗਿਣਤੀ == 0);
    hb_always_assert (copy_move_counter_t::move_ਗਿਣਤੀ >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    ਨਤੀਜਾ<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

ਬਣਤਰ ਵਿੱਚ_error_t {
  ਤਾਰਕਿਕ successful;

  ਵਿੱਚ_error_t(ਤਾਰਕਿਕ s) : successful(s) {}

  ਤਾਰਕਿਕ ਵਿੱਚ_error() ਅਚਲ {
    ਵਾਪਸ !successful;
  }
};

ਸਥਿਰ ਖਾਲੀ test_from () {
  ਵਿੱਚ_error_t has_error(ਝੂਠ);
  ਵਿੱਚ_error_t no_error(ਸੱਚ);
  ਨਤੀਜਾ<ਖਾਲੀ> r = ਨਤੀਜਾ<ਖਾਲੀ>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = ਨਤੀਜਾ<ਖਾਲੀ>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

ਬਣਤਰ default_constructible_t
{
  ਪੂਰਨ val;

  default_constructible_t () : val (123) {}
  ਸਪਸ਼ਟ default_constructible_t (ਪੂਰਨ v) : val (v) {}
};

ਬਣਤਰ move_only_default_t
{
  ਪੂਰਨ val;
  move_only_default_t () : val (77) {}
  ਸਪਸ਼ਟ move_only_default_t (ਪੂਰਨ v) : val (v) {}
  move_only_default_t (ਅਚਲ move_only_default_t&) = ਮਿਟਾਓ;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& ਸੰਚਾਲਕ = (ਅਚਲ move_only_default_t&) = ਮਿਟਾਓ;
  move_only_default_t& ਸੰਚਾਲਕ = (move_only_default_t&& o) { val = o.val; o.val = -1; ਵਾਪਸ *ਇਹ; }
  ~move_only_default_t () = ਮੂਲ;
};

ਸਥਿਰ ਖਾਲੀ test_ਮੁੱਲ_or_default ()
{
  // Test const &
  {
    ਅਚਲ ਨਤੀਜਾ<ਪੂਰਨ> r_ok = Ok(42);
    hb_always_assert (r_ok.ਮੁੱਲ_or_default () == 42);

    ਅਚਲ ਨਤੀਜਾ<ਪੂਰਨ> r_err = Err(ERR_A);
    hb_always_assert (r_err.ਮੁੱਲ_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    ਨਤੀਜਾ<ਪੂਰਨ> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).ਮੁੱਲ_or_default () == 42);

    ਨਤੀਜਾ<ਪੂਰਨ> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).ਮੁੱਲ_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    ਨਤੀਜਾ<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.ਮੁੱਲ_or_default ().val == 456);

    ਨਤੀਜਾ<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.ਮੁੱਲ_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).ਮੁੱਲ_or_default ().val == 456);
    hb_always_assert (std::move (r_err).ਮੁੱਲ_or_default ().val == 123);
  }

  // Test pointers
  {
    ਪੂਰਨ x = 10;
    ਨਤੀਜਾ<ਪੂਰਨ*> r_ok = &x;
    hb_always_assert (r_ok.ਮੁੱਲ_or_default () == &x);

    ਨਤੀਜਾ<ਪੂਰਨ*> r_err = ERR_A;
    hb_always_assert (r_err.ਮੁੱਲ_or_default () == ਖਾਲੀ_ਸੂਚਕ);
  }

  // Test move-only types
  {
    ਨਤੀਜਾ<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).ਮੁੱਲ_or_default ().val == 88);

    ਨਤੀਜਾ<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).ਮੁੱਲ_or_default ().val == 77);
  }
}

ਸਥਿਰ ਖਾਲੀ test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    ਨਤੀਜਾ<copy_move_counter_t> r_ok = ਨਤੀਜਾ<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_ਗਿਣਤੀ == 0);
    hb_always_assert (copy_move_counter_t::move_ਗਿਣਤੀ == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    ਨਤੀਜਾ<copy_move_counter_t> r_fail = ਨਤੀਜਾ<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_ਗਿਣਤੀ == 0);
    hb_always_assert (copy_move_counter_t::move_ਗਿਣਤੀ == 0);
  }
}

ਪੂਰਨ ਮੁੱਖ ()
{
  test_ok_basic ();
  test_err_basic ();
  test_explicit_ok_err ();
  test_ਸੂਚਕ_ਸਾਰੇ ();
  test_void ();
  test_resource_cleanup ();
  test_try_macro ();
  test_same_or_convertible_types ();
  test_error_interception ();
  test_move_only_types ();
  test_return_ਚਾਲਾਂ_from_local ();
  test_from();
  test_ਮੁੱਲ_or_default ();
  test_from_move ();

  ਵਾਪਸ 0;
}
