#include "ruby.h"
#include "txparse.h"

extern "C" VALUE tag_initialize(VALUE self, VALUE orig)
{
	string domain, name;
	map<string, string> attrs;
	bool is_close = false;
	VALUE attr_hash = rb_hash_new();

	rb_check_type(orig, T_STRING);
	txp_parse_tag(string(RSTRING(orig)->ptr), domain, name, attrs, is_close);

/*
printf("PARSE-TAG DBG: ORIG='%s'\n", RSTRING(orig)->ptr);
printf("               DOMAIN='%s', NAME='%s'\n", domain.c_str(), name.c_str());
printf("               ATTRS(num)='%d', CLOSED='%c'\n", (int)attrs.size(), is_close ? 'Y' : 'N');
*/

	if (attrs.size()) {
		for (map<string, string>::const_iterator iter = attrs.begin()
			; iter != attrs.end()
			; ++iter)
		{
			rb_hash_aset(attr_hash, rb_str_new2((*iter).first.c_str()), rb_str_new2((*iter).second.c_str()));
		}
	}

	rb_iv_set(self, "@name", rb_str_new2(name.c_str()));
	rb_iv_set(self, "@domain", rb_str_new2(domain.c_str()));
	rb_iv_set(self, "@is_close", is_close ? Qtrue : Qfalse);
	rb_iv_set(self, "@attr", attr_hash);
	rb_iv_set(self, "@orig", orig);
	return self;
}

extern "C" VALUE tag_name(VALUE self)
{
	return rb_iv_get(self, "@name");
}

extern "C" VALUE tag_name_set(VALUE self, VALUE new_name)
{
	return rb_iv_set(self, "@name", new_name);
}


extern "C" VALUE tag_domain(VALUE self)
{
	return rb_iv_get(self, "@domain");
}

extern "C" VALUE tag_domain_set(VALUE self, VALUE new_domain)
{
	return rb_iv_set(self, "@domain", new_domain);
}

extern "C" VALUE tag_thing(VALUE self)
{
	return rb_iv_get(self, "@thing");
}

extern "C" VALUE tag_thing_set(VALUE self, VALUE new_thing)
{
	return rb_iv_set(self, "@thing", new_thing);
}

extern "C" VALUE tag_attributes(VALUE self)
{
	return rb_iv_get(self, "@attr");
}



extern "C" VALUE tag_is_close(VALUE self)
{
	return rb_iv_get(self, "@is_close");
}

extern "C" VALUE tag_orig(VALUE self)
{
	return rb_iv_get(self, "@orig");
}




VALUE mTXParseTag = Qnil;
void Init_TXParseTag()
{
//	VALUE TXParse_Module = rb_define_module("TXParse");
	mTXParseTag = rb_define_class_under(mTXParse, "Tag", rb_cObject);

	rb_define_method(mTXParseTag, "initialize", RUBY_METHOD_FUNC(tag_initialize), 1);

	rb_define_method(mTXParseTag, "name", RUBY_METHOD_FUNC(tag_name), 0);
	rb_define_method(mTXParseTag, "name=", RUBY_METHOD_FUNC(tag_name_set), 1);

	rb_define_method(mTXParseTag, "domain", RUBY_METHOD_FUNC(tag_domain), 0);
	rb_define_method(mTXParseTag, "domain=", RUBY_METHOD_FUNC(tag_domain_set), 1);

	rb_define_method(mTXParseTag, "thing", RUBY_METHOD_FUNC(tag_thing), 0);
	rb_define_method(mTXParseTag, "thing=", RUBY_METHOD_FUNC(tag_thing_set), 1);

	rb_define_method(mTXParseTag, "attr", RUBY_METHOD_FUNC(tag_attributes), 0);

	rb_define_method(mTXParseTag, "close?", RUBY_METHOD_FUNC(tag_is_close), 0);
	rb_define_method(mTXParseTag, "orig", RUBY_METHOD_FUNC(tag_orig), 0);
}
