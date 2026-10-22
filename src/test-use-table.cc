#ضمن "hb-ot-shaper-use-table.hh"

صحيح main (صحيح argc, حرف **argv)
{
  אם (argc != 2)
  {
    עבור (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    החזר 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  החזר 0;
}
