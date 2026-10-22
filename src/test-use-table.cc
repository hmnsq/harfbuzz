#અન્તર્ભાવ "hb-ot-shaper-use-table.hh"

પૂર્ણાંક મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  જો (argc != 2)
  {
    માટે (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    પાછા 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  પાછા 0;
}
