/*
 * Copyright © 2020  Google, Inc.
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

#నిర్వచితం_కాకపోతే HB_ప్రాధాన్యత_వరుస_HH
#నిర్వచించు HB_ప్రాధాన్యత_వరుస_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-vector.hh"

/*
 * hb_priority_queue_t
 *
 * Priority queue implemented as a binary heap. Supports extract minimum
 * and insert operations.
 *
 * The priority queue is implemented as a binary heap, which is a complete
 * binary tree. The root of the tree is the minimum element. The heap
 * property is that the priority of a node is less than or equal to the
 * priority of its children. The heap is stored in an array, with the
 * children of node i stored at indices 2i + 1 and 2i + 2.
 */
మూస <రకం_పేరు K>
నిర్మాణం hb_ప్రాధాన్యత_వరుస_t
{
 బహిరంగ:
  రకనిర్వచనం hb_pair_t<K, గుర్తులేని> అంశం_t;

 అంతరంగ:
  hb_సదిశ_t<అంశం_t> కుప్ప;

 బహిరంగ:

  hb_ప్రాధాన్యత_వరుస_t () = అప్రమేయ;
  hb_ప్రాధాన్యత_వరుస_t (hb_సదిశ_t<అంశం_t>&& other) : కుప్ప (std::move (other))
  {
    // Heapify the vector.
    కొరకు (పూర్ణాంకం i = (కుప్ప.నిడివి / 2) - 1; i >= 0; i--)
      bubble_కిందికి (i);
  }

  శూన్య reset () { కుప్ప.clear (); }

  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి కుప్ప.లోపల_error (); }

  తార్కిక alloc (గుర్తులేని కొలత)
  { తిరిగి కుప్ప.alloc (కొలత); }

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  శూన్య insert (K ప్రాధాన్యత, గుర్తులేని value)
  {
    కుప్ప.push (అంశం_t (ప్రాధాన్యత, value));
    ఒకవేళ (unlikely (కుప్ప.లోపల_error ())) తిరిగి;
    bubble_పైకి (కుప్ప.నిడివి - 1);
  }

#నిర్వచితం_కాకపోతే HB_OPTIMIZE_కొలత
  HB_ALWAYS_INLINE
#అయితే_ముగింపు
  అంశం_t తీయి_minimum ()
  {
    ప్రతిజ్ఞా (!is_empty ());

    అంశం_t ఫలితం = కుప్ప.arrayZ[0];

    కుప్ప.arrayZ[0] = కుప్ప.arrayZ[కుప్ప.నిడివి - 1];
    కుప్ప.resize (కుప్ప.నిడివి - 1);

    ఒకవేళ (!is_empty ())
      bubble_కిందికి (0);

    తిరిగి ఫలితం;
  }

  స్థిరరాశి అంశం_t& minimum ()
  {
    తిరిగి కుప్ప[0];
  }

  తార్కిక is_empty () స్థిరరాశి { తిరిగి కుప్ప.నిడివి == 0; }
  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి !is_empty (); }
  గుర్తులేని పూర్ణాంకం get_population () స్థిరరాశి { తిరిగి కుప్ప.నిడివి; }

  /* Sink interface. */
  hb_ప్రాధాన్యత_వరుస_t& క్రియాకారకం << (అంశం_t అంశం)
  { insert (అంశం.మొదటి, అంశం.second); తిరిగి *ఇది; }

 అంతరంగ:

  స్థిర constexpr గుర్తులేని మాతృ (గుర్తులేని సూచీ_2)
  {
    తిరిగి (సూచీ_2 - 1) / 2;
  }

  స్థిర constexpr గుర్తులేని ఎడమ_బిడ్డ (గుర్తులేని సూచీ_2)
  {
    తిరిగి 2 * సూచీ_2 + 1;
  }

  స్థిర constexpr గుర్తులేని కుడి_బిడ్డ (గుర్తులేని సూచీ_2)
  {
    తిరిగి 2 * సూచీ_2 + 2;
  }

  HB_ALWAYS_INLINE
  శూన్య bubble_కిందికి (గుర్తులేని సూచీ_2)
  {
    repeat:
    ప్రతిజ్ఞా (సూచీ_2 < కుప్ప.నిడివి);

    గుర్తులేని ఎడమ = ఎడమ_బిడ్డ (సూచీ_2);
    గుర్తులేని కుడి = కుడి_బిడ్డ (సూచీ_2);

    తార్కిక has_ఎడమ = ఎడమ < కుప్ప.నిడివి;
    ఒకవేళ (!has_ఎడమ)
      // If there's no left, then there's also no right.
      తిరిగి;

    తార్కిక has_కుడి = కుడి < కుప్ప.నిడివి;
    ఒకవేళ (కుప్ప.arrayZ[సూచీ_2].మొదటి <= కుప్ప.arrayZ[ఎడమ].మొదటి
        && (!has_కుడి || కుప్ప.arrayZ[సూచీ_2].మొదటి <= కుప్ప.arrayZ[కుడి].మొదటి))
      తిరిగి;

    గుర్తులేని బిడ్డ;
    ఒకవేళ (!has_కుడి || కుప్ప.arrayZ[ఎడమ].మొదటి < కుప్ప.arrayZ[కుడి].మొదటి)
      బిడ్డ = ఎడమ;
    లేకపోతే
      బిడ్డ = కుడి;

    swap (సూచీ_2, బిడ్డ);
    సూచీ_2 = బిడ్డ;
    వెళ్ళు repeat;
  }

  HB_ALWAYS_INLINE
  శూన్య bubble_పైకి (గుర్తులేని సూచీ_2)
  {
    repeat:
    ప్రతిజ్ఞా (సూచీ_2 < కుప్ప.నిడివి);

    ఒకవేళ (సూచీ_2 == 0) తిరిగి;

    గుర్తులేని మాతృ_సూచీ = మాతృ (సూచీ_2);
    ఒకవేళ (కుప్ప.arrayZ[మాతృ_సూచీ].మొదటి <= కుప్ప.arrayZ[సూచీ_2].మొదటి)
      తిరిగి;

    swap (సూచీ_2, మాతృ_సూచీ);
    సూచీ_2 = మాతృ_సూచీ;
    వెళ్ళు repeat;
  }

  శూన్య swap (గుర్తులేని a, గుర్తులేని b) noexcept
  {
    ప్రతిజ్ఞా (a < కుప్ప.నిడివి);
    ప్రతిజ్ఞా (b < కుప్ప.నిడివి);
    hb_మార్పిడి (కుప్ప.arrayZ[a], కుప్ప.arrayZ[b]);
  }
};

#అయితే_ముగింపు /* HB_PRIORITY_QUEUE_HH */
