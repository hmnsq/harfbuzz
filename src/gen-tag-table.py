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

ಆನಯನ collections
ಆನಯನ html
ತಃ html.parser ಆನಯನ HTMLParser
ಆನಯನ itertools
ಆನಯನ os
ಆನಯನ re
ಆನಯನ sys
ಆನಯನ unicodedata

ಯದಿ len (sys.argv) > 1 ಚ sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = ಸತ್ಯ
ಅನ್ಯಥಾ:
	output_rust = ಅಸತ್ಯ

ಯದಿ len (sys.argv) != 3:
	sys.exit (__doc__)

def expect (condition, message=None):
	ಯದಿ ನ condition:
		ಯದಿ message is None:
			ಉತ್ಥಾಪಯ AssertionError
		ಉತ್ಥಾಪಯ AssertionError (message)

def ಲಿಖ (s):
	sys.stdout.flush ()
	sys.stdout.buffer.ಲಿಖ (s.encode ('utf-8'))

_ALLOWED_COMMENT_PUNCT = set (" -—–.,;:’'ʼ()[]/!=>_+")

def sanitize_comment (s):
	"""Validate that a comment contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ಯದಿ ನ isinstance (s, str):
		ಉತ್ಥಾಪಯ TypeError ("Expected string for comment, got %s" % type (s).__name__)
	s = s.strip ()
	ಯದಿ ನ s:
		ಪ್ರತ್ಯಾಗಮ ''
	ಪ್ರತಿ ch ಅನ್ತಃ s:
		cat = unicodedata.category (ch)
		ಯದಿ ನ (cat.startswith (('L', 'M', 'N', 'Z')) ವಾ ch ಅನ್ತಃ _ALLOWED_COMMENT_PUNCT):
			ಉತ್ಥಾಪಯ ಮೌಲ್ಯ_Error ("Disallowed character %r (U+%04X) in comment: %r" % (ch, ord (ch), s))
	ಪ್ರತ್ಯಾಗಮ s

def sanitize_header (s):
	"""Validate that a header string contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ಯದಿ ನ isinstance (s, str):
		ಉತ್ಥಾಪಯ TypeError ("Expected string for header, got %s" % type (s).__name__)
	s = s.strip ()
	ಯದಿ ನ re.fullmatch (r'[a-zA-Z0-9 <>=/:\-_."]{1,200}', s):
		ಉತ್ಥಾಪಯ ಮೌಲ್ಯ_Error ("Invalid header line %r: contains disallowed characters" % s)
	ಪ್ರತ್ಯಾಗಮ s

def sanitize_bcp47_tag (s):
	"""Validate that a BCP-47 tag or subtag contains only alphanumeric characters and hyphens.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ಯದಿ ನ isinstance (s, str):
		ಉತ್ಥಾಪಯ TypeError ("Expected string, got %s" % type (s).__name__)
	ಯದಿ ನ re.fullmatch (r'[a-zA-Z0-9-]+', s):
		ಉತ್ಥಾಪಯ ಮೌಲ್ಯ_Error ("Invalid BCP-47 tag or subtag literal %r: must contain only alphanumeric characters and hyphens" % s)
	ಪ್ರತ್ಯಾಗಮ s

def sanitize_tag (tag):
	"""Validate that an OpenType tag contains only 1-4 alphanumeric characters or spaces.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ಯದಿ ನ isinstance (tag, str):
		ಉತ್ಥಾಪಯ TypeError ("Expected string for tag, got %s" % type (tag).__name__)
	ಯದಿ tag == DEFAULT_LANGUAGE_SYSTEM:
		ಪ್ರತ್ಯಾಗಮ ''
	ಯದಿ ನ re.fullmatch (r'[a-zA-Z0-9 ]{1,4}', tag):
		ಉತ್ಥಾಪಯ ಮೌಲ್ಯ_Error ("Invalid tag %r: must contain only 1-4 alphanumeric characters or spaces" % tag)
	ಪ್ರತ್ಯಾಗಮ tag

DEFAULT_LANGUAGE_SYSTEM = ''

# from https://www-01.sil.org/iso639-3/iso-639-3.tab
ISO_639_3_TO_1 = {
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

ವರ್ಗ LanguageTag (object):
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
	def __init__ (self, tag):
		ವೈಶ್ವಿಕ bcp_47
		self.subtags = tag.lower ().split ('-')
		self.grandfathered = tag.lower () ಅನ್ತಃ bcp_47.grandfathered
		ಯದಿ self.grandfathered:
			self.language = tag.lower ()
			self.script = ''
			self.region = ''
			self.variant = ''
		ಅನ್ಯಥಾ:
			self.language = self.subtags[0]
			self.script = self._find_first (ಅನಾಮನ್ s: len (s) == 4 ಚ s[0] > '9', self.subtags)
			self.region = self._find_first (ಅನಾಮನ್ s: len (s) == 2 ಚ s[0] > '9' ವಾ len (s) == 3 ಚ s[0] <= '9', self.subtags[1:])
			self.variant = self._find_first (ಅನಾಮನ್ s: len (s) > 4 ವಾ len (s) == 4 ಚ s[0] <= '9', self.subtags)

	def __str__(self):
		ಪ್ರತ್ಯಾಗಮ '-'.join(self.subtags)

	def __repr__ (self):
		ಪ್ರತ್ಯಾಗಮ 'LanguageTag(%r)' % str(self)

	@staticmethod
	def _find_first (function, sequence):
		ಯತ್ನ:
			ಪ್ರತ್ಯಾಗಮ next (iter (filter (function, sequence)))
		ಅಪವಾದ StopIteration:
			ಪ್ರತ್ಯಾಗಮ None

	def is_complex (self):
		"""Return whether this tag is too complex to represent as a
		``LangTag`` in the generated code.

		Complex tags need to be handled in
		``hb_ot_tags_from_complex_language``.

		Returns:
			Whether this tag is complex.
		"""
		ಪ್ರತ್ಯಾಗಮ ನ (len (self.subtags) == 1
			ವಾ self.grandfathered
			ಚ len (self.subtags[1]) != 3
			ಚ ot.from_bcp_47[self.subtags[0]] == ot.from_bcp_47[self.language])

	def get_group (self):
		"""Return the group into which this tag should be categorized in
		``hb_ot_tags_from_complex_language``.

		The group is the first letter of the tag, or ``'und'`` if this tag
		should not be matched in a ``switch`` statement in the generated
		code.

		Returns:
			This tag's group.
		"""
		ಪ್ರತ್ಯಾಗಮ ('und'
			ಯದಿ (self.language == 'und'
				ವಾ self.variant ಅನ್ತಃ bcp_47.prefixes ಚ len (bcp_47.prefixes[self.variant]) == 1)
			ಅನ್ಯಥಾ self.language[0])

ವರ್ಗ OpenTypeRegistryParser (HTMLParser):
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
	def __init__ (self):
		HTMLParser.__init__ (self)
		self.header = ''
		self.names = {}
		self.ranks = collections.defaultdict (ಪೂರ್ಣಾಙ್ಕ)
		self.to_bcp_47 = collections.defaultdict (set)
		self.from_bcp_47 = collections.defaultdict (set)
		self.from_bcp_47_uninherited = None
		# Whether the parser is in a <td> element
		self._td = ಅಸತ್ಯ
		# Whether the parser ignores the rest of the current <td> element
		self._disengaged = ಅಸತ್ಯ
		# The text of the <td> elements of the current <tr> element.
		self._current_tr = []

	def handle_starttag (self, tag, attrs):
		ಯದಿ tag == 'a':
			ಯದಿ self._current_tr ಚ ನ self._disengaged:
				self._current_tr[-1] = ''
				self._disengaged = ಸತ್ಯ
		ನೋಚೇದ್ಯದಿ tag == 'br':
			self._disengaged = ಸತ್ಯ
		ನೋಚೇದ್ಯದಿ tag == 'meta':
			ಪ್ರತಿ attr, ಮೌಲ್ಯ_2 ಅನ್ತಃ attrs:
				ಯದಿ attr == 'name' ಚ ಮೌಲ್ಯ_2 == 'updated_at':
					self.header = self.get_starttag_text ()
					ಭಙ್ಗ
		ನೋಚೇದ್ಯದಿ tag == 'td':
			self._td = ಸತ್ಯ
			self._current_tr.append ('')
		ನೋಚೇದ್ಯದಿ tag == 'tr':
			self._disengaged = ಅಸತ್ಯ
			self._current_tr = []

	def handle_endtag (self, tag):
		ಯದಿ tag == 'td':
			self._td = ಅಸತ್ಯ
			self._disengaged = ಅಸತ್ಯ
		ನೋಚೇದ್ಯದಿ tag == 'tr' ಚ self._current_tr:
			expect (2 <= len (self._current_tr) <= 3)
			name = self._current_tr[0].strip ()
			tag = self._current_tr[1].strip ("\t\n\v\f\r '")
			rank = 0
			ಯದಿ len (tag) > 4:
				expect (tag.endswith (' (deprecated)'), 'ill-formed OpenType tag: %s' % tag)
				name += ' (deprecated)'
				tag = tag.split (' ')[0]
				rank = 1
			self.names[tag] = re.sub (' languages$', '', name)
			ಯದಿ ನ self._current_tr[2]:
				ಪ್ರತ್ಯಾಗಮ
			iso_codes = self._current_tr[2].strip ()
			self.to_bcp_47[tag].update (ISO_639_3_TO_1.get (code, code) ಪ್ರತಿ code ಅನ್ತಃ iso_codes.replace (' ', '').split (','))
			rank += 2 * len (self.to_bcp_47[tag])
			self.ranks[tag] = rank

	def handle_data (self, data):
		ಯದಿ self._td ಚ ನ self._disengaged:
			self._current_tr[-1] += data

	def handle_charref (self, name):
		self.handle_data (html.unescape ('&#%s;' % name))

	def handle_entityref (self, name):
		self.handle_data (html.unescape ('&%s;' % name))

	def parse (self, filename):
		"""Parse the OpenType language system tag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		with open (filename, encoding='utf-8') ಇವ f:
			self.feed (f.ಪಠ ())
		expect (self.header)
		ಪ್ರತಿ tag, iso_codes ಅನ್ತಃ self.to_bcp_47.ವಸ್ತುಗಳು ():
			ಪ್ರತಿ iso_code ಅನ್ತಃ iso_codes:
				self.from_bcp_47[iso_code].add (tag)

	def add_language (self, bcp_47_tag, ot_tag):
		"""Add a language as if it were in the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag. If the tag is more than just
				a language subtag, and if the language subtag is a
				macrolanguage, then new languages are added corresponding
				to the macrolanguages' individual languages with the
				remainder of the tag appended.
			ot_tag (str): An OpenType language system tag.
		"""
		ವೈಶ್ವಿಕ bcp_47
		self.to_bcp_47[ot_tag].add (bcp_47_tag)
		self.from_bcp_47[bcp_47_tag].add (ot_tag)
		ಯದಿ bcp_47_tag.lower () ನ ಅನ್ತಃ bcp_47.grandfathered:
			ಯತ್ನ:
				[macrolanguage, suffix] = bcp_47_tag.split ('-', 1)
				ಯದಿ macrolanguage ಅನ್ತಃ bcp_47.macrolanguages:
					s = set ()
					ಪ್ರತಿ language ಅನ್ತಃ bcp_47.macrolanguages[macrolanguage]:
						ಯದಿ language.lower () ನ ಅನ್ತಃ bcp_47.grandfathered:
							s.add ('%s-%s' % (language, suffix))
					bcp_47.macrolanguages['%s-%s' % (macrolanguage, suffix)] = s
			ಅಪವಾದ ಮೌಲ್ಯ_Error:
				pass

	@staticmethod
	def _remove_language (tag_1, dict_1, dict_2):
		ಪ್ರತಿ tag_2 ಅನ್ತಃ dict_1.ತೆಗೆ (tag_1):
			dict_2[tag_2].remove (tag_1)
			ಯದಿ ನ dict_2[tag_2]:
				del dict_2[tag_2]

	def remove_language_ot (self, ot_tag):
		"""Remove an OpenType tag from the registry.

		Args:
			ot_tag (str): An OpenType tag.
		"""
		self._remove_language (ot_tag, self.to_bcp_47, self.from_bcp_47)

	def remove_language_bcp_47 (self, bcp_47_tag):
		"""Remove a BCP 47 tag from the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag.
		"""
		self._remove_language (bcp_47_tag, self.from_bcp_47, self.to_bcp_47)

	def inherit_from_macrolanguages (self):
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
		ವೈಶ್ವಿಕ bcp_47
		first_time = self.from_bcp_47_uninherited is None
		ಯದಿ first_time:
			self.from_bcp_47_uninherited = dict (self.from_bcp_47)
		ಪ್ರತಿ macrolanguage, languages ಅನ್ತಃ dict (bcp_47.macrolanguages).ವಸ್ತುಗಳು ():
			ot_macrolanguages = {
				ot_macrolanguage ಪ್ರತಿ ot_macrolanguage ಅನ್ತಃ self.from_bcp_47_uninherited.get (macrolanguage, set ())
			}
			blocked_ot_macrolanguages = set ()
			ಯದಿ 'retired code' ನ ಅನ್ತಃ bcp_47.scopes.get (macrolanguage, ''):
				ಪ್ರತಿ ot_macrolanguage ಅನ್ತಃ ot_macrolanguages:
					round_trip_macrolanguages = {
						l ಪ್ರತಿ l ಅನ್ತಃ self.to_bcp_47[ot_macrolanguage]
						ಯದಿ 'retired code' ನ ಅನ್ತಃ bcp_47.scopes.get (l, '')
					}
					round_trip_languages = {
						l ಪ್ರತಿ l ಅನ್ತಃ languages
						ಯದಿ 'retired code' ನ ಅನ್ತಃ bcp_47.scopes.get (l, '')
					}
					intersection = round_trip_macrolanguages & round_trip_languages
					ಯದಿ intersection ಚ intersection != round_trip_languages:
						blocked_ot_macrolanguages.add (ot_macrolanguage)
			ಯದಿ ot_macrolanguages:
				ಪ್ರತಿ ot_macrolanguage ಅನ್ತಃ ot_macrolanguages:
					ಯದಿ ot_macrolanguage ನ ಅನ್ತಃ blocked_ot_macrolanguages:
						ಪ್ರತಿ language ಅನ್ತಃ languages:
							self.add_language (language, ot_macrolanguage)
							ಯದಿ ನ blocked_ot_macrolanguages:
								self.ranks[ot_macrolanguage] += 1
			ನೋಚೇದ್ಯದಿ first_time:
				ಪ್ರತಿ language ಅನ್ತಃ languages:
					ಯದಿ language ಅನ್ತಃ self.from_bcp_47_uninherited:
						ot_macrolanguages |= self.from_bcp_47_uninherited[language]
					ಅನ್ಯಥಾ:
						ot_macrolanguages.clear ()
					ಯದಿ ನ ot_macrolanguages:
						ಭಙ್ಗ
				ಪ್ರತಿ ot_macrolanguage ಅನ್ತಃ ot_macrolanguages:
					self.add_language (macrolanguage, ot_macrolanguage)

	def sort_languages (self):
		"""Sort the values of ``from_bcp_47`` in ascending rank order."""
		ಪ್ರತಿ language, tags ಅನ್ತಃ self.from_bcp_47.ವಸ್ತುಗಳು ():
			self.from_bcp_47[language] = sorted (tags,
					ಕೀಲಿ_2=ಅನಾಮನ್ t: (self.ranks[t] + rank_delta (language, t), t))

ot = OpenTypeRegistryParser ()

ವರ್ಗ BCP47Parser (object):
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
	def __init__ (self):
		self.header = ''
		self.names = {}
		self.scopes = {}
		self.macrolanguages = collections.defaultdict (set)
		self.prefixes = collections.defaultdict (set)
		self.grandfathered = set ()

	def parse (self, filename):
		"""Parse the BCP 47 subtag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		with open (filename, encoding='utf-8') ಇವ f:
			subtag_type = None
			subtag = None
			deprecated = ಅಸತ್ಯ
			has_preferred_ಮೌಲ್ಯ = ಅಸತ್ಯ
			ಸಾಲು_buffer = ''
			ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ itertools.chain (f, ['']):
				ಸಾಲು = ಸಾಲು.rstrip ()
				ಯದಿ ಸಾಲು.startswith (' '):
					ಸಾಲು_buffer += ಸಾಲು[1:]
					ಅನುವರ್ತ
				ಸಾಲು, ಸಾಲು_buffer = ಸಾಲು_buffer, ಸಾಲು
				ಯದಿ ಸಾಲು.startswith ('Type: '):
					subtag_type = ಸಾಲು.split (' ')[1]
					deprecated = ಅಸತ್ಯ
					has_preferred_ಮೌಲ್ಯ = ಅಸತ್ಯ
				ನೋಚೇದ್ಯದಿ ಸಾಲು.startswith ('Subtag: ') ವಾ ಸಾಲು.startswith ('Tag: '):
					subtag = ಸಾಲು.split (' ')[1]
					ಯದಿ subtag_type == 'grandfathered':
						self.grandfathered.add (subtag.lower ())
				ನೋಚೇದ್ಯದಿ ಸಾಲು.startswith ('Description: '):
					description = ಸಾಲು.split (' ', 1)[1].replace (' (individual language)', '')
					description = re.sub (r' (\(family\)|\((individual |macro)language\)|languages)$', '',
							description)
					ಯದಿ subtag ಅನ್ತಃ self.names:
						self.names[subtag] += '\n' + description
					ಅನ್ಯಥಾ:
						self.names[subtag] = description
				ನೋಚೇದ್ಯದಿ subtag_type == 'language' ವಾ subtag_type == 'grandfathered':
					ಯದಿ ಸಾಲು.startswith ('Scope: '):
						scope = ಸಾಲು.split (' ')[1]
						ಯದಿ scope == 'macrolanguage':
							scope = ' [macrolanguage]'
						ನೋಚೇದ್ಯದಿ scope == 'collection':
							scope = ' [collection]'
						ಅನ್ಯಥಾ:
							ಅನುವರ್ತ
						self.scopes[subtag] = scope
					ನೋಚೇದ್ಯದಿ ಸಾಲು.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
						deprecated = ಸತ್ಯ
					ನೋಚೇದ್ಯದಿ deprecated ಚ ಸಾಲು.startswith ('Comments: see '):
						# If a subtag is split into multiple replacement subtags,
						# it essentially represents a macrolanguage.
						ಪ್ರತಿ language ಅನ್ತಃ ಸಾಲು.replace (',', '').split (' ')[2:]:
							self._add_macrolanguage (subtag, language)
					ನೋಚೇದ್ಯದಿ ಸಾಲು.startswith ('Preferred-Value: '):
						# If a subtag is deprecated in favor of a single replacement subtag,
						# it is either a dialect or synonym of the preferred subtag. Either
						# way, it is close enough to the truth to consider the replacement
						# the macrolanguage of the deprecated language.
						has_preferred_ಮೌಲ್ಯ = ಸತ್ಯ
						macrolanguage = ಸಾಲು.split (' ')[1]
						self._add_macrolanguage (macrolanguage, subtag)
					ನೋಚೇದ್ಯದಿ ನ has_preferred_ಮೌಲ್ಯ ಚ ಸಾಲು.startswith ('Macrolanguage: '):
						self._add_macrolanguage (ಸಾಲು.split (' ')[1], subtag)
				ನೋಚೇದ್ಯದಿ subtag_type == 'variant':
					ಯದಿ ಸಾಲು.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
					ನೋಚೇದ್ಯದಿ ಸಾಲು.startswith ('Prefix: '):
						self.prefixes[subtag].add (ಸಾಲು.split (' ')[1])
				ನೋಚೇದ್ಯದಿ ಸಾಲು.startswith ('File-Date: '):
					self.header = ಸಾಲು
		expect (self.header)

	def _add_macrolanguage (self, macrolanguage, language):
		ವೈಶ್ವಿಕ ot
		ಯದಿ language ನ ಅನ್ತಃ ot.from_bcp_47:
			ಪ್ರತಿ l ಅನ್ತಃ self.macrolanguages.get (language, set ()):
				self._add_macrolanguage (macrolanguage, l)
		ಯದಿ macrolanguage ನ ಅನ್ತಃ ot.from_bcp_47:
			ಪ್ರತಿ ls ಅನ್ತಃ list (self.macrolanguages.ಮೌಲ್ಯಗಳು ()):
				ಯದಿ macrolanguage ಅನ್ತಃ ls:
					ls.add (language)
					ಪ್ರತ್ಯಾಗಮ
		self.macrolanguages[macrolanguage].add (language)

	def remove_extra_macrolanguages (self):
		"""Make every language have at most one macrolanguage."""
		inverted = collections.defaultdict (list)
		ಪ್ರತಿ macrolanguage, languages ಅನ್ತಃ self.macrolanguages.ವಸ್ತುಗಳು ():
			ಪ್ರತಿ language ಅನ್ತಃ languages:
				inverted[language].append (macrolanguage)
		ಪ್ರತಿ language, macrolanguages ಅನ್ತಃ inverted.ವಸ್ತುಗಳು ():
			ಯದಿ len (macrolanguages) > 1:
				macrolanguages.sort (ಕೀಲಿ_2=ಅನಾಮನ್ ml: len (self.macrolanguages[ml]))
				biggest_macrolanguage = macrolanguages.ತೆಗೆ ()
				ಪ್ರತಿ macrolanguage ಅನ್ತಃ macrolanguages:
					self._add_macrolanguage (biggest_macrolanguage, macrolanguage)

	def _get_name_piece (self, subtag):
		"""Return the first name of a subtag plus its scope suffix.

		Args:
			subtag (str): A BCP 47 subtag.

		Returns:
			The name form of ``subtag``.
		"""
		ಪ್ರತ್ಯಾಗಮ self.names[subtag].split ('\n')[0] + self.scopes.get (subtag, '')

	def get_name (self, lt):
		"""Return the names of the subtags in a language tag.

		Args:
			lt (LanguageTag): A BCP 47 language tag.

		Returns:
			The name form of ``lt``.
		"""
		name = self._get_name_piece (lt.language)
		ಯದಿ lt.script:
			name += '; ' + self._get_name_piece (lt.script.title ())
		ಯದಿ lt.region:
			name += '; ' + self._get_name_piece (lt.region.upper ())
		ಯದಿ lt.variant:
			name += '; ' + self._get_name_piece (lt.variant)
		ಪ್ರತ್ಯಾಗಮ name

bcp_47 = BCP47Parser ()

ot.parse (sys.argv[1])
bcp_47.parse (sys.argv[2])

ot.add_language ('ary', 'MOR')

ot.add_language ('ath', 'ATH')

ot.add_language ('bai', 'BML')

ot.ranks['BAL'] = ot.ranks['KAR'] + 1

ot.add_language ('ber', 'BBR')

ot.remove_language_ot ('PGR')
ot.add_language ('el-polyton', 'PGR')

bcp_47.names['flm'] = 'Falam Chin'
bcp_47.scopes['flm'] = ' (retired code)'
bcp_47.macrolanguages['flm'] = {'cfm'}

ot.ranks['FNE'] = ot.ranks['TNE'] + 1

ot.add_language ('und-fonipa', 'IPPH')

ot.add_language ('und-fonnapa', 'APPH')

ot.add_language ('und-fonupa', 'UPPH')

ot.add_language ('ga-Latg', 'IRT')

ot.add_language ('hy-arevmda', 'HYE')

ot.remove_language_ot ('KGE')
ot.add_language ('und-Geok', 'KGE')

ot.add_language ('kht', 'KHN')
ot.names['KHN'] = ot.names['KHT'] + ' (Microsoft fonts)'
ot.ranks['KHN'] = ot.ranks['KHT'] + 1

ot.ranks['LCR'] = ot.ranks['MCR'] + 1

ot.names['MAL'] = 'Malayalam Traditional'
ot.ranks['MLR'] += 1

bcp_47.names['mhv'] = 'Arakanese'
bcp_47.scopes['mhv'] = ' (retired code)'

ot.add_language ('mnw-TH', 'MONT')

ot.add_language ('no', 'NOR')

ot.add_language ('oc-provenc', 'PRO')

ot.remove_language_ot ('QUZ')
ot.add_language ('qu', 'QUZ')
ot.add_language ('qub', 'QWH')
ot.add_language ('qud', 'QVI')
ot.add_language ('qug', 'QVI')
ot.add_language ('qul', 'QUH')
ot.add_language ('qup', 'QVI')
ot.add_language ('qur', 'QWH')
ot.add_language ('qus', 'QUH')
ot.add_language ('quw', 'QVI')
ot.add_language ('qux', 'QWH')
ot.add_language ('qva', 'QWH')
ot.add_language ('qvh', 'QWH')
ot.add_language ('qvj', 'QVI')
ot.add_language ('qvl', 'QWH')
ot.add_language ('qvm', 'QWH')
ot.add_language ('qvn', 'QWH')
ot.add_language ('qvo', 'QVI')
ot.add_language ('qvp', 'QWH')
ot.add_language ('qvw', 'QWH')
ot.add_language ('qvz', 'QVI')
ot.add_language ('qwa', 'QWH')
ot.add_language ('qws', 'QWH')
ot.add_language ('qxa', 'QWH')
ot.add_language ('qxc', 'QWH')
ot.add_language ('qxh', 'QWH')
ot.add_language ('qxl', 'QVI')
ot.add_language ('qxn', 'QWH')
ot.add_language ('qxo', 'QWH')
ot.add_language ('qxr', 'QVI')
ot.add_language ('qxt', 'QWH')
ot.add_language ('qxw', 'QWH')

bcp_47.macrolanguages['ro-MD'].add ('mo')

ot.remove_language_ot ('SYRE')
ot.remove_language_ot ('SYRJ')
ot.remove_language_ot ('SYRN')
ot.add_language ('und-Syre', 'SYRE')
ot.add_language ('und-Syrj', 'SYRJ')
ot.add_language ('und-Syrn', 'SYRN')

bcp_47.names['xst'] = "Silt'e"
bcp_47.scopes['xst'] = ' (retired code)'
bcp_47.macrolanguages['xst'] = {'stv', 'wle'}

ot.add_language ('xwo', 'TOD')

ot.remove_language_ot ('ZHH')
ot.remove_language_ot ('ZHP')
ot.remove_language_ot ('ZHT')
ot.remove_language_ot ('ZHTM')
bcp_47.macrolanguages['zh'].remove ('lzh')
bcp_47.macrolanguages['zh'].remove ('yue')
ot.add_language ('zh-Hant-MO', 'ZHH')
ot.add_language ('zh-Hant-MO', 'ZHTM')
ot.add_language ('zh-Hant-HK', 'ZHH')
ot.add_language ('zh-Hans', 'ZHS')
ot.add_language ('zh-Hant', 'ZHT')
ot.add_language ('zh-HK', 'ZHH')
ot.add_language ('zh-MO', 'ZHH')
ot.add_language ('zh-MO', 'ZHTM')
ot.add_language ('zh-TW', 'ZHT')
ot.add_language ('lzh', 'ZHT')
ot.add_language ('lzh-Hans', 'ZHS')
ot.add_language ('yue', 'ZHH')
ot.add_language ('yue-Hans', 'ZHS')

def rank_delta (bcp_47, ot):
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
	ಯದಿ bcp_47 == 'ak' ಚ ot == 'AKA':
		ಪ್ರತ್ಯಾಗಮ -1
	ಯದಿ bcp_47 == 'tw' ಚ ot == 'TWI':
		ಪ್ರತ್ಯಾಗಮ -1
	ಪ್ರತ್ಯಾಗಮ 0

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
ot.names[DEFAULT_LANGUAGE_SYSTEM] = ''
ot.ranks[DEFAULT_LANGUAGE_SYSTEM] = max (ot.ranks.ಮೌಲ್ಯಗಳು ()) + 1
ಪ್ರತಿ tricky_ot_tag ಅನ್ತಃ filter (ಅನಾಮನ್ tag: re.match ('[A-Z]{3}$', tag), ot.names):
	possible_bcp_47_tag = tricky_ot_tag.lower ()
	ಯದಿ possible_bcp_47_tag ಅನ್ತಃ bcp_47.names ಚ ನ ot.from_bcp_47[possible_bcp_47_tag]:
		ot.add_language (possible_bcp_47_tag, DEFAULT_LANGUAGE_SYSTEM)
		bcp_47.macrolanguages[possible_bcp_47_tag] = set ()
ot.sort_languages ()

ಯದಿ output_rust:
	def hb_tag_rust (tag):
		ಯದಿ tag == DEFAULT_LANGUAGE_SYSTEM:
			ಪ್ರತ್ಯಾಗಮ 'Tag::new(&[0; 4])'
		sanitize_tag (tag)
		ಪ್ರತ್ಯಾಗಮ 'Tag::new(b"%s%s%s%s")' % tuple (('%-4s' % tag)[:4])

	def get_variant_set_rust (name):
		ಪ್ರತ್ಯಾಗಮ set (unicodedata.normalize (
					'NFD',
					n.replace ('\u02BC', "'").replace ('\u2019', "'"),
				)
				.encode ('ASCII', 'ignore')
				.strip ()
				ಪ್ರತಿ n ಅನ್ತಃ re.split ('[\n(),]', name) ಯದಿ n)

	def language_name_intersection_rust (a, b):
		ಪ್ರತ್ಯಾಗಮ get_variant_set_rust (a).intersection (get_variant_set_rust (b))

	def get_matching_language_name_rust (intersection, candidates):
		ಪ್ರತ್ಯಾಗಮ next (iter (c ಪ್ರತಿ c ಅನ್ತಃ candidates ಯದಿ ನ intersection.isdisjoint (get_variant_set_rust (c))))

	print ('// WARNING: this file was generated by ../../harfbuzz/src/gen-tag-table.py --rust')
	print ()
	print ('use read_fonts::types::Tag;')
	print ()
	print ('pub(crate) struct LangTag {')
	print ('    pub language: [u8; 4],')
	print ('    pub tag: Tag,')
	print ('}')
	print ()
	print ('const fn lang(language: &[u8]) -> [u8; 4] {')
	print ('    let mut bytes = [0; 4];')
	print ('    let mut i = 0;')
	print ()
	print ('    while i < language.len() {')
	print ('        bytes[i] = language[i];')
	print ('        i += 1;')
	print ('    }')
	print ()
	print ('    bytes')
	print ('}')
	print ()
	print ('use super::tag::{lang_matches, strncmp, subtag_matches};')
	print ()
	print ('#[rustfmt::skip]')
	print ('pub(crate) static OPEN_TYPE_LANGUAGES: &[LangTag] = &[')

	ಪ್ರತಿ language, tags ಅನ್ತಃ sorted (ot.from_bcp_47.ವಸ್ತುಗಳು ()):
		ಯದಿ language == '' ವಾ '-' ಅನ್ತಃ language:
			ಅನುವರ್ತ
		commented_out = len (language) == 3 ಚ len (tags) == 1 ಚ language == tags[0].lower ()
		ಪ್ರತಿ tag ಅನ್ತಃ tags:
			print ('%sLangTag { language: lang(b"%s"), \ttag: %s },' % ('//  ' ಯದಿ commented_out ಅನ್ಯಥಾ '    ', sanitize_bcp47_tag (language), hb_tag_rust (tag)), end='')
			print (' // ', end='')
			bcp_47_name = bcp_47.names.get (language, '')
			bcp_47_name_candidates = bcp_47_name.split ('\n')
			ot_name = ot.names[tag]
			scope = bcp_47.scopes.get (language, '')
			ಯದಿ tag == DEFAULT_LANGUAGE_SYSTEM:
				comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
			ಅನ್ಯಥಾ:
				intersection = language_name_intersection_rust (bcp_47_name, ot_name)
				ಯದಿ ನ intersection:
					comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
				ಅನ್ಯಥಾ:
					name = get_matching_language_name_rust (intersection, bcp_47_name_candidates)
					bcp_47.names[language] = name
					comment = '%s%s' % (name ಯದಿ len (name) > len (ot_name) ಅನ್ಯಥಾ ot_name, scope)
			print (sanitize_comment (comment))

	print ('];')
	print ()
	print ('/// Converts a multi-subtag BCP 47 language tag to language tags.')
	print ('pub fn tags_from_complex_language(language: &str, tags: &mut smallvec::SmallVec<[Tag; 3]>) -> bool {')

	def print_subtag_matches_rust (subtag, new_ಸಾಲು):
		ಯದಿ subtag:
			ಯದಿ new_ಸಾಲು:
				print (' && ', end='')
			print ('subtag_matches(language, "-%s")' % sanitize_bcp47_tag (subtag), end='')

	complex_tags = collections.defaultdict (list)
	ಪ್ರತಿ initial, group ಅನ್ತಃ itertools.groupby ((lt_tags ಪ್ರತಿ lt_tags ಅನ್ತಃ [
				(LanguageTag (language), tags)
				ಪ್ರತಿ language, tags ಅನ್ತಃ sorted (ot.from_bcp_47.ವಸ್ತುಗಳು (),
					ಕೀಲಿ_2=ಅನಾಮನ್ i: (-len (i[0]), i[0]))
			] ಯದಿ lt_tags[0].is_complex ()),
			ಕೀಲಿ_2=ಅನಾಮನ್ lt_tags: lt_tags[0].get_group ()):
		complex_tags[initial] += group

	ಪ್ರತಿ initial, ವಸ್ತುಗಳು ಅನ್ತಃ sorted (complex_tags.ವಸ್ತುಗಳು ()):
		ಯದಿ initial != 'und':
			ಅನುವರ್ತ
		ಪ್ರತಿ lt, tags ಅನ್ತಃ ವಸ್ತುಗಳು:
			ಯದಿ ನ tags:
				ಅನುವರ್ತ
			ಯದಿ lt.variant ಅನ್ತಃ bcp_47.prefixes:
				expect (next (iter (bcp_47.prefixes[lt.variant])) == lt.language,
						'%s is not a valid prefix of %s' % (lt.language, lt.variant))
			print ('    if ', end='')
			print_subtag_matches_rust (lt.script, ಅಸತ್ಯ)
			print_subtag_matches_rust (lt.region, ಅಸತ್ಯ)
			print_subtag_matches_rust (lt.variant, ಅಸತ್ಯ)
			print (' {')
			print ('        // %s' % sanitize_comment (bcp_47.get_name (lt)))
			ಯದಿ len (tags) == 1:
				print ('        tags.push(%s); // %s' % (hb_tag_rust (tags[0]), sanitize_comment (ot.names[tags[0]])))
			ಅನ್ಯಥಾ:
				print ('        let possible_tags = &[')
				ಪ್ರತಿ tag ಅನ್ತಃ tags:
					print ('            %s, // %s' % (hb_tag_rust (tag), sanitize_comment (ot.names[tag])))
				print ('        ];')
				print ('        tags.extend_from_slice(possible_tags);')
			print ('        return true;')
			print ('    }')

	print ('    match language.as_bytes()[0] {')
	ಪ್ರತಿ initial, ವಸ್ತುಗಳು ಅನ್ತಃ sorted (complex_tags.ವಸ್ತುಗಳು ()):
		ಯದಿ initial == 'und':
			ಅನುವರ್ತ
		print ("        b'%s' => {" % sanitize_bcp47_tag (initial))
		ಪ್ರತಿ lt, tags ಅನ್ತಃ ವಸ್ತುಗಳು:
			ಯದಿ ನ tags:
				ಅನುವರ್ತ
			print ('            if ', end='')
			script = lt.script
			region = lt.region
			ಯದಿ lt.grandfathered:
				print ('&language[1..] == "%s"' % sanitize_bcp47_tag (lt.language[1:]), end='')
			ಅನ್ಯಥಾ:
				string_literal = lt.language[1:] + '-'
				ಯದಿ script:
					string_literal += script
					script = None
					ಯದಿ region:
						string_literal += '-' + region
						region = None
				ಯದಿ string_literal[-1] == '-':
					print ('strncmp(&language[1..], "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
				ಅನ್ಯಥಾ:
					print ('lang_matches(&language[1..], "%s")' % sanitize_bcp47_tag (string_literal), end='')
			print_subtag_matches_rust (script, ಸತ್ಯ)
			print_subtag_matches_rust (region, ಸತ್ಯ)
			print_subtag_matches_rust (lt.variant, ಸತ್ಯ)
			print (' {')
			print ('                // %s' % sanitize_comment (bcp_47.get_name (lt)))
			ಯದಿ len (tags) == 1:
				print ('                tags.push(%s); // %s' % (hb_tag_rust (tags[0]), sanitize_comment (ot.names[tags[0]])))
			ಅನ್ಯಥಾ:
				print ('                let possible_tags = &[')
				ಪ್ರತಿ tag ಅನ್ತಃ tags:
					print ('                    %s, // %s' % (hb_tag_rust (tag), sanitize_comment (ot.names[tag])))
				print ('                ];')
				print ('                tags.extend_from_slice(possible_tags);')
			print ('                return true;')
			print ('            }')
		print ('        }')
	print ('        _ => {}')
	print ('    }')
	print ('    false')
	print ('}')
	sys.exit (0)

print ('/* == Start of generated table == */')
print ('/*')
print (' * The following table is generated by running:')
print (' *')
print (' *   %s languagetags language-subtag-registry' % os.path.basename (sys.argv[0]))
print (' *')
print (' * on files with these headers:')
print (' *')
print (' * %s' % sanitize_header (ot.header))
print (' * %s' % sanitize_header (bcp_47.header))
print (' */')
print ()
print ('#ifndef HB_OT_TAG_TABLE_HH')
print ('#define HB_OT_TAG_TABLE_HH')
print ()

def hb_tag (tag):
	"""Convert a tag to ``HB_TAG`` form.

	Args:
		tag (str): An OpenType tag.

	Returns:
		A snippet of C++ representing ``tag``.
	"""
	ಯದಿ tag == DEFAULT_LANGUAGE_SYSTEM:
		ಪ್ರತ್ಯಾಗಮ 'HB_TAG_NONE\t       '
	sanitize_tag (tag)
	ಪ್ರತ್ಯಾಗಮ "HB_TAG('%s','%s','%s','%s')" % tuple (('%-4s' % tag)[:4])

def get_variant_set (name):
	"""Return a set of variant language names from a name.

	Args:
		name (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		A set of normalized language names.
	"""
	ಪ್ರತ್ಯಾಗಮ set (unicodedata.normalize (
				'NFD',
				n.replace ('\u02BC', "'").replace ('\u2019', "'"),
			)
			.encode ('ASCII', 'ignore')
			.strip ()
			ಪ್ರತಿ n ಅನ್ತಃ re.split ('[\n(),]', name) ಯದಿ n)

def language_name_intersection (a, b):
	"""Return the names in common between two language names.

	Args:
		a (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.
		b (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		The normalized language names shared by ``a`` and ``b``.
	"""
	ಪ್ರತ್ಯಾಗಮ get_variant_set (a).intersection (get_variant_set (b))

def get_matching_language_name (intersection, candidates):
	ಪ್ರತ್ಯಾಗಮ next (iter (c ಪ್ರತಿ c ಅನ್ತಃ candidates ಯದಿ ನ intersection.isdisjoint (get_variant_set (c))))

def same_tag (bcp_47_tag, ot_tags):
	ಪ್ರತ್ಯಾಗಮ len (bcp_47_tag) == 3 ಚ len (ot_tags) == 1 ಚ bcp_47_tag == ot_tags[0].lower ()

def format_language_comment (language, tag):
	bcp_47_name = bcp_47.names.get (language, '')
	bcp_47_name_candidates = bcp_47_name.split ('\n')
	ot_name = ot.names[tag]
	scope = bcp_47.scopes.get (language, '')
	ಯದಿ tag == DEFAULT_LANGUAGE_SYSTEM:
		comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
	ಅನ್ಯಥಾ:
		intersection = language_name_intersection (bcp_47_name, ot_name)
		ಯದಿ ನ intersection:
			comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
		ಅನ್ಯಥಾ:
			name = get_matching_language_name (intersection, bcp_47_name_candidates)
			bcp_47.names[language] = name
			comment = '%s%s' % (name ಯದಿ len (name) > len (ot_name) ಅನ್ಯಥಾ ot_name, scope)
	ಪ್ರತ್ಯಾಗಮ sanitize_comment (comment)

print ('static const LangTag ot_languages2[] = {')
ಪ್ರತಿ language, tags ಅನ್ತಃ sorted (ot.from_bcp_47.ವಸ್ತುಗಳು ()):
	ಯದಿ language == '' ವಾ '-' ಅನ್ತಃ language ವಾ len (language) != 2:
		ಅನುವರ್ತ
	ಪ್ರತಿ tag ಅನ್ತಃ tags:
		print ('  {%s,\t%s},\t/* %s */' % (
			hb_tag (language),
			hb_tag (tag),
			format_language_comment (language, tag)))
print ('};')
print ()

print ('#ifndef HB_NO_LANGUAGE_LONG')

languages3_blocked = []
languages3_singles = []
languages3_multi_ranges = []
languages3_multi_ಮೌಲ್ಯಗಳು = []
ಪ್ರತಿ language, tags ಅನ್ತಃ sorted (ot.from_bcp_47.ವಸ್ತುಗಳು ()):
	ಯದಿ language == '' ವಾ '-' ಅನ್ತಃ language ವಾ len (language) != 3:
		ಅನುವರ್ತ
	ಯದಿ same_tag (language, tags):
		ಅನುವರ್ತ
	ಯದಿ tags == [DEFAULT_LANGUAGE_SYSTEM]:
		languages3_blocked.append ((language, tags[0]))
		ಅನುವರ್ತ
	ಯದಿ len (tags) == 1:
		languages3_singles.append ((language, tags[0]))
		ಅನುವರ್ತ
	offset = len (languages3_multi_ಮೌಲ್ಯಗಳು)
	languages3_multi_ಮೌಲ್ಯಗಳು.extend ((language, tag) ಪ್ರತಿ tag ಅನ್ತಃ tags)
	languages3_multi_ranges.append ((language, offset, len (tags), tags[0]))

print ('static const hb_tag_t ot_languages3_blocked[] = {')
ಪ್ರತಿ language, tag ಅನ್ತಃ languages3_blocked:
	print ('  %s,\t/* %s */' % (hb_tag (language), format_language_comment (language, tag)))
print ('};')
print ()

print ('static const LangTag ot_languages3[] = {')
ಪ್ರತಿ language, tag ಅನ್ತಃ languages3_singles:
	print ('  {%s,\t%s},\t/* %s */' % (
		hb_tag (language),
		hb_tag (tag),
		format_language_comment (language, tag)))
print ('};')
print ()

print ('static const hb_tag_t ot_languages3_multi_values[] = {')
ಪ್ರತಿ language, tag ಅನ್ತಃ languages3_multi_ಮೌಲ್ಯಗಳು:
	print ('  %s,\t/* %s */' % (hb_tag (tag), format_language_comment (language, tag)))
print ('};')
print ()

print ('static const LangTagRange ot_languages3_multi[] = {')
ಪ್ರತಿ language, offset, ಎಣಿಕೆ, first_tag ಅನ್ತಃ languages3_multi_ranges:
	print ('  {%s,\t%u,\t%u},\t/* %s */' % (
		hb_tag (language),
		ಪೂರ್ಣಾಙ್ಕ(offset),
		ಪೂರ್ಣಾಙ್ಕ(ಎಣಿಕೆ),
		format_language_comment (language, first_tag)))
print ('};')
print ('#endif')
print ()

print ('/**')
print (' * hb_ot_tags_from_complex_language:')
print (' * @lang_str: a BCP 47 language tag to convert.')
print (' * @limit: a pointer to the end of the substring of @lang_str to consider for')
print (' * conversion.')
print (' * @count: maximum number of language tags to retrieve (IN) and actual number of')
print (' * language tags retrieved (OUT). If no tags are retrieved, it is not modified.')
print (' * @tags: array of size at least @language_count to store the language tag')
print (' * results')
print (' *')
print (' * Converts a multi-subtag BCP 47 language tag to language tags.')
print (' *')
print (' * Return value: Whether any language systems were retrieved.')
print (' **/')
print ('static inline bool')
print ('hb_ot_tags_from_complex_language (const char   *lang_str,')
print ('\t\t\t\t  const char   *limit,')
print ('\t\t\t\t  unsigned int *count /* IN/OUT */,')
print ('\t\t\t\t  hb_tag_t     *tags /* OUT */)')
print ('{')

def print_subtag_matches (subtag, string, new_ಸಾಲು):
	ಯದಿ subtag:
		ಯದಿ new_ಸಾಲು:
			print ()
			print ('\t&& ', end='')
		print ('subtag_matches (%s, limit, "-%s", %i)' % (string, sanitize_bcp47_tag (subtag), 1 + len (subtag)), end='')

complex_tags = collections.defaultdict (list)
ಪ್ರತಿ initial, group ಅನ್ತಃ itertools.groupby ((lt_tags ಪ್ರತಿ lt_tags ಅನ್ತಃ [
			(LanguageTag (language), tags)
			ಪ್ರತಿ language, tags ಅನ್ತಃ sorted (ot.from_bcp_47.ವಸ್ತುಗಳು (),
				ಕೀಲಿ_2=ಅನಾಮನ್ i: (-len (i[0]), i[0]))
		] ಯದಿ lt_tags[0].is_complex ()),
		ಕೀಲಿ_2=ಅನಾಮನ್ lt_tags: lt_tags[0].get_group ()):
	complex_tags[initial] += group

# Calculate the min length of the subtags outside the switch
min_subtag_len = 100
ಪ್ರತಿ initial, ವಸ್ತುಗಳು ಅನ್ತಃ sorted (complex_tags.ವಸ್ತುಗಳು ()):
	ಯದಿ initial != 'und':
		ಅನುವರ್ತ
	ಪ್ರತಿ lt, tags ಅನ್ತಃ ವಸ್ತುಗಳು:
		ಯದಿ ನ tags:
			ಅನುವರ್ತ
		subtag_len = 0
		subtag_len += 1 + len (lt.script) ಯದಿ lt.script is ನ None ಅನ್ಯಥಾ 0
		subtag_len += 1 + len (lt.region) ಯದಿ lt.region is ನ None ಅನ್ಯಥಾ 0
		subtag_len += 1 + len (lt.variant) ಯದಿ lt.variant is ನ None ಅನ್ಯಥಾ 0
		min_subtag_len = min(subtag_len, min_subtag_len)

print ('  if (limit - lang_str >= %d)' % (min_subtag_len + 2))
print ('  {')
print ("    const char *p = strchr (lang_str, '-');")
print ("    if (!p || p >= limit || limit - p < %i) goto out;" % min_subtag_len)
ಪ್ರತಿ initial, ವಸ್ತುಗಳು ಅನ್ತಃ sorted (complex_tags.ವಸ್ತುಗಳು ()):
	ಯದಿ initial != 'und':
		ಅನುವರ್ತ
	ಪ್ರತಿ lt, tags ಅನ್ತಃ ವಸ್ತುಗಳು:
		ಯದಿ ನ tags:
			ಅನುವರ್ತ
		ಯದಿ lt.variant ಅನ್ತಃ bcp_47.prefixes:
			expect (next (iter (bcp_47.prefixes[lt.variant])) == lt.language,
					'%s is not a valid prefix of %s' % (lt.language, lt.variant))
		print ('    if (', end='')
		print_subtag_matches (lt.script, 'p', ಅಸತ್ಯ)
		print_subtag_matches (lt.region, 'p', ಅಸತ್ಯ)
		print_subtag_matches (lt.variant, 'p', ಅಸತ್ಯ)
		print (')')
		print ('    {')
		ಲಿಖ ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		print ()
		ಯದಿ len (tags) == 1:
			ಲಿಖ ('      tags[0] = %s;  /* %s */' % (hb_tag (tags[0]), sanitize_comment (ot.names[tags[0]])))
			print ()
			print ('      *count = 1;')
		ಅನ್ಯಥಾ:
			print ('    hb_tag_t possible_tags[] = {')
			ಪ್ರತಿ tag ಅನ್ತಃ tags:
				ಲಿಖ ('      %s,  /* %s */' % (hb_tag (tag), sanitize_comment (ot.names[tag])))
				print ()
			print ('      };')
			print ('      for (i = 0; i < %s && i < *count; i++)' % len (tags))
			print ('\ttags[i] = possible_tags[i];')
			print ('      *count = i;')
		print ('      return true;')
		print ('    }')
print ('  }')
print ('out:')

print ('  switch (lang_str[0])')
print ('  {')
ಪ್ರತಿ initial, ವಸ್ತುಗಳು ಅನ್ತಃ sorted (complex_tags.ವಸ್ತುಗಳು ()):
	ಯದಿ initial == 'und':
		ಅನುವರ್ತ
	print ("  case '%s':" % sanitize_bcp47_tag (initial))
	ಪ್ರತಿ lt, tags ಅನ್ತಃ ವಸ್ತುಗಳು:
		ಯದಿ ನ tags:
			ಅನುವರ್ತ
		print ('    if (', end='')
		script = lt.script
		region = lt.region
		ಯದಿ lt.grandfathered:
			print ('0 == strcmp (&lang_str[1], "%s")' % sanitize_bcp47_tag (lt.language[1:]), end='')
		ಅನ್ಯಥಾ:
			string_literal = lt.language[1:] + '-'
			ಯದಿ script:
				string_literal += script
				script = None
				ಯದಿ region:
					string_literal += '-' + region
					region = None
			ಯದಿ string_literal[-1] == '-':
				print ('0 == strncmp (&lang_str[1], "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
			ಅನ್ಯಥಾ:
				print ('lang_matches (&lang_str[1], limit, "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
		print_subtag_matches (script, 'lang_str', ಸತ್ಯ)
		print_subtag_matches (region, 'lang_str', ಸತ್ಯ)
		print_subtag_matches (lt.variant, 'lang_str', ಸತ್ಯ)
		print (')')
		print ('    {')
		ಲಿಖ ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		print ()
		ಯದಿ len (tags) == 1:
			ಲಿಖ ('      tags[0] = %s;  /* %s */' % (hb_tag (tags[0]), sanitize_comment (ot.names[tags[0]])))
			print ()
			print ('      *count = 1;')
		ಅನ್ಯಥಾ:
			print ('      unsigned int i;')
			print ('      hb_tag_t possible_tags[] = {')
			ಪ್ರತಿ tag ಅನ್ತಃ tags:
				ಲಿಖ ('\t%s,  /* %s */' % (hb_tag (tag), sanitize_comment (ot.names[tag])))
				print ()
			print ('      };')
			print ('      for (i = 0; i < %s && i < *count; i++)' % len (tags))
			print ('\ttags[i] = possible_tags[i];')
			print ('      *count = i;')
		print ('      return true;')
		print ('    }')
	print ('    break;')

print ('  }')
print ('  return false;')
print ('}')
print ()
print ('/**')
print (' * hb_ot_ambiguous_tag_to_language')
print (' * @tag: A language tag.')
print (' *')
print (' * Converts @tag to a BCP 47 language tag if it is ambiguous (it corresponds to')
print (' * many language tags) and the best tag is not the first (sorted alphabetically,')
print (' * with two-letter tags having priority over all three-letter tags), or if the')
print (' * best tag consists of multiple subtags, or if the best tag does not appear in')
print (' * #ot_languages2 or #ot_languages3.')
print (' *')
print (' * Return value: The #hb_language_t corresponding to the BCP 47 language tag,')
print (' * or #HB_LANGUAGE_INVALID if @tag is not ambiguous.')
print (' **/')
print ('static inline hb_language_t')
print ('hb_ot_ambiguous_tag_to_language (hb_tag_t tag)')
print ('{')
print ('  switch (tag)')
print ('  {')

def verify_disambiguation_dict ():
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
	ವೈಶ್ವಿಕ bcp_47
	ವೈಶ್ವಿಕ disambiguation
	ವೈಶ್ವಿಕ ot
	ಪ್ರತಿ ot_tag, bcp_47_tags ಅನ್ತಃ ot.to_bcp_47.ವಸ್ತುಗಳು ():
		ಯದಿ ot_tag == DEFAULT_LANGUAGE_SYSTEM:
			primary_tags = []
		ಅನ್ಯಥಾ:
			primary_tags = list (t ಪ್ರತಿ t ಅನ್ತಃ bcp_47_tags ಯದಿ t ನ ಅನ್ತಃ bcp_47.grandfathered ಚ ot.from_bcp_47.get (t)[0] == ot_tag)
		ಯದಿ len (primary_tags) == 1:
			expect (ot_tag ನ ಅನ್ತಃ disambiguation, 'unnecessary disambiguation for OT tag: %s' % ot_tag)
			ಯದಿ '-' ಅನ್ತಃ primary_tags[0]:
				disambiguation[ot_tag] = primary_tags[0]
			ಅನ್ಯಥಾ:
				first_tag = sorted ((t ಪ್ರತಿ t ಅನ್ತಃ bcp_47_tags ಯದಿ t ನ ಅನ್ತಃ bcp_47.grandfathered ಚ ot_tag ಅನ್ತಃ ot.from_bcp_47.get (t)),
						ಕೀಲಿ_2=ಅನಾಮನ್ t: (len (t), t))[0]
				ಯದಿ primary_tags[0] != first_tag:
					disambiguation[ot_tag] = primary_tags[0]
		ನೋಚೇದ್ಯದಿ len (primary_tags) == 0:
			expect (ot_tag ನ ಅನ್ತಃ disambiguation, 'There is no possible valid disambiguation for %s' % ot_tag)
		ಅನ್ಯಥಾ:
			original_languages = [t ಪ್ರತಿ t ಅನ್ತಃ primary_tags ಯದಿ t ಅನ್ತಃ ot.from_bcp_47_uninherited ಚ 'retired code' ನ ಅನ್ತಃ bcp_47.scopes.get (t, '')]
			ಯದಿ len (original_languages) == 1:
				macrolanguages = original_languages
			ಅನ್ಯಥಾ:
				macrolanguages = [t ಪ್ರತಿ t ಅನ್ತಃ primary_tags ಯದಿ bcp_47.scopes.get (t) == ' [macrolanguage]']
			ಯದಿ len (macrolanguages) != 1:
				macrolanguages = list (t ಪ್ರತಿ t ಅನ್ತಃ primary_tags ಯದಿ bcp_47.scopes.get (t) == ' [collection]')
			ಯದಿ len (macrolanguages) != 1:
				macrolanguages = list (t ಪ್ರತಿ t ಅನ್ತಃ primary_tags ಯದಿ 'retired code' ನ ಅನ್ತಃ bcp_47.scopes.get (t, ''))
			ಯದಿ len (macrolanguages) != 1:
				macrolanguages = list (t ಪ್ರತಿ t ಅನ್ತಃ primary_tags ಯದಿ t.lower () == ISO_639_3_TO_1.get (ot_tag.lower (), ot_tag.lower ()))
			ಯದಿ len (macrolanguages) != 1:
				macrolanguages = list (t ಪ್ರತಿ t ಅನ್ತಃ primary_tags ಯದಿ '-' ನ ಅನ್ತಃ t)
			ಯದಿ len (macrolanguages) != 1:
				expect (ot_tag ಅನ್ತಃ disambiguation, 'ambiguous OT tag: %s %s' % (ot_tag, sorted (primary_tags)))
				expect (disambiguation[ot_tag] ಅನ್ತಃ bcp_47_tags,
						'%s is not a valid disambiguation for %s' % (disambiguation[ot_tag], ot_tag))
			ನೋಚೇದ್ಯದಿ ot_tag ನ ಅನ್ತಃ disambiguation:
				disambiguation[ot_tag] = macrolanguages[0]
			ಯದಿ '-' ನ ಅನ್ತಃ disambiguation[ot_tag]:
				different_bcp_47_tags = sorted ((t ಪ್ರತಿ t ಅನ್ತಃ bcp_47_tags ಯದಿ ನ same_tag (t, ot.from_bcp_47.get (t))),
						ಕೀಲಿ_2=ಅನಾಮನ್ t: (len (t), t))
				ಯದಿ different_bcp_47_tags ಚ disambiguation[ot_tag] == different_bcp_47_tags[0]:
					del disambiguation[ot_tag]
	ಪ್ರತಿ ot_tag ಅನ್ತಃ disambiguation.ಕೀಲಿಗಳು ():
		expect (ot_tag ಅನ್ತಃ ot.to_bcp_47, 'unknown OT tag: %s' % ot_tag)

verify_disambiguation_dict ()
ಪ್ರತಿ ot_tag, bcp_47_tag ಅನ್ತಃ sorted (disambiguation.ವಸ್ತುಗಳು ()):
	ಲಿಖ ('  case %s:  /* %s */' % (hb_tag (ot_tag), sanitize_comment (ot.names[ot_tag])))
	print ()
	ಲಿಖ ('    return hb_language_from_string (\"%s\", -1);  /* %s */' % (sanitize_bcp47_tag (bcp_47_tag), sanitize_comment (bcp_47.get_name (LanguageTag (bcp_47_tag)))))
	print ()

print ('  default:')
print ('    return HB_LANGUAGE_INVALID;')
print ('  }')
print ('}')

print ()
print ('#endif /* HB_OT_TAG_TABLE_HH */')
print ()
print ('/* == End of generated table == */')
