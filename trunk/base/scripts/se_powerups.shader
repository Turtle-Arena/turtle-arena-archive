// Powerup shaders
//

// General Powerups
//powerups/battleSuit
//{
//	deformVertexes wave 100 sin 1 0 0 0
//	{
//		map textures/effects/envmapgold2.tga
//                //map textures/sfx/specular.tga
//		tcGen environment
//		tcMod turb 0 0.15 0 0.3
//                tcmod rotate 333
//                tcmod scroll .3 .3
//		blendfunc GL_ONE GL_ONE
//	}
//}
//powerups/battleWeapon
//{
//	deformVertexes wave 100 sin 0.5 0 0 0
//	{
//		map textures/effects/envmapgold2.tga
//                //map textures/sfx/specular.tga
//		tcGen environment
//		tcMod turb 0 0.15 0 0.3
//                tcmod rotate 333
//                tcmod scroll .3 .3
//		blendfunc GL_ONE GL_ONE
//	}
//}
// Turtle Man: Added rgbGen and alphaGen 
powerups/invisibility
{
	{
		map textures/effects/invismap.tga
		blendfunc GL_ONE GL_ONE
		rgbGen lightingDiffuse
		tcMod turb 0 0.15 0 0.25
		tcGen environment
		alphaGen wave sin .6 .2 0 .6
	}
}
//powerups/quad
//{
//	deformVertexes wave 100 sin 3 0 0 0
//	{
//		map textures/effects/quadmap2.tga
//		blendfunc GL_ONE GL_ONE
//		tcGen environment
//                tcmod rotate 30
//		//tcMod turb 0 0.2 0 .2
//                tcmod scroll 1 .1
//	}
//}
//powerups/quadWeapon
//{
//	deformVertexes wave 100 sin 0.5 0 0 0
//	{
//		map textures/effects/quadmap2.tga
//		blendfunc GL_ONE GL_ONE
//		tcGen environment
//                tcmod rotate 30
//		//tcMod turb 0 0.2 0 .2
//                tcmod scroll 1 .1
//	}
//}
powerups/regen
{
	deformVertexes wave 100 sin 3 0 0 0
	{
		map textures/effects/regenmap2.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
                tcmod rotate 30
		//tcMod turb 0 0.2 0 .2
                tcmod scroll 1 .1
	}
}

// teleport effect
playerTeleportEffect
{
	//deformVertexes wave 100 sin 3 0 0 0
	{
		//map textures/effects/regenmap2.tga
		map models/misc/telep.png
		blendfunc GL_ONE GL_ONE
		tcGen environment
                //tcmod rotate 30
		//tcMod turb 0 0.2 0 .2
                //tcmod scroll 1 .1
	}
}

// CTF powerups
powerups/redflag
{
	deformVertexes wave 100 sin 3 0 0 0
	{
		map textures/effects/redflagmap.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
		tcMod turb 0 0.2 0 1
	}
}

powerups/blueflag
{
	deformVertexes wave 100 sin 3 0 0 0
	{
		map textures/effects/blueflagmap.tga
		blendfunc GL_ONE GL_ONE
		tcGen environment
		tcMod turb 0 0.2 0 1
	}
}

