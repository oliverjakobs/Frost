function onUpdate(entity)
	velocity = Vec2.new(0.0, GetVelocity(entity).y)

	movementSpeed = 400.0
	jumpPower = 800.0

	collidesBottom = CollidesBottom(entity)

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
		Drop(entity)
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
		PlayAnimation(entity, "jump", flip)
	else
		if ((not collidesBottom) and velocity.y <= 0.0)	then
			PlayAnimation(entity, "fall", flip);
		else
			if (velocity.x ~= 0.0) then 
				PlayAnimation(entity, "walk", flip)
			else
				PlayAnimation(entity, "idle", flip);
			end
		end
	end

	-- apply velocity
	SetVelocity(entity, velocity)

	
	if (velocity.x < 0.0) then
		SetDirection(entity, "LEFT")
	else
		if (velocity.x > 0.0) then
			SetDirection(entity, "RIGHT")
		end
	end

	-- update view
	SetView(entity);
end