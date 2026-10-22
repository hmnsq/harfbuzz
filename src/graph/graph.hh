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

#অন্তর্ভাৱ "../hb-set.hh"
#অন্তর্ভাৱ "../hb-priority-queue.hh"
#অন্তর্ভাৱ "../hb-serialize.hh"
#অন্তর্ভাৱ "graph-result.hh"

#ifndef GRAPH_GRAPH_HH
#define GRAPH_GRAPH_HH

#define HB_GRAPH_INVALID ((unsigned) -1)

নামাকাশ graph {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
সংরচনা graph_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  সংরচনা vertex_t
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

    গুপ্ত:
    hb_serialize_context_t::object_t obj_;
    unsigned incoming_edges_ = 0;
    unsigned single_parent = HB_GRAPH_INVALID;
    তার্কিক has_incoming_virtual_edges_ = মিছা;
    hb_hashmap_t<unsigned, unsigned> parents;
    hb_set_t virtual_parents;
    সার্ৱজনিক:

    vertex_t ()
    {}

    explicit vertex_t(ধ্রুৱ hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    auto parents_iter () ধ্রুৱ HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_parent, single_parent != HB_GRAPH_INVALID),
	parents.চাবিগুলি_ref ()
      )
    )

    শূন্য set_buffer (ৱর্ণ* head, ৱর্ণ* tail) {
      obj_.head = head;
      obj_.tail = tail;
    }

    শূন্য shrink_buffer (size_t গুনতি)
    {
      obj_.tail -= গুনতি;
    }

    auto all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    auto real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    auto virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    শূন্য sort_real_links () {
      obj_.real_links.qsort ();
    }

    graph_result_t<শূন্য> add_real_link (unsigned width, unsigned obj_idx, unsigned position) {
      auto* link = obj_.real_links.push ();
      TRY (graph_result_t<শূন্য>::from (obj_.real_links, ALLOCATION_FAILURE));
      link->width = width;
      link->objidx = obj_idx;
      link->position = position;
      ঘূৰাই Ok();
    }

    graph_result_t<শূন্য> add_virtual_link (unsigned obj_idx) {
      auto* link = obj_.virtual_links.push ();
      TRY (graph_result_t<শূন্য>::from (obj_.virtual_links, ALLOCATION_FAILURE));
      link->objidx = obj_idx;
      ঘূৰাই Ok();
    }

    graph_result_t<শূন্য> add_real_link (ধ্রুৱ hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      ঘূৰাই graph_result_t<শূন্য>::from (obj_.real_links, ALLOCATION_FAILURE);
    }

    graph_result_t<শূন্য> set_real_links (hb_vector_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(graph_result_t<শূন্য>::from (links, ALLOCATION_FAILURE));
      obj_.real_links = links;
      ঘূৰাই Ok();
    }

    শূন্য clear_real_links ()
    {
      obj_.real_links.reset();
    }

    শূন্য clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    graph_result_t<শূন্য> add_virtual_link (ধ্রুৱ hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      ঘূৰাই graph_result_t<শূন্য>::from (obj_.virtual_links, ALLOCATION_FAILURE);
    }

    ধ্রুৱ hb_serialize_context_t::object_t& obj() ধ্রুৱ { ঘূৰাই obj_; }

    template<typename T>
    T as () ধ্রুৱ
    {
      ঘূৰাই reinterpret_cast<T> (obj_.head);
    }

    তার্কিক has_incoming_virtual_edges () ধ্রুৱ
    {
      ঘূৰাই has_incoming_virtual_edges_;
    }

    graph_result_t<শূন্য> link_positions_valid (unsigned অঙ্ক_objects, তার্কিক removed_nil)
    {
      hb_set_t assigned_bytes;
      বাবে (ধ্রুৱ auto& l : obj_.real_links)
      {
        যদি (unlikely (l.objidx >= অঙ্ক_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid object index.");
          ঘূৰাই Err(INVALID_ARGUMENT);
        }

        unsigned start = l.position;
        unsigned end = start + l.width - 1;

        যদি (unlikely (l.width < 2 || l.width > 4))
        {
          DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid link width.");
          ঘূৰাই Err(INVALID_ARGUMENT);
        }

        যদি (unlikely (end >= table_size ()))
        {
          DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                     "Invalid graph. Link position is out of bounds.");
          ঘূৰাই Err(OUT_OF_BOUNDS);
        }

        যদি (unlikely (assigned_bytes.intersects (start, end)))
        {
          DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                     "Invalid graph. Found offsets whose positions overlap.");
          ঘূৰাই Err(INVALID_ARGUMENT);
        }

        assigned_bytes.add_range (start, end);
      }

      ঘূৰাই graph_result_t<শূন্য>::from (assigned_bytes, ALLOCATION_FAILURE);
    }

    শূন্য normalize ()
    {
      obj_.real_links.qsort ();
      বাবে (ধ্রুৱ auto& l : obj_.real_links)
      {
        বাবে (unsigned i = 0; i < l.width; i++)
        {
          obj_.head[l.position + i] = 0;
        }
      }
    }

    তার্কিক equals (unsigned this_index,
                 unsigned other_index,
                 ধ্রুৱ vertex_t& other,
                 ধ্রুৱ graph_t& graph,
                 ধ্রুৱ graph_t& other_graph,
                 unsigned depth) ধ্রুৱ
    {
      যদি (!(as_bytes () == other.as_bytes ()))
      {
        DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_index,
                   (unsigned long) table_size (),
                   other_index,
                   (unsigned long) other.table_size (),
                   depth);

        auto a = as_bytes ();
        auto b = other.as_bytes ();
        যেতিয়ালৈ (a || b)
        {
          DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                     "  0x%x %s 0x%x", (unsigned) *a, (*a == *b) ? "==" : "!=", (unsigned) *b);
          a++;
          b++;
        }
        ঘূৰাই মিছা;
      }

      ঘূৰাই links_equal (obj_.real_links, other.obj_.real_links, graph, other_graph, depth);
    }

    hb_bytes_t as_bytes () ধ্রুৱ
    {
      ঘূৰাই hb_bytes_t (obj_.head, table_size ());
    }

    friend শূন্য swap (vertex_t& a, vertex_t& b)
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
    position_to_index_map () ধ্রুৱ
    {
      hb_hashmap_t<unsigned, unsigned> result;

      result.alloc (obj_.real_links.length);
      বাবে (ধ্রুৱ auto& l : obj_.real_links) {
        result.set (l.position, l.objidx);
      }

      ঘূৰাই graph_result_t<hb_hashmap_t<unsigned, unsigned>>::from (std::move(result), ALLOCATION_FAILURE);
    }

    তার্কিক is_shared () ধ্রুৱ
    {
      ঘূৰাই parents.get_population () > virtual_parents.get_population () + 1;
    }

    unsigned incoming_edges () ধ্রুৱ
    {
      যদি (HB_DEBUG_SUBSET_REPACK)
       {
	প্রতিজ্ঞা (incoming_edges_ == (single_parent != HB_GRAPH_INVALID) +
		(parents.মানসমূহ_ref () | hb_reduce (hb_add, 0)));
       }
      ঘূৰাই incoming_edges_;
    }

    unsigned incoming_edges_from_parent (unsigned parent_index) ধ্রুৱ {
      যদি (single_parent != HB_GRAPH_INVALID) {
        ঘূৰাই single_parent == parent_index ? 1 : 0;
      }

      unsigned* গুনতি;
      ঘূৰাই  parents.has(parent_index, &গুনতি) ? *গুনতি : 0;
    }

    শূন্য reset_parents ()
    {
      incoming_edges_ = 0;
      has_incoming_virtual_edges_ = মিছা;
      single_parent = HB_GRAPH_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    graph_result_t<শূন্য> add_parent (unsigned parent_index, তার্কিক is_virtual)
    {
      প্রতিজ্ঞা (parent_index != HB_GRAPH_INVALID);
      has_incoming_virtual_edges_ |= is_virtual;
      যদি (is_virtual)
        virtual_parents.add (parent_index);

      যদি (incoming_edges_ == 0)
      {
	single_parent = parent_index;
	incoming_edges_ = 1;
	ঘূৰাই to_result();
      }
      অন্যথা যদি (single_parent != HB_GRAPH_INVALID)
      {
        প্রতিজ্ঞা (incoming_edges_ == 1);
	যদি (!parents.set (single_parent, 1))
	  ঘূৰাই to_result ();
	single_parent = HB_GRAPH_INVALID;
      }

      unsigned *v;
      যদি (parents.has (parent_index, &v))
      {
        (*v)++;
	incoming_edges_++;
      }
      অন্যথা যদি (parents.set (parent_index, 1))
	incoming_edges_++;

      ঘূৰাই to_result();
    }

    শূন্য remove_parent (unsigned parent_index)
    {
      যদি (parent_index == single_parent)
      {
	single_parent = HB_GRAPH_INVALID;
	incoming_edges_--;
	virtual_parents.reset ();
	ঘূৰাই;
      }

      unsigned *v;
      যদি (parents.has (parent_index, &v))
      {
	incoming_edges_--;
	যদি (*v > 1)
	  (*v)--;
	অন্যথা
        {
	  parents.del (parent_index);
          virtual_parents.del (parent_index);
        }

	যদি (incoming_edges_ == 1)
	{
	  single_parent = *parents.চাবিগুলি ();
	  parents.reset ();
	}
      }
    }

    শূন্য remove_real_link_unordered (unsigned link_array_index) {
      obj_.real_links.remove_unordered (link_array_index);
    }

    শূন্য remove_real_link_unordered (unsigned child_index, ধ্রুৱ শূন্য* offset)
    {
      unsigned গুনতি = obj_.real_links.length;
      বাবে (unsigned i = 0; i < গুনতি; i++)
      {
        auto& link = obj_.real_links.arrayZ[i];
        যদি (link.objidx != child_index)
          অনুৱর্ত;

        যদি ((obj_.head + link.position) != offset)
          অনুৱর্ত;

        obj_.real_links.remove_unordered (i);
        ঘূৰাই;
      }
    }

    graph_result_t<শূন্য> remap_parent (unsigned old_index, unsigned new_index)
    {
      যদি (single_parent != HB_GRAPH_INVALID)
      {
        যদি (single_parent == old_index)
        {
	  single_parent = new_index;
          যদি (virtual_parents.has (old_index))
          {
            virtual_parents.del (old_index);
            virtual_parents.add (new_index);
          }
        }
        ঘূৰাই graph_result_t<শূন্য>::from(virtual_parents, ALLOCATION_FAILURE);
      }

      ধ্রুৱ unsigned *pv;
      যদি (parents.has (old_index, &pv))
      {
        unsigned v = *pv;
	যদি (!parents.set (new_index, v))
          incoming_edges_ -= v;
	parents.del (old_index);

        যদি (incoming_edges_ == 1)
	{
	  single_parent = *parents.চাবিগুলি ();
	  parents.reset ();
	}
      }

      যদি (virtual_parents.has (old_index))
      {
        virtual_parents.del (old_index);
        virtual_parents.add (new_index);
      }

      ঘূৰাই graph_result_t<শূন্য>::from(virtual_parents, ALLOCATION_FAILURE);
    }

    তার্কিক is_leaf () ধ্রুৱ
    {
      ঘূৰাই !obj_.real_links.length && !obj_.virtual_links.length;
    }

    তার্কিক raise_priority ()
    {
      যদি (has_max_priority ()) ঘূৰাই মিছা;
      priority++;
      ঘূৰাই সঁচা;
    }

    তার্কিক give_max_priority ()
    {
      তার্কিক result = মিছা;
      যেতিয়ালৈ (!has_max_priority()) {
        result = সঁচা;
        priority++;
      }
      ঘূৰাই result;
    }

    তার্কিক has_max_priority () ধ্রুৱ {
      ঘূৰাই priority >= 3;
    }

    size_t table_size () ধ্রুৱ {
      ঘূৰাই obj_.tail - obj_.head;
    }

    int64_t modified_distance (unsigned order) ধ্রুৱ
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_distance =
          hb_clamp (distance + distance_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      যদি (has_max_priority ()) {
        modified_distance = 0;
      }
      ঘূৰাই (modified_distance << 18) | (0x003FFFF & order);
    }

    int64_t distance_modifier () ধ্রুৱ
    {
      যদি (!priority) ঘূৰাই 0;
      int64_t table_size = obj_.tail - obj_.head;

      যদি (priority == 1)
        ঘূৰাই -table_size / 2;

      ঘূৰাই -table_size;
    }

   গুপ্ত:
    graph_result_t<শূন্য> to_result() ধ্রুৱ {
      যদি (unlikely (
        parents.in_error() || virtual_parents.in_error() ||
        obj_.real_links.in_error() || obj_.virtual_links.in_error()))
        ঘূৰাই Err(ALLOCATION_FAILURE);
      ঘূৰাই Ok();
    }

    তার্কিক links_equal (ধ্রুৱ hb_vector_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      ধ্রুৱ hb_vector_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      ধ্রুৱ graph_t& graph,
                      ধ্রুৱ graph_t& other_graph,
                      unsigned depth) ধ্রুৱ
    {
      auto a = this_links.iter ();
      auto b = other_links.iter ();

      যেতিয়ালৈ (a && b)
      {
        ধ্রুৱ auto& link_a = *a;
        ধ্রুৱ auto& link_b = *b;

        যদি (link_a.width != link_b.width ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          ঘূৰাই মিছা;

        যদি (!graph.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_graph.vertices_[link_b.objidx], graph, other_graph, depth + 1))
          ঘূৰাই মিছা;

        a++;
        b++;
      }

      যদি (তার্কিক (a) != তার্কিক (b))
        ঘূৰাই মিছা;

      ঘূৰাই সঁচা;
    }
  };

  গণনা vertex_mutability_t
  {
    Immutable,
    Mutable
  };

  template <typename T, vertex_mutability_t mutability = Mutable>
  সংরচনা vertex_and_table_t
  {
    vertex_and_table_t () : index (0), vertex (nullptr), table (nullptr)
    {}

    template <vertex_mutability_t other_mutability>
    vertex_and_table_t (ধ্রুৱ vertex_and_table_t<T, other_mutability>& o)
      : index (o.index), vertex (o.vertex), table (o.table)
    {}

    unsigned index;
    typename std::conditional<mutability == Immutable, ধ্রুৱ vertex_t*, vertex_t*>::type vertex;
    typename std::conditional<mutability == Immutable, ধ্রুৱ T*, T*>::type table;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  template<typename T>
  স্থির graph_result_t<graph_t> create (ধ্রুৱ T& objects)
  {
    যদি (unlikely (objects.length > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.length);
      ঘূৰাই Err(LIMIT_EXCEEDED);
    }

    graph_t g;
    g.অঙ্ক_roots_for_space.push (1);
    TRY (graph_result_t<শূন্য>::from (g.অঙ্ক_roots_for_space, ALLOCATION_FAILURE));

    তার্কিক removed_nil = মিছা;
    ধ্রুৱ unsigned গুনতি = objects.length;
    g.vertices_.alloc (গুনতি);
    g.ordering_.resize (গুনতি);
    g.ordering_scratch_.alloc (গুনতি);
    TRY (graph_result_t<শূন্য>::from (g.vertices_, ALLOCATION_FAILURE));
    TRY (graph_result_t<শূন্য>::from (g.ordering_, ALLOCATION_FAILURE));
    TRY (graph_result_t<শূন্য>::from (g.ordering_scratch_, ALLOCATION_FAILURE));

    unsigned order = objects.length;
    unsigned skip = 0;
    বাবে (unsigned i = 0; i < গুনতি; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      যদি (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = সঁচা;
        order--;
        g.ordering_.resize (গুনতি - 1);
        skip++;
        অনুৱর্ত;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      যদি (unlikely (obj.in_error ())) {
        ঘূৰাই Err(ALLOCATION_FAILURE);
      }
      vertex_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (গুনতি, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      unsigned obj_idx = i - skip;
      g.ordering_[--order] = obj_idx;

      যদি (!removed_nil) অনুৱর্ত;
      // Fix indices to account for removed nil object.
      বাবে (auto& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    auto r = g.is_fully_connected();
    যদি (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    ঘূৰাই Ok (std::move (g));
  }

  graph_t (ধ্রুৱ graph_t&) = delete;
  graph_t& operator = (ধ্রুৱ graph_t&) = delete;
  graph_t (graph_t&&) = স্ৱভাৱ;
  graph_t& operator = (graph_t&&) = স্ৱভাৱ;

  ~graph_t ()
  {
    বাবে (ৱর্ণ* b : buffers)
      hb_free (b);
  }

গুপ্ত:
  graph_t () = স্ৱভাৱ;

সার্ৱজনিক:

  তার্কিক operator== (ধ্রুৱ graph_t& other) ধ্রুৱ
  {
    ঘূৰাই মূল ().equals (মূল_idx(), other.মূল_idx(), other.মূল (), *এতত্, other, 0);
  }

  শূন্য print () ধ্রুৱ {
    বাবে (unsigned id : ordering_)
    {
      ধ্রুৱ auto& v = vertices_[id];
      printf("%u: %u [", id, (unsigned পূর্ণাঙ্ক)v.table_size());
      বাবে (ধ্রুৱ auto &l : v.obj().real_links) {
        printf("%u, ", l.objidx);
      }
      বাবে (ধ্রুৱ auto &l : v.obj().virtual_links) {
        printf("v%u, ", l.objidx);
      }
      printf("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  শূন্য normalize ()
  {
    বাবে (auto& v : vertices_.writer ())
      v.normalize ();
  }

  ধ্রুৱ vertex_t& মূল () ধ্রুৱ
  {
    ঘূৰাই vertices_[মূল_idx ()];
  }

  unsigned মূল_idx () ধ্রুৱ
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    ঘূৰাই ordering_[0];
  }

  ধ্রুৱ hb_serialize_context_t::object_t& object (unsigned i) ধ্রুৱ
  {
    ঘূৰাই vertices_[i].obj();
  }

  graph_result_t<শূন্য> add_buffer (ৱর্ণ* buffer)
  {
    buffers.push (buffer);
    ঘূৰাই graph_result_t<শূন্য>::from (buffers, ALLOCATION_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  template<typename T>
  graph_result_t<শূন্য> add_link (T* offset,
                                 unsigned parent_id,
                                 unsigned child_id)
  {
    যদি (unlikely (parent_id >= vertices_.length || child_id >= vertices_.length))
      ঘূৰাই Err(OUT_OF_BOUNDS);
    auto& v = vertices_[parent_id];
    TRY(v.add_real_link(2, child_id, (ৱর্ণ*) offset - (ৱর্ণ*) v.obj().head));
    ঘূৰাই vertices_[child_id].add_parent (parent_id, মিছা);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  graph_result_t<শূন্য> sort_shortest_distance_if_needed ()
  {
    যদি (!positions_invalid) ঘূৰাই Ok();
    ঘূৰাই sort_shortest_distance ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  graph_result_t<শূন্য> sort_shortest_distance ()
  {
    positions_invalid = সঁচা;

    যদি (vertices_.length <= 1) {
      // Graph of 1 or less doesn't need sorting.
      ঘূৰাই Ok();
    }

    TRY (update_distances ());

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (vertices_.length);
    TRY (graph_result_t<শূন্য>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.length);
    TRY (graph_result_t<শূন্য>::from (new_ordering, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> removed_edges;
    removed_edges.resize (vertices_.length);
    TRY (graph_result_t<শূন্য>::from (removed_edges, ALLOCATION_FAILURE));

    TRY (update_parents ());

    queue.insert (মূল ().modified_distance (0), মূল_idx ());
    unsigned order = 1;
    unsigned pos = 0;
    যেতিয়ালৈ (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_id = queue.বেৰ_কৰো_minimum().second;

      যদি (unlikely (pos >= new_ordering.length)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr, "Invalid graph. Contains cycle.");
        ঘূৰাই Err(CYCLE_DETECTED);
      }
      new_ordering[pos++] = next_id;
      ধ্রুৱ vertex_t& next = vertices_[next_id];

      বাবে (ধ্রুৱ auto& link : next.obj().all_links ()) {
        যদি (unlikely (link.objidx >= vertices_.length))
          ঘূৰাই Err(OUT_OF_BOUNDS);
        removed_edges[link.objidx]++;
        ধ্রুৱ auto& v = vertices_[link.objidx];
        যদি (!(v.incoming_edges () - removed_edges[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          queue.insert (v.modified_distance (order++),
                        link.objidx);
      }
    }

    TRY (graph_result_t<শূন্য>::from (queue, ALLOCATION_FAILURE));
    TRY (graph_result_t<শূন্য>::from (new_ordering, ALLOCATION_FAILURE));

    hb_swap (ordering_, new_ordering);

    যদি (unlikely (pos != vertices_.length)) {
      print_orphaned_nodes ();
      ঘূৰাই Err(ORPHANED_NODES);
    }

    ঘূৰাই Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  graph_result_t<শূন্য> find_space_roots (hb_set_t& visited, hb_set_t& roots)
  {
    unsigned মূল_index = মূল_idx ();
    বাবে (unsigned i : ordering_)
    {
      যদি (visited.has (i)) অনুৱর্ত;

      // Only real links can form 32 bit spaces
      বাবে (ধ্রুৱ auto& l : vertices_[i].obj().real_links)
      {
        যদি (l.is_signed || l.width < 3)
          অনুৱর্ত;

        যদি (i == মূল_index && l.width == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          অনুৱর্ত;

        যদি (l.width >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_set_t sub_roots;
          TRY (find_32bit_roots (l.objidx, sub_roots));
          যদি (sub_roots) {
            বাবে (unsigned sub_মূল_idx : sub_roots) {
              roots.add (sub_মূল_idx);
              TRY (find_subgraph (sub_মূল_idx, visited));
            }
            অনুৱর্ত;
          }
        }

        roots.add (l.objidx);
        TRY (find_subgraph (l.objidx, visited));
      }
    }

    TRY(graph_result_t<শূন্য>::from (visited, ALLOCATION_FAILURE));
    TRY(graph_result_t<শূন্য>::from (roots, ALLOCATION_FAILURE));
    ঘূৰাই Ok();
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Immutable>> as_table (unsigned parent, ধ্রুৱ শূন্য* offset, Ts... ds)
  {
    TRY_ASSIGN (unsigned idx, index_for_offset (parent, offset));
    ঘূৰাই as_table_from_index<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Mutable>> as_mutable_table (unsigned parent, ধ্রুৱ শূন্য* offset, Ts... ds)
  {
    TRY_ASSIGN (auto idx, mutable_index_for_offset (parent, offset));
    ঘূৰাই as_table_from_index<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, vertex_mutability_t mutability = Mutable, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, mutability>> as_table_from_index (unsigned index, Ts... ds)
  {
    যদি (unlikely (index >= vertices_.length))
      ঘূৰাই Err(OUT_OF_BOUNDS);

    vertex_and_table_t<T, mutability> r;
    r.vertex = (typename std::conditional<mutability == Immutable, ধ্রুৱ vertex_t*, vertex_t*>::type) &vertices_[index];
    r.table = r.vertex->template as<typename std::conditional<mutability == Immutable, ধ্রুৱ T*, T*>::type> ();
    r.index = index;
    যদি (unlikely (!r.table))
      ঘূৰাই Err(INVALID_ARGUMENT);

    TRY(r.table->sanitize (*(r.vertex), std::forward<Ts>(ds)...));
    ঘূৰাই r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  graph_result_t<unsigned> index_for_offset (unsigned গ্ৰন্থি_idx, ধ্রুৱ শূন্য* offset) ধ্রুৱ
  {
    যদি (unlikely (গ্ৰন্থি_idx >= vertices_.length)) ঘূৰাই Err(OUT_OF_BOUNDS);
    ধ্রুৱ auto& node = object (গ্ৰন্থি_idx);
    যদি (unlikely (offset < node.head || offset >= node.tail)) ঘূৰাই Err(OUT_OF_BOUNDS);

    unsigned গুনতি = node.real_links.length;
    বাবে (unsigned i = 0; i < গুনতি; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      ধ্রুৱ auto& link = node.real_links.arrayZ[i];
      যদি (offset != node.head + link.position)
        অনুৱর্ত;
      ঘূৰাই Ok(link.objidx);
    }

    ঘূৰাই Err(INVALID_ARGUMENT);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  graph_result_t<unsigned> mutable_index_for_offset (unsigned গ্ৰন্থি_idx, ধ্রুৱ শূন্য* offset)
  {
    TRY_ASSIGN (unsigned child_idx, index_for_offset (গ্ৰন্থি_idx, offset));
    যদি (unlikely (child_idx >= vertices_.length))
      ঘূৰাই Err(OUT_OF_BOUNDS);

    auto& child = vertices_[child_idx];
    বাবে (unsigned p : child.parents_iter ())
    {
      যদি (p != গ্ৰন্থি_idx) {
        ঘূৰাই duplicate (গ্ৰন্থি_idx, child_idx, সঁচা);
      }
    }

    ঘূৰাই Ok(child_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  graph_result_t<তার্কিক> assign_spaces ()
  {
    TRY (update_parents ());

    বাবে (auto& vertex : vertices_.writer ())
    {
      vertex.space = 0;
      vertex.priority = 0;
    }
    অঙ্ক_roots_for_space.resize (1);
    TRY (graph_result_t<শূন্য>::from (অঙ্ক_roots_for_space, ALLOCATION_FAILURE));
    অঙ্ক_roots_for_space[0] = 1;
    distance_invalid = সঁচা;
    positions_invalid = সঁচা;

    hb_set_t visited;
    hb_set_t roots;
    TRY (find_space_roots (visited, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    visited.invert ();

    যদি (!roots) ঘূৰাই Ok(মিছা);

    যেতিয়ালৈ (roots)
    {
      uint32_t next = HB_SET_মান_INVALID;
      TRY (graph_result_t<শূন্য>::from (roots, ALLOCATION_FAILURE));
      যদি (!roots.next (&next)) ভঙ্গ;

      hb_set_t connected_roots;
      TRY (find_connected_nodes (next, roots, visited, connected_roots));
      TRY (graph_result_t<শূন্য>::from (connected_roots, ALLOCATION_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (graph_result_t<শূন্য>::from (connected_roots, ALLOCATION_FAILURE));

      unsigned next_space = এতত্->next_space ();
      যদি (unlikely (next_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 next_space);
        ঘূৰাই Err(LIMIT_EXCEEDED);
      }
      অঙ্ক_roots_for_space.push (0);
      TRY(graph_result_t<শূন্য>::from (অঙ্ক_roots_for_space, ALLOCATION_FAILURE));

      বাবে (unsigned মূল : connected_roots)
      {
        DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr, "Subgraph %u gets space %u", মূল, next_space);
        vertices_[মূল].space = next_space;
        অঙ্ক_roots_for_space[next_space] = অঙ্ক_roots_for_space[next_space] + 1;
        distance_invalid = সঁচা;
        positions_invalid = সঁচা;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    ঘূৰাই Ok(সঁচা);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  graph_result_t<তার্কিক> isolate_subgraph (hb_set_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_set_t parents;
    বাবে (unsigned মূল_idx : roots)
    {
      যদি (unlikely (মূল_idx >= vertices_.length))
        ঘূৰাই Err(OUT_OF_BOUNDS);
      subgraph.set (মূল_idx, wide_parents (মূল_idx, parents));
    }
    বাবে (unsigned মূল_idx : roots)
    {
      TRY (find_subgraph (মূল_idx, subgraph));
    }
    TRY (graph_result_t<শূন্য>::from (subgraph, ALLOCATION_FAILURE));

    hb_map_t index_map;
    তার্কিক made_changes = মিছা;
    বাবে (auto entry : subgraph.iter ())
    {
      প্রতিজ্ঞা (entry.first < vertices_.length);
      ধ্রুৱ auto& node = vertices_[entry.first];
      unsigned subgraph_incoming_edges = entry.second;

      যদি (subgraph_incoming_edges < node.incoming_edges ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = সঁচা;
        TRY (duplicate_subgraph (entry.first, index_map));
      }
    }

    TRY (graph_result_t<শূন্য>::from (index_map, ALLOCATION_FAILURE));

    যদি (!made_changes)
      ঘূৰাই Ok(মিছা);

    auto new_subgraph =
        + subgraph.চাবিগুলি ()
        | hb_map([&] (uint32_t গ্ৰন্থি_idx) {
          ধ্রুৱ uint32_t *v;
          যদি (index_map.has (গ্ৰন্থি_idx, &v)) ঘূৰাই *v;
          ঘূৰাই গ্ৰন্থি_idx;
        })
        ;

    TRY(remap_obj_indices (index_map, new_subgraph));
    TRY(remap_obj_indices (index_map, parents.iter (), সঁচা));

    // Update roots set with new indices as needed.
    বাবে (auto next : roots)
    {
      ধ্রুৱ uint32_t *v;
      যদি (index_map.has (next, &v))
      {
        roots.del (next);
        roots.add (*v);
      }
    }

    ঘূৰাই Ok(সঁচা);
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
  graph_result_t<শূন্য> traverse_directed_bfs (unsigned start_idx, VisitEdgeFunc&& visit_edge)
  {
    যদি (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      ঘূৰাই Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<শূন্য>::from (ordering_scratch_, ALLOCATION_FAILURE));

    যদি (!visit_edge (HB_CODEPOINT_INVALID, nullptr, start_idx, 0))
      ঘূৰাই Ok();

    unsigned head = 0;
    unsigned tail = 0;
    auto& queue = ordering_scratch_;
    queue[tail++] = start_idx;

    unsigned depth = 0;
    যেতিয়ালৈ (head < tail)
    {
      unsigned level_end = tail;
      যেতিয়ালৈ (head < level_end)
      {
        unsigned গ্ৰন্থি_idx = queue[head++];
        ধ্রুৱ auto& v = vertices_[গ্ৰন্থি_idx];

        unsigned অঙ্ক_real = v.obj().real_links.length;
        unsigned total_links = অঙ্ক_real + v.obj().virtual_links.length;

        বাবে (unsigned i = 0; i < total_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          ধ্রুৱ auto& v = vertices_[গ্ৰন্থি_idx];
          ধ্রুৱ auto& link = (i < অঙ্ক_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - অঙ্ক_real];
          unsigned child_idx = link.objidx;

          যদি (!visit_edge (গ্ৰন্থি_idx, &link, child_idx, depth + 1))
            অনুৱর্ত;

          যদি (unlikely (tail >= queue.length))
            ঘূৰাই Err(CYCLE_DETECTED);

          queue[tail++] = child_idx;
        }
      }
      depth++;
    }
    ঘূৰাই Ok();
  }

  graph_result_t<শূন্য> find_subgraph (unsigned গ্ৰন্থি_idx, hb_map_t& subgraph)
  {
    ঘূৰাই traverse_directed_bfs (গ্ৰন্থি_idx, [&] (
      unsigned parent,
      ধ্রুৱ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      যদি (depth == 0) ঘূৰাই সঁচা;
      hb_codepoint_t *গুনতি;
      যদি (subgraph.has (child, &গুনতি))
      {
        (*গুনতি)++;
        ঘূৰাই মিছা;
      }
      subgraph.set (child, 1);
      ঘূৰাই সঁচা;
    });
  }

  graph_result_t<শূন্য> find_subgraph (unsigned গ্ৰন্থি_idx, hb_set_t& subgraph)
  {
    ঘূৰাই traverse_directed_bfs (গ্ৰন্থি_idx, [&] (
      unsigned parent,
      ধ্রুৱ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      যদি (subgraph.has (child)) ঘূৰাই মিছা;
      subgraph.add (child);
      ঘূৰাই সঁচা;
    });
  }

  graph_result_t<size_t> find_subgraph_size (unsigned গ্ৰন্থি_idx, hb_set_t& subgraph, unsigned max_depth = HB_GRAPH_INVALID)
  {
    size_t size = 0;
    TRY (traverse_directed_bfs (গ্ৰন্থি_idx, [&] (
      unsigned parent,
      ধ্রুৱ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      যদি (subgraph.has (child)) ঘূৰাই মিছা;
      subgraph.add (child);

      ধ্রুৱ auto& o = vertices_[child].obj();
      size += o.tail - o.head;
      ঘূৰাই depth < max_depth;
    }));
    ঘূৰাই Ok(size);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  graph_result_t<শূন্য> find_32bit_roots (unsigned গ্ৰন্থি_idx, hb_set_t& found)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_set_t visited;
    ঘূৰাই traverse_directed_bfs (গ্ৰন্থি_idx, [&] (
      unsigned parent,
      ধ্রুৱ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {

      যদি (link && found.has(parent))
        // Don't traverse from something that's already marked as a root.
        ঘূৰাই মিছা;

      যদি (link && !link->is_signed && link->width == 4)
      {
        found.add (link->objidx);
        visited.add (link->objidx);
        ঘূৰাই মিছা;
      }

      যদি (visited.has (child)) ঘূৰাই মিছা;
      visited.add (child);
      ঘূৰাই সঁচা;
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
                                       ধ্রুৱ O* old_offset,
                                       unsigned new_parent_idx,
                                       ধ্রুৱ O* new_offset)
  {
    distance_invalid = সঁচা;
    positions_invalid = সঁচা;

    যদি (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      ঘূৰাই Err(OUT_OF_BOUNDS);

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    TRY_ASSIGN (unsigned child_id, index_for_offset (old_parent_idx,
                                                     old_offset));
    যদি (unlikely (child_id >= vertices_.length))
      ঘূৰাই Err(OUT_OF_BOUNDS);

    TRY(new_v.add_real_link(O::static_size, child_id, (ধ্রুৱ ৱর্ণ*) new_offset - (ধ্রুৱ ৱর্ণ*) new_v.obj().head));

    auto& child = vertices_[child_id];
    TRY(child.add_parent (new_parent_idx, মিছা));

    old_v.remove_real_link_unordered (child_id, old_offset);
    child.remove_parent (old_parent_idx);

    ঘূৰাই Ok(child_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  template<typename O>
  graph_result_t<শূন্য> move_children (unsigned old_parent_idx,
                                      unsigned old_pos_start,
                                      unsigned old_pos_end,
                                      unsigned new_parent_idx,
                                      unsigned new_pos_start)
  {
    যদি (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      ঘূৰাই Err(OUT_OF_BOUNDS);

    distance_invalid = সঁচা;
    positions_invalid = সঁচা;

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    hb_vector_t<hb_serialize_context_t::object_t::link_t> old_links;
    বাবে (ধ্রুৱ auto& l : old_v.obj().real_links)
    {
      যদি (l.position < old_pos_start || l.position >= old_pos_end)
      {
        old_links.push (l);
        অনুৱর্ত;
      }

      unsigned array_pos = l.position - old_pos_start;

      unsigned child_id = l.objidx;
      যদি (unlikely (child_id >= vertices_.length))
        ঘূৰাই Err(OUT_OF_BOUNDS);

      TRY(new_v.add_real_link(O::static_size, child_id, new_pos_start + array_pos));

      auto& child = vertices_[child_id];
      TRY(child.add_parent (new_parent_idx, মিছা));
      child.remove_parent (old_parent_idx);
    }

    TRY (graph_result_t<শূন্য>::from (old_links, ALLOCATION_FAILURE));
    ঘূৰাই old_v.set_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  graph_result_t<শূন্য> duplicate_subgraph (unsigned গ্ৰন্থি_idx, hb_map_t& index_map)
  {
    graph_result_t<শূন্য> result = Ok();
    TRY (traverse_directed_bfs (গ্ৰন্থি_idx, [&] (
      unsigned parent,
      ধ্রুৱ hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {
      যদি (result.is_err () || index_map.has (child)) ঘূৰাই মিছা;
      auto r = duplicate (child);
      যদি (unlikely (r.is_err ()))
      {
        result = Err(r.error ());
        ঘূৰাই মিছা;
      }
      index_map.set (child, *r);
      ঘূৰাই সঁচা;
    }));

    ঘূৰাই graph_result_t<শূন্য>::from(index_map, ALLOCATION_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  graph_result_t<unsigned> duplicate (unsigned গ্ৰন্থি_idx, তার্কিক copy_table = মিছা)
  {
    যদি (unlikely (গ্ৰন্থি_idx >= vertices_.length))
      ঘূৰাই Err(OUT_OF_BOUNDS);

    যদি (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      ঘূৰাই Err(LIMIT_EXCEEDED);
    }

    positions_invalid = সঁচা;
    distance_invalid = সঁচা;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<শূন্য>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<শূন্য>::from (ordering_, ALLOCATION_FAILURE));

    auto& child = vertices_[গ্ৰন্থি_idx];

    unsigned table_size = child.obj().tail - child.obj().head;
    যদি (copy_table && table_size)
    {
      ৱর্ণ* buffer = (ৱর্ণ*) hb_malloc (table_size);
      যদি (unlikely (!buffer))
        ঘূৰাই Err(ALLOCATION_FAILURE);
      auto res = add_buffer (buffer);
      যদি (unlikely (res.is_err ()))
      {
        hb_free (buffer);
        ঘূৰাই Err(res.error ());
      }
      hb_memcpy (buffer, child.obj().head, table_size);
      clone->set_buffer(buffer, buffer + table_size);
    }
    অন্যথা
    {
      clone->set_buffer(child.obj().head, child.obj().tail);
    }
    clone->distance = child.distance;
    clone->space = child.space;
    clone->reset_parents ();

    বাবে (ধ্রুৱ auto& l : child.obj().real_links)
    {
      TRY(clone->add_real_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, মিছা));
    }

    বাবে (ধ্রুৱ auto& l : child.obj().virtual_links)
    {
      TRY(clone->add_virtual_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, সঁচা));
    }

    ঘূৰাই Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (unsigned parent_idx, unsigned child_idx, তার্কিক copy_table = মিছা)
  {
    যদি (unlikely (parent_idx >= vertices_.length || child_idx >= vertices_.length))
      ঘূৰাই Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    ধ্রুৱ auto& child = vertices_[child_idx];
    unsigned links_to_child = child.incoming_edges_from_parent (parent_idx);

    যদি (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr, "  Not duplicating %u => %u",
                 parent_idx, child_idx);
      ঘূৰাই Err(INVALID_ARGUMENT);
    }

    DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr, "  Duplicating %u => %u",
               parent_idx, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx, copy_table));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    যদি (parent_idx == clone_idx) parent_idx++;

    auto& parent = vertices_[parent_idx];
    unsigned গুনতি = 0;
    unsigned অঙ্ক_real = parent.obj().real_links.length;
    বাবে (auto& l : parent.all_links_writer ())
    {
      গুনতি++;
      যদি (l.objidx != child_idx)
        অনুৱর্ত;

      TRY(reassign_link (l, parent_idx, clone_idx, গুনতি > অঙ্ক_real));
    }

    ঘূৰাই Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (ধ্রুৱ hb_set_t* parents, unsigned child_idx)
  {
    যদি (unlikely (!parents || parents->is_empty ()))
      ঘূৰাই Err(INVALID_ARGUMENT);

    যদি (unlikely (child_idx >= vertices_.length))
      ঘূৰাই Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    ধ্রুৱ auto& child = vertices_[child_idx];
    unsigned links_to_child = 0;
    unsigned last_parent = parents->get_max ();
    unsigned first_parent = parents->get_min ();
    বাবে (unsigned parent_idx : *parents) {
      যদি (unlikely (parent_idx >= vertices_.length))
        ঘূৰাই Err(OUT_OF_BOUNDS);
      links_to_child += child.incoming_edges_from_parent (parent_idx);
    }

    যদি (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr, "  Not duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);
      ঘূৰাই Err(INVALID_ARGUMENT);
    }

    DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr, "  Duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx));

    বাবে (unsigned parent_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      যদি (parent_idx == clone_idx) parent_idx++;
      auto& parent = vertices_[parent_idx];
      unsigned গুনতি = 0;
      unsigned অঙ্ক_real = parent.obj().real_links.length;
      বাবে (auto& l : parent.all_links_writer ())
      {
        গুনতি++;
        যদি (l.objidx != child_idx)
          অনুৱর্ত;

        TRY(reassign_link (l, parent_idx, clone_idx, গুনতি > অঙ্ক_real));
      }
    }

    ঘূৰাই Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  graph_result_t<unsigned> new_গ্ৰন্থি (ৱর্ণ* head, ৱর্ণ* tail)
  {
    যদি (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      ঘূৰাই Err(LIMIT_EXCEEDED);
    }

    positions_invalid = সঁচা;
    distance_invalid = সঁচা;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<শূন্য>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<শূন্য>::from (ordering_, ALLOCATION_FAILURE));

    clone->set_buffer (head, tail);
    clone->distance = 0;
    clone->space = 0;

    ঘূৰাই Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  graph_result_t<unsigned> remap_child (unsigned parent_idx, unsigned old_child_idx)
  {
    যদি (unlikely (parent_idx >= vertices_.length || old_child_idx >= vertices_.length))
      ঘূৰাই Err(OUT_OF_BOUNDS);

    TRY_ASSIGN (unsigned new_child_idx, duplicate (old_child_idx));

    auto& parent = vertices_[parent_idx];
    বাবে (auto& l : parent.real_links_writer ())
    {
      যদি (l.objidx != old_child_idx)
        অনুৱর্ত;
      TRY(reassign_link (l, parent_idx, new_child_idx, মিছা));
    }

    বাবে (auto& l : parent.virtual_links_writer ())
    {
      যদি (l.objidx != old_child_idx)
        অনুৱর্ত;
      TRY(reassign_link (l, parent_idx, new_child_idx, সঁচা));
    }
    ঘূৰাই Ok(new_child_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  graph_result_t<তার্কিক> raise_childrens_priority (unsigned parent_idx)
  {
    DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr, "  Raising priority of all children of %u",
               parent_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    যদি (unlikely (parent_idx >= vertices_.length)) ঘূৰাই Err(OUT_OF_BOUNDS);
    তার্কিক made_change = মিছা;
    বাবে (auto& l : vertices_[parent_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_priority ();
    ঘূৰাই Ok(made_change);
  }

  graph_result_t<শূন্য> is_fully_connected ()
  {
    TRY(update_parents());

    যদি (unlikely (মূল().incoming_edges ()))
      // Root cannot have parents.
      ঘূৰাই Err(ORPHANED_NODES);

    বাবে (unsigned i = 0; i < মূল_idx (); i++)
    {
      যদি (unlikely (!vertices_[i].incoming_edges ()))
        ঘূৰাই Err(ORPHANED_NODES);
    }
    ঘূৰাই Ok();
  }

#যদি 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  শূন্য save_fuzzer_seed (hb_tag_t tag) ধ্রুৱ
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((শূন্য*) &tag, পরিমাণ (tag), 1, f);

    uint16_t অঙ্ক_objects = vertices_.length;
    fwrite ((শূন্য*) &অঙ্ক_objects, পরিমাণ (অঙ্ক_objects), 1, f);

    বাবে (ধ্রুৱ auto& v : vertices_)
    {
      uint16_t blob_size = v.table_size ();
      fwrite ((শূন্য*) &blob_size, পরিমাণ (blob_size), 1, f);
      fwrite ((ধ্রুৱ শূন্য*) v.obj.head, blob_size, 1, f);
    }

    uint16_t link_গুনতি = 0;
    বাবে (ধ্রুৱ auto& v : vertices_)
      link_গুনতি += v.obj.real_links.length;

    fwrite ((শূন্য*) &link_গুনতি, পরিমাণ (link_গুনতি), 1, f);

    প্রকারনির্দেশ সংরচনা
    {
      uint16_t parent;
      uint16_t child;
      uint16_t position;
      uint8_t width;
    } link_t;

    বাবে (unsigned i = 0; i < vertices_.length; i++)
    {
      বাবে (ধ্রুৱ auto& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.width
        };
        fwrite ((শূন্য*) &link, পরিমাণ (link), 1, f);
      }
    }

    fclose (f);
  }
#endif

  unsigned অঙ্ক_roots_for_space_2 (unsigned space) ধ্রুৱ
  {
    ঘূৰাই অঙ্ক_roots_for_space[space];
  }

  unsigned next_space () ধ্রুৱ
  {
    ঘূৰাই অঙ্ক_roots_for_space.length;
  }

  graph_result_t<শূন্য> move_to_new_space (ধ্রুৱ hb_set_t& indices)
  {
    যদি (unlikely (অঙ্ক_roots_for_space.length >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 অঙ্ক_roots_for_space.length);
      ঘূৰাই Err(LIMIT_EXCEEDED);
    }
    অঙ্ক_roots_for_space.push (0);
    TRY (graph_result_t<শূন্য>::from (অঙ্ক_roots_for_space, ALLOCATION_FAILURE));
    unsigned new_space = অঙ্ক_roots_for_space.length - 1;

    বাবে (unsigned index : indices) {
      যদি (unlikely (index >= vertices_.length))
        ঘূৰাই Err(OUT_OF_BOUNDS);
      auto& node = vertices_[index];
      অঙ্ক_roots_for_space[node.space] = অঙ্ক_roots_for_space[node.space] - 1;
      অঙ্ক_roots_for_space[new_space] = অঙ্ক_roots_for_space[new_space] + 1;
      node.space = new_space;
      distance_invalid = সঁচা;
      positions_invalid = সঁচা;
    }
    ঘূৰাই Ok();
  }

  unsigned space_for (unsigned index, unsigned* মূল = nullptr) ধ্রুৱ
  {
    যেতিয়ালৈ (সঁচা)
    {
      প্রতিজ্ঞা (index < vertices_.length);
      ধ্রুৱ auto& node = vertices_[index];
      যদি (node.space)
      {
        যদি (মূল != nullptr)
          *মূল = index;
        ঘূৰাই node.space;
      }

      যদি (!node.incoming_edges ())
      {
        যদি (মূল)
          *মূল = index;
        ঘূৰাই 0;
      }

      index = *node.parents_iter ();
    }
  }

  size_t total_size_in_bytes () ধ্রুৱ {
    size_t total_size = 0;
    unsigned গুনতি = vertices_.length;
    বাবে (unsigned i = 0; i < গুনতি; i++) {
      size_t size = vertices_.arrayZ[i].table_size ();
      total_size += size;
    }
    ঘূৰাই total_size;
  }


 গুপ্ত:

  শূন্য print_orphaned_nodes ()
  {
    যদি (!DEBUG_ENABLED(SUBSET_REPACK)) ঘূৰাই;

    DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr, "Graph is not fully connected.");

    parents_invalid = সঁচা;
    (শূন্য) update_parents();

    যদি (মূল().incoming_edges ()) {
      DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr, "Root node has incoming edges.");
    }

    বাবে (unsigned i = 0; i < vertices_.length; i++)
    {
      ধ্রুৱ auto& v = vertices_[i];
      যদি (!v.incoming_edges ())
        DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  unsigned wide_parents (unsigned গ্ৰন্থি_idx, hb_set_t& parents) ধ্রুৱ
  {
    unsigned গুনতি = 0;
    বাবে (unsigned p : vertices_[গ্ৰন্থি_idx].parents_iter ())
    {
      // Only real links can be wide
      বাবে (ধ্রুৱ auto& l : vertices_[p].obj().real_links)
      {
        যদি (l.objidx == গ্ৰন্থি_idx
            && (l.width == 3 || l.width == 4)
            && !l.is_signed)
        {
          গুনতি++;
          parents.add (p);
        }
      }
    }
    ঘূৰাই গুনতি;
  }

 সার্ৱজনিক:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  graph_result_t<শূন্য> update_parents ()
  {
    যদি (!parents_invalid) ঘূৰাই Ok();

    unsigned গুনতি = vertices_.length;

    বাবে (unsigned i = 0; i < গুনতি; i++)
      vertices_.arrayZ[i].reset_parents ();

    বাবে (unsigned p = 0; p < গুনতি; p++)
    {
      বাবে (ধ্রুৱ auto& l : vertices_.arrayZ[p].obj().real_links)
      {
        যদি (unlikely (l.objidx >= গুনতি)) ঘূৰাই Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, মিছা));
      }

      বাবে (ধ্রুৱ auto& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        যদি (unlikely (l.objidx >= গুনতি)) ঘূৰাই Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, সঁচা));
      }
    }

    parents_invalid = মিছা;
    ঘূৰাই Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  শূন্য update_positions ()
  {
    যদি (!positions_invalid) ঘূৰাই;

    unsigned current_pos = 0;
    বাবে (unsigned i : ordering_)
    {
      auto& v = vertices_[i];
      v.start = current_pos;
      current_pos += v.obj().tail - v.obj().head;
      v.end = current_pos;
    }

    positions_invalid = মিছা;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  graph_result_t<শূন্য> update_distances ()
  {
    যদি (!distance_invalid) ঘূৰাই Ok();

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
    unsigned গুনতি = vertices_.length;
    বাবে (unsigned i = 0; i < গুনতি; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[মূল_idx ()].distance = 0;

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (গুনতি);
    queue.insert (0, মূল_idx ());
    TRY (graph_result_t<শূন্য>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<তার্কিক> visited;
    visited.resize (vertices_.length);
    TRY (graph_result_t<শূন্য>::from (visited, ALLOCATION_FAILURE));

    যেতিয়ালৈ (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_idx = queue.বেৰ_কৰো_minimum ().second;
      যদি (visited[next_idx]) অনুৱর্ত;
      ধ্রুৱ auto& next = vertices_[next_idx];
      int64_t next_distance = next.distance;
      visited[next_idx] = সঁচা;

      বাবে (ধ্রুৱ auto& link : next.obj().all_links ())
      {
        যদি (unlikely (link.objidx >= vertices_.length))
          ঘূৰাই Err(OUT_OF_BOUNDS);

        যদি (visited[link.objidx]) অনুৱর্ত;

        auto& child_v = vertices_.arrayZ[link.objidx];
        ধ্রুৱ auto& child = child_v.obj ();
        unsigned link_width = link.width ? link.width : 4; // treat virtual offsets as 32 bits wide
        int64_t child_weight = (child.tail - child.head) +
                               ((int64_t) 1 << (link_width * 8)) * (child_v.space + 1);
        int64_t child_distance = next_distance + child_weight;

        যদি (child_distance < child_v.distance)
        {
          child_v.distance = child_distance;
          queue.insert (child_distance, link.objidx);
        }
      }
    }

    TRY (graph_result_t<শূন্য>::from (queue, ALLOCATION_FAILURE));
    যদি (unlikely (!queue.is_empty ()))
    {
      print_orphaned_nodes ();
      ঘূৰাই Err(ORPHANED_NODES);
    }

    distance_invalid = মিছা;
    ঘূৰাই Ok();
  }

 গুপ্ত:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  graph_result_t<শূন্য> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      unsigned parent_idx,
                      unsigned new_idx,
                      তার্কিক is_virtual)
  {
    unsigned old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_parent (parent_idx);
    ঘূৰাই vertices_[new_idx].add_parent (parent_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  template<typename Iterator, hb_requires (hb_is_iterator (Iterator))>
  graph_result_t<শূন্য> remap_obj_indices (ধ্রুৱ hb_map_t& id_map,
                                          Iterator subgraph,
                                          তার্কিক only_wide = মিছা)
  {
    যদি (!id_map) ঘূৰাই Ok();
    বাবে (unsigned i : subgraph)
    {
      auto& vertex = vertices_[i];
      unsigned অঙ্ক_real = vertex.obj().real_links.length;
      unsigned গুনতি = 0;
      বাবে (auto& link : vertex.all_links_writer ())
      {
        গুনতি++;
        ধ্রুৱ uint32_t *v;
        যদি (!id_map.has (link.objidx, &v)) অনুৱর্ত;
        যদি (only_wide && (link.is_signed || (link.width != 4 && link.width != 3))) অনুৱর্ত;

        TRY(reassign_link (link, i, *v, গুনতি > অঙ্ক_real));
      }
    }
    ঘূৰাই Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  graph_result_t<শূন্য> find_connected_nodes (unsigned start_idx,
                                             hb_set_t& targets,
                                             hb_set_t& visited,
                                             hb_set_t& connected)
  {
    TRY (graph_result_t<শূন্য>::from (visited, ALLOCATION_FAILURE));
    যদি (visited.has (start_idx)) ঘূৰাই Ok();
    যদি (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      ঘূৰাই Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<শূন্য>::from (ordering_scratch_, ALLOCATION_FAILURE));

    auto& স্তূপ = ordering_scratch_;
    unsigned স্তূপ_len = 0;

    graph_result_t<শূন্য> res = Ok();
    auto handle_গ্ৰন্থি = [&] (unsigned গ্ৰন্থি_idx) {
      visited.add (গ্ৰন্থি_idx);
      যদি (targets.has (গ্ৰন্থি_idx)) {
        targets.del (গ্ৰন্থি_idx);
        connected.add (গ্ৰন্থি_idx);
      }

      যদি (unlikely (স্তূপ_len >= স্তূপ.length))
      {
        res = Err(CYCLE_DETECTED);
        ঘূৰাই মিছা;
      }
      স্তূপ[স্তূপ_len++] = গ্ৰন্থি_idx;
      ঘূৰাই সঁচা;
    };

    যদি (!handle_গ্ৰন্থি (start_idx)) ঘূৰাই res;

    যেতিয়ালৈ (স্তূপ_len > 0)
    {
      unsigned গ্ৰন্থি_idx = স্তূপ[--স্তূপ_len];
      ধ্রুৱ auto& v = vertices_[গ্ৰন্থি_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      বাবে (ধ্রুৱ auto& l : v.obj().all_links ())
      {
        unsigned child_idx = l.objidx;
        যদি (visited.has (child_idx)) অনুৱর্ত;
        যদি (!handle_গ্ৰন্থি (child_idx)) ঘূৰাই res;
      }

      বাবে (unsigned parent_idx : v.parents_iter ())
      {
        যদি (visited.has (parent_idx)) অনুৱর্ত;
        যদি (!handle_গ্ৰন্থি (parent_idx)) ঘূৰাই res;
      }
    }

    TRY (graph_result_t<শূন্য>::from (visited, ALLOCATION_FAILURE));
    TRY (graph_result_t<শূন্য>::from (connected, ALLOCATION_FAILURE));

    ঘূৰাই Ok();
  }

 সার্ৱজনিক:
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

 গুপ্ত:
  তার্কিক parents_invalid = সঁচা;
  তার্কিক distance_invalid = সঁচা;
  তার্কিক positions_invalid = সঁচা;
  hb_vector_t<unsigned> অঙ্ক_roots_for_space;
  hb_vector_t<ৱর্ণ*> buffers;
};

}

#endif  // GRAPH_GRAPH_HH
