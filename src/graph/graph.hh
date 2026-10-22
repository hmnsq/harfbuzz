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

#അന്തര്ഭാവ "../hb-set.hh"
#അന്തര്ഭാവ "../hb-priority-queue.hh"
#അന്തര്ഭാവ "../hb-serialize.hh"
#അന്തര്ഭാവ "graph-result.hh"

#ifndef GRAPH_GRAPH_HH
#define GRAPH_GRAPH_HH

#define HB_GRAPH_INVALID ((unsigned) -1)

നാമാകാശ graph {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
ഘടന graph_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  ഘടന vertex_t
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

    സ്വകാര്യ:
    hb_serialize_context_t::object_t obj_;
    unsigned incoming_edges_ = 0;
    unsigned single_parent = HB_GRAPH_INVALID;
    തർക്കം has_incoming_virtual_edges_ = അസത്യം;
    hb_hashmap_t<unsigned, unsigned> parents;
    hb_set_t virtual_parents;
    പൊതു:

    vertex_t ()
    {}

    explicit vertex_t(സ്ഥിരം hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    auto parents_iter () സ്ഥിരം HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_parent, single_parent != HB_GRAPH_INVALID),
	parents.താക്കോലുകൾ_ref ()
      )
    )

    ശൂന്യം set_buffer (അക്ഷരം* head, അക്ഷരം* tail) {
      obj_.head = head;
      obj_.tail = tail;
    }

    ശൂന്യം shrink_buffer (size_t എണ്ണം)
    {
      obj_.tail -= എണ്ണം;
    }

    auto all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    auto real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    auto virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    ശൂന്യം sort_real_links () {
      obj_.real_links.qsort ();
    }

    graph_result_t<ശൂന്യം> add_real_link (unsigned width, unsigned obj_idx, unsigned position) {
      auto* link = obj_.real_links.push ();
      TRY (graph_result_t<ശൂന്യം>::from (obj_.real_links, ALLOCATION_FAILURE));
      link->width = width;
      link->objidx = obj_idx;
      link->position = position;
      തിരികെ Ok();
    }

    graph_result_t<ശൂന്യം> add_virtual_link (unsigned obj_idx) {
      auto* link = obj_.virtual_links.push ();
      TRY (graph_result_t<ശൂന്യം>::from (obj_.virtual_links, ALLOCATION_FAILURE));
      link->objidx = obj_idx;
      തിരികെ Ok();
    }

    graph_result_t<ശൂന്യം> add_real_link (സ്ഥിരം hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      തിരികെ graph_result_t<ശൂന്യം>::from (obj_.real_links, ALLOCATION_FAILURE);
    }

    graph_result_t<ശൂന്യം> set_real_links (hb_vector_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(graph_result_t<ശൂന്യം>::from (links, ALLOCATION_FAILURE));
      obj_.real_links = links;
      തിരികെ Ok();
    }

    ശൂന്യം clear_real_links ()
    {
      obj_.real_links.reset();
    }

    ശൂന്യം clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    graph_result_t<ശൂന്യം> add_virtual_link (സ്ഥിരം hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      തിരികെ graph_result_t<ശൂന്യം>::from (obj_.virtual_links, ALLOCATION_FAILURE);
    }

    സ്ഥിരം hb_serialize_context_t::object_t& obj() സ്ഥിരം { തിരികെ obj_; }

    template<typename T>
    T as () സ്ഥിരം
    {
      തിരികെ reinterpret_cast<T> (obj_.head);
    }

    തർക്കം has_incoming_virtual_edges () സ്ഥിരം
    {
      തിരികെ has_incoming_virtual_edges_;
    }

    graph_result_t<ശൂന്യം> link_positions_valid (unsigned അക്കം_objects, തർക്കം removed_nil)
    {
      hb_set_t assigned_bytes;
      വേണ്ടി (സ്ഥിരം auto& l : obj_.real_links)
      {
        എങ്കിൽ (unlikely (l.objidx >= അക്കം_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid object index.");
          തിരികെ Err(INVALID_ARGUMENT);
        }

        unsigned start = l.position;
        unsigned end = start + l.width - 1;

        എങ്കിൽ (unlikely (l.width < 2 || l.width > 4))
        {
          DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid link width.");
          തിരികെ Err(INVALID_ARGUMENT);
        }

        എങ്കിൽ (unlikely (end >= table_size ()))
        {
          DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                     "Invalid graph. Link position is out of bounds.");
          തിരികെ Err(OUT_OF_BOUNDS);
        }

        എങ്കിൽ (unlikely (assigned_bytes.intersects (start, end)))
        {
          DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                     "Invalid graph. Found offsets whose positions overlap.");
          തിരികെ Err(INVALID_ARGUMENT);
        }

        assigned_bytes.add_range (start, end);
      }

      തിരികെ graph_result_t<ശൂന്യം>::from (assigned_bytes, ALLOCATION_FAILURE);
    }

    ശൂന്യം normalize ()
    {
      obj_.real_links.qsort ();
      വേണ്ടി (സ്ഥിരം auto& l : obj_.real_links)
      {
        വേണ്ടി (unsigned i = 0; i < l.width; i++)
        {
          obj_.head[l.position + i] = 0;
        }
      }
    }

    തർക്കം equals (unsigned this_index,
                 unsigned other_index,
                 സ്ഥിരം vertex_t& other,
                 സ്ഥിരം graph_t& graph,
                 സ്ഥിരം graph_t& other_graph,
                 unsigned depth) സ്ഥിരം
    {
      എങ്കിൽ (!(as_bytes () == other.as_bytes ()))
      {
        DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_index,
                   (unsigned long) table_size (),
                   other_index,
                   (unsigned long) other.table_size (),
                   depth);

        auto a = as_bytes ();
        auto b = other.as_bytes ();
        ആകുമ്പോൾ (a || b)
        {
          DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                     "  0x%x %s 0x%x", (unsigned) *a, (*a == *b) ? "==" : "!=", (unsigned) *b);
          a++;
          b++;
        }
        തിരികെ അസത്യം;
      }

      തിരികെ links_equal (obj_.real_links, other.obj_.real_links, graph, other_graph, depth);
    }

    hb_bytes_t as_bytes () സ്ഥിരം
    {
      തിരികെ hb_bytes_t (obj_.head, table_size ());
    }

    friend ശൂന്യം swap (vertex_t& a, vertex_t& b)
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
    position_to_index_map () സ്ഥിരം
    {
      hb_hashmap_t<unsigned, unsigned> result;

      result.alloc (obj_.real_links.length);
      വേണ്ടി (സ്ഥിരം auto& l : obj_.real_links) {
        result.set (l.position, l.objidx);
      }

      തിരികെ graph_result_t<hb_hashmap_t<unsigned, unsigned>>::from (std::move(result), ALLOCATION_FAILURE);
    }

    തർക്കം is_shared () സ്ഥിരം
    {
      തിരികെ parents.get_population () > virtual_parents.get_population () + 1;
    }

    unsigned incoming_edges () സ്ഥിരം
    {
      എങ്കിൽ (HB_DEBUG_SUBSET_REPACK)
       {
	പ്രതിജ്ഞാ (incoming_edges_ == (single_parent != HB_GRAPH_INVALID) +
		(parents.മൂല്യങ്ങൾ_ref () | hb_reduce (hb_add, 0)));
       }
      തിരികെ incoming_edges_;
    }

    unsigned incoming_edges_from_parent (unsigned parent_index) സ്ഥിരം {
      എങ്കിൽ (single_parent != HB_GRAPH_INVALID) {
        തിരികെ single_parent == parent_index ? 1 : 0;
      }

      unsigned* എണ്ണം;
      തിരികെ  parents.has(parent_index, &എണ്ണം) ? *എണ്ണം : 0;
    }

    ശൂന്യം reset_parents ()
    {
      incoming_edges_ = 0;
      has_incoming_virtual_edges_ = അസത്യം;
      single_parent = HB_GRAPH_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    graph_result_t<ശൂന്യം> add_parent (unsigned parent_index, തർക്കം is_virtual)
    {
      പ്രതിജ്ഞാ (parent_index != HB_GRAPH_INVALID);
      has_incoming_virtual_edges_ |= is_virtual;
      എങ്കിൽ (is_virtual)
        virtual_parents.add (parent_index);

      എങ്കിൽ (incoming_edges_ == 0)
      {
	single_parent = parent_index;
	incoming_edges_ = 1;
	തിരികെ to_result();
      }
      അല്ലെങ്കിൽ എങ്കിൽ (single_parent != HB_GRAPH_INVALID)
      {
        പ്രതിജ്ഞാ (incoming_edges_ == 1);
	എങ്കിൽ (!parents.set (single_parent, 1))
	  തിരികെ to_result ();
	single_parent = HB_GRAPH_INVALID;
      }

      unsigned *v;
      എങ്കിൽ (parents.has (parent_index, &v))
      {
        (*v)++;
	incoming_edges_++;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (parents.set (parent_index, 1))
	incoming_edges_++;

      തിരികെ to_result();
    }

    ശൂന്യം remove_parent (unsigned parent_index)
    {
      എങ്കിൽ (parent_index == single_parent)
      {
	single_parent = HB_GRAPH_INVALID;
	incoming_edges_--;
	virtual_parents.reset ();
	തിരികെ;
      }

      unsigned *v;
      എങ്കിൽ (parents.has (parent_index, &v))
      {
	incoming_edges_--;
	എങ്കിൽ (*v > 1)
	  (*v)--;
	അല്ലെങ്കിൽ
        {
	  parents.del (parent_index);
          virtual_parents.del (parent_index);
        }

	എങ്കിൽ (incoming_edges_ == 1)
	{
	  single_parent = *parents.താക്കോലുകൾ ();
	  parents.reset ();
	}
      }
    }

    ശൂന്യം remove_real_link_unordered (unsigned link_array_index) {
      obj_.real_links.remove_unordered (link_array_index);
    }

    ശൂന്യം remove_real_link_unordered (unsigned child_index, സ്ഥിരം ശൂന്യം* offset)
    {
      unsigned എണ്ണം = obj_.real_links.length;
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      {
        auto& link = obj_.real_links.arrayZ[i];
        എങ്കിൽ (link.objidx != child_index)
          തുടരുക;

        എങ്കിൽ ((obj_.head + link.position) != offset)
          തുടരുക;

        obj_.real_links.remove_unordered (i);
        തിരികെ;
      }
    }

    graph_result_t<ശൂന്യം> remap_parent (unsigned old_index, unsigned new_index)
    {
      എങ്കിൽ (single_parent != HB_GRAPH_INVALID)
      {
        എങ്കിൽ (single_parent == old_index)
        {
	  single_parent = new_index;
          എങ്കിൽ (virtual_parents.has (old_index))
          {
            virtual_parents.del (old_index);
            virtual_parents.add (new_index);
          }
        }
        തിരികെ graph_result_t<ശൂന്യം>::from(virtual_parents, ALLOCATION_FAILURE);
      }

      സ്ഥിരം unsigned *pv;
      എങ്കിൽ (parents.has (old_index, &pv))
      {
        unsigned v = *pv;
	എങ്കിൽ (!parents.set (new_index, v))
          incoming_edges_ -= v;
	parents.del (old_index);

        എങ്കിൽ (incoming_edges_ == 1)
	{
	  single_parent = *parents.താക്കോലുകൾ ();
	  parents.reset ();
	}
      }

      എങ്കിൽ (virtual_parents.has (old_index))
      {
        virtual_parents.del (old_index);
        virtual_parents.add (new_index);
      }

      തിരികെ graph_result_t<ശൂന്യം>::from(virtual_parents, ALLOCATION_FAILURE);
    }

    തർക്കം is_leaf () സ്ഥിരം
    {
      തിരികെ !obj_.real_links.length && !obj_.virtual_links.length;
    }

    തർക്കം raise_priority ()
    {
      എങ്കിൽ (has_max_priority ()) തിരികെ അസത്യം;
      priority++;
      തിരികെ സത്യം;
    }

    തർക്കം give_max_priority ()
    {
      തർക്കം result = അസത്യം;
      ആകുമ്പോൾ (!has_max_priority()) {
        result = സത്യം;
        priority++;
      }
      തിരികെ result;
    }

    തർക്കം has_max_priority () സ്ഥിരം {
      തിരികെ priority >= 3;
    }

    size_t table_size () സ്ഥിരം {
      തിരികെ obj_.tail - obj_.head;
    }

    int64_t modified_distance (unsigned order) സ്ഥിരം
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_distance =
          hb_clamp (distance + distance_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      എങ്കിൽ (has_max_priority ()) {
        modified_distance = 0;
      }
      തിരികെ (modified_distance << 18) | (0x003FFFF & order);
    }

    int64_t distance_modifier () സ്ഥിരം
    {
      എങ്കിൽ (!priority) തിരികെ 0;
      int64_t table_size = obj_.tail - obj_.head;

      എങ്കിൽ (priority == 1)
        തിരികെ -table_size / 2;

      തിരികെ -table_size;
    }

   സ്വകാര്യ:
    graph_result_t<ശൂന്യം> to_result() സ്ഥിരം {
      എങ്കിൽ (unlikely (
        parents.in_error() || virtual_parents.in_error() ||
        obj_.real_links.in_error() || obj_.virtual_links.in_error()))
        തിരികെ Err(ALLOCATION_FAILURE);
      തിരികെ Ok();
    }

    തർക്കം links_equal (സ്ഥിരം hb_vector_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      സ്ഥിരം hb_vector_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      സ്ഥിരം graph_t& graph,
                      സ്ഥിരം graph_t& other_graph,
                      unsigned depth) സ്ഥിരം
    {
      auto a = this_links.iter ();
      auto b = other_links.iter ();

      ആകുമ്പോൾ (a && b)
      {
        സ്ഥിരം auto& link_a = *a;
        സ്ഥിരം auto& link_b = *b;

        എങ്കിൽ (link_a.width != link_b.width ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          തിരികെ അസത്യം;

        എങ്കിൽ (!graph.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_graph.vertices_[link_b.objidx], graph, other_graph, depth + 1))
          തിരികെ അസത്യം;

        a++;
        b++;
      }

      എങ്കിൽ (തർക്കം (a) != തർക്കം (b))
        തിരികെ അസത്യം;

      തിരികെ സത്യം;
    }
  };

  എണ്ണൽ vertex_mutability_t
  {
    Immutable,
    Mutable
  };

  template <typename T, vertex_mutability_t mutability = Mutable>
  ഘടന vertex_and_table_t
  {
    vertex_and_table_t () : index (0), vertex (nullptr), table (nullptr)
    {}

    template <vertex_mutability_t other_mutability>
    vertex_and_table_t (സ്ഥിരം vertex_and_table_t<T, other_mutability>& o)
      : index (o.index), vertex (o.vertex), table (o.table)
    {}

    unsigned index;
    typename std::conditional<mutability == Immutable, സ്ഥിരം vertex_t*, vertex_t*>::type vertex;
    typename std::conditional<mutability == Immutable, സ്ഥിരം T*, T*>::type table;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  template<typename T>
  സ്ഥിതം graph_result_t<graph_t> create (സ്ഥിരം T& objects)
  {
    എങ്കിൽ (unlikely (objects.length > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.length);
      തിരികെ Err(LIMIT_EXCEEDED);
    }

    graph_t g;
    g.അക്കം_roots_for_space.push (1);
    TRY (graph_result_t<ശൂന്യം>::from (g.അക്കം_roots_for_space, ALLOCATION_FAILURE));

    തർക്കം removed_nil = അസത്യം;
    സ്ഥിരം unsigned എണ്ണം = objects.length;
    g.vertices_.alloc (എണ്ണം);
    g.ordering_.resize (എണ്ണം);
    g.ordering_scratch_.alloc (എണ്ണം);
    TRY (graph_result_t<ശൂന്യം>::from (g.vertices_, ALLOCATION_FAILURE));
    TRY (graph_result_t<ശൂന്യം>::from (g.ordering_, ALLOCATION_FAILURE));
    TRY (graph_result_t<ശൂന്യം>::from (g.ordering_scratch_, ALLOCATION_FAILURE));

    unsigned order = objects.length;
    unsigned skip = 0;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      എങ്കിൽ (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = സത്യം;
        order--;
        g.ordering_.resize (എണ്ണം - 1);
        skip++;
        തുടരുക;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      എങ്കിൽ (unlikely (obj.in_error ())) {
        തിരികെ Err(ALLOCATION_FAILURE);
      }
      vertex_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (എണ്ണം, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      unsigned obj_idx = i - skip;
      g.ordering_[--order] = obj_idx;

      എങ്കിൽ (!removed_nil) തുടരുക;
      // Fix indices to account for removed nil object.
      വേണ്ടി (auto& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    auto r = g.is_fully_connected();
    എങ്കിൽ (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    തിരികെ Ok (std::move (g));
  }

  graph_t (സ്ഥിരം graph_t&) = delete;
  graph_t& operator = (സ്ഥിരം graph_t&) = delete;
  graph_t (graph_t&&) = സ്ഥിരസ്ഥിതി;
  graph_t& operator = (graph_t&&) = സ്ഥിരസ്ഥിതി;

  ~graph_t ()
  {
    വേണ്ടി (അക്ഷരം* b : buffers)
      hb_free (b);
  }

സ്വകാര്യ:
  graph_t () = സ്ഥിരസ്ഥിതി;

പൊതു:

  തർക്കം operator== (സ്ഥിരം graph_t& other) സ്ഥിരം
  {
    തിരികെ വേര് ().equals (വേര്_idx(), other.വേര്_idx(), other.വേര് (), *ഇത്, other, 0);
  }

  ശൂന്യം print () സ്ഥിരം {
    വേണ്ടി (unsigned id : ordering_)
    {
      സ്ഥിരം auto& v = vertices_[id];
      printf("%u: %u [", id, (unsigned പൂർണ്ണസംഖ്യ)v.table_size());
      വേണ്ടി (സ്ഥിരം auto &l : v.obj().real_links) {
        printf("%u, ", l.objidx);
      }
      വേണ്ടി (സ്ഥിരം auto &l : v.obj().virtual_links) {
        printf("v%u, ", l.objidx);
      }
      printf("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  ശൂന്യം normalize ()
  {
    വേണ്ടി (auto& v : vertices_.writer ())
      v.normalize ();
  }

  സ്ഥിരം vertex_t& വേര് () സ്ഥിരം
  {
    തിരികെ vertices_[വേര്_idx ()];
  }

  unsigned വേര്_idx () സ്ഥിരം
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    തിരികെ ordering_[0];
  }

  സ്ഥിരം hb_serialize_context_t::object_t& object (unsigned i) സ്ഥിരം
  {
    തിരികെ vertices_[i].obj();
  }

  graph_result_t<ശൂന്യം> add_buffer (അക്ഷരം* buffer)
  {
    buffers.push (buffer);
    തിരികെ graph_result_t<ശൂന്യം>::from (buffers, ALLOCATION_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  template<typename T>
  graph_result_t<ശൂന്യം> add_link (T* offset,
                                 unsigned parent_id,
                                 unsigned child_id)
  {
    എങ്കിൽ (unlikely (parent_id >= vertices_.length || child_id >= vertices_.length))
      തിരികെ Err(OUT_OF_BOUNDS);
    auto& v = vertices_[parent_id];
    TRY(v.add_real_link(2, child_id, (അക്ഷരം*) offset - (അക്ഷരം*) v.obj().head));
    തിരികെ vertices_[child_id].add_parent (parent_id, അസത്യം);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  graph_result_t<ശൂന്യം> sort_shortest_distance_if_needed ()
  {
    എങ്കിൽ (!positions_invalid) തിരികെ Ok();
    തിരികെ sort_shortest_distance ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  graph_result_t<ശൂന്യം> sort_shortest_distance ()
  {
    positions_invalid = സത്യം;

    എങ്കിൽ (vertices_.length <= 1) {
      // Graph of 1 or less doesn't need sorting.
      തിരികെ Ok();
    }

    TRY (update_distances ());

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (vertices_.length);
    TRY (graph_result_t<ശൂന്യം>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.length);
    TRY (graph_result_t<ശൂന്യം>::from (new_ordering, ALLOCATION_FAILURE));

    hb_vector_t<unsigned> removed_edges;
    removed_edges.resize (vertices_.length);
    TRY (graph_result_t<ശൂന്യം>::from (removed_edges, ALLOCATION_FAILURE));

    TRY (update_parents ());

    queue.insert (വേര് ().modified_distance (0), വേര്_idx ());
    unsigned order = 1;
    unsigned pos = 0;
    ആകുമ്പോൾ (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_id = queue.എടുക്കുക_minimum().second;

      എങ്കിൽ (unlikely (pos >= new_ordering.length)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr, "Invalid graph. Contains cycle.");
        തിരികെ Err(CYCLE_DETECTED);
      }
      new_ordering[pos++] = next_id;
      സ്ഥിരം vertex_t& next = vertices_[next_id];

      വേണ്ടി (സ്ഥിരം auto& link : next.obj().all_links ()) {
        എങ്കിൽ (unlikely (link.objidx >= vertices_.length))
          തിരികെ Err(OUT_OF_BOUNDS);
        removed_edges[link.objidx]++;
        സ്ഥിരം auto& v = vertices_[link.objidx];
        എങ്കിൽ (!(v.incoming_edges () - removed_edges[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          queue.insert (v.modified_distance (order++),
                        link.objidx);
      }
    }

    TRY (graph_result_t<ശൂന്യം>::from (queue, ALLOCATION_FAILURE));
    TRY (graph_result_t<ശൂന്യം>::from (new_ordering, ALLOCATION_FAILURE));

    hb_swap (ordering_, new_ordering);

    എങ്കിൽ (unlikely (pos != vertices_.length)) {
      print_orphaned_nodes ();
      തിരികെ Err(ORPHANED_NODES);
    }

    തിരികെ Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  graph_result_t<ശൂന്യം> find_space_roots (hb_set_t& visited, hb_set_t& roots)
  {
    unsigned വേര്_index = വേര്_idx ();
    വേണ്ടി (unsigned i : ordering_)
    {
      എങ്കിൽ (visited.has (i)) തുടരുക;

      // Only real links can form 32 bit spaces
      വേണ്ടി (സ്ഥിരം auto& l : vertices_[i].obj().real_links)
      {
        എങ്കിൽ (l.is_signed || l.width < 3)
          തുടരുക;

        എങ്കിൽ (i == വേര്_index && l.width == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          തുടരുക;

        എങ്കിൽ (l.width >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_set_t sub_roots;
          TRY (find_32bit_roots (l.objidx, sub_roots));
          എങ്കിൽ (sub_roots) {
            വേണ്ടി (unsigned sub_വേര്_idx : sub_roots) {
              roots.add (sub_വേര്_idx);
              TRY (find_subgraph (sub_വേര്_idx, visited));
            }
            തുടരുക;
          }
        }

        roots.add (l.objidx);
        TRY (find_subgraph (l.objidx, visited));
      }
    }

    TRY(graph_result_t<ശൂന്യം>::from (visited, ALLOCATION_FAILURE));
    TRY(graph_result_t<ശൂന്യം>::from (roots, ALLOCATION_FAILURE));
    തിരികെ Ok();
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Immutable>> as_table (unsigned parent, സ്ഥിരം ശൂന്യം* offset, Ts... ds)
  {
    TRY_ASSIGN (unsigned idx, index_for_offset (parent, offset));
    തിരികെ as_table_from_index<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, Mutable>> as_mutable_table (unsigned parent, സ്ഥിരം ശൂന്യം* offset, Ts... ds)
  {
    TRY_ASSIGN (auto idx, mutable_index_for_offset (parent, offset));
    തിരികെ as_table_from_index<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  template <typename T, vertex_mutability_t mutability = Mutable, typename ...Ts>
  graph_result_t<vertex_and_table_t<T, mutability>> as_table_from_index (unsigned index, Ts... ds)
  {
    എങ്കിൽ (unlikely (index >= vertices_.length))
      തിരികെ Err(OUT_OF_BOUNDS);

    vertex_and_table_t<T, mutability> r;
    r.vertex = (typename std::conditional<mutability == Immutable, സ്ഥിരം vertex_t*, vertex_t*>::type) &vertices_[index];
    r.table = r.vertex->template as<typename std::conditional<mutability == Immutable, സ്ഥിരം T*, T*>::type> ();
    r.index = index;
    എങ്കിൽ (unlikely (!r.table))
      തിരികെ Err(INVALID_ARGUMENT);

    TRY(r.table->sanitize (*(r.vertex), std::forward<Ts>(ds)...));
    തിരികെ r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  graph_result_t<unsigned> index_for_offset (unsigned കണ്ണി_idx, സ്ഥിരം ശൂന്യം* offset) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (കണ്ണി_idx >= vertices_.length)) തിരികെ Err(OUT_OF_BOUNDS);
    സ്ഥിരം auto& node = object (കണ്ണി_idx);
    എങ്കിൽ (unlikely (offset < node.head || offset >= node.tail)) തിരികെ Err(OUT_OF_BOUNDS);

    unsigned എണ്ണം = node.real_links.length;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      സ്ഥിരം auto& link = node.real_links.arrayZ[i];
      എങ്കിൽ (offset != node.head + link.position)
        തുടരുക;
      തിരികെ Ok(link.objidx);
    }

    തിരികെ Err(INVALID_ARGUMENT);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  graph_result_t<unsigned> mutable_index_for_offset (unsigned കണ്ണി_idx, സ്ഥിരം ശൂന്യം* offset)
  {
    TRY_ASSIGN (unsigned child_idx, index_for_offset (കണ്ണി_idx, offset));
    എങ്കിൽ (unlikely (child_idx >= vertices_.length))
      തിരികെ Err(OUT_OF_BOUNDS);

    auto& child = vertices_[child_idx];
    വേണ്ടി (unsigned p : child.parents_iter ())
    {
      എങ്കിൽ (p != കണ്ണി_idx) {
        തിരികെ duplicate (കണ്ണി_idx, child_idx, സത്യം);
      }
    }

    തിരികെ Ok(child_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  graph_result_t<തർക്കം> assign_spaces ()
  {
    TRY (update_parents ());

    വേണ്ടി (auto& vertex : vertices_.writer ())
    {
      vertex.space = 0;
      vertex.priority = 0;
    }
    അക്കം_roots_for_space.resize (1);
    TRY (graph_result_t<ശൂന്യം>::from (അക്കം_roots_for_space, ALLOCATION_FAILURE));
    അക്കം_roots_for_space[0] = 1;
    distance_invalid = സത്യം;
    positions_invalid = സത്യം;

    hb_set_t visited;
    hb_set_t roots;
    TRY (find_space_roots (visited, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    visited.invert ();

    എങ്കിൽ (!roots) തിരികെ Ok(അസത്യം);

    ആകുമ്പോൾ (roots)
    {
      uint32_t next = HB_SET_മൂല്യം_INVALID;
      TRY (graph_result_t<ശൂന്യം>::from (roots, ALLOCATION_FAILURE));
      എങ്കിൽ (!roots.next (&next)) നിർത്തുക;

      hb_set_t connected_roots;
      TRY (find_connected_nodes (next, roots, visited, connected_roots));
      TRY (graph_result_t<ശൂന്യം>::from (connected_roots, ALLOCATION_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (graph_result_t<ശൂന്യം>::from (connected_roots, ALLOCATION_FAILURE));

      unsigned next_space = ഇത്->next_space ();
      എങ്കിൽ (unlikely (next_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 next_space);
        തിരികെ Err(LIMIT_EXCEEDED);
      }
      അക്കം_roots_for_space.push (0);
      TRY(graph_result_t<ശൂന്യം>::from (അക്കം_roots_for_space, ALLOCATION_FAILURE));

      വേണ്ടി (unsigned വേര് : connected_roots)
      {
        DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr, "Subgraph %u gets space %u", വേര്, next_space);
        vertices_[വേര്].space = next_space;
        അക്കം_roots_for_space[next_space] = അക്കം_roots_for_space[next_space] + 1;
        distance_invalid = സത്യം;
        positions_invalid = സത്യം;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    തിരികെ Ok(സത്യം);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  graph_result_t<തർക്കം> isolate_subgraph (hb_set_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_set_t parents;
    വേണ്ടി (unsigned വേര്_idx : roots)
    {
      എങ്കിൽ (unlikely (വേര്_idx >= vertices_.length))
        തിരികെ Err(OUT_OF_BOUNDS);
      subgraph.set (വേര്_idx, wide_parents (വേര്_idx, parents));
    }
    വേണ്ടി (unsigned വേര്_idx : roots)
    {
      TRY (find_subgraph (വേര്_idx, subgraph));
    }
    TRY (graph_result_t<ശൂന്യം>::from (subgraph, ALLOCATION_FAILURE));

    hb_map_t index_map;
    തർക്കം made_changes = അസത്യം;
    വേണ്ടി (auto entry : subgraph.iter ())
    {
      പ്രതിജ്ഞാ (entry.first < vertices_.length);
      സ്ഥിരം auto& node = vertices_[entry.first];
      unsigned subgraph_incoming_edges = entry.second;

      എങ്കിൽ (subgraph_incoming_edges < node.incoming_edges ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = സത്യം;
        TRY (duplicate_subgraph (entry.first, index_map));
      }
    }

    TRY (graph_result_t<ശൂന്യം>::from (index_map, ALLOCATION_FAILURE));

    എങ്കിൽ (!made_changes)
      തിരികെ Ok(അസത്യം);

    auto new_subgraph =
        + subgraph.താക്കോലുകൾ ()
        | hb_map([&] (uint32_t കണ്ണി_idx) {
          സ്ഥിരം uint32_t *v;
          എങ്കിൽ (index_map.has (കണ്ണി_idx, &v)) തിരികെ *v;
          തിരികെ കണ്ണി_idx;
        })
        ;

    TRY(remap_obj_indices (index_map, new_subgraph));
    TRY(remap_obj_indices (index_map, parents.iter (), സത്യം));

    // Update roots set with new indices as needed.
    വേണ്ടി (auto next : roots)
    {
      സ്ഥിരം uint32_t *v;
      എങ്കിൽ (index_map.has (next, &v))
      {
        roots.del (next);
        roots.add (*v);
      }
    }

    തിരികെ Ok(സത്യം);
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
  graph_result_t<ശൂന്യം> traverse_directed_bfs (unsigned start_idx, VisitEdgeFunc&& visit_edge)
  {
    എങ്കിൽ (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      തിരികെ Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<ശൂന്യം>::from (ordering_scratch_, ALLOCATION_FAILURE));

    എങ്കിൽ (!visit_edge (HB_CODEPOINT_INVALID, nullptr, start_idx, 0))
      തിരികെ Ok();

    unsigned head = 0;
    unsigned tail = 0;
    auto& queue = ordering_scratch_;
    queue[tail++] = start_idx;

    unsigned depth = 0;
    ആകുമ്പോൾ (head < tail)
    {
      unsigned level_end = tail;
      ആകുമ്പോൾ (head < level_end)
      {
        unsigned കണ്ണി_idx = queue[head++];
        സ്ഥിരം auto& v = vertices_[കണ്ണി_idx];

        unsigned അക്കം_real = v.obj().real_links.length;
        unsigned total_links = അക്കം_real + v.obj().virtual_links.length;

        വേണ്ടി (unsigned i = 0; i < total_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          സ്ഥിരം auto& v = vertices_[കണ്ണി_idx];
          സ്ഥിരം auto& link = (i < അക്കം_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - അക്കം_real];
          unsigned child_idx = link.objidx;

          എങ്കിൽ (!visit_edge (കണ്ണി_idx, &link, child_idx, depth + 1))
            തുടരുക;

          എങ്കിൽ (unlikely (tail >= queue.length))
            തിരികെ Err(CYCLE_DETECTED);

          queue[tail++] = child_idx;
        }
      }
      depth++;
    }
    തിരികെ Ok();
  }

  graph_result_t<ശൂന്യം> find_subgraph (unsigned കണ്ണി_idx, hb_map_t& subgraph)
  {
    തിരികെ traverse_directed_bfs (കണ്ണി_idx, [&] (
      unsigned parent,
      സ്ഥിരം hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      എങ്കിൽ (depth == 0) തിരികെ സത്യം;
      hb_codepoint_t *എണ്ണം;
      എങ്കിൽ (subgraph.has (child, &എണ്ണം))
      {
        (*എണ്ണം)++;
        തിരികെ അസത്യം;
      }
      subgraph.set (child, 1);
      തിരികെ സത്യം;
    });
  }

  graph_result_t<ശൂന്യം> find_subgraph (unsigned കണ്ണി_idx, hb_set_t& subgraph)
  {
    തിരികെ traverse_directed_bfs (കണ്ണി_idx, [&] (
      unsigned parent,
      സ്ഥിരം hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      എങ്കിൽ (subgraph.has (child)) തിരികെ അസത്യം;
      subgraph.add (child);
      തിരികെ സത്യം;
    });
  }

  graph_result_t<size_t> find_subgraph_size (unsigned കണ്ണി_idx, hb_set_t& subgraph, unsigned max_depth = HB_GRAPH_INVALID)
  {
    size_t size = 0;
    TRY (traverse_directed_bfs (കണ്ണി_idx, [&] (
      unsigned parent,
      സ്ഥിരം hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned depth) {
      എങ്കിൽ (subgraph.has (child)) തിരികെ അസത്യം;
      subgraph.add (child);

      സ്ഥിരം auto& o = vertices_[child].obj();
      size += o.tail - o.head;
      തിരികെ depth < max_depth;
    }));
    തിരികെ Ok(size);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  graph_result_t<ശൂന്യം> find_32bit_roots (unsigned കണ്ണി_idx, hb_set_t& found)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_set_t visited;
    തിരികെ traverse_directed_bfs (കണ്ണി_idx, [&] (
      unsigned parent,
      സ്ഥിരം hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {

      എങ്കിൽ (link && found.has(parent))
        // Don't traverse from something that's already marked as a root.
        തിരികെ അസത്യം;

      എങ്കിൽ (link && !link->is_signed && link->width == 4)
      {
        found.add (link->objidx);
        visited.add (link->objidx);
        തിരികെ അസത്യം;
      }

      എങ്കിൽ (visited.has (child)) തിരികെ അസത്യം;
      visited.add (child);
      തിരികെ സത്യം;
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
                                       സ്ഥിരം O* old_offset,
                                       unsigned new_parent_idx,
                                       സ്ഥിരം O* new_offset)
  {
    distance_invalid = സത്യം;
    positions_invalid = സത്യം;

    എങ്കിൽ (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      തിരികെ Err(OUT_OF_BOUNDS);

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    TRY_ASSIGN (unsigned child_id, index_for_offset (old_parent_idx,
                                                     old_offset));
    എങ്കിൽ (unlikely (child_id >= vertices_.length))
      തിരികെ Err(OUT_OF_BOUNDS);

    TRY(new_v.add_real_link(O::static_size, child_id, (സ്ഥിരം അക്ഷരം*) new_offset - (സ്ഥിരം അക്ഷരം*) new_v.obj().head));

    auto& child = vertices_[child_id];
    TRY(child.add_parent (new_parent_idx, അസത്യം));

    old_v.remove_real_link_unordered (child_id, old_offset);
    child.remove_parent (old_parent_idx);

    തിരികെ Ok(child_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  template<typename O>
  graph_result_t<ശൂന്യം> move_children (unsigned old_parent_idx,
                                      unsigned old_pos_start,
                                      unsigned old_pos_end,
                                      unsigned new_parent_idx,
                                      unsigned new_pos_start)
  {
    എങ്കിൽ (unlikely (old_parent_idx >= vertices_.length ||
                  new_parent_idx >= vertices_.length))
      തിരികെ Err(OUT_OF_BOUNDS);

    distance_invalid = സത്യം;
    positions_invalid = സത്യം;

    auto& old_v = vertices_[old_parent_idx];
    auto& new_v = vertices_[new_parent_idx];

    hb_vector_t<hb_serialize_context_t::object_t::link_t> old_links;
    വേണ്ടി (സ്ഥിരം auto& l : old_v.obj().real_links)
    {
      എങ്കിൽ (l.position < old_pos_start || l.position >= old_pos_end)
      {
        old_links.push (l);
        തുടരുക;
      }

      unsigned array_pos = l.position - old_pos_start;

      unsigned child_id = l.objidx;
      എങ്കിൽ (unlikely (child_id >= vertices_.length))
        തിരികെ Err(OUT_OF_BOUNDS);

      TRY(new_v.add_real_link(O::static_size, child_id, new_pos_start + array_pos));

      auto& child = vertices_[child_id];
      TRY(child.add_parent (new_parent_idx, അസത്യം));
      child.remove_parent (old_parent_idx);
    }

    TRY (graph_result_t<ശൂന്യം>::from (old_links, ALLOCATION_FAILURE));
    തിരികെ old_v.set_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  graph_result_t<ശൂന്യം> duplicate_subgraph (unsigned കണ്ണി_idx, hb_map_t& index_map)
  {
    graph_result_t<ശൂന്യം> result = Ok();
    TRY (traverse_directed_bfs (കണ്ണി_idx, [&] (
      unsigned parent,
      സ്ഥിരം hb_serialize_context_t::object_t::link_t* link,
      unsigned child,
      unsigned _) {
      എങ്കിൽ (result.is_err () || index_map.has (child)) തിരികെ അസത്യം;
      auto r = duplicate (child);
      എങ്കിൽ (unlikely (r.is_err ()))
      {
        result = Err(r.error ());
        തിരികെ അസത്യം;
      }
      index_map.set (child, *r);
      തിരികെ സത്യം;
    }));

    തിരികെ graph_result_t<ശൂന്യം>::from(index_map, ALLOCATION_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  graph_result_t<unsigned> duplicate (unsigned കണ്ണി_idx, തർക്കം copy_table = അസത്യം)
  {
    എങ്കിൽ (unlikely (കണ്ണി_idx >= vertices_.length))
      തിരികെ Err(OUT_OF_BOUNDS);

    എങ്കിൽ (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      തിരികെ Err(LIMIT_EXCEEDED);
    }

    positions_invalid = സത്യം;
    distance_invalid = സത്യം;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<ശൂന്യം>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<ശൂന്യം>::from (ordering_, ALLOCATION_FAILURE));

    auto& child = vertices_[കണ്ണി_idx];

    unsigned table_size = child.obj().tail - child.obj().head;
    എങ്കിൽ (copy_table && table_size)
    {
      അക്ഷരം* buffer = (അക്ഷരം*) hb_malloc (table_size);
      എങ്കിൽ (unlikely (!buffer))
        തിരികെ Err(ALLOCATION_FAILURE);
      auto res = add_buffer (buffer);
      എങ്കിൽ (unlikely (res.is_err ()))
      {
        hb_free (buffer);
        തിരികെ Err(res.error ());
      }
      hb_memcpy (buffer, child.obj().head, table_size);
      clone->set_buffer(buffer, buffer + table_size);
    }
    അല്ലെങ്കിൽ
    {
      clone->set_buffer(child.obj().head, child.obj().tail);
    }
    clone->distance = child.distance;
    clone->space = child.space;
    clone->reset_parents ();

    വേണ്ടി (സ്ഥിരം auto& l : child.obj().real_links)
    {
      TRY(clone->add_real_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, അസത്യം));
    }

    വേണ്ടി (സ്ഥിരം auto& l : child.obj().virtual_links)
    {
      TRY(clone->add_virtual_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, സത്യം));
    }

    തിരികെ Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (unsigned parent_idx, unsigned child_idx, തർക്കം copy_table = അസത്യം)
  {
    എങ്കിൽ (unlikely (parent_idx >= vertices_.length || child_idx >= vertices_.length))
      തിരികെ Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    സ്ഥിരം auto& child = vertices_[child_idx];
    unsigned links_to_child = child.incoming_edges_from_parent (parent_idx);

    എങ്കിൽ (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr, "  Not duplicating %u => %u",
                 parent_idx, child_idx);
      തിരികെ Err(INVALID_ARGUMENT);
    }

    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr, "  Duplicating %u => %u",
               parent_idx, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx, copy_table));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    എങ്കിൽ (parent_idx == clone_idx) parent_idx++;

    auto& parent = vertices_[parent_idx];
    unsigned എണ്ണം = 0;
    unsigned അക്കം_real = parent.obj().real_links.length;
    വേണ്ടി (auto& l : parent.all_links_writer ())
    {
      എണ്ണം++;
      എങ്കിൽ (l.objidx != child_idx)
        തുടരുക;

      TRY(reassign_link (l, parent_idx, clone_idx, എണ്ണം > അക്കം_real));
    }

    തിരികെ Ok(clone_idx);
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
  graph_result_t<unsigned> duplicate (സ്ഥിരം hb_set_t* parents, unsigned child_idx)
  {
    എങ്കിൽ (unlikely (!parents || parents->is_empty ()))
      തിരികെ Err(INVALID_ARGUMENT);

    എങ്കിൽ (unlikely (child_idx >= vertices_.length))
      തിരികെ Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    സ്ഥിരം auto& child = vertices_[child_idx];
    unsigned links_to_child = 0;
    unsigned last_parent = parents->get_max ();
    unsigned first_parent = parents->get_min ();
    വേണ്ടി (unsigned parent_idx : *parents) {
      എങ്കിൽ (unlikely (parent_idx >= vertices_.length))
        തിരികെ Err(OUT_OF_BOUNDS);
      links_to_child += child.incoming_edges_from_parent (parent_idx);
    }

    എങ്കിൽ (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr, "  Not duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);
      തിരികെ Err(INVALID_ARGUMENT);
    }

    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr, "  Duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);

    TRY_ASSIGN (unsigned clone_idx, duplicate (child_idx));

    വേണ്ടി (unsigned parent_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      എങ്കിൽ (parent_idx == clone_idx) parent_idx++;
      auto& parent = vertices_[parent_idx];
      unsigned എണ്ണം = 0;
      unsigned അക്കം_real = parent.obj().real_links.length;
      വേണ്ടി (auto& l : parent.all_links_writer ())
      {
        എണ്ണം++;
        എങ്കിൽ (l.objidx != child_idx)
          തുടരുക;

        TRY(reassign_link (l, parent_idx, clone_idx, എണ്ണം > അക്കം_real));
      }
    }

    തിരികെ Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  graph_result_t<unsigned> new_കണ്ണി (അക്ഷരം* head, അക്ഷരം* tail)
  {
    എങ്കിൽ (unlikely (vertices_.length >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.length);
      തിരികെ Err(LIMIT_EXCEEDED);
    }

    positions_invalid = സത്യം;
    distance_invalid = സത്യം;

    auto* clone = vertices_.push ();
    TRY (graph_result_t<ശൂന്യം>::from (vertices_, ALLOCATION_FAILURE));

    unsigned clone_idx = vertices_.length - 1;
    ordering_.push (clone_idx);
    TRY (graph_result_t<ശൂന്യം>::from (ordering_, ALLOCATION_FAILURE));

    clone->set_buffer (head, tail);
    clone->distance = 0;
    clone->space = 0;

    തിരികെ Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  graph_result_t<unsigned> remap_child (unsigned parent_idx, unsigned old_child_idx)
  {
    എങ്കിൽ (unlikely (parent_idx >= vertices_.length || old_child_idx >= vertices_.length))
      തിരികെ Err(OUT_OF_BOUNDS);

    TRY_ASSIGN (unsigned new_child_idx, duplicate (old_child_idx));

    auto& parent = vertices_[parent_idx];
    വേണ്ടി (auto& l : parent.real_links_writer ())
    {
      എങ്കിൽ (l.objidx != old_child_idx)
        തുടരുക;
      TRY(reassign_link (l, parent_idx, new_child_idx, അസത്യം));
    }

    വേണ്ടി (auto& l : parent.virtual_links_writer ())
    {
      എങ്കിൽ (l.objidx != old_child_idx)
        തുടരുക;
      TRY(reassign_link (l, parent_idx, new_child_idx, സത്യം));
    }
    തിരികെ Ok(new_child_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  graph_result_t<തർക്കം> raise_childrens_priority (unsigned parent_idx)
  {
    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr, "  Raising priority of all children of %u",
               parent_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    എങ്കിൽ (unlikely (parent_idx >= vertices_.length)) തിരികെ Err(OUT_OF_BOUNDS);
    തർക്കം made_change = അസത്യം;
    വേണ്ടി (auto& l : vertices_[parent_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_priority ();
    തിരികെ Ok(made_change);
  }

  graph_result_t<ശൂന്യം> is_fully_connected ()
  {
    TRY(update_parents());

    എങ്കിൽ (unlikely (വേര്().incoming_edges ()))
      // Root cannot have parents.
      തിരികെ Err(ORPHANED_NODES);

    വേണ്ടി (unsigned i = 0; i < വേര്_idx (); i++)
    {
      എങ്കിൽ (unlikely (!vertices_[i].incoming_edges ()))
        തിരികെ Err(ORPHANED_NODES);
    }
    തിരികെ Ok();
  }

#എങ്കിൽ 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  ശൂന്യം save_fuzzer_seed (hb_tag_t tag) സ്ഥിരം
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((ശൂന്യം*) &tag, വലുപ്പം (tag), 1, f);

    uint16_t അക്കം_objects = vertices_.length;
    fwrite ((ശൂന്യം*) &അക്കം_objects, വലുപ്പം (അക്കം_objects), 1, f);

    വേണ്ടി (സ്ഥിരം auto& v : vertices_)
    {
      uint16_t blob_size = v.table_size ();
      fwrite ((ശൂന്യം*) &blob_size, വലുപ്പം (blob_size), 1, f);
      fwrite ((സ്ഥിരം ശൂന്യം*) v.obj.head, blob_size, 1, f);
    }

    uint16_t link_എണ്ണം = 0;
    വേണ്ടി (സ്ഥിരം auto& v : vertices_)
      link_എണ്ണം += v.obj.real_links.length;

    fwrite ((ശൂന്യം*) &link_എണ്ണം, വലുപ്പം (link_എണ്ണം), 1, f);

    തരനിർവചനം ഘടന
    {
      uint16_t parent;
      uint16_t child;
      uint16_t position;
      uint8_t width;
    } link_t;

    വേണ്ടി (unsigned i = 0; i < vertices_.length; i++)
    {
      വേണ്ടി (സ്ഥിരം auto& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.width
        };
        fwrite ((ശൂന്യം*) &link, വലുപ്പം (link), 1, f);
      }
    }

    fclose (f);
  }
#endif

  unsigned അക്കം_roots_for_space_2 (unsigned space) സ്ഥിരം
  {
    തിരികെ അക്കം_roots_for_space[space];
  }

  unsigned next_space () സ്ഥിരം
  {
    തിരികെ അക്കം_roots_for_space.length;
  }

  graph_result_t<ശൂന്യം> move_to_new_space (സ്ഥിരം hb_set_t& indices)
  {
    എങ്കിൽ (unlikely (അക്കം_roots_for_space.length >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 അക്കം_roots_for_space.length);
      തിരികെ Err(LIMIT_EXCEEDED);
    }
    അക്കം_roots_for_space.push (0);
    TRY (graph_result_t<ശൂന്യം>::from (അക്കം_roots_for_space, ALLOCATION_FAILURE));
    unsigned new_space = അക്കം_roots_for_space.length - 1;

    വേണ്ടി (unsigned index : indices) {
      എങ്കിൽ (unlikely (index >= vertices_.length))
        തിരികെ Err(OUT_OF_BOUNDS);
      auto& node = vertices_[index];
      അക്കം_roots_for_space[node.space] = അക്കം_roots_for_space[node.space] - 1;
      അക്കം_roots_for_space[new_space] = അക്കം_roots_for_space[new_space] + 1;
      node.space = new_space;
      distance_invalid = സത്യം;
      positions_invalid = സത്യം;
    }
    തിരികെ Ok();
  }

  unsigned space_for (unsigned index, unsigned* വേര് = nullptr) സ്ഥിരം
  {
    ആകുമ്പോൾ (സത്യം)
    {
      പ്രതിജ്ഞാ (index < vertices_.length);
      സ്ഥിരം auto& node = vertices_[index];
      എങ്കിൽ (node.space)
      {
        എങ്കിൽ (വേര് != nullptr)
          *വേര് = index;
        തിരികെ node.space;
      }

      എങ്കിൽ (!node.incoming_edges ())
      {
        എങ്കിൽ (വേര്)
          *വേര് = index;
        തിരികെ 0;
      }

      index = *node.parents_iter ();
    }
  }

  size_t total_size_in_bytes () സ്ഥിരം {
    size_t total_size = 0;
    unsigned എണ്ണം = vertices_.length;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++) {
      size_t size = vertices_.arrayZ[i].table_size ();
      total_size += size;
    }
    തിരികെ total_size;
  }


 സ്വകാര്യ:

  ശൂന്യം print_orphaned_nodes ()
  {
    എങ്കിൽ (!DEBUG_ENABLED(SUBSET_REPACK)) തിരികെ;

    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr, "Graph is not fully connected.");

    parents_invalid = സത്യം;
    (ശൂന്യം) update_parents();

    എങ്കിൽ (വേര്().incoming_edges ()) {
      DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr, "Root node has incoming edges.");
    }

    വേണ്ടി (unsigned i = 0; i < vertices_.length; i++)
    {
      സ്ഥിരം auto& v = vertices_[i];
      എങ്കിൽ (!v.incoming_edges ())
        DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  unsigned wide_parents (unsigned കണ്ണി_idx, hb_set_t& parents) സ്ഥിരം
  {
    unsigned എണ്ണം = 0;
    വേണ്ടി (unsigned p : vertices_[കണ്ണി_idx].parents_iter ())
    {
      // Only real links can be wide
      വേണ്ടി (സ്ഥിരം auto& l : vertices_[p].obj().real_links)
      {
        എങ്കിൽ (l.objidx == കണ്ണി_idx
            && (l.width == 3 || l.width == 4)
            && !l.is_signed)
        {
          എണ്ണം++;
          parents.add (p);
        }
      }
    }
    തിരികെ എണ്ണം;
  }

 പൊതു:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  graph_result_t<ശൂന്യം> update_parents ()
  {
    എങ്കിൽ (!parents_invalid) തിരികെ Ok();

    unsigned എണ്ണം = vertices_.length;

    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      vertices_.arrayZ[i].reset_parents ();

    വേണ്ടി (unsigned p = 0; p < എണ്ണം; p++)
    {
      വേണ്ടി (സ്ഥിരം auto& l : vertices_.arrayZ[p].obj().real_links)
      {
        എങ്കിൽ (unlikely (l.objidx >= എണ്ണം)) തിരികെ Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, അസത്യം));
      }

      വേണ്ടി (സ്ഥിരം auto& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        എങ്കിൽ (unlikely (l.objidx >= എണ്ണം)) തിരികെ Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, സത്യം));
      }
    }

    parents_invalid = അസത്യം;
    തിരികെ Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  ശൂന്യം update_positions ()
  {
    എങ്കിൽ (!positions_invalid) തിരികെ;

    unsigned current_pos = 0;
    വേണ്ടി (unsigned i : ordering_)
    {
      auto& v = vertices_[i];
      v.start = current_pos;
      current_pos += v.obj().tail - v.obj().head;
      v.end = current_pos;
    }

    positions_invalid = അസത്യം;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  graph_result_t<ശൂന്യം> update_distances ()
  {
    എങ്കിൽ (!distance_invalid) തിരികെ Ok();

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
    unsigned എണ്ണം = vertices_.length;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[വേര്_idx ()].distance = 0;

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (എണ്ണം);
    queue.insert (0, വേര്_idx ());
    TRY (graph_result_t<ശൂന്യം>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<തർക്കം> visited;
    visited.resize (vertices_.length);
    TRY (graph_result_t<ശൂന്യം>::from (visited, ALLOCATION_FAILURE));

    ആകുമ്പോൾ (!queue.in_error () && !queue.is_empty ())
    {
      unsigned next_idx = queue.എടുക്കുക_minimum ().second;
      എങ്കിൽ (visited[next_idx]) തുടരുക;
      സ്ഥിരം auto& next = vertices_[next_idx];
      int64_t next_distance = next.distance;
      visited[next_idx] = സത്യം;

      വേണ്ടി (സ്ഥിരം auto& link : next.obj().all_links ())
      {
        എങ്കിൽ (unlikely (link.objidx >= vertices_.length))
          തിരികെ Err(OUT_OF_BOUNDS);

        എങ്കിൽ (visited[link.objidx]) തുടരുക;

        auto& child_v = vertices_.arrayZ[link.objidx];
        സ്ഥിരം auto& child = child_v.obj ();
        unsigned link_width = link.width ? link.width : 4; // treat virtual offsets as 32 bits wide
        int64_t child_weight = (child.tail - child.head) +
                               ((int64_t) 1 << (link_width * 8)) * (child_v.space + 1);
        int64_t child_distance = next_distance + child_weight;

        എങ്കിൽ (child_distance < child_v.distance)
        {
          child_v.distance = child_distance;
          queue.insert (child_distance, link.objidx);
        }
      }
    }

    TRY (graph_result_t<ശൂന്യം>::from (queue, ALLOCATION_FAILURE));
    എങ്കിൽ (unlikely (!queue.is_empty ()))
    {
      print_orphaned_nodes ();
      തിരികെ Err(ORPHANED_NODES);
    }

    distance_invalid = അസത്യം;
    തിരികെ Ok();
  }

 സ്വകാര്യ:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  graph_result_t<ശൂന്യം> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      unsigned parent_idx,
                      unsigned new_idx,
                      തർക്കം is_virtual)
  {
    unsigned old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_parent (parent_idx);
    തിരികെ vertices_[new_idx].add_parent (parent_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  template<typename Iterator, hb_requires (hb_is_iterator (Iterator))>
  graph_result_t<ശൂന്യം> remap_obj_indices (സ്ഥിരം hb_map_t& id_map,
                                          Iterator subgraph,
                                          തർക്കം only_wide = അസത്യം)
  {
    എങ്കിൽ (!id_map) തിരികെ Ok();
    വേണ്ടി (unsigned i : subgraph)
    {
      auto& vertex = vertices_[i];
      unsigned അക്കം_real = vertex.obj().real_links.length;
      unsigned എണ്ണം = 0;
      വേണ്ടി (auto& link : vertex.all_links_writer ())
      {
        എണ്ണം++;
        സ്ഥിരം uint32_t *v;
        എങ്കിൽ (!id_map.has (link.objidx, &v)) തുടരുക;
        എങ്കിൽ (only_wide && (link.is_signed || (link.width != 4 && link.width != 3))) തുടരുക;

        TRY(reassign_link (link, i, *v, എണ്ണം > അക്കം_real));
      }
    }
    തിരികെ Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  graph_result_t<ശൂന്യം> find_connected_nodes (unsigned start_idx,
                                             hb_set_t& targets,
                                             hb_set_t& visited,
                                             hb_set_t& connected)
  {
    TRY (graph_result_t<ശൂന്യം>::from (visited, ALLOCATION_FAILURE));
    എങ്കിൽ (visited.has (start_idx)) തിരികെ Ok();
    എങ്കിൽ (unlikely (start_idx >= vertices_.length))
    {
      DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      തിരികെ Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.length);
    TRY (graph_result_t<ശൂന്യം>::from (ordering_scratch_, ALLOCATION_FAILURE));

    auto& അട്ടി = ordering_scratch_;
    unsigned അട്ടി_len = 0;

    graph_result_t<ശൂന്യം> res = Ok();
    auto handle_കണ്ണി = [&] (unsigned കണ്ണി_idx) {
      visited.add (കണ്ണി_idx);
      എങ്കിൽ (targets.has (കണ്ണി_idx)) {
        targets.del (കണ്ണി_idx);
        connected.add (കണ്ണി_idx);
      }

      എങ്കിൽ (unlikely (അട്ടി_len >= അട്ടി.length))
      {
        res = Err(CYCLE_DETECTED);
        തിരികെ അസത്യം;
      }
      അട്ടി[അട്ടി_len++] = കണ്ണി_idx;
      തിരികെ സത്യം;
    };

    എങ്കിൽ (!handle_കണ്ണി (start_idx)) തിരികെ res;

    ആകുമ്പോൾ (അട്ടി_len > 0)
    {
      unsigned കണ്ണി_idx = അട്ടി[--അട്ടി_len];
      സ്ഥിരം auto& v = vertices_[കണ്ണി_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      വേണ്ടി (സ്ഥിരം auto& l : v.obj().all_links ())
      {
        unsigned child_idx = l.objidx;
        എങ്കിൽ (visited.has (child_idx)) തുടരുക;
        എങ്കിൽ (!handle_കണ്ണി (child_idx)) തിരികെ res;
      }

      വേണ്ടി (unsigned parent_idx : v.parents_iter ())
      {
        എങ്കിൽ (visited.has (parent_idx)) തുടരുക;
        എങ്കിൽ (!handle_കണ്ണി (parent_idx)) തിരികെ res;
      }
    }

    TRY (graph_result_t<ശൂന്യം>::from (visited, ALLOCATION_FAILURE));
    TRY (graph_result_t<ശൂന്യം>::from (connected, ALLOCATION_FAILURE));

    തിരികെ Ok();
  }

 പൊതു:
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

 സ്വകാര്യ:
  തർക്കം parents_invalid = സത്യം;
  തർക്കം distance_invalid = സത്യം;
  തർക്കം positions_invalid = സത്യം;
  hb_vector_t<unsigned> അക്കം_roots_for_space;
  hb_vector_t<അക്ഷരം*> buffers;
};

}

#endif  // GRAPH_GRAPH_HH
