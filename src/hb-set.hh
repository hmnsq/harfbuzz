/*
 * Copyright © 2012,2017  Google, Inc.
 * Copyright © 2021 Behdad Esfahbod
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
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_సమితి_HH
#నిర్వచించు HB_సమితి_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-bit-set-invertible.hh"
#అన్తర్భావ "hb-bit-vector.hh" // Just to include


మూస <రకం_పేరు impl_t>
నిర్మాణం hb_sparseset_t
{
  స్థిర constexpr తార్కిక realloc_move = నిజం;

  hb_object_శీర్షిక_t శీర్షిక;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (స్థిరరాశి hb_sparseset_t& other) : hb_sparseset_t () { సమితి (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& క్రియాకారకం = (స్థిరరాశి hb_sparseset_t& other) { సమితి (other); తిరిగి *ఇది; }
  hb_sparseset_t& క్రియాకారకం = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); తిరిగి *ఇది; }
  మిత్ర శూన్య swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_మార్పిడి (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    కొరకు (స్వయంచాలక&& అంశం : lst)
      కూడు (అంశం);
  }
  మూస <రకం_పేరు Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (స్థిరరాశి Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *ఇది);
  }

  శూన్య init ()
  {
    hb_object_init (ఇది);
    s.init ();
  }
  శూన్య fini ()
  {
    hb_object_fini (ఇది);
    s.fini ();
  }

  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి !is_empty (); }

  శూన్య err () { s.err (); }
  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి s.లోపల_error (); }

  శూన్య alloc (గుర్తులేని sz) { s.alloc (sz); }
  శూన్య reset () { s.reset (); }
  శూన్య clear () { s.clear (); }
  శూన్య invert () { s.invert (); }
  తార్కిక is_inverted () స్థిరరాశి { తిరిగి s.is_inverted (); }
  తార్కిక is_empty () స్థిరరాశి { తిరిగి s.is_empty (); }
  uint32_t hash () స్థిరరాశి { తిరిగి s.hash (); }

  శూన్య కూడు (hb_codepoint_t g) { s.కూడు (g); }
  శూన్య కూడు_బిట్లు (hb_codepoint_t g, uint64_t బిట్లు) { s.కూడు_బిట్లు (g, బిట్లు); }
  తార్కిక కూడు_range (hb_codepoint_t మొదటి, hb_codepoint_t చివరి) { తిరిగి s.కూడు_range (మొదటి, చివరి); }

  మూస <రకం_పేరు T>
  శూన్య కూడు_array (స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  { s.కూడు_array (array, లెక్క, stride); }
  మూస <రకం_పేరు T>
  శూన్య కూడు_array (స్థిరరాశి hb_array_t<స్థిరరాశి T>& arr) { కూడు_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  మూస <రకం_పేరు T>
  తార్కిక కూడు_sorted_array (స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  { తిరిగి s.కూడు_sorted_array (array, లెక్క, stride); }
  మూస <రకం_పేరు T>
  తార్కిక కూడు_sorted_array (స్థిరరాశి hb_sorted_array_t<స్థిరరాశి T>& arr) { తిరిగి కూడు_sorted_array (&arr, arr.len ()); }

  శూన్య del (hb_codepoint_t g) { s.del (g); }
  శూన్య del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  తార్కిక get (hb_codepoint_t g) స్థిరరాశి { తిరిగి s.get (g); }
  తార్కిక may_have (hb_codepoint_t g) స్థిరరాశి { తిరిగి get (g); }

  /* Has interface. */
  తార్కిక క్రియాకారకం [] (hb_codepoint_t k) స్థిరరాశి { తిరిగి get (k); }
  తార్కిక has (hb_codepoint_t k) స్థిరరాశి { తిరిగి (*ఇది)[k]; }

  /* Predicate. */
  తార్కిక క్రియాకారకం () (hb_codepoint_t k) స్థిరరాశి { తిరిగి has (k); }

  /* Sink interface. */
  hb_sparseset_t& క్రియాకారకం << (hb_codepoint_t v)
  { కూడు (v); తిరిగి *ఇది; }
  hb_sparseset_t& క్రియాకారకం << (స్థిరరాశి hb_codepoint_pair_t& range)
  { కూడు_range (range.మొదటి, range.second); తిరిగి *ఇది; }

  తార్కిక may_intersect (స్థిరరాశి hb_sparseset_t &other) స్థిరరాశి
  { తిరిగి s.may_intersect (other.s); }

  తార్కిక intersects (స్థిరరాశి hb_sparseset_t &other) స్థిరరాశి
  { తిరిగి s.intersects (other.s); }

  తార్కిక intersects (hb_codepoint_t మొదటి, hb_codepoint_t చివరి) స్థిరరాశి
  { తిరిగి s.intersects (మొదటి, చివరి); }

  శూన్య సమితి (స్థిరరాశి hb_sparseset_t &other) { s.సమితి (other.s); }

  తార్కిక is_equal (స్థిరరాశి hb_sparseset_t &other) స్థిరరాశి { తిరిగి s.is_equal (other.s); }
  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_సమితి_t &other) స్థిరరాశి { తిరిగి is_equal (other); }
  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_సమితి_t &other) స్థిరరాశి { తిరిగి !is_equal (other); }

  తార్కిక is_subset (స్థిరరాశి hb_sparseset_t &larger_సమితి) స్థిరరాశి { తిరిగి s.is_subset (larger_సమితి.s); }

  శూన్య union_ (స్థిరరాశి hb_sparseset_t &other) { s.union_ (other.s); }
  శూన్య intersect (స్థిరరాశి hb_sparseset_t &other,
		  hb_సదిశ_t<గుర్తులేని> *workspace = శూన్యసూచిక)
  { s.intersect (other.s, workspace); }
  శూన్య వ్యవకలనం (స్థిరరాశి hb_sparseset_t &other) { s.వ్యవకలనం (other.s); }
  శూన్య సౌష్ఠవ_difference (స్థిరరాశి hb_sparseset_t &other) { s.సౌష్ఠవ_difference (other.s); }

  తార్కిక next (hb_codepoint_t *codepoint) స్థిరరాశి { తిరిగి s.next (codepoint); }
  తార్కిక తదుపరి_బిట్లు (hb_codepoint_t *codepoint, uint64_t *బిట్లు) స్థిరరాశి
  { తిరిగి s.తదుపరి_బిట్లు (codepoint, బిట్లు); }
  తార్కిక మునుపటి (hb_codepoint_t *codepoint) స్థిరరాశి { తిరిగి s.మునుపటి (codepoint); }
  తార్కిక తదుపరి_range (hb_codepoint_t *మొదటి, hb_codepoint_t *చివరి) స్థిరరాశి
  { తిరిగి s.తదుపరి_range (మొదటి, చివరి); }
  తార్కిక మునుపటి_range (hb_codepoint_t *మొదటి, hb_codepoint_t *చివరి) స్థిరరాశి
  { తిరిగి s.మునుపటి_range (మొదటి, చివరి); }
  గుర్తులేని పూర్ణాంకం తదుపరి_many (hb_codepoint_t codepoint, hb_codepoint_t *బహిర్గతం, గుర్తులేని పూర్ణాంకం కొలత) స్థిరరాశి
  { తిరిగి s.తదుపరి_many (codepoint, బహిర్గతం, కొలత); }

  గుర్తులేని పూర్ణాంకం get_population () స్థిరరాశి { తిరిగి s.get_population (); }
  తార్కిక get_singleton (hb_codepoint_t *codepoint) స్థిరరాశి { తిరిగి s.get_singleton (codepoint); }
  hb_codepoint_t get_min () స్థిరరాశి { తిరిగి s.get_min (); }
  hb_codepoint_t get_max () స్థిరరాశి { తిరిగి s.get_max (); }

  స్థిర constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  ఉపయోగించి iter_t = రకం_పేరు impl_t::iter_t;
  iter_t iter () స్థిరరాశి { తిరిగి iter_t (ఇది->s); }
  క్రియాకారకం iter_t () స్థిరరాశి { తిరిగి iter (); }
};

నిర్మాణం hb_సమితి_t : hb_sparseset_t<hb_బిట్టు_సమితి_invertible_t>
{
  ఉపయోగించి sparseset = hb_sparseset_t<hb_బిట్టు_సమితి_invertible_t>;

  ~hb_సమితి_t () = అప్రమేయ;
  hb_సమితి_t () : sparseset () {};
  hb_సమితి_t (స్థిరరాశి hb_సమితి_t &o) : sparseset ((sparseset &) o) {};
  hb_సమితి_t (hb_సమితి_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_సమితి_t& క్రియాకారకం = (స్థిరరాశి hb_సమితి_t&) = అప్రమేయ;
  hb_సమితి_t& క్రియాకారకం = (hb_సమితి_t&&) = అప్రమేయ;
  hb_సమితి_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  మూస <రకం_పేరు Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_సమితి_t (స్థిరరాశి Iterable &o) : sparseset (o) {}

  hb_సమితి_t& క్రియాకారకం << (hb_codepoint_t v)
  { sparseset::క్రియాకారకం<< (v); తిరిగి *ఇది; }
  hb_సమితి_t& క్రియాకారకం << (స్థిరరాశి hb_codepoint_pair_t& range)
  { sparseset::క్రియాకారకం<< (range); తిరిగి *ఇది; }
};

static_assert (hb_సమితి_t::INVALID == HB_సమితి_విలువ_INVALID, "");


#అయితే_ముగింపు /* HB_SET_HH */
