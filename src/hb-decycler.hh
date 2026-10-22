/*
 * Copyright © 2025 Behdad Esfahbod
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
 * Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DECYCLER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DECYCLER_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

/*
 * hb_decycler_t is an efficient cycle detector for graph traversal.
 * It's a simple tortoise-and-hare algorithm with a twist: it's
 * designed to detect cycles while traversing a graph in a DFS manner,
 * instead of just a linked list.
 *
 * For Floyd's tortoise and hare algorithm, see:
 * https://en.wikipedia.org/wiki/Cycle_detection#Floyd's_tortoise_and_hare
 *
 * hb_decycler_t is O(n) in the number of nodes in the DFS traversal
 * if there are no cycles. Unlike Floyd's algorithm, hb_decycler_t
 * can be used in a DFS traversal, where the graph is not a simple
 * linked list, but a tree with possible cycles.  Like Floyd's algorithm,
 * it is constant-memory (~three  pointers).
 *
 * The decycler works by creating an implicit linked-list on the stack,
 * of the path from the root to the current node, and apply Floyd's
 * algorithm on that list as it goes.
 *
 * The decycler is malloc-free, and as such, much faster to use than a
 * hb_set_t or hb_map_t equivalent.
 *
 * The decycler detects cycles in the graph *eventually*, not *immediately*.
 * That is, it may not detect a cycle until the cycle is fully traversed,
 * even multiple times. See Floyd's algorithm analysis for details.
 *
 * The implementation saves a pointer storage on the stack by combining
 * this->u.decycler and this->u.next into a union.  This is possible because
 * at any point we only need one of those values. The invariant is that
 * after construction, and before destruction, of a node, the u.decycler
 * field is always valid. The u.next field is only valid when the node is
 * in the traversal path, parent to another node.
 *
 * There are three method's:
 *
 *   - hb_decycler_node_t() constructor: Creates a new node in the traversal.
 *     The constructor takes a reference to the decycler object and inserts
 *     itself as the latest node in the traversal path, by advancing the hare
 *     pointer, and for every other descent, advancing the tortoise pointer.
 *
 *   - ~hb_decycler_node_t() destructor: Restores the decycler object to its
 *      previous state by removing the node from the traversal path.
 *
 *   - bool visit(uintptr_t value): Called on every node in the graph.  Returns
 *     true if the node is not part of a cycle, and false if it is.  The value
 *     parameter is used to detect cycles.  It's the caller's responsibility
 *     to ensure that the value is unique for each node in the graph.
 *     The cycle detection is as simple as comparing the value to the value
 *     held by the tortoise pointer, which is the Floyd's algorithm.
 *
 * For usage examples see test-decycler.cc.
 */

ਬਣਤਰ hb_decycler_ਗੰਢ_t;

ਬਣਤਰ hb_decycler_t
{
  ਮਿੱਤਰ ਬਣਤਰ hb_decycler_ਗੰਢ_t;

  ਨਿਜੀ:
  ਤਾਰਕਿਕ tortoise_awake = ਝੂਠ;
  hb_decycler_ਗੰਢ_t *tortoise = ਖਾਲੀ_ਸੂਚਕ;
  hb_decycler_ਗੰਢ_t *hare = ਖਾਲੀ_ਸੂਚਕ;
};

ਬਣਤਰ hb_decycler_ਗੰਢ_t
{
  hb_decycler_ਗੰਢ_t (hb_decycler_t &decycler)
  {
    u.decycler = &decycler;

    decycler.tortoise_awake = !decycler.tortoise_awake;

    ਜੇ (!decycler.tortoise)
    {
      // First node.
      ਪ੍ਰਤਿਜ੍ਞਾ (decycler.tortoise_awake);
      ਪ੍ਰਤਿਜ੍ਞਾ (!decycler.hare);
      decycler.tortoise = decycler.hare = ਇਹ;
      ਵਾਪਸ;
    }

    ਜੇ (decycler.tortoise_awake)
      decycler.tortoise = decycler.tortoise->u.next; // Time to move.

    ਇਹ->prev = decycler.hare;
    decycler.hare->u.next = ਇਹ;
    decycler.hare = ਇਹ;
  }

  ~hb_decycler_ਗੰਢ_t ()
  {
    hb_decycler_t &decycler = *u.decycler;

    // Inverse of the constructor.

    ਪ੍ਰਤਿਜ੍ਞਾ (decycler.hare == ਇਹ);
    decycler.hare = prev;
    ਜੇ (prev)
      prev->u.decycler = &decycler;

    ਪ੍ਰਤਿਜ੍ਞਾ (decycler.tortoise);
    ਜੇ (decycler.tortoise_awake)
      decycler.tortoise = decycler.tortoise->prev;

    decycler.tortoise_awake = !decycler.tortoise_awake;
  }

  ਤਾਰਕਿਕ ਦੇਖੋ (uintptr_t ਮੁੱਲ_3)
  {
    value = ਮੁੱਲ_3;

    hb_decycler_t &decycler = *u.decycler;

    ਜੇ (decycler.tortoise == ਇਹ)
      ਵਾਪਸ ਸੱਚ; // First node; not a cycle.

    ਜੇ (decycler.tortoise->value == value)
      ਵਾਪਸ ਝੂਠ; // Cycle detected.

    ਵਾਪਸ ਸੱਚ;
  }

  ਨਿਜੀ:
  ਸੰਘ {
    hb_decycler_t *decycler;
    hb_decycler_ਗੰਢ_t *next;
  } u = {ਖਾਲੀ_ਸੂਚਕ};
  hb_decycler_ਗੰਢ_t *prev = ਖਾਲੀ_ਸੂਚਕ;
  uintptr_t value = 0;
};

#ਜੇ_ਅੰਤ /* HB_DECYCLER_HH */
