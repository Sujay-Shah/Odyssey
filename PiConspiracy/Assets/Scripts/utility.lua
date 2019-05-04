-- utility.lua

--deep copy an object BUT NOT ITs METATABLE
function deep_copy(object)
    local lookup_table = {}
    local function _copy(object)
        if type(object) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for index, value in pairs(object) do
            new_table[_copy(index)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return _copy(object)
end

-- Given a Lua table, extract a sub-array or slice. This function works like
-- string.sub: if the end of the range is not specified, then everything up to
-- the end is assumed; this argument can also be a negative number, where -1
-- is the end, -2 second-last, etc.
-- taken from http://snippets.luacode.org/snippets/Table_Slice_116
function table_slice (values,i1,i2)
    local res = {}
    local n = #values
    -- default values for range
    i1 = i1 or 1
    i2 = i2 or n
    if i2 < 0 then
        i2 = n + i2 + 1
    elseif i2 > n then
        i2 = n
    end
    if i1 < 1 or i1 > n then
        return {}
    end
    local k = 1
    for i = i1,i2 do
        res[k] = values[i]
        k = k + 1
    end
    return res
end


function table_to_string(o, maxdepth, depth)
    maxdepth = maxdepth or 4
    depth = depth or 1
    if type(o) == 'table' then
        local s = '{ '
        for k,v in pairs(o) do
            if type(k) ~= 'number' then k = '"'..k..'"' end
            if depth <= maxdepth then
                s = s .. '['..k..'] = ' .. table_to_string(v, maxdepth, depth+1) .. ','
            else
                s = s .. '['..k..'] = {<too deep>},'
            end
        end
        return s .. '} '
    else
        return tostring(o)
    end
end

function table_shuffle(t)
  local n = #t

  while n >= 2 do
    -- n is now the last pertinent index
    local k = math.random(n) -- 1 <= k <= n
    -- Quick swap
    t[n], t[k] = t[k], t[n]
    n = n - 1
  end

  return t
end