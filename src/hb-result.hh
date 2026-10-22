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

#నిర్వచితం_కాకపోతే HB_ఫలితం_HH
#నిర్వచించు HB_ఫలితం_HH

#అన్తర్భావ "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
మూస <రకం_పేరు T>
నిర్మాణం hb_ఫలితం_ok_t
{
  T value;
};

మూస <>
నిర్మాణం hb_ఫలితం_ok_t<శూన్య> {};

మూస <రకం_పేరు E>
నిర్మాణం hb_ఫలితం_err_t
{
  E error;

  మూస <రకం_పేరు OtherE>
  constexpr తార్కిక క్రియాకారకం == (స్థిరరాశి hb_ఫలితం_err_t<OtherE>& other) స్థిరరాశి { తిరిగి error == other.error; }
  మూస <రకం_పేరు OtherE>
  constexpr తార్కిక క్రియాకారకం != (స్థిరరాశి hb_ఫలితం_err_t<OtherE>& other) స్థిరరాశి { తిరిగి error != other.error; }
};

మూస <రకం_పేరు X> నిర్మాణం hb_is_ఫలితం_ట్యాగ్ : hb_false_type {};
మూస <రకం_పేరు X> నిర్మాణం hb_is_ఫలితం_ట్యాగ్<hb_ఫలితం_ok_t<X>> : hb_true_type {};
మూస <రకం_పేరు X> నిర్మాణం hb_is_ఫలితం_ట్యాగ్<hb_ఫలితం_err_t<X>> : hb_true_type {};

మూస <రకం_పేరు T>
స్థిర అంతఃవరుస constexpr hb_ఫలితం_ok_t<T&&> Ok (T&& v)
{
  తిరిగి hb_ఫలితం_ok_t<T&&>{std::forward<T> (v)};
}

స్థిర అంతఃవరుస constexpr hb_ఫలితం_ok_t<శూన్య> Ok ()
{
  తిరిగి hb_ఫలితం_ok_t<శూన్య>{};
}

మూస <రకం_పేరు E>
స్థిర అంతఃవరుస constexpr hb_ఫలితం_err_t<E&&> Err (E&& e)
{
  తిరిగి hb_ఫలితం_err_t<E&&>{std::forward<E> (e)};
}

మూస <రకం_పేరు T, రకం_పేరు E>
నిర్మాణం HB_NODISCARD_STRUCT hb_ఫలితం_t
{
  అంతరంగ:
  సంఘం
  {
    T val_;
    E err_;
  };
  తార్కిక is_ok_;

  శూన్య destroy ()
  {
    ఒకవేళ (is_ok_)
      val_.~T ();
    లేకపోతే
      err_.~E ();
  }

  శూన్య init(స్థిరరాశి hb_ఫలితం_t& o) {
    is_ok_ = o.is_ok_;
    ఒకవేళ (is_ok_)
      కొత్త (std::addressof (val_)) T (o.val_);
    లేకపోతే
      కొత్త (std::addressof (err_)) E (o.err_);
  }

  శూన్య init(hb_ఫలితం_t&& o) {
    is_ok_ = o.is_ok_;
    ఒకవేళ (is_ok_)
      కొత్త (std::addressof (val_)) T (std::move (o.val_));
    లేకపోతే
      కొత్త (std::addressof (err_)) E (std::move (o.err_));
  }

  బహిరంగ:
  ~hb_ఫలితం_t ()
  {
    destroy ();
  }

  hb_ఫలితం_t () = తొలగించు;

  hb_ఫలితం_t (స్థిరరాశి hb_ఫలితం_t& o) : is_ok_ (అబద్ధం)
  {
    init (o);
  }

  hb_ఫలితం_t (hb_ఫలితం_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (అబద్ధం)
  {
    init (std::move (o));
  }

  hb_ఫలితం_t& క్రియాకారకం = (స్థిరరాశి hb_ఫలితం_t& o)
  {
    ఒకవేళ (ఇది == &o) తిరిగి *ఇది;
    ఒకవేళ (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    లేకపోతే ఒకవేళ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    లేకపోతే
    {
      destroy ();
      init (o);
    }
    తిరిగి *ఇది;
  }

  hb_ఫలితం_t& క్రియాకారకం = (hb_ఫలితం_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    ఒకవేళ (ఇది == &o) తిరిగి *ఇది;
    ఒకవేళ (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    లేకపోతే ఒకవేళ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    లేకపోతే
    {
      destroy ();
      init (std::move (o));
    }
    తిరిగి *ఇది;
  }

  // Construct from Ok(...) tag
  మూస <రకం_పేరు U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_ఫలితం_t (hb_ఫలితం_ok_t<U>&& o) : is_ok_ (నిజం)
  {
    కొత్త (std::addressof (val_)) T (std::move (o.value));
  }

  మూస <రకం_పేరు U,
            hb_enable_if ((std::is_constructible<T, స్థిరరాశి U&>::value))>
  hb_ఫలితం_t (స్థిరరాశి hb_ఫలితం_ok_t<U>& o) : is_ok_ (నిజం)
  {
    కొత్త (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  మూస <రకం_పేరు F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_ఫలితం_t (hb_ఫలితం_err_t<F>&& e) : is_ok_ (అబద్ధం)
  {
    కొత్త (std::addressof (err_)) E (std::move (e.error));
  }

  మూస <రకం_పేరు F,
            hb_enable_if ((std::is_constructible<E, స్థిరరాశి F&>::value))>
  hb_ఫలితం_t (స్థిరరాశి hb_ఫలితం_err_t<F>& e) : is_ok_ (అబద్ధం)
  {
    కొత్త (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  మూస <రకం_పేరు F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_ఫలితం_t (F e) : is_ok_ (అబద్ధం)
  {
    కొత్త (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  మూస <రకం_పేరు U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_ఫలితం_t) &&
                           !hb_is_ఫలితం_ట్యాగ్<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_ఫలితం_t (U&& v) : is_ok_ (నిజం)
  {
    కొత్త (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  స్థిర hb_ఫలితం_t from (T&& fallible, E error_విలువ) {
    ఒకవేళ (fallible.లోపల_error()) {
      తిరిగి Err(error_విలువ);
    }
    తిరిగి Ok(fallible);
  }

  తార్కిక is_ok () స్థిరరాశి { తిరిగి is_ok_; }
  తార్కిక is_err () స్థిరరాశి { తిరిగి !is_ok_; }
  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి is_ok (); }

  T& value () & { ప్రతిజ్ఞా (is_ok_); తిరిగి val_; }
  స్థిరరాశి T& value () స్థిరరాశి & { ప్రతిజ్ఞా (is_ok_); తిరిగి val_; }
  T value () && { ప్రతిజ్ఞా (is_ok_); తిరిగి std::move (val_); }

  T& క్రియాకారకం * () & { తిరిగి value (); }
  స్థిరరాశి T& క్రియాకారకం * () స్థిరరాశి & { తిరిగి value (); }
  T క్రియాకారకం * () && { తిరిగి std::move (*ఇది).value (); }

  T* క్రియాకారకం -> () { తిరిగి std::addressof (value ()); }
  స్థిరరాశి T* క్రియాకారకం -> () స్థిరరాశి { తిరిగి std::addressof (value ()); }

  మూస <రకం_పేరు U>
  T విలువ_or (U&& default_విలువ) స్థిరరాశి &
  {
    ఒకవేళ (is_ok_) తిరిగి val_;
    తిరిగి std::forward<U> (default_విలువ);
  }

  మూస <రకం_పేరు U>
  T విలువ_or (U&& default_విలువ) &&
  {
    ఒకవేళ (is_ok_) తిరిగి std::move (val_);
    తిరిగి std::forward<U> (default_విలువ);
  }

  T విలువ_or_default () స్థిరరాశి &
  {
    ఒకవేళ (is_ok_) తిరిగి val_;
    తిరిగి T ();
  }

  T విలువ_or_default () &&
  {
    ఒకవేళ (is_ok_) తిరిగి std::move (val_);
    తిరిగి T ();
  }

  స్థిరరాశి E& error () స్థిరరాశి & { ప్రతిజ్ఞా (!is_ok_); తిరిగి err_; }
  E& error () & { ప్రతిజ్ఞా (!is_ok_); తిరిగి err_; }
  E error () && { ప్రతిజ్ఞా (!is_ok_); తిరిగి std::move (err_); }

  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_ఫలితం_t& o) స్థిరరాశి
  {
    ఒకవేళ (is_ok_ != o.is_ok_) తిరిగి అబద్ధం;
    ఒకవేళ (is_ok_) తిరిగి val_ == o.val_;
    తిరిగి err_ == o.err_;
  }

  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_ఫలితం_t& o) స్థిరరాశి
  {
    తిరిగి !(*ఇది == o);
  }

  మూస <రకం_పేరు U>
  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_ఫలితం_ok_t<U>& o) స్థిరరాశి
  {
    తిరిగి is_ok_ && val_ == o.value;
  }

  మూస <రకం_పేరు U>
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_ఫలితం_ok_t<U>& o) స్థిరరాశి
  {
    తిరిగి !(*ఇది == o);
  }

  మూస <రకం_పేరు F>
  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_ఫలితం_err_t<F>& e) స్థిరరాశి
  {
    తిరిగి !is_ok_ && err_ == e.error;
  }

  మూస <రకం_పేరు F>
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_ఫలితం_err_t<F>& e) స్థిరరాశి
  {
    తిరిగి !(*ఇది == e);
  }

  మూస <రకం_పేరు U>
  తార్కిక క్రియాకారకం == (స్థిరరాశి U&) స్థిరరాశి = తొలగించు;

  మూస <రకం_పేరు U>
  తార్కిక క్రియాకారకం != (స్థిరరాశి U&) స్థిరరాశి = తొలగించు;
};

// Partial specialization for void
మూస <రకం_పేరు E>
నిర్మాణం HB_NODISCARD_STRUCT hb_ఫలితం_t<శూన్య, E>
{
  అంతరంగ:
  సంఘం
  {
    E err_;
  };
  తార్కిక is_ok_;

  మూస <రకం_పేరు, రకం_పేరు> మిత్ర నిర్మాణం hb_ఫలితం_t;

  బహిరంగ:
  ~hb_ఫలితం_t ()
  {
    ఒకవేళ (!is_ok_)
      err_.~E ();
  }

  hb_ఫలితం_t () : is_ok_ (నిజం) {}
  hb_ఫలితం_t (hb_ఫలితం_ok_t<శూన్య>) : is_ok_ (నిజం) {}

  hb_ఫలితం_t (స్థిరరాశి hb_ఫలితం_t& o) : is_ok_ (o.is_ok_)
  {
    ఒకవేళ (!is_ok_)
      కొత్త (std::addressof (err_)) E (o.err_);
  }

  hb_ఫలితం_t (hb_ఫలితం_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    ఒకవేళ (!is_ok_)
      కొత్త (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_ఫలితం_t& క్రియాకారకం = (స్థిరరాశి hb_ఫలితం_t& o)
  {
    ఒకవేళ (ఇది == &o) తిరిగి *ఇది;
    ఒకవేళ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    లేకపోతే ఒకవేళ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = నిజం;
    }
    లేకపోతే ఒకవేళ (is_ok_ && !o.is_ok_)
    {
      కొత్త (std::addressof (err_)) E (o.err_);
      is_ok_ = అబద్ధం;
    }
    తిరిగి *ఇది;
  }

  hb_ఫలితం_t& క్రియాకారకం = (hb_ఫలితం_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    ఒకవేళ (ఇది == &o) తిరిగి *ఇది;
    ఒకవేళ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    లేకపోతే ఒకవేళ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = నిజం;
    }
    లేకపోతే ఒకవేళ (is_ok_ && !o.is_ok_)
    {
      కొత్త (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = అబద్ధం;
    }
    తిరిగి *ఇది;
  }

  మూస <రకం_పేరు F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_ఫలితం_t (hb_ఫలితం_err_t<F>&& e) : is_ok_ (అబద్ధం)
  {
    కొత్త (std::addressof (err_)) E (std::move (e.error));
  }

  మూస <రకం_పేరు F,
            hb_enable_if ((std::is_constructible<E, స్థిరరాశి F&>::value))>
  hb_ఫలితం_t (స్థిరరాశి hb_ఫలితం_err_t<F>& e) : is_ok_ (అబద్ధం)
  {
    కొత్త (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_ఫలితం_t (E e) : is_ok_ (అబద్ధం)
  {
    కొత్త (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  మూస<రకం_పేరు V>
  స్థిర hb_ఫలితం_t<శూన్య, E> from (స్థిరరాశి V& fallible, E error_విలువ) {
    ఒకవేళ (fallible.లోపల_error()) {
      తిరిగి Err(error_విలువ);
    }
    తిరిగి Ok();
  }

  తార్కిక is_ok () స్థిరరాశి { తిరిగి is_ok_; }
  తార్కిక is_err () స్థిరరాశి { తిరిగి !is_ok_; }
  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి is_ok (); }

  స్థిరరాశి E& error () స్థిరరాశి & { ప్రతిజ్ఞా (!is_ok_); తిరిగి err_; }
  E& error () & { ప్రతిజ్ఞా (!is_ok_); తిరిగి err_; }
  E error () && { ప్రతిజ్ఞా (!is_ok_); తిరిగి std::move (err_); }

  శూన్య క్రియాకారకం * () & { తిరిగి; }
  శూన్య క్రియాకారకం * () స్థిరరాశి & { తిరిగి; }
  శూన్య క్రియాకారకం * () && { తిరిగి; }

  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_ఫలితం_t<శూన్య, E>& o) స్థిరరాశి
  {
    తిరిగి is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_ఫలితం_t<శూన్య, E>& o) స్థిరరాశి
  {
    తిరిగి !(*ఇది == o);
  }

  తార్కిక క్రియాకారకం == (hb_ఫలితం_ok_t<శూన్య>) స్థిరరాశి
  {
    తిరిగి is_ok_;
  }

  తార్కిక క్రియాకారకం != (hb_ఫలితం_ok_t<శూన్య>) స్థిరరాశి
  {
    తిరిగి !is_ok_;
  }

  మూస <రకం_పేరు F>
  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_ఫలితం_err_t<F>& e) స్థిరరాశి
  {
    తిరిగి !is_ok_ && err_ == e.error;
  }

  మూస <రకం_పేరు F>
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_ఫలితం_err_t<F>& e) స్థిరరాశి
  {
    తిరిగి !(*ఇది == e);
  }

  మూస <రకం_పేరు U>
  తార్కిక క్రియాకారకం == (స్థిరరాశి U&) స్థిరరాశి = తొలగించు;

  మూస <రకం_పేరు U>
  తార్కిక క్రియాకారకం != (స్థిరరాశి U&) స్థిరరాశి = తొలగించు;
};

మూస <రకం_పేరు U, రకం_పేరు T, రకం_పేరు E>
స్థిర అంతఃవరుస తార్కిక క్రియాకారకం == (స్థిరరాశి hb_ఫలితం_ok_t<U>& o, స్థిరరాశి hb_ఫలితం_t<T, E>& r)
{
  తిరిగి r == o;
}
మూస <రకం_పేరు U, రకం_పేరు T, రకం_పేరు E>
స్థిర అంతఃవరుస తార్కిక క్రియాకారకం != (స్థిరరాశి hb_ఫలితం_ok_t<U>& o, స్థిరరాశి hb_ఫలితం_t<T, E>& r)
{
  తిరిగి r != o;
}
మూస <రకం_పేరు F, రకం_పేరు T, రకం_పేరు E>
స్థిర అంతఃవరుస తార్కిక క్రియాకారకం == (స్థిరరాశి hb_ఫలితం_err_t<F>& e, స్థిరరాశి hb_ఫలితం_t<T, E>& r)
{
  తిరిగి r == e;
}
మూస <రకం_పేరు F, రకం_పేరు T, రకం_పేరు E>
స్థిర అంతఃవరుస తార్కిక క్రియాకారకం != (స్థిరరాశి hb_ఫలితం_err_t<F>& e, స్థిరరాశి hb_ఫలితం_t<T, E>& r)
{
  తిరిగి r != e;
}

#నిర్వచితం_కాకపోతే TRY
#నిర్వచించు TRY(సమాసం)                                                             \
  చేయి {                                                                        \
    స్వయంచాలక _res = (సమాసం);                                                       \
    ఒకవేళ (unlikely (!_res.is_ok())) తిరిగి Err(std::move(_res).error());        \
  } అయితే (0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే TRY_కేటాయించు
#నిర్వచించు TRY_కేటాయించు(var, ...)                                                  \
  స్వయంచాలక HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  ఒకవేళ (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) తిరిగి Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#అయితే_ముగింపు

#అయితే_ముగింపు /* HB_RESULT_HH */
