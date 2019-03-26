//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txSpec : register(t1);
SamplerState samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;

	//float gTime;

	float4 DiffuseMtrl;
	float4 DiffuseLight;
	float3 LightVecW;
	float pad;

	float4 AmbientMtrl;
	float4 AmbientLight;

	float4 SpecularMtrl;
	float4 SpecularLight;
	float SpecularPower;
	float3 EyePosW; 	// Camera position in world space

}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : NORMAL;
	float3 PosW : POSITION;
	float2 Tex : TEXCOORD0;
	float Sat : SATURATION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
/*	
VS_OUTPUT VS( float4 Pos : POSITION, float3 Normal : NORMAL )
{
    VS_OUTPUT output = (VS_OUTPUT)0;

	Pos.xy += 0.5f * sin(Pos.x) * sin(3.0f * gTime);
	Pos.z *= 0.6f + 0.4f * sin(2.0f * gTime);

    output.Pos = mul( Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
	output.Color = float4(Normal, 0.0f);
    return output;
}	
*/
//------------------------------------------------------------------------------------
// Vertex Shader - Implements Gouraud Shading using Diffuse, Ambient and Specular
//------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.PosW = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	// output.Pos is currently the position in world space

	// Convert from local space to world space 
	// W component of vector is 0 as vectors cannot be translated
	float3 normalW = mul(float4(NormalL, 0.0f), World).xyz;
		//normalize normal
		normalW = normalize(normalW);
	// Compute the reflection vector.
	output.Norm = normalW;

	output.Tex = Tex;
	output.Sat = saturate((distance(output.PosW, EyePosW) - 125.0f) / 150.0f);
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
	float3 toEye = normalize(EyePosW - input.PosW.xyz);
	float3 fogCol = (1.0f, 1.0f, 1.0f);
	// Make sure to re-normalize any vectors sent through the pipeline (from the vertex shader)
	float3 normalW = normalize(input.Norm);
	float3 lightVec = normalize(LightVecW);
	float3 r = reflect(-lightVec, normalW);

	// Compute Colour using Diffuse lighting only

	float diffuseAmount = max(dot(lightVec, normalW), 0.0f);
	//Determine how much (if any) specular light makes it into the eye.
	float specularAmount = pow(max(dot(r, toEye), 0.0f), SpecularPower);
	//Compute Colour with Ambient & Diffuse & Specular lighting
	float4 Color;
	float3 diffuse = diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb;
	float3 ambient = (AmbientMtrl * AmbientLight).rgb;
	float3 specular = specularAmount * (SpecularMtrl * SpecularLight).rgb * txSpec.Sample(samLinear, input.Tex).r;
	//Sum all the terms together and copy over the diffuse alpha.

	float4 textureColour = txDiffuse.Sample(samLinear, input.Tex);

	clip(textureColour.a - 0.40);

	Color.rgb = ((diffuse + ambient) * textureColour) + specular;
	Color.a = DiffuseMtrl.a;

	float4 FinalCol;
	FinalCol.rgb = (Color.rgb + (input.Sat * (fogCol - Color.rgb)));
	FinalCol.a = Color.a;

    return FinalCol;
}
