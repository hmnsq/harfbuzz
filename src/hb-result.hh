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

#ifndef HB_RESULT_HH
#define HB_RESULT_HH

#അന്തര്ഭാവ "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
template <typename T>
ഘടന hb_result_ok_t
{
  T value;
};

template <>
ഘടന hb_result_ok_t<ശൂന്യം> {};

template <typename E>
ഘടന hb_result_err_t
{
  E error;

  template <typename OtherE>
  constexpr തർക്കം operator == (സ്ഥിരം hb_result_err_t<OtherE>& other) സ്ഥിരം { തിരികെ error == other.error; }
  template <typename OtherE>
  constexpr തർക്കം operator != (സ്ഥിരം hb_result_err_t<OtherE>& other) സ്ഥിരം { തിരികെ error != other.error; }
};

template <typename X> ഘടന _hb_is_result_tag : hb_false_type {};
template <typename X> ഘടന _hb_is_result_tag<hb_result_ok_t<X>> : hb_true_type {};
template <typename X> ഘടന _hb_is_result_tag<hb_result_err_t<X>> : hb_true_type {};

template <typename T>
സ്ഥിതം വരിയിൽ constexpr hb_result_ok_t<T&&> Ok (T&& v)
{
  തിരികെ hb_result_ok_t<T&&>{std::forward<T> (v)};
}

സ്ഥിതം വരിയിൽ constexpr hb_result_ok_t<ശൂന്യം> Ok ()
{
  തിരികെ hb_result_ok_t<ശൂന്യം>{};
}

template <typename E>
സ്ഥിതം വരിയിൽ constexpr hb_result_err_t<E&&> Err (E&& e)
{
  തിരികെ hb_result_err_t<E&&>{std::forward<E> (e)};
}

template <typename T, typename E>
ഘടന HB_NODISCARD_STRUCT hb_result_t
{
  സ്വകാര്യ:
  സംഘം
  {
    T val_;
    E err_;
  };
  തർക്കം is_ok_;

  ശൂന്യം destroy ()
  {
    എങ്കിൽ (is_ok_)
      val_.~T ();
    അല്ലെങ്കിൽ
      err_.~E ();
  }

  ശൂന്യം init(സ്ഥിരം hb_result_t& o) {
    is_ok_ = o.is_ok_;
    എങ്കിൽ (is_ok_)
      പുതിയ (std::addressof (val_)) T (o.val_);
    അല്ലെങ്കിൽ
      പുതിയ (std::addressof (err_)) E (o.err_);
  }

  ശൂന്യം init(hb_result_t&& o) {
    is_ok_ = o.is_ok_;
    എങ്കിൽ (is_ok_)
      പുതിയ (std::addressof (val_)) T (std::move (o.val_));
    അല്ലെങ്കിൽ
      പുതിയ (std::addressof (err_)) E (std::move (o.err_));
  }

  പൊതു:
  ~hb_result_t ()
  {
    destroy ();
  }

  hb_result_t () = delete;

  hb_result_t (സ്ഥിരം hb_result_t& o) : is_ok_ (അസത്യം)
  {
    init (o);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (അസത്യം)
  {
    init (std::move (o));
  }

  hb_result_t& operator = (സ്ഥിരം hb_result_t& o)
  {
    എങ്കിൽ (ഇത് == &o) തിരികെ *ഇത്;
    എങ്കിൽ (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    അല്ലെങ്കിൽ
    {
      destroy ();
      init (o);
    }
    തിരികെ *ഇത്;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    എങ്കിൽ (ഇത് == &o) തിരികെ *ഇത്;
    എങ്കിൽ (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    അല്ലെങ്കിൽ
    {
      destroy ();
      init (std::move (o));
    }
    തിരികെ *ഇത്;
  }

  // Construct from Ok(...) tag
  template <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_result_t (hb_result_ok_t<U>&& o) : is_ok_ (സത്യം)
  {
    പുതിയ (std::addressof (val_)) T (std::move (o.value));
  }

  template <typename U,
            hb_enable_if ((std::is_constructible<T, സ്ഥിരം U&>::value))>
  hb_result_t (സ്ഥിരം hb_result_ok_t<U>& o) : is_ok_ (സത്യം)
  {
    പുതിയ (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (അസത്യം)
  {
    പുതിയ (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, സ്ഥിരം F&>::value))>
  hb_result_t (സ്ഥിരം hb_result_err_t<F>& e) : is_ok_ (അസത്യം)
  {
    പുതിയ (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  template <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (F e) : is_ok_ (അസത്യം)
  {
    പുതിയ (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  template <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_result_t) &&
                           !_hb_is_result_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (U&& v) : is_ok_ (സത്യം)
  {
    പുതിയ (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  സ്ഥിതം hb_result_t from (T&& fallible, E error_മൂല്യം) {
    എങ്കിൽ (fallible.in_error()) {
      തിരികെ Err(error_മൂല്യം);
    }
    തിരികെ Ok(fallible);
  }

  തർക്കം is_ok () സ്ഥിരം { തിരികെ is_ok_; }
  തർക്കം is_err () സ്ഥിരം { തിരികെ !is_ok_; }
  explicit operator തർക്കം () സ്ഥിരം { തിരികെ is_ok (); }

  T& value () & { പ്രതിജ്ഞാ (is_ok_); തിരികെ val_; }
  സ്ഥിരം T& value () സ്ഥിരം & { പ്രതിജ്ഞാ (is_ok_); തിരികെ val_; }
  T value () && { പ്രതിജ്ഞാ (is_ok_); തിരികെ std::move (val_); }

  T& operator * () & { തിരികെ value (); }
  സ്ഥിരം T& operator * () സ്ഥിരം & { തിരികെ value (); }
  T operator * () && { തിരികെ std::move (*ഇത്).value (); }

  T* operator -> () { തിരികെ std::addressof (value ()); }
  സ്ഥിരം T* operator -> () സ്ഥിരം { തിരികെ std::addressof (value ()); }

  template <typename U>
  T മൂല്യം_or (U&& default_മൂല്യം) സ്ഥിരം &
  {
    എങ്കിൽ (is_ok_) തിരികെ val_;
    തിരികെ std::forward<U> (default_മൂല്യം);
  }

  template <typename U>
  T മൂല്യം_or (U&& default_മൂല്യം) &&
  {
    എങ്കിൽ (is_ok_) തിരികെ std::move (val_);
    തിരികെ std::forward<U> (default_മൂല്യം);
  }

  T മൂല്യം_or_default () സ്ഥിരം &
  {
    എങ്കിൽ (is_ok_) തിരികെ val_;
    തിരികെ T ();
  }

  T മൂല്യം_or_default () &&
  {
    എങ്കിൽ (is_ok_) തിരികെ std::move (val_);
    തിരികെ T ();
  }

  സ്ഥിരം E& error () സ്ഥിരം & { പ്രതിജ്ഞാ (!is_ok_); തിരികെ err_; }
  E& error () & { പ്രതിജ്ഞാ (!is_ok_); തിരികെ err_; }
  E error () && { പ്രതിജ്ഞാ (!is_ok_); തിരികെ std::move (err_); }

  തർക്കം operator == (സ്ഥിരം hb_result_t& o) സ്ഥിരം
  {
    എങ്കിൽ (is_ok_ != o.is_ok_) തിരികെ അസത്യം;
    എങ്കിൽ (is_ok_) തിരികെ val_ == o.val_;
    തിരികെ err_ == o.err_;
  }

  തർക്കം operator != (സ്ഥിരം hb_result_t& o) സ്ഥിരം
  {
    തിരികെ !(*ഇത് == o);
  }

  template <typename U>
  തർക്കം operator == (സ്ഥിരം hb_result_ok_t<U>& o) സ്ഥിരം
  {
    തിരികെ is_ok_ && val_ == o.value;
  }

  template <typename U>
  തർക്കം operator != (സ്ഥിരം hb_result_ok_t<U>& o) സ്ഥിരം
  {
    തിരികെ !(*ഇത് == o);
  }

  template <typename F>
  തർക്കം operator == (സ്ഥിരം hb_result_err_t<F>& e) സ്ഥിരം
  {
    തിരികെ !is_ok_ && err_ == e.error;
  }

  template <typename F>
  തർക്കം operator != (സ്ഥിരം hb_result_err_t<F>& e) സ്ഥിരം
  {
    തിരികെ !(*ഇത് == e);
  }

  template <typename U>
  തർക്കം operator == (സ്ഥിരം U&) സ്ഥിരം = delete;

  template <typename U>
  തർക്കം operator != (സ്ഥിരം U&) സ്ഥിരം = delete;
};

// Partial specialization for void
template <typename E>
ഘടന HB_NODISCARD_STRUCT hb_result_t<ശൂന്യം, E>
{
  സ്വകാര്യ:
  സംഘം
  {
    E err_;
  };
  തർക്കം is_ok_;

  template <typename, typename> friend ഘടന hb_result_t;

  പൊതു:
  ~hb_result_t ()
  {
    എങ്കിൽ (!is_ok_)
      err_.~E ();
  }

  hb_result_t () : is_ok_ (സത്യം) {}
  hb_result_t (hb_result_ok_t<ശൂന്യം>) : is_ok_ (സത്യം) {}

  hb_result_t (സ്ഥിരം hb_result_t& o) : is_ok_ (o.is_ok_)
  {
    എങ്കിൽ (!is_ok_)
      പുതിയ (std::addressof (err_)) E (o.err_);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    എങ്കിൽ (!is_ok_)
      പുതിയ (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_result_t& operator = (സ്ഥിരം hb_result_t& o)
  {
    എങ്കിൽ (ഇത് == &o) തിരികെ *ഇത്;
    എങ്കിൽ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = സത്യം;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (is_ok_ && !o.is_ok_)
    {
      പുതിയ (std::addressof (err_)) E (o.err_);
      is_ok_ = അസത്യം;
    }
    തിരികെ *ഇത്;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    എങ്കിൽ (ഇത് == &o) തിരികെ *ഇത്;
    എങ്കിൽ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = സത്യം;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (is_ok_ && !o.is_ok_)
    {
      പുതിയ (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = അസത്യം;
    }
    തിരികെ *ഇത്;
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (അസത്യം)
  {
    പുതിയ (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, സ്ഥിരം F&>::value))>
  hb_result_t (സ്ഥിരം hb_result_err_t<F>& e) : is_ok_ (അസത്യം)
  {
    പുതിയ (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_result_t (E e) : is_ok_ (അസത്യം)
  {
    പുതിയ (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  template<typename V>
  സ്ഥിതം hb_result_t<ശൂന്യം, E> from (സ്ഥിരം V& fallible, E error_മൂല്യം) {
    എങ്കിൽ (fallible.in_error()) {
      തിരികെ Err(error_മൂല്യം);
    }
    തിരികെ Ok();
  }

  തർക്കം is_ok () സ്ഥിരം { തിരികെ is_ok_; }
  തർക്കം is_err () സ്ഥിരം { തിരികെ !is_ok_; }
  explicit operator തർക്കം () സ്ഥിരം { തിരികെ is_ok (); }

  സ്ഥിരം E& error () സ്ഥിരം & { പ്രതിജ്ഞാ (!is_ok_); തിരികെ err_; }
  E& error () & { പ്രതിജ്ഞാ (!is_ok_); തിരികെ err_; }
  E error () && { പ്രതിജ്ഞാ (!is_ok_); തിരികെ std::move (err_); }

  ശൂന്യം operator * () & { തിരികെ; }
  ശൂന്യം operator * () സ്ഥിരം & { തിരികെ; }
  ശൂന്യം operator * () && { തിരികെ; }

  തർക്കം operator == (സ്ഥിരം hb_result_t<ശൂന്യം, E>& o) സ്ഥിരം
  {
    തിരികെ is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  തർക്കം operator != (സ്ഥിരം hb_result_t<ശൂന്യം, E>& o) സ്ഥിരം
  {
    തിരികെ !(*ഇത് == o);
  }

  തർക്കം operator == (hb_result_ok_t<ശൂന്യം>) സ്ഥിരം
  {
    തിരികെ is_ok_;
  }

  തർക്കം operator != (hb_result_ok_t<ശൂന്യം>) സ്ഥിരം
  {
    തിരികെ !is_ok_;
  }

  template <typename F>
  തർക്കം operator == (സ്ഥിരം hb_result_err_t<F>& e) സ്ഥിരം
  {
    തിരികെ !is_ok_ && err_ == e.error;
  }

  template <typename F>
  തർക്കം operator != (സ്ഥിരം hb_result_err_t<F>& e) സ്ഥിരം
  {
    തിരികെ !(*ഇത് == e);
  }

  template <typename U>
  തർക്കം operator == (സ്ഥിരം U&) സ്ഥിരം = delete;

  template <typename U>
  തർക്കം operator != (സ്ഥിരം U&) സ്ഥിരം = delete;
};

template <typename U, typename T, typename E>
സ്ഥിതം വരിയിൽ തർക്കം operator == (സ്ഥിരം hb_result_ok_t<U>& o, സ്ഥിരം hb_result_t<T, E>& r)
{
  തിരികെ r == o;
}
template <typename U, typename T, typename E>
സ്ഥിതം വരിയിൽ തർക്കം operator != (സ്ഥിരം hb_result_ok_t<U>& o, സ്ഥിരം hb_result_t<T, E>& r)
{
  തിരികെ r != o;
}
template <typename F, typename T, typename E>
സ്ഥിതം വരിയിൽ തർക്കം operator == (സ്ഥിരം hb_result_err_t<F>& e, സ്ഥിരം hb_result_t<T, E>& r)
{
  തിരികെ r == e;
}
template <typename F, typename T, typename E>
സ്ഥിതം വരിയിൽ തർക്കം operator != (സ്ഥിരം hb_result_err_t<F>& e, സ്ഥിരം hb_result_t<T, E>& r)
{
  തിരികെ r != e;
}

#ifndef TRY
#define TRY(വ്യഞ്ജകം)                                                             \
  ചെയ്യുക {                                                                        \
    auto _res = (വ്യഞ്ജകം);                                                       \
    എങ്കിൽ (unlikely (!_res.is_ok())) തിരികെ Err(std::move(_res).error());        \
  } ആകുമ്പോൾ (0)
#endif

#ifndef TRY_ASSIGN
#define TRY_ASSIGN(var, ...)                                                  \
  auto HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  എങ്കിൽ (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) തിരികെ Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#endif

#endif /* HB_RESULT_HH */
