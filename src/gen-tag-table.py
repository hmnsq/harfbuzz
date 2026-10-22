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

దిగుమతి collections
దిగుమతి html
తః html.parser దిగుమతి HTMLParser
దిగుమతి itertools
దిగుమతి os
దిగుమతి re
దిగుమతి sys
దిగుమతి unicodedata

ఒకవేళ పొడవు (sys.argv) > 1 మరియు sys.argv[1] == '--rust':
	తీసివేయి sys.argv[1]
	output_rust = నిజం
లేకపోతే:
	output_rust = అబద్ధం

ఒకవేళ పొడవు (sys.argv) != 3:
	sys.exit (__doc__)

నిర్వచనం expect (condition, సందేశం=ఏమీలేదు):
	ఒకవేళ కాదు condition:
		ఒకవేళ సందేశం అయిన ఏమీలేదు:
			లేపు AssertionError
		లేపు AssertionError (సందేశం)

నిర్వచనం లిఖ (s):
	sys.stdout.flush ()
	sys.stdout.బఫరు.లిఖ (s.encode ('utf-8'))

_ALLOWED_COMMENT_PUNCT = సమితి (" -—–.,;:’'ʼ()[]/!=>_+")

నిర్వచనం sanitize_comment (s):
	"""Validate that a comment contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ఒకవేళ కాదు isinstance (s, str):
		లేపు TypeError ("Expected string for comment, got %s" % type (s).__name__)
	s = s.strip ()
	ఒకవేళ కాదు s:
		తిరిగి ''
	కొరకు ch లో s:
		cat = unicodedata.category (ch)
		ఒకవేళ కాదు (cat.startswith (('L', 'M', 'N', 'Z')) లేదా ch లో _ALLOWED_COMMENT_PUNCT):
			లేపు విలువ_Error ("Disallowed character %r (U+%04X) in comment: %r" % (ch, ord (ch), s))
	తిరిగి s

నిర్వచనం sanitize_శీర్షిక (s):
	"""Validate that a header string contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ఒకవేళ కాదు isinstance (s, str):
		లేపు TypeError ("Expected string for header, got %s" % type (s).__name__)
	s = s.strip ()
	ఒకవేళ కాదు re.fullmatch (r'[a-zA-Z0-9 <>=/:\-_."]{1,200}', s):
		లేపు విలువ_Error ("Invalid header line %r: contains disallowed characters" % s)
	తిరిగి s

నిర్వచనం sanitize_bcp47_ట్యాగ్ (s):
	"""Validate that a BCP-47 tag or subtag contains only alphanumeric characters and hyphens.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ఒకవేళ కాదు isinstance (s, str):
		లేపు TypeError ("Expected string, got %s" % type (s).__name__)
	ఒకవేళ కాదు re.fullmatch (r'[a-zA-Z0-9-]+', s):
		లేపు విలువ_Error ("Invalid BCP-47 tag or subtag literal %r: must contain only alphanumeric characters and hyphens" % s)
	తిరిగి s

నిర్వచనం sanitize_ట్యాగ్ (ట్యాగ్_2):
	"""Validate that an OpenType tag contains only 1-4 alphanumeric characters or spaces.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ఒకవేళ కాదు isinstance (ట్యాగ్_2, str):
		లేపు TypeError ("Expected string for tag, got %s" % type (ట్యాగ్_2).__name__)
	ఒకవేళ ట్యాగ్_2 == DEFAULT_LANGUAGE_SYSTEM:
		తిరిగి ''
	ఒకవేళ కాదు re.fullmatch (r'[a-zA-Z0-9 ]{1,4}', ట్యాగ్_2):
		లేపు విలువ_Error ("Invalid tag %r: must contain only 1-4 alphanumeric characters or spaces" % ట్యాగ్_2)
	తిరిగి ట్యాగ్_2

DEFAULT_LANGUAGE_SYSTEM = ''

# from https://www-01.sil.org/iso639-3/iso-639-3.tab
ISO_639_3_గమ్యం_1 = {
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

తరగతి Language_ట్యాగ్ (object):
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
	నిర్వచనం __init__ (self, ట్యాగ్_2):
		వైశ్విక bcp_47
		self.subtags = ట్యాగ్_2.క్రింది ().విభజించు ('-')
		self.grandfathered = ట్యాగ్_2.క్రింది () లో bcp_47.grandfathered
		ఒకవేళ self.grandfathered:
			self.language = ట్యాగ్_2.క్రింది ()
			self.script = ''
			self.region = ''
			self.variant = ''
		లేకపోతే:
			self.language = self.subtags[0]
			self.script = self.కనుగొను_మొదటి (అజ్ఞాత s: పొడవు (s) == 4 మరియు s[0] > '9', self.subtags)
			self.region = self.కనుగొను_మొదటి (అజ్ఞాత s: పొడవు (s) == 2 మరియు s[0] > '9' లేదా పొడవు (s) == 3 మరియు s[0] <= '9', self.subtags[1:])
			self.variant = self.కనుగొను_మొదటి (అజ్ఞాత s: పొడవు (s) > 4 లేదా పొడవు (s) == 4 మరియు s[0] <= '9', self.subtags)

	నిర్వచనం __str__(self):
		తిరిగి '-'.join(self.subtags)

	నిర్వచనం __repr__ (self):
		తిరిగి 'LanguageTag(%r)' % str(self)

	@staticmethod
	నిర్వచనం కనుగొను_మొదటి (function, sequence):
		ప్రయత్నించు:
			తిరిగి తదుపరి (iter (filter (function, sequence)))
		మినహాయింపు StopIteration:
			తిరిగి ఏమీలేదు

	నిర్వచనం is_complex (self):
		"""Return whether this tag is too complex to represent as a
		``LangTag`` in the generated code.

		Complex tags need to be handled in
		``hb_ot_tags_from_complex_language``.

		Returns:
			Whether this tag is complex.
		"""
		తిరిగి కాదు (పొడవు (self.subtags) == 1
			లేదా self.grandfathered
			మరియు పొడవు (self.subtags[1]) != 3
			మరియు ot.from_bcp_47[self.subtags[0]] == ot.from_bcp_47[self.language])

	నిర్వచనం get_సమూహం (self):
		"""Return the group into which this tag should be categorized in
		``hb_ot_tags_from_complex_language``.

		The group is the first letter of the tag, or ``'und'`` if this tag
		should not be matched in a ``switch`` statement in the generated
		code.

		Returns:
			This tag's group.
		"""
		తిరిగి ('und'
			ఒకవేళ (self.language == 'und'
				లేదా self.variant లో bcp_47.prefixes మరియు పొడవు (bcp_47.prefixes[self.variant]) == 1)
			లేకపోతే self.language[0])

తరగతి OpenTypeRegistryParser (HTMLParser):
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
	నిర్వచనం __init__ (self):
		HTMLParser.__init__ (self)
		self.శీర్షిక = ''
		self.పేర్లు = {}
		self.ranks = collections.defaultdict (పూర్ణాంకం)
		self.గమ్యం_bcp_47 = collections.defaultdict (సమితి)
		self.from_bcp_47 = collections.defaultdict (సమితి)
		self.from_bcp_47_uninherited = ఏమీలేదు
		# Whether the parser is in a <td> element
		self._td = అబద్ధం
		# Whether the parser ignores the rest of the current <td> element
		self._disengaged = అబద్ధం
		# The text of the <td> elements of the current <tr> element.
		self._current_tr = []

	నిర్వచనం handle_starttag (self, ట్యాగ్_2, attrs):
		ఒకవేళ ట్యాగ్_2 == 'a':
			ఒకవేళ self._current_tr మరియు కాదు self._disengaged:
				self._current_tr[-1] = ''
				self._disengaged = నిజం
		లేదావేళ ట్యాగ్_2 == 'br':
			self._disengaged = నిజం
		లేదావేళ ట్యాగ్_2 == 'meta':
			కొరకు attr, విలువ_2 లో attrs:
				ఒకవేళ attr == 'name' మరియు విలువ_2 == 'updated_at':
					self.శీర్షిక = self.get_starttag_పాఠ్యం ()
					ఆపు
		లేదావేళ ట్యాగ్_2 == 'td':
			self._td = నిజం
			self._current_tr.append ('')
		లేదావేళ ట్యాగ్_2 == 'tr':
			self._disengaged = అబద్ధం
			self._current_tr = []

	నిర్వచనం handle_endtag (self, ట్యాగ్_2):
		ఒకవేళ ట్యాగ్_2 == 'td':
			self._td = అబద్ధం
			self._disengaged = అబద్ధం
		లేదావేళ ట్యాగ్_2 == 'tr' మరియు self._current_tr:
			expect (2 <= పొడవు (self._current_tr) <= 3)
			name = self._current_tr[0].strip ()
			ట్యాగ్_2 = self._current_tr[1].strip ("\t\n\v\f\r '")
			rank = 0
			ఒకవేళ పొడవు (ట్యాగ్_2) > 4:
				expect (ట్యాగ్_2.endswith (' (deprecated)'), 'ill-formed OpenType tag: %s' % ట్యాగ్_2)
				name += ' (deprecated)'
				ట్యాగ్_2 = ట్యాగ్_2.విభజించు (' ')[0]
				rank = 1
			self.పేర్లు[ట్యాగ్_2] = re.sub (' languages$', '', name)
			ఒకవేళ కాదు self._current_tr[2]:
				తిరిగి
			iso_codes = self._current_tr[2].strip ()
			self.గమ్యం_bcp_47[ట్యాగ్_2].update (ISO_639_3_గమ్యం_1.get (సంకేతం_2, సంకేతం_2) కొరకు సంకేతం_2 లో iso_codes.replace (' ', '').విభజించు (','))
			rank += 2 * పొడవు (self.గమ్యం_bcp_47[ట్యాగ్_2])
			self.ranks[ట్యాగ్_2] = rank

	నిర్వచనం handle_దత్తాంశం (self, దత్తాంశం):
		ఒకవేళ self._td మరియు కాదు self._disengaged:
			self._current_tr[-1] += దత్తాంశం

	నిర్వచనం handle_charref (self, name):
		self.handle_దత్తాంశం (html.unescape ('&#%s;' % name))

	నిర్వచనం handle_entityref (self, name):
		self.handle_దత్తాంశం (html.unescape ('&%s;' % name))

	నిర్వచనం parse (self, filename):
		"""Parse the OpenType language system tag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		తోడుగా open (filename, encoding='utf-8') ఇవ f:
			self.feed (f.పఠ ())
		expect (self.శీర్షిక)
		కొరకు ట్యాగ్_2, iso_codes లో self.గమ్యం_bcp_47.వస్తువులు ():
			కొరకు iso_సంకేతం లో iso_codes:
				self.from_bcp_47[iso_సంకేతం].కూడు (ట్యాగ్_2)

	నిర్వచనం కూడు_language (self, bcp_47_ట్యాగ్, ot_ట్యాగ్):
		"""Add a language as if it were in the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag. If the tag is more than just
				a language subtag, and if the language subtag is a
				macrolanguage, then new languages are added corresponding
				to the macrolanguages' individual languages with the
				remainder of the tag appended.
			ot_tag (str): An OpenType language system tag.
		"""
		వైశ్విక bcp_47
		self.గమ్యం_bcp_47[ot_ట్యాగ్].కూడు (bcp_47_ట్యాగ్)
		self.from_bcp_47[bcp_47_ట్యాగ్].కూడు (ot_ట్యాగ్)
		ఒకవేళ bcp_47_ట్యాగ్.క్రింది () కాదు లో bcp_47.grandfathered:
			ప్రయత్నించు:
				[macrolanguage, suffix] = bcp_47_ట్యాగ్.విభజించు ('-', 1)
				ఒకవేళ macrolanguage లో bcp_47.macrolanguages:
					s = సమితి ()
					కొరకు language లో bcp_47.macrolanguages[macrolanguage]:
						ఒకవేళ language.క్రింది () కాదు లో bcp_47.grandfathered:
							s.కూడు ('%s-%s' % (language, suffix))
					bcp_47.macrolanguages['%s-%s' % (macrolanguage, suffix)] = s
			మినహాయింపు విలువ_Error:
				దాటు

	@staticmethod
	నిర్వచనం _remove_language (ట్యాగ్_1, dict_1, dict_2):
		కొరకు ట్యాగ్_2_2 లో dict_1.తీయి (ట్యాగ్_1):
			dict_2[ట్యాగ్_2_2].remove (ట్యాగ్_1)
			ఒకవేళ కాదు dict_2[ట్యాగ్_2_2]:
				తీసివేయి dict_2[ట్యాగ్_2_2]

	నిర్వచనం remove_language_ot (self, ot_ట్యాగ్):
		"""Remove an OpenType tag from the registry.

		Args:
			ot_tag (str): An OpenType tag.
		"""
		self._remove_language (ot_ట్యాగ్, self.గమ్యం_bcp_47, self.from_bcp_47)

	నిర్వచనం remove_language_bcp_47 (self, bcp_47_ట్యాగ్):
		"""Remove a BCP 47 tag from the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag.
		"""
		self._remove_language (bcp_47_ట్యాగ్, self.from_bcp_47, self.గమ్యం_bcp_47)

	నిర్వచనం inherit_from_macrolanguages (self):
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
		వైశ్విక bcp_47
		మొదటి_సమయం = self.from_bcp_47_uninherited అయిన ఏమీలేదు
		ఒకవేళ మొదటి_సమయం:
			self.from_bcp_47_uninherited = dict (self.from_bcp_47)
		కొరకు macrolanguage, languages లో dict (bcp_47.macrolanguages).వస్తువులు ():
			ot_macrolanguages = {
				ot_macrolanguage కొరకు ot_macrolanguage లో self.from_bcp_47_uninherited.get (macrolanguage, సమితి ())
			}
			blocked_ot_macrolanguages = సమితి ()
			ఒకవేళ 'retired code' కాదు లో bcp_47.scopes.get (macrolanguage, ''):
				కొరకు ot_macrolanguage లో ot_macrolanguages:
					విడత_trip_macrolanguages = {
						l కొరకు l లో self.గమ్యం_bcp_47[ot_macrolanguage]
						ఒకవేళ 'retired code' కాదు లో bcp_47.scopes.get (l, '')
					}
					విడత_trip_languages = {
						l కొరకు l లో languages
						ఒకవేళ 'retired code' కాదు లో bcp_47.scopes.get (l, '')
					}
					intersection = విడత_trip_macrolanguages & విడత_trip_languages
					ఒకవేళ intersection మరియు intersection != విడత_trip_languages:
						blocked_ot_macrolanguages.కూడు (ot_macrolanguage)
			ఒకవేళ ot_macrolanguages:
				కొరకు ot_macrolanguage లో ot_macrolanguages:
					ఒకవేళ ot_macrolanguage కాదు లో blocked_ot_macrolanguages:
						కొరకు language లో languages:
							self.కూడు_language (language, ot_macrolanguage)
							ఒకవేళ కాదు blocked_ot_macrolanguages:
								self.ranks[ot_macrolanguage] += 1
			లేదావేళ మొదటి_సమయం:
				కొరకు language లో languages:
					ఒకవేళ language లో self.from_bcp_47_uninherited:
						ot_macrolanguages |= self.from_bcp_47_uninherited[language]
					లేకపోతే:
						ot_macrolanguages.clear ()
					ఒకవేళ కాదు ot_macrolanguages:
						ఆపు
				కొరకు ot_macrolanguage లో ot_macrolanguages:
					self.కూడు_language (macrolanguage, ot_macrolanguage)

	నిర్వచనం క్రమీకరించు_languages (self):
		"""Sort the values of ``from_bcp_47`` in ascending rank order."""
		కొరకు language, ట్యాగులు లో self.from_bcp_47.వస్తువులు ():
			self.from_bcp_47[language] = sorted (ట్యాగులు,
					కీలకం_2=అజ్ఞాత t: (self.ranks[t] + rank_delta (language, t), t))

ot = OpenTypeRegistryParser ()

తరగతి BCP47Parser (object):
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
	నిర్వచనం __init__ (self):
		self.శీర్షిక = ''
		self.పేర్లు = {}
		self.scopes = {}
		self.macrolanguages = collections.defaultdict (సమితి)
		self.prefixes = collections.defaultdict (సమితి)
		self.grandfathered = సమితి ()

	నిర్వచనం parse (self, filename):
		"""Parse the BCP 47 subtag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		తోడుగా open (filename, encoding='utf-8') ఇవ f:
			subtag_type = ఏమీలేదు
			subtag = ఏమీలేదు
			deprecated = అబద్ధం
			has_preferred_విలువ = అబద్ధం
			పంక్తి_బఫరు = ''
			కొరకు పంక్తి లో itertools.chain (f, ['']):
				పంక్తి = పంక్తి.rstrip ()
				ఒకవేళ పంక్తి.startswith (' '):
					పంక్తి_బఫరు += పంక్తి[1:]
					కొనసాగు
				పంక్తి, పంక్తి_బఫరు = పంక్తి_బఫరు, పంక్తి
				ఒకవేళ పంక్తి.startswith ('Type: '):
					subtag_type = పంక్తి.విభజించు (' ')[1]
					deprecated = అబద్ధం
					has_preferred_విలువ = అబద్ధం
				లేదావేళ పంక్తి.startswith ('Subtag: ') లేదా పంక్తి.startswith ('Tag: '):
					subtag = పంక్తి.విభజించు (' ')[1]
					ఒకవేళ subtag_type == 'grandfathered':
						self.grandfathered.కూడు (subtag.క్రింది ())
				లేదావేళ పంక్తి.startswith ('Description: '):
					description = పంక్తి.విభజించు (' ', 1)[1].replace (' (individual language)', '')
					description = re.sub (r' (\(family\)|\((individual |macro)language\)|languages)$', '',
							description)
					ఒకవేళ subtag లో self.పేర్లు:
						self.పేర్లు[subtag] += '\n' + description
					లేకపోతే:
						self.పేర్లు[subtag] = description
				లేదావేళ subtag_type == 'language' లేదా subtag_type == 'grandfathered':
					ఒకవేళ పంక్తి.startswith ('Scope: '):
						scope = పంక్తి.విభజించు (' ')[1]
						ఒకవేళ scope == 'macrolanguage':
							scope = ' [macrolanguage]'
						లేదావేళ scope == 'collection':
							scope = ' [collection]'
						లేకపోతే:
							కొనసాగు
						self.scopes[subtag] = scope
					లేదావేళ పంక్తి.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
						deprecated = నిజం
					లేదావేళ deprecated మరియు పంక్తి.startswith ('Comments: see '):
						# If a subtag is split into multiple replacement subtags,
						# it essentially represents a macrolanguage.
						కొరకు language లో పంక్తి.replace (',', '').విభజించు (' ')[2:]:
							self.కూడు_macrolanguage (subtag, language)
					లేదావేళ పంక్తి.startswith ('Preferred-Value: '):
						# If a subtag is deprecated in favor of a single replacement subtag,
						# it is either a dialect or synonym of the preferred subtag. Either
						# way, it is close enough to the truth to consider the replacement
						# the macrolanguage of the deprecated language.
						has_preferred_విలువ = నిజం
						macrolanguage = పంక్తి.విభజించు (' ')[1]
						self.కూడు_macrolanguage (macrolanguage, subtag)
					లేదావేళ కాదు has_preferred_విలువ మరియు పంక్తి.startswith ('Macrolanguage: '):
						self.కూడు_macrolanguage (పంక్తి.విభజించు (' ')[1], subtag)
				లేదావేళ subtag_type == 'variant':
					ఒకవేళ పంక్తి.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
					లేదావేళ పంక్తి.startswith ('Prefix: '):
						self.prefixes[subtag].కూడు (పంక్తి.విభజించు (' ')[1])
				లేదావేళ పంక్తి.startswith ('File-Date: '):
					self.శీర్షిక = పంక్తి
		expect (self.శీర్షిక)

	నిర్వచనం కూడు_macrolanguage (self, macrolanguage, language):
		వైశ్విక ot
		ఒకవేళ language కాదు లో ot.from_bcp_47:
			కొరకు l లో self.macrolanguages.get (language, సమితి ()):
				self.కూడు_macrolanguage (macrolanguage, l)
		ఒకవేళ macrolanguage కాదు లో ot.from_bcp_47:
			కొరకు ls లో list (self.macrolanguages.విలువలు ()):
				ఒకవేళ macrolanguage లో ls:
					ls.కూడు (language)
					తిరిగి
		self.macrolanguages[macrolanguage].కూడు (language)

	నిర్వచనం remove_extra_macrolanguages (self):
		"""Make every language have at most one macrolanguage."""
		inverted = collections.defaultdict (list)
		కొరకు macrolanguage, languages లో self.macrolanguages.వస్తువులు ():
			కొరకు language లో languages:
				inverted[language].append (macrolanguage)
		కొరకు language, macrolanguages లో inverted.వస్తువులు ():
			ఒకవేళ పొడవు (macrolanguages) > 1:
				macrolanguages.క్రమీకరించు (కీలకం_2=అజ్ఞాత ml: పొడవు (self.macrolanguages[ml]))
				biggest_macrolanguage = macrolanguages.తీయి ()
				కొరకు macrolanguage లో macrolanguages:
					self.కూడు_macrolanguage (biggest_macrolanguage, macrolanguage)

	నిర్వచనం _get_name_piece (self, subtag):
		"""Return the first name of a subtag plus its scope suffix.

		Args:
			subtag (str): A BCP 47 subtag.

		Returns:
			The name form of ``subtag``.
		"""
		తిరిగి self.పేర్లు[subtag].విభజించు ('\n')[0] + self.scopes.get (subtag, '')

	నిర్వచనం get_name (self, lt):
		"""Return the names of the subtags in a language tag.

		Args:
			lt (LanguageTag): A BCP 47 language tag.

		Returns:
			The name form of ``lt``.
		"""
		name = self._get_name_piece (lt.language)
		ఒకవేళ lt.script:
			name += '; ' + self._get_name_piece (lt.script.title ())
		ఒకవేళ lt.region:
			name += '; ' + self._get_name_piece (lt.region.పై ())
		ఒకవేళ lt.variant:
			name += '; ' + self._get_name_piece (lt.variant)
		తిరిగి name

bcp_47 = BCP47Parser ()

ot.parse (sys.argv[1])
bcp_47.parse (sys.argv[2])

ot.కూడు_language ('ary', 'MOR')

ot.కూడు_language ('ath', 'ATH')

ot.కూడు_language ('bai', 'BML')

ot.ranks['BAL'] = ot.ranks['KAR'] + 1

ot.కూడు_language ('ber', 'BBR')

ot.remove_language_ot ('PGR')
ot.కూడు_language ('el-polyton', 'PGR')

bcp_47.పేర్లు['flm'] = 'Falam Chin'
bcp_47.scopes['flm'] = ' (retired code)'
bcp_47.macrolanguages['flm'] = {'cfm'}

ot.ranks['FNE'] = ot.ranks['TNE'] + 1

ot.కూడు_language ('und-fonipa', 'IPPH')

ot.కూడు_language ('und-fonnapa', 'APPH')

ot.కూడు_language ('und-fonupa', 'UPPH')

ot.కూడు_language ('ga-Latg', 'IRT')

ot.కూడు_language ('hy-arevmda', 'HYE')

ot.remove_language_ot ('KGE')
ot.కూడు_language ('und-Geok', 'KGE')

ot.కూడు_language ('kht', 'KHN')
ot.పేర్లు['KHN'] = ot.పేర్లు['KHT'] + ' (Microsoft fonts)'
ot.ranks['KHN'] = ot.ranks['KHT'] + 1

ot.ranks['LCR'] = ot.ranks['MCR'] + 1

ot.పేర్లు['MAL'] = 'Malayalam Traditional'
ot.ranks['MLR'] += 1

bcp_47.పేర్లు['mhv'] = 'Arakanese'
bcp_47.scopes['mhv'] = ' (retired code)'

ot.కూడు_language ('mnw-TH', 'MONT')

ot.కూడు_language ('no', 'NOR')

ot.కూడు_language ('oc-provenc', 'PRO')

ot.remove_language_ot ('QUZ')
ot.కూడు_language ('qu', 'QUZ')
ot.కూడు_language ('qub', 'QWH')
ot.కూడు_language ('qud', 'QVI')
ot.కూడు_language ('qug', 'QVI')
ot.కూడు_language ('qul', 'QUH')
ot.కూడు_language ('qup', 'QVI')
ot.కూడు_language ('qur', 'QWH')
ot.కూడు_language ('qus', 'QUH')
ot.కూడు_language ('quw', 'QVI')
ot.కూడు_language ('qux', 'QWH')
ot.కూడు_language ('qva', 'QWH')
ot.కూడు_language ('qvh', 'QWH')
ot.కూడు_language ('qvj', 'QVI')
ot.కూడు_language ('qvl', 'QWH')
ot.కూడు_language ('qvm', 'QWH')
ot.కూడు_language ('qvn', 'QWH')
ot.కూడు_language ('qvo', 'QVI')
ot.కూడు_language ('qvp', 'QWH')
ot.కూడు_language ('qvw', 'QWH')
ot.కూడు_language ('qvz', 'QVI')
ot.కూడు_language ('qwa', 'QWH')
ot.కూడు_language ('qws', 'QWH')
ot.కూడు_language ('qxa', 'QWH')
ot.కూడు_language ('qxc', 'QWH')
ot.కూడు_language ('qxh', 'QWH')
ot.కూడు_language ('qxl', 'QVI')
ot.కూడు_language ('qxn', 'QWH')
ot.కూడు_language ('qxo', 'QWH')
ot.కూడు_language ('qxr', 'QVI')
ot.కూడు_language ('qxt', 'QWH')
ot.కూడు_language ('qxw', 'QWH')

bcp_47.macrolanguages['ro-MD'].కూడు ('mo')

ot.remove_language_ot ('SYRE')
ot.remove_language_ot ('SYRJ')
ot.remove_language_ot ('SYRN')
ot.కూడు_language ('und-Syre', 'SYRE')
ot.కూడు_language ('und-Syrj', 'SYRJ')
ot.కూడు_language ('und-Syrn', 'SYRN')

bcp_47.పేర్లు['xst'] = "Silt'e"
bcp_47.scopes['xst'] = ' (retired code)'
bcp_47.macrolanguages['xst'] = {'stv', 'wle'}

ot.కూడు_language ('xwo', 'TOD')

ot.remove_language_ot ('ZHH')
ot.remove_language_ot ('ZHP')
ot.remove_language_ot ('ZHT')
ot.remove_language_ot ('ZHTM')
bcp_47.macrolanguages['zh'].remove ('lzh')
bcp_47.macrolanguages['zh'].remove ('yue')
ot.కూడు_language ('zh-Hant-MO', 'ZHH')
ot.కూడు_language ('zh-Hant-MO', 'ZHTM')
ot.కూడు_language ('zh-Hant-HK', 'ZHH')
ot.కూడు_language ('zh-Hans', 'ZHS')
ot.కూడు_language ('zh-Hant', 'ZHT')
ot.కూడు_language ('zh-HK', 'ZHH')
ot.కూడు_language ('zh-MO', 'ZHH')
ot.కూడు_language ('zh-MO', 'ZHTM')
ot.కూడు_language ('zh-TW', 'ZHT')
ot.కూడు_language ('lzh', 'ZHT')
ot.కూడు_language ('lzh-Hans', 'ZHS')
ot.కూడు_language ('yue', 'ZHH')
ot.కూడు_language ('yue-Hans', 'ZHS')

నిర్వచనం rank_delta (bcp_47, ot):
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
	ఒకవేళ bcp_47 == 'ak' మరియు ot == 'AKA':
		తిరిగి -1
	ఒకవేళ bcp_47 == 'tw' మరియు ot == 'TWI':
		తిరిగి -1
	తిరిగి 0

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
ot.పేర్లు[DEFAULT_LANGUAGE_SYSTEM] = ''
ot.ranks[DEFAULT_LANGUAGE_SYSTEM] = max (ot.ranks.విలువలు ()) + 1
కొరకు tricky_ot_ట్యాగ్ లో filter (అజ్ఞాత ట్యాగ్_2: re.సరిపోలిక ('[A-Z]{3}$', ట్యాగ్_2), ot.పేర్లు):
	possible_bcp_47_ట్యాగ్ = tricky_ot_ట్యాగ్.క్రింది ()
	ఒకవేళ possible_bcp_47_ట్యాగ్ లో bcp_47.పేర్లు మరియు కాదు ot.from_bcp_47[possible_bcp_47_ట్యాగ్]:
		ot.కూడు_language (possible_bcp_47_ట్యాగ్, DEFAULT_LANGUAGE_SYSTEM)
		bcp_47.macrolanguages[possible_bcp_47_ట్యాగ్] = సమితి ()
ot.క్రమీకరించు_languages ()

ఒకవేళ output_rust:
	నిర్వచనం hb_ట్యాగ్_rust (ట్యాగ్_2):
		ఒకవేళ ట్యాగ్_2 == DEFAULT_LANGUAGE_SYSTEM:
			తిరిగి 'Tag::new(&[0; 4])'
		sanitize_ట్యాగ్ (ట్యాగ్_2)
		తిరిగి 'Tag::new(b"%s%s%s%s")' % tuple (('%-4s' % ట్యాగ్_2)[:4])

	నిర్వచనం get_variant_సమితి_rust (name):
		తిరిగి సమితి (unicodedata.normalize (
					'NFD',
					n.replace ('\u02BC', "'").replace ('\u2019', "'"),
				)
				.encode ('ASCII', 'ignore')
				.strip ()
				కొరకు n లో re.విభజించు ('[\n(),]', name) ఒకవేళ n)

	నిర్వచనం language_name_intersection_rust (a, b):
		తిరిగి get_variant_సమితి_rust (a).intersection (get_variant_సమితి_rust (b))

	నిర్వచనం get_matching_language_name_rust (intersection, candidates):
		తిరిగి తదుపరి (iter (c కొరకు c లో candidates ఒకవేళ కాదు intersection.isdisjoint (get_variant_సమితి_rust (c))))

	ముద్రించు ('// WARNING: this file was generated by ../../harfbuzz/src/gen-tag-table.py --rust')
	ముద్రించు ()
	ముద్రించు ('use read_fonts::types::Tag;')
	ముద్రించు ()
	ముద్రించు ('pub(crate) struct LangTag {')
	ముద్రించు ('    pub language: [u8; 4],')
	ముద్రించు ('    pub tag: Tag,')
	ముద్రించు ('}')
	ముద్రించు ()
	ముద్రించు ('const fn lang(language: &[u8]) -> [u8; 4] {')
	ముద్రించు ('    let mut bytes = [0; 4];')
	ముద్రించు ('    let mut i = 0;')
	ముద్రించు ()
	ముద్రించు ('    while i < language.len() {')
	ముద్రించు ('        bytes[i] = language[i];')
	ముద్రించు ('        i += 1;')
	ముద్రించు ('    }')
	ముద్రించు ()
	ముద్రించు ('    bytes')
	ముద్రించు ('}')
	ముద్రించు ()
	ముద్రించు ('use super::tag::{lang_matches, strncmp, subtag_matches};')
	ముద్రించు ()
	ముద్రించు ('#[rustfmt::skip]')
	ముద్రించు ('pub(crate) static OPEN_TYPE_LANGUAGES: &[LangTag] = &[')

	కొరకు language, ట్యాగులు లో sorted (ot.from_bcp_47.వస్తువులు ()):
		ఒకవేళ language == '' లేదా '-' లో language:
			కొనసాగు
		commented_బహిర్గతం = పొడవు (language) == 3 మరియు పొడవు (ట్యాగులు) == 1 మరియు language == ట్యాగులు[0].క్రింది ()
		కొరకు ట్యాగ్_2 లో ట్యాగులు:
			ముద్రించు ('%sLangTag { language: lang(b"%s"), \ttag: %s },' % ('//  ' ఒకవేళ commented_బహిర్గతం లేకపోతే '    ', sanitize_bcp47_ట్యాగ్ (language), hb_ట్యాగ్_rust (ట్యాగ్_2)), చివర='')
			ముద్రించు (' // ', చివర='')
			bcp_47_name = bcp_47.పేర్లు.get (language, '')
			bcp_47_name_candidates = bcp_47_name.విభజించు ('\n')
			ot_name = ot.పేర్లు[ట్యాగ్_2]
			scope = bcp_47.scopes.get (language, '')
			ఒకవేళ ట్యాగ్_2 == DEFAULT_LANGUAGE_SYSTEM:
				comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
			లేకపోతే:
				intersection = language_name_intersection_rust (bcp_47_name, ot_name)
				ఒకవేళ కాదు intersection:
					comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
				లేకపోతే:
					name = get_matching_language_name_rust (intersection, bcp_47_name_candidates)
					bcp_47.పేర్లు[language] = name
					comment = '%s%s' % (name ఒకవేళ పొడవు (name) > పొడవు (ot_name) లేకపోతే ot_name, scope)
			ముద్రించు (sanitize_comment (comment))

	ముద్రించు ('];')
	ముద్రించు ()
	ముద్రించు ('/// Converts a multi-subtag BCP 47 language tag to language tags.')
	ముద్రించు ('pub fn tags_from_complex_language(language: &str, tags: &mut smallvec::SmallVec<[Tag; 3]>) -> bool {')

	నిర్వచనం print_subtag_matches_rust (subtag, new_పంక్తి):
		ఒకవేళ subtag:
			ఒకవేళ new_పంక్తి:
				ముద్రించు (' && ', చివర='')
			ముద్రించు ('subtag_matches(language, "-%s")' % sanitize_bcp47_ట్యాగ్ (subtag), చివర='')

	complex_ట్యాగులు = collections.defaultdict (list)
	కొరకు initial, సమూహం లో itertools.groupby ((lt_ట్యాగులు కొరకు lt_ట్యాగులు లో [
				(Language_ట్యాగ్ (language), ట్యాగులు)
				కొరకు language, ట్యాగులు లో sorted (ot.from_bcp_47.వస్తువులు (),
					కీలకం_2=అజ్ఞాత i: (-పొడవు (i[0]), i[0]))
			] ఒకవేళ lt_ట్యాగులు[0].is_complex ()),
			కీలకం_2=అజ్ఞాత lt_ట్యాగులు: lt_ట్యాగులు[0].get_సమూహం ()):
		complex_ట్యాగులు[initial] += సమూహం

	కొరకు initial, వస్తువులు లో sorted (complex_ట్యాగులు.వస్తువులు ()):
		ఒకవేళ initial != 'und':
			కొనసాగు
		కొరకు lt, ట్యాగులు లో వస్తువులు:
			ఒకవేళ కాదు ట్యాగులు:
				కొనసాగు
			ఒకవేళ lt.variant లో bcp_47.prefixes:
				expect (తదుపరి (iter (bcp_47.prefixes[lt.variant])) == lt.language,
						'%s is not a valid prefix of %s' % (lt.language, lt.variant))
			ముద్రించు ('    if ', చివర='')
			print_subtag_matches_rust (lt.script, అబద్ధం)
			print_subtag_matches_rust (lt.region, అబద్ధం)
			print_subtag_matches_rust (lt.variant, అబద్ధం)
			ముద్రించు (' {')
			ముద్రించు ('        // %s' % sanitize_comment (bcp_47.get_name (lt)))
			ఒకవేళ పొడవు (ట్యాగులు) == 1:
				ముద్రించు ('        tags.push(%s); // %s' % (hb_ట్యాగ్_rust (ట్యాగులు[0]), sanitize_comment (ot.పేర్లు[ట్యాగులు[0]])))
			లేకపోతే:
				ముద్రించు ('        let possible_tags = &[')
				కొరకు ట్యాగ్_2 లో ట్యాగులు:
					ముద్రించు ('            %s, // %s' % (hb_ట్యాగ్_rust (ట్యాగ్_2), sanitize_comment (ot.పేర్లు[ట్యాగ్_2])))
				ముద్రించు ('        ];')
				ముద్రించు ('        tags.extend_from_slice(possible_tags);')
			ముద్రించు ('        return true;')
			ముద్రించు ('    }')

	ముద్రించు ('    match language.as_bytes()[0] {')
	కొరకు initial, వస్తువులు లో sorted (complex_ట్యాగులు.వస్తువులు ()):
		ఒకవేళ initial == 'und':
			కొనసాగు
		ముద్రించు ("        b'%s' => {" % sanitize_bcp47_ట్యాగ్ (initial))
		కొరకు lt, ట్యాగులు లో వస్తువులు:
			ఒకవేళ కాదు ట్యాగులు:
				కొనసాగు
			ముద్రించు ('            if ', చివర='')
			script = lt.script
			region = lt.region
			ఒకవేళ lt.grandfathered:
				ముద్రించు ('&language[1..] == "%s"' % sanitize_bcp47_ట్యాగ్ (lt.language[1:]), చివర='')
			లేకపోతే:
				string_literal = lt.language[1:] + '-'
				ఒకవేళ script:
					string_literal += script
					script = ఏమీలేదు
					ఒకవేళ region:
						string_literal += '-' + region
						region = ఏమీలేదు
				ఒకవేళ string_literal[-1] == '-':
					ముద్రించు ('strncmp(&language[1..], "%s", %i)' % (sanitize_bcp47_ట్యాగ్ (string_literal), పొడవు (string_literal)), చివర='')
				లేకపోతే:
					ముద్రించు ('lang_matches(&language[1..], "%s")' % sanitize_bcp47_ట్యాగ్ (string_literal), చివర='')
			print_subtag_matches_rust (script, నిజం)
			print_subtag_matches_rust (region, నిజం)
			print_subtag_matches_rust (lt.variant, నిజం)
			ముద్రించు (' {')
			ముద్రించు ('                // %s' % sanitize_comment (bcp_47.get_name (lt)))
			ఒకవేళ పొడవు (ట్యాగులు) == 1:
				ముద్రించు ('                tags.push(%s); // %s' % (hb_ట్యాగ్_rust (ట్యాగులు[0]), sanitize_comment (ot.పేర్లు[ట్యాగులు[0]])))
			లేకపోతే:
				ముద్రించు ('                let possible_tags = &[')
				కొరకు ట్యాగ్_2 లో ట్యాగులు:
					ముద్రించు ('                    %s, // %s' % (hb_ట్యాగ్_rust (ట్యాగ్_2), sanitize_comment (ot.పేర్లు[ట్యాగ్_2])))
				ముద్రించు ('                ];')
				ముద్రించు ('                tags.extend_from_slice(possible_tags);')
			ముద్రించు ('                return true;')
			ముద్రించు ('            }')
		ముద్రించు ('        }')
	ముద్రించు ('        _ => {}')
	ముద్రించు ('    }')
	ముద్రించు ('    false')
	ముద్రించు ('}')
	sys.exit (0)

ముద్రించు ('/* == Start of generated table == */')
ముద్రించు ('/*')
ముద్రించు (' * The following table is generated by running:')
ముద్రించు (' *')
ముద్రించు (' *   %s languagetags language-subtag-registry' % os.మార్గం.basename (sys.argv[0]))
ముద్రించు (' *')
ముద్రించు (' * on files with these headers:')
ముద్రించు (' *')
ముద్రించు (' * %s' % sanitize_శీర్షిక (ot.శీర్షిక))
ముద్రించు (' * %s' % sanitize_శీర్షిక (bcp_47.శీర్షిక))
ముద్రించు (' */')
ముద్రించు ()
ముద్రించు ('#ifndef HB_OT_TAG_TABLE_HH')
ముద్రించు ('#define HB_OT_TAG_TABLE_HH')
ముద్రించు ()

నిర్వచనం hb_ట్యాగ్ (ట్యాగ్_2):
	"""Convert a tag to ``HB_TAG`` form.

	Args:
		tag (str): An OpenType tag.

	Returns:
		A snippet of C++ representing ``tag``.
	"""
	ఒకవేళ ట్యాగ్_2 == DEFAULT_LANGUAGE_SYSTEM:
		తిరిగి 'HB_TAG_NONE\t       '
	sanitize_ట్యాగ్ (ట్యాగ్_2)
	తిరిగి "HB_TAG('%s','%s','%s','%s')" % tuple (('%-4s' % ట్యాగ్_2)[:4])

నిర్వచనం get_variant_సమితి (name):
	"""Return a set of variant language names from a name.

	Args:
		name (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		A set of normalized language names.
	"""
	తిరిగి సమితి (unicodedata.normalize (
				'NFD',
				n.replace ('\u02BC', "'").replace ('\u2019', "'"),
			)
			.encode ('ASCII', 'ignore')
			.strip ()
			కొరకు n లో re.విభజించు ('[\n(),]', name) ఒకవేళ n)

నిర్వచనం language_name_intersection (a, b):
	"""Return the names in common between two language names.

	Args:
		a (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.
		b (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		The normalized language names shared by ``a`` and ``b``.
	"""
	తిరిగి get_variant_సమితి (a).intersection (get_variant_సమితి (b))

నిర్వచనం get_matching_language_name (intersection, candidates):
	తిరిగి తదుపరి (iter (c కొరకు c లో candidates ఒకవేళ కాదు intersection.isdisjoint (get_variant_సమితి (c))))

నిర్వచనం same_ట్యాగ్ (bcp_47_ట్యాగ్, ot_ట్యాగులు):
	తిరిగి పొడవు (bcp_47_ట్యాగ్) == 3 మరియు పొడవు (ot_ట్యాగులు) == 1 మరియు bcp_47_ట్యాగ్ == ot_ట్యాగులు[0].క్రింది ()

నిర్వచనం format_language_comment (language, ట్యాగ్_2):
	bcp_47_name = bcp_47.పేర్లు.get (language, '')
	bcp_47_name_candidates = bcp_47_name.విభజించు ('\n')
	ot_name = ot.పేర్లు[ట్యాగ్_2]
	scope = bcp_47.scopes.get (language, '')
	ఒకవేళ ట్యాగ్_2 == DEFAULT_LANGUAGE_SYSTEM:
		comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
	లేకపోతే:
		intersection = language_name_intersection (bcp_47_name, ot_name)
		ఒకవేళ కాదు intersection:
			comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
		లేకపోతే:
			name = get_matching_language_name (intersection, bcp_47_name_candidates)
			bcp_47.పేర్లు[language] = name
			comment = '%s%s' % (name ఒకవేళ పొడవు (name) > పొడవు (ot_name) లేకపోతే ot_name, scope)
	తిరిగి sanitize_comment (comment)

ముద్రించు ('static const LangTag ot_languages2[] = {')
కొరకు language, ట్యాగులు లో sorted (ot.from_bcp_47.వస్తువులు ()):
	ఒకవేళ language == '' లేదా '-' లో language లేదా పొడవు (language) != 2:
		కొనసాగు
	కొరకు ట్యాగ్_2 లో ట్యాగులు:
		ముద్రించు ('  {%s,\t%s},\t/* %s */' % (
			hb_ట్యాగ్ (language),
			hb_ట్యాగ్ (ట్యాగ్_2),
			format_language_comment (language, ట్యాగ్_2)))
ముద్రించు ('};')
ముద్రించు ()

ముద్రించు ('#ifndef HB_NO_LANGUAGE_LONG')

languages3_blocked = []
languages3_singles = []
languages3_multi_ranges = []
languages3_multi_విలువలు = []
కొరకు language, ట్యాగులు లో sorted (ot.from_bcp_47.వస్తువులు ()):
	ఒకవేళ language == '' లేదా '-' లో language లేదా పొడవు (language) != 3:
		కొనసాగు
	ఒకవేళ same_ట్యాగ్ (language, ట్యాగులు):
		కొనసాగు
	ఒకవేళ ట్యాగులు == [DEFAULT_LANGUAGE_SYSTEM]:
		languages3_blocked.append ((language, ట్యాగులు[0]))
		కొనసాగు
	ఒకవేళ పొడవు (ట్యాగులు) == 1:
		languages3_singles.append ((language, ట్యాగులు[0]))
		కొనసాగు
	స్థానభ్రంశం = పొడవు (languages3_multi_విలువలు)
	languages3_multi_విలువలు.extend ((language, ట్యాగ్_2) కొరకు ట్యాగ్_2 లో ట్యాగులు)
	languages3_multi_ranges.append ((language, స్థానభ్రంశం, పొడవు (ట్యాగులు), ట్యాగులు[0]))

ముద్రించు ('static const hb_tag_t ot_languages3_blocked[] = {')
కొరకు language, ట్యాగ్_2 లో languages3_blocked:
	ముద్రించు ('  %s,\t/* %s */' % (hb_ట్యాగ్ (language), format_language_comment (language, ట్యాగ్_2)))
ముద్రించు ('};')
ముద్రించు ()

ముద్రించు ('static const LangTag ot_languages3[] = {')
కొరకు language, ట్యాగ్_2 లో languages3_singles:
	ముద్రించు ('  {%s,\t%s},\t/* %s */' % (
		hb_ట్యాగ్ (language),
		hb_ట్యాగ్ (ట్యాగ్_2),
		format_language_comment (language, ట్యాగ్_2)))
ముద్రించు ('};')
ముద్రించు ()

ముద్రించు ('static const hb_tag_t ot_languages3_multi_values[] = {')
కొరకు language, ట్యాగ్_2 లో languages3_multi_విలువలు:
	ముద్రించు ('  %s,\t/* %s */' % (hb_ట్యాగ్ (ట్యాగ్_2), format_language_comment (language, ట్యాగ్_2)))
ముద్రించు ('};')
ముద్రించు ()

ముద్రించు ('static const LangTagRange ot_languages3_multi[] = {')
కొరకు language, స్థానభ్రంశం, లెక్క, మొదటి_ట్యాగ్ లో languages3_multi_ranges:
	ముద్రించు ('  {%s,\t%u,\t%u},\t/* %s */' % (
		hb_ట్యాగ్ (language),
		పూర్ణాంకం(స్థానభ్రంశం),
		పూర్ణాంకం(లెక్క),
		format_language_comment (language, మొదటి_ట్యాగ్)))
ముద్రించు ('};')
ముద్రించు ('#endif')
ముద్రించు ()

ముద్రించు ('/**')
ముద్రించు (' * hb_ot_tags_from_complex_language:')
ముద్రించు (' * @lang_str: a BCP 47 language tag to convert.')
ముద్రించు (' * @limit: a pointer to the end of the substring of @lang_str to consider for')
ముద్రించు (' * conversion.')
ముద్రించు (' * @count: maximum number of language tags to retrieve (IN) and actual number of')
ముద్రించు (' * language tags retrieved (OUT). If no tags are retrieved, it is not modified.')
ముద్రించు (' * @tags: array of size at least @language_count to store the language tag')
ముద్రించు (' * results')
ముద్రించు (' *')
ముద్రించు (' * Converts a multi-subtag BCP 47 language tag to language tags.')
ముద్రించు (' *')
ముద్రించు (' * Return value: Whether any language systems were retrieved.')
ముద్రించు (' **/')
ముద్రించు ('static inline bool')
ముద్రించు ('hb_ot_tags_from_complex_language (const char   *lang_str,')
ముద్రించు ('\t\t\t\t  const char   *limit,')
ముద్రించు ('\t\t\t\t  unsigned int *count /* IN/OUT */,')
ముద్రించు ('\t\t\t\t  hb_tag_t     *tags /* OUT */)')
ముద్రించు ('{')

నిర్వచనం print_subtag_matches (subtag, string, new_పంక్తి):
	ఒకవేళ subtag:
		ఒకవేళ new_పంక్తి:
			ముద్రించు ()
			ముద్రించు ('\t&& ', చివర='')
		ముద్రించు ('subtag_matches (%s, limit, "-%s", %i)' % (string, sanitize_bcp47_ట్యాగ్ (subtag), 1 + పొడవు (subtag)), చివర='')

complex_ట్యాగులు = collections.defaultdict (list)
కొరకు initial, సమూహం లో itertools.groupby ((lt_ట్యాగులు కొరకు lt_ట్యాగులు లో [
			(Language_ట్యాగ్ (language), ట్యాగులు)
			కొరకు language, ట్యాగులు లో sorted (ot.from_bcp_47.వస్తువులు (),
				కీలకం_2=అజ్ఞాత i: (-పొడవు (i[0]), i[0]))
		] ఒకవేళ lt_ట్యాగులు[0].is_complex ()),
		కీలకం_2=అజ్ఞాత lt_ట్యాగులు: lt_ట్యాగులు[0].get_సమూహం ()):
	complex_ట్యాగులు[initial] += సమూహం

# Calculate the min length of the subtags outside the switch
min_subtag_len = 100
కొరకు initial, వస్తువులు లో sorted (complex_ట్యాగులు.వస్తువులు ()):
	ఒకవేళ initial != 'und':
		కొనసాగు
	కొరకు lt, ట్యాగులు లో వస్తువులు:
		ఒకవేళ కాదు ట్యాగులు:
			కొనసాగు
		subtag_len = 0
		subtag_len += 1 + పొడవు (lt.script) ఒకవేళ lt.script అయిన కాదు ఏమీలేదు లేకపోతే 0
		subtag_len += 1 + పొడవు (lt.region) ఒకవేళ lt.region అయిన కాదు ఏమీలేదు లేకపోతే 0
		subtag_len += 1 + పొడవు (lt.variant) ఒకవేళ lt.variant అయిన కాదు ఏమీలేదు లేకపోతే 0
		min_subtag_len = min(subtag_len, min_subtag_len)

ముద్రించు ('  if (limit - lang_str >= %d)' % (min_subtag_len + 2))
ముద్రించు ('  {')
ముద్రించు ("    const char *p = strchr (lang_str, '-');")
ముద్రించు ("    if (!p || p >= limit || limit - p < %i) goto out;" % min_subtag_len)
కొరకు initial, వస్తువులు లో sorted (complex_ట్యాగులు.వస్తువులు ()):
	ఒకవేళ initial != 'und':
		కొనసాగు
	కొరకు lt, ట్యాగులు లో వస్తువులు:
		ఒకవేళ కాదు ట్యాగులు:
			కొనసాగు
		ఒకవేళ lt.variant లో bcp_47.prefixes:
			expect (తదుపరి (iter (bcp_47.prefixes[lt.variant])) == lt.language,
					'%s is not a valid prefix of %s' % (lt.language, lt.variant))
		ముద్రించు ('    if (', చివర='')
		print_subtag_matches (lt.script, 'p', అబద్ధం)
		print_subtag_matches (lt.region, 'p', అబద్ధం)
		print_subtag_matches (lt.variant, 'p', అబద్ధం)
		ముద్రించు (')')
		ముద్రించు ('    {')
		లిఖ ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		ముద్రించు ()
		ఒకవేళ పొడవు (ట్యాగులు) == 1:
			లిఖ ('      tags[0] = %s;  /* %s */' % (hb_ట్యాగ్ (ట్యాగులు[0]), sanitize_comment (ot.పేర్లు[ట్యాగులు[0]])))
			ముద్రించు ()
			ముద్రించు ('      *count = 1;')
		లేకపోతే:
			ముద్రించు ('    hb_tag_t possible_tags[] = {')
			కొరకు ట్యాగ్_2 లో ట్యాగులు:
				లిఖ ('      %s,  /* %s */' % (hb_ట్యాగ్ (ట్యాగ్_2), sanitize_comment (ot.పేర్లు[ట్యాగ్_2])))
				ముద్రించు ()
			ముద్రించు ('      };')
			ముద్రించు ('      for (i = 0; i < %s && i < *count; i++)' % పొడవు (ట్యాగులు))
			ముద్రించు ('\ttags[i] = possible_tags[i];')
			ముద్రించు ('      *count = i;')
		ముద్రించు ('      return true;')
		ముద్రించు ('    }')
ముద్రించు ('  }')
ముద్రించు ('out:')

ముద్రించు ('  switch (lang_str[0])')
ముద్రించు ('  {')
కొరకు initial, వస్తువులు లో sorted (complex_ట్యాగులు.వస్తువులు ()):
	ఒకవేళ initial == 'und':
		కొనసాగు
	ముద్రించు ("  case '%s':" % sanitize_bcp47_ట్యాగ్ (initial))
	కొరకు lt, ట్యాగులు లో వస్తువులు:
		ఒకవేళ కాదు ట్యాగులు:
			కొనసాగు
		ముద్రించు ('    if (', చివర='')
		script = lt.script
		region = lt.region
		ఒకవేళ lt.grandfathered:
			ముద్రించు ('0 == strcmp (&lang_str[1], "%s")' % sanitize_bcp47_ట్యాగ్ (lt.language[1:]), చివర='')
		లేకపోతే:
			string_literal = lt.language[1:] + '-'
			ఒకవేళ script:
				string_literal += script
				script = ఏమీలేదు
				ఒకవేళ region:
					string_literal += '-' + region
					region = ఏమీలేదు
			ఒకవేళ string_literal[-1] == '-':
				ముద్రించు ('0 == strncmp (&lang_str[1], "%s", %i)' % (sanitize_bcp47_ట్యాగ్ (string_literal), పొడవు (string_literal)), చివర='')
			లేకపోతే:
				ముద్రించు ('lang_matches (&lang_str[1], limit, "%s", %i)' % (sanitize_bcp47_ట్యాగ్ (string_literal), పొడవు (string_literal)), చివర='')
		print_subtag_matches (script, 'lang_str', నిజం)
		print_subtag_matches (region, 'lang_str', నిజం)
		print_subtag_matches (lt.variant, 'lang_str', నిజం)
		ముద్రించు (')')
		ముద్రించు ('    {')
		లిఖ ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		ముద్రించు ()
		ఒకవేళ పొడవు (ట్యాగులు) == 1:
			లిఖ ('      tags[0] = %s;  /* %s */' % (hb_ట్యాగ్ (ట్యాగులు[0]), sanitize_comment (ot.పేర్లు[ట్యాగులు[0]])))
			ముద్రించు ()
			ముద్రించు ('      *count = 1;')
		లేకపోతే:
			ముద్రించు ('      unsigned int i;')
			ముద్రించు ('      hb_tag_t possible_tags[] = {')
			కొరకు ట్యాగ్_2 లో ట్యాగులు:
				లిఖ ('\t%s,  /* %s */' % (hb_ట్యాగ్ (ట్యాగ్_2), sanitize_comment (ot.పేర్లు[ట్యాగ్_2])))
				ముద్రించు ()
			ముద్రించు ('      };')
			ముద్రించు ('      for (i = 0; i < %s && i < *count; i++)' % పొడవు (ట్యాగులు))
			ముద్రించు ('\ttags[i] = possible_tags[i];')
			ముద్రించు ('      *count = i;')
		ముద్రించు ('      return true;')
		ముద్రించు ('    }')
	ముద్రించు ('    break;')

ముద్రించు ('  }')
ముద్రించు ('  return false;')
ముద్రించు ('}')
ముద్రించు ()
ముద్రించు ('/**')
ముద్రించు (' * hb_ot_ambiguous_tag_to_language')
ముద్రించు (' * @tag: A language tag.')
ముద్రించు (' *')
ముద్రించు (' * Converts @tag to a BCP 47 language tag if it is ambiguous (it corresponds to')
ముద్రించు (' * many language tags) and the best tag is not the first (sorted alphabetically,')
ముద్రించు (' * with two-letter tags having priority over all three-letter tags), or if the')
ముద్రించు (' * best tag consists of multiple subtags, or if the best tag does not appear in')
ముద్రించు (' * #ot_languages2 or #ot_languages3.')
ముద్రించు (' *')
ముద్రించు (' * Return value: The #hb_language_t corresponding to the BCP 47 language tag,')
ముద్రించు (' * or #HB_LANGUAGE_INVALID if @tag is not ambiguous.')
ముద్రించు (' **/')
ముద్రించు ('static inline hb_language_t')
ముద్రించు ('hb_ot_ambiguous_tag_to_language (hb_tag_t tag)')
ముద్రించు ('{')
ముద్రించు ('  switch (tag)')
ముద్రించు ('  {')

నిర్వచనం verify_disambiguation_dict ():
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
	వైశ్విక bcp_47
	వైశ్విక disambiguation
	వైశ్విక ot
	కొరకు ot_ట్యాగ్, bcp_47_ట్యాగులు లో ot.గమ్యం_bcp_47.వస్తువులు ():
		ఒకవేళ ot_ట్యాగ్ == DEFAULT_LANGUAGE_SYSTEM:
			primary_ట్యాగులు = []
		లేకపోతే:
			primary_ట్యాగులు = list (t కొరకు t లో bcp_47_ట్యాగులు ఒకవేళ t కాదు లో bcp_47.grandfathered మరియు ot.from_bcp_47.get (t)[0] == ot_ట్యాగ్)
		ఒకవేళ పొడవు (primary_ట్యాగులు) == 1:
			expect (ot_ట్యాగ్ కాదు లో disambiguation, 'unnecessary disambiguation for OT tag: %s' % ot_ట్యాగ్)
			ఒకవేళ '-' లో primary_ట్యాగులు[0]:
				disambiguation[ot_ట్యాగ్] = primary_ట్యాగులు[0]
			లేకపోతే:
				మొదటి_ట్యాగ్ = sorted ((t కొరకు t లో bcp_47_ట్యాగులు ఒకవేళ t కాదు లో bcp_47.grandfathered మరియు ot_ట్యాగ్ లో ot.from_bcp_47.get (t)),
						కీలకం_2=అజ్ఞాత t: (పొడవు (t), t))[0]
				ఒకవేళ primary_ట్యాగులు[0] != మొదటి_ట్యాగ్:
					disambiguation[ot_ట్యాగ్] = primary_ట్యాగులు[0]
		లేదావేళ పొడవు (primary_ట్యాగులు) == 0:
			expect (ot_ట్యాగ్ కాదు లో disambiguation, 'There is no possible valid disambiguation for %s' % ot_ట్యాగ్)
		లేకపోతే:
			original_languages = [t కొరకు t లో primary_ట్యాగులు ఒకవేళ t లో ot.from_bcp_47_uninherited మరియు 'retired code' కాదు లో bcp_47.scopes.get (t, '')]
			ఒకవేళ పొడవు (original_languages) == 1:
				macrolanguages = original_languages
			లేకపోతే:
				macrolanguages = [t కొరకు t లో primary_ట్యాగులు ఒకవేళ bcp_47.scopes.get (t) == ' [macrolanguage]']
			ఒకవేళ పొడవు (macrolanguages) != 1:
				macrolanguages = list (t కొరకు t లో primary_ట్యాగులు ఒకవేళ bcp_47.scopes.get (t) == ' [collection]')
			ఒకవేళ పొడవు (macrolanguages) != 1:
				macrolanguages = list (t కొరకు t లో primary_ట్యాగులు ఒకవేళ 'retired code' కాదు లో bcp_47.scopes.get (t, ''))
			ఒకవేళ పొడవు (macrolanguages) != 1:
				macrolanguages = list (t కొరకు t లో primary_ట్యాగులు ఒకవేళ t.క్రింది () == ISO_639_3_గమ్యం_1.get (ot_ట్యాగ్.క్రింది (), ot_ట్యాగ్.క్రింది ()))
			ఒకవేళ పొడవు (macrolanguages) != 1:
				macrolanguages = list (t కొరకు t లో primary_ట్యాగులు ఒకవేళ '-' కాదు లో t)
			ఒకవేళ పొడవు (macrolanguages) != 1:
				expect (ot_ట్యాగ్ లో disambiguation, 'ambiguous OT tag: %s %s' % (ot_ట్యాగ్, sorted (primary_ట్యాగులు)))
				expect (disambiguation[ot_ట్యాగ్] లో bcp_47_ట్యాగులు,
						'%s is not a valid disambiguation for %s' % (disambiguation[ot_ట్యాగ్], ot_ట్యాగ్))
			లేదావేళ ot_ట్యాగ్ కాదు లో disambiguation:
				disambiguation[ot_ట్యాగ్] = macrolanguages[0]
			ఒకవేళ '-' కాదు లో disambiguation[ot_ట్యాగ్]:
				different_bcp_47_ట్యాగులు = sorted ((t కొరకు t లో bcp_47_ట్యాగులు ఒకవేళ కాదు same_ట్యాగ్ (t, ot.from_bcp_47.get (t))),
						కీలకం_2=అజ్ఞాత t: (పొడవు (t), t))
				ఒకవేళ different_bcp_47_ట్యాగులు మరియు disambiguation[ot_ట్యాగ్] == different_bcp_47_ట్యాగులు[0]:
					తీసివేయి disambiguation[ot_ట్యాగ్]
	కొరకు ot_ట్యాగ్ లో disambiguation.కీలకాలు ():
		expect (ot_ట్యాగ్ లో ot.గమ్యం_bcp_47, 'unknown OT tag: %s' % ot_ట్యాగ్)

verify_disambiguation_dict ()
కొరకు ot_ట్యాగ్, bcp_47_ట్యాగ్ లో sorted (disambiguation.వస్తువులు ()):
	లిఖ ('  case %s:  /* %s */' % (hb_ట్యాగ్ (ot_ట్యాగ్), sanitize_comment (ot.పేర్లు[ot_ట్యాగ్])))
	ముద్రించు ()
	లిఖ ('    return hb_language_from_string (\"%s\", -1);  /* %s */' % (sanitize_bcp47_ట్యాగ్ (bcp_47_ట్యాగ్), sanitize_comment (bcp_47.get_name (Language_ట్యాగ్ (bcp_47_ట్యాగ్)))))
	ముద్రించు ()

ముద్రించు ('  default:')
ముద్రించు ('    return HB_LANGUAGE_INVALID;')
ముద్రించు ('  }')
ముద్రించు ('}')

ముద్రించు ()
ముద్రించు ('#endif /* HB_OT_TAG_TABLE_HH */')
ముద్రించు ()
ముద్రించు ('/* == End of generated table == */')
