// Turtle Arena team shaders. Based on MISSIONPACK demo shaders in team.shader

team_icon/Sais_red
{
	cull none
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	{
		map team_icon/Sais_red.png
		blendFunc Add
		rgbgen wave triangle 0.2 0.5 0 0.2
	}
}

team_icon/Sais_blue
{
	cull none
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	{
		map team_icon/Sais_blue.png
		blendFunc Add
		rgbgen wave triangle 0.2 0.5 0 0.2
	}
}

team_icon/Katanas_red
{
	cull none
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	{
		map team_icon/Katanas_red.png
		blendFunc Add
		rgbgen wave triangle 0.2 0.5 0 0.2
	}
}

team_icon/Katanas_blue
{
	cull none
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	{
		map team_icon/Katanas_blue.png
		blendFunc Add
		rgbgen wave triangle 0.2 0.5 0 0.2
	}
}

team_icon/Foot_red
{
	cull none
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	{
		map team_icon/Foot_red.png
		blendFunc GL_SRC_ALPHA GL_ONE
		rgbGen wave triangle 0.2 0.5 0 0.2
	}
	{
		map team_icon/Foot_red.png
		blendFunc filter
	}
}

team_icon/Foot_blue
{
	cull none
	surfaceparm nolightmap
	surfaceparm trans
	surfaceparm nomarks
	{
		map team_icon/Foot_blue.png
		blendFunc GL_SRC_ALPHA GL_ONE
		rgbGen wave triangle 0.2 0.5 0 0.2
	}
	{
		map team_icon/Foot_blue.png
		blendFunc filter
	}
}
