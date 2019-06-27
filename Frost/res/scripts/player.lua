function on_update (entity)
	velocity = Vec2.new(0.0, Physics.velocity.y)

	if (Input.KeyPressed("A"))
	then
		velocity.x = velocity.x - Movement.speed;
	end
	if (Input.KeyPressed("D"))
	then
		velocity.x = velocity.x + Movement.speed;
	end

	if (Input.KeyPressed("SPACE") and entity:Physics():CollidesBottom())
	then
		velocity.y = Movement.jump;
	else
		if (Input.KeyReleased("SPACE") and (Physics.velocity.y > 0))
		then
			velocity.y = (Physics.velocity.y * 0.5);
		end
	end

	if (Input.KeyPressed("S"))
	then
		Physics:Drop()
	end

	Physics.velocity = velocity

	Camera:SetView(Transform.position)
end