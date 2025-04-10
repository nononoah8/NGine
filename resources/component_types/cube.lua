cube = {
  -- Variables to track animation
  radius = 0.1,
  speed = 0.0003, -- rotation amount per frame
  
  OnUpdate = function(self)
    -- Get current frame number
    local frame = Application.GetFrame()
    
    -- Calculate angle based on frame count
    local angle = frame * self.speed
    
    -- Calculate position on the circle
    local x = self.radius * math.cos(angle)
    local z = self.radius * math.sin(angle)
    local y = self.radius * math.sin(angle)
    
    -- Draw the cube at the calculated position (size 0.1)
    local pos = Vector3(x, y, z)
    Shape.DrawCube(0.1, pos)
    Shape.DrawSphere(0.05, Vector3(0.8, 0.2, 0.0), 16, Vector3(z, x, y))
    
    -- Debug output (every 60 frames to avoid spam)
    if frame % 60 == 0 then
      Debug.Log("Cube position: " .. x .. ", 0, " .. z)
    end
  end
}