#അന്തര്ഭാവ "hb-ot-shaper-use-table.hh"

പൂർണ്ണസംഖ്യ പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  എങ്കിൽ (argc != 2)
  {
    വേണ്ടി (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    തിരികെ 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  തിരികെ 0;
}
