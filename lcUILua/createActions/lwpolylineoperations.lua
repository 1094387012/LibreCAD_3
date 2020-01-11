LWPolylineOperations = {}
LWPolylineOperations.__index = LWPolylineOperations

setmetatable(LWPolylineOperations, {
    __index = CreateOperations,
    __call = function (o, ...)
        local self = setmetatable({}, o)
        self:_init(...)
        return self
    end,
})

function LWPolylineOperations:_init(id)
    --self.currentVertex_Bulge = 1
    --self.currentVertex_StartWidth = 0
    --self.currentVertex_EndWidth = 0

    --self.lwVertexes = {}

    CreateOperations._init(self, id, lc.builder.LWPolylineBuilder, "enterPoint")
end

function LWPolylineOperations:enterPoint(eventName, data)
	if(eventName == "point" or eventName == "number") then
        self:newData(data["position"])
    elseif(eventName == "mouseMove") then
        --self:createTempLWPolyline(data["position"])
		self.builder:createTempLWPolyline(data["position"])
		self:build()
    end
end

--[[function LWPolylineOperations:onEvent(eventName, data)
    if(Operations.forMe(self, data) == false) then
        return
    end

    if(eventName == "point" or eventName == "number") then
        self:newData(data["position"])
    elseif(eventName == "mouseMove") then
        self:createTempLWPolyline(data["position"])
    end
end ]]--

function LWPolylineOperations:newData(data)
    local point = Operations:getCoordinate(data)
    if(point ~= nil) then
        if(self.currentVertex == "line") then
            --table.insert(self.lwVertexes, LWVertex2D(data))
			self.builder:addLineVertex(data)
        elseif(self.currentVertex == "arc") then
            --table.insert(self.lwVertexes, LWVertex2D(data, self.currentVertex_Bulge))
			self.builder:addArcVertex(data)
        end
    else
		-- # means length of table
        --local vertex = self.lwVertexes[#self.lwVertexes]
        
        --self.currentVertex_Bulge = math.tan(data / 4)
        --self.lwVertexes[#self.lwVertexes] = LWVertex2D(vertex:location(), self.currentVertex_Bulge, vertex:startWidth(), vertex:endWidth())

		self.build:modifyLastVertex(data)
    end
end

--[[function LWPolylineOperations:getLWPolyline(vertexes)
    if(#vertexes > 1) then
        local layer = active_layer(self.target_widget)
        local viewport = active_viewport(self.target_widget)
        local metaInfo = active_metaInfo(self.target_widget)
        local lwp = LWPolyline(vertexes, 1, 1, 1, false, Coord(0,0), layer, viewport, metaInfo)
        lwp:setId(self.entity_id)

        return lwp
    else
        return nil
    end
end ]]--

--[[function LWPolylineOperations:createTempVertex(point)
    local location = self.currentVertex_Location
    local bulge = self.currentVertex_Bulge

    if(location == nil) then
        location = Operations:getCoordinate(point)
    elseif(bulge == nil) then
        bulge = Operations:getDistance(location, point)
    end

    bulge = bulge or 1

    return LWVertex2D(location, bulge)
end ]]--

--[[function LWPolylineOperations:createTempLWPolyline(point)

    local vertexes = {}
    for k, v in pairs(self.lwVertexes) do
        vertexes[k] = v
    end
    table.insert(vertexes, self:createTempVertex(point))

    self.entity = self:getLWPolyline(vertexes)
    self:refreshTempEntity()
end--]]

function LWPolylineOperations:createLWPolyline()
    --local lwp = self:getLWPolyline(self.lwVertexes)
    --self:createEntity(lwp)
	self:createEntity()
end

function LWPolylineOperations:close()
    --[[if(not self.finished) then
        self:createLWPolyline()
        CreateOperations.close(self)
    end--]]
	self:createEntity()
	CreateOperations.close(self)
end

function LWPolylineOperations:createArc()
    self.currentVertex = "arc"

    if(self.builder:getVertices() > 0) then
		self.builder:modifyLastVertexArc()
        --local vertex = self.lwVertexes[#self.lwVertexes]
        --self.lwVertexes[#self.lwVertexes] = LWVertex2D(vertex:location(), self.currentVertex_Bulge, vertex:startWidth(), vertex:endWidth())
    end

    message("Give arc angle and coordinates", self.target_widget)
end

function LWPolylineOperations:createLine()
    self.currentVertex = "line"

    if(self.builder:getVertices() > 0) then
		self.builder:modifyLastVertexLine()
        --local vertex = self.lwVertexes[#self.lwVertexes]
        --self.lwVertexes[#self.lwVertexes] = LWVertex2D(vertex:location(), 0, vertex:startWidth(), vertex:endWidth())
    end

    message("Give line coordinates", self.target_widget)
end