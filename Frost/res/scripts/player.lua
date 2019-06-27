function on_update (entity)
	velocity = Vec2.new(0.0, GetVelocity().y)

	movementSpeed = 400.0
	jumpPower = 800.0

	if (Input.KeyPressed("A"))
	then
		velocity.x = velocity.x - movementSpeed;
	end
	if (Input.KeyPressed("D"))
	then
		velocity.x = velocity.x + movementSpeed;
	end

	if (Input.KeyPressed("SPACE") and CollidesBottom())
	then
		velocity.y = jumpPower;
	else
		if (Input.KeyReleased("SPACE") and (velocity.y > 0))
		then
			velocity.y = (velocity.y * 0.5);
		end
	end

	if (Input.KeyPressed("S"))
	then
		Drop()
	end

	SetVelocity(velocity)
end