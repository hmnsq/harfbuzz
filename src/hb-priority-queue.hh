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

#अगर_अघोषित HB_PRIORITY_QUEUE_HH
#घोषणा HB_PRIORITY_QUEUE_HH

#समावेश "hb.hh"
#समावेश "hb-vector.hh"

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
ढाँचा <typename K>
काष्ठा hb_priority_queue_t
{
 खुला:
  प्रकार hb_pair_t<K, अचिन्हित> item_t;

 निजी:
  hb_vector_t<item_t> heap;

 खुला:

  hb_priority_queue_t () = शेष;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    क्रम (पूर्णांक i = (heap.लंबाई / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  व्योम reset () { heap.सफा (); }

  bool in_error () स्थिर { वापस heap.in_error (); }

  bool alloc (अचिन्हित आकार)
  { वापस heap.alloc (आकार); }

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  व्योम insert (K priority, अचिन्हित value)
  {
    heap.push (item_t (priority, value));
    अगर (unlikely (heap.in_error ())) वापस;
    bubble_up (heap.लंबाई - 1);
  }

#अगर_अघोषित HB_OPTIMIZE_आकार
  HB_ALWAYS_INLINE
#पूर्ण_अगर
  item_t निकालो_minimum ()
  {
    निश्चित (!is_empty ());

    item_t परिणाम = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.लंबाई - 1];
    heap.resize (heap.लंबाई - 1);

    अगर (!is_empty ())
      bubble_down (0);

    वापस परिणाम;
  }

  स्थिर item_t& minimum ()
  {
    वापस heap[0];
  }

  bool is_empty () स्थिर { वापस heap.लंबाई == 0; }
  explicit चालक bool () स्थिर { वापस !is_empty (); }
  अचिन्हित पूर्णांक get_population () स्थिर { वापस heap.लंबाई; }

  /* Sink interface. */
  hb_priority_queue_t& चालक << (item_t item)
  { insert (item.first, item.second); वापस *यह; }

 निजी:

  अटल constexpr अचिन्हित parent (अचिन्हित सूचकांक_2)
  {
    वापस (सूचकांक_2 - 1) / 2;
  }

  अटल constexpr अचिन्हित बायाँ_child (अचिन्हित सूचकांक_2)
  {
    वापस 2 * सूचकांक_2 + 1;
  }

  अटल constexpr अचिन्हित दायाँ_child (अचिन्हित सूचकांक_2)
  {
    वापस 2 * सूचकांक_2 + 2;
  }

  HB_ALWAYS_INLINE
  व्योम bubble_down (अचिन्हित सूचकांक_2)
  {
    repeat:
    निश्चित (सूचकांक_2 < heap.लंबाई);

    अचिन्हित बाँए = बायाँ_child (सूचकांक_2);
    अचिन्हित दाँए = दायाँ_child (सूचकांक_2);

    bool has_बायाँ = बाँए < heap.लंबाई;
    अगर (!has_बायाँ)
      // If there's no left, then there's also no right.
      वापस;

    bool has_दायाँ = दाँए < heap.लंबाई;
    अगर (heap.arrayZ[सूचकांक_2].first <= heap.arrayZ[बाँए].first
        && (!has_दायाँ || heap.arrayZ[सूचकांक_2].first <= heap.arrayZ[दाँए].first))
      वापस;

    अचिन्हित child;
    अगर (!has_दायाँ || heap.arrayZ[बाँए].first < heap.arrayZ[दाँए].first)
      child = बाँए;
    अन्यथा
      child = दाँए;

    swap (सूचकांक_2, child);
    सूचकांक_2 = child;
    जाओ repeat;
  }

  HB_ALWAYS_INLINE
  व्योम bubble_up (अचिन्हित सूचकांक_2)
  {
    repeat:
    निश्चित (सूचकांक_2 < heap.लंबाई);

    अगर (सूचकांक_2 == 0) वापस;

    अचिन्हित parent_सूचकांक = parent (सूचकांक_2);
    अगर (heap.arrayZ[parent_सूचकांक].first <= heap.arrayZ[सूचकांक_2].first)
      वापस;

    swap (सूचकांक_2, parent_सूचकांक);
    सूचकांक_2 = parent_सूचकांक;
    जाओ repeat;
  }

  व्योम swap (अचिन्हित a, अचिन्हित b) noexcept
  {
    निश्चित (a < heap.लंबाई);
    निश्चित (b < heap.लंबाई);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#पूर्ण_अगर /* HB_PRIORITY_QUEUE_HH */
