require 'txprails/txparse'
require 'txprails/version'

module TXPRails
  extend TXPRails::Version
  
  # A string representing the version of TXPRails.
  # A more fine-grained representation is available from TXPRails.version.
  VERSION = version[:string] unless defined?(TXPRails::VERSION)

  # Initializes TXPRails for Rails.
  #
  # This method is called by `init.rb`,
  # which is run by Rails on startup.
  # We use it rather than putting stuff straight into `init.rb`
  # so we can change the initialization behavior
  # without modifying the file itself.
  #
  def self.init_rails
    %w[txprails/template].each {|f| require f}
  end
end

require 'txprails/util'

module TXP
  include TXParse
  
  def txp_doc(tag)
    "<!DOCTYPE html>"
  end

  def txp_content(tag)
    if content_for_layout = @view.instance_variable_get("@content_for_layout")
      parse(content_for_layout)
    end
  end

  def txp_include(tag)
    @view.render :partial => tag.attr["name"]
  end

#  self.extend self
end
