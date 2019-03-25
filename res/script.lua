-- Lua Script
-- This is just for testing some scripts

players = {}

players[0] = {Title = "Godd", Name = "Todd Howard", Level = 2}
players[1] = {Title = "PRGuy", Name = "Pete Hines", Level = 14}

function AddStuff(a, b)
	print("[LUA] AddStuff("..a..", "..b..") called \n")
	return a + b
end


function GetPlayer(n)
	return players[n]
end


function compareInts(a, b)
	if a == b then
		return 0
	elseif a > b then
		return 1
	else return -1
	end
end


function DoAThing(a, b)
	
	print("[LUA] DoAThing("..a..", "..b..") called \n")
	
	c = HostFunction(a + 10, b * 3)

	return c
end

