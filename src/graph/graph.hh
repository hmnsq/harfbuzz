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

#अन्तर्भाव "../hb-set.hh"
#अन्तर्भाव "../hb-priority-queue.hh"
#अन्तर्भाव "../hb-serialize.hh"
#अन्तर्भाव "graph-result.hh"

#ifndef GRAPH_GRAPH_HH
#define GRAPH_GRAPH_HH

#define HB_GRAPH_INVALID ((unsigned) -1)

नामाकाश graph {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
संरचना graph_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  संरचना vertex_t
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

    गुप्त:
    hb_serialize_context_t::object_t obj_;
    unsigned incoming_edges_ = 0;
    unsigned single_parent = HB_GRAPH_INVALID;
    तार्किक has_incoming_virtual_edges_ = मिच्छ;
    hb_hashmap_t<unsigned, unsigned> parents;
    hb_set_t virtual_parents;
    सार्वजनिक:

    vertex_t ()
    {}

    explicit vertex_t(ध्रुव hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    auto parents_iter () ध्रुव HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_parent, single_parent != HB_GRAPH_INVALID),
	parents.कुञ्चिकाः_ref ()
      )
    )

    शून्य set_buffer (वर्ण* head, वर्ण* tail) {
      obj_.head = head;
      obj_.tail = tail;
    }

    शून्य shrink_buffer (size_t गणनम्)
    {
      obj_.tail -= गणनम्;
    }

    auto all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    auto real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    auto virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    शून्य sort_real_links () {
      obj_.real_links.qsort ();
    }

    graph_result_t<शून्य> add_real_link (unsigned width, unsigned obj_idx, unsigned position) {
      auto* link = obj_.real_links.push ();
      TRY (graph_result_t<शून्य>::from (obj_.real_links, ALLOCATION_FAILURE));
      link->width = width;
      link->objidx = obj_idx;
      link->position = position;
      प्रत्यागम Ok();
    }

    graph_result_t<शून्य> add_virtual_link (unsigned obj_idx) {
      auto* link = obj_.virtual_links.push ();
      TRY (graph_result_t<शून्य>::from (obj_.virtual_links, ALLOCATION_FAILURE));
      link->objidx = obj_idx;
      प्रत्यागम Ok();
    }

    graph_result_t<शून्य> add_real_link (ध्रुव hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      प्रत्यागम graph_result_t<शून्य>::from (obj_.real_links, ALLOCATION_FAILURE);
    }

    graph_result_t<शून्य> set_real_links (hb_vector_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(graph_result_t<शून्य>::from (links, ALLOCATION_FAILURE));
      obj_.real_links = links;
      प्रत्यागम Ok();
    }

    शून्य clear_real_links ()
    {
      obj_.real_links.reset();
    }

    शून्य clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    graph_result_t<शून्य> add_virtual_link (ध्रुव hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      प्रत्यागम graph_result_t<शून्य>::from (obj_.virtual_links, ALLOCATION_FAILURE);
    }

    ध्रुव hb_serialize_context_t::object_t& obj() ध्रुव { प्रत्यागम obj_; }

    template<typename T>
    T as () ध्रुव
    {
      प्रत्यागम reinterpret_cast<T> (obj_.head);
    }

    तार्किक has_incoming_virtual_edges () ध्रुव
    {
      प्रत्यागम has_incoming_virtual_edges_;
    }

    graph_result_t<शून्य> link_positions_valid (unsigned अङ्कः_objects, तार्किक removed_nil)
    {
      hb_set_t assigned_bytes;
      प्रति (ध्रुव auto& l : obj_.real_links)
      {
        जइ (unlikely (l.objidx >= अङ्कः_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid object index.");
          प्रत्यागम Err(INVALID_ARGUMENT);
        }

        unsigned start = l.position;
        unsigned end = start + l.width - 1;

        जइ (unlikely (l.width < 2 || l.width > 4))
        {
          DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid link width.");
          प्रत्यागम Err(INVALID_ARGUMENT);
        }

        जइ (unlikely (end >= table_size ()))
        {
          DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                     "Invalid graph. Link position is out of bounds.");
          प्रत्यागम Err(OUT_OF_BOUNDS);
        }

        जइ (unlikely (assigned_bytes.intersects (start, end)))
        {
          DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                     "Invalid graph. Found offsets whose positions overlap.");
          प्रत्यागम Err(INVALID_ARGUMENT);
        }

        assigned_bytes.add_range (start, end);
      }

      प्रत्यागम graph_result_t<शून्य>::from (assigned_bytes, ALLOCATION_FAILURE);
    }

    शून्य normalize ()
    {
      obj_.real_links.qsort ();
      प्रति (ध्रुव auto& l : obj_.real_links)
      {
        प्रति (unsigned i = 0; i < l.width; i++)
        {
          obj_.head[l.position + i] = 0;
        }
      }
    }

    तार्किक equals (unsigned this_index,
                 unsigned other_index,
                 ध्रुव vertex_t& other,
                 ध्रुव graph_t& graph,
                 ध्रुव graph_t& other_graph,
                 unsigned depth) ध्रुव
    {
      जइ (!(as_bytes () == other.as_bytes ()))
      {
        DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_index,
                   (unsigned long) table_size (),
                   other_index,
                   (unsigned long) other.table_size (),
                   depth);

        auto a = as_bytes ();
        auto b = other.as_bytes ();
        जाव (a || b)
        {
          DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                     "  0x%x %s 0x%x", (unsigned) *a, (*a == *b) ? "==" : "!=", (unsigned) *b);
          a++;
          b++;
        }
        प्रत्यागम मिच्छ;
      }

      प्रत्यागम links_equal (obj_.real_links, other.obj_.real_links, graph, other_graph, depth);
    }

    hb_bytes_t as_bytes () ध्रुव
    {
      प्रत्यागम hb_bytes_t (obj_.head, table_size ());
    }

    friend शून्य swap (vertex_t& a, vertex_t& b)
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
    position_to_index_map () ध्रुव
    {
      hb_hashmap_t<unsigned, unsigned> result;

      result.alloc (obj_.real_links.length);
      प्रति (ध्रुव auto& l : obj_.real_links) {
        result.set (l.position, l.objidx);
      }

      प्रत्यागम graph_result_t<hb_hashmap_t<unsigned, unsigned>>::from (std::move(result), ALLOCATION_FAILURE);
    }

    तार्किक is_shared () ध्रुव
    {
      प्रत्यागम parents.get_population () > virtual_parents.get_population () + 1;
    }

    unsigned incoming_edges () ध्रुव
    {
      जइ (HB_DEBUG_SUBSET_REPACK)
       {
	प्रतिज्ञा (incoming_edges_ == (single_parent != HB_GRAPH_INVALID) +
		(parents.मानानि_ref () | hb_reduce (hb_add, 0)));
       }
      प्रत्यागम incoming_edges_;
    }

    unsigned incoming_edges_from_parent (unsigned parent_index) ध्रुव {
      जइ (single_parent != HB_GRAPH_INVALID) {
        प्रत्यागम single_parent == parent_index ? 1 : 0;
      }

      unsigned* गणनम्;
      प्रत्यागम  parents.has(parent_index, &गणनम्) ? *गणनम् : 0;
    }

    शून्य reset_parents ()
    {
      incoming_edges_ = 0;
      has_incoming_virtual_edges_ = मिच्छ;
      single_parent = HB_GRAPH_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    graph_result_t<शून्य> add_parent (unsigned parent_index, तार्किक is_virtual)
    {
      प्रतिज्ञा (parent_index != HB_GRAPH_INVALID);
      has_incoming_virtual_edges_ |= is_virtual;
      जइ (is_virtual)
        virtual_parents.add (parent_index);

      जइ (incoming_edges_ == 0)
      {
	single_parent = parent_index;
	incoming_edges_ = 1;
	प्रत्यागम to_result();
      }
      अण्णहा जइ (single_parent != HB_GRAPH_INVALID)
      {
        प्रतिज्ञा (incoming_edges_ == 1);
	जइ (!parents.set (single_parent, 1))
	  प्रत्यागम to_result ();
	single_parent = HB_GRAPH_INVALID;
      }

      unsigned *v;
      जइ (parents.has (parent_index, &v))
      {
        (*v)++;
	incoming_edges_++;
      }
      अण्णहा जइ (parents.set (parent_index, 1))
	incoming_edges_++;

      प्रत्यागम to_result();
    }

    शून्य remove_parent (unsigned parent_index)
    {
      जइ (parent_index == single_parent)
      {
	single_parent = HB_GRAPH_INVALID;
	incoming_edges_--;
	virtual_parents.reset ();
	प्रत्यागम;
      }

      unsigned *v;
      जइ (parents.has (parent_index, &v))
      {
	incoming_edges_--;
	जइ (*v > 1)
	  (*v)--;
	अण्णहा
        {
	  parents.del (parent_index);
          virtual_parents.del (parent_index);
        }

	जइ (incoming_edges_ == 1)
	{
	  single_parent = *parents.कुञ्चिकाः ();
	  parents.reset ();
	}
      }
    }

    शून्य remove_real_link_unordered (unsigned link_array_index) {
      obj_.real_links.remove_unordered (link_array_index);
    }

    शून्य remove_real_link_unordered (unsigned child_index, ध्रुव शून्य* offset)
    {
      unsigned गणनम् = obj_.real_links.length;
      प्रति (unsigned i = 0; i < गणनम्; i++)
      {
        auto& link = obj_.real_links.arrayZ[i];
        जइ (link.objidx != child_index)
          अनुवर्त;

        जइ ((obj_.head + link.position) != offset)
          अनुवर्त;

        obj_.real_links.remove_unordered (i);
        प्रत्यागम;
      }
    }

    graph_result_t<शून्य> remap_parent (unsigned old_index, unsigned new_index)
    {
      जइ (single_parent != HB_GRAPH_INVALID)
      {
        जइ (single_parent == old_index)
        {
	  single_parent = new_index;
          जइ (virtual_parents.has (old_index))
          {
            virtual_parents.del (old_index);
            virtual_parents.add (new_index);
          }
        }
        प्रत्यागम graph_result_t<शून्य>::from(virtual_parents, ALLOCATION_FAILURE);
      }

      ध्रुव unsigned *pv;
      जइ (parents.has (old_index, &pv))
      {
        unsigned v = *pv;
	जइ (!parents.set (new_index, v))
          incoming_edges_ -= v;
	parents.del (old_index);

        जइ (incoming_edges_ == 1)
	{
	  single_parent = *parents.कुञ्चिकाः ();
	  parents.reset ();
	}
      }

      जइ (virtual_parents.has (old_index))
      {
        virtual_parents.del (old_index);
        virtual_parents.add (new_index);
      }

      प्रत्यागम graph_result_t<शून्य>::from(virtual_parents, ALLOCATION_FAILURE);
    }

    तार्किक is_leaf () ध्रुव
    {
      प्रत्यागम !obj_.real_links.length && !obj_.virtual_links.length;
    }

    तार्किक raise_priority ()
    {
      जइ (has_max_priority ()) प्रत्यागम मिच्छ;
      priority++;
      प्रत्यागम सच्च;
    }

    तार्किक give_max_priority ()
    {
      तार्किक result = मिच्छ;
      जाव (!has_max_priority()) {
        result = सच्च;
        priority++;
      }
      प्रत्यागम result;
    }

    तार्किक has_max_priority () ध्रुव {
      प्रत्यागम priority >= 3;
    }

    size_t table_size () ध्रुव {
      प्रत्यागम obj_.tail - obj_.head;
    }

    int64_t modified_distance (unsigned order) ध्रुव
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_distance =
          hb_clamp (distance + distance_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      जइ (has_max_priority ()) {
        modified_distance = 0;
      }
      प्रत्यागम (modified_distance << 18) | (0x003FFFF & order);
    }

    int64_t distance_modifier () ध्रुव
    {
      जइ (!priority) प्रत्यागम 0;
      int64_t table_size = obj_.tail - obj_.head;

      जइ (priority == 1)
        प्रत्यागम -table_size / 2;

      प्रत्यागम -table_size;
    }

   गुप्त:
    graph_result_t<शून्य> to_result() ध्रुव {
      जइ (unlikely (
        parents.in_error() || virtual_parents.in_error() ||
        obj_.real_links.in_error() || obj_.virtual_links.in_error()))
        प्रत्यागम Err(ALLOCATION_FAILURE);
      प्रत्यागम Ok();
    }

    तार्किक links_equal (ध्रुव hb_vector_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      ध्रुव hb_vector_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      ध्रुव graph_t& graph,
                      ध्रुव graph_t& other_graph,
                      unsigned depth) ध्रुव
    {
      auto a = this_links.iter ();
      auto b = other_links.iter ();

      जाव (a && b)
      {
        ध्रुव auto& link_a = *a;
        ध्रुव auto& link_b = *b;

        जइ (link_a.width != link_b.width ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          प्रत्यागम मिच्छ;

        जइ (!graph.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_graph.vertices_[link_b.objidx], graph, other_graph, depth + 1))
          प्रत्यागम मिच्छ;

        a++;
        b++;
      }

      जइ (तार्किक (a) != तार्किक (b))
        प्रत्यागम मिच्छ;

      प्रत्यागम सच्च;
    }
  };

  गणना vertex_mutability_t
  {
    Immutable,
    Mutable
  };

  template <typename T, vertex_mutability_t mutability = Mutable>
  संरचना vertex_and_table_t
  {
    vertex_and_table_t () : index (0), vertex (nullptr), table (nullptr)
    {}

    template <vertex_mutability_t other_mutability>
    vertex_and_table_t (ध्रुव vertex_and_table_t<T, other_mutability>& o)
      : index (o.index), vertex (o.vertex), table (o.table)
    {}

    unsigned index;
    typename std::conditional<mutability == Immutable, ध्रुव vertex_t*, vertex_t*>::type vertex;
    typename std::conditional<mutability == Immutable, ध्रुव T*, T*>::type table;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  template<typename T>
  स्थिर graph_result_t<graph_t> create (ध्रुव T& objects)
  {
    जइ (unlikely (objects.length > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.length);
      प्रत्यागम Err(LIMIT_EXCEEDED);
    }

    graph_t g;
    g.अङ्कः_roots_for_space.push (1);
    TRY (graph_result_t<शून्य>::from (g.अङ्कः_roots_for_space, ALLOCATION_FAILURE));

    तार्किक removed_nil = मिच्छ;
    ध्रुव unsigned गणनम् = objects.length;
    g.vertices_.alloc (गणनम्);
    g.ordering_.resize (गणनम्);
    g.ordering_scratch_.alloc (गणनम्);
    TRY (graph_result_t<शून्य>::from (g.vertices_, ALLOCATION_FAILURE));
    TRY (graph_result_t<शून्य>::from (g.ordering_, ALLOCATION_FAILURE));
    TRY (graph_result_t<शून्य>::from (g.ordering_scratch_, ALLOCATION_FAILURE));

    unsigned order = objects.length;
    unsigned skip = 0;
    प्रति (unsigned i = 0; i < गणनम्; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      जइ (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = सच्च;
        order--;
        g.ordering_.resize (गणनम् - 1);
        skip++;
        अनुवर्त;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      जइ (unlikely (obj.in_error ())) {
        प्रत्यागम Err(ALLOCATION_FAILURE);
      }
      vertex_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (गणनम्, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      unsigned obj_idx = i - skip;
      g.ordering_[--order] = obj_idx;

      जइ (!removed_nil) अनुवर्त;
      // Fix indices to account for removed nil object.
      प्रति (auto& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    auto r = g.is_fully_connected();
    जइ (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    प्रत्यागम Ok (std::move (g));
  }

  graph_t (ध्रुव graph_t&) = delete;
  graph_t& operator = (ध्रुव graph_t&) = delete;
  graph_t (graph_t&&) = स्वभाव;
  graph_t& operator = (graph_t&&) = स्वभाव;

  ~graph_t ()
  {
    प्रति (वर्ण* b : buffers)
      hb_free (b);
  }

गुप्त:
  graph_t () = स्वभाव;

सार्वजनिक:

  तार्किक operator== (ध्रुव graph_t& other) ध्रुव
  {
    प्रत्यागम मूलम् ().equals (मूलम्_idx(), other.मूलम्_idx(), other.मूलम् (), *एतत्, other, 0);
  }

  शून्य print () ध्रुव {
    प्रति (unsigned id : ordering_)
    {
      ध्रुव auto& v = vertices_[id];
      printf("%u: %u [", id, (unsigned पूर्णाङ्क)v.table_size());
      प्रति (ध्रुव auto &l : v.obj().real_links) {
        printf("%u, ", l.objidx);
      }
      प्रति (ध्रुव auto &l : v.obj().virtual_links) {
        printf("v%u, ", l.objidx);
      }
      printf("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  शून्य normalize ()
  {
    प्रति (auto& v : vertices_.writer ())
      v.normalize ();
  }

  ध्रुव vertex_t& मूलम् () ध्रुव
  {
    प्रत्यागम vertices_[मूलम्_idx ()];
  }

  unsigned मूलम्_idx () ध्रुव
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    प्रत्यागम ordering_[0];
  }

  ध्रुव hb_serialize_context_t::object_t& object (unsigned i) ध्रुव
  {
    प्रत्यागम vertices_[i].obj();
  }

  graph_result_t<शून्य> add_buffer (वर्ण* buffer)
  {
    buffers.push (buffer);
    प्रत्यागम graph_result_t<शून्य>::from (buffers, ALLOCATION_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  template<typename T>
  graph_result_t<शून्य> add_link (T* offset,
                                 unsigned parent_id,
                                 unsigned child_id)
  {
    जइ (unlikely (parent_id >= vertices_.length || child_id >= vertices_.length))
      प्रत्यागम Err(OUT_OF_BOUNDS);
    auto& v = vertices_[parent_id];
    TRY(v.add_real_link(2, child_id, (वर्ण*) offset - (वर्ण*) v.obj().head));
    प्रत्यागम vertices_[child_id].add_parent (parent_id, मिच्छ);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  graph_result_t<शून्य> sort_shortest_distance_if_needed ()
  {
    जइ (!positions_invalid) प्रत्यागम Ok();
    प्रत्यागम sort_shortest_distance ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  graph_result_t<शून्य> sort_shortest_distance ()
  {
    positions_invalid = सच्च;

    जइ (vertices_.length <= 1) {
      // Graph of 1 or less doesn't need sorting.
      प्रत्यागम Ok();
    }

    TRY (update_distances ());

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (vertices_.length);
    TRY (graph_result_t<शून्य>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.length);
    TRY (graph_result_t<शून्य>::from (new_ordering, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> removed_edges;
    removed_edges.resize (vertices_.length);
    TRY (graph_result_t<शून्य>::from (removed_edges, ALLOCATION_FAILURE));

    TRY (update_parents ());

    queue.insert (मूलम् ().modified_distance (0), मूलम्_idx ());
    unsigned order = 1;
    unsigned pos = 0;
    जाव (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_id = queue.निष्कासय_minimum().second;

      जइ (unlikely (pos >= new_ordering.length)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "Invalid graph. Contains cycle.");
        प्रत्यागम Err(CYCLE_DETECTED);
      }
      new_ordering[pos++] = next_id;
      ध्रुव vertex_t& next = vertices_[next_id];

      प्रति (ध्रुव auto& link : next.obj().all_links ()) {
        जइ (unlikely (link.objidx >= vertices_.length))
          प्रत्यागम Err(OUT_OF_BOUNDS);
        removed_edges[link.objidx]++;
        ध्रुव auto& v = vertices_[link.objidx];
        जइ (!(v.incoming_edges () - removed_edges[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          queue.insert (v.modified_distance (order++),
                        link.objidx);
      }
    }

    TRY (graph_result_t<शून्य>::from (queue, ALLOCATION_FAILURE));
    TRY (graph_result_t<शून्य>::from (new_ordering, ALLOCATION_FAILURE));

    hb_swap (ordering_, new_ordering);

    जइ (unlikely (pos != vertices_.length)) {
      print_orphaned_nodes ();
      प्रत्यागम Err(ORPHANED_NODES);
    }

    प्रत्यागम Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  graph_result_t<शून्य> find_space_roots (hb_set_t& visited, hb_set_t& roots)
  {
    unsigned मूलम्_index = मूलम्_idx ();
    प्रति (unsigned i : ordering_)
    {
      जइ (visited.has (i)) अनुवर्त;

      // Only real links can form 32 bit spaces
      प्रति (ध्रुव auto& l : vertices_[i].obj().real_links)
      {
        जइ (l.is_signed || l.width < 3)
          अनुवर्त;

        जइ (i == मूलम्_index && l.width == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          अनुवर्त;

        जइ (l.width >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_set_t sub_roots;
          TRY (find_32bit_roots (l.objidx, sub_roots));
          जइ (sub_roots) {
            प्रति (unsigned sub_मूलम्_idx : sub_roots) {
              roots.add (sub_मूलम्_idx);
              TRY (find_subgraph (sub_मूलम्_idx, visited));
            }
            अनुवर्त;
          }
        }

        roots.add (l.objidx);
        TRY (find_subgraph (l.objidx, visited));
      }
    }

    TRY(graph_result_t<शून्य>::from (visited, ALLOCATION_FAILURE));
    TRY(graph_result_t<शून्य>::from (roots, ALLOCATION_FAILURE));
    प्रत्यागम Ok();
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Immutable>> as_table (unsigned parent, ध्रुव शून्य* offset, Ts... ds)
  {
    TRY_ASSIGN (unsigned idx, index_for_offset (parent, offset));
    प्रत्यागम as_table_from_index<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Mutable>> as_mutable_table (unsigned parent, ध्रुव शून्य* offset, Ts... ds)
  {
    TRY_ASSIGN (auto idx, mutable_index_for_offset (parent, offset));
    प्रत्यागम as_table_from_index<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, vertex_mutability_t mutability = Mutable, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, mutability>> as_table_from_index (unsigned index, Ts... ds)
  {
    जइ (unlikely (index >= vertices_.length))
      प्रत्यागम Err(OUT_OF_BOUNDS);

    vertex_and_table_t<T, mutability> r;
    r.vertex = (typename std::conditional<mutability == Immutable, ध्रुव vertex_t*, vertex_t*>::type) &vertices_[index];
    r.table = r.vertex->template as<typename std::conditional<mutability == Immutable, ध्रुव T*, T*>::type> ();
    r.index = index;
    जइ (unlikely (!r.table))
      प्रत्यागम Err(INVALID_ARGUMENT);

    TRY(r.table->sanitize (*(r.vertex), std::forward<Ts>(ds)...));
    प्रत्यागम r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  graph_result_t<unsigned> index_for_offset (unsigned ग्रन्थिः_idx, ध्रुव शून्य* offset) ध्रुव
  {
    जइ (unlikely (ग्रन्थिः_idx >= vertices_.length)) प्रत्यागम Err(OUT_OF_BOUNDS);
    ध्रुव auto& node = object (ग्रन्थिः_idx);
    जइ (unlikely (offset < node.head || offset >= node.tail)) प्रत्यागम Err(OUT_OF_BOUNDS);

    unsigned गणनम् = node.real_links.length;
    प्रति (unsigned i = 0; i < गणनम्; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      ध्रुव auto& link = node.real_links.arrayZ[i];
      जइ (offset != node.head + link.position)
        अनुवर्त;
      प्रत्यागम Ok(link.objidx);
    }

    प्रत्यागम Err(INVALID_ARGUMENT);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  graph_result_t<unsigned> mutable_index_for_offset (unsigned ग्रन्थिः_idx, ध्रुव शून्य* offset)
  {
    TRY_ASSIGN (unsigned child_idx, index_for_offset (ग्रन्थिः_idx, offset));
    जइ (unlikely (child_idx >= vertices_.length))
      प्रत्यागम Err(OUT_OF_BOUNDS);

    auto& child = vertices_[child_idx];
    प्रति (unsigned p : child.parents_iter ())
    {
      जइ (p != ग्रन्थिः_idx) {
        प्रत्यागम duplicate (ग्रन्थिः_idx, child_idx, सच्च);
      }
    }

    प्रत्यागम Ok(child_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  graph_result_t<तार्किक> assign_spaces ()
  {
    TRY (update_parents ());

    प्रति (auto& vertex : vertices_.writer ())
    {
      vertex.space = 0;
      vertex.priority = 0;
    }
    अङ्कः_roots_for_space.resize (1);
    TRY (graph_result_t<शून्य>::from (अङ्कः_roots_for_space, ALLOCATION_FAILURE));
    अङ्कः_roots_for_space[0] = 1;
    distance_invalid = सच्च;
    positions_invalid = सच्च;

    hb_set_t visited;
    hb_set_t roots;
    TRY (find_space_roots (visited, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    visited.invert ();

    जइ (!roots) प्रत्यागम Ok(मिच्छ);

    जाव (roots)
    {
      uint32_t next = HB_SET_मानम्_INVALID;
      TRY (graph_result_t<शून्य>::from (roots, ALLOCATION_FAILURE));
      जइ (!roots.next (&next)) भङ्ग;

      hb_set_t connected_roots;
      TRY (find_connected_nodes (next, roots, visited, connected_roots));
      TRY (graph_result_t<शून्य>::from (connected_roots, ALLOCATION_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (graph_result_t<शून्य>::from (connected_roots, ALLOCATION_FAILURE));

      unsigned next_space = एतत्->next_space ();
      जइ (unlikely (next_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 next_space);
        प्रत्यागम Err(LIMIT_EXCEEDED);
      }
      अङ्कः_roots_for_space.push (0);
      TRY(graph_result_t<शून्य>::from (अङ्कः_roots_for_space, ALLOCATION_FAILURE));

      प्रति (unsigned मूलम् : connected_roots)
      {
        DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "Subgraph %u gets space %u", मूलम्, next_space);
        vertices_[मूलम्].space = next_space;
        अङ्कः_roots_for_space[next_space] = अङ्कः_roots_for_space[next_space] + 1;
        distance_invalid = सच्च;
        positions_invalid = सच्च;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    प्रत्यागम Ok(सच्च);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  graph_result_t<तार्किक> isolate_subgraph (hb_set_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_set_t parents;
    प्रति (unsigned मूलम्_idx : roots)
    {
      जइ (unlikely (मूलम्_idx >= vertices_.length))
        प्रत्यागम Err(OUT_OF_BOUNDS);
      subgraph.set (मूलम्_idx, wide_parents (मूलम्_idx, parents));
    }
    प्रति (unsigned मूलम्_idx : roots)
    {
      TRY (find_subgraph (मूलम्_idx, subgraph));
    }
    TRY (graph_result_t<शून्य>::from (subgraph, ALLOCATION_FAILURE));

    hb_map_t index_map;
    तार्किक made_changes = मिच्छ;
    प्रति (auto entry : subgraph.iter ())
    {
      प्रतिज्ञा (entry.first < vertices_.length);
      ध्रुव auto& node = vertices_[entry.first];
      unsigned subgraph_incoming_edges = entry.second;

      जइ (subgraph_incoming_edges < node.incoming_edges ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = सच्च;
        TRY (duplicate_subgraph (entry.first, index_map));
      }
    }

    TRY (graph_result_t<शून्य>::from (index_map, ALLOCATION_FAILURE));

    जइ (!made_changes)
      प्रत्यागम Ok(मिच्छ);

    auto new_subgraph =
        + subgraph.कुञ्चिकाः ()
        | hb_map([&] (uint32_t ग्रन्थिः_idx) {
          ध्रुव uint32_t *v;
          जइ (index_map.has (ग्रन्थिः_idx, &v)) प्रत्यागम *v;
          प्रत्यागम ग्रन्थिः_idx;
        })
        ;

    TRY(remap_obj_indices (index_map, new_subgraph));
    TRY(remap_obj_indices (index_map, parents.iter (), सच्च));

    // Update roots set with new indices as needed.
    प्रति (auto next : roots)
    {
      ध्रुव uint32_t *v;
      जइ (index_map.has (next, &v))
      {
        roots.del (next);
        roots.add (*v);
      }
    }

    प्रत्यागम Ok(सच्च);
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
  graph_result_t<शून्य> traverse_directed_bfs (unsigned start_idx, VisitEdgeFunc&& visit_edge)
  {
    जइ (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      प्रत्यागम Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<शून्य>::from (ordering_scratch_, ALLOCATION_FAILURE));

    जइ (!visit_edge (HB_CODEPOINT_INVALID, nullptr, start_idx, 0))
      प्रत्यागम Ok();

    unsigned head = 0;
    unsigned tail = 0;
    auto& queue = ordering_scratch_;
    queue[tail++] = start_idx;

    unsigned depth = 0;
    जाव (head < tail)
    {
      unsigned level_end = tail;
      जाव (head < level_end)
      {
        unsigned ग्रन्थिः_idx = queue[head++];
        ध्रुव auto& v = vertices_[ग्रन्थिः_idx];

        unsigned अङ्कः_real = v.obj().real_links.length;
        unsigned total_links = अङ्कः_real + v.obj().virtual_links.length;

        प्रति (unsigned i = 0; i < total_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          ध्रुव auto& v = vertices_[ग्रन्थिः_idx];
          ध्रुव auto& link = (i < अङ्कः_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - अङ्कः_real];
          unsigned child_idx = link.objidx;

          जइ (!visit_edge (ग्रन्थिः_idx, &link, child_idx, depth + 1))
            अनुवर्त;

          जइ (unlikely (tail >= queue.length))
            प्रत्यागम Err(CYCLE_DETECTED);

          queue[tail++] = child_idx;
        }
      }
      depth++;
    }
    प्रत्यागम Ok();
  }

  graph_result_t<शून्य> find_subgraph (unsigned ग्रन्थिः_idx, hb_map_t& subgraph)
  {
    प्रत्यागम traverse_directed_bfs (ग्रन्थिः_idx, [&] (
      unsigned parent,
      ध्रुव hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      जइ (depth == 0) प्रत्यागम सच्च;
      hb_codepoint_t *गणनम्;
      जइ (subgraph.has (child, &गणनम्))
      {
        (*गणनम्)++;
        प्रत्यागम मिच्छ;
      }
      subgraph.set (child, 1);
      प्रत्यागम सच्च;
    });
  }

  graph_result_t<शून्य> find_subgraph (unsigned ग्रन्थिः_idx, hb_set_t& subgraph)
  {
    प्रत्यागम traverse_directed_bfs (ग्रन्थिः_idx, [&] (
      unsigned parent,
      ध्रुव hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      जइ (subgraph.has (child)) प्रत्यागम मिच्छ;
      subgraph.add (child);
      प्रत्यागम सच्च;
    });
  }

  graph_result_t<size_t> find_subgraph_size (unsigned ग्रन्थिः_idx, hb_set_t& subgraph, unsigned max_depth = HB_GRAPH_INVALID)
  {
    size_t size = 0;
    TRY (traverse_directed_bfs (ग्रन्थिः_idx, [&] (
      unsigned parent,
      ध्रुव hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      जइ (subgraph.has (child)) प्रत्यागम मिच्छ;
      subgraph.add (child);

      ध्रुव auto& o = vertices_[child].obj();
      size += o.tail - o.head;
      प्रत्यागम depth < max_depth;
    }));
    प्रत्यागम Ok(size);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  graph_result_t<शून्य> find_32bit_roots (unsigned ग्रन्थिः_idx, hb_set_t& found)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_set_t visited;
    प्रत्यागम traverse_directed_bfs (ग्रन्थिः_idx, [&] (
      unsigned parent,
      ध्रुव hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {

      जइ (link && found.has(parent))
        // Don't traverse from something that's already marked as a root.
        प्रत्यागम मिच्छ;

      जइ (link && !link->is_signed && link->width == 4)
      {
        found.add (link->objidx);
        visited.add (link->objidx);
        प्रत्यागम मिच्छ;
      }

      जइ (visited.has (child)) प्रत्यागम मिच्छ;
      visited.add (child);
      प्रत्यागम सच्च;
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
                                       ध्रुव O* old_offset,
                                       unsigned new_parent_idx,
                                       ध्रुव O* new_offset)
  {
    distance_invalid = सच्च;
    positions_invalid = सच्च;

    जइ (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      प्रत्यागम Err(OUT_OF_BOUNDS);

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    TRY_ASSIGN (unsigned child_id, index_for_offset (old_parent_idx,
                                                     old_offset));
    जइ (unlikely (child_id >= vertices_.length))
      प्रत्यागम Err(OUT_OF_BOUNDS);

    TRY(new_v.add_real_link(O::static_size, child_id, (ध्रुव वर्ण*) new_offset - (ध्रुव वर्ण*) new_v.obj().head));

    auto& child = vertices_[child_id];
    TRY(child.add_parent (new_parent_idx, मिच्छ));

    old_v.remove_real_link_unordered (child_id, old_offset);
    child.remove_parent (old_parent_idx);

    प्रत्यागम Ok(child_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  template<typename O>
  graph_result_t<शून्य> move_children (unsigned old_parent_idx,
                                      unsigned old_pos_start,
                                      unsigned old_pos_end,
                                      unsigned new_parent_idx,
                                      unsigned new_pos_start)
  {
    जइ (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      प्रत्यागम Err(OUT_OF_BOUNDS);

    distance_invalid = सच्च;
    positions_invalid = सच्च;

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    hb_vector_t<hb_serialize_context_t::object_t::link_t> old_links;
    प्रति (ध्रुव auto& l : old_v.obj().real_links)
    {
      जइ (l.position < old_pos_start || l.position >= old_pos_end)
      {
        old_links.push (l);
        अनुवर्त;
      }

      unsigned array_pos = l.position - old_pos_start;

      unsigned child_id = l.objidx;
      जइ (unlikely (child_id >= vertices_.length))
        प्रत्यागम Err(OUT_OF_BOUNDS);

      TRY(new_v.add_real_link(O::static_size, child_id, new_pos_start + array_pos));

      auto& child = vertices_[child_id];
      TRY(child.add_parent (new_parent_idx, मिच्छ));
      child.remove_parent (old_parent_idx);
    }

    TRY (graph_result_t<शून्य>::from (old_links, ALLOCATION_FAILURE));
    प्रत्यागम old_v.set_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  graph_result_t<शून्य> duplicate_subgraph (unsigned ग्रन्थिः_idx, hb_map_t& index_map)
  {
    graph_result_t<शून्य> result = Ok();
    TRY (traverse_directed_bfs (ग्रन्थिः_idx, [&] (
      unsigned parent,
      ध्रुव hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {
      जइ (result.is_err () || index_map.has (child)) प्रत्यागम मिच्छ;
      auto r = duplicate (child);
      जइ (unlikely (r.is_err ()))
      {
        result = Err(r.error ());
        प्रत्यागम मिच्छ;
      }
      index_map.set (child, *r);
      प्रत्यागम सच्च;
    }));

    प्रत्यागम graph_result_t<शून्य>::from(index_map, ALLOCATION_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  graph_result_t<unsigned> duplicate (unsigned ग्रन्थिः_idx, तार्किक copy_table = मिच्छ)
  {
    जइ (unlikely (ग्रन्थिः_idx >= vertices_.length))
      प्रत्यागम Err(OUT_OF_BOUNDS);

    जइ (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      प्रत्यागम Err(LIMIT_EXCEEDED);
    }

    positions_invalid = सच्च;
    distance_invalid = सच्च;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<शून्य>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<शून्य>::from (ordering_, ALLOCATION_FAILURE));

    auto& child = vertices_[ग्रन्थिः_idx];

    unsigned table_size = child.obj().tail - child.obj().head;
    जइ (copy_table && table_size)
    {
      वर्ण* buffer = (वर्ण*) hb_malloc (table_size);
      जइ (unlikely (!buffer))
        प्रत्यागम Err(ALLOCATION_FAILURE);
      auto res = add_buffer (buffer);
      जइ (unlikely (res.is_err ()))
      {
        hb_free (buffer);
        प्रत्यागम Err(res.error ());
      }
      hb_memcpy (buffer, child.obj().head, table_size);
      clone->set_buffer(buffer, buffer + table_size);
    }
    अण्णहा
    {
      clone->set_buffer(child.obj().head, child.obj().tail);
    }
    clone->distance = child.distance;
    clone->space = child.space;
    clone->reset_parents ();

    प्रति (ध्रुव auto& l : child.obj().real_links)
    {
      TRY(clone->add_real_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, मिच्छ));
    }

    प्रति (ध्रुव auto& l : child.obj().virtual_links)
    {
      TRY(clone->add_virtual_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, सच्च));
    }

    प्रत्यागम Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (unsigned parent_idx, unsigned child_idx, तार्किक copy_table = मिच्छ)
  {
    जइ (unlikely (parent_idx >= vertices_.length || child_idx >= vertices_.length))
      प्रत्यागम Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    ध्रुव auto& child = vertices_[child_idx];
    unsigned links_to_child = child.incoming_edges_from_parent (parent_idx);

    जइ (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "  Not duplicating %u => %u",
                 parent_idx, child_idx);
      प्रत्यागम Err(INVALID_ARGUMENT);
    }

    DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "  Duplicating %u => %u",
               parent_idx, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx, copy_table));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    जइ (parent_idx == clone_idx) parent_idx++;

    auto& parent = vertices_[parent_idx];
    unsigned गणनम् = 0;
    unsigned अङ्कः_real = parent.obj().real_links.length;
    प्रति (auto& l : parent.all_links_writer ())
    {
      गणनम्++;
      जइ (l.objidx != child_idx)
        अनुवर्त;

      TRY(reassign_link (l, parent_idx, clone_idx, गणनम् > अङ्कः_real));
    }

    प्रत्यागम Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (ध्रुव hb_set_t* parents, unsigned child_idx)
  {
    जइ (unlikely (!parents || parents->is_empty ()))
      प्रत्यागम Err(INVALID_ARGUMENT);

    जइ (unlikely (child_idx >= vertices_.length))
      प्रत्यागम Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    ध्रुव auto& child = vertices_[child_idx];
    unsigned links_to_child = 0;
    unsigned last_parent = parents->get_max ();
    unsigned first_parent = parents->get_min ();
    प्रति (unsigned parent_idx : *parents) {
      जइ (unlikely (parent_idx >= vertices_.length))
        प्रत्यागम Err(OUT_OF_BOUNDS);
      links_to_child += child.incoming_edges_from_parent (parent_idx);
    }

    जइ (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "  Not duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);
      प्रत्यागम Err(INVALID_ARGUMENT);
    }

    DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "  Duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx));

    प्रति (unsigned parent_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      जइ (parent_idx == clone_idx) parent_idx++;
      auto& parent = vertices_[parent_idx];
      unsigned गणनम् = 0;
      unsigned अङ्कः_real = parent.obj().real_links.length;
      प्रति (auto& l : parent.all_links_writer ())
      {
        गणनम्++;
        जइ (l.objidx != child_idx)
          अनुवर्त;

        TRY(reassign_link (l, parent_idx, clone_idx, गणनम् > अङ्कः_real));
      }
    }

    प्रत्यागम Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  graph_result_t<unsigned> new_ग्रन्थिः (वर्ण* head, वर्ण* tail)
  {
    जइ (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      प्रत्यागम Err(LIMIT_EXCEEDED);
    }

    positions_invalid = सच्च;
    distance_invalid = सच्च;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<शून्य>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<शून्य>::from (ordering_, ALLOCATION_FAILURE));

    clone->set_buffer (head, tail);
    clone->distance = 0;
    clone->space = 0;

    प्रत्यागम Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  graph_result_t<unsigned> remap_child (unsigned parent_idx, unsigned old_child_idx)
  {
    जइ (unlikely (parent_idx >= vertices_.length || old_child_idx >= vertices_.length))
      प्रत्यागम Err(OUT_OF_BOUNDS);

    TRY_ASSIGN (unsigned new_child_idx, duplicate (old_child_idx));

    auto& parent = vertices_[parent_idx];
    प्रति (auto& l : parent.real_links_writer ())
    {
      जइ (l.objidx != old_child_idx)
        अनुवर्त;
      TRY(reassign_link (l, parent_idx, new_child_idx, मिच्छ));
    }

    प्रति (auto& l : parent.virtual_links_writer ())
    {
      जइ (l.objidx != old_child_idx)
        अनुवर्त;
      TRY(reassign_link (l, parent_idx, new_child_idx, सच्च));
    }
    प्रत्यागम Ok(new_child_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  graph_result_t<तार्किक> raise_childrens_priority (unsigned parent_idx)
  {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "  Raising priority of all children of %u",
               parent_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    जइ (unlikely (parent_idx >= vertices_.length)) प्रत्यागम Err(OUT_OF_BOUNDS);
    तार्किक made_change = मिच्छ;
    प्रति (auto& l : vertices_[parent_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_priority ();
    प्रत्यागम Ok(made_change);
  }

  graph_result_t<शून्य> is_fully_connected ()
  {
    TRY(update_parents());

    जइ (unlikely (मूलम्().incoming_edges ()))
      // Root cannot have parents.
      प्रत्यागम Err(ORPHANED_NODES);

    प्रति (unsigned i = 0; i < मूलम्_idx (); i++)
    {
      जइ (unlikely (!vertices_[i].incoming_edges ()))
        प्रत्यागम Err(ORPHANED_NODES);
    }
    प्रत्यागम Ok();
  }

#जइ 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  शून्य save_fuzzer_seed (hb_tag_t tag) ध्रुव
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((शून्य*) &tag, परिमाण (tag), 1, f);

    uint16_t अङ्कः_objects = vertices_.length;
    fwrite ((शून्य*) &अङ्कः_objects, परिमाण (अङ्कः_objects), 1, f);

    प्रति (ध्रुव auto& v : vertices_)
    {
      uint16_t blob_size = v.table_size ();
      fwrite ((शून्य*) &blob_size, परिमाण (blob_size), 1, f);
      fwrite ((ध्रुव शून्य*) v.obj.head, blob_size, 1, f);
    }

    uint16_t link_गणनम् = 0;
    प्रति (ध्रुव auto& v : vertices_)
      link_गणनम् += v.obj.real_links.length;

    fwrite ((शून्य*) &link_गणनम्, परिमाण (link_गणनम्), 1, f);

    प्रकारनिर्देश संरचना
    {
      uint16_t parent;
      uint16_t child;
      uint16_t position;
      uint8_t width;
    } link_t;

    प्रति (unsigned i = 0; i < vertices_.length; i++)
    {
      प्रति (ध्रुव auto& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.width
        };
        fwrite ((शून्य*) &link, परिमाण (link), 1, f);
      }
    }

    fclose (f);
  }
#endif

  unsigned अङ्कः_roots_for_space_2 (unsigned space) ध्रुव
  {
    प्रत्यागम अङ्कः_roots_for_space[space];
  }

  unsigned next_space () ध्रुव
  {
    प्रत्यागम अङ्कः_roots_for_space.length;
  }

  graph_result_t<शून्य> move_to_new_space (ध्रुव hb_set_t& indices)
  {
    जइ (unlikely (अङ्कः_roots_for_space.length >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 अङ्कः_roots_for_space.length);
      प्रत्यागम Err(LIMIT_EXCEEDED);
    }
    अङ्कः_roots_for_space.push (0);
    TRY (graph_result_t<शून्य>::from (अङ्कः_roots_for_space, ALLOCATION_FAILURE));
    unsigned new_space = अङ्कः_roots_for_space.length - 1;

    प्रति (unsigned index : indices) {
      जइ (unlikely (index >= vertices_.length))
        प्रत्यागम Err(OUT_OF_BOUNDS);
      auto& node = vertices_[index];
      अङ्कः_roots_for_space[node.space] = अङ्कः_roots_for_space[node.space] - 1;
      अङ्कः_roots_for_space[new_space] = अङ्कः_roots_for_space[new_space] + 1;
      node.space = new_space;
      distance_invalid = सच्च;
      positions_invalid = सच्च;
    }
    प्रत्यागम Ok();
  }

  unsigned space_for (unsigned index, unsigned* मूलम् = nullptr) ध्रुव
  {
    जाव (सच्च)
    {
      प्रतिज्ञा (index < vertices_.length);
      ध्रुव auto& node = vertices_[index];
      जइ (node.space)
      {
        जइ (मूलम् != nullptr)
          *मूलम् = index;
        प्रत्यागम node.space;
      }

      जइ (!node.incoming_edges ())
      {
        जइ (मूलम्)
          *मूलम् = index;
        प्रत्यागम 0;
      }

      index = *node.parents_iter ();
    }
  }

  size_t total_size_in_bytes () ध्रुव {
    size_t total_size = 0;
    unsigned गणनम् = vertices_.length;
    प्रति (unsigned i = 0; i < गणनम्; i++) {
      size_t size = vertices_.arrayZ[i].table_size ();
      total_size += size;
    }
    प्रत्यागम total_size;
  }


 गुप्त:

  शून्य print_orphaned_nodes ()
  {
    जइ (!DEBUG_ENABLED(SUBSET_REPACK)) प्रत्यागम;

    DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "Graph is not fully connected.");

    parents_invalid = सच्च;
    (शून्य) update_parents();

    जइ (मूलम्().incoming_edges ()) {
      DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "Root node has incoming edges.");
    }

    प्रति (unsigned i = 0; i < vertices_.length; i++)
    {
      ध्रुव auto& v = vertices_[i];
      जइ (!v.incoming_edges ())
        DEBUG_सन्देशः (SUBSET_REPACK, nullptr, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  unsigned wide_parents (unsigned ग्रन्थिः_idx, hb_set_t& parents) ध्रुव
  {
    unsigned गणनम् = 0;
    प्रति (unsigned p : vertices_[ग्रन्थिः_idx].parents_iter ())
    {
      // Only real links can be wide
      प्रति (ध्रुव auto& l : vertices_[p].obj().real_links)
      {
        जइ (l.objidx == ग्रन्थिः_idx
            && (l.width == 3 || l.width == 4)
            && !l.is_signed)
        {
          गणनम्++;
          parents.add (p);
        }
      }
    }
    प्रत्यागम गणनम्;
  }

 सार्वजनिक:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  graph_result_t<शून्य> update_parents ()
  {
    जइ (!parents_invalid) प्रत्यागम Ok();

    unsigned गणनम् = vertices_.length;

    प्रति (unsigned i = 0; i < गणनम्; i++)
      vertices_.arrayZ[i].reset_parents ();

    प्रति (unsigned p = 0; p < गणनम्; p++)
    {
      प्रति (ध्रुव auto& l : vertices_.arrayZ[p].obj().real_links)
      {
        जइ (unlikely (l.objidx >= गणनम्)) प्रत्यागम Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, मिच्छ));
      }

      प्रति (ध्रुव auto& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        जइ (unlikely (l.objidx >= गणनम्)) प्रत्यागम Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, सच्च));
      }
    }

    parents_invalid = मिच्छ;
    प्रत्यागम Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  शून्य update_positions ()
  {
    जइ (!positions_invalid) प्रत्यागम;

    unsigned current_pos = 0;
    प्रति (unsigned i : ordering_)
    {
      auto& v = vertices_[i];
      v.start = current_pos;
      current_pos += v.obj().tail - v.obj().head;
      v.end = current_pos;
    }

    positions_invalid = मिच्छ;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  graph_result_t<शून्य> update_distances ()
  {
    जइ (!distance_invalid) प्रत्यागम Ok();

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
    unsigned गणनम् = vertices_.length;
    प्रति (unsigned i = 0; i < गणनम्; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[मूलम्_idx ()].distance = 0;

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (गणनम्);
    queue.insert (0, मूलम्_idx ());
    TRY (graph_result_t<शून्य>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<तार्किक> visited;
    visited.resize (vertices_.length);
    TRY (graph_result_t<शून्य>::from (visited, ALLOCATION_FAILURE));

    जाव (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_idx = queue.निष्कासय_minimum ().second;
      जइ (visited[next_idx]) अनुवर्त;
      ध्रुव auto& next = vertices_[next_idx];
      int64_t next_distance = next.distance;
      visited[next_idx] = सच्च;

      प्रति (ध्रुव auto& link : next.obj().all_links ())
      {
        जइ (unlikely (link.objidx >= vertices_.length))
          प्रत्यागम Err(OUT_OF_BOUNDS);

        जइ (visited[link.objidx]) अनुवर्त;

        auto& child_v = vertices_.arrayZ[link.objidx];
        ध्रुव auto& child = child_v.obj ();
        unsigned link_width = link.width ? link.width : 4; // treat virtual offsets as 32 bits wide
        int64_t child_weight = (child.tail - child.head) +
                               ((int64_t) 1 << (link_width * 8)) * (child_v.space + 1);
        int64_t child_distance = next_distance + child_weight;

        जइ (child_distance < child_v.distance)
        {
          child_v.distance = child_distance;
          queue.insert (child_distance, link.objidx);
        }
      }
    }

    TRY (graph_result_t<शून्य>::from (queue, ALLOCATION_FAILURE));
    जइ (unlikely (!queue.is_empty ()))
    {
      print_orphaned_nodes ();
      प्रत्यागम Err(ORPHANED_NODES);
    }

    distance_invalid = मिच्छ;
    प्रत्यागम Ok();
  }

 गुप्त:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  graph_result_t<शून्य> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      unsigned parent_idx,
                      unsigned new_idx,
                      तार्किक is_virtual)
  {
    unsigned old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_parent (parent_idx);
    प्रत्यागम vertices_[new_idx].add_parent (parent_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  template<typename Iterator, hb_requires (hb_is_iterator (Iterator))>
  graph_result_t<शून्य> remap_obj_indices (ध्रुव hb_map_t& id_map,
                                          Iterator subgraph,
                                          तार्किक only_wide = मिच्छ)
  {
    जइ (!id_map) प्रत्यागम Ok();
    प्रति (unsigned i : subgraph)
    {
      auto& vertex = vertices_[i];
      unsigned अङ्कः_real = vertex.obj().real_links.length;
      unsigned गणनम् = 0;
      प्रति (auto& link : vertex.all_links_writer ())
      {
        गणनम्++;
        ध्रुव uint32_t *v;
        जइ (!id_map.has (link.objidx, &v)) अनुवर्त;
        जइ (only_wide && (link.is_signed || (link.width != 4 && link.width != 3))) अनुवर्त;

        TRY(reassign_link (link, i, *v, गणनम् > अङ्कः_real));
      }
    }
    प्रत्यागम Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  graph_result_t<शून्य> find_connected_nodes (unsigned start_idx,
                                             hb_set_t& targets,
                                             hb_set_t& visited,
                                             hb_set_t& connected)
  {
    TRY (graph_result_t<शून्य>::from (visited, ALLOCATION_FAILURE));
    जइ (visited.has (start_idx)) प्रत्यागम Ok();
    जइ (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      प्रत्यागम Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<शून्य>::from (ordering_scratch_, ALLOCATION_FAILURE));

    auto& राशिः = ordering_scratch_;
    unsigned राशिः_len = 0;

    graph_result_t<शून्य> res = Ok();
    auto handle_ग्रन्थिः = [&] (unsigned ग्रन्थिः_idx) {
      visited.add (ग्रन्थिः_idx);
      जइ (targets.has (ग्रन्थिः_idx)) {
        targets.del (ग्रन्थिः_idx);
        connected.add (ग्रन्थिः_idx);
      }

      जइ (unlikely (राशिः_len >= राशिः.length))
      {
        res = Err(CYCLE_DETECTED);
        प्रत्यागम मिच्छ;
      }
      राशिः[राशिः_len++] = ग्रन्थिः_idx;
      प्रत्यागम सच्च;
    };

    जइ (!handle_ग्रन्थिः (start_idx)) प्रत्यागम res;

    जाव (राशिः_len > 0)
    {
      unsigned ग्रन्थिः_idx = राशिः[--राशिः_len];
      ध्रुव auto& v = vertices_[ग्रन्थिः_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      प्रति (ध्रुव auto& l : v.obj().all_links ())
      {
        unsigned child_idx = l.objidx;
        जइ (visited.has (child_idx)) अनुवर्त;
        जइ (!handle_ग्रन्थिः (child_idx)) प्रत्यागम res;
      }

      प्रति (unsigned parent_idx : v.parents_iter ())
      {
        जइ (visited.has (parent_idx)) अनुवर्त;
        जइ (!handle_ग्रन्थिः (parent_idx)) प्रत्यागम res;
      }
    }

    TRY (graph_result_t<शून्य>::from (visited, ALLOCATION_FAILURE));
    TRY (graph_result_t<शून्य>::from (connected, ALLOCATION_FAILURE));

    प्रत्यागम Ok();
  }

 सार्वजनिक:
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

 गुप्त:
  तार्किक parents_invalid = सच्च;
  तार्किक distance_invalid = सच्च;
  तार्किक positions_invalid = सच्च;
  hb_vector_t<unsigned> अङ्कः_roots_for_space;
  hb_vector_t<वर्ण*> buffers;
};

}

#endif  // GRAPH_GRAPH_HH
