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

#که_نه_تعریف_شوی HB_لومړیتوب_کتار_HH
#تعریف_کړه HB_لومړیتوب_کتار_HH

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
کينډۍ <ډول_نوم K>
بنية hb_لومړیتوب_کتار_t
{
 عام:
  تعريفنوع hb_pair_t<K, بینښه> توکی_t;

 خاص:
  hb_ویکټور_t<توکی_t> ډېرۍ;

 عام:

  hb_لومړیتوب_کتار_t () = افتراضي;
  hb_لومړیتوب_کتار_t (hb_ویکټور_t<توکی_t>&& other) : ډېرۍ (std::move (other))
  {
    // Heapify the vector.
    لكل (صحيح i = (ډېرۍ.اوږدتیا / 2) - 1; i >= 0; i--)
      bubble_ښکته (i);
  }

  فراغ reset () { ډېرۍ.clear (); }

  منطقي دننه_error () ثابت { أرجع ډېرۍ.دننه_error (); }

  منطقي alloc (بینښه اندازه)
  { أرجع ډېرۍ.alloc (اندازه); }

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  فراغ insert (K لومړیتوب, بینښه value)
  {
    ډېرۍ.push (توکی_t (لومړیتوب, value));
    إذا (unlikely (ډېرۍ.دننه_error ())) أرجع;
    bubble_پورته (ډېرۍ.اوږدتیا - 1);
  }

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  توکی_t راوباسه_minimum ()
  {
    أكد (!is_empty ());

    توکی_t پایله = ډېرۍ.arrayZ[0];

    ډېرۍ.arrayZ[0] = ډېرۍ.arrayZ[ډېرۍ.اوږدتیا - 1];
    ډېرۍ.resize (ډېرۍ.اوږدتیا - 1);

    إذا (!is_empty ())
      bubble_ښکته (0);

    أرجع پایله;
  }

  ثابت توکی_t& minimum ()
  {
    أرجع ډېرۍ[0];
  }

  منطقي is_empty () ثابت { أرجع ډېرۍ.اوږدتیا == 0; }
  څرګند چلوونکی منطقي () ثابت { أرجع !is_empty (); }
  بینښه صحيح get_population () ثابت { أرجع ډېرۍ.اوږدتیا; }

  /* Sink interface. */
  hb_لومړیتوب_کتار_t& چلوونکی << (توکی_t توکی)
  { insert (توکی.لومړی, توکی.second); أرجع *هذا; }

 خاص:

  ساكن constexpr بینښه پلار (بینښه شاخص_2)
  {
    أرجع (شاخص_2 - 1) / 2;
  }

  ساكن constexpr بینښه کیڼ_ماشوم (بینښه شاخص_2)
  {
    أرجع 2 * شاخص_2 + 1;
  }

  ساكن constexpr بینښه ښی_ماشوم (بینښه شاخص_2)
  {
    أرجع 2 * شاخص_2 + 2;
  }

  HB_ALWAYS_INLINE
  فراغ bubble_ښکته (بینښه شاخص_2)
  {
    repeat:
    أكد (شاخص_2 < ډېرۍ.اوږدتیا);

    بینښه کیڼ = کیڼ_ماشوم (شاخص_2);
    بینښه ښی = ښی_ماشوم (شاخص_2);

    منطقي has_کیڼ = کیڼ < ډېرۍ.اوږدتیا;
    إذا (!has_کیڼ)
      // If there's no left, then there's also no right.
      أرجع;

    منطقي has_ښی = ښی < ډېرۍ.اوږدتیا;
    إذا (ډېرۍ.arrayZ[شاخص_2].لومړی <= ډېرۍ.arrayZ[کیڼ].لومړی
        && (!has_ښی || ډېرۍ.arrayZ[شاخص_2].لومړی <= ډېرۍ.arrayZ[ښی].لومړی))
      أرجع;

    بینښه ماشوم;
    إذا (!has_ښی || ډېرۍ.arrayZ[کیڼ].لومړی < ډېرۍ.arrayZ[ښی].لومړی)
      ماشوم = کیڼ;
    وإلا
      ماشوم = ښی;

    swap (شاخص_2, ماشوم);
    شاخص_2 = ماشوم;
    اذهب repeat;
  }

  HB_ALWAYS_INLINE
  فراغ bubble_پورته (بینښه شاخص_2)
  {
    repeat:
    أكد (شاخص_2 < ډېرۍ.اوږدتیا);

    إذا (شاخص_2 == 0) أرجع;

    بینښه پلار_شاخص = پلار (شاخص_2);
    إذا (ډېرۍ.arrayZ[پلار_شاخص].لومړی <= ډېرۍ.arrayZ[شاخص_2].لومړی)
      أرجع;

    swap (شاخص_2, پلار_شاخص);
    شاخص_2 = پلار_شاخص;
    اذهب repeat;
  }

  فراغ swap (بینښه a, بینښه b) noexcept
  {
    أكد (a < ډېرۍ.اوږدتیا);
    أكد (b < ډېرۍ.اوږدتیا);
    hb_بدلون (ډېرۍ.arrayZ[a], ډېرۍ.arrayZ[b]);
  }
};

#د_که_پای /* HB_PRIORITY_QUEUE_HH */
