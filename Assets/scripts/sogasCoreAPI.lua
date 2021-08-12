function class(baseClass, body)
    local ret = body or {};
    
    if(baseClass ~= nil) then
        setmetatable(ret, ret);
        ret.__index = baseClass;
    end
    
    ret.Create = function(self, constructionData, originalSubClass)
        local obj;
        if(self.__index ~= nil) then
            if(originalSubClass ~= nil) then
                obj = self.__index:Create(constructionData, originalSubClass);
            else
                obj = self.__index:Create(constructionData, self);
            end
        else
            obj = constructionData or {};
        end
        
        setmetatable(obj, obj);
        obj.__index = self;
        
        if(self.__operators ~= nil) then
            for key, val in pairs(self.__operators) do
                obj[key] = val;
            end
        end
        
        return obj;
    end
    
    return ret;
end

Vec3 = class(nil,
    {
        x = 0, y = 0, z = 0
    })

v = Vec3:Create({x = 5, y = 8, z = 7});