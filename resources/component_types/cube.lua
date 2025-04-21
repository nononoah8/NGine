cube = {
  -- Variables to track animation
  radius = 0.1,
  speed = 0.0003, -- rotation amount per frame
  camMoveSpeed = 0.01,
  camAngleSpeed = 0.03,
  pos = Vector3(0, 0, 0),
  
  OnStart = function(self)
    Camera.SetPosition(0,0,0.5)
  end,

  OnUpdate = function(self)
    local xPos = Camera.GetPositionX()
    local yPos = Camera.GetPositionY()
    local zPos = Camera.GetPositionZ()

    -- Get current frame number
    local frame = Application.GetFrame()
    
    -- Calculate angle based on frame count
    local angle = frame * self.speed
    
    -- Calculate position on the circle
    local x = self.radius
    local z = self.radius
    local y = self.radius

    -- Forward/backward movement
    if Input.GetKey("w") then
      zPos = zPos - self.camMoveSpeed  -- Move forward (negative Z)
    end
    
    if Input.GetKey("s") then
      zPos = zPos + self.camMoveSpeed  -- Move backward (positive Z)
    end
    
    -- Left/right movement
    if Input.GetKey("a") then
      xPos = xPos - self.camMoveSpeed  -- Move left (negative X)
    end
    
    if Input.GetKey("d") then
      xPos = xPos + self.camMoveSpeed  -- Move right (positive X)
    end
    
    -- Up/down movement
    if Input.GetKey("space") then
      yPos = yPos + self.camMoveSpeed  -- Move up (positive Y)
    end
    
    if Input.GetKey("lctrl") or Input.GetKey("rctrl") then
      yPos = yPos - self.camMoveSpeed  -- Move down (negative Y)
    end

    if Input.GetKey("escape") then
      Application.Quit()
    end

    -- Get current angles
    local yaw = Camera.GetYaw()
    local pitch = Camera.GetPitch()
    
    -- Adjust angles based on input
    if Input.GetKey("right") then
        Camera.SetYaw(yaw + self.camAngleSpeed)
    end
    if Input.GetKey("left") then
        Camera.SetYaw(yaw - self.camAngleSpeed)
    end
    if Input.GetKey("up") then
        Camera.SetPitch(pitch + self.camAngleSpeed)
    end
    if Input.GetKey("down") then
        Camera.SetPitch(pitch - self.camAngleSpeed)
    end

    if Input.GetKey("i") then
      self.pos.y = self.pos.y + 0.001
    end
    if Input.GetKey("j") then
      self.pos.x = self.pos.x - 0.001
    end
    if Input.GetKey("k") then
      self.pos.y = self.pos.y - 0.001
    end
    if Input.GetKey("l") then
      self.pos.x = self.pos.x + 0.001
    end
    
    -- Update camera position
    Camera.SetPosition(xPos, yPos, zPos)

    Model.DrawCube(0.1, self.pos)
    Model.DrawSphere(0.05, Vector3(0.8, 0.5, 0.0), 16, Vector3(z, x, y))
    Model.DrawModel("tree", Vector3(1, 0, 0))
    Model.DrawTexturedPlane("dirt.jpg", 20.0, 20.0, Vector3(0.5, 0.8, 0.5), Vector3(0, -5, 0))

    if Application.GetFrame() % 1000 == 0 then
      local pos = Camera.GetPosition()
      Debug.Log("x: " .. pos.x .. ", y: " .. pos.y .. ", z: " .. pos.z)
    end
  end
}