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

#include "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
template <typename T>
sa_nracanaa hb_result_ok_t
{
  T value;
};

template <>
sa_nracanaa hb_result_ok_t<void> {};

template <typename E>
sa_nracanaa hb_result_err_t
{
  E error;

  template <typename OtherE>
  constexpr bool operator == (const hb_result_err_t<OtherE>& other) const { ꦮꦭꦶ error == other.error; }
  template <typename OtherE>
  constexpr bool operator != (const hb_result_err_t<OtherE>& other) const { ꦮꦭꦶ error != other.error; }
};

template <typename X> sa_nracanaa _hb_is_result_tag : hb_false_type {};
template <typename X> sa_nracanaa _hb_is_result_tag<hb_result_ok_t<X>> : hb_true_type {};
template <typename X> sa_nracanaa _hb_is_result_tag<hb_result_err_t<X>> : hb_true_type {};

template <typename T>
static anta_hstha constexpr hb_result_ok_t<T&&> Ok (T&& v)
{
  ꦮꦭꦶ hb_result_ok_t<T&&>{std::forward<T> (v)};
}

static anta_hstha constexpr hb_result_ok_t<void> Ok ()
{
  ꦮꦭꦶ hb_result_ok_t<void>{};
}

template <typename E>
static anta_hstha constexpr hb_result_err_t<E&&> Err (E&& e)
{
  ꦮꦭꦶ hb_result_err_t<E&&>{std::forward<E> (e)};
}

template <typename T, typename E>
sa_nracanaa HB_NODISCARD_STRUCT hb_result_t
{
  private:
  sa_ngha
  {
    T val_;
    E err_;
  };
  bool is_ok_;

  void destroy ()
  {
    ꦪꦼꦤ꧀ (is_ok_)
      val_.~T ();
    ꦭꦶꦪ
      err_.~E ();
  }

  void init(const hb_result_t& o) {
    is_ok_ = o.is_ok_;
    ꦪꦼꦤ꧀ (is_ok_)
      new (std::addressof (val_)) T (o.val_);
    ꦭꦶꦪ
      new (std::addressof (err_)) E (o.err_);
  }

  void init(hb_result_t&& o) {
    is_ok_ = o.is_ok_;
    ꦪꦼꦤ꧀ (is_ok_)
      new (std::addressof (val_)) T (std::move (o.val_));
    ꦭꦶꦪ
      new (std::addressof (err_)) E (std::move (o.err_));
  }

  public:
  ~hb_result_t ()
  {
    destroy ();
  }

  hb_result_t () = delete;

  hb_result_t (const hb_result_t& o) : is_ok_ (false)
  {
    init (o);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (false)
  {
    init (std::move (o));
  }

  hb_result_t& operator = (const hb_result_t& o)
  {
    ꦪꦼꦤ꧀ (this == &o) ꦮꦭꦶ *this;
    ꦪꦼꦤ꧀ (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ꦭꦶꦪ
    {
      destroy ();
      init (o);
    }
    ꦮꦭꦶ *this;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    ꦪꦼꦤ꧀ (this == &o) ꦮꦭꦶ *this;
    ꦪꦼꦤ꧀ (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ꦭꦶꦪ
    {
      destroy ();
      init (std::move (o));
    }
    ꦮꦭꦶ *this;
  }

  // Construct from Ok(...) tag
  template <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_result_t (hb_result_ok_t<U>&& o) : is_ok_ (true)
  {
    new (std::addressof (val_)) T (std::move (o.value));
  }

  template <typename U,
            hb_enable_if ((std::is_constructible<T, const U&>::value))>
  hb_result_t (const hb_result_ok_t<U>& o) : is_ok_ (true)
  {
    new (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (false)
  {
    new (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, const F&>::value))>
  hb_result_t (const hb_result_err_t<F>& e) : is_ok_ (false)
  {
    new (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  template <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (F e) : is_ok_ (false)
  {
    new (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  template <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_result_t) &&
                           !_hb_is_result_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (U&& v) : is_ok_ (true)
  {
    new (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  static hb_result_t from (T&& fallible, E error_value) {
    ꦪꦼꦤ꧀ (fallible.in_error()) {
      ꦮꦭꦶ Err(error_value);
    }
    ꦮꦭꦶ Ok(fallible);
  }

  bool is_ok () const { ꦮꦭꦶ is_ok_; }
  bool is_err () const { ꦮꦭꦶ !is_ok_; }
  explicit operator bool () const { ꦮꦭꦶ is_ok (); }

  T& value () & { assert (is_ok_); ꦮꦭꦶ val_; }
  const T& value () const & { assert (is_ok_); ꦮꦭꦶ val_; }
  T value () && { assert (is_ok_); ꦮꦭꦶ std::move (val_); }

  T& operator * () & { ꦮꦭꦶ value (); }
  const T& operator * () const & { ꦮꦭꦶ value (); }
  T operator * () && { ꦮꦭꦶ std::move (*this).value (); }

  T* operator -> () { ꦮꦭꦶ std::addressof (value ()); }
  const T* operator -> () const { ꦮꦭꦶ std::addressof (value ()); }

  template <typename U>
  T value_or (U&& default_value) const &
  {
    ꦪꦼꦤ꧀ (is_ok_) ꦮꦭꦶ val_;
    ꦮꦭꦶ std::forward<U> (default_value);
  }

  template <typename U>
  T value_or (U&& default_value) &&
  {
    ꦪꦼꦤ꧀ (is_ok_) ꦮꦭꦶ std::move (val_);
    ꦮꦭꦶ std::forward<U> (default_value);
  }

  T value_or_default () const &
  {
    ꦪꦼꦤ꧀ (is_ok_) ꦮꦭꦶ val_;
    ꦮꦭꦶ T ();
  }

  T value_or_default () &&
  {
    ꦪꦼꦤ꧀ (is_ok_) ꦮꦭꦶ std::move (val_);
    ꦮꦭꦶ T ();
  }

  const E& error () const & { assert (!is_ok_); ꦮꦭꦶ err_; }
  E& error () & { assert (!is_ok_); ꦮꦭꦶ err_; }
  E error () && { assert (!is_ok_); ꦮꦭꦶ std::move (err_); }

  bool operator == (const hb_result_t& o) const
  {
    ꦪꦼꦤ꧀ (is_ok_ != o.is_ok_) ꦮꦭꦶ false;
    ꦪꦼꦤ꧀ (is_ok_) ꦮꦭꦶ val_ == o.val_;
    ꦮꦭꦶ err_ == o.err_;
  }

  bool operator != (const hb_result_t& o) const
  {
    ꦮꦭꦶ !(*this == o);
  }

  template <typename U>
  bool operator == (const hb_result_ok_t<U>& o) const
  {
    ꦮꦭꦶ is_ok_ && val_ == o.value;
  }

  template <typename U>
  bool operator != (const hb_result_ok_t<U>& o) const
  {
    ꦮꦭꦶ !(*this == o);
  }

  template <typename F>
  bool operator == (const hb_result_err_t<F>& e) const
  {
    ꦮꦭꦶ !is_ok_ && err_ == e.error;
  }

  template <typename F>
  bool operator != (const hb_result_err_t<F>& e) const
  {
    ꦮꦭꦶ !(*this == e);
  }

  template <typename U>
  bool operator == (const U&) const = delete;

  template <typename U>
  bool operator != (const U&) const = delete;
};

// Partial specialization for void
template <typename E>
sa_nracanaa HB_NODISCARD_STRUCT hb_result_t<void, E>
{
  private:
  sa_ngha
  {
    E err_;
  };
  bool is_ok_;

  template <typename, typename> friend sa_nracanaa hb_result_t;

  public:
  ~hb_result_t ()
  {
    ꦪꦼꦤ꧀ (!is_ok_)
      err_.~E ();
  }

  hb_result_t () : is_ok_ (true) {}
  hb_result_t (hb_result_ok_t<void>) : is_ok_ (true) {}

  hb_result_t (const hb_result_t& o) : is_ok_ (o.is_ok_)
  {
    ꦪꦼꦤ꧀ (!is_ok_)
      new (std::addressof (err_)) E (o.err_);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    ꦪꦼꦤ꧀ (!is_ok_)
      new (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_result_t& operator = (const hb_result_t& o)
  {
    ꦪꦼꦤ꧀ (this == &o) ꦮꦭꦶ *this;
    ꦪꦼꦤ꧀ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = true;
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (is_ok_ && !o.is_ok_)
    {
      new (std::addressof (err_)) E (o.err_);
      is_ok_ = false;
    }
    ꦮꦭꦶ *this;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    ꦪꦼꦤ꧀ (this == &o) ꦮꦭꦶ *this;
    ꦪꦼꦤ꧀ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = true;
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (is_ok_ && !o.is_ok_)
    {
      new (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = false;
    }
    ꦮꦭꦶ *this;
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (false)
  {
    new (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, const F&>::value))>
  hb_result_t (const hb_result_err_t<F>& e) : is_ok_ (false)
  {
    new (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_result_t (E e) : is_ok_ (false)
  {
    new (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  template<typename V>
  static hb_result_t<void, E> from (const V& fallible, E error_value) {
    ꦪꦼꦤ꧀ (fallible.in_error()) {
      ꦮꦭꦶ Err(error_value);
    }
    ꦮꦭꦶ Ok();
  }

  bool is_ok () const { ꦮꦭꦶ is_ok_; }
  bool is_err () const { ꦮꦭꦶ !is_ok_; }
  explicit operator bool () const { ꦮꦭꦶ is_ok (); }

  const E& error () const & { assert (!is_ok_); ꦮꦭꦶ err_; }
  E& error () & { assert (!is_ok_); ꦮꦭꦶ err_; }
  E error () && { assert (!is_ok_); ꦮꦭꦶ std::move (err_); }

  void operator * () & { ꦮꦭꦶ; }
  void operator * () const & { ꦮꦭꦶ; }
  void operator * () && { ꦮꦭꦶ; }

  bool operator == (const hb_result_t<void, E>& o) const
  {
    ꦮꦭꦶ is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  bool operator != (const hb_result_t<void, E>& o) const
  {
    ꦮꦭꦶ !(*this == o);
  }

  bool operator == (hb_result_ok_t<void>) const
  {
    ꦮꦭꦶ is_ok_;
  }

  bool operator != (hb_result_ok_t<void>) const
  {
    ꦮꦭꦶ !is_ok_;
  }

  template <typename F>
  bool operator == (const hb_result_err_t<F>& e) const
  {
    ꦮꦭꦶ !is_ok_ && err_ == e.error;
  }

  template <typename F>
  bool operator != (const hb_result_err_t<F>& e) const
  {
    ꦮꦭꦶ !(*this == e);
  }

  template <typename U>
  bool operator == (const U&) const = delete;

  template <typename U>
  bool operator != (const U&) const = delete;
};

template <typename U, typename T, typename E>
static anta_hstha bool operator == (const hb_result_ok_t<U>& o, const hb_result_t<T, E>& r)
{
  ꦮꦭꦶ r == o;
}
template <typename U, typename T, typename E>
static anta_hstha bool operator != (const hb_result_ok_t<U>& o, const hb_result_t<T, E>& r)
{
  ꦮꦭꦶ r != o;
}
template <typename F, typename T, typename E>
static anta_hstha bool operator == (const hb_result_err_t<F>& e, const hb_result_t<T, E>& r)
{
  ꦮꦭꦶ r == e;
}
template <typename F, typename T, typename E>
static anta_hstha bool operator != (const hb_result_err_t<F>& e, const hb_result_t<T, E>& r)
{
  ꦮꦭꦶ r != e;
}

#ifndef TRY
#define TRY(expr)                                                             \
  do {                                                                        \
    auto _res = (expr);                                                       \
    ꦪꦼꦤ꧀ (unlikely (!_res.is_ok())) ꦮꦭꦶ Err(std::move(_res).error());        \
  } ꦱꦼꦢꦼꦭ (0)
#endif

#ifndef TRY_ASSIGN
#define TRY_ASSIGN(var, ...)                                                  \
  auto HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  ꦪꦼꦤ꧀ (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) ꦮꦭꦶ Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#endif

#endif /* HB_RESULT_HH */
