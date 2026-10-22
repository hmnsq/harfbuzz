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

#અન્તર્ભાવ "../hb-set.hh"
#અન્તર્ભાવ "../hb-priority-queue.hh"
#અન્તર્ભાવ "../hb-serialize.hh"
#અન્તર્ભાવ "graph-result.hh"

#ifndef GRAPH_GRAPH_HH
#define GRAPH_GRAPH_HH

#define HB_GRAPH_INVALID ((unsigned) -1)

નામાકાશ graph {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
સંરચના graph_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  સંરચના vertex_t
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

    ખાનગી:
    hb_serialize_context_t::object_t obj_;
    unsigned incoming_edges_ = 0;
    unsigned single_parent = HB_GRAPH_INVALID;
    તાર્કિક has_incoming_virtual_edges_ = ખોટું;
    hb_hashmap_t<unsigned, unsigned> parents;
    hb_set_t virtual_parents;
    જાહેર:

    vertex_t ()
    {}

    explicit vertex_t(અચલ hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    auto parents_iter () અચલ HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_parent, single_parent != HB_GRAPH_INVALID),
	parents.ચાવીઓ_ref ()
      )
    )

    શૂન્ય set_buffer (અક્ષર* head, અક્ષર* tail) {
      obj_.head = head;
      obj_.tail = tail;
    }

    શૂન્ય shrink_buffer (size_t ગણતરી)
    {
      obj_.tail -= ગણતરી;
    }

    auto all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    auto real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    auto virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    શૂન્ય sort_real_links () {
      obj_.real_links.qsort ();
    }

    graph_result_t<શૂન્ય> add_real_link (unsigned width, unsigned obj_idx, unsigned position) {
      auto* link = obj_.real_links.push ();
      TRY (graph_result_t<શૂન્ય>::from (obj_.real_links, ALLOCATION_FAILURE));
      link->width = width;
      link->objidx = obj_idx;
      link->position = position;
      પાછા Ok();
    }

    graph_result_t<શૂન્ય> add_virtual_link (unsigned obj_idx) {
      auto* link = obj_.virtual_links.push ();
      TRY (graph_result_t<શૂન્ય>::from (obj_.virtual_links, ALLOCATION_FAILURE));
      link->objidx = obj_idx;
      પાછા Ok();
    }

    graph_result_t<શૂન્ય> add_real_link (અચલ hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      પાછા graph_result_t<શૂન્ય>::from (obj_.real_links, ALLOCATION_FAILURE);
    }

    graph_result_t<શૂન્ય> set_real_links (hb_vector_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(graph_result_t<શૂન્ય>::from (links, ALLOCATION_FAILURE));
      obj_.real_links = links;
      પાછા Ok();
    }

    શૂન્ય clear_real_links ()
    {
      obj_.real_links.reset();
    }

    શૂન્ય clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    graph_result_t<શૂન્ય> add_virtual_link (અચલ hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      પાછા graph_result_t<શૂન્ય>::from (obj_.virtual_links, ALLOCATION_FAILURE);
    }

    અચલ hb_serialize_context_t::object_t& obj() અચલ { પાછા obj_; }

    template<typename T>
    T as () અચલ
    {
      પાછા reinterpret_cast<T> (obj_.head);
    }

    તાર્કિક has_incoming_virtual_edges () અચલ
    {
      પાછા has_incoming_virtual_edges_;
    }

    graph_result_t<શૂન્ય> link_positions_valid (unsigned અંક_objects, તાર્કિક removed_nil)
    {
      hb_set_t assigned_bytes;
      માટે (અચલ auto& l : obj_.real_links)
      {
        જો (unlikely (l.objidx >= અંક_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid object index.");
          પાછા Err(INVALID_ARGUMENT);
        }

        unsigned start = l.position;
        unsigned end = start + l.width - 1;

        જો (unlikely (l.width < 2 || l.width > 4))
        {
          DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid link width.");
          પાછા Err(INVALID_ARGUMENT);
        }

        જો (unlikely (end >= table_size ()))
        {
          DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                     "Invalid graph. Link position is out of bounds.");
          પાછા Err(OUT_OF_BOUNDS);
        }

        જો (unlikely (assigned_bytes.intersects (start, end)))
        {
          DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                     "Invalid graph. Found offsets whose positions overlap.");
          પાછા Err(INVALID_ARGUMENT);
        }

        assigned_bytes.add_range (start, end);
      }

      પાછા graph_result_t<શૂન્ય>::from (assigned_bytes, ALLOCATION_FAILURE);
    }

    શૂન્ય normalize ()
    {
      obj_.real_links.qsort ();
      માટે (અચલ auto& l : obj_.real_links)
      {
        માટે (unsigned i = 0; i < l.width; i++)
        {
          obj_.head[l.position + i] = 0;
        }
      }
    }

    તાર્કિક equals (unsigned this_index,
                 unsigned other_index,
                 અચલ vertex_t& other,
                 અચલ graph_t& graph,
                 અચલ graph_t& other_graph,
                 unsigned depth) અચલ
    {
      જો (!(as_bytes () == other.as_bytes ()))
      {
        DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_index,
                   (unsigned long) table_size (),
                   other_index,
                   (unsigned long) other.table_size (),
                   depth);

        auto a = as_bytes ();
        auto b = other.as_bytes ();
        જ્યારે (a || b)
        {
          DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                     "  0x%x %s 0x%x", (unsigned) *a, (*a == *b) ? "==" : "!=", (unsigned) *b);
          a++;
          b++;
        }
        પાછા ખોટું;
      }

      પાછા links_equal (obj_.real_links, other.obj_.real_links, graph, other_graph, depth);
    }

    hb_bytes_t as_bytes () અચલ
    {
      પાછા hb_bytes_t (obj_.head, table_size ());
    }

    friend શૂન્ય swap (vertex_t& a, vertex_t& b)
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
    position_to_index_map () અચલ
    {
      hb_hashmap_t<unsigned, unsigned> result;

      result.alloc (obj_.real_links.length);
      માટે (અચલ auto& l : obj_.real_links) {
        result.set (l.position, l.objidx);
      }

      પાછા graph_result_t<hb_hashmap_t<unsigned, unsigned>>::from (std::move(result), ALLOCATION_FAILURE);
    }

    તાર્કિક is_shared () અચલ
    {
      પાછા parents.get_population () > virtual_parents.get_population () + 1;
    }

    unsigned incoming_edges () અચલ
    {
      જો (HB_DEBUG_SUBSET_REPACK)
       {
	પ્રતિજ્ઞા (incoming_edges_ == (single_parent != HB_GRAPH_INVALID) +
		(parents.કિંમતો_ref () | hb_reduce (hb_add, 0)));
       }
      પાછા incoming_edges_;
    }

    unsigned incoming_edges_from_parent (unsigned parent_index) અચલ {
      જો (single_parent != HB_GRAPH_INVALID) {
        પાછા single_parent == parent_index ? 1 : 0;
      }

      unsigned* ગણતરી;
      પાછા  parents.has(parent_index, &ગણતરી) ? *ગણતરી : 0;
    }

    શૂન્ય reset_parents ()
    {
      incoming_edges_ = 0;
      has_incoming_virtual_edges_ = ખોટું;
      single_parent = HB_GRAPH_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    graph_result_t<શૂન્ય> add_parent (unsigned parent_index, તાર્કિક is_virtual)
    {
      પ્રતિજ્ઞા (parent_index != HB_GRAPH_INVALID);
      has_incoming_virtual_edges_ |= is_virtual;
      જો (is_virtual)
        virtual_parents.add (parent_index);

      જો (incoming_edges_ == 0)
      {
	single_parent = parent_index;
	incoming_edges_ = 1;
	પાછા to_result();
      }
      નહીંતર જો (single_parent != HB_GRAPH_INVALID)
      {
        પ્રતિજ્ઞા (incoming_edges_ == 1);
	જો (!parents.set (single_parent, 1))
	  પાછા to_result ();
	single_parent = HB_GRAPH_INVALID;
      }

      unsigned *v;
      જો (parents.has (parent_index, &v))
      {
        (*v)++;
	incoming_edges_++;
      }
      નહીંતર જો (parents.set (parent_index, 1))
	incoming_edges_++;

      પાછા to_result();
    }

    શૂન્ય remove_parent (unsigned parent_index)
    {
      જો (parent_index == single_parent)
      {
	single_parent = HB_GRAPH_INVALID;
	incoming_edges_--;
	virtual_parents.reset ();
	પાછા;
      }

      unsigned *v;
      જો (parents.has (parent_index, &v))
      {
	incoming_edges_--;
	જો (*v > 1)
	  (*v)--;
	નહીંતર
        {
	  parents.del (parent_index);
          virtual_parents.del (parent_index);
        }

	જો (incoming_edges_ == 1)
	{
	  single_parent = *parents.ચાવીઓ ();
	  parents.reset ();
	}
      }
    }

    શૂન્ય remove_real_link_unordered (unsigned link_array_index) {
      obj_.real_links.remove_unordered (link_array_index);
    }

    શૂન્ય remove_real_link_unordered (unsigned child_index, અચલ શૂન્ય* offset)
    {
      unsigned ગણતરી = obj_.real_links.length;
      માટે (unsigned i = 0; i < ગણતરી; i++)
      {
        auto& link = obj_.real_links.arrayZ[i];
        જો (link.objidx != child_index)
          ચાલુ;

        જો ((obj_.head + link.position) != offset)
          ચાલુ;

        obj_.real_links.remove_unordered (i);
        પાછા;
      }
    }

    graph_result_t<શૂન્ય> remap_parent (unsigned old_index, unsigned new_index)
    {
      જો (single_parent != HB_GRAPH_INVALID)
      {
        જો (single_parent == old_index)
        {
	  single_parent = new_index;
          જો (virtual_parents.has (old_index))
          {
            virtual_parents.del (old_index);
            virtual_parents.add (new_index);
          }
        }
        પાછા graph_result_t<શૂન્ય>::from(virtual_parents, ALLOCATION_FAILURE);
      }

      અચલ unsigned *pv;
      જો (parents.has (old_index, &pv))
      {
        unsigned v = *pv;
	જો (!parents.set (new_index, v))
          incoming_edges_ -= v;
	parents.del (old_index);

        જો (incoming_edges_ == 1)
	{
	  single_parent = *parents.ચાવીઓ ();
	  parents.reset ();
	}
      }

      જો (virtual_parents.has (old_index))
      {
        virtual_parents.del (old_index);
        virtual_parents.add (new_index);
      }

      પાછા graph_result_t<શૂન્ય>::from(virtual_parents, ALLOCATION_FAILURE);
    }

    તાર્કિક is_leaf () અચલ
    {
      પાછા !obj_.real_links.length && !obj_.virtual_links.length;
    }

    તાર્કિક raise_priority ()
    {
      જો (has_max_priority ()) પાછા ખોટું;
      priority++;
      પાછા સાચું;
    }

    તાર્કિક give_max_priority ()
    {
      તાર્કિક result = ખોટું;
      જ્યારે (!has_max_priority()) {
        result = સાચું;
        priority++;
      }
      પાછા result;
    }

    તાર્કિક has_max_priority () અચલ {
      પાછા priority >= 3;
    }

    size_t table_size () અચલ {
      પાછા obj_.tail - obj_.head;
    }

    int64_t modified_distance (unsigned order) અચલ
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_distance =
          hb_clamp (distance + distance_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      જો (has_max_priority ()) {
        modified_distance = 0;
      }
      પાછા (modified_distance << 18) | (0x003FFFF & order);
    }

    int64_t distance_modifier () અચલ
    {
      જો (!priority) પાછા 0;
      int64_t table_size = obj_.tail - obj_.head;

      જો (priority == 1)
        પાછા -table_size / 2;

      પાછા -table_size;
    }

   ખાનગી:
    graph_result_t<શૂન્ય> to_result() અચલ {
      જો (unlikely (
        parents.in_error() || virtual_parents.in_error() ||
        obj_.real_links.in_error() || obj_.virtual_links.in_error()))
        પાછા Err(ALLOCATION_FAILURE);
      પાછા Ok();
    }

    તાર્કિક links_equal (અચલ hb_vector_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      અચલ hb_vector_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      અચલ graph_t& graph,
                      અચલ graph_t& other_graph,
                      unsigned depth) અચલ
    {
      auto a = this_links.iter ();
      auto b = other_links.iter ();

      જ્યારે (a && b)
      {
        અચલ auto& link_a = *a;
        અચલ auto& link_b = *b;

        જો (link_a.width != link_b.width ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          પાછા ખોટું;

        જો (!graph.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_graph.vertices_[link_b.objidx], graph, other_graph, depth + 1))
          પાછા ખોટું;

        a++;
        b++;
      }

      જો (તાર્કિક (a) != તાર્કિક (b))
        પાછા ખોટું;

      પાછા સાચું;
    }
  };

  ગણના vertex_mutability_t
  {
    Immutable,
    Mutable
  };

  template <typename T, vertex_mutability_t mutability = Mutable>
  સંરચના vertex_and_table_t
  {
    vertex_and_table_t () : index (0), vertex (nullptr), table (nullptr)
    {}

    template <vertex_mutability_t other_mutability>
    vertex_and_table_t (અચલ vertex_and_table_t<T, other_mutability>& o)
      : index (o.index), vertex (o.vertex), table (o.table)
    {}

    unsigned index;
    typename std::conditional<mutability == Immutable, અચલ vertex_t*, vertex_t*>::type vertex;
    typename std::conditional<mutability == Immutable, અચલ T*, T*>::type table;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  template<typename T>
  સ્થિર graph_result_t<graph_t> create (અચલ T& objects)
  {
    જો (unlikely (objects.length > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.length);
      પાછા Err(LIMIT_EXCEEDED);
    }

    graph_t g;
    g.અંક_roots_for_space.push (1);
    TRY (graph_result_t<શૂન્ય>::from (g.અંક_roots_for_space, ALLOCATION_FAILURE));

    તાર્કિક removed_nil = ખોટું;
    અચલ unsigned ગણતરી = objects.length;
    g.vertices_.alloc (ગણતરી);
    g.ordering_.resize (ગણતરી);
    g.ordering_scratch_.alloc (ગણતરી);
    TRY (graph_result_t<શૂન્ય>::from (g.vertices_, ALLOCATION_FAILURE));
    TRY (graph_result_t<શૂન્ય>::from (g.ordering_, ALLOCATION_FAILURE));
    TRY (graph_result_t<શૂન્ય>::from (g.ordering_scratch_, ALLOCATION_FAILURE));

    unsigned order = objects.length;
    unsigned skip = 0;
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      જો (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = સાચું;
        order--;
        g.ordering_.resize (ગણતરી - 1);
        skip++;
        ચાલુ;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      જો (unlikely (obj.in_error ())) {
        પાછા Err(ALLOCATION_FAILURE);
      }
      vertex_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (ગણતરી, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      unsigned obj_idx = i - skip;
      g.ordering_[--order] = obj_idx;

      જો (!removed_nil) ચાલુ;
      // Fix indices to account for removed nil object.
      માટે (auto& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    auto r = g.is_fully_connected();
    જો (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    પાછા Ok (std::move (g));
  }

  graph_t (અચલ graph_t&) = delete;
  graph_t& operator = (અચલ graph_t&) = delete;
  graph_t (graph_t&&) = મૂળભૂત;
  graph_t& operator = (graph_t&&) = મૂળભૂત;

  ~graph_t ()
  {
    માટે (અક્ષર* b : buffers)
      hb_free (b);
  }

ખાનગી:
  graph_t () = મૂળભૂત;

જાહેર:

  તાર્કિક operator== (અચલ graph_t& other) અચલ
  {
    પાછા મૂળ ().equals (મૂળ_idx(), other.મૂળ_idx(), other.મૂળ (), *આ, other, 0);
  }

  શૂન્ય print () અચલ {
    માટે (unsigned id : ordering_)
    {
      અચલ auto& v = vertices_[id];
      printf("%u: %u [", id, (unsigned પૂર્ણાંક)v.table_size());
      માટે (અચલ auto &l : v.obj().real_links) {
        printf("%u, ", l.objidx);
      }
      માટે (અચલ auto &l : v.obj().virtual_links) {
        printf("v%u, ", l.objidx);
      }
      printf("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  શૂન્ય normalize ()
  {
    માટે (auto& v : vertices_.writer ())
      v.normalize ();
  }

  અચલ vertex_t& મૂળ () અચલ
  {
    પાછા vertices_[મૂળ_idx ()];
  }

  unsigned મૂળ_idx () અચલ
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    પાછા ordering_[0];
  }

  અચલ hb_serialize_context_t::object_t& object (unsigned i) અચલ
  {
    પાછા vertices_[i].obj();
  }

  graph_result_t<શૂન્ય> add_buffer (અક્ષર* buffer)
  {
    buffers.push (buffer);
    પાછા graph_result_t<શૂન્ય>::from (buffers, ALLOCATION_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  template<typename T>
  graph_result_t<શૂન્ય> add_link (T* offset,
                                 unsigned parent_id,
                                 unsigned child_id)
  {
    જો (unlikely (parent_id >= vertices_.length || child_id >= vertices_.length))
      પાછા Err(OUT_OF_BOUNDS);
    auto& v = vertices_[parent_id];
    TRY(v.add_real_link(2, child_id, (અક્ષર*) offset - (અક્ષર*) v.obj().head));
    પાછા vertices_[child_id].add_parent (parent_id, ખોટું);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  graph_result_t<શૂન્ય> sort_shortest_distance_if_needed ()
  {
    જો (!positions_invalid) પાછા Ok();
    પાછા sort_shortest_distance ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  graph_result_t<શૂન્ય> sort_shortest_distance ()
  {
    positions_invalid = સાચું;

    જો (vertices_.length <= 1) {
      // Graph of 1 or less doesn't need sorting.
      પાછા Ok();
    }

    TRY (update_distances ());

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (vertices_.length);
    TRY (graph_result_t<શૂન્ય>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.length);
    TRY (graph_result_t<શૂન્ય>::from (new_ordering, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> removed_edges;
    removed_edges.resize (vertices_.length);
    TRY (graph_result_t<શૂન્ય>::from (removed_edges, ALLOCATION_FAILURE));

    TRY (update_parents ());

    queue.insert (મૂળ ().modified_distance (0), મૂળ_idx ());
    unsigned order = 1;
    unsigned pos = 0;
    જ્યારે (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_id = queue.કાઢો_minimum().second;

      જો (unlikely (pos >= new_ordering.length)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "Invalid graph. Contains cycle.");
        પાછા Err(CYCLE_DETECTED);
      }
      new_ordering[pos++] = next_id;
      અચલ vertex_t& next = vertices_[next_id];

      માટે (અચલ auto& link : next.obj().all_links ()) {
        જો (unlikely (link.objidx >= vertices_.length))
          પાછા Err(OUT_OF_BOUNDS);
        removed_edges[link.objidx]++;
        અચલ auto& v = vertices_[link.objidx];
        જો (!(v.incoming_edges () - removed_edges[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          queue.insert (v.modified_distance (order++),
                        link.objidx);
      }
    }

    TRY (graph_result_t<શૂન્ય>::from (queue, ALLOCATION_FAILURE));
    TRY (graph_result_t<શૂન્ય>::from (new_ordering, ALLOCATION_FAILURE));

    hb_swap (ordering_, new_ordering);

    જો (unlikely (pos != vertices_.length)) {
      print_orphaned_nodes ();
      પાછા Err(ORPHANED_NODES);
    }

    પાછા Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  graph_result_t<શૂન્ય> find_space_roots (hb_set_t& visited, hb_set_t& roots)
  {
    unsigned મૂળ_index = મૂળ_idx ();
    માટે (unsigned i : ordering_)
    {
      જો (visited.has (i)) ચાલુ;

      // Only real links can form 32 bit spaces
      માટે (અચલ auto& l : vertices_[i].obj().real_links)
      {
        જો (l.is_signed || l.width < 3)
          ચાલુ;

        જો (i == મૂળ_index && l.width == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          ચાલુ;

        જો (l.width >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_set_t sub_roots;
          TRY (find_32bit_roots (l.objidx, sub_roots));
          જો (sub_roots) {
            માટે (unsigned sub_મૂળ_idx : sub_roots) {
              roots.add (sub_મૂળ_idx);
              TRY (find_subgraph (sub_મૂળ_idx, visited));
            }
            ચાલુ;
          }
        }

        roots.add (l.objidx);
        TRY (find_subgraph (l.objidx, visited));
      }
    }

    TRY(graph_result_t<શૂન્ય>::from (visited, ALLOCATION_FAILURE));
    TRY(graph_result_t<શૂન્ય>::from (roots, ALLOCATION_FAILURE));
    પાછા Ok();
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Immutable>> as_table (unsigned parent, અચલ શૂન્ય* offset, Ts... ds)
  {
    TRY_ASSIGN (unsigned idx, index_for_offset (parent, offset));
    પાછા as_table_from_index<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Mutable>> as_mutable_table (unsigned parent, અચલ શૂન્ય* offset, Ts... ds)
  {
    TRY_ASSIGN (auto idx, mutable_index_for_offset (parent, offset));
    પાછા as_table_from_index<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, vertex_mutability_t mutability = Mutable, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, mutability>> as_table_from_index (unsigned index, Ts... ds)
  {
    જો (unlikely (index >= vertices_.length))
      પાછા Err(OUT_OF_BOUNDS);

    vertex_and_table_t<T, mutability> r;
    r.vertex = (typename std::conditional<mutability == Immutable, અચલ vertex_t*, vertex_t*>::type) &vertices_[index];
    r.table = r.vertex->template as<typename std::conditional<mutability == Immutable, અચલ T*, T*>::type> ();
    r.index = index;
    જો (unlikely (!r.table))
      પાછા Err(INVALID_ARGUMENT);

    TRY(r.table->sanitize (*(r.vertex), std::forward<Ts>(ds)...));
    પાછા r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  graph_result_t<unsigned> index_for_offset (unsigned ગાંઠ_idx, અચલ શૂન્ય* offset) અચલ
  {
    જો (unlikely (ગાંઠ_idx >= vertices_.length)) પાછા Err(OUT_OF_BOUNDS);
    અચલ auto& node = object (ગાંઠ_idx);
    જો (unlikely (offset < node.head || offset >= node.tail)) પાછા Err(OUT_OF_BOUNDS);

    unsigned ગણતરી = node.real_links.length;
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      અચલ auto& link = node.real_links.arrayZ[i];
      જો (offset != node.head + link.position)
        ચાલુ;
      પાછા Ok(link.objidx);
    }

    પાછા Err(INVALID_ARGUMENT);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  graph_result_t<unsigned> mutable_index_for_offset (unsigned ગાંઠ_idx, અચલ શૂન્ય* offset)
  {
    TRY_ASSIGN (unsigned child_idx, index_for_offset (ગાંઠ_idx, offset));
    જો (unlikely (child_idx >= vertices_.length))
      પાછા Err(OUT_OF_BOUNDS);

    auto& child = vertices_[child_idx];
    માટે (unsigned p : child.parents_iter ())
    {
      જો (p != ગાંઠ_idx) {
        પાછા duplicate (ગાંઠ_idx, child_idx, સાચું);
      }
    }

    પાછા Ok(child_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  graph_result_t<તાર્કિક> assign_spaces ()
  {
    TRY (update_parents ());

    માટે (auto& vertex : vertices_.writer ())
    {
      vertex.space = 0;
      vertex.priority = 0;
    }
    અંક_roots_for_space.resize (1);
    TRY (graph_result_t<શૂન્ય>::from (અંક_roots_for_space, ALLOCATION_FAILURE));
    અંક_roots_for_space[0] = 1;
    distance_invalid = સાચું;
    positions_invalid = સાચું;

    hb_set_t visited;
    hb_set_t roots;
    TRY (find_space_roots (visited, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    visited.invert ();

    જો (!roots) પાછા Ok(ખોટું);

    જ્યારે (roots)
    {
      uint32_t next = HB_SET_કિંમત_INVALID;
      TRY (graph_result_t<શૂન્ય>::from (roots, ALLOCATION_FAILURE));
      જો (!roots.next (&next)) તોડો;

      hb_set_t connected_roots;
      TRY (find_connected_nodes (next, roots, visited, connected_roots));
      TRY (graph_result_t<શૂન્ય>::from (connected_roots, ALLOCATION_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (graph_result_t<શૂન્ય>::from (connected_roots, ALLOCATION_FAILURE));

      unsigned next_space = આ->next_space ();
      જો (unlikely (next_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 next_space);
        પાછા Err(LIMIT_EXCEEDED);
      }
      અંક_roots_for_space.push (0);
      TRY(graph_result_t<શૂન્ય>::from (અંક_roots_for_space, ALLOCATION_FAILURE));

      માટે (unsigned મૂળ : connected_roots)
      {
        DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "Subgraph %u gets space %u", મૂળ, next_space);
        vertices_[મૂળ].space = next_space;
        અંક_roots_for_space[next_space] = અંક_roots_for_space[next_space] + 1;
        distance_invalid = સાચું;
        positions_invalid = સાચું;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    પાછા Ok(સાચું);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  graph_result_t<તાર્કિક> isolate_subgraph (hb_set_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_set_t parents;
    માટે (unsigned મૂળ_idx : roots)
    {
      જો (unlikely (મૂળ_idx >= vertices_.length))
        પાછા Err(OUT_OF_BOUNDS);
      subgraph.set (મૂળ_idx, wide_parents (મૂળ_idx, parents));
    }
    માટે (unsigned મૂળ_idx : roots)
    {
      TRY (find_subgraph (મૂળ_idx, subgraph));
    }
    TRY (graph_result_t<શૂન્ય>::from (subgraph, ALLOCATION_FAILURE));

    hb_map_t index_map;
    તાર્કિક made_changes = ખોટું;
    માટે (auto entry : subgraph.iter ())
    {
      પ્રતિજ્ઞા (entry.first < vertices_.length);
      અચલ auto& node = vertices_[entry.first];
      unsigned subgraph_incoming_edges = entry.second;

      જો (subgraph_incoming_edges < node.incoming_edges ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = સાચું;
        TRY (duplicate_subgraph (entry.first, index_map));
      }
    }

    TRY (graph_result_t<શૂન્ય>::from (index_map, ALLOCATION_FAILURE));

    જો (!made_changes)
      પાછા Ok(ખોટું);

    auto new_subgraph =
        + subgraph.ચાવીઓ ()
        | hb_map([&] (uint32_t ગાંઠ_idx) {
          અચલ uint32_t *v;
          જો (index_map.has (ગાંઠ_idx, &v)) પાછા *v;
          પાછા ગાંઠ_idx;
        })
        ;

    TRY(remap_obj_indices (index_map, new_subgraph));
    TRY(remap_obj_indices (index_map, parents.iter (), સાચું));

    // Update roots set with new indices as needed.
    માટે (auto next : roots)
    {
      અચલ uint32_t *v;
      જો (index_map.has (next, &v))
      {
        roots.del (next);
        roots.add (*v);
      }
    }

    પાછા Ok(સાચું);
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
  graph_result_t<શૂન્ય> traverse_directed_bfs (unsigned start_idx, VisitEdgeFunc&& visit_edge)
  {
    જો (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      પાછા Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<શૂન્ય>::from (ordering_scratch_, ALLOCATION_FAILURE));

    જો (!visit_edge (HB_CODEPOINT_INVALID, nullptr, start_idx, 0))
      પાછા Ok();

    unsigned head = 0;
    unsigned tail = 0;
    auto& queue = ordering_scratch_;
    queue[tail++] = start_idx;

    unsigned depth = 0;
    જ્યારે (head < tail)
    {
      unsigned level_end = tail;
      જ્યારે (head < level_end)
      {
        unsigned ગાંઠ_idx = queue[head++];
        અચલ auto& v = vertices_[ગાંઠ_idx];

        unsigned અંક_real = v.obj().real_links.length;
        unsigned total_links = અંક_real + v.obj().virtual_links.length;

        માટે (unsigned i = 0; i < total_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          અચલ auto& v = vertices_[ગાંઠ_idx];
          અચલ auto& link = (i < અંક_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - અંક_real];
          unsigned child_idx = link.objidx;

          જો (!visit_edge (ગાંઠ_idx, &link, child_idx, depth + 1))
            ચાલુ;

          જો (unlikely (tail >= queue.length))
            પાછા Err(CYCLE_DETECTED);

          queue[tail++] = child_idx;
        }
      }
      depth++;
    }
    પાછા Ok();
  }

  graph_result_t<શૂન્ય> find_subgraph (unsigned ગાંઠ_idx, hb_map_t& subgraph)
  {
    પાછા traverse_directed_bfs (ગાંઠ_idx, [&] (
      unsigned parent,
      અચલ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      જો (depth == 0) પાછા સાચું;
      hb_codepoint_t *ગણતરી;
      જો (subgraph.has (child, &ગણતરી))
      {
        (*ગણતરી)++;
        પાછા ખોટું;
      }
      subgraph.set (child, 1);
      પાછા સાચું;
    });
  }

  graph_result_t<શૂન્ય> find_subgraph (unsigned ગાંઠ_idx, hb_set_t& subgraph)
  {
    પાછા traverse_directed_bfs (ગાંઠ_idx, [&] (
      unsigned parent,
      અચલ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      જો (subgraph.has (child)) પાછા ખોટું;
      subgraph.add (child);
      પાછા સાચું;
    });
  }

  graph_result_t<size_t> find_subgraph_size (unsigned ગાંઠ_idx, hb_set_t& subgraph, unsigned max_depth = HB_GRAPH_INVALID)
  {
    size_t size = 0;
    TRY (traverse_directed_bfs (ગાંઠ_idx, [&] (
      unsigned parent,
      અચલ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      જો (subgraph.has (child)) પાછા ખોટું;
      subgraph.add (child);

      અચલ auto& o = vertices_[child].obj();
      size += o.tail - o.head;
      પાછા depth < max_depth;
    }));
    પાછા Ok(size);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  graph_result_t<શૂન્ય> find_32bit_roots (unsigned ગાંઠ_idx, hb_set_t& found)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_set_t visited;
    પાછા traverse_directed_bfs (ગાંઠ_idx, [&] (
      unsigned parent,
      અચલ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {

      જો (link && found.has(parent))
        // Don't traverse from something that's already marked as a root.
        પાછા ખોટું;

      જો (link && !link->is_signed && link->width == 4)
      {
        found.add (link->objidx);
        visited.add (link->objidx);
        પાછા ખોટું;
      }

      જો (visited.has (child)) પાછા ખોટું;
      visited.add (child);
      પાછા સાચું;
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
                                       અચલ O* old_offset,
                                       unsigned new_parent_idx,
                                       અચલ O* new_offset)
  {
    distance_invalid = સાચું;
    positions_invalid = સાચું;

    જો (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      પાછા Err(OUT_OF_BOUNDS);

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    TRY_ASSIGN (unsigned child_id, index_for_offset (old_parent_idx,
                                                     old_offset));
    જો (unlikely (child_id >= vertices_.length))
      પાછા Err(OUT_OF_BOUNDS);

    TRY(new_v.add_real_link(O::static_size, child_id, (અચલ અક્ષર*) new_offset - (અચલ અક્ષર*) new_v.obj().head));

    auto& child = vertices_[child_id];
    TRY(child.add_parent (new_parent_idx, ખોટું));

    old_v.remove_real_link_unordered (child_id, old_offset);
    child.remove_parent (old_parent_idx);

    પાછા Ok(child_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  template<typename O>
  graph_result_t<શૂન્ય> move_children (unsigned old_parent_idx,
                                      unsigned old_pos_start,
                                      unsigned old_pos_end,
                                      unsigned new_parent_idx,
                                      unsigned new_pos_start)
  {
    જો (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      પાછા Err(OUT_OF_BOUNDS);

    distance_invalid = સાચું;
    positions_invalid = સાચું;

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    hb_vector_t<hb_serialize_context_t::object_t::link_t> old_links;
    માટે (અચલ auto& l : old_v.obj().real_links)
    {
      જો (l.position < old_pos_start || l.position >= old_pos_end)
      {
        old_links.push (l);
        ચાલુ;
      }

      unsigned array_pos = l.position - old_pos_start;

      unsigned child_id = l.objidx;
      જો (unlikely (child_id >= vertices_.length))
        પાછા Err(OUT_OF_BOUNDS);

      TRY(new_v.add_real_link(O::static_size, child_id, new_pos_start + array_pos));

      auto& child = vertices_[child_id];
      TRY(child.add_parent (new_parent_idx, ખોટું));
      child.remove_parent (old_parent_idx);
    }

    TRY (graph_result_t<શૂન્ય>::from (old_links, ALLOCATION_FAILURE));
    પાછા old_v.set_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  graph_result_t<શૂન્ય> duplicate_subgraph (unsigned ગાંઠ_idx, hb_map_t& index_map)
  {
    graph_result_t<શૂન્ય> result = Ok();
    TRY (traverse_directed_bfs (ગાંઠ_idx, [&] (
      unsigned parent,
      અચલ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {
      જો (result.is_err () || index_map.has (child)) પાછા ખોટું;
      auto r = duplicate (child);
      જો (unlikely (r.is_err ()))
      {
        result = Err(r.error ());
        પાછા ખોટું;
      }
      index_map.set (child, *r);
      પાછા સાચું;
    }));

    પાછા graph_result_t<શૂન્ય>::from(index_map, ALLOCATION_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  graph_result_t<unsigned> duplicate (unsigned ગાંઠ_idx, તાર્કિક copy_table = ખોટું)
  {
    જો (unlikely (ગાંઠ_idx >= vertices_.length))
      પાછા Err(OUT_OF_BOUNDS);

    જો (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      પાછા Err(LIMIT_EXCEEDED);
    }

    positions_invalid = સાચું;
    distance_invalid = સાચું;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<શૂન્ય>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<શૂન્ય>::from (ordering_, ALLOCATION_FAILURE));

    auto& child = vertices_[ગાંઠ_idx];

    unsigned table_size = child.obj().tail - child.obj().head;
    જો (copy_table && table_size)
    {
      અક્ષર* buffer = (અક્ષર*) hb_malloc (table_size);
      જો (unlikely (!buffer))
        પાછા Err(ALLOCATION_FAILURE);
      auto res = add_buffer (buffer);
      જો (unlikely (res.is_err ()))
      {
        hb_free (buffer);
        પાછા Err(res.error ());
      }
      hb_memcpy (buffer, child.obj().head, table_size);
      clone->set_buffer(buffer, buffer + table_size);
    }
    નહીંતર
    {
      clone->set_buffer(child.obj().head, child.obj().tail);
    }
    clone->distance = child.distance;
    clone->space = child.space;
    clone->reset_parents ();

    માટે (અચલ auto& l : child.obj().real_links)
    {
      TRY(clone->add_real_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, ખોટું));
    }

    માટે (અચલ auto& l : child.obj().virtual_links)
    {
      TRY(clone->add_virtual_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, સાચું));
    }

    પાછા Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (unsigned parent_idx, unsigned child_idx, તાર્કિક copy_table = ખોટું)
  {
    જો (unlikely (parent_idx >= vertices_.length || child_idx >= vertices_.length))
      પાછા Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    અચલ auto& child = vertices_[child_idx];
    unsigned links_to_child = child.incoming_edges_from_parent (parent_idx);

    જો (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "  Not duplicating %u => %u",
                 parent_idx, child_idx);
      પાછા Err(INVALID_ARGUMENT);
    }

    DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "  Duplicating %u => %u",
               parent_idx, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx, copy_table));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    જો (parent_idx == clone_idx) parent_idx++;

    auto& parent = vertices_[parent_idx];
    unsigned ગણતરી = 0;
    unsigned અંક_real = parent.obj().real_links.length;
    માટે (auto& l : parent.all_links_writer ())
    {
      ગણતરી++;
      જો (l.objidx != child_idx)
        ચાલુ;

      TRY(reassign_link (l, parent_idx, clone_idx, ગણતરી > અંક_real));
    }

    પાછા Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (અચલ hb_set_t* parents, unsigned child_idx)
  {
    જો (unlikely (!parents || parents->is_empty ()))
      પાછા Err(INVALID_ARGUMENT);

    જો (unlikely (child_idx >= vertices_.length))
      પાછા Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    અચલ auto& child = vertices_[child_idx];
    unsigned links_to_child = 0;
    unsigned last_parent = parents->get_max ();
    unsigned first_parent = parents->get_min ();
    માટે (unsigned parent_idx : *parents) {
      જો (unlikely (parent_idx >= vertices_.length))
        પાછા Err(OUT_OF_BOUNDS);
      links_to_child += child.incoming_edges_from_parent (parent_idx);
    }

    જો (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "  Not duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);
      પાછા Err(INVALID_ARGUMENT);
    }

    DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "  Duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx));

    માટે (unsigned parent_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      જો (parent_idx == clone_idx) parent_idx++;
      auto& parent = vertices_[parent_idx];
      unsigned ગણતરી = 0;
      unsigned અંક_real = parent.obj().real_links.length;
      માટે (auto& l : parent.all_links_writer ())
      {
        ગણતરી++;
        જો (l.objidx != child_idx)
          ચાલુ;

        TRY(reassign_link (l, parent_idx, clone_idx, ગણતરી > અંક_real));
      }
    }

    પાછા Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  graph_result_t<unsigned> new_ગાંઠ (અક્ષર* head, અક્ષર* tail)
  {
    જો (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      પાછા Err(LIMIT_EXCEEDED);
    }

    positions_invalid = સાચું;
    distance_invalid = સાચું;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<શૂન્ય>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<શૂન્ય>::from (ordering_, ALLOCATION_FAILURE));

    clone->set_buffer (head, tail);
    clone->distance = 0;
    clone->space = 0;

    પાછા Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  graph_result_t<unsigned> remap_child (unsigned parent_idx, unsigned old_child_idx)
  {
    જો (unlikely (parent_idx >= vertices_.length || old_child_idx >= vertices_.length))
      પાછા Err(OUT_OF_BOUNDS);

    TRY_ASSIGN (unsigned new_child_idx, duplicate (old_child_idx));

    auto& parent = vertices_[parent_idx];
    માટે (auto& l : parent.real_links_writer ())
    {
      જો (l.objidx != old_child_idx)
        ચાલુ;
      TRY(reassign_link (l, parent_idx, new_child_idx, ખોટું));
    }

    માટે (auto& l : parent.virtual_links_writer ())
    {
      જો (l.objidx != old_child_idx)
        ચાલુ;
      TRY(reassign_link (l, parent_idx, new_child_idx, સાચું));
    }
    પાછા Ok(new_child_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  graph_result_t<તાર્કિક> raise_childrens_priority (unsigned parent_idx)
  {
    DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "  Raising priority of all children of %u",
               parent_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    જો (unlikely (parent_idx >= vertices_.length)) પાછા Err(OUT_OF_BOUNDS);
    તાર્કિક made_change = ખોટું;
    માટે (auto& l : vertices_[parent_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_priority ();
    પાછા Ok(made_change);
  }

  graph_result_t<શૂન્ય> is_fully_connected ()
  {
    TRY(update_parents());

    જો (unlikely (મૂળ().incoming_edges ()))
      // Root cannot have parents.
      પાછા Err(ORPHANED_NODES);

    માટે (unsigned i = 0; i < મૂળ_idx (); i++)
    {
      જો (unlikely (!vertices_[i].incoming_edges ()))
        પાછા Err(ORPHANED_NODES);
    }
    પાછા Ok();
  }

#જો 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  શૂન્ય save_fuzzer_seed (hb_tag_t tag) અચલ
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((શૂન્ય*) &tag, કદ (tag), 1, f);

    uint16_t અંક_objects = vertices_.length;
    fwrite ((શૂન્ય*) &અંક_objects, કદ (અંક_objects), 1, f);

    માટે (અચલ auto& v : vertices_)
    {
      uint16_t blob_size = v.table_size ();
      fwrite ((શૂન્ય*) &blob_size, કદ (blob_size), 1, f);
      fwrite ((અચલ શૂન્ય*) v.obj.head, blob_size, 1, f);
    }

    uint16_t link_ગણતરી = 0;
    માટે (અચલ auto& v : vertices_)
      link_ગણતરી += v.obj.real_links.length;

    fwrite ((શૂન્ય*) &link_ગણતરી, કદ (link_ગણતરી), 1, f);

    પ્રકારવ્યાખ્યા સંરચના
    {
      uint16_t parent;
      uint16_t child;
      uint16_t position;
      uint8_t width;
    } link_t;

    માટે (unsigned i = 0; i < vertices_.length; i++)
    {
      માટે (અચલ auto& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.width
        };
        fwrite ((શૂન્ય*) &link, કદ (link), 1, f);
      }
    }

    fclose (f);
  }
#endif

  unsigned અંક_roots_for_space_2 (unsigned space) અચલ
  {
    પાછા અંક_roots_for_space[space];
  }

  unsigned next_space () અચલ
  {
    પાછા અંક_roots_for_space.length;
  }

  graph_result_t<શૂન્ય> move_to_new_space (અચલ hb_set_t& indices)
  {
    જો (unlikely (અંક_roots_for_space.length >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 અંક_roots_for_space.length);
      પાછા Err(LIMIT_EXCEEDED);
    }
    અંક_roots_for_space.push (0);
    TRY (graph_result_t<શૂન્ય>::from (અંક_roots_for_space, ALLOCATION_FAILURE));
    unsigned new_space = અંક_roots_for_space.length - 1;

    માટે (unsigned index : indices) {
      જો (unlikely (index >= vertices_.length))
        પાછા Err(OUT_OF_BOUNDS);
      auto& node = vertices_[index];
      અંક_roots_for_space[node.space] = અંક_roots_for_space[node.space] - 1;
      અંક_roots_for_space[new_space] = અંક_roots_for_space[new_space] + 1;
      node.space = new_space;
      distance_invalid = સાચું;
      positions_invalid = સાચું;
    }
    પાછા Ok();
  }

  unsigned space_for (unsigned index, unsigned* મૂળ = nullptr) અચલ
  {
    જ્યારે (સાચું)
    {
      પ્રતિજ્ઞા (index < vertices_.length);
      અચલ auto& node = vertices_[index];
      જો (node.space)
      {
        જો (મૂળ != nullptr)
          *મૂળ = index;
        પાછા node.space;
      }

      જો (!node.incoming_edges ())
      {
        જો (મૂળ)
          *મૂળ = index;
        પાછા 0;
      }

      index = *node.parents_iter ();
    }
  }

  size_t total_size_in_bytes () અચલ {
    size_t total_size = 0;
    unsigned ગણતરી = vertices_.length;
    માટે (unsigned i = 0; i < ગણતરી; i++) {
      size_t size = vertices_.arrayZ[i].table_size ();
      total_size += size;
    }
    પાછા total_size;
  }


 ખાનગી:

  શૂન્ય print_orphaned_nodes ()
  {
    જો (!DEBUG_ENABLED(SUBSET_REPACK)) પાછા;

    DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "Graph is not fully connected.");

    parents_invalid = સાચું;
    (શૂન્ય) update_parents();

    જો (મૂળ().incoming_edges ()) {
      DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "Root node has incoming edges.");
    }

    માટે (unsigned i = 0; i < vertices_.length; i++)
    {
      અચલ auto& v = vertices_[i];
      જો (!v.incoming_edges ())
        DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  unsigned wide_parents (unsigned ગાંઠ_idx, hb_set_t& parents) અચલ
  {
    unsigned ગણતરી = 0;
    માટે (unsigned p : vertices_[ગાંઠ_idx].parents_iter ())
    {
      // Only real links can be wide
      માટે (અચલ auto& l : vertices_[p].obj().real_links)
      {
        જો (l.objidx == ગાંઠ_idx
            && (l.width == 3 || l.width == 4)
            && !l.is_signed)
        {
          ગણતરી++;
          parents.add (p);
        }
      }
    }
    પાછા ગણતરી;
  }

 જાહેર:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  graph_result_t<શૂન્ય> update_parents ()
  {
    જો (!parents_invalid) પાછા Ok();

    unsigned ગણતરી = vertices_.length;

    માટે (unsigned i = 0; i < ગણતરી; i++)
      vertices_.arrayZ[i].reset_parents ();

    માટે (unsigned p = 0; p < ગણતરી; p++)
    {
      માટે (અચલ auto& l : vertices_.arrayZ[p].obj().real_links)
      {
        જો (unlikely (l.objidx >= ગણતરી)) પાછા Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, ખોટું));
      }

      માટે (અચલ auto& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        જો (unlikely (l.objidx >= ગણતરી)) પાછા Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, સાચું));
      }
    }

    parents_invalid = ખોટું;
    પાછા Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  શૂન્ય update_positions ()
  {
    જો (!positions_invalid) પાછા;

    unsigned current_pos = 0;
    માટે (unsigned i : ordering_)
    {
      auto& v = vertices_[i];
      v.start = current_pos;
      current_pos += v.obj().tail - v.obj().head;
      v.end = current_pos;
    }

    positions_invalid = ખોટું;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  graph_result_t<શૂન્ય> update_distances ()
  {
    જો (!distance_invalid) પાછા Ok();

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
    unsigned ગણતરી = vertices_.length;
    માટે (unsigned i = 0; i < ગણતરી; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[મૂળ_idx ()].distance = 0;

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (ગણતરી);
    queue.insert (0, મૂળ_idx ());
    TRY (graph_result_t<શૂન્ય>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<તાર્કિક> visited;
    visited.resize (vertices_.length);
    TRY (graph_result_t<શૂન્ય>::from (visited, ALLOCATION_FAILURE));

    જ્યારે (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_idx = queue.કાઢો_minimum ().second;
      જો (visited[next_idx]) ચાલુ;
      અચલ auto& next = vertices_[next_idx];
      int64_t next_distance = next.distance;
      visited[next_idx] = સાચું;

      માટે (અચલ auto& link : next.obj().all_links ())
      {
        જો (unlikely (link.objidx >= vertices_.length))
          પાછા Err(OUT_OF_BOUNDS);

        જો (visited[link.objidx]) ચાલુ;

        auto& child_v = vertices_.arrayZ[link.objidx];
        અચલ auto& child = child_v.obj ();
        unsigned link_width = link.width ? link.width : 4; // treat virtual offsets as 32 bits wide
        int64_t child_weight = (child.tail - child.head) +
                               ((int64_t) 1 << (link_width * 8)) * (child_v.space + 1);
        int64_t child_distance = next_distance + child_weight;

        જો (child_distance < child_v.distance)
        {
          child_v.distance = child_distance;
          queue.insert (child_distance, link.objidx);
        }
      }
    }

    TRY (graph_result_t<શૂન્ય>::from (queue, ALLOCATION_FAILURE));
    જો (unlikely (!queue.is_empty ()))
    {
      print_orphaned_nodes ();
      પાછા Err(ORPHANED_NODES);
    }

    distance_invalid = ખોટું;
    પાછા Ok();
  }

 ખાનગી:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  graph_result_t<શૂન્ય> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      unsigned parent_idx,
                      unsigned new_idx,
                      તાર્કિક is_virtual)
  {
    unsigned old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_parent (parent_idx);
    પાછા vertices_[new_idx].add_parent (parent_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  template<typename Iterator, hb_requires (hb_is_iterator (Iterator))>
  graph_result_t<શૂન્ય> remap_obj_indices (અચલ hb_map_t& id_map,
                                          Iterator subgraph,
                                          તાર્કિક only_wide = ખોટું)
  {
    જો (!id_map) પાછા Ok();
    માટે (unsigned i : subgraph)
    {
      auto& vertex = vertices_[i];
      unsigned અંક_real = vertex.obj().real_links.length;
      unsigned ગણતરી = 0;
      માટે (auto& link : vertex.all_links_writer ())
      {
        ગણતરી++;
        અચલ uint32_t *v;
        જો (!id_map.has (link.objidx, &v)) ચાલુ;
        જો (only_wide && (link.is_signed || (link.width != 4 && link.width != 3))) ચાલુ;

        TRY(reassign_link (link, i, *v, ગણતરી > અંક_real));
      }
    }
    પાછા Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  graph_result_t<શૂન્ય> find_connected_nodes (unsigned start_idx,
                                             hb_set_t& targets,
                                             hb_set_t& visited,
                                             hb_set_t& connected)
  {
    TRY (graph_result_t<શૂન્ય>::from (visited, ALLOCATION_FAILURE));
    જો (visited.has (start_idx)) પાછા Ok();
    જો (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      પાછા Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<શૂન્ય>::from (ordering_scratch_, ALLOCATION_FAILURE));

    auto& ઢગલો = ordering_scratch_;
    unsigned ઢગલો_len = 0;

    graph_result_t<શૂન્ય> res = Ok();
    auto handle_ગાંઠ = [&] (unsigned ગાંઠ_idx) {
      visited.add (ગાંઠ_idx);
      જો (targets.has (ગાંઠ_idx)) {
        targets.del (ગાંઠ_idx);
        connected.add (ગાંઠ_idx);
      }

      જો (unlikely (ઢગલો_len >= ઢગલો.length))
      {
        res = Err(CYCLE_DETECTED);
        પાછા ખોટું;
      }
      ઢગલો[ઢગલો_len++] = ગાંઠ_idx;
      પાછા સાચું;
    };

    જો (!handle_ગાંઠ (start_idx)) પાછા res;

    જ્યારે (ઢગલો_len > 0)
    {
      unsigned ગાંઠ_idx = ઢગલો[--ઢગલો_len];
      અચલ auto& v = vertices_[ગાંઠ_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      માટે (અચલ auto& l : v.obj().all_links ())
      {
        unsigned child_idx = l.objidx;
        જો (visited.has (child_idx)) ચાલુ;
        જો (!handle_ગાંઠ (child_idx)) પાછા res;
      }

      માટે (unsigned parent_idx : v.parents_iter ())
      {
        જો (visited.has (parent_idx)) ચાલુ;
        જો (!handle_ગાંઠ (parent_idx)) પાછા res;
      }
    }

    TRY (graph_result_t<શૂન્ય>::from (visited, ALLOCATION_FAILURE));
    TRY (graph_result_t<શૂન્ય>::from (connected, ALLOCATION_FAILURE));

    પાછા Ok();
  }

 જાહેર:
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

 ખાનગી:
  તાર્કિક parents_invalid = સાચું;
  તાર્કિક distance_invalid = સાચું;
  તાર્કિક positions_invalid = સાચું;
  hb_vector_t<unsigned> અંક_roots_for_space;
  hb_vector_t<અક્ષર*> buffers;
};

}

#endif  // GRAPH_GRAPH_HH
