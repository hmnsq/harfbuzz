/*
 * Copyright © 2026 Behdad Esfahbod
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
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_CFF2_ਮੰਜ਼ਿਲ_CFF1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_CFF2_ਮੰਜ਼ਿਲ_CFF1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff1-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-cff-common.hh"

ਨਾਮਾਕਾਸ਼ OT {
  // Forward declarations - these are defined in hb-subset-cff2.cc
  ਬਣਤਰ cff2_subset_plan;
}

ਨਾਮਾਕਾਸ਼ CFF {

// Forward declaration
ਬਣਤਰ cff2_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t;

// Default font name for converted CFF1 fonts
ਸਥਿਰ constexpr ਅਚਲ ਅੱਖਰ CFF1_DEFAULT_FONT_NAME[] = "CFF1Font";

/*
 * CFF2 to CFF1 Converter
 *
 * Converts an instantiated (pinned) CFF2 variable font to CFF1 format.
 * This is used when instantiating a variable font to a static instance.
 *
 * IMPLEMENTATION STATUS:
 * ✓ CFF1 structure (Header, Name INDEX, String INDEX, Top DICT INDEX)
 * ✓ ROS operator (makes font CID-keyed: "Adobe-Identity-0")
 * ✓ FDArray and FDSelect in Top DICT (required for CID fonts)
 * ✓ FDSelect3 format (compact range-based, 8 bytes for single-FD fonts)
 * ✓ CID Charset with identity mapping (format 2)
 * ✓ FontBBox from head table (xMin, yMin, xMax, yMax)
 * ✓ Width optimization (defaultWidthX/nominalWidthX with O(n) algorithm)
 * ✓ Width encoding in CharStrings (prepended if != defaultWidthX)
 * ✓ CharString specialization (h/v operators, combined when possible)
 * ✓ Stack depth control (generalize→specialize with maxstack=48)
 * ✓ CharStrings with endchar operators (CFF1 requires, CFF2 doesn't)
 * ✓ Private DICT instantiation (blend operators evaluated)
 * ✓ Desubroutinized path (CharStrings are flattened, no subroutines)
 * ✓ OTS validation passes
 * ✓ HarfBuzz rendering works
 *
 * FUTURE ENHANCEMENTS:
 * - Curve operator specialization (hhcurveto, vvcurveto, etc.)
 * - Peephole optimization (minor additional size savings)
 *
 * Key conversions:
 * - Version: 2 -> 1
 * - Add Name INDEX (required in CFF1)
 * - Wrap Top DICT in an INDEX (inline in CFF2, indexed in CFF1)
 * - Add String INDEX ("Adobe", "Identity" for ROS operator)
 * - Add ROS operator to Top DICT (makes it CID-keyed)
 * - Add FDSelect to Top DICT (required in CFF1 even with single FD)
 * - Add endchar to CharStrings (required in CFF1, optional in CFF2)
 */

ਬਣਤਰ cff1_subset_plan_from_cff2_t
{
  // Inherits most data from cff2_subset_plan
  ਅਚਲ OT::cff2_subset_plan *cff2_plan;

  // CFF1-specific additions
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> fontName;  // Single font name for Name INDEX

  ਤਾਰਕਿਕ create (ਅਚਲ OT::cff2_subset_plan &cff2_plan_)
  {
    cff2_plan = &cff2_plan_;

    // Create a simple font name (CFF1 requires a Name INDEX)
    fontName.resize (ਲੜੀ_ਲੰਬਾਈ (CFF1_DEFAULT_FONT_NAME));
    ਜੇ (fontName.ਵਿੱਚ_error ()) ਵਾਪਸ ਝੂਠ;
    ਸਮ੍ਰਿਤੀ_ਨਕਲ (fontName.arrayZ, CFF1_DEFAULT_FONT_NAME, ਲੜੀ_ਲੰਬਾਈ (CFF1_DEFAULT_FONT_NAME));

    ਵਾਪਸ ਸੱਚ;
  }
};

/* CFF1 Top DICT operator serializer that adds ROS and removes CFF2-specific ops */
ਬਣਤਰ cff1_from_cff2_ਸਿਖਰ_dict_op_serializer_t : cff_ਸਿਖਰ_dict_op_serializer_t<>
{
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  ਅਚਲ op_str_t &opstr,
                  ਅਚਲ cff_sub_ਸਾਰਣੀ_info_t &info) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਚੋਣ (opstr.op)
    {
      ਹਾਲਤ Op_ਕੋਡ_vstore:
        // CFF2-only operator, skip it
        return_trace (ਸੱਚ);

      ਹਾਲਤ Op_ਕੋਡ_Char_ਲੜੀਆਂ:
        return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_ਲੜੀਆਂ_link, whence_t::Absolute));

      ਹਾਲਤ Op_ਕੋਡ_FDArray:
      ਹਾਲਤ Op_ਕੋਡ_FDSelect:
        // These are explicitly serialized in the main function to ensure they're present
        // even if CFF2 doesn't have them. Skip them here to avoid duplication.
        return_trace (ਸੱਚ);

      ਮੂਲ:
        return_trace (copy_opstr (c, opstr));
    }
  }

  // Serialize ROS operator to make this a CID-keyed font
  ਤਾਰਕਿਕ serialize_ros (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    // ROS = Registry-Ordering-Supplement
    // We use "Adobe", "Identity", 0 for maximum compatibility

    // Allocate space and encode directly
    // Registry: SID for "Adobe" (custom string at index 0 = SID 391)
    // Ordering: SID for "Identity" (custom string at index 1 = SID 392)
    // Supplement: 0
    // Note: CFF standard strings end at SID 390, custom strings start at 391

    str_buff_t buff;
    str_encoder_t encoder (buff);

    encoder.encode_int (391);  // Registry SID ("Adobe" in our String INDEX)
    encoder.encode_int (392);  // Ordering SID ("Identity" in our String INDEX)
    encoder.encode_int (0);    // Supplement
    encoder.encode_op (Op_ਕੋਡ_ROS);

    ਜੇ (encoder.ਵਿੱਚ_error ())
      return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ ਬਾਈਟਾਂ = buff.as_ਬਾਈਟਾਂ ();
    return_trace (c->embed (ਬਾਈਟਾਂ.arrayZ, ਬਾਈਟਾਂ.ਲੰਮਾਈ));
  }
};

/* Main serialization function */
HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
serialize_cff2_ਮੰਜ਼ਿਲ_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ਅਚਲ cff2_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t &cff2_ਸਿਖਰ_Dict,
                        ਅਚਲ OT::cff2::accelerator_subset_t &acc);

} /* namespace CFF */

#ਜੇ_ਅੰਤ /* HB_NO_SUBSET_CFF */

#ਜੇ_ਅੰਤ /* HB_SUBSET_CFF2_TO_CFF1_HH */
