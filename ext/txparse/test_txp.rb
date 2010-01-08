require "TXParse"

class MyParse
  include TXParse

  def strong(tag)
    unless tag.thing.nil?
      "THE THING(" + tag.thing + ")"
    else
      "STRONG" unless tag.close?
    end
  end

  def doctype(tag)
    "<DOCTYPE HTML>"
  end

  def body(tag)
    "<BODY>" + self.parse(tag.thing) + "</BODY>"
  end

  def uppercase(tag)
    self.parse(tag.thing).upcase
  end

  def count(tag)
    return self.parse(tag.thing) if tag.attr["limit"].nil?
    limit = tag.attr["limit"].to_i
    range = if tag.attr["zerobased"].nil? or tag.attr["zerobased"] == "no"
            (1..limit)
          else
            (0...limit)
          end
    out = []
    range.each do |n|
      @num = n
      out << self.parse(tag.thing)
    end
    out.join("\n")
  end

  def num(tag)
    @num.to_s
  end

end

txp = MyParse.new

orig = %{
<dmstk:doctype />
<dmstk:body>
  <dmstk:uppercase>
    <dmstk:count limit="5" zerobased="yes">
      Línea <var:num />
      Esto es una <dmstk:strong>primera <em>impresión</em> de</dmstk:strong>
      prueba de var:parseo de cosas...
    </dmstk:count>
  </dmstk:uppercase>
</dmstk:body>
}

txt = txp.parse(orig)

puts txt
