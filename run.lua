local struct = require 'struct'
local legend = require 'legend'

local function test1()
   print (legend.version)
   local dragons = { }
   for i=1,2 do
      local d = legend.dragon()
      d.weight = i
      d.length = i*2
      assert(d.weight == i)
      assert(d.length == i*2)
      dragons[i] = d
      assert(#struct.members(d) == 4)
      assert(struct.type(d) == 'dragon')
   end
   assert(dragons[2].weight == 2.0)
   for k,v in pairs(getmetatable(dragons[2]).__instances__) do
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

local function test3()
   local d = legend.dragon()
   d.name = "rat"
   d = nil
   collectgarbage()
   print("^^^ was it destroyed?")
end

local function test4()
   local d = legend.dragon()
   d.length = 1.0
   d.name = "bob"
   d.callback = { }
   assert(type(d.length) == 'number')
   assert(type(d.name) == 'string')
   assert(type(d.callback == 'table'))
   d:speak()
end

test1()
test2()
test3()
test4()
