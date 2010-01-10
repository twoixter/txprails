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

  def txp_eval(tag)
    eval(tag.attr["code"])
  end

  def txp_include(tag)
    @view.render :partial => tag.attr["name"]
  end

  def txp_controller(tag)
    @view.controller.instance_variable_names.to_s
  end


  def method_missing(method_id, *args, &block)
    method_name = method_id.to_s
    super if method_name !~ /^mod_/ && method_name !~ /^var_/
    
    # Model or variable requested...
    tag = args.first
    output = []
    case method_name
    when /^mod_/
      if @_current_models.empty? || @_current_var.nil?
        variables = @view.controller.instance_variable_names
        variables -= @view.controller.protected_instance_variables if @view.controller.respond_to?(:protected_instance_variables)
        raise "Model not found" unless variables.include?("@#{tag.name}")
        @_current_models.push(@view.controller.instance_variable_get("@#{tag.name}")) 
      else
        raise "Model not found in current model" unless @_current_var.respond_to?(tag.name.to_sym)
        @_current_models.push(@_current_var.send(tag.name.to_sym))
      end
      output << iterate_current_model(tag)
      @_current_models.pop
    when /^var/
      unless @_current_var.nil?
        output << @_current_var.send(tag.name.to_sym).to_s
      else
        output << "[UNDEFINED]"
      end
    else
      raise "Invalid domain for tag"
    end
    output.join
  end

  private
  def iterate_current_model(tag)
    return if @_current_models.empty?
    output = []
    @_current_models.last.each do |x|
      @_current_var = x
      output << parse(tag.thing) unless tag.thing.nil?
    end
    output.join
  end

end
