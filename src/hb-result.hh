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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਨਤੀਜਾ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਨਤੀਜਾ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_ਨਤੀਜਾ_ok_t
{
  T value;
};

ਸਾਂਚਾ <>
ਬਣਤਰ hb_ਨਤੀਜਾ_ok_t<ਖਾਲੀ> {};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ E>
ਬਣਤਰ hb_ਨਤੀਜਾ_err_t
{
  E error;

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ OtherE>
  constexpr ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਨਤੀਜਾ_err_t<OtherE>& other) ਅਚਲ { ਵਾਪਸ error == other.error; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ OtherE>
  constexpr ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_ਨਤੀਜਾ_err_t<OtherE>& other) ਅਚਲ { ਵਾਪਸ error != other.error; }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ X> ਬਣਤਰ hb_is_ਨਤੀਜਾ_ਟੈਗ : hb_false_type {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ X> ਬਣਤਰ hb_is_ਨਤੀਜਾ_ਟੈਗ<hb_ਨਤੀਜਾ_ok_t<X>> : hb_true_type {};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ X> ਬਣਤਰ hb_is_ਨਤੀਜਾ_ਟੈਗ<hb_ਨਤੀਜਾ_err_t<X>> : hb_true_type {};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ constexpr hb_ਨਤੀਜਾ_ok_t<T&&> Ok (T&& v)
{
  ਵਾਪਸ hb_ਨਤੀਜਾ_ok_t<T&&>{std::forward<T> (v)};
}

ਸਥਿਰ ਸਤਰਅੰਦਰ constexpr hb_ਨਤੀਜਾ_ok_t<ਖਾਲੀ> Ok ()
{
  ਵਾਪਸ hb_ਨਤੀਜਾ_ok_t<ਖਾਲੀ>{};
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ E>
ਸਥਿਰ ਸਤਰਅੰਦਰ constexpr hb_ਨਤੀਜਾ_err_t<E&&> Err (E&& e)
{
  ਵਾਪਸ hb_ਨਤੀਜਾ_err_t<E&&>{std::forward<E> (e)};
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ E>
ਬਣਤਰ HB_NODISCARD_STRUCT hb_ਨਤੀਜਾ_t
{
  ਨਿਜੀ:
  ਸੰਘ
  {
    T val_;
    E err_;
  };
  ਤਾਰਕਿਕ is_ok_;

  ਖਾਲੀ destroy ()
  {
    ਜੇ (is_ok_)
      val_.~T ();
    ਨਹੀਂਤਾਂ
      err_.~E ();
  }

  ਖਾਲੀ init(ਅਚਲ hb_ਨਤੀਜਾ_t& o) {
    is_ok_ = o.is_ok_;
    ਜੇ (is_ok_)
      ਨਵਾਂ (std::addressof (val_)) T (o.val_);
    ਨਹੀਂਤਾਂ
      ਨਵਾਂ (std::addressof (err_)) E (o.err_);
  }

  ਖਾਲੀ init(hb_ਨਤੀਜਾ_t&& o) {
    is_ok_ = o.is_ok_;
    ਜੇ (is_ok_)
      ਨਵਾਂ (std::addressof (val_)) T (std::move (o.val_));
    ਨਹੀਂਤਾਂ
      ਨਵਾਂ (std::addressof (err_)) E (std::move (o.err_));
  }

  ਜਨਤਕ:
  ~hb_ਨਤੀਜਾ_t ()
  {
    destroy ();
  }

  hb_ਨਤੀਜਾ_t () = ਮਿਟਾਓ;

  hb_ਨਤੀਜਾ_t (ਅਚਲ hb_ਨਤੀਜਾ_t& o) : is_ok_ (ਝੂਠ)
  {
    init (o);
  }

  hb_ਨਤੀਜਾ_t (hb_ਨਤੀਜਾ_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (ਝੂਠ)
  {
    init (std::move (o));
  }

  hb_ਨਤੀਜਾ_t& ਸੰਚਾਲਕ = (ਅਚਲ hb_ਨਤੀਜਾ_t& o)
  {
    ਜੇ (ਇਹ == &o) ਵਾਪਸ *ਇਹ;
    ਜੇ (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    ਨਹੀਂਤਾਂ ਜੇ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ਨਹੀਂਤਾਂ
    {
      destroy ();
      init (o);
    }
    ਵਾਪਸ *ਇਹ;
  }

  hb_ਨਤੀਜਾ_t& ਸੰਚਾਲਕ = (hb_ਨਤੀਜਾ_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    ਜੇ (ਇਹ == &o) ਵਾਪਸ *ਇਹ;
    ਜੇ (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    ਨਹੀਂਤਾਂ ਜੇ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ਨਹੀਂਤਾਂ
    {
      destroy ();
      init (std::move (o));
    }
    ਵਾਪਸ *ਇਹ;
  }

  // Construct from Ok(...) tag
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_ਨਤੀਜਾ_t (hb_ਨਤੀਜਾ_ok_t<U>&& o) : is_ok_ (ਸੱਚ)
  {
    ਨਵਾਂ (std::addressof (val_)) T (std::move (o.value));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U,
            hb_enable_if ((std::is_constructible<T, ਅਚਲ U&>::value))>
  hb_ਨਤੀਜਾ_t (ਅਚਲ hb_ਨਤੀਜਾ_ok_t<U>& o) : is_ok_ (ਸੱਚ)
  {
    ਨਵਾਂ (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_ਨਤੀਜਾ_t (hb_ਨਤੀਜਾ_err_t<F>&& e) : is_ok_ (ਝੂਠ)
  {
    ਨਵਾਂ (std::addressof (err_)) E (std::move (e.error));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F,
            hb_enable_if ((std::is_constructible<E, ਅਚਲ F&>::value))>
  hb_ਨਤੀਜਾ_t (ਅਚਲ hb_ਨਤੀਜਾ_err_t<F>& e) : is_ok_ (ਝੂਠ)
  {
    ਨਵਾਂ (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_ਨਤੀਜਾ_t (F e) : is_ok_ (ਝੂਠ)
  {
    ਨਵਾਂ (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_ਨਤੀਜਾ_t) &&
                           !hb_is_ਨਤੀਜਾ_ਟੈਗ<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_ਨਤੀਜਾ_t (U&& v) : is_ok_ (ਸੱਚ)
  {
    ਨਵਾਂ (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  ਸਥਿਰ hb_ਨਤੀਜਾ_t from (T&& fallible, E error_ਮੁੱਲ) {
    ਜੇ (fallible.ਵਿੱਚ_error()) {
      ਵਾਪਸ Err(error_ਮੁੱਲ);
    }
    ਵਾਪਸ Ok(fallible);
  }

  ਤਾਰਕਿਕ is_ok () ਅਚਲ { ਵਾਪਸ is_ok_; }
  ਤਾਰਕਿਕ is_err () ਅਚਲ { ਵਾਪਸ !is_ok_; }
  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ is_ok (); }

  T& value () & { ਪ੍ਰਤਿਜ੍ਞਾ (is_ok_); ਵਾਪਸ val_; }
  ਅਚਲ T& value () ਅਚਲ & { ਪ੍ਰਤਿਜ੍ਞਾ (is_ok_); ਵਾਪਸ val_; }
  T value () && { ਪ੍ਰਤਿਜ੍ਞਾ (is_ok_); ਵਾਪਸ std::move (val_); }

  T& ਸੰਚਾਲਕ * () & { ਵਾਪਸ value (); }
  ਅਚਲ T& ਸੰਚਾਲਕ * () ਅਚਲ & { ਵਾਪਸ value (); }
  T ਸੰਚਾਲਕ * () && { ਵਾਪਸ std::move (*ਇਹ).value (); }

  T* ਸੰਚਾਲਕ -> () { ਵਾਪਸ std::addressof (value ()); }
  ਅਚਲ T* ਸੰਚਾਲਕ -> () ਅਚਲ { ਵਾਪਸ std::addressof (value ()); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U>
  T ਮੁੱਲ_or (U&& default_ਮੁੱਲ) ਅਚਲ &
  {
    ਜੇ (is_ok_) ਵਾਪਸ val_;
    ਵਾਪਸ std::forward<U> (default_ਮੁੱਲ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U>
  T ਮੁੱਲ_or (U&& default_ਮੁੱਲ) &&
  {
    ਜੇ (is_ok_) ਵਾਪਸ std::move (val_);
    ਵਾਪਸ std::forward<U> (default_ਮੁੱਲ);
  }

  T ਮੁੱਲ_or_default () ਅਚਲ &
  {
    ਜੇ (is_ok_) ਵਾਪਸ val_;
    ਵਾਪਸ T ();
  }

  T ਮੁੱਲ_or_default () &&
  {
    ਜੇ (is_ok_) ਵਾਪਸ std::move (val_);
    ਵਾਪਸ T ();
  }

  ਅਚਲ E& error () ਅਚਲ & { ਪ੍ਰਤਿਜ੍ਞਾ (!is_ok_); ਵਾਪਸ err_; }
  E& error () & { ਪ੍ਰਤਿਜ੍ਞਾ (!is_ok_); ਵਾਪਸ err_; }
  E error () && { ਪ੍ਰਤਿਜ੍ਞਾ (!is_ok_); ਵਾਪਸ std::move (err_); }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਨਤੀਜਾ_t& o) ਅਚਲ
  {
    ਜੇ (is_ok_ != o.is_ok_) ਵਾਪਸ ਝੂਠ;
    ਜੇ (is_ok_) ਵਾਪਸ val_ == o.val_;
    ਵਾਪਸ err_ == o.err_;
  }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_ਨਤੀਜਾ_t& o) ਅਚਲ
  {
    ਵਾਪਸ !(*ਇਹ == o);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਨਤੀਜਾ_ok_t<U>& o) ਅਚਲ
  {
    ਵਾਪਸ is_ok_ && val_ == o.value;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_ਨਤੀਜਾ_ok_t<U>& o) ਅਚਲ
  {
    ਵਾਪਸ !(*ਇਹ == o);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਨਤੀਜਾ_err_t<F>& e) ਅਚਲ
  {
    ਵਾਪਸ !is_ok_ && err_ == e.error;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_ਨਤੀਜਾ_err_t<F>& e) ਅਚਲ
  {
    ਵਾਪਸ !(*ਇਹ == e);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ U&) ਅਚਲ = ਮਿਟਾਓ;

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ U&) ਅਚਲ = ਮਿਟਾਓ;
};

// Partial specialization for void
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ E>
ਬਣਤਰ HB_NODISCARD_STRUCT hb_ਨਤੀਜਾ_t<ਖਾਲੀ, E>
{
  ਨਿਜੀ:
  ਸੰਘ
  {
    E err_;
  };
  ਤਾਰਕਿਕ is_ok_;

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ, ਕਿਸਮ_ਨਾਂ> ਮਿੱਤਰ ਬਣਤਰ hb_ਨਤੀਜਾ_t;

  ਜਨਤਕ:
  ~hb_ਨਤੀਜਾ_t ()
  {
    ਜੇ (!is_ok_)
      err_.~E ();
  }

  hb_ਨਤੀਜਾ_t () : is_ok_ (ਸੱਚ) {}
  hb_ਨਤੀਜਾ_t (hb_ਨਤੀਜਾ_ok_t<ਖਾਲੀ>) : is_ok_ (ਸੱਚ) {}

  hb_ਨਤੀਜਾ_t (ਅਚਲ hb_ਨਤੀਜਾ_t& o) : is_ok_ (o.is_ok_)
  {
    ਜੇ (!is_ok_)
      ਨਵਾਂ (std::addressof (err_)) E (o.err_);
  }

  hb_ਨਤੀਜਾ_t (hb_ਨਤੀਜਾ_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    ਜੇ (!is_ok_)
      ਨਵਾਂ (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_ਨਤੀਜਾ_t& ਸੰਚਾਲਕ = (ਅਚਲ hb_ਨਤੀਜਾ_t& o)
  {
    ਜੇ (ਇਹ == &o) ਵਾਪਸ *ਇਹ;
    ਜੇ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ਨਹੀਂਤਾਂ ਜੇ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = ਸੱਚ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (is_ok_ && !o.is_ok_)
    {
      ਨਵਾਂ (std::addressof (err_)) E (o.err_);
      is_ok_ = ਝੂਠ;
    }
    ਵਾਪਸ *ਇਹ;
  }

  hb_ਨਤੀਜਾ_t& ਸੰਚਾਲਕ = (hb_ਨਤੀਜਾ_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    ਜੇ (ਇਹ == &o) ਵਾਪਸ *ਇਹ;
    ਜੇ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ਨਹੀਂਤਾਂ ਜੇ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = ਸੱਚ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (is_ok_ && !o.is_ok_)
    {
      ਨਵਾਂ (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = ਝੂਠ;
    }
    ਵਾਪਸ *ਇਹ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_ਨਤੀਜਾ_t (hb_ਨਤੀਜਾ_err_t<F>&& e) : is_ok_ (ਝੂਠ)
  {
    ਨਵਾਂ (std::addressof (err_)) E (std::move (e.error));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F,
            hb_enable_if ((std::is_constructible<E, ਅਚਲ F&>::value))>
  hb_ਨਤੀਜਾ_t (ਅਚਲ hb_ਨਤੀਜਾ_err_t<F>& e) : is_ok_ (ਝੂਠ)
  {
    ਨਵਾਂ (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_ਨਤੀਜਾ_t (E e) : is_ok_ (ਝੂਠ)
  {
    ਨਵਾਂ (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ V>
  ਸਥਿਰ hb_ਨਤੀਜਾ_t<ਖਾਲੀ, E> from (ਅਚਲ V& fallible, E error_ਮੁੱਲ) {
    ਜੇ (fallible.ਵਿੱਚ_error()) {
      ਵਾਪਸ Err(error_ਮੁੱਲ);
    }
    ਵਾਪਸ Ok();
  }

  ਤਾਰਕਿਕ is_ok () ਅਚਲ { ਵਾਪਸ is_ok_; }
  ਤਾਰਕਿਕ is_err () ਅਚਲ { ਵਾਪਸ !is_ok_; }
  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ is_ok (); }

  ਅਚਲ E& error () ਅਚਲ & { ਪ੍ਰਤਿਜ੍ਞਾ (!is_ok_); ਵਾਪਸ err_; }
  E& error () & { ਪ੍ਰਤਿਜ੍ਞਾ (!is_ok_); ਵਾਪਸ err_; }
  E error () && { ਪ੍ਰਤਿਜ੍ਞਾ (!is_ok_); ਵਾਪਸ std::move (err_); }

  ਖਾਲੀ ਸੰਚਾਲਕ * () & { ਵਾਪਸ; }
  ਖਾਲੀ ਸੰਚਾਲਕ * () ਅਚਲ & { ਵਾਪਸ; }
  ਖਾਲੀ ਸੰਚਾਲਕ * () && { ਵਾਪਸ; }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਨਤੀਜਾ_t<ਖਾਲੀ, E>& o) ਅਚਲ
  {
    ਵਾਪਸ is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_ਨਤੀਜਾ_t<ਖਾਲੀ, E>& o) ਅਚਲ
  {
    ਵਾਪਸ !(*ਇਹ == o);
  }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (hb_ਨਤੀਜਾ_ok_t<ਖਾਲੀ>) ਅਚਲ
  {
    ਵਾਪਸ is_ok_;
  }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (hb_ਨਤੀਜਾ_ok_t<ਖਾਲੀ>) ਅਚਲ
  {
    ਵਾਪਸ !is_ok_;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਨਤੀਜਾ_err_t<F>& e) ਅਚਲ
  {
    ਵਾਪਸ !is_ok_ && err_ == e.error;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_ਨਤੀਜਾ_err_t<F>& e) ਅਚਲ
  {
    ਵਾਪਸ !(*ਇਹ == e);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ U&) ਅਚਲ = ਮਿਟਾਓ;

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U>
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ U&) ਅਚਲ = ਮਿਟਾਓ;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U, ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ E>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਨਤੀਜਾ_ok_t<U>& o, ਅਚਲ hb_ਨਤੀਜਾ_t<T, E>& r)
{
  ਵਾਪਸ r == o;
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U, ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ E>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_ਨਤੀਜਾ_ok_t<U>& o, ਅਚਲ hb_ਨਤੀਜਾ_t<T, E>& r)
{
  ਵਾਪਸ r != o;
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F, ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ E>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਨਤੀਜਾ_err_t<F>& e, ਅਚਲ hb_ਨਤੀਜਾ_t<T, E>& r)
{
  ਵਾਪਸ r == e;
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ F, ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ E>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_ਨਤੀਜਾ_err_t<F>& e, ਅਚਲ hb_ਨਤੀਜਾ_t<T, E>& r)
{
  ਵਾਪਸ r != e;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ TRY
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRY(ਵਿਅੰਜਕ)                                                             \
  ਕਰੋ {                                                                        \
    ਸਵੈਚਾਲਿਤ _res = (ਵਿਅੰਜਕ);                                                       \
    ਜੇ (unlikely (!_res.is_ok())) ਵਾਪਸ Err(std::move(_res).error());        \
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ TRY_ਸੌਂਪੋ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRY_ਸੌਂਪੋ(var, ...)                                                  \
  ਸਵੈਚਾਲਿਤ HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  ਜੇ (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) ਵਾਪਸ Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* HB_RESULT_HH */
