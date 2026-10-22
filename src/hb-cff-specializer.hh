/*
 * CFF CharString Specializer
 *
 * Optimizes CharString bytecode by using specialized operators
 * (hlineto, vlineto, hhcurveto, etc.) to save bytes and respects
 * CFF1 stack limit (48 values).
 *
 * Based on fontTools.cffLib.specializer
 */

#جے_بنا_تعریف HB_CFF_SPECIALIZER_HH
#تعریف_کرو HB_CFF_SPECIALIZER_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
ساكن مضمن منطقي
is_zero (ثابت نمبر_t &n)
{
  أرجع n.منزل_real () == 0.;
}

/* Generalize CharString commands to canonical form
 *
 * Converts all operators to their general forms and breaks down
 * multi-segment operators into single segments. This ensures we
 * start from a clean baseline before specialization.
 *
 * Based on fontTools.cffLib.specializer.generalizeCommands
 */
ساكن فراغ
generalize_commands (hb_سمتیہ_t<cs_command_t> &commands)
{
  hb_سمتیہ_t<cs_command_t> نتیجہ;
  نتیجہ.alloc (commands.لمائی * 2);  /* Estimate: might expand */

  لكل (بنانشانی i = 0; i < commands.لمائی; i++)
  {
    خودکار &cmd = commands[i];

    بدل (cmd.op)
    {
      حالة Op_کوڈ_hmoveto:
      حالة Op_کوڈ_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (Op_کوڈ_rmoveto);
        gen.دلیلاں.alloc (2);

        إذا (cmd.op == Op_کوڈ_hmoveto && cmd.دلیلاں.لمائی >= 1)
        {
          gen.دلیلاں.push (cmd.دلیلاں[0]);  /* dx */
          نمبر_t zero; zero.سیٹ_int (0);
          gen.دلیلاں.push (zero);          /* dy = 0 */
        }
        وإلا إذا (cmd.op == Op_کوڈ_vmoveto && cmd.دلیلاں.لمائی >= 1)
        {
          نمبر_t zero; zero.سیٹ_int (0);
          gen.دلیلاں.push (zero);          /* dx = 0 */
          gen.دلیلاں.push (cmd.دلیلاں[0]);  /* dy */
        }
        نتیجہ.push (gen);
        اكسر;
      }

      حالة Op_کوڈ_hlineto:
      حالة Op_کوڈ_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        منطقي is_h = (cmd.op == Op_کوڈ_hlineto);
        نمبر_t zero; zero.سیٹ_int (0);

        لكل (بنانشانی j = 0; j < cmd.دلیلاں.لمائی; j++)
        {
          cs_command_t seg (Op_کوڈ_rlineto);
          seg.دلیلاں.alloc (2);

          منطقي is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          إذا (is_horizontal)
          {
            seg.دلیلاں.push (cmd.دلیلاں[j]);  /* dx */
            seg.دلیلاں.push (zero);          /* dy = 0 */
          }
          وإلا
          {
            seg.دلیلاں.push (zero);          /* dx = 0 */
            seg.دلیلاں.push (cmd.دلیلاں[j]);  /* dy */
          }
          نتیجہ.push (seg);
        }
        اكسر;
      }

      حالة Op_کوڈ_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        لكل (بنانشانی j = 0; j + 1 < cmd.دلیلاں.لمائی; j += 2)
        {
          cs_command_t seg (Op_کوڈ_rlineto);
          seg.دلیلاں.alloc (2);
          seg.دلیلاں.push (cmd.دلیلاں[j]);
          seg.دلیلاں.push (cmd.دلیلاں[j + 1]);
          نتیجہ.push (seg);
        }
        اكسر;
      }

      حالة Op_کوڈ_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        لكل (بنانشانی j = 0; j + 5 < cmd.دلیلاں.لمائی; j += 6)
        {
          cs_command_t seg (Op_کوڈ_rrcurveto);
          seg.دلیلاں.alloc (6);
          لكل (بنانشانی k = 0; k < 6; k++)
            seg.دلیلاں.push (cmd.دلیلاں[j + k]);
          نتیجہ.push (seg);
        }
        اكسر;
      }

      افتراضي:
        /* Keep other operators as-is */
        نتیجہ.push (cmd);
        اكسر;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  لكل (بنانشانی i = 0; i < نتیجہ.لمائی; i++)
    commands.push (نتیجہ[i]);
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
ساكن فراغ
specialize_commands (hb_سمتیہ_t<cs_command_t> &commands,
                     بنانشانی maxstack = 48)
{
  إذا (commands.لمائی == 0) أرجع;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  لكل (بنانشانی i = 0; i < commands.لمائی; i++)
  {
    خودکار &cmd = commands[i];

    إذا ((cmd.op == Op_کوڈ_rmoveto || cmd.op == Op_کوڈ_rlineto) &&
        cmd.دلیلاں.لمائی == 2)
    {
      منطقي dx_zero = is_zero (cmd.دلیلاں[0]);
      منطقي dy_zero = is_zero (cmd.دلیلاں[1]);

      إذا (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == Op_کوڈ_rmoveto) ? Op_کوڈ_vmoveto : Op_کوڈ_vlineto;
        /* Shift dy to position 0 */
        cmd.دلیلاں[0] = cmd.دلیلاں[1];
        cmd.دلیلاں.resize (1);
      }
      وإلا إذا (!dx_zero && dy_zero)
      {
        /* Horizontal movement (dy=0): keep only dx */
        cmd.op = (cmd.op == Op_کوڈ_rmoveto) ? Op_کوڈ_hmoveto : Op_کوڈ_hlineto;
        cmd.دلیلاں.resize (1);  /* Keep only dx */
      }
      /* else: both zero or both non-zero, keep as rmoveto/rlineto */
    }
  }

  /* Pass 2: Combine adjacent hlineto/vlineto operators
   * hlineto can take multiple args alternating with vlineto
   * This saves operator bytes */
  لكل (صحيح i = (صحيح)commands.لمائی - 1; i > 0; i--)
  {
    خودکار &cmd = commands[i];
    خودکار &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    إذا (((prev.op == Op_کوڈ_hlineto && cmd.op == Op_کوڈ_vlineto) ||
         (prev.op == Op_کوڈ_vlineto && cmd.op == Op_کوڈ_hlineto)) &&
        (prev.دلیلاں.لمائی & 1))
    {
      /* Check stack depth */
      بنانشانی combined_دلیلاں = prev.دلیلاں.لمائی + cmd.دلیلاں.لمائی;
      إذا (combined_دلیلاں < maxstack)
      {
        /* Merge into first command, keep its operator */
        لكل (بنانشانی j = 0; j < cmd.دلیلاں.لمائی; j++)
          prev.دلیلاں.push (cmd.دلیلاں[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  لكل (صحيح i = (صحيح)commands.لمائی - 1; i > 0; i--)
  {
    خودکار &cmd = commands[i];
    خودکار &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    إذا (prev.op == cmd.op &&
        (cmd.op == Op_کوڈ_rlineto || cmd.op == Op_کوڈ_rrcurveto ||
         ((cmd.op == Op_کوڈ_hlineto || cmd.op == Op_کوڈ_vlineto) &&
          !(prev.دلیلاں.لمائی & 1))))
    {
      /* Check stack depth */
      بنانشانی combined_دلیلاں = prev.دلیلاں.لمائی + cmd.دلیلاں.لمائی;
      إذا (combined_دلیلاں < maxstack)
      {
        /* Merge args */
        لكل (بنانشانی j = 0; j < cmd.دلیلاں.لمائی; j++)
          prev.دلیلاں.push (cmd.دلیلاں[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
ساكن منطقي
encode_commands (ثابت hb_سمتیہ_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  لكل (ثابت خودکار &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    لكل (ثابت خودکار &arg : cmd.دلیلاں)
      encoder.encode_عدد_cs (arg);

    /* Encode operator */
    إذا (cmd.op != Op_کوڈ_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    إذا (cmd.op == Op_کوڈ_hintmask || cmd.op == Op_کوڈ_cntrmask)
    {
      لكل (ثابت خودکار &byte : cmd.نقاب_بائٹاں)
        encoder.encode_byte (byte);
    }

    إذا (encoder.وچ_error ())
      أرجع خطأ;
  }

  أرجع سچ;
}

} /* namespace CFF */

#جے_انت /* HB_CFF_SPECIALIZER_HH */
