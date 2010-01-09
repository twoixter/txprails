module TXPRails
  # A module containing various useful functions.
  module Util
    extend self

    # An array of ints representing the Ruby version number.
    RUBY_VERSION = ::RUBY_VERSION.split(".").map {|s| s.to_i}

    # Returns the path of a file relative to the Haml root directory.
    #
    # @param file [String] The filename relative to the Haml root
    # @return [String] The filename relative to the the working directory
    def scope(file)
      File.join(File.dirname(File.dirname(File.dirname(File.expand_path(__FILE__)))), file)
    end

    ## Cross Rails Version Compatibility

    # Returns the root of the Rails application,
    # if this is running in a Rails context.
    # Returns `nil` if no such root is defined.
    #
    # @return [String, nil]
    def rails_root
      return Rails.root.to_s if defined?(Rails.root)
      return RAILS_ROOT.to_s if defined?(RAILS_ROOT)
      return nil
    end

    # Returns an ActionView::Template* class.
    # In pre-3.0 versions of Rails, most of these classes
    # were of the form `ActionView::TemplateFoo`,
    # while afterwards they were of the form `ActionView::Template::Foo`.
    #
    # @param name [#to_s] The name of the class to get.
    #   For example, `:Error` will return `ActionView::TemplateError`
    #   or `ActionView::Template::Error`.
    def av_template_class(name)
      return ActionView.const_get("Template#{name}") if ActionView.const_defined?("Template#{name}")
      return ActionView::Template.const_get(name.to_s)
    end

  end
end
