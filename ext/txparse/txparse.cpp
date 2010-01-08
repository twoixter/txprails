#include "ruby.h"
#include "txparse.h"

extern "C" VALUE txp_initialize(VALUE self)
{
	return self;
}

extern "C" VALUE txp_domain(VALUE self)
{
//  Dejo esto como ejemplo de llamada a "rb_funcall", quitar después...
//	VALUE txt = rb_iv_get(self, "@texto");
//	return rb_funcall(txt, rb_intern("size"), 0);
	return rb_str_new2(txp_domain_str.c_str());
}

extern "C" VALUE txp_domain_set(VALUE self, VALUE new_domain)
{
	txp_domain_str = StringValueCStr(new_domain);
	return rb_str_new2(txp_domain_str.c_str());
}

extern "C" VALUE txp_parse(VALUE self, VALUE orig)
{
	// Lanza una excepción si no hay bloque...
	// (rb_yield lanza una excepcion "LocalJump... noseque" de todas formas,
	// pero parece buena idea comprobar al principio y no hacer nada si
	// no tenemos bloque)...
//	rb_need_block();

	// rb_check_type tampoco retorna... (rb_raise no retorna, y esta funcion
	// llama a rb_raise(rb_eTypeError, ...) si los tipos no coinciden)
	rb_check_type(orig, T_STRING);

	// Mmmmhhh... Lo anterior no es muy "rubyish", quizá debería ser
	// suficiente con llamar al "to_s" del @texto en caso de que éste no sea
	// de tipo String, de esta forma podríamos usar más tipos, pero...
	// a costa de la velocidad.

	// OK, "semos" un string...
	// NOTA: Acceder mediante el macro RSTRING es un poco peligroso, pero
	// es rápido ya que accedemos directamente a la estructura que almacena
	// un string en ruby. Lo podemos hacer aquí más o menos seguro porque
	// nos hemos asegurado que @texto es un String. Para conversiones, mirar
	// la referencia de las funciones "rb_string_value" y "rb_string_value_cstr"
	string::const_iterator iter(RSTRING(orig)->ptr);
	
	return txp_parse_private(self, iter, iter + RSTRING(orig)->len);
}

VALUE txp_parse_private(
		VALUE self,
		string::const_iterator begin,
		string::const_iterator end
		)
{
	VALUE ret = rb_str_new2("");
	string::const_iterator old = begin;

	while (begin != end) {
		begin = txp_find_tag(begin, end);

		if (begin != old) {
			// Texto normal antes de un tag...
			// rb_yield(rb_str_new(&(*old), begin - old));
			rb_str_cat(ret, &(*old), begin - old);
			old = begin;
		}

		if (begin != end) {
			// Estamos en un tag...
			begin = txp_find_close(begin, end);
			if (begin != end) {
				// FIXME: Este es el primer elemento a optimizar. Al crear
				// un tag, ahora mismo tenemos que hacer dos copias de texto,
				// una para crear un VALUE para instanciar un TXParse::Tag y
				// luego otra, para crear un std::string desde este VALUE...
				VALUE tag_txt = rb_str_new2(string(old, begin).c_str());
				VALUE tag = rb_class_new_instance(1, &tag_txt, rb_const_get(mTXParse, rb_intern("Tag")));

				// OK, tenemos el tag... Debemos buscar el "par" de cierre
				// en caso de que este tag no sea de "autocierre"
				VALUE closed = rb_iv_get(tag, "@is_close");
				if (closed == Qfalse) {
					// Buscamos el tag de cierre...
					old = begin;
					begin = txp_find_par(begin, end, tag);
					if (begin != end) {
						rb_iv_set(tag, "@thing", rb_str_new2(string(old, begin).c_str()));
						// Vale, aquí habría que adelantar "begin" para saltar
						// el tag de cierre...
						begin = txp_find_close(begin+1, end);
					} else {
						// TODO:
						// Retomamos donde estábamos... ¿Debería ser ERROR?
						// SI: Tag mal balanceado si el tag original NO era
						// de autocierre...
						
						// begin = old;
						rb_raise(rb_eSyntaxError, "Unbalanced tag '%s'", RSTRING(rb_iv_get(tag, "@name"))->ptr);
					}
				}

				VALUE tag_name = rb_iv_get(tag, "@name");
				VALUE tmp = rb_funcall2(self, rb_intern(StringValueCStr(tag_name)), 1, &tag);
				// VALUE tmp = rb_yield(tag);

				if (!NIL_P(tmp)) rb_str_append(ret, tmp);
			} else {
				rb_str_cat(ret, &(*old), begin - old);
			}
			old = begin;
		}
	}
	return ret;
}



VALUE mTXParse = Qnil;
extern "C" void Init_TXParse()
{
	mTXParse = rb_define_module("TXParse");
	
	rb_define_method(mTXParse, "initialize", RUBY_METHOD_FUNC(txp_initialize), 0);
	rb_define_method(mTXParse, "domain", RUBY_METHOD_FUNC(txp_domain), 0);
	rb_define_method(mTXParse, "domain=", RUBY_METHOD_FUNC(txp_domain_set), 1);
	rb_define_method(mTXParse, "parse", RUBY_METHOD_FUNC(txp_parse), 1);
	
	Init_TXParseTag();
}

extern "C" void Init_txparse()
{
	Init_TXParse();
}
