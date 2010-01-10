# Generated by jeweler
# DO NOT EDIT THIS FILE DIRECTLY
# Instead, edit Jeweler::Tasks in Rakefile, and run the gemspec command
# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{txprails}
  s.version = "0.1.1"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Jose Miguel P\303\251rez Ruiz"]
  s.date = %q{2010-01-10}
  s.default_executable = %q{txprails}
  s.description = %q{TXPRails is a template engine based on TextPattern for Rails.}
  s.email = %q{josemiguel@perezruiz.com}
  s.executables = ["txprails"]
  s.extensions = ["ext/txparse/extconf.rb"]
  s.extra_rdoc_files = [
    "LICENSE",
     "README.rdoc"
  ]
  s.files = [
    ".document",
     ".gitignore",
     "LICENSE",
     "README.rdoc",
     "Rakefile",
     "VERSION",
     "VERSION_NAME",
     "bin/txprails",
     "ext/txparse/Makefile",
     "ext/txparse/extconf.rb",
     "ext/txparse/test_txp.rb",
     "ext/txparse/txparse.cpp",
     "ext/txparse/txparse.h",
     "ext/txparse/txparse_lib.cpp",
     "ext/txparse/txparse_lib.h",
     "ext/txparse/txparse_tag.cpp",
     "ext/txparse/txparse_tag.h",
     "lib/init_rails.rb",
     "lib/txprails.rb",
     "lib/txprails/commands.rb",
     "lib/txprails/template.rb",
     "lib/txprails/util.rb",
     "lib/txprails/version.rb",
     "test/helper.rb",
     "test/test_txprails.rb",
     "txprails.gemspec"
  ]
  s.homepage = %q{http://github.com/twoixter/txprails}
  s.rdoc_options = ["--charset=UTF-8"]
  s.require_paths = ["lib"]
  s.rubygems_version = %q{1.3.5}
  s.summary = %q{TextPattern-like template engine for Rails}
  s.test_files = [
    "test/helper.rb",
     "test/test_txprails.rb"
  ]

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<thoughtbot-shoulda>, [">= 0"])
    else
      s.add_dependency(%q<thoughtbot-shoulda>, [">= 0"])
    end
  else
    s.add_dependency(%q<thoughtbot-shoulda>, [">= 0"])
  end
end

