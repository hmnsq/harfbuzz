/*
 * CFF CharString Specializer
 *
 * Optimizes CharString bytecode by using specialized operators
 * (hlineto, vlineto, hhcurveto, etc.) to save bytes and respects
 * CFF1 stack limit (48 values).
 *
 * Based on fontTools.cffLib.specializer
 */

#నిర్వచితం_కాకపోతే HB_CFF_SPECIALIZER_HH
#నిర్వచించు HB_CFF_SPECIALIZER_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-cff-interp-cs-common.hh"

నామాకాశ CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
స్థిర అంతఃవరుస తార్కిక
is_zero (స్థిరరాశి సంఖ్య_t &n)
{
  తిరిగి n.గమ్యం_real () == 0.;
}

/* Generalize CharString commands to canonical form
 *
 * Converts all operators to their general forms and breaks down
 * multi-segment operators into single segments. This ensures we
 * start from a clean baseline before specialization.
 *
 * Based on fontTools.cffLib.specializer.generalizeCommands
 */
స్థిర శూన్య
generalize_commands (hb_సదిశ_t<cs_command_t> &commands)
{
  hb_సదిశ_t<cs_command_t> ఫలితం;
  ఫలితం.alloc (commands.నిడివి * 2);  /* Estimate: might expand */

  కొరకు (గుర్తులేని i = 0; i < commands.నిడివి; i++)
  {
    స్వయంచాలక &cmd = commands[i];

    ఎంపిక (cmd.op)
    {
      స్థితి Op_సంకేతం_hmoveto:
      స్థితి Op_సంకేతం_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (Op_సంకేతం_rmoveto);
        gen.వాదనలు.alloc (2);

        ఒకవేళ (cmd.op == Op_సంకేతం_hmoveto && cmd.వాదనలు.నిడివి >= 1)
        {
          gen.వాదనలు.push (cmd.వాదనలు[0]);  /* dx */
          సంఖ్య_t zero; zero.సమితి_int (0);
          gen.వాదనలు.push (zero);          /* dy = 0 */
        }
        లేకపోతే ఒకవేళ (cmd.op == Op_సంకేతం_vmoveto && cmd.వాదనలు.నిడివి >= 1)
        {
          సంఖ్య_t zero; zero.సమితి_int (0);
          gen.వాదనలు.push (zero);          /* dx = 0 */
          gen.వాదనలు.push (cmd.వాదనలు[0]);  /* dy */
        }
        ఫలితం.push (gen);
        ఆపు;
      }

      స్థితి Op_సంకేతం_hlineto:
      స్థితి Op_సంకేతం_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        తార్కిక is_h = (cmd.op == Op_సంకేతం_hlineto);
        సంఖ్య_t zero; zero.సమితి_int (0);

        కొరకు (గుర్తులేని j = 0; j < cmd.వాదనలు.నిడివి; j++)
        {
          cs_command_t seg (Op_సంకేతం_rlineto);
          seg.వాదనలు.alloc (2);

          తార్కిక is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          ఒకవేళ (is_horizontal)
          {
            seg.వాదనలు.push (cmd.వాదనలు[j]);  /* dx */
            seg.వాదనలు.push (zero);          /* dy = 0 */
          }
          లేకపోతే
          {
            seg.వాదనలు.push (zero);          /* dx = 0 */
            seg.వాదనలు.push (cmd.వాదనలు[j]);  /* dy */
          }
          ఫలితం.push (seg);
        }
        ఆపు;
      }

      స్థితి Op_సంకేతం_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        కొరకు (గుర్తులేని j = 0; j + 1 < cmd.వాదనలు.నిడివి; j += 2)
        {
          cs_command_t seg (Op_సంకేతం_rlineto);
          seg.వాదనలు.alloc (2);
          seg.వాదనలు.push (cmd.వాదనలు[j]);
          seg.వాదనలు.push (cmd.వాదనలు[j + 1]);
          ఫలితం.push (seg);
        }
        ఆపు;
      }

      స్థితి Op_సంకేతం_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        కొరకు (గుర్తులేని j = 0; j + 5 < cmd.వాదనలు.నిడివి; j += 6)
        {
          cs_command_t seg (Op_సంకేతం_rrcurveto);
          seg.వాదనలు.alloc (6);
          కొరకు (గుర్తులేని k = 0; k < 6; k++)
            seg.వాదనలు.push (cmd.వాదనలు[j + k]);
          ఫలితం.push (seg);
        }
        ఆపు;
      }

      అప్రమేయ:
        /* Keep other operators as-is */
        ఫలితం.push (cmd);
        ఆపు;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  కొరకు (గుర్తులేని i = 0; i < ఫలితం.నిడివి; i++)
    commands.push (ఫలితం[i]);
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
స్థిర శూన్య
specialize_commands (hb_సదిశ_t<cs_command_t> &commands,
                     గుర్తులేని maxstack = 48)
{
  ఒకవేళ (commands.నిడివి == 0) తిరిగి;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  కొరకు (గుర్తులేని i = 0; i < commands.నిడివి; i++)
  {
    స్వయంచాలక &cmd = commands[i];

    ఒకవేళ ((cmd.op == Op_సంకేతం_rmoveto || cmd.op == Op_సంకేతం_rlineto) &&
        cmd.వాదనలు.నిడివి == 2)
    {
      తార్కిక dx_zero = is_zero (cmd.వాదనలు[0]);
      తార్కిక dy_zero = is_zero (cmd.వాదనలు[1]);

      ఒకవేళ (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == Op_సంకేతం_rmoveto) ? Op_సంకేతం_vmoveto : Op_సంకేతం_vlineto;
        /* Shift dy to position 0 */
        cmd.వాదనలు[0] = cmd.వాదనలు[1];
        cmd.వాదనలు.resize (1);
      }
      లేకపోతే ఒకవేళ (!dx_zero && dy_zero)
      {
        /* Horizontal movement (dy=0): keep only dx */
        cmd.op = (cmd.op == Op_సంకేతం_rmoveto) ? Op_సంకేతం_hmoveto : Op_సంకేతం_hlineto;
        cmd.వాదనలు.resize (1);  /* Keep only dx */
      }
      /* else: both zero or both non-zero, keep as rmoveto/rlineto */
    }
  }

  /* Pass 2: Combine adjacent hlineto/vlineto operators
   * hlineto can take multiple args alternating with vlineto
   * This saves operator bytes */
  కొరకు (పూర్ణాంకం i = (పూర్ణాంకం)commands.నిడివి - 1; i > 0; i--)
  {
    స్వయంచాలక &cmd = commands[i];
    స్వయంచాలక &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    ఒకవేళ (((prev.op == Op_సంకేతం_hlineto && cmd.op == Op_సంకేతం_vlineto) ||
         (prev.op == Op_సంకేతం_vlineto && cmd.op == Op_సంకేతం_hlineto)) &&
        (prev.వాదనలు.నిడివి & 1))
    {
      /* Check stack depth */
      గుర్తులేని combined_వాదనలు = prev.వాదనలు.నిడివి + cmd.వాదనలు.నిడివి;
      ఒకవేళ (combined_వాదనలు < maxstack)
      {
        /* Merge into first command, keep its operator */
        కొరకు (గుర్తులేని j = 0; j < cmd.వాదనలు.నిడివి; j++)
          prev.వాదనలు.push (cmd.వాదనలు[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  కొరకు (పూర్ణాంకం i = (పూర్ణాంకం)commands.నిడివి - 1; i > 0; i--)
  {
    స్వయంచాలక &cmd = commands[i];
    స్వయంచాలక &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    ఒకవేళ (prev.op == cmd.op &&
        (cmd.op == Op_సంకేతం_rlineto || cmd.op == Op_సంకేతం_rrcurveto ||
         ((cmd.op == Op_సంకేతం_hlineto || cmd.op == Op_సంకేతం_vlineto) &&
          !(prev.వాదనలు.నిడివి & 1))))
    {
      /* Check stack depth */
      గుర్తులేని combined_వాదనలు = prev.వాదనలు.నిడివి + cmd.వాదనలు.నిడివి;
      ఒకవేళ (combined_వాదనలు < maxstack)
      {
        /* Merge args */
        కొరకు (గుర్తులేని j = 0; j < cmd.వాదనలు.నిడివి; j++)
          prev.వాదనలు.push (cmd.వాదనలు[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
స్థిర తార్కిక
encode_commands (స్థిరరాశి hb_సదిశ_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  కొరకు (స్థిరరాశి స్వయంచాలక &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    కొరకు (స్థిరరాశి స్వయంచాలక &arg : cmd.వాదనలు)
      encoder.encode_అంకె_cs (arg);

    /* Encode operator */
    ఒకవేళ (cmd.op != Op_సంకేతం_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    ఒకవేళ (cmd.op == Op_సంకేతం_hintmask || cmd.op == Op_సంకేతం_cntrmask)
    {
      కొరకు (స్థిరరాశి స్వయంచాలక &byte : cmd.ముసుగు_బైట్లు)
        encoder.encode_byte (byte);
    }

    ఒకవేళ (encoder.లోపల_error ())
      తిరిగి అబద్ధం;
  }

  తిరిగి నిజం;
}

} /* namespace CFF */

#అయితే_ముగింపు /* HB_CFF_SPECIALIZER_HH */
