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

#include "../hb-set.hh"
#include "../hb-priority-queue.hh"
#include "../hb-serialize.hh"
#include "graph-result.hh"

#ifndef GRAPH_GRAPH_HH
#define GRAPH_GRAPH_HH

#define HB_GRAPH_INVALID ((unsigned) -1)

namespace graph {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
sa_nracanaa graph_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  sa_nracanaa vertex_t
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

    private:
    hb_serialize_context_t::object_t obj_;
    unsigned incoming_edges_ = 0;
    unsigned single_parent = HB_GRAPH_INVALID;
    bool has_incoming_virtual_edges_ = false;
    hb_hashmap_t<unsigned, unsigned> parents;
    hb_set_t virtual_parents;
    public:

    vertex_t ()
    {}

    explicit vertex_t(const hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    auto parents_iter () const HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_parent, single_parent != HB_GRAPH_INVALID),
	parents.keys_ref ()
      )
    )

    void set_buffer (var_na* head, var_na* tail) {
      obj_.head = head;
      obj_.tail = tail;
    }

    void shrink_buffer (size_t count)
    {
      obj_.tail -= count;
    }

    auto all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    auto real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    auto virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    void sort_real_links () {
      obj_.real_links.qsort ();
    }

    graph_result_t<void> add_real_link (unsigned width, unsigned obj_idx, unsigned position) {
      auto* link = obj_.real_links.push ();
      TRY (graph_result_t<void>::from (obj_.real_links, ALLOCATION_FAILURE));
      link->width = width;
      link->objidx = obj_idx;
      link->position = position;
      ត្រឡប់ Ok();
    }

    graph_result_t<void> add_virtual_link (unsigned obj_idx) {
      auto* link = obj_.virtual_links.push ();
      TRY (graph_result_t<void>::from (obj_.virtual_links, ALLOCATION_FAILURE));
      link->objidx = obj_idx;
      ត្រឡប់ Ok();
    }

    graph_result_t<void> add_real_link (const hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      ត្រឡប់ graph_result_t<void>::from (obj_.real_links, ALLOCATION_FAILURE);
    }

    graph_result_t<void> set_real_links (hb_vector_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(graph_result_t<void>::from (links, ALLOCATION_FAILURE));
      obj_.real_links = links;
      ត្រឡប់ Ok();
    }

    void clear_real_links ()
    {
      obj_.real_links.reset();
    }

    void clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    graph_result_t<void> add_virtual_link (const hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      ត្រឡប់ graph_result_t<void>::from (obj_.virtual_links, ALLOCATION_FAILURE);
    }

    const hb_serialize_context_t::object_t& obj() const { ត្រឡប់ obj_; }

    template<typename T>
    T as () const
    {
      ត្រឡប់ reinterpret_cast<T> (obj_.head);
    }

    bool has_incoming_virtual_edges () const
    {
      ត្រឡប់ has_incoming_virtual_edges_;
    }

    graph_result_t<void> link_positions_valid (unsigned num_objects, bool removed_nil)
    {
      hb_set_t assigned_bytes;
      សម្រាប់ (const auto& l : obj_.real_links)
      {
        ប្រសិនបើ (unlikely (l.objidx >= num_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_MSG (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid object index.");
          ត្រឡប់ Err(INVALID_ARGUMENT);
        }

        unsigned start = l.position;
        unsigned end = start + l.width - 1;

        ប្រសិនបើ (unlikely (l.width < 2 || l.width > 4))
        {
          DEBUG_MSG (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid link width.");
          ត្រឡប់ Err(INVALID_ARGUMENT);
        }

        ប្រសិនបើ (unlikely (end >= table_size ()))
        {
          DEBUG_MSG (SUBSET_REPACK, nullptr,
                     "Invalid graph. Link position is out of bounds.");
          ត្រឡប់ Err(OUT_OF_BOUNDS);
        }

        ប្រសិនបើ (unlikely (assigned_bytes.intersects (start, end)))
        {
          DEBUG_MSG (SUBSET_REPACK, nullptr,
                     "Invalid graph. Found offsets whose positions overlap.");
          ត្រឡប់ Err(INVALID_ARGUMENT);
        }

        assigned_bytes.add_range (start, end);
      }

      ត្រឡប់ graph_result_t<void>::from (assigned_bytes, ALLOCATION_FAILURE);
    }

    void normalize ()
    {
      obj_.real_links.qsort ();
      សម្រាប់ (const auto& l : obj_.real_links)
      {
        សម្រាប់ (unsigned i = 0; i < l.width; i++)
        {
          obj_.head[l.position + i] = 0;
        }
      }
    }

    bool equals (unsigned this_index,
                 unsigned other_index,
                 const vertex_t& other,
                 const graph_t& graph,
                 const graph_t& other_graph,
                 unsigned depth) const
    {
      ប្រសិនបើ (!(as_bytes () == other.as_bytes ()))
      {
        DEBUG_MSG (SUBSET_REPACK, nullptr,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_index,
                   (unsigned long) table_size (),
                   other_index,
                   (unsigned long) other.table_size (),
                   depth);

        auto a = as_bytes ();
        auto b = other.as_bytes ();
        ខណៈពេល (a || b)
        {
          DEBUG_MSG (SUBSET_REPACK, nullptr,
                     "  0x%x %s 0x%x", (unsigned) *a, (*a == *b) ? "==" : "!=", (unsigned) *b);
          a++;
          b++;
        }
        ត្រឡប់ false;
      }

      ត្រឡប់ links_equal (obj_.real_links, other.obj_.real_links, graph, other_graph, depth);
    }

    hb_bytes_t as_bytes () const
    {
      ត្រឡប់ hb_bytes_t (obj_.head, table_size ());
    }

    friend void swap (vertex_t& a, vertex_t& b)
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
    position_to_index_map () const
    {
      hb_hashmap_t<unsigned, unsigned> result;

      result.alloc (obj_.real_links.length);
      សម្រាប់ (const auto& l : obj_.real_links) {
        result.set (l.position, l.objidx);
      }

      ត្រឡប់ graph_result_t<hb_hashmap_t<unsigned, unsigned>>::from (std::move(result), ALLOCATION_FAILURE);
    }

    bool is_shared () const
    {
      ត្រឡប់ parents.get_population () > virtual_parents.get_population () + 1;
    }

    unsigned incoming_edges () const
    {
      ប្រសិនបើ (HB_DEBUG_SUBSET_REPACK)
       {
	assert (incoming_edges_ == (single_parent != HB_GRAPH_INVALID) +
		(parents.values_ref () | hb_reduce (hb_add, 0)));
       }
      ត្រឡប់ incoming_edges_;
    }

    unsigned incoming_edges_from_parent (unsigned parent_index) const {
      ប្រសិនបើ (single_parent != HB_GRAPH_INVALID) {
        ត្រឡប់ single_parent == parent_index ? 1 : 0;
      }

      unsigned* count;
      ត្រឡប់  parents.has(parent_index, &count) ? *count : 0;
    }

    void reset_parents ()
    {
      incoming_edges_ = 0;
      has_incoming_virtual_edges_ = false;
      single_parent = HB_GRAPH_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    graph_result_t<void> add_parent (unsigned parent_index, bool is_virtual)
    {
      assert (parent_index != HB_GRAPH_INVALID);
      has_incoming_virtual_edges_ |= is_virtual;
      ប្រសិនបើ (is_virtual)
        virtual_parents.add (parent_index);

      ប្រសិនបើ (incoming_edges_ == 0)
      {
	single_parent = parent_index;
	incoming_edges_ = 1;
	ត្រឡប់ to_result();
      }
      បើមិនដូច្នេះ ប្រសិនបើ (single_parent != HB_GRAPH_INVALID)
      {
        assert (incoming_edges_ == 1);
	ប្រសិនបើ (!parents.set (single_parent, 1))
	  ត្រឡប់ to_result ();
	single_parent = HB_GRAPH_INVALID;
      }

      unsigned *v;
      ប្រសិនបើ (parents.has (parent_index, &v))
      {
        (*v)++;
	incoming_edges_++;
      }
      បើមិនដូច្នេះ ប្រសិនបើ (parents.set (parent_index, 1))
	incoming_edges_++;

      ត្រឡប់ to_result();
    }

    void remove_parent (unsigned parent_index)
    {
      ប្រសិនបើ (parent_index == single_parent)
      {
	single_parent = HB_GRAPH_INVALID;
	incoming_edges_--;
	virtual_parents.reset ();
	ត្រឡប់;
      }

      unsigned *v;
      ប្រសិនបើ (parents.has (parent_index, &v))
      {
	incoming_edges_--;
	ប្រសិនបើ (*v > 1)
	  (*v)--;
	បើមិនដូច្នេះ
        {
	  parents.del (parent_index);
          virtual_parents.del (parent_index);
        }

	ប្រសិនបើ (incoming_edges_ == 1)
	{
	  single_parent = *parents.keys ();
	  parents.reset ();
	}
      }
    }

    void remove_real_link_unordered (unsigned link_array_index) {
      obj_.real_links.remove_unordered (link_array_index);
    }

    void remove_real_link_unordered (unsigned child_index, const void* offset)
    {
      unsigned count = obj_.real_links.length;
      សម្រាប់ (unsigned i = 0; i < count; i++)
      {
        auto& link = obj_.real_links.arrayZ[i];
        ប្រសិនបើ (link.objidx != child_index)
          continue;

        ប្រសិនបើ ((obj_.head + link.position) != offset)
          continue;

        obj_.real_links.remove_unordered (i);
        ត្រឡប់;
      }
    }

    graph_result_t<void> remap_parent (unsigned old_index, unsigned new_index)
    {
      ប្រសិនបើ (single_parent != HB_GRAPH_INVALID)
      {
        ប្រសិនបើ (single_parent == old_index)
        {
	  single_parent = new_index;
          ប្រសិនបើ (virtual_parents.has (old_index))
          {
            virtual_parents.del (old_index);
            virtual_parents.add (new_index);
          }
        }
        ត្រឡប់ graph_result_t<void>::from(virtual_parents, ALLOCATION_FAILURE);
      }

      const unsigned *pv;
      ប្រសិនបើ (parents.has (old_index, &pv))
      {
        unsigned v = *pv;
	ប្រសិនបើ (!parents.set (new_index, v))
          incoming_edges_ -= v;
	parents.del (old_index);

        ប្រសិនបើ (incoming_edges_ == 1)
	{
	  single_parent = *parents.keys ();
	  parents.reset ();
	}
      }

      ប្រសិនបើ (virtual_parents.has (old_index))
      {
        virtual_parents.del (old_index);
        virtual_parents.add (new_index);
      }

      ត្រឡប់ graph_result_t<void>::from(virtual_parents, ALLOCATION_FAILURE);
    }

    bool is_leaf () const
    {
      ត្រឡប់ !obj_.real_links.length && !obj_.virtual_links.length;
    }

    bool raise_priority ()
    {
      ប្រសិនបើ (has_max_priority ()) ត្រឡប់ false;
      priority++;
      ត្រឡប់ true;
    }

    bool give_max_priority ()
    {
      bool result = false;
      ខណៈពេល (!has_max_priority()) {
        result = true;
        priority++;
      }
      ត្រឡប់ result;
    }

    bool has_max_priority () const {
      ត្រឡប់ priority >= 3;
    }

    size_t table_size () const {
      ត្រឡប់ obj_.tail - obj_.head;
    }

    int64_t modified_distance (unsigned order) const
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_distance =
          hb_clamp (distance + distance_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      ប្រសិនបើ (has_max_priority ()) {
        modified_distance = 0;
      }
      ត្រឡប់ (modified_distance << 18) | (0x003FFFF & order);
    }

    int64_t distance_modifier () const
    {
      ប្រសិនបើ (!priority) ត្រឡប់ 0;
      int64_t table_size = obj_.tail - obj_.head;

      ប្រសិនបើ (priority == 1)
        ត្រឡប់ -table_size / 2;

      ត្រឡប់ -table_size;
    }

   private:
    graph_result_t<void> to_result() const {
      ប្រសិនបើ (unlikely (
        parents.in_error() || virtual_parents.in_error() ||
        obj_.real_links.in_error() || obj_.virtual_links.in_error()))
        ត្រឡប់ Err(ALLOCATION_FAILURE);
      ត្រឡប់ Ok();
    }

    bool links_equal (const hb_vector_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      const hb_vector_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      const graph_t& graph,
                      const graph_t& other_graph,
                      unsigned depth) const
    {
      auto a = this_links.iter ();
      auto b = other_links.iter ();

      ខណៈពេល (a && b)
      {
        const auto& link_a = *a;
        const auto& link_b = *b;

        ប្រសិនបើ (link_a.width != link_b.width ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          ត្រឡប់ false;

        ប្រសិនបើ (!graph.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_graph.vertices_[link_b.objidx], graph, other_graph, depth + 1))
          ត្រឡប់ false;

        a++;
        b++;
      }

      ប្រសិនបើ (bool (a) != bool (b))
        ត្រឡប់ false;

      ត្រឡប់ true;
    }
  };

  ga_nanaa vertex_mutability_t
  {
    Immutable,
    Mutable
  };

  template <typename T, vertex_mutability_t mutability = Mutable>
  sa_nracanaa vertex_and_table_t
  {
    vertex_and_table_t () : index (0), vertex (nullptr), table (nullptr)
    {}

    template <vertex_mutability_t other_mutability>
    vertex_and_table_t (const vertex_and_table_t<T, other_mutability>& o)
      : index (o.index), vertex (o.vertex), table (o.table)
    {}

    unsigned index;
    typename std::conditional<mutability == Immutable, const vertex_t*, vertex_t*>::type vertex;
    typename std::conditional<mutability == Immutable, const T*, T*>::type table;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  template<typename T>
  static graph_result_t<graph_t> create (const T& objects)
  {
    ប្រសិនបើ (unlikely (objects.length > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_MSG (SUBSET_REPACK, nullptr,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.length);
      ត្រឡប់ Err(LIMIT_EXCEEDED);
    }

    graph_t g;
    g.num_roots_for_space_.push (1);
    TRY (graph_result_t<void>::from (g.num_roots_for_space_, ALLOCATION_FAILURE));

    bool removed_nil = false;
    const unsigned count = objects.length;
    g.vertices_.alloc (count);
    g.ordering_.resize (count);
    g.ordering_scratch_.alloc (count);
    TRY (graph_result_t<void>::from (g.vertices_, ALLOCATION_FAILURE));
    TRY (graph_result_t<void>::from (g.ordering_, ALLOCATION_FAILURE));
    TRY (graph_result_t<void>::from (g.ordering_scratch_, ALLOCATION_FAILURE));

    unsigned order = objects.length;
    unsigned skip = 0;
    សម្រាប់ (unsigned i = 0; i < count; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      ប្រសិនបើ (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = true;
        order--;
        g.ordering_.resize (count - 1);
        skip++;
        continue;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      ប្រសិនបើ (unlikely (obj.in_error ())) {
        ត្រឡប់ Err(ALLOCATION_FAILURE);
      }
      vertex_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (count, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      unsigned obj_idx = i - skip;
      g.ordering_[--order] = obj_idx;

      ប្រសិនបើ (!removed_nil) continue;
      // Fix indices to account for removed nil object.
      សម្រាប់ (auto& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    auto r = g.is_fully_connected();
    ប្រសិនបើ (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    ត្រឡប់ Ok (std::move (g));
  }

  graph_t (const graph_t&) = delete;
  graph_t& operator = (const graph_t&) = delete;
  graph_t (graph_t&&) = default;
  graph_t& operator = (graph_t&&) = default;

  ~graph_t ()
  {
    សម្រាប់ (var_na* b : buffers)
      hb_free (b);
  }

private:
  graph_t () = default;

public:

  bool operator== (const graph_t& other) const
  {
    ត្រឡប់ root ().equals (root_idx(), other.root_idx(), other.root (), *this, other, 0);
  }

  void print () const {
    សម្រាប់ (unsigned id : ordering_)
    {
      const auto& v = vertices_[id];
      printf("%u: %u [", id, (unsigned ចំនួនគត់)v.table_size());
      សម្រាប់ (const auto &l : v.obj().real_links) {
        printf("%u, ", l.objidx);
      }
      សម្រាប់ (const auto &l : v.obj().virtual_links) {
        printf("v%u, ", l.objidx);
      }
      printf("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  void normalize ()
  {
    សម្រាប់ (auto& v : vertices_.writer ())
      v.normalize ();
  }

  const vertex_t& root () const
  {
    ត្រឡប់ vertices_[root_idx ()];
  }

  unsigned root_idx () const
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    ត្រឡប់ ordering_[0];
  }

  const hb_serialize_context_t::object_t& object (unsigned i) const
  {
    ត្រឡប់ vertices_[i].obj();
  }

  graph_result_t<void> add_buffer (var_na* buffer)
  {
    buffers.push (buffer);
    ត្រឡប់ graph_result_t<void>::from (buffers, ALLOCATION_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  template<typename T>
  graph_result_t<void> add_link (T* offset,
                                 unsigned parent_id,
                                 unsigned child_id)
  {
    ប្រសិនបើ (unlikely (parent_id >= vertices_.length || child_id >= vertices_.length))
      ត្រឡប់ Err(OUT_OF_BOUNDS);
    auto& v = vertices_[parent_id];
    TRY(v.add_real_link(2, child_id, (var_na*) offset - (var_na*) v.obj().head));
    ត្រឡប់ vertices_[child_id].add_parent (parent_id, false);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  graph_result_t<void> sort_shortest_distance_if_needed ()
  {
    ប្រសិនបើ (!positions_invalid) ត្រឡប់ Ok();
    ត្រឡប់ sort_shortest_distance ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  graph_result_t<void> sort_shortest_distance ()
  {
    positions_invalid = true;

    ប្រសិនបើ (vertices_.length <= 1) {
      // Graph of 1 or less doesn't need sorting.
      ត្រឡប់ Ok();
    }

    TRY (update_distances ());

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (vertices_.length);
    TRY (graph_result_t<void>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.length);
    TRY (graph_result_t<void>::from (new_ordering, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> removed_edges;
    removed_edges.resize (vertices_.length);
    TRY (graph_result_t<void>::from (removed_edges, ALLOCATION_FAILURE));

    TRY (update_parents ());

    queue.insert (root ().modified_distance (0), root_idx ());
    unsigned order = 1;
    unsigned pos = 0;
    ខណៈពេល (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_id = queue.pop_minimum().second;

      ប្រសិនបើ (unlikely (pos >= new_ordering.length)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_MSG (SUBSET_REPACK, nullptr, "Invalid graph. Contains cycle.");
        ត្រឡប់ Err(CYCLE_DETECTED);
      }
      new_ordering[pos++] = next_id;
      const vertex_t& next = vertices_[next_id];

      សម្រាប់ (const auto& link : next.obj().all_links ()) {
        ប្រសិនបើ (unlikely (link.objidx >= vertices_.length))
          ត្រឡប់ Err(OUT_OF_BOUNDS);
        removed_edges[link.objidx]++;
        const auto& v = vertices_[link.objidx];
        ប្រសិនបើ (!(v.incoming_edges () - removed_edges[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          queue.insert (v.modified_distance (order++),
                        link.objidx);
      }
    }

    TRY (graph_result_t<void>::from (queue, ALLOCATION_FAILURE));
    TRY (graph_result_t<void>::from (new_ordering, ALLOCATION_FAILURE));

    hb_swap (ordering_, new_ordering);

    ប្រសិនបើ (unlikely (pos != vertices_.length)) {
      print_orphaned_nodes ();
      ត្រឡប់ Err(ORPHANED_NODES);
    }

    ត្រឡប់ Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  graph_result_t<void> find_space_roots (hb_set_t& visited, hb_set_t& roots)
  {
    unsigned root_index = root_idx ();
    សម្រាប់ (unsigned i : ordering_)
    {
      ប្រសិនបើ (visited.has (i)) continue;

      // Only real links can form 32 bit spaces
      សម្រាប់ (const auto& l : vertices_[i].obj().real_links)
      {
        ប្រសិនបើ (l.is_signed || l.width < 3)
          continue;

        ប្រសិនបើ (i == root_index && l.width == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          continue;

        ប្រសិនបើ (l.width >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_set_t sub_roots;
          TRY (find_32bit_roots (l.objidx, sub_roots));
          ប្រសិនបើ (sub_roots) {
            សម្រាប់ (unsigned sub_root_idx : sub_roots) {
              roots.add (sub_root_idx);
              TRY (find_subgraph (sub_root_idx, visited));
            }
            continue;
          }
        }

        roots.add (l.objidx);
        TRY (find_subgraph (l.objidx, visited));
      }
    }

    TRY(graph_result_t<void>::from (visited, ALLOCATION_FAILURE));
    TRY(graph_result_t<void>::from (roots, ALLOCATION_FAILURE));
    ត្រឡប់ Ok();
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Immutable>> as_table (unsigned parent, const void* offset, Ts... ds)
  {
    TRY_ASSIGN (unsigned idx, index_for_offset (parent, offset));
    ត្រឡប់ as_table_from_index<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Mutable>> as_mutable_table (unsigned parent, const void* offset, Ts... ds)
  {
    TRY_ASSIGN (auto idx, mutable_index_for_offset (parent, offset));
    ត្រឡប់ as_table_from_index<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, vertex_mutability_t mutability = Mutable, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, mutability>> as_table_from_index (unsigned index, Ts... ds)
  {
    ប្រសិនបើ (unlikely (index >= vertices_.length))
      ត្រឡប់ Err(OUT_OF_BOUNDS);

    vertex_and_table_t<T, mutability> r;
    r.vertex = (typename std::conditional<mutability == Immutable, const vertex_t*, vertex_t*>::type) &vertices_[index];
    r.table = r.vertex->template as<typename std::conditional<mutability == Immutable, const T*, T*>::type> ();
    r.index = index;
    ប្រសិនបើ (unlikely (!r.table))
      ត្រឡប់ Err(INVALID_ARGUMENT);

    TRY(r.table->sanitize (*(r.vertex), std::forward<Ts>(ds)...));
    ត្រឡប់ r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  graph_result_t<unsigned> index_for_offset (unsigned node_idx, const void* offset) const
  {
    ប្រសិនបើ (unlikely (node_idx >= vertices_.length)) ត្រឡប់ Err(OUT_OF_BOUNDS);
    const auto& node = object (node_idx);
    ប្រសិនបើ (unlikely (offset < node.head || offset >= node.tail)) ត្រឡប់ Err(OUT_OF_BOUNDS);

    unsigned count = node.real_links.length;
    សម្រាប់ (unsigned i = 0; i < count; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      const auto& link = node.real_links.arrayZ[i];
      ប្រសិនបើ (offset != node.head + link.position)
        continue;
      ត្រឡប់ Ok(link.objidx);
    }

    ត្រឡប់ Err(INVALID_ARGUMENT);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  graph_result_t<unsigned> mutable_index_for_offset (unsigned node_idx, const void* offset)
  {
    TRY_ASSIGN (unsigned child_idx, index_for_offset (node_idx, offset));
    ប្រសិនបើ (unlikely (child_idx >= vertices_.length))
      ត្រឡប់ Err(OUT_OF_BOUNDS);

    auto& child = vertices_[child_idx];
    សម្រាប់ (unsigned p : child.parents_iter ())
    {
      ប្រសិនបើ (p != node_idx) {
        ត្រឡប់ duplicate (node_idx, child_idx, true);
      }
    }

    ត្រឡប់ Ok(child_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  graph_result_t<bool> assign_spaces ()
  {
    TRY (update_parents ());

    សម្រាប់ (auto& vertex : vertices_.writer ())
    {
      vertex.space = 0;
      vertex.priority = 0;
    }
    num_roots_for_space_.resize (1);
    TRY (graph_result_t<void>::from (num_roots_for_space_, ALLOCATION_FAILURE));
    num_roots_for_space_[0] = 1;
    distance_invalid = true;
    positions_invalid = true;

    hb_set_t visited;
    hb_set_t roots;
    TRY (find_space_roots (visited, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    visited.invert ();

    ប្រសិនបើ (!roots) ត្រឡប់ Ok(false);

    ខណៈពេល (roots)
    {
      uint32_t next = HB_SET_VALUE_INVALID;
      TRY (graph_result_t<void>::from (roots, ALLOCATION_FAILURE));
      ប្រសិនបើ (!roots.next (&next)) bha_nga;

      hb_set_t connected_roots;
      TRY (find_connected_nodes (next, roots, visited, connected_roots));
      TRY (graph_result_t<void>::from (connected_roots, ALLOCATION_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (graph_result_t<void>::from (connected_roots, ALLOCATION_FAILURE));

      unsigned next_space = this->next_space ();
      ប្រសិនបើ (unlikely (next_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_MSG (SUBSET_REPACK, nullptr,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 next_space);
        ត្រឡប់ Err(LIMIT_EXCEEDED);
      }
      num_roots_for_space_.push (0);
      TRY(graph_result_t<void>::from (num_roots_for_space_, ALLOCATION_FAILURE));

      សម្រាប់ (unsigned root : connected_roots)
      {
        DEBUG_MSG (SUBSET_REPACK, nullptr, "Subgraph %u gets space %u", root, next_space);
        vertices_[root].space = next_space;
        num_roots_for_space_[next_space] = num_roots_for_space_[next_space] + 1;
        distance_invalid = true;
        positions_invalid = true;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    ត្រឡប់ Ok(true);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  graph_result_t<bool> isolate_subgraph (hb_set_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_set_t parents;
    សម្រាប់ (unsigned root_idx : roots)
    {
      ប្រសិនបើ (unlikely (root_idx >= vertices_.length))
        ត្រឡប់ Err(OUT_OF_BOUNDS);
      subgraph.set (root_idx, wide_parents (root_idx, parents));
    }
    សម្រាប់ (unsigned root_idx : roots)
    {
      TRY (find_subgraph (root_idx, subgraph));
    }
    TRY (graph_result_t<void>::from (subgraph, ALLOCATION_FAILURE));

    hb_map_t index_map;
    bool made_changes = false;
    សម្រាប់ (auto entry : subgraph.iter ())
    {
      assert (entry.first < vertices_.length);
      const auto& node = vertices_[entry.first];
      unsigned subgraph_incoming_edges = entry.second;

      ប្រសិនបើ (subgraph_incoming_edges < node.incoming_edges ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = true;
        TRY (duplicate_subgraph (entry.first, index_map));
      }
    }

    TRY (graph_result_t<void>::from (index_map, ALLOCATION_FAILURE));

    ប្រសិនបើ (!made_changes)
      ត្រឡប់ Ok(false);

    auto new_subgraph =
        + subgraph.keys ()
        | hb_map([&] (uint32_t node_idx) {
          const uint32_t *v;
          ប្រសិនបើ (index_map.has (node_idx, &v)) ត្រឡប់ *v;
          ត្រឡប់ node_idx;
        })
        ;

    TRY(remap_obj_indices (index_map, new_subgraph));
    TRY(remap_obj_indices (index_map, parents.iter (), true));

    // Update roots set with new indices as needed.
    សម្រាប់ (auto next : roots)
    {
      const uint32_t *v;
      ប្រសិនបើ (index_map.has (next, &v))
      {
        roots.del (next);
        roots.add (*v);
      }
    }

    ត្រឡប់ Ok(true);
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
  graph_result_t<void> traverse_directed_bfs (unsigned start_idx, VisitEdgeFunc&& visit_edge)
  {
    ប្រសិនបើ (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_MSG (SUBSET_REPACK, nullptr,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      ត្រឡប់ Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<void>::from (ordering_scratch_, ALLOCATION_FAILURE));

    ប្រសិនបើ (!visit_edge (HB_CODEPOINT_INVALID, nullptr, start_idx, 0))
      ត្រឡប់ Ok();

    unsigned head = 0;
    unsigned tail = 0;
    auto& queue = ordering_scratch_;
    queue[tail++] = start_idx;

    unsigned depth = 0;
    ខណៈពេល (head < tail)
    {
      unsigned level_end = tail;
      ខណៈពេល (head < level_end)
      {
        unsigned node_idx = queue[head++];
        const auto& v = vertices_[node_idx];

        unsigned num_real = v.obj().real_links.length;
        unsigned total_links = num_real + v.obj().virtual_links.length;

        សម្រាប់ (unsigned i = 0; i < total_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          const auto& v = vertices_[node_idx];
          const auto& link = (i < num_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - num_real];
          unsigned child_idx = link.objidx;

          ប្រសិនបើ (!visit_edge (node_idx, &link, child_idx, depth + 1))
            continue;

          ប្រសិនបើ (unlikely (tail >= queue.length))
            ត្រឡប់ Err(CYCLE_DETECTED);

          queue[tail++] = child_idx;
        }
      }
      depth++;
    }
    ត្រឡប់ Ok();
  }

  graph_result_t<void> find_subgraph (unsigned node_idx, hb_map_t& subgraph)
  {
    ត្រឡប់ traverse_directed_bfs (node_idx, [&] (
      unsigned parent,
      const hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      ប្រសិនបើ (depth == 0) ត្រឡប់ true;
      hb_codepoint_t *count;
      ប្រសិនបើ (subgraph.has (child, &count))
      {
        (*count)++;
        ត្រឡប់ false;
      }
      subgraph.set (child, 1);
      ត្រឡប់ true;
    });
  }

  graph_result_t<void> find_subgraph (unsigned node_idx, hb_set_t& subgraph)
  {
    ត្រឡប់ traverse_directed_bfs (node_idx, [&] (
      unsigned parent,
      const hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      ប្រសិនបើ (subgraph.has (child)) ត្រឡប់ false;
      subgraph.add (child);
      ត្រឡប់ true;
    });
  }

  graph_result_t<size_t> find_subgraph_size (unsigned node_idx, hb_set_t& subgraph, unsigned max_depth = HB_GRAPH_INVALID)
  {
    size_t size = 0;
    TRY (traverse_directed_bfs (node_idx, [&] (
      unsigned parent,
      const hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      ប្រសិនបើ (subgraph.has (child)) ត្រឡប់ false;
      subgraph.add (child);

      const auto& o = vertices_[child].obj();
      size += o.tail - o.head;
      ត្រឡប់ depth < max_depth;
    }));
    ត្រឡប់ Ok(size);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  graph_result_t<void> find_32bit_roots (unsigned node_idx, hb_set_t& found)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_set_t visited;
    ត្រឡប់ traverse_directed_bfs (node_idx, [&] (
      unsigned parent,
      const hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {

      ប្រសិនបើ (link && found.has(parent))
        // Don't traverse from something that's already marked as a root.
        ត្រឡប់ false;

      ប្រសិនបើ (link && !link->is_signed && link->width == 4)
      {
        found.add (link->objidx);
        visited.add (link->objidx);
        ត្រឡប់ false;
      }

      ប្រសិនបើ (visited.has (child)) ត្រឡប់ false;
      visited.add (child);
      ត្រឡប់ true;
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
                                       const O* old_offset,
                                       unsigned new_parent_idx,
                                       const O* new_offset)
  {
    distance_invalid = true;
    positions_invalid = true;

    ប្រសិនបើ (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      ត្រឡប់ Err(OUT_OF_BOUNDS);

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    TRY_ASSIGN (unsigned child_id, index_for_offset (old_parent_idx,
                                                     old_offset));
    ប្រសិនបើ (unlikely (child_id >= vertices_.length))
      ត្រឡប់ Err(OUT_OF_BOUNDS);

    TRY(new_v.add_real_link(O::static_size, child_id, (const var_na*) new_offset - (const var_na*) new_v.obj().head));

    auto& child = vertices_[child_id];
    TRY(child.add_parent (new_parent_idx, false));

    old_v.remove_real_link_unordered (child_id, old_offset);
    child.remove_parent (old_parent_idx);

    ត្រឡប់ Ok(child_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  template<typename O>
  graph_result_t<void> move_children (unsigned old_parent_idx,
                                      unsigned old_pos_start,
                                      unsigned old_pos_end,
                                      unsigned new_parent_idx,
                                      unsigned new_pos_start)
  {
    ប្រសិនបើ (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      ត្រឡប់ Err(OUT_OF_BOUNDS);

    distance_invalid = true;
    positions_invalid = true;

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    hb_vector_t<hb_serialize_context_t::object_t::link_t> old_links;
    សម្រាប់ (const auto& l : old_v.obj().real_links)
    {
      ប្រសិនបើ (l.position < old_pos_start || l.position >= old_pos_end)
      {
        old_links.push (l);
        continue;
      }

      unsigned array_pos = l.position - old_pos_start;

      unsigned child_id = l.objidx;
      ប្រសិនបើ (unlikely (child_id >= vertices_.length))
        ត្រឡប់ Err(OUT_OF_BOUNDS);

      TRY(new_v.add_real_link(O::static_size, child_id, new_pos_start + array_pos));

      auto& child = vertices_[child_id];
      TRY(child.add_parent (new_parent_idx, false));
      child.remove_parent (old_parent_idx);
    }

    TRY (graph_result_t<void>::from (old_links, ALLOCATION_FAILURE));
    ត្រឡប់ old_v.set_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  graph_result_t<void> duplicate_subgraph (unsigned node_idx, hb_map_t& index_map)
  {
    graph_result_t<void> result = Ok();
    TRY (traverse_directed_bfs (node_idx, [&] (
      unsigned parent,
      const hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {
      ប្រសិនបើ (result.is_err () || index_map.has (child)) ត្រឡប់ false;
      auto r = duplicate (child);
      ប្រសិនបើ (unlikely (r.is_err ()))
      {
        result = Err(r.error ());
        ត្រឡប់ false;
      }
      index_map.set (child, *r);
      ត្រឡប់ true;
    }));

    ត្រឡប់ graph_result_t<void>::from(index_map, ALLOCATION_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  graph_result_t<unsigned> duplicate (unsigned node_idx, bool copy_table = false)
  {
    ប្រសិនបើ (unlikely (node_idx >= vertices_.length))
      ត្រឡប់ Err(OUT_OF_BOUNDS);

    ប្រសិនបើ (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_MSG (SUBSET_REPACK, nullptr,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      ត្រឡប់ Err(LIMIT_EXCEEDED);
    }

    positions_invalid = true;
    distance_invalid = true;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<void>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<void>::from (ordering_, ALLOCATION_FAILURE));

    auto& child = vertices_[node_idx];

    unsigned table_size = child.obj().tail - child.obj().head;
    ប្រសិនបើ (copy_table && table_size)
    {
      var_na* buffer = (var_na*) hb_malloc (table_size);
      ប្រសិនបើ (unlikely (!buffer))
        ត្រឡប់ Err(ALLOCATION_FAILURE);
      auto res = add_buffer (buffer);
      ប្រសិនបើ (unlikely (res.is_err ()))
      {
        hb_free (buffer);
        ត្រឡប់ Err(res.error ());
      }
      hb_memcpy (buffer, child.obj().head, table_size);
      clone->set_buffer(buffer, buffer + table_size);
    }
    បើមិនដូច្នេះ
    {
      clone->set_buffer(child.obj().head, child.obj().tail);
    }
    clone->distance = child.distance;
    clone->space = child.space;
    clone->reset_parents ();

    សម្រាប់ (const auto& l : child.obj().real_links)
    {
      TRY(clone->add_real_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, false));
    }

    សម្រាប់ (const auto& l : child.obj().virtual_links)
    {
      TRY(clone->add_virtual_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, true));
    }

    ត្រឡប់ Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (unsigned parent_idx, unsigned child_idx, bool copy_table = false)
  {
    ប្រសិនបើ (unlikely (parent_idx >= vertices_.length || child_idx >= vertices_.length))
      ត្រឡប់ Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    const auto& child = vertices_[child_idx];
    unsigned links_to_child = child.incoming_edges_from_parent (parent_idx);

    ប្រសិនបើ (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_MSG (SUBSET_REPACK, nullptr, "  Not duplicating %u => %u",
                 parent_idx, child_idx);
      ត្រឡប់ Err(INVALID_ARGUMENT);
    }

    DEBUG_MSG (SUBSET_REPACK, nullptr, "  Duplicating %u => %u",
               parent_idx, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx, copy_table));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    ប្រសិនបើ (parent_idx == clone_idx) parent_idx++;

    auto& parent = vertices_[parent_idx];
    unsigned count = 0;
    unsigned num_real = parent.obj().real_links.length;
    សម្រាប់ (auto& l : parent.all_links_writer ())
    {
      count++;
      ប្រសិនបើ (l.objidx != child_idx)
        continue;

      TRY(reassign_link (l, parent_idx, clone_idx, count > num_real));
    }

    ត្រឡប់ Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (const hb_set_t* parents, unsigned child_idx)
  {
    ប្រសិនបើ (unlikely (!parents || parents->is_empty ()))
      ត្រឡប់ Err(INVALID_ARGUMENT);

    ប្រសិនបើ (unlikely (child_idx >= vertices_.length))
      ត្រឡប់ Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    const auto& child = vertices_[child_idx];
    unsigned links_to_child = 0;
    unsigned last_parent = parents->get_max ();
    unsigned first_parent = parents->get_min ();
    សម្រាប់ (unsigned parent_idx : *parents) {
      ប្រសិនបើ (unlikely (parent_idx >= vertices_.length))
        ត្រឡប់ Err(OUT_OF_BOUNDS);
      links_to_child += child.incoming_edges_from_parent (parent_idx);
    }

    ប្រសិនបើ (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_MSG (SUBSET_REPACK, nullptr, "  Not duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);
      ត្រឡប់ Err(INVALID_ARGUMENT);
    }

    DEBUG_MSG (SUBSET_REPACK, nullptr, "  Duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx));

    សម្រាប់ (unsigned parent_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      ប្រសិនបើ (parent_idx == clone_idx) parent_idx++;
      auto& parent = vertices_[parent_idx];
      unsigned count = 0;
      unsigned num_real = parent.obj().real_links.length;
      សម្រាប់ (auto& l : parent.all_links_writer ())
      {
        count++;
        ប្រសិនបើ (l.objidx != child_idx)
          continue;

        TRY(reassign_link (l, parent_idx, clone_idx, count > num_real));
      }
    }

    ត្រឡប់ Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  graph_result_t<unsigned> new_node (var_na* head, var_na* tail)
  {
    ប្រសិនបើ (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_MSG (SUBSET_REPACK, nullptr,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      ត្រឡប់ Err(LIMIT_EXCEEDED);
    }

    positions_invalid = true;
    distance_invalid = true;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<void>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<void>::from (ordering_, ALLOCATION_FAILURE));

    clone->set_buffer (head, tail);
    clone->distance = 0;
    clone->space = 0;

    ត្រឡប់ Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  graph_result_t<unsigned> remap_child (unsigned parent_idx, unsigned old_child_idx)
  {
    ប្រសិនបើ (unlikely (parent_idx >= vertices_.length || old_child_idx >= vertices_.length))
      ត្រឡប់ Err(OUT_OF_BOUNDS);

    TRY_ASSIGN (unsigned new_child_idx, duplicate (old_child_idx));

    auto& parent = vertices_[parent_idx];
    សម្រាប់ (auto& l : parent.real_links_writer ())
    {
      ប្រសិនបើ (l.objidx != old_child_idx)
        continue;
      TRY(reassign_link (l, parent_idx, new_child_idx, false));
    }

    សម្រាប់ (auto& l : parent.virtual_links_writer ())
    {
      ប្រសិនបើ (l.objidx != old_child_idx)
        continue;
      TRY(reassign_link (l, parent_idx, new_child_idx, true));
    }
    ត្រឡប់ Ok(new_child_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  graph_result_t<bool> raise_childrens_priority (unsigned parent_idx)
  {
    DEBUG_MSG (SUBSET_REPACK, nullptr, "  Raising priority of all children of %u",
               parent_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    ប្រសិនបើ (unlikely (parent_idx >= vertices_.length)) ត្រឡប់ Err(OUT_OF_BOUNDS);
    bool made_change = false;
    សម្រាប់ (auto& l : vertices_[parent_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_priority ();
    ត្រឡប់ Ok(made_change);
  }

  graph_result_t<void> is_fully_connected ()
  {
    TRY(update_parents());

    ប្រសិនបើ (unlikely (root().incoming_edges ()))
      // Root cannot have parents.
      ត្រឡប់ Err(ORPHANED_NODES);

    សម្រាប់ (unsigned i = 0; i < root_idx (); i++)
    {
      ប្រសិនបើ (unlikely (!vertices_[i].incoming_edges ()))
        ត្រឡប់ Err(ORPHANED_NODES);
    }
    ត្រឡប់ Ok();
  }

#ប្រសិនបើ 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  void save_fuzzer_seed (hb_tag_t tag) const
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((void*) &tag, parimaa_na (tag), 1, f);

    uint16_t num_objects = vertices_.length;
    fwrite ((void*) &num_objects, parimaa_na (num_objects), 1, f);

    សម្រាប់ (const auto& v : vertices_)
    {
      uint16_t blob_size = v.table_size ();
      fwrite ((void*) &blob_size, parimaa_na (blob_size), 1, f);
      fwrite ((const void*) v.obj.head, blob_size, 1, f);
    }

    uint16_t link_count = 0;
    សម្រាប់ (const auto& v : vertices_)
      link_count += v.obj.real_links.length;

    fwrite ((void*) &link_count, parimaa_na (link_count), 1, f);

    typedef sa_nracanaa
    {
      uint16_t parent;
      uint16_t child;
      uint16_t position;
      uint8_t width;
    } link_t;

    សម្រាប់ (unsigned i = 0; i < vertices_.length; i++)
    {
      សម្រាប់ (const auto& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.width
        };
        fwrite ((void*) &link, parimaa_na (link), 1, f);
      }
    }

    fclose (f);
  }
#endif

  unsigned num_roots_for_space (unsigned space) const
  {
    ត្រឡប់ num_roots_for_space_[space];
  }

  unsigned next_space () const
  {
    ត្រឡប់ num_roots_for_space_.length;
  }

  graph_result_t<void> move_to_new_space (const hb_set_t& indices)
  {
    ប្រសិនបើ (unlikely (num_roots_for_space_.length >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_MSG (SUBSET_REPACK, nullptr,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 num_roots_for_space_.length);
      ត្រឡប់ Err(LIMIT_EXCEEDED);
    }
    num_roots_for_space_.push (0);
    TRY (graph_result_t<void>::from (num_roots_for_space_, ALLOCATION_FAILURE));
    unsigned new_space = num_roots_for_space_.length - 1;

    សម្រាប់ (unsigned index : indices) {
      ប្រសិនបើ (unlikely (index >= vertices_.length))
        ត្រឡប់ Err(OUT_OF_BOUNDS);
      auto& node = vertices_[index];
      num_roots_for_space_[node.space] = num_roots_for_space_[node.space] - 1;
      num_roots_for_space_[new_space] = num_roots_for_space_[new_space] + 1;
      node.space = new_space;
      distance_invalid = true;
      positions_invalid = true;
    }
    ត្រឡប់ Ok();
  }

  unsigned space_for (unsigned index, unsigned* root = nullptr) const
  {
    ខណៈពេល (true)
    {
      assert (index < vertices_.length);
      const auto& node = vertices_[index];
      ប្រសិនបើ (node.space)
      {
        ប្រសិនបើ (root != nullptr)
          *root = index;
        ត្រឡប់ node.space;
      }

      ប្រសិនបើ (!node.incoming_edges ())
      {
        ប្រសិនបើ (root)
          *root = index;
        ត្រឡប់ 0;
      }

      index = *node.parents_iter ();
    }
  }

  size_t total_size_in_bytes () const {
    size_t total_size = 0;
    unsigned count = vertices_.length;
    សម្រាប់ (unsigned i = 0; i < count; i++) {
      size_t size = vertices_.arrayZ[i].table_size ();
      total_size += size;
    }
    ត្រឡប់ total_size;
  }


 private:

  void print_orphaned_nodes ()
  {
    ប្រសិនបើ (!DEBUG_ENABLED(SUBSET_REPACK)) ត្រឡប់;

    DEBUG_MSG (SUBSET_REPACK, nullptr, "Graph is not fully connected.");

    parents_invalid = true;
    (void) update_parents();

    ប្រសិនបើ (root().incoming_edges ()) {
      DEBUG_MSG (SUBSET_REPACK, nullptr, "Root node has incoming edges.");
    }

    សម្រាប់ (unsigned i = 0; i < vertices_.length; i++)
    {
      const auto& v = vertices_[i];
      ប្រសិនបើ (!v.incoming_edges ())
        DEBUG_MSG (SUBSET_REPACK, nullptr, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  unsigned wide_parents (unsigned node_idx, hb_set_t& parents) const
  {
    unsigned count = 0;
    សម្រាប់ (unsigned p : vertices_[node_idx].parents_iter ())
    {
      // Only real links can be wide
      សម្រាប់ (const auto& l : vertices_[p].obj().real_links)
      {
        ប្រសិនបើ (l.objidx == node_idx
            && (l.width == 3 || l.width == 4)
            && !l.is_signed)
        {
          count++;
          parents.add (p);
        }
      }
    }
    ត្រឡប់ count;
  }

 public:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  graph_result_t<void> update_parents ()
  {
    ប្រសិនបើ (!parents_invalid) ត្រឡប់ Ok();

    unsigned count = vertices_.length;

    សម្រាប់ (unsigned i = 0; i < count; i++)
      vertices_.arrayZ[i].reset_parents ();

    សម្រាប់ (unsigned p = 0; p < count; p++)
    {
      សម្រាប់ (const auto& l : vertices_.arrayZ[p].obj().real_links)
      {
        ប្រសិនបើ (unlikely (l.objidx >= count)) ត្រឡប់ Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, false));
      }

      សម្រាប់ (const auto& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        ប្រសិនបើ (unlikely (l.objidx >= count)) ត្រឡប់ Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, true));
      }
    }

    parents_invalid = false;
    ត្រឡប់ Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  void update_positions ()
  {
    ប្រសិនបើ (!positions_invalid) ត្រឡប់;

    unsigned current_pos = 0;
    សម្រាប់ (unsigned i : ordering_)
    {
      auto& v = vertices_[i];
      v.start = current_pos;
      current_pos += v.obj().tail - v.obj().head;
      v.end = current_pos;
    }

    positions_invalid = false;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  graph_result_t<void> update_distances ()
  {
    ប្រសិនបើ (!distance_invalid) ត្រឡប់ Ok();

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
    unsigned count = vertices_.length;
    សម្រាប់ (unsigned i = 0; i < count; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[root_idx ()].distance = 0;

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (count);
    queue.insert (0, root_idx ());
    TRY (graph_result_t<void>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<bool> visited;
    visited.resize (vertices_.length);
    TRY (graph_result_t<void>::from (visited, ALLOCATION_FAILURE));

    ខណៈពេល (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_idx = queue.pop_minimum ().second;
      ប្រសិនបើ (visited[next_idx]) continue;
      const auto& next = vertices_[next_idx];
      int64_t next_distance = next.distance;
      visited[next_idx] = true;

      សម្រាប់ (const auto& link : next.obj().all_links ())
      {
        ប្រសិនបើ (unlikely (link.objidx >= vertices_.length))
          ត្រឡប់ Err(OUT_OF_BOUNDS);

        ប្រសិនបើ (visited[link.objidx]) continue;

        auto& child_v = vertices_.arrayZ[link.objidx];
        const auto& child = child_v.obj ();
        unsigned link_width = link.width ? link.width : 4; // treat virtual offsets as 32 bits wide
        int64_t child_weight = (child.tail - child.head) +
                               ((int64_t) 1 << (link_width * 8)) * (child_v.space + 1);
        int64_t child_distance = next_distance + child_weight;

        ប្រសិនបើ (child_distance < child_v.distance)
        {
          child_v.distance = child_distance;
          queue.insert (child_distance, link.objidx);
        }
      }
    }

    TRY (graph_result_t<void>::from (queue, ALLOCATION_FAILURE));
    ប្រសិនបើ (unlikely (!queue.is_empty ()))
    {
      print_orphaned_nodes ();
      ត្រឡប់ Err(ORPHANED_NODES);
    }

    distance_invalid = false;
    ត្រឡប់ Ok();
  }

 private:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  graph_result_t<void> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      unsigned parent_idx,
                      unsigned new_idx,
                      bool is_virtual)
  {
    unsigned old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_parent (parent_idx);
    ត្រឡប់ vertices_[new_idx].add_parent (parent_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  template<typename Iterator, hb_requires (hb_is_iterator (Iterator))>
  graph_result_t<void> remap_obj_indices (const hb_map_t& id_map,
                                          Iterator subgraph,
                                          bool only_wide = false)
  {
    ប្រសិនបើ (!id_map) ត្រឡប់ Ok();
    សម្រាប់ (unsigned i : subgraph)
    {
      auto& vertex = vertices_[i];
      unsigned num_real = vertex.obj().real_links.length;
      unsigned count = 0;
      សម្រាប់ (auto& link : vertex.all_links_writer ())
      {
        count++;
        const uint32_t *v;
        ប្រសិនបើ (!id_map.has (link.objidx, &v)) continue;
        ប្រសិនបើ (only_wide && (link.is_signed || (link.width != 4 && link.width != 3))) continue;

        TRY(reassign_link (link, i, *v, count > num_real));
      }
    }
    ត្រឡប់ Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  graph_result_t<void> find_connected_nodes (unsigned start_idx,
                                             hb_set_t& targets,
                                             hb_set_t& visited,
                                             hb_set_t& connected)
  {
    TRY (graph_result_t<void>::from (visited, ALLOCATION_FAILURE));
    ប្រសិនបើ (visited.has (start_idx)) ត្រឡប់ Ok();
    ប្រសិនបើ (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_MSG (SUBSET_REPACK, nullptr,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      ត្រឡប់ Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<void>::from (ordering_scratch_, ALLOCATION_FAILURE));

    auto& stack = ordering_scratch_;
    unsigned stack_len = 0;

    graph_result_t<void> res = Ok();
    auto handle_node = [&] (unsigned node_idx) {
      visited.add (node_idx);
      ប្រសិនបើ (targets.has (node_idx)) {
        targets.del (node_idx);
        connected.add (node_idx);
      }

      ប្រសិនបើ (unlikely (stack_len >= stack.length))
      {
        res = Err(CYCLE_DETECTED);
        ត្រឡប់ false;
      }
      stack[stack_len++] = node_idx;
      ត្រឡប់ true;
    };

    ប្រសិនបើ (!handle_node (start_idx)) ត្រឡប់ res;

    ខណៈពេល (stack_len > 0)
    {
      unsigned node_idx = stack[--stack_len];
      const auto& v = vertices_[node_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      សម្រាប់ (const auto& l : v.obj().all_links ())
      {
        unsigned child_idx = l.objidx;
        ប្រសិនបើ (visited.has (child_idx)) continue;
        ប្រសិនបើ (!handle_node (child_idx)) ត្រឡប់ res;
      }

      សម្រាប់ (unsigned parent_idx : v.parents_iter ())
      {
        ប្រសិនបើ (visited.has (parent_idx)) continue;
        ប្រសិនបើ (!handle_node (parent_idx)) ត្រឡប់ res;
      }
    }

    TRY (graph_result_t<void>::from (visited, ALLOCATION_FAILURE));
    TRY (graph_result_t<void>::from (connected, ALLOCATION_FAILURE));

    ត្រឡប់ Ok();
  }

 public:
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

 private:
  bool parents_invalid = true;
  bool distance_invalid = true;
  bool positions_invalid = true;
  hb_vector_t<unsigned> num_roots_for_space_;
  hb_vector_t<var_na*> buffers;
};

}

#endif  // GRAPH_GRAPH_HH
