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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-cff-interp-cs-common.hh"

ನಾಮಾಕಾಶ CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
is_zero (ಧ್ರುವ ಸಂಖ್ಯೆ_t &n)
{
  ಪ್ರತ್ಯಾಗಮ n.to_real () == 0.;
}

/* Generalize CharString commands to canonical form
 *
 * Converts all operators to their general forms and breaks down
 * multi-segment operators into single segments. This ensures we
 * start from a clean baseline before specialization.
 *
 * Based on fontTools.cffLib.specializer.generalizeCommands
 */
ಸ್ಥಿರ ಶೂನ್ಯ
generalize_commands (hb_vector_t<cs_command_t> &commands)
{
  hb_vector_t<cs_command_t> result;
  result.alloc (commands.length * 2);  /* Estimate: might expand */

  ಪ್ರತಿ (unsigned i = 0; i < commands.length; i++)
  {
    auto &cmd = commands[i];

    ವಿಕಲ್ಪ (cmd.op)
    {
      ಪಕ್ಷ OpCode_hmoveto:
      ಪಕ್ಷ OpCode_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (OpCode_rmoveto);
        gen.args.alloc (2);

        ಯದಿ (cmd.op == OpCode_hmoveto && cmd.args.length >= 1)
        {
          gen.args.push (cmd.args[0]);  /* dx */
          ಸಂಖ್ಯೆ_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dy = 0 */
        }
        ಅನ್ಯಥಾ ಯದಿ (cmd.op == OpCode_vmoveto && cmd.args.length >= 1)
        {
          ಸಂಖ್ಯೆ_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dx = 0 */
          gen.args.push (cmd.args[0]);  /* dy */
        }
        result.push (gen);
        ಭಙ್ಗ;
      }

      ಪಕ್ಷ OpCode_hlineto:
      ಪಕ್ಷ OpCode_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        ತಾರ್ಕಿಕ is_h = (cmd.op == OpCode_hlineto);
        ಸಂಖ್ಯೆ_t zero; zero.set_int (0);

        ಪ್ರತಿ (unsigned j = 0; j < cmd.args.length; j++)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);

          ತಾರ್ಕಿಕ is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          ಯದಿ (is_horizontal)
          {
            seg.args.push (cmd.args[j]);  /* dx */
            seg.args.push (zero);          /* dy = 0 */
          }
          ಅನ್ಯಥಾ
          {
            seg.args.push (zero);          /* dx = 0 */
            seg.args.push (cmd.args[j]);  /* dy */
          }
          result.push (seg);
        }
        ಭಙ್ಗ;
      }

      ಪಕ್ಷ OpCode_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        ಪ್ರತಿ (unsigned j = 0; j + 1 < cmd.args.length; j += 2)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);
          seg.args.push (cmd.args[j]);
          seg.args.push (cmd.args[j + 1]);
          result.push (seg);
        }
        ಭಙ್ಗ;
      }

      ಪಕ್ಷ OpCode_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        ಪ್ರತಿ (unsigned j = 0; j + 5 < cmd.args.length; j += 6)
        {
          cs_command_t seg (OpCode_rrcurveto);
          seg.args.alloc (6);
          ಪ್ರತಿ (unsigned k = 0; k < 6; k++)
            seg.args.push (cmd.args[j + k]);
          result.push (seg);
        }
        ಭಙ್ಗ;
      }

      ಸ್ವಭಾವ:
        /* Keep other operators as-is */
        result.push (cmd);
        ಭಙ್ಗ;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  ಪ್ರತಿ (unsigned i = 0; i < result.length; i++)
    commands.push (result[i]);
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
ಸ್ಥಿರ ಶೂನ್ಯ
specialize_commands (hb_vector_t<cs_command_t> &commands,
                     unsigned maxstack = 48)
{
  ಯದಿ (commands.length == 0) ಪ್ರತ್ಯಾಗಮ;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  ಪ್ರತಿ (unsigned i = 0; i < commands.length; i++)
  {
    auto &cmd = commands[i];

    ಯದಿ ((cmd.op == OpCode_rmoveto || cmd.op == OpCode_rlineto) &&
        cmd.args.length == 2)
    {
      ತಾರ್ಕಿಕ dx_zero = is_zero (cmd.args[0]);
      ತಾರ್ಕಿಕ dy_zero = is_zero (cmd.args[1]);

      ಯದಿ (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == OpCode_rmoveto) ? OpCode_vmoveto : OpCode_vlineto;
        /* Shift dy to position 0 */
        cmd.args[0] = cmd.args[1];
        cmd.args.resize (1);
      }
      ಅನ್ಯಥಾ ಯದಿ (!dx_zero && dy_zero)
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
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = (ಪೂರ್ಣಾಙ್ಕ)commands.length - 1; i > 0; i--)
  {
    auto &cmd = commands[i];
    auto &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    ಯದಿ (((prev.op == OpCode_hlineto && cmd.op == OpCode_vlineto) ||
         (prev.op == OpCode_vlineto && cmd.op == OpCode_hlineto)) &&
        (prev.args.length & 1))
    {
      /* Check stack depth */
      unsigned combined_args = prev.args.length + cmd.args.length;
      ಯದಿ (combined_args < maxstack)
      {
        /* Merge into first command, keep its operator */
        ಪ್ರತಿ (unsigned j = 0; j < cmd.args.length; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = (ಪೂರ್ಣಾಙ್ಕ)commands.length - 1; i > 0; i--)
  {
    auto &cmd = commands[i];
    auto &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    ಯದಿ (prev.op == cmd.op &&
        (cmd.op == OpCode_rlineto || cmd.op == OpCode_rrcurveto ||
         ((cmd.op == OpCode_hlineto || cmd.op == OpCode_vlineto) &&
          !(prev.args.length & 1))))
    {
      /* Check stack depth */
      unsigned combined_args = prev.args.length + cmd.args.length;
      ಯದಿ (combined_args < maxstack)
      {
        /* Merge args */
        ಪ್ರತಿ (unsigned j = 0; j < cmd.args.length; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
ಸ್ಥಿರ ತಾರ್ಕಿಕ
encode_commands (ಧ್ರುವ hb_vector_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  ಪ್ರತಿ (ಧ್ರುವ auto &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    ಪ್ರತಿ (ಧ್ರುವ auto &arg : cmd.args)
      encoder.encode_ಅಂಕೆ_cs (arg);

    /* Encode operator */
    ಯದಿ (cmd.op != OpCode_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    ಯದಿ (cmd.op == OpCode_hintmask || cmd.op == OpCode_cntrmask)
    {
      ಪ್ರತಿ (ಧ್ರುವ auto &byte : cmd.mask_bytes)
        encoder.encode_byte (byte);
    }

    ಯದಿ (encoder.in_error ())
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

} /* namespace CFF */

#endif /* HB_CFF_SPECIALIZER_HH */
