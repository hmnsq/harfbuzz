/*
 * CFF CharString Specializer
 *
 * Optimizes CharString bytecode by using specialized operators
 * (hlineto, vlineto, hhcurveto, etc.) to save bytes and respects
 * CFF1 stack limit (48 values).
 *
 * Based on fontTools.cffLib.specializer
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CFF_SPECIALIZER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CFF_SPECIALIZER_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff-interp-cs-common.hh"

ਨਾਮਾਕਾਸ਼ CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
is_zero (ਅਚਲ ਸੰਖਿਆ_t &n)
{
  ਵਾਪਸ n.ਮੰਜ਼ਿਲ_real () == 0.;
}

/* Generalize CharString commands to canonical form
 *
 * Converts all operators to their general forms and breaks down
 * multi-segment operators into single segments. This ensures we
 * start from a clean baseline before specialization.
 *
 * Based on fontTools.cffLib.specializer.generalizeCommands
 */
ਸਥਿਰ ਖਾਲੀ
generalize_commands (hb_ਸਦਿਸ਼_t<cs_command_t> &commands)
{
  hb_ਸਦਿਸ਼_t<cs_command_t> ਨਤੀਜਾ;
  ਨਤੀਜਾ.alloc (commands.ਲੰਮਾਈ * 2);  /* Estimate: might expand */

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < commands.ਲੰਮਾਈ; i++)
  {
    ਸਵੈਚਾਲਿਤ &cmd = commands[i];

    ਚੋਣ (cmd.op)
    {
      ਹਾਲਤ Op_ਕੋਡ_hmoveto:
      ਹਾਲਤ Op_ਕੋਡ_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (Op_ਕੋਡ_rmoveto);
        gen.ਦਲੀਲਾਂ.alloc (2);

        ਜੇ (cmd.op == Op_ਕੋਡ_hmoveto && cmd.ਦਲੀਲਾਂ.ਲੰਮਾਈ >= 1)
        {
          gen.ਦਲੀਲਾਂ.push (cmd.ਦਲੀਲਾਂ[0]);  /* dx */
          ਸੰਖਿਆ_t zero; zero.ਸੈੱਟ_int (0);
          gen.ਦਲੀਲਾਂ.push (zero);          /* dy = 0 */
        }
        ਨਹੀਂਤਾਂ ਜੇ (cmd.op == Op_ਕੋਡ_vmoveto && cmd.ਦਲੀਲਾਂ.ਲੰਮਾਈ >= 1)
        {
          ਸੰਖਿਆ_t zero; zero.ਸੈੱਟ_int (0);
          gen.ਦਲੀਲਾਂ.push (zero);          /* dx = 0 */
          gen.ਦਲੀਲਾਂ.push (cmd.ਦਲੀਲਾਂ[0]);  /* dy */
        }
        ਨਤੀਜਾ.push (gen);
        ਰੋਕੋ;
      }

      ਹਾਲਤ Op_ਕੋਡ_hlineto:
      ਹਾਲਤ Op_ਕੋਡ_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        ਤਾਰਕਿਕ is_h = (cmd.op == Op_ਕੋਡ_hlineto);
        ਸੰਖਿਆ_t zero; zero.ਸੈੱਟ_int (0);

        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < cmd.ਦਲੀਲਾਂ.ਲੰਮਾਈ; j++)
        {
          cs_command_t seg (Op_ਕੋਡ_rlineto);
          seg.ਦਲੀਲਾਂ.alloc (2);

          ਤਾਰਕਿਕ is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          ਜੇ (is_horizontal)
          {
            seg.ਦਲੀਲਾਂ.push (cmd.ਦਲੀਲਾਂ[j]);  /* dx */
            seg.ਦਲੀਲਾਂ.push (zero);          /* dy = 0 */
          }
          ਨਹੀਂਤਾਂ
          {
            seg.ਦਲੀਲਾਂ.push (zero);          /* dx = 0 */
            seg.ਦਲੀਲਾਂ.push (cmd.ਦਲੀਲਾਂ[j]);  /* dy */
          }
          ਨਤੀਜਾ.push (seg);
        }
        ਰੋਕੋ;
      }

      ਹਾਲਤ Op_ਕੋਡ_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j + 1 < cmd.ਦਲੀਲਾਂ.ਲੰਮਾਈ; j += 2)
        {
          cs_command_t seg (Op_ਕੋਡ_rlineto);
          seg.ਦਲੀਲਾਂ.alloc (2);
          seg.ਦਲੀਲਾਂ.push (cmd.ਦਲੀਲਾਂ[j]);
          seg.ਦਲੀਲਾਂ.push (cmd.ਦਲੀਲਾਂ[j + 1]);
          ਨਤੀਜਾ.push (seg);
        }
        ਰੋਕੋ;
      }

      ਹਾਲਤ Op_ਕੋਡ_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j + 5 < cmd.ਦਲੀਲਾਂ.ਲੰਮਾਈ; j += 6)
        {
          cs_command_t seg (Op_ਕੋਡ_rrcurveto);
          seg.ਦਲੀਲਾਂ.alloc (6);
          ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ k = 0; k < 6; k++)
            seg.ਦਲੀਲਾਂ.push (cmd.ਦਲੀਲਾਂ[j + k]);
          ਨਤੀਜਾ.push (seg);
        }
        ਰੋਕੋ;
      }

      ਮੂਲ:
        /* Keep other operators as-is */
        ਨਤੀਜਾ.push (cmd);
        ਰੋਕੋ;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਨਤੀਜਾ.ਲੰਮਾਈ; i++)
    commands.push (ਨਤੀਜਾ[i]);
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
ਸਥਿਰ ਖਾਲੀ
specialize_commands (hb_ਸਦਿਸ਼_t<cs_command_t> &commands,
                     ਬਿਨਾਨਿਸ਼ਾਨੀ maxstack = 48)
{
  ਜੇ (commands.ਲੰਮਾਈ == 0) ਵਾਪਸ;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < commands.ਲੰਮਾਈ; i++)
  {
    ਸਵੈਚਾਲਿਤ &cmd = commands[i];

    ਜੇ ((cmd.op == Op_ਕੋਡ_rmoveto || cmd.op == Op_ਕੋਡ_rlineto) &&
        cmd.ਦਲੀਲਾਂ.ਲੰਮਾਈ == 2)
    {
      ਤਾਰਕਿਕ dx_zero = is_zero (cmd.ਦਲੀਲਾਂ[0]);
      ਤਾਰਕਿਕ dy_zero = is_zero (cmd.ਦਲੀਲਾਂ[1]);

      ਜੇ (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == Op_ਕੋਡ_rmoveto) ? Op_ਕੋਡ_vmoveto : Op_ਕੋਡ_vlineto;
        /* Shift dy to position 0 */
        cmd.ਦਲੀਲਾਂ[0] = cmd.ਦਲੀਲਾਂ[1];
        cmd.ਦਲੀਲਾਂ.resize (1);
      }
      ਨਹੀਂਤਾਂ ਜੇ (!dx_zero && dy_zero)
      {
        /* Horizontal movement (dy=0): keep only dx */
        cmd.op = (cmd.op == Op_ਕੋਡ_rmoveto) ? Op_ਕੋਡ_hmoveto : Op_ਕੋਡ_hlineto;
        cmd.ਦਲੀਲਾਂ.resize (1);  /* Keep only dx */
      }
      /* else: both zero or both non-zero, keep as rmoveto/rlineto */
    }
  }

  /* Pass 2: Combine adjacent hlineto/vlineto operators
   * hlineto can take multiple args alternating with vlineto
   * This saves operator bytes */
  ਲਈ (ਪੂਰਨ i = (ਪੂਰਨ)commands.ਲੰਮਾਈ - 1; i > 0; i--)
  {
    ਸਵੈਚਾਲਿਤ &cmd = commands[i];
    ਸਵੈਚਾਲਿਤ &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    ਜੇ (((prev.op == Op_ਕੋਡ_hlineto && cmd.op == Op_ਕੋਡ_vlineto) ||
         (prev.op == Op_ਕੋਡ_vlineto && cmd.op == Op_ਕੋਡ_hlineto)) &&
        (prev.ਦਲੀਲਾਂ.ਲੰਮਾਈ & 1))
    {
      /* Check stack depth */
      ਬਿਨਾਨਿਸ਼ਾਨੀ combined_ਦਲੀਲਾਂ = prev.ਦਲੀਲਾਂ.ਲੰਮਾਈ + cmd.ਦਲੀਲਾਂ.ਲੰਮਾਈ;
      ਜੇ (combined_ਦਲੀਲਾਂ < maxstack)
      {
        /* Merge into first command, keep its operator */
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < cmd.ਦਲੀਲਾਂ.ਲੰਮਾਈ; j++)
          prev.ਦਲੀਲਾਂ.push (cmd.ਦਲੀਲਾਂ[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  ਲਈ (ਪੂਰਨ i = (ਪੂਰਨ)commands.ਲੰਮਾਈ - 1; i > 0; i--)
  {
    ਸਵੈਚਾਲਿਤ &cmd = commands[i];
    ਸਵੈਚਾਲਿਤ &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    ਜੇ (prev.op == cmd.op &&
        (cmd.op == Op_ਕੋਡ_rlineto || cmd.op == Op_ਕੋਡ_rrcurveto ||
         ((cmd.op == Op_ਕੋਡ_hlineto || cmd.op == Op_ਕੋਡ_vlineto) &&
          !(prev.ਦਲੀਲਾਂ.ਲੰਮਾਈ & 1))))
    {
      /* Check stack depth */
      ਬਿਨਾਨਿਸ਼ਾਨੀ combined_ਦਲੀਲਾਂ = prev.ਦਲੀਲਾਂ.ਲੰਮਾਈ + cmd.ਦਲੀਲਾਂ.ਲੰਮਾਈ;
      ਜੇ (combined_ਦਲੀਲਾਂ < maxstack)
      {
        /* Merge args */
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < cmd.ਦਲੀਲਾਂ.ਲੰਮਾਈ; j++)
          prev.ਦਲੀਲਾਂ.push (cmd.ਦਲੀਲਾਂ[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
ਸਥਿਰ ਤਾਰਕਿਕ
encode_commands (ਅਚਲ hb_ਸਦਿਸ਼_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &arg : cmd.ਦਲੀਲਾਂ)
      encoder.encode_ਅੰਕ_cs (arg);

    /* Encode operator */
    ਜੇ (cmd.op != Op_ਕੋਡ_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    ਜੇ (cmd.op == Op_ਕੋਡ_hintmask || cmd.op == Op_ਕੋਡ_cntrmask)
    {
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &byte : cmd.ਨਕਾਬ_ਬਾਈਟਾਂ)
        encoder.encode_byte (byte);
    }

    ਜੇ (encoder.ਵਿੱਚ_error ())
      ਵਾਪਸ ਝੂਠ;
  }

  ਵਾਪਸ ਸੱਚ;
}

} /* namespace CFF */

#ਜੇ_ਅੰਤ /* HB_CFF_SPECIALIZER_HH */
