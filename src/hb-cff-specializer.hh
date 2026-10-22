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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-cff-interp-cs-common.hh"

நாமாகாஶ CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
நிலையான உள்வரி தருக்க
is_zero (மாறிலி எண்_t &n)
{
  திரும்பு n.to_real () == 0.;
}

/* Generalize CharString commands to canonical form
 *
 * Converts all operators to their general forms and breaks down
 * multi-segment operators into single segments. This ensures we
 * start from a clean baseline before specialization.
 *
 * Based on fontTools.cffLib.specializer.generalizeCommands
 */
நிலையான வெறுமை
generalize_commands (hb_vector_t<cs_command_t> &commands)
{
  hb_vector_t<cs_command_t> result;
  result.alloc (commands.length * 2);  /* Estimate: might expand */

  க்கு (unsigned i = 0; i < commands.length; i++)
  {
    auto &cmd = commands[i];

    தேர்வு (cmd.op)
    {
      நிலை OpCode_hmoveto:
      நிலை OpCode_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (OpCode_rmoveto);
        gen.args.alloc (2);

        எனில் (cmd.op == OpCode_hmoveto && cmd.args.length >= 1)
        {
          gen.args.push (cmd.args[0]);  /* dx */
          எண்_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dy = 0 */
        }
        இல்லையெனில் எனில் (cmd.op == OpCode_vmoveto && cmd.args.length >= 1)
        {
          எண்_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dx = 0 */
          gen.args.push (cmd.args[0]);  /* dy */
        }
        result.push (gen);
        நிறுத்து;
      }

      நிலை OpCode_hlineto:
      நிலை OpCode_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        தருக்க is_h = (cmd.op == OpCode_hlineto);
        எண்_t zero; zero.set_int (0);

        க்கு (unsigned j = 0; j < cmd.args.length; j++)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);

          தருக்க is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          எனில் (is_horizontal)
          {
            seg.args.push (cmd.args[j]);  /* dx */
            seg.args.push (zero);          /* dy = 0 */
          }
          இல்லையெனில்
          {
            seg.args.push (zero);          /* dx = 0 */
            seg.args.push (cmd.args[j]);  /* dy */
          }
          result.push (seg);
        }
        நிறுத்து;
      }

      நிலை OpCode_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        க்கு (unsigned j = 0; j + 1 < cmd.args.length; j += 2)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);
          seg.args.push (cmd.args[j]);
          seg.args.push (cmd.args[j + 1]);
          result.push (seg);
        }
        நிறுத்து;
      }

      நிலை OpCode_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        க்கு (unsigned j = 0; j + 5 < cmd.args.length; j += 6)
        {
          cs_command_t seg (OpCode_rrcurveto);
          seg.args.alloc (6);
          க்கு (unsigned k = 0; k < 6; k++)
            seg.args.push (cmd.args[j + k]);
          result.push (seg);
        }
        நிறுத்து;
      }

      இயல்பு:
        /* Keep other operators as-is */
        result.push (cmd);
        நிறுத்து;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  க்கு (unsigned i = 0; i < result.length; i++)
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
நிலையான வெறுமை
specialize_commands (hb_vector_t<cs_command_t> &commands,
                     unsigned maxstack = 48)
{
  எனில் (commands.length == 0) திரும்பு;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  க்கு (unsigned i = 0; i < commands.length; i++)
  {
    auto &cmd = commands[i];

    எனில் ((cmd.op == OpCode_rmoveto || cmd.op == OpCode_rlineto) &&
        cmd.args.length == 2)
    {
      தருக்க dx_zero = is_zero (cmd.args[0]);
      தருக்க dy_zero = is_zero (cmd.args[1]);

      எனில் (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == OpCode_rmoveto) ? OpCode_vmoveto : OpCode_vlineto;
        /* Shift dy to position 0 */
        cmd.args[0] = cmd.args[1];
        cmd.args.resize (1);
      }
      இல்லையெனில் எனில் (!dx_zero && dy_zero)
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
  க்கு (முழு i = (முழு)commands.length - 1; i > 0; i--)
  {
    auto &cmd = commands[i];
    auto &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    எனில் (((prev.op == OpCode_hlineto && cmd.op == OpCode_vlineto) ||
         (prev.op == OpCode_vlineto && cmd.op == OpCode_hlineto)) &&
        (prev.args.length & 1))
    {
      /* Check stack depth */
      unsigned combined_args = prev.args.length + cmd.args.length;
      எனில் (combined_args < maxstack)
      {
        /* Merge into first command, keep its operator */
        க்கு (unsigned j = 0; j < cmd.args.length; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  க்கு (முழு i = (முழு)commands.length - 1; i > 0; i--)
  {
    auto &cmd = commands[i];
    auto &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    எனில் (prev.op == cmd.op &&
        (cmd.op == OpCode_rlineto || cmd.op == OpCode_rrcurveto ||
         ((cmd.op == OpCode_hlineto || cmd.op == OpCode_vlineto) &&
          !(prev.args.length & 1))))
    {
      /* Check stack depth */
      unsigned combined_args = prev.args.length + cmd.args.length;
      எனில் (combined_args < maxstack)
      {
        /* Merge args */
        க்கு (unsigned j = 0; j < cmd.args.length; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
நிலையான தருக்க
encode_commands (மாறிலி hb_vector_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  க்கு (மாறிலி auto &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    க்கு (மாறிலி auto &arg : cmd.args)
      encoder.encode_இலக்கம்_cs (arg);

    /* Encode operator */
    எனில் (cmd.op != OpCode_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    எனில் (cmd.op == OpCode_hintmask || cmd.op == OpCode_cntrmask)
    {
      க்கு (மாறிலி auto &byte : cmd.mask_bytes)
        encoder.encode_byte (byte);
    }

    எனில் (encoder.in_error ())
      திரும்பு பொய்;
  }

  திரும்பு உண்மை;
}

} /* namespace CFF */

#endif /* HB_CFF_SPECIALIZER_HH */
