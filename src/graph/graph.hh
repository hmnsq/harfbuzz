/*
 * Copyright © 2022  Google, Inc.
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

#ଅନ୍ତର୍ଭାଵ "../hb-set.hh"
#ଅନ୍ତର୍ଭାଵ "../hb-priority-queue.hh"
#ଅନ୍ତର୍ଭାଵ "../hb-serialize.hh"
#ଅନ୍ତର୍ଭାଵ "graph-result.hh"

#ifndef GRAPH_GRAPH_HH
#define GRAPH_GRAPH_HH

#define HB_GRAPH_INVALID ((unsigned) -1)

ନାମାକାଶ graph {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
ସଂରଚନା graph_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  ସଂରଚନା vertex_t
  {
    // TODO(garretrieger): have vertex_t gaurantee that head/tail are non null and head < tail
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): have vertex_t gaurantee that all link indexes are valid (ie < vertices_.length)
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): track where the real links are currently sorted, and automatically sort
    // prior to accessing iterator for real links. Avoids needing to manually trigger sorting by
    // downstream users.

    int64_t distance = 0 ;
    unsigned space = 0 ;
    unsigned start = 0;
    unsigned end = 0;
    unsigned priority = 0;

    ବ୍ୟକ୍ତିଗତ:
    hb_serialize_context_t::object_t obj_;
    unsigned incoming_edges_ = 0;
    unsigned single_parent = HB_GRAPH_INVALID;
    ତାର୍କିକ has_incoming_virtual_edges_ = ମିଥ୍ୟା;
    hb_hashmap_t<unsigned, unsigned> parents;
    hb_set_t virtual_parents;
    ସର୍ବସାଧାରଣ:

    vertex_t ()
    {}

    explicit vertex_t(ଅଚଳ hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    auto parents_iter () ଅଚଳ HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_parent, single_parent != HB_GRAPH_INVALID),
	parents.ଚାବିଗୁଡ଼ିକ_ref ()
      )
    )

    ଶୂନ୍ୟ set_buffer (ଅକ୍ଷର* head, ଅକ୍ଷର* tail) {
      obj_.head = head;
      obj_.tail = tail;
    }

    ଶୂନ୍ୟ shrink_buffer (size_t ଗଣନା_2)
    {
      obj_.tail -= ଗଣନା_2;
    }

    auto all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    auto real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    auto virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    ଶୂନ୍ୟ sort_real_links () {
      obj_.real_links.qsort ();
    }

    graph_result_t<ଶୂନ୍ୟ> add_real_link (unsigned width, unsigned obj_idx, unsigned position) {
      auto* link = obj_.real_links.push ();
      TRY (graph_result_t<ଶୂନ୍ୟ>::from (obj_.real_links, ALLOCATION_FAILURE));
      link->width = width;
      link->objidx = obj_idx;
      link->position = position;
      ଫେରନ୍ତୁ Ok();
    }

    graph_result_t<ଶୂନ୍ୟ> add_virtual_link (unsigned obj_idx) {
      auto* link = obj_.virtual_links.push ();
      TRY (graph_result_t<ଶୂନ୍ୟ>::from (obj_.virtual_links, ALLOCATION_FAILURE));
      link->objidx = obj_idx;
      ଫେରନ୍ତୁ Ok();
    }

    graph_result_t<ଶୂନ୍ୟ> add_real_link (ଅଚଳ hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      ଫେରନ୍ତୁ graph_result_t<ଶୂନ୍ୟ>::from (obj_.real_links, ALLOCATION_FAILURE);
    }

    graph_result_t<ଶୂନ୍ୟ> set_real_links (hb_vector_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(graph_result_t<ଶୂନ୍ୟ>::from (links, ALLOCATION_FAILURE));
      obj_.real_links = links;
      ଫେରନ୍ତୁ Ok();
    }

    ଶୂନ୍ୟ clear_real_links ()
    {
      obj_.real_links.reset();
    }

    ଶୂନ୍ୟ clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    graph_result_t<ଶୂନ୍ୟ> add_virtual_link (ଅଚଳ hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      ଫେରନ୍ତୁ graph_result_t<ଶୂନ୍ୟ>::from (obj_.virtual_links, ALLOCATION_FAILURE);
    }

    ଅଚଳ hb_serialize_context_t::object_t& obj() ଅଚଳ { ଫେରନ୍ତୁ obj_; }

    template<typename T>
    T as () ଅଚଳ
    {
      ଫେରନ୍ତୁ reinterpret_cast<T> (obj_.head);
    }

    ତାର୍କିକ has_incoming_virtual_edges () ଅଚଳ
    {
      ଫେରନ୍ତୁ has_incoming_virtual_edges_;
    }

    graph_result_t<ଶୂନ୍ୟ> link_positions_valid (unsigned ଅଙ୍କ_objects, ତାର୍କିକ removed_nil)
    {
      hb_set_t assigned_bytes;
      ପାଇଁ (ଅଚଳ auto& l : obj_.real_links)
      {
        ଯଦି (unlikely (l.objidx >= ଅଙ୍କ_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid object index.");
          ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
        }

        unsigned start = l.position;
        unsigned end = start + l.width - 1;

        ଯଦି (unlikely (l.width < 2 || l.width > 4))
        {
          DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid link width.");
          ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
        }

        ଯଦି (unlikely (end >= table_size ()))
        {
          DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                     "Invalid graph. Link position is out of bounds.");
          ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);
        }

        ଯଦି (unlikely (assigned_bytes.intersects (start, end)))
        {
          DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                     "Invalid graph. Found offsets whose positions overlap.");
          ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
        }

        assigned_bytes.add_range (start, end);
      }

      ଫେରନ୍ତୁ graph_result_t<ଶୂନ୍ୟ>::from (assigned_bytes, ALLOCATION_FAILURE);
    }

    ଶୂନ୍ୟ normalize ()
    {
      obj_.real_links.qsort ();
      ପାଇଁ (ଅଚଳ auto& l : obj_.real_links)
      {
        ପାଇଁ (unsigned i = 0; i < l.width; i++)
        {
          obj_.head[l.position + i] = 0;
        }
      }
    }

    ତାର୍କିକ equals (unsigned this_index,
                 unsigned other_index,
                 ଅଚଳ vertex_t& other,
                 ଅଚଳ graph_t& graph,
                 ଅଚଳ graph_t& other_graph,
                 unsigned depth) ଅଚଳ
    {
      ଯଦି (!(as_bytes () == other.as_bytes ()))
      {
        DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_index,
                   (unsigned long) table_size (),
                   other_index,
                   (unsigned long) other.table_size (),
                   depth);

        auto a = as_bytes ();
        auto b = other.as_bytes ();
        ଯେତେବେଳେ (a || b)
        {
          DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                     "  0x%x %s 0x%x", (unsigned) *a, (*a == *b) ? "==" : "!=", (unsigned) *b);
          a++;
          b++;
        }
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      ଫେରନ୍ତୁ links_equal (obj_.real_links, other.obj_.real_links, graph, other_graph, depth);
    }

    hb_bytes_t as_bytes () ଅଚଳ
    {
      ଫେରନ୍ତୁ hb_bytes_t (obj_.head, table_size ());
    }

    friend ଶୂନ୍ୟ swap (vertex_t& a, vertex_t& b)
    {
      hb_swap (a.obj_, b.obj_);
      hb_swap (a.distance, b.distance);
      hb_swap (a.space, b.space);
      hb_swap (a.single_parent, b.single_parent);
      hb_swap (a.parents, b.parents);
      hb_swap (a.virtual_parents, b.virtual_parents);
      hb_swap (a.incoming_edges_, b.incoming_edges_);
      hb_swap (a.has_incoming_virtual_edges_, b.has_incoming_virtual_edges_);
      hb_swap (a.start, b.start);
      hb_swap (a.end, b.end);
      hb_swap (a.priority, b.priority);
    }

    graph_result_t<hb_hashmap_t<unsigned, unsigned>>
    position_to_index_map () ଅଚଳ
    {
      hb_hashmap_t<unsigned, unsigned> result;

      result.alloc (obj_.real_links.length);
      ପାଇଁ (ଅଚଳ auto& l : obj_.real_links) {
        result.set (l.position, l.objidx);
      }

      ଫେରନ୍ତୁ graph_result_t<hb_hashmap_t<unsigned, unsigned>>::from (std::move(result), ALLOCATION_FAILURE);
    }

    ତାର୍କିକ is_shared () ଅଚଳ
    {
      ଫେରନ୍ତୁ parents.get_population () > virtual_parents.get_population () + 1;
    }

    unsigned incoming_edges () ଅଚଳ
    {
      ଯଦି (HB_DEBUG_SUBSET_REPACK)
       {
	ପ୍ରତିଜ୍ଞା (incoming_edges_ == (single_parent != HB_GRAPH_INVALID) +
		(parents.ମୂଲ୍ୟଗୁଡ଼ିକ_ref () | hb_reduce (hb_add, 0)));
       }
      ଫେରନ୍ତୁ incoming_edges_;
    }

    unsigned incoming_edges_from_parent (unsigned parent_index) ଅଚଳ {
      ଯଦି (single_parent != HB_GRAPH_INVALID) {
        ଫେରନ୍ତୁ single_parent == parent_index ? 1 : 0;
      }

      unsigned* ଗଣନା_2;
      ଫେରନ୍ତୁ  parents.has(parent_index, &ଗଣନା_2) ? *ଗଣନା_2 : 0;
    }

    ଶୂନ୍ୟ reset_parents ()
    {
      incoming_edges_ = 0;
      has_incoming_virtual_edges_ = ମିଥ୍ୟା;
      single_parent = HB_GRAPH_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    graph_result_t<ଶୂନ୍ୟ> add_parent (unsigned parent_index, ତାର୍କିକ is_virtual)
    {
      ପ୍ରତିଜ୍ଞା (parent_index != HB_GRAPH_INVALID);
      has_incoming_virtual_edges_ |= is_virtual;
      ଯଦି (is_virtual)
        virtual_parents.add (parent_index);

      ଯଦି (incoming_edges_ == 0)
      {
	single_parent = parent_index;
	incoming_edges_ = 1;
	ଫେରନ୍ତୁ to_result();
      }
      ଅନ୍ୟଥା ଯଦି (single_parent != HB_GRAPH_INVALID)
      {
        ପ୍ରତିଜ୍ଞା (incoming_edges_ == 1);
	ଯଦି (!parents.set (single_parent, 1))
	  ଫେରନ୍ତୁ to_result ();
	single_parent = HB_GRAPH_INVALID;
      }

      unsigned *v;
      ଯଦି (parents.has (parent_index, &v))
      {
        (*v)++;
	incoming_edges_++;
      }
      ଅନ୍ୟଥା ଯଦି (parents.set (parent_index, 1))
	incoming_edges_++;

      ଫେରନ୍ତୁ to_result();
    }

    ଶୂନ୍ୟ remove_parent (unsigned parent_index)
    {
      ଯଦି (parent_index == single_parent)
      {
	single_parent = HB_GRAPH_INVALID;
	incoming_edges_--;
	virtual_parents.reset ();
	ଫେରନ୍ତୁ;
      }

      unsigned *v;
      ଯଦି (parents.has (parent_index, &v))
      {
	incoming_edges_--;
	ଯଦି (*v > 1)
	  (*v)--;
	ଅନ୍ୟଥା
        {
	  parents.del (parent_index);
          virtual_parents.del (parent_index);
        }

	ଯଦି (incoming_edges_ == 1)
	{
	  single_parent = *parents.ଚାବିଗୁଡ଼ିକ ();
	  parents.reset ();
	}
      }
    }

    ଶୂନ୍ୟ remove_real_link_unordered (unsigned link_array_index) {
      obj_.real_links.remove_unordered (link_array_index);
    }

    ଶୂନ୍ୟ remove_real_link_unordered (unsigned child_index, ଅଚଳ ଶୂନ୍ୟ* offset)
    {
      unsigned ଗଣନା_2 = obj_.real_links.length;
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      {
        auto& link = obj_.real_links.arrayZ[i];
        ଯଦି (link.objidx != child_index)
          ଜାରି;

        ଯଦି ((obj_.head + link.position) != offset)
          ଜାରି;

        obj_.real_links.remove_unordered (i);
        ଫେରନ୍ତୁ;
      }
    }

    graph_result_t<ଶୂନ୍ୟ> remap_parent (unsigned old_index, unsigned new_index)
    {
      ଯଦି (single_parent != HB_GRAPH_INVALID)
      {
        ଯଦି (single_parent == old_index)
        {
	  single_parent = new_index;
          ଯଦି (virtual_parents.has (old_index))
          {
            virtual_parents.del (old_index);
            virtual_parents.add (new_index);
          }
        }
        ଫେରନ୍ତୁ graph_result_t<ଶୂନ୍ୟ>::from(virtual_parents, ALLOCATION_FAILURE);
      }

      ଅଚଳ unsigned *pv;
      ଯଦି (parents.has (old_index, &pv))
      {
        unsigned v = *pv;
	ଯଦି (!parents.set (new_index, v))
          incoming_edges_ -= v;
	parents.del (old_index);

        ଯଦି (incoming_edges_ == 1)
	{
	  single_parent = *parents.ଚାବିଗୁଡ଼ିକ ();
	  parents.reset ();
	}
      }

      ଯଦି (virtual_parents.has (old_index))
      {
        virtual_parents.del (old_index);
        virtual_parents.add (new_index);
      }

      ଫେରନ୍ତୁ graph_result_t<ଶୂନ୍ୟ>::from(virtual_parents, ALLOCATION_FAILURE);
    }

    ତାର୍କିକ is_leaf () ଅଚଳ
    {
      ଫେରନ୍ତୁ !obj_.real_links.length && !obj_.virtual_links.length;
    }

    ତାର୍କିକ raise_priority ()
    {
      ଯଦି (has_max_priority ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      priority++;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ତାର୍କିକ give_max_priority ()
    {
      ତାର୍କିକ result = ମିଥ୍ୟା;
      ଯେତେବେଳେ (!has_max_priority()) {
        result = ସତ୍ୟ;
        priority++;
      }
      ଫେରନ୍ତୁ result;
    }

    ତାର୍କିକ has_max_priority () ଅଚଳ {
      ଫେରନ୍ତୁ priority >= 3;
    }

    size_t table_size () ଅଚଳ {
      ଫେରନ୍ତୁ obj_.tail - obj_.head;
    }

    int64_t modified_distance (unsigned order) ଅଚଳ
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_distance =
          hb_clamp (distance + distance_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      ଯଦି (has_max_priority ()) {
        modified_distance = 0;
      }
      ଫେରନ୍ତୁ (modified_distance << 18) | (0x003FFFF & order);
    }

    int64_t distance_modifier () ଅଚଳ
    {
      ଯଦି (!priority) ଫେରନ୍ତୁ 0;
      int64_t table_size = obj_.tail - obj_.head;

      ଯଦି (priority == 1)
        ଫେରନ୍ତୁ -table_size / 2;

      ଫେରନ୍ତୁ -table_size;
    }

   ବ୍ୟକ୍ତିଗତ:
    graph_result_t<ଶୂନ୍ୟ> to_result() ଅଚଳ {
      ଯଦି (unlikely (
        parents.in_error() || virtual_parents.in_error() ||
        obj_.real_links.in_error() || obj_.virtual_links.in_error()))
        ଫେରନ୍ତୁ Err(ALLOCATION_FAILURE);
      ଫେରନ୍ତୁ Ok();
    }

    ତାର୍କିକ links_equal (ଅଚଳ hb_vector_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      ଅଚଳ hb_vector_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      ଅଚଳ graph_t& graph,
                      ଅଚଳ graph_t& other_graph,
                      unsigned depth) ଅଚଳ
    {
      auto a = this_links.iter ();
      auto b = other_links.iter ();

      ଯେତେବେଳେ (a && b)
      {
        ଅଚଳ auto& link_a = *a;
        ଅଚଳ auto& link_b = *b;

        ଯଦି (link_a.width != link_b.width ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଯଦି (!graph.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_graph.vertices_[link_b.objidx], graph, other_graph, depth + 1))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        a++;
        b++;
      }

      ଯଦି (ତାର୍କିକ (a) != ତାର୍କିକ (b))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
  };

  ଗଣନା vertex_mutability_t
  {
    Immutable,
    Mutable
  };

  template <typename T, vertex_mutability_t mutability = Mutable>
  ସଂରଚନା vertex_and_table_t
  {
    vertex_and_table_t () : index (0), vertex (nullptr), table (nullptr)
    {}

    template <vertex_mutability_t other_mutability>
    vertex_and_table_t (ଅଚଳ vertex_and_table_t<T, other_mutability>& o)
      : index (o.index), vertex (o.vertex), table (o.table)
    {}

    unsigned index;
    typename std::conditional<mutability == Immutable, ଅଚଳ vertex_t*, vertex_t*>::type vertex;
    typename std::conditional<mutability == Immutable, ଅଚଳ T*, T*>::type table;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  template<typename T>
  ସ୍ଥିର graph_result_t<graph_t> create (ଅଚଳ T& objects)
  {
    ଯଦି (unlikely (objects.length > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.length);
      ଫେରନ୍ତୁ Err(LIMIT_EXCEEDED);
    }

    graph_t g;
    g.ଅଙ୍କ_roots_for_space.push (1);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (g.ଅଙ୍କ_roots_for_space, ALLOCATION_FAILURE));

    ତାର୍କିକ removed_nil = ମିଥ୍ୟା;
    ଅଚଳ unsigned ଗଣନା_2 = objects.length;
    g.vertices_.alloc (ଗଣନା_2);
    g.ordering_.resize (ଗଣନା_2);
    g.ordering_scratch_.alloc (ଗଣନା_2);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (g.vertices_, ALLOCATION_FAILURE));
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (g.ordering_, ALLOCATION_FAILURE));
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (g.ordering_scratch_, ALLOCATION_FAILURE));

    unsigned order = objects.length;
    unsigned skip = 0;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      ଯଦି (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = ସତ୍ୟ;
        order--;
        g.ordering_.resize (ଗଣନା_2 - 1);
        skip++;
        ଜାରି;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      ଯଦି (unlikely (obj.in_error ())) {
        ଫେରନ୍ତୁ Err(ALLOCATION_FAILURE);
      }
      vertex_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (ଗଣନା_2, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      unsigned obj_idx = i - skip;
      g.ordering_[--order] = obj_idx;

      ଯଦି (!removed_nil) ଜାରି;
      // Fix indices to account for removed nil object.
      ପାଇଁ (auto& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    auto r = g.is_fully_connected();
    ଯଦି (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    ଫେରନ୍ତୁ Ok (std::move (g));
  }

  graph_t (ଅଚଳ graph_t&) = delete;
  graph_t& operator = (ଅଚଳ graph_t&) = delete;
  graph_t (graph_t&&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  graph_t& operator = (graph_t&&) = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  ~graph_t ()
  {
    ପାଇଁ (ଅକ୍ଷର* b : buffers)
      hb_free (b);
  }

ବ୍ୟକ୍ତିଗତ:
  graph_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

ସର୍ବସାଧାରଣ:

  ତାର୍କିକ operator== (ଅଚଳ graph_t& other) ଅଚଳ
  {
    ଫେରନ୍ତୁ ମୂଳ ().equals (ମୂଳ_idx(), other.ମୂଳ_idx(), other.ମୂଳ (), *ଏହି, other, 0);
  }

  ଶୂନ୍ୟ print () ଅଚଳ {
    ପାଇଁ (unsigned id : ordering_)
    {
      ଅଚଳ auto& v = vertices_[id];
      printf("%u: %u [", id, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v.table_size());
      ପାଇଁ (ଅଚଳ auto &l : v.obj().real_links) {
        printf("%u, ", l.objidx);
      }
      ପାଇଁ (ଅଚଳ auto &l : v.obj().virtual_links) {
        printf("v%u, ", l.objidx);
      }
      printf("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  ଶୂନ୍ୟ normalize ()
  {
    ପାଇଁ (auto& v : vertices_.writer ())
      v.normalize ();
  }

  ଅଚଳ vertex_t& ମୂଳ () ଅଚଳ
  {
    ଫେରନ୍ତୁ vertices_[ମୂଳ_idx ()];
  }

  unsigned ମୂଳ_idx () ଅଚଳ
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    ଫେରନ୍ତୁ ordering_[0];
  }

  ଅଚଳ hb_serialize_context_t::object_t& object (unsigned i) ଅଚଳ
  {
    ଫେରନ୍ତୁ vertices_[i].obj();
  }

  graph_result_t<ଶୂନ୍ୟ> add_buffer (ଅକ୍ଷର* buffer)
  {
    buffers.push (buffer);
    ଫେରନ୍ତୁ graph_result_t<ଶୂନ୍ୟ>::from (buffers, ALLOCATION_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  template<typename T>
  graph_result_t<ଶୂନ୍ୟ> add_link (T* offset,
                                 unsigned parent_id,
                                 unsigned child_id)
  {
    ଯଦି (unlikely (parent_id >= vertices_.length || child_id >= vertices_.length))
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);
    auto& v = vertices_[parent_id];
    TRY(v.add_real_link(2, child_id, (ଅକ୍ଷର*) offset - (ଅକ୍ଷର*) v.obj().head));
    ଫେରନ୍ତୁ vertices_[child_id].add_parent (parent_id, ମିଥ୍ୟା);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  graph_result_t<ଶୂନ୍ୟ> sort_shortest_distance_if_needed ()
  {
    ଯଦି (!positions_invalid) ଫେରନ୍ତୁ Ok();
    ଫେରନ୍ତୁ sort_shortest_distance ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  graph_result_t<ଶୂନ୍ୟ> sort_shortest_distance ()
  {
    positions_invalid = ସତ୍ୟ;

    ଯଦି (vertices_.length <= 1) {
      // Graph of 1 or less doesn't need sorting.
      ଫେରନ୍ତୁ Ok();
    }

    TRY (update_distances ());

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (vertices_.length);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.length);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (new_ordering, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> removed_edges;
    removed_edges.resize (vertices_.length);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (removed_edges, ALLOCATION_FAILURE));

    TRY (update_parents ());

    queue.insert (ମୂଳ ().modified_distance (0), ମୂଳ_idx ());
    unsigned order = 1;
    unsigned pos = 0;
    ଯେତେବେଳେ (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_id = queue.କାଢ଼_minimum().second;

      ଯଦି (unlikely (pos >= new_ordering.length)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Invalid graph. Contains cycle.");
        ଫେରନ୍ତୁ Err(CYCLE_DETECTED);
      }
      new_ordering[pos++] = next_id;
      ଅଚଳ vertex_t& next = vertices_[next_id];

      ପାଇଁ (ଅଚଳ auto& link : next.obj().all_links ()) {
        ଯଦି (unlikely (link.objidx >= vertices_.length))
          ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);
        removed_edges[link.objidx]++;
        ଅଚଳ auto& v = vertices_[link.objidx];
        ଯଦି (!(v.incoming_edges () - removed_edges[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          queue.insert (v.modified_distance (order++),
                        link.objidx);
      }
    }

    TRY (graph_result_t<ଶୂନ୍ୟ>::from (queue, ALLOCATION_FAILURE));
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (new_ordering, ALLOCATION_FAILURE));

    hb_swap (ordering_, new_ordering);

    ଯଦି (unlikely (pos != vertices_.length)) {
      print_orphaned_nodes ();
      ଫେରନ୍ତୁ Err(ORPHANED_NODES);
    }

    ଫେରନ୍ତୁ Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  graph_result_t<ଶୂନ୍ୟ> find_space_roots (hb_set_t& visited, hb_set_t& roots)
  {
    unsigned ମୂଳ_index = ମୂଳ_idx ();
    ପାଇଁ (unsigned i : ordering_)
    {
      ଯଦି (visited.has (i)) ଜାରି;

      // Only real links can form 32 bit spaces
      ପାଇଁ (ଅଚଳ auto& l : vertices_[i].obj().real_links)
      {
        ଯଦି (l.is_signed || l.width < 3)
          ଜାରି;

        ଯଦି (i == ମୂଳ_index && l.width == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          ଜାରି;

        ଯଦି (l.width >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_set_t sub_roots;
          TRY (find_32bit_roots (l.objidx, sub_roots));
          ଯଦି (sub_roots) {
            ପାଇଁ (unsigned sub_ମୂଳ_idx : sub_roots) {
              roots.add (sub_ମୂଳ_idx);
              TRY (find_subgraph (sub_ମୂଳ_idx, visited));
            }
            ଜାରି;
          }
        }

        roots.add (l.objidx);
        TRY (find_subgraph (l.objidx, visited));
      }
    }

    TRY(graph_result_t<ଶୂନ୍ୟ>::from (visited, ALLOCATION_FAILURE));
    TRY(graph_result_t<ଶୂନ୍ୟ>::from (roots, ALLOCATION_FAILURE));
    ଫେରନ୍ତୁ Ok();
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Immutable>> as_table (unsigned parent, ଅଚଳ ଶୂନ୍ୟ* offset, Ts... ds)
  {
    TRY_ASSIGN (unsigned idx, index_for_offset (parent, offset));
    ଫେରନ୍ତୁ as_table_from_index<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Mutable>> as_mutable_table (unsigned parent, ଅଚଳ ଶୂନ୍ୟ* offset, Ts... ds)
  {
    TRY_ASSIGN (auto idx, mutable_index_for_offset (parent, offset));
    ଫେରନ୍ତୁ as_table_from_index<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, vertex_mutability_t mutability = Mutable, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, mutability>> as_table_from_index (unsigned index, Ts... ds)
  {
    ଯଦି (unlikely (index >= vertices_.length))
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

    vertex_and_table_t<T, mutability> r;
    r.vertex = (typename std::conditional<mutability == Immutable, ଅଚଳ vertex_t*, vertex_t*>::type) &vertices_[index];
    r.table = r.vertex->template as<typename std::conditional<mutability == Immutable, ଅଚଳ T*, T*>::type> ();
    r.index = index;
    ଯଦି (unlikely (!r.table))
      ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);

    TRY(r.table->sanitize (*(r.vertex), std::forward<Ts>(ds)...));
    ଫେରନ୍ତୁ r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  graph_result_t<unsigned> index_for_offset (unsigned ଗଣ୍ଠି_idx, ଅଚଳ ଶୂନ୍ୟ* offset) ଅଚଳ
  {
    ଯଦି (unlikely (ଗଣ୍ଠି_idx >= vertices_.length)) ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);
    ଅଚଳ auto& node = object (ଗଣ୍ଠି_idx);
    ଯଦି (unlikely (offset < node.head || offset >= node.tail)) ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

    unsigned ଗଣନା_2 = node.real_links.length;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      ଅଚଳ auto& link = node.real_links.arrayZ[i];
      ଯଦି (offset != node.head + link.position)
        ଜାରି;
      ଫେରନ୍ତୁ Ok(link.objidx);
    }

    ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  graph_result_t<unsigned> mutable_index_for_offset (unsigned ଗଣ୍ଠି_idx, ଅଚଳ ଶୂନ୍ୟ* offset)
  {
    TRY_ASSIGN (unsigned child_idx, index_for_offset (ଗଣ୍ଠି_idx, offset));
    ଯଦି (unlikely (child_idx >= vertices_.length))
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

    auto& child = vertices_[child_idx];
    ପାଇଁ (unsigned p : child.parents_iter ())
    {
      ଯଦି (p != ଗଣ୍ଠି_idx) {
        ଫେରନ୍ତୁ duplicate (ଗଣ୍ଠି_idx, child_idx, ସତ୍ୟ);
      }
    }

    ଫେରନ୍ତୁ Ok(child_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  graph_result_t<ତାର୍କିକ> assign_spaces ()
  {
    TRY (update_parents ());

    ପାଇଁ (auto& vertex : vertices_.writer ())
    {
      vertex.space = 0;
      vertex.priority = 0;
    }
    ଅଙ୍କ_roots_for_space.resize (1);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (ଅଙ୍କ_roots_for_space, ALLOCATION_FAILURE));
    ଅଙ୍କ_roots_for_space[0] = 1;
    distance_invalid = ସତ୍ୟ;
    positions_invalid = ସତ୍ୟ;

    hb_set_t visited;
    hb_set_t roots;
    TRY (find_space_roots (visited, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    visited.invert ();

    ଯଦି (!roots) ଫେରନ୍ତୁ Ok(ମିଥ୍ୟା);

    ଯେତେବେଳେ (roots)
    {
      uint32_t next = HB_SET_ମୂଲ୍ୟ_INVALID;
      TRY (graph_result_t<ଶୂନ୍ୟ>::from (roots, ALLOCATION_FAILURE));
      ଯଦି (!roots.next (&next)) ଭାଙ୍ଗ;

      hb_set_t connected_roots;
      TRY (find_connected_nodes (next, roots, visited, connected_roots));
      TRY (graph_result_t<ଶୂନ୍ୟ>::from (connected_roots, ALLOCATION_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (graph_result_t<ଶୂନ୍ୟ>::from (connected_roots, ALLOCATION_FAILURE));

      unsigned next_space = ଏହି->next_space ();
      ଯଦି (unlikely (next_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 next_space);
        ଫେରନ୍ତୁ Err(LIMIT_EXCEEDED);
      }
      ଅଙ୍କ_roots_for_space.push (0);
      TRY(graph_result_t<ଶୂନ୍ୟ>::from (ଅଙ୍କ_roots_for_space, ALLOCATION_FAILURE));

      ପାଇଁ (unsigned ମୂଳ : connected_roots)
      {
        DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Subgraph %u gets space %u", ମୂଳ, next_space);
        vertices_[ମୂଳ].space = next_space;
        ଅଙ୍କ_roots_for_space[next_space] = ଅଙ୍କ_roots_for_space[next_space] + 1;
        distance_invalid = ସତ୍ୟ;
        positions_invalid = ସତ୍ୟ;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    ଫେରନ୍ତୁ Ok(ସତ୍ୟ);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  graph_result_t<ତାର୍କିକ> isolate_subgraph (hb_set_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_set_t parents;
    ପାଇଁ (unsigned ମୂଳ_idx : roots)
    {
      ଯଦି (unlikely (ମୂଳ_idx >= vertices_.length))
        ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);
      subgraph.set (ମୂଳ_idx, wide_parents (ମୂଳ_idx, parents));
    }
    ପାଇଁ (unsigned ମୂଳ_idx : roots)
    {
      TRY (find_subgraph (ମୂଳ_idx, subgraph));
    }
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (subgraph, ALLOCATION_FAILURE));

    hb_map_t index_map;
    ତାର୍କିକ made_changes = ମିଥ୍ୟା;
    ପାଇଁ (auto entry : subgraph.iter ())
    {
      ପ୍ରତିଜ୍ଞା (entry.first < vertices_.length);
      ଅଚଳ auto& node = vertices_[entry.first];
      unsigned subgraph_incoming_edges = entry.second;

      ଯଦି (subgraph_incoming_edges < node.incoming_edges ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = ସତ୍ୟ;
        TRY (duplicate_subgraph (entry.first, index_map));
      }
    }

    TRY (graph_result_t<ଶୂନ୍ୟ>::from (index_map, ALLOCATION_FAILURE));

    ଯଦି (!made_changes)
      ଫେରନ୍ତୁ Ok(ମିଥ୍ୟା);

    auto new_subgraph =
        + subgraph.ଚାବିଗୁଡ଼ିକ ()
        | hb_map([&] (uint32_t ଗଣ୍ଠି_idx) {
          ଅଚଳ uint32_t *v;
          ଯଦି (index_map.has (ଗଣ୍ଠି_idx, &v)) ଫେରନ୍ତୁ *v;
          ଫେରନ୍ତୁ ଗଣ୍ଠି_idx;
        })
        ;

    TRY(remap_obj_indices (index_map, new_subgraph));
    TRY(remap_obj_indices (index_map, parents.iter (), ସତ୍ୟ));

    // Update roots set with new indices as needed.
    ପାଇଁ (auto next : roots)
    {
      ଅଚଳ uint32_t *v;
      ଯଦି (index_map.has (next, &v))
      {
        roots.del (next);
        roots.add (*v);
      }
    }

    ଫେରନ୍ତୁ Ok(ସତ୍ୟ);
  }

  // BFS graph traversal starting at start_idx.
  //
  // The visit_edge function will be called once for the root node and then for each traversed edge
  // with the following signature:
  //
  // bool VisitEdgeFunc(unsigned parent, const link_t* link, unsigned child, unsigned depth)
  //
  // Where a return value of false signals that traversal should not continue into child's outgoing
  // edges. parent/child are the vertex indices. depth starts at 0 for the root node.
  // link will be null when called for entering the root node at the start of the traversal.
  //
  // This traversal does not use a visited set internally, it is the responsibility of visit_edge
  // to track and filter visited nodes if required for the particular traversal.
  template <typename VisitEdgeFunc>
  graph_result_t<ଶୂନ୍ୟ> traverse_directed_bfs (unsigned start_idx, VisitEdgeFunc&& visit_edge)
  {
    ଯଦି (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (ordering_scratch_, ALLOCATION_FAILURE));

    ଯଦି (!visit_edge (HB_CODEPOINT_INVALID, nullptr, start_idx, 0))
      ଫେରନ୍ତୁ Ok();

    unsigned head = 0;
    unsigned tail = 0;
    auto& queue = ordering_scratch_;
    queue[tail++] = start_idx;

    unsigned depth = 0;
    ଯେତେବେଳେ (head < tail)
    {
      unsigned level_end = tail;
      ଯେତେବେଳେ (head < level_end)
      {
        unsigned ଗଣ୍ଠି_idx = queue[head++];
        ଅଚଳ auto& v = vertices_[ଗଣ୍ଠି_idx];

        unsigned ଅଙ୍କ_real = v.obj().real_links.length;
        unsigned total_links = ଅଙ୍କ_real + v.obj().virtual_links.length;

        ପାଇଁ (unsigned i = 0; i < total_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          ଅଚଳ auto& v = vertices_[ଗଣ୍ଠି_idx];
          ଅଚଳ auto& link = (i < ଅଙ୍କ_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - ଅଙ୍କ_real];
          unsigned child_idx = link.objidx;

          ଯଦି (!visit_edge (ଗଣ୍ଠି_idx, &link, child_idx, depth + 1))
            ଜାରି;

          ଯଦି (unlikely (tail >= queue.length))
            ଫେରନ୍ତୁ Err(CYCLE_DETECTED);

          queue[tail++] = child_idx;
        }
      }
      depth++;
    }
    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<ଶୂନ୍ୟ> find_subgraph (unsigned ଗଣ୍ଠି_idx, hb_map_t& subgraph)
  {
    ଫେରନ୍ତୁ traverse_directed_bfs (ଗଣ୍ଠି_idx, [&] (
      unsigned parent,
      ଅଚଳ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      ଯଦି (depth == 0) ଫେରନ୍ତୁ ସତ୍ୟ;
      hb_codepoint_t *ଗଣନା_2;
      ଯଦି (subgraph.has (child, &ଗଣନା_2))
      {
        (*ଗଣନା_2)++;
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      subgraph.set (child, 1);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    });
  }

  graph_result_t<ଶୂନ୍ୟ> find_subgraph (unsigned ଗଣ୍ଠି_idx, hb_set_t& subgraph)
  {
    ଫେରନ୍ତୁ traverse_directed_bfs (ଗଣ୍ଠି_idx, [&] (
      unsigned parent,
      ଅଚଳ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      ଯଦି (subgraph.has (child)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      subgraph.add (child);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    });
  }

  graph_result_t<size_t> find_subgraph_size (unsigned ଗଣ୍ଠି_idx, hb_set_t& subgraph, unsigned max_depth = HB_GRAPH_INVALID)
  {
    size_t size = 0;
    TRY (traverse_directed_bfs (ଗଣ୍ଠି_idx, [&] (
      unsigned parent,
      ଅଚଳ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      ଯଦି (subgraph.has (child)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      subgraph.add (child);

      ଅଚଳ auto& o = vertices_[child].obj();
      size += o.tail - o.head;
      ଫେରନ୍ତୁ depth < max_depth;
    }));
    ଫେରନ୍ତୁ Ok(size);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  graph_result_t<ଶୂନ୍ୟ> find_32bit_roots (unsigned ଗଣ୍ଠି_idx, hb_set_t& found)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_set_t visited;
    ଫେରନ୍ତୁ traverse_directed_bfs (ଗଣ୍ଠି_idx, [&] (
      unsigned parent,
      ଅଚଳ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {

      ଯଦି (link && found.has(parent))
        // Don't traverse from something that's already marked as a root.
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (link && !link->is_signed && link->width == 4)
      {
        found.add (link->objidx);
        visited.add (link->objidx);
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      ଯଦି (visited.has (child)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      visited.add (child);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  template<typename O>
  graph_result_t<unsigned> move_child (unsigned old_parent_idx,
                                       ଅଚଳ O* old_offset,
                                       unsigned new_parent_idx,
                                       ଅଚଳ O* new_offset)
  {
    distance_invalid = ସତ୍ୟ;
    positions_invalid = ସତ୍ୟ;

    ଯଦି (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    TRY_ASSIGN (unsigned child_id, index_for_offset (old_parent_idx,
                                                     old_offset));
    ଯଦି (unlikely (child_id >= vertices_.length))
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

    TRY(new_v.add_real_link(O::static_size, child_id, (ଅଚଳ ଅକ୍ଷର*) new_offset - (ଅଚଳ ଅକ୍ଷର*) new_v.obj().head));

    auto& child = vertices_[child_id];
    TRY(child.add_parent (new_parent_idx, ମିଥ୍ୟା));

    old_v.remove_real_link_unordered (child_id, old_offset);
    child.remove_parent (old_parent_idx);

    ଫେରନ୍ତୁ Ok(child_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  template<typename O>
  graph_result_t<ଶୂନ୍ୟ> move_children (unsigned old_parent_idx,
                                      unsigned old_pos_start,
                                      unsigned old_pos_end,
                                      unsigned new_parent_idx,
                                      unsigned new_pos_start)
  {
    ଯଦି (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

    distance_invalid = ସତ୍ୟ;
    positions_invalid = ସତ୍ୟ;

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    hb_vector_t<hb_serialize_context_t::object_t::link_t> old_links;
    ପାଇଁ (ଅଚଳ auto& l : old_v.obj().real_links)
    {
      ଯଦି (l.position < old_pos_start || l.position >= old_pos_end)
      {
        old_links.push (l);
        ଜାରି;
      }

      unsigned array_pos = l.position - old_pos_start;

      unsigned child_id = l.objidx;
      ଯଦି (unlikely (child_id >= vertices_.length))
        ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

      TRY(new_v.add_real_link(O::static_size, child_id, new_pos_start + array_pos));

      auto& child = vertices_[child_id];
      TRY(child.add_parent (new_parent_idx, ମିଥ୍ୟା));
      child.remove_parent (old_parent_idx);
    }

    TRY (graph_result_t<ଶୂନ୍ୟ>::from (old_links, ALLOCATION_FAILURE));
    ଫେରନ୍ତୁ old_v.set_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  graph_result_t<ଶୂନ୍ୟ> duplicate_subgraph (unsigned ଗଣ୍ଠି_idx, hb_map_t& index_map)
  {
    graph_result_t<ଶୂନ୍ୟ> result = Ok();
    TRY (traverse_directed_bfs (ଗଣ୍ଠି_idx, [&] (
      unsigned parent,
      ଅଚଳ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {
      ଯଦି (result.is_err () || index_map.has (child)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      auto r = duplicate (child);
      ଯଦି (unlikely (r.is_err ()))
      {
        result = Err(r.error ());
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      index_map.set (child, *r);
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }));

    ଫେରନ୍ତୁ graph_result_t<ଶୂନ୍ୟ>::from(index_map, ALLOCATION_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  graph_result_t<unsigned> duplicate (unsigned ଗଣ୍ଠି_idx, ତାର୍କିକ copy_table = ମିଥ୍ୟା)
  {
    ଯଦି (unlikely (ଗଣ୍ଠି_idx >= vertices_.length))
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

    ଯଦି (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      ଫେରନ୍ତୁ Err(LIMIT_EXCEEDED);
    }

    positions_invalid = ସତ୍ୟ;
    distance_invalid = ସତ୍ୟ;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (ordering_, ALLOCATION_FAILURE));

    auto& child = vertices_[ଗଣ୍ଠି_idx];

    unsigned table_size = child.obj().tail - child.obj().head;
    ଯଦି (copy_table && table_size)
    {
      ଅକ୍ଷର* buffer = (ଅକ୍ଷର*) hb_malloc (table_size);
      ଯଦି (unlikely (!buffer))
        ଫେରନ୍ତୁ Err(ALLOCATION_FAILURE);
      auto res = add_buffer (buffer);
      ଯଦି (unlikely (res.is_err ()))
      {
        hb_free (buffer);
        ଫେରନ୍ତୁ Err(res.error ());
      }
      hb_memcpy (buffer, child.obj().head, table_size);
      clone->set_buffer(buffer, buffer + table_size);
    }
    ଅନ୍ୟଥା
    {
      clone->set_buffer(child.obj().head, child.obj().tail);
    }
    clone->distance = child.distance;
    clone->space = child.space;
    clone->reset_parents ();

    ପାଇଁ (ଅଚଳ auto& l : child.obj().real_links)
    {
      TRY(clone->add_real_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, ମିଥ୍ୟା));
    }

    ପାଇଁ (ଅଚଳ auto& l : child.obj().virtual_links)
    {
      TRY(clone->add_virtual_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, ସତ୍ୟ));
    }

    ଫେରନ୍ତୁ Ok(clone_idx);
  }

  /*
   * Creates a copy of child and re-assigns the link from
   * parent to the clone. The copy is a shallow copy, objects
   * linked from child are not duplicated.
   *
   * Returns the index of the newly created duplicate.
   *
   * If the child_idx only has incoming edges from parent_idx,
   * duplication isn't possible and this will return an error.
   */
  graph_result_t<unsigned> duplicate (unsigned parent_idx, unsigned child_idx, ତାର୍କିକ copy_table = ମିଥ୍ୟା)
  {
    ଯଦି (unlikely (parent_idx >= vertices_.length || child_idx >= vertices_.length))
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    ଅଚଳ auto& child = vertices_[child_idx];
    unsigned links_to_child = child.incoming_edges_from_parent (parent_idx);

    ଯଦି (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "  Not duplicating %u => %u",
                 parent_idx, child_idx);
      ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
    }

    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "  Duplicating %u => %u",
               parent_idx, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx, copy_table));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    ଯଦି (parent_idx == clone_idx) parent_idx++;

    auto& parent = vertices_[parent_idx];
    unsigned ଗଣନା_2 = 0;
    unsigned ଅଙ୍କ_real = parent.obj().real_links.length;
    ପାଇଁ (auto& l : parent.all_links_writer ())
    {
      ଗଣନା_2++;
      ଯଦି (l.objidx != child_idx)
        ଜାରି;

      TRY(reassign_link (l, parent_idx, clone_idx, ଗଣନା_2 > ଅଙ୍କ_real));
    }

    ଫେରନ୍ତୁ Ok(clone_idx);
  }

  /*
   * Creates a copy of child and re-assigns the links from
   * parents to the clone. The copy is a shallow copy, objects
   * linked from child are not duplicated.
   *
   * Returns the index of the newly created duplicate.
   *
   * If the child_idx only has incoming edges from parents,
   * duplication isn't possible or duplication fails and this will
   * return an error.
   */
  graph_result_t<unsigned> duplicate (ଅଚଳ hb_set_t* parents, unsigned child_idx)
  {
    ଯଦି (unlikely (!parents || parents->is_empty ()))
      ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);

    ଯଦି (unlikely (child_idx >= vertices_.length))
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    ଅଚଳ auto& child = vertices_[child_idx];
    unsigned links_to_child = 0;
    unsigned last_parent = parents->get_max ();
    unsigned first_parent = parents->get_min ();
    ପାଇଁ (unsigned parent_idx : *parents) {
      ଯଦି (unlikely (parent_idx >= vertices_.length))
        ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);
      links_to_child += child.incoming_edges_from_parent (parent_idx);
    }

    ଯଦି (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "  Not duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);
      ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
    }

    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "  Duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx));

    ପାଇଁ (unsigned parent_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      ଯଦି (parent_idx == clone_idx) parent_idx++;
      auto& parent = vertices_[parent_idx];
      unsigned ଗଣନା_2 = 0;
      unsigned ଅଙ୍କ_real = parent.obj().real_links.length;
      ପାଇଁ (auto& l : parent.all_links_writer ())
      {
        ଗଣନା_2++;
        ଯଦି (l.objidx != child_idx)
          ଜାରି;

        TRY(reassign_link (l, parent_idx, clone_idx, ଗଣନା_2 > ଅଙ୍କ_real));
      }
    }

    ଫେରନ୍ତୁ Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  graph_result_t<unsigned> new_ଗଣ୍ଠି (ଅକ୍ଷର* head, ଅକ୍ଷର* tail)
  {
    ଯଦି (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      ଫେରନ୍ତୁ Err(LIMIT_EXCEEDED);
    }

    positions_invalid = ସତ୍ୟ;
    distance_invalid = ସତ୍ୟ;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (ordering_, ALLOCATION_FAILURE));

    clone->set_buffer (head, tail);
    clone->distance = 0;
    clone->space = 0;

    ଫେରନ୍ତୁ Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  graph_result_t<unsigned> remap_child (unsigned parent_idx, unsigned old_child_idx)
  {
    ଯଦି (unlikely (parent_idx >= vertices_.length || old_child_idx >= vertices_.length))
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

    TRY_ASSIGN (unsigned new_child_idx, duplicate (old_child_idx));

    auto& parent = vertices_[parent_idx];
    ପାଇଁ (auto& l : parent.real_links_writer ())
    {
      ଯଦି (l.objidx != old_child_idx)
        ଜାରି;
      TRY(reassign_link (l, parent_idx, new_child_idx, ମିଥ୍ୟା));
    }

    ପାଇଁ (auto& l : parent.virtual_links_writer ())
    {
      ଯଦି (l.objidx != old_child_idx)
        ଜାରି;
      TRY(reassign_link (l, parent_idx, new_child_idx, ସତ୍ୟ));
    }
    ଫେରନ୍ତୁ Ok(new_child_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  graph_result_t<ତାର୍କିକ> raise_childrens_priority (unsigned parent_idx)
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "  Raising priority of all children of %u",
               parent_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    ଯଦି (unlikely (parent_idx >= vertices_.length)) ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);
    ତାର୍କିକ made_change = ମିଥ୍ୟା;
    ପାଇଁ (auto& l : vertices_[parent_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_priority ();
    ଫେରନ୍ତୁ Ok(made_change);
  }

  graph_result_t<ଶୂନ୍ୟ> is_fully_connected ()
  {
    TRY(update_parents());

    ଯଦି (unlikely (ମୂଳ().incoming_edges ()))
      // Root cannot have parents.
      ଫେରନ୍ତୁ Err(ORPHANED_NODES);

    ପାଇଁ (unsigned i = 0; i < ମୂଳ_idx (); i++)
    {
      ଯଦି (unlikely (!vertices_[i].incoming_edges ()))
        ଫେରନ୍ତୁ Err(ORPHANED_NODES);
    }
    ଫେରନ୍ତୁ Ok();
  }

#ଯଦି 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  ଶୂନ୍ୟ save_fuzzer_seed (hb_tag_t tag) ଅଚଳ
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((ଶୂନ୍ୟ*) &tag, ଆକାର (tag), 1, f);

    uint16_t ଅଙ୍କ_objects = vertices_.length;
    fwrite ((ଶୂନ୍ୟ*) &ଅଙ୍କ_objects, ଆକାର (ଅଙ୍କ_objects), 1, f);

    ପାଇଁ (ଅଚଳ auto& v : vertices_)
    {
      uint16_t blob_size = v.table_size ();
      fwrite ((ଶୂନ୍ୟ*) &blob_size, ଆକାର (blob_size), 1, f);
      fwrite ((ଅଚଳ ଶୂନ୍ୟ*) v.obj.head, blob_size, 1, f);
    }

    uint16_t link_ଗଣନା = 0;
    ପାଇଁ (ଅଚଳ auto& v : vertices_)
      link_ଗଣନା += v.obj.real_links.length;

    fwrite ((ଶୂନ୍ୟ*) &link_ଗଣନା, ଆକାର (link_ଗଣନା), 1, f);

    ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
    {
      uint16_t parent;
      uint16_t child;
      uint16_t position;
      uint8_t width;
    } link_t;

    ପାଇଁ (unsigned i = 0; i < vertices_.length; i++)
    {
      ପାଇଁ (ଅଚଳ auto& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.width
        };
        fwrite ((ଶୂନ୍ୟ*) &link, ଆକାର (link), 1, f);
      }
    }

    fclose (f);
  }
#endif

  unsigned ଅଙ୍କ_roots_for_space_2 (unsigned space) ଅଚଳ
  {
    ଫେରନ୍ତୁ ଅଙ୍କ_roots_for_space[space];
  }

  unsigned next_space () ଅଚଳ
  {
    ଫେରନ୍ତୁ ଅଙ୍କ_roots_for_space.length;
  }

  graph_result_t<ଶୂନ୍ୟ> move_to_new_space (ଅଚଳ hb_set_t& indices)
  {
    ଯଦି (unlikely (ଅଙ୍କ_roots_for_space.length >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 ଅଙ୍କ_roots_for_space.length);
      ଫେରନ୍ତୁ Err(LIMIT_EXCEEDED);
    }
    ଅଙ୍କ_roots_for_space.push (0);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (ଅଙ୍କ_roots_for_space, ALLOCATION_FAILURE));
    unsigned new_space = ଅଙ୍କ_roots_for_space.length - 1;

    ପାଇଁ (unsigned index : indices) {
      ଯଦି (unlikely (index >= vertices_.length))
        ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);
      auto& node = vertices_[index];
      ଅଙ୍କ_roots_for_space[node.space] = ଅଙ୍କ_roots_for_space[node.space] - 1;
      ଅଙ୍କ_roots_for_space[new_space] = ଅଙ୍କ_roots_for_space[new_space] + 1;
      node.space = new_space;
      distance_invalid = ସତ୍ୟ;
      positions_invalid = ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ Ok();
  }

  unsigned space_for (unsigned index, unsigned* ମୂଳ = nullptr) ଅଚଳ
  {
    ଯେତେବେଳେ (ସତ୍ୟ)
    {
      ପ୍ରତିଜ୍ଞା (index < vertices_.length);
      ଅଚଳ auto& node = vertices_[index];
      ଯଦି (node.space)
      {
        ଯଦି (ମୂଳ != nullptr)
          *ମୂଳ = index;
        ଫେରନ୍ତୁ node.space;
      }

      ଯଦି (!node.incoming_edges ())
      {
        ଯଦି (ମୂଳ)
          *ମୂଳ = index;
        ଫେରନ୍ତୁ 0;
      }

      index = *node.parents_iter ();
    }
  }

  size_t total_size_in_bytes () ଅଚଳ {
    size_t total_size = 0;
    unsigned ଗଣନା_2 = vertices_.length;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++) {
      size_t size = vertices_.arrayZ[i].table_size ();
      total_size += size;
    }
    ଫେରନ୍ତୁ total_size;
  }


 ବ୍ୟକ୍ତିଗତ:

  ଶୂନ୍ୟ print_orphaned_nodes ()
  {
    ଯଦି (!DEBUG_ENABLED(SUBSET_REPACK)) ଫେରନ୍ତୁ;

    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Graph is not fully connected.");

    parents_invalid = ସତ୍ୟ;
    (ଶୂନ୍ୟ) update_parents();

    ଯଦି (ମୂଳ().incoming_edges ()) {
      DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Root node has incoming edges.");
    }

    ପାଇଁ (unsigned i = 0; i < vertices_.length; i++)
    {
      ଅଚଳ auto& v = vertices_[i];
      ଯଦି (!v.incoming_edges ())
        DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  unsigned wide_parents (unsigned ଗଣ୍ଠି_idx, hb_set_t& parents) ଅଚଳ
  {
    unsigned ଗଣନା_2 = 0;
    ପାଇଁ (unsigned p : vertices_[ଗଣ୍ଠି_idx].parents_iter ())
    {
      // Only real links can be wide
      ପାଇଁ (ଅଚଳ auto& l : vertices_[p].obj().real_links)
      {
        ଯଦି (l.objidx == ଗଣ୍ଠି_idx
            && (l.width == 3 || l.width == 4)
            && !l.is_signed)
        {
          ଗଣନା_2++;
          parents.add (p);
        }
      }
    }
    ଫେରନ୍ତୁ ଗଣନା_2;
  }

 ସର୍ବସାଧାରଣ:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  graph_result_t<ଶୂନ୍ୟ> update_parents ()
  {
    ଯଦି (!parents_invalid) ଫେରନ୍ତୁ Ok();

    unsigned ଗଣନା_2 = vertices_.length;

    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      vertices_.arrayZ[i].reset_parents ();

    ପାଇଁ (unsigned p = 0; p < ଗଣନା_2; p++)
    {
      ପାଇଁ (ଅଚଳ auto& l : vertices_.arrayZ[p].obj().real_links)
      {
        ଯଦି (unlikely (l.objidx >= ଗଣନା_2)) ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, ମିଥ୍ୟା));
      }

      ପାଇଁ (ଅଚଳ auto& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        ଯଦି (unlikely (l.objidx >= ଗଣନା_2)) ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, ସତ୍ୟ));
      }
    }

    parents_invalid = ମିଥ୍ୟା;
    ଫେରନ୍ତୁ Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  ଶୂନ୍ୟ update_positions ()
  {
    ଯଦି (!positions_invalid) ଫେରନ୍ତୁ;

    unsigned current_pos = 0;
    ପାଇଁ (unsigned i : ordering_)
    {
      auto& v = vertices_[i];
      v.start = current_pos;
      current_pos += v.obj().tail - v.obj().head;
      v.end = current_pos;
    }

    positions_invalid = ମିଥ୍ୟା;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  graph_result_t<ଶୂନ୍ୟ> update_distances ()
  {
    ଯଦି (!distance_invalid) ଫେରନ୍ତୁ Ok();

    // Uses Dijkstra's algorithm to find all of the shortest distances.
    // https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
    //
    // Implementation Note:
    // Since our priority queue doesn't support fast priority decreases
    // we instead just add new entries into the queue when a priority changes.
    // Redundant ones are filtered out later on by the visited set.
    // According to https://www3.cs.stonybrook.edu/~rezaul/papers/TR-07-54.pdf
    // for practical performance this is faster then using a more advanced queue
    // (such as a fibonacci queue) with a fast decrease priority.
    unsigned ଗଣନା_2 = vertices_.length;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[ମୂଳ_idx ()].distance = 0;

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (ଗଣନା_2);
    queue.insert (0, ମୂଳ_idx ());
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<ତାର୍କିକ> visited;
    visited.resize (vertices_.length);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (visited, ALLOCATION_FAILURE));

    ଯେତେବେଳେ (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_idx = queue.କାଢ଼_minimum ().second;
      ଯଦି (visited[next_idx]) ଜାରି;
      ଅଚଳ auto& next = vertices_[next_idx];
      int64_t next_distance = next.distance;
      visited[next_idx] = ସତ୍ୟ;

      ପାଇଁ (ଅଚଳ auto& link : next.obj().all_links ())
      {
        ଯଦି (unlikely (link.objidx >= vertices_.length))
          ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);

        ଯଦି (visited[link.objidx]) ଜାରି;

        auto& child_v = vertices_.arrayZ[link.objidx];
        ଅଚଳ auto& child = child_v.obj ();
        unsigned link_width = link.width ? link.width : 4; // treat virtual offsets as 32 bits wide
        int64_t child_weight = (child.tail - child.head) +
                               ((int64_t) 1 << (link_width * 8)) * (child_v.space + 1);
        int64_t child_distance = next_distance + child_weight;

        ଯଦି (child_distance < child_v.distance)
        {
          child_v.distance = child_distance;
          queue.insert (child_distance, link.objidx);
        }
      }
    }

    TRY (graph_result_t<ଶୂନ୍ୟ>::from (queue, ALLOCATION_FAILURE));
    ଯଦି (unlikely (!queue.is_empty ()))
    {
      print_orphaned_nodes ();
      ଫେରନ୍ତୁ Err(ORPHANED_NODES);
    }

    distance_invalid = ମିଥ୍ୟା;
    ଫେରନ୍ତୁ Ok();
  }

 ବ୍ୟକ୍ତିଗତ:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  graph_result_t<ଶୂନ୍ୟ> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      unsigned parent_idx,
                      unsigned new_idx,
                      ତାର୍କିକ is_virtual)
  {
    unsigned old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_parent (parent_idx);
    ଫେରନ୍ତୁ vertices_[new_idx].add_parent (parent_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  template<typename Iterator, hb_requires (hb_is_iterator (Iterator))>
  graph_result_t<ଶୂନ୍ୟ> remap_obj_indices (ଅଚଳ hb_map_t& id_map,
                                          Iterator subgraph,
                                          ତାର୍କିକ only_wide = ମିଥ୍ୟା)
  {
    ଯଦି (!id_map) ଫେରନ୍ତୁ Ok();
    ପାଇଁ (unsigned i : subgraph)
    {
      auto& vertex = vertices_[i];
      unsigned ଅଙ୍କ_real = vertex.obj().real_links.length;
      unsigned ଗଣନା_2 = 0;
      ପାଇଁ (auto& link : vertex.all_links_writer ())
      {
        ଗଣନା_2++;
        ଅଚଳ uint32_t *v;
        ଯଦି (!id_map.has (link.objidx, &v)) ଜାରି;
        ଯଦି (only_wide && (link.is_signed || (link.width != 4 && link.width != 3))) ଜାରି;

        TRY(reassign_link (link, i, *v, ଗଣନା_2 > ଅଙ୍କ_real));
      }
    }
    ଫେରନ୍ତୁ Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  graph_result_t<ଶୂନ୍ୟ> find_connected_nodes (unsigned start_idx,
                                             hb_set_t& targets,
                                             hb_set_t& visited,
                                             hb_set_t& connected)
  {
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (visited, ALLOCATION_FAILURE));
    ଯଦି (visited.has (start_idx)) ଫେରନ୍ତୁ Ok();
    ଯଦି (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (ordering_scratch_, ALLOCATION_FAILURE));

    auto& ଗଦା = ordering_scratch_;
    unsigned ଗଦା_len = 0;

    graph_result_t<ଶୂନ୍ୟ> res = Ok();
    auto handle_ଗଣ୍ଠି = [&] (unsigned ଗଣ୍ଠି_idx) {
      visited.add (ଗଣ୍ଠି_idx);
      ଯଦି (targets.has (ଗଣ୍ଠି_idx)) {
        targets.del (ଗଣ୍ଠି_idx);
        connected.add (ଗଣ୍ଠି_idx);
      }

      ଯଦି (unlikely (ଗଦା_len >= ଗଦା.length))
      {
        res = Err(CYCLE_DETECTED);
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      ଗଦା[ଗଦା_len++] = ଗଣ୍ଠି_idx;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    };

    ଯଦି (!handle_ଗଣ୍ଠି (start_idx)) ଫେରନ୍ତୁ res;

    ଯେତେବେଳେ (ଗଦା_len > 0)
    {
      unsigned ଗଣ୍ଠି_idx = ଗଦା[--ଗଦା_len];
      ଅଚଳ auto& v = vertices_[ଗଣ୍ଠି_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      ପାଇଁ (ଅଚଳ auto& l : v.obj().all_links ())
      {
        unsigned child_idx = l.objidx;
        ଯଦି (visited.has (child_idx)) ଜାରି;
        ଯଦି (!handle_ଗଣ୍ଠି (child_idx)) ଫେରନ୍ତୁ res;
      }

      ପାଇଁ (unsigned parent_idx : v.parents_iter ())
      {
        ଯଦି (visited.has (parent_idx)) ଜାରି;
        ଯଦି (!handle_ଗଣ୍ଠି (parent_idx)) ଫେରନ୍ତୁ res;
      }
    }

    TRY (graph_result_t<ଶୂନ୍ୟ>::from (visited, ALLOCATION_FAILURE));
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (connected, ALLOCATION_FAILURE));

    ଫେରନ୍ତୁ Ok();
  }

 ସର୍ବସାଧାରଣ:
  // TODO(garretrieger): make private, will need to move most of offset overflow code into graph.
  hb_vector_t<vertex_t> vertices_;

  // Specifies the current topological ordering of this graph
  //
  // ordering_[pos] = obj index
  //
  // specifies that the 'pos'th spot is filled by the object
  // given by obj index.
  hb_vector_t<unsigned> ordering_;
  hb_vector_t<unsigned> ordering_scratch_;

 ବ୍ୟକ୍ତିଗତ:
  ତାର୍କିକ parents_invalid = ସତ୍ୟ;
  ତାର୍କିକ distance_invalid = ସତ୍ୟ;
  ତାର୍କିକ positions_invalid = ସତ୍ୟ;
  hb_vector_t<unsigned> ଅଙ୍କ_roots_for_space;
  hb_vector_t<ଅକ୍ଷର*> buffers;
};

}

#endif  // GRAPH_GRAPH_HH
