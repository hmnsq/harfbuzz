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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-cff-interp-cs-common.hh"

नामाकाश CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
स्थिर ओळीत तार्किक
is_zero (स्थिरांक संख्या_t &n)
{
  परत n.to_real () == 0.;
}

/* Generalize CharString commands to canonical form
 *
 * Converts all operators to their general forms and breaks down
 * multi-segment operators into single segments. This ensures we
 * start from a clean baseline before specialization.
 *
 * Based on fontTools.cffLib.specializer.generalizeCommands
 */
स्थिर शून्य
generalize_commands (hb_vector_t<cs_command_t> &commands)
{
  hb_vector_t<cs_command_t> result;
  result.alloc (commands.length * 2);  /* Estimate: might expand */

  साठी (अचिन्हित i = 0; i < commands.length; i++)
  {
    स्वतः &cmd = commands[i];

    निवड (cmd.op)
    {
      स्थिती OpCode_hmoveto:
      स्थिती OpCode_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (OpCode_rmoveto);
        gen.args.alloc (2);

        जर (cmd.op == OpCode_hmoveto && cmd.args.length >= 1)
        {
          gen.args.push (cmd.args[0]);  /* dx */
          संख्या_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dy = 0 */
        }
        अन्यथा जर (cmd.op == OpCode_vmoveto && cmd.args.length >= 1)
        {
          संख्या_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dx = 0 */
          gen.args.push (cmd.args[0]);  /* dy */
        }
        result.push (gen);
        थांबा;
      }

      स्थिती OpCode_hlineto:
      स्थिती OpCode_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        तार्किक is_h = (cmd.op == OpCode_hlineto);
        संख्या_t zero; zero.set_int (0);

        साठी (अचिन्हित j = 0; j < cmd.args.length; j++)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);

          तार्किक is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          जर (is_horizontal)
          {
            seg.args.push (cmd.args[j]);  /* dx */
            seg.args.push (zero);          /* dy = 0 */
          }
          अन्यथा
          {
            seg.args.push (zero);          /* dx = 0 */
            seg.args.push (cmd.args[j]);  /* dy */
          }
          result.push (seg);
        }
        थांबा;
      }

      स्थिती OpCode_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        साठी (अचिन्हित j = 0; j + 1 < cmd.args.length; j += 2)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);
          seg.args.push (cmd.args[j]);
          seg.args.push (cmd.args[j + 1]);
          result.push (seg);
        }
        थांबा;
      }

      स्थिती OpCode_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        साठी (अचिन्हित j = 0; j + 5 < cmd.args.length; j += 6)
        {
          cs_command_t seg (OpCode_rrcurveto);
          seg.args.alloc (6);
          साठी (अचिन्हित k = 0; k < 6; k++)
            seg.args.push (cmd.args[j + k]);
          result.push (seg);
        }
        थांबा;
      }

      पूर्वनिर्धारित:
        /* Keep other operators as-is */
        result.push (cmd);
        थांबा;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  साठी (अचिन्हित i = 0; i < result.length; i++)
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
स्थिर शून्य
specialize_commands (hb_vector_t<cs_command_t> &commands,
                     अचिन्हित maxstack = 48)
{
  जर (commands.length == 0) परत;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  साठी (अचिन्हित i = 0; i < commands.length; i++)
  {
    स्वतः &cmd = commands[i];

    जर ((cmd.op == OpCode_rmoveto || cmd.op == OpCode_rlineto) &&
        cmd.args.length == 2)
    {
      तार्किक dx_zero = is_zero (cmd.args[0]);
      तार्किक dy_zero = is_zero (cmd.args[1]);

      जर (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == OpCode_rmoveto) ? OpCode_vmoveto : OpCode_vlineto;
        /* Shift dy to position 0 */
        cmd.args[0] = cmd.args[1];
        cmd.args.resize (1);
      }
      अन्यथा जर (!dx_zero && dy_zero)
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
  साठी (पूर्णांक i = (पूर्णांक)commands.length - 1; i > 0; i--)
  {
    स्वतः &cmd = commands[i];
    स्वतः &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    जर (((prev.op == OpCode_hlineto && cmd.op == OpCode_vlineto) ||
         (prev.op == OpCode_vlineto && cmd.op == OpCode_hlineto)) &&
        (prev.args.length & 1))
    {
      /* Check stack depth */
      अचिन्हित combined_args = prev.args.length + cmd.args.length;
      जर (combined_args < maxstack)
      {
        /* Merge into first command, keep its operator */
        साठी (अचिन्हित j = 0; j < cmd.args.length; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  साठी (पूर्णांक i = (पूर्णांक)commands.length - 1; i > 0; i--)
  {
    स्वतः &cmd = commands[i];
    स्वतः &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    जर (prev.op == cmd.op &&
        (cmd.op == OpCode_rlineto || cmd.op == OpCode_rrcurveto ||
         ((cmd.op == OpCode_hlineto || cmd.op == OpCode_vlineto) &&
          !(prev.args.length & 1))))
    {
      /* Check stack depth */
      अचिन्हित combined_args = prev.args.length + cmd.args.length;
      जर (combined_args < maxstack)
      {
        /* Merge args */
        साठी (अचिन्हित j = 0; j < cmd.args.length; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
स्थिर तार्किक
encode_commands (स्थिरांक hb_vector_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  साठी (स्थिरांक स्वतः &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    साठी (स्थिरांक स्वतः &arg : cmd.args)
      encoder.encode_अंक_cs (arg);

    /* Encode operator */
    जर (cmd.op != OpCode_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    जर (cmd.op == OpCode_hintmask || cmd.op == OpCode_cntrmask)
    {
      साठी (स्थिरांक स्वतः &byte : cmd.mask_bytes)
        encoder.encode_byte (byte);
    }

    जर (encoder.in_error ())
      परत असत्य;
  }

  परत सत्य;
}

} /* namespace CFF */

#endif /* HB_CFF_SPECIALIZER_HH */
