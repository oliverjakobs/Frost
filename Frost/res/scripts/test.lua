function Vec2Test(a, b)
	vec = Vec2.new()
	vec.x = a
	vec.y = b
	return vec
end

function InputTest(key)
	return Input.KeyPressed(key)
end