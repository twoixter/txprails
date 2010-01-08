#ifndef TXPARSE_H
#define	TXPARSE_H

#include "txparse_lib.h"
#include "txparse_tag.h"

VALUE txp_parse_private(
		VALUE self,
		string::const_iterator begin,
		string::const_iterator end
		);

extern VALUE mTXParse;

#endif
