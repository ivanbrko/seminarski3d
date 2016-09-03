Texture2D Texture[5];
SamplerState ss;

float4 main(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord[2] : TEXCOORD) : SV_TARGET
{
	/*
	float4 PSColor = color* Texture[0].Sample(ss, texcoord[0]);
	float4 PSColor2 = color* Texture[1].Sample(ss, texcoord[1]);
	float4 PSColor3 = color * Texture[2].Sample(ss, texcoord[1]);
	float4 PSColor4 = color * Texture[3].Sample(ss, texcoord[1]);
	float4 PSColor5 = color* Texture[4].Sample(ss, texcoord[0]);
	float4 PSColorF = 0.4*PSColor * (2 - PSColor5.r) + PSColor2*PSColor5.r;
	PSColorF += 0.4*PSColor * (2 - PSColor5.g) + PSColor3*PSColor5.g;
	PSColorF += 0.4*PSColor * (2 - PSColor5.b) + PSColor4*PSColor5.b;
	return PSColorF / 4.2;*/

	return color * Texture[0].Sample(ss, texcoord[0]);
	
}
