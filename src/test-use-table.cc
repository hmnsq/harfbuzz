#அந்தர்பாவ "hb-ot-shaper-use-table.hh"

முழு முதன்மை (முழு argc, எழுத்து **argv)
{
  எனில் (argc != 2)
  {
    க்கு (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    திரும்பு 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  திரும்பு 0;
}
