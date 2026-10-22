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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-cff-interp-cs-common.hh"

નામાકાશ CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
સ્થિર અંતઃરેખ તાર્કિક
is_zero (અચલ સંખ્યા_t &n)
{
  પાછા n.to_real () == 0.;
}

/* Generalize CharString commands to canonical form
 *
 * Converts all operators to their general forms and breaks down
 * multi-segment operators into single segments. This ensures we
 * start from a clean baseline before specialization.
 *
 * Based on fontTools.cffLib.specializer.generalizeCommands
 */
સ્થિર શૂન્ય
generalize_commands (hb_vector_t<cs_command_t> &commands)
{
  hb_vector_t<cs_command_t> result;
  result.alloc (commands.length * 2);  /* Estimate: might expand */

  માટે (unsigned i = 0; i < commands.length; i++)
  {
    auto &cmd = commands[i];

    પસંદગી (cmd.op)
    {
      સ્થિતિ OpCode_hmoveto:
      સ્થિતિ OpCode_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (OpCode_rmoveto);
        gen.args.alloc (2);

        જો (cmd.op == OpCode_hmoveto && cmd.args.length >= 1)
        {
          gen.args.push (cmd.args[0]);  /* dx */
          સંખ્યા_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dy = 0 */
        }
        નહીંતર જો (cmd.op == OpCode_vmoveto && cmd.args.length >= 1)
        {
          સંખ્યા_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dx = 0 */
          gen.args.push (cmd.args[0]);  /* dy */
        }
        result.push (gen);
        તોડો;
      }

      સ્થિતિ OpCode_hlineto:
      સ્થિતિ OpCode_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        તાર્કિક is_h = (cmd.op == OpCode_hlineto);
        સંખ્યા_t zero; zero.set_int (0);

        માટે (unsigned j = 0; j < cmd.args.length; j++)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);

          તાર્કિક is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          જો (is_horizontal)
          {
            seg.args.push (cmd.args[j]);  /* dx */
            seg.args.push (zero);          /* dy = 0 */
          }
          નહીંતર
          {
            seg.args.push (zero);          /* dx = 0 */
            seg.args.push (cmd.args[j]);  /* dy */
          }
          result.push (seg);
        }
        તોડો;
      }

      સ્થિતિ OpCode_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        માટે (unsigned j = 0; j + 1 < cmd.args.length; j += 2)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);
          seg.args.push (cmd.args[j]);
          seg.args.push (cmd.args[j + 1]);
          result.push (seg);
        }
        તોડો;
      }

      સ્થિતિ OpCode_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        માટે (unsigned j = 0; j + 5 < cmd.args.length; j += 6)
        {
          cs_command_t seg (OpCode_rrcurveto);
          seg.args.alloc (6);
          માટે (unsigned k = 0; k < 6; k++)
            seg.args.push (cmd.args[j + k]);
          result.push (seg);
        }
        તોડો;
      }

      મૂળભૂત:
        /* Keep other operators as-is */
        result.push (cmd);
        તોડો;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  માટે (unsigned i = 0; i < result.length; i++)
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
સ્થિર શૂન્ય
specialize_commands (hb_vector_t<cs_command_t> &commands,
                     unsigned maxstack = 48)
{
  જો (commands.length == 0) પાછા;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  માટે (unsigned i = 0; i < commands.length; i++)
  {
    auto &cmd = commands[i];

    જો ((cmd.op == OpCode_rmoveto || cmd.op == OpCode_rlineto) &&
        cmd.args.length == 2)
    {
      તાર્કિક dx_zero = is_zero (cmd.args[0]);
      તાર્કિક dy_zero = is_zero (cmd.args[1]);

      જો (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == OpCode_rmoveto) ? OpCode_vmoveto : OpCode_vlineto;
        /* Shift dy to position 0 */
        cmd.args[0] = cmd.args[1];
        cmd.args.resize (1);
      }
      નહીંતર જો (!dx_zero && dy_zero)
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
  માટે (પૂર્ણાંક i = (પૂર્ણાંક)commands.length - 1; i > 0; i--)
  {
    auto &cmd = commands[i];
    auto &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    જો (((prev.op == OpCode_hlineto && cmd.op == OpCode_vlineto) ||
         (prev.op == OpCode_vlineto && cmd.op == OpCode_hlineto)) &&
        (prev.args.length & 1))
    {
      /* Check stack depth */
      unsigned combined_args = prev.args.length + cmd.args.length;
      જો (combined_args < maxstack)
      {
        /* Merge into first command, keep its operator */
        માટે (unsigned j = 0; j < cmd.args.length; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  માટે (પૂર્ણાંક i = (પૂર્ણાંક)commands.length - 1; i > 0; i--)
  {
    auto &cmd = commands[i];
    auto &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    જો (prev.op == cmd.op &&
        (cmd.op == OpCode_rlineto || cmd.op == OpCode_rrcurveto ||
         ((cmd.op == OpCode_hlineto || cmd.op == OpCode_vlineto) &&
          !(prev.args.length & 1))))
    {
      /* Check stack depth */
      unsigned combined_args = prev.args.length + cmd.args.length;
      જો (combined_args < maxstack)
      {
        /* Merge args */
        માટે (unsigned j = 0; j < cmd.args.length; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
સ્થિર તાર્કિક
encode_commands (અચલ hb_vector_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  માટે (અચલ auto &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    માટે (અચલ auto &arg : cmd.args)
      encoder.encode_અંક_cs (arg);

    /* Encode operator */
    જો (cmd.op != OpCode_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    જો (cmd.op == OpCode_hintmask || cmd.op == OpCode_cntrmask)
    {
      માટે (અચલ auto &byte : cmd.mask_bytes)
        encoder.encode_byte (byte);
    }

    જો (encoder.in_error ())
      પાછા ખોટું;
  }

  પાછા સાચું;
}

} /* namespace CFF */

#endif /* HB_CFF_SPECIALIZER_HH */
