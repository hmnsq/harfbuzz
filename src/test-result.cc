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

#అన్తర్భావ "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
స్థిర పూర్ణాంకం సజీవ_instances = 0;

గణన error_సంకేతం_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

మూస <రకం_పేరు T>
ఉపయోగించి ఫలితం = hb_ఫలితం_t<T, error_సంకేతం_t>;

నిర్మాణం resource_t
{
  పూర్ణాంకం id;

  స్పష్టమైన resource_t (పూర్ణాంకం id_) : id (id_) { సజీవ_instances++; }
  resource_t (స్థిరరాశి resource_t& o) : id (o.id) { సజీవ_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; సజీవ_instances++; }
  ~resource_t () { సజీవ_instances--; }

  resource_t& క్రియాకారకం = (స్థిరరాశి resource_t& o)
  {
    id = o.id;
    తిరిగి *ఇది;
  }
  resource_t& క్రియాకారకం = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    తిరిగి *ఇది;
  }

  తార్కిక క్రియాకారకం == (స్థిరరాశి resource_t& o) స్థిరరాశి { తిరిగి id == o.id; }
};

స్థిర ఫలితం<పూర్ణాంకం> returns_ok (పూర్ణాంకం v)
{
  తిరిగి Ok(v);
}

స్థిర ఫలితం<పూర్ణాంకం> returns_err (error_సంకేతం_t e)
{
  తిరిగి Err(e);
}

స్థిర ఫలితం<పూర్ణాంకం> try_callee (తార్కిక fail, పూర్ణాంకం v, error_సంకేతం_t e = ERR_A)
{
  ఒకవేళ (fail) తిరిగి Err(e);
  తిరిగి Ok(v);
}

స్థిర ఫలితం<శూన్య> try_callee_void (తార్కిక fail,  error_సంకేతం_t e)
{
  ఒకవేళ (fail) తిరిగి e;
  తిరిగి Ok();
}

స్థిర ఫలితం<పూర్ణాంకం> try_caller (తార్కిక fail1, తార్కిక fail2)
{
  TRY_కేటాయించు (పూర్ణాంకం సూచీ_2, try_callee (fail1, 10, ERR_A));
  TRY_కేటాయించు (పూర్ణాంకం index2, try_callee (fail2, 20, ERR_B));
  తిరిగి Ok(సూచీ_2 + index2);
}

స్థిర ఫలితం<శూన్య> try_caller_void (తార్కిక fail)
{
  TRY (try_callee_void (fail, ERR_C));
  తిరిగి Ok();
}

స్థిర ఫలితం<దశాంశం> try_caller_type_change (తార్కిక fail)
{
  TRY_కేటాయించు (పూర్ణాంకం సూచీ_2, try_callee (fail, 100, ERR_A));
  తిరిగి Ok((దశాంశం) సూచీ_2 * 1.5f);
}

స్థిర ఫలితం<పూర్ణాంకం> try_కేటాయించు_existing (తార్కిక fail)
{
  పూర్ణాంకం val = 1;
  TRY_కేటాయించు (val, try_callee (fail, 42, ERR_A));
  తిరిగి Ok(val);
}

స్థిర శూన్య test_ok_basic ()
{
  ఫలితం<పూర్ణాంకం> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((తార్కిక) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.విలువ_or (0) == 42);
  hb_always_assert (r.విలువ_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

స్థిర శూన్య test_err_basic ()
{
  ఫలితం<పూర్ణాంకం> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.విలువ_or (99) == 99);
  hb_always_assert (r.విలువ_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

స్థిర శూన్య test_explicit_ok_err ()
{
  ఫలితం<గుర్తులేని> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  ఫలితం<గుర్తులేని> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

స్థిర శూన్య test_సూచికలు ()
{
  పూర్ణాంకం x = 123;
  ఫలితం<పూర్ణాంకం*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

స్థిర శూన్య test_void ()
{
  ఫలితం<శూన్య> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((తార్కిక) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  ఫలితం<శూన్య> r2 = Err(ERR_A);
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

స్థిర శూన్య test_resource_cleanup ()
{
  hb_always_assert (సజీవ_instances == 0);

  {
    ఫలితం<resource_t> r = resource_t (1);
    hb_always_assert (సజీవ_instances == 1);
    hb_always_assert (r.value ().id == 1);
  }
  hb_always_assert (సజీవ_instances == 0);

  {
    ఫలితం<resource_t> r1 = resource_t (2);
    hb_always_assert (సజీవ_instances == 1);
    ఫలితం<resource_t> r2 = r1;
    hb_always_assert (సజీవ_instances == 2);
    ఫలితం<resource_t> r3 = std::move (r1);
    hb_always_assert (సజీవ_instances == 3);
  }
  hb_always_assert (సజీవ_instances == 0);

  {
    ఫలితం<resource_t> r = resource_t (3);
    hb_always_assert (సజీవ_instances == 1);
    r = Err(ERR_A);
    hb_always_assert (సజీవ_instances == 0);
    hb_always_assert (r.is_err ());
  }
  hb_always_assert (సజీవ_instances == 0);
}

స్థిర శూన్య test_try_macro ()
{
  // Test success path
  ఫలితం<పూర్ణాంకం> r_success = try_caller (అబద్ధం, అబద్ధం);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  ఫలితం<పూర్ణాంకం> r_fail1 = try_caller (నిజం, అబద్ధం);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  ఫలితం<పూర్ణాంకం> r_fail2 = try_caller (అబద్ధం, నిజం);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  ఫలితం<శూన్య> v_ok = try_caller_void (అబద్ధం);
  hb_always_assert (v_ok.is_ok ());

  ఫలితం<శూన్య> v_err = try_caller_void (నిజం);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  ఫలితం<దశాంశం> f_ok = try_caller_type_change (అబద్ధం);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  ఫలితం<దశాంశం> f_err = try_caller_type_change (నిజం);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  ఫలితం<పూర్ణాంకం> a_ok = try_కేటాయించు_existing (అబద్ధం);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  ఫలితం<పూర్ణాంకం> a_err = try_కేటాయించు_existing (నిజం);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

మూస <రకం_పేరు T, రకం_పేరు U, రకం_పేరు = శూన్య>
నిర్మాణం is_equality_comparable : std::false_type {};

మూస <రకం_పేరు T, రకం_పేరు U>
నిర్మాణం is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

స్థిర శూన్య test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<ఫలితం<పూర్ణాంకం>, పూర్ణాంకం>::value, "");
  static_assert (!is_equality_comparable<ఫలితం<పూర్ణాంకం>, int64_t>::value, "");
  static_assert (!is_equality_comparable<పూర్ణాంకం, ఫలితం<పూర్ణాంకం>>::value, "");
  static_assert (!is_equality_comparable<ఫలితం<పూర్ణాంకం>, error_సంకేతం_t>::value, "");
  static_assert (!is_equality_comparable<error_సంకేతం_t, ఫలితం<పూర్ణాంకం>>::value, "");
  static_assert (!is_equality_comparable<ఫలితం<శూన్య>, error_సంకేతం_t>::value, "");
  static_assert (is_equality_comparable<ఫలితం<పూర్ణాంకం>, ఫలితం<పూర్ణాంకం>>::value, "");
  static_assert (is_equality_comparable<ఫలితం<పూర్ణాంకం>, hb_ఫలితం_ok_t<పూర్ణాంకం>>::value, "");
  static_assert (is_equality_comparable<ఫలితం<పూర్ణాంకం>, hb_ఫలితం_err_t<error_సంకేతం_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_ఫలితం_t<int64_t, int32_t>, పూర్ణాంకం>::value, "");
  static_assert (!std::is_constructible<hb_ఫలితం_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_ఫలితం_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_ఫలితం_t<int64_t, int32_t>, hb_ఫలితం_ok_t<పూర్ణాంకం>>::value, "");
  static_assert (std::is_constructible<hb_ఫలితం_t<int64_t, int32_t>, hb_ఫలితం_err_t<పూర్ణాంకం>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_ఫలితం_t<పూర్ణాంకం, పూర్ణాంకం>, పూర్ణాంకం>::value, "");
  static_assert (std::is_constructible<hb_ఫలితం_t<పూర్ణాంకం, పూర్ణాంకం>, hb_ఫలితం_ok_t<పూర్ణాంకం>>::value, "");
  static_assert (std::is_constructible<hb_ఫలితం_t<పూర్ణాంకం, పూర్ణాంకం>, hb_ఫలితం_err_t<పూర్ణాంకం>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_ఫలితం_t<పూర్ణాంకం, error_సంకేతం_t>, పూర్ణాంకం>::value, "");
  static_assert (!std::is_constructible<hb_ఫలితం_t<పూర్ణాంకం, error_సంకేతం_t>, error_సంకేతం_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_ఫలితం_t<resource_t, error_సంకేతం_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_ఫలితం_t<resource_t, error_సంకేతం_t>, error_సంకేతం_t>::value, "");

  hb_ఫలితం_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_ఫలితం_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_ఫలితం_t<పూర్ణాంకం, పూర్ణాంకం> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_ఫలితం_t<పూర్ణాంకం, పూర్ణాంకం> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

నిర్మాణం any_విలువ_t
{
  మూస <రకం_పేరు X>
  any_విలువ_t (X&&) {}
};

స్థిర శూన్య test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_ఫలితం_t<any_విలువ_t, error_సంకేతం_t>, hb_ఫలితం_err_t<స్థిరరాశి అక్షరం*>>::value, "");

  hb_ఫలితం_t<any_విలువ_t, error_సంకేతం_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_ఫలితం_t<any_విలువ_t, error_సంకేతం_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

నిర్మాణం move_only_err_t
{
  పూర్ణాంకం సంకేతం_2;

  స్పష్టమైన move_only_err_t (పూర్ణాంకం c) : సంకేతం_2 (c) {}
  move_only_err_t (స్థిరరాశి move_only_err_t&) = తొలగించు;
  move_only_err_t (move_only_err_t&& o) : సంకేతం_2 (o.సంకేతం_2) { o.సంకేతం_2 = -1; }
  move_only_err_t& క్రియాకారకం = (స్థిరరాశి move_only_err_t&) = తొలగించు;
  move_only_err_t& క్రియాకారకం = (move_only_err_t&& o) { సంకేతం_2 = o.సంకేతం_2; o.సంకేతం_2 = -1; తిరిగి *ఇది; }
  ~move_only_err_t () = అప్రమేయ;

  తార్కిక క్రియాకారకం == (స్థిరరాశి move_only_err_t& o) స్థిరరాశి { తిరిగి సంకేతం_2 == o.సంకేతం_2; }
};

నిర్మాణం move_only_val_t
{
  పూర్ణాంకం val;

  స్పష్టమైన move_only_val_t (పూర్ణాంకం v) : val (v) {}
  move_only_val_t (స్థిరరాశి move_only_val_t&) = తొలగించు;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& క్రియాకారకం = (స్థిరరాశి move_only_val_t&) = తొలగించు;
  move_only_val_t& క్రియాకారకం = (move_only_val_t&& o) { val = o.val; o.val = -1; తిరిగి *ఇది; }
  ~move_only_val_t () = అప్రమేయ;

  తార్కిక క్రియాకారకం == (స్థిరరాశి move_only_val_t& o) స్థిరరాశి { తిరిగి val == o.val; }
};

స్థిర శూన్య test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_ఫలితం_t<పూర్ణాంకం, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().సంకేతం_2 == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.సంకేతం_2 == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_ఫలితం_t<పూర్ణాంకం, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().సంకేతం_2 == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_ఫలితం_t<శూన్య, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().సంకేతం_2 == 789);

    hb_ఫలితం_t<శూన్య, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().సంకేతం_2 == 999);
  }

  // Test value_or with move-only value types
  {
    hb_ఫలితం_t<move_only_val_t, error_సంకేతం_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).విలువ_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_ఫలితం_t<move_only_val_t, error_సంకేతం_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).విలువ_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

నిర్మాణం copy_move_counter_t
{
  స్థిర పూర్ణాంకం copy_లెక్క;
  స్థిర పూర్ణాంకం move_లెక్క;

  స్థిర శూన్య reset ()
  {
    copy_లెక్క = 0;
    move_లెక్క = 0;
  }

  పూర్ణాంకం val;

  తార్కిక లోపల_error() స్థిరరాశి {
    తిరిగి val == -1;
  }

  స్పష్టమైన copy_move_counter_t (పూర్ణాంకం v) : val (v) {}
  copy_move_counter_t (స్థిరరాశి copy_move_counter_t& o) : val (o.val) { copy_లెక్క++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_లెక్క++; }
  ~copy_move_counter_t () = అప్రమేయ;
};

పూర్ణాంకం copy_move_counter_t::copy_లెక్క = 0;
పూర్ణాంకం copy_move_counter_t::move_లెక్క = 0;

స్థిర ఫలితం<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t బహిర్గతం (42);
  తిరిగి Ok (std::move (బహిర్గతం));
}

స్థిర ఫలితం<copy_move_counter_t> return_tracked_from_local_with_ట్యాగ్ ()
{
  copy_move_counter_t బహిర్గతం (42);
  తిరిగి Ok(బహిర్గతం);
}

స్థిర hb_ఫలితం_t<పూర్ణాంకం, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t బహిర్గతం (42);
  తిరిగి Err (std::move (బహిర్గతం));
}

స్థిర hb_ఫలితం_t<పూర్ణాంకం, copy_move_counter_t> return_tracked_error_from_local_with_ట్యాగ్ ()
{
  copy_move_counter_t బహిర్గతం (42);
  తిరిగి Err(బహిర్గతం);
}

స్థిర ఫలితం<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t బహిర్గతం (42);
  తిరిగి Ok (std::move (బహిర్గతం));
}

స్థిర శూన్య test_return_కదలికలు_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    ఫలితం<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_లెక్క == 0);
    hb_always_assert (copy_move_counter_t::move_లెక్క >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    ఫలితం<copy_move_counter_t> r = return_tracked_from_local_with_ట్యాగ్ ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_లెక్క == 0);
    hb_always_assert (copy_move_counter_t::move_లెక్క >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_ఫలితం_t<పూర్ణాంకం, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_లెక్క == 0);
    hb_always_assert (copy_move_counter_t::move_లెక్క >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_ఫలితం_t<పూర్ణాంకం, copy_move_counter_t> r = return_tracked_error_from_local_with_ట్యాగ్ ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_లెక్క == 0);
    hb_always_assert (copy_move_counter_t::move_లెక్క >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    ఫలితం<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

నిర్మాణం లోపల_error_t {
  తార్కిక successful;

  లోపల_error_t(తార్కిక s) : successful(s) {}

  తార్కిక లోపల_error() స్థిరరాశి {
    తిరిగి !successful;
  }
};

స్థిర శూన్య test_from () {
  లోపల_error_t has_error(అబద్ధం);
  లోపల_error_t no_error(నిజం);
  ఫలితం<శూన్య> r = ఫలితం<శూన్య>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = ఫలితం<శూన్య>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

నిర్మాణం default_constructible_t
{
  పూర్ణాంకం val;

  default_constructible_t () : val (123) {}
  స్పష్టమైన default_constructible_t (పూర్ణాంకం v) : val (v) {}
};

నిర్మాణం move_only_default_t
{
  పూర్ణాంకం val;
  move_only_default_t () : val (77) {}
  స్పష్టమైన move_only_default_t (పూర్ణాంకం v) : val (v) {}
  move_only_default_t (స్థిరరాశి move_only_default_t&) = తొలగించు;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& క్రియాకారకం = (స్థిరరాశి move_only_default_t&) = తొలగించు;
  move_only_default_t& క్రియాకారకం = (move_only_default_t&& o) { val = o.val; o.val = -1; తిరిగి *ఇది; }
  ~move_only_default_t () = అప్రమేయ;
};

స్థిర శూన్య test_విలువ_or_default ()
{
  // Test const &
  {
    స్థిరరాశి ఫలితం<పూర్ణాంకం> r_ok = Ok(42);
    hb_always_assert (r_ok.విలువ_or_default () == 42);

    స్థిరరాశి ఫలితం<పూర్ణాంకం> r_err = Err(ERR_A);
    hb_always_assert (r_err.విలువ_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    ఫలితం<పూర్ణాంకం> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).విలువ_or_default () == 42);

    ఫలితం<పూర్ణాంకం> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).విలువ_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    ఫలితం<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.విలువ_or_default ().val == 456);

    ఫలితం<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.విలువ_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).విలువ_or_default ().val == 456);
    hb_always_assert (std::move (r_err).విలువ_or_default ().val == 123);
  }

  // Test pointers
  {
    పూర్ణాంకం x = 10;
    ఫలితం<పూర్ణాంకం*> r_ok = &x;
    hb_always_assert (r_ok.విలువ_or_default () == &x);

    ఫలితం<పూర్ణాంకం*> r_err = ERR_A;
    hb_always_assert (r_err.విలువ_or_default () == శూన్యసూచిక);
  }

  // Test move-only types
  {
    ఫలితం<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).విలువ_or_default ().val == 88);

    ఫలితం<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).విలువ_or_default ().val == 77);
  }
}

స్థిర శూన్య test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    ఫలితం<copy_move_counter_t> r_ok = ఫలితం<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_లెక్క == 0);
    hb_always_assert (copy_move_counter_t::move_లెక్క == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    ఫలితం<copy_move_counter_t> r_fail = ఫలితం<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_లెక్క == 0);
    hb_always_assert (copy_move_counter_t::move_లెక్క == 0);
  }
}

పూర్ణాంకం ప్రధాన ()
{
  test_ok_basic ();
  test_err_basic ();
  test_explicit_ok_err ();
  test_సూచికలు ();
  test_void ();
  test_resource_cleanup ();
  test_try_macro ();
  test_same_or_convertible_types ();
  test_error_interception ();
  test_move_only_types ();
  test_return_కదలికలు_from_local ();
  test_from();
  test_విలువ_or_default ();
  test_from_move ();

  తిరిగి 0;
}
