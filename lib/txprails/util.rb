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

  end
end
