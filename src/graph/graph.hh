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

#অন্তর্ভাব "../hb-set.hh"
#অন্তর্ভাব "../hb-priority-queue.hh"
#অন্তর্ভাব "../hb-serialize.hh"
#অন্তর্ভাব "graph-result.hh"

#ifndef GRAPH_GRAPH_HH
#define GRAPH_GRAPH_HH

#define HB_GRAPH_INVALID ((unsigned) -1)

নামাকাশ graph {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
কাঠামো graph_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  কাঠামো vertex_t
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

    ব্যক্তিগত:
    hb_serialize_context_t::object_t obj_;
    unsigned incoming_edges_ = 0;
    unsigned single_parent = HB_GRAPH_INVALID;
    যৌক্তিক has_incoming_virtual_edges_ = মিথ্যা;
    hb_hashmap_t<unsigned, unsigned> parents;
    hb_set_t virtual_parents;
    সর্বজনীন:

    vertex_t ()
    {}

    explicit vertex_t(ধ্রুবক hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    auto parents_iter () ধ্রুবক HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_parent, single_parent != HB_GRAPH_INVALID),
	parents.চাবিগুলি_ref ()
      )
    )

    শূন্য set_বাফার (অক্ষর* শীর্ষ_2, অক্ষর* লেজ) {
      obj_.শীর্ষ_2 = শীর্ষ_2;
      obj_.লেজ = লেজ;
    }

    শূন্য shrink_বাফার (size_t গুনতি)
    {
      obj_.লেজ -= গুনতি;
    }

    auto all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    auto real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    auto virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    শূন্য sort_real_links () {
      obj_.real_links.qsort ();
    }

    graph_ফলাফল_t<শূন্য> add_real_link (unsigned width, unsigned obj_idx, unsigned position) {
      auto* link = obj_.real_links.push ();
      TRY (graph_ফলাফল_t<শূন্য>::from (obj_.real_links, ALLOCATION_FAILURE));
      link->width = width;
      link->objidx = obj_idx;
      link->position = position;
      ফেরত Ok();
    }

    graph_ফলাফল_t<শূন্য> add_virtual_link (unsigned obj_idx) {
      auto* link = obj_.virtual_links.push ();
      TRY (graph_ফলাফল_t<শূন্য>::from (obj_.virtual_links, ALLOCATION_FAILURE));
      link->objidx = obj_idx;
      ফেরত Ok();
    }

    graph_ফলাফল_t<শূন্য> add_real_link (ধ্রুবক hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      ফেরত graph_ফলাফল_t<শূন্য>::from (obj_.real_links, ALLOCATION_FAILURE);
    }

    graph_ফলাফল_t<শূন্য> set_real_links (hb_vector_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(graph_ফলাফল_t<শূন্য>::from (links, ALLOCATION_FAILURE));
      obj_.real_links = links;
      ফেরত Ok();
    }

    শূন্য clear_real_links ()
    {
      obj_.real_links.reset();
    }

    শূন্য clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    graph_ফলাফল_t<শূন্য> add_virtual_link (ধ্রুবক hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      ফেরত graph_ফলাফল_t<শূন্য>::from (obj_.virtual_links, ALLOCATION_FAILURE);
    }

    ধ্রুবক hb_serialize_context_t::object_t& obj() ধ্রুবক { ফেরত obj_; }

    template<typename T>
    T as () ধ্রুবক
    {
      ফেরত reinterpret_cast<T> (obj_.শীর্ষ_2);
    }

    যৌক্তিক has_incoming_virtual_edges () ধ্রুবক
    {
      ফেরত has_incoming_virtual_edges_;
    }

    graph_ফলাফল_t<শূন্য> link_positions_valid (unsigned অঙ্ক_objects, যৌক্তিক removed_nil)
    {
      hb_set_t assigned_bytes;
      জন্য (ধ্রুবক auto& l : obj_.real_links)
      {
        যদি (unlikely (l.objidx >= অঙ্ক_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid object index.");
          ফেরত Err(INVALID_ARGUMENT);
        }

        unsigned start = l.position;
        unsigned end = start + l.width - 1;

        যদি (unlikely (l.width < 2 || l.width > 4))
        {
          DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid link width.");
          ফেরত Err(INVALID_ARGUMENT);
        }

        যদি (unlikely (end >= table_আকার ()))
        {
          DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                     "Invalid graph. Link position is out of bounds.");
          ফেরত Err(OUT_OF_BOUNDS);
        }

        যদি (unlikely (assigned_bytes.intersects (start, end)))
        {
          DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                     "Invalid graph. Found offsets whose positions overlap.");
          ফেরত Err(INVALID_ARGUMENT);
        }

        assigned_bytes.add_range (start, end);
      }

      ফেরত graph_ফলাফল_t<শূন্য>::from (assigned_bytes, ALLOCATION_FAILURE);
    }

    শূন্য normalize ()
    {
      obj_.real_links.qsort ();
      জন্য (ধ্রুবক auto& l : obj_.real_links)
      {
        জন্য (unsigned i = 0; i < l.width; i++)
        {
          obj_.শীর্ষ_2[l.position + i] = 0;
        }
      }
    }

    যৌক্তিক equals (unsigned this_সূচক,
                 unsigned other_সূচক,
                 ধ্রুবক vertex_t& other,
                 ধ্রুবক graph_t& graph,
                 ধ্রুবক graph_t& other_graph,
                 unsigned depth) ধ্রুবক
    {
      যদি (!(as_bytes () == other.as_bytes ()))
      {
        DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_সূচক,
                   (unsigned long) table_আকার (),
                   other_সূচক,
                   (unsigned long) other.table_আকার (),
                   depth);

        auto a = as_bytes ();
        auto b = other.as_bytes ();
        যখন (a || b)
        {
          DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                     "  0x%x %s 0x%x", (unsigned) *a, (*a == *b) ? "==" : "!=", (unsigned) *b);
          a++;
          b++;
        }
        ফেরত মিথ্যা;
      }

      ফেরত links_equal (obj_.real_links, other.obj_.real_links, graph, other_graph, depth);
    }

    hb_bytes_t as_bytes () ধ্রুবক
    {
      ফেরত hb_bytes_t (obj_.শীর্ষ_2, table_আকার ());
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

    graph_ফলাফল_t<hb_hashmap_t<unsigned, unsigned>>
    position_to_সূচক_map () ধ্রুবক
    {
      hb_hashmap_t<unsigned, unsigned> ফলাফল;

      ফলাফল.alloc (obj_.real_links.দৈর্ঘ্য);
      জন্য (ধ্রুবক auto& l : obj_.real_links) {
        ফলাফল.set (l.position, l.objidx);
      }

      ফেরত graph_ফলাফল_t<hb_hashmap_t<unsigned, unsigned>>::from (std::move(ফলাফল), ALLOCATION_FAILURE);
    }

    যৌক্তিক is_shared () ধ্রুবক
    {
      ফেরত parents.get_population () > virtual_parents.get_population () + 1;
    }

    unsigned incoming_edges () ধ্রুবক
    {
      যদি (HB_DEBUG_SUBSET_REPACK)
       {
	প্রতিজ্ঞা (incoming_edges_ == (single_parent != HB_GRAPH_INVALID) +
		(parents.মানসমূহ_ref () | hb_reduce (hb_add, 0)));
       }
      ফেরত incoming_edges_;
    }

    unsigned incoming_edges_from_parent (unsigned parent_সূচক) ধ্রুবক {
      যদি (single_parent != HB_GRAPH_INVALID) {
        ফেরত single_parent == parent_সূচক ? 1 : 0;
      }

      unsigned* গুনতি;
      ফেরত  parents.has(parent_সূচক, &গুনতি) ? *গুনতি : 0;
    }

    শূন্য reset_parents ()
    {
      incoming_edges_ = 0;
      has_incoming_virtual_edges_ = মিথ্যা;
      single_parent = HB_GRAPH_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    graph_ফলাফল_t<শূন্য> add_parent (unsigned parent_সূচক, যৌক্তিক is_virtual)
    {
      প্রতিজ্ঞা (parent_সূচক != HB_GRAPH_INVALID);
      has_incoming_virtual_edges_ |= is_virtual;
      যদি (is_virtual)
        virtual_parents.add (parent_সূচক);

      যদি (incoming_edges_ == 0)
      {
	single_parent = parent_সূচক;
	incoming_edges_ = 1;
	ফেরত to_ফলাফল();
      }
      অন্যথা যদি (single_parent != HB_GRAPH_INVALID)
      {
        প্রতিজ্ঞা (incoming_edges_ == 1);
	যদি (!parents.set (single_parent, 1))
	  ফেরত to_ফলাফল ();
	single_parent = HB_GRAPH_INVALID;
      }

      unsigned *v;
      যদি (parents.has (parent_সূচক, &v))
      {
        (*v)++;
	incoming_edges_++;
      }
      অন্যথা যদি (parents.set (parent_সূচক, 1))
	incoming_edges_++;

      ফেরত to_ফলাফল();
    }

    শূন্য remove_parent (unsigned parent_সূচক)
    {
      যদি (parent_সূচক == single_parent)
      {
	single_parent = HB_GRAPH_INVALID;
	incoming_edges_--;
	virtual_parents.reset ();
	ফেরত;
      }

      unsigned *v;
      যদি (parents.has (parent_সূচক, &v))
      {
	incoming_edges_--;
	যদি (*v > 1)
	  (*v)--;
	অন্যথা
        {
	  parents.del (parent_সূচক);
          virtual_parents.del (parent_সূচক);
        }

	যদি (incoming_edges_ == 1)
	{
	  single_parent = *parents.চাবিগুলি ();
	  parents.reset ();
	}
      }
    }

    শূন্য remove_real_link_unordered (unsigned link_array_সূচক) {
      obj_.real_links.remove_unordered (link_array_সূচক);
    }

    শূন্য remove_real_link_unordered (unsigned child_সূচক, ধ্রুবক শূন্য* offset)
    {
      unsigned গুনতি = obj_.real_links.দৈর্ঘ্য;
      জন্য (unsigned i = 0; i < গুনতি; i++)
      {
        auto& link = obj_.real_links.arrayZ[i];
        যদি (link.objidx != child_সূচক)
          চালু;

        যদি ((obj_.শীর্ষ_2 + link.position) != offset)
          চালু;

        obj_.real_links.remove_unordered (i);
        ফেরত;
      }
    }

    graph_ফলাফল_t<শূন্য> remap_parent (unsigned old_সূচক, unsigned new_সূচক)
    {
      যদি (single_parent != HB_GRAPH_INVALID)
      {
        যদি (single_parent == old_সূচক)
        {
	  single_parent = new_সূচক;
          যদি (virtual_parents.has (old_সূচক))
          {
            virtual_parents.del (old_সূচক);
            virtual_parents.add (new_সূচক);
          }
        }
        ফেরত graph_ফলাফল_t<শূন্য>::from(virtual_parents, ALLOCATION_FAILURE);
      }

      ধ্রুবক unsigned *pv;
      যদি (parents.has (old_সূচক, &pv))
      {
        unsigned v = *pv;
	যদি (!parents.set (new_সূচক, v))
          incoming_edges_ -= v;
	parents.del (old_সূচক);

        যদি (incoming_edges_ == 1)
	{
	  single_parent = *parents.চাবিগুলি ();
	  parents.reset ();
	}
      }

      যদি (virtual_parents.has (old_সূচক))
      {
        virtual_parents.del (old_সূচক);
        virtual_parents.add (new_সূচক);
      }

      ফেরত graph_ফলাফল_t<শূন্য>::from(virtual_parents, ALLOCATION_FAILURE);
    }

    যৌক্তিক is_leaf () ধ্রুবক
    {
      ফেরত !obj_.real_links.দৈর্ঘ্য && !obj_.virtual_links.দৈর্ঘ্য;
    }

    যৌক্তিক raise_priority ()
    {
      যদি (has_max_priority ()) ফেরত মিথ্যা;
      priority++;
      ফেরত সত্য;
    }

    যৌক্তিক give_max_priority ()
    {
      যৌক্তিক ফলাফল = মিথ্যা;
      যখন (!has_max_priority()) {
        ফলাফল = সত্য;
        priority++;
      }
      ফেরত ফলাফল;
    }

    যৌক্তিক has_max_priority () ধ্রুবক {
      ফেরত priority >= 3;
    }

    size_t table_আকার () ধ্রুবক {
      ফেরত obj_.লেজ - obj_.শীর্ষ_2;
    }

    int64_t modified_distance (unsigned order) ধ্রুবক
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
      ফেরত (modified_distance << 18) | (0x003FFFF & order);
    }

    int64_t distance_modifier () ধ্রুবক
    {
      যদি (!priority) ফেরত 0;
      int64_t table_আকার = obj_.লেজ - obj_.শীর্ষ_2;

      যদি (priority == 1)
        ফেরত -table_আকার / 2;

      ফেরত -table_আকার;
    }

   ব্যক্তিগত:
    graph_ফলাফল_t<শূন্য> to_ফলাফল() ধ্রুবক {
      যদি (unlikely (
        parents.in_error() || virtual_parents.in_error() ||
        obj_.real_links.in_error() || obj_.virtual_links.in_error()))
        ফেরত Err(ALLOCATION_FAILURE);
      ফেরত Ok();
    }

    যৌক্তিক links_equal (ধ্রুবক hb_vector_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      ধ্রুবক hb_vector_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      ধ্রুবক graph_t& graph,
                      ধ্রুবক graph_t& other_graph,
                      unsigned depth) ধ্রুবক
    {
      auto a = this_links.iter ();
      auto b = other_links.iter ();

      যখন (a && b)
      {
        ধ্রুবক auto& link_a = *a;
        ধ্রুবক auto& link_b = *b;

        যদি (link_a.width != link_b.width ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          ফেরত মিথ্যা;

        যদি (!graph.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_graph.vertices_[link_b.objidx], graph, other_graph, depth + 1))
          ফেরত মিথ্যা;

        a++;
        b++;
      }

      যদি (যৌক্তিক (a) != যৌক্তিক (b))
        ফেরত মিথ্যা;

      ফেরত সত্য;
    }
  };

  গণনা vertex_mutability_t
  {
    Immutable,
    Mutable
  };

  template <typename T, vertex_mutability_t mutability = Mutable>
  কাঠামো vertex_and_table_t
  {
    vertex_and_table_t () : সূচক_2 (0), vertex (nullptr), table (nullptr)
    {}

    template <vertex_mutability_t other_mutability>
    vertex_and_table_t (ধ্রুবক vertex_and_table_t<T, other_mutability>& o)
      : সূচক_2 (o.সূচক_2), vertex (o.vertex), table (o.table)
    {}

    unsigned সূচক_2;
    typename std::conditional<mutability == Immutable, ধ্রুবক vertex_t*, vertex_t*>::type vertex;
    typename std::conditional<mutability == Immutable, ধ্রুবক T*, T*>::type table;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  template<typename T>
  স্থির graph_ফলাফল_t<graph_t> create (ধ্রুবক T& objects)
  {
    যদি (unlikely (objects.দৈর্ঘ্য > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.দৈর্ঘ্য);
      ফেরত Err(LIMIT_EXCEEDED);
    }

    graph_t g;
    g.অঙ্ক_roots_for_space.push (1);
    TRY (graph_ফলাফল_t<শূন্য>::from (g.অঙ্ক_roots_for_space, ALLOCATION_FAILURE));

    যৌক্তিক removed_nil = মিথ্যা;
    ধ্রুবক unsigned গুনতি = objects.দৈর্ঘ্য;
    g.vertices_.alloc (গুনতি);
    g.ordering_.resize (গুনতি);
    g.ordering_scratch_.alloc (গুনতি);
    TRY (graph_ফলাফল_t<শূন্য>::from (g.vertices_, ALLOCATION_FAILURE));
    TRY (graph_ফলাফল_t<শূন্য>::from (g.ordering_, ALLOCATION_FAILURE));
    TRY (graph_ফলাফল_t<শূন্য>::from (g.ordering_scratch_, ALLOCATION_FAILURE));

    unsigned order = objects.দৈর্ঘ্য;
    unsigned skip = 0;
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      যদি (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = সত্য;
        order--;
        g.ordering_.resize (গুনতি - 1);
        skip++;
        চালু;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      যদি (unlikely (obj.in_error ())) {
        ফেরত Err(ALLOCATION_FAILURE);
      }
      vertex_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (গুনতি, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      unsigned obj_idx = i - skip;
      g.ordering_[--order] = obj_idx;

      যদি (!removed_nil) চালু;
      // Fix indices to account for removed nil object.
      জন্য (auto& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    auto r = g.is_fully_connected();
    যদি (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    ফেরত Ok (std::move (g));
  }

  graph_t (ধ্রুবক graph_t&) = delete;
  graph_t& operator = (ধ্রুবক graph_t&) = delete;
  graph_t (graph_t&&) = পূর্বনির্ধারিত;
  graph_t& operator = (graph_t&&) = পূর্বনির্ধারিত;

  ~graph_t ()
  {
    জন্য (অক্ষর* b : buffers)
      hb_free (b);
  }

ব্যক্তিগত:
  graph_t () = পূর্বনির্ধারিত;

সর্বজনীন:

  যৌক্তিক operator== (ধ্রুবক graph_t& other) ধ্রুবক
  {
    ফেরত মূল ().equals (মূল_idx(), other.মূল_idx(), other.মূল (), *এই, other, 0);
  }

  শূন্য print () ধ্রুবক {
    জন্য (unsigned id : ordering_)
    {
      ধ্রুবক auto& v = vertices_[id];
      printf("%u: %u [", id, (unsigned পূর্ণসংখ্যা)v.table_আকার());
      জন্য (ধ্রুবক auto &l : v.obj().real_links) {
        printf("%u, ", l.objidx);
      }
      জন্য (ধ্রুবক auto &l : v.obj().virtual_links) {
        printf("v%u, ", l.objidx);
      }
      printf("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  শূন্য normalize ()
  {
    জন্য (auto& v : vertices_.writer ())
      v.normalize ();
  }

  ধ্রুবক vertex_t& মূল () ধ্রুবক
  {
    ফেরত vertices_[মূল_idx ()];
  }

  unsigned মূল_idx () ধ্রুবক
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    ফেরত ordering_[0];
  }

  ধ্রুবক hb_serialize_context_t::object_t& object (unsigned i) ধ্রুবক
  {
    ফেরত vertices_[i].obj();
  }

  graph_ফলাফল_t<শূন্য> add_বাফার (অক্ষর* বাফার)
  {
    buffers.push (বাফার);
    ফেরত graph_ফলাফল_t<শূন্য>::from (buffers, ALLOCATION_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  template<typename T>
  graph_ফলাফল_t<শূন্য> add_link (T* offset,
                                 unsigned parent_id,
                                 unsigned child_id)
  {
    যদি (unlikely (parent_id >= vertices_.দৈর্ঘ্য || child_id >= vertices_.দৈর্ঘ্য))
      ফেরত Err(OUT_OF_BOUNDS);
    auto& v = vertices_[parent_id];
    TRY(v.add_real_link(2, child_id, (অক্ষর*) offset - (অক্ষর*) v.obj().শীর্ষ_2));
    ফেরত vertices_[child_id].add_parent (parent_id, মিথ্যা);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  graph_ফলাফল_t<শূন্য> sort_shortest_distance_if_needed ()
  {
    যদি (!positions_invalid) ফেরত Ok();
    ফেরত sort_shortest_distance ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  graph_ফলাফল_t<শূন্য> sort_shortest_distance ()
  {
    positions_invalid = সত্য;

    যদি (vertices_.দৈর্ঘ্য <= 1) {
      // Graph of 1 or less doesn't need sorting.
      ফেরত Ok();
    }

    TRY (update_distances ());

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (vertices_.দৈর্ঘ্য);
    TRY (graph_ফলাফল_t<শূন্য>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.দৈর্ঘ্য);
    TRY (graph_ফলাফল_t<শূন্য>::from (new_ordering, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> removed_edges;
    removed_edges.resize (vertices_.দৈর্ঘ্য);
    TRY (graph_ফলাফল_t<শূন্য>::from (removed_edges, ALLOCATION_FAILURE));

    TRY (update_parents ());

    queue.insert (মূল ().modified_distance (0), মূল_idx ());
    unsigned order = 1;
    unsigned pos = 0;
    যখন (!queue.in_error () && !queue.is_empty ())
    {
      unsigned পরবর্তী_id = queue.বের_করো_minimum().second;

      যদি (unlikely (pos >= new_ordering.দৈর্ঘ্য)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Invalid graph. Contains cycle.");
        ফেরত Err(CYCLE_DETECTED);
      }
      new_ordering[pos++] = পরবর্তী_id;
      ধ্রুবক vertex_t& next = vertices_[পরবর্তী_id];

      জন্য (ধ্রুবক auto& link : next.obj().all_links ()) {
        যদি (unlikely (link.objidx >= vertices_.দৈর্ঘ্য))
          ফেরত Err(OUT_OF_BOUNDS);
        removed_edges[link.objidx]++;
        ধ্রুবক auto& v = vertices_[link.objidx];
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

    TRY (graph_ফলাফল_t<শূন্য>::from (queue, ALLOCATION_FAILURE));
    TRY (graph_ফলাফল_t<শূন্য>::from (new_ordering, ALLOCATION_FAILURE));

    hb_swap (ordering_, new_ordering);

    যদি (unlikely (pos != vertices_.দৈর্ঘ্য)) {
      print_orphaned_nodes ();
      ফেরত Err(ORPHANED_NODES);
    }

    ফেরত Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  graph_ফলাফল_t<শূন্য> find_space_roots (hb_set_t& visited, hb_set_t& roots)
  {
    unsigned মূল_সূচক = মূল_idx ();
    জন্য (unsigned i : ordering_)
    {
      যদি (visited.has (i)) চালু;

      // Only real links can form 32 bit spaces
      জন্য (ধ্রুবক auto& l : vertices_[i].obj().real_links)
      {
        যদি (l.is_signed || l.width < 3)
          চালু;

        যদি (i == মূল_সূচক && l.width == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          চালু;

        যদি (l.width >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_set_t sub_roots;
          TRY (find_32bit_roots (l.objidx, sub_roots));
          যদি (sub_roots) {
            জন্য (unsigned sub_মূল_idx : sub_roots) {
              roots.add (sub_মূল_idx);
              TRY (find_subgraph (sub_মূল_idx, visited));
            }
            চালু;
          }
        }

        roots.add (l.objidx);
        TRY (find_subgraph (l.objidx, visited));
      }
    }

    TRY(graph_ফলাফল_t<শূন্য>::from (visited, ALLOCATION_FAILURE));
    TRY(graph_ফলাফল_t<শূন্য>::from (roots, ALLOCATION_FAILURE));
    ফেরত Ok();
  }

  template <typename T, typename ...Ts>
  graph_ফলাফল_t<vertex_and_table_t<T, Immutable>> as_table (unsigned parent, ধ্রুবক শূন্য* offset, Ts... ds)
  {
    TRY_ASSIGN (unsigned idx, সূচক_for_offset (parent, offset));
    ফেরত as_table_from_সূচক<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, typename ...Ts>
  graph_ফলাফল_t<vertex_and_table_t<T, Mutable>> as_mutable_table (unsigned parent, ধ্রুবক শূন্য* offset, Ts... ds)
  {
    TRY_ASSIGN (auto idx, mutable_সূচক_for_offset (parent, offset));
    ফেরত as_table_from_সূচক<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, vertex_mutability_t mutability = Mutable, typename ...Ts>
  graph_ফলাফল_t<vertex_and_table_t<T, mutability>> as_table_from_সূচক (unsigned সূচক_2, Ts... ds)
  {
    যদি (unlikely (সূচক_2 >= vertices_.দৈর্ঘ্য))
      ফেরত Err(OUT_OF_BOUNDS);

    vertex_and_table_t<T, mutability> r;
    r.vertex = (typename std::conditional<mutability == Immutable, ধ্রুবক vertex_t*, vertex_t*>::type) &vertices_[সূচক_2];
    r.table = r.vertex->template as<typename std::conditional<mutability == Immutable, ধ্রুবক T*, T*>::type> ();
    r.সূচক_2 = সূচক_2;
    যদি (unlikely (!r.table))
      ফেরত Err(INVALID_ARGUMENT);

    TRY(r.table->sanitize (*(r.vertex), std::forward<Ts>(ds)...));
    ফেরত r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  graph_ফলাফল_t<unsigned> সূচক_for_offset (unsigned গ্রন্থি_idx, ধ্রুবক শূন্য* offset) ধ্রুবক
  {
    যদি (unlikely (গ্রন্থি_idx >= vertices_.দৈর্ঘ্য)) ফেরত Err(OUT_OF_BOUNDS);
    ধ্রুবক auto& node = object (গ্রন্থি_idx);
    যদি (unlikely (offset < node.শীর্ষ_2 || offset >= node.লেজ)) ফেরত Err(OUT_OF_BOUNDS);

    unsigned গুনতি = node.real_links.দৈর্ঘ্য;
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      ধ্রুবক auto& link = node.real_links.arrayZ[i];
      যদি (offset != node.শীর্ষ_2 + link.position)
        চালু;
      ফেরত Ok(link.objidx);
    }

    ফেরত Err(INVALID_ARGUMENT);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  graph_ফলাফল_t<unsigned> mutable_সূচক_for_offset (unsigned গ্রন্থি_idx, ধ্রুবক শূন্য* offset)
  {
    TRY_ASSIGN (unsigned child_idx, সূচক_for_offset (গ্রন্থি_idx, offset));
    যদি (unlikely (child_idx >= vertices_.দৈর্ঘ্য))
      ফেরত Err(OUT_OF_BOUNDS);

    auto& child = vertices_[child_idx];
    জন্য (unsigned p : child.parents_iter ())
    {
      যদি (p != গ্রন্থি_idx) {
        ফেরত duplicate (গ্রন্থি_idx, child_idx, সত্য);
      }
    }

    ফেরত Ok(child_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  graph_ফলাফল_t<যৌক্তিক> assign_spaces ()
  {
    TRY (update_parents ());

    জন্য (auto& vertex : vertices_.writer ())
    {
      vertex.space = 0;
      vertex.priority = 0;
    }
    অঙ্ক_roots_for_space.resize (1);
    TRY (graph_ফলাফল_t<শূন্য>::from (অঙ্ক_roots_for_space, ALLOCATION_FAILURE));
    অঙ্ক_roots_for_space[0] = 1;
    distance_invalid = সত্য;
    positions_invalid = সত্য;

    hb_set_t visited;
    hb_set_t roots;
    TRY (find_space_roots (visited, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    visited.invert ();

    যদি (!roots) ফেরত Ok(মিথ্যা);

    যখন (roots)
    {
      uint32_t next = HB_SET_মান_INVALID;
      TRY (graph_ফলাফল_t<শূন্য>::from (roots, ALLOCATION_FAILURE));
      যদি (!roots.next (&next)) বিরতি;

      hb_set_t connected_roots;
      TRY (find_connected_nodes (next, roots, visited, connected_roots));
      TRY (graph_ফলাফল_t<শূন্য>::from (connected_roots, ALLOCATION_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (graph_ফলাফল_t<শূন্য>::from (connected_roots, ALLOCATION_FAILURE));

      unsigned পরবর্তী_space = এই->পরবর্তী_space ();
      যদি (unlikely (পরবর্তী_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 পরবর্তী_space);
        ফেরত Err(LIMIT_EXCEEDED);
      }
      অঙ্ক_roots_for_space.push (0);
      TRY(graph_ফলাফল_t<শূন্য>::from (অঙ্ক_roots_for_space, ALLOCATION_FAILURE));

      জন্য (unsigned মূল : connected_roots)
      {
        DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Subgraph %u gets space %u", মূল, পরবর্তী_space);
        vertices_[মূল].space = পরবর্তী_space;
        অঙ্ক_roots_for_space[পরবর্তী_space] = অঙ্ক_roots_for_space[পরবর্তী_space] + 1;
        distance_invalid = সত্য;
        positions_invalid = সত্য;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    ফেরত Ok(সত্য);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  graph_ফলাফল_t<যৌক্তিক> isolate_subgraph (hb_set_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_set_t parents;
    জন্য (unsigned মূল_idx : roots)
    {
      যদি (unlikely (মূল_idx >= vertices_.দৈর্ঘ্য))
        ফেরত Err(OUT_OF_BOUNDS);
      subgraph.set (মূল_idx, wide_parents (মূল_idx, parents));
    }
    জন্য (unsigned মূল_idx : roots)
    {
      TRY (find_subgraph (মূল_idx, subgraph));
    }
    TRY (graph_ফলাফল_t<শূন্য>::from (subgraph, ALLOCATION_FAILURE));

    hb_map_t সূচক_map;
    যৌক্তিক made_changes = মিথ্যা;
    জন্য (auto entry : subgraph.iter ())
    {
      প্রতিজ্ঞা (entry.first < vertices_.দৈর্ঘ্য);
      ধ্রুবক auto& node = vertices_[entry.first];
      unsigned subgraph_incoming_edges = entry.second;

      যদি (subgraph_incoming_edges < node.incoming_edges ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = সত্য;
        TRY (duplicate_subgraph (entry.first, সূচক_map));
      }
    }

    TRY (graph_ফলাফল_t<শূন্য>::from (সূচক_map, ALLOCATION_FAILURE));

    যদি (!made_changes)
      ফেরত Ok(মিথ্যা);

    auto new_subgraph =
        + subgraph.চাবিগুলি ()
        | hb_map([&] (uint32_t গ্রন্থি_idx) {
          ধ্রুবক uint32_t *v;
          যদি (সূচক_map.has (গ্রন্থি_idx, &v)) ফেরত *v;
          ফেরত গ্রন্থি_idx;
        })
        ;

    TRY(remap_obj_indices (সূচক_map, new_subgraph));
    TRY(remap_obj_indices (সূচক_map, parents.iter (), সত্য));

    // Update roots set with new indices as needed.
    জন্য (auto next : roots)
    {
      ধ্রুবক uint32_t *v;
      যদি (সূচক_map.has (next, &v))
      {
        roots.del (next);
        roots.add (*v);
      }
    }

    ফেরত Ok(সত্য);
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
  graph_ফলাফল_t<শূন্য> traverse_directed_bfs (unsigned start_idx, VisitEdgeFunc&& visit_edge)
  {
    যদি (unlikely (start_idx >= vertices_.দৈর্ঘ্য))
    {
      DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      ফেরত Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.দৈর্ঘ্য);
    TRY (graph_ফলাফল_t<শূন্য>::from (ordering_scratch_, ALLOCATION_FAILURE));

    যদি (!visit_edge (HB_CODEPOINT_INVALID, nullptr, start_idx, 0))
      ফেরত Ok();

    unsigned শীর্ষ_2 = 0;
    unsigned লেজ = 0;
    auto& queue = ordering_scratch_;
    queue[লেজ++] = start_idx;

    unsigned depth = 0;
    যখন (শীর্ষ_2 < লেজ)
    {
      unsigned level_end = লেজ;
      যখন (শীর্ষ_2 < level_end)
      {
        unsigned গ্রন্থি_idx = queue[শীর্ষ_2++];
        ধ্রুবক auto& v = vertices_[গ্রন্থি_idx];

        unsigned অঙ্ক_real = v.obj().real_links.দৈর্ঘ্য;
        unsigned যোগ_links = অঙ্ক_real + v.obj().virtual_links.দৈর্ঘ্য;

        জন্য (unsigned i = 0; i < যোগ_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          ধ্রুবক auto& v = vertices_[গ্রন্থি_idx];
          ধ্রুবক auto& link = (i < অঙ্ক_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - অঙ্ক_real];
          unsigned child_idx = link.objidx;

          যদি (!visit_edge (গ্রন্থি_idx, &link, child_idx, depth + 1))
            চালু;

          যদি (unlikely (লেজ >= queue.দৈর্ঘ্য))
            ফেরত Err(CYCLE_DETECTED);

          queue[লেজ++] = child_idx;
        }
      }
      depth++;
    }
    ফেরত Ok();
  }

  graph_ফলাফল_t<শূন্য> find_subgraph (unsigned গ্রন্থি_idx, hb_map_t& subgraph)
  {
    ফেরত traverse_directed_bfs (গ্রন্থি_idx, [&] (
      unsigned parent,
      ধ্রুবক hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      যদি (depth == 0) ফেরত সত্য;
      hb_codepoint_t *গুনতি;
      যদি (subgraph.has (child, &গুনতি))
      {
        (*গুনতি)++;
        ফেরত মিথ্যা;
      }
      subgraph.set (child, 1);
      ফেরত সত্য;
    });
  }

  graph_ফলাফল_t<শূন্য> find_subgraph (unsigned গ্রন্থি_idx, hb_set_t& subgraph)
  {
    ফেরত traverse_directed_bfs (গ্রন্থি_idx, [&] (
      unsigned parent,
      ধ্রুবক hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      যদি (subgraph.has (child)) ফেরত মিথ্যা;
      subgraph.add (child);
      ফেরত সত্য;
    });
  }

  graph_ফলাফল_t<size_t> find_subgraph_আকার (unsigned গ্রন্থি_idx, hb_set_t& subgraph, unsigned max_depth = HB_GRAPH_INVALID)
  {
    size_t আকার_2 = 0;
    TRY (traverse_directed_bfs (গ্রন্থি_idx, [&] (
      unsigned parent,
      ধ্রুবক hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      যদি (subgraph.has (child)) ফেরত মিথ্যা;
      subgraph.add (child);

      ধ্রুবক auto& o = vertices_[child].obj();
      আকার_2 += o.লেজ - o.শীর্ষ_2;
      ফেরত depth < max_depth;
    }));
    ফেরত Ok(আকার_2);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  graph_ফলাফল_t<শূন্য> find_32bit_roots (unsigned গ্রন্থি_idx, hb_set_t& found)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_set_t visited;
    ফেরত traverse_directed_bfs (গ্রন্থি_idx, [&] (
      unsigned parent,
      ধ্রুবক hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {

      যদি (link && found.has(parent))
        // Don't traverse from something that's already marked as a root.
        ফেরত মিথ্যা;

      যদি (link && !link->is_signed && link->width == 4)
      {
        found.add (link->objidx);
        visited.add (link->objidx);
        ফেরত মিথ্যা;
      }

      যদি (visited.has (child)) ফেরত মিথ্যা;
      visited.add (child);
      ফেরত সত্য;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  template<typename O>
  graph_ফলাফল_t<unsigned> move_child (unsigned old_parent_idx,
                                       ধ্রুবক O* old_offset,
                                       unsigned new_parent_idx,
                                       ধ্রুবক O* new_offset)
  {
    distance_invalid = সত্য;
    positions_invalid = সত্য;

    যদি (unlikely (old_parent_idx >= vertices_.দৈর্ঘ্য ||
                  new_parent_idx >= vertices_.দৈর্ঘ্য))
      ফেরত Err(OUT_OF_BOUNDS);

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    TRY_ASSIGN (unsigned child_id, সূচক_for_offset (old_parent_idx,
                                                     old_offset));
    যদি (unlikely (child_id >= vertices_.দৈর্ঘ্য))
      ফেরত Err(OUT_OF_BOUNDS);

    TRY(new_v.add_real_link(O::static_আকার, child_id, (ধ্রুবক অক্ষর*) new_offset - (ধ্রুবক অক্ষর*) new_v.obj().শীর্ষ_2));

    auto& child = vertices_[child_id];
    TRY(child.add_parent (new_parent_idx, মিথ্যা));

    old_v.remove_real_link_unordered (child_id, old_offset);
    child.remove_parent (old_parent_idx);

    ফেরত Ok(child_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  template<typename O>
  graph_ফলাফল_t<শূন্য> move_children (unsigned old_parent_idx,
                                      unsigned old_pos_start,
                                      unsigned old_pos_end,
                                      unsigned new_parent_idx,
                                      unsigned new_pos_start)
  {
    যদি (unlikely (old_parent_idx >= vertices_.দৈর্ঘ্য ||
                  new_parent_idx >= vertices_.দৈর্ঘ্য))
      ফেরত Err(OUT_OF_BOUNDS);

    distance_invalid = সত্য;
    positions_invalid = সত্য;

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    hb_vector_t<hb_serialize_context_t::object_t::link_t> old_links;
    জন্য (ধ্রুবক auto& l : old_v.obj().real_links)
    {
      যদি (l.position < old_pos_start || l.position >= old_pos_end)
      {
        old_links.push (l);
        চালু;
      }

      unsigned array_pos = l.position - old_pos_start;

      unsigned child_id = l.objidx;
      যদি (unlikely (child_id >= vertices_.দৈর্ঘ্য))
        ফেরত Err(OUT_OF_BOUNDS);

      TRY(new_v.add_real_link(O::static_আকার, child_id, new_pos_start + array_pos));

      auto& child = vertices_[child_id];
      TRY(child.add_parent (new_parent_idx, মিথ্যা));
      child.remove_parent (old_parent_idx);
    }

    TRY (graph_ফলাফল_t<শূন্য>::from (old_links, ALLOCATION_FAILURE));
    ফেরত old_v.set_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  graph_ফলাফল_t<শূন্য> duplicate_subgraph (unsigned গ্রন্থি_idx, hb_map_t& সূচক_map)
  {
    graph_ফলাফল_t<শূন্য> ফলাফল = Ok();
    TRY (traverse_directed_bfs (গ্রন্থি_idx, [&] (
      unsigned parent,
      ধ্রুবক hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {
      যদি (ফলাফল.is_err () || সূচক_map.has (child)) ফেরত মিথ্যা;
      auto r = duplicate (child);
      যদি (unlikely (r.is_err ()))
      {
        ফলাফল = Err(r.error ());
        ফেরত মিথ্যা;
      }
      সূচক_map.set (child, *r);
      ফেরত সত্য;
    }));

    ফেরত graph_ফলাফল_t<শূন্য>::from(সূচক_map, ALLOCATION_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  graph_ফলাফল_t<unsigned> duplicate (unsigned গ্রন্থি_idx, যৌক্তিক copy_table = মিথ্যা)
  {
    যদি (unlikely (গ্রন্থি_idx >= vertices_.দৈর্ঘ্য))
      ফেরত Err(OUT_OF_BOUNDS);

    যদি (unlikely (vertices_.দৈর্ঘ্য >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.দৈর্ঘ্য);
      ফেরত Err(LIMIT_EXCEEDED);
    }

    positions_invalid = সত্য;
    distance_invalid = সত্য;

    auto* clone = vertices_.push ();
    TRY (graph_ফলাফল_t<শূন্য>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.দৈর্ঘ্য - 1;
    ordering_.push (clone_idx);
    TRY (graph_ফলাফল_t<শূন্য>::from (ordering_, ALLOCATION_FAILURE));

    auto& child = vertices_[গ্রন্থি_idx];

    unsigned table_আকার = child.obj().লেজ - child.obj().শীর্ষ_2;
    যদি (copy_table && table_আকার)
    {
      অক্ষর* বাফার = (অক্ষর*) hb_malloc (table_আকার);
      যদি (unlikely (!বাফার))
        ফেরত Err(ALLOCATION_FAILURE);
      auto res = add_বাফার (বাফার);
      যদি (unlikely (res.is_err ()))
      {
        hb_free (বাফার);
        ফেরত Err(res.error ());
      }
      hb_memcpy (বাফার, child.obj().শীর্ষ_2, table_আকার);
      clone->set_বাফার(বাফার, বাফার + table_আকার);
    }
    অন্যথা
    {
      clone->set_বাফার(child.obj().শীর্ষ_2, child.obj().লেজ);
    }
    clone->distance = child.distance;
    clone->space = child.space;
    clone->reset_parents ();

    জন্য (ধ্রুবক auto& l : child.obj().real_links)
    {
      TRY(clone->add_real_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, মিথ্যা));
    }

    জন্য (ধ্রুবক auto& l : child.obj().virtual_links)
    {
      TRY(clone->add_virtual_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, সত্য));
    }

    ফেরত Ok(clone_idx);
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
  graph_ফলাফল_t<unsigned> duplicate (unsigned parent_idx, unsigned child_idx, যৌক্তিক copy_table = মিথ্যা)
  {
    যদি (unlikely (parent_idx >= vertices_.দৈর্ঘ্য || child_idx >= vertices_.দৈর্ঘ্য))
      ফেরত Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    ধ্রুবক auto& child = vertices_[child_idx];
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
      DEBUG_বার্তা (SUBSET_REPACK, nullptr, "  Not duplicating %u => %u",
                 parent_idx, child_idx);
      ফেরত Err(INVALID_ARGUMENT);
    }

    DEBUG_বার্তা (SUBSET_REPACK, nullptr, "  Duplicating %u => %u",
               parent_idx, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx, copy_table));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    যদি (parent_idx == clone_idx) parent_idx++;

    auto& parent = vertices_[parent_idx];
    unsigned গুনতি = 0;
    unsigned অঙ্ক_real = parent.obj().real_links.দৈর্ঘ্য;
    জন্য (auto& l : parent.all_links_writer ())
    {
      গুনতি++;
      যদি (l.objidx != child_idx)
        চালু;

      TRY(reassign_link (l, parent_idx, clone_idx, গুনতি > অঙ্ক_real));
    }

    ফেরত Ok(clone_idx);
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
  graph_ফলাফল_t<unsigned> duplicate (ধ্রুবক hb_set_t* parents, unsigned child_idx)
  {
    যদি (unlikely (!parents || parents->is_empty ()))
      ফেরত Err(INVALID_ARGUMENT);

    যদি (unlikely (child_idx >= vertices_.দৈর্ঘ্য))
      ফেরত Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    ধ্রুবক auto& child = vertices_[child_idx];
    unsigned links_to_child = 0;
    unsigned last_parent = parents->get_max ();
    unsigned first_parent = parents->get_min ();
    জন্য (unsigned parent_idx : *parents) {
      যদি (unlikely (parent_idx >= vertices_.দৈর্ঘ্য))
        ফেরত Err(OUT_OF_BOUNDS);
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
      DEBUG_বার্তা (SUBSET_REPACK, nullptr, "  Not duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);
      ফেরত Err(INVALID_ARGUMENT);
    }

    DEBUG_বার্তা (SUBSET_REPACK, nullptr, "  Duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx));

    জন্য (unsigned parent_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      যদি (parent_idx == clone_idx) parent_idx++;
      auto& parent = vertices_[parent_idx];
      unsigned গুনতি = 0;
      unsigned অঙ্ক_real = parent.obj().real_links.দৈর্ঘ্য;
      জন্য (auto& l : parent.all_links_writer ())
      {
        গুনতি++;
        যদি (l.objidx != child_idx)
          চালু;

        TRY(reassign_link (l, parent_idx, clone_idx, গুনতি > অঙ্ক_real));
      }
    }

    ফেরত Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  graph_ফলাফল_t<unsigned> new_গ্রন্থি (অক্ষর* শীর্ষ_2, অক্ষর* লেজ)
  {
    যদি (unlikely (vertices_.দৈর্ঘ্য >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.দৈর্ঘ্য);
      ফেরত Err(LIMIT_EXCEEDED);
    }

    positions_invalid = সত্য;
    distance_invalid = সত্য;

    auto* clone = vertices_.push ();
    TRY (graph_ফলাফল_t<শূন্য>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.দৈর্ঘ্য - 1;
    ordering_.push (clone_idx);
    TRY (graph_ফলাফল_t<শূন্য>::from (ordering_, ALLOCATION_FAILURE));

    clone->set_বাফার (শীর্ষ_2, লেজ);
    clone->distance = 0;
    clone->space = 0;

    ফেরত Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  graph_ফলাফল_t<unsigned> remap_child (unsigned parent_idx, unsigned old_child_idx)
  {
    যদি (unlikely (parent_idx >= vertices_.দৈর্ঘ্য || old_child_idx >= vertices_.দৈর্ঘ্য))
      ফেরত Err(OUT_OF_BOUNDS);

    TRY_ASSIGN (unsigned new_child_idx, duplicate (old_child_idx));

    auto& parent = vertices_[parent_idx];
    জন্য (auto& l : parent.real_links_writer ())
    {
      যদি (l.objidx != old_child_idx)
        চালু;
      TRY(reassign_link (l, parent_idx, new_child_idx, মিথ্যা));
    }

    জন্য (auto& l : parent.virtual_links_writer ())
    {
      যদি (l.objidx != old_child_idx)
        চালু;
      TRY(reassign_link (l, parent_idx, new_child_idx, সত্য));
    }
    ফেরত Ok(new_child_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  graph_ফলাফল_t<যৌক্তিক> raise_childrens_priority (unsigned parent_idx)
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr, "  Raising priority of all children of %u",
               parent_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    যদি (unlikely (parent_idx >= vertices_.দৈর্ঘ্য)) ফেরত Err(OUT_OF_BOUNDS);
    যৌক্তিক made_change = মিথ্যা;
    জন্য (auto& l : vertices_[parent_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_priority ();
    ফেরত Ok(made_change);
  }

  graph_ফলাফল_t<শূন্য> is_fully_connected ()
  {
    TRY(update_parents());

    যদি (unlikely (মূল().incoming_edges ()))
      // Root cannot have parents.
      ফেরত Err(ORPHANED_NODES);

    জন্য (unsigned i = 0; i < মূল_idx (); i++)
    {
      যদি (unlikely (!vertices_[i].incoming_edges ()))
        ফেরত Err(ORPHANED_NODES);
    }
    ফেরত Ok();
  }

#যদি 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  শূন্য save_fuzzer_seed (hb_tag_t tag) ধ্রুবক
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((শূন্য*) &tag, আকার (tag), 1, f);

    uint16_t অঙ্ক_objects = vertices_.দৈর্ঘ্য;
    fwrite ((শূন্য*) &অঙ্ক_objects, আকার (অঙ্ক_objects), 1, f);

    জন্য (ধ্রুবক auto& v : vertices_)
    {
      uint16_t blob_আকার = v.table_আকার ();
      fwrite ((শূন্য*) &blob_আকার, আকার (blob_আকার), 1, f);
      fwrite ((ধ্রুবক শূন্য*) v.obj.শীর্ষ_2, blob_আকার, 1, f);
    }

    uint16_t link_গুনতি = 0;
    জন্য (ধ্রুবক auto& v : vertices_)
      link_গুনতি += v.obj.real_links.দৈর্ঘ্য;

    fwrite ((শূন্য*) &link_গুনতি, আকার (link_গুনতি), 1, f);

    প্রকারলেখ কাঠামো
    {
      uint16_t parent;
      uint16_t child;
      uint16_t position;
      uint8_t width;
    } link_t;

    জন্য (unsigned i = 0; i < vertices_.দৈর্ঘ্য; i++)
    {
      জন্য (ধ্রুবক auto& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.width
        };
        fwrite ((শূন্য*) &link, আকার (link), 1, f);
      }
    }

    fclose (f);
  }
#endif

  unsigned অঙ্ক_roots_for_space_2 (unsigned space) ধ্রুবক
  {
    ফেরত অঙ্ক_roots_for_space[space];
  }

  unsigned পরবর্তী_space () ধ্রুবক
  {
    ফেরত অঙ্ক_roots_for_space.দৈর্ঘ্য;
  }

  graph_ফলাফল_t<শূন্য> move_to_new_space (ধ্রুবক hb_set_t& indices)
  {
    যদি (unlikely (অঙ্ক_roots_for_space.দৈর্ঘ্য >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 অঙ্ক_roots_for_space.দৈর্ঘ্য);
      ফেরত Err(LIMIT_EXCEEDED);
    }
    অঙ্ক_roots_for_space.push (0);
    TRY (graph_ফলাফল_t<শূন্য>::from (অঙ্ক_roots_for_space, ALLOCATION_FAILURE));
    unsigned new_space = অঙ্ক_roots_for_space.দৈর্ঘ্য - 1;

    জন্য (unsigned সূচক_2 : indices) {
      যদি (unlikely (সূচক_2 >= vertices_.দৈর্ঘ্য))
        ফেরত Err(OUT_OF_BOUNDS);
      auto& node = vertices_[সূচক_2];
      অঙ্ক_roots_for_space[node.space] = অঙ্ক_roots_for_space[node.space] - 1;
      অঙ্ক_roots_for_space[new_space] = অঙ্ক_roots_for_space[new_space] + 1;
      node.space = new_space;
      distance_invalid = সত্য;
      positions_invalid = সত্য;
    }
    ফেরত Ok();
  }

  unsigned space_for (unsigned সূচক_2, unsigned* মূল = nullptr) ধ্রুবক
  {
    যখন (সত্য)
    {
      প্রতিজ্ঞা (সূচক_2 < vertices_.দৈর্ঘ্য);
      ধ্রুবক auto& node = vertices_[সূচক_2];
      যদি (node.space)
      {
        যদি (মূল != nullptr)
          *মূল = সূচক_2;
        ফেরত node.space;
      }

      যদি (!node.incoming_edges ())
      {
        যদি (মূল)
          *মূল = সূচক_2;
        ফেরত 0;
      }

      সূচক_2 = *node.parents_iter ();
    }
  }

  size_t যোগ_আকার_in_bytes () ধ্রুবক {
    size_t যোগ_আকার = 0;
    unsigned গুনতি = vertices_.দৈর্ঘ্য;
    জন্য (unsigned i = 0; i < গুনতি; i++) {
      size_t আকার_2 = vertices_.arrayZ[i].table_আকার ();
      যোগ_আকার += আকার_2;
    }
    ফেরত যোগ_আকার;
  }


 ব্যক্তিগত:

  শূন্য print_orphaned_nodes ()
  {
    যদি (!DEBUG_ENABLED(SUBSET_REPACK)) ফেরত;

    DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Graph is not fully connected.");

    parents_invalid = সত্য;
    (শূন্য) update_parents();

    যদি (মূল().incoming_edges ()) {
      DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Root node has incoming edges.");
    }

    জন্য (unsigned i = 0; i < vertices_.দৈর্ঘ্য; i++)
    {
      ধ্রুবক auto& v = vertices_[i];
      যদি (!v.incoming_edges ())
        DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  unsigned wide_parents (unsigned গ্রন্থি_idx, hb_set_t& parents) ধ্রুবক
  {
    unsigned গুনতি = 0;
    জন্য (unsigned p : vertices_[গ্রন্থি_idx].parents_iter ())
    {
      // Only real links can be wide
      জন্য (ধ্রুবক auto& l : vertices_[p].obj().real_links)
      {
        যদি (l.objidx == গ্রন্থি_idx
            && (l.width == 3 || l.width == 4)
            && !l.is_signed)
        {
          গুনতি++;
          parents.add (p);
        }
      }
    }
    ফেরত গুনতি;
  }

 সর্বজনীন:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  graph_ফলাফল_t<শূন্য> update_parents ()
  {
    যদি (!parents_invalid) ফেরত Ok();

    unsigned গুনতি = vertices_.দৈর্ঘ্য;

    জন্য (unsigned i = 0; i < গুনতি; i++)
      vertices_.arrayZ[i].reset_parents ();

    জন্য (unsigned p = 0; p < গুনতি; p++)
    {
      জন্য (ধ্রুবক auto& l : vertices_.arrayZ[p].obj().real_links)
      {
        যদি (unlikely (l.objidx >= গুনতি)) ফেরত Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, মিথ্যা));
      }

      জন্য (ধ্রুবক auto& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        যদি (unlikely (l.objidx >= গুনতি)) ফেরত Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, সত্য));
      }
    }

    parents_invalid = মিথ্যা;
    ফেরত Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  শূন্য update_positions ()
  {
    যদি (!positions_invalid) ফেরত;

    unsigned current_pos = 0;
    জন্য (unsigned i : ordering_)
    {
      auto& v = vertices_[i];
      v.start = current_pos;
      current_pos += v.obj().লেজ - v.obj().শীর্ষ_2;
      v.end = current_pos;
    }

    positions_invalid = মিথ্যা;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  graph_ফলাফল_t<শূন্য> update_distances ()
  {
    যদি (!distance_invalid) ফেরত Ok();

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
    unsigned গুনতি = vertices_.দৈর্ঘ্য;
    জন্য (unsigned i = 0; i < গুনতি; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[মূল_idx ()].distance = 0;

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (গুনতি);
    queue.insert (0, মূল_idx ());
    TRY (graph_ফলাফল_t<শূন্য>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<যৌক্তিক> visited;
    visited.resize (vertices_.দৈর্ঘ্য);
    TRY (graph_ফলাফল_t<শূন্য>::from (visited, ALLOCATION_FAILURE));

    যখন (!queue.in_error () && !queue.is_empty ())
    {
      unsigned পরবর্তী_idx = queue.বের_করো_minimum ().second;
      যদি (visited[পরবর্তী_idx]) চালু;
      ধ্রুবক auto& next = vertices_[পরবর্তী_idx];
      int64_t পরবর্তী_distance = next.distance;
      visited[পরবর্তী_idx] = সত্য;

      জন্য (ধ্রুবক auto& link : next.obj().all_links ())
      {
        যদি (unlikely (link.objidx >= vertices_.দৈর্ঘ্য))
          ফেরত Err(OUT_OF_BOUNDS);

        যদি (visited[link.objidx]) চালু;

        auto& child_v = vertices_.arrayZ[link.objidx];
        ধ্রুবক auto& child = child_v.obj ();
        unsigned link_width = link.width ? link.width : 4; // treat virtual offsets as 32 bits wide
        int64_t child_weight = (child.লেজ - child.শীর্ষ_2) +
                               ((int64_t) 1 << (link_width * 8)) * (child_v.space + 1);
        int64_t child_distance = পরবর্তী_distance + child_weight;

        যদি (child_distance < child_v.distance)
        {
          child_v.distance = child_distance;
          queue.insert (child_distance, link.objidx);
        }
      }
    }

    TRY (graph_ফলাফল_t<শূন্য>::from (queue, ALLOCATION_FAILURE));
    যদি (unlikely (!queue.is_empty ()))
    {
      print_orphaned_nodes ();
      ফেরত Err(ORPHANED_NODES);
    }

    distance_invalid = মিথ্যা;
    ফেরত Ok();
  }

 ব্যক্তিগত:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  graph_ফলাফল_t<শূন্য> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      unsigned parent_idx,
                      unsigned new_idx,
                      যৌক্তিক is_virtual)
  {
    unsigned old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_parent (parent_idx);
    ফেরত vertices_[new_idx].add_parent (parent_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  template<typename Iterator, hb_requires (hb_is_iterator (Iterator))>
  graph_ফলাফল_t<শূন্য> remap_obj_indices (ধ্রুবক hb_map_t& id_map,
                                          Iterator subgraph,
                                          যৌক্তিক only_wide = মিথ্যা)
  {
    যদি (!id_map) ফেরত Ok();
    জন্য (unsigned i : subgraph)
    {
      auto& vertex = vertices_[i];
      unsigned অঙ্ক_real = vertex.obj().real_links.দৈর্ঘ্য;
      unsigned গুনতি = 0;
      জন্য (auto& link : vertex.all_links_writer ())
      {
        গুনতি++;
        ধ্রুবক uint32_t *v;
        যদি (!id_map.has (link.objidx, &v)) চালু;
        যদি (only_wide && (link.is_signed || (link.width != 4 && link.width != 3))) চালু;

        TRY(reassign_link (link, i, *v, গুনতি > অঙ্ক_real));
      }
    }
    ফেরত Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  graph_ফলাফল_t<শূন্য> find_connected_nodes (unsigned start_idx,
                                             hb_set_t& targets,
                                             hb_set_t& visited,
                                             hb_set_t& connected)
  {
    TRY (graph_ফলাফল_t<শূন্য>::from (visited, ALLOCATION_FAILURE));
    যদি (visited.has (start_idx)) ফেরত Ok();
    যদি (unlikely (start_idx >= vertices_.দৈর্ঘ্য))
    {
      DEBUG_বার্তা (SUBSET_REPACK, nullptr,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      ফেরত Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.দৈর্ঘ্য);
    TRY (graph_ফলাফল_t<শূন্য>::from (ordering_scratch_, ALLOCATION_FAILURE));

    auto& স্তূপ = ordering_scratch_;
    unsigned স্তূপ_len = 0;

    graph_ফলাফল_t<শূন্য> res = Ok();
    auto handle_গ্রন্থি = [&] (unsigned গ্রন্থি_idx) {
      visited.add (গ্রন্থি_idx);
      যদি (targets.has (গ্রন্থি_idx)) {
        targets.del (গ্রন্থি_idx);
        connected.add (গ্রন্থি_idx);
      }

      যদি (unlikely (স্তূপ_len >= স্তূপ.দৈর্ঘ্য))
      {
        res = Err(CYCLE_DETECTED);
        ফেরত মিথ্যা;
      }
      স্তূপ[স্তূপ_len++] = গ্রন্থি_idx;
      ফেরত সত্য;
    };

    যদি (!handle_গ্রন্থি (start_idx)) ফেরত res;

    যখন (স্তূপ_len > 0)
    {
      unsigned গ্রন্থি_idx = স্তূপ[--স্তূপ_len];
      ধ্রুবক auto& v = vertices_[গ্রন্থি_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      জন্য (ধ্রুবক auto& l : v.obj().all_links ())
      {
        unsigned child_idx = l.objidx;
        যদি (visited.has (child_idx)) চালু;
        যদি (!handle_গ্রন্থি (child_idx)) ফেরত res;
      }

      জন্য (unsigned parent_idx : v.parents_iter ())
      {
        যদি (visited.has (parent_idx)) চালু;
        যদি (!handle_গ্রন্থি (parent_idx)) ফেরত res;
      }
    }

    TRY (graph_ফলাফল_t<শূন্য>::from (visited, ALLOCATION_FAILURE));
    TRY (graph_ফলাফল_t<শূন্য>::from (connected, ALLOCATION_FAILURE));

    ফেরত Ok();
  }

 সর্বজনীন:
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

 ব্যক্তিগত:
  যৌক্তিক parents_invalid = সত্য;
  যৌক্তিক distance_invalid = সত্য;
  যৌক্তিক positions_invalid = সত্য;
  hb_vector_t<unsigned> অঙ্ক_roots_for_space;
  hb_vector_t<অক্ষর*> buffers;
};

}

#endif  // GRAPH_GRAPH_HH
