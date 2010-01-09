# Simple template initialization code
# Borrowed ideas from Haml, but VERY much simplyfied
# WARNING: We choose to use the plugin approach, so NO RAILS < 2.1.0 compatibility!

module TXPRails

  class TXPTemplateHandler < TXPRails::Util.av_template_class(:Handler)
    include TXP

    def initialize(view)
      @view = view
    end
    
    def render(template, local_assigns = {})
      parse(template.source)
    end
    
    def compilable?
      false
    end
  end
end


if defined? ActionView::Template and ActionView::Template.respond_to? :register_template_handler
  ActionView::Template
else
  ActionView::Base
end.register_template_handler(:txp, TXPRails::TXPTemplateHandler)
