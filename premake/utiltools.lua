-- table转字符串
local function __tostring(value, indent, vmap)
    local str = ''
    indent = indent or ''
    vmap = vmap or {}

    if (type(value) ~= 'table') then
        if (type(value) == 'string') then
            
            if string.byte(value,1) == 91 then 
                str = string.format("'%s'", value)
            else
                if value:match('%[') then 
                    str = string.format('"%s"', value)
                else
                    str = string.format("[[%s]]", value)
                end 
            end 

        else
            str = tostring(value)
        end
    else
        if type(vmap) == 'table' then
            if vmap[value] then return '('..tostring(value)..')' end
            vmap[value] = true
        end
        local auxTable = {}
        local iauxTable = {}
        local iiauxTable = {}
        for i, v in pairs(value) do
            if type(i) == 'number' then
                if i == 0 then
                    table.insert(iiauxTable, i)
                else
                    table.insert(iauxTable, i)
                end
            else
                table.insert(auxTable, i)
            end
        end 

        table.sort(iauxTable)

        str = str..'{\n'
        local separator = ""
        local entry = "\n"
        local barray = true
        local kk,vv
        for i, k in ipairs (iauxTable) do 
            if i == k and barray then
                entry = __tostring(value[k], indent..'    ', vmap)
                str = str..separator..indent..'    '..entry
                separator = ", \n"
            else
                barray = false
                table.insert(iiauxTable, k)
            end
        end 
        for i, fieldName in ipairs (iiauxTable) do 
            kk = tostring(fieldName)
            if type(fieldName) == "number" then kk = '['..kk.."]" end 
            if type(fieldName) == "string" and (fieldName:match("%.") or fieldName:match("-")) then kk = '["'..kk..'"]' end 
            entry = kk .. " = " .. __tostring(value[fieldName],indent..'    ',vmap)

            str = str..separator..indent..'    '..entry
            separator = ", \n"
        end 
        for i, fieldName in ipairs (auxTable) do 
            kk = tostring(fieldName)
            if type(fieldName) == "number" then kk = '['..kk.."]" end 
            if type(fieldName) == "string" and (fieldName:match("%.") or fieldName:match("-"))then kk = '["'..kk..'"]' end 

            vv = value[fieldName]
            entry = kk .. " = " .. __tostring(value[fieldName],indent..'    ',vmap)


            str = str..separator..indent..'    '..entry
            separator = ", \n"
        end 
        str = str..'\n'..indent..'}'
    end
    return str
end
table.tostring =  __tostring

--重写print函数 
local print_ = _G.print
_G.print = function (...)
    local a = {...}
    do --{{{ 
        local info = debug.getinfo(2)
        if info then table.insert(a, 1, string.format('[%s:%d]', info.short_src or '', info.currentline or 0)) end 
    end --}}}

    local m = string.format('[%s]', os.date("%H:%M:%S", os.time()))
    for i, v in ipairs (a) do 
        if type(v) == 'table' then 
            local ss;
            if v.toString then
                ss = v:toString()
            elseif table.tostring then
                ss = table.tostring(v)
            else
                ss = tostring(v)
            end
            if #ss > 8000 then ss = string.sub(ss, 1, 8000) end 
            m = m .. ss .. ' ' 
        else
            local ss = tostring(v)
            if #ss > 8000 then ss = string.sub(ss, 1, 8000) end 
            m = m .. ss .. ' ' 
        end 
    end 

    if #m >= 8*1024 then m = string.sub(m, 1, 8*1024 - 1) end 
    if #a <= 0 then m = 'nil' end 
    print_(m)
end