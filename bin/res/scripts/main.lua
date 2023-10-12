--[[
local xman = ClassList.Male.new()
local fman = ClassList.Female.new()
print(xman.super)
print(fman.super)
--]]
local tp = {a = 1, b = 2}
for k, v in pairs(tp) do
	print('fx==>', k, v)
	if not next(tp) then 
	end
end

print(type(""))

local xx = {1, nil, 2}
print('fxkk====>>>', unpack(xx))


c_Mode = "(%w+)%s*=%s*(%w+)";
c_Str = "key1=value1 key2=value2";
c_Tag = "<%1>%2</%1>";
r = string.gsub(c_Str,c_Mode,c_Tag)
u = string.upper(r)
print(r)

ticks = 1
ticks = ticks + 1
print('fxkk=====>>>', ticks)

xx = {
	func1 = function()
		print('fx==>>>1')
		func2()
	end,
	func2 = function()
		print('fx==>>>2')
	end,
}