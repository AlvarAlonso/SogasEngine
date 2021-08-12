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
            for key, val in paris(self.__operators) do
                obj[key] = val;
            end
        end
        
        return obj;
    end
    
    return ret;
end