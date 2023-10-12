require("res.scripts.class")
require("res.scripts.utils")
require("res.scripts.person")

local xman = ClassList.Male.new()
local fman = ClassList.Female.new()
print(xman.super)
print(fman.super)