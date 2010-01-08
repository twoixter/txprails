require 'mkmf'

case RUBY_PLATFORM
when /darwin/, /linux/
  CONFIG['LDSHARED'] = "$(CXX) " + CONFIG['LDSHARED'].split[1..-1].join(' ')
end

create_makefile('txprails/txparse')
