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
#अगर_अघोषित HB_OT_CFF_COMMON_HH
#घोषणा HB_OT_CFF_COMMON_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-bimap.hh"
#समावेश "hb-ot-layout-common.hh"
#समावेश "hb-cff-interp-dict-common.hh"
#समावेश "hb-subset-plan.hh"

namespace CFF {

using namespace OT;

#घोषणा CFF_UNDEF_CODE  0xFFFFFFFF

using objidx_t = hb_serialize_context_t::objidx_t;
using whence_t = hb_serialize_context_t::whence_t;

/* CFF offsets can technically be negative */
ढाँचा<typename Type, typename ...Ts>
अटल अंतरभूत स्थिर Type& StructAtOffsetOrNull (स्थिर व्योम *P, पूर्णांक offset, hb_sanitize_context_t &sc, Ts&&... ds)
{
  अगर (!offset) वापस Null (Type);

  स्थिर अक्षर *p = (स्थिर अक्षर *) P + offset;
  अगर (!sc.check_point (p)) वापस Null (Type);

  स्थिर Type &obj = *reinterpret_cast<स्थिर Type *> (p);
  अगर (!obj.sanitize (&sc, std::forward<Ts> (ds)...)) वापस Null (Type);

  वापस obj;
}


काष्ठा code_pair_t
{
  अचिन्हित code;
  hb_codepoint_t glyph;
};


using str_buff_t = hb_vector_t<अचिन्हित अक्षर>;
using str_buff_vec_t = hb_vector_t<str_buff_t>;
using glyph_to_sid_map_t = hb_vector_t<code_pair_t>;

/* Top Dict, Font Dict, Private Dict */
काष्ठा Dict : UnsizedByteStr
{
  ढाँचा <typename DICTVAL, typename OP_SERIALIZER, typename ...Ts>
  bool serialize (hb_serialize_context_t *c,
		  स्थिर DICTVAL &dictval,
		  OP_SERIALIZER& opszr,
		  Ts&&... ds)
  {
    TRACE_SERIALIZE (यह);
    क्रम (अचिन्हित पूर्णांक i = 0; i < dictval.get_गिनती (); i++)
      अगर (unlikely (!opszr.serialize (c, dictval[i], ds...)))
	return_trace (false);

    return_trace (true);
  }

  ढाँचा <typename T, typename V>
  अटल bool serialize_int_op (hb_serialize_context_t *c, op_code_t op, V value, op_code_t intOp)
  {
    अगर (unlikely ((!serialize_int<T, V> (c, intOp, value))))
      वापस false;

    TRACE_SERIALIZE (यह);
    /* serialize the opcode */
    HBUINT8 *p = c->allocate_आकार<HBUINT8> (Op_Code_आकार (op), false);
    अगर (unlikely (!p)) return_trace (false);
    अगर (Is_OpCode_ESC (op))
    {
      *p = OpCode_escape;
      op = Unmake_OpCode_ESC (op);
      p++;
    }
    *p = op;
    return_trace (true);
  }

  ढाँचा <typename V>
  अटल bool serialize_int4_op (hb_serialize_context_t *c, op_code_t op, V value)
  { वापस serialize_int_op<HBINT32> (c, op, value, OpCode_longintdict); }

  ढाँचा <typename V>
  अटल bool serialize_int2_op (hb_serialize_context_t *c, op_code_t op, V value)
  { वापस serialize_int_op<HBINT16> (c, op, value, OpCode_shortint); }

  ढाँचा <typename T, पूर्णांक int_op>
  अटल bool serialize_link_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence)
  {
    T &ofs = *(T *) (c->शीर्ष_2 + Op_Code_आकार (int_op));
    अगर (unlikely (!serialize_int_op<T> (c, op, 0, int_op))) वापस false;
    c->add_link (ofs, link, whence);
    वापस true;
  }

  अटल bool serialize_link4_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::शीर्ष)
  { वापस serialize_link_op<HBINT32, OpCode_longintdict> (c, op, link, whence); }

  अटल bool serialize_link2_op (hb_serialize_context_t *c, op_code_t op, objidx_t link, whence_t whence = whence_t::शीर्ष)
  { वापस serialize_link_op<HBINT16, OpCode_shortint> (c, op, link, whence); }
};

काष्ठा शीर्ष_Dict : Dict {};
काष्ठा FontDict : Dict {};
काष्ठा PrivateDict : Dict {};

काष्ठा table_info_t
{
  व्योम init () { offset = आकार = 0; link = 0; }

  अचिन्हित पूर्णांक    offset;
  अचिन्हित पूर्णांक    आकार;
  objidx_t	  link;
};

ढाँचा <typename गिनती_3>
काष्ठा FDArray : CFFIndex<गिनती_3>
{
  ढाँचा <typename DICTVAL, typename INFO, typename Iterator, typename OP_SERIALIZER>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it,
		  OP_SERIALIZER& opszr)
  {
    TRACE_SERIALIZE (यह);

    /* serialize INDEX data */
    hb_vector_t<अचिन्हित> sizes;
    अगर (it.is_random_access_iterator)
      sizes.alloc (hb_len (it));

    c->push ();
    अक्षर *तथ्य_base = c->शीर्ष_2;
    + it
    | hb_map ([&] (स्थिर hb_pair_t<स्थिर DICTVAL&, स्थिर INFO&> &_)
    {
      FontDict *dict = c->start_embed<FontDict> ();
		dict->serialize (c, _.first, opszr, _.second);
		वापस c->शीर्ष_2 - (स्थिर अक्षर*)dict;
	      })
    | hb_sink (sizes)
    ;
    अचिन्हित तथ्य_आकार = c->शीर्ष_2 - तथ्य_base;
    c->निकालो_pack (false);

    अगर (unlikely (sizes.in_error ())) return_trace (false);

    /* It just happens that the above is packed right after the header below.
     * Such a hack. */

    /* serialize INDEX header */
    return_trace (CFFIndex<गिनती_3>::serialize_header (c, hb_iter (sizes), तथ्य_आकार));
  }
};

/* FDSelect */
काष्ठा FDSelect0 {
  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक fdcount,
                 अचिन्हित पूर्णांक glyph_गिनती = HB_CODEPOINT_INVALID) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!(c->check_struct (यह))))
      return_trace (false);
    hb_barrier ();
    अगर (unlikely (!c->check_array (fds, glyph_गिनती == HB_CODEPOINT_INVALID ?
                                     c->get_अंक_glyphs () : glyph_गिनती)))
      return_trace (false);

    return_trace (true);
  }

  अचिन्हित get_fd (hb_codepoint_t glyph) स्थिर
  { वापस fds[glyph]; }

  hb_pair_t<अचिन्हित, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) स्थिर
  { वापस {fds[glyph], glyph + 1}; }

  माप_प्रकार get_आकार (अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  { वापस hb_unsigned_mul_saturate (HBUINT8::static_आकार, अंक_glyphs); }

  HBUINT8     fds[HB_VAR_ARRAY];

  DEFINE_आकार_MIN (0);
};

ढाँचा <typename GID_TYPE, typename FD_TYPE>
काष्ठा FDSelect3_4_Range
{
  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम * /*nullptr*/, अचिन्हित पूर्णांक fdcount) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  first < c->get_अंक_glyphs () && (fd < fdcount));
  }

  GID_TYPE    first;
  FD_TYPE     fd;
  खुला:
  DEFINE_आकार_STATIC (GID_TYPE::static_आकार + FD_TYPE::static_आकार);
};

ढाँचा <typename GID_TYPE, typename FD_TYPE>
काष्ठा FDSelect3_4
{
  माप_प्रकार get_आकार () स्थिर
  { वापस hb_unsigned_add_saturate ((माप_प्रकार) GID_TYPE::static_आकार * 2, ranges.get_आकार ()); }

  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक fdcount,
                 अचिन्हित पूर्णांक min_glyphs = HB_CODEPOINT_INVALID) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!(c->check_struct (यह) &&
		    ranges.sanitize (c, nullptr, fdcount) &&
		    hb_barrier () &&
		    (nRanges () != 0) &&
		    ranges[0].first == 0)))
      return_trace (false);

    क्रम (अचिन्हित पूर्णांक i = 1; i < nRanges (); i++)
      अगर (unlikely (ranges[i - 1].first >= ranges[i].first))
	return_trace (false);

    अगर (unlikely (!(sentinel().sanitize (c) &&
		   hb_barrier () &&
                   sentinel() > ranges[nRanges () - 1].first &&
                   sentinel() <= c->get_अंक_glyphs () &&
                   sentinel() >= (min_glyphs == HB_CODEPOINT_INVALID ?
                                  c->get_अंक_glyphs () : min_glyphs))))
      return_trace (false);

    return_trace (true);
  }

  अटल पूर्णांक _cmp_range (स्थिर व्योम *कुंजी_3, स्थिर व्योम *_item)
  {
    hb_codepoint_t glyph = * (hb_codepoint_t *) कुंजी_3;
    FDSelect3_4_Range<GID_TYPE, FD_TYPE> *range = (FDSelect3_4_Range<GID_TYPE, FD_TYPE> *) _item;

    अगर (glyph < range[0].first) वापस -1;
    अगर (glyph < range[1].first) वापस 0;
    वापस +1;
  }

  अचिन्हित get_fd (hb_codepoint_t glyph) स्थिर
  {
    स्वतः *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, माप (ranges[0]), _cmp_range);
    वापस range ? range->fd : ranges[nRanges () - 1].fd;
  }

  hb_pair_t<अचिन्हित, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) स्थिर
  {
    स्वतः *range = hb_bsearch (glyph, &ranges[0], nRanges () - 1, माप (ranges[0]), _cmp_range);
    अचिन्हित fd = range ? range->fd : ranges[nRanges () - 1].fd;
    hb_codepoint_t end = range ? range[1].first : ranges[nRanges () - 1].first;
    वापस {fd, end};
  }

  GID_TYPE        &nRanges ()       { वापस ranges.len; }
  GID_TYPE         nRanges () स्थिर { वापस ranges.len; }
  GID_TYPE       &sentinel ()       { वापस StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }
  स्थिर GID_TYPE &sentinel () स्थिर { वापस StructAfter<GID_TYPE> (ranges[nRanges () - 1]); }

  ArrayOf<FDSelect3_4_Range<GID_TYPE, FD_TYPE>, GID_TYPE> ranges;
  /* GID_TYPE sentinel */

  DEFINE_आकार_ARRAY (GID_TYPE::static_आकार, ranges);
};

प्रकार FDSelect3_4<HBUINT16, HBUINT8> FDSelect3;
प्रकार FDSelect3_4_Range<HBUINT16, HBUINT8> FDSelect3_Range;

काष्ठा FDSelect
{
  bool serialize (hb_serialize_context_t *c, स्थिर FDSelect &src, अचिन्हित पूर्णांक अंक_glyphs)
  {
    TRACE_SERIALIZE (यह);
    अचिन्हित पूर्णांक आकार = src.get_आकार (अंक_glyphs);
    FDSelect *dest = c->allocate_आकार<FDSelect> (आकार, false);
    अगर (unlikely (!dest)) return_trace (false);
    hb_memcpy (dest, &src, आकार);
    return_trace (true);
  }

  माप_प्रकार get_आकार (अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    चयन (format)
    {
    हाल 0: hb_barrier (); वापस hb_unsigned_add_saturate (format.static_आकार, u.format0.get_आकार (अंक_glyphs));
    हाल 3: hb_barrier (); वापस hb_unsigned_add_saturate (format.static_आकार, u.format3.get_आकार ());
    शेष:वापस 0;
    }
  }

  अचिन्हित get_fd (hb_codepoint_t glyph) स्थिर
  {
    अगर (यह == &Null (FDSelect)) वापस 0;

    चयन (format)
    {
    हाल 0: hb_barrier (); वापस u.format0.get_fd (glyph);
    हाल 3: hb_barrier (); वापस u.format3.get_fd (glyph);
    शेष:वापस 0;
    }
  }
  /* Returns pair of fd and one after last glyph in range. */
  hb_pair_t<अचिन्हित, hb_codepoint_t> get_fd_range (hb_codepoint_t glyph) स्थिर
  {
    अगर (यह == &Null (FDSelect)) वापस {0, 1};

    चयन (format)
    {
    हाल 0: hb_barrier (); वापस u.format0.get_fd_range (glyph);
    हाल 3: hb_barrier (); वापस u.format3.get_fd_range (glyph);
    शेष:वापस {0, 1};
    }
  }

  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक fdcount) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह)))
      return_trace (false);
    hb_barrier ();

    चयन (format)
    {
    हाल 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount));
    हाल 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount));
    शेष:return_trace (false);
    }
  }

  HBUINT8	format;
  जोड़ {
  FDSelect0	format0;
  FDSelect3	format3;
  } u;
  खुला:
  DEFINE_आकार_MIN (1);
};

ढाँचा <typename गिनती_3>
काष्ठा Subrs : CFFIndex<गिनती_3>
{
  प्रकार गिनती_3 गिनती_type;
  प्रकार CFFIndex<गिनती_3> SUPER;
};

} /* namespace CFF */

#पूर्ण_अगर /* HB_OT_CFF_COMMON_HH */
