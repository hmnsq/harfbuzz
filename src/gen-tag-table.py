#!/usr/bin/env python3

"""Generator of the mapping from OpenType tags to BCP 47 tags and vice
versa.

It creates a ``const LangTag[]``, matching the tags from the OpenType
languages system tag list to the language subtags of the BCP 47 language
subtag registry, with some manual adjustments. The mappings are
supplemented with macrolanguages' sublanguages and retired codes'
replacements, according to BCP 47 and some manual additions where BCP 47
omits a retired code entirely.

Also generated is a function, ``hb_ot_ambiguous_tag_to_language``,
intended for use by ``hb_ot_tag_to_language``. It maps OpenType tags
back to BCP 47 tags. Ambiguous OpenType tags (those that correspond to
multiple BCP 47 tags) are listed here, except when the alphabetically
first BCP 47 tag happens to be the chosen disambiguated tag. In that
case, the fallback behavior will choose the right tag anyway.

usage: ./gen-tag-table.py [--rust] languagetags language-subtag-registry

Input files:
* https://docs.microsoft.com/en-us/typography/opentype/spec/languagetags
* https://www.iana.org/assignments/language-subtag-registry/language-subtag-registry
"""

استورد collections
استورد html
من html.parser استورد HTMLParser
استورد itertools
استورد os
استورد re
استورد sys
استورد unicodedata

إذا ڊيگهه (sys.argv) > 1 و sys.argv[1] == '--rust':
	هٽايو sys.argv[1]
	output_rust = سچ
وإلا:
	output_rust = خطأ

إذا ڊيگهه (sys.argv) != 3:
	sys.exit (__doc__)

وصف expect (condition, پيغام=ڪجهه_نه):
	إذا ليس condition:
		إذا پيغام آهي ڪجهه_نه:
			ارفع AssertionError
		ارفع AssertionError (پيغام)

وصف اكتب (s):
	sys.stdout.flush ()
	sys.stdout.بفر.اكتب (s.encode ('utf-8'))

_ALLOWED_COMMENT_PUNCT = سيٽ (" -—–.,;:’'ʼ()[]/!=>_+")

وصف sanitize_comment (s):
	"""Validate that a comment contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (s, str):
		ارفع TypeError ("Expected string for comment, got %s" % type (s).__name__)
	s = s.strip ()
	إذا ليس s:
		أرجع ''
	لكل ch في s:
		cat = unicodedata.category (ch)
		إذا ليس (cat.startswith (('L', 'M', 'N', 'Z')) أو ch في _ALLOWED_COMMENT_PUNCT):
			ارفع قدر_Error ("Disallowed character %r (U+%04X) in comment: %r" % (ch, ord (ch), s))
	أرجع s

وصف sanitize_سرو (s):
	"""Validate that a header string contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (s, str):
		ارفع TypeError ("Expected string for header, got %s" % type (s).__name__)
	s = s.strip ()
	إذا ليس re.fullmatch (r'[a-zA-Z0-9 <>=/:\-_."]{1,200}', s):
		ارفع قدر_Error ("Invalid header line %r: contains disallowed characters" % s)
	أرجع s

وصف sanitize_bcp47_ٽيگ (s):
	"""Validate that a BCP-47 tag or subtag contains only alphanumeric characters and hyphens.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (s, str):
		ارفع TypeError ("Expected string, got %s" % type (s).__name__)
	إذا ليس re.fullmatch (r'[a-zA-Z0-9-]+', s):
		ارفع قدر_Error ("Invalid BCP-47 tag or subtag literal %r: must contain only alphanumeric characters and hyphens" % s)
	أرجع s

وصف sanitize_ٽيگ (ٽيگ_2):
	"""Validate that an OpenType tag contains only 1-4 alphanumeric characters or spaces.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (ٽيگ_2, str):
		ارفع TypeError ("Expected string for tag, got %s" % type (ٽيگ_2).__name__)
	إذا ٽيگ_2 == DEFAULT_LANGUAGE_SYSTEM:
		أرجع ''
	إذا ليس re.fullmatch (r'[a-zA-Z0-9 ]{1,4}', ٽيگ_2):
		ارفع قدر_Error ("Invalid tag %r: must contain only 1-4 alphanumeric characters or spaces" % ٽيگ_2)
	أرجع ٽيگ_2

DEFAULT_LANGUAGE_SYSTEM = ''

# from https://www-01.sil.org/iso639-3/iso-639-3.tab
ISO_639_3_منزل_1 = {
	'aar': 'aa',
	'abk': 'ab',
	'afr': 'af',
	'aka': 'ak',
	'amh': 'am',
	'ara': 'ar',
	'arg': 'an',
	'asm': 'as',
	'ava': 'av',
	'ave': 'ae',
	'aym': 'ay',
	'aze': 'az',
	'bak': 'ba',
	'bam': 'bm',
	'bel': 'be',
	'ben': 'bn',
	'bis': 'bi',
	'bod': 'bo',
	'bos': 'bs',
	'bre': 'br',
	'bul': 'bg',
	'cat': 'ca',
	'ces': 'cs',
	'cha': 'ch',
	'che': 'ce',
	'chu': 'cu',
	'chv': 'cv',
	'cor': 'kw',
	'cos': 'co',
	'cre': 'cr',
	'cym': 'cy',
	'dan': 'da',
	'deu': 'de',
	'div': 'dv',
	'dzo': 'dz',
	'ell': 'el',
	'eng': 'en',
	'epo': 'eo',
	'est': 'et',
	'eus': 'eu',
	'ewe': 'ee',
	'fao': 'fo',
	'fas': 'fa',
	'fij': 'fj',
	'fin': 'fi',
	'fra': 'fr',
	'fry': 'fy',
	'ful': 'ff',
	'gla': 'gd',
	'gle': 'ga',
	'glg': 'gl',
	'glv': 'gv',
	'grn': 'gn',
	'guj': 'gu',
	'hat': 'ht',
	'hau': 'ha',
	'hbs': 'sh',
	'heb': 'he',
	'her': 'hz',
	'hin': 'hi',
	'hmo': 'ho',
	'hrv': 'hr',
	'hun': 'hu',
	'hye': 'hy',
	'ibo': 'ig',
	'ido': 'io',
	'iii': 'ii',
	'iku': 'iu',
	'ile': 'ie',
	'ina': 'ia',
	'ind': 'id',
	'ipk': 'ik',
	'isl': 'is',
	'ita': 'it',
	'jav': 'jv',
	'jpn': 'ja',
	'kal': 'kl',
	'kan': 'kn',
	'kas': 'ks',
	'kat': 'ka',
	'kau': 'kr',
	'kaz': 'kk',
	'khm': 'km',
	'kik': 'ki',
	'kin': 'rw',
	'kir': 'ky',
	'kom': 'kv',
	'kon': 'kg',
	'kor': 'ko',
	'kua': 'kj',
	'kur': 'ku',
	'lao': 'lo',
	'lat': 'la',
	'lav': 'lv',
	'lim': 'li',
	'lin': 'ln',
	'lit': 'lt',
	'ltz': 'lb',
	'lub': 'lu',
	'lug': 'lg',
	'mah': 'mh',
	'mal': 'ml',
	'mar': 'mr',
	'mkd': 'mk',
	'mlg': 'mg',
	'mlt': 'mt',
	'mol': 'mo',
	'mon': 'mn',
	'mri': 'mi',
	'msa': 'ms',
	'mya': 'my',
	'nau': 'na',
	'nav': 'nv',
	'nbl': 'nr',
	'nde': 'nd',
	'ndo': 'ng',
	'nep': 'ne',
	'nld': 'nl',
	'nno': 'nn',
	'nob': 'nb',
	'nor': 'no',
	'nya': 'ny',
	'oci': 'oc',
	'oji': 'oj',
	'ori': 'or',
	'orm': 'om',
	'oss': 'os',
	'pan': 'pa',
	'pli': 'pi',
	'pol': 'pl',
	'por': 'pt',
	'pus': 'ps',
	'que': 'qu',
	'roh': 'rm',
	'ron': 'ro',
	'run': 'rn',
	'rus': 'ru',
	'sag': 'sg',
	'san': 'sa',
	'sin': 'si',
	'slk': 'sk',
	'slv': 'sl',
	'sme': 'se',
	'smo': 'sm',
	'sna': 'sn',
	'snd': 'sd',
	'som': 'so',
	'sot': 'st',
	'spa': 'es',
	'sqi': 'sq',
	'srd': 'sc',
	'srp': 'sr',
	'ssw': 'ss',
	'sun': 'su',
	'swa': 'sw',
	'swe': 'sv',
	'tah': 'ty',
	'tam': 'ta',
	'tat': 'tt',
	'tel': 'te',
	'tgk': 'tg',
	'tgl': 'tl',
	'tha': 'th',
	'tir': 'ti',
	'ton': 'to',
	'tsn': 'tn',
	'tso': 'ts',
	'tuk': 'tk',
	'tur': 'tr',
	'twi': 'tw',
	'uig': 'ug',
	'ukr': 'uk',
	'urd': 'ur',
	'uzb': 'uz',
	'ven': 've',
	'vie': 'vi',
	'vol': 'vo',
	'wln': 'wa',
	'wol': 'wo',
	'xho': 'xh',
	'yid': 'yi',
	'yor': 'yo',
	'zha': 'za',
	'zho': 'zh',
	'zul': 'zu',
}

صنف Language_ٽيگ (object):
	"""A BCP 47 language tag.

	Attributes:
		subtags (List[str]): The list of subtags in this tag.
		grandfathered (bool): Whether this tag is grandfathered. If
			``true``, the entire lowercased tag is the ``language``
			and the other subtag fields are empty.
		language (str): The language subtag.
		script (str): The script subtag.
		region (str): The region subtag.
		variant (str): The variant subtag.

	Args:
		tag (str): A BCP 47 language tag.

	"""
	وصف __init__ (self, ٽيگ_2):
		عام bcp_47
		self.subtags = ٽيگ_2.هيٺاهون ().ورهايو ('-')
		self.grandfathered = ٽيگ_2.هيٺاهون () في bcp_47.grandfathered
		إذا self.grandfathered:
			self.language = ٽيگ_2.هيٺاهون ()
			self.script = ''
			self.region = ''
			self.variant = ''
		وإلا:
			self.language = self.subtags[0]
			self.script = self.ڳوليو_پهريون (مجهول s: ڊيگهه (s) == 4 و s[0] > '9', self.subtags)
			self.region = self.ڳوليو_پهريون (مجهول s: ڊيگهه (s) == 2 و s[0] > '9' أو ڊيگهه (s) == 3 و s[0] <= '9', self.subtags[1:])
			self.variant = self.ڳوليو_پهريون (مجهول s: ڊيگهه (s) > 4 أو ڊيگهه (s) == 4 و s[0] <= '9', self.subtags)

	وصف __str__(self):
		أرجع '-'.join(self.subtags)

	وصف __repr__ (self):
		أرجع 'LanguageTag(%r)' % str(self)

	@staticmethod
	وصف ڳوليو_پهريون (function, sequence):
		حاول:
			أرجع ايندڙ (iter (filter (function, sequence)))
		استثناء StopIteration:
			أرجع ڪجهه_نه

	وصف is_complex (self):
		"""Return whether this tag is too complex to represent as a
		``LangTag`` in the generated code.

		Complex tags need to be handled in
		``hb_ot_tags_from_complex_language``.

		Returns:
			Whether this tag is complex.
		"""
		أرجع ليس (ڊيگهه (self.subtags) == 1
			أو self.grandfathered
			و ڊيگهه (self.subtags[1]) != 3
			و ot.from_bcp_47[self.subtags[0]] == ot.from_bcp_47[self.language])

	وصف get_گروهه (self):
		"""Return the group into which this tag should be categorized in
		``hb_ot_tags_from_complex_language``.

		The group is the first letter of the tag, or ``'und'`` if this tag
		should not be matched in a ``switch`` statement in the generated
		code.

		Returns:
			This tag's group.
		"""
		أرجع ('und'
			إذا (self.language == 'und'
				أو self.variant في bcp_47.prefixes و ڊيگهه (bcp_47.prefixes[self.variant]) == 1)
			وإلا self.language[0])

صنف OpenTypeRegistryParser (HTMLParser):
	"""A parser for the OpenType language system tag registry.

	Attributes:
		header (str): The "last updated" line of the registry.
		names (Mapping[str, str]): A map of language system tags to the
			names they are given in the registry.
		ranks (DefaultDict[str, int]): A map of language system tags to
			numbers. If a single BCP 47 tag corresponds to multiple
			OpenType tags, the tags are ordered in increasing order by
			rank. The rank is based on the number of BCP 47 tags
			associated with a tag, though it may be manually modified.
		to_bcp_47 (DefaultDict[str, AbstractSet[str]]): A map of
			OpenType language system tags to sets of BCP 47 tags.
		from_bcp_47 (DefaultDict[str, AbstractSet[str]]): ``to_bcp_47``
			inverted. Its values start as unsorted sets;
			``sort_languages`` converts them to sorted lists.
		from_bcp_47_uninherited (Optional[Dict[str, AbstractSet[str]]]):
			A copy of ``from_bcp_47``. It starts as ``None`` and is
			populated at the beginning of the first call to
			``inherit_from_macrolanguages``.

	"""
	وصف __init__ (self):
		HTMLParser.__init__ (self)
		self.سرو = ''
		self.نالا = {}
		self.ranks = collections.defaultdict (صحيح)
		self.منزل_bcp_47 = collections.defaultdict (سيٽ)
		self.from_bcp_47 = collections.defaultdict (سيٽ)
		self.from_bcp_47_uninherited = ڪجهه_نه
		# Whether the parser is in a <td> element
		self._td = خطأ
		# Whether the parser ignores the rest of the current <td> element
		self._disengaged = خطأ
		# The text of the <td> elements of the current <tr> element.
		self._current_tr = []

	وصف handle_starttag (self, ٽيگ_2, attrs):
		إذا ٽيگ_2 == 'a':
			إذا self._current_tr و ليس self._disengaged:
				self._current_tr[-1] = ''
				self._disengaged = سچ
		وإلاإذا ٽيگ_2 == 'br':
			self._disengaged = سچ
		وإلاإذا ٽيگ_2 == 'meta':
			لكل attr, قدر_2 في attrs:
				إذا attr == 'name' و قدر_2 == 'updated_at':
					self.سرو = self.get_starttag_متن ()
					اكسر
		وإلاإذا ٽيگ_2 == 'td':
			self._td = سچ
			self._current_tr.append ('')
		وإلاإذا ٽيگ_2 == 'tr':
			self._disengaged = خطأ
			self._current_tr = []

	وصف handle_endtag (self, ٽيگ_2):
		إذا ٽيگ_2 == 'td':
			self._td = خطأ
			self._disengaged = خطأ
		وإلاإذا ٽيگ_2 == 'tr' و self._current_tr:
			expect (2 <= ڊيگهه (self._current_tr) <= 3)
			name = self._current_tr[0].strip ()
			ٽيگ_2 = self._current_tr[1].strip ("\t\n\v\f\r '")
			rank = 0
			إذا ڊيگهه (ٽيگ_2) > 4:
				expect (ٽيگ_2.endswith (' (deprecated)'), 'ill-formed OpenType tag: %s' % ٽيگ_2)
				name += ' (deprecated)'
				ٽيگ_2 = ٽيگ_2.ورهايو (' ')[0]
				rank = 1
			self.نالا[ٽيگ_2] = re.sub (' languages$', '', name)
			إذا ليس self._current_tr[2]:
				أرجع
			iso_codes = self._current_tr[2].strip ()
			self.منزل_bcp_47[ٽيگ_2].update (ISO_639_3_منزل_1.get (ڪوڊ, ڪوڊ) لكل ڪوڊ في iso_codes.replace (' ', '').ورهايو (','))
			rank += 2 * ڊيگهه (self.منزل_bcp_47[ٽيگ_2])
			self.ranks[ٽيگ_2] = rank

	وصف handle_معلومات (self, معلومات):
		إذا self._td و ليس self._disengaged:
			self._current_tr[-1] += معلومات

	وصف handle_charref (self, name):
		self.handle_معلومات (html.unescape ('&#%s;' % name))

	وصف handle_entityref (self, name):
		self.handle_معلومات (html.unescape ('&%s;' % name))

	وصف parse (self, filename):
		"""Parse the OpenType language system tag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		سان open (filename, encoding='utf-8') كـ f:
			self.feed (f.اقرأ ())
		expect (self.سرو)
		لكل ٽيگ_2, iso_codes في self.منزل_bcp_47.شيون ():
			لكل iso_ڪوڊ في iso_codes:
				self.from_bcp_47[iso_ڪوڊ].جوڙيو (ٽيگ_2)

	وصف جوڙيو_language (self, bcp_47_ٽيگ, ot_ٽيگ):
		"""Add a language as if it were in the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag. If the tag is more than just
				a language subtag, and if the language subtag is a
				macrolanguage, then new languages are added corresponding
				to the macrolanguages' individual languages with the
				remainder of the tag appended.
			ot_tag (str): An OpenType language system tag.
		"""
		عام bcp_47
		self.منزل_bcp_47[ot_ٽيگ].جوڙيو (bcp_47_ٽيگ)
		self.from_bcp_47[bcp_47_ٽيگ].جوڙيو (ot_ٽيگ)
		إذا bcp_47_ٽيگ.هيٺاهون () ليس في bcp_47.grandfathered:
			حاول:
				[macrolanguage, suffix] = bcp_47_ٽيگ.ورهايو ('-', 1)
				إذا macrolanguage في bcp_47.macrolanguages:
					s = سيٽ ()
					لكل language في bcp_47.macrolanguages[macrolanguage]:
						إذا language.هيٺاهون () ليس في bcp_47.grandfathered:
							s.جوڙيو ('%s-%s' % (language, suffix))
					bcp_47.macrolanguages['%s-%s' % (macrolanguage, suffix)] = s
			استثناء قدر_Error:
				لنگهه

	@staticmethod
	وصف _remove_language (ٽيگ_1, dict_1, dict_2):
		لكل ٽيگ_2_2 في dict_1.ڪڍو (ٽيگ_1):
			dict_2[ٽيگ_2_2].remove (ٽيگ_1)
			إذا ليس dict_2[ٽيگ_2_2]:
				هٽايو dict_2[ٽيگ_2_2]

	وصف remove_language_ot (self, ot_ٽيگ):
		"""Remove an OpenType tag from the registry.

		Args:
			ot_tag (str): An OpenType tag.
		"""
		self._remove_language (ot_ٽيگ, self.منزل_bcp_47, self.from_bcp_47)

	وصف remove_language_bcp_47 (self, bcp_47_ٽيگ):
		"""Remove a BCP 47 tag from the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag.
		"""
		self._remove_language (bcp_47_ٽيگ, self.from_bcp_47, self.منزل_bcp_47)

	وصف inherit_from_macrolanguages (self):
		"""Copy mappings from macrolanguages to individual languages.

		If a BCP 47 tag for an individual mapping has no OpenType
		mapping but its macrolanguage does, the mapping is copied to
		the individual language. For example, als (Tosk Albanian) has no
		explicit mapping, so it inherits from sq (Albanian) the mapping
		to SQI.

		However, if an OpenType tag maps to a BCP 47 macrolanguage and
		some but not all of its individual languages, the mapping is not
		inherited from the macrolanguage to the missing individual
		languages. For example, INUK (Nunavik Inuktitut) is mapped to
		ike (Eastern Canadian Inuktitut) and iu (Inuktitut) but not to
		ikt (Inuinnaqtun, which is an individual language of iu), so
		this method does not add a mapping from ikt to INUK.

		If a BCP 47 tag for a macrolanguage has no OpenType mapping but
		some of its individual languages do, their mappings are copied
		to the macrolanguage.
		"""
		عام bcp_47
		پهريون_وقت = self.from_bcp_47_uninherited آهي ڪجهه_نه
		إذا پهريون_وقت:
			self.from_bcp_47_uninherited = dict (self.from_bcp_47)
		لكل macrolanguage, languages في dict (bcp_47.macrolanguages).شيون ():
			ot_macrolanguages = {
				ot_macrolanguage لكل ot_macrolanguage في self.from_bcp_47_uninherited.get (macrolanguage, سيٽ ())
			}
			blocked_ot_macrolanguages = سيٽ ()
			إذا 'retired code' ليس في bcp_47.scopes.get (macrolanguage, ''):
				لكل ot_macrolanguage في ot_macrolanguages:
					ڦيرو_trip_macrolanguages = {
						l لكل l في self.منزل_bcp_47[ot_macrolanguage]
						إذا 'retired code' ليس في bcp_47.scopes.get (l, '')
					}
					ڦيرو_trip_languages = {
						l لكل l في languages
						إذا 'retired code' ليس في bcp_47.scopes.get (l, '')
					}
					intersection = ڦيرو_trip_macrolanguages & ڦيرو_trip_languages
					إذا intersection و intersection != ڦيرو_trip_languages:
						blocked_ot_macrolanguages.جوڙيو (ot_macrolanguage)
			إذا ot_macrolanguages:
				لكل ot_macrolanguage في ot_macrolanguages:
					إذا ot_macrolanguage ليس في blocked_ot_macrolanguages:
						لكل language في languages:
							self.جوڙيو_language (language, ot_macrolanguage)
							إذا ليس blocked_ot_macrolanguages:
								self.ranks[ot_macrolanguage] += 1
			وإلاإذا پهريون_وقت:
				لكل language في languages:
					إذا language في self.from_bcp_47_uninherited:
						ot_macrolanguages |= self.from_bcp_47_uninherited[language]
					وإلا:
						ot_macrolanguages.clear ()
					إذا ليس ot_macrolanguages:
						اكسر
				لكل ot_macrolanguage في ot_macrolanguages:
					self.جوڙيو_language (macrolanguage, ot_macrolanguage)

	وصف ترتيب_ڏيو_languages (self):
		"""Sort the values of ``from_bcp_47`` in ascending rank order."""
		لكل language, ٽيگ_فهرست في self.from_bcp_47.شيون ():
			self.from_bcp_47[language] = sorted (ٽيگ_فهرست,
					ڪنجي_2=مجهول t: (self.ranks[t] + rank_delta (language, t), t))

ot = OpenTypeRegistryParser ()

صنف BCP47Parser (object):
	"""A parser for the BCP 47 subtag registry.

	Attributes:
		header (str): The "File-Date" line of the registry.
		names (Mapping[str, str]): A map of subtags to the names they
			are given in the registry. Each value is a
			``'\\n'``-separated list of names.
		scopes (Mapping[str, str]): A map of language subtags to strings
			suffixed to language names, including suffixes to explain
			language scopes.
		macrolanguages (DefaultDict[str, AbstractSet[str]]): A map of
			language subtags to the sets of language subtags which
			inherit from them. See
			``OpenTypeRegistryParser.inherit_from_macrolanguages``.
		prefixes (DefaultDict[str, AbstractSet[str]]): A map of variant
			subtags to their prefixes.
		grandfathered (AbstractSet[str]): The set of grandfathered tags,
			normalized to lowercase.

	"""
	وصف __init__ (self):
		self.سرو = ''
		self.نالا = {}
		self.scopes = {}
		self.macrolanguages = collections.defaultdict (سيٽ)
		self.prefixes = collections.defaultdict (سيٽ)
		self.grandfathered = سيٽ ()

	وصف parse (self, filename):
		"""Parse the BCP 47 subtag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		سان open (filename, encoding='utf-8') كـ f:
			subtag_type = ڪجهه_نه
			subtag = ڪجهه_نه
			deprecated = خطأ
			has_preferred_قدر = خطأ
			سٽ_بفر = ''
			لكل سٽ في itertools.chain (f, ['']):
				سٽ = سٽ.rstrip ()
				إذا سٽ.startswith (' '):
					سٽ_بفر += سٽ[1:]
					استمر
				سٽ, سٽ_بفر = سٽ_بفر, سٽ
				إذا سٽ.startswith ('Type: '):
					subtag_type = سٽ.ورهايو (' ')[1]
					deprecated = خطأ
					has_preferred_قدر = خطأ
				وإلاإذا سٽ.startswith ('Subtag: ') أو سٽ.startswith ('Tag: '):
					subtag = سٽ.ورهايو (' ')[1]
					إذا subtag_type == 'grandfathered':
						self.grandfathered.جوڙيو (subtag.هيٺاهون ())
				وإلاإذا سٽ.startswith ('Description: '):
					description = سٽ.ورهايو (' ', 1)[1].replace (' (individual language)', '')
					description = re.sub (r' (\(family\)|\((individual |macro)language\)|languages)$', '',
							description)
					إذا subtag في self.نالا:
						self.نالا[subtag] += '\n' + description
					وإلا:
						self.نالا[subtag] = description
				وإلاإذا subtag_type == 'language' أو subtag_type == 'grandfathered':
					إذا سٽ.startswith ('Scope: '):
						scope = سٽ.ورهايو (' ')[1]
						إذا scope == 'macrolanguage':
							scope = ' [macrolanguage]'
						وإلاإذا scope == 'collection':
							scope = ' [collection]'
						وإلا:
							استمر
						self.scopes[subtag] = scope
					وإلاإذا سٽ.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
						deprecated = سچ
					وإلاإذا deprecated و سٽ.startswith ('Comments: see '):
						# If a subtag is split into multiple replacement subtags,
						# it essentially represents a macrolanguage.
						لكل language في سٽ.replace (',', '').ورهايو (' ')[2:]:
							self.جوڙيو_macrolanguage (subtag, language)
					وإلاإذا سٽ.startswith ('Preferred-Value: '):
						# If a subtag is deprecated in favor of a single replacement subtag,
						# it is either a dialect or synonym of the preferred subtag. Either
						# way, it is close enough to the truth to consider the replacement
						# the macrolanguage of the deprecated language.
						has_preferred_قدر = سچ
						macrolanguage = سٽ.ورهايو (' ')[1]
						self.جوڙيو_macrolanguage (macrolanguage, subtag)
					وإلاإذا ليس has_preferred_قدر و سٽ.startswith ('Macrolanguage: '):
						self.جوڙيو_macrolanguage (سٽ.ورهايو (' ')[1], subtag)
				وإلاإذا subtag_type == 'variant':
					إذا سٽ.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
					وإلاإذا سٽ.startswith ('Prefix: '):
						self.prefixes[subtag].جوڙيو (سٽ.ورهايو (' ')[1])
				وإلاإذا سٽ.startswith ('File-Date: '):
					self.سرو = سٽ
		expect (self.سرو)

	وصف جوڙيو_macrolanguage (self, macrolanguage, language):
		عام ot
		إذا language ليس في ot.from_bcp_47:
			لكل l في self.macrolanguages.get (language, سيٽ ()):
				self.جوڙيو_macrolanguage (macrolanguage, l)
		إذا macrolanguage ليس في ot.from_bcp_47:
			لكل ls في list (self.macrolanguages.قدرون ()):
				إذا macrolanguage في ls:
					ls.جوڙيو (language)
					أرجع
		self.macrolanguages[macrolanguage].جوڙيو (language)

	وصف remove_extra_macrolanguages (self):
		"""Make every language have at most one macrolanguage."""
		inverted = collections.defaultdict (list)
		لكل macrolanguage, languages في self.macrolanguages.شيون ():
			لكل language في languages:
				inverted[language].append (macrolanguage)
		لكل language, macrolanguages في inverted.شيون ():
			إذا ڊيگهه (macrolanguages) > 1:
				macrolanguages.ترتيب_ڏيو (ڪنجي_2=مجهول ml: ڊيگهه (self.macrolanguages[ml]))
				biggest_macrolanguage = macrolanguages.ڪڍو ()
				لكل macrolanguage في macrolanguages:
					self.جوڙيو_macrolanguage (biggest_macrolanguage, macrolanguage)

	وصف _get_name_piece (self, subtag):
		"""Return the first name of a subtag plus its scope suffix.

		Args:
			subtag (str): A BCP 47 subtag.

		Returns:
			The name form of ``subtag``.
		"""
		أرجع self.نالا[subtag].ورهايو ('\n')[0] + self.scopes.get (subtag, '')

	وصف get_name (self, lt):
		"""Return the names of the subtags in a language tag.

		Args:
			lt (LanguageTag): A BCP 47 language tag.

		Returns:
			The name form of ``lt``.
		"""
		name = self._get_name_piece (lt.language)
		إذا lt.script:
			name += '; ' + self._get_name_piece (lt.script.title ())
		إذا lt.region:
			name += '; ' + self._get_name_piece (lt.region.مٿانهون ())
		إذا lt.variant:
			name += '; ' + self._get_name_piece (lt.variant)
		أرجع name

bcp_47 = BCP47Parser ()

ot.parse (sys.argv[1])
bcp_47.parse (sys.argv[2])

ot.جوڙيو_language ('ary', 'MOR')

ot.جوڙيو_language ('ath', 'ATH')

ot.جوڙيو_language ('bai', 'BML')

ot.ranks['BAL'] = ot.ranks['KAR'] + 1

ot.جوڙيو_language ('ber', 'BBR')

ot.remove_language_ot ('PGR')
ot.جوڙيو_language ('el-polyton', 'PGR')

bcp_47.نالا['flm'] = 'Falam Chin'
bcp_47.scopes['flm'] = ' (retired code)'
bcp_47.macrolanguages['flm'] = {'cfm'}

ot.ranks['FNE'] = ot.ranks['TNE'] + 1

ot.جوڙيو_language ('und-fonipa', 'IPPH')

ot.جوڙيو_language ('und-fonnapa', 'APPH')

ot.جوڙيو_language ('und-fonupa', 'UPPH')

ot.جوڙيو_language ('ga-Latg', 'IRT')

ot.جوڙيو_language ('hy-arevmda', 'HYE')

ot.remove_language_ot ('KGE')
ot.جوڙيو_language ('und-Geok', 'KGE')

ot.جوڙيو_language ('kht', 'KHN')
ot.نالا['KHN'] = ot.نالا['KHT'] + ' (Microsoft fonts)'
ot.ranks['KHN'] = ot.ranks['KHT'] + 1

ot.ranks['LCR'] = ot.ranks['MCR'] + 1

ot.نالا['MAL'] = 'Malayalam Traditional'
ot.ranks['MLR'] += 1

bcp_47.نالا['mhv'] = 'Arakanese'
bcp_47.scopes['mhv'] = ' (retired code)'

ot.جوڙيو_language ('mnw-TH', 'MONT')

ot.جوڙيو_language ('no', 'NOR')

ot.جوڙيو_language ('oc-provenc', 'PRO')

ot.remove_language_ot ('QUZ')
ot.جوڙيو_language ('qu', 'QUZ')
ot.جوڙيو_language ('qub', 'QWH')
ot.جوڙيو_language ('qud', 'QVI')
ot.جوڙيو_language ('qug', 'QVI')
ot.جوڙيو_language ('qul', 'QUH')
ot.جوڙيو_language ('qup', 'QVI')
ot.جوڙيو_language ('qur', 'QWH')
ot.جوڙيو_language ('qus', 'QUH')
ot.جوڙيو_language ('quw', 'QVI')
ot.جوڙيو_language ('qux', 'QWH')
ot.جوڙيو_language ('qva', 'QWH')
ot.جوڙيو_language ('qvh', 'QWH')
ot.جوڙيو_language ('qvj', 'QVI')
ot.جوڙيو_language ('qvl', 'QWH')
ot.جوڙيو_language ('qvm', 'QWH')
ot.جوڙيو_language ('qvn', 'QWH')
ot.جوڙيو_language ('qvo', 'QVI')
ot.جوڙيو_language ('qvp', 'QWH')
ot.جوڙيو_language ('qvw', 'QWH')
ot.جوڙيو_language ('qvz', 'QVI')
ot.جوڙيو_language ('qwa', 'QWH')
ot.جوڙيو_language ('qws', 'QWH')
ot.جوڙيو_language ('qxa', 'QWH')
ot.جوڙيو_language ('qxc', 'QWH')
ot.جوڙيو_language ('qxh', 'QWH')
ot.جوڙيو_language ('qxl', 'QVI')
ot.جوڙيو_language ('qxn', 'QWH')
ot.جوڙيو_language ('qxo', 'QWH')
ot.جوڙيو_language ('qxr', 'QVI')
ot.جوڙيو_language ('qxt', 'QWH')
ot.جوڙيو_language ('qxw', 'QWH')

bcp_47.macrolanguages['ro-MD'].جوڙيو ('mo')

ot.remove_language_ot ('SYRE')
ot.remove_language_ot ('SYRJ')
ot.remove_language_ot ('SYRN')
ot.جوڙيو_language ('und-Syre', 'SYRE')
ot.جوڙيو_language ('und-Syrj', 'SYRJ')
ot.جوڙيو_language ('und-Syrn', 'SYRN')

bcp_47.نالا['xst'] = "Silt'e"
bcp_47.scopes['xst'] = ' (retired code)'
bcp_47.macrolanguages['xst'] = {'stv', 'wle'}

ot.جوڙيو_language ('xwo', 'TOD')

ot.remove_language_ot ('ZHH')
ot.remove_language_ot ('ZHP')
ot.remove_language_ot ('ZHT')
ot.remove_language_ot ('ZHTM')
bcp_47.macrolanguages['zh'].remove ('lzh')
bcp_47.macrolanguages['zh'].remove ('yue')
ot.جوڙيو_language ('zh-Hant-MO', 'ZHH')
ot.جوڙيو_language ('zh-Hant-MO', 'ZHTM')
ot.جوڙيو_language ('zh-Hant-HK', 'ZHH')
ot.جوڙيو_language ('zh-Hans', 'ZHS')
ot.جوڙيو_language ('zh-Hant', 'ZHT')
ot.جوڙيو_language ('zh-HK', 'ZHH')
ot.جوڙيو_language ('zh-MO', 'ZHH')
ot.جوڙيو_language ('zh-MO', 'ZHTM')
ot.جوڙيو_language ('zh-TW', 'ZHT')
ot.جوڙيو_language ('lzh', 'ZHT')
ot.جوڙيو_language ('lzh-Hans', 'ZHS')
ot.جوڙيو_language ('yue', 'ZHH')
ot.جوڙيو_language ('yue-Hans', 'ZHS')

وصف rank_delta (bcp_47, ot):
	"""Return a delta to apply to a BCP 47 tag's rank.

	Most OpenType tags have a constant rank, but a few have ranks that
	depend on the BCP 47 tag.

	Args:
		bcp_47 (str): A BCP 47 tag.
		ot (str): An OpenType tag to.

	Returns:
		A number to add to ``ot``'s rank when sorting ``bcp_47``'s
		OpenType equivalents.
	"""
	إذا bcp_47 == 'ak' و ot == 'AKA':
		أرجع -1
	إذا bcp_47 == 'tw' و ot == 'TWI':
		أرجع -1
	أرجع 0

disambiguation = {
	'ALT': 'alt',
	'ARK': 'rki',
	'ATH': 'ath',
	'BHI': 'bhb',
	'BLN': 'bjt',
	'BTI': 'beb',
	'CCHN': 'cco',
	'CMR': 'swb',
	'CPP': 'crp',
	'CRR': 'crx',
	'DUJ': 'dwu',
	'ECR': 'crj',
	'HAL': 'cfm',
	'HND': 'hnd',
	'HYE': 'hyw',
	'KIS': 'kqs',
	'KUI': 'uki',
	'LRC': 'bqi',
	'NDB': 'nd',
	'NIS': 'njz',
	'PLG': 'pce',
	'PRO': 'pro',
	'QIN': 'bgr',
	'QUH': 'quh',
	'QVI': 'qvi',
	'QWH': 'qwh',
	'SIG': 'stv',
	'SRB': 'sr',
	'SXT': 'xnj',
	'ZHH': 'zh-HK',
	'ZHS': 'zh-Hans',
	'ZHT': 'zh-Hant',
	'ZHTM': 'zh-MO',
}

ot.inherit_from_macrolanguages ()
bcp_47.remove_extra_macrolanguages ()
ot.inherit_from_macrolanguages ()
ot.نالا[DEFAULT_LANGUAGE_SYSTEM] = ''
ot.ranks[DEFAULT_LANGUAGE_SYSTEM] = max (ot.ranks.قدرون ()) + 1
لكل tricky_ot_ٽيگ في filter (مجهول ٽيگ_2: re.ميلاپ ('[A-Z]{3}$', ٽيگ_2), ot.نالا):
	possible_bcp_47_ٽيگ = tricky_ot_ٽيگ.هيٺاهون ()
	إذا possible_bcp_47_ٽيگ في bcp_47.نالا و ليس ot.from_bcp_47[possible_bcp_47_ٽيگ]:
		ot.جوڙيو_language (possible_bcp_47_ٽيگ, DEFAULT_LANGUAGE_SYSTEM)
		bcp_47.macrolanguages[possible_bcp_47_ٽيگ] = سيٽ ()
ot.ترتيب_ڏيو_languages ()

إذا output_rust:
	وصف hb_ٽيگ_rust (ٽيگ_2):
		إذا ٽيگ_2 == DEFAULT_LANGUAGE_SYSTEM:
			أرجع 'Tag::new(&[0; 4])'
		sanitize_ٽيگ (ٽيگ_2)
		أرجع 'Tag::new(b"%s%s%s%s")' % tuple (('%-4s' % ٽيگ_2)[:4])

	وصف get_variant_سيٽ_rust (name):
		أرجع سيٽ (unicodedata.normalize (
					'NFD',
					n.replace ('\u02BC', "'").replace ('\u2019', "'"),
				)
				.encode ('ASCII', 'ignore')
				.strip ()
				لكل n في re.ورهايو ('[\n(),]', name) إذا n)

	وصف language_name_intersection_rust (a, b):
		أرجع get_variant_سيٽ_rust (a).intersection (get_variant_سيٽ_rust (b))

	وصف get_matching_language_name_rust (intersection, candidates):
		أرجع ايندڙ (iter (c لكل c في candidates إذا ليس intersection.isdisjoint (get_variant_سيٽ_rust (c))))

	ڇاپيو ('// WARNING: this file was generated by ../../harfbuzz/src/gen-tag-table.py --rust')
	ڇاپيو ()
	ڇاپيو ('use read_fonts::types::Tag;')
	ڇاپيو ()
	ڇاپيو ('pub(crate) struct LangTag {')
	ڇاپيو ('    pub language: [u8; 4],')
	ڇاپيو ('    pub tag: Tag,')
	ڇاپيو ('}')
	ڇاپيو ()
	ڇاپيو ('const fn lang(language: &[u8]) -> [u8; 4] {')
	ڇاپيو ('    let mut bytes = [0; 4];')
	ڇاپيو ('    let mut i = 0;')
	ڇاپيو ()
	ڇاپيو ('    while i < language.len() {')
	ڇاپيو ('        bytes[i] = language[i];')
	ڇاپيو ('        i += 1;')
	ڇاپيو ('    }')
	ڇاپيو ()
	ڇاپيو ('    bytes')
	ڇاپيو ('}')
	ڇاپيو ()
	ڇاپيو ('use super::tag::{lang_matches, strncmp, subtag_matches};')
	ڇاپيو ()
	ڇاپيو ('#[rustfmt::skip]')
	ڇاپيو ('pub(crate) static OPEN_TYPE_LANGUAGES: &[LangTag] = &[')

	لكل language, ٽيگ_فهرست في sorted (ot.from_bcp_47.شيون ()):
		إذا language == '' أو '-' في language:
			استمر
		commented_ٻاهريون = ڊيگهه (language) == 3 و ڊيگهه (ٽيگ_فهرست) == 1 و language == ٽيگ_فهرست[0].هيٺاهون ()
		لكل ٽيگ_2 في ٽيگ_فهرست:
			ڇاپيو ('%sLangTag { language: lang(b"%s"), \ttag: %s },' % ('//  ' إذا commented_ٻاهريون وإلا '    ', sanitize_bcp47_ٽيگ (language), hb_ٽيگ_rust (ٽيگ_2)), پڇاڙي='')
			ڇاپيو (' // ', پڇاڙي='')
			bcp_47_name = bcp_47.نالا.get (language, '')
			bcp_47_name_candidates = bcp_47_name.ورهايو ('\n')
			ot_name = ot.نالا[ٽيگ_2]
			scope = bcp_47.scopes.get (language, '')
			إذا ٽيگ_2 == DEFAULT_LANGUAGE_SYSTEM:
				comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
			وإلا:
				intersection = language_name_intersection_rust (bcp_47_name, ot_name)
				إذا ليس intersection:
					comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
				وإلا:
					name = get_matching_language_name_rust (intersection, bcp_47_name_candidates)
					bcp_47.نالا[language] = name
					comment = '%s%s' % (name إذا ڊيگهه (name) > ڊيگهه (ot_name) وإلا ot_name, scope)
			ڇاپيو (sanitize_comment (comment))

	ڇاپيو ('];')
	ڇاپيو ()
	ڇاپيو ('/// Converts a multi-subtag BCP 47 language tag to language tags.')
	ڇاپيو ('pub fn tags_from_complex_language(language: &str, tags: &mut smallvec::SmallVec<[Tag; 3]>) -> bool {')

	وصف print_subtag_matches_rust (subtag, new_سٽ):
		إذا subtag:
			إذا new_سٽ:
				ڇاپيو (' && ', پڇاڙي='')
			ڇاپيو ('subtag_matches(language, "-%s")' % sanitize_bcp47_ٽيگ (subtag), پڇاڙي='')

	complex_ٽيگ_فهرست = collections.defaultdict (list)
	لكل initial, گروهه في itertools.groupby ((lt_ٽيگ_فهرست لكل lt_ٽيگ_فهرست في [
				(Language_ٽيگ (language), ٽيگ_فهرست)
				لكل language, ٽيگ_فهرست في sorted (ot.from_bcp_47.شيون (),
					ڪنجي_2=مجهول i: (-ڊيگهه (i[0]), i[0]))
			] إذا lt_ٽيگ_فهرست[0].is_complex ()),
			ڪنجي_2=مجهول lt_ٽيگ_فهرست: lt_ٽيگ_فهرست[0].get_گروهه ()):
		complex_ٽيگ_فهرست[initial] += گروهه

	لكل initial, شيون في sorted (complex_ٽيگ_فهرست.شيون ()):
		إذا initial != 'und':
			استمر
		لكل lt, ٽيگ_فهرست في شيون:
			إذا ليس ٽيگ_فهرست:
				استمر
			إذا lt.variant في bcp_47.prefixes:
				expect (ايندڙ (iter (bcp_47.prefixes[lt.variant])) == lt.language,
						'%s is not a valid prefix of %s' % (lt.language, lt.variant))
			ڇاپيو ('    if ', پڇاڙي='')
			print_subtag_matches_rust (lt.script, خطأ)
			print_subtag_matches_rust (lt.region, خطأ)
			print_subtag_matches_rust (lt.variant, خطأ)
			ڇاپيو (' {')
			ڇاپيو ('        // %s' % sanitize_comment (bcp_47.get_name (lt)))
			إذا ڊيگهه (ٽيگ_فهرست) == 1:
				ڇاپيو ('        tags.push(%s); // %s' % (hb_ٽيگ_rust (ٽيگ_فهرست[0]), sanitize_comment (ot.نالا[ٽيگ_فهرست[0]])))
			وإلا:
				ڇاپيو ('        let possible_tags = &[')
				لكل ٽيگ_2 في ٽيگ_فهرست:
					ڇاپيو ('            %s, // %s' % (hb_ٽيگ_rust (ٽيگ_2), sanitize_comment (ot.نالا[ٽيگ_2])))
				ڇاپيو ('        ];')
				ڇاپيو ('        tags.extend_from_slice(possible_tags);')
			ڇاپيو ('        return true;')
			ڇاپيو ('    }')

	ڇاپيو ('    match language.as_bytes()[0] {')
	لكل initial, شيون في sorted (complex_ٽيگ_فهرست.شيون ()):
		إذا initial == 'und':
			استمر
		ڇاپيو ("        b'%s' => {" % sanitize_bcp47_ٽيگ (initial))
		لكل lt, ٽيگ_فهرست في شيون:
			إذا ليس ٽيگ_فهرست:
				استمر
			ڇاپيو ('            if ', پڇاڙي='')
			script = lt.script
			region = lt.region
			إذا lt.grandfathered:
				ڇاپيو ('&language[1..] == "%s"' % sanitize_bcp47_ٽيگ (lt.language[1:]), پڇاڙي='')
			وإلا:
				string_literal = lt.language[1:] + '-'
				إذا script:
					string_literal += script
					script = ڪجهه_نه
					إذا region:
						string_literal += '-' + region
						region = ڪجهه_نه
				إذا string_literal[-1] == '-':
					ڇاپيو ('strncmp(&language[1..], "%s", %i)' % (sanitize_bcp47_ٽيگ (string_literal), ڊيگهه (string_literal)), پڇاڙي='')
				وإلا:
					ڇاپيو ('lang_matches(&language[1..], "%s")' % sanitize_bcp47_ٽيگ (string_literal), پڇاڙي='')
			print_subtag_matches_rust (script, سچ)
			print_subtag_matches_rust (region, سچ)
			print_subtag_matches_rust (lt.variant, سچ)
			ڇاپيو (' {')
			ڇاپيو ('                // %s' % sanitize_comment (bcp_47.get_name (lt)))
			إذا ڊيگهه (ٽيگ_فهرست) == 1:
				ڇاپيو ('                tags.push(%s); // %s' % (hb_ٽيگ_rust (ٽيگ_فهرست[0]), sanitize_comment (ot.نالا[ٽيگ_فهرست[0]])))
			وإلا:
				ڇاپيو ('                let possible_tags = &[')
				لكل ٽيگ_2 في ٽيگ_فهرست:
					ڇاپيو ('                    %s, // %s' % (hb_ٽيگ_rust (ٽيگ_2), sanitize_comment (ot.نالا[ٽيگ_2])))
				ڇاپيو ('                ];')
				ڇاپيو ('                tags.extend_from_slice(possible_tags);')
			ڇاپيو ('                return true;')
			ڇاپيو ('            }')
		ڇاپيو ('        }')
	ڇاپيو ('        _ => {}')
	ڇاپيو ('    }')
	ڇاپيو ('    false')
	ڇاپيو ('}')
	sys.exit (0)

ڇاپيو ('/* == Start of generated table == */')
ڇاپيو ('/*')
ڇاپيو (' * The following table is generated by running:')
ڇاپيو (' *')
ڇاپيو (' *   %s languagetags language-subtag-registry' % os.رستو.basename (sys.argv[0]))
ڇاپيو (' *')
ڇاپيو (' * on files with these headers:')
ڇاپيو (' *')
ڇاپيو (' * %s' % sanitize_سرو (ot.سرو))
ڇاپيو (' * %s' % sanitize_سرو (bcp_47.سرو))
ڇاپيو (' */')
ڇاپيو ()
ڇاپيو ('#ifndef HB_OT_TAG_TABLE_HH')
ڇاپيو ('#define HB_OT_TAG_TABLE_HH')
ڇاپيو ()

وصف hb_ٽيگ (ٽيگ_2):
	"""Convert a tag to ``HB_TAG`` form.

	Args:
		tag (str): An OpenType tag.

	Returns:
		A snippet of C++ representing ``tag``.
	"""
	إذا ٽيگ_2 == DEFAULT_LANGUAGE_SYSTEM:
		أرجع 'HB_TAG_NONE\t       '
	sanitize_ٽيگ (ٽيگ_2)
	أرجع "HB_TAG('%s','%s','%s','%s')" % tuple (('%-4s' % ٽيگ_2)[:4])

وصف get_variant_سيٽ (name):
	"""Return a set of variant language names from a name.

	Args:
		name (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		A set of normalized language names.
	"""
	أرجع سيٽ (unicodedata.normalize (
				'NFD',
				n.replace ('\u02BC', "'").replace ('\u2019', "'"),
			)
			.encode ('ASCII', 'ignore')
			.strip ()
			لكل n في re.ورهايو ('[\n(),]', name) إذا n)

وصف language_name_intersection (a, b):
	"""Return the names in common between two language names.

	Args:
		a (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.
		b (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		The normalized language names shared by ``a`` and ``b``.
	"""
	أرجع get_variant_سيٽ (a).intersection (get_variant_سيٽ (b))

وصف get_matching_language_name (intersection, candidates):
	أرجع ايندڙ (iter (c لكل c في candidates إذا ليس intersection.isdisjoint (get_variant_سيٽ (c))))

وصف same_ٽيگ (bcp_47_ٽيگ, ot_ٽيگ_فهرست):
	أرجع ڊيگهه (bcp_47_ٽيگ) == 3 و ڊيگهه (ot_ٽيگ_فهرست) == 1 و bcp_47_ٽيگ == ot_ٽيگ_فهرست[0].هيٺاهون ()

وصف format_language_comment (language, ٽيگ_2):
	bcp_47_name = bcp_47.نالا.get (language, '')
	bcp_47_name_candidates = bcp_47_name.ورهايو ('\n')
	ot_name = ot.نالا[ٽيگ_2]
	scope = bcp_47.scopes.get (language, '')
	إذا ٽيگ_2 == DEFAULT_LANGUAGE_SYSTEM:
		comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
	وإلا:
		intersection = language_name_intersection (bcp_47_name, ot_name)
		إذا ليس intersection:
			comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
		وإلا:
			name = get_matching_language_name (intersection, bcp_47_name_candidates)
			bcp_47.نالا[language] = name
			comment = '%s%s' % (name إذا ڊيگهه (name) > ڊيگهه (ot_name) وإلا ot_name, scope)
	أرجع sanitize_comment (comment)

ڇاپيو ('static const LangTag ot_languages2[] = {')
لكل language, ٽيگ_فهرست في sorted (ot.from_bcp_47.شيون ()):
	إذا language == '' أو '-' في language أو ڊيگهه (language) != 2:
		استمر
	لكل ٽيگ_2 في ٽيگ_فهرست:
		ڇاپيو ('  {%s,\t%s},\t/* %s */' % (
			hb_ٽيگ (language),
			hb_ٽيگ (ٽيگ_2),
			format_language_comment (language, ٽيگ_2)))
ڇاپيو ('};')
ڇاپيو ()

ڇاپيو ('#ifndef HB_NO_LANGUAGE_LONG')

languages3_blocked = []
languages3_singles = []
languages3_multi_ranges = []
languages3_multi_قدرون = []
لكل language, ٽيگ_فهرست في sorted (ot.from_bcp_47.شيون ()):
	إذا language == '' أو '-' في language أو ڊيگهه (language) != 3:
		استمر
	إذا same_ٽيگ (language, ٽيگ_فهرست):
		استمر
	إذا ٽيگ_فهرست == [DEFAULT_LANGUAGE_SYSTEM]:
		languages3_blocked.append ((language, ٽيگ_فهرست[0]))
		استمر
	إذا ڊيگهه (ٽيگ_فهرست) == 1:
		languages3_singles.append ((language, ٽيگ_فهرست[0]))
		استمر
	وٿي = ڊيگهه (languages3_multi_قدرون)
	languages3_multi_قدرون.extend ((language, ٽيگ_2) لكل ٽيگ_2 في ٽيگ_فهرست)
	languages3_multi_ranges.append ((language, وٿي, ڊيگهه (ٽيگ_فهرست), ٽيگ_فهرست[0]))

ڇاپيو ('static const hb_tag_t ot_languages3_blocked[] = {')
لكل language, ٽيگ_2 في languages3_blocked:
	ڇاپيو ('  %s,\t/* %s */' % (hb_ٽيگ (language), format_language_comment (language, ٽيگ_2)))
ڇاپيو ('};')
ڇاپيو ()

ڇاپيو ('static const LangTag ot_languages3[] = {')
لكل language, ٽيگ_2 في languages3_singles:
	ڇاپيو ('  {%s,\t%s},\t/* %s */' % (
		hb_ٽيگ (language),
		hb_ٽيگ (ٽيگ_2),
		format_language_comment (language, ٽيگ_2)))
ڇاپيو ('};')
ڇاپيو ()

ڇاپيو ('static const hb_tag_t ot_languages3_multi_values[] = {')
لكل language, ٽيگ_2 في languages3_multi_قدرون:
	ڇاپيو ('  %s,\t/* %s */' % (hb_ٽيگ (ٽيگ_2), format_language_comment (language, ٽيگ_2)))
ڇاپيو ('};')
ڇاپيو ()

ڇاپيو ('static const LangTagRange ot_languages3_multi[] = {')
لكل language, وٿي, ڳڻپ, پهريون_ٽيگ في languages3_multi_ranges:
	ڇاپيو ('  {%s,\t%u,\t%u},\t/* %s */' % (
		hb_ٽيگ (language),
		صحيح(وٿي),
		صحيح(ڳڻپ),
		format_language_comment (language, پهريون_ٽيگ)))
ڇاپيو ('};')
ڇاپيو ('#endif')
ڇاپيو ()

ڇاپيو ('/**')
ڇاپيو (' * hb_ot_tags_from_complex_language:')
ڇاپيو (' * @lang_str: a BCP 47 language tag to convert.')
ڇاپيو (' * @limit: a pointer to the end of the substring of @lang_str to consider for')
ڇاپيو (' * conversion.')
ڇاپيو (' * @count: maximum number of language tags to retrieve (IN) and actual number of')
ڇاپيو (' * language tags retrieved (OUT). If no tags are retrieved, it is not modified.')
ڇاپيو (' * @tags: array of size at least @language_count to store the language tag')
ڇاپيو (' * results')
ڇاپيو (' *')
ڇاپيو (' * Converts a multi-subtag BCP 47 language tag to language tags.')
ڇاپيو (' *')
ڇاپيو (' * Return value: Whether any language systems were retrieved.')
ڇاپيو (' **/')
ڇاپيو ('static inline bool')
ڇاپيو ('hb_ot_tags_from_complex_language (const char   *lang_str,')
ڇاپيو ('\t\t\t\t  const char   *limit,')
ڇاپيو ('\t\t\t\t  unsigned int *count /* IN/OUT */,')
ڇاپيو ('\t\t\t\t  hb_tag_t     *tags /* OUT */)')
ڇاپيو ('{')

وصف print_subtag_matches (subtag, string, new_سٽ):
	إذا subtag:
		إذا new_سٽ:
			ڇاپيو ()
			ڇاپيو ('\t&& ', پڇاڙي='')
		ڇاپيو ('subtag_matches (%s, limit, "-%s", %i)' % (string, sanitize_bcp47_ٽيگ (subtag), 1 + ڊيگهه (subtag)), پڇاڙي='')

complex_ٽيگ_فهرست = collections.defaultdict (list)
لكل initial, گروهه في itertools.groupby ((lt_ٽيگ_فهرست لكل lt_ٽيگ_فهرست في [
			(Language_ٽيگ (language), ٽيگ_فهرست)
			لكل language, ٽيگ_فهرست في sorted (ot.from_bcp_47.شيون (),
				ڪنجي_2=مجهول i: (-ڊيگهه (i[0]), i[0]))
		] إذا lt_ٽيگ_فهرست[0].is_complex ()),
		ڪنجي_2=مجهول lt_ٽيگ_فهرست: lt_ٽيگ_فهرست[0].get_گروهه ()):
	complex_ٽيگ_فهرست[initial] += گروهه

# Calculate the min length of the subtags outside the switch
min_subtag_len = 100
لكل initial, شيون في sorted (complex_ٽيگ_فهرست.شيون ()):
	إذا initial != 'und':
		استمر
	لكل lt, ٽيگ_فهرست في شيون:
		إذا ليس ٽيگ_فهرست:
			استمر
		subtag_len = 0
		subtag_len += 1 + ڊيگهه (lt.script) إذا lt.script آهي ليس ڪجهه_نه وإلا 0
		subtag_len += 1 + ڊيگهه (lt.region) إذا lt.region آهي ليس ڪجهه_نه وإلا 0
		subtag_len += 1 + ڊيگهه (lt.variant) إذا lt.variant آهي ليس ڪجهه_نه وإلا 0
		min_subtag_len = min(subtag_len, min_subtag_len)

ڇاپيو ('  if (limit - lang_str >= %d)' % (min_subtag_len + 2))
ڇاپيو ('  {')
ڇاپيو ("    const char *p = strchr (lang_str, '-');")
ڇاپيو ("    if (!p || p >= limit || limit - p < %i) goto out;" % min_subtag_len)
لكل initial, شيون في sorted (complex_ٽيگ_فهرست.شيون ()):
	إذا initial != 'und':
		استمر
	لكل lt, ٽيگ_فهرست في شيون:
		إذا ليس ٽيگ_فهرست:
			استمر
		إذا lt.variant في bcp_47.prefixes:
			expect (ايندڙ (iter (bcp_47.prefixes[lt.variant])) == lt.language,
					'%s is not a valid prefix of %s' % (lt.language, lt.variant))
		ڇاپيو ('    if (', پڇاڙي='')
		print_subtag_matches (lt.script, 'p', خطأ)
		print_subtag_matches (lt.region, 'p', خطأ)
		print_subtag_matches (lt.variant, 'p', خطأ)
		ڇاپيو (')')
		ڇاپيو ('    {')
		اكتب ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		ڇاپيو ()
		إذا ڊيگهه (ٽيگ_فهرست) == 1:
			اكتب ('      tags[0] = %s;  /* %s */' % (hb_ٽيگ (ٽيگ_فهرست[0]), sanitize_comment (ot.نالا[ٽيگ_فهرست[0]])))
			ڇاپيو ()
			ڇاپيو ('      *count = 1;')
		وإلا:
			ڇاپيو ('    hb_tag_t possible_tags[] = {')
			لكل ٽيگ_2 في ٽيگ_فهرست:
				اكتب ('      %s,  /* %s */' % (hb_ٽيگ (ٽيگ_2), sanitize_comment (ot.نالا[ٽيگ_2])))
				ڇاپيو ()
			ڇاپيو ('      };')
			ڇاپيو ('      for (i = 0; i < %s && i < *count; i++)' % ڊيگهه (ٽيگ_فهرست))
			ڇاپيو ('\ttags[i] = possible_tags[i];')
			ڇاپيو ('      *count = i;')
		ڇاپيو ('      return true;')
		ڇاپيو ('    }')
ڇاپيو ('  }')
ڇاپيو ('out:')

ڇاپيو ('  switch (lang_str[0])')
ڇاپيو ('  {')
لكل initial, شيون في sorted (complex_ٽيگ_فهرست.شيون ()):
	إذا initial == 'und':
		استمر
	ڇاپيو ("  case '%s':" % sanitize_bcp47_ٽيگ (initial))
	لكل lt, ٽيگ_فهرست في شيون:
		إذا ليس ٽيگ_فهرست:
			استمر
		ڇاپيو ('    if (', پڇاڙي='')
		script = lt.script
		region = lt.region
		إذا lt.grandfathered:
			ڇاپيو ('0 == strcmp (&lang_str[1], "%s")' % sanitize_bcp47_ٽيگ (lt.language[1:]), پڇاڙي='')
		وإلا:
			string_literal = lt.language[1:] + '-'
			إذا script:
				string_literal += script
				script = ڪجهه_نه
				إذا region:
					string_literal += '-' + region
					region = ڪجهه_نه
			إذا string_literal[-1] == '-':
				ڇاپيو ('0 == strncmp (&lang_str[1], "%s", %i)' % (sanitize_bcp47_ٽيگ (string_literal), ڊيگهه (string_literal)), پڇاڙي='')
			وإلا:
				ڇاپيو ('lang_matches (&lang_str[1], limit, "%s", %i)' % (sanitize_bcp47_ٽيگ (string_literal), ڊيگهه (string_literal)), پڇاڙي='')
		print_subtag_matches (script, 'lang_str', سچ)
		print_subtag_matches (region, 'lang_str', سچ)
		print_subtag_matches (lt.variant, 'lang_str', سچ)
		ڇاپيو (')')
		ڇاپيو ('    {')
		اكتب ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		ڇاپيو ()
		إذا ڊيگهه (ٽيگ_فهرست) == 1:
			اكتب ('      tags[0] = %s;  /* %s */' % (hb_ٽيگ (ٽيگ_فهرست[0]), sanitize_comment (ot.نالا[ٽيگ_فهرست[0]])))
			ڇاپيو ()
			ڇاپيو ('      *count = 1;')
		وإلا:
			ڇاپيو ('      unsigned int i;')
			ڇاپيو ('      hb_tag_t possible_tags[] = {')
			لكل ٽيگ_2 في ٽيگ_فهرست:
				اكتب ('\t%s,  /* %s */' % (hb_ٽيگ (ٽيگ_2), sanitize_comment (ot.نالا[ٽيگ_2])))
				ڇاپيو ()
			ڇاپيو ('      };')
			ڇاپيو ('      for (i = 0; i < %s && i < *count; i++)' % ڊيگهه (ٽيگ_فهرست))
			ڇاپيو ('\ttags[i] = possible_tags[i];')
			ڇاپيو ('      *count = i;')
		ڇاپيو ('      return true;')
		ڇاپيو ('    }')
	ڇاپيو ('    break;')

ڇاپيو ('  }')
ڇاپيو ('  return false;')
ڇاپيو ('}')
ڇاپيو ()
ڇاپيو ('/**')
ڇاپيو (' * hb_ot_ambiguous_tag_to_language')
ڇاپيو (' * @tag: A language tag.')
ڇاپيو (' *')
ڇاپيو (' * Converts @tag to a BCP 47 language tag if it is ambiguous (it corresponds to')
ڇاپيو (' * many language tags) and the best tag is not the first (sorted alphabetically,')
ڇاپيو (' * with two-letter tags having priority over all three-letter tags), or if the')
ڇاپيو (' * best tag consists of multiple subtags, or if the best tag does not appear in')
ڇاپيو (' * #ot_languages2 or #ot_languages3.')
ڇاپيو (' *')
ڇاپيو (' * Return value: The #hb_language_t corresponding to the BCP 47 language tag,')
ڇاپيو (' * or #HB_LANGUAGE_INVALID if @tag is not ambiguous.')
ڇاپيو (' **/')
ڇاپيو ('static inline hb_language_t')
ڇاپيو ('hb_ot_ambiguous_tag_to_language (hb_tag_t tag)')
ڇاپيو ('{')
ڇاپيو ('  switch (tag)')
ڇاپيو ('  {')

وصف verify_disambiguation_dict ():
	"""Verify and normalize ``disambiguation``.

	``disambiguation`` is a map of ambiguous OpenType language system
	tags to the particular BCP 47 tags they correspond to. This function
	checks that all its keys really are ambiguous and that each key's
	value is valid for that key. It checks that no ambiguous tag is
	missing, except when it can figure out which BCP 47 tag is the best
	by itself.

	It modifies ``disambiguation`` to remove keys whose values are the
	same as those that the fallback would return anyway, and to add
	ambiguous keys whose disambiguations it determined automatically.

	Raises:
		AssertionError: Verification failed.
	"""
	عام bcp_47
	عام disambiguation
	عام ot
	لكل ot_ٽيگ, bcp_47_ٽيگ_فهرست في ot.منزل_bcp_47.شيون ():
		إذا ot_ٽيگ == DEFAULT_LANGUAGE_SYSTEM:
			primary_ٽيگ_فهرست = []
		وإلا:
			primary_ٽيگ_فهرست = list (t لكل t في bcp_47_ٽيگ_فهرست إذا t ليس في bcp_47.grandfathered و ot.from_bcp_47.get (t)[0] == ot_ٽيگ)
		إذا ڊيگهه (primary_ٽيگ_فهرست) == 1:
			expect (ot_ٽيگ ليس في disambiguation, 'unnecessary disambiguation for OT tag: %s' % ot_ٽيگ)
			إذا '-' في primary_ٽيگ_فهرست[0]:
				disambiguation[ot_ٽيگ] = primary_ٽيگ_فهرست[0]
			وإلا:
				پهريون_ٽيگ = sorted ((t لكل t في bcp_47_ٽيگ_فهرست إذا t ليس في bcp_47.grandfathered و ot_ٽيگ في ot.from_bcp_47.get (t)),
						ڪنجي_2=مجهول t: (ڊيگهه (t), t))[0]
				إذا primary_ٽيگ_فهرست[0] != پهريون_ٽيگ:
					disambiguation[ot_ٽيگ] = primary_ٽيگ_فهرست[0]
		وإلاإذا ڊيگهه (primary_ٽيگ_فهرست) == 0:
			expect (ot_ٽيگ ليس في disambiguation, 'There is no possible valid disambiguation for %s' % ot_ٽيگ)
		وإلا:
			original_languages = [t لكل t في primary_ٽيگ_فهرست إذا t في ot.from_bcp_47_uninherited و 'retired code' ليس في bcp_47.scopes.get (t, '')]
			إذا ڊيگهه (original_languages) == 1:
				macrolanguages = original_languages
			وإلا:
				macrolanguages = [t لكل t في primary_ٽيگ_فهرست إذا bcp_47.scopes.get (t) == ' [macrolanguage]']
			إذا ڊيگهه (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_ٽيگ_فهرست إذا bcp_47.scopes.get (t) == ' [collection]')
			إذا ڊيگهه (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_ٽيگ_فهرست إذا 'retired code' ليس في bcp_47.scopes.get (t, ''))
			إذا ڊيگهه (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_ٽيگ_فهرست إذا t.هيٺاهون () == ISO_639_3_منزل_1.get (ot_ٽيگ.هيٺاهون (), ot_ٽيگ.هيٺاهون ()))
			إذا ڊيگهه (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_ٽيگ_فهرست إذا '-' ليس في t)
			إذا ڊيگهه (macrolanguages) != 1:
				expect (ot_ٽيگ في disambiguation, 'ambiguous OT tag: %s %s' % (ot_ٽيگ, sorted (primary_ٽيگ_فهرست)))
				expect (disambiguation[ot_ٽيگ] في bcp_47_ٽيگ_فهرست,
						'%s is not a valid disambiguation for %s' % (disambiguation[ot_ٽيگ], ot_ٽيگ))
			وإلاإذا ot_ٽيگ ليس في disambiguation:
				disambiguation[ot_ٽيگ] = macrolanguages[0]
			إذا '-' ليس في disambiguation[ot_ٽيگ]:
				different_bcp_47_ٽيگ_فهرست = sorted ((t لكل t في bcp_47_ٽيگ_فهرست إذا ليس same_ٽيگ (t, ot.from_bcp_47.get (t))),
						ڪنجي_2=مجهول t: (ڊيگهه (t), t))
				إذا different_bcp_47_ٽيگ_فهرست و disambiguation[ot_ٽيگ] == different_bcp_47_ٽيگ_فهرست[0]:
					هٽايو disambiguation[ot_ٽيگ]
	لكل ot_ٽيگ في disambiguation.ڪنجيون ():
		expect (ot_ٽيگ في ot.منزل_bcp_47, 'unknown OT tag: %s' % ot_ٽيگ)

verify_disambiguation_dict ()
لكل ot_ٽيگ, bcp_47_ٽيگ في sorted (disambiguation.شيون ()):
	اكتب ('  case %s:  /* %s */' % (hb_ٽيگ (ot_ٽيگ), sanitize_comment (ot.نالا[ot_ٽيگ])))
	ڇاپيو ()
	اكتب ('    return hb_language_from_string (\"%s\", -1);  /* %s */' % (sanitize_bcp47_ٽيگ (bcp_47_ٽيگ), sanitize_comment (bcp_47.get_name (Language_ٽيگ (bcp_47_ٽيگ)))))
	ڇاپيو ()

ڇاپيو ('  default:')
ڇاپيو ('    return HB_LANGUAGE_INVALID;')
ڇاپيو ('  }')
ڇاپيو ('}')

ڇاپيو ()
ڇاپيو ('#endif /* HB_OT_TAG_TABLE_HH */')
ڇاپيو ()
ڇاپيو ('/* == End of generated table == */')
