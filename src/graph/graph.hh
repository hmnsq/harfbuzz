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

#అన్తర్భావ "../hb-set.hh"
#అన్తర్భావ "../hb-priority-queue.hh"
#అన్తర్భావ "../hb-serialize.hh"
#అన్తర్భావ "graph-result.hh"

#నిర్వచితం_కాకపోతే లేఖాచిత్రం_లేఖాచిత్రం_HH
#నిర్వచించు లేఖాచిత్రం_లేఖాచిత్రం_HH

#నిర్వచించు HB_లేఖాచిత్రం_INVALID ((గుర్తులేని) -1)

నామాకాశ లేఖాచిత్రం {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
నిర్మాణం లేఖాచిత్రం_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  నిర్మాణం శీర్షం_t
  {
    // TODO(garretrieger): have vertex_t gaurantee that head/tail are non null and head < tail
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): have vertex_t gaurantee that all link indexes are valid (ie < vertices_.length)
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): track where the real links are currently sorted, and automatically sort
    // prior to accessing iterator for real links. Avoids needing to manually trigger sorting by
    // downstream users.

    int64_t distance = 0 ;
    గుర్తులేని space = 0 ;
    గుర్తులేని ప్రారంభం = 0;
    గుర్తులేని చివర = 0;
    గుర్తులేని ప్రాధాన్యత = 0;

    అంతరంగ:
    hb_serialize_context_t::object_t obj_;
    గుర్తులేని incoming_అంచులు = 0;
    గుర్తులేని single_మాతృ = HB_లేఖాచిత్రం_INVALID;
    తార్కిక has_incoming_virtual_అంచులు = అబద్ధం;
    hb_hashmap_t<గుర్తులేని, గుర్తులేని> parents;
    hb_సమితి_t virtual_parents;
    బహిరంగ:

    శీర్షం_t ()
    {}

    స్పష్టమైన శీర్షం_t(స్థిరరాశి hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    స్వయంచాలక parents_iter () స్థిరరాశి HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_మాతృ, single_మాతృ != HB_లేఖాచిత్రం_INVALID),
	parents.కీలకాలు_ref ()
      )
    )

    శూన్య సమితి_బఫరు (అక్షరం* తల_2, అక్షరం* తోక) {
      obj_.తల_2 = తల_2;
      obj_.తోక = తోక;
    }

    శూన్య shrink_బఫరు (size_t లెక్క)
    {
      obj_.తోక -= లెక్క;
    }

    స్వయంచాలక all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    స్వయంచాలక real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    స్వయంచాలక virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    శూన్య క్రమీకరించు_real_links () {
      obj_.real_links.qsort ();
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_real_link (గుర్తులేని వెడల్పు, గుర్తులేని obj_idx, గుర్తులేని position) {
      స్వయంచాలక* link = obj_.real_links.push ();
      TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (obj_.real_links, కేటాయింపు_FAILURE));
      link->వెడల్పు = వెడల్పు;
      link->objidx = obj_idx;
      link->position = position;
      తిరిగి Ok();
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_virtual_link (గుర్తులేని obj_idx) {
      స్వయంచాలక* link = obj_.virtual_links.push ();
      TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (obj_.virtual_links, కేటాయింపు_FAILURE));
      link->objidx = obj_idx;
      తిరిగి Ok();
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_real_link (స్థిరరాశి hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      తిరిగి లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (obj_.real_links, కేటాయింపు_FAILURE);
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> సమితి_real_links (hb_సదిశ_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (links, కేటాయింపు_FAILURE));
      obj_.real_links = links;
      తిరిగి Ok();
    }

    శూన్య clear_real_links ()
    {
      obj_.real_links.reset();
    }

    శూన్య clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_virtual_link (స్థిరరాశి hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      తిరిగి లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (obj_.virtual_links, కేటాయింపు_FAILURE);
    }

    స్థిరరాశి hb_serialize_context_t::object_t& obj() స్థిరరాశి { తిరిగి obj_; }

    మూస<రకం_పేరు T>
    T as () స్థిరరాశి
    {
      తిరిగి reinterpret_cast<T> (obj_.తల_2);
    }

    తార్కిక has_incoming_virtual_అంచులు_2 () స్థిరరాశి
    {
      తిరిగి has_incoming_virtual_అంచులు;
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> link_positions_valid (గుర్తులేని అంకె_objects, తార్కిక removed_nil)
    {
      hb_సమితి_t assigned_బైట్లు;
      కొరకు (స్థిరరాశి స్వయంచాలక& l : obj_.real_links)
      {
        ఒకవేళ (unlikely (l.objidx >= అంకె_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                     "Invalid graph. Invalid object index.");
          తిరిగి Err(INVALID_వాదం);
        }

        గుర్తులేని ప్రారంభం = l.position;
        గుర్తులేని చివర = ప్రారంభం + l.వెడల్పు - 1;

        ఒకవేళ (unlikely (l.వెడల్పు < 2 || l.వెడల్పు > 4))
        {
          DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                     "Invalid graph. Invalid link width.");
          తిరిగి Err(INVALID_వాదం);
        }

        ఒకవేళ (unlikely (చివర >= పట్టిక_కొలత ()))
        {
          DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                     "Invalid graph. Link position is out of bounds.");
          తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);
        }

        ఒకవేళ (unlikely (assigned_బైట్లు.intersects (ప్రారంభం, చివర)))
        {
          DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                     "Invalid graph. Found offsets whose positions overlap.");
          తిరిగి Err(INVALID_వాదం);
        }

        assigned_బైట్లు.కూడు_range (ప్రారంభం, చివర);
      }

      తిరిగి లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (assigned_బైట్లు, కేటాయింపు_FAILURE);
    }

    శూన్య normalize ()
    {
      obj_.real_links.qsort ();
      కొరకు (స్థిరరాశి స్వయంచాలక& l : obj_.real_links)
      {
        కొరకు (గుర్తులేని i = 0; i < l.వెడల్పు; i++)
        {
          obj_.తల_2[l.position + i] = 0;
        }
      }
    }

    తార్కిక equals (గుర్తులేని this_సూచీ,
                 గుర్తులేని other_సూచీ,
                 స్థిరరాశి శీర్షం_t& other,
                 స్థిరరాశి లేఖాచిత్రం_t& లేఖాచిత్రం,
                 స్థిరరాశి లేఖాచిత్రం_t& other_లేఖాచిత్రం,
                 గుర్తులేని లోతు) స్థిరరాశి
    {
      ఒకవేళ (!(as_బైట్లు () == other.as_బైట్లు ()))
      {
        DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_సూచీ,
                   (గుర్తులేని దీర్ఘ) పట్టిక_కొలత (),
                   other_సూచీ,
                   (గుర్తులేని దీర్ఘ) other.పట్టిక_కొలత (),
                   లోతు);

        స్వయంచాలక a = as_బైట్లు ();
        స్వయంచాలక b = other.as_బైట్లు ();
        అయితే (a || b)
        {
          DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                     "  0x%x %s 0x%x", (గుర్తులేని) *a, (*a == *b) ? "==" : "!=", (గుర్తులేని) *b);
          a++;
          b++;
        }
        తిరిగి అబద్ధం;
      }

      తిరిగి links_equal (obj_.real_links, other.obj_.real_links, లేఖాచిత్రం, other_లేఖాచిత్రం, లోతు);
    }

    hb_బైట్లు_t as_బైట్లు () స్థిరరాశి
    {
      తిరిగి hb_బైట్లు_t (obj_.తల_2, పట్టిక_కొలత ());
    }

    మిత్ర శూన్య swap (శీర్షం_t& a, శీర్షం_t& b)
    {
      hb_మార్పిడి (a.obj_, b.obj_);
      hb_మార్పిడి (a.distance, b.distance);
      hb_మార్పిడి (a.space, b.space);
      hb_మార్పిడి (a.single_మాతృ, b.single_మాతృ);
      hb_మార్పిడి (a.parents, b.parents);
      hb_మార్పిడి (a.virtual_parents, b.virtual_parents);
      hb_మార్పిడి (a.incoming_అంచులు, b.incoming_అంచులు);
      hb_మార్పిడి (a.has_incoming_virtual_అంచులు, b.has_incoming_virtual_అంచులు);
      hb_మార్పిడి (a.ప్రారంభం, b.ప్రారంభం);
      hb_మార్పిడి (a.చివర, b.చివర);
      hb_మార్పిడి (a.ప్రాధాన్యత, b.ప్రాధాన్యత);
    }

    లేఖాచిత్రం_ఫలితం_t<hb_hashmap_t<గుర్తులేని, గుర్తులేని>>
    position_గమ్యం_సూచీ_map () స్థిరరాశి
    {
      hb_hashmap_t<గుర్తులేని, గుర్తులేని> ఫలితం;

      ఫలితం.alloc (obj_.real_links.నిడివి);
      కొరకు (స్థిరరాశి స్వయంచాలక& l : obj_.real_links) {
        ఫలితం.సమితి (l.position, l.objidx);
      }

      తిరిగి లేఖాచిత్రం_ఫలితం_t<hb_hashmap_t<గుర్తులేని, గుర్తులేని>>::from (std::move(ఫలితం), కేటాయింపు_FAILURE);
    }

    తార్కిక is_shared () స్థిరరాశి
    {
      తిరిగి parents.get_population () > virtual_parents.get_population () + 1;
    }

    గుర్తులేని incoming_అంచులు_2 () స్థిరరాశి
    {
      ఒకవేళ (HB_DEBUG_SUBSET_REPACK)
       {
	ప్రతిజ్ఞా (incoming_అంచులు == (single_మాతృ != HB_లేఖాచిత్రం_INVALID) +
		(parents.విలువలు_ref () | hb_reduce (hb_కూడు, 0)));
       }
      తిరిగి incoming_అంచులు;
    }

    గుర్తులేని incoming_అంచులు_from_మాతృ (గుర్తులేని మాతృ_సూచీ) స్థిరరాశి {
      ఒకవేళ (single_మాతృ != HB_లేఖాచిత్రం_INVALID) {
        తిరిగి single_మాతృ == మాతృ_సూచీ ? 1 : 0;
      }

      గుర్తులేని* లెక్క;
      తిరిగి  parents.has(మాతృ_సూచీ, &లెక్క) ? *లెక్క : 0;
    }

    శూన్య reset_parents ()
    {
      incoming_అంచులు = 0;
      has_incoming_virtual_అంచులు = అబద్ధం;
      single_మాతృ = HB_లేఖాచిత్రం_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_మాతృ (గుర్తులేని మాతృ_సూచీ, తార్కిక is_virtual)
    {
      ప్రతిజ్ఞా (మాతృ_సూచీ != HB_లేఖాచిత్రం_INVALID);
      has_incoming_virtual_అంచులు |= is_virtual;
      ఒకవేళ (is_virtual)
        virtual_parents.కూడు (మాతృ_సూచీ);

      ఒకవేళ (incoming_అంచులు == 0)
      {
	single_మాతృ = మాతృ_సూచీ;
	incoming_అంచులు = 1;
	తిరిగి గమ్యం_ఫలితం();
      }
      లేకపోతే ఒకవేళ (single_మాతృ != HB_లేఖాచిత్రం_INVALID)
      {
        ప్రతిజ్ఞా (incoming_అంచులు == 1);
	ఒకవేళ (!parents.సమితి (single_మాతృ, 1))
	  తిరిగి గమ్యం_ఫలితం ();
	single_మాతృ = HB_లేఖాచిత్రం_INVALID;
      }

      గుర్తులేని *v;
      ఒకవేళ (parents.has (మాతృ_సూచీ, &v))
      {
        (*v)++;
	incoming_అంచులు++;
      }
      లేకపోతే ఒకవేళ (parents.సమితి (మాతృ_సూచీ, 1))
	incoming_అంచులు++;

      తిరిగి గమ్యం_ఫలితం();
    }

    శూన్య remove_మాతృ (గుర్తులేని మాతృ_సూచీ)
    {
      ఒకవేళ (మాతృ_సూచీ == single_మాతృ)
      {
	single_మాతృ = HB_లేఖాచిత్రం_INVALID;
	incoming_అంచులు--;
	virtual_parents.reset ();
	తిరిగి;
      }

      గుర్తులేని *v;
      ఒకవేళ (parents.has (మాతృ_సూచీ, &v))
      {
	incoming_అంచులు--;
	ఒకవేళ (*v > 1)
	  (*v)--;
	లేకపోతే
        {
	  parents.del (మాతృ_సూచీ);
          virtual_parents.del (మాతృ_సూచీ);
        }

	ఒకవేళ (incoming_అంచులు == 1)
	{
	  single_మాతృ = *parents.కీలకాలు ();
	  parents.reset ();
	}
      }
    }

    శూన్య remove_real_link_unordered (గుర్తులేని link_array_సూచీ) {
      obj_.real_links.remove_unordered (link_array_సూచీ);
    }

    శూన్య remove_real_link_unordered (గుర్తులేని బిడ్డ_సూచీ, స్థిరరాశి శూన్య* స్థానభ్రంశం)
    {
      గుర్తులేని లెక్క = obj_.real_links.నిడివి;
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      {
        స్వయంచాలక& link = obj_.real_links.arrayZ[i];
        ఒకవేళ (link.objidx != బిడ్డ_సూచీ)
          కొనసాగు;

        ఒకవేళ ((obj_.తల_2 + link.position) != స్థానభ్రంశం)
          కొనసాగు;

        obj_.real_links.remove_unordered (i);
        తిరిగి;
      }
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> remap_మాతృ (గుర్తులేని old_సూచీ, గుర్తులేని new_సూచీ)
    {
      ఒకవేళ (single_మాతృ != HB_లేఖాచిత్రం_INVALID)
      {
        ఒకవేళ (single_మాతృ == old_సూచీ)
        {
	  single_మాతృ = new_సూచీ;
          ఒకవేళ (virtual_parents.has (old_సూచీ))
          {
            virtual_parents.del (old_సూచీ);
            virtual_parents.కూడు (new_సూచీ);
          }
        }
        తిరిగి లేఖాచిత్రం_ఫలితం_t<శూన్య>::from(virtual_parents, కేటాయింపు_FAILURE);
      }

      స్థిరరాశి గుర్తులేని *pv;
      ఒకవేళ (parents.has (old_సూచీ, &pv))
      {
        గుర్తులేని v = *pv;
	ఒకవేళ (!parents.సమితి (new_సూచీ, v))
          incoming_అంచులు -= v;
	parents.del (old_సూచీ);

        ఒకవేళ (incoming_అంచులు == 1)
	{
	  single_మాతృ = *parents.కీలకాలు ();
	  parents.reset ();
	}
      }

      ఒకవేళ (virtual_parents.has (old_సూచీ))
      {
        virtual_parents.del (old_సూచీ);
        virtual_parents.కూడు (new_సూచీ);
      }

      తిరిగి లేఖాచిత్రం_ఫలితం_t<శూన్య>::from(virtual_parents, కేటాయింపు_FAILURE);
    }

    తార్కిక is_ఆకు () స్థిరరాశి
    {
      తిరిగి !obj_.real_links.నిడివి && !obj_.virtual_links.నిడివి;
    }

    తార్కిక raise_ప్రాధాన్యత ()
    {
      ఒకవేళ (has_max_ప్రాధాన్యత ()) తిరిగి అబద్ధం;
      ప్రాధాన్యత++;
      తిరిగి నిజం;
    }

    తార్కిక give_max_ప్రాధాన్యత ()
    {
      తార్కిక ఫలితం = అబద్ధం;
      అయితే (!has_max_ప్రాధాన్యత()) {
        ఫలితం = నిజం;
        ప్రాధాన్యత++;
      }
      తిరిగి ఫలితం;
    }

    తార్కిక has_max_ప్రాధాన్యత () స్థిరరాశి {
      తిరిగి ప్రాధాన్యత >= 3;
    }

    size_t పట్టిక_కొలత () స్థిరరాశి {
      తిరిగి obj_.తోక - obj_.తల_2;
    }

    int64_t modified_దూరం (గుర్తులేని క్రమం) స్థిరరాశి
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_దూరం =
          hb_clamp (distance + దూరం_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      ఒకవేళ (has_max_ప్రాధాన్యత ()) {
        modified_దూరం = 0;
      }
      తిరిగి (modified_దూరం << 18) | (0x003FFFF & క్రమం);
    }

    int64_t దూరం_modifier () స్థిరరాశి
    {
      ఒకవేళ (!ప్రాధాన్యత) తిరిగి 0;
      int64_t పట్టిక_కొలత = obj_.తోక - obj_.తల_2;

      ఒకవేళ (ప్రాధాన్యత == 1)
        తిరిగి -పట్టిక_కొలత / 2;

      తిరిగి -పట్టిక_కొలత;
    }

   అంతరంగ:
    లేఖాచిత్రం_ఫలితం_t<శూన్య> గమ్యం_ఫలితం() స్థిరరాశి {
      ఒకవేళ (unlikely (
        parents.లోపల_error() || virtual_parents.లోపల_error() ||
        obj_.real_links.లోపల_error() || obj_.virtual_links.లోపల_error()))
        తిరిగి Err(కేటాయింపు_FAILURE);
      తిరిగి Ok();
    }

    తార్కిక links_equal (స్థిరరాశి hb_సదిశ_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      స్థిరరాశి hb_సదిశ_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      స్థిరరాశి లేఖాచిత్రం_t& లేఖాచిత్రం,
                      స్థిరరాశి లేఖాచిత్రం_t& other_లేఖాచిత్రం,
                      గుర్తులేని లోతు) స్థిరరాశి
    {
      స్వయంచాలక a = this_links.iter ();
      స్వయంచాలక b = other_links.iter ();

      అయితే (a && b)
      {
        స్థిరరాశి స్వయంచాలక& link_a = *a;
        స్థిరరాశి స్వయంచాలక& link_b = *b;

        ఒకవేళ (link_a.వెడల్పు != link_b.వెడల్పు ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          తిరిగి అబద్ధం;

        ఒకవేళ (!లేఖాచిత్రం.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_లేఖాచిత్రం.vertices_[link_b.objidx], లేఖాచిత్రం, other_లేఖాచిత్రం, లోతు + 1))
          తిరిగి అబద్ధం;

        a++;
        b++;
      }

      ఒకవేళ (తార్కిక (a) != తార్కిక (b))
        తిరిగి అబద్ధం;

      తిరిగి నిజం;
    }
  };

  గణన శీర్షం_mutability_t
  {
    Immutable,
    Mutable
  };

  మూస <రకం_పేరు T, శీర్షం_mutability_t mutability = Mutable>
  నిర్మాణం శీర్షం_and_పట్టిక_t
  {
    శీర్షం_and_పట్టిక_t () : సూచీ_2 (0), శీర్షం (శూన్యసూచిక), పట్టిక (శూన్యసూచిక)
    {}

    మూస <శీర్షం_mutability_t other_mutability>
    శీర్షం_and_పట్టిక_t (స్థిరరాశి శీర్షం_and_పట్టిక_t<T, other_mutability>& o)
      : సూచీ_2 (o.సూచీ_2), శీర్షం (o.శీర్షం), పట్టిక (o.పట్టిక)
    {}

    గుర్తులేని సూచీ_2;
    రకం_పేరు std::conditional<mutability == Immutable, స్థిరరాశి శీర్షం_t*, శీర్షం_t*>::type శీర్షం;
    రకం_పేరు std::conditional<mutability == Immutable, స్థిరరాశి T*, T*>::type పట్టిక;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  మూస<రకం_పేరు T>
  స్థిర లేఖాచిత్రం_ఫలితం_t<లేఖాచిత్రం_t> create (స్థిరరాశి T& objects)
  {
    ఒకవేళ (unlikely (objects.నిడివి > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.నిడివి);
      తిరిగి Err(హద్దు_EXCEEDED);
    }

    లేఖాచిత్రం_t g;
    g.అంకె_roots_for_space.push (1);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (g.అంకె_roots_for_space, కేటాయింపు_FAILURE));

    తార్కిక removed_nil = అబద్ధం;
    స్థిరరాశి గుర్తులేని లెక్క = objects.నిడివి;
    g.vertices_.alloc (లెక్క);
    g.ordering_.resize (లెక్క);
    g.ordering_scratch_.alloc (లెక్క);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (g.vertices_, కేటాయింపు_FAILURE));
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (g.ordering_, కేటాయింపు_FAILURE));
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (g.ordering_scratch_, కేటాయింపు_FAILURE));

    గుర్తులేని క్రమం = objects.నిడివి;
    గుర్తులేని skip = 0;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      ఒకవేళ (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = నిజం;
        క్రమం--;
        g.ordering_.resize (లెక్క - 1);
        skip++;
        కొనసాగు;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      ఒకవేళ (unlikely (obj.లోపల_error ())) {
        తిరిగి Err(కేటాయింపు_FAILURE);
      }
      శీర్షం_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (లెక్క, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      గుర్తులేని obj_idx = i - skip;
      g.ordering_[--క్రమం] = obj_idx;

      ఒకవేళ (!removed_nil) కొనసాగు;
      // Fix indices to account for removed nil object.
      కొరకు (స్వయంచాలక& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    స్వయంచాలక r = g.is_fully_connected();
    ఒకవేళ (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    తిరిగి Ok (std::move (g));
  }

  లేఖాచిత్రం_t (స్థిరరాశి లేఖాచిత్రం_t&) = తొలగించు;
  లేఖాచిత్రం_t& క్రియాకారకం = (స్థిరరాశి లేఖాచిత్రం_t&) = తొలగించు;
  లేఖాచిత్రం_t (లేఖాచిత్రం_t&&) = అప్రమేయ;
  లేఖాచిత్రం_t& క్రియాకారకం = (లేఖాచిత్రం_t&&) = అప్రమేయ;

  ~లేఖాచిత్రం_t ()
  {
    కొరకు (అక్షరం* b : buffers)
      hb_free (b);
  }

అంతరంగ:
  లేఖాచిత్రం_t () = అప్రమేయ;

బహిరంగ:

  తార్కిక క్రియాకారకం== (స్థిరరాశి లేఖాచిత్రం_t& other) స్థిరరాశి
  {
    తిరిగి మూలం ().equals (మూలం_idx(), other.మూలం_idx(), other.మూలం (), *ఇది, other, 0);
  }

  శూన్య print () స్థిరరాశి {
    కొరకు (గుర్తులేని id : ordering_)
    {
      స్థిరరాశి స్వయంచాలక& v = vertices_[id];
      రాయి("%u: %u [", id, (గుర్తులేని పూర్ణాంకం)v.పట్టిక_కొలత());
      కొరకు (స్థిరరాశి స్వయంచాలక &l : v.obj().real_links) {
        రాయి("%u, ", l.objidx);
      }
      కొరకు (స్థిరరాశి స్వయంచాలక &l : v.obj().virtual_links) {
        రాయి("v%u, ", l.objidx);
      }
      రాయి("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  శూన్య normalize ()
  {
    కొరకు (స్వయంచాలక& v : vertices_.writer ())
      v.normalize ();
  }

  స్థిరరాశి శీర్షం_t& మూలం () స్థిరరాశి
  {
    తిరిగి vertices_[మూలం_idx ()];
  }

  గుర్తులేని మూలం_idx () స్థిరరాశి
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    తిరిగి ordering_[0];
  }

  స్థిరరాశి hb_serialize_context_t::object_t& object (గుర్తులేని i) స్థిరరాశి
  {
    తిరిగి vertices_[i].obj();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_బఫరు (అక్షరం* బఫరు)
  {
    buffers.push (బఫరు);
    తిరిగి లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (buffers, కేటాయింపు_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  మూస<రకం_పేరు T>
  లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_link (T* స్థానభ్రంశం,
                                 గుర్తులేని మాతృ_id,
                                 గుర్తులేని బిడ్డ_id)
  {
    ఒకవేళ (unlikely (మాతృ_id >= vertices_.నిడివి || బిడ్డ_id >= vertices_.నిడివి))
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);
    స్వయంచాలక& v = vertices_[మాతృ_id];
    TRY(v.కూడు_real_link(2, బిడ్డ_id, (అక్షరం*) స్థానభ్రంశం - (అక్షరం*) v.obj().తల_2));
    తిరిగి vertices_[బిడ్డ_id].కూడు_మాతృ (మాతృ_id, అబద్ధం);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  లేఖాచిత్రం_ఫలితం_t<శూన్య> క్రమీకరించు_అతితక్కువ_దూరం_if_needed ()
  {
    ఒకవేళ (!positions_invalid) తిరిగి Ok();
    తిరిగి క్రమీకరించు_అతితక్కువ_దూరం ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  లేఖాచిత్రం_ఫలితం_t<శూన్య> క్రమీకరించు_అతితక్కువ_దూరం ()
  {
    positions_invalid = నిజం;

    ఒకవేళ (vertices_.నిడివి <= 1) {
      // Graph of 1 or less doesn't need sorting.
      తిరిగి Ok();
    }

    TRY (update_distances ());

    hb_ప్రాధాన్యత_వరుస_t<int64_t> వరుస;
    వరుస.alloc (vertices_.నిడివి);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (వరుస, కేటాయింపు_FAILURE));

    hb_సదిశ_t<గుర్తులేని> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.నిడివి);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (new_ordering, కేటాయింపు_FAILURE));

    hb_సదిశ_t<గుర్తులేని> removed_అంచులు;
    removed_అంచులు.resize (vertices_.నిడివి);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (removed_అంచులు, కేటాయింపు_FAILURE));

    TRY (update_parents ());

    వరుస.insert (మూలం ().modified_దూరం (0), మూలం_idx ());
    గుర్తులేని క్రమం = 1;
    గుర్తులేని pos = 0;
    అయితే (!వరుస.లోపల_error () && !వరుస.is_empty ())
    {
      గుర్తులేని తదుపరి_id = వరుస.తీయి_minimum().second;

      ఒకవేళ (unlikely (pos >= new_ordering.నిడివి)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Invalid graph. Contains cycle.");
        తిరిగి Err(చక్రం_DETECTED);
      }
      new_ordering[pos++] = తదుపరి_id;
      స్థిరరాశి శీర్షం_t& next = vertices_[తదుపరి_id];

      కొరకు (స్థిరరాశి స్వయంచాలక& link : next.obj().all_links ()) {
        ఒకవేళ (unlikely (link.objidx >= vertices_.నిడివి))
          తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);
        removed_అంచులు[link.objidx]++;
        స్థిరరాశి స్వయంచాలక& v = vertices_[link.objidx];
        ఒకవేళ (!(v.incoming_అంచులు_2 () - removed_అంచులు[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          వరుస.insert (v.modified_దూరం (క్రమం++),
                        link.objidx);
      }
    }

    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (వరుస, కేటాయింపు_FAILURE));
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (new_ordering, కేటాయింపు_FAILURE));

    hb_మార్పిడి (ordering_, new_ordering);

    ఒకవేళ (unlikely (pos != vertices_.నిడివి)) {
      print_orphaned_nodes ();
      తిరిగి Err(ORPHANED_NODES);
    }

    తిరిగి Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  లేఖాచిత్రం_ఫలితం_t<శూన్య> కనుగొను_space_roots (hb_సమితి_t& సందర్శించిన, hb_సమితి_t& roots)
  {
    గుర్తులేని మూలం_సూచీ = మూలం_idx ();
    కొరకు (గుర్తులేని i : ordering_)
    {
      ఒకవేళ (సందర్శించిన.has (i)) కొనసాగు;

      // Only real links can form 32 bit spaces
      కొరకు (స్థిరరాశి స్వయంచాలక& l : vertices_[i].obj().real_links)
      {
        ఒకవేళ (l.is_signed || l.వెడల్పు < 3)
          కొనసాగు;

        ఒకవేళ (i == మూలం_సూచీ && l.వెడల్పు == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          కొనసాగు;

        ఒకవేళ (l.వెడల్పు >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_సమితి_t sub_roots;
          TRY (కనుగొను_32bit_roots (l.objidx, sub_roots));
          ఒకవేళ (sub_roots) {
            కొరకు (గుర్తులేని sub_మూలం_idx : sub_roots) {
              roots.కూడు (sub_మూలం_idx);
              TRY (కనుగొను_subgraph (sub_మూలం_idx, సందర్శించిన));
            }
            కొనసాగు;
          }
        }

        roots.కూడు (l.objidx);
        TRY (కనుగొను_subgraph (l.objidx, సందర్శించిన));
      }
    }

    TRY(లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (సందర్శించిన, కేటాయింపు_FAILURE));
    TRY(లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (roots, కేటాయింపు_FAILURE));
    తిరిగి Ok();
  }

  మూస <రకం_పేరు T, రకం_పేరు ...Ts>
  లేఖాచిత్రం_ఫలితం_t<శీర్షం_and_పట్టిక_t<T, Immutable>> as_పట్టిక (గుర్తులేని మాతృ, స్థిరరాశి శూన్య* స్థానభ్రంశం, Ts... ds)
  {
    TRY_కేటాయించు (గుర్తులేని idx, సూచీ_for_స్థానభ్రంశం (మాతృ, స్థానభ్రంశం));
    తిరిగి as_పట్టిక_from_సూచీ<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  మూస <రకం_పేరు T, రకం_పేరు ...Ts>
  లేఖాచిత్రం_ఫలితం_t<శీర్షం_and_పట్టిక_t<T, Mutable>> as_mutable_పట్టిక (గుర్తులేని మాతృ, స్థిరరాశి శూన్య* స్థానభ్రంశం, Ts... ds)
  {
    TRY_కేటాయించు (స్వయంచాలక idx, mutable_సూచీ_for_స్థానభ్రంశం (మాతృ, స్థానభ్రంశం));
    తిరిగి as_పట్టిక_from_సూచీ<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  మూస <రకం_పేరు T, శీర్షం_mutability_t mutability = Mutable, రకం_పేరు ...Ts>
  లేఖాచిత్రం_ఫలితం_t<శీర్షం_and_పట్టిక_t<T, mutability>> as_పట్టిక_from_సూచీ (గుర్తులేని సూచీ_2, Ts... ds)
  {
    ఒకవేళ (unlikely (సూచీ_2 >= vertices_.నిడివి))
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

    శీర్షం_and_పట్టిక_t<T, mutability> r;
    r.శీర్షం = (రకం_పేరు std::conditional<mutability == Immutable, స్థిరరాశి శీర్షం_t*, శీర్షం_t*>::type) &vertices_[సూచీ_2];
    r.పట్టిక = r.శీర్షం->మూస as<రకం_పేరు std::conditional<mutability == Immutable, స్థిరరాశి T*, T*>::type> ();
    r.సూచీ_2 = సూచీ_2;
    ఒకవేళ (unlikely (!r.పట్టిక))
      తిరిగి Err(INVALID_వాదం);

    TRY(r.పట్టిక->sanitize (*(r.శీర్షం), std::forward<Ts>(ds)...));
    తిరిగి r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> సూచీ_for_స్థానభ్రంశం (గుర్తులేని కణుపు_idx, స్థిరరాశి శూన్య* స్థానభ్రంశం) స్థిరరాశి
  {
    ఒకవేళ (unlikely (కణుపు_idx >= vertices_.నిడివి)) తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);
    స్థిరరాశి స్వయంచాలక& node = object (కణుపు_idx);
    ఒకవేళ (unlikely (స్థానభ్రంశం < node.తల_2 || స్థానభ్రంశం >= node.తోక)) తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

    గుర్తులేని లెక్క = node.real_links.నిడివి;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      స్థిరరాశి స్వయంచాలక& link = node.real_links.arrayZ[i];
      ఒకవేళ (స్థానభ్రంశం != node.తల_2 + link.position)
        కొనసాగు;
      తిరిగి Ok(link.objidx);
    }

    తిరిగి Err(INVALID_వాదం);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> mutable_సూచీ_for_స్థానభ్రంశం (గుర్తులేని కణుపు_idx, స్థిరరాశి శూన్య* స్థానభ్రంశం)
  {
    TRY_కేటాయించు (గుర్తులేని బిడ్డ_idx, సూచీ_for_స్థానభ్రంశం (కణుపు_idx, స్థానభ్రంశం));
    ఒకవేళ (unlikely (బిడ్డ_idx >= vertices_.నిడివి))
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

    స్వయంచాలక& బిడ్డ = vertices_[బిడ్డ_idx];
    కొరకు (గుర్తులేని p : బిడ్డ.parents_iter ())
    {
      ఒకవేళ (p != కణుపు_idx) {
        తిరిగి duplicate (కణుపు_idx, బిడ్డ_idx, నిజం);
      }
    }

    తిరిగి Ok(బిడ్డ_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  లేఖాచిత్రం_ఫలితం_t<తార్కిక> కేటాయించు_spaces ()
  {
    TRY (update_parents ());

    కొరకు (స్వయంచాలక& శీర్షం : vertices_.writer ())
    {
      శీర్షం.space = 0;
      శీర్షం.ప్రాధాన్యత = 0;
    }
    అంకె_roots_for_space.resize (1);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (అంకె_roots_for_space, కేటాయింపు_FAILURE));
    అంకె_roots_for_space[0] = 1;
    దూరం_invalid = నిజం;
    positions_invalid = నిజం;

    hb_సమితి_t సందర్శించిన;
    hb_సమితి_t roots;
    TRY (కనుగొను_space_roots (సందర్శించిన, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    సందర్శించిన.invert ();

    ఒకవేళ (!roots) తిరిగి Ok(అబద్ధం);

    అయితే (roots)
    {
      uint32_t next = HB_సమితి_విలువ_INVALID;
      TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (roots, కేటాయింపు_FAILURE));
      ఒకవేళ (!roots.next (&next)) ఆపు;

      hb_సమితి_t connected_roots;
      TRY (కనుగొను_connected_nodes (next, roots, సందర్శించిన, connected_roots));
      TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (connected_roots, కేటాయింపు_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (connected_roots, కేటాయింపు_FAILURE));

      గుర్తులేని తదుపరి_space = ఇది->తదుపరి_space ();
      ఒకవేళ (unlikely (తదుపరి_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 తదుపరి_space);
        తిరిగి Err(హద్దు_EXCEEDED);
      }
      అంకె_roots_for_space.push (0);
      TRY(లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (అంకె_roots_for_space, కేటాయింపు_FAILURE));

      కొరకు (గుర్తులేని మూలం : connected_roots)
      {
        DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Subgraph %u gets space %u", మూలం, తదుపరి_space);
        vertices_[మూలం].space = తదుపరి_space;
        అంకె_roots_for_space[తదుపరి_space] = అంకె_roots_for_space[తదుపరి_space] + 1;
        దూరం_invalid = నిజం;
        positions_invalid = నిజం;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    తిరిగి Ok(నిజం);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  లేఖాచిత్రం_ఫలితం_t<తార్కిక> isolate_subgraph (hb_సమితి_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_సమితి_t parents;
    కొరకు (గుర్తులేని మూలం_idx : roots)
    {
      ఒకవేళ (unlikely (మూలం_idx >= vertices_.నిడివి))
        తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);
      subgraph.సమితి (మూలం_idx, wide_parents (మూలం_idx, parents));
    }
    కొరకు (గుర్తులేని మూలం_idx : roots)
    {
      TRY (కనుగొను_subgraph (మూలం_idx, subgraph));
    }
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (subgraph, కేటాయింపు_FAILURE));

    hb_map_t సూచీ_map;
    తార్కిక made_changes = అబద్ధం;
    కొరకు (స్వయంచాలక నమోదు_2 : subgraph.iter ())
    {
      ప్రతిజ్ఞా (నమోదు_2.మొదటి < vertices_.నిడివి);
      స్థిరరాశి స్వయంచాలక& node = vertices_[నమోదు_2.మొదటి];
      గుర్తులేని subgraph_incoming_అంచులు = నమోదు_2.second;

      ఒకవేళ (subgraph_incoming_అంచులు < node.incoming_అంచులు_2 ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = నిజం;
        TRY (duplicate_subgraph (నమోదు_2.మొదటి, సూచీ_map));
      }
    }

    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (సూచీ_map, కేటాయింపు_FAILURE));

    ఒకవేళ (!made_changes)
      తిరిగి Ok(అబద్ధం);

    స్వయంచాలక new_subgraph =
        + subgraph.కీలకాలు ()
        | hb_map([&] (uint32_t కణుపు_idx) {
          స్థిరరాశి uint32_t *v;
          ఒకవేళ (సూచీ_map.has (కణుపు_idx, &v)) తిరిగి *v;
          తిరిగి కణుపు_idx;
        })
        ;

    TRY(remap_obj_indices (సూచీ_map, new_subgraph));
    TRY(remap_obj_indices (సూచీ_map, parents.iter (), నిజం));

    // Update roots set with new indices as needed.
    కొరకు (స్వయంచాలక next : roots)
    {
      స్థిరరాశి uint32_t *v;
      ఒకవేళ (సూచీ_map.has (next, &v))
      {
        roots.del (next);
        roots.కూడు (*v);
      }
    }

    తిరిగి Ok(నిజం);
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
  మూస <రకం_పేరు సందర్శించు_అంచు_Func>
  లేఖాచిత్రం_ఫలితం_t<శూన్య> traverse_directed_bfs (గుర్తులేని ప్రారంభం_idx, సందర్శించు_అంచు_Func&& సందర్శించు_అంచు)
  {
    ఒకవేళ (unlikely (ప్రారంభం_idx >= vertices_.నిడివి))
    {
      DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.నిడివి);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (ordering_scratch_, కేటాయింపు_FAILURE));

    ఒకవేళ (!సందర్శించు_అంచు (HB_CODEPOINT_INVALID, శూన్యసూచిక, ప్రారంభం_idx, 0))
      తిరిగి Ok();

    గుర్తులేని తల_2 = 0;
    గుర్తులేని తోక = 0;
    స్వయంచాలక& వరుస = ordering_scratch_;
    వరుస[తోక++] = ప్రారంభం_idx;

    గుర్తులేని లోతు = 0;
    అయితే (తల_2 < తోక)
    {
      గుర్తులేని level_చివర = తోక;
      అయితే (తల_2 < level_చివర)
      {
        గుర్తులేని కణుపు_idx = వరుస[తల_2++];
        స్థిరరాశి స్వయంచాలక& v = vertices_[కణుపు_idx];

        గుర్తులేని అంకె_real = v.obj().real_links.నిడివి;
        గుర్తులేని మొత్తం_links = అంకె_real + v.obj().virtual_links.నిడివి;

        కొరకు (గుర్తులేని i = 0; i < మొత్తం_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          స్థిరరాశి స్వయంచాలక& v = vertices_[కణుపు_idx];
          స్థిరరాశి స్వయంచాలక& link = (i < అంకె_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - అంకె_real];
          గుర్తులేని బిడ్డ_idx = link.objidx;

          ఒకవేళ (!సందర్శించు_అంచు (కణుపు_idx, &link, బిడ్డ_idx, లోతు + 1))
            కొనసాగు;

          ఒకవేళ (unlikely (తోక >= వరుస.నిడివి))
            తిరిగి Err(చక్రం_DETECTED);

          వరుస[తోక++] = బిడ్డ_idx;
        }
      }
      లోతు++;
    }
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> కనుగొను_subgraph (గుర్తులేని కణుపు_idx, hb_map_t& subgraph)
  {
    తిరిగి traverse_directed_bfs (కణుపు_idx, [&] (
      గుర్తులేని మాతృ,
      స్థిరరాశి hb_serialize_context_t::object_t::link_t* link,
      గుర్తులేని బిడ్డ,
      గుర్తులేని లోతు) {
      ఒకవేళ (లోతు == 0) తిరిగి నిజం;
      hb_codepoint_t *లెక్క;
      ఒకవేళ (subgraph.has (బిడ్డ, &లెక్క))
      {
        (*లెక్క)++;
        తిరిగి అబద్ధం;
      }
      subgraph.సమితి (బిడ్డ, 1);
      తిరిగి నిజం;
    });
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> కనుగొను_subgraph (గుర్తులేని కణుపు_idx, hb_సమితి_t& subgraph)
  {
    తిరిగి traverse_directed_bfs (కణుపు_idx, [&] (
      గుర్తులేని మాతృ,
      స్థిరరాశి hb_serialize_context_t::object_t::link_t* link,
      గుర్తులేని బిడ్డ,
      గుర్తులేని లోతు) {
      ఒకవేళ (subgraph.has (బిడ్డ)) తిరిగి అబద్ధం;
      subgraph.కూడు (బిడ్డ);
      తిరిగి నిజం;
    });
  }

  లేఖాచిత్రం_ఫలితం_t<size_t> కనుగొను_subgraph_కొలత (గుర్తులేని కణుపు_idx, hb_సమితి_t& subgraph, గుర్తులేని max_లోతు = HB_లేఖాచిత్రం_INVALID)
  {
    size_t కొలత = 0;
    TRY (traverse_directed_bfs (కణుపు_idx, [&] (
      గుర్తులేని మాతృ,
      స్థిరరాశి hb_serialize_context_t::object_t::link_t* link,
      గుర్తులేని బిడ్డ,
      గుర్తులేని లోతు) {
      ఒకవేళ (subgraph.has (బిడ్డ)) తిరిగి అబద్ధం;
      subgraph.కూడు (బిడ్డ);

      స్థిరరాశి స్వయంచాలక& o = vertices_[బిడ్డ].obj();
      కొలత += o.తోక - o.తల_2;
      తిరిగి లోతు < max_లోతు;
    }));
    తిరిగి Ok(కొలత);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  లేఖాచిత్రం_ఫలితం_t<శూన్య> కనుగొను_32bit_roots (గుర్తులేని కణుపు_idx, hb_సమితి_t& దొరికింది)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_సమితి_t సందర్శించిన;
    తిరిగి traverse_directed_bfs (కణుపు_idx, [&] (
      గుర్తులేని మాతృ,
      స్థిరరాశి hb_serialize_context_t::object_t::link_t* link,
      గుర్తులేని బిడ్డ,
      గుర్తులేని _) {

      ఒకవేళ (link && దొరికింది.has(మాతృ))
        // Don't traverse from something that's already marked as a root.
        తిరిగి అబద్ధం;

      ఒకవేళ (link && !link->is_signed && link->వెడల్పు == 4)
      {
        దొరికింది.కూడు (link->objidx);
        సందర్శించిన.కూడు (link->objidx);
        తిరిగి అబద్ధం;
      }

      ఒకవేళ (సందర్శించిన.has (బిడ్డ)) తిరిగి అబద్ధం;
      సందర్శించిన.కూడు (బిడ్డ);
      తిరిగి నిజం;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  మూస<రకం_పేరు O>
  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> move_బిడ్డ (గుర్తులేని old_మాతృ_idx,
                                       స్థిరరాశి O* old_స్థానభ్రంశం,
                                       గుర్తులేని new_మాతృ_idx,
                                       స్థిరరాశి O* new_స్థానభ్రంశం)
  {
    దూరం_invalid = నిజం;
    positions_invalid = నిజం;

    ఒకవేళ (unlikely (old_మాతృ_idx >= vertices_.నిడివి ||
                  new_మాతృ_idx >= vertices_.నిడివి))
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

    స్వయంచాలక& old_v = vertices_[old_మాతృ_idx];
    స్వయంచాలక& new_v = vertices_[new_మాతృ_idx];

    TRY_కేటాయించు (గుర్తులేని బిడ్డ_id, సూచీ_for_స్థానభ్రంశం (old_మాతృ_idx,
                                                     old_స్థానభ్రంశం));
    ఒకవేళ (unlikely (బిడ్డ_id >= vertices_.నిడివి))
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

    TRY(new_v.కూడు_real_link(O::static_కొలత, బిడ్డ_id, (స్థిరరాశి అక్షరం*) new_స్థానభ్రంశం - (స్థిరరాశి అక్షరం*) new_v.obj().తల_2));

    స్వయంచాలక& బిడ్డ = vertices_[బిడ్డ_id];
    TRY(బిడ్డ.కూడు_మాతృ (new_మాతృ_idx, అబద్ధం));

    old_v.remove_real_link_unordered (బిడ్డ_id, old_స్థానభ్రంశం);
    బిడ్డ.remove_మాతృ (old_మాతృ_idx);

    తిరిగి Ok(బిడ్డ_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  మూస<రకం_పేరు O>
  లేఖాచిత్రం_ఫలితం_t<శూన్య> move_children (గుర్తులేని old_మాతృ_idx,
                                      గుర్తులేని old_pos_ప్రారంభం,
                                      గుర్తులేని old_pos_చివర,
                                      గుర్తులేని new_మాతృ_idx,
                                      గుర్తులేని new_pos_ప్రారంభం)
  {
    ఒకవేళ (unlikely (old_మాతృ_idx >= vertices_.నిడివి ||
                  new_మాతృ_idx >= vertices_.నిడివి))
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

    దూరం_invalid = నిజం;
    positions_invalid = నిజం;

    స్వయంచాలక& old_v = vertices_[old_మాతృ_idx];
    స్వయంచాలక& new_v = vertices_[new_మాతృ_idx];

    hb_సదిశ_t<hb_serialize_context_t::object_t::link_t> old_links;
    కొరకు (స్థిరరాశి స్వయంచాలక& l : old_v.obj().real_links)
    {
      ఒకవేళ (l.position < old_pos_ప్రారంభం || l.position >= old_pos_చివర)
      {
        old_links.push (l);
        కొనసాగు;
      }

      గుర్తులేని array_pos = l.position - old_pos_ప్రారంభం;

      గుర్తులేని బిడ్డ_id = l.objidx;
      ఒకవేళ (unlikely (బిడ్డ_id >= vertices_.నిడివి))
        తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

      TRY(new_v.కూడు_real_link(O::static_కొలత, బిడ్డ_id, new_pos_ప్రారంభం + array_pos));

      స్వయంచాలక& బిడ్డ = vertices_[బిడ్డ_id];
      TRY(బిడ్డ.కూడు_మాతృ (new_మాతృ_idx, అబద్ధం));
      బిడ్డ.remove_మాతృ (old_మాతృ_idx);
    }

    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (old_links, కేటాయింపు_FAILURE));
    తిరిగి old_v.సమితి_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  లేఖాచిత్రం_ఫలితం_t<శూన్య> duplicate_subgraph (గుర్తులేని కణుపు_idx, hb_map_t& సూచీ_map)
  {
    లేఖాచిత్రం_ఫలితం_t<శూన్య> ఫలితం = Ok();
    TRY (traverse_directed_bfs (కణుపు_idx, [&] (
      గుర్తులేని మాతృ,
      స్థిరరాశి hb_serialize_context_t::object_t::link_t* link,
      గుర్తులేని బిడ్డ,
      గుర్తులేని _) {
      ఒకవేళ (ఫలితం.is_err () || సూచీ_map.has (బిడ్డ)) తిరిగి అబద్ధం;
      స్వయంచాలక r = duplicate (బిడ్డ);
      ఒకవేళ (unlikely (r.is_err ()))
      {
        ఫలితం = Err(r.error ());
        తిరిగి అబద్ధం;
      }
      సూచీ_map.సమితి (బిడ్డ, *r);
      తిరిగి నిజం;
    }));

    తిరిగి లేఖాచిత్రం_ఫలితం_t<శూన్య>::from(సూచీ_map, కేటాయింపు_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> duplicate (గుర్తులేని కణుపు_idx, తార్కిక copy_పట్టిక = అబద్ధం)
  {
    ఒకవేళ (unlikely (కణుపు_idx >= vertices_.నిడివి))
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

    ఒకవేళ (unlikely (vertices_.నిడివి >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.నిడివి);
      తిరిగి Err(హద్దు_EXCEEDED);
    }

    positions_invalid = నిజం;
    దూరం_invalid = నిజం;

    స్వయంచాలక* clone = vertices_.push ();
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (vertices_, కేటాయింపు_FAILURE));

    గుర్తులేని clone_idx = vertices_.నిడివి - 1;
    ordering_.push (clone_idx);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (ordering_, కేటాయింపు_FAILURE));

    స్వయంచాలక& బిడ్డ = vertices_[కణుపు_idx];

    గుర్తులేని పట్టిక_కొలత = బిడ్డ.obj().తోక - బిడ్డ.obj().తల_2;
    ఒకవేళ (copy_పట్టిక && పట్టిక_కొలత)
    {
      అక్షరం* బఫరు = (అక్షరం*) hb_malloc (పట్టిక_కొలత);
      ఒకవేళ (unlikely (!బఫరు))
        తిరిగి Err(కేటాయింపు_FAILURE);
      స్వయంచాలక res = కూడు_బఫరు (బఫరు);
      ఒకవేళ (unlikely (res.is_err ()))
      {
        hb_free (బఫరు);
        తిరిగి Err(res.error ());
      }
      hb_memcpy (బఫరు, బిడ్డ.obj().తల_2, పట్టిక_కొలత);
      clone->సమితి_బఫరు(బఫరు, బఫరు + పట్టిక_కొలత);
    }
    లేకపోతే
    {
      clone->సమితి_బఫరు(బిడ్డ.obj().తల_2, బిడ్డ.obj().తోక);
    }
    clone->distance = బిడ్డ.distance;
    clone->space = బిడ్డ.space;
    clone->reset_parents ();

    కొరకు (స్థిరరాశి స్వయంచాలక& l : బిడ్డ.obj().real_links)
    {
      TRY(clone->కూడు_real_link (l));
      TRY(vertices_[l.objidx].కూడు_మాతృ (clone_idx, అబద్ధం));
    }

    కొరకు (స్థిరరాశి స్వయంచాలక& l : బిడ్డ.obj().virtual_links)
    {
      TRY(clone->కూడు_virtual_link (l));
      TRY(vertices_[l.objidx].కూడు_మాతృ (clone_idx, నిజం));
    }

    తిరిగి Ok(clone_idx);
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
  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> duplicate (గుర్తులేని మాతృ_idx, గుర్తులేని బిడ్డ_idx, తార్కిక copy_పట్టిక = అబద్ధం)
  {
    ఒకవేళ (unlikely (మాతృ_idx >= vertices_.నిడివి || బిడ్డ_idx >= vertices_.నిడివి))
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

    TRY (update_parents ());

    స్థిరరాశి స్వయంచాలక& బిడ్డ = vertices_[బిడ్డ_idx];
    గుర్తులేని links_గమ్యం_బిడ్డ = బిడ్డ.incoming_అంచులు_from_మాతృ (మాతృ_idx);

    ఒకవేళ (unlikely (బిడ్డ.incoming_అంచులు_2 () <= links_గమ్యం_బిడ్డ || బిడ్డ.has_incoming_virtual_అంచులు_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "  Not duplicating %u => %u",
                 మాతృ_idx, బిడ్డ_idx);
      తిరిగి Err(INVALID_వాదం);
    }

    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "  Duplicating %u => %u",
               మాతృ_idx, బిడ్డ_idx);

    TRY_కేటాయించు (గుర్తులేని clone_idx, duplicate (బిడ్డ_idx, copy_పట్టిక));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    ఒకవేళ (మాతృ_idx == clone_idx) మాతృ_idx++;

    స్వయంచాలక& మాతృ = vertices_[మాతృ_idx];
    గుర్తులేని లెక్క = 0;
    గుర్తులేని అంకె_real = మాతృ.obj().real_links.నిడివి;
    కొరకు (స్వయంచాలక& l : మాతృ.all_links_writer ())
    {
      లెక్క++;
      ఒకవేళ (l.objidx != బిడ్డ_idx)
        కొనసాగు;

      TRY(reassign_link (l, మాతృ_idx, clone_idx, లెక్క > అంకె_real));
    }

    తిరిగి Ok(clone_idx);
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
  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> duplicate (స్థిరరాశి hb_సమితి_t* parents, గుర్తులేని బిడ్డ_idx)
  {
    ఒకవేళ (unlikely (!parents || parents->is_empty ()))
      తిరిగి Err(INVALID_వాదం);

    ఒకవేళ (unlikely (బిడ్డ_idx >= vertices_.నిడివి))
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

    TRY (update_parents ());

    స్థిరరాశి స్వయంచాలక& బిడ్డ = vertices_[బిడ్డ_idx];
    గుర్తులేని links_గమ్యం_బిడ్డ = 0;
    గుర్తులేని చివరి_మాతృ = parents->get_max ();
    గుర్తులేని మొదటి_మాతృ = parents->get_min ();
    కొరకు (గుర్తులేని మాతృ_idx : *parents) {
      ఒకవేళ (unlikely (మాతృ_idx >= vertices_.నిడివి))
        తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);
      links_గమ్యం_బిడ్డ += బిడ్డ.incoming_అంచులు_from_మాతృ (మాతృ_idx);
    }

    ఒకవేళ (unlikely (బిడ్డ.incoming_అంచులు_2 () <= links_గమ్యం_బిడ్డ || బిడ్డ.has_incoming_virtual_అంచులు_2 ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "  Not duplicating %u, ..., %u => %u", మొదటి_మాతృ, చివరి_మాతృ, బిడ్డ_idx);
      తిరిగి Err(INVALID_వాదం);
    }

    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "  Duplicating %u, ..., %u => %u", మొదటి_మాతృ, చివరి_మాతృ, బిడ్డ_idx);

    TRY_కేటాయించు (గుర్తులేని clone_idx, duplicate (బిడ్డ_idx));

    కొరకు (గుర్తులేని మాతృ_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      ఒకవేళ (మాతృ_idx == clone_idx) మాతృ_idx++;
      స్వయంచాలక& మాతృ = vertices_[మాతృ_idx];
      గుర్తులేని లెక్క = 0;
      గుర్తులేని అంకె_real = మాతృ.obj().real_links.నిడివి;
      కొరకు (స్వయంచాలక& l : మాతృ.all_links_writer ())
      {
        లెక్క++;
        ఒకవేళ (l.objidx != బిడ్డ_idx)
          కొనసాగు;

        TRY(reassign_link (l, మాతృ_idx, clone_idx, లెక్క > అంకె_real));
      }
    }

    తిరిగి Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> new_కణుపు (అక్షరం* తల_2, అక్షరం* తోక)
  {
    ఒకవేళ (unlikely (vertices_.నిడివి >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.నిడివి);
      తిరిగి Err(హద్దు_EXCEEDED);
    }

    positions_invalid = నిజం;
    దూరం_invalid = నిజం;

    స్వయంచాలక* clone = vertices_.push ();
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (vertices_, కేటాయింపు_FAILURE));

    గుర్తులేని clone_idx = vertices_.నిడివి - 1;
    ordering_.push (clone_idx);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (ordering_, కేటాయింపు_FAILURE));

    clone->సమితి_బఫరు (తల_2, తోక);
    clone->distance = 0;
    clone->space = 0;

    తిరిగి Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> remap_బిడ్డ (గుర్తులేని మాతృ_idx, గుర్తులేని old_బిడ్డ_idx)
  {
    ఒకవేళ (unlikely (మాతృ_idx >= vertices_.నిడివి || old_బిడ్డ_idx >= vertices_.నిడివి))
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

    TRY_కేటాయించు (గుర్తులేని new_బిడ్డ_idx, duplicate (old_బిడ్డ_idx));

    స్వయంచాలక& మాతృ = vertices_[మాతృ_idx];
    కొరకు (స్వయంచాలక& l : మాతృ.real_links_writer ())
    {
      ఒకవేళ (l.objidx != old_బిడ్డ_idx)
        కొనసాగు;
      TRY(reassign_link (l, మాతృ_idx, new_బిడ్డ_idx, అబద్ధం));
    }

    కొరకు (స్వయంచాలక& l : మాతృ.virtual_links_writer ())
    {
      ఒకవేళ (l.objidx != old_బిడ్డ_idx)
        కొనసాగు;
      TRY(reassign_link (l, మాతృ_idx, new_బిడ్డ_idx, నిజం));
    }
    తిరిగి Ok(new_బిడ్డ_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  లేఖాచిత్రం_ఫలితం_t<తార్కిక> raise_childrens_ప్రాధాన్యత (గుర్తులేని మాతృ_idx)
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "  Raising priority of all children of %u",
               మాతృ_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    ఒకవేళ (unlikely (మాతృ_idx >= vertices_.నిడివి)) తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);
    తార్కిక made_change = అబద్ధం;
    కొరకు (స్వయంచాలక& l : vertices_[మాతృ_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_ప్రాధాన్యత ();
    తిరిగి Ok(made_change);
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> is_fully_connected ()
  {
    TRY(update_parents());

    ఒకవేళ (unlikely (మూలం().incoming_అంచులు_2 ()))
      // Root cannot have parents.
      తిరిగి Err(ORPHANED_NODES);

    కొరకు (గుర్తులేని i = 0; i < మూలం_idx (); i++)
    {
      ఒకవేళ (unlikely (!vertices_[i].incoming_అంచులు_2 ()))
        తిరిగి Err(ORPHANED_NODES);
    }
    తిరిగి Ok();
  }

#ఒకవేళ 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  శూన్య save_fuzzer_విత్తనం (hb_ట్యాగ్_t ట్యాగ్_2) స్థిరరాశి
  {
    FILE* f = fopen ("./repacker_fuzzer_seed", "w");
    fwrite ((శూన్య*) &ట్యాగ్_2, పరిమాణం (ట్యాగ్_2), 1, f);

    uint16_t అంకె_objects = vertices_.నిడివి;
    fwrite ((శూన్య*) &అంకె_objects, పరిమాణం (అంకె_objects), 1, f);

    కొరకు (స్థిరరాశి స్వయంచాలక& v : vertices_)
    {
      uint16_t blob_కొలత = v.పట్టిక_కొలత ();
      fwrite ((శూన్య*) &blob_కొలత, పరిమాణం (blob_కొలత), 1, f);
      fwrite ((స్థిరరాశి శూన్య*) v.obj.తల_2, blob_కొలత, 1, f);
    }

    uint16_t link_లెక్క = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& v : vertices_)
      link_లెక్క += v.obj.real_links.నిడివి;

    fwrite ((శూన్య*) &link_లెక్క, పరిమాణం (link_లెక్క), 1, f);

    రకనిర్వచనం నిర్మాణం
    {
      uint16_t మాతృ;
      uint16_t బిడ్డ;
      uint16_t position;
      uint8_t వెడల్పు;
    } link_t;

    కొరకు (గుర్తులేని i = 0; i < vertices_.నిడివి; i++)
    {
      కొరకు (స్థిరరాశి స్వయంచాలక& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.వెడల్పు
        };
        fwrite ((శూన్య*) &link, పరిమాణం (link), 1, f);
      }
    }

    fclose (f);
  }
#అయితే_ముగింపు

  గుర్తులేని అంకె_roots_for_space_2 (గుర్తులేని space) స్థిరరాశి
  {
    తిరిగి అంకె_roots_for_space[space];
  }

  గుర్తులేని తదుపరి_space () స్థిరరాశి
  {
    తిరిగి అంకె_roots_for_space.నిడివి;
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> move_గమ్యం_new_space (స్థిరరాశి hb_సమితి_t& indices)
  {
    ఒకవేళ (unlikely (అంకె_roots_for_space.నిడివి >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 అంకె_roots_for_space.నిడివి);
      తిరిగి Err(హద్దు_EXCEEDED);
    }
    అంకె_roots_for_space.push (0);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (అంకె_roots_for_space, కేటాయింపు_FAILURE));
    గుర్తులేని new_space = అంకె_roots_for_space.నిడివి - 1;

    కొరకు (గుర్తులేని సూచీ_2 : indices) {
      ఒకవేళ (unlikely (సూచీ_2 >= vertices_.నిడివి))
        తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);
      స్వయంచాలక& node = vertices_[సూచీ_2];
      అంకె_roots_for_space[node.space] = అంకె_roots_for_space[node.space] - 1;
      అంకె_roots_for_space[new_space] = అంకె_roots_for_space[new_space] + 1;
      node.space = new_space;
      దూరం_invalid = నిజం;
      positions_invalid = నిజం;
    }
    తిరిగి Ok();
  }

  గుర్తులేని space_for (గుర్తులేని సూచీ_2, గుర్తులేని* మూలం = శూన్యసూచిక) స్థిరరాశి
  {
    అయితే (నిజం)
    {
      ప్రతిజ్ఞా (సూచీ_2 < vertices_.నిడివి);
      స్థిరరాశి స్వయంచాలక& node = vertices_[సూచీ_2];
      ఒకవేళ (node.space)
      {
        ఒకవేళ (మూలం != శూన్యసూచిక)
          *మూలం = సూచీ_2;
        తిరిగి node.space;
      }

      ఒకవేళ (!node.incoming_అంచులు_2 ())
      {
        ఒకవేళ (మూలం)
          *మూలం = సూచీ_2;
        తిరిగి 0;
      }

      సూచీ_2 = *node.parents_iter ();
    }
  }

  size_t మొత్తం_కొలత_లోపల_బైట్లు () స్థిరరాశి {
    size_t మొత్తం_కొలత = 0;
    గుర్తులేని లెక్క = vertices_.నిడివి;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++) {
      size_t కొలత = vertices_.arrayZ[i].పట్టిక_కొలత ();
      మొత్తం_కొలత += కొలత;
    }
    తిరిగి మొత్తం_కొలత;
  }


 అంతరంగ:

  శూన్య print_orphaned_nodes ()
  {
    ఒకవేళ (!DEBUG_ENABLED(SUBSET_REPACK)) తిరిగి;

    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Graph is not fully connected.");

    parents_invalid = నిజం;
    (శూన్య) update_parents();

    ఒకవేళ (మూలం().incoming_అంచులు_2 ()) {
      DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Root node has incoming edges.");
    }

    కొరకు (గుర్తులేని i = 0; i < vertices_.నిడివి; i++)
    {
      స్థిరరాశి స్వయంచాలక& v = vertices_[i];
      ఒకవేళ (!v.incoming_అంచులు_2 ())
        DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  గుర్తులేని wide_parents (గుర్తులేని కణుపు_idx, hb_సమితి_t& parents) స్థిరరాశి
  {
    గుర్తులేని లెక్క = 0;
    కొరకు (గుర్తులేని p : vertices_[కణుపు_idx].parents_iter ())
    {
      // Only real links can be wide
      కొరకు (స్థిరరాశి స్వయంచాలక& l : vertices_[p].obj().real_links)
      {
        ఒకవేళ (l.objidx == కణుపు_idx
            && (l.వెడల్పు == 3 || l.వెడల్పు == 4)
            && !l.is_signed)
        {
          లెక్క++;
          parents.కూడు (p);
        }
      }
    }
    తిరిగి లెక్క;
  }

 బహిరంగ:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  లేఖాచిత్రం_ఫలితం_t<శూన్య> update_parents ()
  {
    ఒకవేళ (!parents_invalid) తిరిగి Ok();

    గుర్తులేని లెక్క = vertices_.నిడివి;

    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      vertices_.arrayZ[i].reset_parents ();

    కొరకు (గుర్తులేని p = 0; p < లెక్క; p++)
    {
      కొరకు (స్థిరరాశి స్వయంచాలక& l : vertices_.arrayZ[p].obj().real_links)
      {
        ఒకవేళ (unlikely (l.objidx >= లెక్క)) తిరిగి Err(INVALID_వాదం);
        TRY(vertices_[l.objidx].కూడు_మాతృ (p, అబద్ధం));
      }

      కొరకు (స్థిరరాశి స్వయంచాలక& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        ఒకవేళ (unlikely (l.objidx >= లెక్క)) తిరిగి Err(INVALID_వాదం);
        TRY(vertices_[l.objidx].కూడు_మాతృ (p, నిజం));
      }
    }

    parents_invalid = అబద్ధం;
    తిరిగి Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  శూన్య update_positions ()
  {
    ఒకవేళ (!positions_invalid) తిరిగి;

    గుర్తులేని current_pos = 0;
    కొరకు (గుర్తులేని i : ordering_)
    {
      స్వయంచాలక& v = vertices_[i];
      v.ప్రారంభం = current_pos;
      current_pos += v.obj().తోక - v.obj().తల_2;
      v.చివర = current_pos;
    }

    positions_invalid = అబద్ధం;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  లేఖాచిత్రం_ఫలితం_t<శూన్య> update_distances ()
  {
    ఒకవేళ (!దూరం_invalid) తిరిగి Ok();

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
    గుర్తులేని లెక్క = vertices_.నిడివి;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[మూలం_idx ()].distance = 0;

    hb_ప్రాధాన్యత_వరుస_t<int64_t> వరుస;
    వరుస.alloc (లెక్క);
    వరుస.insert (0, మూలం_idx ());
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (వరుస, కేటాయింపు_FAILURE));

    hb_సదిశ_t<తార్కిక> సందర్శించిన;
    సందర్శించిన.resize (vertices_.నిడివి);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (సందర్శించిన, కేటాయింపు_FAILURE));

    అయితే (!వరుస.లోపల_error () && !వరుస.is_empty ())
    {
      గుర్తులేని తదుపరి_idx = వరుస.తీయి_minimum ().second;
      ఒకవేళ (సందర్శించిన[తదుపరి_idx]) కొనసాగు;
      స్థిరరాశి స్వయంచాలక& next = vertices_[తదుపరి_idx];
      int64_t తదుపరి_దూరం = next.distance;
      సందర్శించిన[తదుపరి_idx] = నిజం;

      కొరకు (స్థిరరాశి స్వయంచాలక& link : next.obj().all_links ())
      {
        ఒకవేళ (unlikely (link.objidx >= vertices_.నిడివి))
          తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);

        ఒకవేళ (సందర్శించిన[link.objidx]) కొనసాగు;

        స్వయంచాలక& బిడ్డ_v = vertices_.arrayZ[link.objidx];
        స్థిరరాశి స్వయంచాలక& బిడ్డ = బిడ్డ_v.obj ();
        గుర్తులేని link_వెడల్పు = link.వెడల్పు ? link.వెడల్పు : 4; // treat virtual offsets as 32 bits wide
        int64_t బిడ్డ_బరువు = (బిడ్డ.తోక - బిడ్డ.తల_2) +
                               ((int64_t) 1 << (link_వెడల్పు * 8)) * (బిడ్డ_v.space + 1);
        int64_t బిడ్డ_దూరం = తదుపరి_దూరం + బిడ్డ_బరువు;

        ఒకవేళ (బిడ్డ_దూరం < బిడ్డ_v.distance)
        {
          బిడ్డ_v.distance = బిడ్డ_దూరం;
          వరుస.insert (బిడ్డ_దూరం, link.objidx);
        }
      }
    }

    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (వరుస, కేటాయింపు_FAILURE));
    ఒకవేళ (unlikely (!వరుస.is_empty ()))
    {
      print_orphaned_nodes ();
      తిరిగి Err(ORPHANED_NODES);
    }

    దూరం_invalid = అబద్ధం;
    తిరిగి Ok();
  }

 అంతరంగ:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  లేఖాచిత్రం_ఫలితం_t<శూన్య> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      గుర్తులేని మాతృ_idx,
                      గుర్తులేని new_idx,
                      తార్కిక is_virtual)
  {
    గుర్తులేని old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_మాతృ (మాతృ_idx);
    తిరిగి vertices_[new_idx].కూడు_మాతృ (మాతృ_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  మూస<రకం_పేరు Iterator, hb_requires (hb_is_iterator (Iterator))>
  లేఖాచిత్రం_ఫలితం_t<శూన్య> remap_obj_indices (స్థిరరాశి hb_map_t& id_map,
                                          Iterator subgraph,
                                          తార్కిక only_wide = అబద్ధం)
  {
    ఒకవేళ (!id_map) తిరిగి Ok();
    కొరకు (గుర్తులేని i : subgraph)
    {
      స్వయంచాలక& శీర్షం = vertices_[i];
      గుర్తులేని అంకె_real = శీర్షం.obj().real_links.నిడివి;
      గుర్తులేని లెక్క = 0;
      కొరకు (స్వయంచాలక& link : శీర్షం.all_links_writer ())
      {
        లెక్క++;
        స్థిరరాశి uint32_t *v;
        ఒకవేళ (!id_map.has (link.objidx, &v)) కొనసాగు;
        ఒకవేళ (only_wide && (link.is_signed || (link.వెడల్పు != 4 && link.వెడల్పు != 3))) కొనసాగు;

        TRY(reassign_link (link, i, *v, లెక్క > అంకె_real));
      }
    }
    తిరిగి Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  లేఖాచిత్రం_ఫలితం_t<శూన్య> కనుగొను_connected_nodes (గుర్తులేని ప్రారంభం_idx,
                                             hb_సమితి_t& targets,
                                             hb_సమితి_t& సందర్శించిన,
                                             hb_సమితి_t& connected)
  {
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (సందర్శించిన, కేటాయింపు_FAILURE));
    ఒకవేళ (సందర్శించిన.has (ప్రారంభం_idx)) తిరిగి Ok();
    ఒకవేళ (unlikely (ప్రారంభం_idx >= vertices_.నిడివి))
    {
      DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      తిరిగి Err(బహిర్గతం_యొక్క_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.నిడివి);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (ordering_scratch_, కేటాయింపు_FAILURE));

    స్వయంచాలక& దొంతర = ordering_scratch_;
    గుర్తులేని దొంతర_len = 0;

    లేఖాచిత్రం_ఫలితం_t<శూన్య> res = Ok();
    స్వయంచాలక handle_కణుపు = [&] (గుర్తులేని కణుపు_idx) {
      సందర్శించిన.కూడు (కణుపు_idx);
      ఒకవేళ (targets.has (కణుపు_idx)) {
        targets.del (కణుపు_idx);
        connected.కూడు (కణుపు_idx);
      }

      ఒకవేళ (unlikely (దొంతర_len >= దొంతర.నిడివి))
      {
        res = Err(చక్రం_DETECTED);
        తిరిగి అబద్ధం;
      }
      దొంతర[దొంతర_len++] = కణుపు_idx;
      తిరిగి నిజం;
    };

    ఒకవేళ (!handle_కణుపు (ప్రారంభం_idx)) తిరిగి res;

    అయితే (దొంతర_len > 0)
    {
      గుర్తులేని కణుపు_idx = దొంతర[--దొంతర_len];
      స్థిరరాశి స్వయంచాలక& v = vertices_[కణుపు_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      కొరకు (స్థిరరాశి స్వయంచాలక& l : v.obj().all_links ())
      {
        గుర్తులేని బిడ్డ_idx = l.objidx;
        ఒకవేళ (సందర్శించిన.has (బిడ్డ_idx)) కొనసాగు;
        ఒకవేళ (!handle_కణుపు (బిడ్డ_idx)) తిరిగి res;
      }

      కొరకు (గుర్తులేని మాతృ_idx : v.parents_iter ())
      {
        ఒకవేళ (సందర్శించిన.has (మాతృ_idx)) కొనసాగు;
        ఒకవేళ (!handle_కణుపు (మాతృ_idx)) తిరిగి res;
      }
    }

    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (సందర్శించిన, కేటాయింపు_FAILURE));
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (connected, కేటాయింపు_FAILURE));

    తిరిగి Ok();
  }

 బహిరంగ:
  // TODO(garretrieger): make private, will need to move most of offset overflow code into graph.
  hb_సదిశ_t<శీర్షం_t> vertices_;

  // Specifies the current topological ordering of this graph
  //
  // ordering_[pos] = obj index
  //
  // specifies that the 'pos'th spot is filled by the object
  // given by obj index.
  hb_సదిశ_t<గుర్తులేని> ordering_;
  hb_సదిశ_t<గుర్తులేని> ordering_scratch_;

 అంతరంగ:
  తార్కిక parents_invalid = నిజం;
  తార్కిక దూరం_invalid = నిజం;
  తార్కిక positions_invalid = నిజం;
  hb_సదిశ_t<గుర్తులేని> అంకె_roots_for_space;
  hb_సదిశ_t<అక్షరం*> buffers;
};

}

#అయితే_ముగింపు  // GRAPH_GRAPH_HH
