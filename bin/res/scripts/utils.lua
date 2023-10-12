-----------------------一些实用小方法--------------------------
-- 去掉字符串首尾空格
function string.trim(strx)
   return (string.gsub(strx, "^%s*(.-)%s*$", "%1"))
end

-- 分割字符串
function string.split(str, mark)

 	if string.len(str)==0 then return nil end
 
	local sIdx = 1
	local tsub = {}
	while true do
		local pos = string.find(str, mark, sIdx, true)
	   	if not pos then break end

	    table.insert(tsub, string.sub(str, sIdx, pos-1))
	    sIdx = pos + string.len(mark)
	end
  	table.insert (tsub, string.sub(str, sIdx))

	return tsub
end

local function __tostrx(param)
	local result = tostring(param)
	if type(param) == 'table' then
		local pAttr = string.sub(result, -8)
		result = ('table: 0x%s'):format(pAttr)
	elseif type(param) == 'function' then
		local pAttr = string.sub(result, -8)
		result = ('function: 0x%s'):format(pAttr)
	end
	return result
end

local function __th_tostring(tbl)
	local ref_cache = {} -- 记录已存在的
    local function getTable(ptbl, level)
    	local str_tbl = __tostrx(ptbl)
        local str_format = ('{ --%s\n'):format(str_tbl)
		local index = 0
		local size = table.size(ptbl)
		ref_cache[ptbl] = true
        for key, value in pairs(ptbl) do
			index = index + 1
			key = getKey(key, level)
            if type(value) == "table" then
				if ref_cache[value] then
					tabstr = tabstr .. key .. (' : ref%q'):format(tostrx(value))
				else
					tabstr = tabstr .. key .. ' : ' .. getTable(value, level+1)
				end
            else
				tabstr = tabstr .. key .. " : " .. getValue(value)
            end
            tabstr = tabstr .. (index==size and "\n" or ",\n")
        end
        tabstr = tabstr .. ('  '):rep(level-1) .. "}"
        return tabstr
    end
end

function tostrx(param)
	local result = tostring(param)
	if type(param) == 'table' then
		local pAttr = string.sub(result, -8)
		result = ('table: 0x%s'):format(pAttr)
	elseif type(param) == 'function' then
		local pAttr = string.sub(result, -8)
		result = ('function: 0x%s'):format(pAttr)
	end
	return result
end
