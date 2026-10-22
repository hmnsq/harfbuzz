/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod, Garret Rieger
 */

#నిర్వచితం_కాకపోతే OT_LAYOUT_COMMON_RANGERECORD_HH
#నిర్వచించు OT_LAYOUT_COMMON_RANGERECORD_HH

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ Common {

మూస <రకం_పేరు Types, రకం_పేరు విలువ = HBUINT16>
నిర్మాణం Range_రికార్డు
{
  రకం_పేరు Types::HBGlyphID     మొదటి;          /* First GlyphID in the range */
  రకం_పేరు Types::HBGlyphID     చివరి;           /* Last GlyphID in the range */
  విలువ                         value;          /*విలువ*/

  DEFINE_కొలత_STATIC (2 * Types::కొలత + విలువ::static_కొలత);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  పూర్ణాంకం cmp (hb_codepoint_t g) స్థిరరాశి
  { తిరిగి g < మొదటి ? -1 : g <= చివరి ? 0 : +1; }

  HB_అంతర్గత స్థిర పూర్ణాంకం cmp_range (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb) {
    స్థిరరాశి Range_రికార్డు *a = (స్థిరరాశి Range_రికార్డు *) pa;
    స్థిరరాశి Range_రికార్డు *b = (స్థిరరాశి Range_రికార్డు *) pb;
    ఒకవేళ (a->మొదటి < b->మొదటి) తిరిగి -1;
    ఒకవేళ (a->మొదటి > b->మొదటి) తిరిగి +1;
    ఒకవేళ (a->చివరి < b->చివరి) తిరిగి -1;
    ఒకవేళ (a->చివరి > b->చివరి) తిరిగి +1;
    ఒకవేళ (a->value < b->value) తిరిగి -1;
    ఒకవేళ (a->value > b->value) తిరిగి +1;
    తిరిగి 0;
  }

  గుర్తులేని get_population () స్థిరరాశి
  {
    ఒకవేళ (unlikely (చివరి < మొదటి)) తిరిగి 0;
    తిరిగి (చివరి - మొదటి + 1);
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t &glyphs) స్థిరరాశి
  { తిరిగి glyphs.intersects (మొదటి, చివరి); }

  మూస <రకం_పేరు సమితి_t>
  తార్కిక collect_coverage (సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి glyphs->కూడు_range (మొదటి, చివరి); }
};

}
}
}

// TODO(garretrieger): This was previously implemented using
//    DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (OT, RangeRecord, 9);
//    but that only works when there is only a single namespace level.
//    The macro should probably be fixed so it can work in this situation.
బాహ్య HB_అంతర్గత స్థిరరాశి గుర్తులేని అక్షరం hb_Null_OT_Range_రికార్డు[9];
మూస <రకం_పేరు Spec, రకం_పేరు విలువ>
నిర్మాణం Null<OT::Layout::Common::Range_రికార్డు<Spec, విలువ>> {
  స్థిర OT::Layout::Common::Range_రికార్డు<Spec, విలువ> స్థిరరాశి & get_null () {
    తిరిగి *reinterpret_cast<స్థిరరాశి OT::Layout::Common::Range_రికార్డు<Spec, విలువ> *> (hb_Null_OT_Range_రికార్డు);
  }
};


#అయితే_ముగింపు  // #ifndef OT_LAYOUT_COMMON_RANGERECORD_HH
