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

#ضمن "../hb-set.hh"
#ضمن "../hb-priority-queue.hh"
#ضمن "../hb-serialize.hh"
#ضمن "graph-result.hh"

#ifndef GRAPH_GRAPH_HH
#define GRAPH_GRAPH_HH

#define HB_GRAPH_INVALID ((unsigned) -1)

نطاق graph {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
بنية graph_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  بنية vertex_t
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

    خاص:
    hb_serialize_context_t::object_t obj_;
    unsigned incoming_edges_ = 0;
    unsigned single_parent = HB_GRAPH_INVALID;
    منطقي has_incoming_virtual_edges_ = خطأ;
    hb_hashmap_t<unsigned, unsigned> parents;
    hb_set_t virtual_parents;
    عام:

    vertex_t ()
    {}

    explicit vertex_t(ثابت hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    auto parents_iter () ثابت HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_parent, single_parent != HB_GRAPH_INVALID),
	parents.مفاتيح_ref ()
      )
    )

    فراغ set_buffer (حرف* head, حرف* tail) {
      obj_.head = head;
      obj_.tail = tail;
    }

    فراغ shrink_buffer (size_t عدد)
    {
      obj_.tail -= عدد;
    }

    auto all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    auto real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    auto virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    فراغ sort_real_links () {
      obj_.real_links.qsort ();
    }

    graph_result_t<فراغ> add_real_link (unsigned width, unsigned obj_idx, unsigned position) {
      auto* link = obj_.real_links.push ();
      TRY (graph_result_t<فراغ>::from (obj_.real_links, ALLOCATION_FAILURE));
      link->width = width;
      link->objidx = obj_idx;
      link->position = position;
      أرجع Ok();
    }

    graph_result_t<فراغ> add_virtual_link (unsigned obj_idx) {
      auto* link = obj_.virtual_links.push ();
      TRY (graph_result_t<فراغ>::from (obj_.virtual_links, ALLOCATION_FAILURE));
      link->objidx = obj_idx;
      أرجع Ok();
    }

    graph_result_t<فراغ> add_real_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      أرجع graph_result_t<فراغ>::from (obj_.real_links, ALLOCATION_FAILURE);
    }

    graph_result_t<فراغ> set_real_links (hb_vector_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(graph_result_t<فراغ>::from (links, ALLOCATION_FAILURE));
      obj_.real_links = links;
      أرجع Ok();
    }

    فراغ clear_real_links ()
    {
      obj_.real_links.reset();
    }

    فراغ clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    graph_result_t<فراغ> add_virtual_link (ثابت hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      أرجع graph_result_t<فراغ>::from (obj_.virtual_links, ALLOCATION_FAILURE);
    }

    ثابت hb_serialize_context_t::object_t& obj() ثابت { أرجع obj_; }

    template<typename T>
    T as () ثابت
    {
      أرجع reinterpret_cast<T> (obj_.head);
    }

    منطقي has_incoming_virtual_edges () ثابت
    {
      أرجع has_incoming_virtual_edges_;
    }

    graph_result_t<فراغ> link_positions_valid (unsigned رقم_objects, منطقي removed_nil)
    {
      hb_set_t assigned_bytes;
      لكل (ثابت auto& l : obj_.real_links)
      {
        إذا (unlikely (l.objidx >= رقم_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_رسالة (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid object index.");
          أرجع Err(INVALID_ARGUMENT);
        }

        unsigned start = l.position;
        unsigned end = start + l.width - 1;

        إذا (unlikely (l.width < 2 || l.width > 4))
        {
          DEBUG_رسالة (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid link width.");
          أرجع Err(INVALID_ARGUMENT);
        }

        إذا (unlikely (end >= table_size ()))
        {
          DEBUG_رسالة (SUBSET_REPACK, nullptr,
                     "Invalid graph. Link position is out of bounds.");
          أرجع Err(OUT_OF_BOUNDS);
        }

        إذا (unlikely (assigned_bytes.intersects (start, end)))
        {
          DEBUG_رسالة (SUBSET_REPACK, nullptr,
                     "Invalid graph. Found offsets whose positions overlap.");
          أرجع Err(INVALID_ARGUMENT);
        }

        assigned_bytes.add_range (start, end);
      }

      أرجع graph_result_t<فراغ>::from (assigned_bytes, ALLOCATION_FAILURE);
    }

    فراغ normalize ()
    {
      obj_.real_links.qsort ();
      لكل (ثابت auto& l : obj_.real_links)
      {
        لكل (unsigned i = 0; i < l.width; i++)
        {
          obj_.head[l.position + i] = 0;
        }
      }
    }

    منطقي equals (unsigned this_index,
                 unsigned other_index,
                 ثابت vertex_t& other,
                 ثابت graph_t& graph,
                 ثابت graph_t& other_graph,
                 unsigned depth) ثابت
    {
      إذا (!(as_bytes () == other.as_bytes ()))
      {
        DEBUG_رسالة (SUBSET_REPACK, nullptr,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_index,
                   (unsigned long) table_size (),
                   other_index,
                   (unsigned long) other.table_size (),
                   depth);

        auto a = as_bytes ();
        auto b = other.as_bytes ();
        طالما (a || b)
        {
          DEBUG_رسالة (SUBSET_REPACK, nullptr,
                     "  0x%x %s 0x%x", (unsigned) *a, (*a == *b) ? "==" : "!=", (unsigned) *b);
          a++;
          b++;
        }
        أرجع خطأ;
      }

      أرجع links_equal (obj_.real_links, other.obj_.real_links, graph, other_graph, depth);
    }

    hb_bytes_t as_bytes () ثابت
    {
      أرجع hb_bytes_t (obj_.head, table_size ());
    }

    friend فراغ swap (vertex_t& a, vertex_t& b)
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
    position_to_index_map () ثابت
    {
      hb_hashmap_t<unsigned, unsigned> result;

      result.alloc (obj_.real_links.length);
      لكل (ثابت auto& l : obj_.real_links) {
        result.set (l.position, l.objidx);
      }

      أرجع graph_result_t<hb_hashmap_t<unsigned, unsigned>>::from (std::move(result), ALLOCATION_FAILURE);
    }

    منطقي is_shared () ثابت
    {
      أرجع parents.get_population () > virtual_parents.get_population () + 1;
    }

    unsigned incoming_edges () ثابت
    {
      إذا (HB_DEBUG_SUBSET_REPACK)
       {
	أكد (incoming_edges_ == (single_parent != HB_GRAPH_INVALID) +
		(parents.قيم_ref () | hb_reduce (hb_add, 0)));
       }
      أرجع incoming_edges_;
    }

    unsigned incoming_edges_from_parent (unsigned parent_index) ثابت {
      إذا (single_parent != HB_GRAPH_INVALID) {
        أرجع single_parent == parent_index ? 1 : 0;
      }

      unsigned* عدد;
      أرجع  parents.has(parent_index, &عدد) ? *عدد : 0;
    }

    فراغ reset_parents ()
    {
      incoming_edges_ = 0;
      has_incoming_virtual_edges_ = خطأ;
      single_parent = HB_GRAPH_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    graph_result_t<فراغ> add_parent (unsigned parent_index, منطقي is_virtual)
    {
      أكد (parent_index != HB_GRAPH_INVALID);
      has_incoming_virtual_edges_ |= is_virtual;
      إذا (is_virtual)
        virtual_parents.add (parent_index);

      إذا (incoming_edges_ == 0)
      {
	single_parent = parent_index;
	incoming_edges_ = 1;
	أرجع to_result();
      }
      وإلا إذا (single_parent != HB_GRAPH_INVALID)
      {
        أكد (incoming_edges_ == 1);
	إذا (!parents.set (single_parent, 1))
	  أرجع to_result ();
	single_parent = HB_GRAPH_INVALID;
      }

      unsigned *v;
      إذا (parents.has (parent_index, &v))
      {
        (*v)++;
	incoming_edges_++;
      }
      وإلا إذا (parents.set (parent_index, 1))
	incoming_edges_++;

      أرجع to_result();
    }

    فراغ remove_parent (unsigned parent_index)
    {
      إذا (parent_index == single_parent)
      {
	single_parent = HB_GRAPH_INVALID;
	incoming_edges_--;
	virtual_parents.reset ();
	أرجع;
      }

      unsigned *v;
      إذا (parents.has (parent_index, &v))
      {
	incoming_edges_--;
	إذا (*v > 1)
	  (*v)--;
	وإلا
        {
	  parents.del (parent_index);
          virtual_parents.del (parent_index);
        }

	إذا (incoming_edges_ == 1)
	{
	  single_parent = *parents.مفاتيح ();
	  parents.reset ();
	}
      }
    }

    فراغ remove_real_link_unordered (unsigned link_array_index) {
      obj_.real_links.remove_unordered (link_array_index);
    }

    فراغ remove_real_link_unordered (unsigned child_index, ثابت فراغ* offset)
    {
      unsigned عدد = obj_.real_links.length;
      لكل (unsigned i = 0; i < عدد; i++)
      {
        auto& link = obj_.real_links.arrayZ[i];
        إذا (link.objidx != child_index)
          استمر;

        إذا ((obj_.head + link.position) != offset)
          استمر;

        obj_.real_links.remove_unordered (i);
        أرجع;
      }
    }

    graph_result_t<فراغ> remap_parent (unsigned old_index, unsigned new_index)
    {
      إذا (single_parent != HB_GRAPH_INVALID)
      {
        إذا (single_parent == old_index)
        {
	  single_parent = new_index;
          إذا (virtual_parents.has (old_index))
          {
            virtual_parents.del (old_index);
            virtual_parents.add (new_index);
          }
        }
        أرجع graph_result_t<فراغ>::from(virtual_parents, ALLOCATION_FAILURE);
      }

      ثابت unsigned *pv;
      إذا (parents.has (old_index, &pv))
      {
        unsigned v = *pv;
	إذا (!parents.set (new_index, v))
          incoming_edges_ -= v;
	parents.del (old_index);

        إذا (incoming_edges_ == 1)
	{
	  single_parent = *parents.مفاتيح ();
	  parents.reset ();
	}
      }

      إذا (virtual_parents.has (old_index))
      {
        virtual_parents.del (old_index);
        virtual_parents.add (new_index);
      }

      أرجع graph_result_t<فراغ>::from(virtual_parents, ALLOCATION_FAILURE);
    }

    منطقي is_leaf () ثابت
    {
      أرجع !obj_.real_links.length && !obj_.virtual_links.length;
    }

    منطقي raise_priority ()
    {
      إذا (has_max_priority ()) أرجع خطأ;
      priority++;
      أرجع true;
    }

    منطقي give_max_priority ()
    {
      منطقي result = خطأ;
      طالما (!has_max_priority()) {
        result = true;
        priority++;
      }
      أرجع result;
    }

    منطقي has_max_priority () ثابت {
      أرجع priority >= 3;
    }

    size_t table_size () ثابت {
      أرجع obj_.tail - obj_.head;
    }

    int64_t modified_distance (unsigned order) ثابت
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_distance =
          hb_clamp (distance + distance_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      إذا (has_max_priority ()) {
        modified_distance = 0;
      }
      أرجع (modified_distance << 18) | (0x003FFFF & order);
    }

    int64_t distance_modifier () ثابت
    {
      إذا (!priority) أرجع 0;
      int64_t table_size = obj_.tail - obj_.head;

      إذا (priority == 1)
        أرجع -table_size / 2;

      أرجع -table_size;
    }

   خاص:
    graph_result_t<فراغ> to_result() ثابت {
      إذا (unlikely (
        parents.in_error() || virtual_parents.in_error() ||
        obj_.real_links.in_error() || obj_.virtual_links.in_error()))
        أرجع Err(ALLOCATION_FAILURE);
      أرجع Ok();
    }

    منطقي links_equal (ثابت hb_vector_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      ثابت hb_vector_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      ثابت graph_t& graph,
                      ثابت graph_t& other_graph,
                      unsigned depth) ثابت
    {
      auto a = this_links.iter ();
      auto b = other_links.iter ();

      طالما (a && b)
      {
        ثابت auto& link_a = *a;
        ثابت auto& link_b = *b;

        إذا (link_a.width != link_b.width ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          أرجع خطأ;

        إذا (!graph.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_graph.vertices_[link_b.objidx], graph, other_graph, depth + 1))
          أرجع خطأ;

        a++;
        b++;
      }

      إذا (منطقي (a) != منطقي (b))
        أرجع خطأ;

      أرجع true;
    }
  };

  تعداد vertex_mutability_t
  {
    Immutable,
    Mutable
  };

  template <typename T, vertex_mutability_t mutability = Mutable>
  بنية vertex_and_table_t
  {
    vertex_and_table_t () : index (0), vertex (nullptr), table (nullptr)
    {}

    template <vertex_mutability_t other_mutability>
    vertex_and_table_t (ثابت vertex_and_table_t<T, other_mutability>& o)
      : index (o.index), vertex (o.vertex), table (o.table)
    {}

    unsigned index;
    typename std::conditional<mutability == Immutable, ثابت vertex_t*, vertex_t*>::type vertex;
    typename std::conditional<mutability == Immutable, ثابت T*, T*>::type table;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  template<typename T>
  ساكن graph_result_t<graph_t> create (ثابت T& objects)
  {
    إذا (unlikely (objects.length > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_رسالة (SUBSET_REPACK, nullptr,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.length);
      أرجع Err(LIMIT_EXCEEDED);
    }

    graph_t g;
    g.رقم_roots_for_space.push (1);
    TRY (graph_result_t<فراغ>::from (g.رقم_roots_for_space, ALLOCATION_FAILURE));

    منطقي removed_nil = خطأ;
    ثابت unsigned عدد = objects.length;
    g.vertices_.alloc (عدد);
    g.ordering_.resize (عدد);
    g.ordering_scratch_.alloc (عدد);
    TRY (graph_result_t<فراغ>::from (g.vertices_, ALLOCATION_FAILURE));
    TRY (graph_result_t<فراغ>::from (g.ordering_, ALLOCATION_FAILURE));
    TRY (graph_result_t<فراغ>::from (g.ordering_scratch_, ALLOCATION_FAILURE));

    unsigned order = objects.length;
    unsigned skip = 0;
    لكل (unsigned i = 0; i < عدد; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      إذا (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = true;
        order--;
        g.ordering_.resize (عدد - 1);
        skip++;
        استمر;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      إذا (unlikely (obj.in_error ())) {
        أرجع Err(ALLOCATION_FAILURE);
      }
      vertex_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (عدد, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      unsigned obj_idx = i - skip;
      g.ordering_[--order] = obj_idx;

      إذا (!removed_nil) استمر;
      // Fix indices to account for removed nil object.
      لكل (auto& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    auto r = g.is_fully_connected();
    إذا (r.is_err() && r.error() == ORPHANED_NODES) {
      g.اطبع_orphaned_nodes ();
    }
    TRY(r);

    أرجع Ok (std::move (g));
  }

  graph_t (ثابت graph_t&) = delete;
  graph_t& operator = (ثابت graph_t&) = delete;
  graph_t (graph_t&&) = افتراضي;
  graph_t& operator = (graph_t&&) = افتراضي;

  ~graph_t ()
  {
    لكل (حرف* b : buffers)
      hb_free (b);
  }

خاص:
  graph_t () = افتراضي;

عام:

  منطقي operator== (ثابت graph_t& other) ثابت
  {
    أرجع جذر ().equals (جذر_idx(), other.جذر_idx(), other.جذر (), *هذا, other, 0);
  }

  فراغ اطبع () ثابت {
    لكل (unsigned id : ordering_)
    {
      ثابت auto& v = vertices_[id];
      printf("%u: %u [", id, (unsigned صحيح)v.table_size());
      لكل (ثابت auto &l : v.obj().real_links) {
        printf("%u, ", l.objidx);
      }
      لكل (ثابت auto &l : v.obj().virtual_links) {
        printf("v%u, ", l.objidx);
      }
      printf("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  فراغ normalize ()
  {
    لكل (auto& v : vertices_.writer ())
      v.normalize ();
  }

  ثابت vertex_t& جذر () ثابت
  {
    أرجع vertices_[جذر_idx ()];
  }

  unsigned جذر_idx () ثابت
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    أرجع ordering_[0];
  }

  ثابت hb_serialize_context_t::object_t& object (unsigned i) ثابت
  {
    أرجع vertices_[i].obj();
  }

  graph_result_t<فراغ> add_buffer (حرف* buffer)
  {
    buffers.push (buffer);
    أرجع graph_result_t<فراغ>::from (buffers, ALLOCATION_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  template<typename T>
  graph_result_t<فراغ> add_link (T* offset,
                                 unsigned parent_id,
                                 unsigned child_id)
  {
    إذا (unlikely (parent_id >= vertices_.length || child_id >= vertices_.length))
      أرجع Err(OUT_OF_BOUNDS);
    auto& v = vertices_[parent_id];
    TRY(v.add_real_link(2, child_id, (حرف*) offset - (حرف*) v.obj().head));
    أرجع vertices_[child_id].add_parent (parent_id, خطأ);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  graph_result_t<فراغ> sort_shortest_distance_if_needed ()
  {
    إذا (!positions_invalid) أرجع Ok();
    أرجع sort_shortest_distance ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  graph_result_t<فراغ> sort_shortest_distance ()
  {
    positions_invalid = true;

    إذا (vertices_.length <= 1) {
      // Graph of 1 or less doesn't need sorting.
      أرجع Ok();
    }

    TRY (update_distances ());

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (vertices_.length);
    TRY (graph_result_t<فراغ>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.length);
    TRY (graph_result_t<فراغ>::from (new_ordering, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> removed_edges;
    removed_edges.resize (vertices_.length);
    TRY (graph_result_t<فراغ>::from (removed_edges, ALLOCATION_FAILURE));

    TRY (update_parents ());

    queue.insert (جذر ().modified_distance (0), جذر_idx ());
    unsigned order = 1;
    unsigned pos = 0;
    طالما (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_id = queue.اسحب_minimum().second;

      إذا (unlikely (pos >= new_ordering.length)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_رسالة (SUBSET_REPACK, nullptr, "Invalid graph. Contains cycle.");
        أرجع Err(CYCLE_DETECTED);
      }
      new_ordering[pos++] = next_id;
      ثابت vertex_t& next = vertices_[next_id];

      لكل (ثابت auto& link : next.obj().all_links ()) {
        إذا (unlikely (link.objidx >= vertices_.length))
          أرجع Err(OUT_OF_BOUNDS);
        removed_edges[link.objidx]++;
        ثابت auto& v = vertices_[link.objidx];
        إذا (!(v.incoming_edges () - removed_edges[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          queue.insert (v.modified_distance (order++),
                        link.objidx);
      }
    }

    TRY (graph_result_t<فراغ>::from (queue, ALLOCATION_FAILURE));
    TRY (graph_result_t<فراغ>::from (new_ordering, ALLOCATION_FAILURE));

    hb_swap (ordering_, new_ordering);

    إذا (unlikely (pos != vertices_.length)) {
      اطبع_orphaned_nodes ();
      أرجع Err(ORPHANED_NODES);
    }

    أرجع Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  graph_result_t<فراغ> find_space_roots (hb_set_t& visited, hb_set_t& roots)
  {
    unsigned جذر_index = جذر_idx ();
    لكل (unsigned i : ordering_)
    {
      إذا (visited.has (i)) استمر;

      // Only real links can form 32 bit spaces
      لكل (ثابت auto& l : vertices_[i].obj().real_links)
      {
        إذا (l.is_signed || l.width < 3)
          استمر;

        إذا (i == جذر_index && l.width == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          استمر;

        إذا (l.width >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_set_t sub_roots;
          TRY (find_32bit_roots (l.objidx, sub_roots));
          إذا (sub_roots) {
            لكل (unsigned sub_جذر_idx : sub_roots) {
              roots.add (sub_جذر_idx);
              TRY (find_subgraph (sub_جذر_idx, visited));
            }
            استمر;
          }
        }

        roots.add (l.objidx);
        TRY (find_subgraph (l.objidx, visited));
      }
    }

    TRY(graph_result_t<فراغ>::from (visited, ALLOCATION_FAILURE));
    TRY(graph_result_t<فراغ>::from (roots, ALLOCATION_FAILURE));
    أرجع Ok();
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Immutable>> as_table (unsigned parent, ثابت فراغ* offset, Ts... ds)
  {
    TRY_ASSIGN (unsigned idx, index_for_offset (parent, offset));
    أرجع as_table_from_index<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Mutable>> as_mutable_table (unsigned parent, ثابت فراغ* offset, Ts... ds)
  {
    TRY_ASSIGN (auto idx, mutable_index_for_offset (parent, offset));
    أرجع as_table_from_index<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, vertex_mutability_t mutability = Mutable, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, mutability>> as_table_from_index (unsigned index, Ts... ds)
  {
    إذا (unlikely (index >= vertices_.length))
      أرجع Err(OUT_OF_BOUNDS);

    vertex_and_table_t<T, mutability> r;
    r.vertex = (typename std::conditional<mutability == Immutable, ثابت vertex_t*, vertex_t*>::type) &vertices_[index];
    r.table = r.vertex->template as<typename std::conditional<mutability == Immutable, ثابت T*, T*>::type> ();
    r.index = index;
    إذا (unlikely (!r.table))
      أرجع Err(INVALID_ARGUMENT);

    TRY(r.table->sanitize (*(r.vertex), std::forward<Ts>(ds)...));
    أرجع r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  graph_result_t<unsigned> index_for_offset (unsigned عقدة_idx, ثابت فراغ* offset) ثابت
  {
    إذا (unlikely (عقدة_idx >= vertices_.length)) أرجع Err(OUT_OF_BOUNDS);
    ثابت auto& node = object (عقدة_idx);
    إذا (unlikely (offset < node.head || offset >= node.tail)) أرجع Err(OUT_OF_BOUNDS);

    unsigned عدد = node.real_links.length;
    لكل (unsigned i = 0; i < عدد; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      ثابت auto& link = node.real_links.arrayZ[i];
      إذا (offset != node.head + link.position)
        استمر;
      أرجع Ok(link.objidx);
    }

    أرجع Err(INVALID_ARGUMENT);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  graph_result_t<unsigned> mutable_index_for_offset (unsigned عقدة_idx, ثابت فراغ* offset)
  {
    TRY_ASSIGN (unsigned child_idx, index_for_offset (عقدة_idx, offset));
    إذا (unlikely (child_idx >= vertices_.length))
      أرجع Err(OUT_OF_BOUNDS);

    auto& child = vertices_[child_idx];
    لكل (unsigned p : child.parents_iter ())
    {
      إذا (p != عقدة_idx) {
        أرجع duplicate (عقدة_idx, child_idx, true);
      }
    }

    أرجع Ok(child_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  graph_result_t<منطقي> assign_spaces ()
  {
    TRY (update_parents ());

    لكل (auto& vertex : vertices_.writer ())
    {
      vertex.space = 0;
      vertex.priority = 0;
    }
    رقم_roots_for_space.resize (1);
    TRY (graph_result_t<فراغ>::from (رقم_roots_for_space, ALLOCATION_FAILURE));
    رقم_roots_for_space[0] = 1;
    distance_invalid = true;
    positions_invalid = true;

    hb_set_t visited;
    hb_set_t roots;
    TRY (find_space_roots (visited, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    visited.invert ();

    إذا (!roots) أرجع Ok(خطأ);

    طالما (roots)
    {
      uint32_t next = HB_SET_قيمة_INVALID;
      TRY (graph_result_t<فراغ>::from (roots, ALLOCATION_FAILURE));
      إذا (!roots.next (&next)) اكسر;

      hb_set_t connected_roots;
      TRY (find_connected_nodes (next, roots, visited, connected_roots));
      TRY (graph_result_t<فراغ>::from (connected_roots, ALLOCATION_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (graph_result_t<فراغ>::from (connected_roots, ALLOCATION_FAILURE));

      unsigned next_space = هذا->next_space ();
      إذا (unlikely (next_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_رسالة (SUBSET_REPACK, nullptr,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 next_space);
        أرجع Err(LIMIT_EXCEEDED);
      }
      رقم_roots_for_space.push (0);
      TRY(graph_result_t<فراغ>::from (رقم_roots_for_space, ALLOCATION_FAILURE));

      لكل (unsigned جذر : connected_roots)
      {
        DEBUG_رسالة (SUBSET_REPACK, nullptr, "Subgraph %u gets space %u", جذر, next_space);
        vertices_[جذر].space = next_space;
        رقم_roots_for_space[next_space] = رقم_roots_for_space[next_space] + 1;
        distance_invalid = true;
        positions_invalid = true;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    أرجع Ok(true);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  graph_result_t<منطقي> isolate_subgraph (hb_set_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_set_t parents;
    لكل (unsigned جذر_idx : roots)
    {
      إذا (unlikely (جذر_idx >= vertices_.length))
        أرجع Err(OUT_OF_BOUNDS);
      subgraph.set (جذر_idx, wide_parents (جذر_idx, parents));
    }
    لكل (unsigned جذر_idx : roots)
    {
      TRY (find_subgraph (جذر_idx, subgraph));
    }
    TRY (graph_result_t<فراغ>::from (subgraph, ALLOCATION_FAILURE));

    hb_map_t index_map;
    منطقي made_changes = خطأ;
    لكل (auto entry : subgraph.iter ())
    {
      أكد (entry.first < vertices_.length);
      ثابت auto& node = vertices_[entry.first];
      unsigned subgraph_incoming_edges = entry.second;

      إذا (subgraph_incoming_edges < node.incoming_edges ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = true;
        TRY (duplicate_subgraph (entry.first, index_map));
      }
    }

    TRY (graph_result_t<فراغ>::from (index_map, ALLOCATION_FAILURE));

    إذا (!made_changes)
      أرجع Ok(خطأ);

    auto new_subgraph =
        + subgraph.مفاتيح ()
        | hb_map([&] (uint32_t عقدة_idx) {
          ثابت uint32_t *v;
          إذا (index_map.has (عقدة_idx, &v)) أرجع *v;
          أرجع عقدة_idx;
        })
        ;

    TRY(remap_obj_indices (index_map, new_subgraph));
    TRY(remap_obj_indices (index_map, parents.iter (), true));

    // Update roots set with new indices as needed.
    لكل (auto next : roots)
    {
      ثابت uint32_t *v;
      إذا (index_map.has (next, &v))
      {
        roots.del (next);
        roots.add (*v);
      }
    }

    أرجع Ok(true);
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
  graph_result_t<فراغ> traverse_directed_bfs (unsigned start_idx, VisitEdgeFunc&& visit_edge)
  {
    إذا (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_رسالة (SUBSET_REPACK, nullptr,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      أرجع Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<فراغ>::from (ordering_scratch_, ALLOCATION_FAILURE));

    إذا (!visit_edge (HB_CODEPOINT_INVALID, nullptr, start_idx, 0))
      أرجع Ok();

    unsigned head = 0;
    unsigned tail = 0;
    auto& queue = ordering_scratch_;
    queue[tail++] = start_idx;

    unsigned depth = 0;
    طالما (head < tail)
    {
      unsigned level_end = tail;
      طالما (head < level_end)
      {
        unsigned عقدة_idx = queue[head++];
        ثابت auto& v = vertices_[عقدة_idx];

        unsigned رقم_real = v.obj().real_links.length;
        unsigned total_links = رقم_real + v.obj().virtual_links.length;

        لكل (unsigned i = 0; i < total_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          ثابت auto& v = vertices_[عقدة_idx];
          ثابت auto& link = (i < رقم_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - رقم_real];
          unsigned child_idx = link.objidx;

          إذا (!visit_edge (عقدة_idx, &link, child_idx, depth + 1))
            استمر;

          إذا (unlikely (tail >= queue.length))
            أرجع Err(CYCLE_DETECTED);

          queue[tail++] = child_idx;
        }
      }
      depth++;
    }
    أرجع Ok();
  }

  graph_result_t<فراغ> find_subgraph (unsigned عقدة_idx, hb_map_t& subgraph)
  {
    أرجع traverse_directed_bfs (عقدة_idx, [&] (
      unsigned parent,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      إذا (depth == 0) أرجع true;
      hb_codepoint_t *عدد;
      إذا (subgraph.has (child, &عدد))
      {
        (*عدد)++;
        أرجع خطأ;
      }
      subgraph.set (child, 1);
      أرجع true;
    });
  }

  graph_result_t<فراغ> find_subgraph (unsigned عقدة_idx, hb_set_t& subgraph)
  {
    أرجع traverse_directed_bfs (عقدة_idx, [&] (
      unsigned parent,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      إذا (subgraph.has (child)) أرجع خطأ;
      subgraph.add (child);
      أرجع true;
    });
  }

  graph_result_t<size_t> find_subgraph_size (unsigned عقدة_idx, hb_set_t& subgraph, unsigned max_depth = HB_GRAPH_INVALID)
  {
    size_t size = 0;
    TRY (traverse_directed_bfs (عقدة_idx, [&] (
      unsigned parent,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      إذا (subgraph.has (child)) أرجع خطأ;
      subgraph.add (child);

      ثابت auto& o = vertices_[child].obj();
      size += o.tail - o.head;
      أرجع depth < max_depth;
    }));
    أرجع Ok(size);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  graph_result_t<فراغ> find_32bit_roots (unsigned عقدة_idx, hb_set_t& found)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_set_t visited;
    أرجع traverse_directed_bfs (عقدة_idx, [&] (
      unsigned parent,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {

      إذا (link && found.has(parent))
        // Don't traverse from something that's already marked as a root.
        أرجع خطأ;

      إذا (link && !link->is_signed && link->width == 4)
      {
        found.add (link->objidx);
        visited.add (link->objidx);
        أرجع خطأ;
      }

      إذا (visited.has (child)) أرجع خطأ;
      visited.add (child);
      أرجع true;
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
                                       ثابت O* old_offset,
                                       unsigned new_parent_idx,
                                       ثابت O* new_offset)
  {
    distance_invalid = true;
    positions_invalid = true;

    إذا (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      أرجع Err(OUT_OF_BOUNDS);

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    TRY_ASSIGN (unsigned child_id, index_for_offset (old_parent_idx,
                                                     old_offset));
    إذا (unlikely (child_id >= vertices_.length))
      أرجع Err(OUT_OF_BOUNDS);

    TRY(new_v.add_real_link(O::static_size, child_id, (ثابت حرف*) new_offset - (ثابت حرف*) new_v.obj().head));

    auto& child = vertices_[child_id];
    TRY(child.add_parent (new_parent_idx, خطأ));

    old_v.remove_real_link_unordered (child_id, old_offset);
    child.remove_parent (old_parent_idx);

    أرجع Ok(child_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  template<typename O>
  graph_result_t<فراغ> move_children (unsigned old_parent_idx,
                                      unsigned old_pos_start,
                                      unsigned old_pos_end,
                                      unsigned new_parent_idx,
                                      unsigned new_pos_start)
  {
    إذا (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      أرجع Err(OUT_OF_BOUNDS);

    distance_invalid = true;
    positions_invalid = true;

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    hb_vector_t<hb_serialize_context_t::object_t::link_t> old_links;
    لكل (ثابت auto& l : old_v.obj().real_links)
    {
      إذا (l.position < old_pos_start || l.position >= old_pos_end)
      {
        old_links.push (l);
        استمر;
      }

      unsigned array_pos = l.position - old_pos_start;

      unsigned child_id = l.objidx;
      إذا (unlikely (child_id >= vertices_.length))
        أرجع Err(OUT_OF_BOUNDS);

      TRY(new_v.add_real_link(O::static_size, child_id, new_pos_start + array_pos));

      auto& child = vertices_[child_id];
      TRY(child.add_parent (new_parent_idx, خطأ));
      child.remove_parent (old_parent_idx);
    }

    TRY (graph_result_t<فراغ>::from (old_links, ALLOCATION_FAILURE));
    أرجع old_v.set_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  graph_result_t<فراغ> duplicate_subgraph (unsigned عقدة_idx, hb_map_t& index_map)
  {
    graph_result_t<فراغ> result = Ok();
    TRY (traverse_directed_bfs (عقدة_idx, [&] (
      unsigned parent,
      ثابت hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {
      إذا (result.is_err () || index_map.has (child)) أرجع خطأ;
      auto r = duplicate (child);
      إذا (unlikely (r.is_err ()))
      {
        result = Err(r.error ());
        أرجع خطأ;
      }
      index_map.set (child, *r);
      أرجع true;
    }));

    أرجع graph_result_t<فراغ>::from(index_map, ALLOCATION_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  graph_result_t<unsigned> duplicate (unsigned عقدة_idx, منطقي copy_table = خطأ)
  {
    إذا (unlikely (عقدة_idx >= vertices_.length))
      أرجع Err(OUT_OF_BOUNDS);

    إذا (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_رسالة (SUBSET_REPACK, nullptr,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      أرجع Err(LIMIT_EXCEEDED);
    }

    positions_invalid = true;
    distance_invalid = true;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<فراغ>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<فراغ>::from (ordering_, ALLOCATION_FAILURE));

    auto& child = vertices_[عقدة_idx];

    unsigned table_size = child.obj().tail - child.obj().head;
    إذا (copy_table && table_size)
    {
      حرف* buffer = (حرف*) hb_malloc (table_size);
      إذا (unlikely (!buffer))
        أرجع Err(ALLOCATION_FAILURE);
      auto res = add_buffer (buffer);
      إذا (unlikely (res.is_err ()))
      {
        hb_free (buffer);
        أرجع Err(res.error ());
      }
      hb_memcpy (buffer, child.obj().head, table_size);
      clone->set_buffer(buffer, buffer + table_size);
    }
    وإلا
    {
      clone->set_buffer(child.obj().head, child.obj().tail);
    }
    clone->distance = child.distance;
    clone->space = child.space;
    clone->reset_parents ();

    لكل (ثابت auto& l : child.obj().real_links)
    {
      TRY(clone->add_real_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, خطأ));
    }

    لكل (ثابت auto& l : child.obj().virtual_links)
    {
      TRY(clone->add_virtual_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, true));
    }

    أرجع Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (unsigned parent_idx, unsigned child_idx, منطقي copy_table = خطأ)
  {
    إذا (unlikely (parent_idx >= vertices_.length || child_idx >= vertices_.length))
      أرجع Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    ثابت auto& child = vertices_[child_idx];
    unsigned links_to_child = child.incoming_edges_from_parent (parent_idx);

    إذا (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_رسالة (SUBSET_REPACK, nullptr, "  Not duplicating %u => %u",
                 parent_idx, child_idx);
      أرجع Err(INVALID_ARGUMENT);
    }

    DEBUG_رسالة (SUBSET_REPACK, nullptr, "  Duplicating %u => %u",
               parent_idx, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx, copy_table));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    إذا (parent_idx == clone_idx) parent_idx++;

    auto& parent = vertices_[parent_idx];
    unsigned عدد = 0;
    unsigned رقم_real = parent.obj().real_links.length;
    لكل (auto& l : parent.all_links_writer ())
    {
      عدد++;
      إذا (l.objidx != child_idx)
        استمر;

      TRY(reassign_link (l, parent_idx, clone_idx, عدد > رقم_real));
    }

    أرجع Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (ثابت hb_set_t* parents, unsigned child_idx)
  {
    إذا (unlikely (!parents || parents->is_empty ()))
      أرجع Err(INVALID_ARGUMENT);

    إذا (unlikely (child_idx >= vertices_.length))
      أرجع Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    ثابت auto& child = vertices_[child_idx];
    unsigned links_to_child = 0;
    unsigned last_parent = parents->get_max ();
    unsigned first_parent = parents->get_min ();
    لكل (unsigned parent_idx : *parents) {
      إذا (unlikely (parent_idx >= vertices_.length))
        أرجع Err(OUT_OF_BOUNDS);
      links_to_child += child.incoming_edges_from_parent (parent_idx);
    }

    إذا (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_رسالة (SUBSET_REPACK, nullptr, "  Not duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);
      أرجع Err(INVALID_ARGUMENT);
    }

    DEBUG_رسالة (SUBSET_REPACK, nullptr, "  Duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx));

    لكل (unsigned parent_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      إذا (parent_idx == clone_idx) parent_idx++;
      auto& parent = vertices_[parent_idx];
      unsigned عدد = 0;
      unsigned رقم_real = parent.obj().real_links.length;
      لكل (auto& l : parent.all_links_writer ())
      {
        عدد++;
        إذا (l.objidx != child_idx)
          استمر;

        TRY(reassign_link (l, parent_idx, clone_idx, عدد > رقم_real));
      }
    }

    أرجع Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  graph_result_t<unsigned> new_عقدة (حرف* head, حرف* tail)
  {
    إذا (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_رسالة (SUBSET_REPACK, nullptr,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      أرجع Err(LIMIT_EXCEEDED);
    }

    positions_invalid = true;
    distance_invalid = true;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<فراغ>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<فراغ>::from (ordering_, ALLOCATION_FAILURE));

    clone->set_buffer (head, tail);
    clone->distance = 0;
    clone->space = 0;

    أرجع Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  graph_result_t<unsigned> remap_child (unsigned parent_idx, unsigned old_child_idx)
  {
    إذا (unlikely (parent_idx >= vertices_.length || old_child_idx >= vertices_.length))
      أرجع Err(OUT_OF_BOUNDS);

    TRY_ASSIGN (unsigned new_child_idx, duplicate (old_child_idx));

    auto& parent = vertices_[parent_idx];
    لكل (auto& l : parent.real_links_writer ())
    {
      إذا (l.objidx != old_child_idx)
        استمر;
      TRY(reassign_link (l, parent_idx, new_child_idx, خطأ));
    }

    لكل (auto& l : parent.virtual_links_writer ())
    {
      إذا (l.objidx != old_child_idx)
        استمر;
      TRY(reassign_link (l, parent_idx, new_child_idx, true));
    }
    أرجع Ok(new_child_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  graph_result_t<منطقي> raise_childrens_priority (unsigned parent_idx)
  {
    DEBUG_رسالة (SUBSET_REPACK, nullptr, "  Raising priority of all children of %u",
               parent_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    إذا (unlikely (parent_idx >= vertices_.length)) أرجع Err(OUT_OF_BOUNDS);
    منطقي made_change = خطأ;
    لكل (auto& l : vertices_[parent_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_priority ();
    أرجع Ok(made_change);
  }

  graph_result_t<فراغ> is_fully_connected ()
  {
    TRY(update_parents());

    إذا (unlikely (جذر().incoming_edges ()))
      // Root cannot have parents.
      أرجع Err(ORPHANED_NODES);

    لكل (unsigned i = 0; i < جذر_idx (); i++)
    {
      إذا (unlikely (!vertices_[i].incoming_edges ()))
        أرجع Err(ORPHANED_NODES);
    }
    أرجع Ok();
  }

#إذا 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  فراغ save_fuzzer_seed (hb_tag_t tag) ثابت
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((فراغ*) &tag, حجم (tag), 1, f);

    uint16_t رقم_objects = vertices_.length;
    fwrite ((فراغ*) &رقم_objects, حجم (رقم_objects), 1, f);

    لكل (ثابت auto& v : vertices_)
    {
      uint16_t blob_size = v.table_size ();
      fwrite ((فراغ*) &blob_size, حجم (blob_size), 1, f);
      fwrite ((ثابت فراغ*) v.obj.head, blob_size, 1, f);
    }

    uint16_t link_عدد = 0;
    لكل (ثابت auto& v : vertices_)
      link_عدد += v.obj.real_links.length;

    fwrite ((فراغ*) &link_عدد, حجم (link_عدد), 1, f);

    تعريفنوع بنية
    {
      uint16_t parent;
      uint16_t child;
      uint16_t position;
      uint8_t width;
    } link_t;

    لكل (unsigned i = 0; i < vertices_.length; i++)
    {
      لكل (ثابت auto& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.width
        };
        fwrite ((فراغ*) &link, حجم (link), 1, f);
      }
    }

    fclose (f);
  }
#endif

  unsigned رقم_roots_for_space_2 (unsigned space) ثابت
  {
    أرجع رقم_roots_for_space[space];
  }

  unsigned next_space () ثابت
  {
    أرجع رقم_roots_for_space.length;
  }

  graph_result_t<فراغ> move_to_new_space (ثابت hb_set_t& indices)
  {
    إذا (unlikely (رقم_roots_for_space.length >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_رسالة (SUBSET_REPACK, nullptr,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 رقم_roots_for_space.length);
      أرجع Err(LIMIT_EXCEEDED);
    }
    رقم_roots_for_space.push (0);
    TRY (graph_result_t<فراغ>::from (رقم_roots_for_space, ALLOCATION_FAILURE));
    unsigned new_space = رقم_roots_for_space.length - 1;

    لكل (unsigned index : indices) {
      إذا (unlikely (index >= vertices_.length))
        أرجع Err(OUT_OF_BOUNDS);
      auto& node = vertices_[index];
      رقم_roots_for_space[node.space] = رقم_roots_for_space[node.space] - 1;
      رقم_roots_for_space[new_space] = رقم_roots_for_space[new_space] + 1;
      node.space = new_space;
      distance_invalid = true;
      positions_invalid = true;
    }
    أرجع Ok();
  }

  unsigned space_for (unsigned index, unsigned* جذر = nullptr) ثابت
  {
    طالما (true)
    {
      أكد (index < vertices_.length);
      ثابت auto& node = vertices_[index];
      إذا (node.space)
      {
        إذا (جذر != nullptr)
          *جذر = index;
        أرجع node.space;
      }

      إذا (!node.incoming_edges ())
      {
        إذا (جذر)
          *جذر = index;
        أرجع 0;
      }

      index = *node.parents_iter ();
    }
  }

  size_t total_size_in_bytes () ثابت {
    size_t total_size = 0;
    unsigned عدد = vertices_.length;
    لكل (unsigned i = 0; i < عدد; i++) {
      size_t size = vertices_.arrayZ[i].table_size ();
      total_size += size;
    }
    أرجع total_size;
  }


 خاص:

  فراغ اطبع_orphaned_nodes ()
  {
    إذا (!DEBUG_ENABLED(SUBSET_REPACK)) أرجع;

    DEBUG_رسالة (SUBSET_REPACK, nullptr, "Graph is not fully connected.");

    parents_invalid = true;
    (فراغ) update_parents();

    إذا (جذر().incoming_edges ()) {
      DEBUG_رسالة (SUBSET_REPACK, nullptr, "Root node has incoming edges.");
    }

    لكل (unsigned i = 0; i < vertices_.length; i++)
    {
      ثابت auto& v = vertices_[i];
      إذا (!v.incoming_edges ())
        DEBUG_رسالة (SUBSET_REPACK, nullptr, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  unsigned wide_parents (unsigned عقدة_idx, hb_set_t& parents) ثابت
  {
    unsigned عدد = 0;
    لكل (unsigned p : vertices_[عقدة_idx].parents_iter ())
    {
      // Only real links can be wide
      لكل (ثابت auto& l : vertices_[p].obj().real_links)
      {
        إذا (l.objidx == عقدة_idx
            && (l.width == 3 || l.width == 4)
            && !l.is_signed)
        {
          عدد++;
          parents.add (p);
        }
      }
    }
    أرجع عدد;
  }

 عام:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  graph_result_t<فراغ> update_parents ()
  {
    إذا (!parents_invalid) أرجع Ok();

    unsigned عدد = vertices_.length;

    لكل (unsigned i = 0; i < عدد; i++)
      vertices_.arrayZ[i].reset_parents ();

    لكل (unsigned p = 0; p < عدد; p++)
    {
      لكل (ثابت auto& l : vertices_.arrayZ[p].obj().real_links)
      {
        إذا (unlikely (l.objidx >= عدد)) أرجع Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, خطأ));
      }

      لكل (ثابت auto& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        إذا (unlikely (l.objidx >= عدد)) أرجع Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, true));
      }
    }

    parents_invalid = خطأ;
    أرجع Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  فراغ update_positions ()
  {
    إذا (!positions_invalid) أرجع;

    unsigned current_pos = 0;
    لكل (unsigned i : ordering_)
    {
      auto& v = vertices_[i];
      v.start = current_pos;
      current_pos += v.obj().tail - v.obj().head;
      v.end = current_pos;
    }

    positions_invalid = خطأ;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  graph_result_t<فراغ> update_distances ()
  {
    إذا (!distance_invalid) أرجع Ok();

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
    unsigned عدد = vertices_.length;
    لكل (unsigned i = 0; i < عدد; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[جذر_idx ()].distance = 0;

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (عدد);
    queue.insert (0, جذر_idx ());
    TRY (graph_result_t<فراغ>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<منطقي> visited;
    visited.resize (vertices_.length);
    TRY (graph_result_t<فراغ>::from (visited, ALLOCATION_FAILURE));

    طالما (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_idx = queue.اسحب_minimum ().second;
      إذا (visited[next_idx]) استمر;
      ثابت auto& next = vertices_[next_idx];
      int64_t next_distance = next.distance;
      visited[next_idx] = true;

      لكل (ثابت auto& link : next.obj().all_links ())
      {
        إذا (unlikely (link.objidx >= vertices_.length))
          أرجع Err(OUT_OF_BOUNDS);

        إذا (visited[link.objidx]) استمر;

        auto& child_v = vertices_.arrayZ[link.objidx];
        ثابت auto& child = child_v.obj ();
        unsigned link_width = link.width ? link.width : 4; // treat virtual offsets as 32 bits wide
        int64_t child_weight = (child.tail - child.head) +
                               ((int64_t) 1 << (link_width * 8)) * (child_v.space + 1);
        int64_t child_distance = next_distance + child_weight;

        إذا (child_distance < child_v.distance)
        {
          child_v.distance = child_distance;
          queue.insert (child_distance, link.objidx);
        }
      }
    }

    TRY (graph_result_t<فراغ>::from (queue, ALLOCATION_FAILURE));
    إذا (unlikely (!queue.is_empty ()))
    {
      اطبع_orphaned_nodes ();
      أرجع Err(ORPHANED_NODES);
    }

    distance_invalid = خطأ;
    أرجع Ok();
  }

 خاص:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  graph_result_t<فراغ> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      unsigned parent_idx,
                      unsigned new_idx,
                      منطقي is_virtual)
  {
    unsigned old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_parent (parent_idx);
    أرجع vertices_[new_idx].add_parent (parent_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  template<typename Iterator, hb_requires (hb_is_iterator (Iterator))>
  graph_result_t<فراغ> remap_obj_indices (ثابت hb_map_t& id_map,
                                          Iterator subgraph,
                                          منطقي only_wide = خطأ)
  {
    إذا (!id_map) أرجع Ok();
    لكل (unsigned i : subgraph)
    {
      auto& vertex = vertices_[i];
      unsigned رقم_real = vertex.obj().real_links.length;
      unsigned عدد = 0;
      لكل (auto& link : vertex.all_links_writer ())
      {
        عدد++;
        ثابت uint32_t *v;
        إذا (!id_map.has (link.objidx, &v)) استمر;
        إذا (only_wide && (link.is_signed || (link.width != 4 && link.width != 3))) استمر;

        TRY(reassign_link (link, i, *v, عدد > رقم_real));
      }
    }
    أرجع Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  graph_result_t<فراغ> find_connected_nodes (unsigned start_idx,
                                             hb_set_t& targets,
                                             hb_set_t& visited,
                                             hb_set_t& connected)
  {
    TRY (graph_result_t<فراغ>::from (visited, ALLOCATION_FAILURE));
    إذا (visited.has (start_idx)) أرجع Ok();
    إذا (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_رسالة (SUBSET_REPACK, nullptr,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      أرجع Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<فراغ>::from (ordering_scratch_, ALLOCATION_FAILURE));

    auto& مكدس = ordering_scratch_;
    unsigned مكدس_len = 0;

    graph_result_t<فراغ> res = Ok();
    auto handle_عقدة = [&] (unsigned عقدة_idx) {
      visited.add (عقدة_idx);
      إذا (targets.has (عقدة_idx)) {
        targets.del (عقدة_idx);
        connected.add (عقدة_idx);
      }

      إذا (unlikely (مكدس_len >= مكدس.length))
      {
        res = Err(CYCLE_DETECTED);
        أرجع خطأ;
      }
      مكدس[مكدس_len++] = عقدة_idx;
      أرجع true;
    };

    إذا (!handle_عقدة (start_idx)) أرجع res;

    طالما (مكدس_len > 0)
    {
      unsigned عقدة_idx = مكدس[--مكدس_len];
      ثابت auto& v = vertices_[عقدة_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      لكل (ثابت auto& l : v.obj().all_links ())
      {
        unsigned child_idx = l.objidx;
        إذا (visited.has (child_idx)) استمر;
        إذا (!handle_عقدة (child_idx)) أرجع res;
      }

      لكل (unsigned parent_idx : v.parents_iter ())
      {
        إذا (visited.has (parent_idx)) استمر;
        إذا (!handle_عقدة (parent_idx)) أرجع res;
      }
    }

    TRY (graph_result_t<فراغ>::from (visited, ALLOCATION_FAILURE));
    TRY (graph_result_t<فراغ>::from (connected, ALLOCATION_FAILURE));

    أرجع Ok();
  }

 عام:
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

 خاص:
  منطقي parents_invalid = true;
  منطقي distance_invalid = true;
  منطقي positions_invalid = true;
  hb_vector_t<unsigned> رقم_roots_for_space;
  hb_vector_t<حرف*> buffers;
};

}

#endif  // GRAPH_GRAPH_HH
