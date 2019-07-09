function onUpdate(entity)
	velocity = Vec2.new(0.0, entity:GetVelocity().y)

	movementSpeed = 400.0
	jumpPower = 800.0

	collidesBottom = entity:CollidesBottom()

	-- Input
	if (Input.KeyPressed("A")) then
		velocity.x = velocity.x - movementSpeed;
	end
	if (Input.KeyPressed("D")) then
		velocity.x = velocity.x + movementSpeed;
	end

	if (Input.KeyPressed("SPACE") and collidesBottom) then
		velocity.y = jumpPower;
	else
		if (Input.KeyReleased("SPACE") and (velocity.y > 0)) then
			velocity.y = (velocity.y * 0.5);
		end
	end

	if (Input.KeyPressed("S"))
	then
		entity:Drop()
	end

	-- Animation
	if (velocity.x < 0.0) then
		flip = 2
	else
		if (velocity.x > 0.0) then
			flip = 1
		else
			flip = 0
		end
	end

	if (velocity.y > 0.0) then
		entity:PlayAnimation("jump", flip)
	else
		if ((not collidesBottom) and velocity.y <= 0.0)	then
			entity:PlayAnimation("fall", flip);
		else
			if (velocity.x ~= 0.0) then 
				entity:PlayAnimation("walk", flip)
			else
				entity:PlayAnimation("idle", flip);
			end
		end
	end

	-- apply velocity
	entity:SetVelocity(velocity)
	
	if (velocity.x < 0.0) then
		entity:SetDirection("LEFT")
	else
		if (velocity.x > 0.0) then
			entity:SetDirection("RIGHT")
		end
	end

	-- update view
	entity:SetView();
end