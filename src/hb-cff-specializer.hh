/*
 * CFF CharString Specializer
 *
 * Optimizes CharString bytecode by using specialized operators
 * (hlineto, vlineto, hhcurveto, etc.) to save bytes and respects
 * CFF1 stack limit (48 values).
 *
 * Based on fontTools.cffLib.specializer
 */

#अगर_अघोषित HB_CFF_SPECIALIZER_HH
#घोषणा HB_CFF_SPECIALIZER_HH

#समावेश "hb.hh"
#समावेश "hb-cff-interp-cs-common.hh"

namespace CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
अटल अंतरभूत bool
is_zero (स्थिर संख्या_t &n)
{
  वापस n.to_real () == 0.;
}

/* Generalize CharString commands to canonical form
 *
 * Converts all operators to their general forms and breaks down
 * multi-segment operators into single segments. This ensures we
 * start from a clean baseline before specialization.
 *
 * Based on fontTools.cffLib.specializer.generalizeCommands
 */
अटल व्योम
generalize_commands (hb_vector_t<cs_command_t> &commands)
{
  hb_vector_t<cs_command_t> परिणाम;
  परिणाम.alloc (commands.लंबाई * 2);  /* Estimate: might expand */

  क्रम (अचिन्हित i = 0; i < commands.लंबाई; i++)
  {
    स्वतः &cmd = commands[i];

    चयन (cmd.op)
    {
      हाल OpCode_hmoveto:
      हाल OpCode_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (OpCode_rmoveto);
        gen.args.alloc (2);

        अगर (cmd.op == OpCode_hmoveto && cmd.args.लंबाई >= 1)
        {
          gen.args.push (cmd.args[0]);  /* dx */
          संख्या_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dy = 0 */
        }
        अन्यथा अगर (cmd.op == OpCode_vmoveto && cmd.args.लंबाई >= 1)
        {
          संख्या_t zero; zero.set_int (0);
          gen.args.push (zero);          /* dx = 0 */
          gen.args.push (cmd.args[0]);  /* dy */
        }
        परिणाम.push (gen);
        अवरोध;
      }

      हाल OpCode_hlineto:
      हाल OpCode_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        bool is_h = (cmd.op == OpCode_hlineto);
        संख्या_t zero; zero.set_int (0);

        क्रम (अचिन्हित j = 0; j < cmd.args.लंबाई; j++)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);

          bool is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          अगर (is_horizontal)
          {
            seg.args.push (cmd.args[j]);  /* dx */
            seg.args.push (zero);          /* dy = 0 */
          }
          अन्यथा
          {
            seg.args.push (zero);          /* dx = 0 */
            seg.args.push (cmd.args[j]);  /* dy */
          }
          परिणाम.push (seg);
        }
        अवरोध;
      }

      हाल OpCode_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        क्रम (अचिन्हित j = 0; j + 1 < cmd.args.लंबाई; j += 2)
        {
          cs_command_t seg (OpCode_rlineto);
          seg.args.alloc (2);
          seg.args.push (cmd.args[j]);
          seg.args.push (cmd.args[j + 1]);
          परिणाम.push (seg);
        }
        अवरोध;
      }

      हाल OpCode_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        क्रम (अचिन्हित j = 0; j + 5 < cmd.args.लंबाई; j += 6)
        {
          cs_command_t seg (OpCode_rrcurveto);
          seg.args.alloc (6);
          क्रम (अचिन्हित k = 0; k < 6; k++)
            seg.args.push (cmd.args[j + k]);
          परिणाम.push (seg);
        }
        अवरोध;
      }

      शेष:
        /* Keep other operators as-is */
        परिणाम.push (cmd);
        अवरोध;
    }
  }

  /* Replace commands with generalized result */
  commands.सफा ();
  क्रम (अचिन्हित i = 0; i < परिणाम.लंबाई; i++)
    commands.push (परिणाम[i]);
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
अटल व्योम
specialize_commands (hb_vector_t<cs_command_t> &commands,
                     अचिन्हित maxstack = 48)
{
  अगर (commands.लंबाई == 0) वापस;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  क्रम (अचिन्हित i = 0; i < commands.लंबाई; i++)
  {
    स्वतः &cmd = commands[i];

    अगर ((cmd.op == OpCode_rmoveto || cmd.op == OpCode_rlineto) &&
        cmd.args.लंबाई == 2)
    {
      bool dx_zero = is_zero (cmd.args[0]);
      bool dy_zero = is_zero (cmd.args[1]);

      अगर (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == OpCode_rmoveto) ? OpCode_vmoveto : OpCode_vlineto;
        /* Shift dy to position 0 */
        cmd.args[0] = cmd.args[1];
        cmd.args.resize (1);
      }
      अन्यथा अगर (!dx_zero && dy_zero)
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
  क्रम (पूर्णांक i = (पूर्णांक)commands.लंबाई - 1; i > 0; i--)
  {
    स्वतः &cmd = commands[i];
    स्वतः &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    अगर (((prev.op == OpCode_hlineto && cmd.op == OpCode_vlineto) ||
         (prev.op == OpCode_vlineto && cmd.op == OpCode_hlineto)) &&
        (prev.args.लंबाई & 1))
    {
      /* Check stack depth */
      अचिन्हित combined_args = prev.args.लंबाई + cmd.args.लंबाई;
      अगर (combined_args < maxstack)
      {
        /* Merge into first command, keep its operator */
        क्रम (अचिन्हित j = 0; j < cmd.args.लंबाई; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  क्रम (पूर्णांक i = (पूर्णांक)commands.लंबाई - 1; i > 0; i--)
  {
    स्वतः &cmd = commands[i];
    स्वतः &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    अगर (prev.op == cmd.op &&
        (cmd.op == OpCode_rlineto || cmd.op == OpCode_rrcurveto ||
         ((cmd.op == OpCode_hlineto || cmd.op == OpCode_vlineto) &&
          !(prev.args.लंबाई & 1))))
    {
      /* Check stack depth */
      अचिन्हित combined_args = prev.args.लंबाई + cmd.args.लंबाई;
      अगर (combined_args < maxstack)
      {
        /* Merge args */
        क्रम (अचिन्हित j = 0; j < cmd.args.लंबाई; j++)
          prev.args.push (cmd.args[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
अटल bool
encode_commands (स्थिर hb_vector_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  क्रम (स्थिर स्वतः &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    क्रम (स्थिर स्वतः &arg : cmd.args)
      encoder.encode_अंक_cs (arg);

    /* Encode operator */
    अगर (cmd.op != OpCode_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    अगर (cmd.op == OpCode_hintmask || cmd.op == OpCode_cntrmask)
    {
      क्रम (स्थिर स्वतः &byte : cmd.mask_bytes)
        encoder.encode_byte (byte);
    }

    अगर (encoder.in_error ())
      वापस false;
  }

  वापस true;
}

} /* namespace CFF */

#पूर्ण_अगर /* HB_CFF_SPECIALIZER_HH */
