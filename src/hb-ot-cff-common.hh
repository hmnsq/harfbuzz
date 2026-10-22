/*
 * Copyright © 2018 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */
#నిర్వచితం_కాకపోతే HB_OT_CFF_COMMON_HH
#నిర్వచించు HB_OT_CFF_COMMON_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-bimap.hh"
#అన్తర్భావ "hb-ot-layout-common.hh"
#అన్తర్భావ "hb-cff-interp-dict-common.hh"
#అన్తర్భావ "hb-subset-plan.hh"

నామాకాశ CFF {

ఉపయోగించి నామాకాశ OT;

#నిర్వచించు CFF_UNDEF_సంకేతం  0xFFFFFFFF

ఉపయోగించి objidx_t = hb_serialize_context_t::objidx_t;
ఉపయోగించి whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
మూస<రకం_పేరు Type, రకం_పేరు ...Ts>
స్థిర అంతఃవరుస స్థిరరాశి Type& Struct_At_స్థానభ్రంశం_Or_Null (స్థిరరాశి శూన్య *P, పూర్ణాంకం స్థానభ్రంశం, hb_sanitize_context_t &sc, Ts&&... ds)
{
  ఒకవేళ (!స్థానభ్రంశం) తిరిగి Null (Type);

  స్థిరరాశి అక్షరం *p = (స్థిరరాశి అక్షరం *) P + స్థానభ్రంశం;
  ఒకవేళ (!sc.check_బిందువు (p)) తిరిగి Null (Type);

  స్థిరరాశి Type &obj = *reinterpret_cast<స్థిరరాశి Type *> (p);
  ఒకవేళ (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) తిరిగి Null (Type);

  తిరిగి obj;
}


నిర్మాణం సంకేతం_pair_t
{
  గుర్తులేని సంకేతం_2;
  hb_codepoint_t glyph;
};


ఉపయోగించి str_buff_t = hb_సదిశ_t<గుర్తులేని అక్షరం>;
ఉపయోగించి str_buff_vec_t = hb_సదిశ_t<str_buff_t>;
ఉపయోగించి glyph_గమ్యం_sid_map_t = hb_సదిశ_t<సంకేతం_pair_t>;

/* Top Dict, Font Dict, Private Dict */
నిర్మాణం Dict : UnsizedByteStr
{
  మూస <రకం_పేరు DICTVAL, రకం_పేరు OP_SERIALIZER, రకం_పేరు ...Ts>
  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (ఇది);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < dictval.get_లెక్క (); i++)
      ఒకవేళ (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (అబద్ధం);

    return_trace (నిజం);
  }

  మూస <రకం_పేరు T, రకం_పేరు V>
  స్థిర తార్కిక serialize_int_op (hb_serialize_context_t *c, op_సంకేతం_t op, V value, op_సంకేతం_t intOp)
  {
    ఒకవేళ (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      తిరిగి అబద్ధం;

    TRACE_SERIALIZE (ఇది);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_కొలత<HBUINT8> (Op_సంకేతం_కొలత (op), అబద్ధం);
    ఒకవేళ (unlikely (!p)) return_trace (అబద్ధం);
    ఒకవేళ (Is_Op_సంకేతం_ESC (op))
    {
      *p = Op_సంకేతం_escape;
      op = Unmake_Op_సంకేతం_ESC (op);
      p++;
    }
    *p = op;
    return_trace (నిజం);
  }

  మూస <రకం_పేరు V>
  స్థిర తార్కిక serialize_int4_op (hb_serialize_context_t *c, op_సంకేతం_t op, V value)
  { తిరిగి serialize_int_op<HBINT32> (c, op, value, Op_సంకేతం_longintdict); }

  మూస <రకం_పేరు V>
  స్థిర తార్కిక serialize_int2_op (hb_serialize_context_t *c, op_సంకేతం_t op, V value)
  { తిరిగి serialize_int_op<HBINT16> (c, op, value, Op_సంకేతం_shortint); }

  మూస <రకం_పేరు T, పూర్ణాంకం int_op>
  స్థిర తార్కిక serialize_link_op (hb_serialize_context_t *c, op_సంకేతం_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->తల_2 + Op_సంకేతం_కొలత (int_op));
    ఒకవేళ (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) తిరిగి అబద్ధం;
    c->కూడు_link (ofs, link, whence);
    తిరిగి నిజం;
  }

  స్థిర తార్కిక serialize_link4_op (hb_serialize_context_t *c, op_సంకేతం_t op, objidx_t link, whence_t whence = whence_t::తల)
  { తిరిగి serialize_link_op<HBINT32, Op_సంకేతం_longintdict> (c, op, link, whence); }

  స్థిర తార్కిక serialize_link2_op (hb_serialize_context_t *c, op_సంకేతం_t op, objidx_t link, whence_t whence = whence_t::తల)
  { తిరిగి serialize_link_op<HBINT16, Op_సంకేతం_shortint> (c, op, link, whence); }
};

నిర్మాణం శిఖరం_Dict : Dict {};
నిర్మాణం FontDict : Dict {};
నిర్మాణం వ్యక్తిగత_Dict : Dict {};

నిర్మాణం పట్టిక_info_t
{
  శూన్య init () { స్థానభ్రంశం = కొలత = 0; link = 0; }

  గుర్తులేని పూర్ణాంకం    స్థానభ్రంశం;
  గుర్తులేని పూర్ణాంకం    కొలత;
  objidx_t	  link;
};

మూస <రకం_పేరు లెక్క_3>
నిర్మాణం FDArray : CFFIndex<లెక్క_3>
{
  మూస <రకం_పేరు DICTVAL, రకం_పేరు INFO, రకం_పేరు Iterator, రకం_పేరు OP_SERIALIZER>
  తార్కిక serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (ఇది);

    /* serialize INDEX data */
    hb_సదిశ_t<గుర్తులేని> sizes;
    ఒకవేళ (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    అక్షరం *దత్తాంశం_పునాది = c->తల_2;
    + it
    | hb_map ([&] (స్థిరరాశి hb_pair_t<స్థిరరాశి DICTVAL&, స్థిరరాశి INFO&> &_)
    {
      FontDict *dict = c->ప్రారంభం_embed<FontDict> ();
		dict->serialize (c, _.మొదటి, opszr, _.second);
		తిరిగి c->తల_2 - (స్థిరరాశి అక్షరం*)dict;
	      })
    | hb_sink (sizes)
    ;
    గుర్తులేని దత్తాంశం_కొలత = c->తల_2 - దత్తాంశం_పునాది;
    c->తీయి_pack (అబద్ధం);

    ఒకవేళ (unlikely (sizes.లోపల_error ())) return_trace (అబద్ధం);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<లెక్క_3>::serialize_శీర్షిక (c, hb_iter (sizes), దత్తాంశం_కొలత));
  }
};

/* FDSelect */
నిర్మాణం FDSelect0 {
  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం fdcount,
                 గుర్తులేని పూర్ణాంకం glyph_లెక్క = HB_CODEPOINT_INVALID) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!(c->check_struct (ఇది))))
      return_trace (అబద్ధం);
    hb_barrier ();
    ఒకవేళ (unlikely (!c->check_array (fds, glyph_లెక్క == HB_CODEPOINT_INVALID ?
                                     c->get_అంకె_glyphs () : glyph_లెక్క)))
      return_trace (అబద్ధం);

    return_trace (నిజం);
  }

  గుర్తులేని get_fd (hb_codepoint_t glyph) స్థిరరాశి
  { తిరిగి fds[glyph]; }

  hb_pair_t<గుర్తులేని, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) స్థిరరాశి
  { తిరిగి {fds[glyph], glyph + 1}; }

  size_t get_కొలత (గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  { తిరిగి hb_unsigned_mul_saturate (HBUINT8::static_కొలత, అంకె_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_కొలత_MIN (0);
};

మూస <రకం_పేరు GID_TYPE, రకం_పేరు FD_TYPE>
నిర్మాణం FDSelect3_4_Range
{
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య * /*nullptr*/, గుర్తులేని పూర్ణాంకం fdcount) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  మొదటి < c->get_అంకె_glyphs () && (fd < fdcount));
  }

  GID_TYPE    మొదటి;
  FD_TYPE     fd;
  బహిరంగ:
  DEFINE_కొలత_STATIC (GID_TYPE::static_కొలత + FD_TYPE::static_కొలత);
};

మూస <రకం_పేరు GID_TYPE, రకం_పేరు FD_TYPE>
నిర్మాణం FDSelect3_4
{
  size_t get_కొలత () స్థిరరాశి
  { తిరిగి hb_unsigned_కూడు_saturate ((size_t) GID_TYPE::static_కొలత * 2, ranges.get_కొలత ()); }

  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం fdcount,
                 గుర్తులేని పూర్ణాంకం min_glyphs = HB_CODEPOINT_INVALID) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!(c->check_struct (ఇది) &&
		    ranges.sanitize (c, శూన్యసూచిక, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].మొదటి == 0)))
      return_trace (అబద్ధం);

    కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < nRanges (); i++)
      ఒకవేళ (unlikely (ranges[i - 1].మొదటి >= ranges[i].మొదటి))
	return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].మొదటి &&
                   sentinel() <= c->get_అంకె_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_అంకె_glyphs () : min_glyphs))))
      return_trace (అబద్ధం);

    return_trace (నిజం);
  }

  స్థిర పూర్ణాంకం _cmp_range (స్థిరరాశి శూన్య *కీలకం_3, స్థిరరాశి శూన్య *అంశం_2)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) కీలకం_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) అంశం_2;

    ఒకవేళ (glyph < range[0].మొదటి) తిరిగి -1;
    ఒకవేళ (glyph < range[1].మొదటి) తిరిగి 0;
    తిరిగి +1;
  }

  గుర్తులేని get_fd (hb_codepoint_t glyph) స్థిరరాశి
  {
    స్వయంచాలక *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, పరిమాణం (ranges[0]), _cmp_range);
    తిరిగి range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<గుర్తులేని, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) స్థిరరాశి
  {
    స్వయంచాలక *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, పరిమాణం (ranges[0]), _cmp_range);
    గుర్తులేని fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t చివర = range ? range[1].మొదటి : ranges[nRanges () - 1].మొదటి;
    తిరిగి {fd, చివర};
  }

  GID_TYPE        &nRanges ()       { తిరిగి ranges.len; }
  GID_TYPE         nRanges () స్థిరరాశి { తిరిగి ranges.len; }
  GID_TYPE       &sentinel ()       { తిరిగి StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  స్థిరరాశి GID_TYPE &sentinel () స్థిరరాశి { తిరిగి StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  Array_యొక్క<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_కొలత_ARRAY (GID_TYPE::static_కొలత, ranges);
};

రకనిర్వచనం FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
రకనిర్వచనం FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

నిర్మాణం FDSelect
{
  తార్కిక serialize (hb_serialize_context_t *c, స్థిరరాశి FDSelect &src, గుర్తులేని పూర్ణాంకం అంకె_glyphs)
  {
    TRACE_SERIALIZE (ఇది);
    గుర్తులేని పూర్ణాంకం కొలత = src.get_కొలత (అంకె_glyphs);
    FDSelect *dest = c->allocate_కొలత<FDSelect> (కొలత, అబద్ధం);
    ఒకవేళ (unlikely (!dest)) return_trace (అబద్ధం);
    hb_memcpy (dest, &src, కొలత);
    return_trace (నిజం);
  }

  size_t get_కొలత (గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); తిరిగి hb_unsigned_కూడు_saturate (format.static_కొలత, u.format0.get_కొలత (అంకె_glyphs));
    స్థితి 3: hb_barrier (); తిరిగి hb_unsigned_కూడు_saturate (format.static_కొలత, u.format3.get_కొలత ());
    అప్రమేయ:తిరిగి 0;
    }
  }

  గుర్తులేని get_fd (hb_codepoint_t glyph) స్థిరరాశి
  {
    ఒకవేళ (ఇది == &Null (FDSelect)) తిరిగి 0;

    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_fd (glyph);
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_fd (glyph);
    అప్రమేయ:తిరిగి 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<గుర్తులేని, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) స్థిరరాశి
  {
    ఒకవేళ (ఇది == &Null (FDSelect)) తిరిగి {0, 1};

    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_fd_range (glyph);
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_fd_range (glyph);
    అప్రమేయ:తిరిగి {0, 1};
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం fdcount) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది)))
      return_trace (అబద్ధం);
    hb_barrier ();

    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    స్థితి 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  HBUINT8	format;
  సంఘం {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  బహిరంగ:
  DEFINE_కొలత_MIN (1);
};

మూస <రకం_పేరు లెక్క_3>
నిర్మాణం Subrs : CFFIndex<లెక్క_3>
{
  రకనిర్వచనం లెక్క_3 లెక్క_type;
  రకనిర్వచనం CFFIndex<లెక్క_3> SUPER;
};

} /* namespace CFF */

#అయితే_ముగింపు /* HB_OT_CFF_COMMON_HH */
