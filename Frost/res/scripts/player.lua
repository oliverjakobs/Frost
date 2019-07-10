function onUpdate(entity)
	velocity = Vec2.new(0.0, entity:GetVelocity().y)

	movementSpeed = 400.0
	jumpPower = 800.0

	collidesBottom = entity:CollidesBottom()

	if (Input.KeyPressed("1")) then
		Scenes.Change("station")
	end
	if (Input.KeyPressed("2")) then
		Scenes.Change("station2")
	end

	-- Input
	if (Input.KeyPressed("A")) then
		velocity.x = velocity.x - movementSpeed
	end
	if (Input.KeyPressed("D")) then
		velocity.x = velocity.x + movementSpeed
	end

	if (Input.KeyPressed("SPACE") and collidesBottom) then
		velocity.y = jumpPower
	else
		if (Input.KeyReleased("SPACE") and (velocity.y > 0)) then
			velocity.y = (velocity.y * 0.5)
		end
	end

	if (Input.KeyPressed("S"))
	then
		entity:Drop()
	end
	
	
	if (velocity.x > 0.0) then
		entity:SetDirection("RIGHT")
		entity:SetRenderFlip("NONE")
	else
		if (velocity.x < 0.0) then
			entity:SetDirection("LEFT")
			entity:SetRenderFlip("HORIZONTAL")
		end
	end

	-- Animation
	if (velocity.y > 0.0) then
		entity:PlayAnimation("jump")
	else
		if (not collidesBottom)	then
			entity:PlayAnimation("fall")
		else
			if (velocity.x ~= 0.0) then 
				entity:PlayAnimation("walk")
			else
				entity:PlayAnimation("idle")
			end
		end
	end

	-- apply velocity
	entity:SetVelocity(velocity)

	-- update view
	cameraOffset = Vec2.new(0.0, 30.0)

	entity:CenterView(cameraOffset, true)
end