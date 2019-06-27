function on_update (entity)
	velocity = Vec2.new(0.0, entity:Physics().velocity.y)

	if (Input.KeyPressed("A"))
	then
		velocity.x = velocity.x - entity:Movement().speed;
	end
	if (Input.KeyPressed("D"))
	then
		velocity.x = velocity.x + entity:Movement().speed;
	end

	if (Input.KeyPressed("SPACE") and entity:Physics():CollidesBottom())
	then
		velocity.y = entity:Movement().jump;
	else
		if (Input.KeyReleased("SPACE") and (entity:Physics().velocity.y > 0))
		then
			velocity.y = (entity:Physics().velocity.y * 0.5);
		end
	end

	if (Input.KeyPressed("S"))
	then
		entity:Physics():Drop()
	end

	entity:Physics().velocity = velocity

	entity:Camera():SetView(entity:Transform().position)
end