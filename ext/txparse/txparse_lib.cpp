#include "ruby.h"
#include "txparse.h"

//---------------------------------------------------------------------------
const string TXPDictSpaces   = " \t\r\n";
const string TXPDictWordLim  = " \t\r\n,<>/=";
const string TXPDictSingleQuote = "'";
const string TXPDictDoubleQuote = "\"";

string txp_domain_str = "dmstk";

//---------------------------------------------------------------------------
static bool txp_isspace(string::const_iterator iter)
{
    return find(TXPDictSpaces.begin(), TXPDictSpaces.end(), *iter) != TXPDictSpaces.end();
}

//---------------------------------------------------------------------------
static bool txp_valid_chars(
		string::const_iterator iter,
		string::const_iterator end
		)
{
	if (*iter == '/') { iter++; end++; }
	for (; iter != end; ++iter) {
		if (::isspace(*iter)) return false;
		if (*iter == ':') return true;
		if (!::isalpha(*iter)) return false;
	}
	return false; // No se encontró los dos puntos...
}

//---------------------------------------------------------------------------
string::const_iterator txp_find_tag(
		string::const_iterator iter,
		string::const_iterator end
		)
{
	// Encontramos un TAG de plantilla cuando tengamos "<" + [a-z]{3} + ":"
	// O también cuando tengamos "</" ya que es de cierre...
    while (iter != end) {
        if ((*iter == '<') && (end - iter > 5) && txp_valid_chars(iter+1, iter+7))
			return iter;
        ++iter;
    }
	return iter;
}

//---------------------------------------------------------------------------
string::const_iterator txp_find_close(
		string::const_iterator iter,
		string::const_iterator end
		)
{
	// Encontramos un final de TAG cuando tengamos ">"
	for (; iter != end; ++iter) {
		if (*iter == '>') return iter+1;
	}
	return iter;
}

//---------------------------------------------------------------------------
string::const_iterator txp_find_par(
		string::const_iterator begin,
		string::const_iterator end,
		VALUE tag
		)
{
	// TODO:
	// Tenemos que hacer el mismo recorrido a partir de "begin" que en el 
	// bucle de parsing, pero esta vez sólo para encontrar un tag coincidente
	// con el que nos pasan... Todo esto no es más que una prueba, habría que
	// hacer un algoritmo más "robusto" para esto (Por ejemplo con pilas para
	// encontrar un tag balanceado, ahora no pueden estar anidados.)
	
	string::const_iterator old;

	while (begin != end) {
		begin = txp_find_tag(begin, end);
		if (begin != end) {
			// Estamos en un tag...
			old = begin;
			begin = txp_find_close(begin, end);
			if (begin != end) {
				VALUE tag_txt = rb_str_new2(string(old, begin).c_str());
				VALUE new_tag = rb_class_new_instance(1, &tag_txt, rb_const_get(mTXParse, rb_intern("Tag")));
/*
				printf(":::: SEARCHING PAR (D:'%s' vs '%s', N:'%s' vs. '%s')\n"
							, RSTRING(rb_iv_get(tag, "@domain"))->ptr
							, RSTRING(rb_iv_get(new_tag, "@domain"))->ptr
							, RSTRING(rb_iv_get(tag, "@name"))->ptr
							, RSTRING(rb_iv_get(new_tag, "@name"))->ptr
						);
*/

				// Vale, comprobamos que el tag se llama igual...
				if (!rb_str_cmp(rb_iv_get(tag, "@domain"), rb_iv_get(new_tag, "@domain")) &&
					!rb_str_cmp(rb_iv_get(tag, "@name"), rb_iv_get(new_tag, "@name"))) {
//						printf(":::: FOUND!!!\n");

						// Es el mismo TAG... ¿Es de cierre?
						if (rb_iv_get(new_tag, "@is_close") == Qtrue) {
//							printf(":::: New Tag is CLOSE\n");
//							printf(":::: DBG: [%s]\n", string(old, end).c_str());
							return old;
						}
				}
			}
		}
	}
	return end;	// What else?
}


#define	DEBUG_POS(str, pos) \
{\
	string strdebug = string(str.size(), ' '); \
	strdebug[pos] = '$'; \
	printf("|%s|\n", str.c_str()); \
	printf("|%s|\n", strdebug.c_str()); \
}

//---------------------------------------------------------------------------
void txp_parse_tag(const string tag, string &domain, string &name, map<string, string> &attrs, bool &close)
{
	string::size_type pos = 0, pos2;

	if ((tag.size() == 0) || (tag[pos++] != '<')) {
		rb_raise(rb_eSyntaxError, "Tag should begin with '<'");
		// rb_raise no retorna...
	}

	if (tag[pos] == '/') {
		close = true;
		pos++;
	}

	pos2 = tag.find_first_of(":/> \t\r\n", pos);
	if ((pos2 == string::npos) || (tag[pos2] != ':')) {
		rb_warn("TAG Should have a 'domain'. Assuming 'dmstk'");
		// rb_raise(rb_eSyntaxError, "Tag should have a domain.");
		domain = "dmstk";
		name = string(tag, pos, pos2 - pos);
	} else {
		domain = string(tag, pos, pos2 - pos);
		pos = pos2 + 1;
		pos2 = tag.find_first_of(":/> \t\r\n", pos);
		name = string(tag, pos, pos2 - pos);
	}
	
	// Si soy tag de cierre, no debo tener attributos.
	if (close) return;

	// Parseado el dominio y el nombre, saltamos a los attributos...
	pos = tag.find_first_not_of(" \t\r\n", pos2);
	if ((pos == string::npos) || (pos == '>')) return;

	while ((pos != string::npos) && (tag[pos] != '>')) {
		// Caso especial del tag de autocierre y el final de tag '>'
		if ((tag[pos] == '/') && (tag[pos+1] == '>')) {
			close = true;	// autoclose ??
			pos = string::npos;
			break;
		}
		
		pos2 = tag.find_first_of(" \t\r\n,<>/=", pos);
		string attrname = string(tag, pos, pos2 - pos);
		string attrval  = "";

		pos2 = tag.find_first_not_of(" \t\r\n", pos2);
		if ((pos2 != string::npos) && (tag[pos2]=='=')) {
			// Parseando valor de atributo
			pos2 = tag.find_first_not_of(" \t\r\n", pos2+1);

			switch (tag[pos2]) {
				case '\'': pos = pos2+1; pos2 = tag.find_first_of("'", pos); break;
				case '"':  pos = pos2+1; pos2 = tag.find_first_of("\"", pos); break;
				default:   pos = pos2; pos2 = tag.find_first_of(" \t\r\n"); break;
			}
			attrval = string(tag, pos, pos2 - pos);
		} else {
			// Atributo sin valores!! Asumimos -->  nombre="nombre"
			attrval = attrname;
		}

		attrs[attrname] = attrval;

		// Tenemos que dejar "pos" en el siguiente atributo...
		pos = tag.find_first_not_of(" \t\r\n", pos2+1);
	}
}
