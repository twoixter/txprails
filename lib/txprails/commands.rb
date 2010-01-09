require 'optparse'
require 'fileutils'

module TXPRails
  # This module handles miscelaneous command line utilities
  class Commands
    # @param args [Array<String>] The command-line arguments
    def initialize(args)
      @args = args
      @options = {}
      @options[:requires] = []    # ??
      @options[:load_paths] = []  # ??
    end

    # Parses the command-line arguments and runs the executable.
    # Calls `Kernel#exit` at the end, so it never returns.
    def parse!
      begin
        @opts = OptionParser.new(&method(:set_opts))
        @opts.parse!(@args)
        @options
      rescue Exception => e
        raise e if e.is_a?(SystemExit)

        $stderr.puts e.message
        exit 1
      end
      exit 0
    end

    # @return [String] A description of the executable
    def to_s
      @opts.to_s
    end

    protected

    # Tells optparse how to parse the arguments
    # available for all executables.
    #
    # This is meant to be overridden by subclasses
    # so they can add their own options.
    #
    # @param opts [OptionParser]
    def set_opts(opts)
      opts.on_tail("-?", "-h", "--help", "Show this message") do
        puts opts
        exit
      end

      opts.on_tail("-v", "--version", "Print version") do
        puts("TXPRails #{::TXPRails.version[:string]}")
        exit
      end

      opts.on('--rails RAILS_DIR', "Install TXPRails from the Gem to a Rails project") do |dir|
        original_dir = dir
        dir = File.join(dir, 'vendor', 'plugins')
        unless File.exists?(dir)
          puts "Directory #{dir} doesn't exist"
          exit
        end

        dir = File.join(dir, 'txprails')
        if File.exists?(dir)
          print "Directory #{dir} already exists, overwrite [y/N]? "
          exit if gets !~ /y/i
          FileUtils.rm_rf(dir)
        end

        begin
          Dir.mkdir(dir)
        rescue SystemCallError
          puts "Cannot create #{dir}"
          exit
        end

        File.open(File.join(dir, 'init.rb'), 'w') do |file|
          file << File.read(File.dirname(__FILE__) + "/../init_rails.rb")
        end

        puts "Haml plugin added to #{original_dir}"
        exit
      end
    end

  end
end
