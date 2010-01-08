#ifndef	TXPARSE_LIB
#define TXPARSE_LIB

#include <string>
#include <map>

using namespace std;

extern string txp_domain_str;

//---------------------------------------------------------------------------
string::const_iterator txp_find_tag(string::const_iterator iter, string::const_iterator end);
string::const_iterator txp_find_close(string::const_iterator iter, string::const_iterator end);
string::const_iterator txp_find_par(string::const_iterator begin, string::const_iterator end, VALUE tag);
void txp_parse_tag(const string tag, string &domain, string &name, map<string, string> &attrs, bool &close);

#endif
