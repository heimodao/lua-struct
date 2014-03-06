local struct = require 'struct'
local legend = require 'legend'

local function test1()
   print (legend.version)
   local dragons = { }
   for i=1,100 do
      local d = legend.dragon()
      d.weight = i
      d.length = i*2
      assert(d.weight == i)
      assert(d.length == i*2)
      dragons[i] = d
      assert(#struct.members(d) == 4)
      assert(struct.type(d) == 'dragon')
   end
   assert(dragons[50].weight == 50.0)
   for k,v in pairs(getmetatable(dragons[50]).__instances__) do
      assert(type(k) == 'userdata')
      assert(type(v) == 'table')
   end
end

local function test2()
   local d = legend.dragon()
   d.name = "cat"
   assert(d.name == "cat")
   d.name = "dog"
   assert(d.name == "dog")
   d.callback = function(x) return x end
   assert(d.callback(2.3) == 2.3)
   assert(legend.speak(d) == d.callback)
end

test1()
test2()
