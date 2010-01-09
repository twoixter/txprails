begin
  require File.join(File.dirname(__FILE__), 'txprails') # From here
rescue LoadError
  begin
    require 'txprails' # From gem
  rescue LoadError => e
    # gems:install may be run to install TXPRails with the skeleton plugin
    # but not the gem itself installed.
    # Don't die if this is the case.
    raise e unless defined?(Rake) && Rake.application.top_level_tasks.include?('gems:install')
  end
end

# Load TXPRails.
# TXPRails may be undefined if we're running gems:install.
TXPRails.init_rails if defined?(TXPRails)
