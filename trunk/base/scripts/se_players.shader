// Player model shaders

//
// Shaders shared by all turtles
//

// White eyes/mouth
// TODO: The brightness should change base on how close we are? I think they are whiter farther away?
models/players/raph/white
{
	{
		map $whiteimage
		rgbGen const ( 0.9 0.9 0.9 )
	}
	//{
		//map $whiteimage
		//rgbGen const ( 0.0 0.0 0.0 )
		//alphaGen portal 256
	//}
}

models/players/raph/plastron
{
	{
		map models/players/raph/plastron.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/belt
{
	{
		map models/players/raph/belt.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/belt_blue
{
	{
		map models/players/raph/belt_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/belt_red
{
	{
		map models/players/raph/belt_red.png
		rgbGen lightingDiffuse
	}
}

//
// Raph
//

models/players/raph/head
{
	{
		map models/players/raph/head.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/carapace
{
	{
		map models/players/raph/carapace.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/arm
{
	{
		map models/players/raph/arm.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/leg
{
	{
		map models/players/raph/leg.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/arm_blue
{
	{
		map models/players/raph/arm_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/leg_blue
{
	{
		map models/players/raph/leg_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/arm_red
{
	{
		map models/players/raph/arm_red.png
		rgbGen lightingDiffuse
	}
}

models/players/raph/leg_red
{
	{
		map models/players/raph/leg_red.png
		rgbGen lightingDiffuse
	}
}

//
// Don
//

models/players/don/head
{
	{
		map models/players/don/head.png
		rgbGen lightingDiffuse
	}
}

models/players/don/carapace
{
	{
		map models/players/don/carapace.png
		rgbGen lightingDiffuse
	}
}

models/players/don/arm
{
	{
		map models/players/don/arm.png
		rgbGen lightingDiffuse
	}
}

models/players/don/leg
{
	{
		map models/players/don/leg.png
		rgbGen lightingDiffuse
	}
}

models/players/don/arm_blue
{
	{
		map models/players/don/arm_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/don/leg_blue
{
	{
		map models/players/don/leg_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/don/arm_red
{
	{
		map models/players/don/arm_red.png
		rgbGen lightingDiffuse
	}
}

models/players/don/leg_red
{
	{
		map models/players/don/leg_red.png
		rgbGen lightingDiffuse
	}
}

//
// Leo
//

models/players/leo/head
{
	{
		map models/players/leo/head.png
		rgbGen lightingDiffuse
	}
}

models/players/leo/carapace
{
	{
		map models/players/leo/carapace.png
		rgbGen lightingDiffuse
	}
}

models/players/leo/arm
{
	{
		map models/players/leo/arm.png
		rgbGen lightingDiffuse
	}
}

models/players/leo/leg
{
	{
		map models/players/leo/leg.png
		rgbGen lightingDiffuse
	}
}

models/players/leo/arm_blue
{
	{
		map models/players/leo/arm_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/leo/leg_blue
{
	{
		map models/players/leo/leg_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/leo/arm_red
{
	{
		map models/players/leo/arm_red.png
		rgbGen lightingDiffuse
	}
}

models/players/leo/leg_red
{
	{
		map models/players/leo/leg_red.png
		rgbGen lightingDiffuse
	}
}

//
// Mike
//

models/players/mike/head
{
	{
		map models/players/mike/head.png
		rgbGen lightingDiffuse
	}
}

models/players/mike/carapace
{
	{
		map models/players/mike/carapace.png
		rgbGen lightingDiffuse
	}
}

models/players/mike/arm
{
	{
		map models/players/mike/arm.png
		rgbGen lightingDiffuse
	}
}

models/players/mike/leg
{
	{
		map models/players/mike/leg.png
		rgbGen lightingDiffuse
	}
}

models/players/mike/arm_blue
{
	{
		map models/players/mike/arm_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/mike/leg_blue
{
	{
		map models/players/mike/leg_blue.png
		rgbGen lightingDiffuse
	}
}

models/players/mike/arm_red
{
	{
		map models/players/mike/arm_red.png
		rgbGen lightingDiffuse
	}
}

models/players/mike/leg_red
{
	{
		map models/players/mike/leg_red.png
		rgbGen lightingDiffuse
	}
}

//
// The Fugitoid!
//
// green eyes and mouth
models/players/fugitoid/eyes_mouth
{
	{
		map $whiteimage
		rgbGen const ( 0.0 0.9 0.0 )
	}
}

models/players/fugitoid/eyes_mouth_red
{
	{
		map $whiteimage
		rgbGen const ( 0.9 0.0 0.0 )
	}
}

models/players/fugitoid/eyes_mouth_blue
{
	{
		map $whiteimage
		rgbGen const ( 0.0 0.0 0.9 )
	}
}

models/players/fugitoid/body
{
	{
		map textures/effects/tinfx_lighter.png
		tcGen environment
		rgbGen lightingDiffuse
	}
}
