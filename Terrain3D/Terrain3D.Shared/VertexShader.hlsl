cbuffer ConstantBuffer
{
	float4x4 final;
	float4 lightvec;     
	float4 lightcol;      
	float4 ambientcol;    
	float time;
	float waves;
}

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord[2]: TEXCOORD;  
};

VOut main(float4 position : POSITION, float4 normal : NORMAL, float2 texcoord[2] : TEXCOORD)
{
	float4 lightcolor = lightcol;
	VOut output;
	if (waves == 1 && position.y < 7.8 && position.x != 511 && position.z != 511 && position.x != 0 && position.z != 0)
	{
		float factor = 1.6 * (sin(( position.x + position.z + time)/5)) + 0.8*sin( -position.z / 7 -   -position.x / 12 + position.y);
		position.y += 1.5*factor;
		float lightfactor = factor * 0.07;
		lightcolor += (lightfactor, lightfactor, lightfactor, lightfactor);
	}
	output.position = mul(final, position);
	output.color = ambientcol;

	float diffusebrightness = dot(normal, lightvec);
	output.color += saturate(lightcolor * diffusebrightness);

	output.texcoord[0] = texcoord[0];
	output.texcoord[1] = texcoord[1];

	

	return output;
}
