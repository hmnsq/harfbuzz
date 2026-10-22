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

#ಅನ್ತರ್ಭಾವ "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
template <typename T>
ಸಂರಚನಾ hb_result_ok_t
{
  T value;
};

template <>
ಸಂರಚನಾ hb_result_ok_t<ಶೂನ್ಯ> {};

template <typename E>
ಸಂರಚನಾ hb_result_err_t
{
  E error;

  template <typename OtherE>
  constexpr ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_result_err_t<OtherE>& other) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ error == other.error; }
  template <typename OtherE>
  constexpr ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_result_err_t<OtherE>& other) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ error != other.error; }
};

template <typename X> ಸಂರಚನಾ _hb_is_result_tag : hb_false_type {};
template <typename X> ಸಂರಚನಾ _hb_is_result_tag<hb_result_ok_t<X>> : hb_true_type {};
template <typename X> ಸಂರಚನಾ _hb_is_result_tag<hb_result_err_t<X>> : hb_true_type {};

template <typename T>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ constexpr hb_result_ok_t<T&&> Ok (T&& v)
{
  ಪ್ರತ್ಯಾಗಮ hb_result_ok_t<T&&>{std::forward<T> (v)};
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ constexpr hb_result_ok_t<ಶೂನ್ಯ> Ok ()
{
  ಪ್ರತ್ಯಾಗಮ hb_result_ok_t<ಶೂನ್ಯ>{};
}

template <typename E>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ constexpr hb_result_err_t<E&&> Err (E&& e)
{
  ಪ್ರತ್ಯಾಗಮ hb_result_err_t<E&&>{std::forward<E> (e)};
}

template <typename T, typename E>
ಸಂರಚನಾ HB_NODISCARD_STRUCT hb_result_t
{
  ಗುಪ್ತ:
  ಸಙ್ಘ
  {
    T val_;
    E err_;
  };
  ತಾರ್ಕಿಕ is_ok_;

  ಶೂನ್ಯ destroy ()
  {
    ಯದಿ (is_ok_)
      val_.~T ();
    ಅನ್ಯಥಾ
      err_.~E ();
  }

  ಶೂನ್ಯ init(ಧ್ರುವ hb_result_t& o) {
    is_ok_ = o.is_ok_;
    ಯದಿ (is_ok_)
      ನೂತನ (std::addressof (val_)) T (o.val_);
    ಅನ್ಯಥಾ
      ನೂತನ (std::addressof (err_)) E (o.err_);
  }

  ಶೂನ್ಯ init(hb_result_t&& o) {
    is_ok_ = o.is_ok_;
    ಯದಿ (is_ok_)
      ನೂತನ (std::addressof (val_)) T (std::move (o.val_));
    ಅನ್ಯಥಾ
      ನೂತನ (std::addressof (err_)) E (std::move (o.err_));
  }

  ಸಾರ್ವಜನಿಕ:
  ~hb_result_t ()
  {
    destroy ();
  }

  hb_result_t () = delete;

  hb_result_t (ಧ್ರುವ hb_result_t& o) : is_ok_ (ಅಸತ್ಯ)
  {
    init (o);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (ಅಸತ್ಯ)
  {
    init (std::move (o));
  }

  hb_result_t& operator = (ಧ್ರುವ hb_result_t& o)
  {
    ಯದಿ (ಏತತ್ == &o) ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    ಯದಿ (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    ಅನ್ಯಥಾ ಯದಿ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ಅನ್ಯಥಾ
    {
      destroy ();
      init (o);
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    ಯದಿ (ಏತತ್ == &o) ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    ಯದಿ (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    ಅನ್ಯಥಾ ಯದಿ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ಅನ್ಯಥಾ
    {
      destroy ();
      init (std::move (o));
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  // Construct from Ok(...) tag
  template <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_result_t (hb_result_ok_t<U>&& o) : is_ok_ (ಸತ್ಯ)
  {
    ನೂತನ (std::addressof (val_)) T (std::move (o.value));
  }

  template <typename U,
            hb_enable_if ((std::is_constructible<T, ಧ್ರುವ U&>::value))>
  hb_result_t (ಧ್ರುವ hb_result_ok_t<U>& o) : is_ok_ (ಸತ್ಯ)
  {
    ನೂತನ (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (ಅಸತ್ಯ)
  {
    ನೂತನ (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ಧ್ರುವ F&>::value))>
  hb_result_t (ಧ್ರುವ hb_result_err_t<F>& e) : is_ok_ (ಅಸತ್ಯ)
  {
    ನೂತನ (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  template <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (F e) : is_ok_ (ಅಸತ್ಯ)
  {
    ನೂತನ (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  template <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_result_t) &&
                           !_hb_is_result_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (U&& v) : is_ok_ (ಸತ್ಯ)
  {
    ನೂತನ (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  ಸ್ಥಿರ hb_result_t from (T&& fallible, E error_ಮೌಲ್ಯ) {
    ಯದಿ (fallible.in_error()) {
      ಪ್ರತ್ಯಾಗಮ Err(error_ಮೌಲ್ಯ);
    }
    ಪ್ರತ್ಯಾಗಮ Ok(fallible);
  }

  ತಾರ್ಕಿಕ is_ok () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ is_ok_; }
  ತಾರ್ಕಿಕ is_err () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !is_ok_; }
  explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ is_ok (); }

  T& value () & { ಪ್ರತಿಜ್ಞಾ (is_ok_); ಪ್ರತ್ಯಾಗಮ val_; }
  ಧ್ರುವ T& value () ಧ್ರುವ & { ಪ್ರತಿಜ್ಞಾ (is_ok_); ಪ್ರತ್ಯಾಗಮ val_; }
  T value () && { ಪ್ರತಿಜ್ಞಾ (is_ok_); ಪ್ರತ್ಯಾಗಮ std::move (val_); }

  T& operator * () & { ಪ್ರತ್ಯಾಗಮ value (); }
  ಧ್ರುವ T& operator * () ಧ್ರುವ & { ಪ್ರತ್ಯಾಗಮ value (); }
  T operator * () && { ಪ್ರತ್ಯಾಗಮ std::move (*ಏತತ್).value (); }

  T* operator -> () { ಪ್ರತ್ಯಾಗಮ std::addressof (value ()); }
  ಧ್ರುವ T* operator -> () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ std::addressof (value ()); }

  template <typename U>
  T ಮೌಲ್ಯ_or (U&& default_ಮೌಲ್ಯ) ಧ್ರುವ &
  {
    ಯದಿ (is_ok_) ಪ್ರತ್ಯಾಗಮ val_;
    ಪ್ರತ್ಯಾಗಮ std::forward<U> (default_ಮೌಲ್ಯ);
  }

  template <typename U>
  T ಮೌಲ್ಯ_or (U&& default_ಮೌಲ್ಯ) &&
  {
    ಯದಿ (is_ok_) ಪ್ರತ್ಯಾಗಮ std::move (val_);
    ಪ್ರತ್ಯಾಗಮ std::forward<U> (default_ಮೌಲ್ಯ);
  }

  T ಮೌಲ್ಯ_or_default () ಧ್ರುವ &
  {
    ಯದಿ (is_ok_) ಪ್ರತ್ಯಾಗಮ val_;
    ಪ್ರತ್ಯಾಗಮ T ();
  }

  T ಮೌಲ್ಯ_or_default () &&
  {
    ಯದಿ (is_ok_) ಪ್ರತ್ಯಾಗಮ std::move (val_);
    ಪ್ರತ್ಯಾಗಮ T ();
  }

  ಧ್ರುವ E& error () ಧ್ರುವ & { ಪ್ರತಿಜ್ಞಾ (!is_ok_); ಪ್ರತ್ಯಾಗಮ err_; }
  E& error () & { ಪ್ರತಿಜ್ಞಾ (!is_ok_); ಪ್ರತ್ಯಾಗಮ err_; }
  E error () && { ಪ್ರತಿಜ್ಞಾ (!is_ok_); ಪ್ರತ್ಯಾಗಮ std::move (err_); }

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_result_t& o) ಧ್ರುವ
  {
    ಯದಿ (is_ok_ != o.is_ok_) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (is_ok_) ಪ್ರತ್ಯಾಗಮ val_ == o.val_;
    ಪ್ರತ್ಯಾಗಮ err_ == o.err_;
  }

  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_result_t& o) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ == o);
  }

  template <typename U>
  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_result_ok_t<U>& o) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ is_ok_ && val_ == o.value;
  }

  template <typename U>
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_result_ok_t<U>& o) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ == o);
  }

  template <typename F>
  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_result_err_t<F>& e) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ !is_ok_ && err_ == e.error;
  }

  template <typename F>
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_result_err_t<F>& e) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ == e);
  }

  template <typename U>
  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ U&) ಧ್ರುವ = delete;

  template <typename U>
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ U&) ಧ್ರುವ = delete;
};

// Partial specialization for void
template <typename E>
ಸಂರಚನಾ HB_NODISCARD_STRUCT hb_result_t<ಶೂನ್ಯ, E>
{
  ಗುಪ್ತ:
  ಸಙ್ಘ
  {
    E err_;
  };
  ತಾರ್ಕಿಕ is_ok_;

  template <typename, typename> friend ಸಂರಚನಾ hb_result_t;

  ಸಾರ್ವಜನಿಕ:
  ~hb_result_t ()
  {
    ಯದಿ (!is_ok_)
      err_.~E ();
  }

  hb_result_t () : is_ok_ (ಸತ್ಯ) {}
  hb_result_t (hb_result_ok_t<ಶೂನ್ಯ>) : is_ok_ (ಸತ್ಯ) {}

  hb_result_t (ಧ್ರುವ hb_result_t& o) : is_ok_ (o.is_ok_)
  {
    ಯದಿ (!is_ok_)
      ನೂತನ (std::addressof (err_)) E (o.err_);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    ಯದಿ (!is_ok_)
      ನೂತನ (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_result_t& operator = (ಧ್ರುವ hb_result_t& o)
  {
    ಯದಿ (ಏತತ್ == &o) ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    ಯದಿ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ಅನ್ಯಥಾ ಯದಿ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = ಸತ್ಯ;
    }
    ಅನ್ಯಥಾ ಯದಿ (is_ok_ && !o.is_ok_)
    {
      ನೂತನ (std::addressof (err_)) E (o.err_);
      is_ok_ = ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    ಯದಿ (ಏತತ್ == &o) ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
    ಯದಿ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ಅನ್ಯಥಾ ಯದಿ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = ಸತ್ಯ;
    }
    ಅನ್ಯಥಾ ಯದಿ (is_ok_ && !o.is_ok_)
    {
      ನೂತನ (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (ಅಸತ್ಯ)
  {
    ನೂತನ (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ಧ್ರುವ F&>::value))>
  hb_result_t (ಧ್ರುವ hb_result_err_t<F>& e) : is_ok_ (ಅಸತ್ಯ)
  {
    ನೂತನ (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_result_t (E e) : is_ok_ (ಅಸತ್ಯ)
  {
    ನೂತನ (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  template<typename V>
  ಸ್ಥಿರ hb_result_t<ಶೂನ್ಯ, E> from (ಧ್ರುವ V& fallible, E error_ಮೌಲ್ಯ) {
    ಯದಿ (fallible.in_error()) {
      ಪ್ರತ್ಯಾಗಮ Err(error_ಮೌಲ್ಯ);
    }
    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  ತಾರ್ಕಿಕ is_ok () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ is_ok_; }
  ತಾರ್ಕಿಕ is_err () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !is_ok_; }
  explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ is_ok (); }

  ಧ್ರುವ E& error () ಧ್ರುವ & { ಪ್ರತಿಜ್ಞಾ (!is_ok_); ಪ್ರತ್ಯಾಗಮ err_; }
  E& error () & { ಪ್ರತಿಜ್ಞಾ (!is_ok_); ಪ್ರತ್ಯಾಗಮ err_; }
  E error () && { ಪ್ರತಿಜ್ಞಾ (!is_ok_); ಪ್ರತ್ಯಾಗಮ std::move (err_); }

  ಶೂನ್ಯ operator * () & { ಪ್ರತ್ಯಾಗಮ; }
  ಶೂನ್ಯ operator * () ಧ್ರುವ & { ಪ್ರತ್ಯಾಗಮ; }
  ಶೂನ್ಯ operator * () && { ಪ್ರತ್ಯಾಗಮ; }

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_result_t<ಶೂನ್ಯ, E>& o) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_result_t<ಶೂನ್ಯ, E>& o) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ == o);
  }

  ತಾರ್ಕಿಕ operator == (hb_result_ok_t<ಶೂನ್ಯ>) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ is_ok_;
  }

  ತಾರ್ಕಿಕ operator != (hb_result_ok_t<ಶೂನ್ಯ>) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ !is_ok_;
  }

  template <typename F>
  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_result_err_t<F>& e) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ !is_ok_ && err_ == e.error;
  }

  template <typename F>
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_result_err_t<F>& e) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ == e);
  }

  template <typename U>
  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ U&) ಧ್ರುವ = delete;

  template <typename U>
  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ U&) ಧ್ರುವ = delete;
};

template <typename U, typename T, typename E>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_result_ok_t<U>& o, ಧ್ರುವ hb_result_t<T, E>& r)
{
  ಪ್ರತ್ಯಾಗಮ r == o;
}
template <typename U, typename T, typename E>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_result_ok_t<U>& o, ಧ್ರುವ hb_result_t<T, E>& r)
{
  ಪ್ರತ್ಯಾಗಮ r != o;
}
template <typename F, typename T, typename E>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_result_err_t<F>& e, ಧ್ರುವ hb_result_t<T, E>& r)
{
  ಪ್ರತ್ಯಾಗಮ r == e;
}
template <typename F, typename T, typename E>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_result_err_t<F>& e, ಧ್ರುವ hb_result_t<T, E>& r)
{
  ಪ್ರತ್ಯಾಗಮ r != e;
}

#ifndef TRY
#define TRY(ಅಭಿವ್ಯಕ್ತಿ)                                                             \
  ಕುರು {                                                                        \
    auto _res = (ಅಭಿವ್ಯಕ್ತಿ);                                                       \
    ಯದಿ (unlikely (!_res.is_ok())) ಪ್ರತ್ಯಾಗಮ Err(std::move(_res).error());        \
  } ಯಾವತ್ (0)
#endif

#ifndef TRY_ASSIGN
#define TRY_ASSIGN(var, ...)                                                  \
  auto HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  ಯದಿ (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) ಪ್ರತ್ಯಾಗಮ Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#endif

#endif /* HB_RESULT_HH */
