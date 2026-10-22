/*
 * CFF CharString Specializer
 *
 * Optimizes CharString bytecode by using specialized operators
 * (hlineto, vlineto, hhcurveto, etc.) to save bytes and respects
 * CFF1 stack limit (48 values).
 *
 * Based on fontTools.cffLib.specializer
 */

#ifndef HB_CFF_SPECIALIZER_HH
#define HB_CFF_SPECIALIZER_HH

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-cff-interp-cs-common.hh"

নামাকাশ CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
স্থির অন্তঃরেখ যৌক্তিক
is_zero (ধ্রুবক সংখ্যা_t &n)
{
  ফেরত n.to_real () == 0.;
}

/* Generalize CharString commands to canonical form
 *
 * Converts all operators to their general forms and breaks down
 * multi-segment operators into single segments. This ensures we
 * start from a clean baseline before specialization.
 *
 * Based on fontTools.cffLib.specializer.generalizeCommands
 */
স্থির শূন্য
generalize_commands (hb_vector_t<cs_command_t> &commands)
{
  hb_vector_t<cs_command_t> ফলাফল;
  ফলাফল.alloc (commands.দৈর্ঘ্য * 2);  /* Estimate: might expand */

  জন্য (unsigned i = 0; i < commands.দৈর্ঘ্য; i++)
  {
    auto &cmd = commands[i];

    নির্বাচন (cmd.op)
    {
      ক্ষেত্র OpCode_hmoveto:
      ক্ষেত্র OpCode_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (OpCode_rmoveto);
        gen.args.alloc (2);

        যদি (cmd.op == OpCode_hmoveto && cmd.args.দৈর্ঘ্য >= 1)
        {
          gen.args.push (cmd.args[0]);  /* dx */
          সংখ্যা_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dy = 0 */
        }
        অন্যথা যদি (cmd.op == OpCode_vmoveto && cmd.args.দৈর্ঘ্য >= 1)
        {
          সংখ্যা_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dx = 0 */
          gen.args.push (cmd.args[0]);  /* dy */
        }
        ফলাফল.push (gen);
        বিরতি;
      }

      ক্ষেত্র OpCode_hlineto:
      ক্ষেত্র OpCode_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        যৌক্তিক is_h = (cmd.op == OpCode_hlineto);
        সংখ্যা_t zero; zero.set_int (0);

        জন্য (unsigned j = 0; j < cmd.args.দৈর্ঘ্য; j++)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);

          যৌক্তিক is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          যদি (is_horizontal)
          {
            seg.args.push (cmd.args[j]);  /* dx */
            seg.args.push (zero);          /* dy = 0 */
          }
          অন্যথা
          {
            seg.args.push (zero);          /* dx = 0 */
            seg.args.push (cmd.args[j]);  /* dy */
          }
          ফলাফল.push (seg);
        }
        বিরতি;
      }

      ক্ষেত্র OpCode_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        জন্য (unsigned j = 0; j + 1 < cmd.args.দৈর্ঘ্য; j += 2)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);
          seg.args.push (cmd.args[j]);
          seg.args.push (cmd.args[j + 1]);
          ফলাফল.push (seg);
        }
        বিরতি;
      }

      ক্ষেত্র OpCode_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        জন্য (unsigned j = 0; j + 5 < cmd.args.দৈর্ঘ্য; j += 6)
        {
          cs_command_t seg (OpCode_rrcurveto);
          seg.args.alloc (6);
          জন্য (unsigned k = 0; k < 6; k++)
            seg.args.push (cmd.args[j + k]);
          ফলাফল.push (seg);
        }
        বিরতি;
      }

      পূর্বনির্ধারিত:
        /* Keep other operators as-is */
        ফলাফল.push (cmd);
        বিরতি;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  জন্য (unsigned i = 0; i < ফলাফল.দৈর্ঘ্য; i++)
    commands.push (ফলাফল[i]);
}

/* Specialize CharString commands to optimize bytecode size
 *
 * Follows fontTools approach:
 * 0. Generalize: Break down to canonical single-segment form
 * 1. Specialize: Convert rmoveto/rlineto to h/v variants when dx or dy is zero
 * 2. Combine: Merge adjacent compatible operators
 * 3. Enforce: Respect maxstack limit (default 48 for CFF1)
 *
 * This ensures we never exceed stack depth while optimizing bytecode.
 */
স্থির শূন্য
specialize_commands (hb_vector_t<cs_command_t> &commands,
                     unsigned maxstack = 48)
{
  যদি (commands.দৈর্ঘ্য == 0) ফেরত;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  জন্য (unsigned i = 0; i < commands.দৈর্ঘ্য; i++)
  {
    auto &cmd = commands[i];

    যদি ((cmd.op == OpCode_rmoveto || cmd.op == OpCode_rlineto) &&
        cmd.args.দৈর্ঘ্য == 2)
    {
      যৌক্তিক dx_zero = is_zero (cmd.args[0]);
      যৌক্তিক dy_zero = is_zero (cmd.args[1]);

      যদি (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == OpCode_rmoveto) ? OpCode_vmoveto : OpCode_vlineto;
        /* Shift dy to position 0 */
        cmd.args[0] = cmd.args[1];
        cmd.args.resize (1);
      }
      অন্যথা যদি (!dx_zero && dy_zero)
      {
        /* Horizontal movement (dy=0): keep only dx */
        cmd.op = (cmd.op == OpCode_rmoveto) ? OpCode_hmoveto : OpCode_hlineto;
        cmd.args.resize (1);  /* Keep only dx */
      }
      /* else: both zero or both non-zero, keep as rmoveto/rlineto */
    }
  }

  /* Pass 2: Combine adjacent hlineto/vlineto operators
   * hlineto can take multiple args alternating with vlineto
   * This saves operator bytes */
  জন্য (পূর্ণসংখ্যা i = (পূর্ণসংখ্যা)commands.দৈর্ঘ্য - 1; i > 0; i--)
  {
    auto &cmd = commands[i];
    auto &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    যদি (((prev.op == OpCode_hlineto && cmd.op == OpCode_vlineto) ||
         (prev.op == OpCode_vlineto && cmd.op == OpCode_hlineto)) &&
        (prev.args.দৈর্ঘ্য & 1))
    {
      /* Check stack depth */
      unsigned combined_args = prev.args.দৈর্ঘ্য + cmd.args.দৈর্ঘ্য;
      যদি (combined_args < maxstack)
      {
        /* Merge into first command, keep its operator */
        জন্য (unsigned j = 0; j < cmd.args.দৈর্ঘ্য; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  জন্য (পূর্ণসংখ্যা i = (পূর্ণসংখ্যা)commands.দৈর্ঘ্য - 1; i > 0; i--)
  {
    auto &cmd = commands[i];
    auto &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    যদি (prev.op == cmd.op &&
        (cmd.op == OpCode_rlineto || cmd.op == OpCode_rrcurveto ||
         ((cmd.op == OpCode_hlineto || cmd.op == OpCode_vlineto) &&
          !(prev.args.দৈর্ঘ্য & 1))))
    {
      /* Check stack depth */
      unsigned combined_args = prev.args.দৈর্ঘ্য + cmd.args.দৈর্ঘ্য;
      যদি (combined_args < maxstack)
      {
        /* Merge args */
        জন্য (unsigned j = 0; j < cmd.args.দৈর্ঘ্য; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
স্থির যৌক্তিক
encode_commands (ধ্রুবক hb_vector_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  জন্য (ধ্রুবক auto &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    জন্য (ধ্রুবক auto &arg : cmd.args)
      encoder.encode_অঙ্ক_cs (arg);

    /* Encode operator */
    যদি (cmd.op != OpCode_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    যদি (cmd.op == OpCode_hintmask || cmd.op == OpCode_cntrmask)
    {
      জন্য (ধ্রুবক auto &byte : cmd.mask_bytes)
        encoder.encode_byte (byte);
    }

    যদি (encoder.in_error ())
      ফেরত মিথ্যা;
  }

  ফেরত সত্য;
}

} /* namespace CFF */

#endif /* HB_CFF_SPECIALIZER_HH */
