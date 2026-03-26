#ifndef CNET_MCI_H
#define CNET_MCI_H

// MCI sequence inducers/terminators
#define MCIS_BEG '' // starts single arg MCI commands
#define MCIQ_BEG '' // starts multi-byte MCI commands
#define MCIQ_END '}' // ends MCI multi-byte commands

/* MCI sequence commands */
#define MCI_ENVDEF			'@'
#define MCI_AUTOINDENT		':'
#define MCI_TABCOLUMN		'.'
#define MCI_DISABLEABORT	'A'
#define MCI_BELL				'B'
#define MCI_COLOR				'C'
#define MCI_HOMECLEAR		'F'
#define MCI_GETKEY			'G'
#define MCI_BACKSPACE		'H'
#define MCI_LINEINPUT		'I'
#define MCI_LABELJUMP		'J'
#define MCI_KOLORIFIC		'K'
#define MCI_LOADVAR			'L'
#define MCI_MATH				'M'
#define MCI_NEWLINE			'N'
#define MCI_BOLDFLASHING	'O'
#define MCI_PRINTDIRECTION	'P'
#define MCI_CONTROL			'Q'
#define MCI_REVERSE			'R'
#define MCI_PRINTSPEED		'S'
#define MCI_TESTVAR			'T'
#define MCI_UNDERLINE		'U'
#define MCI_SHOWGETUSER		'V'
#define MCI_DELAY				'W'
#define MCI_MCIREGSTORE		'X'
#define MCI_BGCOLOR			'Z'
#define MCI_CURSORUP			'^'
#define MCI_CURSORDOWN		'!'
#define MCI_CURSORRIGHT		'>'
#define MCI_CURSORLEFT		'<'
#define MCI_INSERTCHARS		'-'
#define MCI_DISPLAYFILE		'*'
#define MCI_RUNDOOR			'#'
#define MCI_ADOSCMD			'$'
#define MCI_LOGENTRY			'+'
#define MCI_BBSCMD			'&'
#define MCI_DATECOMPDUMP	'%'
#define MCI_YNPROMPT			'?'
#define MCI_PROMPTDEFAULT	'='

/* MCI CTRL-Q BITPATTERNS - for MCI filtering */
#define BMCI_RUNDOOR			0x01
#define BMCI_ADOSCMD			0x02
#define BMCI_BBSCMD			0x04
#define BMCI_DATECOMPDUMP	0x08

#endif /* CNET_MCI_H */
