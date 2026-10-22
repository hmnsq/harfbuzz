/*
 * Copyright © 2020  Google, Inc.
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

#جے_بنا_تعریف HB_ترجیح_قطار_HH
#تعریف_کرو HB_ترجیح_قطار_HH

#ضمن "hb.hh"
#ضمن "hb-vector.hh"

/*
 * hb_priority_queue_t
 *
 * Priority queue implemented as a binary heap. Supports extract minimum
 * and insert operations.
 *
 * The priority queue is implemented as a binary heap, which is a complete
 * binary tree. The root of the tree is the minimum element. The heap
 * property is that the priority of a node is less than or equal to the
 * priority of its children. The heap is stored in an array, with the
 * children of node i stored at indices 2i + 1 and 2i + 2.
 */
سانچا <قسم_ناں K>
بنية hb_ترجیح_قطار_t
{
 عام:
  تعريفنوع hb_pair_t<K, بنانشانی> شے_t;

 خاص:
  hb_سمتیہ_t<شے_t> ڈھیر_2;

 عام:

  hb_ترجیح_قطار_t () = افتراضي;
  hb_ترجیح_قطار_t (hb_سمتیہ_t<شے_t>&& other) : ڈھیر_2 (std::move (other))
  {
    // Heapify the vector.
    لكل (صحيح i = (ڈھیر_2.لمائی / 2) - 1; i >= 0; i--)
      bubble_تھلے (i);
  }

  فراغ reset () { ڈھیر_2.clear (); }

  منطقي وچ_error () ثابت { أرجع ڈھیر_2.وچ_error (); }

  منطقي alloc (بنانشانی ناپ)
  { أرجع ڈھیر_2.alloc (ناپ); }

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  فراغ insert (K ترجیح, بنانشانی value)
  {
    ڈھیر_2.push (شے_t (ترجیح, value));
    إذا (unlikely (ڈھیر_2.وچ_error ())) أرجع;
    bubble_اتے (ڈھیر_2.لمائی - 1);
  }

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  شے_t نکالو_minimum ()
  {
    أكد (!is_empty ());

    شے_t نتیجہ = ڈھیر_2.arrayZ[0];

    ڈھیر_2.arrayZ[0] = ڈھیر_2.arrayZ[ڈھیر_2.لمائی - 1];
    ڈھیر_2.resize (ڈھیر_2.لمائی - 1);

    إذا (!is_empty ())
      bubble_تھلے (0);

    أرجع نتیجہ;
  }

  ثابت شے_t& minimum ()
  {
    أرجع ڈھیر_2[0];
  }

  منطقي is_empty () ثابت { أرجع ڈھیر_2.لمائی == 0; }
  واضح عامل منطقي () ثابت { أرجع !is_empty (); }
  بنانشانی صحيح get_population () ثابت { أرجع ڈھیر_2.لمائی; }

  /* Sink interface. */
  hb_ترجیح_قطار_t& عامل << (شے_t شے)
  { insert (شے.پہلا, شے.second); أرجع *هذا; }

 خاص:

  ساكن constexpr بنانشانی ماپا (بنانشانی اشاریہ_2)
  {
    أرجع (اشاریہ_2 - 1) / 2;
  }

  ساكن constexpr بنانشانی بایاں_بچہ (بنانشانی اشاریہ_2)
  {
    أرجع 2 * اشاریہ_2 + 1;
  }

  ساكن constexpr بنانشانی دایاں_بچہ (بنانشانی اشاریہ_2)
  {
    أرجع 2 * اشاریہ_2 + 2;
  }

  HB_ALWAYS_INLINE
  فراغ bubble_تھلے (بنانشانی اشاریہ_2)
  {
    repeat:
    أكد (اشاریہ_2 < ڈھیر_2.لمائی);

    بنانشانی کھبے = بایاں_بچہ (اشاریہ_2);
    بنانشانی سجے = دایاں_بچہ (اشاریہ_2);

    منطقي has_بایاں = کھبے < ڈھیر_2.لمائی;
    إذا (!has_بایاں)
      // If there's no left, then there's also no right.
      أرجع;

    منطقي has_دایاں = سجے < ڈھیر_2.لمائی;
    إذا (ڈھیر_2.arrayZ[اشاریہ_2].پہلا <= ڈھیر_2.arrayZ[کھبے].پہلا
        && (!has_دایاں || ڈھیر_2.arrayZ[اشاریہ_2].پہلا <= ڈھیر_2.arrayZ[سجے].پہلا))
      أرجع;

    بنانشانی بچہ;
    إذا (!has_دایاں || ڈھیر_2.arrayZ[کھبے].پہلا < ڈھیر_2.arrayZ[سجے].پہلا)
      بچہ = کھبے;
    وإلا
      بچہ = سجے;

    swap (اشاریہ_2, بچہ);
    اشاریہ_2 = بچہ;
    اذهب repeat;
  }

  HB_ALWAYS_INLINE
  فراغ bubble_اتے (بنانشانی اشاریہ_2)
  {
    repeat:
    أكد (اشاریہ_2 < ڈھیر_2.لمائی);

    إذا (اشاریہ_2 == 0) أرجع;

    بنانشانی ماپا_اشاریہ = ماپا (اشاریہ_2);
    إذا (ڈھیر_2.arrayZ[ماپا_اشاریہ].پہلا <= ڈھیر_2.arrayZ[اشاریہ_2].پہلا)
      أرجع;

    swap (اشاریہ_2, ماپا_اشاریہ);
    اشاریہ_2 = ماپا_اشاریہ;
    اذهب repeat;
  }

  فراغ swap (بنانشانی a, بنانشانی b) noexcept
  {
    أكد (a < ڈھیر_2.لمائی);
    أكد (b < ڈھیر_2.لمائی);
    hb_وٹاندرا (ڈھیر_2.arrayZ[a], ڈھیر_2.arrayZ[b]);
  }
};

#جے_انت /* HB_PRIORITY_QUEUE_HH */
