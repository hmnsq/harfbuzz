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

#अगर_अघोषित HB_परिणाम_HH
#घोषणा HB_परिणाम_HH

#समावेश "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
ढाँचा <typename T>
काष्ठा hb_परिणाम_ok_t
{
  T value;
};

ढाँचा <>
काष्ठा hb_परिणाम_ok_t<व्योम> {};

ढाँचा <typename E>
काष्ठा hb_परिणाम_err_t
{
  E error;

  ढाँचा <typename OtherE>
  constexpr bool चालक == (स्थिर hb_परिणाम_err_t<OtherE>& other) स्थिर { वापस error == other.error; }
  ढाँचा <typename OtherE>
  constexpr bool चालक != (स्थिर hb_परिणाम_err_t<OtherE>& other) स्थिर { वापस error != other.error; }
};

ढाँचा <typename X> काष्ठा hb_is_परिणाम_tag : hb_false_type {};
ढाँचा <typename X> काष्ठा hb_is_परिणाम_tag<hb_परिणाम_ok_t<X>> : hb_true_type {};
ढाँचा <typename X> काष्ठा hb_is_परिणाम_tag<hb_परिणाम_err_t<X>> : hb_true_type {};

ढाँचा <typename T>
अटल अंतरभूत constexpr hb_परिणाम_ok_t<T&&> Ok (T&& v)
{
  वापस hb_परिणाम_ok_t<T&&>{std::forward<T> (v)};
}

अटल अंतरभूत constexpr hb_परिणाम_ok_t<व्योम> Ok ()
{
  वापस hb_परिणाम_ok_t<व्योम>{};
}

ढाँचा <typename E>
अटल अंतरभूत constexpr hb_परिणाम_err_t<E&&> Err (E&& e)
{
  वापस hb_परिणाम_err_t<E&&>{std::forward<E> (e)};
}

ढाँचा <typename T, typename E>
काष्ठा HB_NODISCARD_STRUCT hb_परिणाम_t
{
  निजी:
  जोड़
  {
    T val_;
    E err_;
  };
  bool is_ok_;

  व्योम destroy ()
  {
    अगर (is_ok_)
      val_.~T ();
    अन्यथा
      err_.~E ();
  }

  व्योम init(स्थिर hb_परिणाम_t& o) {
    is_ok_ = o.is_ok_;
    अगर (is_ok_)
      नया (std::addressof (val_)) T (o.val_);
    अन्यथा
      नया (std::addressof (err_)) E (o.err_);
  }

  व्योम init(hb_परिणाम_t&& o) {
    is_ok_ = o.is_ok_;
    अगर (is_ok_)
      नया (std::addressof (val_)) T (std::move (o.val_));
    अन्यथा
      नया (std::addressof (err_)) E (std::move (o.err_));
  }

  खुला:
  ~hb_परिणाम_t ()
  {
    destroy ();
  }

  hb_परिणाम_t () = मिटाओ;

  hb_परिणाम_t (स्थिर hb_परिणाम_t& o) : is_ok_ (false)
  {
    init (o);
  }

  hb_परिणाम_t (hb_परिणाम_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (false)
  {
    init (std::move (o));
  }

  hb_परिणाम_t& चालक = (स्थिर hb_परिणाम_t& o)
  {
    अगर (यह == &o) वापस *यह;
    अगर (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    अन्यथा अगर (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    अन्यथा
    {
      destroy ();
      init (o);
    }
    वापस *यह;
  }

  hb_परिणाम_t& चालक = (hb_परिणाम_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    अगर (यह == &o) वापस *यह;
    अगर (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    अन्यथा अगर (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    अन्यथा
    {
      destroy ();
      init (std::move (o));
    }
    वापस *यह;
  }

  // Construct from Ok(...) tag
  ढाँचा <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_परिणाम_t (hb_परिणाम_ok_t<U>&& o) : is_ok_ (true)
  {
    नया (std::addressof (val_)) T (std::move (o.value));
  }

  ढाँचा <typename U,
            hb_enable_if ((std::is_constructible<T, स्थिर U&>::value))>
  hb_परिणाम_t (स्थिर hb_परिणाम_ok_t<U>& o) : is_ok_ (true)
  {
    नया (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  ढाँचा <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_परिणाम_t (hb_परिणाम_err_t<F>&& e) : is_ok_ (false)
  {
    नया (std::addressof (err_)) E (std::move (e.error));
  }

  ढाँचा <typename F,
            hb_enable_if ((std::is_constructible<E, स्थिर F&>::value))>
  hb_परिणाम_t (स्थिर hb_परिणाम_err_t<F>& e) : is_ok_ (false)
  {
    नया (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  ढाँचा <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_परिणाम_t (F e) : is_ok_ (false)
  {
    नया (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  ढाँचा <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_परिणाम_t) &&
                           !hb_is_परिणाम_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_परिणाम_t (U&& v) : is_ok_ (true)
  {
    नया (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  अटल hb_परिणाम_t from (T&& fallible, E error_मूल्य) {
    अगर (fallible.in_error()) {
      वापस Err(error_मूल्य);
    }
    वापस Ok(fallible);
  }

  bool is_ok () स्थिर { वापस is_ok_; }
  bool is_err () स्थिर { वापस !is_ok_; }
  explicit चालक bool () स्थिर { वापस is_ok (); }

  T& value () & { निश्चित (is_ok_); वापस val_; }
  स्थिर T& value () स्थिर & { निश्चित (is_ok_); वापस val_; }
  T value () && { निश्चित (is_ok_); वापस std::move (val_); }

  T& चालक * () & { वापस value (); }
  स्थिर T& चालक * () स्थिर & { वापस value (); }
  T चालक * () && { वापस std::move (*यह).value (); }

  T* चालक -> () { वापस std::addressof (value ()); }
  स्थिर T* चालक -> () स्थिर { वापस std::addressof (value ()); }

  ढाँचा <typename U>
  T मूल्य_or (U&& default_मूल्य) स्थिर &
  {
    अगर (is_ok_) वापस val_;
    वापस std::forward<U> (default_मूल्य);
  }

  ढाँचा <typename U>
  T मूल्य_or (U&& default_मूल्य) &&
  {
    अगर (is_ok_) वापस std::move (val_);
    वापस std::forward<U> (default_मूल्य);
  }

  T मूल्य_or_default () स्थिर &
  {
    अगर (is_ok_) वापस val_;
    वापस T ();
  }

  T मूल्य_or_default () &&
  {
    अगर (is_ok_) वापस std::move (val_);
    वापस T ();
  }

  स्थिर E& error () स्थिर & { निश्चित (!is_ok_); वापस err_; }
  E& error () & { निश्चित (!is_ok_); वापस err_; }
  E error () && { निश्चित (!is_ok_); वापस std::move (err_); }

  bool चालक == (स्थिर hb_परिणाम_t& o) स्थिर
  {
    अगर (is_ok_ != o.is_ok_) वापस false;
    अगर (is_ok_) वापस val_ == o.val_;
    वापस err_ == o.err_;
  }

  bool चालक != (स्थिर hb_परिणाम_t& o) स्थिर
  {
    वापस !(*यह == o);
  }

  ढाँचा <typename U>
  bool चालक == (स्थिर hb_परिणाम_ok_t<U>& o) स्थिर
  {
    वापस is_ok_ && val_ == o.value;
  }

  ढाँचा <typename U>
  bool चालक != (स्थिर hb_परिणाम_ok_t<U>& o) स्थिर
  {
    वापस !(*यह == o);
  }

  ढाँचा <typename F>
  bool चालक == (स्थिर hb_परिणाम_err_t<F>& e) स्थिर
  {
    वापस !is_ok_ && err_ == e.error;
  }

  ढाँचा <typename F>
  bool चालक != (स्थिर hb_परिणाम_err_t<F>& e) स्थिर
  {
    वापस !(*यह == e);
  }

  ढाँचा <typename U>
  bool चालक == (स्थिर U&) स्थिर = मिटाओ;

  ढाँचा <typename U>
  bool चालक != (स्थिर U&) स्थिर = मिटाओ;
};

// Partial specialization for void
ढाँचा <typename E>
काष्ठा HB_NODISCARD_STRUCT hb_परिणाम_t<व्योम, E>
{
  निजी:
  जोड़
  {
    E err_;
  };
  bool is_ok_;

  ढाँचा <typename, typename> मित्र काष्ठा hb_परिणाम_t;

  खुला:
  ~hb_परिणाम_t ()
  {
    अगर (!is_ok_)
      err_.~E ();
  }

  hb_परिणाम_t () : is_ok_ (true) {}
  hb_परिणाम_t (hb_परिणाम_ok_t<व्योम>) : is_ok_ (true) {}

  hb_परिणाम_t (स्थिर hb_परिणाम_t& o) : is_ok_ (o.is_ok_)
  {
    अगर (!is_ok_)
      नया (std::addressof (err_)) E (o.err_);
  }

  hb_परिणाम_t (hb_परिणाम_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    अगर (!is_ok_)
      नया (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_परिणाम_t& चालक = (स्थिर hb_परिणाम_t& o)
  {
    अगर (यह == &o) वापस *यह;
    अगर (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    अन्यथा अगर (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = true;
    }
    अन्यथा अगर (is_ok_ && !o.is_ok_)
    {
      नया (std::addressof (err_)) E (o.err_);
      is_ok_ = false;
    }
    वापस *यह;
  }

  hb_परिणाम_t& चालक = (hb_परिणाम_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    अगर (यह == &o) वापस *यह;
    अगर (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    अन्यथा अगर (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = true;
    }
    अन्यथा अगर (is_ok_ && !o.is_ok_)
    {
      नया (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = false;
    }
    वापस *यह;
  }

  ढाँचा <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_परिणाम_t (hb_परिणाम_err_t<F>&& e) : is_ok_ (false)
  {
    नया (std::addressof (err_)) E (std::move (e.error));
  }

  ढाँचा <typename F,
            hb_enable_if ((std::is_constructible<E, स्थिर F&>::value))>
  hb_परिणाम_t (स्थिर hb_परिणाम_err_t<F>& e) : is_ok_ (false)
  {
    नया (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_परिणाम_t (E e) : is_ok_ (false)
  {
    नया (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  ढाँचा<typename V>
  अटल hb_परिणाम_t<व्योम, E> from (स्थिर V& fallible, E error_मूल्य) {
    अगर (fallible.in_error()) {
      वापस Err(error_मूल्य);
    }
    वापस Ok();
  }

  bool is_ok () स्थिर { वापस is_ok_; }
  bool is_err () स्थिर { वापस !is_ok_; }
  explicit चालक bool () स्थिर { वापस is_ok (); }

  स्थिर E& error () स्थिर & { निश्चित (!is_ok_); वापस err_; }
  E& error () & { निश्चित (!is_ok_); वापस err_; }
  E error () && { निश्चित (!is_ok_); वापस std::move (err_); }

  व्योम चालक * () & { वापस; }
  व्योम चालक * () स्थिर & { वापस; }
  व्योम चालक * () && { वापस; }

  bool चालक == (स्थिर hb_परिणाम_t<व्योम, E>& o) स्थिर
  {
    वापस is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  bool चालक != (स्थिर hb_परिणाम_t<व्योम, E>& o) स्थिर
  {
    वापस !(*यह == o);
  }

  bool चालक == (hb_परिणाम_ok_t<व्योम>) स्थिर
  {
    वापस is_ok_;
  }

  bool चालक != (hb_परिणाम_ok_t<व्योम>) स्थिर
  {
    वापस !is_ok_;
  }

  ढाँचा <typename F>
  bool चालक == (स्थिर hb_परिणाम_err_t<F>& e) स्थिर
  {
    वापस !is_ok_ && err_ == e.error;
  }

  ढाँचा <typename F>
  bool चालक != (स्थिर hb_परिणाम_err_t<F>& e) स्थिर
  {
    वापस !(*यह == e);
  }

  ढाँचा <typename U>
  bool चालक == (स्थिर U&) स्थिर = मिटाओ;

  ढाँचा <typename U>
  bool चालक != (स्थिर U&) स्थिर = मिटाओ;
};

ढाँचा <typename U, typename T, typename E>
अटल अंतरभूत bool चालक == (स्थिर hb_परिणाम_ok_t<U>& o, स्थिर hb_परिणाम_t<T, E>& r)
{
  वापस r == o;
}
ढाँचा <typename U, typename T, typename E>
अटल अंतरभूत bool चालक != (स्थिर hb_परिणाम_ok_t<U>& o, स्थिर hb_परिणाम_t<T, E>& r)
{
  वापस r != o;
}
ढाँचा <typename F, typename T, typename E>
अटल अंतरभूत bool चालक == (स्थिर hb_परिणाम_err_t<F>& e, स्थिर hb_परिणाम_t<T, E>& r)
{
  वापस r == e;
}
ढाँचा <typename F, typename T, typename E>
अटल अंतरभूत bool चालक != (स्थिर hb_परिणाम_err_t<F>& e, स्थिर hb_परिणाम_t<T, E>& r)
{
  वापस r != e;
}

#अगर_अघोषित TRY
#घोषणा TRY(व्यंजक)                                                             \
  करो {                                                                        \
    स्वतः _res = (व्यंजक);                                                       \
    अगर (unlikely (!_res.is_ok())) वापस Err(std::move(_res).error());        \
  } जबतक (0)
#पूर्ण_अगर

#अगर_अघोषित TRY_ASSIGN
#घोषणा TRY_ASSIGN(var, ...)                                                  \
  स्वतः HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  अगर (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) वापस Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#पूर्ण_अगर

#पूर्ण_अगर /* HB_RESULT_HH */
